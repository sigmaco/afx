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

#ifndef ASX_QUADTREE_H
#define ASX_QUADTREE_H

#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/math/afxMathDefs.h"
#include "qwadro/inc/math/coll/afxBox.h"
#include "qwadro/inc/math/coll/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/sim/body/afxPose.h"
#include "qwadro/inc/sim/body/afxPlacement.h"


AFX_DEFINE_STRUCT(asxQuadrant)
{
    afxRect     bounds;
    afxUnit     childIdx[4];
    afxUnit     parentIdx;
    afxChain    contents;
};

AFX_DEFINE_STRUCT(asxQuadtree)
{
    asxQuadrant root;
    afxPool     quadrants;
};


#endif//ASX_QUADTREE_H
