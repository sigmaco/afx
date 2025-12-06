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
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "avxIcd.h"

_AVX afxError _AvxDoutImplIoctl2Cb(afxSurface dout, afxUnit code, afxUnit inSiz, void* in, afxUnit outCap, void* out, afxUnit32* outSiz, avxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    return err;
}

_AVX afxError _AvxDoutImplIoctlCb(afxSurface dout, afxUnit code, va_list ap)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    return err;
}

_AVX afxError _AvxDoutImplAdjustCb(afxSurface dout, afxRect const* area, afxBool fse)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxInt swdiff = dout->area.w - dout->srcArea.w;
    afxInt shdiff = dout->area.h - dout->srcArea.h;
    afxInt dwdiff = dout->area.w - dout->dstArea.w;
    afxInt dhdiff = dout->area.h - dout->dstArea.h;

    dout->area.x = area->x;
    dout->area.y = area->y;
    dout->area.w = AFX_MAX(1, area->w);
    dout->area.h = AFX_MAX(1, area->h);
    dout->srcArea.w = dout->area.w + swdiff;
    dout->srcArea.h = dout->area.h + shdiff;
    dout->dstArea.w = dout->area.w + dwdiff;
    dout->dstArea.h = dout->area.h + dhdiff;
    dout->wwOverHw = (afxReal64)dout->area.w / (afxReal64)dout->area.h;
    dout->ccfg.whd = AvxMaxRange(AVX_RANGE(1, 1, 1), AVX_RANGE(dout->area.w, dout->area.h, 1));

    return err;
}

_AVX afxError _AvxDoutImplRegenBuffers(afxSurface dout, afxBool build)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AvxWaitForSurface(dout, 0);

    afxDrawSystem dsys = AvxGetSurfaceHost(dout);

    afxBool again = 1;

    while (again--)
    {
        for (afxUnit i = 0; i < dout->swapCnt; i++)
        {
            _avxSurfaceSwap* swap = &dout->swaps[i];
            ++swap->locked;

            // If any yeild have occuried, we must reiterate every buffer again
            // because it can be again resized in this interval.

            if (swap->locked != 1)
            {
                again = 1;
                while (swap->locked != 1) AfxYield();
            }

            if (swap->canv)
            {
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &swap->canv);
                AfxDisposeObjects(1, &swap->canv);
                AFX_ASSERT(!swap->canv);
                swap->ras = NIL;
            }

            if (swap->fenc)
            {
                AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &swap->fenc);
                AfxDisposeObjects(1, &swap->fenc);
                AFX_ASSERT(!swap->fenc);
                swap->fencNextValue = 1;
            }

            if (!build)
                continue;

            if (!swap->fenc)
            {
                avxFenceInfo fenci = { 0 };
                fenci.initialVal = 0;
                fenci.tag = AFX_STRING("dout-swap");
                fenci.udd = dout;
                if (AvxAcquireFences(dsys, 1, &fenci, &swap->fenc))
                {
                    AfxThrowError();
                }
                else
                {
                    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &swap->fenc);
                }
            }

            if (AvxAcquireCanvas(dsys, &dout->ccfg, 1, &swap->canv))
            {
                AfxThrowError();

                // delete buffers?
            }
            else
            {
                avxCanvas canv = swap->canv;
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
#if 0
                afxUnit objId = AfxGetObjectId(canv);
                afxClass const* cls = _AvxDsysGetCanvClassCb_SW(dsys);

                avxCanvas canv2 = NIL;
                AfxEnumerateObjects(cls, objId, 1, (afxObject*)&canv2);
                afxUnit objId2 = AfxGetObjectId(canv2);
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv2);
                AFX_ASSERT(canv == canv2);
#endif

                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
                avxRaster ras;
                AvxGetDrawBuffers(canv, 0, 1, &ras);
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

#if 0
                avxCanvas canv3 = NIL;
                AfxEnumerateObjects(cls, objId, 1, (afxObject*)&canv3);
                afxUnit objId3 = AfxGetObjectId(canv3);
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv3);
                AFX_ASSERT(canv == canv3);
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
#endif

                AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                swap->ras = ras;

                afxLayeredRect rc;
                AvxGetCanvasExtent(canv, NIL, &rc);
                swap->bounds = rc;
            }

            --swap->locked;
        }
    }
    return err;
}

_AVX afxError _AvxDoutImplLockBufferCb(afxSurface dout, afxMask exuMask, avxFence signal, afxUnit64 timeout, afxUnit* bufIdx)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxUnit bufIdx2 = AFX_INVALID_INDEX;

    afxBool success = FALSE;
    afxClock timeoutStart, last;
    afxBool wait = (timeout != AFX_TIMEOUT_IGNORED);
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
        afxInt64 bufGenPeriod = AFX_NANOSECS_PER_SECS(1) / dout->bufReqPerSec;

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
            _avxSurfaceSwap* swap = &dout->swaps[lockedBufIdx];
            avxCanvas canv = swap->canv;

            if (canv)
            {
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
            }
            //swap->lockedCanv = canv;
            AFX_ASSERT(swap->locked == 0);
            ++swap->locked;
            swap->exuMask = exuMask;
            swap->fencNextValue = AvxGetCompletedFenceValue(swap->fenc) + 1;
            bufIdx2 = lockedBufIdx;
            AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
            AFX_ASSERT_RANGE(dout->swapCnt, bufIdx2, 1);
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
                    timeout -= ((afxInt64)timeout > elapsed) ? elapsed : 0;
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

_AVX afxError _AvxDoutImplUnlockBufferCb(afxSurface dout, afxUnit bufIdx)
{
    afxError err = { 0 };
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    _avxSurfaceSwap* swap = &dout->swaps[bufIdx];
    if (!swap->locked)
    {
        AfxThrowError();
        err = afxError_BAD_COMMAND;
        return err;
    }

    if (!AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { bufIdx }))
    {
        AfxThrowError();
        err = afxError_TOO_MANY_OBJECTS;
        return err;
    }
    
#if 0
    avxCanvas lockedCanv = swap->lockedCanv;

    if ((lockedCanv != swap->canv))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &lockedCanv);
        AfxDisposeObjects(1, &lockedCanv);
    }
#endif
    
    //swap->lockedCanv = NIL;

    // Bumped up by _AvxDoutImplLockBufferCb.
    --swap->locked;

    return err;
}

_AVX afxError _AvxDquePresentBuffers(afxDrawQueue dque, afxUnit cnt, avxPresentation presentations[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxPresentation* pres = &presentations[i];

        afxSurface dout = pres->dout;
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
#if 0
        if (pres->waitOnDpu && pres->dout->ddi->presOnDpuCb)
        {
            _AvxDqueSubmitCallback(dque, (void*)pres->dout->ddi->presOnDpuCb, pres);
        }
        else
#endif
        {
            //AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
            //dout->presentingBufIdx = pres->bufIdx;
#if 0
            if (pres->waitOnDpu)
            {
                while (AvxWaitForFences(AfxGetHost(pres->dout), AFX_TIMEOUT_INFINITE, FALSE, 1, &pres->waitOnDpu, NIL))
                {
                    AfxYield();
                }
            }
#endif
            if (dout->ddi->presentCb && dout->ddi->presentCb(dque, pres))
                AfxThrowError();

            dout->lastPresentedBufIdx = (afxAtom32)pres->bufIdx;

            afxClock currClock;
            AfxGetClock(&currClock);
            ++dout->outNo;

            if ((1.0 <= AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)))
            {
                dout->outCntResetClock = currClock;
                dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
                dout->outNo = 0;

                afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
                afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
                dout->lastClock = currClock;

                if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
                {
                    //AfxFormatWindowTitle(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
                }

                if (dout->endpointNotifyFn)
                {
                    dout->endpointNotifyFn(dout->endpointNotifyObj, pres->bufIdx);
                }
            }

            --dout->swaps[pres->bufIdx].locked;
            AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { pres->bufIdx });
            //AfxDecAtom32(&dout->m.submCnt);
        }
    }
    return err;
}

_AVX _avxDdiDout const _AVX_DDI_DOUT =
{
    .ioctlCb = _AvxDoutImplIoctlCb,
    .adjustCb = _AvxDoutImplAdjustCb,
    .presentCb = NIL,
    .lockCb = _AvxDoutImplLockBufferCb,
    .unlockCb = _AvxDoutImplUnlockBufferCb,
    .regenCb = _AvxDoutImplRegenBuffers
};
