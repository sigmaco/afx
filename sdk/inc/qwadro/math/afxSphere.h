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

#ifndef AFX_SPHERE_H
#define AFX_SPHERE_H

#include "qwadro/math/afxVector.h"

AFX_DEFINE_STRUCT(afxSphere)
{
    afxV3d      origin;
    afxReal     radius;
};

AFXINL afxSphere* AfxSphereDeploy(afxSphere* sph, afxV3d const origin, afxReal radius)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sph);
    AfxAssert(origin);
    AfxCopyV3d(sph->origin, origin);
    sph->radius = radius;
    return sph;
}

#endif//AFX_SPHERE_H
