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

#ifndef AFX_TRIGONOMETRY_H
#define AFX_TRIGONOMETRY_H

#include "qwadro/inc/math/afxVector.h"

AFXINL void     AfxComputeTriangleNormal(afxV3d const a, afxV3d const b, afxV3d const c, afxReal n[3]);

/**
    Barycentric coordinates are very useful in 2D and 3D graphics. 
    Most graphic applications use them because they provide an easy way to interpolate the value of attributes (color, textures, normals…) between vertices. 
    This can be done because, by definition, barycentric coordinates express “how much of each vertex does a point have”.
    Another frequent use is to use the barycentric coordinates to check if a point is inside, on the edge, or outside the triangle. As we saw:
        If all barycentric coordinates of a point are positive and sum one, they point lies inside the triangle.
        If one barycentric coordinate is zero and the other ones are positive and less than one, the point lies on an edge.
        If any barycentric coordinate is negative, the point is outside the triangle.
        
    Returns a point in Barycentric coordinates, using the specified position vectors.
*/

AFXINL void     AfxBarycentricV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g);
AFXINL void     AfxBarycentricV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g);
AFXINL void     AfxBarycentricV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g);

AFXINL void     AfxBarycentricV2d2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g);
AFXINL void     AfxBarycentricV3d2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g);
AFXINL void     AfxBarycentricV4d2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g);

#endif//AFX_TRIGONOMETRY_H
