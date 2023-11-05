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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_AABB_H
#define AFX_AABB_H

#include "afx/math/afxMathDefs.h"
#include "afxSphere.h"

/// An axis-aligned bounding box, or AABB for short, is a box aligned with coordinate axes and fully enclosing some object.
/// Because the box is never rotated with respect to the axes, it canv be defined by supremum and infemum points.

#define AFX_AABB_SUP 0
#define AFX_AABB_INF 1

AFX_DEFINE_STRUCT(afxAabb)
{
    afxV4d  extremes[2]; // [ sup/max, inf/min ]
};

AFXINL void         AfxResetAabb(afxAabb* aabb); // make identity

/// Automaticallly constructs an AABB from a list of vertex positions only.
AFXINL void         AfxRecomputeAabb(afxAabb* aabb, afxNat cnt, afxReal const points[][3]);

AFXINL void         AfxCopyAabb(afxAabb* aabb, afxAabb const* in);

/// Returns the centre point of this AABB and the distance to each side of the box.
AFXINL void         AfxGetAabbExtents(afxAabb const* aabb, afxReal extent[3]);
AFXINL void         AfxGetAabbCentre(afxAabb const* aabb, afxReal centre[4]);

AFXINL void         AfxExtractAabbCorner(afxAabb const* aabb, afxNat index, afxV4d corner);

AFXINL void         AfxEncapsulateAabbs(afxAabb* aabb, afxNat cnt, afxAabb const other[]);
AFXINL void         AfxEncapsulatePoints(afxAabb* aabb, afxNat cnt, afxReal const point[][4]);
AFXINL void         AfxEncapsulateSpheres(afxAabb* aabb, afxNat cnt, afxSphere const sph[]);
AFXINL void         AfxEncapsulateVertices(afxAabb* aabb, afxNat cnt, afxReal const point[][3]);

AFXINL afxBool      AfxAabbContainsAabbs(afxAabb const* aabb, afxNat cnt, afxAabb const other[]);
AFXINL afxMask   AfxAabbContainsPoints(afxAabb const* aabb, afxNat cnt, afxReal const point[32][3]);
AFXINL afxBool      AfxAabbContainsSpheres(afxAabb const* aabb, afxNat cnt, afxSphere const sph[]);

/// Given an AABB in local space, transform to world space
AFXINL void         AfxTransformAabb(afxAabb const* aabb, afxM4d const m, afxAabb* to);

AFXINL void         AfxTransformAabbs(afxReal const affine[3], afxReal const linear[3][3], afxNat cnt, afxAabb const in[], afxAabb out[]);

#endif//AFX_AABB_H