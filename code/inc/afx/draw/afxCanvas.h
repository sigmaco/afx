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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_CANVAS_H
#define AFX_CANVAS_H

#include "afx/draw/afxSurface.h"

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
AFX_OBJECT(afxCanvas)
#else
struct afxBaseCanvas
#endif
{
    afxNat              whd[3]; // d= layers; just for 3D images, else case its 1.

    afxNat              annexCnt;
    struct
    {
        afxRaster      tex; // 8 raster surfaces [afxRaster] are minimal amount garanteed.
        afxPixelFormat  fmt;
        afxRasterFlags usage;
    }*                  annexes;
    afxFlags            ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
//afxCanvasSurfaceInterlink interlinkSentinel;

    afxNat              rasterCnt;
    afxNat              dsIdx[2];
    afxCanvasFlags      flags;
    void*               udd[2];
};
#endif

// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

AFX void            AfxGetCanvasExtent(afxCanvas canv, afxWhd extent);

AFX afxResult       AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask);
AFX void            AfxFlagCanvas(afxCanvas canv, afxCanvasFlags bitmask);
AFX void            AfxUnflagCanvas(afxCanvas canv, afxCanvasFlags bitmask);

AFX afxNat          AfxGetAnnexedSurfaceCount(afxCanvas canv);
AFX afxBool         AfxGetAnnexedSurface(afxCanvas canv, afxNat surfIdx, afxRaster *tex);
AFX afxNat          AfxGetAnnexedRasterSurfaceCount(afxCanvas canv);
AFX afxBool         AfxGetAnnexedRasterSurface(afxCanvas canv, afxNat rasIdx, afxRaster *raster);
AFX afxBool         AfxGetAnnexedDepthSurface(afxCanvas canv, afxRaster *depth);
AFX afxBool         AfxGetAnnexedStencilSurface(afxCanvas canv, afxRaster *stencil);

AFX afxError        AfxAnnexRasterSurface(afxCanvas canv, afxRaster ras);
AFX afxError        AfxAnnexDepthSurface(afxCanvas canv, afxRaster d);
AFX afxError        AfxAnnexStencilSurface(afxCanvas canv, afxRaster s);
AFX afxError        AfxAnnexCombinedDepthStencil(afxCanvas canv, afxRaster ds);

#endif//AFX_CANVAS_H