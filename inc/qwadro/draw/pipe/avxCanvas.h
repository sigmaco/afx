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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// No Qwadro, avxCanvas é um recurso que combina operações de framebuffer e render pass em outras APIs.
/// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

#ifndef AVX_CANVAS_H
#define AVX_CANVAS_H

#include "qwadro/draw/afxRaster.h"

AFX_DEFINE_STRUCT(afxSurfaceConfig)
{
    afxPixelFormat  fmt; // layout
    afxNat          sampleCnt; // layout
    afxRasterUsage  rasUsage;
    afxRasterFlags  rasFlags;
};

AVX afxResult       AfxTestCanvas(avxCanvas canv, afxFlags bitmask);

AVX void            AfxGetCanvasExtent(avxCanvas canv, afxWhd whd);
AVX afxError        AfxReadjustCanvas(avxCanvas canv, afxWhd const whd);

AVX afxNat          AfxCountRasterSlots(avxCanvas canv);
AVX afxNat          AfxCountColorBufferSlots(avxCanvas canv);

AVX afxBool         AfxGetDepthBufferSlot(avxCanvas canv, afxNat* surIdx);
AVX afxBool         AfxGetStencilBufferSlot(avxCanvas canv, afxNat* surIdx);
AVX afxBool         AfxHasCombinedDepthStencilBuffer(avxCanvas canv);

AVX afxBool         AfxEnumerateDrawBuffers(avxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);
AVX afxBool         AfxGetColorBuffer(avxCanvas canv, afxNat surIdx, afxRaster* buf);
AVX afxBool         AfxGetDepthBuffer(avxCanvas canv, afxRaster* buf);
AVX afxBool         AfxGetStencilBuffer(avxCanvas canv, afxRaster* buf);

// must be compatible with format, have at least sampleCnt, and not be less extent than canvas.
AVX afxError        AfxRelinkDrawBuffers(avxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);
AVX afxError        AfxRelinkDepthBuffer(avxCanvas canv, afxRaster depth);
AVX afxError        AfxRelinkStencilBuffer(avxCanvas canv, afxRaster stencil);

AVX afxError        AfxPrintDrawBuffer(avxCanvas canv, afxNat surIdx, afxNat portIdx, afxRasterIo const* op, afxUri const* uri);

AVX afxError        AfxRedoDrawBuffers(avxCanvas canv);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireCanvases(afxDrawContext dctx, afxWhd const whd, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, avxCanvas canvases[]);

#endif//AVX_CANVAS_H
