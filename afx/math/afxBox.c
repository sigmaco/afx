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

#include "qwadro/math/afxBox.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"

_AFXINL void AfxBoxReset(afxBox bb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxResetV4d(bb[AFX_AABB_SUP]);
    AfxResetV4d(bb[AFX_AABB_INF]);
}

_AFXINL void AfxAabbSet(afxBox bb, afxNat cnt, afxV3d const points[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(points);
    AfxAssert(cnt);
    AfxResetV4d(bb[AFX_AABB_SUP]);
    AfxResetV4d(bb[AFX_AABB_INF]);
    AfxAabbAbsorbAtv3d(bb, cnt, points);
}

_AFXINL void AfxBoxCopy(afxBox bb, afxBox const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(in);
    AfxV4dCopy(bb[AFX_AABB_SUP], in[AFX_AABB_SUP]);
    AfxV4dCopy(bb[AFX_AABB_INF], in[AFX_AABB_INF]);
}

_AFXINL void AfxAabbAbsorbAtv3d(afxBox bb, afxNat cnt, afxV3d const v[])
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

_AFXINL void AfxAabbAbsorbAtv4d(afxBox bb, afxNat cnt, afxV4d const v[])
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

_AFXINL void AfxAabbAbsorbSpheres(afxBox bb, afxNat cnt, afxSphere const sph[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(sph);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxV3d range[2], radius = { sph[i].radius, sph[i].radius, sph[i].radius };
        AfxV3dAdd(range[0], sph[i].centre, radius);
        AfxV3dSub(range[1], sph[i].centre, radius);
        AfxAabbAbsorbAtv3d(bb, 2, range);
    }
    bb[AFX_AABB_SUP][3] = (bb[AFX_AABB_INF][3] = AfxScalar(1));
}

_AFXINL void AfxAabbAbsorb(afxBox bb, afxNat cnt, afxBox const other[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(other);

    for (afxNat i = 0; i < cnt; i++)
        AfxAabbAbsorbAtv4d(bb, 2, other[i]);
}

_AFXINL afxNat AfxAabbTestAtv3d(afxBox const bb, afxNat cnt, afxV3d const point[])
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

_AFXINL afxResult AfxAabbTestPlanes(afxBox const aabb, afxNat cnt, afxPlane const planes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(aabb, planes);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxReal minD;
        afxReal maxD;

        if (planes[i][0] > 0.0f)
        {
            minD = planes[i][0] * aabb[AFX_AABB_INF][0];
            maxD = planes[i][0] * aabb[AFX_AABB_SUP][0];
        }
        else
        {
            minD = planes[i][0] * aabb[AFX_AABB_SUP][0];
            maxD = planes[i][0] * aabb[AFX_AABB_INF][0];
        }

        if (planes[i][1] > 0.0f)
        {
            minD += planes[i][1] * aabb[AFX_AABB_INF][1];
            maxD += planes[i][1] * aabb[AFX_AABB_SUP][1];
        }
        else
        {
            minD += planes[i][1] * aabb[AFX_AABB_SUP][1];;
            maxD += planes[i][1] * aabb[AFX_AABB_INF][1];
        }

        if (planes[i][2] > 0.0f)
        {
            minD += planes[i][2] * aabb[AFX_AABB_INF][2];
            maxD += planes[i][2] * aabb[AFX_AABB_SUP][2];
        }
        else
        {
            minD += planes[i][2] * aabb[AFX_AABB_SUP][2];
            maxD += planes[i][2] * aabb[AFX_AABB_INF][2];
        }

        // What side of the plane

        if (minD >= -planes[i][AFX_PLANE_OFFSET])
            return +1;

        if (maxD <= -planes[i][AFX_PLANE_OFFSET])
            return -1;
    }
    //Intersection
    return 0;
}

_AFXINL void AfxAabbGetExtents(afxBox const bb, afxV3d extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(extent);
#if 0
    AfxV4dSub(extent, bb[AFX_AABB_SUP], bb[AFX_AABB_INF]);
#else
    // 0.5 * (max - min)
    AfxV3dSub(extent, bb[AFX_AABB_SUP], bb[AFX_AABB_INF]);
    AfxHalfV3d(extent, extent);
#endif
}

_AFXINL void AfxAabbGetCentre(afxBox const bb, afxV4d centre)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(centre);
    // 0.5 * (min + max)
    AfxV3dAdd(centre, bb[AFX_AABB_INF], bb[AFX_AABB_SUP]);
    AfxHalfV3d(centre, centre);
    centre[3] = 1.f;
}

_AFXINL void AfxAabbGetCorner(afxBox const bb, afxNat index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(corner);
    AfxAssert(index < 8);

    switch (index)
    {
    case 0: AfxV4dSet(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 1: AfxV4dSet(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 2: AfxV4dSet(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 3: AfxV4dSet(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_SUP][2], (afxReal)1); break;
    case 4: AfxV4dSet(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    case 5: AfxV4dSet(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_INF][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    case 6: AfxV4dSet(corner, bb[AFX_AABB_SUP][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    case 7: AfxV4dSet(corner, bb[AFX_AABB_INF][0], bb[AFX_AABB_SUP][1], bb[AFX_AABB_INF][2], (afxReal)1); break;
    default: AfxThrowError(); break;
    }
}

_AFXINL void AfxTransformAabb(afxBox const bb, afxM4d const m, afxBox to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bb);
    AfxAssert(m);
    AfxAssert(to);
    AfxBoxReset(to);
    AfxM4dPostMultiplyV4d(m, 2, bb, to);
}

_AFXINL void AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxBox const in[], afxBox out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(cnt);
    AfxAssert(in);
    AfxAssert(out);

    // box = obb [min, max]

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
                    AfxV3dSet(tmp, x ? in[i][AFX_AABB_SUP][0] : in[i][AFX_AABB_INF][0], y ? in[i][AFX_AABB_SUP][1] : in[i][AFX_AABB_INF][1], z ? in[i][AFX_AABB_SUP][2] : in[i][AFX_AABB_INF][2]);

                    AfxV3dPostMultiplyM3d(pos, ltm, tmp);
                    AfxV3dAdd(pos, pos, atv);

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

_AFXINL void AfxCopyBoxes(afxNat cnt, afxBox in[], afxBox out[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(out);
    AfxAssert(in);

    for (afxNat i = 0; i < cnt; i++)
        AfxBoxCopy(out[i], in[i]);
}

_AFXINL void AfxResetBoxes(afxNat cnt, afxBox boxes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(boxes);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
        AfxBoxReset(boxes[i]);
}
