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

#include "qwadro/inc/math/bound/afxVolume.h"

#define AFX_SPHERE_RADIUS (3)

#define AFX_SPHERE(x_, y_, z_, radius_) \
    (afxSphere){ .xyzr = { (x_), (y_), (z_), (radius_) }

AFXINL afxSphere*   AfxMakeSphere(afxSphere* sph, afxV3d const centre, afxReal radius);

AFXINL void         AfxGetSphereOrigin(afxSphere const sph, afxV4d centre);

AFXINL afxBox       AfxGetSphereAabb(afxSphere const sph);

#endif//AFX_SPHERE_H
