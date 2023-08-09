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

#ifndef AFX_CANVAS_H
#define AFX_CANVAS_H

#include "afx/draw/res/afxSurface.h"

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

AFX_DECLARE_STRUCT(_afxCanvVmt);

AFX_OBJECT(afxCanvas) // our concept of a framebuffer
{
    afxInstance           obj;
    _afxCanvVmt const*  vmt;
    void*               idd; // implementation-defined data
#ifdef _AFX_CANVAS_C
    afxNat              whd[3]; // d= layers; just for 3D images, else case its 1.

    afxNat              annexCnt;
    struct
    {
        afxSurface      surf; // 8 raster surfaces [afxTexture] are minimal amount garanteed.
        afxPixelFormat  fmt;
        afxTextureFlags usage;
    }*                  annexes;
    afxFlags            ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
//afxCanvasSurfaceInterlink interlinkSentinel;

    afxNat              rasterCnt;
    afxNat              dsIdx[2];
    afxCanvasFlags      flags;
    void*               udd[2];
#endif
};

// An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

AFX void            AfxGetCanvasContext(afxCanvas canv, afxDrawContext *dctx);

AFX void            AfxGetCanvasExtent(afxCanvas canv, afxWhd extent);
AFX afxError        AfxReadjustCanvas(afxCanvas canv, afxWhd const extent);

AFX afxResult       AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask);
AFX void            AfxFlagCanvas(afxCanvas canv, afxCanvasFlags bitmask);
AFX void            AfxUnflagCanvas(afxCanvas canv, afxCanvasFlags bitmask);

AFX afxNat          AfxGetAnnexedSurfaceCount(afxCanvas canv);
AFX afxBool         AfxGetAnnexedSurface(afxCanvas canv, afxNat surfIdx, afxSurface *surf);
AFX afxNat          AfxGetAnnexedRasterSurfaceCount(afxCanvas canv);
AFX afxBool         AfxGetAnnexedRasterSurface(afxCanvas canv, afxNat rasIdx, afxSurface *raster);
AFX afxBool         AfxGetAnnexedDepthSurface(afxCanvas canv, afxSurface *depth);
AFX afxBool         AfxGetAnnexedStencilSurface(afxCanvas canv, afxSurface *stencil);

AFX afxError        AfxAnnexRasterSurface(afxCanvas canv, afxSurface ras);
AFX afxError        AfxAnnexDepthSurface(afxCanvas canv, afxSurface d);
AFX afxError        AfxAnnexStencilSurface(afxCanvas canv, afxSurface s);
AFX afxError        AfxAnnexCombinedDepthStencil(afxCanvas canv, afxSurface ds);

#endif//AFX_CANVAS_H