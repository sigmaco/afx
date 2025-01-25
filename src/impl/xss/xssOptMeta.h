#ifndef xssOpt_meta_h
#define xssOpt_meta_h

#include "xssCommon.h"
#include "./xss.h"

// This module defines the Meta class and its associated methods.
#if LUNA_OPT_META

const char* xssMetaSource();
LunaForeignMethodFn xssMetaBindForeignMethod(xssVm env,
                                              const char* className,
                                              bool isStatic,
                                              const char* signature);

#endif

#endif
