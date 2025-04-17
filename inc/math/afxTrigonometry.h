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

/*
    Trigonometric Functions
    Functions that deal with angles and their corresponding trigonometric values.

    Sine: sin
    Cosine: cos
    Tangent: tan
    Cotangent: cot
    Secant: sec
    Cosecant: csc
    Inverse Trigonometric: asin, acos, atan, acot, asec, acsc
*/

#ifndef AFX_TRIGONOMETRY_H
#define AFX_TRIGONOMETRY_H

#include "qwadro/inc/math/afxVector.h"

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

AFXINL afxReal64    AfxRad(afxReal64 degrees);
AFXINL afxReal      AfxRadf(afxReal degrees);

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

AFXINL afxReal64    AfxDeg(afxReal64 radians);
AFXINL afxReal      AfxDegf(afxReal radians);


// The standard trigonometric sine function.

AFXINL afxReal64    AfxSin(afxReal64 radians);
AFXINL afxReal      AfxSinf(afxReal radians);

AFXINL void         AfxSinV2d(afxV2d v, afxV2d const in); // v = sin(in)
AFXINL void         AfxSinV3d(afxV3d v, afxV3d const in); // v = sin(in)
AFXINL void         AfxSinV4d(afxV4d v, afxV4d const in); // v = sin(in)

// The standard trigonometric cosine function.

AFXINL afxReal64    AfxCos(afxReal64 radians);
AFXINL afxReal      AfxCosf(afxReal radians);

AFXINL void         AfxCosV2d(afxV2d v, afxV2d const in); // v = cos(in)
AFXINL void         AfxCosV3d(afxV3d v, afxV3d const in); // v = cos(in)
AFXINL void         AfxCosV4d(afxV4d v, afxV4d const in); // v = cos(in)

// Combined cosine and sine

AFXINL void         AfxCosSin(afxReal64 radians, afxReal64* cos, afxReal64* sin);
AFXINL void         AfxCosSinf(afxReal radians, afxReal* cos, afxReal* sin);

// The standard trigonometric tangent function.

AFXINL afxReal64    AfxTan(afxReal64 radians);
AFXINL afxReal      AfxTanf(afxReal radians);

AFXINL void         AfxTanV2d(afxV2d v, afxV2d const in); // v = tan(in)
AFXINL void         AfxTanV3d(afxV3d v, afxV3d const in); // v = tan(in)
AFXINL void         AfxTanV4d(afxV4d v, afxV4d const in); // v = tan(in)

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

AFXINL afxReal64    AfxAsin(afxReal64 x);
AFXINL afxReal      AfxAsinf(afxReal x);

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

AFXINL afxReal64    AfxAcos(afxReal64 x);
AFXINL afxReal      AfxAcosf(afxReal x);

AFXINL void         AfxAcosV2d(afxV2d v, afxV2d const in); // v = acos(in)
AFXINL void         AfxAcosV3d(afxV3d v, afxV3d const in); // v = acos(in)
AFXINL void         AfxAcosV4d(afxV4d v, afxV4d const in); // v = acos(in)

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

AFXINL afxReal64    AfxAtan(afxReal64 y_over_x);
AFXINL afxReal      AfxAtanf(afxReal y_over_x);

AFXINL void         AfxAtanV2d(afxV2d v, afxV2d const in); // v = atan(in)
AFXINL void         AfxAtanV3d(afxV3d v, afxV3d const in); // v = atan(in)
AFXINL void         AfxAtanV4d(afxV4d v, afxV4d const in); // v = atan(in)

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

AFXINL afxReal64    AfxAtan2(afxReal64 y, afxReal64 x);
AFXINL afxReal      AfxAtan2f(afxReal y, afxReal x);



AFXINL void     AfxComputeTriangleNormal(afxV3d const a, afxV3d const b, afxV3d const c, afxReal n[3]);

/**
    Barycentric coordinates are very useful in 2D and 3D graphics. 
    Most graphic applications use them because they provide an easy way to interpolate the value of attributes (color, textures, normals…) between vertices. 
    This can be done because, by definition, barycentric coordinates express “how much of each vertex does a point have”.
    Another frequent use is to use the barycentric coordinates to check if a point is inside, on the edge, or outside the triangle. As we saw:
        If all barycentric coordinates of a point are positive and sum one, they point lies inside the triangle.
        If one barycentric coordinate is zero and the other ones are positive and less than one, the point lies on an edge.
        If any barycentric coordinate is negative, the point is outside the triangle.
        
    Returns a point in Barycentric coordinates, using the specified position vectors.
*/

AFXINL void     AfxBarycentricV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g);
AFXINL void     AfxBarycentricV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g);
AFXINL void     AfxBarycentricV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g);

AFXINL void     AfxBarycentricV2d2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g);
AFXINL void     AfxBarycentricV3d2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g);
AFXINL void     AfxBarycentricV4d2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g);

#endif//AFX_TRIGONOMETRY_H
