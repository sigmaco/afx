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

/*
    The afxCapsule structure defines a capsule shape, which is a 3D geometric primitive commonly used in 
    physics engines and 3D modeling (like for collision detection or bounding volumes).

    The capsule is assumed to be oriented vertically (i.e., along the Y-axis) by default. 
    This means the cylindrical body stretches along Y, and the hemispherical ends are at the top and bottom.

    The center point of the entire capsule --- typically located at the midpoint of the cylindrical part. 
    The full capsule extends along Y from: origin.y - (length/2 + radius)  to  origin.y + (length/2 + radius)

    Radius of the hemispherical ends and the cylinder's circular cross-section.

    This is only the length of the cylindrical middle part, not counting the hemispherical caps.
*/

#ifndef AFX_CAPSULE_H
#define AFX_CAPSULE_H

#include "qwadro/inc/math/afxVolume.h"

AFX_DEFINE_STRUCT(afxCapsule)
// A capsule is a 3D geometric primitive commonly used in physics engines and 3D modeling (like for collision detection or bounding volumes).
// The capsule is assumed to be oriented vertically (along the Y-axis) by default. 
// This means the cylindrical body stretches along Y, and the hemispherical ends are at the top and bottom.
// The center point of the entire capsule --- typically located at the midpoint of the cylindrical part.
// The full capsule extends along Y from: origin.y - (length/2 + radius)  to  origin.y + (length/2 + radius)
{
    // Center of capsule (often midpoint between ends).
    // The center point of the entire capsule --- typically located at the midpoint of the cylindrical part.
    afxV3d origin;
    // Radius of the capsule's hemispherical ends (and the cylinder).
    // Radius of the hemispherical ends and the cylinder's circular cross-section.
    afxReal radius;
    // Length of the cylindrical part of the capsule (excluding hemispheres).
    // This is only the length of the cylindrical middle part, not counting the hemispherical caps.
    afxReal length;
};

#define AFX_CAPSULE(radius_, length_) (afxCapsule){ .radius = (radius_), .length = (length_) }


// Compute Total Height
AFXINL afxReal AfxGetCapsuleHeight(afxCapsule const cap);

// Compute Endpoints of the Capsule
// Get the Y-aligned end points of the capsule’s internal segment (cylinder)
AFXINL afxReal AfxGetCapsuleEndpoints(afxCapsule const cap, afxV3d outA, afxV3d outB);

// Compute Axis-Aligned Bounding Box (AABB)
AFXINL void AfxGetCapsuleAabb(afxCapsule const cap, afxAabb* aabb);

// Distance from Point to Capsule
AFXINL afxReal AfxDistanceFromCapsule(afxCapsule const cap, afxV3d const point);

// Check if a Point is Inside the Capsule
AFXINL afxBool AfxIsPointInsideCapsule(afxCapsule const cap, afxV3d const point);



#endif//AFX_CAPSULE_H
