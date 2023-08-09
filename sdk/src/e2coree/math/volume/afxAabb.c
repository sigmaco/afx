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

#include "afx/math/volume/afxAabb.h"
#include "afx/math/afxMathDefs.h"

// UNARY

_AFXINL void AfxAabbMakeIdentity(afxAabb *aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    aabb->vol.type = AFX_VOL_AABB;
    AfxV4dMakeIdentity(aabb->sup);
    AfxV4dMakeIdentity(aabb->inf);
}

// BINARY

_AFXINL void AfxAabbCopy(afxAabb *aabb, afxAabb const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(src);
    AfxV4dCopy(aabb->sup, src->sup);
    AfxV4dCopy(aabb->inf, src->inf);
}

_AFXINL void AfxAabbEncapsulatePoint(afxAabb *aabb, afxV4d const point)
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

_AFXINL void AfxAabbEncapsulateSphere(afxAabb *aabb, afxSphere const *sph)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(sph);
    afxV4d pnt, radius = { sph->radius, sph->radius, sph->radius, AfxScalar(0) };
    AfxV3dAdd(sph->origin, radius, pnt);
    AfxAabbEncapsulatePoint(aabb, pnt);
    AfxV3dSub(sph->origin, radius, pnt);
    AfxAabbEncapsulatePoint(aabb, pnt);
    aabb->sup[3] = (aabb->inf[3] = AfxScalar(1));
}

_AFXINL void AfxAabbEncapsulateAabb(afxAabb *aabb, afxAabb const *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(other);
    AfxAabbEncapsulatePoint(aabb, other->sup);
    AfxAabbEncapsulatePoint(aabb, other->inf);
}

_AFXINL void AfxAabbRecompute(afxAabb *aabb, afxV4d const points[], afxNat cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(points);
    AfxAssert(cnt);
    AfxV4dCopy(aabb->sup, *(points));
    AfxV4dCopy(aabb->inf, *(points));

    while (--(cnt))
        AfxAabbEncapsulatePoint(aabb, *(++(points)));
}

_AFXINL afxBool AfxAabbDoesContain(afxAabb const *aabb, afxV4d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(point);
    return (!(aabb->sup[0] < (point)[0])) && (!(aabb->sup[1] < (point)[1])) && (!(aabb->sup[2] < (point)[2])) && (!(aabb->inf[0] > (point)[0])) && (!(aabb->inf[1] > (point)[1])) && (!(aabb->inf[2] > (point)[2]));
}

_AFXINL void AfxAabbGetExtent(afxAabb const *aabb, afxV4d extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(extent);
    AfxV4dSub(aabb->sup, aabb->inf, extent);
}

_AFXINL void AfxAabbGetOrigin(afxAabb const *aabb, afxV4d origin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(origin);
    AfxV4dAdd(aabb->sup, aabb->inf, origin);
    AfxV4dScale(origin, (afxReal)0.5, origin);
}

_AFXINL void AfxAabbExtractCorner(afxAabb const *aabb, afxNat index, afxV4d corner)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(corner);
    AfxAssert(index < 8);

    switch (index)
    {
    case 0: AfxV4dSet(corner, aabb->inf[0], aabb->inf[1], aabb->sup[2], (afxReal)1); break;
    case 1: AfxV4dSet(corner, aabb->sup[0], aabb->inf[1], aabb->sup[2], (afxReal)1); break;
    case 2: AfxV4dSet(corner, aabb->sup[0], aabb->sup[1], aabb->sup[2], (afxReal)1); break;
    case 3: AfxV4dSet(corner, aabb->inf[0], aabb->sup[1], aabb->sup[2], (afxReal)1); break;
    case 4: AfxV4dSet(corner, aabb->inf[0], aabb->inf[1], aabb->inf[2], (afxReal)1); break;
    case 5: AfxV4dSet(corner, aabb->sup[0], aabb->inf[1], aabb->inf[2], (afxReal)1); break;
    case 6: AfxV4dSet(corner, aabb->sup[0], aabb->sup[1], aabb->inf[2], (afxReal)1); break;
    case 7: AfxV4dSet(corner, aabb->inf[0], aabb->sup[1], aabb->inf[2], (afxReal)1); break;
    default: AfxThrowError(); break;
    }
}

_AFXINL void AfxAabbTransform(afxAabb const* aabb, afxM4d const m, afxAabb* to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(aabb);
    AfxAssert(m);
    AfxAssert(to);
    AfxAabbMakeIdentity(to);
    AfxV4dMultiplyM4d(aabb->sup, m, to->sup);
    AfxV4dMultiplyM4d(aabb->inf, m, to->inf);
}
