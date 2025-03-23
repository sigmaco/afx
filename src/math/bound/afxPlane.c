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

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/bound/afxPlane.h"

_AFXINL void AfxMakePlane(afxPlane* p, afxV3d const normal, afxReal dist)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(p);
    AFX_ASSERT(normal);
    afxReal invLen = AfxV3dMag(normal);
    p->normal[0] = normal[0] / invLen;
    p->normal[1] = normal[1] / invLen;
    p->normal[2] = normal[2] / invLen;
    p->dist = dist * invLen;
}

_AFXINL void AfxMakePlaneFromTriangle(afxPlane* p, afxV3d const a, afxV3d const b, afxV3d const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(p);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    afxV3d ba, ca;
    AfxV3dSub(ba, b, a);
    AfxV3dSub(ca, c, a);
    AfxV3dCross(p->normal, ba, ca);
    AfxV3dNormalize(p->normal, p->normal);
    p->dist = -AfxV3dDot(p->normal, a);
}

_AFXINL void AfxGetPlaneNormal(afxPlane* p, afxV3d normal)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(p);
    AFX_ASSERT(normal);
    AfxV3dCopy(normal, p->normal);
}

_AFXINL afxReal AfxGetPlaneOffset(afxPlane const* p)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(p);
    return p->offset;
}

_AFXINL afxReal AfxFindPlaneDistance(afxPlane const* p, afxV3d const point)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(p);
    AFX_ASSERT(point);
    return AfxV3dDot(p->normal, point) + p->offset;
}

_AFXINL afxReal AfxFindPlaneHitInterpolationConstant(afxPlane const* p, afxV3d const a, afxV3d const b)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(p);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    afxV3d t;
    AfxV3dSub(t, a, b);
    return (AfxFindPlaneDistance(p, a)) / AfxV3dDot(p->normal, t);
}

_AFXINL afxBool AfxPlaneTestSpheres(afxPlane const* p, afxUnit cnt, afxSphere const spheres[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT2(p, spheres);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxSphere const* s = &spheres[i];

        if (AfxV3dDot(s->centre, p->normal) + p->offset <= -s->radius)
            return FALSE;
    }
    return TRUE;
}

_AFXINL afxResult AfxPlaneTestAabbs(afxPlane const* p, afxUnit cnt, afxBox const aabb[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT2(p, aabb);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal minD;
        afxReal maxD;

        if (p->normal[0] > 0.0f)
        {
            minD = p->normal[0] * aabb[i].min[0];
            maxD = p->normal[0] * aabb[i].max[0];
        }
        else
        {
            minD = p->normal[0] * aabb[i].max[0];
            maxD = p->normal[0] * aabb[i].min[0];
        }

        if (p->normal[1] > 0.0f)
        {
            minD += p->normal[1] * aabb[i].min[1];
            maxD += p->normal[1] * aabb[i].max[1];
        }
        else
        {
            minD += p->normal[1] * aabb[i].max[1];;
            maxD += p->normal[1] * aabb[i].min[1];
        }

        if (p->normal[2] > 0.0f)
        {
            minD += p->normal[2] * aabb[i].min[2];
            maxD += p->normal[2] * aabb[i].max[2];
        }
        else
        {
            minD += p->normal[2] * aabb[i].max[2];
            maxD += p->normal[2] * aabb[i].min[2];
        }

        // What side of the plane

        if (minD >= -p->offset)
            return +1;

        if (maxD <= -p->offset)
            return -1;
    }
    //Intersection
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxCopyPlanes(afxUnit cnt, afxPlane const src[], afxPlane dst[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dCopy(dst[i].normal, src[i].normal);
        dst[i].offset = src[i].offset;
    }
}
