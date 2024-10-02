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

#include "qwadro/inc/draw/io/afxRaster.h"

typedef enum afxCanvasFlag
{
    afxCanvasFlag_HAS_DEPTH     = AFX_BIT(0),
    afxCanvasFlag_HAS_STENCIL   = AFX_BIT(1),
    afxCanvasFlag_COMBINED_DS   = AFX_BIT(2)
} afxCanvasFlags;

AFX_DEFINE_STRUCT(afxSurfaceConfig)
{
    avxFormat       fmt; // layout
    afxNat          sampleCnt; // layout
    afxRasterUsage  rasUsage;
    afxRasterFlags  rasFlags;
};

AVX afxResult       AfxTestCanvas(avxCanvas canv, afxCanvasFlags bitmask);

AVX void            AfxGetCanvasExtent(avxCanvas canv, afxWhd whd);
AVX afxError        AfxReadjustCanvas(avxCanvas canv, afxWhd const whd);

AVX void            AfxCountDrawBufferSlots(avxCanvas canv, afxNat* slotCnt, afxNat* colorSlotCnt);

AVX afxBool         AfxGetDepthBufferSlots(avxCanvas canv, afxNat* dSlotIdx, afxNat* sSlotIdx);

AVX afxBool         AfxGetDrawBuffers(avxCanvas canv, afxNat baseSlotIdx, afxNat cnt, afxRaster rasters[]);
AVX afxBool         AfxGetColorBuffers(avxCanvas canv, afxNat baseSlotIdx, afxNat cnt, afxRaster rasters[]);
AVX afxBool         AfxGetDepthBuffers(avxCanvas canv, afxRaster* depth, afxRaster* stencil);

// must be compatible with format, have at least sampleCnt, and not be less extent than canvas.
AVX afxError        AfxRelinkDrawBuffers(avxCanvas canv, afxNat baseSlot, afxNat cnt, afxRaster rasters[]);
AVX afxError        AfxRelinkDepthBuffer(avxCanvas canv, afxRaster depth);
AVX afxError        AfxRelinkStencilBuffer(avxCanvas canv, afxRaster stencil);

AVX afxError        AfxPrintDrawBuffer(avxCanvas canv, afxNat surIdx, afxNat portIdx, afxRasterIo const* op, afxUri const* uri);

AVX afxError        AfxRedoDrawBuffers(avxCanvas canv);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxCoacquireCanvas(afxDrawContext dctx, afxWhd const whd, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, avxCanvas canvases[]);

#endif//AVX_CANVAS_H
