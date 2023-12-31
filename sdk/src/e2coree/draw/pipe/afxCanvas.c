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

#define _AFX_DRAW_C
#define _AFX_CANVAS_C
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/afxDrawContext.h"

_AFX afxResult AfxTestCanvas(afxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->flags & bitmask;
}

_AFX afxBool AfxHasCombinedDepthStencilBuffer(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->combinedDs;
}

_AFX afxNat AfxCountSurfaces(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->surfaceCnt;
}

_AFX afxNat AfxCountDrawSurfaces(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    return canv->colorCnt;
}

_AFX void AfxGetCanvasExtent(afxCanvas canv, afxNat wh[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(wh);
    wh[0] = canv->wh[0];
    wh[1] = canv->wh[1];
    //wh[2] = canv->layerCnt;
}

_AFX afxBool AfxGetDepthSurface(afxCanvas canv, afxNat* surfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxBool rslt = (AFX_INVALID_INDEX != canv->dsIdx[0]);

    if (surfIdx)
        *surfIdx = canv->dsIdx[0];
    
    return rslt;
}

_AFX afxBool AfxGetStencilSurface(afxCanvas canv, afxNat* surfIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxBool rslt = (AFX_INVALID_INDEX != canv->dsIdx[1]);

    if (surfIdx)
        *surfIdx = canv->dsIdx[1];

    return rslt;
}

_AFX afxNat AfxGetDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);
    afxNat rslt = 0;

    for (afxNat i = 0; i < surfCnt; i++)
    {
        afxNat slotIdx = baseSurf + i;
        afxRaster ras = canv->surfaces[slotIdx].ras;

        if (ras)
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AFX afxRaster AfxGetLinkedDepthBuffer(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxRaster ras = NIL;
    afxNat surfIdx;

    if (AfxGetDepthSurface(canv, &surfIdx))
    {
        if (AfxGetDrawBuffers(canv, surfIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    return ras;
}

_AFX afxRaster AfxGetLinkedStencilBuffer(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxRaster ras = NIL;
    afxNat surfIdx;

    if (AfxGetStencilSurface(canv, &surfIdx))
    {
        if (AfxGetDrawBuffers(canv, surfIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    return ras;
}

_AFX afxError AfxReadjustCanvas(afxCanvas canv, afxNat const wh[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(wh);

    if (canv->readjust(canv, wh))
        AfxThrowError();

    return err;
}

_AFX afxError AfxRelinkDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
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

_AFX afxError AfxRelinkDepthBuffer(afxCanvas canv, afxRaster depth)
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

_AFX afxError AfxRelinkStencilBuffer(afxCanvas canv, afxRaster stencil)
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

_AFX afxError AfxGenerateDrawBuffers(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxDrawContext dctx = AfxGetObjectProvider(canv);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    for (afxNat i = 0; i < canv->surfaceCnt; i++)
    {
        afxSurface* sur = &canv->surfaces[i];

        if (sur->managed)
        {
            if (sur->ras)
                AfxRelinkDrawBuffers(canv, i, 1, NIL);

            afxRasterInfo texi = { 0 };
            texi.whd[0] = canv->wh[0];
            texi.whd[1] = canv->wh[1];
            texi.whd[2] = 1;
            texi.fmt = sur->fmt;
            texi.lodCnt = 1;
            texi.sampleCnt = sur->sampleCnt;
            texi.usage = sur->usage | afxRasterUsage_DRAW;
            texi.layerCnt = canv->layerCnt;
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

_AFX afxError AfxAcquireCanvases(afxDrawContext dctx, afxNat const wh[2], afxNat layerCnt, afxNat surCnt, afxSurfaceConfig const surCfg[], afxNat cnt, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(surCnt);
    AfxAssert(surCfg);
    AfxAssert(canv);
    AfxAssert(wh);
    AfxAssert(wh[0]);
    AfxAssert(wh[1]);
    AfxEntry("dctx=%p,wh=[%u,%u],layerCnt=%u,surCnt=%p", dctx, wh[0], wh[1], layerCnt, surCnt);

    afxClass* cls = AfxGetCanvasClass(dctx);
    AfxAssertClass(cls, afxFcc_CANV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)canv, (void const*[]) { dctx, wh, &layerCnt, &surCnt, surCfg }))
        AfxThrowError();

    return err;
}
