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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_PLATFORM_DEFS_H
#define AFX_PLATFORM_DEFS_H

#include <stdarg.h>
#include <signal.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <intrin.h>

#ifdef _MSC_VER
//#   pragma warning (disable : 4054) // 'type cast': from function pointer '' to data pointer ''
#   pragma warning (disable : 4152) // nonstandard extension, function/data pointer conversion in expression
#   pragma warning (disable : 4324) // '': structure was padded due to alignment specifier
#   pragma warning (disable : 4204) // nonstandard extension used : non-constant aggregate initializer
#   pragma warning (disable : 4201) // nonstandard extension used: nameless struct/union
#   pragma warning (disable : 4706) // assignment within conditional expression
#endif

#ifdef _WIN64
#   define AFX_PLATFORM_64 1
#   define AFX_PLATFORM_X86_64 1
#   define AFX_PLATFORM_WIN 1
#   define AFX_PLATFORM_W64 1
#elif _WIN32
#   define AFX_PLATFORM_32 1
#   define AFX_PLATFORM_X86_32 1
#   define AFX_PLATFORM_WIN 1
#   define AFX_PLATFORM_W32 1
#else
#   define AFX_PLATFORM_64 1
#   define AFX_PLATFORM_X86_64 1
#   define AFX_PLATFORM_LINUX 1 
#   define AFX_PLATFORM_X64 1
#endif//_WIN64

#if !defined(AFX_ENDIANNESS_LITTLE) && !defined(AFX_ENDIANNESS_BIG)
#   define AFX_ENDIANNESS_LITTLE
#endif

#define _AFXIMPORT __declspec(dllimport)
#define _AFXEXPORT __declspec(dllexport)
#define _AFXINLINE __forceinline
#define AFXCALL __cdecl


#ifndef __e2coree__
#   ifdef _DEBUG
#       define AFX _AFXIMPORT extern 
#       define AFXINL _AFXIMPORT extern inline
#   else
#       define AFX _AFXIMPORT extern 
#       define AFXINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _AFX _AFXEXPORT
#       define AFX _AFXEXPORT extern 
#       define _AFXINL _AFXEXPORT inline
#       define AFXINL _AFXEXPORT extern inline
#   else
#       define _AFX _AFXEXPORT
#       define AFX _AFXEXPORT extern 
#       define _AFXINL _AFXEXPORT inline
#       define AFXINL _AFXEXPORT extern inline
#   endif
#endif//__e2coree__

////////////////////////////////////////////////////////////////////////////////

#define afxSimd(x) __declspec(align(AFX_SIMD_ALIGN)) x

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
typedef afxNat32    afxMask;


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

#define AFX_SIMD_ALIGN 16u

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
