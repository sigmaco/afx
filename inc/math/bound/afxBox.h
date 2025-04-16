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

/**
    An axis-aligned bounding box, or AABB for short, is a box aligned with coordinate axes and fully enclosing some object.
    Because the box is never rotated with respect to the axes, it can be defined by supremum and infemum points.
    The AABB data in memory is six 32-bit floats consisting of the minimum x, y, and z values followed by the maximum x, y, and z values.
*/

#ifndef AFX_BOX_H
#define AFX_BOX_H

#include "qwadro/inc/math/bound/afxVolume.h"

#define AFX_NUM_BOX_CORNERS (8)
#define AFX_NUM_BOX_EDGES (12)
#define AFX_NUM_BOX_EDGE_VERTICES (24)

#define AFX_AABB(minX_, minY_, minZ_, maxX_, maxY_, maxZ_) \
    (afxBox){ .min = { (minX_), (minY_), (minZ_) }, .max = { (maxX_), (maxY_), (maxZ_) } }

AFX afxBox const    AFX_AABB_IDENTITY;

/// Automaticallly constructs an AABB from a list of vertex positions only.
AFXINL void         AfxMakeAabb(afxBox* bb, afxUnit cnt, afxV3d const points[]);

/// Returns the centre point of this AABB and the distance to each side of the box.
AFXINL void         AfxGetAabbCentre(afxBox const* bb, afxV4d centre);

// Calculate the bounding box extent.
// Returns TRUE if AABB actually represents a volume.
AFXINL afxBool      AfxGetAabbExtents(afxBox const* bb, afxV3d extent);

// The vertices are defined as 8 unique corner points.
AFXINL void         AfxGetAabbCorners(afxBox const* bb, afxV3d vertices[AFX_NUM_BOX_CORNERS]);

AFXINL void         AfxEmboxAabbs(afxBox* bb, afxUnit cnt, afxBox const other[]);
AFXINL void         AfxEmboxPoints(afxBox* bb, afxUnit cnt, afxV4d const v[]);
AFXINL void         AfxEmboxSpheres(afxBox* bb, afxUnit cnt, afxSphere const sph[]);
AFXINL void         AfxEmboxVectors(afxBox* bb, afxUnit cnt, afxV3d const v[]);
AFXINL void         AfxEmboxTriangles(afxBox* bb, afxUnit triCnt, afxUnit const indices[], afxUnit idxStride, afxV3d const vertices[]);

// TODO: contains or intersects?

AFXINL afxUnit      AfxDoesAabbInclude(afxBox const* bb, afxUnit cnt, afxBox const other[]);
AFXINL afxUnit      AfxDoesAabbIncludeAtv3d(afxBox const* bb, afxUnit cnt, afxV3d const v[]);
AFXINL afxUnit      AfxDoesAabbIncludeSpheres(afxBox const* bb, afxUnit cnt, afxSphere const sph[]);

//AFXINL afxResult    AfxAabbTestPlanes(afxBox const* aabb, afxUnit cnt, afxPlane const planes[]);

/// Given an AABB in local space, transform to world space
AFXINL void         AfxTransformAabb(afxBox const* bb, afxM4d const m, afxBox* to);
AFXINL void         AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxBox const in[], afxBox out[]);

AFXINL void         AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxUnit inStride, afxBox out[], afxUnit outStride);
AFXINL void         AfxResetBoxes(afxUnit cnt, afxBox boxes[], afxUnit stride);


AFXINL void         AsxGenerateAabbFaces(afxUnit cnt, afxBox const aabbs[], afxV3d vertices[][8], afxUnit16 indices16[][24], afxUnit32 indices32[][24], afxUnit* outVtxCnt, afxUnit* outIdxCnt);
AFXINL void         AsxGenerateAabbEdges(afxUnit cnt, afxBox const aabbs[], afxV3d vertices[][8], afxUnit16 indices16[][24], afxUnit32 indices32[][24], afxUnit* outVtxCnt, afxUnit* outIdxCnt);

#endif//AFX_BOX_H
