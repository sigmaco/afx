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

AFXINL void         AfxResetAabb(afxAabb *aabb); // make identity

AFXINL void         AfxRecomputeAabb(afxAabb *aabb, afxV4d const points[], afxNat cnt);

AFXINL void         AfxCopyAabb(afxAabb *aabb, afxAabb const *src);

AFXINL void         AfxGetAabbExtent(afxAabb const *aabb, afxV4d extent);
AFXINL void         AfxGetAabbOrigin(afxAabb const *aabb, afxV4d origin);

AFXINL void         AfxExtractAabbCorner(afxAabb const *aabb, afxNat index, afxV4d corner);
AFXINL afxBool      AfxAabbContainsPoint(afxAabb const *aabb, afxV4d const point);

AFXINL void         AfxEncapsulatePoint(afxAabb *aabb, afxV4d const point);
AFXINL void         AfxEncapsulateSphere(afxAabb *aabb, afxSphere const *sph);
AFXINL void         AfxEncapsulateAabb(afxAabb *aabb, afxAabb const *other);

AFXINL void         AfxTransformAabb(afxAabb const *aabb, afxM4d const m, afxAabb *to);

AFXINL void         AfxTransformBoundingBox(afxAabb* aabb, afxAabb const* in, afxReal const affine[3], afxReal linear[3][3]);

#endif//AFX_AABB_H