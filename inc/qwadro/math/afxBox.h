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

/// An axis-aligned bounding box, or AABB for short, is a box aligned with coordinate axes and fully enclosing some object.
/// Because the box is never rotated with respect to the axes, it can be defined by supremum and infemum points.
/// The AABB data in memory is six 32-bit floats consisting of the minimum x, y, and z values followed by the maximum x, y, and z values.

#ifndef AFX_BOX_H
#define AFX_BOX_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxSphere.h"

#define AFX_AABB_SUP 0
#define AFX_AABB_INF 1

typedef afxV4d afxBox[2];
typedef afxBox afxAabb; // [ sup/max, inf/min ]
typedef afxBox afxObb;

/*
AFX_DEFINE_STRUCT(afxBox)
{
    afxV4d  extremes[2]; // [ sup/max, inf/min ]
};
*/

AFXINL void         AfxResetBox(afxBox bb); // make identity

AFXINL void         AfxCopyBox(afxBox bb, afxBox const in);

/// Automaticallly constructs an AABB from a list of vertex positions only.
AFXINL void         AfxRecomputeAabb(afxBox bb, afxNat cnt, afxV3d const points[]);

/// Returns the centre point of this AABB and the distance to each side of the box.
AFXINL void         AfxGetAabbCentre(afxBox const bb, afxV4d centre);
AFXINL void         AfxGetAabbExtents(afxBox const bb, afxV3d extent);

AFXINL void         AfxExtractAabbCorner(afxBox const bb, afxNat index, afxV4d corner);

AFXINL void         AfxEncapsulateAabbs(afxBox bb, afxNat cnt, afxBox const other[]);
AFXINL void         AfxEncapsulatePoints(afxBox bb, afxNat cnt, afxV4d const v[]);
AFXINL void         AfxEncapsulateSpheres(afxBox bb, afxNat cnt, afxSphere const sph[]);
AFXINL void         AfxEncapsulateVertices(afxBox bb, afxNat cnt, afxV3d const v[]);

AFXINL afxBool      AfxAabbContainsAabbs(afxBox const bb, afxNat cnt, afxBox const other[]);
AFXINL afxNat       AfxAabbContainsPoints(afxBox const bb, afxNat cnt, afxV3d const v[]);
AFXINL afxBool      AfxAabbContainsSpheres(afxBox const bb, afxNat cnt, afxSphere const sph[]);

/// Given an AABB in local space, transform to world space
AFXINL void         AfxTransformAabb(afxBox const bb, afxM4d const m, afxBox to);
AFXINL void         AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxNat cnt, afxBox const in[], afxBox out[]);

AFXINL void         AfxCopyBoxes(afxNat cnt, afxBox in[], afxBox out[]);
AFXINL void         AfxResetBoxes(afxNat cnt, afxBox boxes[]);

#endif//AFX_BOX_H
