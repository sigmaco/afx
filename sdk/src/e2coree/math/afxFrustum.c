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

#include "qwadro/math/afxFrustum.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxPlane.h"

_AFX afxV4d const AFX_V4D_LBN = { -1, -1, -1, 1 };
_AFX afxV4d const AFX_V4D_RBN = { 1, -1, -1, 1 };
_AFX afxV4d const AFX_V4D_RTN = { 1, 1, -1, 1 };
_AFX afxV4d const AFX_V4D_LTN = { -1, 1, -1, 1 };

_AFX afxV4d const AFX_V4D_LBF = { -1, -1, 1, 1 };
_AFX afxV4d const AFX_V4D_RBF = { 1, -1, 1, 1 };
_AFX afxV4d const AFX_V4D_RTF = { 1, 1, 1, 1 };
_AFX afxV4d const AFX_V4D_LTF = { -1, 1, 1, 1 };

_AFXINL void AfxCopyFrustum(afxFrustum* f, afxFrustum const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(in);
    afxNat i;

    AfxCopyV4d(f->origin, in->origin);

    for (i = 0; i < afxCubeFace_TOTAL; i++)
        AfxCopyPlane(f->planes[i], in->planes[i]);

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
        if (!AfxTestPlaneAgainstSphere(f->planes[i], sph))
            return FALSE;
    }
    return TRUE;
}

_AFXINL afxBool AfxFrustumDoesCullAabb(afxFrustum const* f, afxAabb const aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(aabb);

    for (afxNat i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        if (AfxTestPlaneAgainstAabb(f->planes[i], aabb) == -1)
            return FALSE;
    }
    return TRUE;

    afxV4d v;
    afxBool volInside = TRUE;

    // check box outside/inside of frustum

    for (afxNat i = 0; i < afxCubeFace_TOTAL; i++)
    {
        v[0] = f->planes[i][0];
        v[1] = f->planes[i][1];
        v[2] = f->planes[i][2];
        v[3] = f->planes[i][3];

        afxNat out = 0;
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxDotV4d(v, AfxSpawnV4d(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        
        if (out == 8)
        {
            volInside = FALSE;
            break;
        }
    }

    if (volInside)
    {
        // check frustum outside/inside box
        
        for (afxNat j = 0; j < 3; j++)
        {
            afxNat out;

            for (afxNat i = 0; i < afxCubeCorner_TOTAL; i++)
                out += ((f->corners[i][j] > aabb[AFX_AABB_SUP][j]) ? 1 : 0);

            if (out == 8)
            {
                volInside = FALSE;
                break;
            }

            for (afxNat i = 0; i < afxCubeCorner_TOTAL; i++)
                out += ((f->corners[i][j] < aabb[AFX_AABB_INF][j]) ? 1 : 0);

            if (out == 8)
            {
                volInside = FALSE;
                break;
            }
        }
    }
    return !volInside;
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
            if (AfxFindPlaneDistance(f->planes[i], other->corners[j]) < 0)
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

_AFXINL void AfxAabbEncapsulateFrustum(afxAabb aabb, afxFrustum const* f)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(aabb);
    AfxEncapsulatePoints(aabb, afxCubeCorner_TOTAL, f->corners);
}

_AFXINL void AfxRecomputeFrustumPlanes(afxFrustum* f, afxM4d const pv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(pv);
    afxV3d x, y, z, w;
    AfxSetV3d(x, pv[0][0], pv[1][0], pv[2][0]);
    AfxSetV3d(y, pv[0][1], pv[1][1], pv[2][1]);
    AfxSetV3d(z, pv[0][2], pv[1][2], pv[2][2]);
    AfxSetV3d(w, pv[0][3], pv[1][3], pv[2][3]);

    afxV3d wMx, wPx, wMy, wPy, wMz, wPz;
    AfxSubV3d(wMx, w, x);
    AfxAddV3d(wPx, w, x);
    AfxSubV3d(wMy, w, y);
    AfxAddV3d(wPy, w, y);
    AfxSubV3d(wMz, w, z);
    AfxAddV3d(wPz, w, z);

    afxReal ww = pv[3][3];

    AfxResetPlane(f->planes[afxCubeFace_R], wMx, ww - pv[3][0]); // Right
    AfxResetPlane(f->planes[afxCubeFace_L], wPx, ww + pv[3][0]); // Left

    AfxResetPlane(f->planes[afxCubeFace_B], wPy, ww + pv[3][1]); // Bottom
    AfxResetPlane(f->planes[afxCubeFace_T], wMy, ww - pv[3][1]); // Top 

    AfxResetPlane(f->planes[afxCubeFace_F], wMz, ww - pv[3][2]); // Near (LH Coords)
    AfxResetPlane(f->planes[afxCubeFace_B], wPz, ww + pv[3][2]); // Far
}

_AFXINL void AfxRecomputeFrustumCorners(afxFrustum* f, afxM4d const pv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(pv);

    afxV4d v;
    AfxPostMultiplyV4d(pv, AFX_V4D_LBN, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_LBN], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_RBN, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_RBN], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_RTN, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_RTN], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_LTN, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_LTN], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_LBF, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_LBF], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_RBF, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_RBF], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_RTF, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_RTF], v);

    AfxPostMultiplyV4d(pv, AFX_V4D_LTF, v);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxCopyV4d(f->corners[afxCubeCorner_LTF], v);
}

_AFXINL void AfxRecomputeFrustum3(afxFrustum* f, afxM4d const pv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(pv);
    AfxRecomputeFrustumPlanes(f, pv);
    AfxRecomputeFrustumCorners(f, pv);
}

_AFXINL void AfxRecomputeFrustum(afxFrustum* f, afxM4d const v, afxM4d const p)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(v);
    AfxAssert(p);

    afxM4d pv;
    AfxMultiplyM4dTransposed(pv, v, p);
    AfxPostMultiplyV4d(pv, AFX_V4D_ZERO, f->origin);
    AfxRecomputeFrustum3(f, pv);
}
