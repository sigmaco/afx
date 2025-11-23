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

#ifndef ASX_OCTREE_H
#define ASX_OCTREE_H

#include "qwadro/base/afxObject.h"
#include "qwadro/base/afxString.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/coll/afxBox.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxPlacement.h"

/*
    An Octree is a tree data structure that is commonly used to partition a three-dimensional space by recursively
    subdividing it into eight smaller regions (octants).

    An octant is one of the eight divisions of a three-dimensional coordinate system defined by the signs of the coordinates.
*/

// Intrusive Octree

AFX_DEFINE_STRUCT(arxOctant)
{
    afxBox      aabb;
    arxOctant*  parent; // parent octant's index
    arxOctant*  child[8]; // child octant's index
    afxChain    contents;
};

AFX_DEFINE_STRUCT(arxOctree)
{
    arxOctant   root; // root octant --- always exist
    afxBox      (*calc)(void* content);
};




#endif//ASX_OCTREE_H
