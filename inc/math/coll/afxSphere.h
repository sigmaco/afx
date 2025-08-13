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

#ifndef AFX_SPHERE_H
#define AFX_SPHERE_H

#include "qwadro/inc/math/coll/afxVolume.h"

#define AFX_SPHERE_RADIUS (3)

#define AFX_SPHERE(x_, y_, z_, radius_) \
    (afxSphere){ .xyzr = { (x_), (y_), (z_), (radius_) }

AFXINL afxSphere*   AfxMakeSphere(afxSphere* sph, afxV3d const centre, afxReal radius);

AFXINL void         AfxGetSphereOrigin(afxSphere const sph, afxV4d centre);

AFXINL afxBox       AfxGetSphereAabb(afxSphere const sph);


/*
    The AfxDoesSphereIntersects() function tests whether a single sphere intersects with an array of spheres.
    It implement a function that checks for each sphere in the array if it intersects with the target sphere.
    The intersection test between two spheres is straightforward: two spheres intersect if the distance between
    their centers is less than or equal to the sum of their radii.
*/

AFXINL afxUnit AfxDoesSphereIntersects(afxSphere* sph, afxUnit cnt, afxSphere const others[]);

/*
    The AfxDoesSphereIntersectsAabbs() function tests whether a sphere intersects with an array of axis-aligned boxes.
    It need to perform collision detection between the box and the sphere. This involves checking if the sphere's
    center is inside the box or if the distance from the sphere's center to the closest point on the box is less than
    or equal to the radius of the sphere.

    Returns the index for the first collided box in array, else AFX_INVALID_INDEX for none of them.
*/

AFXINL afxUnit AfxDoesSphereIntersectsAabbs(afxSphere* sph, afxUnit cnt, afxBox const boxes[]);

// Tests whether a given point is inside, on, or outside a sphere. 
// The function compares the squared distance between the point and the center of the sphere with the square of the radius.
// Returns: -1 = outside, 0 = on surface, 1 = inside
AFXINL afxInt AfxTestSphereEnglobingPoint(afxSphere const* sph, afxV3d const point);

// Tests whether a given point is inside, on, or outside a sphere. 
// The function compares the squared distance between the point and the center of the sphere with the square of the radius.
// Returns: -1 = outside, 0 = on surface, 1 = inside
AFXINL afxInt AfxTestSphereEnglobingPoints(afxSphere const* sph, afxBool all, afxBool fully, afxUnit cnt, afxV3d const points[]);

#endif//AFX_SPHERE_H
