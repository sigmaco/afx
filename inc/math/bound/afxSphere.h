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

#include "qwadro/inc/math/afxVector.h"

#pragma pack(push, 1)
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxSphere)
{
    afxV3d  centre;
    afxReal radius;
};
#pragma pack(pop)

AFX_STATIC_ASSERT(sizeof(afxSphere) == 16, "");
AFX_STATIC_ASSERT(offsetof(afxSphere, radius) == 12, "");

#define AFX_SPHERE(x_, y_, z_, radius_) (afxSphere){ .centre = { (x_), (y_), (z_) }, .radius = (radius_)}

AFXINL afxSphere* AfxMakeSphere(afxSphere* sph, afxV3d const centre, afxReal radius);

#endif//AFX_SPHERE_H
