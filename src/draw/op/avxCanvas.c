/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_CANVAS_C
#include "../impl/avxImplementation.h"

_AVX afxResult AvxTestCanvas(avxCanvas canv, afxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    return canv->flags & bitmask;
}

_AVX avxRange AvxGetCanvasArea(avxCanvas canv, avxOrigin origin)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    origin = AvxClampOrigin(origin, AVX_RANGE(canv->extent.w - 1, canv->extent.h - 1, canv->extent.d - 1));
    avxRange whd = AvxSubtractRange(canv->extent, AVX_RANGE(origin.x, origin.y, origin.z));    
    return whd;
}

_AVX avxRange AvxGetCanvasExtentNdc(avxCanvas canv, afxV2d const origin, afxV2d const extent)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxV2d at, ran;
    AfxNdcV2d(at, origin, AFX_V2D(canv->extent.w, canv->extent.h));
    AfxNdcV2d(ran, extent, AFX_V2D(canv->extent.w, canv->extent.h));
    return AVX_RANGE(ran[0], ran[1], canv->extent.d);
}

_AVX afxUnit AvxQueryDrawBufferSlots(avxCanvas canv, afxUnit* colorSlotCnt, afxUnit* dSlotIdx, afxUnit* sSlotIdx)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    if (colorSlotCnt)
        *colorSlotCnt = canv->colorCnt;

    if (dSlotIdx)
        *dSlotIdx = canv->dsSlotIdx[0];

    if (sSlotIdx)
        *sSlotIdx = canv->dsSlotIdx[1];

    return canv->slotCnt;
}

_AVX afxUnit AvxGetDrawBuffers(avxCanvas canv, afxUnit baseSlotIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->slotCnt, baseSlotIdx, cnt);
    AFX_ASSERT(rasters);
    afxUnit rslt = 0;
    
    baseSlotIdx = AfxMin(baseSlotIdx, canv->slotCnt - 1);
    cnt = AfxMax(cnt, canv->slotCnt - baseSlotIdx);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit slotIdx = baseSlotIdx + i;
        avxRaster ras = canv->slots[slotIdx].ras;
        
        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxBool AvxGetColorBuffers(avxCanvas canv, afxUnit baseSlotIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->colorCnt, baseSlotIdx, cnt);

    baseSlotIdx = AfxMin(baseSlotIdx, canv->colorCnt - 1);
    cnt = AfxMax(cnt, canv->colorCnt - baseSlotIdx);

    afxBool rslt = AvxGetDrawBuffers(canv, baseSlotIdx, cnt, rasters);
    AFX_ASSERT_OBJECTS(afxFcc_RAS, rslt, rasters);
    return rslt;
}

_AVX afxBool AvxGetDepthBuffers(avxCanvas canv, avxRaster* depth, avxRaster* stencil)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(depth || stencil);
    afxBool rslt = 0;

    afxUnit dSlotIdx = AFX_INVALID_INDEX;
    afxUnit sSlotIdx = AFX_INVALID_INDEX;    
    if (AvxQueryDrawBufferSlots(canv, NIL, &dSlotIdx, &sSlotIdx) && 
        ((dSlotIdx != AFX_INVALID_INDEX) || (sSlotIdx != AFX_INVALID_INDEX)))
    {
        avxRaster ras = NIL;

        if (depth)
        {
            if (!AvxGetDrawBuffers(canv, dSlotIdx, 1, &ras)) *depth = NIL;
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                *depth = ras;
                ++rslt;
            }
        }

        if (stencil)
        {
            if (!AvxGetDrawBuffers(canv, sSlotIdx, 1, &ras)) *stencil = NIL;
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                *stencil = ras;
                ++rslt;
            }
        }
    }
    return rslt;
}

_AVX afxError _AvxRelinkDrawBuffersCb(avxCanvas canv, afxBool managed, afxUnit baseSlot, afxUnit cnt, avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->slotCnt, baseSlot, cnt);
    AFX_ASSERT(rasters);

    for (afxUnit idx = 0; idx < cnt; idx++)
    {
        afxUnit slotIdx = baseSlot + idx;
        avxRaster curr = canv->slots[slotIdx].ras;
        avxRaster ras = rasters ? rasters[idx] : NIL;

        if (ras == curr)
            continue;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            if (!AvxTestRasterUsage(ras, avxRasterUsage_DRAW))
            {
                AfxThrowError();
                continue;
            }

            if (canv->slots[slotIdx].fmt != AvxGetRasterFormat(ras))
            {
                AfxThrowError();
                continue;
            }

            avxRange ext = AvxGetRasterExtent(ras, 0);

            if ((ext.w < canv->extent.w) && (ext.h < canv->extent.h) && (ext.d < canv->extent.d))
            {
                AfxThrowError();
                continue;
            }

            AfxReacquireObjects(1, &ras);
            canv->slots[slotIdx].managed = !!managed;

            if (!curr)
                ++canv->linkedCnt;
        }

        canv->slots[slotIdx].ras = ras;

        if (!ras)
            canv->slots[slotIdx].managed = TRUE;

        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);

            if (!ras)
                --canv->linkedCnt;
        }
    }
    return err;
}

_AVX afxError _AvxBindDrawBuffers(avxCanvas canv, afxUnit baseSlot, afxUnit cnt, avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);
    AFX_ASSERT_RANGE(canv->slotCnt, baseSlot, cnt);
    AFX_ASSERT(rasters);

    if (!canv->relink)
    {
        if (_AvxRelinkDrawBuffersCb(canv, FALSE, baseSlot, cnt, rasters))
            AfxThrowError();
    }
    else if (canv->relink(canv, FALSE, baseSlot, cnt, rasters))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxBindDepthBuffer(avxCanvas canv, avxRaster depth)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &depth);
    
    afxUnit slotIdx;
    if (!AvxQueryDrawBufferSlots(canv, NIL, &slotIdx, NIL) || (slotIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxBindDrawBuffers(canv, slotIdx, 1, &depth))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxBindStencilBuffer(avxCanvas canv, avxRaster stencil)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &stencil);
    
    afxUnit slotIdx;
    if (!AvxQueryDrawBufferSlots(canv, NIL, NIL, &slotIdx) || (slotIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxBindDrawBuffers(canv, slotIdx, 1, &stencil))
        AfxThrowError();

    return err;
}

_AVX afxError AvxPrintDrawBuffer(avxCanvas canv, afxUnit slotIdx, avxRasterIo const* op, afxUnit exuIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->slotCnt, slotIdx, 1);
    
    avxRaster ras;
    if (!AvxGetDrawBuffers(canv, slotIdx, 1, &ras))
    {
        AfxThrowError();
        return err;
    }

    if (op)
    {
        AFX_ASSERT_RANGE(canv->extent.w, op->rgn.origin.x, 1);
        AFX_ASSERT_RANGE(canv->extent.h, op->rgn.origin.y, 1);
        AFX_ASSERT_RANGE(canv->extent.d, op->rgn.origin.z, 1);
        AFX_ASSERT2(canv->extent.w >= op->rgn.extent.w, op->rgn.extent.w);
        AFX_ASSERT2(canv->extent.h >= op->rgn.extent.h, op->rgn.extent.h);
        AFX_ASSERT2(canv->extent.d >= op->rgn.extent.d, op->rgn.extent.d);
            
        if (AvxPrintRaster(ras, op, 1, exuIdx, uri))
            AfxThrowError();
    }
    else
    {
        avxRasterIo op2 = { 0 };
        op2.rgn.extent = AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO);

        if (AvxPrintRaster(ras, &op2, 1, exuIdx, uri))
            AfxThrowError();
    }
    return err;
}

#if 0
_AVX afxError _AvxRedoDrawBuffers(avxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxDrawSystem dsys = AfxGetProvider(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    avxRasterInfo rasi = { 0 };
    rasi.lodCnt = 1;
    rasi.extent = AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO);

    for (afxUnit i = 0; i < canv->slotCnt; i++)
    {
        avxDrawSurfaceSlot* surf = &canv->slots[i];

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
            if (_AvxRelinkDrawBuffersCb(canv, TRUE, i, 1, &ras))
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
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    for (afxUnit i = 0; i < canv->slotCnt; i++)
    {
        avxRaster ras = canv->slots[i].ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            AfxDisposeObjects(1, &ras);
            canv->slots[i].ras = NIL;
        }
    }

    afxObjectStash const stashs[] =
    {
        {
            .cnt = canv->slotCnt,
            .siz = sizeof(canv->slots[0]),
            .var = (void**)&canv->slots
        }
    };
    AfxDeallocateInstanceData(canv, ARRAY_SIZE(stashs), stashs);

    return err;
}

_AVX afxError _AvxCanvCtorCb(avxCanvas canv, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    (void)invokeNo;

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxCanvasConfig const* cfg = args[1];
    AFX_ASSERT(cfg);

    //afxWhd const* whd = args[1];
    //afxUnit surCnt = *(afxUnit const *)args[2];
    //avxDrawSurface const* surCfgs = args[3];

    if (!cfg->surCnt)
    {
        AfxThrowError();
        return err;
    }

    afxDrawLimits const* limits = _AvxDsysAccessLimits(dsys);
    // @whd must be less than or equal to maxCanvasWhd
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.w, cfg->whd.w);
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.h, cfg->whd.h);
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.d, cfg->whd.d);

    canv->extent.w = AfxMax(1, cfg->whd.w);
    canv->extent.h = AfxMax(1, cfg->whd.h);
    canv->extent.d = AfxMax(1, cfg->whd.d);

    
    canv->readjust = NIL;// _SglReadjustCanvasCb;

    canv->flags = NIL;

    afxBool combinedDs = FALSE;
    afxUnit depthInIdx = AFX_INVALID_INDEX;
    afxUnit stencilInIdx = AFX_INVALID_INDEX;
    afxUnit slotCnt = 0;
    afxUnit colorCnt = 0;

    for (afxUnit i = 0; i < cfg->surCnt; i++)
    {
        avxDrawSurface const* sur = &cfg->surfs[i];
        AFX_ASSERT(sur->fmt);
        AFX_ASSERT(sur->sampleCnt);

        if (AfxIsPixelFormatDepth(sur->fmt))
        {
            if (depthInIdx == AFX_INVALID_INDEX)
                ++slotCnt;

            depthInIdx = i;

            if ((combinedDs = AfxIsPixelFormatCombinedDepthStencil(sur->fmt)))
                stencilInIdx = depthInIdx;

            continue;
        }

        if (AfxIsPixelFormatStencil(sur->fmt))
        {
            if (stencilInIdx == AFX_INVALID_INDEX)
                ++slotCnt;

            stencilInIdx = i;
            continue;
        }

        ++colorCnt;
        ++slotCnt;
    }

    canv->linkedCnt = 0;
    AFX_ASSERT_RANGE(limits->maxColorAttachments, 0, colorCnt);
    canv->colorCnt = colorCnt;
    canv->ownershipMask = NIL;
    canv->tag = cfg->tag;

    if (stencilInIdx == AFX_INVALID_INDEX)
        canv->dsSlotIdx[1] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= afxCanvasFlag_HAS_STENCIL;
        canv->dsSlotIdx[1] = (slotCnt - 1);
    }

    if (depthInIdx == AFX_INVALID_INDEX)
        canv->dsSlotIdx[0] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= afxCanvasFlag_HAS_DEPTH;
        if (combinedDs)
        {
            canv->dsSlotIdx[0] = canv->dsSlotIdx[1];
            canv->flags |= afxCanvasFlag_COMBINED_DS;
        }
        else if (stencilInIdx != AFX_INVALID_INDEX) canv->dsSlotIdx[0] = (canv->dsSlotIdx[1] - 1);
        else canv->dsSlotIdx[0] = (slotCnt - 1);
    }

    afxObjectStash const stashs[] =
    {
        {
            .cnt = slotCnt,
            .siz = sizeof(canv->slots[0]),
            .var = (void**)&canv->slots
        }
    };

    if (AfxAllocateInstanceData(canv, ARRAY_SIZE(stashs), stashs))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(canv->slots);
    canv->slotCnt = slotCnt;
    avxDrawSurface const* surCfg;
    avxDrawSurfaceSlot* surf;

    for (afxUnit i = 0; i < slotCnt; i++)
    {
        surf = &canv->slots[i];
        *surf = (avxDrawSurfaceSlot) { 0 };

        surCfg = &cfg->surfs[i];

        if (i == canv->dsSlotIdx[1])
            surCfg = &cfg->surfs[stencilInIdx];

        if (i == canv->dsSlotIdx[0])
            surCfg = &cfg->surfs[depthInIdx];

        avxRaster ras = surCfg->ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            avxRasterInfo rasi;
            AvxDescribeRaster(ras, &rasi);

            surf->ras = ras;
            surf->managed = FALSE;
            surf->resolve = NIL;
            surf->fmt = rasi.fmt;
            surf->usage = rasi.usage;
            surf->flags = rasi.flags;
            surf->sampleCnt = rasi.lodCnt;

            if (_AvxBindDrawBuffers(canv, i, 1, &ras))
            {
                AfxThrowError();
            }
            AFX_ASSERT(canv->slots[i].ras == ras); // relink must set it.
        }
        else
        {
            surf->ras = NIL;
            surf->managed = TRUE;
            surf->resolve = NIL;
            surf->fmt = cfg->surfs[i].fmt;
            surf->usage = cfg->surfs[i].rasUsage | avxRasterUsage_DRAW;
            surf->flags = cfg->surfs[i].rasFlags;
            surf->sampleCnt = cfg->surfs[i].sampleCnt;

            avxRasterInfo rasi = { 0 };
            rasi.usage = surf->usage | avxRasterUsage_DRAW;
            rasi.flags = surf->flags;
            rasi.fmt = surf->fmt;
            rasi.extent.w = AFX_ALIGNED_SIZE(canv->extent.w, 16); // aligned to tile
            rasi.extent.h = AFX_ALIGNED_SIZE(canv->extent.h, 16); // aligned to tile
            rasi.extent.d = canv->extent.d;
            
            if (1 < (rasi.lodCnt = surf->sampleCnt))
                rasi.flags |= avxRasterFlag_UPSAMPLED;

            if (AvxAcquireRasters(dsys, 1, &rasi, &ras))
            {
                AfxThrowError();
            }
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

                if (_AvxBindDrawBuffers(canv, i, 1, &ras))
                {
                    AfxThrowError();
                }

                AFX_ASSERT(canv->slots[i].ras == ras); // relink must set it.
                AfxDisposeObjects(1, &ras);
            }
        }

        if (err)
        {
            for (afxUnit j = i; j--> 0;)
            {
#if 0
                if ((ras = canv->slots[i].ras))
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

_AVX afxClassConfig const _AVX_CANV_CLASS_CONFIG =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .desc = "Buffered Drawing Canvas",
    .fixedSiz = sizeof(AFX_OBJECT(avxCanvas)),
    .ctor = (void*)_AvxCanvCtorCb,
    .dtor = (void*)_AvxCanvDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxCoacquireCanvas(afxDrawSystem dsys, avxCanvasConfig const* cfg, afxUnit cnt, avxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(canvases);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->canvCls(dsys);
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

        //AFX_ASSERT(canv->slotCnt >= cfg->surCnt);
        AFX_ASSERT(canv->extent.w >= cfg->whd.w);
        AFX_ASSERT(canv->extent.h >= cfg->whd.h);
        AFX_ASSERT(canv->extent.d >= cfg->whd.d);
        AFX_ASSERT((canv->flags & cfg->flags) == cfg->flags);
        AFX_ASSERT(canv->tag.start == cfg->tag.start);

        for (afxUnit j = 0; j < cfg->surCnt; j++)
        {
            avxDrawSurfaceSlot* surf = &canv->slots[j];

            // combined DS reduces one slot.
            if (j == canv->slotCnt)
                break;

            if (!cfg->surfs[j].ras)
            {
                AFX_ASSERT((surf->fmt == cfg->surfs[j].fmt) || !(cfg->surfs[j].fmt));
                AFX_ASSERT((surf->flags & cfg->surfs[j].rasFlags) == cfg->surfs[j].rasFlags);
                AFX_ASSERT((surf->usage & cfg->surfs[j].rasUsage) == cfg->surfs[j].rasUsage);
                AFX_ASSERT((surf->usage & avxRasterUsage_DRAW) == avxRasterUsage_DRAW);
                AFX_ASSERT(surf->sampleCnt >= cfg->surfs[j].sampleCnt);
            }
            else
            {
                AFX_ASSERT(surf->ras == cfg->surfs[j].ras);
            }
        }
    }
#endif
    return err;
}
