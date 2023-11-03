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

#include "afx/math/afxMathDefs.h"
#include "afx/math/afxQuaternion.h"
#include "afx/math/afxReal.h"
#include "afx/math/afxMatrix.h"
#include "afx/math/afxVector.h"

_AFX afxQuat const AFX_QUAT_ZERO = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(0) };
_AFX afxQuat const AFX_QUAT_IDENTITY = { AfxScalar(0), AfxScalar(0), AfxScalar(0), AfxScalar(1) };

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxResetQuat(afxQuat q)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxCopyQuat(q, AFX_QUAT_IDENTITY);
}

_AFXINL void AfxZeroQuat(afxQuat q)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxCopyQuat(q, AFX_QUAT_ZERO);
}

_AFXINL void AfxSetQuat(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    q[0] = x;
    q[1] = y;
    q[2] = z;
    q[3] = w;
}

_AFXINL void AfxQuatFromV3d(afxQuat q, afxReal const in[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(in);

    // Since rotation quaternions are normalized, there are only 3 degrees of freedom instead of 4. 
    // Thus you only really need to store 3 components of the quaternion, and the 4th can be calculated using the formula
    // We can compress a Quaternion down to three elements by making sure one of the them is greater than or equal to zero. We can then rebuild the missing element with this function.

    afxReal t = 1.f - AfxDotV3d(in, in);
    AfxCopyV3d(q, in);
    q[3] = t < 0.f ? 0.f : -AfxSqrt(t);
}

_AFXINL void AfxQuatFromM3d(afxQuat q, afxReal const m[3][3])
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(m);
    AfxAssertDiffSoft(q, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrt(fourXSqr);

            q[0] = fourXSqr * inv4x;
            q[1] = (m[0][1] + m[1][0]) * inv4x;
            q[2] = (m[0][2] + m[2][0]) * inv4x;
            q[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrt(fourYSqr);

            q[0] = (m[0][1] + m[1][0]) * inv4y;
            q[1] = fourYSqr * inv4y;
            q[2] = (m[1][2] + m[2][1]) * inv4y;
            q[3] = (m[2][0] - m[0][2]) * inv4y;
        }
    }
    else  // z^2 + w^2 >= x^2 + y^2
    {
        afxReal sum10 = m[1][1] + m[0][0];
        afxReal opr22 = 1.f + r22;

        if (sum10 <= 0.f)  // z^2 >= w^2
        {
            afxReal fourZSqr = opr22 - sum10;
            afxReal inv4z = 0.5f / AfxSqrt(fourZSqr);

            q[0] = (m[0][2] + m[2][0]) * inv4z;
            q[1] = (m[1][2] + m[2][1]) * inv4z;
            q[2] = fourZSqr * inv4z;
            q[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrt(fourWSqr);

            q[0] = (m[1][2] - m[2][1]) * inv4w;
            q[1] = (m[2][0] - m[0][2]) * inv4w;
            q[2] = (m[0][1] - m[1][0]) * inv4w;
            q[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatFromLinearM4d(afxQuat q, afxReal const m[4][4])
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(m);
    AfxAssertDiffSoft(q, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrt(fourXSqr);

            q[0] = fourXSqr * inv4x;
            q[1] = (m[0][1] + m[1][0]) * inv4x;
            q[2] = (m[0][2] + m[2][0]) * inv4x;
            q[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrt(fourYSqr);

            q[0] = (m[0][1] + m[1][0]) * inv4y;
            q[1] = fourYSqr * inv4y;
            q[2] = (m[1][2] + m[2][1]) * inv4y;
            q[3] = (m[2][0] - m[0][2]) * inv4y;
        }
    }
    else  // z^2 + w^2 >= x^2 + y^2
    {
        afxReal sum10 = m[1][1] + m[0][0];
        afxReal opr22 = 1.f + r22;

        if (sum10 <= 0.f)  // z^2 >= w^2
        {
            afxReal fourZSqr = opr22 - sum10;
            afxReal inv4z = 0.5f / AfxSqrt(fourZSqr);

            q[0] = (m[0][2] + m[2][0]) * inv4z;
            q[1] = (m[1][2] + m[2][1]) * inv4z;
            q[2] = fourZSqr * inv4z;
            q[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrt(fourWSqr);

            q[0] = (m[1][2] - m[2][1]) * inv4w;
            q[1] = (m[2][0] - m[0][2]) * inv4w;
            q[2] = (m[0][1] - m[1][0]) * inv4w;
            q[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatFromAxisAngle(afxQuat q, afxReal const axis[3], afxReal phi)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(axis); // radians
    afxReal s = AfxSin(phi * AfxScalar(0.5));
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = AfxCos(phi * AfxScalar(0.5));
    AfxNormalizeQuat(q, q); // reduz erros causados por AfxSin() e AfxCos().
}

_AFXINL void AfxQuatFromEuler(afxQuat q, afxReal const pitchYawRoll[3])
{
    // Should be compatible with XMVECTOR XMQuaternionRotationRollPitchYawFromVector(FXMVECTOR Angles)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(pitchYawRoll);

    afxReal const halfpitch = pitchYawRoll[0] * 0.5f;
    afxReal cp = AfxCos(halfpitch);
    afxReal sp = AfxSin(halfpitch);
    afxReal const halfyaw = pitchYawRoll[1] * 0.5f;
    afxReal cy = AfxCos(halfyaw);
    afxReal sy = AfxSin(halfyaw);
    afxReal const halfroll = pitchYawRoll[2] * 0.5f;
    afxReal cr = AfxCos(halfroll);
    afxReal sr = AfxSin(halfroll);

    q[0] = cr * sp * cy + sr * cp * sy;
    q[1] = cr * cp * sy - sr * sp * cy;
    q[2] = sr * cp * cy - cr * sp * sy;
    q[3] = cr * cp * cy + sr * sp * sy;
}

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxBool AfxQuatIsIdentity(afxQuat const q)
{
    return (q[0] + q[1] + q[2]) == 0 && (q[3] == 1);
}

_AFXINL afxBool AfxQuatIsEqual(afxQuat const q, afxQuat const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    return AfxV4dAreEquals(q, other);
}

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxSwapQuat(afxQuat q, afxQuat other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    AfxAssert(q != other);
    AfxSwapV4d(q, other);
}

_AFXINL void AfxCopyQuat(afxQuat q, afxQuat const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(in);
    AfxAssertDiffSoft(q, in);
    AfxCopyV4d(q, in);
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxReal AfxNormalizeQuat(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalize(FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(q);
    return AfxGetNormalizedV4d(q, in);
}

_AFXINL afxReal AfxEstimateNormalizedQuat(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalizeEst(FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(q);
    return AfxEstimateNormalizedV4d(q, in);
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAddQuat(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(a);
    AfxAddV4d(q, a, b);
}

_AFXINL void AfxSubQuat(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(b);
    AfxAssert(a);
    AfxSubV4d(q, a, b);
}

_AFXINL void AfxScaleQuat(afxQuat q, afxQuat const in, afxReal scalar)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(q);
    AfxScaleV4d(q, in, scalar);
}

AFXINL void AfxDivQuat(afxQuat q, afxQuat const in, afxReal dividend)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(q);
    AfxDivV4d(q, in, (afxV4d) { dividend, dividend, dividend, dividend });
}

_AFXINL void AfxMultiplyQuat(afxQuat q, afxQuat const a, afxQuat const b)
{
    // Should be compatible with XMVECTOR XMQuaternionMultiply(FXMVECTOR Q1, FXMVECTOR Q2)

    // The result represents the rotation A followed by the rotation B to be consistent with MatrixMulplity concatenation since this function is typically used to concatenate quaternions that represent rotations (i.e. it returns B*A).

    // Returns the product B*A (which is the concatenation of a rotation A followed by the rotation B)

    afxError err = AFX_ERR_NONE;
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(q != a);
    AfxAssert(q);

    afxQuat const t = { b[0], b[1], b[2], b[3] };
    q[0] = (t[3] * a[0]) + (t[0] * a[3]) + (t[1] * a[2]) - (t[2] * a[1]);
    q[1] = (t[3] * a[1]) - (t[0] * a[2]) + (t[1] * a[3]) + (t[2] * a[0]);
    q[2] = (t[3] * a[2]) + (t[0] * a[1]) - (t[1] * a[0]) + (t[2] * a[3]);
    q[3] = (t[3] * a[3]) - (t[0] * a[0]) - (t[1] * a[1]) - (t[2] * a[2]);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxConjugateQuat(afxQuat q, afxQuat const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(in);
    AfxNegateV3d(q, in);
    q[3] = in[3]; // w is preserved
}

_AFXINL void AfxNegateQuat(afxQuat q, afxQuat const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(q);
    AfxNegateV4d(q, in);
}

_AFXINL void AfxInvertQuat(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionInverse(FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(q);

    afxReal norm = AfxMagQuatSq(in);

    if (AfxIsGreaterThanEpsilon(norm)) AfxZeroQuat(q);
    else
    {
        AfxConjugateQuat(q, in);
        afxV4d const v = { norm, norm, norm, norm };
        AfxDivV4d(q, q, v);
    }
}

_AFXINL afxReal AfxDotQuat(afxQuat const q, afxQuat const other)
{
    // Should be compatible with  XMVECTOR XMQuaternionDot(FXMVECTOR Q1, FXMVECTOR Q2)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    return AfxDotV4d(q, other);
}

_AFXINL afxReal AfxMagQuatSq(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionLengthSq(FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    return AfxMagV4dSq(q);
}

_AFXINL afxReal AfxMagQuat(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionLength(FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    return AfxSqrt(AfxMagQuatSq(q));
}

_AFXINL afxReal AfxMagQuatRecip(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionReciprocalLength(FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    return AfxRsqrt(AfxMagQuatSq(q));
}

_AFXINL void AfxLerpQuat(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent)
{
    /// Should be compatible with physicsforgames.blogspot.com/2010/02/quaternions.html

    afxError err = AFX_ERR_NONE;
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(q);

    afxReal f = AfxScalar(1) - percent;
    
    if (AfxDotQuat(a, b) < 0.f)
    {
        q[3] = f * a[0] + percent * -b[3];
        q[0] = f * a[0] + percent * -b[0];
        q[1] = f * a[1] + percent * -b[1];
        q[2] = f * a[2] + percent * -b[2];
    }
    else
    {
        // mul & add
        q[3] = f * a[3] + percent * b[3];
        q[0] = f * a[0] + percent * b[0];
        q[1] = f * a[1] + percent * b[1];
        q[2] = f * a[2] + percent * b[2];
    }
    AfxNormalizeQuat(q, q);
}

_AFXINL void AfxSlerpQuat(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(q);

    if (AfxRealIsEquivalent(percent, AfxScalar(0))) AfxCopyQuat(q, a);
    else
    {
        if (AfxRealIsEquivalent(percent, AfxScalar(1))) AfxCopyQuat(q, b);
        else
        {
            // if they are close q parallel, use LERP, This avoids div/0. At small angles, the slerp a lerp are the same.
            afxReal dot = AfxDotQuat(a, b);

            if (AfxRealIsEquivalent(dot, AfxScalar(1))) AfxLerpQuat(q, a, q, percent);
            else
            {
                // if dot is negative, they are "pointing" away from one another, use the shortest arc instead (reverse end a start)
                // This has the effect of changing the direction of travel around the sphere beginning with "end" a going the b way around the sphere.

                if (dot < AfxScalar(0))
                {
                    afxQuat neg;
                    AfxNegateQuat(neg, a);
                    AfxSlerpQuat(q, a, neg, percent);
                    AfxNegateQuat(q, q);
                }
                else
                {
                    // keep the dot product in the range that acos canv handle (shouldn't get here)
                    dot = AfxClamp(dot, AfxScalar(-1), AfxScalar(1));
                    afxReal theta = AfxAcos(dot); // the angle between start a end in radians
                    afxReal s = AfxSin(theta), f1 = AfxSin((AfxScalar(1) - percent) * theta) / s, f2 = AfxSin(percent * theta) / s; // compute negative a positive

                    // mul & add
                    q[3] = f1 * a[3] + f2 * b[3];
                    q[0] = f1 * a[0] + f2 * b[0];
                    q[1] = f1 * a[1] + f2 * b[1];
                    q[2] = f1 * a[2] + f2 * b[2];
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Affine transformation                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAssimilateQuat(afxReal const lt[3][3], afxReal const ilt[3][3], afxNat cnt, afxQuat const in[], afxQuat out[])
{
    // Should be compatible with void InPlaceSimilarityTransformOrientation(const float *Linear3x3, const float *InverseLinear3x3, float *Orientation4)

    afxError err = NIL;
    AfxAssert(lt);
    AfxAssert(ilt);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxReal len = AfxMagQuat(in[i]);
        AfxScaleQuat(out[i], in[i], 1.f / len);

        afxM3d rm, tmp;
        AfxM3dFromQuat(rm, out[i]);
        AfxMultiplyM3d(tmp, lt, rm);
        AfxMultiplyM3d(rm, tmp, ilt);
        AfxQuatFromM3d(out[i], rm);

        AfxScaleQuat(out[i], out[i], len);
    }
}

_AFXINL void AfxRotateV3d(afxQuat const q, afxNat cnt, afxReal const in[][3], afxReal out[][3])
{
    /// Should be compatible with XMVECTOR XM_CALLCONV XMVector3Rotate(FXMVECTOR V, FXMVECTOR RotationQuaternion)

    // Transform a vector using a rotation expressed as a unit quaternion

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);
    AfxAssert(q != in[0]);
    AfxAssert(q != out[0]);
    

    for (afxNat i = 0; i < cnt; i++)
    {
        afxQuat a = { in[i][0], in[i][1], in[i][2], 0 }, b, c;
        AfxConjugateQuat(b, q);
        AfxMultiplyQuat(c, b, a);
        AfxMultiplyQuat(out[i], c, q);
    }
}

_AFXINL void AfxExtractAxialRotation(afxQuat const q, afxReal axis[3], afxReal *radians)
{
    // Should be compatible with void XMQuaternionToAxisAngle(XMVECTOR* pAxis, float* pAngle, FXMVECTOR Q)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(axis);
    AfxCopyV3d(axis, q);
    *radians = 2.f * AfxAcos(q[3]);
}

////////////////////////////////////////////////////////////////////////////////
// Trigonometry                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxBarycentricQuat(afxQuat q, afxQuat const a, afxQuat const b, afxQuat const c, afxReal f, afxReal g)
{
    // Should be compatible with  XMVECTOR XMQuaternionBaryCentric(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, float f, float g)

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    afxReal s = f + g;

    if ((s < 0.00001f) && (s > -0.00001f)) AfxCopyQuat(q, a);
    else
    {
        afxQuat ab, ac;
        AfxSlerpQuat(ab, a, b, s);
        AfxSlerpQuat(ac, a, c, s);
        AfxSlerpQuat(q, ab, ac, g / s);
    }
}

_AFXINL void AfxQuatFromTangentM3d(afxQuat q, afxReal const tbn[3][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(tbn);

    AfxQuatFromM3d(q, tbn);
    AfxNormalizeQuat(q, q);

    // Make sure QTangent is always positive

    if (q[3] < 0)
        q[3] = -q[3];

    // yosoygames.com.ar/wp/2018/03/vertex-formats-part-1-compression/

#if !0
    /// As Crytek found out, this code is faulty when using SNORM. 
    /// This is because when qTangent.w is exactly 0, there is no negative 0 for SNORM. 
    /// Crytek’s solution was to add a bias, so that the sign of the bitangent is negative when q.w < bias, instead of testing q.w < 0. 
    /// This means that q.w can never be 0. It’s a small precision sacrifice to solve an edge case.

    // Because '-0' sign information is lost when using integers, we need to apply a "bias"; while making sure the Quaternion stays normalized.
    // ** Also our shaders assume qTangent.w is never 0. **

    afxNat const BITS = 16;
    afxReal const bias = 1.0 / (2 ^ (BITS - 1) - 1);

    if (q[3] < bias)
    {
        afxReal normFactor = AfxSqrt(1 - bias * bias);
        AfxScaleV3d(q, q, normFactor);
        q[3] = bias;
    }
#endif

    //If it's reflected, then make sure .w is negative.
    afxV3d naturalBinormal;
    AfxCrossV3d(naturalBinormal, tbn[0], tbn[2]);

    if (AfxDotV3d(naturalBinormal, tbn[1]) <= 0)
        AfxNegateQuat(q, q);
}

_AFXINL void AfxQuatFromTangentFrame(afxQuat q, afxReal const normal[3], afxReal const tangent[3], afxReal const bitangent[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(normal);
    AfxAssert(tangent);
    AfxAssert(bitangent);

    afxM3d tbn;
    AfxSetM3d(tbn, normal, tangent, bitangent);
    AfxQuatFromTangentM3d(q, tbn);
}

_AFXINL void AfxQuatFromHeading(afxQuat q, afxReal const from[3], afxReal const to[3])
{
    /// Should be compatible with physicsforgames.blogspot.com/2010/03/quaternion-tricks.html
    
    afxV3d h;
    AfxAddV3d(h, from, to);
    AfxGetNormalizedV3d(h, h);

    q[3] = AfxDotV3d(from, h);
    q[0] = from[1] * h[2] - from[2] * h[1];
    q[1] = from[2] * h[0] - from[0] * h[2];
    q[2] = from[0] * h[1] - from[1] * h[0];
}

_AFXINL void AfxIntegrateQuat(afxQuat q, afxQuat const in, afxReal const omega[3], afxReal dt)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/02/quaternions.html

    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(in);
    AfxAssert(omega);

    afxQuat deltaQ;
    afxV3d theta;
    AfxScaleV3d(theta, omega, dt * 0.5f);

    afxReal thetaMagSq = AfxMagV3dSq(theta);
    afxReal s;

    if (thetaMagSq * thetaMagSq / 24.f < /*MACHINE_SMALL_FLOAT*/AFX_EPSILON)
    {
        deltaQ[3] = 1.f - thetaMagSq / 2.f;
        s = 1.f - thetaMagSq / 6.f;
    }
    else
    {
        afxReal thetaMag = sqrt(thetaMagSq);
        deltaQ[3] = cos(thetaMag);
        s = AfxSin(thetaMag) / thetaMag;
    }

    AfxScaleV3d(deltaQ, theta, s);

    AfxMultiplyQuat(q, deltaQ, in);
}

// Utils

_AFXINL void AfxEnsureQuaternionContinuity(afxNat cnt, afxQuat quat[])
{
    // Should be compatible with void EnsureQuaternionContinuity(int QuaternionCount, float *Quaternions)

    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(quat);

    afxQuat last = { 0.f, 0.f, 0.f, 0.f };
    
    for (afxNat i = 0, j = cnt; j-- > 0;)
    {
        afxQuat tmp;
        AfxCopyQuat(tmp, quat[i]);

        if (AfxDotQuat(tmp, last) < 0.0)
            AfxNegateQuat(tmp, tmp);

        AfxCopyQuat(last, tmp);
        AfxCopyQuat(quat[i], tmp);
    }
}
