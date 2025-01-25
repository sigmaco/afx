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

#ifndef AFX_PLANE_H
#define AFX_PLANE_H

#include "qwadro/inc/math/afxVector.h"
#include "qwadro/inc/math/bound/afxBox.h"

#define AFX_PLANE_OFFSET 3
#define AFX_PLANE_DIST 3

typedef enum afxFloorPlane
// The orientation of the the floor plane with respect to the X,Y,Z axes, and which axes represent the side, front and up vectors as a basis for rotations.
{
    // The floor is oriented along the ZX-plane. The default value.
    // The front vector points towards +Z, the up vector towards +Y, and the side vector towards +X.
    afxFloorPlane_XZ, // the default

    // The floor is oriented along the XY-plane. 
    // The front vector points towards +X, the up vector towards +Z, and the side vector towards +Y.
    afxFloorPlane_XY,

    // The floor is oriented along the YZ-plane. 
    // The front vector points towards +Y, the up vector towards +X, and the side vector towards +Z.
    afxFloorPlane_YZ
} afxFloorPlane;

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxPlane)
{
    afxV3d      normal;
    union
    {
        afxReal dist; // aka dist/offset
        afxReal offset;
    };
};

AFXINL void         AfxPlaneCopy(afxPlane* p, afxPlane const* in);

AFXINL void         AfxPlaneReset(afxPlane* p, afxV3d const normal, afxReal dist);

AFXINL void         AfxPlaneFromTriangle(afxPlane* p, afxV3d const a, afxV3d const b, afxV3d const c);

AFXINL void         AfxPlaneGetNormal(afxPlane* p, afxV3d normal);

AFXINL afxReal      AfxPlaneGetOffset(afxPlane const* p); // aka GetPlaneDistance
#define AfxPlaneGetDistance AfxPlaneGetOffset

AFXINL afxReal      AfxPlaneFindV3d(afxPlane const* p, afxV3d const point);

AFXINL afxReal      AfxPlaneFindHitInterpolationConstant(afxPlane const* p, afxV3d const a, afxV3d const b);

// Test this plane with an AABB
// <0 if the box is completly on the back side of the plane
// >0 if the box is completly on the front side of the plane
// 0 if the box intersects with the plane
AFXINL afxResult    AfxPlaneTestAabbs(afxPlane const* p, afxUnit cnt, afxBox const aabb[]);

// Test this plane with a collision sphere.
AFXINL afxResult    AfxPlaneTestSpheres(afxPlane const* p, afxUnit cnt, afxSphere const spheres[]);

#endif//AFX_PLANE_H
