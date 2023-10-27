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

#define _AFX_DRAW_C
#define _AFX_CANVAS_C
#define _AFX_DRAW_CONTEXT_C
#include "afxDrawClassified.h"

_AFXINL void AfxBeginCanvasBlueprint(afxCanvasBlueprint *blueprint, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(extent);
    *blueprint = (afxCanvasBlueprint) { 0 };
    blueprint->dctx = NIL;
    AfxCanvasBlueprintSetExtent(blueprint, extent);
    AfxCanvasBlueprintSetDepth(blueprint, NIL, NIL);
}

_AFXINL void AfxCanvasBlueprintSetExtent(afxCanvasBlueprint *blueprint, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(extent);
    blueprint->extent[0] = AfxMaxi(extent[0], 1);
    blueprint->extent[1] = AfxMaxi(extent[1], 1);
    blueprint->extent[2] = AfxMaxi(extent[2], 1);
}

_AFXINL void AfxCanvasBlueprintSetDepth(afxCanvasBlueprint *blueprint, afxPixelFormat depth, afxPixelFormat stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(!depth || !(AFX_PFD_S8 > depth && AFX_PFD_D32FS8 < depth));
    AfxAssert(!stencil || !(AFX_PFD_S8 > stencil && AFX_PFD_D32FS8 < stencil));
    blueprint->ds[0].fmt = depth;
    blueprint->ds[1].fmt = stencil;
}

_AFXINL void AfxCanvasBlueprintAddRaster(afxCanvasBlueprint *blueprint, afxPixelFormat fmt, afxTextureFlags usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(fmt);
    blueprint->rasters[blueprint->rasterCnt].tex = NIL;
    blueprint->rasters[blueprint->rasterCnt].fmt = fmt;
    blueprint->rasters[blueprint->rasterCnt].usage = usage;
    ++blueprint->rasterCnt;
}

_AFXINL void AfxCanvasBlueprintAddExistingRaster(afxCanvasBlueprint *blueprint, afxTexture existing)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(existing);
    blueprint->rasters[blueprint->rasterCnt].tex = existing;
    blueprint->rasters[blueprint->rasterCnt].fmt = NIL;
    blueprint->rasters[blueprint->rasterCnt].usage = NIL;
    ++blueprint->rasterCnt;
}

_AFX afxError _AfxCanvFreeAllSurfaces(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(canv, AFX_FCC_CANV);

    if (canv->annexes)
    {
        for (afxNat i = 0; i < canv->annexCnt; i++)
        {
            afxTexture tex = canv->annexes[i].tex;

            if (tex)
            {
                AfxAssertObjects(1, &tex, AFX_FCC_TEX);
                AfxReleaseObjects(1, (void*[]) { tex });
                canv->annexes[i].tex = NIL;
            }
        }
    }
    return err;
}

_AFX void AfxFlagCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    canv->flags |= bitmask;
}

_AFX void AfxUnflagCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    canv->flags &= ~bitmask;
}

_AFX afxResult AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    return canv->flags & bitmask;
}

_AFX afxBool AfxGetAnnexedStencilSurface(afxCanvas canv, afxTexture *stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    afxTexture tex = canv->dsIdx[1] != AFX_INVALID_INDEX ? canv->annexes[canv->dsIdx[1]].tex : NIL;

    if (tex)
    {
        AfxAssertObjects(1, &tex, AFX_FCC_TEX);

        if (stencil)
            *stencil = tex;
    }
    return !!tex;
}

_AFX afxBool AfxGetAnnexedDepthSurface(afxCanvas canv, afxTexture *depth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    afxTexture tex = canv->dsIdx[0] != AFX_INVALID_INDEX ? canv->annexes[canv->dsIdx[0]].tex : NIL;

    if (tex)
    {
        AfxAssertObjects(1, &tex, AFX_FCC_TEX);

        if (depth)
            *depth = tex;
    }
    return !!tex;
}

_AFX afxBool AfxGetAnnexedRasterSurface(afxCanvas canv, afxNat rasIdx, afxTexture *raster)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    AfxAssert(rasIdx < canv->rasterCnt);

    afxBool rslt;
    afxTexture tex2;
    if ((rslt = !!(tex2 = canv->annexes[rasIdx].tex)))
    {
        AfxAssertObjects(1, &tex2, AFX_FCC_TEX);

        if (raster)
            *raster = tex2;
    }
    return rslt;
}

_AFX afxNat AfxGetAnnexedRasterSurfaceCount(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    return canv->rasterCnt;
}

_AFX afxBool AfxGetAnnexedSurface(afxCanvas canv, afxNat surfIdx, afxTexture *tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    AfxAssert(surfIdx < canv->annexCnt);
    
    afxBool rslt;
    afxTexture tex2;
    if ((rslt = !!(tex2 = canv->annexes[surfIdx].tex)))
    {
        AfxAssertObjects(1, &tex2, AFX_FCC_TEX);

        if (tex)
            *tex = tex2;
    }
    return rslt;
}

_AFX afxNat AfxGetAnnexedSurfaceCount(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    return canv->annexCnt;
}

_AFX void AfxGetCanvasExtent(afxCanvas canv, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, AFX_FCC_CANV);
    AfxAssert(extent);
    extent[0] = canv->whd[0];
    extent[1] = canv->whd[1];
    extent[2] = canv->whd[2];
}

_AFX afxError AfxBuildCanvases(afxDrawContext dctx, afxNat cnt, afxCanvas canv[], afxCanvasBlueprint const *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(canv);
    AfxEntry("dctx=%p,cnt=%u,blueprint=%p,canv=%p", blueprint->dctx, cnt, blueprint, canv);

    if (AfxAcquireObjects(&dctx->canvases, cnt, (afxHandle*)canv, (void*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
};
