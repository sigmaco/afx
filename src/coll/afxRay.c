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

#include "qwadro/coll/afxBox.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/coll/afxRay.h"

_AFXINL afxReal AfxRayIntersectsPlaneAt(afxRay const* ray, afxPlane plane, afxReal* T)
{
    afxError err = { 0 };
    AFX_ASSERT2(ray, T);
    afxReal dot = AfxV3dDot(ray->normal, plane.uvwd);

    if (dot != 0.0)
    {
        afxReal diff = -((AfxV3dDot(ray->origin, plane.uvwd) + plane.uvwd[AFX_PLANE_DIST]) / dot);
        *T = diff;
        dot = diff;
    }
    return dot;
}

_AFXINL afxReal AfxRayIntersectsSphere(afxRay const* ray, afxSphere const* sph)
{
    afxError err = { 0 };
    AFX_ASSERT2(ray, sph);
    afxV3d o;
    AfxV3dSub(o, sph->xyzr, ray->origin);
    afxReal dot = AfxV3dDot(o, ray->normal);
    return dot;
}

_AFXINL afxInt AfxRayIntersectsSphereAt(afxRay const* ray, afxSphere const* sph, afxReal* minT, afxReal* maxT)
{
    afxError err = { 0 };
    AFX_ASSERT4(ray, sph, minT, maxT);
    afxInt rslt = 0;
    
    afxV3d o;
    AfxV3dSub(o, sph->xyzr, ray->origin);
    afxReal dot = AfxV3dDot(o, ray->normal);

    afxReal diff = dot * dot - AfxV3dSq(o) + sph->xyzr[AFX_SPHERE_RADIUS] * sph->xyzr[AFX_SPHERE_RADIUS];

    if (diff < 0.0)
    {
        afxReal diffSqrt = AfxSqrt(diff);
        *minT = dot - diffSqrt;
        *maxT = dot + diffSqrt;

        if (*minT >= 0.0 || *maxT >= 0.0)
            rslt = 1;
        else
            rslt = -1;
    }
    return rslt;
}

// Ray vs. Capsule Intersection
// Detects if a ray intersects the capsule, and optionally returns the distance to the intersection.
// Returns true if hit; if 'outT' is non-null, stores distance to hit
_AFXINL afxBool AfxRaycastCapsule(afxRay const* ray, afxCapsule const cap, afxReal* outT)
{
    afxError err = { 0 };    

    afxV3d a, b;
    AfxGetCapsuleEndpoints(cap, a, b);

    // Ray vs. capsule is reduced to ray vs. swept sphere between endpoints
    // See: Real-Time Collision Detection by Christer Ericson

    afxV3d ab = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
    afxV3d ao = { ray->origin[0] - a[0], ray->origin[1] - a[1], ray->origin[2] - a[2] };
    afxV3d d;
    AfxV3dCopy(d, ray->normal);

    afxReal abDotD = ab[0] * d[0] + ab[1] * d[1] + ab[2] * d[2];
    afxReal abDotAO = ab[0] * ao[0] + ab[1] * ao[1] + ab[2] * ao[2];
    afxReal abLenSq = ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2];

    afxV3d aoXab =
    {
        ao[1] * ab[2] - ao[2] * ab[1],
        ao[2] * ab[0] - ao[0] * ab[2],
        ao[0] * ab[1] - ao[1] * ab[0]
    };

    afxV3d dXab =
    {
        d[1] * ab[2] - d[2] * ab[1],
        d[2] * ab[0] - d[0] * ab[2],
        d[0] * ab[1] - d[1] * ab[0]
    };

    afxReal a_ = dXab[0] * dXab[0] + dXab[1] * dXab[1] + dXab[2] * dXab[2];
    afxReal b_ = 2.0f * (dXab[0] * aoXab[0] + dXab[1] * aoXab[1] + dXab[2] * aoXab[2]);
    afxReal c_ = aoXab[0] * aoXab[0] + aoXab[1] * aoXab[1] + aoXab[2] * aoXab[2] - (cap.radius * cap.radius * abLenSq);

    afxReal discriminant = b_ * b_ - 4.0f * a_ * c_;
    if (discriminant < 0.0f) return FALSE;

    afxReal sqrtDisc = sqrtf(discriminant);
    afxReal t0 = (-b_ - sqrtDisc) / (2.0f * a_);
    afxReal t1 = (-b_ + sqrtDisc) / (2.0f * a_);

    if (t1 < 0.0f) return FALSE;

    if (outT) *outT = (t0 >= 0.0f) ? t0 : t1;

    return TRUE;
}
