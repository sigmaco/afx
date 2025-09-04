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
#include "ddi/avxImplementation.h"

_AVX avxCanvasFlags AvxGetCanvasFlags(avxCanvas canv, avxCanvasFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    return bitmask ? (canv->flags & bitmask) : canv->flags;
}

_AVX afxUnit AvxGetCanvasArea(avxCanvas canv, avxOrigin origin, afxRect* area)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    origin = AvxClampOrigin(origin, AVX_RANGE(canv->whd.w - 1, canv->whd.h - 1, canv->whd.d - 1));
    avxRange whd = AvxSubtractRange(canv->whd, AVX_RANGE(origin.x, origin.y, origin.z));
    AFX_ASSERT(area);
    *area = AFX_RECT(origin.x, origin.y, whd.w, whd.h);
    return whd.w * whd.h;
}

_AVX afxUnit AvxClipCanvas_(avxCanvas canv, afxRect const* rc, afxBool flipY, avxViewport* vp)
{
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxV2d at, ran;
    AfxV2dNdc(at, origin, AFX_V2D(canv->whd.w, canv->whd.h));
    AfxV2dNdc(ran, whd, AFX_V2D(canv->whd.w, canv->whd.h));
    return AVX_RANGE(ran[0], ran[1], canv->whd.d);
}

_AVX afxUnit AvxQueryCanvasSlots(avxCanvas canv, afxUnit* colorSlotCnt, afxUnit* dSlotIdx, afxUnit* sSlotIdx)
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
    afxUnit rslt = 0;

    AFX_ASSERT_RANGE(canv->slotCnt, baseSlotIdx, cnt);
    if (baseSlotIdx >= canv->slotCnt)
        return rslt;

    baseSlotIdx = AFX_MIN(baseSlotIdx, canv->slotCnt - 1);
    cnt = AFX_MAX(cnt, canv->slotCnt - baseSlotIdx);
    AFX_ASSERT(rasters);

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

_AVX afxUnit AvxGetColorBuffers(avxCanvas canv, afxUnit baseSlotIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxUnit rslt = 0;

    AFX_ASSERT_RANGE(canv->colorCnt, baseSlotIdx, cnt);
    if (baseSlotIdx >= canv->colorCnt)
        return rslt;

    baseSlotIdx = AFX_MIN(baseSlotIdx, canv->colorCnt - 1);
    cnt = AFX_MAX(cnt, canv->colorCnt - baseSlotIdx);

    rslt = AvxGetDrawBuffers(canv, baseSlotIdx, cnt, rasters);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, rslt, rasters);

    return rslt;
}

_AVX afxUnit AvxGetAuxBuffers(avxCanvas canv, avxRaster* depth, avxRaster* stencil)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(depth || stencil);
    afxBool rslt = 0;

    afxUnit dSlotIdx = AFX_INVALID_INDEX;
    afxUnit sSlotIdx = AFX_INVALID_INDEX;    
    if (!AvxQueryCanvasSlots(canv, NIL, &dSlotIdx, &sSlotIdx))
        return rslt;

    if (depth)
    {
        avxRaster d = NIL;
        if ((dSlotIdx != AFX_INVALID_INDEX) && AvxGetDrawBuffers(canv, dSlotIdx, 1, &d))
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
        if ((sSlotIdx != AFX_INVALID_INDEX) && AvxGetDrawBuffers(canv, sSlotIdx, 1, &s))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &s);
            *stencil = s;
            ++rslt;
        }
        else *stencil = NIL;
    }
    return rslt;
}

_AVX afxError _AvxCanvRelinkDrawBuffersCb_SW(avxCanvas canv, afxBool regen, afxUnit baseSlot, afxUnit cnt, avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->slotCnt, baseSlot, cnt);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxDrawSystem dsys = AfxGetProvider(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    for (afxUnit idx = 0; idx < cnt; idx++)
    {
        afxUnit slotIdx = baseSlot + idx;
        _avxCanvasUnit* slot = &canv->slots[slotIdx];
        avxRaster curr = slot->ras;
        avxRaster ras = rasters ? rasters[idx] : NIL;

        // If current is NIL, we must proceed to managed generation.
        if ((ras == curr) && (curr)) continue;

        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            slot->ras = NIL;
            --canv->linkedCnt;
        }
        else if (regen)
        {
            //canv->slots[slotIdx].managed = TRUE;

            avxRasterInfo rasi = { 0 };
            rasi.usage = slot->usage | avxRasterUsage_DRAW;
            rasi.flags = slot->flags;
            rasi.fmt = slot->fmt;
            rasi.whd.w = AFX_ALIGN_SIZE(canv->whd.w, 16); // aligned to tile
            rasi.whd.h = AFX_ALIGN_SIZE(canv->whd.h, 16); // aligned to tile
            rasi.whd.d = canv->whd.d;

            if (1 < (rasi.lodCnt = slot->sampleCnt))
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

            if (slot->usage != AvxTestRasterUsage(ras, slot->usage))
            {
                AfxThrowError();
                continue;
            }

            if (slot->fmt != AvxGetRasterFormat(ras))
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
            AfxReacquireObjects(1, &ras);
            //slot->managed = !!managed;
            ++canv->linkedCnt;
        }
        slot->ras = ras;
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

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    if (!canv->relink)
    {
        // canv->pimpl->bind(canv, FASE, baseSlot, cnt, rasters);
        if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, baseSlot, cnt, rasters))
            AfxThrowError();
    }
    else if (canv->relink(canv, TRUE, baseSlot, cnt, rasters))
        AfxThrowError();

    return err;
}

_AVXINL afxError _AvxBindDepthBuffer(avxCanvas canv, avxRaster depth)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &depth);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxUnit slotIdx;
    if (!AvxQueryCanvasSlots(canv, NIL, &slotIdx, NIL) || (slotIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxBindDrawBuffers(canv, slotIdx, 1, &depth))
        AfxThrowError();

    return err;
}

_AVXINL afxError _AvxBindStencilBuffer(avxCanvas canv, avxRaster stencil)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &stencil);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxUnit slotIdx;
    if (!AvxQueryCanvasSlots(canv, NIL, NIL, &slotIdx) || (slotIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxBindDrawBuffers(canv, slotIdx, 1, &stencil))
        AfxThrowError();

    return err;
}

_AVX afxError AvxPrintDrawBuffer(avxCanvas canv, afxUnit slotIdx, avxRasterIo const* op, afxUri const* uri, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->slotCnt, slotIdx, 1);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    avxRaster ras;
    if (!AvxGetDrawBuffers(canv, slotIdx, 1, &ras))
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
        afxRect area;
        avxRasterIo op2 = { 0 };
        AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO, &area);
        op2.rgn.whd = (avxRange){ area.w, area.h };

        if (AvxPrintRaster(ras, &op2, 1, uri, exuMask))
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
    rasi.whd = AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO);

    for (afxUnit i = 0; i < canv->slotCnt; i++)
    {
        _avxCanvasUnit* surf = &canv->slots[i];

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
    afxError err = AFX_ERR_NONE;
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    for (afxUnit i = 0; i < canv->slotCnt; i++)
    {
        _avxCanvasUnit* slot = &canv->slots[i];        
        _AvxCanvRelinkDrawBuffersCb_SW(canv, FALSE, i, 1, NIL);
        AFX_ASSERT(!slot->ras);
#if 0
        avxRaster ras = slot->ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            AfxDisposeObjects(1, &ras);
            canv->slots[i].ras = NIL;
        }
#endif
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

    //afxWarp const* whd = args[1];
    //afxUnit surCnt = *(afxUnit const *)args[2];
    //avxCanvasBuffer const* surCfgs = args[3];

    if (!cfg->slotCnt)
    {
        AfxThrowError();
        return err;
    }

    canv->tag = cfg->tag;
    canv->udd = cfg->udd;

    afxDrawLimits const* limits = _AvxDsysAccessLimits(dsys);
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
    
    canv->readjust = NIL;// _SglReadjustCanvasCb;

    canv->flags = cfg->flags;

    afxBool combinedDs = FALSE;
    afxUnit depthInIdx = AFX_INVALID_INDEX;
    afxUnit stencilInIdx = AFX_INVALID_INDEX;
    afxUnit slotCnt = 0;
    afxUnit colorCnt = 0;

    for (afxUnit i = 0; i < cfg->slotCnt; i++)
    {
        avxCanvasBuffer const* sur = &cfg->slots[i];
        AFX_ASSERT(sur->fmt);
        AFX_ASSERT(sur->sampleCnt);

        if (AvxTestDepthFormat(sur->fmt))
        {
            if (depthInIdx == AFX_INVALID_INDEX)
                ++slotCnt;

            depthInIdx = i;

            if ((combinedDs = AvxTestCombinedDsFormat(sur->fmt)))
                stencilInIdx = depthInIdx;

            continue;
        }

        if (AvxTestStencilFormat(sur->fmt))
        {
            if (stencilInIdx == AFX_INVALID_INDEX)
                ++slotCnt;

            stencilInIdx = i;
            continue;
        }

        ++colorCnt;
        ++slotCnt;
    }

    if (!colorCnt)
        canv->flags |= avxCanvasFlag_COLOR;

    if (!slotCnt)
    {
        canv->flags |= cfg->flags & ~(avxCanvasFlag_COLOR | avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_COMBINED_DS);
        canv->flags |= avxCanvasFlag_VOID;
    }

    canv->linkedCnt = 0;
    AFX_ASSERT_RANGE(limits->maxColorAttachments, 0, colorCnt);
    canv->colorCnt = colorCnt;
    canv->ownershipMask = NIL;

    if (stencilInIdx == AFX_INVALID_INDEX)
        canv->dsSlotIdx[1] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= avxCanvasFlag_STENCIL;
        canv->dsSlotIdx[1] = (slotCnt - 1);
    }

    if (depthInIdx == AFX_INVALID_INDEX)
        canv->dsSlotIdx[0] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= avxCanvasFlag_DEPTH;
        if (combinedDs)
        {
            canv->dsSlotIdx[0] = canv->dsSlotIdx[1];
            canv->flags |= avxCanvasFlag_COMBINED_DS;
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
    avxCanvasBuffer const* surCfg;
    _avxCanvasUnit* surf;

    for (afxUnit i = 0; i < slotCnt; i++)
    {
        surf = &canv->slots[i];
        *surf = (_avxCanvasUnit) { 0 };

        surCfg = &cfg->slots[i];

        if (i == canv->dsSlotIdx[1])
            surCfg = &cfg->slots[stencilInIdx];

        if (i == canv->dsSlotIdx[0])
            surCfg = &cfg->slots[depthInIdx];

        avxRaster ras = surCfg->ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            avxRasterInfo rasi;
            AvxDescribeRaster(ras, &rasi, NIL, NIL);

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
            surf->fmt = cfg->slots[i].fmt;
            surf->usage = cfg->slots[i].usage | avxRasterUsage_DRAW;
            surf->flags = cfg->slots[i].flags;
            surf->sampleCnt = cfg->slots[i].sampleCnt;

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

_AVX afxError AvxConfigureCanvas(afxDrawSystem dsys, avxCanvasConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
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

    afxDrawLimits const* limits = _AvxDsysAccessLimits(dsys);
    // @whd must be less than or equal to limits(maxCanvasWhd).
    cfg2.whd.w = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.w, cfg->whd.w));
    cfg2.whd.h = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.h, cfg->whd.h));
    cfg2.whd.d = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.d, cfg->whd.d));
    
    cfg2.lodCnt = AFX_MAX(1, AFX_MIN(limits->canvasNoAttachmentsSampleCnts, cfg->lodCnt));

    cfg2.slotCnt = AFX_MIN(cfg->slotCnt, limits->maxColorAttachments + 2);

    if (!cfg2.slotCnt)
    {
        cfg2.flags |= cfg->flags & ~(avxCanvasFlag_COLOR | avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_COMBINED_DS);
        cfg2.flags |= avxCanvasFlag_VOID;
    }
    else
    {
        afxBool combinedDs = FALSE;
        afxUnit depthInIdx = AFX_INVALID_INDEX;
        afxUnit stencilInIdx = AFX_INVALID_INDEX;
        afxUnit slotCnt = 0;
        afxUnit colorCnt = 0;

        for (afxUnit i = 0; i < cfg2.slotCnt; i++)
        {
            avxCanvasBuffer const* ac = &cfg->slots[i];
            AFX_ASSERT(ac->fmt);

            if (AvxTestDepthFormat(ac->fmt))
            {
                if (depthInIdx == AFX_INVALID_INDEX)
                    ++slotCnt;

                depthInIdx = i;

                if ((combinedDs = AvxTestCombinedDsFormat(ac->fmt)))
                    stencilInIdx = depthInIdx;

                continue;
            }

            if (AvxTestStencilFormat(ac->fmt))
            {
                if (stencilInIdx == AFX_INVALID_INDEX)
                    ++slotCnt;

                stencilInIdx = i;
                continue;
            }

            avxCanvasBuffer* c = &cfg2.slots[colorCnt];
            *c = *ac;
            c->usage |= avxRasterUsage_DRAW;
            c->fmt = ac->fmt;
            c->flags = ac->flags;
            c->ras = ac->ras;
            c->sampleCnt = AFX_MAX(1, c->sampleCnt);

            ++colorCnt;
            ++slotCnt;
        }

        if (!slotCnt)
            cfg2.flags |= avxCanvasFlag_VOID;

        if (!colorCnt)
            cfg2.flags |= avxCanvasFlag_COLOR;

        if (depthInIdx != AFX_INVALID_INDEX)
        {
            if (combinedDs)
            {
                cfg2.flags |= avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_COMBINED_DS;
                avxCanvasBuffer* ds = &cfg2.slots[(slotCnt - colorCnt - 1)];
                avxCanvasBuffer const* dsc = &cfg->slots[depthInIdx];
                *ds = *dsc;
                ds->usage |= avxRasterUsage_DRAW;
                ds->fmt = dsc->fmt;
                ds->flags = dsc->flags;
                ds->ras = dsc->ras;
                ds->sampleCnt = AFX_MAX(1, ds->sampleCnt);

                // How to nest separated D/S?
            }
            else
            {
                cfg2.flags |= avxCanvasFlag_DEPTH;
                avxCanvasBuffer* d = &cfg2.slots[(slotCnt - colorCnt - 1)];
                avxCanvasBuffer const* dc = &cfg->slots[depthInIdx];
                *d = *dc;
                d->usage |= avxRasterUsage_DRAW;
                d->fmt = dc->fmt;
                d->flags = dc->flags;
                d->ras = dc->ras;
                d->sampleCnt = AFX_MAX(1, d->sampleCnt);
            }
        }

        if (stencilInIdx != AFX_INVALID_INDEX)
        {
            if (!combinedDs)
            {
                cfg2.flags |= avxCanvasFlag_STENCIL;

                avxCanvasBuffer* s = &cfg2.slots[(slotCnt - 1)];
                avxCanvasBuffer const* sc = &cfg->slots[stencilInIdx];
                *s = *sc;
                s->usage |= avxRasterUsage_DRAW;
                s->fmt = sc->fmt;
                s->flags = sc->flags;
                s->ras = sc->ras;
                s->sampleCnt = AFX_MAX(1, s->sampleCnt);
            }
        }
    }

    *cfg = cfg2;
    return err;
}

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
        AFX_ASSERT(canv->whd.w >= cfg->whd.w);
        AFX_ASSERT(canv->whd.h >= cfg->whd.h);
        AFX_ASSERT(canv->whd.d >= cfg->whd.d);
        AFX_ASSERT((canv->flags & cfg->flags) == cfg->flags);
        AFX_ASSERT(canv->tag.start == cfg->tag.start);

        for (afxUnit j = 0; j < cfg->slotCnt; j++)
        {
            _avxCanvasUnit* surf = &canv->slots[j];

            // combined DS reduces one slot.
            if (j == canv->slotCnt)
                break;

            if (!cfg->slots[j].ras)
            {
                AFX_ASSERT((surf->fmt == cfg->slots[j].fmt) || !(cfg->slots[j].fmt));
                AFX_ASSERT((surf->flags & cfg->slots[j].flags) == cfg->slots[j].flags);
                AFX_ASSERT((surf->usage & cfg->slots[j].usage) == cfg->slots[j].usage);
                AFX_ASSERT((surf->usage & avxRasterUsage_DRAW) == avxRasterUsage_DRAW);
                AFX_ASSERT(surf->sampleCnt >= cfg->slots[j].sampleCnt);
            }
            else
            {
                AFX_ASSERT(surf->ras == cfg->slots[j].ras);
            }
        }
    }
#endif
    return err;
}
