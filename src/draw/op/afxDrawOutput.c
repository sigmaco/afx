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
#include "../impl/avxImplementation.h"

_AVX afxUnit _AvxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxUnit suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxUnit suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxUnit _AvxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxUnit suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxError _AvxDoutFreeAllBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    if (dout->canvases)
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, dout->bufCnt, dout->canvases);
        AfxDisposeObjects(dout->bufCnt, dout->canvases);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawSystem AfxGetDrawOutputContext(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AfxGetProvider(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxModule AfxGetDrawOutputIcd(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AfxGetDrawOutputContext(dout);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AfxGetDrawSystemIcd(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_ICD, 1, &icd);
    return icd;
}

_AVX afxError AfxGetDrawOutputIdd(afxDrawOutput dout, afxUnit code, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    dout->iddCb(dout, code, dst);
    return err;
}

_AVX void* AfxGetDrawOutputUdd(afxDrawOutput dout, afxUnit slotIdx)
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

_AVX void AfxDescribeDrawOutput(afxDrawOutput dout, afxDrawOutputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(cfg);
    *cfg = (afxDrawOutputConfig) { 0 };
    cfg->bufFlags = dout->bufFlags;
    cfg->bufFlagsDs[0] = dout->bufFlagsDs[0];
    cfg->bufFlagsDs[1] = dout->bufFlagsDs[1];
    cfg->bufUsage = dout->bufUsage;
    cfg->bufUsageDs[0] = dout->bufUsageDs[0];
    cfg->bufUsageDs[1] = dout->bufUsageDs[1];
    cfg->colorSpc = dout->colorSpc;
    cfg->doNotClip = dout->doNotClip;
    cfg->endpointNotifyFn = dout->endpointNotifyFn;
    cfg->endpointNotifyObj = dout->endpointNotifyObj;
    cfg->minBufCnt = dout->bufCnt;
    cfg->pixelFmt = dout->pixelFmt;
    cfg->pixelFmtDs[0] = dout->pixelFmtDs[0];
    cfg->pixelFmtDs[1] = dout->pixelFmtDs[1];
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

_AVX void AfxGetDrawOutputFrequency(afxDrawOutput dout, afxUnit* rate)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(rate);
    *rate = dout->outRate;
}

_AVX void AfxQueryDrawOutputResolution(afxDrawOutput dout, afxReal64* wpOverHp, afxReal* refreshRate, afxWhd* resolution, afxReal64* wrOverHr)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
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

_AVX void AfxQueryDrawOutputExtent(afxDrawOutput dout, afxWhd* extent, afxReal64* wwOverHw)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(extent || wwOverHw);

    if (extent)
        *extent = dout->extent;

    if (wwOverHw)
        *wwOverHw = dout->wwOverHw;
}

_AVX void _AvxGetDrawOutputExtentNormalized(afxDrawOutput dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT(whd);
    afxWhd whd2;
    AfxQueryDrawOutputExtent(dout, &whd2, NIL);
    AfxV3dSet(whd, AfxNdcf(whd2.w, dout->resolution.w), AfxNdcf(whd2.h, dout->resolution.h), (afxReal)1);
}

_AVX afxError AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxWhd whd2;
    //AfxAssert4(whd, whd.w, whd.h, whd.d);
    whd2.w = AfxMax(whd.w, 1);
    whd2.h = AfxMax(whd.h, 1);
    whd2.d = AfxMax(whd.d, 1);
    AfxAssertExtent(dout->resolution.w, whd.w);
    AfxAssertExtent(dout->resolution.h, whd.h);
    AfxAssertExtent(dout->resolution.d, whd.d);

    afxBool changed = (whd2.w != dout->extent.w || whd2.h != dout->extent.h || whd2.d != dout->extent.d);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        if (dout->adjustCb)
        {
            if (dout->adjustCb(dout, whd2))
                AfxThrowError();
        }
        else if (_AfxAdjustDrawOutput(dout, whd2))
            AfxThrowError();

        if (!err)
        {
            afxV2d ndc;
            whd2;
            AfxQueryDrawOutputExtent(dout, &whd2, NIL);
            AfxNdcV2d(ndc, AFX_V2D(whd2.w, whd2.h), AFX_V2D(dout->resolution.w, dout->resolution.h));
#ifdef _AFX_DOUT_LOGS
            AfxLogEcho("Draw output %03u adjusted. %ux%u %ux%u %.3fx%.3f %f", AfxGetObjectId(dout), whd2.w, whd2.h, dout->resolution.w, dout->resolution.h, ndc[0], ndc[1], dout->wwOverHw);
#endif
        }

        --dout->resizing;
    }
    return err;
}

_AVX afxError _AvxAdjustDrawOutputNormalized(afxDrawOutput dout, afxV3d const whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AfxAssert4(whd, whd[0], whd[1], whd[2]);

    afxWhd const whd2 =
    {
        (afxUnit)AfxUnndcf(AfxMax(1, whd[0]), dout->resolution.w),
        (afxUnit)AfxUnndcf(AfxMax(1, whd[1]), dout->resolution.y),
        AfxMax(1, whd[2])
    };
    return AfxAdjustDrawOutput(dout, whd2);
}

_AVX afxError AfxResetDrawOutputResolution(afxDrawOutput dout, afxReal64 physAspectRatio, afxReal refreshRate, afxWhd resolution, afxBool exclusive)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    
    afxV3d whdNdc;
    _AvxGetDrawOutputExtentNormalized(dout, whdNdc);
    
    if (AfxWhdDot(resolution, resolution))
    {

        AfxAssert3(resolution.w, resolution.h, resolution.d);
        AfxWhdMax(&dout->resolution, AfxWhd(1, 1, 1), resolution);
        dout->wrOverHr = (afxReal64)dout->resolution.w / (afxReal64)dout->resolution.h;
    }

    if (refreshRate)
        dout->refreshRate = refreshRate;

    if (physAspectRatio)
        dout->wpOverHp = physAspectRatio;

#ifdef _AVX_DOUT_LOGS
    AfxLogEcho("afxDrawOutput %03u readapted. %ux%u @ %fHz <%fr, %fp>", AfxGetObjectId(dout), dout->resolution.w, dout->resolution.h, dout->refreshRate, dout->wrOverHr, dout->wpOverHp);
#endif

    // readjust draw output extent if it overflows the new resolution.

    afxWhd whd;
    AfxQueryDrawOutputExtent(dout, &whd, NIL);
    afxBool readjust = FALSE;
    
    if (whd.w > dout->resolution.w)
        whd.w = dout->resolution.w, readjust = TRUE;

    if (whd.h > dout->resolution.h)
        whd.h = dout->resolution.h, readjust = TRUE;

    if (whd.d > dout->resolution.d)
        whd.d = dout->resolution.d, readjust = TRUE;

    if (readjust)
        _AvxAdjustDrawOutputNormalized(dout, whdNdc);

    return err;
}

_AVX afxError AfxPrintDrawOutput(afxDrawOutput dout, afxUnit bufIdx, afxUnit exuIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    
    avxCanvas canv;
    if (!AfxGetDrawOutputCanvas(dout, bufIdx, &canv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    if (AfxPrintDrawBuffer(canv, 0, NIL, exuIdx, uri))
        AfxThrowError();

    return err;
}

// BUFFERIZATION ///////////////////////////////////////////////////////////////

_AVX afxError AfxWaitForDrawOutput(afxDrawOutput dout, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AfxLockMutex(&dout->idleCndMtx);
    
    afxTimeSpec ts =
    {
        .sec = timeout / 1000000000, // Convert nanoseconds to seconds
        .nsec = timeout % 1000000000 // Get the remainder in nanoseconds
    };

    while (dout->submCnt)
        AfxWaitTimedCondition(&dout->idleCnd, &dout->idleCndMtx, &ts);

    AfxUnlockMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxUnit *bufIdx)
// Pull an available draw output buffer (usually from the WSI).
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxUnit bufIdx2 = AFX_INVALID_INDEX;

    if (dout->lockCb)
    {
        if (!(err = dout->lockCb(dout, timeout, &bufIdx2)))
        {
            AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
            AFX_ASSERT_RANGE(dout->bufCnt, bufIdx2, 1);
        }
    }
    else
    {
        afxBool success = FALSE;
        afxTime time, t2;
        AfxGetTime(&time);

        afxClock start, last;

        if (timeout)
        {
            AfxGetClock(&start);
            last = start;
        }

        while (1)
        {
            afxUnit lockedBufIdx = AFX_INVALID_INDEX;

            if (AfxPopInterlockedQueue(&dout->freeBuffers, &lockedBufIdx))
            {
                avxCanvas canv = dout->canvases[lockedBufIdx];

                if (canv)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
                }
                bufIdx2 = lockedBufIdx;
                success = TRUE;
            }

            if (success)
                break;

            if (!success && (!timeout || timeout < AfxGetTime(&t2) - time))
            {
                err = __LINE__;
                bufIdx2 = AFX_INVALID_INDEX;
                break;
            }
        }
    }
    AFX_ASSERT(bufIdx);
    *bufIdx = bufIdx2;
    return err;
}

_AVX afxError AfxRecycleDrawOutputBuffer(afxDrawOutput dout, afxUnit bufIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    
    if (bufIdx < dout->bufCnt)
        AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { bufIdx });
    else
        AfxThrowError();

    return err;
}

_AVX afxBool AfxGetDrawOutputCanvas(afxDrawOutput dout, afxUnit bufIdx, avxCanvas* canvas)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    avxCanvas canv = NIL;

    if (bufIdx >= dout->bufCnt) AfxThrowError();
    else if ((canv = dout->canvases[bufIdx]))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    }
    AFX_ASSERT(canvas);
    *canvas = canv;
    return !!canv;
}

_AVX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxUnit bufIdx, afxRaster* buffer)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    AFX_ASSERT_RANGE(dout->bufCnt, bufIdx, 1);
    afxRaster ras = NIL;
    avxCanvas canv;

    if (AfxGetDrawOutputCanvas(dout, bufIdx, &canv))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

        if (AfxGetDrawBuffers(canv, 0, 1, &ras))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
        }
    }
    AFX_ASSERT(buffer);
    *buffer = ras;
    return !!ras;
}

_AVX afxError AfxRevalidateDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_TRY_ASSERT_OBJECTS(afxFcc_CANV, dout->bufCnt, dout->canvases);
    AfxDisposeObjects(dout->bufCnt, dout->canvases);
    AFX_ASSERT(!dout->canvases[0]);
    afxDrawSystem dsys = AfxGetDrawOutputContext(dout);
        
    AfxWaitForDrawOutput(dout, 0);

    afxSurfaceConfig surCfgs[3] =
    {
        {
            .fmt = dout->pixelFmt,
            .sampleCnt = 1,
            .rasUsage = dout->bufUsage,
            .rasFlags = dout->bufFlags
        },
        {
            .fmt = dout->pixelFmtDs[0],
            .sampleCnt = 1,
            .rasUsage = dout->bufUsageDs[0],
            .rasFlags = dout->bufFlagsDs[0]
        },
        {
            .fmt = dout->pixelFmtDs[1],
            .sampleCnt = 1,
            .rasUsage = dout->bufUsageDs[1],
            .rasFlags = dout->bufFlagsDs[1]
        }
    };

    afxUnit surCnt = 1;

    if (dout->pixelFmtDs[0])
        ++surCnt;

    if (dout->pixelFmtDs[1])
        ++surCnt;

    if (AfxCoacquireCanvas(dsys, dout->extent, surCnt, surCfgs, dout->bufCnt, dout->canvases))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CANV, dout->bufCnt, dout->canvases);

    for (afxUnit i = 0; i < dout->bufCnt; i++)
    {
        avxCanvas canv = dout->canvases[i];
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        AfxRedoDrawBuffers(canv);
    }
    return err;
}

_AVX afxError _AvxDoutDtorCb(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    //dout->disabled;

    AFX_ASSERT(!dout->idd);

    _AvxDoutSuspendFunction(dout);
    
    _AvxDoutFreeAllBuffers(dout);
    
    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->canvases[0]),
            .var = (void**)&dout->canvases
        }
    };

    AfxDeallocateInstanceData(dout, ARRAY_SIZE(stashs), stashs);

    AfxDismantleInterlockedQueue(&dout->freeBuffers);

    AfxDismantleSlock(&dout->suspendSlock);

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
    AfxConfigureDrawOutput(dsys, &def);

    // endpoint
    dout->resizing = FALSE;
    dout->resizable = TRUE;
    dout->refreshRate = 1;
    //AFX_ASSERT(AFX_IS_ALIGNED(&dout->resolution, 16));
    dout->resolution = AfxWhd(AFX_N32_MAX, AFX_N32_MAX, AFX_N32_MAX);
    dout->wrOverHr = dout->resolution.w / dout->resolution.h;
    dout->wpOverHp = AfxFindPhysicalAspectRatio(dout->resolution.w, dout->resolution.h);
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
    AfxWhdMax(&dout->extent, AfxWhd(1, 1, 1), cfg->extent);
    dout->wwOverHw = dout->extent.w / dout->extent.h;
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : avxColorSpace_STANDARD; // sRGB is the default
    dout->pixelFmt = cfg->pixelFmt ? cfg->pixelFmt : avxFormat_BGRA8; // or avxFormat_RGBA8R ?
    dout->bufUsage = cfg->bufUsage ? (cfg->bufUsage | afxRasterUsage_DRAW) : def.bufUsage;
    dout->bufFlags = cfg->bufFlags ? cfg->bufFlags : def.bufFlags;
    dout->pixelFmtDs[0] = cfg->pixelFmtDs[0] ? cfg->pixelFmtDs[0] : def.pixelFmtDs[0];
    dout->pixelFmtDs[1] = cfg->pixelFmtDs[1] ? cfg->pixelFmtDs[1] : def.pixelFmtDs[1];
    dout->bufUsageDs[0] = cfg->bufUsageDs[0] ? (cfg->bufUsageDs[0] | afxRasterUsage_DRAW) : def.bufUsageDs[0];
    dout->bufUsageDs[1] = cfg->bufUsageDs[1] ? (cfg->bufUsageDs[1] | afxRasterUsage_DRAW) : def.bufUsageDs[1];
    dout->bufFlagsDs[0] = cfg->bufFlagsDs[0] ? cfg->bufFlagsDs[0] : def.bufFlagsDs[0];
    dout->bufFlagsDs[1] = cfg->bufFlagsDs[1] ? cfg->bufFlagsDs[1] : def.bufFlagsDs[1];
    
    // swapchain
    dout->bufCnt = AfxMax(1, cfg->minBufCnt ? cfg->minBufCnt : def.minBufCnt); // 2 or 3; double or triple buffered for via-memory presentation.

    dout->submCnt = 0;
    dout->presentingBufIdx = AFX_INVALID_INDEX;
    dout->suspendCnt = 1;
    AfxDeploySlock(&dout->suspendSlock);

    AfxDeployMutex(&dout->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dout->idleCnd);

    dout->lockCb = NIL;
    dout->canvases = NIL;

    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->canvases[0]),
            .var = (void**)&dout->canvases
        }
    };

    if (AfxAllocateInstanceData(dout, ARRAY_SIZE(stashs), stashs)) AfxThrowError();
    else
    {
        AfxZero(dout->canvases, sizeof(dout->canvases[0]) * dout->bufCnt);

        AfxDeployInterlockedQueue(&dout->freeBuffers, sizeof(afxUnit), dout->bufCnt);

        for (afxUnit i = 0; i < dout->bufCnt; i++)
            AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { i });

        AFX_ASSERT(dout->resolution.w);
        AFX_ASSERT(dout->resolution.h);
        AFX_ASSERT(dout->resolution.d);
        AfxAssertExtent(dout->resolution.w, dout->extent.w);
        AfxAssertExtent(dout->resolution.h, dout->extent.h);
        AfxAssertExtent(dout->resolution.d, dout->extent.d);

        AFX_ASSERT(dout->bufCnt);
        AFX_ASSERT(dout->bufUsage & afxRasterUsage_DRAW);
        AFX_ASSERT(dout->refreshRate);
        AFX_ASSERT(dout->wpOverHp);
        AFX_ASSERT(dout->wrOverHr);
        AFX_ASSERT(dout->wwOverHw);
        

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

_AVX afxError AfxConfigureDrawOutput(afxDrawSystem dsys, afxDrawOutputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);
    *cfg = (afxDrawOutputConfig const) { 0 };
    cfg->minBufCnt = 3;
    cfg->bufUsage = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
    cfg->bufUsageDs[0] = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
    cfg->bufUsageDs[1] = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
    cfg->colorSpc = avxColorSpace_STANDARD;
    cfg->pixelFmt = avxFormat_BGRA8;
    cfg->pixelFmtDs[0] = avxFormat_D32f;
    cfg->pixelFmtDs[1] = NIL;
    return err;
}

_AVX afxError AfxOpenDrawOutput(afxDrawSystem dsys, afxDrawOutputConfig const* cfg, afxDrawOutput* output)
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

    afxClass* cls = (afxClass*)_AvxGetDrawOutputClass(dsys);
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
