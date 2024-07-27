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

#ifndef AFX_PLATFORM_DEFS_H
#define AFX_PLATFORM_DEFS_H

 //#ifdef __clang__
#ifdef _AFX_CORE_C
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
// macro name is a reserved identifier
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
//#pragma clang diagnostic pop
#endif

#if !0
#ifdef _MSC_VER
 //#   pragma warning (disable : 4054) // 'type cast': from function pointer '' to data pointer ''
#   pragma warning (disable : 4152) // nonstandard extension, function/data pointer conversion in expression
#   pragma warning (disable : 4324) // '': structure was padded due to alignment specifier
#   pragma warning (disable : 4204) // nonstandard extension used : non-constant aggregate initializer
#   pragma warning (disable : 4201) // nonstandard extension used: nameless struct/union
//#   pragma warning (disable : 4706) // assignment within conditional expression
#   pragma warning (disable : 4820) // '' bytes padding added after data member ''
#endif
#endif

#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <intrin.h>

#ifdef _DEBUG // after afxDebug!!!
#   define VLD_FORCE_ENABLE
#endif

#if !0
#if (defined(_WIN64) || defined(_WIN32))
#   ifdef VLD_FORCE_ENABLE
#       include <vld.h>
#   endif
#endif
#endif

#ifdef _WIN64
#   define AFX_ISA_X86_64 1
#   define AFX_OS_WIN 1
#elif _WIN32
#   define AFX_ISA_X86_32 1
#   define AFX_OS_WIN 1
#else
#   define AFX_ISA_X86_64 1
#   define AFX_OS_LNX 1
#endif

#if (defined(AFX_ISA_X86_64)||defined(AFX_ISA_X86_32))
#   define AFX_ISA_X86 1
#endif

#ifdef AFX_OS_WIN
#   if (defined(AFX_ISA_X86_64))
#       define AFX_OS_WIN64 1
#   elif defined(AFX_ISA_X86_32)
#       define AFX_OS_WIN32 1
#   else
#       error "Unsupported ISA"
#   endif
#elif AFX_OS_LNX
#   if (defined(AFX_ISA_X86_64))
#       define AFX_OS_LNX64 1
#   elif defined(AFX_ISA_X86_32)
#       define AFX_OS_LNX32 1
#   else
#       error "Unsupported ISA"
#   endif
#else
#   error "Unsupported OS"
#endif

#ifdef AFX_ISA_X86
#   define AFX_LE
#else
#   error "Unsupported endianess"
#endif

#ifdef AFX_ISA_X86_64
#   define AFX_ISA_SSE
#   define AFX_ISA_SSE2
#endif

#define AFXCALL __cdecl

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#if defined(_WIN32) 
#   define INLINE __forceinline
#else
#   define INLINE inline __attribute__((always_inline))
#endif

#define EMBED extern INLINE

////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_THREAD_LOCAL
#   ifndef _Thread_local
#       if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201102L)) && !defined(_Thread_local)
#           if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#               define _Thread_local __thread
#           else
#               define _Thread_local __declspec(thread)
#           endif
#       elif defined(__GNUC__) && defined(__GNUC_MINOR__) && (((__GNUC__ << 8) | __GNUC_MINOR__) < ((4 << 8) | 9))
#           define _Thread_local __thread
#       endif
#   endif
#   define AFX_THREAD_LOCAL _Thread_local
#endif

#ifndef AFX_STATIC_ASSERT
#   ifndef _Static_assert
#       define AFX_STATIC_ASSERT(_expression_, _message_) static_assert(_expression_, _message_)
#       define _Static_assert(_expression_, _message_) static_assert(_expression_, _message_)
#   else
#       define AFX_STATIC_ASSERT(_expression_, _message_) _Static_assert(_expression_, _message_)
#   endif
#endif

#define AFX_SIMD_ALIGN 16u

#if defined(_WIN32) && !defined(__MINGW32__)
#  define AFX_ALIGN(...) __declspec(align(__VA_ARGS__))
#else
#  define AFX_ALIGN(...) __attribute__((aligned(__VA_ARGS__)))
#endif

typedef int8_t          afxInt8;
typedef int16_t         afxInt16;
typedef int32_t         afxInt32;
typedef int64_t         afxInt64;

typedef uint8_t         afxNat8;
typedef uint16_t        afxNat16;
typedef uint32_t        afxNat32;
typedef uint64_t        afxNat64;

typedef afxNat16        afxReal16;
typedef float           afxReal32;
typedef double          afxReal64;

typedef afxInt8         afxBool8;
typedef afxInt16        afxBool16;
typedef afxInt32        afxBool32;

typedef char            afxChar8;
typedef afxInt16        afxChar16;
typedef afxInt32        afxChar32;

typedef afxChar8        afxChar;
typedef afxNat8         afxByte;
typedef afxInt32        afxInt; // (signed) integral number unit
typedef afxNat32        afxNat; // (unsigned) natural number unit
typedef afxReal32       afxReal; // (floating-point) real number unit
typedef afxBool32       afxBool;
typedef afxInt32        afxResult;

typedef intptr_t        afxOffset;
typedef uintptr_t       afxAddress;
typedef size_t          afxSize;
typedef sig_atomic_t    afxAtomic;
//typedef afxInt64    afxOffset;
//typedef afxNat64    afxAddress;
//typedef afxNat64    afxSize;

typedef afxNat32    afxFlags;
typedef afxNat32    afxMask32;
typedef afxNat64    afxMask64;
typedef afxMask32   afxMask;

typedef afxNat8     afxIndex8;
typedef afxNat16    afxIndex16;
typedef afxNat32    afxIndex32;
typedef afxIndex32  afxIndex;

typedef afxInt8     afxI8;
typedef afxInt16    afxI16;
typedef afxInt32    afxI32;
typedef afxInt64    afxI64;

typedef afxNat8     afxN8; 
typedef afxNat16    afxN16; 
typedef afxNat32    afxN32; 
typedef afxNat64    afxN64;

typedef afxReal16   afxR16;
typedef afxReal32   afxR32;
typedef afxReal64   afxR64;

typedef afxBool8    afxB8;
typedef afxBool16   afxB16;
typedef afxBool32   afxB32;

typedef afxChar8    afxC8;
typedef afxChar16   afxC16;
typedef afxChar32   afxC32;

////////////////////////////////////////////////////////////////////////////////

#define AFX_BYTE_SIZE 8
#define AFX_CHAR_SIZE 8

#define AFX_I8_MIN  ((afxInt8)INT8_MIN)
#define AFX_I8_MAX  ((afxInt8)INT8_MAX)
#define AFX_N8_MAX  ((afxNat8)UINT8_MAX)

#define AFX_I16_MIN ((afxInt16)INT16_MIN)
#define AFX_I16_MAX ((afxInt16)INT16_MAX)
#define AFX_N16_MAX ((afxNat16)UINT16_MAX)

#define AFX_I32_MIN ((afxInt32)INT32_MIN)
#define AFX_I32_MAX ((afxInt32)INT32_MAX)
#define AFX_N32_MAX ((afxNat32)UINT32_MAX)

#define AFX_I64_MIN ((afxInt64)INT64_MIN)
#define AFX_I64_MAX ((afxInt64)INT64_MAX)
#define AFX_N64_MAX ((afxNat64)UINT64_MAX)

#define AFX_R32_MIN ((afxReal32)FLT_MIN)
#define AFX_R32_MAX ((afxReal32)FLT_MAX)
#define AFX_R64_MIN ((afxReal64)DBL_MIN)
#define AFX_R64_MAX ((afxReal64)DBL_MAX)


#define AFX_ATOMIC_MIN SIG_ATOMIC_MIN
#define AFX_ATOMIC_MAX SIG_ATOMIC_MAX


#endif//AFX_PLATFORM_DEFS_H
