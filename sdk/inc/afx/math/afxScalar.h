/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SCALAR_H
#define AFX_SCALAR_H

#include "afxMathDefs.h"

AFXINL afxBool      AfxRealIsEqual(afxReal a, afxReal b) { return !!(AfxAbs(a - b) < AFX_EPSILON); }

AFXINL afxReal    AfxRecip(afxReal value) { return AfxScalar(1) / value; }

AFXINL afxReal    AfxLerp(afxReal a, afxReal time, afxReal b) { return  (AfxScalar(1) - time) * a + time * b; }

AFXINL afxReal    AfxClamp(afxReal min, afxReal value, afxReal max) { return (value < min ? min : (value > max ? max : value)); }

AFXINL afxReal    AfxSq(afxReal s) { return s * s; }

AFXINL afxReal    AfxFloor(afxReal s) { return (afxReal)floor((afxReal)s); }

AFXINL afxReal    AfxCeil(afxReal s) { return (afxReal)ceil((afxReal)s); }

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
AFXINL afxReal AfxRad(afxReal degrees)
{
    return degrees * (AFX_PI / AfxScalar(180));
}

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.
AFXINL afxReal AfxDeg(afxReal radians)
{
    return radians * (AfxScalar(180) / AFX_PI);
}

// The standard trigonometric sine function.
AFXINL afxReal AfxSin(afxReal radians)
{
    return sinf(radians);
}

// The standard trigonometric cosine function.
AFXINL afxReal AfxCos(afxReal radians)
{
    return cosf(radians);
}

// The standard trigonometric tangent function.
AFXINL afxReal AfxTan(afxReal radians)
{
    return tanf(radians);
}

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.
AFXINL afxReal AfxAsin(afxReal x)
{
    return asinf(x);
}

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.
AFXINL afxReal AfxAcos(afxReal x)
{
    return acosf(x);
}

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].
AFXINL afxReal AfxAtan(afxReal y_over_x)
{
    return atanf(y_over_x);
}

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.
AFXINL afxReal AfxAtan2(afxReal y, afxReal x)
{
    return atan2f(y, x);
}

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.
AFXINL afxReal AfxSinh(afxReal x)
{
    return sinhf(x);
}

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.
AFXINL afxReal AfxCosh(afxReal x)
{
    return coshf(x);
}

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).
AFXINL afxReal AfxTanh(afxReal x)
{
    return tanhf(x);
}

// Arc hyperbolic sine; returns the inverse of sinh.
AFXINL afxReal AfxAsinh(afxReal x)
{
    return asinhf(x);
}

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.
AFXINL afxReal AfxAcosh(afxReal x)
{
    return acoshf(x);
}

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.
AFXINL afxReal AfxAtanh(afxReal y_over_x)
{
    return atanhf(y_over_x);
}


/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

// Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
// Results are undefined if x <= 0.
AFXINL afxReal AfxLog(afxReal s)
{
    return logf(s);
}

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.
AFXINL afxReal AfxLog2(afxReal s)
{
    return log2f(s);
}

// Returns the natural exponentiation of x; e^x.
AFXINL afxReal AfxExp(afxReal s)
{
    return expf(s);
}

// Returns 2 raised to the x power; 2^x.
AFXINL afxReal AfxExp2(afxReal s)
{
    return exp2f(s);
}

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.
AFXINL afxReal AfxPow(afxReal base, afxReal exp)
{
    return AfxExp(exp * AfxLog(base));
}

// Returns sqrt(x).
// Results are undefined if x < 0.
AFXINL afxReal AfxSqrt(afxReal s)
{
#ifdef MFX_USE_SIMD
    __m128 simd = _mm_set_ss(s);
    __m128 simd2 = _mm_sqrt_ss(simd);
    return _mm_cvtss_f32(simd2);
#else
    return sqrtf(s);
#endif
}

// Returns 1 / sqrt(x).
// Results are undefined if x <= 0.
AFXINL afxReal AfxRecipSqrt(afxReal s)
{
#ifdef MFX_USE_SIMD
    __m128 simd = _mm_set_ss(s);
    __m128 simd2 = _mm_rsqrt_ss(simd);
    return _mm_cvtss_f32(simd2);
#else
    return AfxRecip(AfxSqrt(s));
#endif
}

#endif//AFX_SCALAR_H