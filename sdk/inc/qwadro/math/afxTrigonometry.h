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

#ifndef AFX_TRIGONOMETRY_H
#define AFX_TRIGONOMETRY_H

#include "afxVector.h"

AFXINL void     AfxComputeTriangleNormal(afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal n[3]);

/// Barycentric coordinates are very useful in 2D and 3D graphics. 
/// Most graphic applications use them because they provide an easy way to interpolate the value of attributes (color, textures, normals…) between vertices. 
/// This can be done because, by definition, barycentric coordinates express “how much of each vertex does a point have”.
/// Another frequent use is to use the barycentric coordinates to check if a point is inside, on the edge, or outside the triangle. As we saw:
///     If all barycentric coordinates of a point are positive and sum one, they point lies inside the triangle.
///     If one barycentric coordinate is zero and the other ones are positive and less than one, the point lies on an edge.
///     If any barycentric coordinate is negative, the point is outside the triangle.

/// Returns a point in Barycentric coordinates, using the specified position vectors.

AFXINL void     AfxBarycentricV2d(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal f, afxReal g);
AFXINL void     AfxBarycentricV3d(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal f, afxReal g);
AFXINL void     AfxBarycentricV4d(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal f, afxReal g);

AFXINL void     AfxBarycentricV2d2(afxReal v[2], afxReal const a[2], afxReal const b[2], afxReal const c[2], afxReal const f[2], afxReal const g[2]);
AFXINL void     AfxBarycentricV3d2(afxReal v[3], afxReal const a[3], afxReal const b[3], afxReal const c[3], afxReal const f[3], afxReal const g[3]);
AFXINL void     AfxBarycentricV4d2(afxReal v[4], afxReal const a[4], afxReal const b[4], afxReal const c[4], afxReal const f[4], afxReal const g[4]);

#endif//AFX_TRIGONOMETRY_H
