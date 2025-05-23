#include <stdarg.h>
#include <string.h>

#include "./xss.h"
#include "xssCommon.h"
#include "xssCompiler.h"
#include "xssCore.h"
#include "xssDebug.h"
#include "xssPrimitive.h"
#include "xssVm.h"

#if LUNA_OPT_META
  #include "xssOptMeta.h"
#endif
#if LUNA_OPT_RANDOM
  #include "xssOptRandom.h"
#endif

#if LUNA_DEBUG_TRACE_MEMORY || LUNA_DEBUG_TRACE_GC
  #include <time.h>
  #include <stdio.h>
#endif
//#include "qwadro/inc/ux/afxUxDefs.h"

// The behavior of realloc() when the size is 0 is implementation defined. It
// may return a non-NULL pointer which must not be dereferenced but nevertheless
// should be freed. To prevent that, we avoid calling realloc() with a zero
// size.
static void* defaultReallocate(void* ptr, size_t newSize, void* _)
{
  if (newSize == 0)
  {
    free(ptr);
    return NULL;
  }

  return realloc(ptr, newSize);
}

int xssGetVersionNumber() 
{ 
  return LUNA_VERSION_NUMBER;
}

void xssInitConfiguration(LunaConfiguration* config)
{
  config->reallocateFn = defaultReallocate;
  config->resolveModuleFn = NULL;
  config->loadModuleFn = NULL;
  config->bindForeignMethodFn = NULL;
  config->bindForeignClassFn = NULL;
  config->writeFn = NULL;
  config->errorFn = NULL;
  config->initialHeapSize = 1024 * 1024 * 10;
  config->minHeapSize = 1024 * 1024;
  config->heapGrowthPercent = 50;
  config->userData = NULL;
}

#if 0
xssVm xssNewVM(LunaConfiguration* config)
{
  LunaReallocateFn reallocate = defaultReallocate;
  void* userData = NULL;
  if (config != NULL) {
    userData = config->userData;
    reallocate = config->reallocateFn ? config->reallocateFn : defaultReallocate;
  }
  
  xssVm vm = (xssVm)reallocate(NULL, sizeof(*vm), userData);
  memset(vm, 0, sizeof(*vm));

  // Copy the configuration if given one.
  if (config != NULL)
  {
    memcpy(&vm->config, config, sizeof(LunaConfiguration));

    // We choose to set this after copying, 
    // rather than modifying the user config pointer
    vm->config.reallocateFn = reallocate;
  }
  else
  {
    xssInitConfiguration(&vm->config);
  }

  // TODO: Should we allocate and free this during a GC?
  vm->grayCount = 0;
  // TODO: Tune this.
  vm->grayCapacity = 4;
  vm->gray = (Obj**)reallocate(NULL, vm->grayCapacity * sizeof(Obj*), userData);
  vm->nextGC = vm->config.initialHeapSize;

  xssSymbolTableInit(&vm->methodNames);

  vm->modules = xssNewMap(vm);
  xssInitializeCore(vm);
  return vm;
}
#endif

#if 0
void xssFreeVM(xssVm vm)
{
  ASSERT(vm->methodNames.count > 0, "VM appears to have already been freed.");
  
  // Free all of the GC objects.
  Obj* obj = vm->first;
  while (obj != NULL)
  {
    Obj* next = obj->next;
    xssFreeObj(vm, obj);
    obj = next;
  }

  // Free up the GC gray set.
  vm->gray = (Obj**)vm->config.reallocateFn(vm->gray, 0, vm->config.userData);

  // Tell the user if they didn't free any handles. We don't want to just free
  // them here because the host app may still have pointers to them that they
  // may try to use. Better to tell them about the bug early.
  ASSERT(vm->handles == NULL, "All handles have not been released.");

  xssSymbolTableClear(vm, &vm->methodNames);

  DEALLOCATE(vm, vm);
}
#endif

_AFX xssVm xssCreateVm(LunaConfiguration const* config)
{
    afxError err = AFX_ERR_NONE;

    LunaReallocateFn reallocate = defaultReallocate;
    void* userData = NULL;
    
#if !0
    if (config != NULL)
    {
        userData = config->userData;
        reallocate = config->reallocateFn ? config->reallocateFn : defaultReallocate;
    }

    xssVm vm = (xssVm)reallocate(NULL, sizeof(*vm), userData);
    memset(vm, 0, sizeof(*vm));

    // Copy the configuration if given one.
    if (config != NULL)
    {
        memcpy(&vm->config, config, sizeof(LunaConfiguration));

        // We choose to set this after copying, 
        // rather than modifying the user config pointer
        vm->config.reallocateFn = reallocate;
    }
    else
#endif
    {
        xssInitConfiguration(&vm->config);
    }

    // TODO: Should we allocate and free this during a GC?
    vm->grayCount = 0;
    // TODO: Tune this.
    vm->grayCapacity = 4;
    vm->gray = (Obj**)reallocate(NULL, vm->grayCapacity * sizeof(Obj*), userData);
    vm->nextGC = vm->config.initialHeapSize;

    xssSymbolTableInit(&vm->methodNames);

    vm->modules = xssNewMap(vm);
    xssInitializeCore(vm);

    return vm;
}

_AFX afxError xssDestroyVm(xssVm vm)
{
    afxError err = AFX_ERR_NONE;

    ASSERT(vm->methodNames.count > 0, "VM appears to have already been freed.");

    // Free all of the GC objects.
    Obj* obj = vm->first;
    while (obj != NULL)
    {
        Obj* next = obj->next;
        xssFreeObj(vm, obj);
        obj = next;
    }

    // Free up the GC gray set.
    vm->gray = (Obj**)vm->config.reallocateFn(vm->gray, 0, vm->config.userData);

    // Tell the user if they didn't free any handles. We don't want to just free
    // them here because the host app may still have pointers to them that they
    // may try to use. Better to tell them about the bug early.
    ASSERT(vm->handles == NULL, "All handles have not been released.");

    xssSymbolTableClear(vm, &vm->methodNames);

    //DEALLOCATE(vm, vm);
    return err;
}

void xssCollectGarbage(xssVm vm)
{
#if LUNA_DEBUG_TRACE_MEMORY || LUNA_DEBUG_TRACE_GC
  printf("-- gc --\n");

  size_t before = vm->bytesAllocated;
  double startTime = (double)clock() / CLOCKS_PER_SEC;
#endif

  // Mark all reachable objects.

  // Reset this. As we mark objects, their size will be counted again so that
  // we can track how much memory is in use without needing to know the size
  // of each *freed* object.
  //
  // This is important because when freeing an unmarked object, we don't always
  // know how much memory it is using. For example, when freeing an instance,
  // we need to know its class to know how big it is, but its class may have
  // already been freed.
  vm->bytesAllocated = 0;

  xssGrayObj(vm, (Obj*)vm->modules);

  // Temporary roots.
  for (int i = 0; i < vm->numTempRoots; i++)
  {
    xssGrayObj(vm, vm->tempRoots[i]);
  }

  // The current fiber.
  xssGrayObj(vm, (Obj*)vm->fiber);

  // The handles.
  for (LunaHandle* handle = vm->handles;
       handle != NULL;
       handle = handle->next)
  {
    xssGrayValue(vm, handle->value);
  }

  // Any object the compiler is using (if there is one).
  if (vm->compiler != NULL) xssMarkCompiler(vm, vm->compiler);

  // Method names.
  xssBlackenSymbolTable(vm, &vm->methodNames);

  // Now that we have grayed the roots, do a depth-first search over all of the
  // reachable objects.
  xssBlackenObjects(vm);

  // Collect the white objects.
  Obj** obj = &vm->first;
  while (*obj != NULL)
  {
    if (!((*obj)->isDark))
    {
      // This object wasn't reached, so remove it from the list and free it.
      Obj* unreached = *obj;
      *obj = unreached->next;
      xssFreeObj(vm, unreached);
    }
    else
    {
      // This object was reached, so unmark it (for the next GC) and move on to
      // the next.
      (*obj)->isDark = false;
      obj = &(*obj)->next;
    }
  }

  // Calculate the next gc point, this is the current allocation plus
  // a configured percentage of the current allocation.
  vm->nextGC = vm->bytesAllocated + ((vm->bytesAllocated * vm->config.heapGrowthPercent) / 100);
  if (vm->nextGC < vm->config.minHeapSize) vm->nextGC = vm->config.minHeapSize;

#if LUNA_DEBUG_TRACE_MEMORY || LUNA_DEBUG_TRACE_GC
  double elapsed = ((double)clock() / CLOCKS_PER_SEC) - startTime;
  // Explicit cast because size_t has different sizes on 32-bit and 64-bit and
  // we need a consistent type for the format string.
  printf("GC %lu before, %lu after (%lu collected), next at %lu. Took %.3fms.\n",
         (unsigned long)before,
         (unsigned long)vm->bytesAllocated,
         (unsigned long)(before - vm->bytesAllocated),
         (unsigned long)vm->nextGC,
         elapsed*1000.0);
#endif
}

void* xssReallocate(xssVm vm, void* memory, size_t oldSize, size_t newSize)
{
#if LUNA_DEBUG_TRACE_MEMORY
  // Explicit cast because size_t has different sizes on 32-bit and 64-bit and
  // we need a consistent type for the format string.
  printf("reallocate %p %lu -> %lu\n",
         memory, (unsigned long)oldSize, (unsigned long)newSize);
#endif

  // If new bytes are being allocated, add them to the total count. If objects
  // are being completely deallocated, we don't track that (since we don't
  // track the original size). Instead, that will be handled while marking
  // during the next GC.
  vm->bytesAllocated += newSize - oldSize;

#if LUNA_DEBUG_GC_STRESS
  // Since collecting calls this function to free things, make sure we don't
  // recurse.
  if (newSize > 0) xssCollectGarbage(vm);
#else
  if (newSize > 0 && vm->bytesAllocated > vm->nextGC) xssCollectGarbage(vm);
#endif

  return vm->config.reallocateFn(memory, newSize, vm->config.userData);
}

// Captures the local variable [local] into an [Upvalue]. If that local is
// already in an upvalue, the existing one will be used. (This is important to
// ensure that multiple closures closing over the same variable actually see
// the same variable.) Otherwise, it will create a new open upvalue and add it
// the fiber's list of upvalues.
static ObjUpvalue* captureUpvalue(xssVm vm, ObjFiber* fiber, Value* local)
{
  // If there are no open upvalues at all, we must need a new one.
  if (fiber->openUpvalues == NULL)
  {
    fiber->openUpvalues = xssNewUpvalue(vm, local);
    return fiber->openUpvalues;
  }

  ObjUpvalue* prevUpvalue = NULL;
  ObjUpvalue* upvalue = fiber->openUpvalues;

  // Walk towards the bottom of the stack until we find a previously existing
  // upvalue or pass where it should be.
  while (upvalue != NULL && upvalue->value > local)
  {
    prevUpvalue = upvalue;
    upvalue = upvalue->next;
  }

  // Found an existing upvalue for this local.
  if (upvalue != NULL && upvalue->value == local) return upvalue;

  // We've walked past this local on the stack, so there must not be an
  // upvalue for it already. Make a new one and link it in in the right
  // place to keep the list sorted.
  ObjUpvalue* createdUpvalue = xssNewUpvalue(vm, local);
  if (prevUpvalue == NULL)
  {
    // The new one is the first one in the list.
    fiber->openUpvalues = createdUpvalue;
  }
  else
  {
    prevUpvalue->next = createdUpvalue;
  }

  createdUpvalue->next = upvalue;
  return createdUpvalue;
}

// Closes any open upvalues that have been created for stack slots at [last]
// and above.
static void closeUpvalues(ObjFiber* fiber, Value* last)
{
  while (fiber->openUpvalues != NULL &&
         fiber->openUpvalues->value >= last)
  {
    ObjUpvalue* upvalue = fiber->openUpvalues;

    // Move the value into the upvalue itself and point the upvalue to it.
    upvalue->closed = *upvalue->value;
    upvalue->value = &upvalue->closed;

    // Remove it from the open upvalue list.
    fiber->openUpvalues = upvalue->next;
  }
}

// Looks up a foreign method in [moduleName] on [className] with [signature].
//
// This will try the host's foreign method binder first. If that fails, it
// falls back to handling the built-in modules.
static LunaForeignMethodFn findForeignMethod(xssVm vm,
                                             const char* moduleName,
                                             const char* className,
                                             bool isStatic,
                                             const char* signature)
{
  LunaForeignMethodFn method = NULL;
  
#if 0
  if (vm->config.bindForeignMethodFn != NULL)
  {
    method = vm->config.bindForeignMethodFn(vm, moduleName, className, isStatic,
                                            signature);
  }
#else

  afxUri uri;

  if (0 == AfxCompareString(&AFX_STRING("core"), 0, moduleName, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2coree", 0);
  else if (0 == AfxCompareString(&AFX_STRING("draw"), 0, moduleName, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2draw", 0);
  else if (0 == AfxCompareString(&AFX_STRING("sound"), 0, moduleName, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2sound", 0);
  else if (0 == AfxCompareString(&AFX_STRING("sim"), 0, moduleName, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2sim", 0);
  else if (0 == AfxCompareString(&AFX_STRING("ux"), 0, moduleName, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0);
  else
      AfxMakeUri(&uri, 0, moduleName, 0);

  afxModule exe;
  AfxFindModule(&uri, &exe);

  xssVm* xthr = xssGetUserData(vm);
  void*(*ffiMeth)(xssVm* vm, afxString const* class, afxString const* signature, afxBool firm) = AfxFindModuleSymbol(exe, "GetFfiMethod");

  afxString s, s2;
  AfxMakeString(&s, 0, className, 0);
  AfxMakeString(&s2, 0, signature, 0);
  method = ffiMeth(xthr, &s, &s2, isStatic);
  afxError err;
  AFX_ASSERT(method);
#endif
  
  // If the host didn't provide it, see if it's an optional one.
  if (method == NULL)
  {
#if LUNA_OPT_META
    if (strcmp(moduleName, "meta") == 0)
    {
      method = xssMetaBindForeignMethod(vm, className, isStatic, signature);
    }
#endif
#if LUNA_OPT_RANDOM
    if (strcmp(moduleName, "random") == 0)
    {
      method = xssRandomBindForeignMethod(vm, className, isStatic, signature);
    }
#endif
  }

  return method;
}

// Defines [methodValue] as a method on [classObj].
//
// Handles both foreign methods where [methodValue] is a string containing the
// method's signature and Luna methods where [methodValue] is a function.
//
// Aborts the current fiber if the method is a foreign method that could not be
// found.
static void bindMethod(xssVm vm, int methodType, int symbol,
                       ObjModule* module, ObjClass* classObj, Value methodValue)
{
  const char* className = classObj->name->value;
  if (methodType == CODE_METHOD_STATIC) classObj = classObj->obj.classObj;

  Method method;
  if (IS_STRING(methodValue))
  {
    const char* name = AS_CSTRING(methodValue);
    method.type = METHOD_FOREIGN;
    method.as.foreign = findForeignMethod(vm, module->name->value,
                                          className,
                                          methodType == CODE_METHOD_STATIC,
                                          name);

    if (method.as.foreign == NULL)
    {
      vm->fiber->error = xssStringFormat(vm,
          "Could not find foreign method '@' for class $ in module '$'.",
          methodValue, classObj->name->value, module->name->value);
      return;
    }
  }
  else
  {
    method.as.closure = AS_CLOSURE(methodValue);
    method.type = METHOD_BLOCK;

    // Patch up the bytecode now that we know the superclass.
    xssBindMethodCode(classObj, method.as.closure->fn);
  }

  xssBindMethod(vm, classObj, symbol, method);
}

static void callForeign(xssVm vm, ObjFiber* fiber,
                        LunaForeignMethodFn foreign, int numArgs)
{
  ASSERT(vm->apiStack == NULL, "Cannot already be in foreign call.");
  vm->apiStack = fiber->stackTop - numArgs;

  foreign(vm);

  // Discard the stack slots for the arguments and temporaries but leave one
  // for the result.
  fiber->stackTop = vm->apiStack + 1;

  vm->apiStack = NULL;
}

// Handles the current fiber having aborted because of an error.
//
// Walks the call chain of fibers, aborting each one until it hits a fiber that
// handles the error. If none do, tells the VM to stop.
static void runtimeError(xssVm vm)
{
  ASSERT(xssHasError(vm->fiber), "Should only call this after an error.");

  ObjFiber* current = vm->fiber;
  Value error = current->error;
  
  while (current != NULL)
  {
    // Every fiber along the call chain gets aborted with the same error.
    current->error = error;

    // If the caller ran this fiber using "try", give it the error and stop.
    if (current->state == FIBER_TRY)
    {
      // Make the caller's try method return the error message.
      current->caller->stackTop[-1] = vm->fiber->error;
      vm->fiber = current->caller;
      return;
    }
    
    // Otherwise, unhook the caller since we will never resume and return to it.
    ObjFiber* caller = current->caller;
    current->caller = NULL;
    current = caller;
  }

  // If we got here, nothing caught the error, so show the stack trace.
  xssDebugPrintStackTrace(vm);
  vm->fiber = NULL;
  vm->apiStack = NULL;
}

// Aborts the current fiber with an appropriate method not found error for a
// method with [symbol] on [classObj].
static void methodNotFound(xssVm vm, ObjClass* classObj, int symbol)
{
  vm->fiber->error = xssStringFormat(vm, "@ does not implement '$'.",
      OBJ_VAL(classObj->name), vm->methodNames.data[symbol]->value);
}

// Looks up the previously loaded module with [name].
//
// Returns `NULL` if no module with that name has been loaded.
static ObjModule* getModule(xssVm vm, Value name)
{
  Value moduleValue = xssMapGet(vm->modules, name);
  return !IS_UNDEFINED(moduleValue) ? AS_MODULE(moduleValue) : NULL;
}

static ObjClosure* compileInModule(xssVm vm, Value name, const char* source,
                                   bool isExpression, bool printErrors)
{
  // See if the module has already been loaded.
  ObjModule* module = getModule(vm, name);
  if (module == NULL)
  {
    module = xssNewModule(vm, AS_STRING(name));

    // It's possible for the xssMapSet below to resize the modules map,
    // and trigger a GC while doing so. When this happens it will collect
    // the module we've just created. Once in the map it is safe.
    xssPushRoot(vm, (Obj*)module);

    // Store it in the VM's module registry so we don't load the same module
    // multiple times.
    xssMapSet(vm, vm->modules, name, OBJ_VAL(module));

    xssPopRoot(vm);

    // Implicitly import the core module.
    ObjModule* coreModule = getModule(vm, NULL_VAL);
    for (int i = 0; i < coreModule->variables.count; i++)
    {
      xssDefineVariable(vm, module,
                         coreModule->variableNames.data[i]->value,
                         coreModule->variableNames.data[i]->length,
                         coreModule->variables.data[i], NULL);
    }
  }

  ObjFn* fn = xssCompile(vm, module, source, isExpression, printErrors);
  if (fn == NULL)
  {
    // TODO: Should we still store the module even if it didn't compile?
    return NULL;
  }

  // Functions are always wrapped in closures.
  xssPushRoot(vm, (Obj*)fn);
  ObjClosure* closure = xssNewClosure(vm, fn);
  xssPopRoot(vm); // fn.

  return closure;
}

// Verifies that [superclassValue] is a valid object to inherit from. That
// means it must be a class and cannot be the class of any built-in type.
//
// Also validates that it doesn't result in a class with too many fields and
// the other limitations foreign classes have.
//
// If successful, returns `null`. Otherwise, returns a string for the runtime
// error message.
static Value validateSuperclass(xssVm vm, Value name, Value superclassValue,
                                int numFields)
{
  // Make sure the superclass is a class.
  if (!IS_CLASS(superclassValue))
  {
    return xssStringFormat(vm,
        "Class '@' cannot inherit from a non-class object.",
        name);
  }

  // Make sure it doesn't inherit from a sealed built-in type. Primitive methods
  // on these classes assume the instance is one of the other Obj___ types and
  // will fail horribly if it's actually an ObjInstance.
  ObjClass* superclass = AS_CLASS(superclassValue);
  if (superclass == vm->classClass ||
      superclass == vm->fiberClass ||
      superclass == vm->fnClass || // Includes OBJ_CLOSURE.
      superclass == vm->listClass ||
      superclass == vm->mapClass ||
      superclass == vm->rangeClass ||
      superclass == vm->stringClass ||
      superclass == vm->boolClass ||
      superclass == vm->nullClass ||
      superclass == vm->numClass)
  {
    return xssStringFormat(vm,
        "Class '@' cannot inherit from built-in class '@'.",
        name, OBJ_VAL(superclass->name));
  }

  if (superclass->numFields == -1)
  {
    return xssStringFormat(vm,
        "Class '@' cannot inherit from foreign class '@'.",
        name, OBJ_VAL(superclass->name));
  }

  if (numFields == -1 && superclass->numFields > 0)
  {
    return xssStringFormat(vm,
        "Foreign class '@' may not inherit from a class with fields.",
        name);
  }

  if (superclass->numFields + numFields > MAX_FIELDS)
  {
    return xssStringFormat(vm,
        "Class '@' may not have more than 255 fields, including inherited "
        "ones.", name);
  }

  return NULL_VAL;
}

static void bindForeignClass(xssVm vm, ObjClass* classObj, ObjModule* module)
{
  LunaForeignClassMethods methods;
  methods.allocate = NULL;
  methods.finalize = NULL;
  
  // Check the optional built-in module first so the host can override it.

  
#if 0
  if (vm->config.bindForeignClassFn != NULL)
  {

    methods = vm->config.bindForeignClassFn(vm, module->name->value,
                                            classObj->name->value);
  }
#else
  afxUri uri;

  if (0 == AfxCompareString(&AFX_STRING("core"), 0, module->name->value, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2coree", 0);
  else if (0 == AfxCompareString(&AFX_STRING("draw"), 0, module->name->value, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2draw", 0);
  else if (0 == AfxCompareString(&AFX_STRING("sound"), 0, module->name->value, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2sound", 0);
  else if (0 == AfxCompareString(&AFX_STRING("sim"), 0, module->name->value, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0); //AfxMakeUri(&uri, "e2sim", 0);
  else if (0 == AfxCompareString(&AFX_STRING("ux"), 0, module->name->value, 0, FALSE))
      AfxMakeUri(&uri, 0, "e2ux", 0);
  else
      AfxMakeUri(&uri, 0, module->name->value, 0);

  afxModule exe;
  AfxFindModule(&uri, &exe);

  xssVm* xthr = xssGetUserData(vm);
  afxBool(*ffiCls)(xssVm* vm, afxString const* class, LunaForeignClassMethods* methods) = AfxFindModuleSymbol(exe, "GetFfiClass");

  afxString s;
  AfxMakeString(&s, 0, classObj->name->value, 0);
  ffiCls(xthr, &s, &methods);
#endif

  // If the host didn't provide it, see if it's a built in optional module.
  if (methods.allocate == NULL && methods.finalize == NULL)
  {
#if LUNA_OPT_RANDOM
    if (strcmp(module->name->value, "random") == 0)
    {
      methods = xssRandomBindForeignClass(vm, module->name->value,
                                           classObj->name->value);
    }
#endif
  }
  
  Method method;
  method.type = METHOD_FOREIGN;

  // Add the symbol even if there is no allocator so we can ensure that the
  // symbol itself is always in the symbol table.
  int symbol = xssSymbolTableEnsure(vm, &vm->methodNames, "<allocate>", 10);
  if (methods.allocate != NULL)
  {
    method.as.foreign = methods.allocate;
    xssBindMethod(vm, classObj, symbol, method);
  }
  
  // Add the symbol even if there is no finalizer so we can ensure that the
  // symbol itself is always in the symbol table.
  symbol = xssSymbolTableEnsure(vm, &vm->methodNames, "<finalize>", 10);
  if (methods.finalize != NULL)
  {
    method.as.foreign = (LunaForeignMethodFn)methods.finalize;
    xssBindMethod(vm, classObj, symbol, method);
  }
}

// Completes the process for creating a new class.
//
// The class attributes instance and the class itself should be on the 
// top of the fiber's stack. 
//
// This process handles moving the attribute data for a class from
// compile time to runtime, since it now has all the attributes associated
// with a class, including for methods.
static void endClass(xssVm vm) 
{
  // Pull the attributes and class off the stack
  Value attributes = vm->fiber->stackTop[-2];
  Value classValue = vm->fiber->stackTop[-1];

  // Remove the stack items
  vm->fiber->stackTop -= 2;

  ObjClass* classObj = AS_CLASS(classValue);
    classObj->attributes = attributes;
}

// Creates a new class.
//
// If [numFields] is -1, the class is a foreign class. The name and superclass
// should be on top of the fiber's stack. After calling this, the top of the
// stack will contain the new class.
//
// Aborts the current fiber if an error occurs.
static void createClass(xssVm vm, int numFields, ObjModule* module)
{
  // Pull the name and superclass off the stack.
  Value name = vm->fiber->stackTop[-2];
  Value superclass = vm->fiber->stackTop[-1];

  // We have two values on the stack and we are going to leave one, so discard
  // the other slot.
  vm->fiber->stackTop--;

  vm->fiber->error = validateSuperclass(vm, name, superclass, numFields);
  if (xssHasError(vm->fiber)) return;

  ObjClass* classObj = xssNewClass(vm, AS_CLASS(superclass), numFields,
                                    AS_STRING(name));
  vm->fiber->stackTop[-1] = OBJ_VAL(classObj);

  if (numFields == -1) bindForeignClass(vm, classObj, module);
}

static void createForeign(xssVm vm, ObjFiber* fiber, Value* stack)
{
  ObjClass* classObj = AS_CLASS(stack[0]);
  ASSERT(classObj->numFields == -1, "Class must be a foreign class.");

  // TODO: Don't look up every time.
  int symbol = xssSymbolTableFind(&vm->methodNames, "<allocate>", 10);
  ASSERT(symbol != -1, "Should have defined <allocate> symbol.");

  ASSERT(classObj->methods.count > symbol, "Class should have allocator.");
  Method* method = &classObj->methods.data[symbol];
  ASSERT(method->type == METHOD_FOREIGN, "Allocator should be foreign.");

  // Pass the constructor arguments to the allocator as well.
  ASSERT(vm->apiStack == NULL, "Cannot already be in foreign call.");
  vm->apiStack = stack;

  method->as.foreign(vm);

  vm->apiStack = NULL;
}

void xssFinalizeForeign(xssVm vm, ObjForeign* foreign)
{
  // TODO: Don't look up every time.
  int symbol = xssSymbolTableFind(&vm->methodNames, "<finalize>", 10);
  ASSERT(symbol != -1, "Should have defined <finalize> symbol.");

  // If there are no finalizers, don't finalize it.
  if (symbol == -1) return;

  // If the class doesn't have a finalizer, bail out.
  ObjClass* classObj = foreign->obj.classObj;
  if (symbol >= classObj->methods.count) return;

  Method* method = &classObj->methods.data[symbol];
  if (method->type == METHOD_NONE) return;

  ASSERT(method->type == METHOD_FOREIGN, "Finalizer should be foreign.");

  LunaFinalizerFn finalizer = (LunaFinalizerFn)method->as.foreign;
  finalizer(foreign->data);
}

// Let the host resolve an imported module name if it wants to.
static Value resolveModule(xssVm vm, Value name)
{
  // If the host doesn't care to resolve, leave the name alone.
  if (vm->config.resolveModuleFn == NULL) return name;

  ObjFiber* fiber = vm->fiber;
  ObjFn* fn = fiber->frames[fiber->numFrames - 1].closure->fn;
  ObjString* importer = fn->module->name;
  
  const char* resolved = vm->config.resolveModuleFn(vm, importer->value,
                                                    AS_CSTRING(name));
  if (resolved == NULL)
  {
    vm->fiber->error = xssStringFormat(vm,
        "Could not resolve module '@' imported from '@'.",
        name, OBJ_VAL(importer));
    return NULL_VAL;
  }
  
  // If they resolved to the exact same string, we don't need to copy it.
  if (resolved == AS_CSTRING(name)) return name;

  // Copy the string into a Luna String object.
  name = xssNewString(vm, resolved);
  DEALLOCATE(vm, (char*)resolved);
  return name;
}

static Value importModule(xssVm vm, Value name)
{
  name = resolveModule(vm, name);
  
  // If the module is already loaded, we don't need to do anything.
  Value existing = xssMapGet(vm->modules, name);
  if (!IS_UNDEFINED(existing)) return existing;

  xssPushRoot(vm, AS_OBJ(name));

  LunaLoadModuleResult result = {0};
  const char* source = NULL;
  
#if 0
  // Let the host try to provide the module.
  if (vm->config.loadModuleFn != NULL)
  {
    result = vm->config.loadModuleFn(vm, AS_CSTRING(name));
  }
#else
  afxUri2048 uri;
  AfxMakeUri2048(&uri, NIL);
  afxError err = 0;
  ObjString* nameString = AS_STRING(name);
  //AfxMakeUri(&uri, nameString->value, 0);
  AfxFormatUri(&uri.uri, "code/%.*s.xs", nameString->length, nameString->value);
  
  afxStream file;
  afxStreamInfo iobi = { 0 };
  iobi.usage = afxIoUsage_FILE;
  iobi.flags = afxIoFlag_READABLE;
  AfxAcquireStream(1, &iobi, &file);
  AfxReloadFile(file, &uri.uri);

  if (!file)
  {
      AfxThrowError();
      return NIL;
  }
  else
  {
      afxString code;
      AfxMakeString(&code, 0, AfxGetStreamBuffer(file, 0), AfxGetStreamCapacity(file));
      result.source = code.start;
      result.userData = file;
  }
#endif
  // If the host didn't provide it, see if it's a built in optional module.
  if (result.source == NULL)
  {
    result.onComplete = NULL;
    ObjString* nameString = AS_STRING(name);
#if LUNA_OPT_META
    if (strcmp(nameString->value, "meta") == 0) result.source = xssMetaSource();
#endif
#if LUNA_OPT_RANDOM
    if (strcmp(nameString->value, "random") == 0) result.source = xssRandomSource();
#endif
  }

  if (result.source == NULL)
  {
    vm->fiber->error = xssStringFormat(vm, "Could not load module '@'.", name);
    xssPopRoot(vm); // name.
    return NULL_VAL;
  }
  
  ObjClosure* moduleClosure = compileInModule(vm, name, result.source, false, true);
  
  // Now that we're done, give the result back in case there's cleanup to do.
  if(result.onComplete) result.onComplete(vm, AS_CSTRING(name), result);
  
  if (result.userData == file)
      AfxResetStream(result.userData);

  if (moduleClosure == NULL)
  {
    vm->fiber->error = xssStringFormat(vm,
                                        "Could not compile module '@'.", name);
    xssPopRoot(vm); // name.
    return NULL_VAL;
  }

  xssPopRoot(vm); // name.

  // Return the closure that executes the module.
  return OBJ_VAL(moduleClosure);
}

static Value getModuleVariable(xssVm vm, ObjModule* module,
                               Value variableName)
{
  ObjString* variable = AS_STRING(variableName);
  uint32_t variableEntry = xssSymbolTableFind(&module->variableNames,
                                               variable->value,
                                               variable->length);
  
  // It's a runtime error if the imported variable does not exist.
  if (variableEntry != UINT32_MAX)
  {
    return module->variables.data[variableEntry];
  }
  
  vm->fiber->error = xssStringFormat(vm,
      "Could not find a variable named '@' in module '@'.",
      variableName, OBJ_VAL(module->name));
  return NULL_VAL;
}

inline static bool checkArity(xssVm vm, Value value, int numArgs)
{
  ASSERT(IS_CLOSURE(value), "Receiver must be a closure.");
  ObjFn* fn = AS_CLOSURE(value)->fn;

  // We only care about missing arguments, not extras. The "- 1" is because
  // numArgs includes the receiver, the function itself, which we don't want to
  // count.
  if (numArgs - 1 >= fn->arity) return true;

  vm->fiber->error = CONST_STRING(vm, "Function expects more arguments.");
  return false;
}


// The main bytecode interpreter loop. This is where the magic happens. It is
// also, as you can imagine, highly performance critical.
static LunaInterpretResult runInterpreter(xssVm vm, register ObjFiber* fiber)
{
  // Remember the current fiber so we can find it if a GC happens.
  vm->fiber = fiber;
  fiber->state = FIBER_ROOT;

  // Hoist these into local variables. They are accessed frequently in the loop
  // but assigned less frequently. Keeping them in locals and updating them when
  // a call frame has been pushed or popped gives a large speed boost.
  register CallFrame* frame;
  register Value* stackStart;
  register uint8_t* ip;
  register ObjFn* fn;

  // These macros are designed to only be invoked within this function.
  #define PUSH(value)  (*fiber->stackTop++ = value)
  #define POP()        (*(--fiber->stackTop))
  #define DROP()       (fiber->stackTop--)
  #define PEEK()       (*(fiber->stackTop - 1))
  #define PEEK2()      (*(fiber->stackTop - 2))
  #define READ_BYTE()  (*ip++)
  #define READ_SHORT() (ip += 2, (uint16_t)((ip[-2] << 8) | ip[-1]))

  // Use this before a CallFrame is pushed to store the local variables back
  // into the current one.
  #define STORE_FRAME() frame->ip = ip

  // Use this after a CallFrame has been pushed or popped to refresh the local
  // variables.
  #define LOAD_FRAME()                                                         \
      do                                                                       \
      {                                                                        \
        frame = &fiber->frames[fiber->numFrames - 1];                          \
        stackStart = frame->stackStart;                                        \
        ip = frame->ip;                                                        \
        fn = frame->closure->fn;                                               \
      } while (false)

  // Terminates the current fiber with error string [error]. If another calling
  // fiber is willing to catch the error, transfers control to it, otherwise
  // exits the interpreter.
  #define RUNTIME_ERROR()                                                      \
      do                                                                       \
      {                                                                        \
        STORE_FRAME();                                                         \
        runtimeError(vm);                                                      \
        if (vm->fiber == NULL) return LUNA_RESULT_RUNTIME_ERROR;               \
        fiber = vm->fiber;                                                     \
        LOAD_FRAME();                                                          \
        DISPATCH();                                                            \
      } while (false)

  #if LUNA_DEBUG_TRACE_INSTRUCTIONS
    // Prints the stack and instruction before each instruction is executed.
    #define DEBUG_TRACE_INSTRUCTIONS()                                         \
        do                                                                     \
        {                                                                      \
          xssDumpStack(fiber);                                                \
          xssDumpInstruction(vm, fn, (int)(ip - fn->code.data));              \
        } while (false)
  #else
    #define DEBUG_TRACE_INSTRUCTIONS() do { } while (false)
  #endif

  #if LUNA_COMPUTED_GOTO

  static void* dispatchTable[] = {
    #define OPCODE(name, _) &&code_##name,
    #include "xssOpcodes.h"
    #undef OPCODE
  };

  #define INTERPRET_LOOP    DISPATCH();
  #define CASE_CODE(name)   code_##name

  #define DISPATCH()                                                           \
      do                                                                       \
      {                                                                        \
        DEBUG_TRACE_INSTRUCTIONS();                                            \
        goto *dispatchTable[instruction = (Code)READ_BYTE()];                  \
      } while (false)

  #else

  #define INTERPRET_LOOP                                                       \
      loop:                                                                    \
        DEBUG_TRACE_INSTRUCTIONS();                                            \
        switch (instruction = (Code)READ_BYTE())

  #define CASE_CODE(name)  case CODE_##name
  #define DISPATCH()       goto loop

  #endif

  LOAD_FRAME();

  Code instruction;
  INTERPRET_LOOP
  {
    CASE_CODE(LOAD_LOCAL_0):
    CASE_CODE(LOAD_LOCAL_1):
    CASE_CODE(LOAD_LOCAL_2):
    CASE_CODE(LOAD_LOCAL_3):
    CASE_CODE(LOAD_LOCAL_4):
    CASE_CODE(LOAD_LOCAL_5):
    CASE_CODE(LOAD_LOCAL_6):
    CASE_CODE(LOAD_LOCAL_7):
    CASE_CODE(LOAD_LOCAL_8):
      PUSH(stackStart[instruction - CODE_LOAD_LOCAL_0]);
      DISPATCH();

    CASE_CODE(LOAD_LOCAL):
      PUSH(stackStart[READ_BYTE()]);
      DISPATCH();

    CASE_CODE(LOAD_FIELD_THIS):
    {
      uint8_t field = READ_BYTE();
      Value receiver = stackStart[0];
      ASSERT(IS_INSTANCE(receiver), "Receiver should be instance.");
      ObjInstance* instance = AS_INSTANCE(receiver);
      ASSERT(field < instance->obj.classObj->numFields, "Out of bounds field.");
      PUSH(instance->fields[field]);
      DISPATCH();
    }

    CASE_CODE(POP):   DROP(); DISPATCH();
    CASE_CODE(NULL):  PUSH(NULL_VAL); DISPATCH();
    CASE_CODE(FALSE): PUSH(FALSE_VAL); DISPATCH();
    CASE_CODE(TRUE):  PUSH(TRUE_VAL); DISPATCH();

    CASE_CODE(STORE_LOCAL):
      stackStart[READ_BYTE()] = PEEK();
      DISPATCH();

    CASE_CODE(CONSTANT):
      PUSH(fn->constants.data[READ_SHORT()]);
      DISPATCH();

    {
      // The opcodes for doing method and superclass calls share a lot of code.
      // However, doing an if() test in the middle of the instruction sequence
      // to handle the bit that is special to super calls makes the non-super
      // call path noticeably slower.
      //
      // Instead, we do this old school using an explicit goto to share code for
      // everything at the tail end of the call-handling code that is the same
      // between normal and superclass calls.
      int numArgs;
      int symbol;

      Value* args;
      ObjClass* classObj;

      Method* method;

    CASE_CODE(CALL_0):
    CASE_CODE(CALL_1):
    CASE_CODE(CALL_2):
    CASE_CODE(CALL_3):
    CASE_CODE(CALL_4):
    CASE_CODE(CALL_5):
    CASE_CODE(CALL_6):
    CASE_CODE(CALL_7):
    CASE_CODE(CALL_8):
    CASE_CODE(CALL_9):
    CASE_CODE(CALL_10):
    CASE_CODE(CALL_11):
    CASE_CODE(CALL_12):
    CASE_CODE(CALL_13):
    CASE_CODE(CALL_14):
    CASE_CODE(CALL_15):
    CASE_CODE(CALL_16):
      // Add one for the implicit receiver argument.
      numArgs = instruction - CODE_CALL_0 + 1;
      symbol = READ_SHORT();

      // The receiver is the first argument.
      args = fiber->stackTop - numArgs;
      classObj = xssGetClassInline(vm, args[0]);
      goto completeCall;

    CASE_CODE(SUPER_0):
    CASE_CODE(SUPER_1):
    CASE_CODE(SUPER_2):
    CASE_CODE(SUPER_3):
    CASE_CODE(SUPER_4):
    CASE_CODE(SUPER_5):
    CASE_CODE(SUPER_6):
    CASE_CODE(SUPER_7):
    CASE_CODE(SUPER_8):
    CASE_CODE(SUPER_9):
    CASE_CODE(SUPER_10):
    CASE_CODE(SUPER_11):
    CASE_CODE(SUPER_12):
    CASE_CODE(SUPER_13):
    CASE_CODE(SUPER_14):
    CASE_CODE(SUPER_15):
    CASE_CODE(SUPER_16):
      // Add one for the implicit receiver argument.
      numArgs = instruction - CODE_SUPER_0 + 1;
      symbol = READ_SHORT();

      // The receiver is the first argument.
      args = fiber->stackTop - numArgs;

      // The superclass is stored in a constant.
      classObj = AS_CLASS(fn->constants.data[READ_SHORT()]);
      goto completeCall;

    completeCall:
      // If the class's method table doesn't include the symbol, bail.
      if (symbol >= classObj->methods.count ||
          (method = &classObj->methods.data[symbol])->type == METHOD_NONE)
      {
        methodNotFound(vm, classObj, symbol);
        RUNTIME_ERROR();
      }

      switch (method->type)
      {
        case METHOD_PRIMITIVE:
          if (method->as.primitive(vm, args))
          {
            // The result is now in the first arg slot. Discard the other
            // stack slots.
            fiber->stackTop -= numArgs - 1;
          } else {
            // An error, fiber switch, or call frame change occurred.
            STORE_FRAME();

            // If we don't have a fiber to switch to, stop interpreting.
            fiber = vm->fiber;
            if (fiber == NULL) return LUNA_RESULT_SUCCESS;
            if (xssHasError(fiber)) RUNTIME_ERROR();
            LOAD_FRAME();
          }
          break;

        case METHOD_FUNCTION_CALL: 
          if (!checkArity(vm, args[0], numArgs)) {
            RUNTIME_ERROR();
            break;
          }

          STORE_FRAME();
          method->as.primitive(vm, args);
          LOAD_FRAME();
          break;

        case METHOD_FOREIGN:
          callForeign(vm, fiber, method->as.foreign, numArgs);
          if (xssHasError(fiber)) RUNTIME_ERROR();
          break;

        case METHOD_BLOCK:
          STORE_FRAME();
          xssCallFunction(vm, fiber, (ObjClosure*)method->as.closure, numArgs);
          LOAD_FRAME();
          break;

        case METHOD_NONE:
          UNREACHABLE();
          break;
      }
      DISPATCH();
    }

    CASE_CODE(LOAD_UPVALUE):
    {
      ObjUpvalue** upvalues = frame->closure->upvalues;
      PUSH(*upvalues[READ_BYTE()]->value);
      DISPATCH();
    }

    CASE_CODE(STORE_UPVALUE):
    {
      ObjUpvalue** upvalues = frame->closure->upvalues;
      *upvalues[READ_BYTE()]->value = PEEK();
      DISPATCH();
    }

    CASE_CODE(LOAD_MODULE_VAR):
      PUSH(fn->module->variables.data[READ_SHORT()]);
      DISPATCH();

    CASE_CODE(STORE_MODULE_VAR):
      fn->module->variables.data[READ_SHORT()] = PEEK();
      DISPATCH();

    CASE_CODE(STORE_FIELD_THIS):
    {
      uint8_t field = READ_BYTE();
      Value receiver = stackStart[0];
      ASSERT(IS_INSTANCE(receiver), "Receiver should be instance.");
      ObjInstance* instance = AS_INSTANCE(receiver);
      ASSERT(field < instance->obj.classObj->numFields, "Out of bounds field.");
      instance->fields[field] = PEEK();
      DISPATCH();
    }

    CASE_CODE(LOAD_FIELD):
    {
      uint8_t field = READ_BYTE();
      Value receiver = POP();
      ASSERT(IS_INSTANCE(receiver), "Receiver should be instance.");
      ObjInstance* instance = AS_INSTANCE(receiver);
      ASSERT(field < instance->obj.classObj->numFields, "Out of bounds field.");
      PUSH(instance->fields[field]);
      DISPATCH();
    }

    CASE_CODE(STORE_FIELD):
    {
      uint8_t field = READ_BYTE();
      Value receiver = POP();
      ASSERT(IS_INSTANCE(receiver), "Receiver should be instance.");
      ObjInstance* instance = AS_INSTANCE(receiver);
      ASSERT(field < instance->obj.classObj->numFields, "Out of bounds field.");
      instance->fields[field] = PEEK();
      DISPATCH();
    }

    CASE_CODE(JUMP):
    {
      uint16_t offset = READ_SHORT();
      ip += offset;
      DISPATCH();
    }

    CASE_CODE(LOOP):
    {
      // Jump back to the top of the loop.
      uint16_t offset = READ_SHORT();
      ip -= offset;
      DISPATCH();
    }

    CASE_CODE(JUMP_IF):
    {
      uint16_t offset = READ_SHORT();
      Value condition = POP();

      if (xssIsFalsyValue(condition)) ip += offset;
      DISPATCH();
    }

    CASE_CODE(AND):
    {
      uint16_t offset = READ_SHORT();
      Value condition = PEEK();

      if (xssIsFalsyValue(condition))
      {
        // Short-circuit the right hand side.
        ip += offset;
      }
      else
      {
        // Discard the condition and evaluate the right hand side.
        DROP();
      }
      DISPATCH();
    }

    CASE_CODE(OR):
    {
      uint16_t offset = READ_SHORT();
      Value condition = PEEK();

      if (xssIsFalsyValue(condition))
      {
        // Discard the condition and evaluate the right hand side.
        DROP();
      }
      else
      {
        // Short-circuit the right hand side.
        ip += offset;
      }
      DISPATCH();
    }

    CASE_CODE(CLOSE_UPVALUE):
      // Close the upvalue for the local if we have one.
      closeUpvalues(fiber, fiber->stackTop - 1);
      DROP();
      DISPATCH();

    CASE_CODE(RETURN):
    {
      Value result = POP();
      fiber->numFrames--;

      // Close any upvalues still in scope.
      closeUpvalues(fiber, stackStart);

      // If the fiber is complete, end it.
      if (fiber->numFrames == 0)
      {
        // See if there's another fiber to return to. If not, we're done.
        if (fiber->caller == NULL)
        {
          // Store the final result value at the beginning of the stack so the
          // C API can get it.
          fiber->stack[0] = result;
          fiber->stackTop = fiber->stack + 1;
          return LUNA_RESULT_SUCCESS;
        }
        
        ObjFiber* resumingFiber = fiber->caller;
        fiber->caller = NULL;
        fiber = resumingFiber;
        vm->fiber = resumingFiber;
        
        // Store the result in the resuming fiber.
        fiber->stackTop[-1] = result;
      }
      else
      {
        // Store the result of the block in the first slot, which is where the
        // caller expects it.
        stackStart[0] = result;

        // Discard the stack slots for the call frame (leaving one slot for the
        // result).
        fiber->stackTop = frame->stackStart + 1;
      }
      
      LOAD_FRAME();
      DISPATCH();
    }

    CASE_CODE(CONSTRUCT):
      ASSERT(IS_CLASS(stackStart[0]), "'this' should be a class.");
      stackStart[0] = xssNewInstance(vm, AS_CLASS(stackStart[0]));
      DISPATCH();

    CASE_CODE(FOREIGN_CONSTRUCT):
      ASSERT(IS_CLASS(stackStart[0]), "'this' should be a class.");
      createForeign(vm, fiber, stackStart);
      if (xssHasError(fiber)) RUNTIME_ERROR();
      DISPATCH();

    CASE_CODE(CLOSURE):
    {
      // Create the closure and push it on the stack before creating upvalues
      // so that it doesn't get collected.
      ObjFn* function = AS_FN(fn->constants.data[READ_SHORT()]);
      ObjClosure* closure = xssNewClosure(vm, function);
      PUSH(OBJ_VAL(closure));

      // Capture upvalues, if any.
      for (int i = 0; i < function->numUpvalues; i++)
      {
        uint8_t isLocal = READ_BYTE();
        uint8_t index = READ_BYTE();
        if (isLocal)
        {
          // Make an new upvalue to close over the parent's local variable.
          closure->upvalues[i] = captureUpvalue(vm, fiber,
                                                frame->stackStart + index);
        }
        else
        {
          // Use the same upvalue as the current call frame.
          closure->upvalues[i] = frame->closure->upvalues[index];
        }
      }
      DISPATCH();
    }

    CASE_CODE(END_CLASS):
    {
      endClass(vm);
      if (xssHasError(fiber)) RUNTIME_ERROR();
      DISPATCH();
    }

    CASE_CODE(CLASS):
    {
      createClass(vm, READ_BYTE(), NULL);
      if (xssHasError(fiber)) RUNTIME_ERROR();
      DISPATCH();
    }

    CASE_CODE(FOREIGN_CLASS):
    {
      createClass(vm, -1, fn->module);
      if (xssHasError(fiber)) RUNTIME_ERROR();
      DISPATCH();
    }

    CASE_CODE(METHOD_INSTANCE):
    CASE_CODE(METHOD_STATIC):
    {
      uint16_t symbol = READ_SHORT();
      ObjClass* classObj = AS_CLASS(PEEK());
      Value method = PEEK2();
      bindMethod(vm, instruction, symbol, fn->module, classObj, method);
      if (xssHasError(fiber)) RUNTIME_ERROR();
      DROP();
      DROP();
      DISPATCH();
    }
    
    CASE_CODE(END_MODULE):
    {
      vm->lastModule = fn->module;
      PUSH(NULL_VAL);
      DISPATCH();
    }
    
    CASE_CODE(IMPORT_MODULE):
    {
      // Make a slot on the stack for the module's fiber to place the return
      // value. It will be popped after this fiber is resumed. Store the
      // imported module's closure in the slot in case a GC happens when
      // invoking the closure.
      PUSH(importModule(vm, fn->constants.data[READ_SHORT()]));
      if (xssHasError(fiber)) RUNTIME_ERROR();
      
      // If we get a closure, call it to execute the module body.
      if (IS_CLOSURE(PEEK()))
      {
        STORE_FRAME();
        ObjClosure* closure = AS_CLOSURE(PEEK());
        xssCallFunction(vm, fiber, closure, 1);
        LOAD_FRAME();
      }
      else
      {
        // The module has already been loaded. Remember it so we can import
        // variables from it if needed.
        vm->lastModule = AS_MODULE(PEEK());
      }

      DISPATCH();
    }
    
    CASE_CODE(IMPORT_VARIABLE):
    {
      Value variable = fn->constants.data[READ_SHORT()];
      ASSERT(vm->lastModule != NULL, "Should have already imported module.");
      Value result = getModuleVariable(vm, vm->lastModule, variable);
      if (xssHasError(fiber)) RUNTIME_ERROR();

      PUSH(result);
      DISPATCH();
    }

    CASE_CODE(END):
      // A CODE_END should always be preceded by a CODE_RETURN. If we get here,
      // the compiler generated wrong code.
      UNREACHABLE();
  }

  // We should only exit this function from an explicit return from CODE_RETURN
  // or a runtime error.
  UNREACHABLE();
  return LUNA_RESULT_RUNTIME_ERROR;

  #undef READ_BYTE
  #undef READ_SHORT
}

LunaHandle* xssMakeCallHandle(xssVm vm, const char* signature)
{
  ASSERT(signature != NULL, "Signature cannot be NULL.");
  
  int signatureLength = (int)strlen(signature);
  ASSERT(signatureLength > 0, "Signature cannot be empty.");
  
  // Count the number parameters the method expects.
  int numParams = 0;
  if (signature[signatureLength - 1] == ')')
  {
    for (int i = signatureLength - 1; i > 0 && signature[i] != '('; i--)
    {
      if (signature[i] == '_') numParams++;
    }
  }
  
  // Count subscript arguments.
  if (signature[0] == '[')
  {
    for (int i = 0; i < signatureLength && signature[i] != ']'; i++)
    {
      if (signature[i] == '_') numParams++;
    }
  }
  
  // Add the signatue to the method table.
  int method =  xssSymbolTableEnsure(vm, &vm->methodNames,
                                      signature, signatureLength);
  
  // Create a little stub function that assumes the arguments are on the stack
  // and calls the method.
  ObjFn* fn = xssNewFunction(vm, NULL, numParams + 1);
  
  // Wrap the function in a closure and then in a handle. Do this here so it
  // doesn't get collected as we fill it in.
  LunaHandle* value = xssMakeHandle(vm, OBJ_VAL(fn));
  value->value = OBJ_VAL(xssNewClosure(vm, fn));
  
  xssByteBufferWrite(vm, &fn->code, (uint8_t)(CODE_CALL_0 + numParams));
  xssByteBufferWrite(vm, &fn->code, (method >> 8) & 0xff);
  xssByteBufferWrite(vm, &fn->code, method & 0xff);
  xssByteBufferWrite(vm, &fn->code, CODE_RETURN);
  xssByteBufferWrite(vm, &fn->code, CODE_END);
  xssIntBufferFill(vm, &fn->debug->sourceLines, 0, 5);
  xssFunctionBindName(vm, fn, signature, signatureLength);

  return value;
}

LunaInterpretResult xssCall(xssVm vm, LunaHandle* method)
{
  ASSERT(method != NULL, "Method cannot be NULL.");
  ASSERT(IS_CLOSURE(method->value), "Method must be a method handle.");
  ASSERT(vm->fiber != NULL, "Must set up arguments for call first.");
  ASSERT(vm->apiStack != NULL, "Must set up arguments for call first.");
  ASSERT(vm->fiber->numFrames == 0, "Can not call from a foreign method.");
  
  ObjClosure* closure = AS_CLOSURE(method->value);
  
  ASSERT(vm->fiber->stackTop - vm->fiber->stack >= closure->fn->arity,
         "Stack must have enough arguments for method.");
  
  // Clear the API stack. Now that xssCall() has control, we no longer need
  // it. We use this being non-null to tell if re-entrant calls to foreign
  // methods are happening, so it's important to clear it out now so that you
  // can call foreign methods from within calls to xssCall().
  vm->apiStack = NULL;

  // Discard any extra temporary slots. We take for granted that the stub
  // function has exactly one slot for each argument.
  vm->fiber->stackTop = &vm->fiber->stack[closure->fn->maxSlots];
  
  xssCallFunction(vm, vm->fiber, closure, 0);
  LunaInterpretResult result = runInterpreter(vm, vm->fiber);
  
  // If the call didn't abort, then set up the API stack to point to the
  // beginning of the stack so the host can access the call's return value.
  if (vm->fiber != NULL) vm->apiStack = vm->fiber->stack;
  
  return result;
}

LunaHandle* xssMakeHandle(xssVm vm, Value value)
{
  if (IS_OBJ(value)) xssPushRoot(vm, AS_OBJ(value));
  
  // Make a handle for it.
  LunaHandle* handle = ALLOCATE(vm, LunaHandle);
  handle->value = value;

  if (IS_OBJ(value)) xssPopRoot(vm);

  // Add it to the front of the linked list of handles.
  if (vm->handles != NULL) vm->handles->prev = handle;
  handle->prev = NULL;
  handle->next = vm->handles;
  vm->handles = handle;
  
  return handle;
}

void xssReleaseHandle(xssVm vm, LunaHandle* handle)
{
  ASSERT(handle != NULL, "Handle cannot be NULL.");

  // Update the VM's head pointer if we're releasing the first handle.
  if (vm->handles == handle) vm->handles = handle->next;

  // Unlink it from the list.
  if (handle->prev != NULL) handle->prev->next = handle->next;
  if (handle->next != NULL) handle->next->prev = handle->prev;

  // Clear it out. This isn't strictly necessary since we're going to free it,
  // but it makes for easier debugging.
  handle->prev = NULL;
  handle->next = NULL;
  handle->value = NULL_VAL;
  DEALLOCATE(vm, handle);
}

LunaInterpretResult xssInterpret(xssVm vm, const char* module,
                                  const char* source)
{
  ObjClosure* closure = xssCompileSource(vm, module, source, false, true);
  if (closure == NULL) return LUNA_RESULT_COMPILE_ERROR;
  
  xssPushRoot(vm, (Obj*)closure);
  ObjFiber* fiber = xssNewFiber(vm, closure);
  xssPopRoot(vm); // closure.
  vm->apiStack = NULL;

  return runInterpreter(vm, fiber);
}

ObjClosure* xssCompileSource(xssVm vm, const char* module, const char* source,
                            bool isExpression, bool printErrors)
{
  Value nameValue = NULL_VAL;
  if (module != NULL)
  {
    nameValue = xssNewString(vm, module);
    xssPushRoot(vm, AS_OBJ(nameValue));
  }
  
  ObjClosure* closure = compileInModule(vm, nameValue, source,
                                        isExpression, printErrors);

  if (module != NULL) xssPopRoot(vm); // nameValue.
  return closure;
}

Value xssGetModuleVariable(xssVm vm, Value moduleName, Value variableName)
{
  ObjModule* module = getModule(vm, moduleName);
  if (module == NULL)
  {
    vm->fiber->error = xssStringFormat(vm, "Module '@' is not loaded.",
                                        moduleName);
    return NULL_VAL;
  }
  
  return getModuleVariable(vm, module, variableName);
}

Value xssFindVariable(xssVm vm, ObjModule* module, const char* name)
{
  int symbol = xssSymbolTableFind(&module->variableNames, name, strlen(name));
  return module->variables.data[symbol];
}

int xssDeclareVariable(xssVm vm, ObjModule* module, const char* name,
                        size_t length, int line)
{
  if (module->variables.count == MAX_MODULE_VARS) return -2;

  // Implicitly defined variables get a "value" that is the line where the
  // variable is first used. We'll use that later to report an error on the
  // right line.
  xssValueBufferWrite(vm, &module->variables, NUM_VAL(line));
  return xssSymbolTableAdd(vm, &module->variableNames, name, length);
}

int xssDefineVariable(xssVm vm, ObjModule* module, const char* name,
                       size_t length, Value value, int* line)
{
  if (module->variables.count == MAX_MODULE_VARS) return -2;

  if (IS_OBJ(value)) xssPushRoot(vm, AS_OBJ(value));

  // See if the variable is already explicitly or implicitly declared.
  int symbol = xssSymbolTableFind(&module->variableNames, name, length);

  if (symbol == -1)
  {
    // Brand new variable.
    symbol = xssSymbolTableAdd(vm, &module->variableNames, name, length);
    xssValueBufferWrite(vm, &module->variables, value);
  }
  else if (IS_NUM(module->variables.data[symbol]))
  {
    // An implicitly declared variable's value will always be a number.
    // Now we have a real definition.
    if(line) *line = (int)AS_NUM(module->variables.data[symbol]);
    module->variables.data[symbol] = value;

	// If this was a localname we want to error if it was 
	// referenced before this definition.
	if (xssIsLocalName(name)) symbol = -3;
  }
  else
  {
    // Already explicitly declared.
    symbol = -1;
  }

  if (IS_OBJ(value)) xssPopRoot(vm);

  return symbol;
}

// TODO: Inline?
void xssPushRoot(xssVm vm, Obj* obj)
{
  ASSERT(obj != NULL, "Can't root NULL.");
  ASSERT(vm->numTempRoots < LUNA_MAX_TEMP_ROOTS, "Too many temporary roots.");

  vm->tempRoots[vm->numTempRoots++] = obj;
}

void xssPopRoot(xssVm vm)
{
  ASSERT(vm->numTempRoots > 0, "No temporary roots to release.");
  vm->numTempRoots--;
}

int xssGetSlotCount(xssVm vm)
{
  if (vm->apiStack == NULL) return 0;
  
  return (int)(vm->fiber->stackTop - vm->apiStack);
}

void xssEnsureSlots(xssVm vm, int numSlots)
{
  // If we don't have a fiber accessible, create one for the API to use.
  if (vm->apiStack == NULL)
  {
    vm->fiber = xssNewFiber(vm, NULL);
    vm->apiStack = vm->fiber->stack;
  }
  
  int currentSize = (int)(vm->fiber->stackTop - vm->apiStack);
  if (currentSize >= numSlots) return;
  
  // Grow the stack if needed.
  int needed = (int)(vm->apiStack - vm->fiber->stack) + numSlots;
  xssEnsureStack(vm, vm->fiber, needed);
  
  vm->fiber->stackTop = vm->apiStack + numSlots;
}

// Ensures that [slot] is a valid index into the API's stack of slots.
static void validateApiSlot(xssVm vm, int slot)
{
  ASSERT(slot >= 0, "Slot cannot be negative.");
  ASSERT(slot < xssGetSlotCount(vm), "Not that many slots.");
}

// Gets the type of the object in [slot].
LunaType xssGetSlotType(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  if (IS_BOOL(vm->apiStack[slot])) return LUNA_TYPE_BOOL;
  if (IS_NUM(vm->apiStack[slot])) return LUNA_TYPE_NUM;
  if (IS_FOREIGN(vm->apiStack[slot])) return LUNA_TYPE_FOREIGN;
  if (IS_LIST(vm->apiStack[slot])) return LUNA_TYPE_LIST;
  if (IS_MAP(vm->apiStack[slot])) return LUNA_TYPE_MAP;
  if (IS_NULL(vm->apiStack[slot])) return LUNA_TYPE_NULL;
  if (IS_STRING(vm->apiStack[slot])) return LUNA_TYPE_STRING;
  
  return LUNA_TYPE_UNKNOWN;
}

bool xssGetSlotBool(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_BOOL(vm->apiStack[slot]), "Slot must hold a bool.");

  return AS_BOOL(vm->apiStack[slot]);
}

const char* xssGetSlotBytes(xssVm vm, int slot, int* length)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_STRING(vm->apiStack[slot]), "Slot must hold a string.");
  
  ObjString* string = AS_STRING(vm->apiStack[slot]);
  *length = string->length;
  return string->value;
}

double xssGetSlotDouble(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_NUM(vm->apiStack[slot]), "Slot must hold a number.");

  return AS_NUM(vm->apiStack[slot]);
}

void* xssGetSlotForeign(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_FOREIGN(vm->apiStack[slot]),
         "Slot must hold a foreign instance.");

  return AS_FOREIGN(vm->apiStack[slot])->data;
}

const char* xssGetSlotString(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_STRING(vm->apiStack[slot]), "Slot must hold a string.");

  return AS_CSTRING(vm->apiStack[slot]);
}

LunaHandle* xssGetSlotHandle(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  return xssMakeHandle(vm, vm->apiStack[slot]);
}

// Stores [value] in [slot] in the foreign call stack.
static void setSlot(xssVm vm, int slot, Value value)
{
  validateApiSlot(vm, slot);
  vm->apiStack[slot] = value;
}

void xssSetSlotBool(xssVm vm, int slot, bool value)
{
  setSlot(vm, slot, BOOL_VAL(value));
}

void xssSetSlotBytes(xssVm vm, int slot, const char* bytes, size_t length)
{
  ASSERT(bytes != NULL, "Byte array cannot be NULL.");
  setSlot(vm, slot, xssNewStringLength(vm, bytes, length));
}

void xssSetSlotDouble(xssVm vm, int slot, double value)
{
  setSlot(vm, slot, NUM_VAL(value));
}

void* xssSetSlotNewForeign(xssVm vm, int slot, int classSlot, size_t size)
{
  validateApiSlot(vm, slot);
  validateApiSlot(vm, classSlot);
  ASSERT(IS_CLASS(vm->apiStack[classSlot]), "Slot must hold a class.");
  
  ObjClass* classObj = AS_CLASS(vm->apiStack[classSlot]);
  ASSERT(classObj->numFields == -1, "Class must be a foreign class.");
  
  ObjForeign* foreign = xssNewForeign(vm, classObj, size);
  vm->apiStack[slot] = OBJ_VAL(foreign);
  
  return (void*)foreign->data;
}

void xssSetSlotNewList(xssVm vm, int slot)
{
  setSlot(vm, slot, OBJ_VAL(xssNewList(vm, 0)));
}

void xssSetSlotNewMap(xssVm vm, int slot)
{
  setSlot(vm, slot, OBJ_VAL(xssNewMap(vm)));
}

void xssSetSlotNull(xssVm vm, int slot)
{
  setSlot(vm, slot, NULL_VAL);
}

void xssSetSlotString(xssVm vm, int slot, const char* text)
{
  ASSERT(text != NULL, "String cannot be NULL.");
  
  setSlot(vm, slot, xssNewString(vm, text));
}

void xssSetSlotHandle(xssVm vm, int slot, LunaHandle* handle)
{
  ASSERT(handle != NULL, "Handle cannot be NULL.");

  setSlot(vm, slot, handle->value);
}

int xssGetListCount(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_LIST(vm->apiStack[slot]), "Slot must hold a list.");
  
  ValueBuffer elements = AS_LIST(vm->apiStack[slot])->elements;
  return elements.count;
}

void xssGetListElement(xssVm vm, int listSlot, int index, int elementSlot)
{
  validateApiSlot(vm, listSlot);
  validateApiSlot(vm, elementSlot);
  ASSERT(IS_LIST(vm->apiStack[listSlot]), "Slot must hold a list.");

  ValueBuffer elements = AS_LIST(vm->apiStack[listSlot])->elements;

  uint32_t usedIndex = xssValidateIndex(elements.count, index);
  ASSERT(usedIndex != UINT32_MAX, "Index out of bounds.");

  vm->apiStack[elementSlot] = elements.data[usedIndex];
}

void xssSetListElement(xssVm vm, int listSlot, int index, int elementSlot)
{
  validateApiSlot(vm, listSlot);
  validateApiSlot(vm, elementSlot);
  ASSERT(IS_LIST(vm->apiStack[listSlot]), "Slot must hold a list.");

  ObjList* list = AS_LIST(vm->apiStack[listSlot]);

  uint32_t usedIndex = xssValidateIndex(list->elements.count, index);
  ASSERT(usedIndex != UINT32_MAX, "Index out of bounds.");
  
  list->elements.data[usedIndex] = vm->apiStack[elementSlot];
}

void xssInsertInList(xssVm vm, int listSlot, int index, int elementSlot)
{
  validateApiSlot(vm, listSlot);
  validateApiSlot(vm, elementSlot);
  ASSERT(IS_LIST(vm->apiStack[listSlot]), "Must insert into a list.");
  
  ObjList* list = AS_LIST(vm->apiStack[listSlot]);
  
  // Negative indices count from the end. 
  // We don't use xssValidateIndex here because insert allows 1 past the end.
  if (index < 0) index = list->elements.count + 1 + index;
  
  ASSERT(index <= list->elements.count, "Index out of bounds.");
  
  xssListInsert(vm, list, vm->apiStack[elementSlot], index);
}

int xssGetMapCount(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  ASSERT(IS_MAP(vm->apiStack[slot]), "Slot must hold a map.");

  ObjMap* map = AS_MAP(vm->apiStack[slot]);
  return map->count;
}

bool xssGetMapContainsKey(xssVm vm, int mapSlot, int keySlot)
{
  validateApiSlot(vm, mapSlot);
  validateApiSlot(vm, keySlot);
  ASSERT(IS_MAP(vm->apiStack[mapSlot]), "Slot must hold a map.");

  Value key = vm->apiStack[keySlot];
  ASSERT(xssMapIsValidKey(key), "Key must be a value type");
  if (!validateKey(vm, key)) return false;

  ObjMap* map = AS_MAP(vm->apiStack[mapSlot]);
  Value value = xssMapGet(map, key);

  return !IS_UNDEFINED(value);
}

void xssGetMapValue(xssVm vm, int mapSlot, int keySlot, int valueSlot)
{
  validateApiSlot(vm, mapSlot);
  validateApiSlot(vm, keySlot);
  validateApiSlot(vm, valueSlot);
  ASSERT(IS_MAP(vm->apiStack[mapSlot]), "Slot must hold a map.");

  ObjMap* map = AS_MAP(vm->apiStack[mapSlot]);
  Value value = xssMapGet(map, vm->apiStack[keySlot]);
  if (IS_UNDEFINED(value)) {
    value = NULL_VAL;
  }

  vm->apiStack[valueSlot] = value;
}

void xssSetMapValue(xssVm vm, int mapSlot, int keySlot, int valueSlot)
{
  validateApiSlot(vm, mapSlot);
  validateApiSlot(vm, keySlot);
  validateApiSlot(vm, valueSlot);
  ASSERT(IS_MAP(vm->apiStack[mapSlot]), "Must insert into a map.");
  
  Value key = vm->apiStack[keySlot];
  ASSERT(xssMapIsValidKey(key), "Key must be a value type");

  if (!validateKey(vm, key)) {
    return;
  }

  Value value = vm->apiStack[valueSlot];
  ObjMap* map = AS_MAP(vm->apiStack[mapSlot]);
  
  xssMapSet(vm, map, key, value);
}

void xssRemoveMapValue(xssVm vm, int mapSlot, int keySlot, 
                        int removedValueSlot)
{
  validateApiSlot(vm, mapSlot);
  validateApiSlot(vm, keySlot);
  ASSERT(IS_MAP(vm->apiStack[mapSlot]), "Slot must hold a map.");

  Value key = vm->apiStack[keySlot];
  if (!validateKey(vm, key)) {
    return;
  }

  ObjMap* map = AS_MAP(vm->apiStack[mapSlot]);
  Value removed = xssMapRemoveKey(vm, map, key);
  setSlot(vm, removedValueSlot, removed);
}

void xssGetVariable(xssVm vm, const char* module, const char* name,
                     int slot)
{
  ASSERT(module != NULL, "Module cannot be NULL.");
  ASSERT(name != NULL, "Variable name cannot be NULL.");  

  Value moduleName = xssStringFormat(vm, "$", module);
  xssPushRoot(vm, AS_OBJ(moduleName));
  
  ObjModule* moduleObj = getModule(vm, moduleName);
  ASSERT(moduleObj != NULL, "Could not find module.");
  
  xssPopRoot(vm); // moduleName.

  int variableSlot = xssSymbolTableFind(&moduleObj->variableNames,
                                         name, strlen(name));
  ASSERT(variableSlot != -1, "Could not find variable.");
  
  setSlot(vm, slot, moduleObj->variables.data[variableSlot]);
}

bool xssHasVariable(xssVm vm, const char* module, const char* name)
{
  ASSERT(module != NULL, "Module cannot be NULL.");
  ASSERT(name != NULL, "Variable name cannot be NULL.");

  Value moduleName = xssStringFormat(vm, "$", module);
  xssPushRoot(vm, AS_OBJ(moduleName));

  //We don't use xssHasModule since we want to use the module object.
  ObjModule* moduleObj = getModule(vm, moduleName);
  ASSERT(moduleObj != NULL, "Could not find module.");

  xssPopRoot(vm); // moduleName.

  int variableSlot = xssSymbolTableFind(&moduleObj->variableNames,
    name, strlen(name));

  return variableSlot != -1;
}

bool xssHasModule(xssVm vm, const char* module)
{
  ASSERT(module != NULL, "Module cannot be NULL.");
  
  Value moduleName = xssStringFormat(vm, "$", module);
  xssPushRoot(vm, AS_OBJ(moduleName));

  ObjModule* moduleObj = getModule(vm, moduleName);
  
  xssPopRoot(vm); // moduleName.

  return moduleObj != NULL;
}

void xssAbortFiber(xssVm vm, int slot)
{
  validateApiSlot(vm, slot);
  vm->fiber->error = vm->apiStack[slot];
}

void* xssGetUserData(xssVm vm)
{
	return vm->config.userData;
}

void xssSetUserData(xssVm vm, void* userData)
{
	vm->config.userData = userData;
}
