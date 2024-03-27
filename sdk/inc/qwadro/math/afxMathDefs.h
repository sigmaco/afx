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

// IMPORTANT: O módulo de matemática do Qwadro é matemática para programadores, não para matemáticos. 
// Nossos ângulos e suas contrapartes são representadas em "radians", não em "degrees". Funções para degrees são apenas utilitários de conveniência em conversão.
// Nossas rotações e orientações são representadas primarialmente por "quaternion". Funções para axial rotation e Euler são apenas utilitários.
// É totalmente vedado adulterar os princípios gigachads e heterotops do Qwadro com essas tcholices dessas game engines randômicas do mundo por responder a vontade das pessoas de querer serem eternos subcolocados..

// No Qwadro, usamos, como estilo de programação, muitos parênteses irrelevantes. Eles existem para fins didáticos mesmo, facilitando a vida de pessoas que não conhecem o princípio de precedência de PEMDAS.

#ifndef AFX_MATH_DEFS_H
#define AFX_MATH_DEFS_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/math/afxSimd.h"
#include <math.h>
//#include <stdalign.h>

//#define MFX_USE_RWMATH

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RWMATH

#define AfxScalar(x_) ((afxReal)x_)

#define AFX_PI          AfxScalar(3.14159265358979323846)
#define AFX_PI_OVER2    (AFX_PI / AfxScalar(2))
#define AFX_EPSILON     AfxScalar(FLT_EPSILON)

typedef afxReal         afxSimd(afxV2d[2]);
typedef afxReal         afxSimd(afxV3d[3]);
typedef afxReal         afxSimd(afxV4d[4]);
static_assert(sizeof(__m128) == sizeof(afxV4d), "");

typedef afxV4d          afxSimd(afxQuat); // 0,1,2 = imaginary, 3 = real
typedef afxV4d          afxSimd(afxRotor); // 0,1,2 = imaginary, 3 = real

typedef afxV2d          afxSimd(afxM2d[2]);
typedef afxV3d          afxSimd(afxM3d[3]);
typedef afxV4d          afxSimd(afxM4d[4]);
typedef afxV4d          afxSimd(afxM4d3[3]);

typedef afxV4d          afxSimd(afxVector);
typedef afxV4d          afxSimd(afxPoint);
typedef afxM4d          afxSimd(afxMatrix);

#if 0
static_assert(__alignof(afxV2d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxV3d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxV4d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxQuat) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxM3d) == AFX_SIMD_ALIGN, "");
static_assert(__alignof(afxM4d) == AFX_SIMD_ALIGN, "");
#endif

AFX_DEFINE_STRUCT(afxRect)
{
    union { afxInt offset[2]; struct { afxInt x, y; }; };
    union { afxNat extent[2]; struct { afxNat w, h; }; };
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

AFX_DEFINE_STRUCT(afxPolarCoord) // polar coordinates
{
    afxReal32       r; // norm/distance
    afxReal32       theta; // azimuth
};

AFX_DEFINE_STRUCT(afxSphericalCoord) // spherical coordinates
{
    afxPolarCoord   polar;
    afxReal32       phi; // elevation
};


AFXINL afxReal16 AfxReal32To16(afxReal f);
AFXINL afxReal32 AfxReal16To32(afxReal16 hf);

AFXINL afxInt16 AfxReal32ToSnorm(afxReal v);
AFXINL afxReal AfxSnorm16ToReal32(afxInt16 v);

AFXINL afxBool AfxIsGreaterThanEpsilon(afxReal value);

AFXINL afxBool AfxIsNan(afxReal value);

AFXINL afxBool AfxIsInfinite(afxReal value);

AFXINL afxBool AfxRealIsEquivalent(afxReal a, afxReal b);

AFXINL afxBool AfxRealIsEqual(afxReal a, afxReal b, afxReal epsilon);

AFXINL afxReal AfxRecip(afxReal value);

AFXINL afxReal AfxLerp(afxReal a, afxReal b, afxReal time);

AFXINL afxReal AfxClamp(afxReal value, afxReal min, afxReal max);

AFXINL afxReal AfxSq(afxReal s);

AFXINL afxReal AfxFloor(afxReal s);

AFXINL afxReal AfxCeil(afxReal s);

/*********************************************************************************
 * ANGLE AND TRIGONOMETRY FUNCTIONS                                              *
 *                                                                               *
 * Function parameters specified as angle are assumed to be in units of radians. *
 * In no case will any of these functions result in a divide by zero error.      *
 * If the divisor of a ratio is 0, then results will be undefined.               *
 *                                                                               *
 * These all operate component-wise. The description is per component.           *
 *********************************************************************************/

 // Converts degrees to radians.
 // The value returned by this function is (pi / 180) * degrees.

AFXINL afxReal AfxRad(afxReal degrees);

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

AFXINL afxReal AfxDeg(afxReal radians);

// The standard trigonometric sine function.

AFXINL afxReal AfxSin(afxReal radians);

// The standard trigonometric cosine function.

AFXINL afxReal AfxCos(afxReal radians);

// Combined cosine and sine

AFXINL void AfxCosSin(afxReal radians, afxReal* cos, afxReal* sin);

// The standard trigonometric tangent function.

AFXINL afxReal AfxTan(afxReal radians);

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

AFXINL afxReal AfxAsin(afxReal x);

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

AFXINL afxReal AfxAcos(afxReal x);

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

AFXINL afxReal AfxAtan(afxReal y_over_x);

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

AFXINL afxReal AfxAtan2(afxReal y, afxReal x);

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.

AFXINL afxReal AfxSinh(afxReal x);

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.

AFXINL afxReal AfxCosh(afxReal x);

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).

AFXINL afxReal AfxTanh(afxReal x);

// Arc hyperbolic sine; returns the inverse of sinh.

AFXINL afxReal AfxAsinh(afxReal x);

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.

AFXINL afxReal AfxAcosh(afxReal x);

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.

AFXINL afxReal AfxAtanh(afxReal y_over_x);


/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

 // Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
 // Results are undefined if x <= 0.

AFXINL afxReal AfxLog(afxReal s);

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

AFXINL afxReal AfxLog2(afxReal s);

AFXINL afxReal AfxLog10(afxReal s);

// Returns the natural exponentiation of x; e^x.

AFXINL afxReal AfxExp(afxReal s);

// Returns 2 raised to the x power; 2^x.

AFXINL afxReal AfxExp2(afxReal s);

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

AFXINL afxReal AfxPow(afxReal base, afxReal exp);

AFXINL afxReal AfxExp10(afxReal s);

// Returns sqrt(x).
// Results are undefined if x < 0.

AFXINL afxReal AfxSqrt(afxReal s);

// Returns 1 / sqrt(x).
// Results are undefined if x <= 0.

AFXINL afxReal AfxRsqrt(afxReal s);

////////////////////////////////////////////////////////////////////////////////

AFXINL void         AfxRectZero(afxRect *rect);
AFXINL void         AfxRectCopy(afxRect *rect, afxRect const *src);

AFXINL afxReal      AfxToNdc(afxReal x, afxReal total);
AFXINL afxReal      AfxFromNdc(afxReal x, afxReal total);

AFXINL void         AfxExtractV2dInNdc(afxV2d v, afxV2d const b, afxV2d const total);
AFXINL void         AfxExtractV2dOutNdc(afxV2d v, afxV2d const b, afxV2d const total);

AFXINL void         AfxMakeV2dNdc(afxV2d v, afxV2d const total);
AFXINL void         AfxUnmakeV2dNdc(afxV2d v, afxV2d const total);

#endif//AFX_MATH_DEFS_H
