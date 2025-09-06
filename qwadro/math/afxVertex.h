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

#ifndef AFX_VERTEX_H
#define AFX_VERTEX_H

#include "qwadro/math/afxVector.h"

AFXINL afxReal  AfxGetAngleBetweenV3d(afxV3d const v, afxV3d const other);

AFXINL afxReal  AfxGetDistanceBetweenV3d(afxV3d const v, afxV3d const other);

AFXINL void     AfxExtractNormalV3dComponents(afxV3d const v, afxV3d const normal, afxV3d parallel, afxV3d perpendicular);

/// Hermite interpolation
/// v = (2 * t^3 - 3 * t^2 + 1) * posA + (t^3 - 2 * t^2 + t) * tanA + (-2 * t^3 + 3 * t^2) * posB + (t^3 - t^2) * tanB

AFXINL void     AfxHermiteV2d(afxV2d v, afxV2d const posA, afxV2d const tanA, afxV2d const posB, afxV2d const tanB, afxReal t);
AFXINL void     AfxHermiteV3d(afxV3d v, afxV3d const posA, afxV3d const tanA, afxV3d const posB, afxV3d const tanB, afxReal t);
AFXINL void     AfxHermiteV4d(afxV4d v, afxV4d const posA, afxV4d const tanA, afxV4d const posB, afxV4d const tanB, afxReal t);

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
///
///     [ aX, aY, aZ, aW ] [  0  2  0  0 ] [  1  ]
///     [ bX, bY, bZ, bW ] [ -1  0  1  0 ] [  t  ]
///     [ cX, cY, cZ, cW ] [  2 -5  4 -1 ] [ t^2 ]
///     [ dX, dY, dZ, dW ] [ -1  3 -3  1 ] [ t^3 ] * 0.5 => v

/// v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

AFXINL void     AfxCatmullV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const d, afxReal t);
AFXINL void     AfxCatmullV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const d, afxReal t);
AFXINL void     AfxCatmullV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const d, afxReal t);

AFXINL void     AfxReflectV2d(afxV2d v, afxV2d const incident, afxV2d const normal);
AFXINL void     AfxReflectV3d(afxV3d v, afxV3d const incident, afxV3d const normal);
AFXINL void     AfxReflectV4d(afxV4d v, afxV3d const incident, afxV3d const normal);

AFXINL void     AfxRefractV2d(afxV2d v, afxV2d const incident, afxV2d const normal, afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxV3d v, afxV3d const incident, afxV3d const normal, afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxV3d v, afxV3d const incident, afxV3d const normal, afxReal refracIdx);

#endif//AFX_VERTEX_H
