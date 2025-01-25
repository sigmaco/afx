#include "xssOptMeta.h"

#if LUNA_OPT_META

#include <string.h>

#include "xssVm.h"
#include "xssOptMeta.xss.inc"

void metaCompile(xssVm env)
{
  const char* source = xssGetSlotString(env, 1);
  bool isExpression = xssGetSlotBool(env, 2);
  bool printErrors = xssGetSlotBool(env, 3);

  // TODO: Allow passing in module?
  // Look up the module surrounding the callsite. This is brittle. The -2 walks
  // up the callstack assuming that the meta module has one level of
  // indirection before hitting the user's code. Any change to meta may require
  // this constant to be tweaked.
  ObjFiber* currentFiber = env->fiber;
  ObjFn* fn = currentFiber->frames[currentFiber->numFrames - 2].closure->fn;
  ObjString* module = fn->module->name;

  ObjClosure* closure = xssCompileSource(env, module->value, source,
                                          isExpression, printErrors);
  
  // Return the result. We can't use the public API for this since we have a
  // bare ObjClosure*.
  if (closure == NULL)
  {
    env->apiStack[0] = NULL_VAL;
  }
  else
  {
    env->apiStack[0] = OBJ_VAL(closure);
  }
}

void metaGetModuleVariables(xssVm env)
{
  xssEnsureSlots(env, 3);
  
  Value moduleValue = xssMapGet(env->modules, env->apiStack[1]);
  if (IS_UNDEFINED(moduleValue))
  {
    env->apiStack[0] = NULL_VAL;
    return;
  }
    
  ObjModule* module = AS_MODULE(moduleValue);
  ObjList* names = xssNewList(env, module->variableNames.count);
  env->apiStack[0] = OBJ_VAL(names);

  // Initialize the elements to null in case a collection happens when we
  // allocate the strings below.
  for (int i = 0; i < names->elements.count; i++)
  {
    names->elements.data[i] = NULL_VAL;
  }
  
  for (int i = 0; i < names->elements.count; i++)
  {
    names->elements.data[i] = OBJ_VAL(module->variableNames.data[i]);
  }
}

const char* xssMetaSource()
{
  return metaModuleSource;
}

LunaForeignMethodFn xssMetaBindForeignMethod(xssVm env,
                                              const char* className,
                                              bool isStatic,
                                              const char* signature)
{
  // There is only one foreign method in the meta module.
  ASSERT(strcmp(className, "Meta") == 0, "Should be in Meta class.");
  ASSERT(isStatic, "Should be static.");
  
  if (strcmp(signature, "compile_(_,_,_)") == 0)
  {
    return metaCompile;
  }
  
  if (strcmp(signature, "getModuleVariables_(_)") == 0)
  {
    return metaGetModuleVariables;
  }
  
  ASSERT(false, "Unknown method.");
  return NULL;
}

#endif
