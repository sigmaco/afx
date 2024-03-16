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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

AVXINL afxDrawSystem AfxGetDrawSystem(void);

_AVX afxNat _AfxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat bufferLockCnt = dout->bufferLockCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return bufferLockCnt;
}

_AVX afxNat _AfxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = ++dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AVX afxNat _AfxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = --dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AVX afxNat _AfxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxNat _AfxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxNat _AfxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxError _AfxDoutFreeAllBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        afxCanvas canv = dout->buffers[i].canv;
        //afxRaster ras = dout->buffers[i].ras;

        if (/*!ras*/!canv)
        {
            AfxAssert(_AfxDoutIsSuspended(dout)); // dout sem surfaces é inoperante
        }
        else
        {
            //AfxAssertObjects(1, &ras, afxFcc_RAS);
            AfxAssertObjects(1, &canv, afxFcc_CANV);
            //AfxReleaseObjects(1, (void*[]) { ras });
            AfxReleaseObjects(1, (void*[]) { canv });
            //dout->buffers[i].ras = NIL;
            dout->buffers[i].canv = NIL;

            if (dout->buffers[i].readySem)
                AfxReleaseObjects(1, (void*[]) { dout->buffers[i].readySem }), dout->buffers[i].readySem = NIL;
        }
    }

    //dout->bufCnt = 0;
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// UTILITY                                                                    //
////////////////////////////////////////////////////////////////////////////////

#if 0
_AVX afxError AfxResetDrawOutputAuxiliarBuffers(afxDrawOutput dout, afxPixelFormat depth, afxPixelFormat stencil)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (depth && !AfxPixelFormatIsDepth(depth))
        AfxThrowError();
    else
        dout->auxDsFmt[0] = depth;
    
    if (stencil && !AfxPixelFormatIsStencil(stencil))
        AfxThrowError();
    else
        dout->auxDsFmt[1] = stencil;

    AfxRegenerateDrawOutputBuffers(dout);

    return err;
}
#endif

#if 0
_AVX afxError AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxNat auxSurfCnt, afxSurfaceConfig const auxSurfs[], afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(canv);

    afxDrawContext dctx;

    if (!(AfxGetDrawOutputContext(dout, &dctx))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, afxFcc_DCTX);
        afxResult rslt = 0;

        for (afxNat i = 0; i < AfxMin(cnt, dout->bufCnt); i++)
        {
            afxRaster raster;
            
            if (!(raster = AfxGetDrawOutputBuffer(dout, first + i))) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &raster, afxFcc_RAS);
                //afxRaster ras = AfxGetSurfaceTexture(raster);

                afxWhd whd;
                AfxGetRasterExtent(raster, 0, whd);

                afxCanvasBlueprint blueprint;
                AfxBeginCanvasBlueprint(&blueprint, whd);
                AfxCanvasBlueprintSetDepth(&blueprint, dout->auxDsFmt[0], dout->auxDsFmt[1]);
                AfxCanvasBlueprintAddExistingRaster(&blueprint, raster);
                blueprint.udd[0] = (void*)dout;
                blueprint.udd[1] = (void*)((afxSize)i);

                if (AfxBuildCanvases(dctx, 1, &canv[i], &blueprint))
                {
                    AfxThrowError();

                    for (afxNat j = 0; j < i; j++)
                    {
                        //AfxObjectRemoveEventFilter(&dout->obj, &canv[j]->obj);
                        AfxReleaseObjects(1, (void*[]) { canv[j] });
                        canv[j] = NIL;
                    }
                    break;
                }
                else
                {
                    AfxAssertObjects(1, &canv[i], afxFcc_CANV);
                    //AfxObjectInstallEventFilter(&dout->obj, &canv[i]->obj);
                    rslt++;
                }
            }
        }
    }
    return err;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawContext AfxGetDrawOutputContext(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext dctx = dout->dctx;
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxBool AfxDrawOutputIsOnline(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return !!(AfxGetDrawOutputContext(dout));
}

_AVX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext curr = AfxGetDrawOutputContext(dout);

    if (dctx != curr)
    {
        afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, afxFcc_DDEV);

            if (ddev != ddrv2) // can not connect to context acquired of another device
                AfxThrowError();
        }

        if (!err)
        {
            if (ddev->relinkDout(ddev, dctx, 1, &dout))
                AfxThrowError();
        }
    }
    AfxAssert(dout->dctx == dctx);
    return !err;
}

_AVX afxError AfxDisconnectDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawOutputIsOnline(dout));
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat baseBufIdx, afxNat bufCnt, afxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, baseBufIdx, bufCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < bufCnt; i++)
    {
        afxNat bufIdx = baseBufIdx + i;
        AfxAssertRange(dout->bufCnt, bufIdx, 1);
        afxCanvas canv;

        if ((canv = dout->buffers[bufIdx].canv))
        {
            AfxAssertObjects(1, &canv, afxFcc_CANV);
            ++rslt;
        }
        canvases[i] = canv;
    }
    return rslt;
}

_AVX afxNat AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat baseBufIdx, afxNat bufCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, baseBufIdx, bufCnt);
    afxNat rslt = 0;

    for (afxNat i = 0; i < bufCnt; i++)
    {
        afxNat bufIdx = baseBufIdx + i;
        AfxAssertRange(dout->bufCnt, bufIdx, 1);
        afxRaster ras = NIL;
        afxCanvas canv;
        
        if (AfxGetDrawOutputCanvas(dout, baseBufIdx, bufCnt, &canv))
        {
            AfxAssertObjects(1, &canv, afxFcc_CANV);

            if (AfxGetDrawBuffers(canv, 0, 1, &ras))
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);
                ++rslt;
            }
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxNat AfxGetDrawOutputCapacity(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->bufCnt;
}

_AVX afxError AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    *bufIdx = AFX_INVALID_INDEX;

    if (dout->reqCb(dout, timeout, bufIdx)) AfxThrowError();
    else
    {
        AfxAssertRange(dout->bufCnt, *bufIdx, 1);
        AfxAssert(AFX_INVALID_INDEX != *bufIdx);
    }
    return err;
}

_AVX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(rasters);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat bufIdx = first + i;
        afxRaster ras = NIL;
        
        if (AfxGetDrawOutputBuffer(dout, bufIdx, 1, &ras))
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxError AfxRegenerateDrawOutputBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (dout->buffers)
    {
        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
#if 0
            afxRaster ras = dout->buffers[i].ras;

            if (ras)
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);
                AfxReleaseObjects(1, (void*[]) { ras });
                dout->buffers[i].ras = NIL;
            }
#endif       
            afxCanvas canv = dout->buffers[i].canv;

            if (canv)
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);
                AfxReleaseObjects(1, (void*[]) { canv });
                dout->buffers[i].canv = NIL;
            }

            afxSemaphore sem = dout->buffers[i].readySem;

            if (sem)
            {
                AfxAssertObjects(1, &sem, afxFcc_SEM);
                AfxReleaseObjects(1, (void*[]) { sem });
                dout->buffers[i].readySem = NIL;
            }

            afxDrawContext dctx;
            
            if ((dctx = AfxGetDrawOutputContext(dout)))
            {
                //AfxAssertType(dctxD, afxFcc_DCTX);
#if 0
                afxRaster ras2;
                afxRasterInfo texi = { 0 };
                texi.fmt = dout->pixelFmt;
                texi.whd[0] = dout->whd[0];
                texi.whd[1] = dout->whd[1];
                texi.whd[2] = dout->whd[2];
                texi.layerCnt = 1;
                texi.lodCnt = 1;
                texi.usage = dout->rasUsage;

                if (AfxAcquireRasters(dctx, 1, &texi, &ras2)) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &ras2, afxFcc_RAS);
                    dout->buffers[i].ras = ras2;
#endif
                    //afxEvent ev;
                    //AfxEventDeploy(&ev, AFX_EVENT_DOUT_REBUF, &dout->obj, NIL);
                    //ev.udd[0] = (void*)i; // from
                    //AfxObjectEmitEvent(&dout->obj, &ev);

                    afxSurfaceConfig surCfg[3] =
                    {
                        {
                            .fmt = dout->pixelFmt,
                            .sampleCnt = 1,
                            .rasUsage = dout->rasUsage,
                            .rasFlags = dout->rasFlags
                        },
                        {
                            .fmt = dout->auxDsFmt[0],
                            .sampleCnt = 1,
                            .rasUsage = dout->auxDsUsage[0],
                            .rasFlags = dout->auxDsFlags[0]
                        },
                        {
                            .fmt = dout->auxDsFmt[1],
                            .sampleCnt = 1,
                            .rasUsage = dout->auxDsUsage[1],
                            .rasFlags = dout->auxDsFlags[1]
                        }
                    };

                    afxNat surCnt = 1;

                    if (dout->auxDsFmt[0])
                        ++surCnt;

                    if (dout->auxDsFmt[1])
                        ++surCnt;
                    
                    if (AfxAcquireCanvases(dctx, dout->whd, 1, surCnt, surCfg, 1, &canv)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &canv, afxFcc_CANV);
                        dout->buffers[i].canv = canv;

                        AfxGenerateDrawBuffers(canv);
#if 0
                        if (AfxRelinkDrawBuffers(canv, 0, 1, &ras2))
                            AfxThrowError();
#endif
                        if (AfxAcquireSemaphores(dctx, 1, &dout->buffers[i].readySem))
                            AfxThrowError();
                    }
#if 0
                }
#endif
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX void AfxGetDrawOutputNormalizedExtent(afxDrawOutput dout, afxV3d whd) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    AfxSetV3d(whd, AfxToNdc(dout->whd[0], dout->resolution[0]), AfxToNdc(dout->whd[1], dout->resolution[1]), (afxReal)1);
}

_AVX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    whd[0] = dout->whd[0];
    whd[1] = dout->whd[1];
    whd[2] = dout->whd[2];
}

_AVX afxError AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    AfxAssertExtent(dout->resolution[0], whd[0]);
    AfxAssertExtent(dout->resolution[1], whd[1]);
    AfxAssertExtent(dout->resolution[2], whd[2]);

    afxBool changed = (dout->whd[0] != whd[0] || dout->whd[1] != whd[1] || dout->whd[2] != whd[2]);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        dout->whd[0] = AfxMax(whd[0], 1);
        dout->whd[1] = AfxMax(whd[1], 1);
        dout->whd[2] = AfxMax(whd[2], 1);

        AfxAdjustDrawOutputProportion(dout, dout->wpOverHp, dout->resolution);

        AfxRegenerateDrawOutputBuffers(dout);

        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->whd);
        //AfxObjectEmitEvent(&dout->obj, &ev);

        afxV3d whd2;
        AfxGetDrawOutputNormalizedExtent(dout, whd2);
        AfxEcho("Draw output (%p) whd readjusted to %ux%u; %.0f:%.0f%% of %ux%u.", dout, whd2[0], whd2[1], whd2[0] / dout->resolution[0], whd2[1] / dout->resolution[1], dout->resolution[0], dout->resolution[1]);

        --dout->resizing;
    }
    return err;
}

_AVX afxError AfxAdjustDrawOutputNormalized(afxDrawOutput dout, afxV3d const whd) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    afxWhd const whd2 = { (afxNat)AfxFromNdc(whd[0], dout->resolution[0]), (afxNat)AfxFromNdc(whd[1], dout->resolution[1]), 1 };
    return AfxAdjustDrawOutput(dout, whd2);
}

_AVX void AfxAdjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (resolution)
    {
        AfxAssert(resolution[0]);
        AfxAssert(resolution[1]);
        AfxAssert(resolution[2]);
        dout->resolution[0] = AfxMax(resolution[0], 1);
        dout->resolution[1] = AfxMax(resolution[1], 1);
        dout->resolution[2] = AfxMax(resolution[2], 1);
    }

    if (!physicalAspectRatio)
    {
        afxReal64 div = (afxReal64)dout->resolution[0] / (afxReal64)dout->resolution[1];

        if (div <= 1.4)
            physicalAspectRatio = 1.33;
        else if (div <= 1.6)
            physicalAspectRatio = 1.5599999;
        else
            physicalAspectRatio = 1.78;
    }

    dout->wpOverHp = physicalAspectRatio;
    dout->wrOverHr = (afxReal64)dout->resolution[0] / (afxReal64)dout->resolution[1];
    dout->wwOverHw = (afxReal64)dout->whd[0] / (afxReal64)dout->whd[1];
    AfxEcho("Draw output (%p) porportions readjusted. P %f, R %f W %f", dout, dout->wpOverHp, dout->wrOverHr, dout->wwOverHw);
}

_AVX afxDrawDevice AfxGetDrawOutputDevice(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetObjectProvider(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxOpenDrawOutputs(afxNat ddevId, afxNat cnt, afxDrawOutputConfig const cfg[], afxDrawOutput outputs[]) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(ddev = AfxGetDrawDevice(ddevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = AfxGetDrawOutputClass(ddev);
        AfxAssertClass(cls, afxFcc_DOUT);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)outputs, (void const*[]) { ddev, cfg }))
            AfxThrowError();

        AfxAssertObjects(cnt, outputs, afxFcc_DOUT);
    }
    return err;
}

_AVX afxError AfxStampDrawOutputBuffers(afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(req);
    AfxAssert(cnt);
    AfxAssert(fmt);

    va_list args;
    afxString4096 str;
    va_start(args, fmt);
    AfxMakeString4096(&str, NIL);
    afxNat len = AfxFormatStringArg(&str.str, fmt->start, args);
    va_end(args);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawOutput dout = req[i].dout;
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxAssertRange(dout->bufCnt, req[i].bufIdx, 1);
        AfxTryAssertObjects(1, &req[i].wait, afxFcc_SEM);
        afxDrawContext dctx;

        if (!(dctx = AfxGetDrawOutputContext(dout))) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);

            if (dctx->stampCb(dctx, 1, &req[i], origin, &str.str.str))
                AfxThrowError();
        }
    }
    return err;
}

_AVX afxError AfxPresentDrawOutputBuffers(afxNat cnt, afxPresentationRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(req);
    AfxAssert(cnt);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawOutput dout = req[i].dout;
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxAssertRange(dout->bufCnt, req[i].bufIdx, 1);
        AfxTryAssertObjects(1, &req[i].wait, afxFcc_SEM);
        afxDrawContext dctx;

        if (!(dctx = AfxGetDrawOutputContext(dout))) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);

            if (dctx->presentCb(dctx, 1, &req[i]))
                AfxThrowError();
        }
    }
    return err;
}

_AVX afxNat AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass* cls = AfxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxNat AfxCountDrawOutputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxCountInstances(cls);
}
