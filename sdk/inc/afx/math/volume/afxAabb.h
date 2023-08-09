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

#ifndef AFX_AABB_H
#define AFX_AABB_H

#include "afx/math/afxMathDefs.h"
#include "afxSphere.h"
#include "afxVolume.h"

/// An axis-aligned bounding box, or AABB for short, is a box aligned with coordinate axes and fully enclosing some object.
/// Because the box is never rotated with respect to the axes, it canv be defined by supremum and infemum points.

AFX_DEFINE_STRUCT(afxAabb)
{
    afxVolume   vol;
    afxV4d      sup, inf;
};

AFXINL void         AfxAabbMakeIdentity(afxAabb *aabb); // make identity

AFXINL void         AfxAabbEncapsulatePoint(afxAabb *aabb, afxV4d const point);

AFXINL void         AfxAabbEncapsulateSphere(afxAabb *aabb, afxSphere const *sph);

AFXINL void         AfxAabbRecompute(afxAabb *aabb, afxV4d const points[], afxNat cnt);

AFXINL afxBool      AfxAabbDoesContain(afxAabb const *aabb, afxV4d const point);

AFXINL void         AfxAabbGetExtent(afxAabb const *aabb, afxV4d extent);

AFXINL void         AfxAabbGetOrigin(afxAabb const *aabb, afxV4d origin);

AFXINL void         AfxAabbExtractCorner(afxAabb const *aabb, afxNat index, afxV4d corner);

AFXINL void         AfxAabbCopy(afxAabb *aabb, afxAabb const *src);

AFXINL void         AfxAabbEncapsulateAabb(afxAabb *aabb, afxAabb const *other);

AFXINL void         AfxAabbTransform(afxAabb const *aabb, afxM4d const m, afxAabb *to);

#endif//AFX_AABB_H