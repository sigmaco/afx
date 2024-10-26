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

// IMPORTANT: O módulo de matemática do Qwadro é matemática para programadores, não para matemáticos. 
// Nossos ângulos e suas contrapartes são representadas em "radians", não em "degrees". Funções para degrees são apenas utilitários de conveniência em conversão.
// Nossas rotações e orientações são representadas primariamente por "quaternion". Funções para axial rotation e Euler são apenas utilitários.
// É totalmente vedado adulterar os princípios gigachads e heterotops do Qwadro com essas tcholices dessas game engines randômicas do mundo por responder a vontade das pessoas de querer serem eternos subcolocados..

// IMPORTANT: This library WAS NOT made by a math guy. Most things here WERE NOT tested and there IS NOT any guarantee to me accurate.

// IMPORTANT: Qwadro uses their OWN matrix layout, which is a combined form of 3x3 matrix, 1x3 matrix and 4x1 matrix based.

// IMPORTANT: Qwadro is right handed.
// Left vs right-handed coordinate system.
// In a left-handed: the positive X, Y and Z axes point right, up and far/forward, respectively. Positive rotation is clockwise about the axis of rotation.
// In a right-handed: the positive X, Y and Z axes point right, up and near/backward (negative Z axis points far/forward), respectively. Positive rotation is counterclockwise about the axis of rotation.

// No Qwadro, usamos, como estilo de programação, muitos parênteses irrelevantes. 
// Eles existem para fins didáticos mesmo, facilitando a vida de pessoas que não conhecem o princípio de precedência de PEMDAS.

#include <math.h>
#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/math/afxSimd.h"
//#include <stdalign.h>

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RW_MATH // use RenderWare over Qwadro conventions
//#define MFX_ALIGN_ALL // force SIMD alignment to afxV3d and other types.

#define AfxScalar(x_)   (x_)

#ifndef AFX_PI
#   define AFX_PI 3.14159265358979323846
#endif
#ifndef AFX_EPSILON
#   define AFX_EPSILON DBL_EPSILON
#endif

#define AFX_PI_OVER2    (AFX_PI / 2.0)

#ifndef MFX_ALIGN_ALL
typedef afxReal afxV2d[2];
typedef afxReal afxV4d[4];
typedef afxV2d  afxM2d[2]; // 2D Linear Transform (2x2 Matrix)
typedef afxV4d  afxM4d[4];
#else
typedef afxReal AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxV2d[2];
typedef afxReal AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxV4d[4];
typedef afxV2d  AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxM2d[2]; // 2D Linear Transform (2x2 Matrix)
typedef afxV4d  AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxM4d[4];
#endif

typedef afxV4d  afxQuat; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  afxRotor; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  afxVector;
typedef afxV4d  afxPoint;
typedef afxM4d  afxMatrix;

#ifndef MFX_ALIGN_ALL
typedef afxReal afxV3d[3];
typedef afxV3d  afxM3d[3]; // 3D Linear Transform (3x3 Matrix)
typedef afxV3d  afxAtm3d[4]; // non-aligned affine space.
typedef afxV3d  afxAtm3d[4];
#else
typedef afxReal AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxV3d[3];
typedef afxV3d  AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxM3d[3]; // 3D Linear Transform (3x3 Matrix)
typedef afxV3d  AFX_ALIGN(AFX_SIMD_ALIGNMENT) afxAtm3d[4]; // non-aligned affine space.
#endif

//typedef  afxUnit afxWhd[3];

#if 0
// always aligned
AFX_STATIC_ASSERT(__alignof(afxV4d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxQuat) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxRotor) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxM4d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxMatrix) == AFX_SIMD_ALIGNMENT, "");
#endif
#ifdef MFX_ALIGN_ALL
AFX_STATIC_ASSERT(__alignof(afxV2d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxV3d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxM3d) == AFX_SIMD_ALIGNMENT, "");
#endif

AFXINL void         AfxNdcV2d(afxV2d v, afxV2d const b, afxV2d const total);
AFXINL void         AfxUnndcV2d(afxV2d v, afxV2d const b, afxV2d const total);

#endif//AFX_MATH_DEFS_H
