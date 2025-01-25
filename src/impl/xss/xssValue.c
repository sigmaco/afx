#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "./xss.h"
#include "xssValue.h"
#include "xssVm.h"

#if LUNA_DEBUG_TRACE_MEMORY
  #include "xssDebug.h"
#endif

// TODO: Tune these.
// The initial (and minimum) capacity of a non-empty list or map object.
#define MIN_CAPACITY 16

// The rate at which a collection's capacity grows when the size exceeds the
// current capacity. The new capacity will be determined by *multiplying* the
// old capacity by this. Growing geometrically is necessary to ensure that
// adding to a collection has O(1) amortized complexity.
#define GROW_FACTOR 2

// The maximum percentage of map entries that can be filled before the map is
// grown. A lower load takes more memory but reduces collisions which makes
// lookup faster.
#define MAP_LOAD_PERCENT 75

// The number of call frames initially allocated when a fiber is created. Making
// this smaller makes fibers use less memory (at first) but spends more time
// reallocating when the call stack grows.
#define INITIAL_CALL_FRAMES 4

DEFINE_BUFFER(Value, Value);
DEFINE_BUFFER(Method, Method);

static void initObj(xssVm env, Obj* obj, ObjType type, ObjClass* classObj)
{
  obj->type = type;
  obj->isDark = false;
  obj->classObj = classObj;
  obj->next = env->first;
  env->first = obj;
}

ObjClass* xssNewSingleClass(xssVm env, int numFields, ObjString* name)
{
  ObjClass* classObj = ALLOCATE(env, ObjClass);
  initObj(env, &classObj->obj, OBJ_CLASS, NULL);
  classObj->superclass = NULL;
  classObj->numFields = numFields;
  classObj->name = name;
  classObj->attributes = NULL_VAL;

  xssPushRoot(env, (Obj*)classObj);
  xssMethodBufferInit(&classObj->methods);
  xssPopRoot(env);

  return classObj;
}

void xssBindSuperclass(xssVm env, ObjClass* subclass, ObjClass* superclass)
{
  ASSERT(superclass != NULL, "Must have superclass.");

  subclass->superclass = superclass;

  // Include the superclass in the total number of fields.
  if (subclass->numFields != -1)
  {
    subclass->numFields += superclass->numFields;
  }
  else
  {
    ASSERT(superclass->numFields == 0,
           "A foreign class cannot inherit from a class with fields.");
  }

  // Inherit methods from its superclass.
  for (int i = 0; i < superclass->methods.count; i++)
  {
    xssBindMethod(env, subclass, i, superclass->methods.data[i]);
  }
}

ObjClass* xssNewClass(xssVm env, ObjClass* superclass, int numFields,
                       ObjString* name)
{
  // Create the metaclass.
  Value metaclassName = xssStringFormat(env, "@ metaclass", OBJ_VAL(name));
  xssPushRoot(env, AS_OBJ(metaclassName));

  ObjClass* metaclass = xssNewSingleClass(env, 0, AS_STRING(metaclassName));
  metaclass->obj.classObj = env->classClass;

  xssPopRoot(env);

  // Make sure the metaclass isn't collected when we allocate the class.
  xssPushRoot(env, (Obj*)metaclass);

  // Metaclasses always inherit Class and do not parallel the non-metaclass
  // hierarchy.
  xssBindSuperclass(env, metaclass, env->classClass);

  ObjClass* classObj = xssNewSingleClass(env, numFields, name);

  // Make sure the class isn't collected while the inherited methods are being
  // bound.
  xssPushRoot(env, (Obj*)classObj);

  classObj->obj.classObj = metaclass;
  xssBindSuperclass(env, classObj, superclass);

  xssPopRoot(env);
  xssPopRoot(env);

  return classObj;
}

void xssBindMethod(xssVm env, ObjClass* classObj, int symbol, Method method)
{
  // Make sure the buffer is big enough to contain the symbol's index.
  if (symbol >= classObj->methods.count)
  {
    Method noMethod;
    noMethod.type = METHOD_NONE;
    xssMethodBufferFill(env, &classObj->methods, noMethod,
                         symbol - classObj->methods.count + 1);
  }

  classObj->methods.data[symbol] = method;
}

ObjClosure* xssNewClosure(xssVm env, ObjFn* fn)
{
  ObjClosure* closure = ALLOCATE_FLEX(env, ObjClosure,
                                      ObjUpvalue*, fn->numUpvalues);
  initObj(env, &closure->obj, OBJ_CLOSURE, env->fnClass);

  closure->fn = fn;

  // Clear the upvalue array. We need to do this in case a GC is triggered
  // after the closure is created but before the upvalue array is populated.
  for (int i = 0; i < fn->numUpvalues; i++) closure->upvalues[i] = NULL;

  return closure;
}

ObjFiber* xssNewFiber(xssVm env, ObjClosure* closure)
{
  // Allocate the arrays before the fiber in case it triggers a GC.
  CallFrame* frames = ALLOCATE_ARRAY(env, CallFrame, INITIAL_CALL_FRAMES);
  
  // Add one slot for the unused implicit receiver slot that the compiler
  // assumes all functions have.
  int stackCapacity = closure == NULL
      ? 1
      : xssPowerOf2Ceil(closure->fn->maxSlots + 1);
  Value* stack = ALLOCATE_ARRAY(env, Value, stackCapacity);
  
  ObjFiber* fiber = ALLOCATE(env, ObjFiber);
  initObj(env, &fiber->obj, OBJ_FIBER, env->fiberClass);

  fiber->stack = stack;
  fiber->stackTop = fiber->stack;
  fiber->stackCapacity = stackCapacity;

  fiber->frames = frames;
  fiber->frameCapacity = INITIAL_CALL_FRAMES;
  fiber->numFrames = 0;

  fiber->openUpvalues = NULL;
  fiber->caller = NULL;
  fiber->error = NULL_VAL;
  fiber->state = FIBER_OTHER;
  
  if (closure != NULL)
  {
    // Initialize the first call frame.
    xssAppendCallFrame(env, fiber, closure, fiber->stack);

    // The first slot always holds the closure.
    fiber->stackTop[0] = OBJ_VAL(closure);
    fiber->stackTop++;
  }
  
  return fiber;
}

void xssEnsureStack(xssVm env, ObjFiber* fiber, int needed)
{
  if (fiber->stackCapacity >= needed) return;
  
  int capacity = xssPowerOf2Ceil(needed);
  
  Value* oldStack = fiber->stack;
  fiber->stack = (Value*)xssReallocate(env, fiber->stack,
                                        sizeof(Value) * fiber->stackCapacity,
                                        sizeof(Value) * capacity);
  fiber->stackCapacity = capacity;
  
  // If the reallocation moves the stack, then we need to recalculate every
  // pointer that points into the old stack to into the same relative distance
  // in the new stack. We have to be a little careful about how these are
  // calculated because pointer subtraction is only well-defined within a
  // single array, hence the slightly redundant-looking arithmetic below.
  if (fiber->stack != oldStack)
  {
    // Top of the stack.
    if (env->apiStack >= oldStack && env->apiStack <= fiber->stackTop)
    {
      env->apiStack = fiber->stack + (env->apiStack - oldStack);
    }
    
    // Stack pointer for each call frame.
    for (int i = 0; i < fiber->numFrames; i++)
    {
      CallFrame* frame = &fiber->frames[i];
      frame->stackStart = fiber->stack + (frame->stackStart - oldStack);
    }
    
    // Open upvalues.
    for (ObjUpvalue* upvalue = fiber->openUpvalues;
         upvalue != NULL;
         upvalue = upvalue->next)
    {
      upvalue->value = fiber->stack + (upvalue->value - oldStack);
    }
    
    fiber->stackTop = fiber->stack + (fiber->stackTop - oldStack);
  }
}

ObjForeign* xssNewForeign(xssVm env, ObjClass* classObj, size_t size)
{
  ObjForeign* object = ALLOCATE_FLEX(env, ObjForeign, uint8_t, size);
  initObj(env, &object->obj, OBJ_FOREIGN, classObj);

  // Zero out the bytes.
  memset(object->data, 0, size);
  return object;
}

ObjFn* xssNewFunction(xssVm env, ObjModule* module, int maxSlots)
{
  FnDebug* debug = ALLOCATE(env, FnDebug);
  debug->name = NULL;
  xssIntBufferInit(&debug->sourceLines);

  ObjFn* fn = ALLOCATE(env, ObjFn);
  initObj(env, &fn->obj, OBJ_FN, env->fnClass);
  
  xssValueBufferInit(&fn->constants);
  xssByteBufferInit(&fn->code);
  fn->module = module;
  fn->maxSlots = maxSlots;
  fn->numUpvalues = 0;
  fn->arity = 0;
  fn->debug = debug;
  
  return fn;
}

void xssFunctionBindName(xssVm env, ObjFn* fn, const char* name, int length)
{
  fn->debug->name = ALLOCATE_ARRAY(env, char, length + 1);
  memcpy(fn->debug->name, name, length);
  fn->debug->name[length] = '\0';
}

Value xssNewInstance(xssVm env, ObjClass* classObj)
{
  ObjInstance* instance = ALLOCATE_FLEX(env, ObjInstance,
                                        Value, classObj->numFields);
  initObj(env, &instance->obj, OBJ_INSTANCE, classObj);

  // Initialize fields to null.
  for (int i = 0; i < classObj->numFields; i++)
  {
    instance->fields[i] = NULL_VAL;
  }

  return OBJ_VAL(instance);
}

ObjList* xssNewList(xssVm env, uint32_t numElements)
{
  // Allocate this before the list object in case it triggers a GC which would
  // free the list.
  Value* elements = NULL;
  if (numElements > 0)
  {
    elements = ALLOCATE_ARRAY(env, Value, numElements);
  }

  ObjList* list = ALLOCATE(env, ObjList);
  initObj(env, &list->obj, OBJ_LIST, env->listClass);
  list->elements.capacity = numElements;
  list->elements.count = numElements;
  list->elements.data = elements;
  return list;
}

void xssListInsert(xssVm env, ObjList* list, Value value, uint32_t index)
{
  if (IS_OBJ(value)) xssPushRoot(env, AS_OBJ(value));

  // Add a slot at the end of the list.
  xssValueBufferWrite(env, &list->elements, NULL_VAL);

  if (IS_OBJ(value)) xssPopRoot(env);

  // Shift the existing elements down.
  for (uint32_t i = list->elements.count - 1; i > index; i--)
  {
    list->elements.data[i] = list->elements.data[i - 1];
  }

  // Store the new element.
  list->elements.data[index] = value;
}

int xssListIndexOf(xssVm env, ObjList* list, Value value)
{
  int count = list->elements.count;
  for (int i = 0; i < count; i++)
  {
    Value item = list->elements.data[i];
    if(xssValuesEqual(item, value)) {
      return i;
    }
  }
  return -1;
}

Value xssListRemoveAt(xssVm env, ObjList* list, uint32_t index)
{
  Value removed = list->elements.data[index];

  if (IS_OBJ(removed)) xssPushRoot(env, AS_OBJ(removed));

  // Shift items up.
  for (int i = index; i < list->elements.count - 1; i++)
  {
    list->elements.data[i] = list->elements.data[i + 1];
  }

  // If we have too much excess capacity, shrink it.
  if (list->elements.capacity / GROW_FACTOR >= list->elements.count)
  {
    list->elements.data = (Value*)xssReallocate(env, list->elements.data,
        sizeof(Value) * list->elements.capacity,
        sizeof(Value) * (list->elements.capacity / GROW_FACTOR));
    list->elements.capacity /= GROW_FACTOR;
  }

  if (IS_OBJ(removed)) xssPopRoot(env);

  list->elements.count--;
  return removed;
}

ObjMap* xssNewMap(xssVm env)
{
  ObjMap* map = ALLOCATE(env, ObjMap);
  initObj(env, &map->obj, OBJ_MAP, env->mapClass);
  map->capacity = 0;
  map->count = 0;
  map->entries = NULL;
  return map;
}

static inline uint32_t hashBits(uint64_t hash)
{
  // From v8's ComputeLongHash() which in turn cites:
  // Thomas Wang, Integer Hash Functions.
  // http://www.concentric.net/~Ttwang/tech/inthash.htm
  hash = ~hash + (hash << 18);  // hash = (hash << 18) - hash - 1;
  hash = hash ^ (hash >> 31);
  hash = hash * 21;  // hash = (hash + (hash << 2)) + (hash << 4);
  hash = hash ^ (hash >> 11);
  hash = hash + (hash << 6);
  hash = hash ^ (hash >> 22);
  return (uint32_t)(hash & 0x3fffffff);
}

// Generates a hash code for [num].
static inline uint32_t hashNumber(double num)
{
  // Hash the raw bits of the value.
  return hashBits(xssDoubleToBits(num));
}

// Generates a hash code for [object].
static uint32_t hashObject(Obj* object)
{
  switch (object->type)
  {
    case OBJ_CLASS:
      // Classes just use their name.
      return hashObject((Obj*)((ObjClass*)object)->name);
      
      // Allow bare (non-closure) functions so that we can use a map to find
      // existing constants in a function's constant table. This is only used
      // internally. Since user code never sees a non-closure function, they
      // cannot use them as map keys.
    case OBJ_FN:
    {
      ObjFn* fn = (ObjFn*)object;
      return hashNumber(fn->arity) ^ hashNumber(fn->code.count);
    }

    case OBJ_RANGE:
    {
      ObjRange* range = (ObjRange*)object;
      return hashNumber(range->from) ^ hashNumber(range->to);
    }

    case OBJ_STRING:
      return ((ObjString*)object)->hash;

    default:
      ASSERT(false, "Only immutable objects can be hashed.");
      return 0;
  }
}

// Generates a hash code for [value], which must be one of the built-in
// immutable types: null, bool, class, num, range, or string.
static uint32_t hashValue(Value value)
{
  // TODO: We'll probably want to randomize this at some point.

#if LUNA_NAN_TAGGING
  if (IS_OBJ(value)) return hashObject(AS_OBJ(value));

  // Hash the raw bits of the unboxed value.
  return hashBits(value);
#else
  switch (value.type)
  {
    case VAL_FALSE: return 0;
    case VAL_NULL:  return 1;
    case VAL_NUM:   return hashNumber(AS_NUM(value));
    case VAL_TRUE:  return 2;
    case VAL_OBJ:   return hashObject(AS_OBJ(value));
    default:        UNREACHABLE();
  }
  
  return 0;
#endif
}

// Looks for an entry with [key] in an array of [capacity] [entries].
//
// If found, sets [result] to point to it and returns `true`. Otherwise,
// returns `false` and points [result] to the entry where the key/value pair
// should be inserted.
static bool findEntry(MapEntry* entries, uint32_t capacity, Value key,
                      MapEntry** result)
{
  // If there is no entry array (an empty map), we definitely won't find it.
  if (capacity == 0) return false;
  
  // Figure out where to insert it in the table. Use open addressing and
  // basic linear probing.
  uint32_t startIndex = hashValue(key) % capacity;
  uint32_t index = startIndex;
  
  // If we pass a tombstone and don't end up finding the key, its entry will
  // be re-used for the insert.
  MapEntry* tombstone = NULL;
  
  // Walk the probe sequence until we've tried every slot.
  do
  {
    MapEntry* entry = &entries[index];
    
    if (IS_UNDEFINED(entry->key))
    {
      // If we found an empty slot, the key is not in the table. If we found a
      // slot that contains a deleted key, we have to keep looking.
      if (IS_FALSE(entry->value))
      {
        // We found an empty slot, so we've reached the end of the probe
        // sequence without finding the key. If we passed a tombstone, then
        // that's where we should insert the item, otherwise, put it here at
        // the end of the sequence.
        *result = tombstone != NULL ? tombstone : entry;
        return false;
      }
      else
      {
        // We found a tombstone. We need to keep looking in case the key is
        // after it, but we'll use this entry as the insertion point if the
        // key ends up not being found.
        if (tombstone == NULL) tombstone = entry;
      }
    }
    else if (xssValuesEqual(entry->key, key))
    {
      // We found the key.
      *result = entry;
      return true;
    }
    
    // Try the next slot.
    index = (index + 1) % capacity;
  }
  while (index != startIndex);
  
  // If we get here, the table is full of tombstones. Return the first one we
  // found.
  ASSERT(tombstone != NULL, "Map should have tombstones or empty entries.");
  *result = tombstone;
  return false;
}

// Inserts [key] and [value] in the array of [entries] with the given
// [capacity].
//
// Returns `true` if this is the first time [key] was added to the map.
static bool insertEntry(MapEntry* entries, uint32_t capacity,
                        Value key, Value value)
{
  ASSERT(entries != NULL, "Should ensure capacity before inserting.");
  
  MapEntry* entry;
  if (findEntry(entries, capacity, key, &entry))
  {
    // Already present, so just replace the value.
    entry->value = value;
    return false;
  }
  else
  {
    entry->key = key;
    entry->value = value;
    return true;
  }
}

// Updates [map]'s entry array to [capacity].
static void resizeMap(xssVm env, ObjMap* map, uint32_t capacity)
{
  // Create the new empty hash table.
  MapEntry* entries = ALLOCATE_ARRAY(env, MapEntry, capacity);
  for (uint32_t i = 0; i < capacity; i++)
  {
    entries[i].key = UNDEFINED_VAL;
    entries[i].value = FALSE_VAL;
  }

  // Re-add the existing entries.
  if (map->capacity > 0)
  {
    for (uint32_t i = 0; i < map->capacity; i++)
    {
      MapEntry* entry = &map->entries[i];
      
      // Don't copy empty entries or tombstones.
      if (IS_UNDEFINED(entry->key)) continue;

      insertEntry(entries, capacity, entry->key, entry->value);
    }
  }

  // Replace the array.
  DEALLOCATE(env, map->entries);
  map->entries = entries;
  map->capacity = capacity;
}

Value xssMapGet(ObjMap* map, Value key)
{
  MapEntry* entry;
  if (findEntry(map->entries, map->capacity, key, &entry)) return entry->value;

  return UNDEFINED_VAL;
}

void xssMapSet(xssVm env, ObjMap* map, Value key, Value value)
{
  // If the map is getting too full, make room first.
  if (map->count + 1 > map->capacity * MAP_LOAD_PERCENT / 100)
  {
    // Figure out the new hash table size.
    uint32_t capacity = map->capacity * GROW_FACTOR;
    if (capacity < MIN_CAPACITY) capacity = MIN_CAPACITY;

    resizeMap(env, map, capacity);
  }

  if (insertEntry(map->entries, map->capacity, key, value))
  {
    // A new key was added.
    map->count++;
  }
}

void xssMapClear(xssVm env, ObjMap* map)
{
  DEALLOCATE(env, map->entries);
  map->entries = NULL;
  map->capacity = 0;
  map->count = 0;
}

Value xssMapRemoveKey(xssVm env, ObjMap* map, Value key)
{
  MapEntry* entry;
  if (!findEntry(map->entries, map->capacity, key, &entry)) return NULL_VAL;

  // Remove the entry from the map. Set this value to true, which marks it as a
  // deleted slot. When searching for a key, we will stop on empty slots, but
  // continue past deleted slots.
  Value value = entry->value;
  entry->key = UNDEFINED_VAL;
  entry->value = TRUE_VAL;

  if (IS_OBJ(value)) xssPushRoot(env, AS_OBJ(value));

  map->count--;

  if (map->count == 0)
  {
    // Removed the last item, so free the array.
    xssMapClear(env, map);
  }
  else if (map->capacity > MIN_CAPACITY &&
           map->count < map->capacity / GROW_FACTOR * MAP_LOAD_PERCENT / 100)
  {
    uint32_t capacity = map->capacity / GROW_FACTOR;
    if (capacity < MIN_CAPACITY) capacity = MIN_CAPACITY;

    // The map is getting empty, so shrink the entry array back down.
    // TODO: Should we do this less aggressively than we grow?
    resizeMap(env, map, capacity);
  }

  if (IS_OBJ(value)) xssPopRoot(env);
  return value;
}

ObjModule* xssNewModule(xssVm env, ObjString* name)
{
  ObjModule* module = ALLOCATE(env, ObjModule);

  // Modules are never used as first-class objects, so don't need a class.
  initObj(env, (Obj*)module, OBJ_MODULE, NULL);

  xssPushRoot(env, (Obj*)module);

  xssSymbolTableInit(&module->variableNames);
  xssValueBufferInit(&module->variables);

  module->name = name;

  xssPopRoot(env);
  return module;
}

Value xssNewRange(xssVm env, double from, double to, bool isInclusive)
{
  ObjRange* range = ALLOCATE(env, ObjRange);
  initObj(env, &range->obj, OBJ_RANGE, env->rangeClass);
  range->from = from;
  range->to = to;
  range->isInclusive = isInclusive;

  return OBJ_VAL(range);
}

// Creates a new string object with a null-terminated buffer large enough to
// hold a string of [length] but does not fill in the bytes.
//
// The caller is expected to fill in the buffer and then calculate the string's
// hash.
static ObjString* allocateString(xssVm env, size_t length)
{
  ObjString* string = ALLOCATE_FLEX(env, ObjString, char, length + 1);
  initObj(env, &string->obj, OBJ_STRING, env->stringClass);
  string->length = (int)length;
  string->value[length] = '\0';

  return string;
}

// Calculates and stores the hash code for [string].
static void hashString(ObjString* string)
{
  // FNV-1a hash. See: http://www.isthe.com/chongo/tech/comp/fnv/
  uint32_t hash = 2166136261u;

  // This is O(n) on the length of the string, but we only call this when a new
  // string is created. Since the creation is also O(n) (to copy/initialize all
  // the bytes), we allow this here.
  for (uint32_t i = 0; i < string->length; i++)
  {
    hash ^= string->value[i];
    hash *= 16777619;
  }

  string->hash = hash;
}

Value xssNewString(xssVm env, const char* text)
{
  return xssNewStringLength(env, text, strlen(text));
}

Value xssNewStringLength(xssVm env, const char* text, size_t length)
{
  // Allow NULL if the string is empty since byte buffers don't allocate any
  // characters for a zero-length string.
  ASSERT(length == 0 || text != NULL, "Unexpected NULL string.");
  
  ObjString* string = allocateString(env, length);
  
  // Copy the string (if given one).
  if (length > 0 && text != NULL) memcpy(string->value, text, length);
  
  hashString(string);
  return OBJ_VAL(string);
}


Value xssNewStringFromRange(xssVm env, ObjString* source, int start,
                             uint32_t count, int step)
{
  uint8_t* from = (uint8_t*)source->value;
  int length = 0;
  for (uint32_t i = 0; i < count; i++)
  {
    length += xssUtf8DecodeNumBytes(from[start + i * step]);
  }

  ObjString* result = allocateString(env, length);
  result->value[length] = '\0';

  uint8_t* to = (uint8_t*)result->value;
  for (uint32_t i = 0; i < count; i++)
  {
    int index = start + i * step;
    int codePoint = xssUtf8Decode(from + index, source->length - index);

    if (codePoint != -1)
    {
      to += xssUtf8Encode(codePoint, to);
    }
  }

  hashString(result);
  return OBJ_VAL(result);
}

Value xssNumToString(xssVm env, double value)
{
  // Edge case: If the value is NaN or infinity, different versions of libc
  // produce different outputs (some will format it signed and some won't). To
  // get reliable output, handle it ourselves.
  if (isnan(value)) return CONST_STRING(env, "nan");
  if (isinf(value))
  {
    if (value > 0.0)
    {
      return CONST_STRING(env, "infinity");
    }
    else
    {
      return CONST_STRING(env, "-infinity");
    }
  }

  // This is large enough to hold any double converted to a string using
  // "%.14g". Example:
  //
  //     -1.12345678901234e-1022
  //
  // So we have:
  //
  // + 1 char for sign
  // + 1 char for digit
  // + 1 char for "."
  // + 14 chars for decimal digits
  // + 1 char for "e"
  // + 1 char for "-" or "+"
  // + 4 chars for exponent
  // + 1 char for "\0"
  // = 24
  char buffer[24];
  int length = sprintf(buffer, "%.14g", value);
  return xssNewStringLength(env, buffer, length);
}

Value xssStringFromCodePoint(xssVm env, int value)
{
  int length = xssUtf8EncodeNumBytes(value);
  ASSERT(length != 0, "Value out of range.");

  ObjString* string = allocateString(env, length);

  xssUtf8Encode(value, (uint8_t*)string->value);
  hashString(string);

  return OBJ_VAL(string);
}

Value xssStringFromByte(xssVm env, uint8_t value)
{
  int length = 1;
  ObjString* string = allocateString(env, length);
  string->value[0] = value;
  hashString(string);
  return OBJ_VAL(string);
}

Value xssStringFormat(xssVm env, const char* format, ...)
{
  va_list argList;

  // Calculate the length of the result string. Do this up front so we can
  // create the final string with a single allocation.
  va_start(argList, format);
  size_t totalLength = 0;
  for (const char* c = format; *c != '\0'; c++)
  {
    switch (*c)
    {
      case '$':
        totalLength += strlen(va_arg(argList, const char*));
        break;

      case '@':
        totalLength += AS_STRING(va_arg(argList, Value))->length;
        break;

      default:
        // Any other character is interpreted literally.
        totalLength++;
    }
  }
  va_end(argList);

  // Concatenate the string.
  ObjString* result = allocateString(env, totalLength);

  va_start(argList, format);
  char* start = result->value;
  for (const char* c = format; *c != '\0'; c++)
  {
    switch (*c)
    {
      case '$':
      {
        const char* string = va_arg(argList, const char*);
        size_t length = strlen(string);
        memcpy(start, string, length);
        start += length;
        break;
      }

      case '@':
      {
        ObjString* string = AS_STRING(va_arg(argList, Value));
        memcpy(start, string->value, string->length);
        start += string->length;
        break;
      }

      default:
        // Any other character is interpreted literally.
        *start++ = *c;
    }
  }
  va_end(argList);

  hashString(result);

  return OBJ_VAL(result);
}

Value xssStringCodePointAt(xssVm env, ObjString* string, uint32_t index)
{
  ASSERT(index < string->length, "Index out of bounds.");

  int codePoint = xssUtf8Decode((uint8_t*)string->value + index,
                                 string->length - index);
  if (codePoint == -1)
  {
    // If it isn't a valid UTF-8 sequence, treat it as a single raw byte.
    char bytes[2];
    bytes[0] = string->value[index];
    bytes[1] = '\0';
    return xssNewStringLength(env, bytes, 1);
  }

  return xssStringFromCodePoint(env, codePoint);
}

// Uses the Boyer-Moore-Horspool string matching algorithm.
uint32_t xssStringFind(ObjString* haystack, ObjString* needle, uint32_t start)
{
  // Edge case: An empty needle is always found.
  if (needle->length == 0) return start;

  // If the needle goes past the haystack it won't be found.
  if (start + needle->length > haystack->length) return UINT32_MAX;

  // If the startIndex is too far it also won't be found.
  if (start >= haystack->length) return UINT32_MAX;

  // Pre-calculate the shift table. For each character (8-bit value), we
  // determine how far the search window can be advanced if that character is
  // the last character in the haystack where we are searching for the needle
  // and the needle doesn't match there.
  uint32_t shift[UINT8_MAX];
  uint32_t needleEnd = needle->length - 1;

  // By default, we assume the character is not the needle at all. In that case
  // case, if a match fails on that character, we can advance one whole needle
  // width since.
  for (uint32_t index = 0; index < UINT8_MAX; index++)
  {
    shift[index] = needle->length;
  }

  // Then, for every character in the needle, determine how far it is from the
  // end. If a match fails on that character, we can advance the window such
  // that it the last character in it lines up with the last place we could
  // find it in the needle.
  for (uint32_t index = 0; index < needleEnd; index++)
  {
    char c = needle->value[index];
    shift[(uint8_t)c] = needleEnd - index;
  }

  // Slide the needle across the haystack, looking for the first match or
  // stopping if the needle goes off the end.
  char lastChar = needle->value[needleEnd];
  uint32_t range = haystack->length - needle->length;

  for (uint32_t index = start; index <= range; )
  {
    // Compare the last character in the haystack's window to the last character
    // in the needle. If it matches, see if the whole needle matches.
    char c = haystack->value[index + needleEnd];
    if (lastChar == c &&
        memcmp(haystack->value + index, needle->value, needleEnd) == 0)
    {
      // Found a match.
      return index;
    }

    // Otherwise, slide the needle forward.
    index += shift[(uint8_t)c];
  }

  // Not found.
  return UINT32_MAX;
}

ObjUpvalue* xssNewUpvalue(xssVm env, Value* value)
{
  ObjUpvalue* upvalue = ALLOCATE(env, ObjUpvalue);

  // Upvalues are never used as first-class objects, so don't need a class.
  initObj(env, &upvalue->obj, OBJ_UPVALUE, NULL);

  upvalue->value = value;
  upvalue->closed = NULL_VAL;
  upvalue->next = NULL;
  return upvalue;
}

void xssGrayObj(xssVm env, Obj* obj)
{
  if (obj == NULL) return;

  // Stop if the object is already darkened so we don't get stuck in a cycle.
  if (obj->isDark) return;

  // It's been reached.
  obj->isDark = true;

  // Add it to the gray list so it can be recursively explored for
  // more marks later.
  if (env->grayCount >= env->grayCapacity)
  {
    env->grayCapacity = env->grayCount * 2;
    env->gray = (Obj**)env->config.reallocateFn(env->gray,
                                              env->grayCapacity * sizeof(Obj*),
                                              env->config.userData);
  }

  env->gray[env->grayCount++] = obj;
}

void xssGrayValue(xssVm env, Value value)
{
  if (!IS_OBJ(value)) return;
  xssGrayObj(env, AS_OBJ(value));
}

void xssGrayBuffer(xssVm env, ValueBuffer* buffer)
{
  for (int i = 0; i < buffer->count; i++)
  {
    xssGrayValue(env, buffer->data[i]);
  }
}

static void blackenClass(xssVm env, ObjClass* classObj)
{
  // The metaclass.
  xssGrayObj(env, (Obj*)classObj->obj.classObj);

  // The superclass.
  xssGrayObj(env, (Obj*)classObj->superclass);

  // Method function objects.
  for (int i = 0; i < classObj->methods.count; i++)
  {
    if (classObj->methods.data[i].type == METHOD_BLOCK)
    {
      xssGrayObj(env, (Obj*)classObj->methods.data[i].as.closure);
    }
  }

  xssGrayObj(env, (Obj*)classObj->name);

  if(!IS_NULL(classObj->attributes)) xssGrayObj(env, AS_OBJ(classObj->attributes));

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjClass);
  env->bytesAllocated += classObj->methods.capacity * sizeof(Method);
}

static void blackenClosure(xssVm env, ObjClosure* closure)
{
  // Mark the function.
  xssGrayObj(env, (Obj*)closure->fn);

  // Mark the upvalues.
  for (int i = 0; i < closure->fn->numUpvalues; i++)
  {
    xssGrayObj(env, (Obj*)closure->upvalues[i]);
  }

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjClosure);
  env->bytesAllocated += sizeof(ObjUpvalue*) * closure->fn->numUpvalues;
}

static void blackenFiber(xssVm env, ObjFiber* fiber)
{
  // Stack functions.
  for (int i = 0; i < fiber->numFrames; i++)
  {
    xssGrayObj(env, (Obj*)fiber->frames[i].closure);
  }

  // Stack variables.
  for (Value* slot = fiber->stack; slot < fiber->stackTop; slot++)
  {
    xssGrayValue(env, *slot);
  }

  // Open upvalues.
  ObjUpvalue* upvalue = fiber->openUpvalues;
  while (upvalue != NULL)
  {
    xssGrayObj(env, (Obj*)upvalue);
    upvalue = upvalue->next;
  }

  // The caller.
  xssGrayObj(env, (Obj*)fiber->caller);
  xssGrayValue(env, fiber->error);

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjFiber);
  env->bytesAllocated += fiber->frameCapacity * sizeof(CallFrame);
  env->bytesAllocated += fiber->stackCapacity * sizeof(Value);
}

static void blackenFn(xssVm env, ObjFn* fn)
{
  // Mark the constants.
  xssGrayBuffer(env, &fn->constants);

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjFn);
  env->bytesAllocated += sizeof(uint8_t) * fn->code.capacity;
  env->bytesAllocated += sizeof(Value) * fn->constants.capacity;
  
  // The debug line number buffer.
  env->bytesAllocated += sizeof(int) * fn->code.capacity;
  // TODO: What about the function name?
}

static void blackenForeign(xssVm env, ObjForeign* foreign)
{
  // TODO: Keep track of how much memory the foreign object uses. We can store
  // this in each foreign object, but it will balloon the size. We may not want
  // that much overhead. One option would be to let the foreign class register
  // a C function that returns a size for the object. That way the VM doesn't
  // always have to explicitly store it.
}

static void blackenInstance(xssVm env, ObjInstance* instance)
{
  xssGrayObj(env, (Obj*)instance->obj.classObj);

  // Mark the fields.
  for (int i = 0; i < instance->obj.classObj->numFields; i++)
  {
    xssGrayValue(env, instance->fields[i]);
  }

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjInstance);
  env->bytesAllocated += sizeof(Value) * instance->obj.classObj->numFields;
}

static void blackenList(xssVm env, ObjList* list)
{
  // Mark the elements.
  xssGrayBuffer(env, &list->elements);

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjList);
  env->bytesAllocated += sizeof(Value) * list->elements.capacity;
}

static void blackenMap(xssVm env, ObjMap* map)
{
  // Mark the entries.
  for (uint32_t i = 0; i < map->capacity; i++)
  {
    MapEntry* entry = &map->entries[i];
    if (IS_UNDEFINED(entry->key)) continue;

    xssGrayValue(env, entry->key);
    xssGrayValue(env, entry->value);
  }

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjMap);
  env->bytesAllocated += sizeof(MapEntry) * map->capacity;
}

static void blackenModule(xssVm env, ObjModule* module)
{
  // Top-level variables.
  for (int i = 0; i < module->variables.count; i++)
  {
    xssGrayValue(env, module->variables.data[i]);
  }

  xssBlackenSymbolTable(env, &module->variableNames);

  xssGrayObj(env, (Obj*)module->name);

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjModule);
}

static void blackenRange(xssVm env, ObjRange* range)
{
  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjRange);
}

static void blackenString(xssVm env, ObjString* string)
{
  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjString) + string->length + 1;
}

static void blackenUpvalue(xssVm env, ObjUpvalue* upvalue)
{
  // Mark the closed-over object (in case it is closed).
  xssGrayValue(env, upvalue->closed);

  // Keep track of how much memory is still in use.
  env->bytesAllocated += sizeof(ObjUpvalue);
}

static void blackenObject(xssVm env, Obj* obj)
{
#if LUNA_DEBUG_TRACE_MEMORY
  printf("mark ");
  xssDumpValue(OBJ_VAL(obj));
  printf(" @ %p\n", obj);
#endif

  // Traverse the object's fields.
  switch (obj->type)
  {
    case OBJ_CLASS:    blackenClass(   env, (ObjClass*)   obj); break;
    case OBJ_CLOSURE:  blackenClosure( env, (ObjClosure*) obj); break;
    case OBJ_FIBER:    blackenFiber(   env, (ObjFiber*)   obj); break;
    case OBJ_FN:       blackenFn(      env, (ObjFn*)      obj); break;
    case OBJ_FOREIGN:  blackenForeign( env, (ObjForeign*) obj); break;
    case OBJ_INSTANCE: blackenInstance(env, (ObjInstance*)obj); break;
    case OBJ_LIST:     blackenList(    env, (ObjList*)    obj); break;
    case OBJ_MAP:      blackenMap(     env, (ObjMap*)     obj); break;
    case OBJ_MODULE:   blackenModule(  env, (ObjModule*)  obj); break;
    case OBJ_RANGE:    blackenRange(   env, (ObjRange*)   obj); break;
    case OBJ_STRING:   blackenString(  env, (ObjString*)  obj); break;
    case OBJ_UPVALUE:  blackenUpvalue( env, (ObjUpvalue*) obj); break;
  }
}

void xssBlackenObjects(xssVm env)
{
  while (env->grayCount > 0)
  {
    // Pop an item from the gray stack.
    Obj* obj = env->gray[--env->grayCount];
    blackenObject(env, obj);
  }
}

void xssFreeObj(xssVm env, Obj* obj)
{
#if LUNA_DEBUG_TRACE_MEMORY
  printf("free ");
  xssDumpValue(OBJ_VAL(obj));
  printf(" @ %p\n", obj);
#endif

  switch (obj->type)
  {
    case OBJ_CLASS:
      xssMethodBufferClear(env, &((ObjClass*)obj)->methods);
      break;

    case OBJ_FIBER:
    {
      ObjFiber* fiber = (ObjFiber*)obj;
      DEALLOCATE(env, fiber->frames);
      DEALLOCATE(env, fiber->stack);
      break;
    }
      
    case OBJ_FN:
    {
      ObjFn* fn = (ObjFn*)obj;
      xssValueBufferClear(env, &fn->constants);
      xssByteBufferClear(env, &fn->code);
      xssIntBufferClear(env, &fn->debug->sourceLines);
      DEALLOCATE(env, fn->debug->name);
      DEALLOCATE(env, fn->debug);
      break;
    }

    case OBJ_FOREIGN:
      xssFinalizeForeign(env, (ObjForeign*)obj);
      break;

    case OBJ_LIST:
      xssValueBufferClear(env, &((ObjList*)obj)->elements);
      break;

    case OBJ_MAP:
      DEALLOCATE(env, ((ObjMap*)obj)->entries);
      break;

    case OBJ_MODULE:
      xssSymbolTableClear(env, &((ObjModule*)obj)->variableNames);
      xssValueBufferClear(env, &((ObjModule*)obj)->variables);
      break;

    case OBJ_CLOSURE:
    case OBJ_INSTANCE:
    case OBJ_RANGE:
    case OBJ_STRING:
    case OBJ_UPVALUE:
      break;
  }

  DEALLOCATE(env, obj);
}

ObjClass* xssGetClass(xssVm env, Value value)
{
  return xssGetClassInline(env, value);
}

bool xssValuesEqual(Value a, Value b)
{
  if (xssValuesSame(a, b)) return true;

  // If we get here, it's only possible for two heap-allocated immutable objects
  // to be equal.
  if (!IS_OBJ(a) || !IS_OBJ(b)) return false;

  Obj* aObj = AS_OBJ(a);
  Obj* bObj = AS_OBJ(b);

  // Must be the same type.
  if (aObj->type != bObj->type) return false;

  switch (aObj->type)
  {
    case OBJ_RANGE:
    {
      ObjRange* aRange = (ObjRange*)aObj;
      ObjRange* bRange = (ObjRange*)bObj;
      return aRange->from == bRange->from &&
             aRange->to == bRange->to &&
             aRange->isInclusive == bRange->isInclusive;
    }

    case OBJ_STRING:
    {
      ObjString* aString = (ObjString*)aObj;
      ObjString* bString = (ObjString*)bObj;
      return aString->hash == bString->hash &&
      xssStringEqualsCString(aString, bString->value, bString->length);
    }

    default:
      // All other types are only equal if they are same, which they aren't if
      // we get here.
      return false;
  }
}
