/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxTrigonometry.h"

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

_AFXINL afxReal64 AfxRad(afxReal64 degrees)
{
    return degrees * (AFX_PI / 180.0);
}

_AFXINL afxReal AfxRadf(afxReal degrees)
{
    return degrees * (AFX_PI / 180.f);
}

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

_AFXINL afxReal64 AfxDeg(afxReal64 radians)
{
    return radians * (180.0 / AFX_PI);
}

_AFXINL afxReal AfxDegf(afxReal radians)
{
    return radians * (180.f / AFX_PI);
}

// The standard trigonometric sine function.

_AFXINL afxReal64 AfxSin(afxReal64 radians)
{
    return sin(radians);
}

_AFXINL afxReal AfxSinf(afxReal radians)
{
    return sinf(radians);
}

// Sine

_AFXINL void AfxSinV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxSinf(in[0]);
    v[1] = AfxSinf(in[1]);
}

_AFXINL void AfxSinV3d(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxSinf(in[0]);
    v[1] = AfxSinf(in[1]);
    v[2] = AfxSinf(in[2]);
}

_AFXINL void AfxSinV4d(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxSinf(in[0]);
    v[1] = AfxSinf(in[1]);
    v[2] = AfxSinf(in[2]);
    v[3] = AfxSinf(in[3]);
}

// The standard trigonometric cosine function.

_AFXINL afxReal64 AfxCos(afxReal64 radians)
{
    return cos(radians);
}

_AFXINL afxReal AfxCosf(afxReal radians)
{
    return cosf(radians);
}

// Cosine

_AFXINL void AfxCosV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxCosf(in[0]);
    v[1] = AfxCosf(in[1]);
}

_AFXINL void AfxCosV3d(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxCosf(in[0]);
    v[1] = AfxCosf(in[1]);
    v[2] = AfxCosf(in[2]);
}

_AFXINL void AfxCosV4d(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxCosf(in[0]);
    v[1] = AfxCosf(in[1]);
    v[2] = AfxCosf(in[2]);
    v[3] = AfxCosf(in[3]);
}

// A combined cosine and sine function

_AFXINL void AfxCosSin(afxReal64 radians, afxReal64* cos, afxReal64* sin)
{
    *cos = AfxCos(radians);
    *sin = AfxSin(radians);
}

_AFXINL void AfxCosSinf(afxReal radians, afxReal* cos, afxReal* sin)
{
    *cos = AfxCosf(radians);
    *sin = AfxSinf(radians);
}

// The standard trigonometric tangent function.

_AFXINL afxReal64 AfxTan(afxReal64 radians)
{
    return tan(radians);
}

_AFXINL afxReal AfxTanf(afxReal radians)
{
    return tanf(radians);
}

// Tangent

_AFXINL void AfxTanV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxTanf(in[0]);
    v[1] = AfxTanf(in[1]);
}

_AFXINL void AfxTanV3d(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxTanf(in[0]);
    v[1] = AfxTanf(in[1]);
    v[2] = AfxTanf(in[2]);
}

_AFXINL void AfxTanV4d(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxTanf(in[0]);
    v[1] = AfxTanf(in[1]);
    v[2] = AfxTanf(in[2]);
    v[3] = AfxTanf(in[3]);
}

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

_AFXINL afxReal64 AfxAsin(afxReal64 x)
{
    return asin(x);
}

_AFXINL afxReal AfxAsinf(afxReal x)
{
    return asinf(x);
}

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

_AFXINL afxReal64 AfxAcos(afxReal64 x)
{
    return acos(x);
}

_AFXINL afxReal AfxAcosf(afxReal x)
{
    return acosf(x);
}

// Arc cosine

_AFXINL void AfxAcosV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxAcosf(in[0]);
    v[1] = AfxAcosf(in[1]);
}

_AFXINL void AfxAcosV3d(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxAcosf(in[0]);
    v[1] = AfxAcosf(in[1]);
    v[2] = AfxAcosf(in[2]);
}

_AFXINL void AfxAcosV4d(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxAcosf(in[0]);
    v[1] = AfxAcosf(in[1]);
    v[2] = AfxAcosf(in[2]);
    v[3] = AfxAcosf(in[3]);
}

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

_AFXINL afxReal64 AfxAtan(afxReal64 y_over_x)
{
    return atan(y_over_x);
}

_AFXINL afxReal AfxAtanf(afxReal y_over_x)
{
    return atanf(y_over_x);
}

// Arc tangent

_AFXINL void AfxAtanV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxAtanf(in[0]);
    v[1] = AfxAtanf(in[1]);
}

_AFXINL void AfxAtanV3d(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxAtanf(in[0]);
    v[1] = AfxAtanf(in[1]);
    v[2] = AfxAtanf(in[2]);
}

_AFXINL void AfxAtanV4d(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxAtanf(in[0]);
    v[1] = AfxAtanf(in[1]);
    v[2] = AfxAtanf(in[2]);
    v[3] = AfxAtanf(in[3]);
}

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

_AFXINL afxReal64 AfxAtan2(afxReal64 y, afxReal64 x)
{
    return atan2(y, x);
}

_AFXINL afxReal AfxAtan2f(afxReal y, afxReal x)
{
    return atan2f(y, x);
}







_AFXINL void AfxComputeTriangleNormal(afxV3d const a, afxV3d const b, afxV3d const c, afxV3d n)
{
    afxV3d t, t2;
    AfxV3dSub(t, b, a);
    AfxV3dSub(t2, c, a);
    AfxV3dCross(n, t, t2);
    AfxV3dNormalize(n, n);
}

// BaryCentric

_AFXINL void AfxBarycentricV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // Adapted from DirectXMath. Works?

    afxV2d p10;
    AfxV2dSub(p10, b, a);

    afxV2d fv;
    AfxFillV2d(fv, f);

    afxV2d p20;
    AfxV2dSub(p20, c, a);

    afxV2d gv;
    AfxFillV2d(gv, g);

    afxV2d tmp;
    AfxV2dMad(tmp, a, p10, fv);
    AfxV2dMad(v, tmp, p20, gv);
}

_AFXINL void AfxBarycentricV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV3d p10;
    AfxV3dSub(p10, b, a);

    afxV3d fv;
    AfxFillV3d(fv, f);

    afxV3d p20;
    AfxV3dSub(p20, c, a);

    afxV3d gv;
    AfxFillV3d(gv, g);

    afxV3d tmp;
    AfxV3dMad(tmp, a, p10, fv);
    AfxV3dMad(v, tmp, p20, gv);
}

_AFXINL void AfxBarycentricV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV4d p10;
    AfxV4dSub(p10, b, a);

    afxV4d fv;
    AfxFillV4d(fv, f);

    afxV4d p20;
    AfxV4dSub(p20, c, a);

    afxV4d gv;
    AfxFillV4d(gv, g);

    afxV4d tmp;
    AfxV4dMad(tmp, a, p10, fv);
    AfxV4dMad(v, tmp, p20, gv);
}

// BaryCentric2

_AFXINL void AfxBarycentricV2d2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV2d p10;
    AfxV2dSub(p10, b, a);

    afxV2d p20;
    AfxV2dSub(p20, c, a);

    afxV2d tmp;
    AfxV2dMad(tmp, a, p10, f);
    AfxV2dMad(v, tmp, p20, g);
}

_AFXINL void AfxBarycentricV3d2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV3d p10;
    AfxV3dSub(p10, b, a);

    afxV3d p20;
    AfxV3dSub(p20, c, a);

    afxV3d tmp;
    AfxV3dMad(tmp, a, p10, f);
    AfxV3dMad(v, tmp, p20, g);
}

_AFXINL void AfxBarycentricV4d2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV4d p10;
    AfxV4dSub(p10, b, a);

    afxV4d p20;
    AfxV4dSub(p20, c, a);

    afxV4d tmp;
    AfxV4dMad(tmp, a, p10, f);
    AfxV4dMad(v, tmp, p20, g);
}
