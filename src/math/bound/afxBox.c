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

#include "qwadro/inc/math/bound/afxBox.h"
#include "qwadro/inc/math/afxScalar.h"
#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/afxMatrix.h"

_AFX afxBox const AFX_AABB_IDENTITY = { .min = { 0, 0, 0, 1 }, .max = { 0, 0, 0, 1 } };

_AFXINL void AfxMakeAabbFromVertices(afxBox* bb, afxUnit cnt, afxV3d const points[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(points);
    AFX_ASSERT(cnt);
    AfxResetV4d(bb->max);
    AfxResetV4d(bb->min);
    AfxEmboxVectors(bb, cnt, points);
}

_AFXINL void AfxEmboxVectors(afxBox* bb, afxUnit cnt, afxV3d const v[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(v);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dMax(bb->max, bb->max, v[i]);
        AfxV3dMin(bb->min, bb->min, v[i]);
    }
}

_AFXINL void AfxEmboxPoints(afxBox* bb, afxUnit cnt, afxV4d const v[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(v);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dMax(bb->max, bb->max, v[i]);
        AfxV3dMin(bb->min, bb->min, v[i]);
    }
}

_AFXINL void AfxEmboxSpheres(afxBox* bb, afxUnit cnt, afxSphere const sph[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(sph);
    
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d range[2], radius = { sph[i].radius, sph[i].radius, sph[i].radius };
        AfxV3dAdd(range[0], sph[i].centre, radius);
        AfxV3dSub(range[1], sph[i].centre, radius);
        AfxEmboxVectors(bb, 2, range);
    }
    bb->max[3] = (bb->min[3] = AFX_R(1));
}

_AFXINL void AfxEmboxAabbs(afxBox* bb, afxUnit cnt, afxBox const other[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(other);

    for (afxUnit i = 0; i < cnt; i++)
        AfxEmboxPoints(bb, 2, &other[i].max);
}

_AFXINL afxUnit AfxDoesAabbIncludeAtv3d(afxBox const* bb, afxUnit cnt, afxV3d const point[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(point);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if
        (
            (!(bb->max[0] < point[i][0])) && 
            (!(bb->max[1] < point[i][1])) && 
            (!(bb->max[2] < point[i][2])) &&
            (!(bb->min[0] > point[i][0])) && 
            (!(bb->min[1] > point[i][1])) && 
            (!(bb->min[2] > point[i][2]))
        )
        {
            ++rslt;
        }
    }
    return rslt;
}

_AFXINL afxUnit AfxDoesAabbInclude(afxBox const* bb, afxUnit cnt, afxBox const other[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(other);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        // Check if an AABB intersects with another AABB (overlapping region)

        if ((bb->min[0] < other[i].max[0] && bb->max[0] > other[i].min[0]) &&
            (bb->min[1] < other[i].max[1] && bb->max[1] > other[i].min[1]) &&
            (bb->min[2] < other[i].max[2] && bb->max[2] > other[i].min[2]))
        {
            ++rslt;
        }
    }
    return rslt;
}

afxBool AfxTestSphereBox(afxSphere const* bs, afxBox const* bb)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);

    // test if a bounding box is fully inside a bounding sphere.

    for (afxUnit i = 0; i < 3; i++)
        if ((bs->centre[i] + bs->radius < bb->min[i]) || 
            (bs->centre[i] - bs->radius > bb->max[i]))
            return FALSE;

    return TRUE;
}

_AFXINL void AfxGetAabbExtents(afxBox const* bb, afxV3d extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(extent);
#if 0
    AfxV4dSub(extent, bb->max, bb->min);
#else
    // 0.5 * (max - min)
    AfxV3dSub(extent, bb->max, bb->min);
    AfxHalfV3d(extent, extent);
#endif
}

_AFXINL void AfxGetAabbCentre(afxBox const* bb, afxV4d centre)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(centre);
    // 0.5 * (min + max)
    AfxV3dAdd(centre, bb->min, bb->max);
    AfxHalfV3d(centre, centre);
    centre[3] = 1.f;
}

_AFXINL void AfxGetAabbCorner(afxBox const* bb, afxUnit index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(corner);
    AFX_ASSERT(index < 8);

    switch (index)
    {
    case 0: AfxV4dSet(corner, bb->min[0], bb->min[1], bb->max[2], (afxReal)1); break;
    case 1: AfxV4dSet(corner, bb->max[0], bb->min[1], bb->max[2], (afxReal)1); break;
    case 2: AfxV4dSet(corner, bb->max[0], bb->max[1], bb->max[2], (afxReal)1); break;
    case 3: AfxV4dSet(corner, bb->min[0], bb->max[1], bb->max[2], (afxReal)1); break;
    case 4: AfxV4dSet(corner, bb->min[0], bb->min[1], bb->min[2], (afxReal)1); break;
    case 5: AfxV4dSet(corner, bb->max[0], bb->min[1], bb->min[2], (afxReal)1); break;
    case 6: AfxV4dSet(corner, bb->max[0], bb->max[1], bb->min[2], (afxReal)1); break;
    case 7: AfxV4dSet(corner, bb->min[0], bb->max[1], bb->min[2], (afxReal)1); break;
    default: AfxThrowError(); break;
    }
}

_AFXINL void AfxTransformAabb(afxBox const* bb, afxM4d const m, afxBox* to)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(m);
    AFX_ASSERT(to);
    AfxResetBoxes(1, to);
    AfxM4dPostMultiplyV4d(m, 2, &bb->max, &to->max);
}

_AFXINL void AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxBox const in[], afxBox out[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // box = obb [min, max]

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d max, min, pos;
        AfxFillV3d(max, -3.4028235e38);
        AfxFillV3d(min, 3.4028235e38);

        for (afxUnit z = 0; z < 2; z++)
        {
            for (afxUnit y = 0; y < 2; y++)
            {
                for (afxUnit x = 0; x < 2; x++)
                {
                    afxV3d tmp;
                    AfxV3dSet(tmp,  x ? in[i].max[0] : in[i].min[0],
                                    y ? in[i].max[1] : in[i].min[1],
                                    z ? in[i].max[2] : in[i].min[2]);

                    AfxV3dPostMultiplyM3d(pos, ltm, tmp);
                    AfxV3dAdd(pos, pos, atv);

                    AfxV3dMin(min, min, pos);
                    AfxV3dMax(max, max, pos);
                }
            }
        }

        AfxV3dCopy(out[i].max, max);
        AfxV3dCopy(out[i].min, min);
    }
}

_AFXINL void AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxBox out[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(out);
    AFX_ASSERT(in);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = in[i];
}

_AFXINL void AfxResetBoxes(afxUnit cnt, afxBox boxes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(boxes);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
        boxes[i] = AFX_AABB_IDENTITY;
}
