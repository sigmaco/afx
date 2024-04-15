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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/math/afxAabb.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"

_AFXINL void AfxResetAabb(afxAabb bb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxResetV4d(bb[AFX_AABB_SUP]);
    AfxResetV4d(bb[AFX_AABB_INF]);
}

_AFXINL void AfxCopyAabb(afxAabb bb, afxAabb const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(in);
    AfxCopyV4d(bb[AFX_AABB_SUP], in[AFX_AABB_SUP]);
    AfxCopyV4d(bb[AFX_AABB_INF], in[AFX_AABB_INF]);
}

_AFXINL void AfxEncapsulateVertices(afxAabb bb, afxNat cnt, afxV3d const v[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(v);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMaxV3d(bb[AFX_AABB_SUP], bb[AFX_AABB_SUP], v[i]);
        AfxMinV3d(bb[AFX_AABB_INF], bb[AFX_AABB_INF], v[i]);
    }
}

_AFXINL void AfxEncapsulatePoints(afxAabb bb, afxNat cnt, afxV4d const v[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(v);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxMaxV3d(bb[AFX_AABB_SUP], bb[AFX_AABB_SUP], v[i]);
        AfxMinV3d(bb[AFX_AABB_INF], bb[AFX_AABB_INF], v[i]);
    }
}

_AFXINL void AfxEncapsulateSpheres(afxAabb bb, afxNat cnt, afxSphere const sph[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(sph);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxV3d range[2], radius = { sph[i].radius, sph[i].radius, sph[i].radius };
        AfxAddV3d(range[0], sph[i].origin, radius);
        AfxSubV3d(range[1], sph[i].origin, radius);
        AfxEncapsulateVertices(bb, 2, range);
    }    
    bb[AFX_AABB_SUP][3] = (bb[AFX_AABB_INF][3] = AfxScalar(1));
}

_AFXINL void AfxEncapsulateAabbs(afxAabb bb, afxNat cnt, afxAabb const other[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(other);

    for (afxNat i = 0; i < cnt; i++)
        AfxEncapsulatePoints(bb, 2, other[i]);
}

_AFXINL void AfxRecomputeAabb(afxAabb bb, afxNat cnt, afxV3d const points[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(points);
    AfxAssert(cnt);
    AfxResetV4d(bb[AFX_AABB_SUP]);
    AfxResetV4d(bb[AFX_AABB_INF]);
    AfxEncapsulateVertices(bb, cnt, points);
}

_AFXINL afxNat AfxAabbContainsPoints(afxAabb const bb, afxNat cnt, afxV3d const point[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(point);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        if
        (
            (!(bb[AFX_AABB_SUP][0] < point[i][0])) && (!(bb[AFX_AABB_SUP][1] < point[i][1])) && (!(bb[AFX_AABB_SUP][2] < point[i][2])) &&
            (!(bb[AFX_AABB_INF][0] > point[i][0])) && (!(bb[AFX_AABB_INF][1] > point[i][1])) && (!(bb[AFX_AABB_INF][2] > point[i][2]))
        )
        {
            ++rslt;
        }
    }
    return rslt;
}

_AFXINL void AfxGetAabbExtents(afxAabb const bb, afxV3d extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(extent);
#if 0
    AfxSubV4d(extent, bb[AFX_AABB_SUP], bb[AFX_AABB_INF]);
#else
    // 0.5 * (max - min)
    AfxSubV3d(extent, bb[AFX_AABB_SUP], bb[AFX_AABB_INF]);
    AfxHalfV3d(extent, extent);
#endif
}

_AFXINL void AfxGetAabbCentre(afxAabb const bb, afxV4d centre)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(centre);
    // 0.5 * (min + max)
    AfxAddV3d(centre, bb[AFX_AABB_INF], bb[AFX_AABB_SUP]);
    AfxHalfV3d(centre, centre);
    centre[3] = 1.f;
}

_AFXINL void AfxExtractAabbCorner(afxAabb const bb, afxNat index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(corner);
    AfxAssert(index < 8);

    switch (index)
    {
    case 0: AfxSetV4d(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 1: AfxSetV4d(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 2: AfxSetV4d(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 3: AfxSetV4d(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 4: AfxSetV4d(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    case 5: AfxSetV4d(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    case 6: AfxSetV4d(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    case 7: AfxSetV4d(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    default: AfxThrowError(); break;
    }
}

_AFXINL void AfxTransformAabb(afxAabb const bb, afxM4d const m, afxAabb to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(m);
    AfxAssert(to);
    AfxResetAabb(to);
    AfxPostMultiplyArrayedV4d(m, 2, bb, to);
}

_AFXINL void AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxAabb const in[], afxAabb out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
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
                    AfxSetV3d(tmp, x ? in[i][AFX_AABB_SUP][0] : in[i][AFX_AABB_INF][0], y ? in[i][AFX_AABB_SUP][1] : in[i][AFX_AABB_INF][1], z ? in[i][AFX_AABB_SUP][2] : in[i][AFX_AABB_INF][2]);

                    AfxPostMultiplyV3d(ltm, tmp, pos);
                    AfxAddV3d(pos, pos, atv);

                    AfxMinV3d(min, min, pos);
                    AfxMaxV3d(max, max, pos);
                }
            }
        }

        out[i][AFX_AABB_SUP][0] = max[0];
        out[i][AFX_AABB_SUP][1] = max[1];
        out[i][AFX_AABB_SUP][2] = max[2];
        out[i][AFX_AABB_INF][0] = min[0];
        out[i][AFX_AABB_INF][1] = min[1];
        out[i][AFX_AABB_INF][2] = min[2];
    }
}
