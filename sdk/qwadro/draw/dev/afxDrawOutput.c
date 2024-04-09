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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxNat _AvxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat bufferLockCnt = dout->bufferLockCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return bufferLockCnt;
}

_AVX afxNat _AvxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = ++dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AVX afxNat _AvxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = --dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
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
        afxCanvas canv = dout->buffers[i].canv;
        //afxRaster ras = dout->buffers[i].ras;

        if (/*!ras*/!canv)
        {
            AfxAssert(_AvxDoutIsSuspended(dout)); // dout sem surfaces é inoperante
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

    AfxRevalidateDrawOutputBuffers(dout);

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
            
            if (!(raster = AfxEnumerateDrawOutputBuffers(dout, first + i))) AfxThrowError();
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

_AVX afxError AfxStampDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxSemaphore wait, afxV2d const origin, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxDrawContext dctx;

    if (!AfxGetDrawOutputContext(dout, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxPresentationRequest req;
        req.bufIdx = bufIdx;
        req.dout = dout;
        req.wait = wait;

        afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        if (AfxEnqueueStampRequest(ddge, 1, &req, origin, caption))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxPresentDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxSemaphore wait)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxDrawContext dctx;

    if (!AfxGetDrawOutputContext(dout, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxPresentationRequest req;
        req.bufIdx = bufIdx;
        req.dout = dout;
        req.wait = wait;

        afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        if (AfxEnqueuePresentRequest(ddge, 1, &req))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxPrintDrawOutput(afxDrawOutput dout, afxNat bufIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxCanvas canv = dout->buffers[bufIdx].canv;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    
    if (AfxPrintDrawBuffer(canv, 0, uri))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AVX afxBool AfxGetDrawOutputContext(afxDrawOutput dout, afxDrawContext* dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext dctx2 = AfxGetLinker(&dout->dctx);
    AfxTryAssertObjects(1, &dctx2, afxFcc_DCTX);
    AfxAssert(dctx);
    *dctx = dctx2;
    return !!dctx2;
}

_AVX afxError AfxDisconnectDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    AfxAssert(!AfxGetLinker(&dout->dctx));
    return err;
}

_AVX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext curr;
    AfxGetDrawOutputContext(dout, &curr);

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
            if (AfxCallDevice((afxDevice)ddev, _ddevReqCode_DOUT_RLNK, dctx, 1, &dout))
                AfxThrowError();
        }
    }
    AfxAssert(AfxGetLinker(&dout->dctx) == dctx);
    return !err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxCountDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->bufCnt;
}

_AVX afxError AfxLockDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat bufIdx2 = AFX_INVALID_INDEX;

    if (!(err = dout->lockCb(dout, timeout, &bufIdx2)))
    {
        AfxAssert(AFX_INVALID_INDEX != bufIdx2);
        AfxAssertRange(dout->bufCnt, bufIdx2, 1);
    }

    AfxAssert(bufIdx);
    *bufIdx = bufIdx2;
    return err;
}

_AVX afxNat AfxEnumerateDrawOutputCanvases(afxDrawOutput dout, afxNat baseBuf, afxNat bufCnt, afxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, baseBuf, bufCnt);
    AfxAssert(canvases);
    afxNat rslt = 0;

    for (afxNat i = 0; i < bufCnt; i++)
    {
        afxNat bufIdx = baseBuf + i;
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

_AVX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat baseBuf, afxNat bufCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, baseBuf, bufCnt);
    AfxAssert(rasters);
    afxNat rslt = 0;

    for (afxNat i = 0; i < bufCnt; i++)
    {
        afxNat bufIdx = baseBuf + i;
        AfxAssertRange(dout->bufCnt, bufIdx, 1);
        afxRaster ras = NIL;
        afxCanvas canv;

        if (AfxEnumerateDrawOutputCanvases(dout, bufIdx, 1, &canv))
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
    return rslt;
}

_AVX afxError AfxRevalidateDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!dout->buffers)
    {
        AfxAssert(dout->bufCnt);

        if (!(dout->buffers = AfxAllocate(dout->bufCnt, sizeof(dout->buffers[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            AfxZero2(dout->bufCnt, sizeof(dout->buffers[0]), dout->buffers);
        }
    }

    if (!err)
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
            
            if (AfxGetDrawOutputContext(dout, &dctx))
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
                    
                    if (AfxAcquireCanvases(dctx, dout->whd, 1, surCnt, surCfg, 1, &canv)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &canv, afxFcc_CANV);
                        dout->buffers[i].canv = canv;

                        AfxRevalidateDrawBuffers(canv);
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

_AVX void AfxGetDrawOutputAspectRatio(afxDrawOutput dout, afxReal64* wpOverHp, afxReal64* wrOverHr, afxReal64* wwOverHw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert3(wpOverHp, wrOverHr, wwOverHw);
    *wpOverHp = dout->wpOverHp;
    *wrOverHr = dout->wrOverHr;
    *wwOverHw = dout->wwOverHw;
}

_AVX void AfxGetDrawOutputResolution(afxDrawOutput dout, afxWhd res, afxNat* refreshRate)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(refreshRate);
    AfxAssert(res);
    res[0] = dout->res[0];
    res[1] = dout->res[1];
    res[2] = dout->res[2];
    *refreshRate = dout->refreshRate;
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

_AVX void AfxGetDrawOutputExtentNdc(afxDrawOutput dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    afxWhd whd2;
    AfxGetDrawOutputExtent(dout, whd2);
    AfxSetV3d(whd, AfxNdcf(whd2[0], dout->res[0]), AfxNdcf(whd2[1], dout->res[1]), (afxReal)1);
}

_AVX afxError AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    AfxAssertExtent(dout->res[0], whd[0]);
    AfxAssertExtent(dout->res[1], whd[1]);
    AfxAssertExtent(dout->res[2], whd[2]);

    afxBool changed = (dout->whd[0] != whd[0] || dout->whd[1] != whd[1] || dout->whd[2] != whd[2]);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        AfxAssert(whd[0]);
        AfxAssert(whd[1]);
        AfxAssert(whd[2]);
        dout->whd[0] = AfxMax(whd[0], 1);
        dout->whd[1] = AfxMax(whd[1], 1);
        dout->whd[2] = AfxMax(whd[2], 1);
        dout->wwOverHw = (afxReal64)dout->whd[0] / (afxReal64)dout->whd[1];

        AfxRevalidateDrawOutputBuffers(dout);

        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->whd);
        //AfxNotifyObject(&dout->obj, &ev);

        afxWhd whd2;
        AfxGetDrawOutputExtent(dout, whd2);
        afxV2d ndc;
        AfxNdcV2d(ndc, AfxSpawnV2d(whd2[0], whd2[1]), AfxSpawnV2d(dout->res[0], dout->res[1]));
        AfxLogEcho("DOUT#%03u adjusted. <%u, %u, %.3f> %f <%u, %u, %.3f>", AfxGetObjectId(dout), whd2[0], dout->res[0], ndc[0], dout->wwOverHw, whd2[1], dout->res[1], ndc[1]);

        --dout->resizing;
    }
    return err;
}

_AVX afxError AfxAdjustDrawOutputNdc(afxDrawOutput dout, afxV3d const whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);

    afxWhd const whd2 =
    {
        (afxNat)AfxUnndcf(whd[0], dout->res[0]),
        (afxNat)AfxUnndcf(whd[1], dout->res[1]),
        1
    };
    return AfxAdjustDrawOutput(dout, whd2);
}

_AVX void AfxAdaptDrawOutputResolution(afxDrawOutput dout, afxWhd const res, afxNat refreshRate, afxReal64 physAspectRatio)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (res)
    {
        AfxAssert(res[0]);
        AfxAssert(res[1]);
        AfxAssert(res[2]);
        dout->res[0] = AfxMax(res[0], 1);
        dout->res[1] = AfxMax(res[1], 1);
        dout->res[2] = AfxMax(res[2], 1);
        dout->wrOverHr = (afxReal64)dout->res[0] / (afxReal64)dout->res[1];
    }

    if (refreshRate)
        dout->refreshRate = refreshRate;

    if (physAspectRatio)
        dout->wpOverHp = physAspectRatio;

    AfxLogEcho("DOUT#%03u readapted. %ux%u @ %fHz <%fr, %fp>", AfxGetObjectId(dout), dout->res[0], dout->res[1], dout->refreshRate, dout->wrOverHr, dout->wpOverHp);
}

_AVX afxDrawDevice AfxGetDrawOutputDevice(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetLinker(&dout->ddev);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxError _AvxDoutDtor(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;

    AfxDisconnectDrawOutput(dout);

    afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    if (AfxCallDevice((afxDevice)ddev, _ddevReqCode_DOUT_DTOR, dout))
        AfxThrowError();

    AfxAssert(!dout->idd);

    _AvxDoutSuspendFunction(dout);
    _AvxDoutLockBuffers(dout);

    _AvxDoutFreeAllBuffers(dout);
    AfxDeallocate(dout->buffers);

    AfxAssert(dout->swapchain.cnt == 0);

    AfxCleanUpSlock(&dout->buffersLock);
    AfxCleanUpSlock(&dout->suspendSlock);

    return err;
}

_AVX afxError _AvxDoutCtor(afxDrawOutput dout, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(dout, sizeof(dout[0]));

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawOutputConfig const* cfg = ((afxDrawOutputConfig const *)cookie->udd[1]) + cookie->no;
    AfxAssert(cfg);
    afxUri const* endpoint = cookie->udd[2];

    AfxPushLinkage(&dout->ddev, &ddev->outputs);
    AfxPushLinkage(&dout->dctx, NIL);

    dout->whd[0] = AfxMax(1, cfg->whd[0]);
    dout->whd[1] = AfxMax(1, cfg->whd[1]);
    dout->whd[2] = AfxMax(1, cfg->whd[2]);

    // endpoint
    dout->resizing = FALSE;
    dout->resizable = TRUE;
    dout->refreshRate = 1;
    dout->res[0] = dout->whd[0];
    dout->res[1] = dout->whd[1];
    dout->res[2] = dout->whd[2];
    dout->wpOverHp = AfxDeterminePhysicalAspectRatio(dout->res[0], dout->res[1]);
    dout->wrOverHr = dout->res[0] / dout->res[1];
    dout->wwOverHw = dout->whd[0] / dout->whd[1];
    dout->presentAlpha = cfg->presentAlpha; // consider transparency for window composing.
    dout->presentTransform = cfg->presentTransform; // The default is to leave it as it is.
    dout->presentMode = cfg->presentMode; // The default afxPresentMode_LIFO is already zero.
    dout->doNotClip = !!cfg->doNotClip; // don't spend resources doing off-screen draw.
    dout->udd = cfg->udd;

    // canvas
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : afxColorSpace_SRGB; // sRGB is the default
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

    dout->bufferLockCnt = 1;
    dout->lastReqBufIdx = dout->bufCnt - 1; // to start at 0 instead of 1 we set to last one.
    dout->swapping = FALSE;

    AfxSetUpChain(&dout->swapchain, (void*)dout);
    AfxSetUpSlock(&dout->buffersLock);

    dout->suspendCnt = 1;
    AfxSetUpSlock(&dout->suspendSlock);

    dout->lockCb = NIL;
    dout->buffers = NIL;

    if (AfxCallDevice((afxDevice)ddev, _ddevReqCode_DOUT_CTOR, dout, cfg, endpoint)) AfxThrowError();
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

_AVX afxClassConfig const _AvxDoutMgrCfg =
{
    .fcc = afxFcc_DOUT,
    .name = "DrawOutput",
    .desc = "Draw Output Management",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawOutput)),
    .mmu = NIL,
    .ctor = (void*)_AvxDoutCtor,
    .dtor = (void*)_AvxDoutDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxOpenDrawOutput(afxNat ddevId, afxDrawOutputConfig const* cfg, afxUri const* endpoint, afxDrawOutput* dout)
// file, window, desktop, widget, etc; physical or virtual VDUs.
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxManager* cls = AfxGetDrawOutputClass();
        AfxAssertClass(cls, afxFcc_DOUT);
        afxDrawOutput dout2;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&dout2, (void const*[]) { ddev, cfg, endpoint })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &dout2, afxFcc_DOUT);
            AfxAssert(dout);
            *dout = dout2;
        }
    }
    return err;
}

_AVX afxError AfxAcquireDrawOutput(afxNat ddevId, afxDrawOutputConfig const* cfg, afxDrawOutput* dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxDrawOutput dout2;

    if (AfxOpenDrawOutput(ddevId, cfg, NIL, &dout2)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dout2, afxFcc_DOUT);
        AfxAssert(dout);
        *dout = dout2;
    }
    return err;
}

_AVX afxNat AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxManager* cls = AfxGetDrawOutputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxNat AfxCountDrawOutputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager*cls = AfxGetDrawOutputClass();
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxCountObjects(cls);
}
