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
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

AFXINL afxDrawSystem _AfxGetDsysData(void);

_AFX afxNat _AfxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat bufferLockCnt = dout->bufferLockCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = ++dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = --dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxError _AfxDoutFreeAllBuffers(afxDrawOutput dout)
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
_AFX afxError AfxResetDrawOutputAuxiliarBuffers(afxDrawOutput dout, afxPixelFormat depth, afxPixelFormat stencil)
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
_AFX afxError AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxNat auxSurfCnt, afxSurfaceConfig const auxSurfs[], afxCanvas canv[])
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

_AFX afxDrawContext AfxGetDrawOutputContext(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext dctx = dout->dctx;
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AFX afxBool AfxDrawOutputIsOnline(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return !!(AfxGetDrawOutputContext(dout));
}

_AFX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
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

_AFX afxError AfxDisconnectDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawOutputIsOnline(dout));
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// BUFFER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat baseBufIdx, afxNat bufCnt, afxCanvas canvases[])
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

_AFX afxNat AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat baseBufIdx, afxNat bufCnt, afxRaster rasters[])
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

_AFX afxNat AfxGetDrawOutputCapacity(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->bufCnt;
}

_AFX afxError AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
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

_AFX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[])
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

_AFX afxError AfxRegenerateDrawOutputBuffers(afxDrawOutput dout)
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
// EXTENT                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX void AfxGetDrawOutputNormalizedExtent(afxDrawOutput dout, afxV3d whd) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    AfxSetV3d(whd, AfxToNdc(dout->whd[0], dout->resolution[0]), AfxToNdc(dout->whd[1], dout->resolution[1]), (afxReal)1);
}

_AFX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    whd[0] = dout->whd[0];
    whd[1] = dout->whd[1];
    whd[2] = dout->whd[2];
}

_AFX afxError AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const whd)
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

        AfxReadjustDrawOutputProportion(dout, dout->wpOverHp, dout->resolution);

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

_AFX afxError AfxReadjustDrawOutputNormalized(afxDrawOutput dout, afxV3d const whd) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    afxWhd const whd2 = { (afxNat)AfxFromNdc(whd[0], dout->resolution[0]), (afxNat)AfxFromNdc(whd[1], dout->resolution[1]), 1 };
    return AfxReadjustDrawOutput(dout, whd2);
}

_AFX void AfxReadjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution)
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

_AFX afxDrawDevice AfxGetDrawOutputDevice(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetObjectProvider(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxOpenDrawOutputs(afxNat ddevId, afxNat cnt, afxDrawOutputConfig const cfg[], afxDrawOutput dout[]) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(ddev = AfxGetDrawDevice(ddevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = AfxGetDrawOutputClass(ddev);
        AfxAssertClass(cls, afxFcc_DOUT);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)dout, (void const*[]) { ddev, cfg }))
            AfxThrowError();

        AfxAssertObjects(cnt, dout, afxFcc_DOUT);
    }
    return err;
}

_AFX afxNat AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    afxClass* cls;

    if (ddev)
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        cls = AfxGetDrawInputClass(ddev);        
    }
    else
    {
        afxDrawSystem dsys = _AfxGetDsysData();
        AfxAssertType(dsys, afxFcc_DSYS);
        cls = &dsys->doutputs;
    }
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dout);
    afxClass* cls;

    if (ddev)
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        cls = AfxGetDrawOutputClass(ddev);
    }
    else
    {
        afxDrawSystem dsys = _AfxGetDsysData();
        AfxAssertType(dsys, afxFcc_DSYS);
        cls = &dsys->doutputs;
    }
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)dout);
}

_AFX afxNat AfxCountDrawOutputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxCountInstances(cls);
}
