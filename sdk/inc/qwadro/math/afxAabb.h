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

#ifndef AFX_AABB_H
#define AFX_AABB_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxSphere.h"

/// An axis-aligned bounding box, or AABB for short, is a box aligned with coordinate axes and fully enclosing some object.
/// Because the box is never rotated with respect to the axes, it canv be defined by supremum and infemum points.

#define AFX_AABB_SUP 0
#define AFX_AABB_INF 1

typedef afxV4d afxAabb[2]; // [ sup/max, inf/min ]

/*
AFX_DEFINE_STRUCT(afxAabb)
{
    afxV4d  extremes[2]; // [ sup/max, inf/min ]
};
*/

AFXINL void         AfxResetAabb(afxAabb aabb); // make identity

/// Automaticallly constructs an AABB from a list of vertex positions only.
AFXINL void         AfxRecomputeAabb(afxAabb aabb, afxNat cnt, afxV3d const points[]);

AFXINL void         AfxCopyAabb(afxAabb aabb, afxAabb const in);

/// Returns the centre point of this AABB and the distance to each side of the box.
AFXINL void         AfxGetAabbExtents(afxAabb const aabb, afxV3d extent);
AFXINL void         AfxGetAabbCentre(afxAabb const aabb, afxV4d centre);

AFXINL void         AfxExtractAabbCorner(afxAabb const aabb, afxNat index, afxV4d corner);

AFXINL void         AfxEncapsulateAabbs(afxAabb aabb, afxNat cnt, afxAabb const other[]);
AFXINL void         AfxEncapsulatePoints(afxAabb aabb, afxNat cnt, afxV4d const v[]);
AFXINL void         AfxEncapsulateSpheres(afxAabb aabb, afxNat cnt, afxSphere const sph[]);
AFXINL void         AfxEncapsulateVertices(afxAabb aabb, afxNat cnt, afxV3d const v[]);

AFXINL afxBool      AfxAabbContainsAabbs(afxAabb const aabb, afxNat cnt, afxAabb const other[]);
AFXINL afxMask      AfxAabbContainsPoints(afxAabb const aabb, afxNat cnt, afxV3d const v[32]);
AFXINL afxBool      AfxAabbContainsSpheres(afxAabb const aabb, afxNat cnt, afxSphere const sph[]);

/// Given an AABB in local space, transform to world space
AFXINL void         AfxTransformAabb(afxAabb const aabb, afxM4d const m, afxAabb to);

AFXINL void         AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxAabb const in[], afxAabb out[]);

#endif//AFX_AABB_H
