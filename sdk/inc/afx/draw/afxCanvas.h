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

#ifndef AFX_CANVAS_H
#define AFX_CANVAS_H

#include "afx/draw/afxSurface.h"

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
