/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/coll/afxCapsule.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/coll/afxBox.h"



// Compute Total Height
_AFXINL afxReal AfxGetCapsuleHeight(afxCapsule const cap)
{
    return cap.length + 2.0f * cap.radius;
}

// Compute Endpoints of the Capsule
// Get the Y-aligned end points of the capsule’s internal segment (cylinder)
_AFXINL afxReal AfxGetCapsuleEndpoints(afxCapsule const cap, afxV3d outA, afxV3d outB)
{
    afxError err;
    //AFX_ASSERT(cap);
    AFX_ASSERT(outA);
    AFX_ASSERT(outB);

    AfxV3dCopy(outA, cap.origin);
    AfxV3dCopy(outB, cap.origin);
    afxReal seg = cap.length / 2.0f;
    // Endpoint A = origin - offset (bottom hemisphere center)
    // Endpoint B = origin + offset (top hemisphere center)
    outA[1] -= seg;
    outB[1] += seg;
    return seg;
}

// Compute Axis-Aligned Bounding Box (AABB)
_AFXINL void AfxGetCapsuleAabb(afxCapsule const cap, afxAabb* aabb)
{
    afxError err;
    //AFX_ASSERT(cap);
    AFX_ASSERT(aabb);

    afxReal halfHeight = (cap.length / 2.0f) + cap.radius;

    aabb->min[0] = cap.origin[0] - cap.radius;
    aabb->min[1] = cap.origin[1] - halfHeight;
    aabb->min[2] = cap.origin[2] - cap.radius;

    aabb->max[0] = cap.origin[0] + cap.radius;
    aabb->max[1] = cap.origin[1] + halfHeight;
    aabb->max[2] = cap.origin[2] + cap.radius;
}

// Distance from Point to Capsule
_AFXINL afxReal AfxDistanceFromCapsule(afxCapsule const cap, afxV3d const point)
{
    afxError err;
    AFX_ASSERT(point);

    afxV3d a, b;
    AfxGetCapsuleEndpoints(cap, a, b);

    // Compute vector projection of point onto line segment
    afxV3d ab = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
    afxV3d ap = { point[0] - a[0], point[1] - a[1], point[2] - a[2] };

    afxReal ab_len2 = ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2];
    afxReal t = (ap[0] * ab[0] + ap[1] * ab[1] + ap[2] * ab[2]) / ab_len2;

    // Clamp t to [0,1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    afxV3d closest =
    {
        a[0] + ab[0] * t,
        a[1] + ab[1] * t,
        a[2] + ab[2] * t
    };

    afxReal dx = point[0] - closest[0];
    afxReal dy = point[1] - closest[1];
    afxReal dz = point[2] - closest[2];
    afxReal dist2 = dx * dx + dy * dy + dz * dz;
    afxReal dist = AfxSqrtf(dist2);
    return dist - cap.radius; // Subtract radius for surface distance
}

// Check if a Point is Inside the Capsule
_AFXINL afxBool AfxIsPointInsideCapsule(afxCapsule const cap, afxV3d const point)
{
    afxError err;
    //AFX_ASSERT(cap);
    //AFX_ASSERT(point);

    afxV3d a, b;
    AfxGetCapsuleEndpoints(cap, a, b);

    // Vector from A to B
    afxV3d ab = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
    afxV3d ap = { point[0] - a[0], point[1] - a[1], point[2] - a[2] };

    afxReal ab_len2 = ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2];
    afxReal t = (ap[0] * ab[0] + ap[1] * ab[1] + ap[2] * ab[2]) / ab_len2;

    // Clamp t to [0,1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // Closest point on the segment
    afxV3d closest =
    {
        a[0] + ab[0] * t,
        a[1] + ab[1] * t,
        a[2] + ab[2] * t
    };

    // Distance squared from point to closest point
    afxReal dx = point[0] - closest[0];
    afxReal dy = point[1] - closest[1];
    afxReal dz = point[2] - closest[2];
    afxReal dist2 = dx * dx + dy * dy + dz * dz;

    return dist2 <= (cap.radius * cap.radius);
}

