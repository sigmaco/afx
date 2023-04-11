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

#ifndef AFX_AABB_H
#define AFX_AABB_H

#include "../afxVector.h"
#include "afxSphere.h"
#include "afxVolume.h"

/// An axis-aligned bounding box, or AABB for short, is a box aligned with coordinate axes and fully enclosing some object.
/// Because the box is never rotated with respect to the axes, it canv be defined by supremum and infemum points.

AFX_DEFINE_STRUCT(afxAabb)
{
    afxVolume   vol;
    afxV4d      sup, inf;
};

AFXINL afxAabb*     AfxAabbReset(afxAabb *aabb); // make identity

AFXINL void         AfxAabbEncapsulatePoint(afxAabb *aabb, afxV4d const point);

AFXINL void         AfxAabbEncapsulateSphere(afxAabb *aabb, afxSphere const *sph);

AFXINL afxAabb*     AfxAabbRecompute(afxAabb *aabb, afxV4d const points[], afxNat cnt);

AFXINL afxBool      AfxAabbDoesContain(afxAabb const *aabb, afxV4d const point);

AFXINL afxReal*     AfxAabbGetExtent(afxAabb const *aabb, afxV3d extent);

AFXINL afxReal*     AfxAabbGetOrigin(afxAabb const *aabb, afxV4d origin);

AFXINL afxReal*     AfxAabbExtractCorner(afxAabb const *aabb, afxNat index, afxV4d corner);

AFXINL afxAabb*     AfxAabbCopy(afxAabb *aabb, afxAabb const *src);

AFXINL afxAabb*     AfxAabbEncapsulateAabb(afxAabb *aabb, afxAabb const *other);

AFXINL afxAabb*     AfxAabbTransform(afxAabb const *aabb, afxM4d const m, afxAabb *to);

#endif//AFX_AABB_H