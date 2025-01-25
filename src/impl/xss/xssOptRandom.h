#ifndef xssOpt_random_h
#define xssOpt_random_h

#include "xssCommon.h"
#include "./xss.h"

#if LUNA_OPT_RANDOM

const char* xssRandomSource();
LunaForeignClassMethods xssRandomBindForeignClass(xssVm env,
                                                   const char* module,
                                                   const char* className);
LunaForeignMethodFn xssRandomBindForeignMethod(xssVm env,
                                                const char* className,
                                                bool isStatic,
                                                const char* signature);

#endif

#endif
