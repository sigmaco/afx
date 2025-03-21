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

typedef afxInt8         afxBool8;
typedef afxInt16        afxBool16;
typedef afxInt32        afxBool32;

typedef char            afxChar8;
typedef afxInt16        afxChar16;
typedef afxInt32        afxChar32;

typedef afxChar8        afxChar;
typedef afxUnit8        afxByte;
typedef afxInt32        afxInt; // (signed) integral number unit
typedef afxUnit32       afxUnit; // (unsigned) natural number unit
typedef afxReal32       afxReal; // (floating-point) real number unit
typedef afxBool32       afxBool;
typedef afxInt32        afxResult;

typedef uintptr_t       afxAddress;
typedef size_t          afxSize;
typedef sig_atomic_t    afxAtomic;
//typedef afxInt64    afxOffset;
//typedef afxUnit64    afxAddress;
//typedef afxUnit64    afxSize;

typedef afxUnit32   afxFlags;
typedef afxUnit32   afxMask32;
typedef afxUnit64   afxMask64;
typedef afxMask32   afxMask;

typedef afxUnit8    afxIndex8;
typedef afxUnit16   afxIndex16;
typedef afxUnit32   afxIndex32;
typedef afxIndex32  afxIndex;

typedef afxInt8     afxI8;
typedef afxInt16    afxI16;
typedef afxInt32    afxI32;
typedef afxInt64    afxI64;

typedef afxUnit8    afxN8; 
typedef afxUnit16   afxN16; 
typedef afxUnit32   afxN32; 
typedef afxUnit64   afxN64;

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
#define AFX_U8_MAX  ((afxUnit8)UINT8_MAX)

#define AFX_I16_MIN ((afxInt16)INT16_MIN)
#define AFX_I16_MAX ((afxInt16)INT16_MAX)
#define AFX_U16_MAX ((afxUnit16)UINT16_MAX)

#define AFX_I32_MIN ((afxInt32)INT32_MIN)
#define AFX_I32_MAX ((afxInt32)INT32_MAX)
#define AFX_U32_MAX ((afxUnit32)UINT32_MAX)

#define AFX_I64_MIN ((afxInt64)INT64_MIN)
#define AFX_I64_MAX ((afxInt64)INT64_MAX)
#define AFX_N64_MAX ((afxUnit64)UINT64_MAX)

#define AFX_R32_MIN ((afxReal32)FLT_MIN)
#define AFX_R32_MAX ((afxReal32)FLT_MAX)
#define AFX_R64_MIN ((afxReal64)DBL_MIN)
#define AFX_R64_MAX ((afxReal64)DBL_MAX)


#define AFX_ATOMIC_MIN SIG_ATOMIC_MIN
#define AFX_ATOMIC_MAX SIG_ATOMIC_MAX

#endif//AFX_PLATFORM_DEFS_H
