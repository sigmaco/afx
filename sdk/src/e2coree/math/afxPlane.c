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

#include "afx/math/afxVector.h"
#include "afx/math/afxPlane.h"

_AFXINL void AfxCopyPlane(afxPlane *p, afxPlane const *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(in);
    AfxCopyV3d(p->normal, in->normal); // dist also will be copied
    p->offset = in->offset;
}

_AFXINL void AfxResetPlane(afxPlane* p, afxReal const normal[3], afxReal dist)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(normal);
    afxReal invLen = AfxMagV3dRecip(normal);
    AfxScaleV3d(p->normal, normal, invLen); // normaliza
    p->offset = dist * invLen;
}

_AFXINL void AfxPlaneFromTriangle(afxPlane* p, afxReal const a[3], afxReal const b[3], afxReal const c[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    afxV3d ba, ca;
    AfxSubV3d(ba, b, a);
    AfxSubV3d(ca, c, a);
    AfxCrossV3d(p->normal, ba, ca);
    AfxGetNormalizedV3d(p->normal, p->normal);
    p->offset = -AfxDotV3d(p->normal, a);
}

_AFXINL afxReal* AfxGetPlaneNormal(afxPlane* p)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    return p->normal;
}

_AFXINL afxReal AfxGetPlaneOffset(afxPlane const* p)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    return p->offset;
}

_AFXINL afxReal AfxFindPlaneDistanceToPoint(afxPlane const* p, afxReal const point[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(point);
    return AfxDotV3d(p->normal, point) + p->offset;
}

_AFXINL afxReal AfxFindPlaneHitInterpolationConstant(afxPlane const* p, afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(a);
    AfxAssert(b);
    afxV3d t;
    AfxSubV3d(t, a, b);
    return (AfxFindPlaneDistanceToPoint(p, a)) / AfxDotV3d(p->normal, t);
}

_AFXINL afxResult AfxTestPlaneAgainstAabb(afxPlane const* p, afxAabb const* aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(aabb);

    afxReal minD;
    afxReal maxD;

    if (p->normal[0] > 0.0f)
    {
        minD = p->normal[0] * aabb->extremes[AFX_AABB_INF][0];
        maxD = p->normal[0] * aabb->extremes[AFX_AABB_SUP][0];
    }
    else
    {
        minD = p->normal[0] * aabb->extremes[AFX_AABB_SUP][0];
        maxD = p->normal[0] * aabb->extremes[AFX_AABB_INF][0];
    }

    if (p->normal[1] > 0.0f)
    {
        minD += p->normal[1] * aabb->extremes[AFX_AABB_INF][1];
        maxD += p->normal[1] * aabb->extremes[AFX_AABB_SUP][1];
    }
    else
    {
        minD += p->normal[1] * aabb->extremes[AFX_AABB_SUP][1];;
        maxD += p->normal[1] * aabb->extremes[AFX_AABB_INF][1];
    }

    if (p->normal[2] > 0.0f)
    {
        minD += p->normal[2] * aabb->extremes[AFX_AABB_INF][2];
        maxD += p->normal[2] * aabb->extremes[AFX_AABB_SUP][2];
    }
    else
    {
        minD += p->normal[2] * aabb->extremes[AFX_AABB_SUP][2];
        maxD += p->normal[2] * aabb->extremes[AFX_AABB_INF][2];
    }

    // What side of the plane

    if (minD >= -p->offset)
        return +1;

    if (maxD <= -p->offset)
        return -1;

    //Intersection
    return 0;
}

_AFXINL afxBool AfxTestPlaneAgainstSphere(afxPlane const* p, afxSphere const* s)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(s);

    if (AfxDotV3d(s->origin, p->normal) + p->offset <= -s->radius)
        return FALSE;

    return TRUE;
}
