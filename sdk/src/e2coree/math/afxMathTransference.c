/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxMathDefs.h"
#include "afx/math/afxScalar.h"

// Memory layout: row-major

// Copy

_AFXINL void AfxV2dCopy(afxV2d v, afxV2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
}

_AFXINL void AfxV3dCopy(afxV3d v, afxV3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
}

_AFXINL void AfxV4dCopy(afxV4d v, afxV4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(in);
    AfxAssertDiffSoft(v, in);
    v[0] = in[0];
    v[1] = in[1];
    v[2] = in[2];
    v[3] = in[3];
}

_AFXINL void AfxQuatCopy(afxQuat q, afxQuat const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(in);
    AfxAssertDiffSoft(q, in);
    q[0] = in[0];
    q[1] = in[1];
    q[2] = in[2];
    q[3] = in[3];
}

_AFXINL void AfxM2dCopy(afxM2d m, afxM2d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssertDiffSoft(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopy(afxM3d m, afxM3d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssertDiffSoft(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM4dCopy(afxM4d m, afxM4d const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(m);
    AfxAssertDiffSoft(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = in[0][3];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = in[1][3];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = in[2][3];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
    m[3][3] = in[3][3];
}

// Make (Combined Copy & Promote/Demote operations)

_AFXINL void AfxV3dMakeV2d(afxV3d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
}

_AFXINL void AfxV4dMakeV2d(afxV4d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
}

_AFXINL void AfxV2dMakeV3d(afxV2d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
    out[2] = AfxScalar(0);
}

_AFXINL void AfxV4dMakeV3d(afxV4d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
    out[2] = v[2];
}

_AFXINL void AfxV2dMakeV4d(afxV2d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
    out[2] = AfxScalar(0);
    out[3] = AfxScalar(0);
}

_AFXINL void AfxV3dMakeV4d(afxV3d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
    out[2] = v[2];
    out[3] = AfxScalar(0);
}

_AFXINL void AfxV2dMakePoint(afxV2d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
    out[2] = AfxScalar(0);
    out[3] = AfxScalar(1);
}

_AFXINL void AfxV3dMakePoint(afxV3d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiffSoft(out, v);
    out[0] = v[0];
    out[1] = v[1];
    out[2] = v[2];
    out[3] = AfxScalar(1);
}

_AFXINL void AfxM3dMakeM2d(afxM3d const m, afxM2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);
    AfxAssertDiff(out, m);

    out[0][0] = m[0][0];
    out[0][1] = m[0][1];

    out[1][0] = m[1][0];
    out[1][1] = m[1][1];
}

_AFXINL void AfxM4dMakeM2d(afxM4d const m, afxM2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);
    AfxAssertDiff(out, m);

    out[0][0] = m[0][0];
    out[0][1] = m[0][1];

    out[1][0] = m[1][0];
    out[1][1] = m[1][1];
}

_AFXINL void AfxM2dMakeM3d(afxM2d const m, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiff(out, m);

    out[0][0] = m[0][0];
    out[0][1] = m[0][1];
    out[0][2] = AfxScalar(0);

    out[1][0] = m[1][0];
    out[1][1] = m[1][1];
    out[1][2] = AfxScalar(0);

    out[2][0] = AfxScalar(0);
    out[2][1] = AfxScalar(0);
    out[2][2] = AfxScalar(1);
}

_AFXINL void AfxM4dMakeM3d(afxM4d const m, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);
    AfxAssertDiff(out, m);

    out[0][0] = m[0][0];
    out[0][1] = m[0][1];
    out[0][2] = m[0][2];

    out[1][0] = m[1][0];
    out[1][1] = m[1][1];
    out[1][2] = m[1][2];

    out[2][0] = m[2][0];
    out[2][1] = m[2][1];
    out[2][2] = m[2][2];
}

_AFXINL void AfxM2dMakeM4d(afxM2d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiff(out, m);

    out[0][0] = m[0][0];
    out[0][1] = m[0][1];
    out[0][2] = AfxScalar(0);
    out[0][3] = AfxScalar(0);

    out[1][0] = m[1][0];
    out[1][1] = m[1][1];
    out[1][2] = AfxScalar(0);
    out[1][3] = AfxScalar(0);

    out[2][0] = AfxScalar(0);
    out[2][1] = AfxScalar(0);
    out[2][2] = AfxScalar(1);
    out[2][3] = AfxScalar(0);

    out[3][0] = AfxScalar(0);
    out[3][1] = AfxScalar(0);
    out[3][2] = AfxScalar(0);
    out[3][3] = AfxScalar(1);
}

_AFXINL void AfxM3dMakeM4d(afxM3d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiff(out, m);

    out[0][0] = m[0][0];
    out[0][1] = m[0][1];
    out[0][2] = m[0][2];
    out[0][3] = AfxScalar(0);

    out[1][0] = m[1][0];
    out[1][1] = m[1][1];
    out[1][2] = m[1][2];
    out[1][3] = AfxScalar(0);

    out[2][0] = m[2][0];
    out[2][1] = m[2][1];
    out[2][2] = m[2][2];
    out[2][3] = AfxScalar(0);

    out[3][0] = AfxScalar(0);
    out[3][1] = AfxScalar(0);
    out[3][2] = AfxScalar(0);
    out[3][3] = AfxScalar(1);
}

_AFXINL void AfxV2dMakeScalingM3d(afxV2d const v, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = v[0];
    out[0][1] = 0.f;
    out[0][2] = 0.f;

    out[1][0] = 0.f;
    out[1][1] = v[1];
    out[1][2] = 0.f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = 1.f;
}

_AFXINL void AfxV3dMakeScalingM3d(afxV3d const v, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = v[0];
    out[0][1] = 0.f;
    out[0][2] = 0.f;

    out[1][0] = 0.f;
    out[1][1] = v[1];
    out[1][2] = 0.f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = v[2];
}

_AFXINL void AfxV4dMakeScalingM3d(afxV4d const v, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = v[0];
    out[0][1] = 0.f;
    out[0][2] = 0.f;

    out[1][0] = 0.f;
    out[1][1] = v[1];
    out[1][2] = 0.f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = v[2];
}

_AFXINL void AfxV2dMakeScalingM4d(afxV2d const v, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = v[0];
    out[0][1] = 0.f;
    out[0][2] = 0.f;
    out[0][3] = 0.f;

    out[1][0] = 0.f;
    out[1][1] = v[1];
    out[1][2] = 0.f;
    out[1][3] = 0.f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = 1.f;
    out[2][3] = 0.f;

    out[3][0] = 0.f;
    out[3][1] = 0.f;
    out[3][2] = 0.f;
    out[3][3] = 0.f;
}

_AFXINL void AfxV3dMakeScalingM4d(afxV3d const v, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = v[0];
    out[0][1] = 0.f;
    out[0][2] = 0.f;
    out[0][3] = 0.f;

    out[1][0] = 0.f;
    out[1][1] = v[1];
    out[1][2] = 0.f;
    out[1][3] = 0.f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = v[2];
    out[2][3] = 0.f;

    out[3][0] = 0.f;
    out[3][1] = 0.f;
    out[3][2] = 0.f;
    out[3][3] = 0.f;
}

_AFXINL void AfxV4dMakeScalingM4d(afxV4d const v, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = v[0];
    out[0][1] = 0.f;
    out[0][2] = 0.f;
    out[0][3] = 0.f;

    out[1][0] = 0.f;
    out[1][1] = v[1];
    out[1][2] = 0.f;
    out[1][3] = 0.f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = v[2];
    out[2][3] = 0.f;

    out[3][0] = 0.f;
    out[3][1] = 0.f;
    out[3][2] = 0.f;
    out[3][3] = 0.f;
}

_AFXINL void AfxM3dMakeRotationQuat(afxM3d const m, afxQuat out)
{
    // Inspired in one from DXMath (designed to represent rotation matrices)
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiffSoft(out, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrt(fourXSqr);

            out[0] = fourXSqr * inv4x;
            out[1] = (m[0][1] + m[1][0]) * inv4x;
            out[2] = (m[0][2] + m[2][0]) * inv4x;
            out[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrt(fourYSqr);

            out[0] = (m[0][1] + m[1][0]) * inv4y;
            out[1] = fourYSqr * inv4y;
            out[2] = (m[1][2] + m[2][1]) * inv4y;
            out[3] = (m[2][0] - m[0][2]) * inv4y;
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

            out[0] = (m[0][2] + m[2][0]) * inv4z;
            out[1] = (m[1][2] + m[2][1]) * inv4z;
            out[2] = fourZSqr * inv4z;
            out[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrt(fourWSqr);

            out[0] = (m[1][2] - m[2][1]) * inv4w;
            out[1] = (m[2][0] - m[0][2]) * inv4w;
            out[2] = (m[0][1] - m[1][0]) * inv4w;
            out[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxM4dMakeRotationQuat(afxM4d const m, afxQuat out)
{
    // From DxMath (designed to represent rotation matrices)
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiffSoft(out, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrt(fourXSqr);

            out[0] = fourXSqr * inv4x;
            out[1] = (m[0][1] + m[1][0]) * inv4x;
            out[2] = (m[0][2] + m[2][0]) * inv4x;
            out[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrt(fourYSqr);

            out[0] = (m[0][1] + m[1][0]) * inv4y;
            out[1] = fourYSqr * inv4y;
            out[2] = (m[1][2] + m[2][1]) * inv4y;
            out[3] = (m[2][0] - m[0][2]) * inv4y;
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

            out[0] = (m[0][2] + m[2][0]) * inv4z;
            out[1] = (m[1][2] + m[2][1]) * inv4z;
            out[2] = fourZSqr * inv4z;
            out[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrt(fourWSqr);

            out[0] = (m[1][2] - m[2][1]) * inv4w;
            out[1] = (m[2][0] - m[0][2]) * inv4w;
            out[2] = (m[0][1] - m[1][0]) * inv4w;
            out[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatMakeRotationM3d(afxQuat const q, afxM3d out)
{
    // Inspired in one from DXMath (designed to represent rotation matrices)
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(q);
    AfxAssertDiff(out, q);

    afxReal qx = q[0], qxx = qx * qx;
    afxReal qy = q[1], qyy = qy * qy;
    afxReal qz = q[2], qzz = qz * qz;
    afxReal qw = q[3];

    out[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    out[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    out[0][2] = 2.f * qx * qz - 2.f * qy * qw;

    out[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    out[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    out[1][2] = 2.f * qy * qz + 2.f * qx * qw;

    out[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    out[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    out[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
}

_AFXINL void AfxQuatMakeRotationM4d(afxQuat const q, afxM4d out)
{
    // From DXMath (designed to represent rotation matrices)
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(q);
    AfxAssertDiff(out, q);

    afxReal qx = q[0], qxx = qx * qx;
    afxReal qy = q[1], qyy = qy * qy;
    afxReal qz = q[2], qzz = qz * qz;
    afxReal qw = q[3];

    out[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    out[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    out[0][2] = 2.f * qx * qz - 2.f * qy * qw;
    out[0][3] = 0.f;

    out[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    out[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    out[1][2] = 2.f * qy * qz + 2.f * qx * qw;
    out[1][3] = 0.f;

    out[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    out[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    out[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
    out[2][3] = 0.f;

    out[3][0] = 0.f;
    out[3][1] = 0.f;
    out[3][2] = 0.f;
    out[3][3] = 1.0f;
}

_AFXINL void AfxM4dMakeTranslationV2d(afxM4d const m, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiff(out, m);

    out[0] = m[3][0];
    out[1] = m[3][1];
}

_AFXINL void AfxM4dMakeTranslationV3d(afxM4d const m, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiff(out, m);

    out[0] = m[3][0];
    out[1] = m[3][1];
    out[2] = m[3][2];
}

_AFXINL void AfxM4dMakeTranslationV4d(afxM4d const m, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(m);
    AfxAssertDiff(out, m);

    out[0] = m[3][0];
    out[1] = m[3][1];
    out[2] = m[3][2];
    out[3] = m[3][3];
}

_AFXINL void AfxV2dMakeTranslationM4d(afxV2d const v, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = 1.f;
    out[0][1] = 0.f;
    out[0][2] = 0.f;
    out[0][3] = 0.0f;

    out[1][0] = 0.f;
    out[1][1] = 1.f;
    out[1][2] = 0.f;
    out[1][3] = 0.0f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = 1.f;
    out[2][3] = 0.0f;

    out[3][0] = v[0];
    out[3][1] = v[1];
    out[3][2] = 0.f;
    out[3][3] = 1.0f;
}

_AFXINL void AfxV3dMakeTranslationM4d(afxV3d const v, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = 1.f;
    out[0][1] = 0.f;
    out[0][2] = 0.f;
    out[0][3] = 0.0f;

    out[1][0] = 0.f;
    out[1][1] = 1.f;
    out[1][2] = 0.f;
    out[1][3] = 0.0f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = 1.f;
    out[2][3] = 0.0f;

    out[3][0] = v[0];
    out[3][1] = v[1];
    out[3][2] = v[2];
    out[3][3] = 1.0f;
}

_AFXINL void AfxV4dMakeTranslationM4d(afxV4d const v, afxM4d out)
{
    // From DXMath (designed to represent rotation matrices)
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssertDiff(out, v);

    out[0][0] = 1.f;
    out[0][1] = 0.f;
    out[0][2] = 0.f;
    out[0][3] = 0.0f;

    out[1][0] = 0.f;
    out[1][1] = 1.f;
    out[1][2] = 0.f;
    out[1][3] = 0.0f;

    out[2][0] = 0.f;
    out[2][1] = 0.f;
    out[2][2] = 1.f;
    out[2][3] = 0.0f;

    out[3][0] = v[0];
    out[3][1] = v[1];
    out[3][2] = v[2];
    out[3][3] = v[3];
}

// Swap

_AFXINL void AfxV2dSwap(afxV2d v, afxV2d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV2d tmp;
    AfxV2dCopy(tmp, v);
    AfxV2dCopy(other, tmp);
    AfxV2dCopy(v, other);
}

_AFXINL void AfxV3dSwap(afxV3d v, afxV3d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV3d tmp;
    AfxV3dCopy(tmp, v);
    AfxV3dCopy(other, tmp);
    AfxV3dCopy(v, other);
}

_AFXINL void AfxV4dSwap(afxV4d v, afxV4d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(v != other);
    afxV4d tmp;
    AfxV4dCopy(tmp, v);
    AfxV4dCopy(other, tmp);
    AfxV4dCopy(v, other);
}

_AFXINL void AfxQuatSwap(afxQuat q, afxQuat other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    AfxAssert(q != other);
    AfxV4dSwap(q, other);
}

_AFXINL void AfxM2dSwap(afxM2d m, afxM2d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM2d tmp;
    AfxM2dCopy(tmp, m);
    AfxM2dCopy(other, tmp);
    AfxM2dCopy(m, other);
}

_AFXINL void AfxM3dSwap(afxM3d m, afxM3d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM3d tmp;
    AfxM3dCopy(tmp, m);
    AfxM3dCopy(other, tmp);
    AfxM3dCopy(m, other);
}

_AFXINL void AfxM4dSwap(afxM4d m, afxM4d other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(other);
    AfxAssert(m != other);
    afxM4d tmp;
    AfxM4dCopy(tmp, m);
    AfxM4dCopy(other, tmp);
    AfxM4dCopy(m, other);
}

// Transpose

_AFXINL void AfxM2dTranspose(afxM2d const m, afxM2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
}

_AFXINL void AfxM3dTranspose(afxM3d const m, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];
    out[0][2] = m[2][0];

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
    out[1][2] = m[2][1];

    out[2][0] = m[0][2];
    out[2][1] = m[1][2];
    out[2][2] = m[2][2];
}

_AFXINL void AfxM4dTranspose(afxM4d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];
    out[0][2] = m[2][0];
    out[0][3] = m[3][0];

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
    out[1][2] = m[2][1];
    out[1][3] = m[3][1];

    out[2][0] = m[0][2];
    out[2][1] = m[1][2];
    out[2][2] = m[2][2];
    out[2][3] = m[3][2];

    out[3][0] = m[0][3];
    out[3][1] = m[1][3];
    out[3][2] = m[2][3];
    out[3][3] = m[3][3];
}

_AFXINL void AfxM4dTransposeLinear(afxM4d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];
    out[0][2] = m[2][0];
    out[0][3] = 0.f;

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
    out[1][2] = m[2][1];
    out[1][3] = 0.f;

    out[2][0] = m[0][2];
    out[2][1] = m[1][2];
    out[2][2] = m[2][2];
    out[2][3] = 0.f;

    out[3][0] = 0.f;
    out[3][1] = 0.f;
    out[3][2] = 0.f;
    out[3][3] = 1.f;
}

_AFXINL void AfxM4dTransposeAffine(afxM4d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];
    out[0][2] = m[2][0];
    out[0][3] = 0.f;

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
    out[1][2] = m[2][1];
    out[1][3] = 0.f;

    out[2][0] = m[0][2];
    out[2][1] = m[1][2];
    out[2][2] = m[2][2];
    out[2][3] = 0.f;

    out[3][0] = m[0][3];
    out[3][1] = m[1][3];
    out[3][2] = m[2][3];
    out[3][3] = 1.f;
}

_AFXINL void AfxM4dTransposeM3d(afxM4d const m, afxM3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];
    out[0][2] = m[2][0];

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
    out[1][2] = m[2][1];

    out[2][0] = m[0][2];
    out[2][1] = m[1][2];
    out[2][2] = m[2][2];
}

_AFXINL void AfxM3dTransposeM4d(afxM3d const m, afxM4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    AfxAssert(out);

    out[0][0] = m[0][0];
    out[0][1] = m[1][0];
    out[0][2] = m[2][0];
    out[0][3] = 0.f;

    out[1][0] = m[0][1];
    out[1][1] = m[1][1];
    out[1][2] = m[2][1];
    out[1][3] = 0.f;

    out[2][0] = m[0][2];
    out[2][1] = m[1][2];
    out[2][2] = m[2][2];
    out[2][3] = 0.f;

    out[3][0] = 0.f;
    out[3][1] = 0.f;
    out[3][2] = 0.f;
    out[3][3] = 1.f;
}
