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

#include "afx/math/afxAabb.h"
#include "afx/math/afxMathDefs.h"
#include "afx/math/afxVector.h"

// UNARY

_AFXINL void AfxResetAabb(afxAabb *aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxResetV4d(aabb->sup);
    AfxResetV4d(aabb->inf);
}

// BINARY

_AFXINL void AfxCopyAabb(afxAabb *aabb, afxAabb const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(src);
    AfxCopyV4d(aabb->sup, src->sup);
    AfxCopyV4d(aabb->inf, src->inf);
}

_AFXINL void AfxEncapsulatePoint(afxAabb *aabb, afxV4d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(point);

    if (aabb->sup[0] < (point)[0])
        aabb->sup[0] = (point)[0];

    if (aabb->sup[1] < (point)[1])
        aabb->sup[1] = (point)[1];

    if (aabb->sup[2] < (point)[2])
        aabb->sup[2] = (point)[2];

    if (aabb->inf[0] > (point)[0])
        aabb->inf[0] = (point)[0];

    if (aabb->inf[1] > (point)[1])
        aabb->inf[1] = (point)[1];

    if (aabb->inf[2] > (point)[2])
        aabb->inf[2] = (point)[2];
}

_AFXINL void AfxEncapsulateSphere(afxAabb *aabb, afxSphere const *sph)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(sph);
    afxV4d pnt, radius = { sph->radius, sph->radius, sph->radius, AfxScalar(0) };
    AfxAddV3d(pnt, sph->origin, radius);
    AfxEncapsulatePoint(aabb, pnt);
    AfxSubV3d(pnt, sph->origin, radius);
    AfxEncapsulatePoint(aabb, pnt);
    aabb->sup[3] = (aabb->inf[3] = AfxScalar(1));
}

_AFXINL void AfxEncapsulateAabb(afxAabb *aabb, afxAabb const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(other);
    AfxEncapsulatePoint(aabb, other->sup);
    AfxEncapsulatePoint(aabb, other->inf);
}

_AFXINL void AfxRecomputeAabb(afxAabb *aabb, afxV4d const points[], afxNat cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(points);
    AfxAssert(cnt);
    AfxCopyV4d(aabb->sup, *(points));
    AfxCopyV4d(aabb->inf, *(points));

    while (--(cnt))
        AfxEncapsulatePoint(aabb, *(++(points)));
}

_AFXINL afxBool AfxAabbContainsPoint(afxAabb const *aabb, afxV4d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(point);
    return (!(aabb->sup[0] < (point)[0])) && (!(aabb->sup[1] < (point)[1])) && (!(aabb->sup[2] < (point)[2])) && (!(aabb->inf[0] > (point)[0])) && (!(aabb->inf[1] > (point)[1])) && (!(aabb->inf[2] > (point)[2]));
}

_AFXINL void AfxGetAabbExtent(afxAabb const *aabb, afxV4d extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(extent);
    AfxSubV4d(extent, aabb->sup, aabb->inf);
}

_AFXINL void AfxGetAabbOrigin(afxAabb const *aabb, afxV4d origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(origin);
    AfxAddV4d(origin, aabb->sup, aabb->inf);
    AfxScaleV4d(origin, origin, (afxReal)0.5);
}

_AFXINL void AfxExtractAabbCorner(afxAabb const *aabb, afxNat index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(corner);
    AfxAssert(index < 8);

    switch (index)
    {
    case 0: AfxSetV4d(corner, aabb->inf[0], aabb->inf[1], aabb->sup[2], (afxReal)1); break;
    case 1: AfxSetV4d(corner, aabb->sup[0], aabb->inf[1], aabb->sup[2], (afxReal)1); break;
    case 2: AfxSetV4d(corner, aabb->sup[0], aabb->sup[1], aabb->sup[2], (afxReal)1); break;
    case 3: AfxSetV4d(corner, aabb->inf[0], aabb->sup[1], aabb->sup[2], (afxReal)1); break;
    case 4: AfxSetV4d(corner, aabb->inf[0], aabb->inf[1], aabb->inf[2], (afxReal)1); break;
    case 5: AfxSetV4d(corner, aabb->sup[0], aabb->inf[1], aabb->inf[2], (afxReal)1); break;
    case 6: AfxSetV4d(corner, aabb->sup[0], aabb->sup[1], aabb->inf[2], (afxReal)1); break;
    case 7: AfxSetV4d(corner, aabb->inf[0], aabb->sup[1], aabb->inf[2], (afxReal)1); break;
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
    AfxTransformV4d(to->sup, m, aabb->sup);
    AfxTransformV4d(to->inf, m, aabb->inf);
}

_AFXINL void AfxTransformBoundingBox(afxAabb* aabb, afxAabb const* in, afxReal const affine[3], afxReal linear[3][3])
{
    // Should be compatible with void TransformBoundingBox(const float *Affine3, const float *Linear3x3, float *OBBMin, float *OBBMax)
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(in);
    AfxAssert(affine);
    AfxAssert(linear);

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
                AfxSetV3d(tmp, x ? aabb->sup[0] : aabb->inf[0], y ? aabb->sup[1] : aabb->inf[1], z ? aabb->sup[2] : aabb->inf[2]);

                AfxTransformV3d(pos, linear, tmp);

                AfxMiniV3d(min, min, pos);
                AfxMaxiV3d(max, max, pos);
            }
        }
    }

    aabb->sup[0] = max[0];
    aabb->sup[1] = max[1];
    aabb->sup[2] = max[2];
    aabb->inf[0] = min[0];
    aabb->inf[1] = min[1];
    aabb->inf[2] = min[2];
}
