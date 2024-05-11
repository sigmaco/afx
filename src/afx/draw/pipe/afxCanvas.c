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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_CANVAS_C
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

_AVX afxBool AfxEnumerateDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);
    afxBool rslt = 0;
    
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

_AVX afxBool AfxGetColorBuffer(afxCanvas canv, afxNat surIdx, afxRaster* buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->colorCnt, surIdx, 1);
    afxRaster ras = NIL;

    if (canv->colorCnt > surIdx)
    {
        if (AfxEnumerateDrawBuffers(canv, surIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    AfxAssert(buf);
    *buf = ras;
    return !!ras;
}

_AVX afxBool AfxGetDepthBuffer(afxCanvas canv, afxRaster* buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxRaster ras = NIL;
    afxNat bufIdx;

    if (AfxGetDepthSurface(canv, &bufIdx))
    {
        if (AfxEnumerateDrawBuffers(canv, bufIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    AfxAssert(buf);
    *buf = ras;
    return !!ras;
}

_AVX afxBool AfxGetStencilBuffer(afxCanvas canv, afxRaster* buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    afxRaster ras = NIL;
    afxNat bufIdx;

    if (AfxGetStencilSurface(canv, &bufIdx))
    {
        if (AfxEnumerateDrawBuffers(canv, bufIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
        }
    }
    AfxAssert(buf);
    *buf = ras;
    return !!ras;
}

_AVX afxError _AvxRelinkDrawBuffersCb(afxCanvas canv, afxBool managed, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);

    for (afxNat i = 0; i < surfCnt; i++)
    {
        afxNat surfIdx = baseSurf + i;
        afxRaster curr = canv->surfaces[surfIdx].ras;
        afxRaster ras = rasters ? rasters[i] : NIL;

        if (ras != curr)
        {
            if (!ras)
            {
                canv->surfaces[surfIdx].managed = TRUE;
            }
            else
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);

                if (!AfxTestRasterUsage(ras, afxRasterUsage_DRAW)) AfxThrowError();
                else
                {
                    if (canv->surfaces[surfIdx].fmt != AfxGetRasterFormat(ras)) AfxThrowError();
                    else
                    {
                        afxWhd ext;
                        AfxGetRasterExtent(ras, 0, ext);
                        afxNat layerCnt = AfxCountRasterLayers(ras);

                        if ((ext[0] < canv->wh[0]) && (ext[1] < canv->wh[1]) && (layerCnt < canv->layerCnt)) AfxThrowError();
                        else
                        {
                            AfxReacquireObjects(1, (void*[]) { ras });
                            canv->surfaces[surfIdx].managed = !!managed;

                            if (!curr)
                                ++canv->linkedCnt;
                        }
                    }
                }
            }

            canv->surfaces[surfIdx].ras = ras;

            if (curr)
            {
                AfxAssertObjects(1, &curr, afxFcc_RAS);
                AfxReleaseObjects(1, &curr);

                if (!ras)
                    --canv->linkedCnt;
            }
        }
    }
    return err;
}

_AVX afxError AfxRelinkDrawBuffers(afxCanvas canv, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxTryAssertObjects(surfCnt, rasters, afxFcc_RAS);
    AfxAssertRange(canv->surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);

    if (!canv->relink)
    {
        if (_AvxRelinkDrawBuffersCb(canv, FALSE, baseSurf, surfCnt, rasters))
            AfxThrowError();
    }
    else if (canv->relink(canv, FALSE, baseSurf, surfCnt, rasters))
        AfxThrowError();

    return err;
}

_AVX afxError AfxRedoDrawBuffers(afxCanvas canv)
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
                if (!canv->relink)
                {
                    if (_AvxRelinkDrawBuffersCb(canv, TRUE, i, 1, &ras))
                        AfxThrowError();
                }
                else if (canv->relink(canv, TRUE, i, 1, &ras))
                    AfxThrowError();

                AfxReleaseObjects(1, &ras);
            }
        }
    }
    return err;
}

_AVX afxError AfxReadjustCanvas(afxCanvas canv, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssert(whd);

    for (afxNat i = 0; i < canv->surfaceCnt; i++)
    {
        afxSurface* surf = &canv->surfaces[i];

        if (!surf->managed)
        {
            afxWhd rasWhd;
            AfxAssert(surf->ras);
            AfxGetRasterExtent(surf->ras, 0, rasWhd);

            if ((whd[0] > rasWhd[0]) ||
                (whd[1] > rasWhd[1]) ||
                (whd[2] > rasWhd[2])
                )
            {
                AfxThrowError();
            }
        }
    }
    AfxThrowError(); // incomplete
    return err;
}

_AVX afxError AfxPrintDrawBuffer(afxCanvas canv, afxNat surIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->surfaceCnt, surIdx, 1);
    afxRaster ras;

    if (!AfxEnumerateDrawBuffers(canv, surIdx, 1, &ras)) AfxThrowError();
    else
    {
        afxRasterCopyOp op = { 0 };
        op.dst.layerCnt = canv->layerCnt;
        AfxGetCanvasExtent(canv, op.dst.whd);

        if (AfxPrintRasterToTarga(ras, &op, uri))
            AfxThrowError();
    }
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

_AVX afxError _AvxCanvStdDtor(afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    for (afxNat i = 0; i < canv->surfaceCnt; i++)
    {
        afxRaster ras = canv->surfaces[i].ras;

        if (ras)
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            AfxReleaseObjects(1, &ras);
            canv->surfaces[i].ras = NIL;
        }
    }
    AfxDeallocate(canv->surfaces);

    return err;
}

_AVX afxError _AvxCanvStdCtor(afxCanvas canv, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = cookie->udd[0];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat const* wh = cookie->udd[1];
    afxNat layerCnt = *(afxNat const*)cookie->udd[2];
    afxNat surCnt = *(afxNat const *)cookie->udd[3];
    afxSurfaceConfig const* surCfgs = cookie->udd[4];

    canv->wh[0] = wh[0];
    canv->wh[1] = wh[1];
    canv->layerCnt = layerCnt;

    canv->readjust = NIL;// _SglReadjustCanvasCb;
    
    afxBool combinedDs = FALSE;
    afxNat depthInIdx = AFX_INVALID_INDEX;
    afxNat stencilInIdx = AFX_INVALID_INDEX;
    afxNat surfaceCnt = 0;
    afxNat colorCnt = 0;
    afxRaster colorRasters[8];

    for (afxNat i = 0; i < surCnt; i++)
    {
        afxSurfaceConfig const* sur = &surCfgs[i];
        AfxAssert(sur->fmt);
        AfxAssert(sur->sampleCnt);

        if (depthInIdx == AFX_INVALID_INDEX)
        {
            if (AfxPixelFormatIsDepth(sur->fmt))
            {
                depthInIdx = i;

                if ((combinedDs = AfxPixelFormatIsCombinedDepthStencil(sur->fmt)))
                    stencilInIdx = depthInIdx;

                ++surfaceCnt;
                continue;
            }
        }

        if (!combinedDs && (stencilInIdx == AFX_INVALID_INDEX))
        {
            if (AfxPixelFormatIsStencil(sur->fmt))
            {
                stencilInIdx = i;
                ++surfaceCnt;
                continue;
            }
        }

        ++colorCnt;
        ++surfaceCnt;
    }

    canv->linkedCnt = 0;
    canv->colorCnt = colorCnt;
    canv->ownershipMask = NIL;
    canv->combinedDs = combinedDs;

    if (!(canv->surfaces = AfxAllocate(surfaceCnt, sizeof(canv->surfaces[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxSurface* surf;

        for (afxNat i = 0; i < surfaceCnt; i++)
        {
            surf = &canv->surfaces[i];
            surf->ras = NIL;
            surf->resolve = NIL;
            surf->managed = TRUE;
            surf->fmt = surCfgs[i].fmt;
            surf->usage = surCfgs[i].rasUsage | afxRasterUsage_DRAW;
            surf->flags = surCfgs[i].rasFlags;
            surf->sampleCnt = surCfgs[i].sampleCnt;
        }

        canv->surfaceCnt = surfaceCnt;

        if (stencilInIdx == AFX_INVALID_INDEX) canv->dsIdx[1] = AFX_INVALID_INDEX;
        else
        {
            canv->dsIdx[1] = surfaceCnt - 1;

            surf = &canv->surfaces[canv->dsIdx[1]];
            surf->ras = NIL;
            surf->resolve = NIL;
            surf->managed = TRUE;
            surf->fmt = surCfgs[stencilInIdx].fmt;
            surf->usage = surCfgs[stencilInIdx].rasUsage | afxRasterUsage_DRAW;
            surf->flags = surCfgs[stencilInIdx].rasFlags;
            surf->sampleCnt = surCfgs[stencilInIdx].sampleCnt;
        }

        if (depthInIdx == AFX_INVALID_INDEX) canv->dsIdx[0] = AFX_INVALID_INDEX;
        else
        {
            if (combinedDs)
                canv->dsIdx[0] = canv->dsIdx[1];
            else if (stencilInIdx != AFX_INVALID_INDEX)
                canv->dsIdx[0] = canv->dsIdx[1] - 1;
            else
                canv->dsIdx[0] = surfaceCnt - 1;

            surf = &canv->surfaces[canv->dsIdx[0]];
            surf->ras = NIL;
            surf->resolve = NIL;
            surf->managed = TRUE;
            surf->fmt = surCfgs[depthInIdx].fmt;
            surf->usage = surCfgs[depthInIdx].rasUsage | afxRasterUsage_DRAW;
            surf->flags = surCfgs[depthInIdx].rasFlags;
            surf->sampleCnt = surCfgs[depthInIdx].sampleCnt;
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxCanvStdImplementation =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .desc = "Drawable Canvas",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxCanvas)),
    .mmu = NIL,
    .ctor = (void*)_AvxCanvStdCtor,
    .dtor = (void*)_AvxCanvStdDtor
};

////////////////////////////////////////////////////////////////////////////////

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
