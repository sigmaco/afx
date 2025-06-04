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
#include "impl/avxImplementation.h"

_AVX afxDrawOutputConfig const AVX_DEFAULT_DRAW_OUTPUT_CONFIG =
{
    .colorSpc = avxColorSpace_STANDARD,
    .bufFmt[0] = avxFormat_BGRA8v,
    .bufFmt[1] = avxFormat_D32f,
    .bufFmt[2] = NIL,
    .bufUsage[0] = avxRasterUsage_DRAW | avxRasterUsage_RESAMPLE,
    .bufUsage[1] = avxRasterUsage_DRAW,
    .bufUsage[2] = avxRasterUsage_DRAW,
    .bufFlags[0] = NIL,
    .bufFlags[1] = NIL,
    .bufFlags[2] = NIL,
    .extent = { 1, 1, 1 },
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

_AVX afxUnit _AvxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, TRUE);
    afxUnit suspendCnt = dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, TRUE);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, FALSE);
    afxUnit suspendCnt = ++dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, FALSE);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxLockFutex(&dout->suspendSlock, FALSE);
    afxUnit suspendCnt = --dout->suspendCnt;
    AfxUnlockFutex(&dout->suspendSlock, FALSE);
    return suspendCnt;
}

_AVX afxError _AvxDoutFreeAllBuffers(afxDrawOutput dout)
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

_AVX afxDrawSystem AvxGetDrawOutputContext(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AfxGetProvider(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxModule AvxGetDrawOutputIcd(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AvxGetDrawOutputContext(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AvxGetDrawSystemIcd(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_ICD, 1, &icd);
    return icd;
}

_AVX void* AvxGetDrawOutputUdd(afxDrawOutput dout, afxUnit slotIdx)
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

_AVX void AvxDescribeDrawOutput(afxDrawOutput dout, afxDrawOutputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(cfg);
    *cfg = (afxDrawOutputConfig) { 0 };
    cfg->bufFmt[0] = dout->bufFmt[0];
    cfg->bufFmt[1] = dout->bufFmt[1];
    cfg->bufFmt[2] = dout->bufFmt[2];
    cfg->bufUsage[0] = dout->bufUsage[0];
    cfg->bufUsage[1] = dout->bufUsage[1];
    cfg->bufUsage[2] = dout->bufUsage[2];
    cfg->bufFlags[0] = dout->bufFlags[0];
    cfg->bufFlags[1] = dout->bufFlags[1];
    cfg->bufFlags[2] = dout->bufFlags[2];
    cfg->colorSpc = dout->colorSpc;
    cfg->doNotClip = dout->doNotClip;
    cfg->endpointNotifyFn = dout->endpointNotifyFn;
    cfg->endpointNotifyObj = dout->endpointNotifyObj;
    cfg->minBufCnt = dout->bufCnt;
    cfg->presentAlpha = dout->presentAlpha;
    cfg->presentMode = dout->presentMode;
    cfg->presentTransform = dout->presentTransform;
    cfg->udd[0] = dout->udd[0];
    cfg->udd[1] = dout->udd[1];
    cfg->udd[2] = dout->udd[2];
    cfg->udd[3] = dout->udd[3];
    cfg->extent = dout->extent;
    cfg->resolution = dout->resolution;
    cfg->refreshRate = dout->refreshRate;
}

_AVX void AvxGetDrawOutputRate(afxDrawOutput dout, afxUnit* rate)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(rate);
    *rate = dout->outRate;
}

_AVX void AvxQueryDrawOutputResolution(afxDrawOutput dout, afxReal64* wpOverHp, afxReal* refreshRate, avxRange* resolution, afxReal64* wrOverHr)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(wpOverHp || refreshRate || resolution || wrOverHr);

    if (wpOverHp)
        *wpOverHp = dout->wpOverHp;

    if (refreshRate)
        *refreshRate = dout->refreshRate;

    if (resolution)
        *resolution = dout->resolution;

    if (wrOverHr)
        *wrOverHr = dout->wrOverHr;
}

_AVX void AvxQueryDrawOutputExtent(afxDrawOutput dout, avxRange* extent, afxReal64* wwOverHw)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(extent || wwOverHw);

    if (extent)
        *extent = dout->extent;

    if (wwOverHw)
        *wwOverHw = dout->wwOverHw;
}

_AVX void _AvxDoutGetExtentNormalized(afxDrawOutput dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(whd);
    avxRange whd2;
    AvxQueryDrawOutputExtent(dout, &whd2, NIL);
    AfxV3dSet(whd, AfxNdcf(whd2.w, dout->resolution.w), AfxNdcf(whd2.h, dout->resolution.h), (afxReal)1);
}

_AVX afxError AvxAdjustDrawOutput(afxDrawOutput dout, avxRange whd)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    avxRange whd2;
    //AFX_ASSERT4(whd, whd.w, whd.h, whd.d);
    whd2.w = AFX_MAX(whd.w, 1);
    whd2.h = AFX_MAX(whd.h, 1);
    whd2.d = AFX_MAX(whd.d, 1);
    AFX_ASSERT_EXTENT(dout->resolution.w, whd.w);
    AFX_ASSERT_EXTENT(dout->resolution.h, whd.h);
    AFX_ASSERT_EXTENT(dout->resolution.d, whd.d);

    afxBool changed = (whd2.w != dout->extent.w || whd2.h != dout->extent.h || whd2.d != dout->extent.d);

    if (changed)
    {
        if (dout->pimpl->adjust(dout, whd2))
            AfxThrowError();

        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        if (!err)
        {
            afxV2d ndc;
            whd2;
            AvxQueryDrawOutputExtent(dout, &whd2, NIL);
            AfxV2dNdc(ndc, AFX_V2D(whd2.w, whd2.h), AFX_V2D(dout->resolution.w, dout->resolution.h));
#ifdef _AFX_DOUT_LOGS
            AfxReportMessage("Draw output %03u adjusted. %ux%u %ux%u %.3fx%.3f %f", AfxGetObjectId(dout), whd2.w, whd2.h, dout->resolution.w, dout->resolution.h, ndc[0], ndc[1], dout->wwOverHw);
#endif
        }

        --dout->resizing;
    }
    return err;
}

_AVX afxError _AvxDoutAdjustNormalized(afxDrawOutput dout, afxV3d const whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT4(whd, whd[0], whd[1], whd[2]);

    avxRange const whd2 =
    {
        (afxUnit)AfxUnndcf(AFX_MAX(1, whd[0]), dout->resolution.w),
        (afxUnit)AfxUnndcf(AFX_MAX(1, whd[1]), dout->resolution.h),
        AFX_MAX(1, whd[2])
    };
    return AvxAdjustDrawOutput(dout, whd2);
}

_AVX afxError AvxModifyDrawOutputSettings(afxDrawOutput dout, afxReal64 physAspectRatio, afxReal refreshRate, avxRange resolution, afxBool exclusive)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    
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
    AfxReportMessage("afxDrawOutput %03u readapted. %ux%u @ %fHz <%fr, %fp>", AfxGetObjectId(dout), dout->resolution.w, dout->resolution.h, dout->refreshRate, dout->wrOverHr, dout->wpOverHp);
#endif

    // readjust draw output extent if it overflows the new resolution.

    avxRange whd;
    AvxQueryDrawOutputExtent(dout, &whd, NIL);
    afxBool readjust = FALSE;
    
    if (whd.w > dout->resolution.w)
        whd.w = dout->resolution.w, readjust = TRUE;

    if (whd.h > dout->resolution.h)
        whd.h = dout->resolution.h, readjust = TRUE;

    if (whd.d > dout->resolution.d)
        whd.d = dout->resolution.d, readjust = TRUE;

    if (readjust)
        _AvxDoutAdjustNormalized(dout, whdNdc);

    return err;
}

_AVX afxError AvxPrintDrawOutput(afxDrawOutput dout, afxUnit bufIdx, avxRasterIo const* op, afxUnit exuIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    
    avxCanvas canv;
    if (!AvxGetDrawOutputCanvas(dout, bufIdx, &canv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    if (AvxPrintDrawBuffer(canv, 0, op, exuIdx, uri))
        AfxThrowError();

    return err;
}

// BUFFERIZATION ///////////////////////////////////////////////////////////////

_AVX afxError AvxWaitForDrawOutput(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxTimeSpec ts =
    {
        .secs = timeout / 1000000000, // Convert nanoseconds to seconds
        .nsecs = timeout % 1000000000 // Get the remainder in nanoseconds
    };

    AfxLockMutex(&dout->idleCndMtx);    
    while (dout->submCnt)
        AfxWaitTimedCondition(&dout->idleCnd, &dout->idleCndMtx, &ts);
    AfxUnlockMutex(&dout->idleCndMtx);

    return err;
}

// Pull an available draw output buffer (usually from the WSI).

_AVX afxError AvxRequestDrawOutputBuffer(afxDrawOutput dout, afxUnit64 timeout, afxSemaphore sem, avxFence fenc, afxMask exuMask, afxUnit* bufIdx)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    
    afxUnit bufIdx2 = AFX_INVALID_INDEX;

    if (dout->pimpl->reqBuf)
    {
        err = dout->pimpl->reqBuf(dout, timeout, sem, fenc, exuMask, &bufIdx2);

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
    afxBool finite = (timeout != AFX_TIME_INFINITE);

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

_AVX afxError AvxRecycleDrawOutputBuffer(afxDrawOutput dout, afxUnit bufIdx)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);

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

    if (dout->pimpl->recycBuf)
    {
        if (dout->pimpl->recycBuf(dout, bufIdx))
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

_AVX afxBool AvxGetDrawOutputCanvas(afxDrawOutput dout, afxUnit bufIdx, avxCanvas* canvas)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    avxCanvas canv = NIL;

    if (!(bufIdx < dout->bufCnt)) AfxThrowError();
    else if ((canv = dout->buffers[bufIdx].canv))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    }
    AFX_ASSERT(canvas);
    *canvas = canv;
    return !!canv;
}

_AVX afxBool AvxGetDrawOutputBuffer(afxDrawOutput dout, afxUnit bufIdx, avxRaster* buffer)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    AFX_ASSERT(bufIdx != AFX_INVALID_INDEX);
    avxRaster ras = NIL;
    avxCanvas canv;

    if (AvxGetDrawOutputCanvas(dout, bufIdx, &canv))
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

_AVX afxError AvxRevalidateDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AvxWaitForDrawOutput(dout, 0);

    avxDrawSurface surCfgs[3] =
    {
        {
            .fmt = dout->bufFmt[0],
            .sampleCnt = 1,
            .rasUsage = dout->bufUsage[0],
            .rasFlags = dout->bufFlags[0]
        },
        {
            .fmt = dout->bufFmt[1],
            .sampleCnt = 1,
            .rasUsage = dout->bufUsage[1],
            .rasFlags = dout->bufFlags[1]
        },
        {
            .fmt = dout->bufFmt[2],
            .sampleCnt = 1,
            .rasUsage = dout->bufUsage[2],
            .rasFlags = dout->bufFlags[2]
        }
    };

    afxUnit surCnt = 1;

    if (dout->bufFmt[1])
        ++surCnt;

    if (dout->bufFmt[2])
        ++surCnt;

    avxCanvasConfig ccfg =
    {
        .flags = NIL,
        .whd = dout->extent,
        .surCnt = surCnt,
        .surfs =
        {
            {
                .ras = NIL,
                .fmt = dout->bufFmt[0],
                .rasUsage = dout->bufUsage[0],
                .rasFlags = dout->bufFlags[0],
                .sampleCnt = 1
            },
            {
                .ras = NIL,
                .fmt = dout->bufFmt[1],
                .rasUsage = dout->bufUsage[1],
                .rasFlags = dout->bufFlags[1],
                .sampleCnt = 1
            },
            {
                .ras = NIL,
                .fmt = dout->bufFmt[2],
                .rasUsage = dout->bufUsage[2],
                .rasFlags = dout->bufFlags[2],
                .sampleCnt = 1
            }
        }
    };

    afxDrawSystem dsys = AvxGetDrawOutputContext(dout);

    for (afxUnit i = 0; i < dout->bufCnt; i++)
    {
        _avxDoutBuffer* slot = &dout->buffers[i];
        ++slot->locked;

        if (slot->canv)
        {
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &slot->canv);
            AfxDisposeObjects(1, &slot->canv);
            AFX_ASSERT(!slot->canv);
            slot->ras = FALSE;
        }

        if (AvxCoacquireCanvas(dsys, &ccfg, 1, &slot->canv))
        {
            AfxThrowError();
            
            // delete buffers?
        }
        else
        {
            avxCanvas canv = slot->canv;
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
            avxRaster ras;
            AvxGetDrawBuffers(canv, 0, 1, &ras);
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            slot->ras = ras;
        }

        --slot->locked;
    }
    return err;
}

_AVX afxError AvxCallDrawOutput(afxDrawOutput dout, afxUnit code, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(dout->pimpl);
    AFX_ASSERT(dout->pimpl->ioctl);

    va_list ap;
    va_start(ap, code);
    afxError rslt = dout->pimpl->ioctl(dout, code, ap);
    va_end(ap);

    return rslt;
}

_AVX afxError _AvxDoutDtorCb(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxDrawOutput handle.
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

_AVX afxError _AvxDoutCtorCb(afxDrawOutput dout, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(dout, sizeof(dout[0]));

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawOutputConfig const* cfg = ((afxDrawOutputConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    afxDrawOutputConfig def;
    AvxConfigureDrawOutput(dsys, &def);

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
    dout->endpointNotifyObj = cfg->endpointNotifyObj;
    dout->endpointNotifyFn = cfg->endpointNotifyFn;
    dout->idd = NIL;

    // canvas
    //dout->extent = AvxMaxRange(AVX_RANGE(1, 1, 1), cfg->extent);
    dout->extent = AVX_RANGE(1, 1, 1);
    dout->wwOverHw = dout->extent.w / dout->extent.h;
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : avxColorSpace_STANDARD; // sRGB is the default
    dout->bufFmt[0] = cfg->bufFmt[0] ? cfg->bufFmt[0] : def.bufFmt[0]; // or avxFormat_RGBA8R ?
    dout->bufFmt[1] = cfg->bufFmt[1] ? cfg->bufFmt[1] : def.bufFmt[1];
    dout->bufFmt[2] = cfg->bufFmt[2] ? cfg->bufFmt[2] : def.bufFmt[2];
    dout->bufUsage[0] = cfg->bufUsage[0] ? (cfg->bufUsage[0] | avxRasterUsage_DRAW) : def.bufUsage[0];
    dout->bufUsage[1] = cfg->bufUsage[1] ? (cfg->bufUsage[1] | avxRasterUsage_DRAW) : def.bufUsage[1];
    dout->bufUsage[2] = cfg->bufUsage[2] ? (cfg->bufUsage[2] | avxRasterUsage_DRAW) : def.bufUsage[2];
    dout->bufFlags[0] = cfg->bufFlags[0] ? cfg->bufFlags[0] : def.bufFlags[0];
    dout->bufFlags[1] = cfg->bufFlags[1] ? cfg->bufFlags[1] : def.bufFlags[1];
    dout->bufFlags[2] = cfg->bufFlags[2] ? cfg->bufFlags[2] : def.bufFlags[2];
    
    // swapchain
    dout->bufCnt = AFX_MAX(1, cfg->minBufCnt ? cfg->minBufCnt : def.minBufCnt); // 2 or 3; double or triple buffered for via-memory presentation.

    dout->submCnt = 0;
    dout->presentingBufIdx = AFX_INVALID_INDEX;
    dout->suspendCnt = 1;
    AfxDeployFutex(&dout->suspendSlock);

    AfxDeployMutex(&dout->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dout->idleCnd);

    dout->lockCb = NIL;
    dout->buffers = NIL;

    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->buffers[0]),
            .var = (void**)&dout->buffers
        }
    };

    if (AfxAllocateInstanceData(dout, ARRAY_SIZE(stashs), stashs)) AfxThrowError();
    else
    {
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
        AFX_ASSERT_EXTENT(dout->resolution.w, dout->extent.w);
        AFX_ASSERT_EXTENT(dout->resolution.h, dout->extent.h);
        AFX_ASSERT_EXTENT(dout->resolution.d, dout->extent.d);

        AFX_ASSERT(dout->bufCnt);
        AFX_ASSERT(dout->bufUsage[0] & avxRasterUsage_DRAW);
        AFX_ASSERT(dout->refreshRate);
        AFX_ASSERT(dout->wpOverHp);
        AFX_ASSERT(dout->wrOverHr);
        AFX_ASSERT(dout->wwOverHw);
        
        // provoke buffer geneartion
        AvxAdjustDrawOutput(dout, cfg->extent);

        if (err)
            AfxDeallocateInstanceData(dout, ARRAY_SIZE(stashs), stashs);
    }
    return err;
}

_AVX afxClassConfig const _AVX_DOUT_CLASS_CONFIG =
{
    .fcc = afxFcc_DOUT,
    .name = "DrawOutput",
    .desc = "Draw Output Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawOutput)),
    .ctor = (void*)_AvxDoutCtorCb,
    .dtor = (void*)_AvxDoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxUnit AvxEnumerateDrawOutputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(!cnt || outputs);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AvxEvokeDrawOutputs(afxDrawSystem dsys, afxBool(*f)(afxDrawOutput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(outputs);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)outputs);
}

_AVX afxUnit AvxInvokeDrawOutputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->doutCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DOUT);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxError AvxConfigureDrawOutput(afxDrawSystem dsys, afxDrawOutputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);
    *cfg = AVX_DEFAULT_DRAW_OUTPUT_CONFIG;
    return err;
}

_AVX afxError AvxOpenDrawOutput(afxDrawSystem dsys, afxDrawOutputConfig const* cfg, afxDrawOutput* output)
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

    afxDrawOutput dout;
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

AFX_DEFINE_STRUCT(avxPresent)
{
    // --- VkDeviceGroupPresentInfoKHR
    // Mode and mask controlling which physical devices' images are presented



    // --- VkSwapchainPresentFenceInfoEXT

    avxFence const* completionSignal;

    // --- VkDisplayPresentInfoKHR
    // Structure describing parameters of a queue presentation to a swapchain

    // srcRect is a rectangular region of pixels to present. 
    // It must be a subset of the image being presented. 
    // If VkDisplayPresentInfoKHR is not specified, this region will be assumed to be the entire presentable image.
    afxRect srcRect;

    // dstRect is a rectangular region within the visible region of the swapchain's display mode.
    // If VkDisplayPresentInfoKHR is not specified, 
    // this region will be assumed to be the entire visible region of the swapchain's mode.
    // If the specified rectangle is a subset of the display mode's visible region, 
    // content from display planes below the swapchain's plane will be visible outside the rectangle.
    // If there are no planes below the swapchain's, the area outside the specified rectangle will be black.
    // If portions of the specified rectangle are outside of the display's visible region, 
    // pixels mapping only to those portions of the rectangle will be discarded.
    afxRect dstRect;

    // persistent, when TRUE, the display engine will enable buffered mode on displays that support it. 
    // This allows the display engine to stop sending content to the display until a new image is presented. 
    // The display will instead maintain a copy of the last presented image. 
    // This allows less power to be used, but may increase presentation latency. 
    // If VkDisplayPresentInfoKHR is not specified, persistent mode will not be used.
    afxBool persistent;
};

_AVX afxError AvxPresentDrawOutputs(afxDrawSystem dsys, avxPresentation* ctrl, avxFence wait, afxUnit cnt, afxDrawOutput outputs[], afxUnit const bufIdx[], avxFence signal[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(ctrl);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, AFX_INVALID_INDEX, NIL, 0, 0, NIL);
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxDrawBridge dexu;
        if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }

        afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

        // sanitize arguments
        afxUnit totalQueCnt = dqueCls->instCnt;
        afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
        afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
        AFX_ASSERT(queCnt);
        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit queIt = 0; queIt < queCnt; queIt++)
            {
                afxUnit queIdx = baseQueIdx + queIt;

                afxDrawQueue dque;
                if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque))
                {
                    AfxThrowError();
                    break;
                }
                AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

                for (afxUnit j = 0; j < cnt; j++)
                {
                    afxDrawOutput dout = outputs[j];
                    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

                    afxError err2 = _AvxDoutImplPresentCb(dque, ctrl, wait, dout, bufIdx[queIt], signal ? signal[queIt] : NIL);
                    
                    if (!err2)
                    {
                        queued = TRUE;
                        break; // for
                    }
                    
                    if (err2 == afxError_TIMEOUT)
                        continue; // for

                    AfxThrowError();
                    break; // for
                }
            }

            if (err || queued)
                break; // while --- reiterate if not queue for timeout?
        }
    }
    return err;
}
