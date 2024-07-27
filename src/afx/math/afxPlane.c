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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxPlane.h"

_AFXINL void AfxCopyPlane(afxPlane p, afxPlane const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(in);
    AfxV4dCopy(p, in);
    //AfxV3dCopy(p->normal, in->normal); // dist also will be copied
    //p->offset = in->offset;
}

_AFXINL void AfxResetPlane(afxPlane p, afxV3d const normal, afxReal dist)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(normal);
    afxReal invLen = AfxV3dMag(normal);
    p[0] = normal[0] / invLen;
    p[1] = normal[1] / invLen;
    p[2] = normal[2] / invLen;
    p[AFX_PLANE_DIST] = dist * invLen;
}

_AFXINL void AfxPlaneFromTriangle(afxPlane p, afxV3d const a, afxV3d const b, afxV3d const c)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(a);
    AfxAssert(b);
    AfxAssert(c);

    afxV3d ba, ca;
    AfxV3dSub(ba, b, a);
    AfxV3dSub(ca, c, a);
    AfxV3dCross(p, ba, ca);
    AfxV3dNormalize(p, p);
    p[AFX_PLANE_DIST] = -AfxV3dDot(p, a);
}

_AFXINL void AfxGetPlaneNormal(afxPlane p, afxV3d normal)
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

_AFXINL afxReal AfxFindPlaneDistance(afxPlane const p, afxV3d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(point);
    return AfxV3dDot(p, point) + p[AFX_PLANE_OFFSET];
}

_AFXINL afxReal AfxFindPlaneHitInterpolationConstant(afxPlane const p, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(a);
    AfxAssert(b);
    afxV3d t;
    AfxV3dSub(t, a, b);
    return (AfxFindPlaneDistance(p, a)) / AfxV3dDot(p, t);
}

_AFXINL afxResult AfxTestPlaneAgainstAabb(afxPlane const p, afxBox const aabb)
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

    if (AfxV3dDot(s->centre, p) + p[AFX_PLANE_OFFSET] <= -s->radius)
        return FALSE;

    return TRUE;
}
