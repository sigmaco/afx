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
    return !!(AFX_ABS(a - b) < AFX_EPSILON);
}

_AFXINL afxBool  AfxRealIsEqual(afxReal a, afxReal b, afxReal epsilon)
{
    return !!(AFX_ABS(a - b) < epsilon);
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
    return x * (AFX_REAL(1) / total);
}

_AFXINL afxReal64 AfxUnndc(afxReal64 x, afxReal64 total)
{
    afxError err = { 0 };
    AFX_ASSERT(AFX_REAL(0) <= x);
    AFX_ASSERT(AFX_REAL(1) >= x);
    return x * total;
}

_AFXINL afxReal AfxUnndcf(afxReal x, afxReal total)
{
    afxError err = { 0 };
    AFX_ASSERT(AFX_REAL(0) <= x);
    AFX_ASSERT(AFX_REAL(1) >= x);
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
    afxUnit i = *(afxUnit *)&s;
    i = 0x5f3759df - (i >> 1);
    s = *(afxReal *)&i;
    return s * (1.5f - v_half * s * s);
#   else
    return 1.f / AfxSqrtf(s);
#   endif
#endif
}

_AFXINL afxInt16 AfxReal32ToSnorm(afxReal v)
{
    //According to D3D10 rules, the value "-1.0f" has two representations: 0x1000 and 0x10001
    //This allows everyone to convert by just multiplying by 32767 instead of multiplying the negative values by 32768 and 32767 for positive.
    return (afxInt16)(AfxClampd(v >= 0.0f ? (v * 32767.0f + 0.5f) : (v * 32767.0f - 0.5f), -32768.0f, 32767.0f));
}
