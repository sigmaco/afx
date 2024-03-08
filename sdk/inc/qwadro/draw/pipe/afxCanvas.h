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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

/// No Qwadro, afxCanvas é um recurso que combina operações de framebuffer e render pass em outras APIs.

#ifndef AFX_CANVAS_H
#define AFX_CANVAS_H

#include "qwadro/draw/io/afxRaster.h"

typedef enum afxEventCanvas
{
    AFX_EVENT_CANV_RESIZ
} afxEventCanvas;

typedef enum afxCanvasFlags
{
    AFX_CANV_FLAG_REVALIDATE
} afxCanvasFlags;

#ifdef _AFX_DRAW_C
#ifdef _AFX_CANVAS_C
AFX_DEFINE_STRUCT(afxSurface)
{
    afxRaster       ras; /// the texture subresource that will be output to for this color attachment.
    afxRaster       resolve; /// the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxBool         managed;
    afxPixelFormat  fmt; /// the format of the image that will be used for the attachment.
    afxRasterUsage  usage; /// additional properties of the attachment.
    afxRasterFlags  flags;
    afxNat          sampleCnt; /// the number of samples of the image.
};

#ifndef _AFX_CANVAS_IMPL
AFX_OBJECT(afxCanvas)
#else
struct afxBaseCanvas
#endif
{
    afxNat          wh[2];
    afxNat          minWh[2]; // when a user-provided raster are attached
    afxNat          layerCnt;
    afxNat          linkedCnt;
    afxNat          surfaceCnt; // immutable
    afxSurface*     surfaces;
    afxFlags        ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
    afxNat          colorCnt;
    afxNat          dsIdx[2];
    afxBool         combinedDs; // just for convenience
    afxCanvasFlags  flags;
    afxError        (*readjust)(afxCanvas, afxWhd const);
    afxError        (*relink)(afxCanvas,afxBool,afxNat,afxNat,afxRaster[]);
    void*           udd[2];
};
#endif
#endif

// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

AVX void            AfxGetCanvasExtent(afxCanvas canv, afxWhd whd);
AVX afxError        AfxReadjustCanvas(afxCanvas canv, afxWhd const whd);

AVX afxResult       AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask);

AVX afxBool         AfxHasCombinedDepthStencilBuffer(afxCanvas canv);

AVX afxNat          AfxCountSurfaces(afxCanvas canv);
AVX afxNat          AfxCountDrawSurfaces(afxCanvas canv);
AVX afxBool         AfxGetDepthSurface(afxCanvas canv, afxNat* surfIdx);
AVX afxBool         AfxGetStencilSurface(afxCanvas canv, afxNat* surfIdx);

AVX afxNat          AfxGetDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);
AVX afxRaster       AfxGetLinkedDepthBuffer(afxCanvas canv);
AVX afxRaster       AfxGetLinkedStencilBuffer(afxCanvas canv);

AVX afxNat          AfxGetSurfaceFormats(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);

// must be compatible with format, have at least sampleCnt, and not be less extent than canvas.
AVX afxError        AfxRelinkDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);
AVX afxError        AfxRelinkDepthBuffer(afxCanvas canv, afxRaster depth);
AVX afxError        AfxRelinkStencilBuffer(afxCanvas canv, afxRaster stencil);

AVX afxError        AfxGenerateDrawBuffers(afxCanvas canv);

AFX_DEFINE_STRUCT(afxSurfaceConfig)
{
    afxPixelFormat      fmt; // layout
    afxNat              sampleCnt; // layout
    afxRasterUsage      rasUsage;
    afxRasterFlags      rasFlags;
};

AVX afxError        AfxAcquireCanvases(afxDrawContext dctx, afxWhd const whd, afxNat layerCnt, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, afxCanvas canvases[]);

#endif//AFX_CANVAS_H
