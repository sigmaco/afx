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

#ifndef AFX_PLANE_H
#define AFX_PLANE_H

#include "afx/math/afxVector.h"
#include "afxAabb.h"

//typedef afxV4d afxPlane;

AFX_DEFINE_STRUCT(afxPlane)
{
    afxV3d  normal;
    afxReal offset; // aka dist
};

AFXINL void     AfxCopyPlane(afxPlane *p, afxPlane const *in);

AFXINL void     AfxResetPlane(afxPlane* p, afxReal const normal[3], afxReal dist);

AFXINL void     AfxPlaneFromTriangle(afxPlane* p, afxReal const a[3], afxReal const b[3], afxReal const c[3]);

AFXINL afxReal* AfxGetPlaneNormal(afxPlane* p);

AFXINL afxReal  AfxGetPlaneOffset(afxPlane const* p); // aka GetPlaneDistance
#define AfxGetPlaneDistance AfxGetPlaneOffset

AFXINL afxReal  AfxFindPlaneDistanceToPoint(afxPlane const* p, afxReal const point[3]);

AFXINL afxReal  AfxFindPlaneHitInterpolationConstant(afxPlane const* p, afxReal const a[3], afxReal const b[3]);

/// Test this plane with an AABB
/// <0 if the box is completly on the back side of the plane
/// >0 if the box is completly on the front side of the plane
/// 0 if the box intersects with the plane
AFXINL afxResult    AfxTestPlaneAgainstAabb(afxPlane const* p, afxAabb const* aabb);

/// Test this plane with a collision sphere.
AFXINL afxBool AfxTestPlaneAgainstSphere(afxPlane const* p, afxSphere const* s);

#endif//AFX_PLANE_H