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
//#define _AVX_DRAW_SYSTEM_C
//#define _AFX_CORE_C
//#define _AFX_DEVICE_C
//#define _AFX_CONTEXT_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_SURFACE_C
#include "avxIcd.h"

_AVX afxUnit _AvxDoutIsSuspended(afxSurface dout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, TRUE);
    afxUnit suspendCnt = dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, TRUE);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutSuspendFunction(afxSurface dout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, FALSE);
    afxUnit suspendCnt = ++dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, FALSE);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutResumeFunction(afxSurface dout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, FALSE);
    afxUnit suspendCnt = --dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, FALSE);
    return suspendCnt;
}

_AVX afxError _AvxDoutFreeAllBuffers(afxSurface dout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    if (dout->swaps)
    {
        for (afxUnit i = 0; i < dout->swapCnt; i++)
        {
            _avxSurfaceSwap* swap = &dout->swaps[i];
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &swap->canv);
            AfxDisposeObjects(1, &swap->canv);
            swap->ras = NIL; // ras is just cached, not owned.
            //*swap = (_avxSurfaceSwap) { 0 };
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawSystem AvxGetSurfaceHost(afxSurface dout)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AfxGetHost(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxModule AvxGetSurfaceIcd(afxSurface dout)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AvxGetSurfaceHost(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AvxGetSystemIcd(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_ICD, 1, &icd);
    return icd;
}

_AVX void* AvxGetSurfaceUdd(afxSurface dout, afxUnit slotIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    
    if (slotIdx >= ARRAY_SIZE(dout->udd))
    {
        AfxThrowError();
        return NIL;
    }
    return dout->udd[slotIdx];
}

_AVX afxError AvxQuerySurfaceConfiguration(afxSurface dout, afxSurfaceConfig* cfg)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(cfg);
    *cfg = (afxSurfaceConfig) { 0 };
    cfg->ccfg = dout->ccfg;
    cfg->colorSpc = dout->colorSpc;
    cfg->doNotClip = dout->doNotClip;
    cfg->iop.endpointNotifyFn = dout->endpointNotifyFn;
    cfg->iop.endpointNotifyObj = dout->endpointNotifyObj;
    cfg->latency = dout->swapCnt;
    cfg->presentAlpha = dout->presentAlpha;
    cfg->presentMode = dout->presentMode;
    cfg->presentTransform = dout->presentTransform;
    cfg->udd[0] = dout->udd[0];
    cfg->udd[1] = dout->udd[1];
    cfg->udd[2] = dout->udd[2];
    cfg->udd[3] = dout->udd[3];
    cfg->tag = dout->tag;
    cfg->resolution = dout->resolution;
    cfg->refreshRate = dout->refreshRate;
    return err;
}

_AVX afxError AvxQuerySurfaceMode(afxSurface dout, avxModeSetting* mode)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(mode);

    *mode = (avxModeSetting) { 0 };
    mode->wpOverHp = dout->wpOverHp;
    mode->refreshRate = dout->refreshRate;
    mode->resolution = dout->resolution;
    mode->wrOverHr = dout->wrOverHr;
    mode->exclusive = dout->fse;
    mode->outRate = dout->outRate;
    mode->dstArea = dout->dstArea;
    mode->persistent = dout->persistBlit;
    mode->srcArea = dout->srcArea;
    mode->wwOverHw = dout->wwOverHw;
    return err;
}

_AVX afxReal64 AvxGetSurfaceArea(afxSurface dout, afxRect* area)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(area);
    *area = dout->area;

    return dout->wwOverHw;
}

_AVX void _AvxDoutGetExtentNormalized(afxSurface dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(whd);
    afxRect whd2;
    AvxGetSurfaceArea(dout, &whd2);
    AfxV3dSet(whd, AfxNdcf(whd2.w, dout->resolution.w), AfxNdcf(whd2.h, dout->resolution.h), (afxReal)1);
}

_AVX afxError AvxAdjustSurface(afxSurface dout, afxRect const* area, afxBool fse)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

    afxRect rc;
    //AFX_ASSERT4(whd, whd.w, whd.h, whd.d);
    rc.x = area->x;
    rc.y = area->y;
    rc.w = AFX_MAX(area->w, 1);
    rc.h = AFX_MAX(area->h, 1);
    AFX_ASSERT_EXTENT(dout->resolution.w, area->w);
    AFX_ASSERT_EXTENT(dout->resolution.h, area->h);

    if (dout->ddi->adjustCb(dout, &rc, fse))
        AfxThrowError();

    if (dout->ddi->regenCb(dout, TRUE))
        AfxThrowError();

    if (!err)
    {
        afxV2d ndc;
        AvxGetSurfaceArea(dout, &rc);
        AfxV2dNdc(ndc, AFX_V2D(rc.w, rc.h), AFX_V2D(dout->resolution.w, dout->resolution.h));
#ifdef _AFX_DOUT_LOGS
        AfxReportMessage("Draw output %03u adjusted. %ux%u %ux%u %.3fx%.3f %f", AfxGetObjectId(dout), whd2.w, whd2.h, dout->resolution.w, dout->resolution.h, ndc[0], ndc[1], dout->wwOverHw);
#endif
    }

    --dout->resizing;

    return err;
}

_AVX afxError _AvxDoutAdjustNormalized(afxSurface dout, afxV3d const whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT4(whd, whd[0], whd[1], whd[2]);

    afxRect const whd2 =
    {
        .x = dout->area.x,
        .y = dout->area.y,
        .w = (afxUnit)AfxUnndcf(AFX_MAX(1, whd[0]), dout->resolution.w),
        .h = (afxUnit)AfxUnndcf(AFX_MAX(1, whd[1]), dout->resolution.h)
    };
    return AvxAdjustSurface(dout, &whd2, dout->fse);
}

_AVX afxError AvxResetSurfaceMode(afxSurface dout, avxModeSetting const* mode)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(mode);

    if (dout->ddi->modeSetCb)
    {
        if (dout->ddi->modeSetCb(dout, mode))
        {
            AfxThrowError();
        }
        return err;
    }

    afxReal64 physAspectRatio = mode->wpOverHp;
    afxReal refreshRate = mode->refreshRate;
    avxRange resolution = mode->resolution;
    afxBool exclusive = mode->exclusive;

    afxV3d whdNdc;
    _AvxDoutGetExtentNormalized(dout, whdNdc);
    
    if (AvxDotRange(resolution, resolution))
    {

        AFX_ASSERT3(resolution.w, resolution.h, resolution.d);
        dout->resolution = AvxMaxRange(AVX_RANGE(1, 1, 1), resolution);
        dout->wrOverHr = (afxReal64)dout->resolution.w / (afxReal64)dout->resolution.h;
    }

    if (refreshRate)
        dout->refreshRate = refreshRate;

    if (physAspectRatio)
        dout->wpOverHp = physAspectRatio;

#ifdef _AVX_DOUT_LOGS
    AfxReportMessage("afxSurface %03u readapted. %ux%u @ %fHz <%fr, %fp>", AfxGetObjectId(dout), dout->resolution.w, dout->resolution.h, dout->refreshRate, dout->wrOverHr, dout->wpOverHp);
#endif

    // readjust draw output extent if it overflows the new resolution.

    afxRect whd;
    AvxGetSurfaceArea(dout, &whd);
    afxBool readjust = FALSE;
    
    if (whd.w > dout->resolution.w)
        whd.w = dout->resolution.w, readjust = TRUE;

    if (whd.h > dout->resolution.h)
        whd.h = dout->resolution.h, readjust = TRUE;

    dout->dstArea = mode->dstArea;
    dout->srcArea = mode->srcArea;

    if (readjust)
        _AvxDoutAdjustNormalized(dout, whdNdc);

    return err;
}

// BUFFERIZATION ///////////////////////////////////////////////////////////////

// Pull an available draw output buffer (usually from the WSI).

_AVX afxError AvxLockSurfaceBuffer(afxSurface dout, afxUnit64 timeout, afxMask exuMask, avxFence signal, afxUnit* bufIdx)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    if (dout->resizing)
    {
        err = afxError_NOT_READY;
        return err;
    }

    afxUnit bufIdx2 = AFX_INVALID_INDEX;

    AFX_ASSERT(dout->ddi->lockCb);
    err = dout->ddi->lockCb(dout, exuMask, signal, timeout, &bufIdx2);

    if (!err)
    {
        AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
        AFX_ASSERT_RANGE(dout->swapCnt, bufIdx2, 1);
    }
    else
    {
        AFX_ASSERT(AFX_INVALID_INDEX == bufIdx2);
        bufIdx2 = AFX_INVALID_INDEX;
    }
    AFX_ASSERT(bufIdx);
    *bufIdx = bufIdx2;
    return err;
}

_AVX afxError AvxUnlockSurfaceBuffer(afxSurface dout, afxUnit bufIdx)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->swapCnt, bufIdx, 1);
    if (!(bufIdx < dout->swapCnt))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(dout->ddi->unlockCb);
    if (dout->ddi->unlockCb(dout, bufIdx))
        AfxThrowError();

    return err;
}

_AVX afxBool AvxGetSurfaceCanvas(afxSurface dout, afxUnit bufIdx, avxCanvas* canvas, afxLayeredRect* bounds)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    avxCanvas canv = NIL;

    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->swapCnt, bufIdx, 1);
    if (bufIdx >= dout->swapCnt)
    {
        AfxThrowError();
    }
    else
    {
        _avxSurfaceSwap* swap = &dout->swaps[bufIdx];

        if ((canv = swap->canv))
        {
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        }

        if (bounds)
            *bounds = swap->bounds;
    }

    AFX_ASSERT(canvas);
    *canvas = canv;
    return !!canv;
}

_AVX afxBool AvxGetSurfaceBuffer(afxSurface dout, afxUnit bufIdx, avxRaster* buffer)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    avxRaster ras = NIL;

    afxLayeredRect area;
    avxCanvas canv;
    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->swapCnt, bufIdx, 1);
    if (AvxGetSurfaceCanvas(dout, bufIdx, &canv, &area))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        if (AvxGetDrawBuffers(canv, 0, 1, &ras))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
        }
    }

    AFX_ASSERT(buffer);
    *buffer = ras;
    return !!ras;
}

_AVX afxBool AvxGetSurfaceFence(afxSurface dout, afxUnit bufIdx, avxFence* fence, afxUnit64* nextValue)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    avxFence fenc = NIL;
    afxUnit64 fencNextValue = 0;

    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->swapCnt, bufIdx, 1);
    if (bufIdx >= dout->swapCnt)
    {
        AfxThrowError();
    }
    else
    {
        _avxSurfaceSwap* swap = &dout->swaps[bufIdx];

        if ((fenc = swap->fenc))
        {
            AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
        }
        fencNextValue = swap->fencNextValue;
    }

    AFX_ASSERT(fence);
    *fence = fenc;
    AFX_ASSERT(fence);
    fencNextValue = fencNextValue;
    return !!fenc;
}

_AVX afxError AvxPrintSurfaceBuffer(afxSurface dout, afxUnit bufIdx, avxRasterIo const* op, afxUri const* uri, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxLayeredRect area;
    avxCanvas canv;
    AFX_ASSERT_RANGE(dout->swapCnt, bufIdx, 1);
    if (!AvxGetSurfaceCanvas(dout, bufIdx, &canv, &area))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    AFX_ASSERT(uri);
    AFX_ASSERT(!AfxIsUriBlank(uri));

    if (AvxPrintDrawBuffer(canv, 0, op, uri, exuMask))
        AfxThrowError();

    return err;
}



_AVX afxError AvxWaitForSurface(afxSurface dout, afxUnit64 timeout)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxTimeSpec ts =
    {
        .secs = AFX_SECS_PER_NANOSECS(timeout), // Convert nanoseconds to seconds
        .nsecs = timeout % AFX_NANOSECS_PER_SECS(1) // Get the remainder in nanoseconds
    };

    AfxLockMutex(&dout->idleCndMtx);
    while (dout->submCnt)
        AfxWaitTimedCondition(&dout->idleCnd, &dout->idleCndMtx, &ts);
    AfxUnlockMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError AvxCallSurfaceEndpoint(afxSurface dout, afxUnit code, ...)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    va_list ap;
    va_start(ap, code);
    AFX_ASSERT(dout->ddi);
    AFX_ASSERT(dout->ddi->ioctlCb);
    afxError rslt = dout->ddi->ioctlCb(dout, code, ap);
    va_end(ap);

    return rslt;
}

_AVX afxError _AvxDoutDtorCb(afxSurface dout)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    //dout->disabled;

    AFX_ASSERT(!dout->idd);

    _AvxDoutSuspendFunction(dout);
    
    _AvxDoutFreeAllBuffers(dout);
    
    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->swapCnt,
            .siz = sizeof(dout->swaps[0]),
            .var = (void**)&dout->swaps
        }
    };

    AfxDeallocateInstanceData(dout, ARRAY_SIZE(stashs), stashs);

    AfxExhaustInterlockedQueue(&dout->freeBuffers);

    AfxCleanUpFutex(&dout->suspendSlock);

    AfxDismantleCondition(&dout->idleCnd);
    AfxDismantleMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError _AvxDoutCtorCb(afxSurface dout, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };

    AfxZero(dout, sizeof(dout[0]));

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxSurfaceConfig const* cfg = ((afxSurfaceConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    afxSurfaceConfig def = { 0 };
    AvxConfigureSurface(dsys, &def);

    dout->ddi = &_AVX_DDI_DOUT;

    dout->udd[0] = cfg->udd[0];
    dout->udd[1] = cfg->udd[1];
    dout->udd[2] = cfg->udd[2];
    dout->udd[3] = cfg->udd[3];
    dout->tag = cfg->tag;

    // endpoint
    dout->resizing = FALSE;
    dout->resizable = TRUE;
    dout->refreshRate = 1;
    //AFX_ASSERT(AFX_TEST_ALIGNMENT(&dout->resolution, 16));
    dout->resolution = AVX_RANGE(AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX);
    dout->wrOverHr = dout->resolution.w / dout->resolution.h;
    dout->wpOverHp = AvxFindPhysicalAspectRatio(dout->resolution.w, dout->resolution.h);
    dout->presentAlpha = cfg->presentAlpha ? cfg->presentAlpha : def.presentAlpha; // consider transparency for window composing.
    dout->presentTransform = cfg->presentTransform ? cfg->presentTransform : def.presentTransform; // The default is to leave it as it is.
    dout->presentMode = cfg->presentMode ? cfg->presentMode : def.presentMode; // The default avxPresentMode_LIFO is already zero.
    dout->doNotClip = !!cfg->doNotClip; // don't spend resources doing off-screen draw.
    dout->endpointNotifyObj = cfg->iop.endpointNotifyObj;
    dout->endpointNotifyFn = cfg->iop.endpointNotifyFn;
    dout->idd = NIL;
    
    // canvas
    //dout->extent = AvxMaxRange(AVX_RANGE(1, 1, 1), cfg->extent);
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : avxColorSpace_STANDARD; // sRGB is the default
    dout->ccfg = cfg->ccfg;
    AvxConfigureCanvas(dsys, &dout->ccfg);
    dout->wwOverHw = dout->ccfg.whd.w / dout->ccfg.whd.h;

    if (!dout->ccfg.binCnt)
    {
        AFX_ASSERT(dout->ccfg.binCnt);
        AfxThrowError();
    }
#if 0
    for (afxUnit i = 0; i < dout->ccfg.annexCnt; i++)
    {
        avxDrawBin* a = &dout->ccfg.annexes[i];
    }
#endif
    // swapchain
    dout->swapCnt = AFX_MAX(1, AFX_MIN(cfg->latency, def.latency)); // 2 or 3; double or triple buffered for via-memory presentation.

    dout->area = AFX_RECT(0, 0, dout->ccfg.whd.w, dout->ccfg.whd.h);
    dout->dstArea = dout->area;
    dout->srcArea = dout->area;
    dout->persistBlit = FALSE;

    dout->submCnt = 0;
    dout->lastPresentedBufIdx = AFX_INVALID_INDEX;
    dout->suspendCnt = 1;
    AfxSetUpFutex(&dout->suspendSlock);

    AfxDeployMutex(&dout->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dout->idleCnd);

    dout->swaps = NIL;

    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->swapCnt,
            .siz = sizeof(dout->swaps[0]),
            .var = (void**)&dout->swaps
        }
    };

    if (AfxAllocateInstanceData(dout, ARRAY_SIZE(stashs), stashs))
    {
        AfxThrowError();
    }
    AfxZero(dout->swaps, sizeof(dout->swaps[0]) * dout->swapCnt);

    AfxMakeInterlockedQueue(&dout->freeBuffers, sizeof(afxUnit), dout->swapCnt);

    for (afxUnit i = 0; i < dout->swapCnt; i++)
        AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { i });

    AfxGetClock(&dout->prevBufReqTime);
    dout->outCntResetClock = dout->prevBufReqTime;
    dout->startClock = dout->prevBufReqTime;
    dout->lastClock = dout->prevBufReqTime;

    AFX_ASSERT(dout->resolution.w);
    AFX_ASSERT(dout->resolution.h);
    AFX_ASSERT(dout->resolution.d);
    AFX_ASSERT_EXTENT(dout->resolution.w, dout->ccfg.whd.w);
    AFX_ASSERT_EXTENT(dout->resolution.h, dout->ccfg.whd.h);
    AFX_ASSERT_EXTENT(dout->resolution.d, dout->ccfg.whd.d);

    AFX_ASSERT(dout->swapCnt);
    AFX_ASSERT(dout->ccfg.bins[0].usage & avxRasterUsage_DRAW);
    AFX_ASSERT(dout->refreshRate);
    AFX_ASSERT(dout->wpOverHp);
    AFX_ASSERT(dout->wrOverHr);
    AFX_ASSERT(dout->wwOverHw);
        
    // provoke buffer geneartion
    AvxAdjustSurface(dout, &dout->area, cfg->exclusive);

    if (err)
        AfxDeallocateInstanceData(dout, ARRAY_SIZE(stashs), stashs);

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_DOUT =
{
    .fcc = afxFcc_DOUT,
    .name = "Surface",
    .desc = "Presentable Video Surface",
    .fixedSiz = sizeof(AFX_OBJECT(afxSurface)),
    .ctor = (void*)_AvxDoutCtorCb,
    .dtor = (void*)_AvxDoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxUnit AvxEnumerateSurfaces(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxSurface outputs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(!cnt || outputs);
    afxClass const* cls = _AvxDsysGetDdi(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AvxEvokeSurfaces(afxDrawSystem dsys, afxBool(*f)(afxSurface, void*), void* udd, afxUnit first, afxUnit cnt, afxSurface outputs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(outputs);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetDdi(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AvxInvokeSurfaces(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxSurface, void*), void *udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetDdi(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxInvokeObjects(cls, (void*)f, udd, first, cnt);
}

_AVX afxError AvxConfigureSurface(afxDrawSystem dsys, afxSurfaceConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    // Used to opt for sRGB format. Safe if used after being zeroed.
    //cfg->colorSpc = avxColorSpace_STANDARD;

    if (!cfg->ccfg.binCnt)
    {
        cfg->ccfg.binCnt = 1;

        if (cfg->colorSpc == avxColorSpace_STANDARD)
            cfg->ccfg.bins[0].fmt = avxFormat_BGRA8v;
        else
            cfg->ccfg.bins[0].fmt = avxFormat_BGRA8un;
    }

    AvxConfigureCanvas(dsys, &cfg->ccfg);

    if (!cfg->latency)
        cfg->latency = 2;

    if (!(cfg->resolution.w * cfg->resolution.h * cfg->resolution.d))
        cfg->resolution = AVX_RANGE(AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX);

    if (!cfg->refreshRate)
        cfg->refreshRate = 1;

    //cfg->latency = 2;
    //cfg->presentMode = NIL;
    //cfg->presentAlpha = FALSE;
    //cfg->presentTransform = NIL;
    //cfg->doNotClip = FALSE;
    //cfg->resolution = AVX_RANGE(AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX);
    //cfg->resizable = TRUE;
    //cfg->refreshRate = 1;
    //cfg->exclusive = FALSE;

    return err;
}

_AVX afxError AvxAcquireSurface(afxDrawSystem dsys, afxSurfaceConfig const* cfg, afxSurface* output)
// file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);

    afxSurface dout;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&dout, (void const*[]) { dsys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(output);
    *output = dout;
    return err;
}

_AVX afxError AvxPresentSurfaces(afxDrawSystem dsys, afxUnit cnt, avxPresentation presentations[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(presentations);
    AFX_ASSERT(cnt);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    for (afxUnit doutIt = 0; doutIt < cnt; doutIt++)
    {
        avxPresentation* pres = &presentations[doutIt];

        afxSurface dout = pres->dout;
        if (!dout)
        {
            AFX_ASSERT(dout);
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

        afxUnit bufxIdx = pres->bufIdx;
        if (bufxIdx >= dout->swapCnt)
        {
            AFX_ASSERT_RANGE(dout->swapCnt, bufxIdx, 1);
            continue;
        }

        afxMask exuMask = pres->exuMask;
        afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIdx = nextExuIdx; exuIdx < exuCnt; exuIdx++)
            {
                nextExuIdx = 0;

                if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                    continue;

                afxDrawBridge dexu;
                if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
                {
                    AfxThrowError();
                    return err;
                }

                afxError err2 = _AvxDexuPresentBuffers(dexu, cnt, presentations);
                err = err2;

                if (!err2)
                {
                    queued = TRUE;
                    break; // for --- iterate bridges
                }

                if ((err2 == afxError_TIMEOUT) || (err2 == afxError_BUSY))
                {
                    continue;
                }

                AfxThrowError();
            }

            if (err || queued)
                break; // while --- find bridges
        }

        if (err || queued)
            break; // for
    }
    return err;
}
