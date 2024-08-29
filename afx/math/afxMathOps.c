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

#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"

/// Converting a Float to a Half-Float
/// The following routines describe how to convert a single-precision floating-point number to a half-float value, and vice versa.  
/// The conversion routines are useful when vertex attributes are generated using single-precision floating-point calculations but then converted to half-floats before they are used as vertex attributes:

// -15 stored using a single-precision bias of 127
afxNat32 const REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP = 0x38000000;
// max exponent value in single precision that will be converted to Inf or NaN when stored as a half-float
afxNat32 const REAL16_MAX_BIASED_EXP_AS_SINGLE_FP_EXP = 0x47800000;
// 255 is the max exponent biased value
afxNat32 const FLOAT_MAX_BIASED_EXP = (0x1F << 23);
afxNat32 const REAL16_MAX_BIASED_EXP = (0x1F << 10);

_AFXINL afxReal16 AfxPackReal16(afxReal f)
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
    else if (exp <= REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) // check if exponent is <= -15
    {
        // store a denorm half-float value or zero
        exp = (REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP - exp) >> 23;
        mantissa >>= (14 + exp);

        hf = (((afxReal16)sign) << 15) | (afxReal16)(mantissa);
    }
    else
    {
        hf = (((afxReal16)sign) << 15) | (afxReal16)((exp - REAL16_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) >> 13) | (afxReal16)(mantissa >> 13);
    }
    return hf;
}

_AFXINL afxReal32 AfxUnpackReal16(afxReal16 hf)
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
    return (afxInt16)(AfxClampd(v >= 0.0f ? (v * 32767.0f + 0.5f) : (v * 32767.0f - 0.5f), -32768.0f, 32767.0f));
}

_AFXINL afxReal AfxSnorm16ToReal32(afxInt16 v)
{
    // -32768 & -32767 both map to -1 according to D3D10 rules.
    return AfxMax(v / 32767.0f, -1.0f);
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxNdcV2d(afxV2d v, afxV2d const from, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(from);
    AfxAssert(total);
    v[0] = AfxNdcf(from[0], total[0]);
    v[1] = AfxNdcf(from[1], total[1]);
}

_AFXINL void AfxUnndcV2d(afxV2d v, afxV2d const from, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(from);
    AfxAssert(total);
    v[0] = AfxUnndcf(from[0], total[0]);
    v[1] = AfxUnndcf(from[1], total[1]);
}
