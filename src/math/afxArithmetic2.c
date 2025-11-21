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

#include "qwadro/math/afxArithmetic2.h"

////////////////////////////////////////////////////////////////////////////////
// EXTENDED ARITHMETICA                                                       //
////////////////////////////////////////////////////////////////////////////////

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

_AFXINL void AfxV2dMix(afxV2d v, afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] * (1.f - t) + y[0] * t;
    v[1] = x[1] * (1.f - t) + y[1] * t;
}

_AFXINL void AfxV3dMix(afxV3d v, afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] * (1.f - t) + y[0] * t;
    v[1] = x[1] * (1.f - t) + y[1] * t;
    v[2] = x[2] * (1.f - t) + y[2] * t;
}

_AFXINL void AfxV4dMix(afxV4d v, afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] * (1.f - t) + y[0] * t;
    v[1] = x[1] * (1.f - t) + y[1] * t;
    v[2] = x[2] * (1.f - t) + y[2] * t;
    v[3] = x[3] * (1.f - t) + y[3] * t;
}

// Lerp
// LERP is a method to interpolate linearly between two values. In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// v = x + t * (y - x)

_AFXINL void AfxV2dLerp(afxV2d v, afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
}

_AFXINL void AfxV3dLerp(afxV3d v, afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
    v[2] = x[2] + t * (y[2] - x[2]);
}

_AFXINL void AfxV4dLerp(afxV4d v, afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(v, x, y);

    v[0] = x[0] + t * (y[0] - x[0]);
    v[1] = x[1] + t * (y[1] - x[1]);
    v[2] = x[2] + t * (y[2] - x[2]);
    v[3] = x[3] + t * (y[3] - x[3]);
}

// Square root

_AFXINL void AfxV2dSqrt(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxSqrtf(in[0]);
    v[1] = AfxSqrtf(in[1]);
}

_AFXINL void AfxV3dSqrt(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxSqrtf(in[0]);
    v[1] = AfxSqrtf(in[1]);
    v[2] = AfxSqrtf(in[2]);
}

_AFXINL void AfxV4dSqrt(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxSqrtf(in[0]);
    v[1] = AfxSqrtf(in[1]);
    v[2] = AfxSqrtf(in[2]);
    v[3] = AfxSqrtf(in[3]);
}

// Reciprocal square root

_AFXINL void AfxV2dRsqrt(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxRsqrtf(in[0]);
    v[1] = AfxRsqrtf(in[1]);
}

_AFXINL void AfxV3dRsqrt(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxRsqrtf(in[0]);
    v[1] = AfxRsqrtf(in[1]);
    v[2] = AfxRsqrtf(in[2]);
}

_AFXINL void AfxV4dRsqrt(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxRsqrtf(in[0]);
    v[1] = AfxRsqrtf(in[1]);
    v[2] = AfxRsqrtf(in[2]);
    v[3] = AfxRsqrtf(in[3]);
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

// Clamp

_AFXINL void AfxV2dClamp(afxV2d v, afxV2d const in, afxV2d const min, afxV2d const max)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT(min);
    AFX_ASSERT(max);
    AFX_ASSERT(min[0] <= max[0]);
    AFX_ASSERT(min[1] <= max[1]);

    AfxV2dMax(v, min, in);
    AfxV2dMin(v, max, v);
}

_AFXINL void AfxV3dClamp(afxV3d v, afxV3d const in, afxV3d const min, afxV3d const max)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT(min);
    AFX_ASSERT(max);
    AFX_ASSERT(min[0] <= max[0]);
    AFX_ASSERT(min[1] <= max[1]);
    AFX_ASSERT(min[2] <= max[2]);

    AfxV3dMax(v, min, in);
    AfxV3dMin(v, max, v);
}

_AFXINL void AfxV4dClamp(afxV4d v, afxV4d const in, afxV4d const min, afxV4d const max)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AFX_ASSERT(min);
    AFX_ASSERT(max);
    AFX_ASSERT(min[0] <= max[0]);
    AFX_ASSERT(min[1] <= max[1]);
    AFX_ASSERT(min[2] <= max[2]);
    AFX_ASSERT(min[3] <= max[3]);

    AfxV4dMax(v, min, in);
    AfxV4dMin(v, max, v);
}

// Truncate

_AFXINL void AfxTruncateV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AFX_ABS(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AFX_ABS(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
}

_AFXINL void AfxTruncateV3d(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AFX_ABS(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AFX_ABS(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
    v[2] = AfxIsNan(in[2]) ? 0x7FC00000 : ((AFX_ABS(in[2]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[2]))) : in[2]);
}

_AFXINL void AfxTruncateV4d(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);

    v[0] = AfxIsNan(in[0]) ? 0x7FC00000 : ((AFX_ABS(in[0]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[0]))) : in[0]);
    v[1] = AfxIsNan(in[1]) ? 0x7FC00000 : ((AFX_ABS(in[1]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[1]))) : in[1]);
    v[2] = AfxIsNan(in[2]) ? 0x7FC00000 : ((AFX_ABS(in[2]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[2]))) : in[2]);
    v[3] = AfxIsNan(in[3]) ? 0x7FC00000 : ((AFX_ABS(in[3]) < 8388608.0f) ? (afxReal)(((afxInt32)(in[3]))) : in[3]);
}

// Saturate

_AFXINL void AfxSaturateV2d(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);

    AfxV2dClamp(v, in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

_AFXINL void AfxSaturateV3d(afxV3d v, afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);

    AfxV3dClamp(v, in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

_AFXINL void AfxSaturateV4d(afxV4d v, afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);

    AfxV4dClamp(v, in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

// Mini

_AFXINL void AfxV2dMin(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
}

_AFXINL void AfxV3dMin(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
    v[2] = a[2] < b[2] ? a[2] : b[2];
}

_AFXINL void AfxV4dMin(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] < b[0] ? a[0] : b[0];
    v[1] = a[1] < b[1] ? a[1] : b[1];
    v[2] = a[2] < b[2] ? a[2] : b[2];
    v[3] = a[3] < b[3] ? a[3] : b[3];
}

// Maxi

_AFXINL void AfxV2dMax(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
}

_AFXINL void AfxV3dMax(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
    v[2] = a[2] > b[2] ? a[2] : b[2];
}

_AFXINL void AfxV4dMax(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] > b[0] ? a[0] : b[0];
    v[1] = a[1] > b[1] ? a[1] : b[1];
    v[2] = a[2] > b[2] ? a[2] : b[2];
    v[3] = a[3] > b[3] ? a[3] : b[3];
}

// Abs

_AFXINL void AfxV2dAbs(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AFX_ABS(in[0]);
    v[1] = AFX_ABS(in[1]);
}

_AFXINL void AfxV3dAbs(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AFX_ABS(in[0]);
    v[1] = AFX_ABS(in[1]);
    v[2] = AFX_ABS(in[2]);
}

_AFXINL void AfxV4dAbs(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AFX_ABS(in[0]);
    v[1] = AFX_ABS(in[1]);
    v[2] = AFX_ABS(in[2]);
    v[3] = AFX_ABS(in[3]);
}

// Ceil

_AFXINL void AfxV2dCeil(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxCeilf(in[0]);
    v[1] = AfxCeilf(in[1]);
}

_AFXINL void AfxV3dCeil(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxCeilf(in[0]);
    v[1] = AfxCeilf(in[1]);
    v[2] = AfxCeilf(in[2]);
}

_AFXINL void AfxV4dCeil(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxCeilf(in[0]);
    v[1] = AfxCeilf(in[1]);
    v[2] = AfxCeilf(in[2]);
    v[3] = AfxCeilf(in[3]);
}

// Floor

_AFXINL void AfxV2dFloor(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxFloorf(in[0]);
    v[1] = AfxFloorf(in[1]);
}

_AFXINL void AfxV3dFloor(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxFloorf(in[0]);
    v[1] = AfxFloorf(in[1]);
    v[2] = AfxFloorf(in[2]);
}

_AFXINL void AfxV4dFloor(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxFloorf(in[0]);
    v[1] = AfxFloorf(in[1]);
    v[2] = AfxFloorf(in[2]);
    v[3] = AfxFloorf(in[3]);
}

////////////////////////////////////////////////////////////////////////////////
// EXPONENTIAL ARITHMETICA                                                    //
////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

// Returns the natural exponentiation of x; e^x.

_AFXINL afxReal64 AfxExp(afxReal64 s)
{
    return exp(s);
}

_AFXINL afxReal AfxExpf(afxReal s)
{
    return expf(s);
}

// Returns 2 raised to the x power; 2^x.

_AFXINL afxReal64 AfxExp2(afxReal64 s)
{
    return exp2(s);
}

_AFXINL afxReal AfxExp2f(afxReal s)
{
    return exp2f(s);
}

_AFXINL afxReal64 AfxExp10(afxReal64 s)
{
    return AfxPow(10.0, s);
}

_AFXINL afxReal AfxExp10f(afxReal s)
{
    return AfxPowf(10.f, s);
}

// Exp

_AFXINL void AfxV2dExp(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
}

_AFXINL void AfxV3dExp(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
    v[2] = AfxExpf(in[2]);
}

_AFXINL void AfxV4dExp(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExpf(in[0]);
    v[1] = AfxExpf(in[1]);
    v[2] = AfxExpf(in[2]);
    v[3] = AfxExpf(in[3]);
}

// Exp2

_AFXINL void AfxV2dExp2(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
}

_AFXINL void AfxV3dExp2(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
    v[2] = AfxExp2f(in[2]);
}

_AFXINL void AfxV4dExp2(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp2f(in[0]);
    v[1] = AfxExp2f(in[1]);
    v[2] = AfxExp2f(in[2]);
    v[3] = AfxExp2f(in[3]);
}

// Exp10

_AFXINL void AfxV2dExp10(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
}

_AFXINL void AfxV3dExp10(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
    v[2] = AfxExp10f(in[2]);
}

_AFXINL void AfxV4dExp10(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxExp10f(in[0]);
    v[1] = AfxExp10f(in[1]);
    v[2] = AfxExp10f(in[2]);
    v[3] = AfxExp10f(in[3]);
}

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

_AFXINL afxReal64 AfxPow(afxReal64 base, afxReal64 exp)
{
    return AfxExp(exp * AfxLog(base));
}

_AFXINL afxReal AfxPowf(afxReal base, afxReal exp)
{
    return AfxExpf(exp * AfxLogf(base));
}

// Pow

_AFXINL void AfxV2dPow(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
}

_AFXINL void AfxV3dPow(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
    v[2] = AfxPowf(in[2], in[2]);
}

_AFXINL void AfxV4dPow(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxPowf(in[0], in[0]);
    v[1] = AfxPowf(in[1], in[1]);
    v[2] = AfxPowf(in[2], in[2]);
    v[3] = AfxPowf(in[3], in[3]);
}


////////////////////////////////////////////////////////////////////////////////
// LOGARITHMIC ARITHMETICA                                                    //
////////////////////////////////////////////////////////////////////////////////


// Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
// Results are undefined if x <= 0.

_AFXINL afxReal64 AfxLog(afxReal64 s)
{
    return log(s);
}

_AFXINL afxReal AfxLogf(afxReal s)
{
    return logf(s);
}

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

_AFXINL afxReal64 AfxLog2(afxReal64 s)
{
    return log2(s);
}

_AFXINL afxReal AfxLog2f(afxReal s)
{
    return log2f(s);
}

_AFXINL afxReal64 AfxLog10(afxReal64 s)
{
    return log10(s);
}

_AFXINL afxReal AfxLog10f(afxReal s)
{
    return log10f(s);
}

// Log

_AFXINL void AfxV2dLog(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
}

_AFXINL void AfxV3dLog(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
    v[2] = AfxLogf(in[2]);
}

_AFXINL void AfxV4dLog(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLogf(in[0]);
    v[1] = AfxLogf(in[1]);
    v[2] = AfxLogf(in[2]);
    v[3] = AfxLogf(in[3]);
}

// Log2

_AFXINL void AfxV2dLog2(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
}

_AFXINL void AfxV3dLog2(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
    v[2] = AfxLog2f(in[2]);
}

_AFXINL void AfxV4dLog2(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog2f(in[0]);
    v[1] = AfxLog2f(in[1]);
    v[2] = AfxLog2f(in[2]);
    v[3] = AfxLog2f(in[3]);
}

// Log10

_AFXINL void AfxV2dLog10(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
}

_AFXINL void AfxV3dLog10(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
    v[2] = AfxLog10f(in[2]);
}

_AFXINL void AfxV4dLog10(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = AfxLog10f(in[0]);
    v[1] = AfxLog10f(in[1]);
    v[2] = AfxLog10f(in[2]);
    v[3] = AfxLog10f(in[3]);
}
