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
{
    afxV4d  sup; // [ sup/max, inf/min ]
    afxV4d  inf;
};

typedef afxBox afxAabb; // [ sup/max, inf/min ]
typedef afxBox afxObb;

AFXINL void         AfxBoxReset(afxBox* bb); // make identity

AFXINL void         AfxBoxCopy(afxBox* bb, afxBox const* in);

/// Automaticallly constructs an AABB from a list of vertex positions only.
AFXINL void         AfxAabbSet(afxBox* bb, afxUnit cnt, afxV3d const points[]);

/// Returns the centre point of this AABB and the distance to each side of the box.
AFXINL void         AfxAabbGetCentre(afxBox const* bb, afxV4d centre);
AFXINL void         AfxAabbGetExtents(afxBox const* bb, afxV3d extent);

AFXINL void         AfxAabbGetCorner(afxBox const* bb, afxUnit index, afxV4d corner);

AFXINL void         AfxAabbAbsorb(afxBox* bb, afxUnit cnt, afxBox const other[]);
AFXINL void         AfxAabbAbsorbAtv4d(afxBox* bb, afxUnit cnt, afxV4d const v[]);
AFXINL void         AfxAabbAbsorbSpheres(afxBox* bb, afxUnit cnt, afxSphere const sph[]);
AFXINL void         AfxAabbAbsorbAtv3d(afxBox* bb, afxUnit cnt, afxV3d const v[]);

// TODO: contains or intersects?

AFXINL afxUnit      AfxAabbContainsAabbs(afxBox const* bb, afxUnit cnt, afxBox const other[]);
AFXINL afxUnit      AfxAabbContainsAtv3d(afxBox const* bb, afxUnit cnt, afxV3d const v[]);
AFXINL afxUnit      AfxAabbContainsSpheres(afxBox const* bb, afxUnit cnt, afxSphere const sph[]);

//AFXINL afxResult    AfxAabbTestPlanes(afxBox const* aabb, afxUnit cnt, afxPlane const planes[]);

/// Given an AABB in local space, transform to world space
AFXINL void         AfxTransformAabb(afxBox const* bb, afxM4d const m, afxBox* to);
AFXINL void         AfxTransformObbs(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxBox const in[], afxBox out[]);

AFXINL void         AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxBox out[]);
AFXINL void         AfxResetBoxes(afxUnit cnt, afxBox boxes[]);

#endif//AFX_BOX_H
