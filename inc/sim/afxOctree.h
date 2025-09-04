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

#ifndef ASX_OCTREE_H
#define ASX_OCTREE_H

#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/math/afxMathDefs.h"
#include "qwadro/inc/math/coll/afxBox.h"
#include "qwadro/inc/math/coll/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/render/cad/arxPose.h"
#include "qwadro/inc/render/cad/arxPlacement.h"

/*
    An Octree is a tree data structure that is commonly used to partition a three-dimensional space by recursively
    subdividing it into eight smaller regions (octants).

    An octant is one of the eight divisions of a three-dimensional coordinate system defined by the signs of the coordinates.
*/

AFX_DEFINE_STRUCT(afxOctant)
{
    afxBox      bounds;
    afxUnit     childIdx[8]; // child octant's index
    afxUnit     parentIdx; // parent octant's index
    afxChain    contents;
};

AFX_DEFINE_STRUCT(afxOctree)
{
    afxOctant   root; // root octant --- always exist
    afxPool     octants;
    afxBox(*calc)(void* content);
};




#endif//ASX_OCTREE_H
