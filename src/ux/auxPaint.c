/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia UX Extensions & Experiments.

#include "qwadro/ux/auxPaint.h"

void ClumpRenderBoundingBox(afxDrawContext dctx, afxBox box, afxM4d const m)
{
    // based on RenderWare void ClumpRenderBoundingBox(RpClump *clump);

    avxImmVertex vtx[8];
    avxVertexIndex const indices[24] = { 0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7 };
    
    for (int i = 0; i < 8; i++)
    {
        AfxV3dSet(vtx[i].posn, i & 1 ? box.max[0] : box.min[0], i & 2 ? box.max[1] : box.min[1], i & 4 ? box.max[2] : box.min[2]);
        vtx[i].col = AVX_ARGB(255, 196, 196, 0);
    }

    // set flat shade mode
    // set no raster

    //AvxCmdTransformImmVertices(dctx, m, 8, vtx);
    //AvxCmdPushImmIndexedPrimitive(dctx, avxTopology_LINE_LIST, 24, indices);
    
    return;
}




