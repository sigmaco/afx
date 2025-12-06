/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_CANVAS_C
#include "avxIcd.h"

_AVX afxDrawSystem AvxGetCanvasHost(avxCanvas canv)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxDrawSystem dsys = AfxGetHost(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX avxCanvasFlags AvxGetCanvasFlags(avxCanvas canv, avxCanvasFlags mask)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    return (!mask) ? canv->flags : (canv->flags & mask);
}

_AVX afxUnit AvxGetCanvasExtent(avxCanvas canv, avxOrigin const* origin, afxLayeredRect* extent)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    avxRange whd = canv->whd;
    avxOrigin origin2 = (avxOrigin) { 0 };

    if (origin)
    {
        origin2 = *origin;
        origin2 = AvxClampOrigin(origin2, AVX_RANGE(whd.w - 1, whd.h - 1, whd.d - 1));
        whd = AvxSubtractRange(whd, AVX_RANGE(origin2.x, origin2.y, origin2.z));
    }
    
    AFX_ASSERT(extent);
    *extent = AFX_LAYERED_RECT(origin2.x, origin2.y, whd.w, whd.h, origin2.z, whd.d);
    return whd.w * whd.h;
}

_AVX afxUnit AvxClipCanvas_(avxCanvas canv, afxRect const* rc, afxBool flipY, avxViewport* vp)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(rc);

    avxRange whd = canv->whd;
    afxRect rcClip =
    {
        .x = AFX_MIN(rc->x, (afxInt)whd.w - 1),
        .y = AFX_MIN(rc->y, (afxInt)whd.h - 1),
        .w = rc->w ? AFX_MIN(rc->w, (afxInt)whd.w - rcClip.x) : (afxInt)whd.w - rcClip.x,
        .h = rc->h ? AFX_MIN(rc->h, (afxInt)whd.h - rcClip.y) : (afxInt)whd.h - rcClip.y
    };

    if (flipY)
        AfxFlipRect(&rcClip, whd.h, &rcClip);

    vp->origin[0] = AFX_MIN(rc->x, (afxInt)whd.w - 1);
    vp->origin[1] = AFX_MIN(rc->y, (afxInt)whd.h - 1);
    vp->extent[0] = AFX_MAX(rc->w, AFX_MIN(rc->x, (afxInt)whd.w - 1));
    vp->extent[1] = AFX_MAX(rc->h, AFX_MIN(rc->y, (afxInt)whd.h - 1));
    vp->minDepth = 0;
    vp->maxDepth = 1;
    return (vp->extent[0] * vp->extent[1]);
}

_AVX afxUnit AvxClipCanvas2_(avxCanvas canv, afxRect const* rc, afxBool flipY, avxViewport* vp)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(rc);

    avxRange whd = canv->whd;
    afxRect rcClip =
    {
        .x = AFX_MIN(rc->x, (afxInt)whd.w - 1),
        .y = AFX_MIN(rc->y, (afxInt)whd.h - 1),
        .w = rc->w ? AFX_MIN(rc->w, whd.w - rcClip.x) : (afxInt)whd.w - rcClip.x,
        .h = rc->h ? AFX_MIN(rc->h, whd.h - rcClip.y) : (afxInt)whd.h - rcClip.y
    };

    if (flipY)
        AfxFlipRect(&rcClip, whd.h, &rcClip);

    vp->origin[0] = AFX_MIN(rc->x, (afxInt)whd.w - 1);
    vp->origin[1] = AFX_MIN(rc->y, (afxInt)whd.h - 1);
    vp->extent[0] = AFX_MAX(rc->w, AFX_MIN(rc->x, (afxInt)whd.w - 1));
    vp->extent[1] = AFX_MAX(rc->h, AFX_MIN(rc->y, (afxInt)whd.h - 1));
    vp->minDepth = 0;
    vp->maxDepth = 1;
    return (vp->extent[0] * vp->extent[1]);
}

_AVX avxRange AvxGetCanvasExtentNdc(avxCanvas canv, afxV2d const origin, afxV2d const whd)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxV2d at, ran;
    AfxV2dNdc(at, origin, AFX_V2D(canv->whd.w, canv->whd.h));
    AfxV2dNdc(ran, whd, AFX_V2D(canv->whd.w, canv->whd.h));
    return AVX_RANGE(ran[0], ran[1], canv->whd.d);
}

_AVX afxUnit AvxQueryCanvasBins(avxCanvas canv, afxUnit* colBinCnt, afxUnit* dBinIdx, afxUnit* sBinIdx)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    if (colBinCnt)
        *colBinCnt = canv->colorCnt;

    if (dBinIdx)
        *dBinIdx = canv->dsBinIdx[0];

    if (sBinIdx)
        *sBinIdx = canv->dsBinIdx[1];

    return canv->binCnt;
}

_AVX afxUnit AvxGetDrawBuffers(avxCanvas canv, afxUnit baseBinIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxUnit rslt = 0;

    AFX_ASSERT_RANGE(canv->binCnt, baseBinIdx, cnt);
    if (baseBinIdx >= canv->binCnt) return rslt;
    //baseBinIdx = AFX_MIN(baseBinIdx, canv->binCnt - 1);
    cnt = AFX_MIN(cnt, canv->binCnt - baseBinIdx);

    AFX_ASSERT(rasters);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit binIdx = baseBinIdx + i;
        avxRaster ras = canv->bins[binIdx].buf;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxUnit AvxGetColorBuffers(avxCanvas canv, afxUnit baseBinIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxUnit rslt = 0;

    AFX_ASSERT_RANGE(canv->colorCnt, baseBinIdx, cnt);
    if (baseBinIdx >= canv->colorCnt) return rslt;
    //baseBinIdx = AFX_MIN(baseBinIdx, canv->colorCnt - 1);
    cnt = AFX_MIN(cnt, canv->colorCnt - baseBinIdx);

    rslt = AvxGetDrawBuffers(canv, baseBinIdx, cnt, rasters);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, rslt, rasters);

    return rslt;
}

_AVX afxUnit AvxGetAuxBuffers(avxCanvas canv, avxRaster* depth, avxRaster* stencil)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(depth || stencil);
    afxBool rslt = 0;

    afxUnit dBinIdx = AFX_INVALID_INDEX;
    afxUnit sBinIdx = AFX_INVALID_INDEX;    
    if (!AvxQueryCanvasBins(canv, NIL, &dBinIdx, &sBinIdx))
        return rslt;

    if (depth)
    {
        avxRaster d = NIL;
        if ((dBinIdx != AFX_INVALID_INDEX) && AvxGetDrawBuffers(canv, dBinIdx, 1, &d))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &d);
            *depth = d;
            ++rslt;
        }
        else *depth = NIL;
    }

    if (stencil)
    {
        avxRaster s = NIL;
        if ((sBinIdx != AFX_INVALID_INDEX) && AvxGetDrawBuffers(canv, sBinIdx, 1, &s))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &s);
            *stencil = s;
            ++rslt;
        }
        else *stencil = NIL;
    }
    return rslt;
}

_AVX afxError _AvxCanvRelinkDrawBuffersCb_SW(avxCanvas canv, afxBool regen, afxUnit baseBin, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->binCnt, baseBin, cnt);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxDrawSystem dsys = AvxGetCanvasHost(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    for (afxUnit idx = 0; idx < cnt; idx++)
    {
        afxUnit binIdx = baseBin + idx;
        _avxCanvasBin* bin = &canv->bins[binIdx];
        avxRaster curr = bin->buf;
        avxRaster ras = rasters ? rasters[idx] : NIL;

        // If current is NIL, we must proceed to managed generation.
        if ((ras == curr) && (curr)) continue;

        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            bin->buf = NIL;
            --canv->linkedCnt;
        }
        else if (regen)
        {
            //canv->bins[binIdx].managed = TRUE;

            avxRasterInfo rasi = { 0 };
            rasi.usage = bin->usage | avxRasterUsage_DRAW;
            rasi.flags = bin->flags;
            rasi.fmt = bin->fmt;
            rasi.whd.w = AFX_ALIGN_SIZE(canv->whd.w, 16); // aligned to tile
            rasi.whd.h = AFX_ALIGN_SIZE(canv->whd.h, 16); // aligned to tile
            rasi.whd.d = canv->whd.d;

            if (1 < (rasi.lodCnt = bin->lodCnt))
                rasi.flags |= avxRasterFlag_MULTISAMP;

            if (AvxAcquireRasters(dsys, 1, &rasi, &ras))
            {
                AfxThrowError();
            }
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            }
        }

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            if (bin->usage != AvxGetRasterUsage(ras, bin->usage))
            {
                AfxThrowError();
                continue;
            }

            if (bin->fmt != AvxGetRasterFormat(ras))
            {
                AfxThrowError();
                continue;
            }

            avxRange ext = AvxGetRasterExtent(ras, 0);
            // Each buffer must be at least as large as the canvas dimensions.
            if ((ext.w < canv->whd.w) && (ext.h < canv->whd.h) && (ext.d < canv->whd.d))
            {
                AfxThrowError();
                continue;
            }

            if (!regen)
            {
                AfxReacquireObjects(1, &ras);
                //bin->managed = !!managed;
            }
            ++canv->linkedCnt;
        }
        bin->buf = ras;
    }
    return err;
}

_AVX afxError _AvxBindDrawBuffers(avxCanvas canv, afxUnit baseBin, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);
    AFX_ASSERT_RANGE(canv->binCnt, baseBin, cnt);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    if (!canv->ddi->relink)
    {
        // canv->pimpl->bind(canv, FASE, baseBin, cnt, rasters);
        if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, baseBin, cnt, rasters))
            AfxThrowError();
    }
    else if (canv->ddi->relink(canv, TRUE, baseBin, cnt, rasters))
        AfxThrowError();

    return err;
}

_AVXINL afxError _AvxBindDepthBuffer(avxCanvas canv, avxRaster depth)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &depth);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxUnit binIdx;
    if (!AvxQueryCanvasBins(canv, NIL, &binIdx, NIL) || (binIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxBindDrawBuffers(canv, binIdx, 1, &depth))
        AfxThrowError();

    return err;
}

_AVXINL afxError _AvxBindStencilBuffer(avxCanvas canv, avxRaster stencil)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &stencil);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxUnit binIdx;
    if (!AvxQueryCanvasBins(canv, NIL, NIL, &binIdx) || (binIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxBindDrawBuffers(canv, binIdx, 1, &stencil))
        AfxThrowError();

    return err;
}

_AVX afxError AvxPrintDrawBuffer(avxCanvas canv, afxUnit binIdx, avxRasterIo const* op, afxUri const* uri, afxMask exuMask)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->binCnt, binIdx, 1);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    avxRaster ras;
    if (!AvxGetDrawBuffers(canv, binIdx, 1, &ras))
    {
        AfxThrowError();
        return err;
    }

    if (op)
    {
        AFX_ASSERT_RANGE((afxInt)canv->whd.w, op->rgn.origin.x, 1);
        AFX_ASSERT_RANGE((afxInt)canv->whd.h, op->rgn.origin.y, 1);
        AFX_ASSERT_RANGE((afxInt)canv->whd.d, op->rgn.origin.z, 1);
        AFX_ASSERT2(canv->whd.w >= op->rgn.whd.w, op->rgn.whd.w);
        AFX_ASSERT2(canv->whd.h >= op->rgn.whd.h, op->rgn.whd.h);
        AFX_ASSERT2(canv->whd.d >= op->rgn.whd.d, op->rgn.whd.d);
            
        if (AvxPrintRaster(ras, op, 1, uri, exuMask))
            AfxThrowError();
    }
    else
    {
        afxLayeredRect bounds;
        avxRasterIo op2 = { 0 };
        AvxGetCanvasExtent(canv, NIL, &bounds);
        op2.rgn.origin = (avxOrigin) { bounds.area.x, bounds.area.y, bounds.baseLayer };
        op2.rgn.whd = (avxRange){ bounds.area.w, bounds.area.h, bounds.layerCnt };

        if (AvxPrintRaster(ras, &op2, 1, uri, exuMask))
            AfxThrowError();
    }
    return err;
}

#if 0
_AVX afxError _AvxRedoDrawBuffers(avxCanvas canv)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxDrawSystem dsys = AvxGetCanvasHost(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    avxRasterInfo rasi = { 0 };
    rasi.lodCnt = 1;
    rasi.whd = AvxGetCanvasExtent(canv, AVX_ORIGIN_ZERO);

    for (afxUnit i = 0; i < canv->binCnt; i++)
    {
        _avxCanvasBin* surf = &canv->bins[i];

        if (!surf->managed)
            continue;

        if (surf->ras)
            AfxBindDrawBuffers(canv, i, 1, NIL);

        rasi.fmt = surf->fmt;
        rasi.lodCnt = surf->sampleCnt;
        rasi.usage = surf->usage | avxRasterUsage_DRAW;
        avxRaster ras;

        if (AvxAcquireRasters(dsys, 1, &rasi, &ras))
        {
            AfxThrowError();
            continue;
        }

        if (!canv->relink)
        {
            if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, i, 1, &ras))
                AfxThrowError();
        }
        else if (canv->relink(canv, TRUE, i, 1, &ras))
            AfxThrowError();

        AfxDisposeObjects(1, &ras);
    }
    return err;
}
#endif

_AVX afxError _AvxCanvDtorCb(avxCanvas canv)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    for (afxUnit i = 0; i < canv->binCnt; i++)
    {
        _avxCanvasBin* bin = &canv->bins[i];        
        _AvxCanvRelinkDrawBuffersCb_SW(canv, FALSE, i, 1, NIL);
        AFX_ASSERT(!bin->buf);
#if 0
        avxRaster ras = bin->ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            AfxDisposeObjects(1, &ras);
            canv->bins[i].ras = NIL;
        }
#endif
    }

    afxObjectStash const stashs[] =
    {
        {
            .cnt = canv->binCnt,
            .siz = sizeof(canv->bins[0]),
            .var = (void**)&canv->bins
        }
    };
    AfxDeallocateInstanceData(canv, ARRAY_SIZE(stashs), stashs);

    return err;
}

_avxDdiCanv const _AVX_CANV_DDI =
{
    .relink = _AvxCanvRelinkDrawBuffersCb_SW,
};

_AVX afxError _AvxCanvCtorCb(avxCanvas canv, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    (void)invokeNo;

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxCanvasConfig const* cfg = args[1];
    AFX_ASSERT(cfg);

    //afxWarp const* whd = args[1];
    //afxUnit surCnt = *(afxUnit const *)args[2];
    //avxDrawBin const* surCfgs = args[3];

    if (!cfg->binCnt)
    {
        AfxThrowError();
        return err;
    }

    canv->tag = cfg->tag;
    canv->udd = cfg->udd;

    avxLimits const* limits = _AvxDsysGetLimits(dsys);
    // @whd must be less than or equal to maxCanvasWhd
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.w, cfg->whd.w);
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.h, cfg->whd.h);
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.d, cfg->whd.d);

    canv->whd.w = AFX_MAX(1, cfg->whd.w);
    canv->whd.h = AFX_MAX(1, cfg->whd.h);
    canv->whd.d = AFX_MAX(1, cfg->whd.d);
    canv->whdMin = canv->whd;
    canv->whdMax = (avxRange){ limits->maxCanvasWhd.w, limits->maxCanvasWhd.h, limits->maxCanvasWhd.d};

    AFX_ASSERT_CAPACITY(limits->canvasNoAttachmentsSampleCnts, cfg->lodCnt);
    canv->lodCnt = AFX_MAX(1, cfg->lodCnt);
    
    canv->ddi = &_AVX_CANV_DDI;

    canv->flags = cfg->flags;

    afxBool combinedDs = FALSE;
    afxUnit depthInIdx = AFX_INVALID_INDEX;
    afxUnit stencilInIdx = AFX_INVALID_INDEX;
    afxUnit binCnt = 0;
    afxUnit colorCnt = 0;

    for (afxUnit i = 0; i < cfg->binCnt; i++)
    {
        avxDrawBin const* sur = &cfg->bins[i];
        AFX_ASSERT(sur->fmt);
        AFX_ASSERT(sur->lodCnt);

        if (AvxTestDepthFormat(sur->fmt))
        {
            if (depthInIdx == AFX_INVALID_INDEX)
                ++binCnt;

            depthInIdx = i;

            if ((combinedDs = AvxTestCombinedDsFormat(sur->fmt)))
                stencilInIdx = depthInIdx;

            continue;
        }

        if (AvxTestStencilFormat(sur->fmt))
        {
            if (stencilInIdx == AFX_INVALID_INDEX)
                ++binCnt;

            stencilInIdx = i;
            continue;
        }

        ++colorCnt;
        ++binCnt;
    }

    if (!colorCnt)
        canv->flags |= avxCanvasFlag_COLOR;

    if (!binCnt)
    {
        canv->flags |= cfg->flags & ~(avxCanvasFlag_COLOR | avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_CODEST);
        canv->flags |= avxCanvasFlag_VOID;
    }

    canv->linkedCnt = 0;
    AFX_ASSERT_RANGE(limits->maxColorAttachments, 0, colorCnt);
    canv->colorCnt = colorCnt;
    canv->ownershipMask = NIL;

    if (stencilInIdx == AFX_INVALID_INDEX)
        canv->dsBinIdx[1] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= avxCanvasFlag_STENCIL;
        canv->dsBinIdx[1] = (binCnt - 1);
    }

    if (depthInIdx == AFX_INVALID_INDEX)
        canv->dsBinIdx[0] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= avxCanvasFlag_DEPTH;
        if (combinedDs)
        {
            canv->dsBinIdx[0] = canv->dsBinIdx[1];
            canv->flags |= avxCanvasFlag_CODEST;
        }
        else if (stencilInIdx != AFX_INVALID_INDEX) canv->dsBinIdx[0] = (canv->dsBinIdx[1] - 1);
        else canv->dsBinIdx[0] = (binCnt - 1);
    }

    afxObjectStash const stashs[] =
    {
        {
            .cnt = binCnt,
            .siz = sizeof(canv->bins[0]),
            .var = (void**)&canv->bins
        }
    };

    if (AfxAllocateInstanceData(canv, ARRAY_SIZE(stashs), stashs))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(canv->bins);
    canv->binCnt = binCnt;
    avxDrawBin const* binCfg;
    _avxCanvasBin* bin;

    for (afxUnit i = 0; i < binCnt; i++)
    {
        bin = &canv->bins[i];
        *bin = (_avxCanvasBin) { 0 };

        binCfg = &cfg->bins[i];

        if (i == canv->dsBinIdx[1])
            binCfg = &cfg->bins[stencilInIdx];

        if (i == canv->dsBinIdx[0])
            binCfg = &cfg->bins[depthInIdx];

        avxRaster ras = binCfg->buf;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            avxRasterInfo rasi;
            AvxDescribeRaster(ras, &rasi, NIL, NIL);

            bin->buf = ras;
            bin->managed = FALSE;
            bin->resolve = NIL;
            bin->fmt = rasi.fmt;
            bin->usage = rasi.usage;
            bin->flags = rasi.flags;
            bin->lodCnt = rasi.lodCnt;

            if (_AvxBindDrawBuffers(canv, i, 1, &ras))
            {
                AfxThrowError();
            }
            AFX_ASSERT(canv->bins[i].buf == ras); // relink must set it.
        }
        else
        {
            bin->buf = NIL;
            bin->managed = TRUE;
            bin->resolve = NIL;
            bin->fmt = binCfg->fmt;
            bin->usage = binCfg->usage | avxRasterUsage_DRAW;
            bin->flags = binCfg->flags;
            bin->lodCnt = binCfg->lodCnt;

            if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, i, 1, &ras))
            {
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxUnit j = i; j--> 0;)
            {
#if 0
                if ((ras = canv->bins[i].ras))
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                    AfxDisposeObjects(1, &ras);
                }
#else
                _AvxBindDrawBuffers(canv, j, 1, NIL);
#endif
            }
        }
    }

#if 0
    if (_AvxRedoDrawBuffers(canv))
        AfxThrowError();
#endif

    if (err)
    {
        AfxDeallocateInstanceData(canv, ARRAY_SIZE(stashs), stashs);
    }
    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_CANV =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .desc = "Buffered Drawing Canvas",
    .fixedSiz = sizeof(AFX_OBJECT(avxCanvas)),
    .ctor = (void*)_AvxCanvCtorCb,
    .dtor = (void*)_AvxCanvDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxConfigureCanvas(afxDrawSystem dsys, avxCanvasConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    /*
        A framebuffer in Vulkan is a collection of image views that are used as attachments in a render pass instance.
        Each framebuffer must match the render pass in:
            Number of attachments
            Format of each attachment
            Sample count of each attachment

        Sample count (VkSampleCountFlagBits) defines how many samples per pixel are used in the attachments, 
        enabling multisample anti-aliasing (MSAA). Common values: VK_SAMPLE_COUNT_1_BIT (no MSAA), 2, 4, 8, 16, depending on GPU support.

        Framebuffer Limitations with Sample Count
        All attachments must have the same sample count:
            Vulkan requires that all color, depth, and stencil attachments in a framebuffer must have the same sample count.
            You can't mix VK_SAMPLE_COUNT_1_BIT and VK_SAMPLE_COUNT_4_BIT attachments in the same framebuffer.

        Must match render pass sample count:
            If a render pass defines multisampled attachments, then the framebuffer must contain images with the same sample count.

        Hardware-dependent maximums:
            Use VkPhysicalDeviceProperties.limits.framebufferColorSampleCounts, framebufferDepthSampleCounts, etc., 
            to check what sample counts are supported.

        Resolve attachments required:
            If your render pass is multisampled, you typically need resolve attachments (single-sample images) 
            to copy the final color output for presentation.
    */

    avxCanvasConfig cfg2 = { 0 };
    cfg2.udd = cfg->udd;
    cfg2.tag = cfg->tag;

    avxLimits const* limits = _AvxDsysGetLimits(dsys);
    // @whd must be less than or equal to limits(maxCanvasWhd).
    cfg2.whd.w = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.w, cfg->whd.w));
    cfg2.whd.h = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.h, cfg->whd.h));
    cfg2.whd.d = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.d, cfg->whd.d));
    
    cfg2.lodCnt = AFX_MAX(1, AFX_MIN(limits->canvasNoAttachmentsSampleCnts, cfg->lodCnt));

    cfg2.binCnt = AFX_MIN(cfg->binCnt, limits->maxColorAttachments + 2);

    if (!cfg2.binCnt)
    {
        cfg2.flags |= cfg->flags & ~(avxCanvasFlag_COLOR | avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_CODEST);
        cfg2.flags |= avxCanvasFlag_VOID;
    }
    else
    {
        afxBool combinedDs = FALSE;
        afxUnit depthInIdx = AFX_INVALID_INDEX;
        afxUnit stencilInIdx = AFX_INVALID_INDEX;
        afxUnit binCnt = 0;
        afxUnit colorCnt = 0;

        afxBool expectsD = (cfg->flags & avxCanvasFlag_DEPTH);
        afxBool expectsS = (cfg->flags & avxCanvasFlag_STENCIL);
        afxBool expectsDS = (cfg->flags & avxCanvasFlag_CODEST);

        for (afxUnit i = 0; i < cfg->binCnt; i++)
        {
            avxDrawBin const* ac = &cfg->bins[i];
            avxFormat fmt = ac->fmt;
            avxRasterUsage usage = ac->usage;
            avxRasterFlags flags = ac->flags;
            avxRaster buf = ac->buf;
            afxUnit lodCnt = ac->lodCnt;

            if (!fmt)
            {
                if (!colorCnt)
                {
                    fmt = avxFormat_RGBA8un;
                }
            }
            else
            {
                if (AvxTestDepthFormat(fmt))
                {
                    if (depthInIdx == AFX_INVALID_INDEX)
                        ++binCnt;

                    depthInIdx = i;

                    if ((combinedDs = AvxTestCombinedDsFormat(fmt)))
                        stencilInIdx = depthInIdx;

                    continue;
                }

                if (AvxTestStencilFormat(fmt))
                {
                    if (stencilInIdx == AFX_INVALID_INDEX)
                        ++binCnt;

                    stencilInIdx = i;
                    continue;
                }
            }

            avxDrawBin* c = &cfg2.bins[colorCnt];
            c->lodCnt = AFX_MAX(lodCnt, cfg2.lodCnt);
            c->usage = avxRasterUsage_DRAW | usage;
            c->flags = flags;
            c->fmt = fmt;
            c->buf = buf;

            ++colorCnt;
            ++binCnt;
        }

        if (!binCnt) cfg2.flags |= avxCanvasFlag_VOID;
        else
        {
            if (colorCnt)
                cfg2.flags |= avxCanvasFlag_COLOR;

            if (stencilInIdx != AFX_INVALID_INDEX)
            {
                cfg2.flags |= avxCanvasFlag_STENCIL;

                if (!combinedDs)
                {
                    avxDrawBin* s = &cfg2.bins[(binCnt - 1)];
                    avxDrawBin const* sc = &cfg->bins[stencilInIdx];
                    *s = *sc;
                    s->buf = sc->buf;
                    s->fmt = sc->fmt;
                    s->flags = sc->flags;
                    s->usage = sc->usage | avxRasterUsage_DRAW;
                    s->lodCnt = AFX_MAX(1, sc->lodCnt);
                }
            }

            if (depthInIdx != AFX_INVALID_INDEX)
            {
                cfg2.flags |= avxCanvasFlag_DEPTH;

                if (!combinedDs)
                {
                    avxDrawBin* d = &cfg2.bins[colorCnt];
                    avxDrawBin const* dc = &cfg->bins[depthInIdx];
                    *d = *dc;
                    d->buf = dc->buf;
                    d->fmt = dc->fmt;
                    d->flags = dc->flags;
                    d->usage = dc->usage | avxRasterUsage_DRAW;
                    d->lodCnt = AFX_MAX(1, dc->lodCnt);
                }
                else
                {
                    cfg2.flags |= avxCanvasFlag_CODEST;

                    avxDrawBin* ds = &cfg2.bins[colorCnt];
                    avxDrawBin const* dsc = &cfg->bins[depthInIdx];
                    *ds = *dsc;
                    ds->buf = dsc->buf;
                    ds->fmt = dsc->fmt;
                    ds->flags = dsc->flags;
                    ds->usage = dsc->usage | avxRasterUsage_DRAW;
                    ds->lodCnt = AFX_MAX(1, dsc->lodCnt);

                    // How to nest separated D/S?
                }
            }

        }
    }

    for (afxUnit i = 0; i < AVX_MAX_CANVAS_BUFFERS; i++)
        cfg2.bins[i].usage |= avxRasterUsage_DRAW;

    *cfg = cfg2;
    return err;
}

_AVX afxError AvxAcquireCanvas(afxDrawSystem dsys, avxCanvasConfig const* cfg, afxUnit cnt, avxCanvas canvases[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(canvases);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->canvCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_CANV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)canvases, (void const*[]) { dsys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CANV, cnt, canvases);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxCanvas canv = canvases[i];

        //AFX_ASSERT(canv->binCnt >= cfg->surCnt);
        AFX_ASSERT(canv->whd.w >= cfg->whd.w);
        AFX_ASSERT(canv->whd.h >= cfg->whd.h);
        AFX_ASSERT(canv->whd.d >= cfg->whd.d);
        AFX_ASSERT((canv->flags & cfg->flags) == cfg->flags);
        AFX_ASSERT(canv->tag.start == cfg->tag.start);

        for (afxUnit j = 0; j < cfg->binCnt; j++)
        {
            _avxCanvasBin* surf = &canv->bins[j];

            // combined DS reduces one bin.
            if (j == canv->binCnt)
                break;

            if (!cfg->bins[j].buf)
            {
                AFX_ASSERT((surf->fmt == cfg->bins[j].fmt) || !(cfg->bins[j].fmt));
                AFX_ASSERT((surf->flags & cfg->bins[j].flags) == cfg->bins[j].flags);
                AFX_ASSERT((surf->usage & cfg->bins[j].usage) == cfg->bins[j].usage);
                AFX_ASSERT((surf->usage & avxRasterUsage_DRAW) == avxRasterUsage_DRAW);
                AFX_ASSERT(surf->lodCnt >= cfg->bins[j].lodCnt);
            }
            else
            {
                AFX_ASSERT(surf->buf == cfg->bins[j].buf);
            }
        }
    }
#endif
    return err;
}
