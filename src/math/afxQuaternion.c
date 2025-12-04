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
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"

_AFX afxQuat const AFX_QUAT_ZERO = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(0) };
_AFX afxQuat const AFX_QUAT_IDENTITY = { AFX_REAL(0), AFX_REAL(0), AFX_REAL(0), AFX_REAL(1) };

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatReset(afxQuat q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AfxQuatCopy(q, AFX_QUAT_IDENTITY);
}

_AFXINL void AfxQuatZero(afxQuat q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AfxQuatCopy(q, AFX_QUAT_ZERO);
}

_AFXINL void AfxQuatSet(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AfxV4dSet(q, x, y, z, w);
}

_AFXINL void AfxQuatReconstructV3d(afxQuat q, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);

    // Since rotation quaternions are normalized, there are only 3 degrees of freedom instead of 4. 
    // Thus you only really need to store 3 components of the quaternion, and the 4th can be calculated using the formula
    // We can compress a Quaternion down to three elements by making sure one of the them is greater than or equal to zero. 
    // We can then rebuild the missing element with this function.

    afxReal t = 1.f - AfxV3dDot(in, in);
    AfxV3dCopy(q, in);
    q[3] = t < 0.f ? 0.f : -AfxSqrtf(t);
}

_AFXINL void AfxQuatRotationM3d(afxQuat q, afxM3d const m)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(q, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrtf(fourXSqr);

            q[0] = fourXSqr * inv4x;
            q[1] = (m[0][1] + m[1][0]) * inv4x;
            q[2] = (m[0][2] + m[2][0]) * inv4x;
            q[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrtf(fourYSqr);

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
            afxReal inv4z = 0.5f / AfxSqrtf(fourZSqr);

            q[0] = (m[0][2] + m[2][0]) * inv4z;
            q[1] = (m[1][2] + m[2][1]) * inv4z;
            q[2] = fourZSqr * inv4z;
            q[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrtf(fourWSqr);

            q[0] = (m[1][2] - m[2][1]) * inv4w;
            q[1] = (m[2][0] - m[0][2]) * inv4w;
            q[2] = (m[0][1] - m[1][0]) * inv4w;
            q[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatRotationM4d(afxQuat q, afxM4d const m)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(q, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrtf(fourXSqr);

            q[0] = fourXSqr * inv4x;
            q[1] = (m[0][1] + m[1][0]) * inv4x;
            q[2] = (m[0][2] + m[2][0]) * inv4x;
            q[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrtf(fourYSqr);

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
            afxReal inv4z = 0.5f / AfxSqrtf(fourZSqr);

            q[0] = (m[0][2] + m[2][0]) * inv4z;
            q[1] = (m[1][2] + m[2][1]) * inv4z;
            q[2] = fourZSqr * inv4z;
            q[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrtf(fourWSqr);

            q[0] = (m[1][2] - m[2][1]) * inv4w;
            q[1] = (m[2][0] - m[0][2]) * inv4w;
            q[2] = (m[0][1] - m[1][0]) * inv4w;
            q[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatRotationAxial(afxQuat q, afxV3d const axis, afxReal phi)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(axis); // radians
    
    afxReal s, c;
    AfxCosSinf(phi * AFX_REAL(0.5), &c, &s);
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = c;
    AfxQuatNormalize(q, q); // reduz erros causados por AfxSinf() e AfxCosf().
}

_AFXINL void AfxQuatRotationEuler(afxQuat q, afxV3d const pitchYawRoll)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationRollPitchYawFromVector(FXMVECTOR Angles)
    /*
        WARNING:

        Qwadro assumes pitchYawRoll vector -> [pitch, yaw, roll], which typically means:
            pitch -> rotation around X-axis (elevation)
            yaw -> rotation around Y-axis (azimuth)
            roll -> rotation around Z-axis

        This is the standard aeronautical convention, and it matches Yaw-Pitch-Roll rotation order if applied as:
        Q = Q_roll * Q_pitch * Q_yaw

        This uses the well-established formula for composing a quaternion from Euler angles in Y-X-Z rotation order, assuming:
            First rotate about Y (azimuth)
            Then rotate about X (elevation)
            Then rotate about Z (roll)
        Which is exactly what works for a spherical camera system with orientation driven by azimuth, elevation, and roll.

        But DirectXMath (XMQuaternionRotationRollPitchYawFromVector) assumes a Roll-Pitch-Yaw (Z-X-Y) vector,
        and parameters are passed as:
        XMQuaternionRotationRollPitchYaw(roll, pitch, yaw)
        So if you're feeding [pitch, yaw, roll] into a DXMath function that expects [roll, pitch, yaw], 
        you'll get completely wrong orientations.


    */

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(pitchYawRoll);

    afxReal const halfpitch = pitchYawRoll[0] * 0.5f; // rotation around X
    afxReal cp = AfxCosf(halfpitch);
    afxReal sp = AfxSinf(halfpitch);
    afxReal const halfyaw = pitchYawRoll[1] * 0.5f; // rotation around Y
    afxReal cy = AfxCosf(halfyaw);
    afxReal sy = AfxSinf(halfyaw);
    afxReal const halfroll = pitchYawRoll[2] * 0.5f; // rotation around Z
    afxReal cr = AfxCosf(halfroll);
    afxReal sr = AfxSinf(halfroll);

#if 0 // DXMath wrong way
    q[0] = cr * sp * cy + sr * cp * sy;
    q[1] = cr * cp * sy - sr * sp * cy;
    q[2] = sr * cp * cy - cr * sp * sy;
    q[3] = cr * cp * cy + sr * sp * sy;
#endif
    // Quaternion composition: Q = Q_yaw * Q_pitch * Q_roll
    q[0] = sp * cy * cr + cp * sy * sr; // x
    q[1] = cp * sy * cr - sp * cy * sr; // y
    q[2] = cp * cy * sr - sp * sy * cr; // z
    q[3] = cp * cy * cr + sp * sy * sr; // w
}

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxBool AfxQuatIsIdentity(afxQuat const q)
{
    return AfxV4dIsIdentity(q);
}

_AFXINL afxBool AfxQuatIsEqual(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
    return AfxV4dAreEquals(q, other);
}

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatSwap(afxQuat q, afxQuat other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
    AFX_ASSERT(q != other);
    AfxSwapV4d(q, other);
}

_AFXINL void AfxQuatCopy(afxQuat q, afxQuat const in)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(q, in);
    AfxV4dCopy(q, in);
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxReal AfxQuatNormalize(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalize(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    return AfxV4dNormalize(q, in);
}

_AFXINL afxReal AfxQuatNormalizeEstimated(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalizeEst(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    return AfxV4dNormalizeEstimated(q, in);
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatAdd(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AfxV4dAdd(q, a, b);
}

_AFXINL void AfxQuatSub(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(b);
    AFX_ASSERT(a);
    AfxV4dSub(q, a, b);
}

_AFXINL void AfxQuatScale(afxQuat q, afxQuat const in, afxReal scalar)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    AfxV4dScale(q, in, scalar);
}

AFXINL void AfxQuatDiv(afxQuat q, afxQuat const in, afxReal dividend)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    AfxV4dDiv(q, in, (afxV4d) { dividend, dividend, dividend, dividend });
}

_AFXINL void AfxQuatMultiply(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(q != a);
    AFX_ASSERT(q);

#if 0

    // Should be compatible with XMVECTOR XMQuaternionMultiply(FXMVECTOR Q1, FXMVECTOR Q2)

    // The result represents the rotation A followed by the rotation B to be consistent with MatrixMulplity concatenation since this function is typically used to concatenate quaternions that represent rotations (i.e. it returns B*A).

    // Returns the product B*A (which is the concatenation of a rotation A followed by the rotation B)

    afxQuat const t = { b[0], b[1], b[2], b[3] };
    q[0] = (t[3] * a[0]) + (t[0] * a[3]) + (t[1] * a[2]) - (t[2] * a[1]);
    q[1] = (t[3] * a[1]) - (t[0] * a[2]) + (t[1] * a[3]) + (t[2] * a[0]);
    q[2] = (t[3] * a[2]) + (t[0] * a[1]) - (t[1] * a[0]) + (t[2] * a[3]);
    q[3] = (t[3] * a[3]) - (t[0] * a[0]) - (t[1] * a[1]) - (t[2] * a[2]);
#else

    // Compatible with QuaternionMultiply4(q, a, b)

    afxQuat const t = { b[0], b[1], b[2], b[3] };
    q[0] = (t[3] * a[0]) + ((t[0] * a[3]) +  (t[2] * a[1]) - (t[1] * a[2]));
    q[1] =                  (t[1] * a[3]) +                  (t[0] * a[2]) -  (t[2] * a[0]) + (t[3] * a[1]);
    q[2] = (t[3] * a[2]) + ((t[2] * a[3]) + ((t[1] * a[0]) - (t[0] * a[1])));
    q[3] = (t[3] * a[3]) - ((t[2] * a[2]) +  (t[1] * a[1]) + (t[0] * a[0]));
#endif
}

_AFXINL void AfxQuatMad(afxQuat q, afxQuat const add, afxQuat const mul, afxQuat const f)
{
    AfxV4dMad(q, add, mul, f);
}

_AFXINL void AfxQuatMads(afxQuat q, afxQuat const add, afxQuat const mul, afxReal lambda)
{
    AfxV4dMads(q, add, mul, lambda);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatConj(afxQuat q, afxQuat const in)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AfxV4dNegAffine(q, in);
}

_AFXINL void AfxQuatNeg(afxQuat q, afxQuat const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    AfxV4dNeg(q, in);
}

_AFXINL void AfxQuatInv(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionInverse(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);

    afxReal norm = AfxQuatSq(in);

    if (AfxIsGreaterThanEpsilon(norm)) AfxQuatZero(q);
    else
    {
        AfxQuatConj(q, in);
        afxV4d const v = { norm, norm, norm, norm };
        AfxV4dDiv(q, q, v);
    }
}

_AFXINL void AfxQuatGetImaginaryPart(afxQuat const q, afxV3d v)
{
    return AfxV3dCopy(v, q);
}

_AFXINL void AfxQuatGetBasisVector0(afxQuat const q, afxV3d v)
{
    afxReal const x2 = q[0] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[3] * w2) - 1.0  + q[0] * x2,
                    ( q[2] * w2)        + q[1] * x2,
                    (-q[1] * w2)        + q[2] * x2);
}

_AFXINL void AfxQuatGetBasisVector1(afxQuat const q, afxV3d v)
{
    afxReal const y2 = q[1] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    (-q[2] * w2)        + q[0] * y2, 
                    ( q[3] * w2) - 1.0  + q[1] * y2, 
                    ( q[0] * w2)        + q[2] * y2);
}

_AFXINL void AfxQuatGetBasisVector2(afxQuat const q, afxV3d v)
{
    afxReal const z2 = q[2] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[1] * w2)        + q[0] * z2, 
                    (-q[0] * w2)        + q[1] * z2, 
                    ( q[3] * w2) - 1.0  + q[2] * z2);
}

_AFXINL void AfxQuatGetInvBasisVector0(afxQuat const q, afxV3d v)
{
    afxReal const x2 = q[0] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[3] * w2) - 1.0  + q[0] * x2, 
                    (-q[2] * w2)        + q[1] * x2, 
                    ( q[1] * w2)        + q[2] * x2);
}

_AFXINL void AfxQuatGetInvBasisVector1(afxQuat const q, afxV3d v)
{
    afxReal const y2 = q[1] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[2] * w2)        + q[0] * y2, 
                    ( q[3] * w2) - 1.0  + q[1] * y2, 
                    (-q[0] * w2)        + q[2] * y2);
}

_AFXINL void AfxQuatGetInvBasisVector2(afxQuat const q, afxV3d v)
{
    afxReal const z2 = q[2] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    (-q[1] * w2)        + q[0] * z2, 
                    ( q[0] * w2)        + q[1] * z2, 
                    ( q[3] * w2) - 1.0  + q[2] * z2);
}

_AFXINL afxReal AfxQuatDot(afxQuat const q, afxQuat const other)
{
    // Should be compatible with  XMVECTOR XMQuaternionDot(FXMVECTOR Q1, FXMVECTOR Q2)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
    return AfxV4dDot(q, other);
}

_AFXINL afxReal AfxQuatSq(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionLengthSq(FXMVECTOR Q)

    afxError err = { 0 };
    return AfxV4dSq(q);
}

_AFXINL afxReal AfxQuatMag(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionLength(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxV4dMag(q);
}

_AFXINL afxReal AfxQuatMagRecip(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionReciprocalLength(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxV4dMagRecip(q);
}

_AFXINL afxReal AfxQuatAngle(afxQuat const q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxAcosf(q[3]) * 2.0;
}

_AFXINL afxReal AfxQuatAngle2(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxAcosf(AfxQuatDot(q, other)) * 2.0;
}

_AFXINL void AfxQuatLerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/02/quaternions.html

    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(q);

    afxReal f = AFX_REAL(1) - percent;
    
    if (AfxQuatDot(a, b) < 0.f)
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
    AfxQuatNormalize(q, q);
}

_AFXINL void AfxQuatSlerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(q);

    if (AfxRealIsEquivalent(percent, AFX_REAL(0))) AfxQuatCopy(q, a);
    else
    {
        if (AfxRealIsEquivalent(percent, AFX_REAL(1))) AfxQuatCopy(q, b);
        else
        {
            // if they are close q parallel, use LERP, This avoids div/0. At small angles, the slerp a lerp are the same.
            afxReal dot = AfxQuatDot(a, b);

            if (AfxRealIsEquivalent(dot, AFX_REAL(1))) AfxQuatLerp(q, a, q, percent);
            else
            {
                // if dot is negative, they are "pointing" away from one another, use the shortest arc instead (reverse end a start)
                // This has the effect of changing the direction of travel around the sphere beginning with "end" a going the b way around the sphere.

                if (dot < AFX_REAL(0))
                {
                    afxQuat neg;
                    AfxQuatNeg(neg, a);
                    AfxQuatSlerp(q, a, neg, percent);
                    AfxQuatNeg(q, q);
                }
                else
                {
                    // keep the dot product in the range that acos canv handle (shouldn't get here)
                    dot = AfxClampd(dot, AFX_REAL(-1), AFX_REAL(1));
                    afxReal theta = AfxAcosf(dot); // the angle between start a end in radians
                    afxReal s = AfxSinf(theta), f1 = AfxSinf((AFX_REAL(1) - percent) * theta) / s, f2 = AfxSinf(percent * theta) / s; // compute negative a positive

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

_AFXINL void AfxAssimilateQuat(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxQuat const in[], afxQuat out[])
{
    // Should be compatible with void InPlaceSimilarityTransformOrientation

    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxQuatMag(in[i]);
        AfxQuatScale(out[i], in[i], 1.f / len);

        afxM3d rm, tmp;
        AfxM3dRotationFromQuat(rm, out[i]);
        AfxM3dMultiply(tmp, ltm, rm);
        AfxM3dMultiply(rm, tmp, iltm);
        AfxQuatRotationM3d(out[i], rm);

        AfxQuatScale(out[i], out[i], len);
    }
}

_AFXINL void AfxQuatRotateV3dArray(afxQuat const q, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);
    AFX_ASSERT(q != in[0]);
    AFX_ASSERT(q != out[0]);

    // Transform a vector using a rotation expressed as a unit quaternion

#if 0

    // Compatible with out = XMVector3Rotate(in, q)

    afxQuat b, c;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxQuat a = { in[i][0], in[i][1], in[i][2], 0 };
        AfxQuatConj(b, q);
        AfxQuatMultiply(c, b, a);
        AfxQuatMultiply(out[i], c, q);
    }
#else

    // Compatible with NormalQuaternionTransform3(in/out, q)

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal v2 = q[0] * q[0];
        afxReal v3 = q[1] * q[1];
        afxReal v4 = q[2] * q[2];
        afxReal v5 = q[1] * q[0];
        afxReal ac = q[2] * q[0];
        afxReal ad = q[3] * q[0];
        afxReal bc = q[2] * q[1];
        afxReal bd = q[1] * q[3];
        afxReal v10 = q[2] * q[3];
        afxReal v12 = in[i][0];
        afxReal v13 = in[i][2];
        afxReal z2 = in[i][1];
        afxReal x2 = v12 + v12;
        afxReal y2 = z2 + z2;
        afxReal Vector3a = v13 + v13;
        out[i][0] = (1.0 - (v4 + v3 + v4 + v3)) * v12 + (v5 - v10) * y2 + (bd + ac) * Vector3a;
        out[i][1] = (1.0 - (v4 + v2 + v4 + v2)) * z2 + (bc - ad) * Vector3a + (v10 + v5) * x2;
        out[i][2] = (1.0 - (v3 + v2 + v3 + v2)) * v13 + (ac - bd) * x2 + (bc + ad) * y2;
    }

#endif
}

_AFXINL void AfxQuatRotateInv(afxQuat const q, afxV3d const in, afxV3d out)
{
    afxReal const vx = 2.0 * in[0];
    afxReal const vy = 2.0 * in[1];
    afxReal const vz = 2.0 * in[2];
    afxReal const w2 = q[3] * q[3] - 0.5;
    afxReal const dot2 = (q[0] * vx + q[1] * vy + q[2] * vz);
    AfxV3dSet(out,  (vx * w2 - (q[1] * vz - q[2] * vy) * q[3] + q[0] * dot2), 
                    (vy * w2 - (q[2] * vx - q[0] * vz) * q[3] + q[1] * dot2),
                    (vz * w2 - (q[0] * vy - q[1] * vx) * q[3] + q[2] * dot2));
}

_AFXINL void AfxQuatExtractAxialRotation(afxQuat const q, afxV3d axis, afxReal *radians)
{
    // Should be compatible with void XMQuaternionToAxisAngle(XMVECTOR* pAxis, float* pAngle, FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(axis);
    AfxV3dCopy(axis, q);
    *radians = 2.f * AfxAcosf(q[3]);
}

////////////////////////////////////////////////////////////////////////////////
// Trigonometry                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatBarycentric(afxQuat q, afxQuat const a, afxQuat const b, afxQuat const c, afxReal f, afxReal g)
{
    // Should be compatible with  XMVECTOR XMQuaternionBaryCentric(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, float f, float g)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    afxReal s = f + g;

    if ((s < 0.00001f) && (s > -0.00001f)) AfxQuatCopy(q, a);
    else
    {
        afxQuat ab, ac;
        AfxQuatSlerp(ab, a, b, s);
        AfxQuatSlerp(ac, a, c, s);
        AfxQuatSlerp(q, ab, ac, g / s);
    }
}

_AFXINL void AfxQuatTangentM3d(afxQuat q, afxM3d const tbn)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(tbn);

    AfxQuatRotationM3d(q, tbn);
    AfxQuatNormalize(q, q);

    // Make sure QTangent is always positive

    if (q[3] < 0)
        q[3] = -q[3];

    // yosoygames.com.ar/wp/2018/03/vertex-formats-part-1-compression/

#if !0
    // As Crytek found out, this code is faulty when using SNORM. 
    // This is because when qTangent.w is exactly 0, there is no negative 0 for SNORM. 
    // Crytek’s solution was to add a bias, so that the sign of the bitangent is negative when q.w < bias, instead of testing q.w < 0. 
    // This means that q.w can never be 0. It’s a small precision sacrifice to solve an edge case.

    // Because '-0' sign information is lost when using integers, we need to apply a "bias"; while making sure the Quaternion stays normalized.
    // ** Also our shaders assume qTangent.w is never 0. **

    afxUnit const BITS = 16;
    afxReal const bias = 1.f / (2 ^ (BITS - 1) - 1);

    if (q[3] < bias)
    {
        afxReal normFactor = AfxSqrtf(1.f - bias * bias);
        AfxV3dScale(q, q, normFactor);
        q[3] = bias;
    }
#endif

    //If it's reflected, then make sure .w is negative.
    afxV3d naturalBinormal;
    AfxV3dCross(naturalBinormal, tbn[0], tbn[2]);

    if (AfxV3dDot(naturalBinormal, tbn[1]) <= 0)
        AfxQuatNeg(q, q);
}

_AFXINL void AfxQuatTangentFrame(afxQuat q, afxV3d const normal, afxV3d const tangent, afxV3d const bitangent)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(normal);
    AFX_ASSERT(tangent);
    AFX_ASSERT(bitangent);

    afxM3d tbn;
    AfxM3dSet(tbn, normal, tangent, bitangent);
    AfxQuatTangentM3d(q, tbn);
}

_AFXINL void AfxQuatLookTo(afxQuat q, afxV3d const from, afxV3d const to)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/03/quaternion-tricks.html
    
    afxV3d h;
    AfxV3dAdd(h, from, to);
    AfxV3dNormalize(h, h);

    q[3] = AfxV3dDot(from, h);
    q[0] = from[1] * h[2] - from[2] * h[1];
    q[1] = from[2] * h[0] - from[0] * h[2];
    q[2] = from[0] * h[1] - from[1] * h[0];
}

_AFXINL void AfxQuatFromAngularVelocity(afxQuat q, afxV3d const vel)
{
    afxError err = { 0 };
    AFX_ASSERT(vel);
    AFX_ASSERT(q);

    // Compatible with AngularVelocityToQuaternion(q, rot)

    afxV4d n;
    afxReal mag = AfxV3dMag(vel);
    AfxV3dScale(n, vel, 1.0 / mag);
    afxReal halfSq = mag * 0.5f;
    AfxV3dScale(q, n, AfxSinf(halfSq));
    q[3] = AfxCosf(halfSq);
}

/*
    The AfxQuatIntegrate function uses exponential map to compute deltaq = exp(0.5 w dt).
    This is a closed-form integration.The code computes rotation magnitude and forms a quaternion exponential.
    This is the exact solution of the quaternion ODE for constant angular velocity during dt.
    
    Pros:
    Much more accurate
    Stable even for large dt
    No need to normalize each step

    Cons:
    Slightly slower
    More code

    This function should be prefered over EULER variant when:
        dt is moderate or irregular
        angular velocities are high
        you want numerically stable integration (e.g., rigid-body sims, spacecraft attitude)
        you want physically correct results regardless of step size
*/

_AFXINL void AfxQuatIntegrate(afxQuat q, afxQuat const in, afxV3d const omega, afxReal dt)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT(omega);

    afxV3d theta;
    AfxV3dScale(theta, omega, dt * 0.5f);
    afxReal thetaMagSq = AfxV3dSq(theta);
    afxReal w, s;

    if (thetaMagSq * thetaMagSq / 24.f < AFX_EPSILON)
    {
        // small-angle Taylor series
        w = 1.0f - thetaMagSq * 0.5f;
        s = 1.0f - thetaMagSq / 6.0f;
    }
    else
    {
        afxReal thetaMag = AfxSqrt(thetaMagSq);
        w = AfxCos(thetaMag);
        s = AfxSinf(thetaMag) / thetaMag;
    }

    afxQuat deltaQ;
    // delta quaternion (rotation over this timestep)
    AfxV3dScale(deltaQ, theta, s);
    deltaQ[3] = w;

    // Integrate.
    AfxQuatMultiply(q, deltaQ, in);
}

/*
    The AfxQuatIntegrateEULER function for first-order (explicit Euler) quaternion integration.
    This uses the quaternion differential equation and integrates it using explicit Euler.
    This is the simplest possible method.

    Pros: fast, small code
    Cons: low accuracy for large rotations or large dt, 
    not stable for high angular velocities (requires normalization).

    This method is accurate only when: |w|dt<<1

    This function is mathematically equivalent to AfxQuatIntegrate using exponential map for small dt.

    This function should be prefered over EXP-MAP variant when:
        dt is very small (e.g., physics running at 1000 Hz)
        angular velocities are small
        you want maximum speed
        some accuracy loss is acceptable
*/

_AFXINL void AfxQuatIntegrateEULER(afxQuat q, afxQuat const in, afxV3d const omega, afxReal dt)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT(omega);

    afxQuat wq;
    AfxQuatSet(wq, omega[0], omega[1], omega[2], 0);
    // dq = 0.5 * (wq * q)
    afxQuat dq;
    AfxQuatMultiply(dq, wq, q);
    AfxQuatScale(dq, dq, 0.5);

    // integrate
    AfxV4dMads(q, q, dq, dt);

    // renormalize
    AfxQuatNormalize(q, q);
}

// Utils

_AFXINL void AfxEnforceQuaternionContinuity(afxUnit cnt, afxQuat q[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(q);
    
    // Compatible with EnsureQuaternionContinuity(cnt, q)

    afxQuat last = { 0.0, 0.0, 0.0, 0.0 };

    for (afxUnit i = cnt; i-- > 0;)
    {
        afxQuat t;
        AfxQuatCopy(t, q[i]);

        if (AfxQuatDot(t, last) < 0.0)
            AfxQuatNeg(t, t);

        AfxQuatCopy(last, t);
        AfxQuatCopy(q[i], t);
    }
}
