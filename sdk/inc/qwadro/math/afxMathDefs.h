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
#include "qwadro/core/afxSimd.h"
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

typedef afxReal         afxV2d[2];
typedef afxReal         afxV3d[3];
typedef afxReal         afxSimd(afxV4d[4]);
static_assert(sizeof(__m128) == sizeof(afxV4d), "");

typedef afxV4d          afxSimd(afxQuat); // 0,1,2 = imaginary, 3 = real
typedef afxV4d          afxSimd(afxRotor); // 0,1,2 = imaginary, 3 = real

typedef afxV2d          afxM2d[2];
typedef afxV3d          afxM3d[3];
typedef afxV4d          afxSimd(afxM4d[4]);
typedef afxV4d          afxSimd(afxM4d3[3]);

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

AFXINL void         AfxRectZero(afxRect *rect) { rect->x = 0; rect->y = 0; rect->w = 1; rect->h = 1; }
AFXINL void         AfxRectCopy(afxRect *rect, afxRect const *src) { rect->x = src->x; rect->y = src->y; rect->w = src->w; rect->h = src->h; }

AFXINL afxReal      AfxToNdc(afxReal x, afxReal total) { return x * (AfxScalar(1) / total); }
AFXINL afxReal      AfxFromNdc(afxReal x, afxReal total) { afxError err = AFX_ERR_NONE; AfxAssert(AfxScalar(0) <= x); AfxAssert(AfxScalar(1) >= x); return x * total; }

AFXINL void AfxExtractV2dInNdc(afxReal v[2], afxReal const b[2], afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxToNdc(b[0], total[0]);
    v[1] = AfxToNdc(b[1], total[1]);
}

AFXINL void AfxExtractV2dOutNdc(afxReal v[2], afxReal const b[2], afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(b);
    AfxAssert(total);
    v[0] = AfxFromNdc(b[0], total[0]);
    v[1] = AfxFromNdc(b[1], total[1]);
}

AFXINL void AfxMakeV2dNdc(afxReal v[2], afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxToNdc(v[0], total[0]);
    v[1] = AfxToNdc(v[1], total[1]);
}

AFXINL void AfxUnmakeV2dNdc(afxReal v[2], afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(total);
    AfxAssert(v != total);
    v[0] = AfxFromNdc(v[0], total[0]);
    v[1] = AfxFromNdc(v[1], total[1]);
}

#include "qwadro/mem/afxMmu.h"

//#pragma warning (push)
//#pragma warning (disable : 4090)

/*
Normalized Device Coordinates
Normalized Device Coordinates (NDC) is the coordinate system that vertices are in once vertex processing is finished.
You could consider this the "native" 3D coordinate system of OpenGL.
It has a range of [-1.0, 1.0] for all 3 coordinates, and is left-handed.

Fixed Pipeline
In the legacy fixed pipeline, vertices are initially specified in object coordinates (see http://www.glprogramming.com/red/chapter03.html).
The modelview matrix transforms them to eye coordinates, which the projection matrix transforms to clip coordinates. After perspective division, they become NDC.
It is most common to use a right-handed coordinate system for the object coordinates.
A modelview matrix that preserves handedness is used, which results in eye coordinates that are still right-handed.
The projection matrix flips the direction of the z-axis, resulting in left-handed clip coordinates and NDC.
If you choose so, you can easily use left-handed object coordinates, and use a modelview matrix that flips handedness, so that you end up with right-handed eye coordinates again.

Programmable Pipeline
With the programmable pipeline, you're at complete liberty to use any coordinate systems you want in your vertex shader. 
Say if your original vertices are specified in spherical coordinates, that's no problem at all. 
You just need to process them accordingly in the shader code. 
The vertex shader needs to produce clip coordinates (which as previously explained are left-handed) as output, but everything before that is completely open.
*/

//////////////////////// QWADRO NAME CONVENTIONS ///////////////////////////////

// LINEAR TRANSFORMATIONS AFFECTS ONLY 3X3 SUBSET OF MATRICES.
// AFFINE TRANSFORMATIONS AFFECTS ONLY 4X3 SUBSET OF MATRICES. 

#endif//AFX_MATH_DEFS_H
