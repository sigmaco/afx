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

#include "qwadro/inc/draw/io/avxPaint.h"

void ClumpRenderBoundingBox(avxCmdb cmdb, afxBox box, afxM4d const m)
{
    // based on RenderWare void ClumpRenderBoundingBox(RpClump *clump);

    avxImmVertex vtx[8];
    avxVertexIndex const indices[24] = { 0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7 };
    
    for (int i = 0; i < 8; i++)
    {
        AfxV3dSet(vtx[i].posn, i & 1 ? box.sup[0] : box.inf[0], i & 2 ? box.sup[1] : box.inf[1], i & 4 ? box.sup[2] : box.inf[2]);
        vtx[i].col = AfxPackArgb32(255, 196, 196, 0);
    }

    // set flat shade mode
    // set no raster

    //AvxCmdTransformImmVertices(cmdb, m, 8, vtx);
    //AvxCmdPushImmIndexedPrimitive(cmdb, avxTopology_LINE_LIST, 24, indices);
    
    return;
}




