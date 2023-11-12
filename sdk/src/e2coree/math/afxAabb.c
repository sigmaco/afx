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

#include "afx/math/afxAabb.h"
#include "afx/math/afxMathDefs.h"
#include "afx/math/afxVector.h"
#include "afx/math/afxMatrix.h"

_AFXINL void AfxResetAabb(afxAabb* aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxResetV4d(aabb->extremes[AFX_AABB_SUP]);
    AfxResetV4d(aabb->extremes[AFX_AABB_INF]);
}

_AFXINL void AfxCopyAabb(afxAabb* aabb, afxAabb const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(in);
    AfxCopyV4d(aabb->extremes[AFX_AABB_SUP], in->extremes[AFX_AABB_SUP]);
    AfxCopyV4d(aabb->extremes[AFX_AABB_INF], in->extremes[AFX_AABB_INF]);
}

_AFXINL void AfxEncapsulateVertices(afxAabb* aabb, afxNat cnt, afxReal const point[][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(point);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMaxiV3d(aabb->extremes[AFX_AABB_SUP], aabb->extremes[AFX_AABB_SUP], point[i]);
        AfxMiniV3d(aabb->extremes[AFX_AABB_INF], aabb->extremes[AFX_AABB_INF], point[i]);
    }
}

_AFXINL void AfxEncapsulatePoints(afxAabb* aabb, afxNat cnt, afxReal const point[][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(point);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMaxiV3d(aabb->extremes[AFX_AABB_SUP], aabb->extremes[AFX_AABB_SUP], point[i]);
        AfxMiniV3d(aabb->extremes[AFX_AABB_INF], aabb->extremes[AFX_AABB_INF], point[i]);
    }
}

_AFXINL void AfxEncapsulateSpheres(afxAabb *aabb, afxNat cnt, afxSphere const sph[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(sph);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxV3d range[2], radius = { sph[i].radius, sph[i].radius, sph[i].radius };
        AfxAddV3d(range[0], sph[i].origin, radius);
        AfxSubV3d(range[1], sph[i].origin, radius);
        AfxEncapsulateVertices(aabb, 2, range);
    }    
    aabb->extremes[AFX_AABB_SUP][3] = (aabb->extremes[AFX_AABB_INF][3] = AfxScalar(1));
}

_AFXINL void AfxEncapsulateAabbs(afxAabb *aabb, afxNat cnt, afxAabb const other[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(other);

    for (afxNat i = 0; i < cnt; i++)
        AfxEncapsulatePoints(aabb, 2, other[i].extremes);
}

_AFXINL void AfxRecomputeAabb(afxAabb *aabb, afxNat cnt, afxReal const points[][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(points);
    AfxAssert(cnt);
    AfxResetV4d(aabb->extremes[AFX_AABB_SUP]);
    AfxResetV4d(aabb->extremes[AFX_AABB_INF]);
    AfxEncapsulateVertices(aabb, cnt, points);
}

_AFXINL afxMask AfxAabbContainsPoints(afxAabb const *aabb, afxNat cnt, afxReal const point[32][3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(point);
    AfxAssert(32 >= cnt);
    
    afxMask rslt = NIL;
    for (afxNat i = 0; i < cnt; i++)
    {
        if
        (
            (!(aabb->extremes[AFX_AABB_SUP][0] < point[i][0])) && (!(aabb->extremes[AFX_AABB_SUP][1] < point[i][1])) && (!(aabb->extremes[AFX_AABB_SUP][2] < point[i][2])) &&
            (!(aabb->extremes[AFX_AABB_INF][0] > point[i][0])) && (!(aabb->extremes[AFX_AABB_INF][1] > point[i][1])) && (!(aabb->extremes[AFX_AABB_INF][2] > point[i][2]))
        )
        {
            rslt |= AfxGetBitOffset(i);
        }
    }
    return rslt;
}

_AFXINL void AfxGetAabbExtents(afxAabb const *aabb, afxReal extent[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(extent);
#if 0
    AfxSubV4d(extent, aabb->extremes[AFX_AABB_SUP], aabb->extremes[AFX_AABB_INF]);
#else
    // 0.5 * (max - min)
    AfxSubV3d(extent, aabb->extremes[AFX_AABB_SUP], aabb->extremes[AFX_AABB_INF]);
    AfxHalfV3d(extent, extent);
#endif
}

_AFXINL void AfxGetAabbCentre(afxAabb const *aabb, afxReal centre[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(centre);
    // 0.5 * (min + max)
    AfxAddV3d(centre, aabb->extremes[AFX_AABB_INF], aabb->extremes[AFX_AABB_SUP]);
    AfxHalfV3d(centre, centre);
    centre[3] = 1.f;
}

_AFXINL void AfxExtractAabbCorner(afxAabb const *aabb, afxNat index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(corner);
    AfxAssert(index < 8);

    switch (index)
    {
    case 0: AfxSetV4d(corner, aabb->extremes[AFX_AABB_INF][0], aabb->extremes[AFX_AABB_INF][1], aabb->extremes[AFX_AABB_SUP][2], (afxReal)1); break;
    case 1: AfxSetV4d(corner, aabb->extremes[AFX_AABB_SUP][0], aabb->extremes[AFX_AABB_INF][1], aabb->extremes[AFX_AABB_SUP][2], (afxReal)1); break;
    case 2: AfxSetV4d(corner, aabb->extremes[AFX_AABB_SUP][0], aabb->extremes[AFX_AABB_SUP][1], aabb->extremes[AFX_AABB_SUP][2], (afxReal)1); break;
    case 3: AfxSetV4d(corner, aabb->extremes[AFX_AABB_INF][0], aabb->extremes[AFX_AABB_SUP][1], aabb->extremes[AFX_AABB_SUP][2], (afxReal)1); break;
    case 4: AfxSetV4d(corner, aabb->extremes[AFX_AABB_INF][0], aabb->extremes[AFX_AABB_INF][1], aabb->extremes[AFX_AABB_INF][2], (afxReal)1); break;
    case 5: AfxSetV4d(corner, aabb->extremes[AFX_AABB_SUP][0], aabb->extremes[AFX_AABB_INF][1], aabb->extremes[AFX_AABB_INF][2], (afxReal)1); break;
    case 6: AfxSetV4d(corner, aabb->extremes[AFX_AABB_SUP][0], aabb->extremes[AFX_AABB_SUP][1], aabb->extremes[AFX_AABB_INF][2], (afxReal)1); break;
    case 7: AfxSetV4d(corner, aabb->extremes[AFX_AABB_INF][0], aabb->extremes[AFX_AABB_SUP][1], aabb->extremes[AFX_AABB_INF][2], (afxReal)1); break;
    default: AfxThrowError(); break;
    }
}

_AFXINL void AfxTransformAabb(afxAabb const* aabb, afxM4d const m, afxAabb* to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(m);
    AfxAssert(to);
    AfxResetAabb(to);
    AfxPostMultiplyArrayedV4d(m, 2, aabb->extremes, to->extremes);
}

_AFXINL void AfxTransformAabbs(afxReal const linear[3][3], afxReal const affine[3], afxNat cnt, afxAabb const in[], afxAabb out[])
{
    // Should be compatible with void TransformBoundingBox(const float *Affine3, const float *Linear3x3, float *OBBMin, float *OBBMax)
    afxError err = AFX_ERR_NONE;
    AfxAssert(affine);
    AfxAssert(linear);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxV3d max, min, pos;
        AfxFillV3d(max, -3.4028235e38);
        AfxFillV3d(min, 3.4028235e38);

        for (afxNat z = 0; z < 2; z++)
        {
            for (afxNat y = 0; y < 2; y++)
            {
                for (afxNat x = 0; x < 2; x++)
                {
                    afxV3d tmp;
                    AfxSetV3d(tmp, x ? in[i].extremes[AFX_AABB_SUP][0] : in[i].extremes[AFX_AABB_INF][0], y ? in[i].extremes[AFX_AABB_SUP][1] : in[i].extremes[AFX_AABB_INF][1], z ? in[i].extremes[AFX_AABB_SUP][2] : in[i].extremes[AFX_AABB_INF][2]);

                    AfxPostMultiplyV3d(pos, linear, tmp);

                    AfxMiniV3d(min, min, pos);
                    AfxMaxiV3d(max, max, pos);
                }
            }
        }

        out[i].extremes[AFX_AABB_SUP][0] = max[0];
        out[i].extremes[AFX_AABB_SUP][1] = max[1];
        out[i].extremes[AFX_AABB_SUP][2] = max[2];
        out[i].extremes[AFX_AABB_INF][0] = min[0];
        out[i].extremes[AFX_AABB_INF][1] = min[1];
        out[i].extremes[AFX_AABB_INF][2] = min[2];
    }
}
