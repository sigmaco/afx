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

#include "afx/math/afxFrustum.h"
#include "afx/math/afxMatrix.h"
#include "afx/math/afxPlane.h"

_AFXINL void AfxCopyFrustum(afxFrustum* f, afxFrustum const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(in);
    afxNat i;

    AfxCopyV4d(f->origin, in->origin);

    for (i = 0; i < afxCubeFace_TOTAL; i++)
        AfxCopyPlane(&(f->planes[i]), &(in->planes[i]));

    for (i = 0; i < afxCubeCorner_TOTAL; i++)
        AfxCopyV4d(f->corners[i], in->corners[i]);
}

_AFXINL afxBool AfxFrustumDoesCullSphere(afxFrustum const* f, afxSphere const* sph)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(sph);

    for (afxNat i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        if (AfxFindPlaneDistanceToPoint(&(f->planes[i]), sph->origin) > sph->radius)
            return TRUE;
    }
    return FALSE;
}

_AFXINL afxBool AfxFrustumDoesCullAabb(afxFrustum const* f, afxAabb const* aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(aabb);
    afxBool allOut = TRUE;

#if !0
    for (afxNat i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        afxReal const* n = f->planes[i].normal;

#if !0
        afxV4d positive;
        AfxCopyV4d(positive, aabb->sup);

        if (n[0] <= 0)
            positive[0] = aabb->inf[0];

        if (n[1] <= 0)
            positive[1] = aabb->inf[1];

        if (n[2] <= 0)
            positive[2] = aabb->inf[2];

        if (AfxFindPlaneDistanceToPoint(&(f->planes[i]), positive) < 0)
            allOut = FALSE;
#else
        for (afxNat j = 0; j < afxCubeCorner_TOTAL; ++j)
        {
            afxV4d corner, corner2;

            if (AfxAabbContainsPoint(aabb, f->corners[j]))
            {
                allOut = FALSE;
                break;
            }
        }

        if (!allOut)
            break;
#endif//0
#endif

    }
    return allOut;
}

_AFXINL afxBool AfxFrustumDoesCullFrustum(afxFrustum const* f, afxFrustum const* other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(other);

    afxBool allOut = TRUE;

    for (afxNat i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        for (afxNat j = 0; j < afxCubeCorner_TOTAL; ++j)
        {
            if (AfxFindPlaneDistanceToPoint(&(f->planes[i]), other->corners[j]) < 0)
            {
                allOut = FALSE;
                break;
            }
        }

        if (!allOut)
            break;
    }
    return allOut;
}

_AFXINL void AfxAabbEncapsulateFrustum(afxAabb* aabb, afxFrustum const* f)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(aabb);
#if !0
    AfxRecomputeAabb(aabb, f->corners, afxCubeCorner_TOTAL);
#else
    AfxSetV4d(aabb->inf, AFX_R32_MAX, AFX_R32_MAX, AFX_R32_MAX, 1);
    AfxSetV4d(aabb->sup, -AFX_R32_MAX, -AFX_R32_MAX, -AFX_R32_MAX, 1);

    for (afxNat i = 0; i < afxCubeCorner_TOTAL; ++i)
    {
        if (frustum->corners[i][0] < aabb->inf[0])
            aabb->inf[0] = frus->corners[i][0];

        if (frustum->corners[i][1] < aabb->inf[1])
            aabb->inf[1] = frus->corners[i][1];

        if (frustum->corners[i][2] < aabb->inf[2])
            aabb->inf[2] = frus->corners[i][2];

        if (frustum->corners[i][0] > aabb->sup[0])
            aabb->sup[0] = frus->corners[i][0];

        if (frustum->corners[i][1] > aabb->sup[1])
            aabb->sup[1] = frus->corners[i][1];

        if (frustum->corners[i][2] > aabb->sup[2])
            aabb->sup[2] = frus->corners[i][2];
    }
#endif
}

_AFXINL void AfxRecomputeFrustum(afxFrustum* f, afxReal const v[4][4], afxReal const p[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(v);
    AfxAssert(p);
    f->vol.type = AFX_VOL_FRUSTUM;

    afxM4d vp;
    AfxMultiplyM4d(vp, v, p); // TODO check if it is correct.
    AfxSetPlane(&(f->planes[afxCubeFace_L]), AfxSpawnV3d(-(vp[0][3] + vp[0][0]), -(vp[1][3] + vp[1][0]), -(vp[2][3] + vp[2][0])), -(vp[3][3] + vp[3][0]));
    AfxSetPlane(&(f->planes[afxCubeFace_R]), AfxSpawnV3d(-(vp[0][3] - vp[0][0]), -(vp[1][3] - vp[1][0]), -(vp[2][3] - vp[2][0])), -(vp[3][3] - vp[3][0]));
    AfxSetPlane(&(f->planes[afxCubeFace_B]), AfxSpawnV3d(-(vp[0][3] + vp[0][1]), -(vp[1][3] + vp[1][1]), -(vp[2][3] + vp[2][1])), -(vp[3][3] + vp[3][1]));
    AfxSetPlane(&(f->planes[afxCubeFace_T]), AfxSpawnV3d(-(vp[0][3] - vp[0][1]), -(vp[1][3] - vp[1][1]), -(vp[2][3] - vp[2][1])), -(vp[3][3] - vp[3][1]));
    AfxSetPlane(&(f->planes[afxCubeFace_N]), AfxSpawnV3d(-(vp[0][3] + vp[0][2]), -(vp[1][3] + vp[1][2]), -(vp[2][3] + vp[2][2])), -(vp[3][3] + vp[3][2]));
    AfxSetPlane(&(f->planes[afxCubeFace_F]), AfxSpawnV3d(-(vp[0][3] - vp[0][2]), -(vp[1][3] - vp[1][2]), -(vp[2][3] - vp[2][2])), -(vp[3][3] - vp[3][2]));

    afxM4d vi;
    AfxInvertM4d(vi, v);
    AfxTransformV4d(f->origin, AfxSpawnPointConst(0, 0, 0), vi);

    // Calculate corners
    afxM4d vpi;
    AfxInvertM4d(vpi, vp);
    afxV4d corner;

    AfxTransformV4d(corner, AfxSpawnPointConst(-1, -1, -1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_LBN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(1, -1, -1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_RBN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(1, 1, -1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_RTN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(-1, 1, -1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_LTN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(-1, -1, 1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_LBF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(1, -1, 1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_RBF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(1, 1, 1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_RTF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxTransformV4d(corner, AfxSpawnPointConst(-1, 1, 1), vpi);
    AfxCopyV4d(f->corners[afxCubeCorner_LTF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));
}
