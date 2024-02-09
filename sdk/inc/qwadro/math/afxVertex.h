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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_VERTEX_H
#define AFX_VERTEX_H

#include "qwadro/math/afxVector.h"

AFXINL afxReal  AfxGetAngleBetweenV3d(afxReal const v[3], afxReal const other[3]);

AFXINL afxReal  AfxGetDistanceBetweenV3d(afxReal const v[3], afxReal const other[3]);

AFXINL void     AfxExtractNormalV3dComponents(afxReal const v[3], afxReal const normal[3], afxReal parallel[3], afxReal perpendicular[3]);

/// Hermite interpolation
/// v = (2 * t^3 - 3 * t^2 + 1) * posA + (t^3 - 2 * t^2 + t) * tanA + (-2 * t^3 + 3 * t^2) * posB + (t^3 - t^2) * tanB

AFXINL void     AfxHermiteV2d(afxReal v[2], afxReal const posA[2], afxReal const tanA[2], afxReal const posB[2], afxReal const tanB[2], afxReal t);
AFXINL void     AfxHermiteV3d(afxReal v[3], afxReal const posA[3], afxReal const tanA[3], afxReal const posB[3], afxReal const tanB[3], afxReal t);
AFXINL void     AfxHermiteV4d(afxReal v[4], afxReal const posA[4], afxReal const tanA[4], afxReal const posB[4], afxReal const tanB[4], afxReal t);

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
///
///     [ aX, aY, aZ, aW ] [  0  2  0  0 ] [  1  ]
///     [ bX, bY, bZ, bW ] [ -1  0  1  0 ] [  t  ]
///     [ cX, cY, cZ, cW ] [  2 -5  4 -1 ] [ t^2 ]
///     [ dX, dY, dZ, dW ] [ -1  3 -3  1 ] [ t^3 ] * 0.5 => v

/// v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

AFXINL void     AfxCatmullV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal const d[2], afxReal t);
AFXINL void     AfxCatmullV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal const d[3], afxReal t);
AFXINL void     AfxCatmullV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal const d[4], afxReal t);

AFXINL void     AfxReflectV2d(afxReal v[2], afxReal const incident[2], afxReal const normal[2]);
AFXINL void     AfxReflectV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3]);
AFXINL void     AfxReflectV4d(afxReal v[4], afxReal const incident[3], afxReal const normal[3]);

AFXINL void     AfxRefractV2d(afxReal v[2], afxReal const incident[2], afxReal const normal[2], afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3], afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxReal v[3], afxReal const incident[3], afxReal const normal[3], afxReal refracIdx);

#endif//AFX_VERTEX_H
