/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

///////////////////////////////////////////////////////////////////////////////
// PLATFORM HANDLING                                                         //
///////////////////////////////////////////////////////////////////////////////

#ifndef AFX_PLATFORM_DEFS_H
#define AFX_PLATFORM_DEFS_H

#include <assert.h>
#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <intrin.h>

// Detect x64 architecture.
#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#   define AFX_ARCH_X64 1
#   define AFX_ISA_X86 1
#   define AFX_ISA_X64 1
#endif

// Detect 32-bit x86 architecture.
#if defined(_M_IX86) || defined(__i386__)
#   define AFX_ARCH_X86 1
#   define AFX_ISA_X86 1
#endif

#ifdef _WIN64
#   define AFX_ON_X86_64 1
#   define AFX_ON_WINDOWS 1
#elif _WIN32
#   define AFX_ON_X86_32 1
#   define AFX_ON_WINDOWS 1
#else
#   define AFX_ON_X86_64 1
#   define AFX_ON_LINUX 1
#endif

#if (defined(AFX_ON_X86_64)||defined(AFX_ON_X86_32))
#   define AFX_ON_X86 1
#endif

#ifdef AFX_ON_WINDOWS
#   if (defined(AFX_ON_X86_64))
#       define AFX_ON_WIN64 1
#   elif defined(AFX_ON_X86_32)
#       define AFX_ON_WIN32 1
#   else
#       error "Unsupported ISA"
#   endif
#elif AFX_ON_LINUX
#   if (defined(AFX_ON_X86_64))
#       define AFX_ON_LINUX64 1
#   elif defined(AFX_ON_X86_32)
#       define AFX_ON_LINUX32 1
#   else
#       error "Unsupported ISA"
#   endif
#else
#   error "Unsupported OS"
#endif

#ifdef AFX_ON_X86
#   define AFX_LE
#else
#   error "Unsupported endianess"
#endif

#ifdef AFX_ON_X86_64
#   define AFX_ISA_SSE
#   define AFX_ISA_SSE2
#endif

///////////////////////////////////////////////////////////////////////////////
// COMPILER HANDLING                                                         //
///////////////////////////////////////////////////////////////////////////////

 //#ifdef __clang__
#ifdef _AFX_CORE_C
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
// macro name is a reserved identifier
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
//#pragma clang diagnostic pop
#endif//_AFX_CORE_C

#if !0
// Let's shut up MSVC. O que ela quer tá é mole.
#if defined(_MSC_VER) && !defined(__clang__)
 //#   pragma warning (disable : 4054) // 'type cast': from function pointer '' to data pointer ''
#   pragma warning (disable : 4152) // nonstandard extension, function/data pointer conversion in expression
#   pragma warning (disable : 4324) // '': structure was padded due to alignment specifier
#   pragma warning (disable : 4204) // nonstandard extension used : non-constant aggregate initializer
#   pragma warning (disable : 4201) // nonstandard extension used: nameless struct/union
//#   pragma warning (disable : 4706) // assignment within conditional expression
#   pragma warning (disable : 4820) // '' bytes padding added after data member ''
#endif
#endif

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#if (defined(_MSC_VER)/* && !defined(__clang__)*/)
#   define INLINE __forceinline
#else
#   define INLINE inline __attribute__((always_inline))
#endif

#define EMBED extern INLINE

#define AFXCALL __cdecl

#if (defined(_MSC_VER)/* && !defined(__clang__)*/)
#  define AFX_ALIGNED(...) __declspec(align(__VA_ARGS__))
#else
#  define AFX_ALIGNED(...) __attribute__((aligned(__VA_ARGS__)))
#endif

#if !defined (AFX_DEPRECATED)
#   ifdef __GNUC__
#       define AFX_DEPRECATED __attribute__((deprecated))
#   elif defined(_MSC_VER)
#       define AFX_DEPRECATED __declspec(deprecated)
#   else
#       define AFX_DEPRECATED
#   endif
#endif

#ifndef AFX_THREAD_LOCAL
#   if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201102L)) && !defined(_Thread_local)
#       if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#           define AFX_THREAD_LOCAL __thread
#       else
#           define AFX_THREAD_LOCAL __declspec(thread)
#       endif
#   elif defined(__GNUC__) && defined(__GNUC_MINOR__) && (((__GNUC__ << 8) | __GNUC_MINOR__) < ((4 << 8) | 9))
#       define AFX_THREAD_LOCAL __thread
#   else
#       define AFX_THREAD_LOCAL _Thread_local
#   endif
#endif//AFX_THREAD_LOCAL
#ifndef AFX_TLS
#   define AFX_TLS AFX_THREAD_LOCAL
#endif//AFX_TLS

#ifndef AFX_STATIC_ASSERT
// Best fully portable approach (works on MSVC, GCC, Clang).
// Use _Static_assert when available, otherwise fall back to an array - size assertion.

// Detect C11 _Static_assert
#   if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#       define AFX_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#   elif defined(_MSC_VER)
/*
 * MSVC: avoid static_assert because it misbehaves with macro expressions.
 * Use typedef trick. Generates a readable line-number error.
 */
#       define AFX_STATIC_ASSERT_GLUE_(a, b) a##b
#       define AFX_STATIC_ASSERT_GLUE(a, b) AFX_STATIC_ASSERT_GLUE_(a, b)
#       define AFX_STATIC_ASSERT(cond, msg) \
            typedef char AFX_STATIC_ASSERT_GLUE(static_assert_failed_at_line_, __LINE__)[(cond) ? 1 : -1]
#   else
/* Generic fallback for old GCC, TinyCC, etc. */
#       define AFX_STATIC_ASSERT_GLUE_(a, b) a##b
#       define AFX_STATIC_ASSERT_GLUE(a, b) AFX_STATIC_ASSERT_GLUE_(a, b)
#       define AFX_STATIC_ASSERT(cond, msg) \
            typedef char AFX_STATIC_ASSERT_GLUE(static_assert_failed_at_line_, __LINE__)[(cond) ? 1 : -1]
#   endif//_MSC_VER
#endif//AFX_STATIC_ASSERT


// MSC (for surprise of a totally of zero persons) shits on "AFX_PTR_ALIGNMENT sizeof(void*)" and other sizeof() macroes.
#if (defined(_MSC_VER) && !defined(__clang__))
#ifdef AFX_ON_X86_64
#   define AFX_PTR_ALIGNMENT 64
#   define AFX_ATOMIC_ALIGNMENT 64
#elif  AFX_ON_X86_32
#   define AFX_PTR_ALIGNMENT 32
#   define AFX_ATOMIC_ALIGNMENT 64 // assumes AFX is on WOW64
#else
#error ""
#endif
#endif

/*
    Unified TYPEOF() (C23 -> compiler extensions -> error)
    This is enough for most projects.
*/

#ifndef AFX_TYPEOF
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L) // C23 or later: typeof is standard
#       define AFX_TYPEOF(x) typeof(x)
#   elif defined(__GNUC__) || defined(__clang__) // GCC and Clang provide __typeof__
#       define AFX_TYPEOF(x) __typeof__(x)
#   elif defined(_MSC_VER) // MSVC (C mode) provides __typeof (note: no trailing underscores)
#       define AFX_TYPEOF(x) __typeof(x)
#   else
#       error "Compiler does not support typeof or any known extension"
#   endif
#endif

/*
    Safe version (avoids VLA evaluation or side effects).
    Some compilers evaluate typeof(expr) multiple times.
    The AFX_TYPEOF_SAFE version forces the expression to be evaluated just once.
    The (1 ? expr : expr) trick ensures that expr is never evaluated twice.
    Ex.:
    {
        TYPEOF_SAFE(a + b) tmp = a + b;
    }
*/

#ifndef AFX_TYPEOF_SAFE
#   define AFX_TYPEOF_SAFE(expr) AFX_TYPEOF( (1 ? (expr) : (expr)) )
#endif

/*
    Standard-C23–compatible unqualified type: TYPEOF_UNQUAL.
    C23 introduces typeof_unqual.
    We can emulate it on older compilers.

    Why does +(x) strip qualifiers?
    Because applying unary + forces the operand to be treated as a value, not an lvalue, so the resulting type is unqualified.
*/

#ifndef AFX_TYPEOF_UNQUAL
#   if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#       define AFX_TYPEOF_UNQUAL(x) typeof_unqual(x)
#   elif defined(__GNUC__) || defined(__clang__)
// Remove qualifiers using typeof applied to assignment
#       define AFX_TYPEOF_UNQUAL(x) __typeof__(+ (x))
#   elif defined(_MSC_VER)
// MSVC __typeof also drops qualifiers with unary + trick
#       define AFX_TYPEOF_UNQUAL(x) __typeof(+ (x))
#   else
#       error "typeof_unqual emulation not supported"
#   endif
#endif

/*
    Re-apply qualifiers to a type: QUALIFY(dst, src).
    This emulates the qualifier-propagation behavior of decltype(auto) or C++ templates.
    propagate const/volatile/tags
    Ex.:
    {
        const int a = 3;
        QUALIFY(a, int) x = a; // x becomes const int
    }
*/

#ifndef QUALIFY
#   define QUALIFY(src, dst) AFX_TYPEOF( *(AFX_TYPEOF(src) *) (dst) )
#endif//QUALIFY

/*
    A real-world use case: portable container_of() macro.
    Here is a Linux-kernel style container_of that uses Qwadro's AFX_TYPEOF.

    #ifndef container_of
    #   define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
    #endif
*/

#ifndef container_of_safe
#   define container_of_safe(ptr, member) ((AFX_TYPEOF(ptr))((char *)(ptr) - offsetof(AFX_TYPEOF(*ptr), member)))
#endif//container_of_safe

////////////////////////////////////////////////////////////////////////////////

typedef int8_t          afxInt8;
typedef int16_t         afxInt16;
typedef int32_t         afxInt32;
typedef int64_t         afxInt64;

typedef uint8_t         afxUnit8;
typedef uint16_t        afxUnit16;
typedef uint32_t        afxUnit32;
typedef uint64_t        afxUnit64;

typedef afxUnit16       afxReal16;
typedef float           afxReal32;
typedef double          afxReal64;

typedef char            afxChar8;
typedef afxInt16        afxChar16;
typedef afxInt32        afxChar32;

typedef afxChar8        afxChar;
typedef afxUnit8        afxByte;
typedef afxInt32        afxInt; // (signed) integral number unit
typedef afxUnit32       afxUnit; // (unsigned) natural number unit
typedef afxReal32       afxReal; // (floating-point) real number unit

typedef uintptr_t       afxAddress;
typedef size_t          afxSize;
typedef sig_atomic_t    afxAtomic;
//typedef afxInt64    afxOffset;
//typedef afxUnit64    afxAddress;
//typedef afxUnit64    afxSize;

////////////////////////////////////////////////////////////////////////////////

#define AFX_BYTE_SIZE 8
#define AFX_CHAR_SIZE 8

#ifdef INT8_MIN
#   define AFX_I8_MIN  INT8_MIN
#else
#   define AFX_I8_MIN  (-127i8 - 1)
#endif

#ifdef INT8_MAX
#   define AFX_I8_MAX  INT8_MAX
#else
#   define AFX_I8_MAX  127i8
#endif

#ifdef UINT8_MAX
#   define AFX_U8_MAX  UINT8_MAX
#else
#   define AFX_U8_MAX  0xffui8
#endif

#ifdef INT16_MIN
#   define AFX_I16_MIN INT16_MIN)
#else
#   define AFX_I16_MIN (-32767i16 - 1)
#endif

#ifdef INT16_MAX
#   define AFX_I16_MAX INT16_MAX
#else
#   define AFX_I16_MAX 32767i16
#endif

#ifdef UINT16_MAX
#   define AFX_U16_MAX UINT16_MAX
#else
#   define AFX_U16_MAX 0xffffui16
#endif

#ifdef INT32_MIN
#   define AFX_I32_MIN INT32_MIN
#else
#   define AFX_I32_MIN (-2147483647i32 - 1)
#endif

#ifdef INT32_MAX
#   define AFX_I32_MAX INT32_MAX
#else
#   define AFX_I32_MAX 2147483647i32
#endif

#ifdef UINT32_MAX
#   define AFX_U32_MAX UINT32_MAX
#else
#   define AFX_U32_MAX 0xffffffffui32
#endif

#ifdef INT64_MIN
#   define AFX_I64_MIN INT64_MIN
#else
#   define AFX_I64_MIN (-9223372036854775807i64 - 1)
#endif

#ifdef INT64_MAX
#   define AFX_I64_MAX INT64_MAX
#else
#   define AFX_I64_MAX 9223372036854775807i64
#endif

#ifdef UINT64_MAX
#   define AFX_U64_MAX UINT64_MAX
#else
#   define AFX_U64_MAX 0xffffffffffffffffui64
#endif


#ifdef FLT_MIN
#   define AFX_R32_MIN FLT_MIN
#else
#   define AFX_R32_MIN 1.175494351e-38F
#endif
#ifdef FLT_MAX
#   define AFX_R32_MAX FLT_MAX
#else
#   define AFX_R32_MAX 3.402823466e+38F
#endif

#ifdef DBL_MIN
#   define AFX_R64_MIN DBL_MIN
#else
#   define AFX_R64_MIN 2.2250738585072014e-308
#endif
#ifdef DBL_MAX
#   define AFX_R64_MAX DBL_MAX
#else
#   define AFX_R64_MAX 1.7976931348623158e+308
#endif


#ifdef SIG_ATOMIC_MIN
#   define AFX_ATOMIC_MIN SIG_ATOMIC_MIN
#else
#   define AFX_ATOMIC_MIN AFX_I32_MIN
#endif
#ifdef SIG_ATOMIC_MAX
#   define AFX_ATOMIC_MAX SIG_ATOMIC_MAX
#else
#   define AFX_ATOMIC_MAX AFX_I32_MAX
#endif

// This sets up how many XMM fill registers we'll use.
#ifndef AFX_MAX_SIMD_REGISTERS
#   if defined(__x86_64__) || defined(_M_X64)
#       define AFX_MAX_SIMD_REGISTERS (16)
#   else
#       define AFX_MAX_SIMD_REGISTERS (8)
#   endif
#endif//AFX_MAX_SIMD_REGISTERS

#endif//AFX_PLATFORM_DEFS_H
