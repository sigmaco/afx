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

#ifndef AFX_PLANE_H
#define AFX_PLANE_H

#include "afx/math/afxVector.h"
#include "afx/math/afxVector.h"

//typedef afxV4d afxPlane;

AFX_DEFINE_STRUCT(afxPlane)
{
    afxV3d  normal;
    afxReal offset; // aka dist
};

AFXINL void     AfxCopyPlane(afxPlane *p, afxPlane const *in);

AFXINL void     AfxSetPlane(afxPlane* p, afxReal const normal[3], afxReal dist);

AFXINL void     AfxPlaneFromTriangle(afxPlane* p, afxReal const a[3], afxReal const b[3], afxReal const c[3]);

AFXINL afxReal* AfxGetPlaneOrigin(afxPlane* p);

AFXINL afxReal  AfxGetPlaneOffset(afxPlane const* p);

AFXINL afxReal  AfxFindPlaneDistanceToPoint(afxPlane const* p, afxReal const point[3]);

AFXINL afxReal  AfxFindPlaneHitInterpolationConstant(afxPlane const* p, afxReal const a[3], afxReal const b[3]);

#endif//AFX_PLANE_H