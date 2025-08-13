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
//#define _AVX_DRAW_SYSTEM_C
//#define _AFX_CORE_C
//#define _AFX_DEVICE_C
//#define _AFX_CONTEXT_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_OUTPUT_C
#include "../ddi/avxImplementation.h"

_AVX afxSurfaceConfig const AVX_DEFAULT_SURFACE_CONFIG =
{
    .colorSpc = avxColorSpace_STANDARD,
    .canvas.whd = { 1, 1, 1 },
    .canvas.slotCnt = 2,
    .canvas.slots[0].fmt = avxFormat_BGRA8v,
    .canvas.slots[0].usage = avxRasterUsage_DRAW | avxRasterUsage_TEXTURE,
    .canvas.slots[1].fmt = avxFormat_D32f,
    .canvas.slots[1].usage = avxRasterUsage_DRAW,
    .canvas.slots[2].usage = avxRasterUsage_DRAW,
    .canvas.slots[3].usage = avxRasterUsage_DRAW,
    .canvas.slots[4].usage = avxRasterUsage_DRAW,
    .canvas.slots[5].usage = avxRasterUsage_DRAW,
    .canvas.slots[6].usage = avxRasterUsage_DRAW,
    .canvas.slots[7].usage = avxRasterUsage_DRAW,
    .canvas.slots[8].usage = avxRasterUsage_DRAW,
    .canvas.slots[9].usage = avxRasterUsage_DRAW,
    .minBufCnt = 3,
    .presentMode = NIL,
    .presentAlpha = FALSE,
    .presentTransform = NIL,
    .doNotClip = FALSE,
    .resolution = AVX_RANGE(AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX), // ignore the IntelliDumb warning
    .resizable = TRUE,
    .refreshRate = 1,
    .exclusive = FALSE
};

_AVX afxUnit _AvxDoutIsSuspended(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, TRUE);
    afxUnit suspendCnt = dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, TRUE);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutSuspendFunction(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, FALSE);
    afxUnit suspendCnt = ++dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, FALSE);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutResumeFunction(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, FALSE);
    afxUnit suspendCnt = --dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, FALSE);
    return suspendCnt;
}

_AVX afxError _AvxDoutFreeAllBuffers(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    if (dout->buffers)
    {
        for (afxUnit i = 0; i < dout->bufCnt; i++)
        {
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &dout->buffers[i].canv);
            AfxDisposeObjects(1, &dout->buffers[i].canv);
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawSystem AvxGetSurfaceSystem(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AfxGetProvider(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxModule AvxGetSurfaceIcd(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AvxGetSurfaceSystem(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AvxGetDrawSystemIcd(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_ICD, 1, &icd);
    return icd;
}

_AVX void* AvxGetSurfaceUdd(afxSurface dout, afxUnit slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    
    if (slotIdx >= ARRAY_SIZE(dout->udd))
    {
        AfxThrowError();
        return NIL;
    }
    return dout->udd[slotIdx];
}

_AVX void AvxDescribeSurface(afxSurface dout, afxSurfaceConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(cfg);
    *cfg = (afxSurfaceConfig) { 0 };
    cfg->canvas = dout->ccfg;
    cfg->colorSpc = dout->colorSpc;
    cfg->doNotClip = dout->doNotClip;
    cfg->iop.endpointNotifyFn = dout->endpointNotifyFn;
    cfg->iop.endpointNotifyObj = dout->endpointNotifyObj;
    cfg->minBufCnt = dout->bufCnt;
    cfg->presentAlpha = dout->presentAlpha;
    cfg->presentMode = dout->presentMode;
    cfg->presentTransform = dout->presentTransform;
    cfg->udd[0] = dout->udd[0];
    cfg->udd[1] = dout->udd[1];
    cfg->udd[2] = dout->udd[2];
    cfg->udd[3] = dout->udd[3];
    cfg->resolution = dout->resolution;
    cfg->refreshRate = dout->refreshRate;
}

_AVX void AvxQuerySurfaceSettings(afxSurface dout, avxModeSetting* mode)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(mode);

    *mode = (avxModeSetting) { 0 };
    mode->wrOverHr = dout->wpOverHp;
    mode->refreshRate = dout->refreshRate;
    mode->resolution = dout->resolution;
    mode->wrOverHr = dout->wrOverHr;
    mode->exclusive = dout->fse;
    mode->outRate = dout->outRate;
}

_AVX afxReal64 AvxGetSurfaceArea(afxSurface dout, afxRect* area)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(area);
    *area = dout->area;

    return dout->wwOverHw;
}

_AVX void _AvxDoutGetExtentNormalized(afxSurface dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(whd);
    afxRect whd2;
    AvxGetSurfaceArea(dout, &whd2);
    AfxV3dSet(whd, AfxNdcf(whd2.w, dout->resolution.w), AfxNdcf(whd2.h, dout->resolution.h), (afxReal)1);
}

_AVX afxError AvxAdjustSurface(afxSurface dout, afxRect const* area, afxBool fse)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxRect rc;
    //AFX_ASSERT4(whd, whd.w, whd.h, whd.d);
    rc.x = area->x;
    rc.y = area->y;
    rc.w = AFX_MAX(area->w, 1);
    rc.h = AFX_MAX(area->h, 1);
    AFX_ASSERT_EXTENT(dout->resolution.w, area->w);
    AFX_ASSERT_EXTENT(dout->resolution.h, area->h);

    if (dout->pimpl->adjustCb(dout, &rc, fse))
        AfxThrowError();

    if (dout->pimpl->regenCb(dout, TRUE))
        AfxThrowError();

    ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

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
    afxError err = AFX_ERR_NONE;
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

_AVX afxError AvxChangeSurfaceSettings(afxSurface dout, avxModeSetting const* mode)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(mode);

    if (dout->pimpl->modeSetCb)
    {
        if (dout->pimpl->modeSetCb(dout, mode))
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

    if (readjust)
        _AvxDoutAdjustNormalized(dout, whdNdc);

    return err;
}

// BUFFERIZATION ///////////////////////////////////////////////////////////////

// Pull an available draw output buffer (usually from the WSI).

_AVX afxError AvxLockSurfaceBuffer(afxSurface dout, afxUnit64 timeout, afxMask exuMask, afxUnit* bufIdx, afxSemaphore sem, avxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxUnit bufIdx2 = AFX_INVALID_INDEX;

    if (dout->pimpl->lockCb)
    {
        err = dout->pimpl->lockCb(dout, timeout, sem, fenc, exuMask, &bufIdx2);

        if (!err)
        {
            AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
            AFX_ASSERT_RANGE(dout->bufCnt, bufIdx2, 1);
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

    afxBool success = FALSE;
    afxClock timeoutStart, last;
    afxBool wait = (timeout != 0);
    afxBool finite = (timeout != AFX_TIMEOUT_INFINITE);

    if (wait && finite)
    {
        AfxGetClock(&timeoutStart);
        last = timeoutStart;
    }

    if (dout->bufReqPerSec)
    {
        // Target frames per second (FPS)

        // Duration per frame in nanoseconds
        afxInt64 bufGenPeriod = AFX_NSECS_PER_SEC(1) / dout->bufReqPerSec;

        while (1)
        {
            AfxGetClock(&last);
            afxInt64 elapsedTime = AfxGetNanosecondsElapsed(&dout->prevBufReqTime, &last);

            if (elapsedTime >= bufGenPeriod) break;
            else
            {
                if (!wait)
                {
                    err = afxError_TIMEOUT;
                    *bufIdx = AFX_INVALID_INDEX;
                    return err;
                }
                else
                {
                    if (!finite) continue;
                    else
                    {
                        AfxGetClock(&last);
                        afxInt64 elapsed = AfxGetNanosecondsElapsed(&timeoutStart, &last);

                        if (elapsed > 0)
                            timeout -= elapsed;
                        else if (0 > elapsed)
                            timeout = 0;

                        if (timeout == 0)
                        {
                            err = afxError_TIMEOUT;
                            *bufIdx = AFX_INVALID_INDEX;
                            return err;
                        }
                    }
                }
            }
        }
    }

    while (1)
    {
        afxUnit lockedBufIdx = AFX_INVALID_INDEX;

        if (AfxPopInterlockedQueue(&dout->freeBuffers, &lockedBufIdx))
        {
            _avxDoutBuffer* slot = &dout->buffers[lockedBufIdx];
            avxCanvas canv = slot->canv;

            if (canv)
            {
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
            }
            AFX_ASSERT(slot->locked == 0);
            ++slot->locked;
            bufIdx2 = lockedBufIdx;
            AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
            AFX_ASSERT_RANGE(dout->bufCnt, bufIdx2, 1);
            AFX_ASSERT(bufIdx);
            *bufIdx = bufIdx2;
            success = TRUE;
            AfxGetClock(&dout->prevBufReqTime);
            break;
        }

        if (!wait)
        {
            err = afxError_TIMEOUT;
            AFX_ASSERT(bufIdx);
            *bufIdx = AFX_INVALID_INDEX;
            break;
        }
        else
        {
            if (!finite) continue;
            else
            {
                AfxGetClock(&last);
                afxInt64 elapsed = AfxGetNanosecondsElapsed(&timeoutStart, &last);

                if (elapsed > 0)
                    timeout -= elapsed;
                else if (0 > elapsed)
                    timeout = 0;

                if (timeout == 0)
                {
                    err = afxError_TIMEOUT;
                    AFX_ASSERT(bufIdx);
                    *bufIdx = AFX_INVALID_INDEX;
                    break;
                }
            }
        }
    }

    if (!err)
    {
        AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
        AFX_ASSERT_RANGE(dout->bufCnt, bufIdx2, 1);
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
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    if (!(bufIdx < dout->bufCnt))
    {
        AfxThrowError();
        return err;
    }

    _avxDoutBuffer* slot = &dout->buffers[bufIdx];
    if (!slot->locked)
    {
        AfxThrowError();
        return err;
    }

    if (dout->pimpl->unlockCb)
    {
        if (dout->pimpl->unlockCb(dout, bufIdx))
            AfxThrowError();

        return err;
    }

    if (!AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { bufIdx })) AfxThrowError();
    else
    {
        --slot->locked;
    }

    return err;
}

_AVX afxBool AvxGetSurfaceCanvas(afxSurface dout, afxUnit bufIdx, avxCanvas* canvas, afxRect* area)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxRect rc = AVX_RECT_ZERO;
    avxCanvas canv;

    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    if (bufIdx >= dout->bufCnt)
    {
        AfxThrowError();
    }
    else
    {
        if ((canv = dout->buffers[bufIdx].canv))
        {
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
            AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO, &rc);
        }
    }

    AFX_ASSERT(canvas);
    *canvas = canv;
    AFX_ASSERT(area);
    *area = rc;
    return !!canv;
}

_AVX afxBool AvxGetSurfaceBuffer(afxSurface dout, afxUnit bufIdx, avxRaster* buffer)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    avxRaster ras = NIL;

    afxRect area;
    avxCanvas canv;
    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
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

_AVX afxError AvxPrintSurfaceBuffer(afxSurface dout, afxUnit bufIdx, avxRasterIo const* op, afxUri const* uri, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxRect area;
    avxCanvas canv;
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
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
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxTimeSpec ts =
    {
        .secs = AFX_SECS_PER_NSECS(timeout), // Convert nanoseconds to seconds
        .nsecs = timeout % AFX_NSECS_PER_SEC(1) // Get the remainder in nanoseconds
    };

    AfxLockMutex(&dout->idleCndMtx);
    while (dout->submCnt)
        AfxWaitTimedCondition(&dout->idleCnd, &dout->idleCndMtx, &ts);
    AfxUnlockMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError AvxCallSurfaceEndpoint(afxSurface dout, afxUnit code, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    va_list ap;
    va_start(ap, code);
    AFX_ASSERT(dout->pimpl);
    AFX_ASSERT(dout->pimpl->ioctlCb);
    afxError rslt = dout->pimpl->ioctlCb(dout, code, ap);
    va_end(ap);

    return rslt;
}

_AVX afxError _AvxDoutDtorCb(afxSurface dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    //dout->disabled;

    AFX_ASSERT(!dout->idd);

    _AvxDoutSuspendFunction(dout);
    
    _AvxDoutFreeAllBuffers(dout);
    
    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->buffers[0]),
            .var = (void**)&dout->buffers
        }
    };

    AfxDeallocateInstanceData(dout, ARRAY_SIZE(stashs), stashs);

    AfxDismantleInterlockedQueue(&dout->freeBuffers);

    AfxDismantleFutex(&dout->suspendSlock);

    AfxDismantleCondition(&dout->idleCnd);
    AfxDismantleMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError _AvxDoutCtorCb(afxSurface dout, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(dout, sizeof(dout[0]));

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxSurfaceConfig const* cfg = ((afxSurfaceConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    afxSurfaceConfig def;
    AvxConfigureSurface(dsys, &def);

    dout->pimpl = &_AVX_DOUT_DDI;

    // endpoint
    dout->resizing = FALSE;
    dout->resizable = TRUE;
    dout->refreshRate = 1;
    //AFX_ASSERT(AFX_IS_ALIGNED(&dout->resolution, 16));
    dout->resolution = AVX_RANGE(AFX_U32_MAX, AFX_U32_MAX, AFX_U32_MAX);
    dout->wrOverHr = dout->resolution.w / dout->resolution.h;
    dout->wpOverHp = AvxFindPhysicalAspectRatio(dout->resolution.w, dout->resolution.h);
    dout->presentAlpha = cfg->presentAlpha ? cfg->presentAlpha : def.presentAlpha; // consider transparency for window composing.
    dout->presentTransform = cfg->presentTransform ? cfg->presentTransform : def.presentTransform; // The default is to leave it as it is.
    dout->presentMode = cfg->presentMode ? cfg->presentMode : def.presentMode; // The default avxPresentMode_LIFO is already zero.
    dout->doNotClip = !!cfg->doNotClip; // don't spend resources doing off-screen draw.
    dout->udd[0] = cfg->udd[0];
    dout->udd[1] = cfg->udd[1];
    dout->udd[2] = cfg->udd[2];
    dout->udd[3] = cfg->udd[3];
    dout->endpointNotifyObj = cfg->iop.endpointNotifyObj;
    dout->endpointNotifyFn = cfg->iop.endpointNotifyFn;
    dout->idd = NIL;
    dout->tag = cfg->tag;

    // canvas
    //dout->extent = AvxMaxRange(AVX_RANGE(1, 1, 1), cfg->extent);
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : avxColorSpace_STANDARD; // sRGB is the default
    dout->ccfg = cfg->canvas;
    AvxConfigureCanvas(dsys, &dout->ccfg);
    dout->wwOverHw = dout->ccfg.whd.w / dout->ccfg.whd.h;

    if (!dout->ccfg.slotCnt)
    {
        AFX_ASSERT(dout->ccfg.slotCnt);
        AfxThrowError();
    }
#if 0
    for (afxUnit i = 0; i < dout->ccfg.annexCnt; i++)
    {
        avxCanvasAnnex* a = &dout->ccfg.annexes[i];
    }
#endif
    // swapchain
    dout->bufCnt = AFX_MAX(1, AFX_MIN(cfg->minBufCnt, def.minBufCnt)); // 2 or 3; double or triple buffered for via-memory presentation.

    dout->area = AVX_RECT(0, 0, dout->ccfg.whd.w, dout->ccfg.whd.h);
    dout->dstArea = AVX_RECT_ZERO;
    dout->persistBlit = FALSE;

    dout->submCnt = 0;
    dout->presentingBufIdx = AFX_INVALID_INDEX;
    dout->suspendCnt = 1;
    AfxDeployFutex(&dout->suspendSlock);

    AfxDeployMutex(&dout->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dout->idleCnd);

    dout->buffers = NIL;

    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->buffers[0]),
            .var = (void**)&dout->buffers
        }
    };

    if (AfxAllocateInstanceData(dout, ARRAY_SIZE(stashs), stashs))
    {
        AfxThrowError();
    }
    AfxZero(dout->buffers, sizeof(dout->buffers[0]) * dout->bufCnt);

    AfxDeployInterlockedQueue(&dout->freeBuffers, sizeof(afxUnit), dout->bufCnt);

    for (afxUnit i = 0; i < dout->bufCnt; i++)
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

    AFX_ASSERT(dout->bufCnt);
    AFX_ASSERT(dout->ccfg.slots[0].usage & avxRasterUsage_DRAW);
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

_AVX afxClassConfig const _AVX_DOUT_CLASS_CONFIG =
{
    .fcc = afxFcc_DOUT,
    .name = "DrawOutput",
    .desc = "Draw Output Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxSurface)),
    .ctor = (void*)_AvxDoutCtorCb,
    .dtor = (void*)_AvxDoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxUnit AvxEnumerateSurfaces(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxSurface outputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(!cnt || outputs);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AvxEvokeSurfaces(afxDrawSystem dsys, afxBool(*f)(afxSurface, void*), void* udd, afxUnit first, afxUnit cnt, afxSurface outputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(outputs);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AvxInvokeSurfaces(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxSurface, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxError AvxConfigureSurface(afxDrawSystem dsys, afxSurfaceConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);
    *cfg = AVX_DEFAULT_SURFACE_CONFIG;
    return err;
}

_AVX afxError AvxOpenSurface(afxDrawSystem dsys, afxSurfaceConfig const* cfg, afxSurface* output)
// file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->doutCls(dsys);
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
    afxError err = AFX_ERR_NONE;
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
        if (bufxIdx >= dout->bufCnt)
        {
            AFX_ASSERT_RANGE(dout->bufCnt, bufxIdx, 1);
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
