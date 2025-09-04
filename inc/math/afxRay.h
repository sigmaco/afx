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

#ifndef AFX_RAY_H
#define AFX_RAY_H

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/afxSphere.h"
#include "qwadro/inc/math/afxCapsule.h"

#pragma pack(push, 1)
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxRay)
{
    // The position of the ray origin.
    afxV3d  origin;
    // The start of ray segment.
    afxReal near; // 0.0
    // The normalized direction of the ray
    afxV3d  normal; // (0, 0, 1)
    // The end of ray segment (may be clamped to hit distance).
    afxReal far; // AFX_R32_MAX
    // The time for the ray (used for motion blur).
    afxReal t; // 0.0

    // User-defined flags.
    afxFlags flags;
    // User-defined mask.
    afxMask mask;
    // User-defined identifier.
    afxUnit id;
};
#pragma pack(pop)

// afxRay.near is expected to be physically packed with afxRay.origin as an afxV4d.
AFX_STATIC_ASSERT(offsetof(afxRay, near) == 12, "");
// afxRay.far is expected to be physically packed with afxRay.normal as an afxV4d.
AFX_STATIC_ASSERT(offsetof(afxRay, far) == 28, "");

#define AFX_RAY() (afxRay){ \
    .origin[0] = from[0], .origin[1] = from[1], .origin[2] = from[2], .near = tnear, \
    .normal[0] = to[0], .normal[1] = to[1], .normal[2] = to[2], .far = tfar, \
    .t = time, .id = id, .mask = mask, .flags = flags };

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxRayHit)
{
    // The normalized direction of the geometry.
    afxV3d normal;
    // The barycentric coordinate of the hit.
    afxV2d uv;
    
    // Resulting idenfiers for geometry, instance and primitive hit.
    afxUnit geomId;
    afxUnit instId;
    afxUnit primId;
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxLine)
{
    afxV3d  from;
    afxV3d  to;
};

// Ray vs. Capsule Intersection
// Detects if a ray intersects the capsule, and optionally returns the distance to the intersection.
// Returns true if hit; if 'outT' is non-null, stores distance to hit
AFXINL afxBool  AfxRaycastCapsule(afxRay const* ray, afxCapsule const cap, afxReal* outT);

#endif//AFX_RAY_H
