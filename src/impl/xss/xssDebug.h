#ifndef xssDebug_h
#define xssDebug_h

#include "xssValue.h"
#include "xssVm.h"

// Prints the stack trace for the current fiber.
//
// Used when a fiber throws a runtime error which is not caught.
void xssDebugPrintStackTrace(xssVm env);

// The "dump" functions are used for debugging Luna itself. Normal code paths
// will not call them unless one of the various DEBUG_ flags is enabled.

// Prints a representation of [value] to stdout.
void xssDumpValue(Value value);

// Prints a representation of the bytecode for [fn] at instruction [i].
int xssDumpInstruction(xssVm env, ObjFn* fn, int i);

// Prints the disassembled code for [fn] to stdout.
void xssDumpCode(xssVm env, ObjFn* fn);

// Prints the contents of the current stack for [fiber] to stdout.
void xssDumpStack(ObjFiber* fiber);

#endif
