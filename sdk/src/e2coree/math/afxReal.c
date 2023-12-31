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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "qwadro/math/afxReal.h"

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

_AFXINL afxReal AfxRecip(afxReal value)
{
    return (afxReal)1.f / value;
}

_AFXINL afxReal AfxLerp(afxReal a, afxReal b, afxReal time)
{
    return  a * ((afxReal)1.f - time) + time * b;
}

_AFXINL afxReal AfxClamp(afxReal value, afxReal min, afxReal max)
{
    return (value < min ? min : (value > max ? max : value));
}

_AFXINL afxReal AfxSq(afxReal s)
{
    return s * s;
}

_AFXINL afxReal AfxFloor(afxReal s)
{
    return (afxReal)floor((afxReal)s);
}

_AFXINL afxReal AfxCeil(afxReal s)
{
    return (afxReal)ceil((afxReal)s);
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

_AFXINL afxReal AfxRad(afxReal degrees)
{
    return degrees * (AFX_PI / AfxScalar(180));
}

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

_AFXINL afxReal AfxDeg(afxReal radians)
{
    return radians * (AfxScalar(180) / AFX_PI);
}

// The standard trigonometric sine function.

_AFXINL afxReal AfxSin(afxReal radians)
{
    return sinf(radians);
}

// The standard trigonometric cosine function.

_AFXINL afxReal AfxCos(afxReal radians)
{
    return cosf(radians);
}

// A combined cosine and sine function

_AFXINL void AfxCosSin(afxReal radians, afxReal* cos, afxReal* sin)
{
    *cos = AfxCos(radians);
    *sin = AfxSin(radians);
}

// The standard trigonometric tangent function.

_AFXINL afxReal AfxTan(afxReal radians)
{
    return tanf(radians);
}

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

_AFXINL afxReal AfxAsin(afxReal x)
{
    return asinf(x);
}

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

_AFXINL afxReal AfxAcos(afxReal x)
{
    return acosf(x);
}

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

_AFXINL afxReal AfxAtan(afxReal y_over_x)
{
    return atanf(y_over_x);
}

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

_AFXINL afxReal AfxAtan2(afxReal y, afxReal x)
{
    return atan2f(y, x);
}

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.

_AFXINL afxReal AfxSinh(afxReal x)
{
    return sinhf(x);
}

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.

_AFXINL afxReal AfxCosh(afxReal x)
{
    return coshf(x);
}

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).

_AFXINL afxReal AfxTanh(afxReal x)
{
    return tanhf(x);
}

// Arc hyperbolic sine; returns the inverse of sinh.

_AFXINL afxReal AfxAsinh(afxReal x)
{
    return asinhf(x);
}

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.

_AFXINL afxReal AfxAcosh(afxReal x)
{
    return acoshf(x);
}

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.

_AFXINL afxReal AfxAtanh(afxReal y_over_x)
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

_AFXINL afxReal AfxLog(afxReal s)
{
    return logf(s);
}

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

_AFXINL afxReal AfxLog2(afxReal s)
{
    return log2f(s);
}

_AFXINL afxReal AfxLog10(afxReal s)
{
    return log10f(s);
}

// Returns the natural exponentiation of x; e^x.

_AFXINL afxReal AfxExp(afxReal s)
{
    return expf(s);
}

// Returns 2 raised to the x power; 2^x.

_AFXINL afxReal AfxExp2(afxReal s)
{
    return exp2f(s);
}

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

_AFXINL afxReal AfxPow(afxReal base, afxReal exp)
{
    return AfxExp(exp * AfxLog(base));
}

_AFXINL afxReal AfxExp10(afxReal s)
{
    return AfxPow(10.f, s);
}

// Returns sqrt(x).
// Results are undefined if x < 0.

_AFXINL afxReal AfxSqrt(afxReal s)
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

_AFXINL afxReal AfxRsqrt(afxReal s)
{
#ifdef MFX_USE_SIMD
    __m128 simd = _mm_set_ss(s);
    __m128 simd2 = _mm_rsqrt_ss(simd);
    return _mm_cvtss_f32(simd2);
#else
#if 0
    // Este código supostamente foi originado por Jonh Carmark/Id Software.
    // Ele faz uma rápida aproximação de 1 / sqrt(s).
    afxReal v_half = s * 0.5f;
    afxNat i = *(afxNat *)&s;
    i = 0x5f3759df - (i >> 1);
    s = *(afxReal *)&i;
    return s * (1.5f - v_half * s * s);
#else
    return AfxRecip(AfxSqrt(s));
#endif
#endif
}

/// Converting a Float to a Half-Float
/// The following routines describe how to convert a single-precision floating-point number to a half-float value, and vice versa.  
/// The conversion routines are useful when vertex attributes are generated using single-precision floating-point calculations but then converted to half-floats before they are used as vertex attributes:

// −15 stored using a single-precision bias of 127
afxNat32 const REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP = 0x38000000;
// max exponent value in single precision that will be converted to Inf orNaN when stored as a half-float
afxNat32 const REAL16_MAX_BIASED_EXP_AS_SINGLE_FP_EXP = 0x47800000;
// 255 is the max exponent biased value
afxNat32 const FLOAT_MAX_BIASED_EXP = (0x1F << 23);
afxNat32 const REAL16_MAX_BIASED_EXP = (0x1F << 10);

_AFXINL afxReal16 AfxReal32To16(afxReal f)
{
    afxNat32 x = *(afxNat32*)&f;
    afxNat32 sign = (afxNat16)(x >> 31);    
    afxReal16 hf;

    // get mantissa
    afxNat32 mantissa = x & ((1 << 23) - 1);
    // get exponent bits
    afxNat32 exp = x & FLOAT_MAX_BIASED_EXP;

    if (exp >= REAL16_MAX_BIASED_EXP_AS_SINGLE_FP_EXP)
    {
        // check if the original single-precision float number is a NaN

        if (mantissa && (exp == FLOAT_MAX_BIASED_EXP))
        {
            // we have a single-precision NaN
            mantissa = (1 << 23) - 1;
        }
        else
        {
            // 16-bit half-float representation stores number
            // as Inf mantissa = 0;
        }

        hf = (((afxReal16)sign) << 15) | (afxReal16)(REAL16_MAX_BIASED_EXP) | (afxReal16)(mantissa >> 13);
    }
    else if (exp <= REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) // check if exponent is <= −15
    {
        // store a denorm half-float value or zero
        exp = (REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP - exp) >> 23;
        mantissa >>= (14 + exp);

        hf = (((afxReal16)sign) << 15) | (afxReal16)(mantissa);
    }
    else
    {
        hf = (((afxReal16)sign) << 15) | (afxReal16) ((exp - REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) >> 13) | (afxReal16)(mantissa >> 13);
    }
    return hf;
}

_AFXINL afxReal32 AfxReal16To32(afxReal16 hf)
{
    afxNat32 mantissa = (afxNat32)(hf & ((1 << 10) - 1));
    afxNat32 exp = (afxNat32)(hf & REAL16_MAX_BIASED_EXP);
    
    if (exp == REAL16_MAX_BIASED_EXP)
    {
        // we have a half-float NaN or Inf
        // half-float NaNs will be converted to a single-precision NaN
        // half-float Infs will be converted to a single-precision Inf

        exp = FLOAT_MAX_BIASED_EXP;

        if (mantissa)
            mantissa = (1 << 23) - 1; // set all bits to indicate a NaN
    }
    else if (exp == 0x0)
    {
        // convert half-float zero/denorm to single-precision value

        if (mantissa)
        {
            mantissa <<= 1;
            exp = REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;

            // check for leading 1 in denorm mantissa
            while ((mantissa & (1 << 10)) == 0)
            {
                // for every leading 0, decrement single-precision exponent by 1 and shift half-float mantissa value to the left mantissa <<= 1;

                exp -= (1 << 23);
            }

            // clamp the mantissa to 10 bits
            mantissa &= ((1 << 10) - 1);
            // shift left to generate single-precision mantissa of 23-bits mantissa <<= 13;
        }
    }
    else
    {
        // shift left to generate single-precision mantissa of 23-bits mantissa <<= 13;
        // generate single-precision biased exponent value
        exp = (exp << 13) + REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
    }

    afxNat32 sign = (afxNat32)(hf >> 15);
    afxNat32 f = (sign << 31) | exp | mantissa;
    return *((afxReal*)&f);
}

_AFXINL afxInt16 AfxReal32ToSnorm(afxReal v)
{
    //According to D3D10 rules, the value "-1.0f" has two representations: 0x1000 and 0x10001
    //This allows everyone to convert by just multiplying by 32767 instead of multiplying the negative values by 32768 and 32767 for positive.
    return (afxInt16)(AfxClamp(v >= 0.0f ? (v * 32767.0f + 0.5f) : (v * 32767.0f - 0.5f), -32768.0f, 32767.0f));
}

_AFXINL afxReal AfxSnorm16ToReal32(afxInt16 v)
{
    // -32768 & -32767 both map to -1 according to D3D10 rules.
    return AfxMax(v / 32767.0f, -1.0f);
}
