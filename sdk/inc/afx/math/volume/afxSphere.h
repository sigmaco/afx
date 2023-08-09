/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SPHERE_H
#define AFX_SPHERE_H

#include "afxVolume.h"
#include "afx/math/afxMathDefs.h"

AFX_DEFINE_STRUCT(afxSphere)
{
    afxVolume   vol;
    afxV3d      origin;
    afxReal     radius;
};

AFXINL afxSphere* AfxSphereDeploy(afxSphere* sph, afxV3d const origin, afxReal radius)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sph);
    AfxAssert(origin);
    sph->vol.type = AFX_VOL_SPHERE;
    AfxV3dCopy(sph->origin, origin);
    sph->radius = radius;
    return sph;
}

#endif//AFX_SPHERE_H