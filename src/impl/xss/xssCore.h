#ifndef xssCore_h
#define xssCore_h

#include "xssVm.h"

// This module defines the built-in classes and their primitives methods that
// are implemented directly in C code. Some languages try to implement as much
// of the core module itself in the primary language instead of in the host
// language.
//
// With Luna, we try to do as much of it in C as possible. Primitive methods
// are always faster than code written in Luna, and it minimizes startup time
// since we don't have to parse, compile, and execute Luna code.
//
// There is one limitation, though. Methods written in C cannot call Luna ones.
// They can only be the top of the callstack, and immediately return. This
// makes it difficult to have primitive methods that rely on polymorphic
// behavior. For example, `System.print` should call `toString` on its argument,
// including user-defined `toString` methods on user-defined classes.

void xssInitializeCore(xssVm env);

#endif
