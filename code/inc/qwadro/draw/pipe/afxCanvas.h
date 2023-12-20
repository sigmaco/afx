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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
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
AFX_DEFINE_STRUCT(afxSurface)
{
    afxRaster       ras; /// the texture subresource that will be output to for this color attachment.
    afxRaster       resolve; /// the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxBool         managed;
    afxPixelFormat  fmt; /// the format of the image that will be used for the attachment.
    afxRasterFlags  usage; /// additional properties of the attachment.
    afxNat          sampleCnt; /// the number of samples of the image.
};

#ifdef _AFX_CANVAS_C
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
    afxError        (*readjust)(afxCanvas,afxNat const[2]);
    afxError        (*relink)(afxCanvas,afxBool,afxNat,afxNat,afxRaster[]);
    void*           udd[2];
};
#endif

// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

AFX void            AfxGetCanvasExtent(afxCanvas canv, afxNat wh[2]);
AFX afxError        AfxReadjustCanvas(afxCanvas canv, afxNat const wh[2]);

AFX afxResult       AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask);

AFX afxBool         AfxCanvasHasCombinedDepthStencil(afxCanvas canv);

AFX afxNat          AfxCountSurfaces(afxCanvas canv);
AFX afxNat          AfxCountColorSurfaces(afxCanvas canv);
AFX afxBool         AfxGetDepthSurface(afxCanvas canv, afxNat* surfIdx);
AFX afxBool         AfxGetStencilSurface(afxCanvas canv, afxNat* surfIdx);

AFX afxNat          AfxGetSurfaceRasters(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);
AFX afxRaster       AfxGetLinkedDepthRaster(afxCanvas canv);
AFX afxRaster       AfxGetLinkedStencilRaster(afxCanvas canv);

AFX afxNat          AfxGetSurfaceFormats(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);

// must be compatible with format, have at least sampleCnt, and not be less extent than canvas.
AFX afxError        AfxRelinkSurfaceRasters(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[]);
AFX afxError        AfxRelinkDepthSurface(afxCanvas canv, afxRaster depth);
AFX afxError        AfxRelinkStencilSurface(afxCanvas canv, afxRaster stencil);

AFX afxError        AfxGenerateSurfaceRasters(afxCanvas canv);

AFX_DEFINE_STRUCT(afxSurfaceConfig)
{
    afxPixelFormat      fmt; // layout
    afxNat              sampleCnt; // layout
    afxRasterFlags      rasUsage;
};

AFX afxError        AfxAcquireCanvases(afxDrawContext dctx, afxNat const wh[2], afxNat layerCnt, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, afxCanvas canv[]);

#endif//AFX_CANVAS_H
