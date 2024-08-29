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

#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"

_AFXINL afxBool AfxIsGreaterThanEpsilon(afxReal value) // if not, it is less or equal
{
    return (value <= AFX_EPSILON);
}

_AFXINL afxBool AfxIsNan(afxReal value)
{
    return isnan(value);
}

_AFXINL afxBool AfxIsInfinite(afxReal value)
{
    return isinf(value);
}

_AFXINL afxBool AfxRealIsEquivalent(afxReal a, afxReal b)
{
    return !!(AfxAbs(a - b) < AFX_EPSILON);
}

_AFXINL afxBool  AfxRealIsEqual(afxReal a, afxReal b, afxReal epsilon)
{
    return !!(AfxAbs(a - b) < epsilon);
}

_AFXINL afxReal64 AfxRecip(afxReal64 value)
{
    return 1.0 / value;
}

_AFXINL afxReal AfxRecipf(afxReal value)
{
    return 1.f / value;
}

_AFXINL afxReal64 AfxNdc(afxReal64 x, afxReal64 total)
{
    return x * (1.0 / total);
}

_AFXINL afxReal AfxNdcf(afxReal x, afxReal total)
{
    return x * (AfxScalar(1) / total);
}

_AFXINL afxReal64 AfxUnndc(afxReal64 x, afxReal64 total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(AfxScalar(0) <= x);
    AfxAssert(AfxScalar(1) >= x);
    return x * total;
}

_AFXINL afxReal AfxUnndcf(afxReal x, afxReal total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(AfxScalar(0) <= x);
    AfxAssert(AfxScalar(1) >= x);
    return x * total;
}

_AFXINL afxReal64 AfxLerp(afxReal64 a, afxReal64 b, afxReal64 time)
{
    return  a * (1.0 - time) + time * b;
}

_AFXINL afxReal AfxLerpf(afxReal a, afxReal b, afxReal time)
{
    return  a * (1.f - time) + time * b;
}

_AFXINL afxReal64 AfxClampd(afxReal64 value, afxReal64 min, afxReal64 max)
{
    return (value < min ? min : (value > max ? max : value));
}

_AFXINL afxReal AfxClampf(afxReal value, afxReal min, afxReal max)
{
    return (value < min ? min : (value > max ? max : value));
}

_AFXINL afxReal AfxModf(afxReal x, afxReal y)
{
    return fmodf(x, y);
}

_AFXINL afxReal64 AfxMod(afxReal64 x, afxReal64 y)
{
    return fmod(x, y);
}

_AFXINL afxReal64 AfxSq(afxReal64 s)
{
    return s * s;
}

_AFXINL afxReal AfxSqf(afxReal s)
{
    return s * s;
}

_AFXINL afxReal64 AfxFloor(afxReal64 s)
{
    return floor(s);
}

_AFXINL afxReal AfxFloorf(afxReal s)
{
    return floorf(s);
}

_AFXINL afxReal64 AfxCeil(afxReal64 s)
{
    return ceil(s);
}

_AFXINL afxReal AfxCeilf(afxReal s)
{
    return ceilf(s);
}

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

// The standard trigonometric cosine function.

_AFXINL afxReal64 AfxCos(afxReal64 radians)
{
    return cos(radians);
}

_AFXINL afxReal AfxCosf(afxReal radians)
{
    return cosf(radians);
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

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.

_AFXINL afxReal64 AfxSinh(afxReal64 x)
{
    return sinh(x);
}

_AFXINL afxReal AfxSinhf(afxReal x)
{
    return sinhf(x);
}

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.

_AFXINL afxReal64 AfxCosh(afxReal64 x)
{
    return cosh(x);
}

_AFXINL afxReal AfxCoshf(afxReal x)
{
    return coshf(x);
}

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).

_AFXINL afxReal64 AfxTanh(afxReal64 x)
{
    return tanh(x);
}

_AFXINL afxReal AfxTanhf(afxReal x)
{
    return tanhf(x);
}

// Arc hyperbolic sine; returns the inverse of sinh.

_AFXINL afxReal64 AfxAsinh(afxReal64 x)
{
    return asinh(x);
}

_AFXINL afxReal AfxAsinhf(afxReal x)
{
    return asinhf(x);
}

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.

_AFXINL afxReal64 AfxAcosh(afxReal64 x)
{
    return acosh(x);
}

_AFXINL afxReal AfxAcoshf(afxReal x)
{
    return acoshf(x);
}

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.

_AFXINL afxReal64 AfxAtanh(afxReal64 y_over_x)
{
    return atanh(y_over_x);
}

_AFXINL afxReal AfxAtanhf(afxReal y_over_x)
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

_AFXINL afxReal64 AfxExp10(afxReal64 s)
{
    return AfxPow(10.0, s);
}

_AFXINL afxReal AfxExp10f(afxReal s)
{
    return AfxPowf(10.f, s);
}

// Returns sqrt(x).
// Results are undefined if x < 0.

_AFXINL afxReal64 AfxSqrt(afxReal64 s)
{
    return sqrt(s);
}

_AFXINL afxReal AfxSqrtf(afxReal s)
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

_AFXINL afxReal64 AfxRsqrt(afxReal64 s)
{
    return 1.0 / AfxSqrt(s);
}

_AFXINL afxReal AfxRsqrtf(afxReal s)
{
#ifdef MFX_USE_SIMD
    __m128 simd = _mm_set_ss(s);
    __m128 simd2 = _mm_rsqrt_ss(simd);
    return _mm_cvtss_f32(simd2);
#else
#   ifdef MFX_USE_CARMARK_RSQRT
    // Este código supostamente foi originado por Jonh Carmark/Id Software.
    // Ele faz uma rápida aproximação de 1 / sqrt(s).
    afxReal v_half = s * 0.5f;
    afxNat i = *(afxNat *)&s;
    i = 0x5f3759df - (i >> 1);
    s = *(afxReal *)&i;
    return s * (1.5f - v_half * s * s);
#   else
    return 1.f / AfxSqrtf(s);
#   endif
#endif
}
