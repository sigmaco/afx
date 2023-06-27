/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_CANVAS_H
#define AFX_CANVAS_H

#include "afx/draw/res/afxSurface.h"

AFX_DEFINE_HANDLE(afxCanvas);

#if 0
AFX_DEFINE_STRUCT(afxInterlink)
{
    void            *a; // usually a active object controlling B.
    afxInterlink    *aPrev;
    afxInterlink    *aNext;
    void            *b; // usually a passive object being controlled by A.
    afxInterlink    *bPrev;
    afxInterlink    *bNext;
};

#define AFX_DEFINE_INTERLINK(interlink_, typeA_, nameA_, typeB_, nameB_) typedef struct interlink_ { typeA_ nameA_; typeA_*nameA_##Prev;typeA_*nameA_##Next;typeB_ nameB_; typeB_*nameB_##Prev;typeB_*nameB_##Next;} interlink_;

AFXINL afxError AfxInterlinkDeployAsA(afxInterlink *intk, void *a, afxInterlink *prev, afxInterlink *next)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(intk);
    AfxAssert(a);
    intk->b = NIL;
    intk->aPrev = NIL;
    intk->bNext = NIL;
    intk->a = a;
    intk->aPrev = prev;
    intk->aNext = next;
}

AFXINL afxError AfxInterlinkDeployAsB(afxInterlink *intk, void *b, afxInterlink *prev, afxInterlink *next)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(intk);
    AfxAssert(b);
    intk->a = NIL;
    intk->aPrev = NIL;
    intk->aNext = NIL;
    intk->b = b;
    intk->bPrev = prev;
    intk->bNext = next;
}

AFX_DEFINE_STRUCT(afxCanvasSurfaceInterlink2222222)
{
    afxCanvas   canv;
    afxCanvas   *canvPrev;
    afxCanvas   *canvNext;
    afxSurface  surf;
    afxSurface  *surfPrev;
    afxSurface  *surfNext;
};

AFX_DEFINE_INTERLINK(afxCanvasSurfaceInterlink, afxCanvas, canv, afxSurface, surf);
#endif

AFX_DEFINE_STRUCT(afxCanvasBlueprint)
{
    afxWhd              extent;
    afxNat              rasterCnt;
    afxSurface          rasterExistingObj[8]; // one for each rasCnt. pass NIL to it be created using rasFmt and rasUsage. If exists must has been created with AFX_TEX_USAGE_SURFACE_RASTER flag.
    afxPixelFormat      rasterFmt[8]; // one for each rasCnt
    afxTextureUsage     rasterUsage[8]; // one for each rasCnt
    afxPixelFormat      depthFmt; // AFX_PIXEL_FMT_D24. Pass nil to it doesn't have depth.
    afxPixelFormat      stencilFmt; // AFX_PIXEL_FMT_S8. Pass nil to it doesn't have stencil.
};

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxCanvas) // our concept of a framebuffer
{
    afxObject           obj;
};

#endif

// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

AFX void*               AfxCanvasGetContext(afxCanvas canv);
AFX void*               AfxCanvasGetDriver(afxCanvas canv);
AFX void*               AfxCanvasGetDrawSystem(afxCanvas canv);

AFX afxNat*             AfxCanvasGetExtent(afxCanvas canv, afxWhd extent);
AFX afxError            AfxCanvasSetExtent(afxCanvas canv, afxWhd const extent);

AFX afxNat              AfxCanvasGetSurfaceCount(afxCanvas canv);
AFX afxSurface          AfxCanvasGetSurface(afxCanvas canv, afxNat surfIdx);
AFX afxNat              AfxCanvasGetRasterCount(afxCanvas canv);
AFX afxSurface          AfxCanvasGetRaster(afxCanvas canv, afxNat rasIdx);
AFX afxSurface          AfxCanvasGetDepth(afxCanvas canv);
AFX afxSurface          AfxCanvasGetStencil(afxCanvas canv);

////////////////////////////////////////////////////////////////////////////////
// CANVAS BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxError         AfxCanvasBlueprintReset(afxCanvasBlueprint *blueprint, afxWhd extent, afxPixelFormat depth, afxPixelFormat stencil);
AFXINL afxError         AfxCanvasBlueprintAddRaster(afxCanvasBlueprint *blueprint, afxSurface existing, afxPixelFormat fmt, afxTextureUsage usage);
AFXINL afxError         AfxCanvasBlueprintResetRaster(afxCanvasBlueprint *blueprint, afxNat idx, afxSurface existing, afxPixelFormat fmt, afxTextureUsage usage);

#endif//AFX_CANVAS_H