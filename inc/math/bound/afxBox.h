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

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/bound/afxSphere.h"

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxBox)
// A set of six 32-bit floats consisting of the minimum x, y, and z values followed by the maximum x, y, and z values.
{
    // A vector that stores the minimum coordinates (the lower corner of a bounding box in 3D or 4D space).
    afxV4d  min;
    // A vector storing the maximum coordinates (the upper corner of a bounding box).
    afxV4d  max; // [ sup/max, inf/min ]
};

typedef afxBox afxAabb; // [ sup/max, inf/min ]
typedef afxBox afxObb;

AFX afxBox const    AFX_AABB_IDENTITY;

/// Automaticallly constructs an AABB from a list of vertex positions only.
AFXINL void         AfxMakeAabbFromVertices(afxBox* bb, afxUnit cnt, afxV3d const points[]);

/// Returns the centre point of this AABB and the distance to each side of the box.
AFXINL void         AfxGetAabbCentre(afxBox const* bb, afxV4d centre);
AFXINL void         AfxGetAabbExtents(afxBox const* bb, afxV3d extent);

AFXINL void         AfxGetAabbCorner(afxBox const* bb, afxUnit index, afxV4d corner);

AFXINL void         AfxEmboxAabbs(afxBox* bb, afxUnit cnt, afxBox const other[]);
AFXINL void         AfxEmboxPoints(afxBox* bb, afxUnit cnt, afxV4d const v[]);
AFXINL void         AfxEmboxSpheres(afxBox* bb, afxUnit cnt, afxSphere const sph[]);
AFXINL void         AfxEmboxVectors(afxBox* bb, afxUnit cnt, afxV3d const v[]);

// TODO: contains or intersects?

AFXINL afxUnit      AfxDoesAabbInclude(afxBox const* bb, afxUnit cnt, afxBox const other[]);
AFXINL afxUnit      AfxDoesAabbIncludeAtv3d(afxBox const* bb, afxUnit cnt, afxV3d const v[]);
AFXINL afxUnit      AfxDoesAabbIncludeSpheres(afxBox const* bb, afxUnit cnt, afxSphere const sph[]);

//AFXINL afxResult    AfxAabbTestPlanes(afxBox const* aabb, afxUnit cnt, afxPlane const planes[]);

/// Given an AABB in local space, transform to world space
AFXINL void         AfxTransformAabb(afxBox const* bb, afxM4d const m, afxBox* to);
AFXINL void         AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxBox const in[], afxBox out[]);

AFXINL void         AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxBox out[]);
AFXINL void         AfxResetBoxes(afxUnit cnt, afxBox boxes[]);

#endif//AFX_BOX_H
