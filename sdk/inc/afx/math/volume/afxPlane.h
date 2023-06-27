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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_PLANE_H
#define AFX_PLANE_H

#include "../afxVector.h"

AFX_DEFINE_STRUCT(afxPlane)
{
    afxV3d  normal;
    afxReal dist;
};

AFXINL afxPlane* AfxPlaneCopy(afxPlane *p, afxPlane const *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(in);
    AfxV3dCopy(p->normal, in->normal); // dist also will be copied
    p->dist = in->dist;
    return p;
}

AFXINL afxPlane* AfxPlaneSet(afxPlane* plan, afxV3d const normal, afxReal dist)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(plan);
    AfxAssert(normal);
    afxReal invLen = AfxV3dMagnitudeRecip(normal);
    AfxV3dScale(normal, invLen, plan->normal); // normaliza
    plan->dist = dist * invLen;
    return plan;
}

AFXINL afxReal* AfxPlaneGetOrigin(afxPlane* plan)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(plan);
    return plan->normal;
}

AFXINL afxReal AfxPlaneGetDistance(afxPlane const* plan)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(plan);
    return plan->dist;
}

AFXINL afxReal AfxPlaneGetDistanceToPoint(afxPlane const* plan, afxV3d const point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(plan);
    AfxAssert(point);
    return AfxV3dDot(plan->normal, point) + plan->dist;
}

#endif//AFX_PLANE_H