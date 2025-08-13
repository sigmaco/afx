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

 ///////////////////////////////////////////////////////////////////////////////
 // PLATFORM HANDLING                                                         //
 ///////////////////////////////////////////////////////////////////////////////

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
#endif

#if !0
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

#if defined(_WIN32) 
#   define INLINE __forceinline
#else
#   define INLINE inline __attribute__((always_inline))
#endif

#define EMBED extern INLINE

#define AFXCALL __cdecl

#if defined(_WIN32) && !defined(__MINGW32__)
#  define AFX_ALIGN(...) __declspec(align(__VA_ARGS__))
#else
#  define AFX_ALIGN(...) __attribute__((aligned(__VA_ARGS__)))
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

#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201102L)) && !defined(_Thread_local)
 #if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
  #define _Thread_local __thread
 #else
  #define _Thread_local __declspec(thread)
 #endif
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && (((__GNUC__ << 8) | __GNUC_MINOR__) < ((4 << 8) | 9))
 #define _Thread_local __thread
#endif
#define AFX_TLS _Thread_local 
#define AFX_THREAD_LOCAL AFX_TLS 

#include <assert.h>

#ifndef _Static_assert
#   define _Static_assert(_expression_, _message_) static_assert(_expression_, _message_)
#endif
#define AFX_STATIC_ASSERT(_expression_, _message_) _Static_assert(_expression_, _message_) 

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

////////////////////////////////////////////////////////////////////////////////


#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <intrin.h>

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
#   define AFX_I8_MIN  ((afxInt8)INT8_MIN)
#else
#   define AFX_I8_MIN  ((afxInt8)(-127i8 - 1))
#endif

#ifdef INT8_MAX
#   define AFX_I8_MAX  ((afxInt8)INT8_MAX)
#else
#   define AFX_I8_MAX  ((afxInt8)127i8)
#endif

#ifdef UINT8_MAX
#   define AFX_U8_MAX  ((afxUnit8)UINT8_MAX)
#else
#   define AFX_U8_MAX  ((afxUnit8)0xffui8)
#endif

#ifdef INT16_MIN
#   define AFX_I16_MIN ((afxInt16)INT16_MIN)
#else
#   define AFX_I16_MIN ((afxInt16)(-32767i16 - 1))
#endif

#ifdef INT16_MAX
#   define AFX_I16_MAX ((afxInt16)INT16_MAX)
#else
#   define AFX_I16_MAX ((afxInt16)32767i16)
#endif

#ifdef UINT16_MAX
#   define AFX_U16_MAX ((afxUnit16)UINT16_MAX)
#else
#   define AFX_U16_MAX ((afxUnit16)0xffffui16)
#endif

#ifdef INT32_MIN
#   define AFX_I32_MIN ((afxInt32)INT32_MIN)
#else
#   define AFX_I32_MIN ((afxInt32)(-2147483647i32 - 1))
#endif

#ifdef INT32_MAX
#   define AFX_I32_MAX ((afxInt32)INT32_MAX)
#else
#   define AFX_I32_MAX ((afxInt32)2147483647i32)
#endif

#ifdef UINT32_MAX
#   define AFX_U32_MAX ((afxUnit32)UINT32_MAX)
#else
#   define AFX_U32_MAX ((afxUnit32)0xffffffffui32)
#endif

#ifdef INT64_MIN
#   define AFX_I64_MIN ((afxInt64)INT64_MIN)
#else
#   define AFX_I64_MIN ((afxInt64)(-9223372036854775807i64 - 1))
#endif

#ifdef INT64_MAX
#   define AFX_I64_MAX ((afxInt64)INT64_MAX)
#else
#   define AFX_I64_MAX ((afxInt64)9223372036854775807i64)
#endif

#ifdef UINT64_MAX
#   define AFX_U64_MAX ((afxUnit64)UINT64_MAX)
#else
#   define AFX_U64_MAX ((afxUnit64)0xffffffffffffffffui64)
#endif


#ifdef FLT_MIN
#   define AFX_R32_MIN ((afxReal32)FLT_MIN)
#else
#   define AFX_R32_MIN ((afxReal32)1.175494351e-38F)
#endif
#ifdef FLT_MAX
#   define AFX_R32_MAX ((afxReal32)FLT_MAX)
#else
#   define AFX_R32_MAX ((afxReal32)3.402823466e+38F)
#endif

#ifdef DBL_MIN
#   define AFX_R64_MIN ((afxReal64)DBL_MIN)
#else
#   define AFX_R64_MIN ((afxReal64)2.2250738585072014e-308)
#endif
#ifdef DBL_MAX
#   define AFX_R64_MAX ((afxReal64)DBL_MAX)
#else
#   define AFX_R64_MAX ((afxReal64)1.7976931348623158e+308)
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
#if defined(__x86_64__) || defined(_M_X64)
#define AFX_MAX_SIMD_REGISTERS (16)
#else
#define AFX_MAX_SIMD_REGISTERS (8)
#endif

#endif//AFX_PLATFORM_DEFS_H
