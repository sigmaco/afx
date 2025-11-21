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

#include "qwadro/coll/afxSphere.h"
#include "qwadro/coll/afxBox.h"

_AFXINL afxSphere* AfxMakeSphere(afxSphere* sph, afxV3d const centre, afxReal radius)
{
    afxError err = { 0 };
    AFX_ASSERT(sph);
    AFX_ASSERT(centre);
    AfxV4dSet(sph->xyzr, centre[0], centre[1], centre[2], radius);
    return sph;
}

_AFXINL void AfxGetSphereOrigin(afxSphere const sph, afxV4d centre)
{
    afxError err = { 0 };
    AFX_ASSERT(centre);
    AfxV4dSet(centre, sph.xyzr[0], sph.xyzr[1], sph.xyzr[2], 1);
}

_AFXINL afxBox AfxGetSphereAabb(afxSphere const sph)
{
    afxError err = { 0 };
    // Calculate the AABB for this sphere
    afxReal r = sph.xyzr[AFX_SPHERE_RADIUS];
    return AFX_AABB(sph.xyzr[0] - r, sph.xyzr[1] - r, sph.xyzr[2] - r,
                    sph.xyzr[0] + r, sph.xyzr[1] + r, sph.xyzr[2] + r);
}

/*
    The AfxDoesSphereIntersects() function tests whether a single sphere intersects with an array of spheres.
    It implement a function that checks for each sphere in the array if it intersects with the target sphere.
    The intersection test between two spheres is straightforward: two spheres intersect if the distance between
    their centers is less than or equal to the sum of their radii.
*/

_AFXINL afxUnit AfxDoesSphereIntersects(afxSphere* sph, afxUnit cnt, afxSphere const others[])
{
    afxError err = { 0 };
    AFX_ASSERT(sph);
    AFX_ASSERT(!cnt || others);

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxSphere const* b = &others[i];

        afxReal distSquared = AfxV3dDist(sph->xyzr, b->xyzr);
        afxReal radiusSum = sph->xyzr[AFX_SPHERE_RADIUS] + b->xyzr[AFX_SPHERE_RADIUS];

        // Compare the squared distance with the squared sum of the radii to avoid square root
        if (distSquared <= radiusSum * radiusSum)
        {
            // Return i if any sphere intersects with the target sphere
            return i;
        }
    }
    return AFX_INVALID_INDEX;  // Return 0 if no sphere intersects with the target sphere
}

/*
    The AfxDoesSphereIntersectsAabbs() function tests whether a sphere intersects with an array of axis-aligned boxes.
    It need to perform collision detection between the box and the sphere. This involves checking if the sphere's
    center is inside the box or if the distance from the sphere's center to the closest point on the box is less than
    or equal to the radius of the sphere.

    Returns the index for the first collided box in array, else AFX_INVALID_INDEX for none of them.
*/

_AFXINL afxUnit AfxDoesSphereIntersectsAabbs(afxSphere* sph, afxUnit cnt, afxBox const boxes[])
{
    afxError err = { 0 };
    AFX_ASSERT(sph);
    AFX_ASSERT(!cnt || boxes);

    afxReal r2 = sph->xyzr[AFX_SPHERE_RADIUS] * sph->xyzr[AFX_SPHERE_RADIUS];

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxBox const* box = &boxes[i];

        // Find the closest point on the box to the sphere's center
        afxV3d closest;
        AfxV3dMin(closest, sph->xyzr, box->max);
        AfxV3dMax(closest, box->min, closest);
        // Calculate the squared distance from the sphere's center to the closest point
        afxReal distSquared = AfxV3dDist(sph->xyzr, closest);

        // Check if the distance is less than or equal to the squared radius
        if (distSquared <= r2)
        {
            return i;
        }
    }
    // Return 0 if no box intersects with the sphere
    return AFX_INVALID_INDEX;
}

// Tests whether a given point is inside, on, or outside a sphere. 
// The function compares the squared distance between the point and the center of the sphere with the square of the radius.
// Returns: -1 = outside, 0 = on surface, 1 = inside
_AFXINL afxInt AfxTestSphereEnglobingPoint(afxSphere const* sph, afxV3d const point)
{
    afxError err = { 0 };
    AFX_ASSERT(sph);
    AFX_ASSERT(point);

    afxV3d s;
    AfxV3dSub(s, point, sph->xyzr);
    afxReal distSq = AfxV3dDot(s, s);
    afxReal radiusSq = sph->xyzr[AFX_SPHERE_RADIUS] * sph->xyzr[AFX_SPHERE_RADIUS];

    if (distSq < radiusSq)
        return 1; // Inside the sphere
    else if (fabs(distSq - radiusSq) < 1e-9)
        return 0; // On the sphere surface (within a tiny margin)
    else
        return -1; // Outside the sphere
}

// Tests whether a given point is inside, on, or outside a sphere. 
// The function compares the squared distance between the point and the center of the sphere with the square of the radius.
// Returns: -1 = outside, 0 = on surface, 1 = inside
_AFXINL afxInt AfxTestSphereEnglobingPoints(afxSphere const* sph, afxBool all, afxBool fully, afxUnit cnt, afxV3d const points[])
{
    afxError err = { 0 };
    AFX_ASSERT(sph);
    AFX_ASSERT(points);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d s;
        AfxV3dSub(s, points[i], sph->xyzr);
        afxReal distSq = AfxV3dDot(s, s);
        afxReal radiusSq = sph->xyzr[AFX_SPHERE_RADIUS] * sph->xyzr[AFX_SPHERE_RADIUS];

        if (distSq < radiusSq)
        {
            //return 1; // Inside the sphere
        }
        else if (AFX_ABS(distSq - radiusSq) < 1e-9)
        {
            if (fully && all)
                return 0; // On the sphere surface (within a tiny margin)
        }
        else
        {
            if (all)
                return -1; // Outside the sphere
        }
    }
    return 1;
}
