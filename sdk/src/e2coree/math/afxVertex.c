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

#include "afx/math/afxVertex.h"

_AFXINL afxReal AfxGetDistanceBetweenV3d(afxReal const v[3], afxReal const other[3])
{
    // Contribution to us from Crytek

    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(other);
    afxV3d t;
    AfxSubV3d(t, v, other);
    AfxSqrtV3d(t, t);
    return AfxSqrt(AfxSumV3d(t));
}

_AFXINL afxReal AfxGetAngleBetweenV3d(afxReal const v[3], afxReal const other[3])
{
    // Contribution to us from Crytek

    afxError err = NIL;
    AfxAssert(v);
    AfxAssert(other);

    afxReal cosAngle = AfxDotV3d(v, other) * (AfxMagV3dRecip(v) * AfxMagV3dRecip(other));
    AfxClamp(cosAngle, -1.0, 1.0);
    return AfxAcos(cosAngle);
}

// ExtractNormalComponent

_AFXINL void AfxExtractNormalV3dComponents(afxReal const v[3], afxReal const normal[3], afxReal parallel[3], afxReal perpendicular[3])
{
    // Should be compatible with void XMVector3ComponentsFromNormal(XMVECTOR* pParallel, XMVECTOR* pPerpendicular, FXMVECTOR  V, FXMVECTOR  Normal)

    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(normal);
    AfxAssert(parallel);
    AfxAssert(perpendicular);

    afxReal scale = AfxDotV3d(v, normal);
    AfxScaleV3d(parallel, normal, scale);
    AfxSubV3d(perpendicular, v, parallel);
}

// Hermite

_AFXINL void AfxHermiteV2d(afxReal v[2], afxReal const posA[2], afxReal const tanA[2], afxReal const posB[2], afxReal const tanB[2], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(posA);
    AfxAssert(tanA);
    AfxAssert(posB);
    AfxAssert(tanB);

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV2d pa, ta, pb, tb;
    AfxFillV2d(pa, 2.f * t3 - 3.f * t2 + 1.f);
    AfxFillV2d(ta, t3 - 2.f * t2 + t);
    AfxFillV2d(pb, -2.f * t3 + 3.f * t2);
    AfxFillV2d(tb, t3 - t2);

    AfxMultiplyV2d(v, pa, posA);
    AfxMadV2d(v, ta, ta, v);
    AfxMadV2d(v, pb, pb, v);
    AfxMadV2d(v, tb, tb, v);
}

_AFXINL void AfxHermiteV3d(afxReal v[3], afxReal const posA[3], afxReal const tanA[3], afxReal const posB[3], afxReal const tanB[3], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(posA);
    AfxAssert(tanA);
    AfxAssert(posB);
    AfxAssert(tanB);

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV3d pa, ta, pb, tb;
    AfxFillV3d(pa, 2.f * t3 - 3.f * t2 + 1.f);
    AfxFillV3d(ta, t3 - 2.f * t2 + t);
    AfxFillV3d(pb, -2.f * t3 + 3.f * t2);
    AfxFillV3d(tb, t3 - t2);

    AfxMultiplyV3d(v, pa, posA);
    AfxMadV3d(v, ta, ta, v);
    AfxMadV3d(v, pb, pb, v);
    AfxMadV3d(v, tb, tb, v);
}

_AFXINL void AfxHermiteV4d(afxReal v[4], afxReal const posA[4], afxReal const tanA[4], afxReal const posB[4], afxReal const tanB[4], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(posA);
    AfxAssert(tanA);
    AfxAssert(posB);
    AfxAssert(tanB);

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV4d pa, ta, pb, tb;
    AfxFillV4d(pa, 2.f * t3 - 3.f * t2 + 1.f);
    AfxFillV4d(ta, t3 - 2.f * t2 + t);
    AfxFillV4d(pb, -2.f * t3 + 3.f * t2);
    AfxFillV4d(tb, t3 - t2);

    AfxMultiplyV4d(v, pa, posA);
    AfxMadV4d(v, ta, ta, v);
    AfxMadV4d(v, pb, pb, v);
    AfxMadV4d(v, tb, tb, v);
}

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
/// The geometry matrix.

// CatmullRom

_AFXINL void AfxCatmullV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal const d[2], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);
    AfxAssert(d);

    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV2d pa, pb, pc, pd;
    AfxFillV2d(pa, (-t3 + 2.0f * t2 - t) * 0.5f);
    AfxFillV2d(pb, (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    AfxFillV2d(pc, (-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    AfxFillV2d(pd, (t3 - t2) * 0.5f);

    AfxMultiplyV2d(v, pa, a);
    AfxMadV2d(v, pb, b, v);
    AfxMadV2d(v, pc, c, v);
    AfxMadV2d(v, pd, d, v);
}

_AFXINL void AfxCatmullV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal const d[3], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);
    AfxAssert(d);

    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV3d pa, pb, pc, pd;
    AfxFillV3d(pa, (-t3 + 2.0f * t2 - t) * 0.5f);
    AfxFillV3d(pb, (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    AfxFillV3d(pc, (-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    AfxFillV3d(pd, (t3 - t2) * 0.5f);

    AfxMultiplyV3d(v, pa, a);
    AfxMadV3d(v, pb, b, v);
    AfxMadV3d(v, pc, c, v);
    AfxMadV3d(v, pd, d, v);
}

_AFXINL void AfxCatmullV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal const d[4], afxReal t)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);
    AfxAssert(d);

    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV4d pa, pb, pc, pd;
    AfxFillV4d(pa, (-t3 + 2.0f * t2 - t) * 0.5f);
    AfxFillV4d(pb, (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    AfxFillV4d(pc, (-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    AfxFillV4d(pd, (t3 - t2) * 0.5f);

    AfxMultiplyV4d(v, pa, a);
    AfxMadV4d(v, pb, b, v);
    AfxMadV4d(v, pc, c, v);
    AfxMadV4d(v, pd, d, v);
}

// Reflect

_AFXINL void AfxReflectV2d(afxReal v[2], afxReal const incident[2], afxReal const normal[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(incident);
    AfxAssert(normal);

    // v = incident - (2 * dot(incident, normal)) * normal

    AfxFillV2d(v, AfxDotV2d(incident, normal));
    AfxAddV2d(v, v, v);
    AfxNemusV2d(v, v, normal, incident);
}

_AFXINL void AfxReflectV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(incident);
    AfxAssert(normal);

    // v = incident - (2 * dot(incident, normal)) * normal

    AfxFillV3d(v, AfxDotV3d(incident, normal));
    AfxAddV3d(v, v, v);
    AfxNemusV3d(v, v, normal, incident);
}

_AFXINL void AfxReflectV4d(afxReal v[4], afxReal const incident[3], afxReal const normal[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(incident);
    AfxAssert(normal);

    // v = incident - (2 * dot(incident, normal)) * normal

    AfxFillV4d(v, AfxDotV3d(incident, normal));
    AfxAddV4d(v, v, v);
    AfxNemusV4d(v, v, normal, incident);
}

// Refract

_AFXINL void AfxRefractV2d(afxReal v[2], afxReal const incident[2], afxReal const normal[2], afxReal refracIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(incident);
    AfxAssert(normal);

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV2d dot;
    AfxFillV2d(dot, AfxDotV2d(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV2d R;
    AfxNemusV2d(R, dot, dot, AFX_V2D_11);
    afxV2d const riv = { refracIdx, refracIdx };
    AfxMultiplyV2d(R, R, riv);
    AfxNemusV2d(R, R, riv, AFX_V2D_11);

    if (AfxV2dIsLessOrEqual(R, AFX_V2D_00))
    {
        // Total internal reflection
        AfxZeroV2d(v);
    }
    else
    {
        // R = refracIdx * dot + sqrt(R)

        AfxSqrtV2d(R, R);
        AfxMadV2d(R, riv, dot, R);

        // v = refracIdx * incident - normal * R
        AfxMultiplyV2d(v, riv, incident);
        AfxNemusV2d(v, normal, R, v);
    }
}

_AFXINL void AfxRefractV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3], afxReal refracIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(incident);
    AfxAssert(normal);

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV3d dot;
    AfxFillV3d(dot, AfxDotV3d(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV3d R;
    AfxNemusV3d(R, dot, dot, AFX_V3D_111);
    afxV3d const riv = { refracIdx, refracIdx, refracIdx };
    AfxMultiplyV3d(R, R, riv);
    AfxNemusV3d(R, R, riv, AFX_V3D_111);

    if (AfxV3dIsLessOrEqual(R, AFX_V3D_000))
    {
        // Total internal reflection
        AfxZeroV3d(v);
    }
    else
    {
        // R = refracIdx * dot + sqrt(R)

        AfxSqrtV3d(R, R);
        AfxMadV3d(R, riv, dot, R);

        // v = refracIdx * incident - normal * R
        AfxMultiplyV3d(v, riv, incident);
        AfxNemusV3d(v, normal, R, v);
    }
}

_AFXINL void AfxRefractV4d(afxReal v[4], afxReal const incident[3], afxReal const normal[3], afxReal refracIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(v);
    AfxAssert(incident);
    AfxAssert(normal);

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV4d dot;
    AfxFillV4d(dot, AfxDotV3d(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV4d R;
    AfxNemusV4d(R, dot, dot, AFX_V4D_1111);
    afxV4d const riv = { refracIdx, refracIdx, refracIdx, refracIdx };
    AfxMultiplyV4d(R, R, riv);
    AfxNemusV4d(R, R, riv, AFX_V4D_1111);

    if (AfxV4dIsLessOrEqual(R, AFX_V4D_0000))
    {
        // Total internal reflection
        AfxZeroV4d(v);
    }
    else
    {
        // R = refracIdx * dot + sqrt(R)

        AfxSqrtV4d(R, R);
        AfxMadV4d(R, riv, dot, R);

        // v = refracIdx * incident - normal * R
        AfxMultiplyV4d(v, riv, incident);
        AfxNemusV4d(v, normal, R, v);
    }
}
