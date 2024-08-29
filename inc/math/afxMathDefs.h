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

#ifndef AFX_MATH_DEFS_H
#define AFX_MATH_DEFS_H

#include <math.h>
#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/math/afxSimd.h"
//#include <stdalign.h>

//#define MFX_USE_RWMATH

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RWMATH

#define AfxScalar(x_)   (x_)

#define AFX_PI          3.14159265358979323846
#define AFX_PI_OVER2    (AFX_PI / 2.0)
#define AFX_EPSILON     DBL_EPSILON

//#define MFX_ALIGN_ALL

#ifdef MFX_ALIGN_ALL

typedef afxReal AFX_SIMD afxV2d[2];
typedef afxReal AFX_SIMD afxV3d[3];
typedef afxReal AFX_SIMD afxV4d[4];

typedef afxV2d  AFX_SIMD afxM2d[2]; // 2D Linear Transform (2x2 Matrix)
typedef afxV3d  AFX_SIMD afxM3d[3]; // 3D Linear Transform (3x3 Matrix)
typedef afxV4d  AFX_SIMD afxM4d[4];
//typedef afxV4d  AFX_SIMD afxM4d3[3];
typedef afxV3d  AFX_SIMD afxM4dc[4]; // non-aligned affine space.

typedef afxV4d  AFX_SIMD afxQuat; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  AFX_SIMD afxRotor; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  AFX_SIMD afxVector;
typedef afxV4d  AFX_SIMD afxPoint;
typedef afxM4d  AFX_SIMD afxMatrix;

#if !0
AFX_STATIC_ASSERT(__alignof(afxV2d) == AFX_SIMD_ALIGN, "");
AFX_STATIC_ASSERT(__alignof(afxV3d) == AFX_SIMD_ALIGN, "");
AFX_STATIC_ASSERT(__alignof(afxV4d) == AFX_SIMD_ALIGN, "");
AFX_STATIC_ASSERT(__alignof(afxQuat) == AFX_SIMD_ALIGN, "");
AFX_STATIC_ASSERT(__alignof(afxM3d) == AFX_SIMD_ALIGN, "");
AFX_STATIC_ASSERT(__alignof(afxM4d) == AFX_SIMD_ALIGN, "");
AFX_STATIC_ASSERT(sizeof(__m128) == sizeof(afxV4d), "");
#endif

#else

typedef  afxReal afxV2d[2];
typedef  afxReal afxV3d[3];
typedef  afxReal afxV4d[4];

typedef  afxV2d  afxM2d[2]; // 2D Linear Transform (2x2 Matrix)
typedef  afxV3d  afxM3d[3]; // 3D Linear Transform (3x3 Matrix)
typedef  afxV4d  afxM4d[4];
//typedef afxV4d  afxM4d3[3];
typedef  afxV3d  afxM4dc[4]; // non-aligned affine space.

typedef  afxV4d  afxQuat; // 0,1,2 = imaginary, 3 = real
typedef  afxV4d  afxRotor; // 0,1,2 = imaginary, 3 = real
typedef  afxV4d  afxVector;
typedef  afxV4d  afxPoint;
typedef  afxM4d  afxMatrix;

#endif

typedef  afxNat afxWhd[3];
typedef  afxReal afxNdc[3];

typedef  afxV4d afxBox[2];
typedef  afxBox afxAabb; // [ sup/max, inf/min ]
typedef  afxBox afxObb;

typedef  afxV4d afxPlane;

AFX_DEFINE_STRUCT_SIMD(afxSpace)
/// A 3D coordinate system, expressed relative to a canonical coordinate system.
{
    afxV3d  right; /// Unit vector pointing to the right (local +x axis).
    afxV3d  up; /// Unit vector pointing upwards (local +y axis).
    afxV3d  ahead; /// Unit vector pointing forwards (local -z axis).
    afxV3d  origin; /// The origin, relative to the canonical coordinate system.
};

/// A spherical coordinate system is a coordinate system for three-dimensional space where the position of a point is specified by three numbers: 
///     the radial distance of that point from a fixed origin; 
///     its polar angle measured from a fixed polar axis or zenith direction; 
///     and the azimuthal angle of its orthogonal projection on a reference plane that passes through the origin and is orthogonal to the fixed axis, measured from another fixed reference direction on that plane. 
/// When radius is fixed, the two angular coordinates make a coordinate system on the sphere sometimes called spherical polar coordinates.

/// The radial distance is also called the radius or radial coordinate. The polar angle may be called colatitude, zenith angle, normal angle, or inclination angle. 
/// The polar angle is often replaced by the elevation angle measured from the reference plane towards the positive Z axis; the depression angle is the negative of the elevation angle.

/// According to the conventions of geographical coordinate systems, positions are measured by latitude, longitude, and height (altitude). 
/// There are a number of celestial coordinate systems based on different fundamental planes and with different terms for the various coordinates. 
/// The spherical coordinate systems used in mathematics normally use radians rather than degrees and measure the azimuthal angle counterclockwise from the x-axis to the y-axis rather than clockwise from north (0°) to east (+90°) like the horizontal coordinate system

/// The spherical coordinate system can be seen as one possible generalization of the polar coordinate system in three-dimensional space. 
/// It can also be further extended to higher-dimensional spaces and is then referred to as a hyperspherical coordinate system.

/// The polar coordinate system is a two-dimensional coordinate system in which each point on a plane is determined by a distance from a reference point and an angle from a reference direction. 
/// The reference point (analogous to the origin of a Cartesian coordinate system) is called the pole, and the ray from the pole in the reference direction is the polar axis. 
/// The distance from the pole is called the radial coordinate, radial distance or simply radius, and the angle is called the angular coordinate, polar angle, or azimuth. 
/// Angles in polar notation are generally expressed in either degrees or radians (2PI radians being equal to 360 degrees).

AFX_DEFINE_STRUCT_SIMD(afxPolarCoord)
// polar coordinates
{
    afxReal32       r; // norm/distance
    afxReal32       theta; // azimuth
};

AFX_DEFINE_STRUCT_SIMD(afxSphericalCoord)
// spherical coordinates
{
    afxPolarCoord   polar;
    afxReal32       phi; // elevation
};

AFXINL afxReal16    AfxPackReal16(afxReal f);
AFXINL afxReal32    AfxUnpackReal16(afxReal16 hf);

AFXINL afxInt16     AfxReal32ToSnorm(afxReal v);
AFXINL afxReal      AfxSnorm16ToReal32(afxInt16 v);

AFXINL void         AfxNdcV2d(afxV2d v, afxV2d const b, afxV2d const total);
AFXINL void         AfxUnndcV2d(afxV2d v, afxV2d const b, afxV2d const total);

#endif//AFX_MATH_DEFS_H
