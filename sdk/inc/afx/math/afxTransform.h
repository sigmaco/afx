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

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "afxMathDefs.h"

// You can initialize a afxTransform to the identity transform like this:

AFXINL void     AfxTransformZero(afxTransform *t); // make zero
AFXINL void     AfxTransformMakeIdentity(afxTransform *t); // make identity

AFXINL void     AfxTransformCopy(afxTransform *t, afxTransform const *in);

AFXINL void     AfxTransformSet(afxTransform *t, afxV4d const origin, afxQuat const orientation, afxM3d const scaleShear);

AFXINL void     AfxTransformInvert(afxTransform const *t, afxTransform *out);

AFXINL void     AfxTransformClipDofs(afxTransform *t, afxFlags AllowedDOFs);

AFXINL void     AfxTransformLerp(afxTransform const *t, afxTransform const *other, afxReal time, afxTransform *out); // aka linear blend

AFXINL void     AfxTransformSetWithIdentityCheck(afxTransform *t, afxV4d const origin, afxQuat const orientation, afxM3d const scaleShear);

AFXINL void     AfxTransformPreMultiply(afxTransform *t, afxTransform const *pre);
AFXINL void     AfxTransformPostMultiply(afxTransform *t, afxTransform const *post);
AFXINL void     AfxTransformMultiply(afxTransform const *t, afxTransform const *other, afxTransform *out);

AFXINL afxReal  AfxTransformDet(afxTransform const *t);
AFXINL void     AfxTransformComposeAffineM4d(afxTransform const *t, afxM4d m); // build composite transform 4x4

#endif//AFX_TRANSFORM_H
