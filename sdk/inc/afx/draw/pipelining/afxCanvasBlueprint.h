/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_CANVAS_BLUEPRINT_H
#define AFX_CANVAS_BLUEPRINT_H

#include "afx/draw/pipelining/afxCanvas.h"

AFX_DEFINE_STRUCT(afxCanvasBlueprint)
{
    afxDrawContext      dctx;
    afxWhd              extent;
    afxNat              rasterCnt;
    struct
    {
        afxTexture      tex; // one for each rasCnt. pass NIL to it be created using rasFmt and rasUsage. If exists must has been created with AFX_TEX_FLAG_SURFACE_RASTER flag.
        afxPixelFormat  fmt; // one for each rasCnt
        afxTextureFlags usage; // one for each rasCnt
    }                   rasters[8];
    struct
    {
        afxTexture      tex; // actually unsupported
        afxPixelFormat  fmt;
        afxTextureFlags usage;
    }                   ds[2];
    void*               udd[2];
};

//afxDrawContext dctx, afxNat cnt,

AFXINL void         AfxBeginCanvasBlueprint(afxCanvasBlueprint *blueprint, afxWhd const extent);

AFXINL void         AfxCanvasBlueprintSetExtent(afxCanvasBlueprint *blueprint, afxWhd const extent);
AFXINL void         AfxCanvasBlueprintSetDepth(afxCanvasBlueprint *blueprint, afxPixelFormat depth, afxPixelFormat stencil);

AFXINL void         AfxCanvasBlueprintAddRaster(afxCanvasBlueprint *blueprint, afxPixelFormat fmt, afxTextureFlags usage);
AFXINL void         AfxCanvasBlueprintAddExistingRaster(afxCanvasBlueprint *blueprint, afxTexture existing);

AFX afxError        AfxBuildCanvases(afxDrawContext dctx, afxNat cnt, afxCanvas canv[], afxCanvasBlueprint const *blueprint);

#endif//AFX_CANVAS_BLUEPRINT_H