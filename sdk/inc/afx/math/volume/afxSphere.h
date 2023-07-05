/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
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