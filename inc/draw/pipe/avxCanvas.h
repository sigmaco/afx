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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/**
    No Qwadro, avxCanvas é um recurso que combina operações de framebuffer e render pass em outras APIs.
    An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.
*/

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
    afxRasterUsage  rasUsage;
    afxRasterFlags  rasFlags;
    afxUnit         sampleCnt; // layout
};

AVX afxResult       AfxTestCanvas(avxCanvas canv, afxCanvasFlags bitmask);

AVX afxWhd          AfxGetCanvasExtent(avxCanvas canv);
AVX afxError        AfxReadjustCanvas(avxCanvas canv, afxWhd whd);

AVX void            AfxCountDrawBufferSlots(avxCanvas canv, afxUnit* slotCnt, afxUnit* colorSlotCnt);

AVX afxBool         AfxGetDepthBufferSlots(avxCanvas canv, afxUnit* dSlotIdx, afxUnit* sSlotIdx);

AVX afxBool         AfxGetDrawBuffers(avxCanvas canv, afxUnit baseSlotIdx, afxUnit cnt, afxRaster rasters[]);
AVX afxBool         AfxGetColorBuffers(avxCanvas canv, afxUnit baseSlotIdx, afxUnit cnt, afxRaster rasters[]);
AVX afxBool         AfxGetDepthBuffers(avxCanvas canv, afxRaster* depth, afxRaster* stencil);

// must be compatible with format, have at least sampleCnt, and not be less extent than canvas.
AVX afxError        AfxRelinkDrawBuffers(avxCanvas canv, afxUnit baseSlot, afxUnit cnt, afxRaster rasters[]);
AVX afxError        AfxRelinkDepthBuffer(avxCanvas canv, afxRaster depth);
AVX afxError        AfxRelinkStencilBuffer(avxCanvas canv, afxRaster stencil);

AVX afxError        AfxPrintDrawBuffer(avxCanvas canv, afxUnit surIdx, afxRasterIo const* op, afxUnit exuIdx, afxUri const* uri);

AVX afxError        AfxRedoDrawBuffers(avxCanvas canv);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxCoacquireCanvas(afxDrawSystem dsys, afxWhd whd, afxUnit surCnt, afxSurfaceConfig const surCfg[], afxUnit cnt, avxCanvas canvases[]);

#endif//AVX_CANVAS_H
