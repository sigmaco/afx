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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_OUTPUT_IMPL
#include "dev/AvxDevKit.h"

_AVX afxNat _AvxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat lockedBufCnt = AfxLoadAtom32(&dout->lockedBufCnt);
    AfxExitSlockShared(&dout->buffersLock);
    return lockedBufCnt;
}

_AVX afxNat _AvxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat lockedBufCnt = AfxIncAtom32(&dout->lockedBufCnt);
    AfxExitSlockExclusive(&dout->buffersLock);
    return lockedBufCnt;
}

_AVX afxNat _AvxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat lockedBufCnt = AfxDecAtom32(&dout->lockedBufCnt);
    AfxExitSlockExclusive(&dout->buffersLock);
    return lockedBufCnt;
}

_AVX afxNat _AvxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxNat _AvxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxNat _AvxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxError _AvxDoutFreeAllBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        avxCanvas canv = dout->buffers[i].canv;
        //afxRaster ras = dout->buffers[i].ras;

        if (/*!ras*/!canv)
        {
            AfxAssert(_AvxDoutIsSuspended(dout)); // dout sem surfaces é inoperante
        }
        else
        {
            AfxAssertObjects(1, &canv, afxFcc_CANV);
            AfxReleaseObjects(1, &canv);
            dout->buffers[i].canv = NIL;

            if (dout->buffers[i].readySem)
                AfxReleaseObjects(1, &dout->buffers[i].readySem), dout->buffers[i].readySem = NIL;
        }
    }

    //dout->bufCnt = 0;
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// UTILITY                                                                    //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxGetDrawOutputIdd(afxDrawOutput dout, afxNat code, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    dout->iddCb(dout, code, dst);
    return err;
}

_AVX void** AfxGetDrawOutputUdd(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->udd;
}

_AVX afxNat AfxStampDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxSemaphore wait, afxV2d const origin, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxDrawContext dctx;

    if (!AfxGetDrawOutputContext(dout, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxPresentationRequest req;
        req.bufIdx = bufIdx;
        req.dout = dout;
        req.wait = wait;

        afxDrawBridge ddge;
        AfxGetDrawBridge(dctx, portIdx, &ddge);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueStampRequest(ddge, 1, &req, origin, caption)))
            AfxThrowError();
    }
    return queIdx;
}

_AVX afxNat AfxPresentDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxSemaphore wait)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxDrawContext dctx;
    
    if (!AfxGetDrawOutputContext(dout, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxPresentationRequest req;
        req.bufIdx = bufIdx;
        req.dout = dout;
        req.wait = wait;

        afxDrawBridge ddge;
        AfxGetDrawBridge(dctx, portIdx, &ddge);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        if (AFX_INVALID_INDEX == (queIdx = AfxEnqueuePresentRequest(ddge, 1, &req)))
            AfxThrowError();
    }
    return queIdx;
}

_AVX afxError AfxPrintDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    avxCanvas canv;

    if (!AfxGetDrawOutputCanvas(dout, bufIdx, &canv)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);

        if (AfxPrintDrawBuffer(canv, 0, NIL, uri))
            AfxThrowError();
    }
    return err;
}

// CONNECTION //////////////////////////////////////////////////////////////////

_AVX afxBool AfxGetDrawOutputContext(afxDrawOutput dout, afxDrawContext* context)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext dctx = AfxGetLinker(&dout->dctx);

    if (dctx)
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        AfxAssert(context);
        *context = dctx;
    }
    return !!dctx;
}

_AVX afxError AfxDisconnectDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    afxDrawContext dctx;
    AfxAssert(!AfxGetDrawOutputContext(dout, &dctx));
    return err;
}

_AVX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    
    AfxLockMutex(&ddev->relinkedCndMtx);

    while (ddev->relinking)
        AfxWaitCondition(&ddev->relinkedCnd, &ddev->relinkedCndMtx);

    afxDrawContext from = NIL;
    AfxGetDrawOutputContext(dout, &from);

    if (dctx != from)
    {
        ddev->relinking = TRUE;
        dout->reconnecting = TRUE;

        if (from)
        {
            AfxAssertObjects(1, &from, afxFcc_DCTX);

            afxDrawOutput found;
            AfxIterateLinkageB2F(AFX_OBJECT(afxDrawOutput), found, &from->outputs, dctx)
            {
                AfxAssertObjects(1, &found, afxFcc_DOUT);

                if (found == dout)
                {
                    do
                    {
                        AfxAssertObjects(1, &from, afxFcc_DCTX);

                        if (AfxWaitForDrawContext(from))
                            AfxThrowError();

                    } while (AfxLoadAtom32(&dout->submCnt));

                    AfxAssertObjects(1, &dout, afxFcc_DOUT);

                    if (ddev->relinkDoutCb && ddev->relinkDoutCb(ddev, NIL, 1, &dout)) // if disconnection was refused
                    {
                        AfxThrowError();
                    }
                    else
                    {
                        AfxPopLinkage(&dout->dctx);
                        AfxRedoDrawOutputBuffers(dout);
                        AfxAssertObjects(1, &from, afxFcc_DCTX);
                        AfxReleaseObjects(1, &from);
                    }
                    break;
                }
            }
            AfxAssert(found == dout);
        }

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, afxFcc_DDEV);

            if (ddev != ddrv2) // can not connect to context acquired of another device
            {
                AfxThrowError();
                dctx = NIL; // so just disconnect
            }
            else
            {
                AfxReacquireObjects(1, &dctx);
                AfxPushLinkage(&dout->dctx, &dctx->outputs);

                AfxAssertObjects(1, &dout, afxFcc_DOUT);

                if (ddev->relinkDoutCb && ddev->relinkDoutCb(ddev, dctx, 1, &dout)) // if reconnection was refused
                {
                    AfxThrowError();
                    AfxPopLinkage(&dout->dctx);
                    AfxReleaseObjects(1, &dctx);
                }
                else
                {
                    AfxRedoDrawOutputBuffers(dout);
                }
            }
        }

        dout->reconnecting = FALSE;
        ddev->relinking = FALSE;
    }

    AfxUnlockMutex(&ddev->relinkedCndMtx);
    AfxSignalCondition(&ddev->relinkedCnd);
    return !err;
}

// BUFFERIZATION ///////////////////////////////////////////////////////////////

_AVX afxNat AfxCountDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->bufCnt;
}

_AVX afxError AfxLockDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
// Pull an available draw output buffer (usually from the WSI).
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat bufIdx2 = AFX_INVALID_INDEX;

    if (AfxLoadAtom32(&dout->reconnecting)) AfxThrowError();
    else
    {
        if (dout->lockCb)
        {
            if (!(err = dout->lockCb(dout, timeout, &bufIdx2)))
            {
                AfxAssert(AFX_INVALID_INDEX != bufIdx2);
                AfxAssertRange(dout->bufCnt, bufIdx2, 1);
            }
        }
        else
        {

        }
    }
    AfxAssert(bufIdx);
    *bufIdx = bufIdx2;
    return err;
}

_AVX afxNat AfxEnumerateDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, avxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat totalBufCnt = dout->bufCnt;
    AfxAssertRange(totalBufCnt, first, cnt);
    afxNat rslt = 0;

    if (totalBufCnt > first)
    {
        AfxAssert(canvases);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxNat bufIdx = first + i;
            AfxAssertRange(totalBufCnt, bufIdx, 1);
            avxCanvas canv;

            if ((canv = dout->buffers[bufIdx].canv))
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);
                ++rslt;
            }
            canvases[i] = canv;
        }
    }
    return rslt;
}

_AVX afxBool AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat bufIdx, avxCanvas* canvas)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxBool rslt;
    avxCanvas canv;
    while ((rslt = AfxEnumerateDrawOutputCanvases(dout, bufIdx, 1, &canv)))
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);
        AfxAssert(canvas);
        *canvas = canv;
        break;
    }
    return rslt;
}

_AVX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat totalBufCnt = dout->bufCnt;
    AfxAssertRange(totalBufCnt, first, cnt);
    afxNat rslt = 0;

    if (totalBufCnt > first)
    {
        AfxAssert(rasters);
    
        for (afxNat i = 0; i < cnt; i++)
        {
            afxNat bufIdx = first + i;
            AfxAssertRange(totalBufCnt, bufIdx, 1);
            afxRaster ras = NIL;
            avxCanvas canv;

            if (AfxGetDrawOutputCanvas(dout, bufIdx, &canv))
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);

                if (AfxEnumerateDrawBuffers(canv, 0, 1, &ras))
                {
                    AfxAssertObjects(1, &ras, afxFcc_RAS);
                    ++rslt;
                }
            }
            rasters[i] = ras;
        }
    }
    return rslt;
}

_AVX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxRaster* raster)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxBool rslt;
    afxRaster ras;
    while ((rslt = AfxEnumerateDrawOutputBuffers(dout, bufIdx, 1, &ras)))
    {
        AfxAssertObjects(1, &ras, afxFcc_RAS);
        AfxAssert(raster);
        *raster = ras;
        break;
    }
    return rslt;
}

_AVX afxError AfxRedoDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!dout->buffers)
    {
        AfxAssert(dout->bufCnt);

        if ((dout->buffers = AfxAllocate(dout->bufCnt, sizeof(dout->buffers[0]), 0, AfxHere())))
            AfxZero2(dout->bufCnt, sizeof(dout->buffers[0]), dout->buffers);
        else
            AfxThrowError();
    }

    if (!err)
    {
        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
            avxCanvas canv = dout->buffers[i].canv;

            if (canv)
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);
                AfxReleaseObjects(1, &canv);
                dout->buffers[i].canv = NIL;
            }

            afxSemaphore sem = dout->buffers[i].readySem;

            if (sem)
            {
                AfxAssertObjects(1, &sem, afxFcc_SEM);
                AfxReleaseObjects(1, &sem);
                dout->buffers[i].readySem = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetDrawOutputContext(dout, &dctx))
            {
                //afxEvent ev;
                //AfxEventDeploy(&ev, AFX_EVENT_DOUT_REBUF, &dout->obj, NIL);
                //ev.udd[0] = (void*)i; // from
                //AfxNotifyObject(&dout->obj, &ev);

                afxSurfaceConfig surCfg[3] =
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

                afxNat surCnt = 1;

                if (dout->pixelFmtDs[0])
                    ++surCnt;

                if (dout->pixelFmtDs[1])
                    ++surCnt;
                    
                if (AfxAcquireCanvases(dctx, dout->whd, surCnt, surCfg, 1, &canv)) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &canv, afxFcc_CANV);
                    dout->buffers[i].canv = canv;

                    AfxRedoDrawBuffers(canv);

                    if (AfxAcquireSemaphores(dctx, 1, &dout->buffers[i].readySem))
                        AfxThrowError();
                }
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX void AfxGetDrawOutputAspectRatio(afxDrawOutput dout, afxReal64* wpOverHp, afxReal64* wrOverHr, afxReal64* wwOverHw)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert3(wpOverHp, wrOverHr, wwOverHw);
    *wpOverHp = dout->wpOverHp;
    *wrOverHr = dout->wrOverHr;
    *wwOverHw = dout->wwOverHw;
}

_AVX void AfxGetDrawOutputResolution(afxDrawOutput dout, afxWhd res, afxReal* refreshRate)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(res);
    res[0] = dout->res[0];
    res[1] = dout->res[1];
    res[2] = dout->res[2];
    AfxAssert(refreshRate);
    *refreshRate = dout->refreshRate;
}

_AVX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    whd[0] = dout->whd[0];
    whd[1] = dout->whd[1];
    whd[2] = dout->whd[2];
}

_AVX void AfxGetDrawOutputExtentAsNdc(afxDrawOutput dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    afxWhd whd2;
    AfxGetDrawOutputExtent(dout, whd2);
    AfxSetV3d(whd, AfxNdcf(whd2[0], dout->res[0]), AfxNdcf(whd2[1], dout->res[1]), (afxReal)1);
}

_AVX afxError AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    
    afxWhd whd2;
    AfxAssert4(whd, whd[0], whd[1], whd[2]);
    whd2[0] = AfxMax(whd[0], 1);
    whd2[1] = AfxMax(whd[1], 1);
    whd2[2] = AfxMax(whd[2], 1);
    AfxAssertExtent(dout->res[0], whd[0]);
    AfxAssertExtent(dout->res[1], whd[1]);
    AfxAssertExtent(dout->res[2], whd[2]);

    afxBool changed = (whd2[0] != dout->whd[0] || whd2[1] != dout->whd[1] || whd2[2] != dout->whd[2]);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        AfxAssert3(whd2[0], whd2[1], whd2[2]);
        dout->whd[0] = AfxMax(whd2[0], 1);
        dout->whd[1] = AfxMax(whd2[1], 1);
        dout->whd[2] = AfxMax(whd2[2], 1);
        dout->wwOverHw = (afxReal64)dout->whd[0] / (afxReal64)dout->whd[1];

        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->whd);
        //AfxNotifyObject(&dout->obj, &ev);

        afxV2d ndc;
        AfxGetDrawOutputExtent(dout, whd2);
        AfxNdcV2d(ndc, AfxSpawnV2d(whd2[0], whd2[1]), AfxSpawnV2d(dout->res[0], dout->res[1]));
        AfxLogEcho("afxDrawOutput %03u adjusted. <%u, %u, %.3f> %f <%u, %u, %.3f>", AfxGetObjectId(dout), whd2[0], dout->res[0], ndc[0], dout->wwOverHw, whd2[1], dout->res[1], ndc[1]);

        AfxRedoDrawOutputBuffers(dout);

        --dout->resizing;
    }
    return err;
}

_AVX afxError AfxAdjustDrawOutputFromNdc(afxDrawOutput dout, afxV3d const whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert4(whd, whd[0], whd[1], whd[2]);

    afxWhd const whd2 =
    {
        (afxNat)AfxUnndcf(AfxMax(1, whd[0]), dout->res[0]),
        (afxNat)AfxUnndcf(AfxMax(1, whd[1]), dout->res[1]),
        AfxMax(1, whd[2])
    };
    return AfxAdjustDrawOutput(dout, whd2);
}

_AVX afxBool AfxResetDrawOutputResolution(afxDrawOutput dout, afxWhd const res, afxReal refreshRate, afxReal64 physAspectRatio)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxBool rslt = TRUE;

    afxV3d whdNdc;
    AfxGetDrawOutputExtentAsNdc(dout, whdNdc);

    if (res)
    {
        AfxAssert3(res[0], res[1], res[2]);
        dout->res[0] = AfxMax(1, res[0]);
        dout->res[1] = AfxMax(1, res[1]);
        dout->res[2] = AfxMax(1, res[2]);
        dout->wrOverHr = (afxReal64)dout->res[0] / (afxReal64)dout->res[1];
    }

    if (refreshRate)
        dout->refreshRate = refreshRate;

    if (physAspectRatio)
        dout->wpOverHp = physAspectRatio;

    AfxLogEcho("afxDrawOutput %03u readapted. %ux%u @ %fHz <%fr, %fp>", AfxGetObjectId(dout), dout->res[0], dout->res[1], dout->refreshRate, dout->wrOverHr, dout->wpOverHp);

    // readjust draw output extent if it overflows the new resolution.
    
    afxWhd whd;
    afxBool readjust = FALSE;
    AfxGetDrawOutputExtent(dout, whd);

    if (whd[0] > dout->res[0])
        whd[0] = dout->res[0], readjust = TRUE;

    if (whd[1] > dout->res[1])
        whd[1] = dout->res[1], readjust = TRUE;

    if (whd[2] > dout->res[2])
        whd[2] = dout->res[2], readjust = TRUE;

    if (readjust)
        AfxAdjustDrawOutputFromNdc(dout, whdNdc);

    return rslt;
}

_AVX afxDrawDevice AfxGetDrawOutputDevice(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetLinker(&dout->ddev);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxError _AvxDoutDtorCb(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxDisconnectDrawOutput(dout);
    //dout->disabled;

    if (ddev->closeDoutCb && ddev->closeDoutCb(ddev, dout))
        AfxThrowError();

    AfxAssert(!dout->idd);

    _AvxDoutSuspendFunction(dout);
    _AvxDoutLockBuffers(dout);

    if (dout->buffers)
        AfxDeallocate(dout->buffers);

    AfxAssert(dout->swapchain.cnt == 0);

    AfxCleanUpSlock(&dout->buffersLock);
    AfxCleanUpSlock(&dout->suspendSlock);

    return err;
}

_AVX afxError _AvxDoutCtorCb(afxDrawOutput dout, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(dout, sizeof(dout[0]));

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxUri const* endpoint = cookie->udd[1];
    afxDrawOutputConfig const* cfg = ((afxDrawOutputConfig const *)cookie->udd[2]) + cookie->no;
    AfxAssert(cfg);
    
    AfxPushLinkage(&dout->ddev, &ddev->openedDoutChain);
    AfxPushLinkage(&dout->dctx, NIL);

    // endpoint
    dout->resizing = FALSE;
    dout->resizable = TRUE;
    dout->refreshRate = 1;
    dout->res[0] = 1;
    dout->res[1] = 1;
    dout->res[2] = 1;
    dout->wrOverHr = dout->res[0] / dout->res[1];
    dout->wpOverHp = AfxFindPhysicalAspectRatio(dout->res[0], dout->res[1]);
    dout->presentAlpha = cfg->presentAlpha; // consider transparency for window composing.
    dout->presentTransform = cfg->presentTransform; // The default is to leave it as it is.
    dout->presentMode = cfg->presentMode; // The default avxPresentMode_LIFO is already zero.
    dout->doNotClip = !!cfg->doNotClip; // don't spend resources doing off-screen draw.
    dout->udd[0] = cfg->udd[0];
    dout->udd[1] = cfg->udd[1];
    dout->udd[2] = cfg->udd[2];
    dout->udd[3] = cfg->udd[3];
    dout->endpointNotifyObj = cfg->endpointNotifyObj;
    dout->endpointNotifyFn = cfg->endpointNotifyFn;
    dout->idd = NIL;

    // canvas
    dout->whd[0] = AfxMax(1, cfg->whd[0]);
    dout->whd[1] = AfxMax(1, cfg->whd[1]);
    dout->whd[2] = AfxMax(1, cfg->whd[2]);
    dout->wwOverHw = dout->whd[0] / dout->whd[1];
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : avxColorSpace_SRGB; // sRGB is the default
    dout->pixelFmt = cfg->pixelFmt ? cfg->pixelFmt : afxPixelFormat_RGBA8; // or afxPixelFormat_RGBA8R ?
    dout->pixelFmtDs[0] = cfg->pixelFmtDs[0];
    dout->pixelFmtDs[1] = cfg->pixelFmtDs[1];
    dout->bufUsage = cfg->bufUsage | afxRasterUsage_DRAW;
    dout->bufUsageDs[0] = cfg->bufUsageDs[0] | afxRasterUsage_DRAW;
    dout->bufUsageDs[1] = cfg->bufUsageDs[1] | afxRasterUsage_DRAW;
    dout->bufFlags = cfg->bufFlags;
    dout->bufFlagsDs[0] = cfg->bufFlagsDs[0] | afxRasterUsage_DRAW;
    dout->bufFlagsDs[1] = cfg->bufFlagsDs[1] | afxRasterUsage_DRAW;
    
    // swapchain
    dout->bufCnt = AfxMax(1, cfg->bufCnt); // 2 or 3; double or triple buffered for via-memory presentation.

    dout->submCnt = 0;
    dout->lockedBufCnt = 0;
    dout->lastLockedBufIdx = dout->bufCnt - 1; // to start at 0 instead of 1 we set to last one.
    dout->lastUnlockedBufIdx = dout->bufCnt - 1;
    dout->presentingBufIdx = AFX_INVALID_INDEX;

    AfxSetUpChain(&dout->swapchain, (void*)dout);
    AfxSetUpSlock(&dout->buffersLock);

    dout->reconnecting = FALSE;

    dout->suspendCnt = 1;
    AfxSetUpSlock(&dout->suspendSlock);

    dout->lockCb = NIL;
    dout->buffers = NIL;

    AfxAssert(ddev->openDoutCb);

    if (ddev->openDoutCb(ddev, dout, cfg, endpoint)) AfxThrowError();
    else
    {
        AfxAssert(dout->res[0]);
        AfxAssert(dout->res[1]);
        AfxAssert(dout->res[2]);
        AfxAssertExtent(dout->res[0], dout->whd[0]);
        AfxAssertExtent(dout->res[1], dout->whd[1]);
        AfxAssertExtent(dout->res[2], dout->whd[2]);

        AfxAssert(dout->bufCnt);
        AfxAssert(dout->bufUsage & afxRasterUsage_DRAW);
        AfxAssert(dout->refreshRate);
        AfxAssert(dout->wpOverHp);
        AfxAssert(dout->wrOverHr);
        AfxAssert(dout->wwOverHw);
    }
    return err;
}

_AVX afxClassConfig const _AvxDoutClsCfg =
{
    .fcc = afxFcc_DOUT,
    .name = "DrawOutput",
    .desc = "Draw Output Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawOutput)),
    .ctor = (void*)_AvxDoutCtorCb,
    .dtor = (void*)_AvxDoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxOpenDrawOutput(afxNat ddevId, afxUri const* endpoint, afxDrawOutputConfig const* cfg, afxDrawOutput* output)
// file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = (afxClass*)AfxGetDrawOutputClass();
        AfxAssertClass(cls, afxFcc_DOUT);
        afxDrawOutput dout;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&dout, (void const*[]) { ddev, endpoint, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            AfxAssert(output);
            *output = dout;
        }
    }
    return err;
}

_AVX afxError AfxAcquireDrawOutput(afxNat ddevId, afxDrawOutputConfig const* cfg, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxDrawOutput dout;

    if (AfxOpenDrawOutput(ddevId, NIL, cfg, &dout)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxAssert(output);
        *output = dout;
    }
    return err;
}

_AVXINL afxBool _AvxTestDoutIsFromDdevFltCb(afxDrawOutput dout, afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return (ddev == AfxGetDrawOutputDevice(dout));
}

_AVXINL afxBool _AvxTestDoutIsFromDdevFlt2Cb(afxDrawOutput dout, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return (ddev == AfxGetDrawOutputDevice(dout)) && ((afxBool(*)(afxDrawOutput,void*))udd[1])(dout, udd[2]);
}

_AVX afxNat AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass const* cls = AfxGetDrawOutputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)outputs);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestDoutIsFromDdevFltCb, ddev, first, cnt, (afxObject*)outputs);
    }
    return rslt;
}

_AVX afxNat AfxEvokeDrawOutputs(afxDrawDevice ddev, afxBool(*flt)(afxDrawOutput, void*), void* fdd, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass const* cls = AfxGetDrawOutputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)outputs);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestDoutIsFromDdevFlt2Cb, (void*[]) { ddev, flt, fdd }, first, cnt, (afxObject*)outputs);
    }
    return rslt;
}

_AVX afxNat AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetDrawOutputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestDoutIsFromDdevFltCb, ddev, (void*)f, udd);
    }
    return rslt;
}

_AVX afxNat AfxInvokeDrawOutputs2(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*flt)(afxDrawOutput, void*), void *fdd, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetDrawOutputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)flt, fdd, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestDoutIsFromDdevFlt2Cb, (void*[]) { ddev, flt, fdd }, (void*)f, udd);
    }
    return rslt;
}
