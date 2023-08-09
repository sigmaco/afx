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
#include "afx/math/afxScalar.h"

// Dot

_AFXINL afxReal AfxV2dDot(afxV2d const v, afxV2d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]);
}

_AFXINL afxReal AfxV3dDot(afxV3d const v, afxV3d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]) + (v[2] * other[2]);
}

_AFXINL afxReal AfxV4dDot(afxV4d const v, afxV4d const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(other);
    return (v[0] * other[0]) + (v[1] * other[1]) + (v[2] * other[2]) + (v[3] * other[3]);
}

_AFXINL afxReal AfxQuatDot(afxQuat const q, afxQuat const other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(other);
    return AfxV4dDot(q, other);
}

// Cross

_AFXINL void AfxV3dCross(afxV3d const v, afxV3d const other, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    AfxAssert(other);
    AfxAssert(out != v);
    AfxAssert(out != other);

    out[0] = (v[1] * other[2]) - (v[2] * other[1]);
    out[1] = (v[2] * other[0]) - (v[0] * other[2]);
    out[2] = (v[0] * other[1]) - (v[1] * other[0]);
}

// Det

_AFXINL afxReal AfxM2dDet(afxM2d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
}

_AFXINL afxReal AfxM3dDet(afxM3d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])) - (m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])) + (m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

_AFXINL afxReal AfxM4dDet(afxM4d const m)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(m);
    return  (m[0][0] * m[1][1] * m[2][2] * m[3][3]) +
        (m[0][0] * m[1][2] * m[2][3] * m[3][1]) +
        (m[0][0] * m[1][3] * m[2][1] * m[3][2]) +
        (m[0][1] * m[1][0] * m[2][3] * m[3][2]) +
        (m[0][1] * m[1][2] * m[2][0] * m[3][3]) +
        (m[0][1] * m[1][3] * m[2][2] * m[3][0]) +
        (m[0][2] * m[1][0] * m[2][1] * m[3][3]) +
        (m[0][2] * m[1][1] * m[2][3] * m[3][0]) +
        (m[0][2] * m[1][3] * m[2][0] * m[3][1]) +
        (m[0][3] * m[1][0] * m[2][2] * m[3][1]) +
        (m[0][3] * m[1][1] * m[2][0] * m[3][2]) +
        (m[0][3] * m[1][2] * m[2][1] * m[3][0]) -
        (m[0][0] * m[1][1] * m[2][3] * m[3][2]) -
        (m[0][0] * m[1][2] * m[2][1] * m[3][3]) -
        (m[0][0] * m[1][3] * m[2][2] * m[3][1]) -
        (m[0][1] * m[1][0] * m[2][2] * m[3][3]) -
        (m[0][1] * m[1][2] * m[2][3] * m[3][0]) -
        (m[0][1] * m[1][3] * m[2][0] * m[3][2]) -
        (m[0][2] * m[1][0] * m[2][3] * m[3][1]) -
        (m[0][2] * m[1][1] * m[2][0] * m[3][3]) -
        (m[0][2] * m[1][3] * m[2][1] * m[3][0]) -
        (m[0][3] * m[1][0] * m[2][1] * m[3][2]) -
        (m[0][3] * m[1][1] * m[2][2] * m[3][0]) -
        (m[0][3] * m[1][2] * m[2][0] * m[3][1]);
}

// Norm

_AFXINL afxReal AfxV2dNorm(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxV2dDot(v, v);
}

_AFXINL afxReal AfxV3dNorm(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxV3dDot(v, v);
}

_AFXINL afxReal AfxV4dNorm(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxV4dDot(v, v);
}

_AFXINL afxReal AfxQuatNorm(afxQuat const q)
{
    afxError err = AFX_ERR_NONE;
    return AfxV4dNorm(q);
}

// Magnitude

_AFXINL afxReal AfxV2dMagnitude(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrt(AfxV2dNorm(v));
}

_AFXINL afxReal AfxV3dMagnitude(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrt(AfxV3dNorm(v));
}

_AFXINL afxReal AfxV4dMagnitude(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxSqrt(AfxV4dNorm(v));
}

_AFXINL afxReal AfxQuatMagnitude(afxQuat const q)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    return AfxSqrt(AfxQuatNorm(q));
}

// MagnitudeRecip

_AFXINL afxReal AfxV2dMagnitudeRecip(afxV2d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRecipSqrt(AfxV2dNorm(v));
}

_AFXINL afxReal AfxV3dMagnitudeRecip(afxV3d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRecipSqrt(AfxV3dNorm(v));
}

_AFXINL afxReal AfxV4dMagnitudeRecip(afxV4d const v)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    return AfxRecipSqrt(AfxV4dNorm(v));
}

_AFXINL afxReal AfxQuatMagnitudeRecip(afxQuat const q)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    return AfxRecipSqrt(AfxQuatNorm(q));
}

// Normalize

_AFXINL afxReal AfxV2dNormalize(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV2dMagnitude(v);

    if (len > 0)
        len = 1.f / len;

    AfxV2dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxV3dNormalize(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV3dMagnitude(v);

    if (len > 0)
        len = 1.f / len;

    AfxV3dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxV4dNormalize(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV4dMagnitude(v);

    if (len > 0)
        len = 1.f / len;

    AfxV4dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxQuatNormalize(afxQuat const q, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(out);
    return AfxV4dNormalize(q, out);
}

// NormalizeV*d

_AFXINL afxReal AfxV4dNormalizeV3d(afxV4d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV4dMagnitude(v);

    if (len > 0)
        len = 1.f / len;

    AfxV3dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxV3dNormalizeV4d(afxV3d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV3dMagnitude(v);

    if (len > 0)
        len = 1.f / len;

    AfxV4dScale(v, len, out);
    return len;
}

// NormalizeEst

_AFXINL afxReal AfxV2dNormalizeEst(afxV2d const v, afxV2d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV2dMagnitudeRecip(v);
    AfxV2dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxV3dNormalizeEst(afxV3d const v, afxV3d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV3dMagnitudeRecip(v);
    AfxV3dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxV4dNormalizeEst(afxV4d const v, afxV4d out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(out);
    AfxAssert(v);
    afxReal len = AfxV4dMagnitudeRecip(v);
    AfxV4dScale(v, len, out);
    return len;
}

_AFXINL afxReal AfxQuatNormalizeEst(afxQuat const q, afxQuat out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(q);
    AfxAssert(out);
    return AfxV4dNormalizeEst(q, out);
}
