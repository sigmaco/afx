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

#include "qwadro/inc/math/bound/afxSphere.h"
#include "qwadro/inc/math/bound/afxBox.h"

_AFXINL afxSphere* AfxMakeSphere(afxSphere* sph, afxV3d const centre, afxReal radius)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(sph);
    AFX_ASSERT(centre);
    AfxV4dSet(sph->xyzr, centre[0], centre[1], centre[2], radius);
    return sph;
}

_AFXINL void AfxGetSphereOrigin(afxSphere const sph, afxV4d centre)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(centre);
    AfxV4dSet(centre, sph.xyzr[0], sph.xyzr[1], sph.xyzr[2], 1);
}

_AFXINL afxBox AfxGetSphereAabb(afxSphere const sph)
{
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
