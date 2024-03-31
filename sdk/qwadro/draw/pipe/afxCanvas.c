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

#define _AFX_DRAW_C
#define _AFX_CANVAS_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxResult AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->flags & bitmask;
}

_AVX afxBool AfxHasCombinedDepthStencilBuffer(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->combinedDs;
}

_AVX afxNat AfxCountSurfaces(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->surfaceCnt;
}

_AVX afxNat AfxCountDrawSurfaces(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->colorCnt;
}

_AVX void AfxGetCanvasExtent(afxCanvas canv, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(whd);
    whd[0] = canv->wh[0];
    whd[1] = canv->wh[1];
    whd[2] = 1; //canv->layerCnt;
}

_AVX afxBool AfxGetDepthSurface(afxCanvas canv, afxNat* surfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxNat surfIdx2 = canv->dsIdx[0];
    AfxAssert(surfIdx);
    *surfIdx = surfIdx2;
    return (surfIdx2 != AFX_INVALID_INDEX);
}

_AVX afxBool AfxGetStencilSurface(afxCanvas canv, afxNat* surfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxNat surfIdx2 = canv->dsIdx[1];
    AfxAssert(surfIdx);
    *surfIdx = surfIdx2;
    return (surfIdx2 != AFX_INVALID_INDEX);
}

_AVX afxNat AfxEnumerateDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);
    afxNat rslt = 0;
    
    for (afxNat i = 0; i < surfCnt; i++)
    {
        afxNat surfIdx = baseSurf + i;
        afxRaster ras = canv->surfaces[surfIdx].ras;

        if (ras)
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxRaster AfxGetDepthBuffer(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxRaster ras = NIL;
    afxNat surfIdx;

    if (AfxGetDepthSurface(canv, &surfIdx))
    {
        if (AfxEnumerateDrawBuffers(canv, surfIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    return ras;
}

_AVX afxRaster AfxGetStencilBuffer(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxRaster ras = NIL;
    afxNat surfIdx;

    if (AfxGetStencilSurface(canv, &surfIdx))
    {
        if (AfxEnumerateDrawBuffers(canv, surfIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    return ras;
}

_AVX afxError AfxReadjustCanvas(afxCanvas canv, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(whd);

    if (canv->readjust(canv, whd))
        AfxThrowError();

    return err;
}

_AVX afxError AfxRelinkDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(surfCnt, rasters, afxFcc_RAS);
    AfxAssertRange(canv->surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);
    
    if (canv->relink(canv, FALSE, baseSurf, surfCnt, rasters))
        AfxThrowError();

    return err;
}

_AVX afxError AfxRelinkDepthBuffer(afxCanvas canv, afxRaster depth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(1, &depth, afxFcc_RAS);
    afxNat surfIdx;

    if (!AfxGetDepthSurface(canv, &surfIdx)) AfxThrowError();
    else
    {
        if (AfxRelinkDrawBuffers(canv, surfIdx, 1, &depth))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxRelinkStencilBuffer(afxCanvas canv, afxRaster stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(1, &stencil, afxFcc_RAS);
    afxNat surfIdx;

    if (!AfxGetStencilSurface(canv, &surfIdx)) AfxThrowError();
    else
    {
        if (AfxRelinkDrawBuffers(canv, surfIdx, 1, &stencil))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxRevalidateDrawBuffers(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    afxDrawContext dctx = AfxGetObjectProvider(canv);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    afxRasterInfo texi = { 0 };
    texi.lodCnt = 1;
    texi.layerCnt = canv->layerCnt;
    texi.whd[0] = canv->wh[0];
    texi.whd[1] = canv->wh[1];
    texi.whd[2] = 1;

    for (afxNat i = 0; i < canv->surfaceCnt; i++)
    {
        afxSurface* surf = &canv->surfaces[i];

        if (surf->managed)
        {
            if (surf->ras)
                AfxRelinkDrawBuffers(canv, i, 1, NIL);

            texi.fmt = surf->fmt;
            texi.sampleCnt = surf->sampleCnt;
            texi.usage = surf->usage | afxRasterUsage_DRAW;
            afxRaster ras;

            if (AfxAcquireRasters(dctx, 1, &texi, &ras)) AfxThrowError();
            {
                if (canv->relink(canv, TRUE, i, 1, &ras))
                    AfxThrowError();

                AfxReleaseObjects(1, (void*[]) { ras });
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(canvases);
    AfxAssert(cnt);
    afxManager* cls = AfxGetCanvasClass(dctx);
    AfxAssertClass(cls, afxFcc_CANV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)canvases);
}

_AVX afxError AfxAcquireCanvases(afxDrawContext dctx, afxWhd const whd, afxNat layerCnt, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, afxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(canvases);
    AfxAssert(surCnt);
    AfxAssert(surCfg);
    AfxAssert(whd);
    AfxAssert(whd[0]);
    AfxAssert(whd[1]);

    afxManager* cls = AfxGetCanvasClass(dctx);
    AfxAssertClass(cls, afxFcc_CANV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)canvases, (void const*[]) { dctx, whd, &layerCnt, &surCnt, surCfg }))
        AfxThrowError();

    return err;
}
