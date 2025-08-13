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

#include "qwadro/inc/math/coll/afxBox.h"

#define AFX_PLANE_DIST (3)

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

#define AFX_PLANE(u_, v_, w_, d_) (afxPlane){ .uvwd = { (u_), (v_), (w_), (d_) } }

AFXINL afxBool      AfxPlaneIsEqual(afxPlane a, afxPlane b);
AFXINL afxBool      AfxPlaneIsNearEqual(afxPlane a, afxPlane b, afxV4d epsilon);
AFXINL afxBool      AfxPlaneIsNotEqual(afxPlane a, afxPlane b);
AFXINL afxBool      AfxPlaneIsNaN(afxPlane p);
AFXINL afxBool      AfxPlaneIsInfinite(afxPlane p);
AFXINL afxReal      AfxPlaneDot(afxPlane a, afxPlane b);
AFXINL afxReal      AfxPlaneDotCoord(afxPlane p, afxV3d v);
AFXINL afxReal      AfxPlaneDotNormal(afxPlane p, afxV3d v);
AFXINL afxPlane     AfxPlaneNormalizeEst(afxPlane p);
AFXINL afxPlane     AfxPlaneNormalize(afxPlane p);
AFXINL afxPlane     AfxPlaneIntersectLine(afxPlane p, afxV4d start, afxV4d end);
AFXINL void         AfxPlaneIntersectPlane(afxPlane a, afxPlane b, afxV4d start, afxV4d end);
AFXINL afxPlane     AfxPlaneTransform(afxPlane p, afxM4d const m);
AFXINL afxPlane     AfxPlaneFromPointNormal(afxV4d point, afxV4d normal);
AFXINL afxPlane     AfxPlaneFromPoints(afxV4d a, afxV4d b, afxV4d c);

AFXINL void         AfxMakePlane(afxPlane* p, afxV3d const normal, afxReal dist);

AFXINL void         AfxMakePlaneFromTriangle(afxPlane* p, afxV3d const a, afxV3d const b, afxV3d const c);

AFXINL void         AfxGetPlaneNormal(afxPlane* p, afxV3d normal);

AFXINL afxReal      AfxGetPlaneOffset(afxPlane const* p); // aka GetPlaneDistance
#define AfxGetPlaneDistance AfxGetPlaneOffset

AFXINL afxReal      AfxFindPlaneDistance(afxPlane const* p, afxV3d const point);

AFXINL afxReal      AfxFindPlaneHitInterpolationConstant(afxPlane const* p, afxV3d const a, afxV3d const b);

// Test this plane with an AABB
// <0 if the box is completly on the back side of the plane
// >0 if the box is completly on the front side of the plane
// 0 if the box intersects with the plane
AFXINL afxResult    AfxPlaneTestAabbs(afxPlane const* p, afxUnit cnt, afxBox const aabb[]);

// Test this plane with a collision sphere.
AFXINL afxResult    AfxPlaneTestSpheres(afxPlane const* p, afxUnit cnt, afxSphere const spheres[]);

////////////////////////////////////////

AFXINL void         AfxCopyPlanes(afxUnit cnt, afxPlane const src[], afxPlane dst[]);

#endif//AFX_PLANE_H
