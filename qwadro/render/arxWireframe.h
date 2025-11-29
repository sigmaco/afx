/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ARX_WIREFRAME_H
#define ARX_WIREFRAME_H

#include "qwadro/render/arxRenderable.h"

/*
    Wireframe should support line lists, line strips and face-based (degenerate) lines.
*/

AFX_DEFINE_STRUCT(arxWireframePush)
{
    afxM4d  uM; // transform matrix
    afxV3d  uWireColor; // = vec3(1.0, 1.0, 1.0);
    afxReal uWireThickness; // = 0.02;
    afxV3d  uFillColor; // = vec3(0.1, 0.1, 0.1);
};

AFX_DEFINE_STRUCT(arxWireVertex)
{
    afxV3d pos;
    afxV3d bary;
};

ARX afxError ArxLoadWireframePipeline(arxRenderContext rctx, afxBool bary, afxUri const* uri, avxPipeline* pipeline);

ARX void SphereDebugDraw_barycentric(arxRenderContext rctx, afxM4d const m, afxReal radius);
ARX void SphereDebugDraw(arxRenderContext rctx, afxM4d const m, afxReal radius);
ARX void drawWireSphere(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs);
ARX void drawWireSphereLineStrip(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs);

ARX void drawWireBox(arxRenderContext rctx, afxM4d const m, afxV3d extents);
ARX void drawWireBox2(arxRenderContext rctx, afxV3d const center, afxQuat const rotation, afxV3d const halfExtents);

ARX afxError ArxSetWireframeConstants(arxRenderContext rctx, afxReal wireThickness, avxColor wireCol, avxColor fillCol);

#endif//ARX_WIREFRAME_H
