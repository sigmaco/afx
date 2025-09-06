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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_MATH_DEFS_H
#define AFX_MATH_DEFS_H

// IMPORTANT: O m�dulo de matem�tica do Qwadro � matem�tica para programadores, n�o para matem�ticos. 
// Nossos �ngulos e suas contrapartes s�o representadas em "radians", n�o em "degrees". Fun��es para degrees s�o apenas utilit�rios de conveni�ncia em convers�o.
// Nossas rota��es e orienta��es s�o representadas primariamente por "quaternion". Fun��es para axial rotation e Euler s�o apenas utilit�rios.
// � totalmente vedado adulterar os princ�pios gigachads e heterotops do Qwadro com essas tcholices dessas game engines rand�micas do mundo por responder a vontade das pessoas de querer serem eternos subcolocados..

// IMPORTANT: This library WAS NOT made by a math guy. Most things here WERE NOT tested and there IS NOT any guarantee to me accurate.

// IMPORTANT: Qwadro uses their OWN matrix layout, which is a combined form of 3x3 matrix, 1x3 matrix and 4x1 matrix based.

// IMPORTANT: Qwadro is right handed.
// Left vs right-handed coordinate system.
// In a left-handed: the positive X, Y and Z axes point right, up and far/forward, respectively. Positive rotation is clockwise about the axis of rotation.
// In a right-handed: the positive X, Y and Z axes point right, up and near/backward (negative Z axis points far/forward), respectively. Positive rotation is counterclockwise about the axis of rotation.

// No Qwadro, usamos, como estilo de programa��o, muitos par�nteses irrelevantes. 
// Eles existem para fins did�ticos mesmo, facilitando a vida de pessoas que n�o conhecem o princ�pio de preced�ncia de PEMDAS.

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

#include <math.h>
#include "qwadro/base/afxDebug.h"
#include "qwadro/math/afxSimd.h"
//#include <stdalign.h>

#ifdef AFX_ON_WINDOWS
/*
    __vectorcall is a Microsoft-specific calling convention introduced to pass vector types via registers,
    not the stack, improve performance for functions involving SIMD types, reduce overhead of copying 128-bit/256-bit vectors.
    It's part of MSVC's ABI, and also supported by Clang when targeting Windows (x86/x64) platforms.
*/
#   define AFXCALLV __vectorcall
#else
#   define AFXCALLV 
#endif

#define AFX_R(x_)   (x_)

#ifndef AFX_PI
#   define AFX_PI (3.14159265358979323846)
#endif
#ifndef AFX_EPSILON32
#   ifdef FLT_EPSILON
#       define AFX_EPSILON32 FLT_EPSILON
#   else
#       define AFX_EPSILON32 (1.192092896e-07f) // smallest such that ((1.0 + AFX_EPSILON32) != 1.0)
#   endif
#endif
#ifndef AFX_EPSILON64
#   ifdef DBL_EPSILON
#       define AFX_EPSILON64 DBL_EPSILON
#   else
#       define AFX_EPSILON64 (2.2204460492503131e-016) // smallest such that ((1.0 + AFX_EPSILON64) != 1.0)
#   endif
#endif
#ifndef AFX_EPSILON
#   define AFX_EPSILON AFX_EPSILON32
#endif

#define AFX_PI_OVER2        (AFX_PI / 2.0) // 0.0174532925

// The value of (PI / 180) is often used in converting degrees to radians, as there are "Pi" radians in 180 degrees.
#define AFX_PI_OVER_180     (AFX_PI / 180.0)
#define AFX_PI32_OVER_180   (0.0174532925f)
#define AFX_PI64_OVER_180   (0.017453292519943295)
// The value of (180 / PI) is commonly used for converting radians to degrees.
#define AFX_180_OVER_PI     (180.0 / AFX_PI)
#define AFX_180_OVER_PI32   (57.29578f)
#define AFX_180_OVER_PI64   (57.295779513082320)

// Converts degrees to radians.
 // The value returned by this function is (pi / 180) * degrees.
#define AFX_RADIANS(degrees_) ((degrees_) * AFX_PI_OVER_180)

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.
#define AFX_DEGREES(radians_) ((radians_) * AFX_180_OVER_PI)

#ifndef MFX_ALIGN_ALL
typedef afxReal afxV2d[2];
typedef afxReal afxV3d[3];
typedef afxV3d  afxM3d[3]; // 3D Linear Transform (3x3 Matrix)
typedef afxV3d  afxM4r[4]; // A 4x3 matrix has 4 rows and 3 columns.
#else
typedef afxReal AFX_SIMD afxV2d[2];
typedef afxReal AFX_SIMD afxV3d[3];
typedef afxV3d  AFX_SIMD afxM3d[3]; // 3D Linear Transform (3x3 Matrix)
typedef afxV3d  AFX_SIMD afxM4r[4]; // A 4x3 matrix has 4 rows and 3 columns.
#endif

typedef afxReal AFX_SIMD afxV4d[4];
typedef afxV2d  AFX_SIMD afxM2d[2]; // 2D Linear Transform (2x2 Matrix)
typedef afxV4d  AFX_SIMD afxM4d[4]; // 4x4

typedef afxV4d  AFX_SIMD afxM4c[3]; // A 3x4 matrix has 3 rows and 4 columns.

typedef afxV4d  AFX_SIMD afxQuat; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  AFX_SIMD afxRotor; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  AFX_SIMD afxVector;
typedef afxV4d  AFX_SIMD afxPoint;
typedef afxM4d  AFX_SIMD afxMatrix;

//typedef  afxUnit afxWarp[3];

#ifdef MFX_ALIGN_ALL
AFX_STATIC_ASSERT(__alignof(afxV2d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxV3d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxM3d) == AFX_SIMD_ALIGNMENT, "");
#endif
// always aligned
AFX_STATIC_ASSERT(__alignof(afxM4d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxV4d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxQuat) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxRotor) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxMatrix) == AFX_SIMD_ALIGNMENT, "");

#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RW_MATH // use RenderWare over Qwadro conventions
//#define MFX_ALIGN_ALL // force SIMD alignment to afxV3d and other types.

AFXINL void         AfxV2dNdc(afxV2d v, afxV2d const b, afxV2d const total);
AFXINL void         AfxV2dUnndc(afxV2d v, afxV2d const b, afxV2d const total);

#endif//AFX_MATH_DEFS_H
