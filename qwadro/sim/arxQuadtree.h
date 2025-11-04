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

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ASX_QUADTREE_H
#define ASX_QUADTREE_H

#include "qwadro/base/afxObject.h"
#include "qwadro/base/afxString.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/coll/afxBox.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/render/arxPlacement.h"


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
