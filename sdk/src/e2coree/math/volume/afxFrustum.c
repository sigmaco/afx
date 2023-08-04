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

#include "afx/math/volume/afxFrustum.h"

_AFXINL afxFrustum* AfxFrustumCopy(afxFrustum *f, afxFrustum const *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(in);
    afxNat i;

    AfxV4dCopy(f->origin, in->origin);

    for (i = 0; i < AFX_FRUS_PLAN_TOTAL; i++)
        AfxPlaneCopy(&(f->planes[i]), &(in->planes[i]));

    for (i = 0; i < AFX_FRUS_CORN_TOTAL; i++)
        AfxV4dCopy(f->corners[i], in->corners[i]);

    return f;
}

_AFXINL afxBool AfxFrustumDoesCullSphere(afxFrustum const *f, afxSphere const* sph)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(sph);

    for (afxNat i = 0; i < AFX_FRUS_PLAN_TOTAL; ++i)
    {
        if (AfxPlaneGetDistanceToPoint(&(f->planes[i]), sph->origin) > sph->radius)
            return TRUE;
    }
    return FALSE;
}

_AFXINL afxBool AfxFrustumDoesCullAabb(afxFrustum const *f, afxAabb const* aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(aabb);
    afxBool allOut = TRUE;

#if !0
    for (afxNat i = 0; i < AFX_FRUS_PLAN_TOTAL; ++i)
    {
        afxReal const* n = f->planes[i].normal;

#if !0
        afxV4d positive;
        AfxV4dCopy(positive, aabb->sup);

        if (n[0] <= 0)
            positive[0] = aabb->inf[0];

        if (n[1] <= 0)
            positive[1] = aabb->inf[1];

        if (n[2] <= 0)
            positive[2] = aabb->inf[2];

        if (AfxPlaneGetDistanceToPoint(&(f->planes[i]), positive) < 0)
            allOut = FALSE;
#else
        for (afxNat j = 0; j < AFX_FRUS_CORN_TOTAL; ++j)
        {
            afxV4d corner, corner2;

            if (AfxAabbDoesContain(aabb, f->corners[j]))
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

_AFXINL afxBool AfxFrustumDoesCullFrustum(afxFrustum const *f, afxFrustum const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(other);

    afxBool allOut = TRUE;

    for (afxNat i = 0; i < AFX_FRUS_PLAN_TOTAL; ++i)
    {
        for (afxNat j = 0; j < AFX_FRUS_CORN_TOTAL; ++j)
        {
            if (AfxPlaneGetDistanceToPoint(&(f->planes[i]), other->corners[j]) < 0)
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

_AFXINL afxAabb* AfxAabbEncapsulateFrustum(afxAabb *aabb, afxFrustum const *f)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(aabb);
#if !0
    AfxAabbRecompute(aabb, f->corners, AFX_FRUS_CORN_TOTAL);
#else
    AfxV4dSet(aabb->inf, AFX_R32_MAX, AFX_R32_MAX, AFX_R32_MAX, 1);
    AfxV4dSet(aabb->sup, -AFX_R32_MAX, -AFX_R32_MAX, -AFX_R32_MAX, 1);

    for (afxNat i = 0; i < AFX_FRUS_CORN_TOTAL; ++i)
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
    return aabb;
}

_AFXINL afxFrustum* AfxFrustumRecompute(afxFrustum *f, afxM4d const v, const afxM4d p)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(v);
    AfxAssert(p);
    f->vol.type = AFX_VOL_FRUSTUM;

    afxM4d vp;
    AfxM4dMultiply(v, p, vp); // TODO check if it is correct.
    AfxPlaneSet(&(f->planes[AFX_FRUS_PLAN_L]), AfxSpawnV3d(-(vp[0][3] + vp[0][0]), -(vp[1][3] + vp[1][0]), -(vp[2][3] + vp[2][0])), -(vp[3][3] + vp[3][0]));
    AfxPlaneSet(&(f->planes[AFX_FRUS_PLAN_R]), AfxSpawnV3d(-(vp[0][3] - vp[0][0]), -(vp[1][3] - vp[1][0]), -(vp[2][3] - vp[2][0])), -(vp[3][3] - vp[3][0]));
    AfxPlaneSet(&(f->planes[AFX_FRUS_PLAN_B]), AfxSpawnV3d(-(vp[0][3] + vp[0][1]), -(vp[1][3] + vp[1][1]), -(vp[2][3] + vp[2][1])), -(vp[3][3] + vp[3][1]));
    AfxPlaneSet(&(f->planes[AFX_FRUS_PLAN_T]), AfxSpawnV3d(-(vp[0][3] - vp[0][1]), -(vp[1][3] - vp[1][1]), -(vp[2][3] - vp[2][1])), -(vp[3][3] - vp[3][1]));
    AfxPlaneSet(&(f->planes[AFX_FRUS_PLAN_N]), AfxSpawnV3d(-(vp[0][3] + vp[0][2]), -(vp[1][3] + vp[1][2]), -(vp[2][3] + vp[2][2])), -(vp[3][3] + vp[3][2]));
    AfxPlaneSet(&(f->planes[AFX_FRUS_PLAN_F]), AfxSpawnV3d(-(vp[0][3] - vp[0][2]), -(vp[1][3] - vp[1][2]), -(vp[2][3] - vp[2][2])), -(vp[3][3] - vp[3][2]));

    afxM4d vi;
    AfxM4dInvert(v, vi);
    AfxV4dMultiplyM4d(AfxSpawnPointConst(0, 0, 0), vi, f->origin);

    // Calculate corners
    afxM4d vpi;
    AfxM4dInvert(vp, vpi);
    afxV4d corner;

    AfxV4dMultiplyM4d(AfxSpawnPointConst(-1, -1, -1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_LBN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(1, -1, -1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_RBN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(1, 1, -1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_RTN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(-1, 1, -1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_LTN], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(-1, -1, 1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_LBF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(1, -1, 1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_RBF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(1, 1, 1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_RTF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    AfxV4dMultiplyM4d(AfxSpawnPointConst(-1, 1, 1), vpi, corner);
    AfxV4dCopy(f->corners[AFX_FRUS_CORN_LTF], AfxSpawnPointConst(corner[0] / corner[3], corner[1] / corner[3], corner[2] / corner[3]));

    return f;
}
