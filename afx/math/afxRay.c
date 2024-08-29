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

#include "qwadro/math/afxBox.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxRay.h"

_AFXINL afxReal AfxRayIntersectsPlaneAt(afxRay const* ray, afxPlane plane, afxReal* T)
{
    afxError err = NIL;
    AfxAssert3(ray, plane, T);
    afxReal dot = AfxV3dDot(ray->normal, plane);

    if (dot != 0.0)
    {
        afxReal diff = -((AfxV3dDot(ray->origin, plane) + plane[AFX_PLANE_DIST]) / dot);
        *T = diff;
        dot = diff;
    }
    return dot;
}

_AFXINL afxReal AfxRayIntersectsSphere(afxRay const* ray, afxSphere const* sph)
{
    afxError err = NIL;
    AfxAssert2(ray, sph);
    afxV3d o;
    AfxV3dSub(o, sph->centre, ray->origin);
    afxReal dot = AfxV3dDot(o, ray->normal);
    return dot;
}

_AFXINL afxInt AfxRayIntersectsSphereAt(afxRay const* ray, afxSphere const* sph, afxReal* minT, afxReal* maxT)
{
    afxError err = NIL;
    AfxAssert4(ray, sph, minT, maxT);
    afxInt rslt = 0;
    
    afxV3d o;
    AfxV3dSub(o, sph->centre, ray->origin);
    afxReal dot = AfxV3dDot(o, ray->normal);

    afxReal diff = dot * dot - AfxV3dSq(o) + sph->radius * sph->radius;

    if (diff < 0.0)
    {
        afxReal diffSqrt = AfxSqrt(diff);
        *minT = dot - diffSqrt;
        *maxT = dot + diffSqrt;

        if (*minT >= 0.0 || *maxT >= 0.0)
            rslt = 1;
        else
            rslt = -1;
    }
    return rslt;
}


