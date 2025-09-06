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

#ifndef ARX_COLLIDER_H
#define ARX_COLLIDER_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/cad/arxMesh.h"

AFX_DEFINE_STRUCT(arxCollTriangle)
{
    afxV3d  v0; // Reference vertex of the triangle
    afxV3d  e1; // First edge of the triangle (v1 - v0)
    afxV3d  e2; // Second edge of the triangle (v2 - v0)
    afxV3d  nrm; // Normal of the triangle (cross(edge1, edge2))
};



#endif//ARX_COLLIDER_H
