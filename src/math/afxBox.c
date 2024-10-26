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

#include "qwadro/inc/math/afxBox.h"
#include "qwadro/inc/math/afxScalar.h"
#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/afxMatrix.h"

_AFXINL void AfxBoxReset(afxBox* bb)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AfxResetV4d(bb->sup);
    AfxResetV4d(bb->inf);
}

_AFXINL void AfxAabbSet(afxBox* bb, afxUnit cnt, afxV3d const points[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(points);
    AFX_ASSERT(cnt);
    AfxResetV4d(bb->sup);
    AfxResetV4d(bb->inf);
    AfxAabbAbsorbAtv3d(bb, cnt, points);
}

_AFXINL void AfxBoxCopy(afxBox* bb, afxBox const* in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(in);
    AfxV4dCopy(bb->sup, in->sup);
    AfxV4dCopy(bb->inf, in->inf);
}

_AFXINL void AfxAabbAbsorbAtv3d(afxBox* bb, afxUnit cnt, afxV3d const v[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(v);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMaxV3d(bb->sup, bb->sup, v[i]);
        AfxMinV3d(bb->inf, bb->inf, v[i]);
    }
}

_AFXINL void AfxAabbAbsorbAtv4d(afxBox* bb, afxUnit cnt, afxV4d const v[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(v);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMaxV3d(bb->sup, bb->sup, v[i]);
        AfxMinV3d(bb->inf, bb->inf, v[i]);
    }
}

_AFXINL void AfxAabbAbsorbSpheres(afxBox* bb, afxUnit cnt, afxSphere const sph[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(sph);
    
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d range[2], radius = { sph[i].radius, sph[i].radius, sph[i].radius };
        AfxV3dAdd(range[0], sph[i].centre, radius);
        AfxV3dSub(range[1], sph[i].centre, radius);
        AfxAabbAbsorbAtv3d(bb, 2, range);
    }
    bb->sup[3] = (bb->inf[3] = AfxScalar(1));
}

_AFXINL void AfxAabbAbsorb(afxBox* bb, afxUnit cnt, afxBox const other[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(other);

    for (afxUnit i = 0; i < cnt; i++)
        AfxAabbAbsorbAtv4d(bb, 2, &other[i].sup);
}

_AFXINL afxUnit AfxAabbTestAtv3d(afxBox const* bb, afxUnit cnt, afxV3d const point[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(point);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if
        (
            (!(bb->sup[0] < point[i][0])) && (!(bb->sup[1] < point[i][1])) && (!(bb->sup[2] < point[i][2])) &&
            (!(bb->inf[0] > point[i][0])) && (!(bb->inf[1] > point[i][1])) && (!(bb->inf[2] > point[i][2]))
        )
        {
            ++rslt;
        }
    }
    return rslt;
}

_AFXINL void AfxAabbGetExtents(afxBox const* bb, afxV3d extent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(extent);
#if 0
    AfxV4dSub(extent, bb->sup, bb->inf);
#else
    // 0.5 * (max - min)
    AfxV3dSub(extent, bb->sup, bb->inf);
    AfxHalfV3d(extent, extent);
#endif
}

_AFXINL void AfxAabbGetCentre(afxBox const* bb, afxV4d centre)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(centre);
    // 0.5 * (min + max)
    AfxV3dAdd(centre, bb->inf, bb->sup);
    AfxHalfV3d(centre, centre);
    centre[3] = 1.f;
}

_AFXINL void AfxAabbGetCorner(afxBox const* bb, afxUnit index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(corner);
    AFX_ASSERT(index < 8);

    switch (index)
    {
    case 0: AfxV4dSet(corner, bb->inf[0], bb->inf[1], bb->sup[2], (afxReal)1); break;
    case 1: AfxV4dSet(corner, bb->sup[0], bb->inf[1], bb->sup[2], (afxReal)1); break;
    case 2: AfxV4dSet(corner, bb->sup[0], bb->sup[1], bb->sup[2], (afxReal)1); break;
    case 3: AfxV4dSet(corner, bb->inf[0], bb->sup[1], bb->sup[2], (afxReal)1); break;
    case 4: AfxV4dSet(corner, bb->inf[0], bb->inf[1], bb->inf[2], (afxReal)1); break;
    case 5: AfxV4dSet(corner, bb->sup[0], bb->inf[1], bb->inf[2], (afxReal)1); break;
    case 6: AfxV4dSet(corner, bb->sup[0], bb->sup[1], bb->inf[2], (afxReal)1); break;
    case 7: AfxV4dSet(corner, bb->inf[0], bb->sup[1], bb->inf[2], (afxReal)1); break;
    default: AfxThrowError(); break;
    }
}

_AFXINL void AfxTransformAabb(afxBox const* bb, afxM4d const m, afxBox* to)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(bb);
    AFX_ASSERT(m);
    AFX_ASSERT(to);
    AfxBoxReset(to);
    AfxM4dPostMultiplyV4d(m, 2, &bb->sup, &to->sup);
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
                    AfxV3dSet(tmp, x ? in[i].sup[0] : in[i].inf[0], y ? in[i].sup[1] : in[i].inf[1], z ? in[i].sup[2] : in[i].inf[2]);

                    AfxV3dPostMultiplyM3d(pos, ltm, tmp);
                    AfxV3dAdd(pos, pos, atv);

                    AfxMinV3d(min, min, pos);
                    AfxMaxV3d(max, max, pos);
                }
            }
        }

        out[i].sup[0] = max[0];
        out[i].sup[1] = max[1];
        out[i].sup[2] = max[2];
        out[i].inf[0] = min[0];
        out[i].inf[1] = min[1];
        out[i].inf[2] = min[2];
    }
}

_AFXINL void AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxBox out[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(out);
    AFX_ASSERT(in);

    for (afxUnit i = 0; i < cnt; i++)
        AfxBoxCopy(&out[i], &in[i]);
}

_AFXINL void AfxResetBoxes(afxUnit cnt, afxBox boxes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(boxes);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
        AfxBoxReset(&boxes[i]);
}
