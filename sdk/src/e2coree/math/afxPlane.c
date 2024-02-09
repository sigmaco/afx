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

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxPlane.h"

_AFXINL void AfxCopyPlane(afxPlane p, afxPlane const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(in);
    AfxCopyV4d(p, in);
    //AfxCopyV3d(p->normal, in->normal); // dist also will be copied
    //p->offset = in->offset;
}

_AFXINL void AfxResetPlane(afxPlane p, afxReal const normal[3], afxReal dist)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(normal);
    afxReal invLen = AfxMagV3d(normal);
    p[0] = normal[0] / invLen;
    p[1] = normal[1] / invLen;
    p[2] = normal[2] / invLen;
    p[AFX_PLANE_DIST] = dist * invLen;
}

_AFXINL void AfxPlaneFromTriangle(afxPlane p, afxReal const a[3], afxReal const b[3], afxReal const c[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    afxV3d ba, ca;
    AfxSubV3d(ba, b, a);
    AfxSubV3d(ca, c, a);
    AfxCrossV3d(p, ba, ca);
    AfxNormalV3d(p, p);
    p[AFX_PLANE_DIST] = -AfxDotV3d(p, a);
}

_AFXINL void AfxGetPlaneNormal(afxPlane p, afxReal normal[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(normal);
    normal[0] = p[0];
    normal[1] = p[1];
    normal[2] = p[2];
}

_AFXINL afxReal AfxGetPlaneOffset(afxPlane const p)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    return p[AFX_PLANE_OFFSET];
}

_AFXINL afxReal AfxFindPlaneDistance(afxPlane const p, afxReal const point[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(point);
    return AfxDotV3d(p, point) + p[AFX_PLANE_OFFSET];
}

_AFXINL afxReal AfxFindPlaneHitInterpolationConstant(afxPlane const p, afxReal const a[3], afxReal const b[3])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(a);
    AfxAssert(b);
    afxV3d t;
    AfxSubV3d(t, a, b);
    return (AfxFindPlaneDistance(p, a)) / AfxDotV3d(p, t);
}

_AFXINL afxResult AfxTestPlaneAgainstAabb(afxPlane const p, afxAabb const aabb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(aabb);

    afxReal minD;
    afxReal maxD;

    if (p[0] > 0.0f)
    {
        minD = p[0] * aabb[AFX_AABB_INF][0];
        maxD = p[0] * aabb[AFX_AABB_SUP][0];
    }
    else
    {
        minD = p[0] * aabb[AFX_AABB_SUP][0];
        maxD = p[0] * aabb[AFX_AABB_INF][0];
    }

    if (p[1] > 0.0f)
    {
        minD += p[1] * aabb[AFX_AABB_INF][1];
        maxD += p[1] * aabb[AFX_AABB_SUP][1];
    }
    else
    {
        minD += p[1] * aabb[AFX_AABB_SUP][1];;
        maxD += p[1] * aabb[AFX_AABB_INF][1];
    }

    if (p[2] > 0.0f)
    {
        minD += p[2] * aabb[AFX_AABB_INF][2];
        maxD += p[2] * aabb[AFX_AABB_SUP][2];
    }
    else
    {
        minD += p[2] * aabb[AFX_AABB_SUP][2];
        maxD += p[2] * aabb[AFX_AABB_INF][2];
    }

    // What side of the plane

    if (minD >= -p[AFX_PLANE_OFFSET])
        return +1;

    if (maxD <= -p[AFX_PLANE_OFFSET])
        return -1;

    //Intersection
    return 0;
}

_AFXINL afxBool AfxTestPlaneAgainstSphere(afxPlane const p, afxSphere const* s)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(s);

    if (AfxDotV3d(s->origin, p) + p[AFX_PLANE_OFFSET] <= -s->radius)
        return FALSE;

    return TRUE;
}
