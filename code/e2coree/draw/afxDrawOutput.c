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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_DEVICE_C
//#define _AFX_SURFACE_C
#include "afx/draw/afxDrawSystem.h"

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
        //afxCanvas canv = dout->buffers[i].canv;
        afxRaster tex = dout->buffers[i].tex;

        if (!tex)
        {
            AfxAssert(_AfxDoutIsSuspended(dout)); // dout sem surfaces � inoperante
        }
        else
        {
            AfxAssertObjects(1, &tex, afxFcc_RAS);
            //AfxAssertObjects(1, &canv, afxFcc_CANV);
            AfxReleaseObjects(1, (void*[]) { tex });
            //AfxReleaseObjects(1, (void*[]) { canv });
            dout->buffers[i].tex = NIL;
            //dout->buffers[i].canv = NIL;
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

_AFX afxError AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxPixelFormat depth, afxPixelFormat stencil, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(canv);

    afxDrawContext dctx;

    if (!(AfxGetDrawOutputConnection(dout, &dctx))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, afxFcc_DCTX);
        afxResult rslt = 0;

        for (afxNat i = 0; i < AfxMini(cnt, dout->bufCnt); i++)
        {
            afxRaster raster;
            
            if (!(AfxGetDrawOutputBuffer(dout, first + i, &raster))) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &raster, afxFcc_RAS);
                //afxRaster tex = AfxGetSurfaceTexture(raster);

                afxWhd extent;
                AfxGetRasterExtent(raster, 0, extent);

                afxCanvasBlueprint blueprint;
                AfxBeginCanvasBlueprint(&blueprint, extent);
                AfxCanvasBlueprintSetDepth(&blueprint, depth, stencil);
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

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetDrawOutputConnection(afxDrawOutput dout, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    
    afxBool rslt;
    afxDrawContext dctx2 = AfxGetLinker(&dout->dctx);
    
    if ((rslt = !!dctx2))
    {
        AfxAssertObjects(1, &dctx2, afxFcc_DCTX);
        
        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawOutputIsConnected(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return !!(AfxGetDrawOutputConnection(dout, NIL));
}

_AFX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext curr = NIL;
    AfxGetDrawOutputConnection(dout, &curr);

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
            if (dctx)
                AfxReacquireObjects(1, (void*[]) { dctx });

            if (ddev->relinkDout(ddev, dout, dctx))
                AfxThrowError();

            if (err)
                AfxReleaseObjects(1, (void*[]) { dctx });
            else if (curr)
            {
                AfxAssertObjects(1, &curr, afxFcc_DCTX);
                AfxReleaseObjects(1, (void*[]) { curr });
            }
        }
    }
    AfxAssert(AfxGetLinker(&dout->dctx) == dctx);
    return !err;
}

_AFX afxError AfxDisconnectDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawOutputIsConnected(dout));
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// BUFFER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat idx, afxRaster *tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, idx, 1);

    afxBool rslt;
    afxRaster tex2;
    if ((rslt = !!(tex2 = dout->buffers[idx].tex)))
    {
        AfxAssertObjects(1, &tex2, afxFcc_RAS);

        if (tex)
            *tex = tex2;
    }
    return rslt;
}

#if 0
_AFX afxBool AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat idx, afxCanvas *canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, idx, 1);

    afxBool rslt;
    afxCanvas canv2;    
    if ((rslt = !!(canv2 = dout->buffers[idx].canv)))
    {
        AfxAssertObjects(1, &canv2, afxFcc_CANV);

        if (canv)
            *canv = canv2;
    }
    return rslt;
}
#endif

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

_AFX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(tex);
    afxNat hitcnt = 0;

    for (afxNat i = 0; i < AfxMini(dout->bufCnt, cnt); i++)
    {
        afxRaster tex2 = dout->buffers[first + i].tex;

        if (tex2)
        {
            AfxAssertObjects(1, &tex2, afxFcc_RAS);
        }
        tex[i] = tex2;
        ++hitcnt;
    }

    return hitcnt;
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
            afxRaster tex = dout->buffers[i].tex;

            if (tex)
            {
                AfxAssertObjects(1, &tex, afxFcc_RAS);
                AfxReleaseObjects(1, (void*[]) { tex });
                dout->buffers[i].tex = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetDrawOutputConnection(dout, &dctx))
            {
                //AfxAssertType(dctxD, afxFcc_DCTX);
                afxRaster tex2;
                afxRasterInfo texi = { 0 };
                texi.fmt = dout->pixelFmt;
                texi.whd[0] = dout->extent[0];
                texi.whd[1] = dout->extent[1];
                texi.whd[2] = dout->extent[2];
                texi.layerCnt = 1;
                texi.lodCnt = 1;
                texi.usage = dout->bufUsage;

                if (AfxAcquireRasters(dctx, 1, &texi, &tex2)) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &tex2, afxFcc_RAS);
                    dout->buffers[i].tex = tex2;

                    //afxEvent ev;
                    //AfxEventDeploy(&ev, AFX_EVENT_DOUT_REBUF, &dout->obj, NIL);
                    //ev.udd[0] = (void*)i; // from
                    //AfxObjectEmitEvent(&dout->obj, &ev);
                }
            }
        }
#if 0
        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
            afxCanvas canv = dout->buffers[i].canv;

            if (canv)
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);
                AfxReleaseObjects(1, (void*[]) { canv });
                dout->buffers[i].canv = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetDrawOutputConnection(dout, &dctx))
            {
                //AfxAssertType(dctxD, afxFcc_DCTX);

                afxRaster raster = dout->buffers[i].tex;
                
                if (!raster) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &raster, afxFcc_RAS);

                    afxWhd extent;
                    afxCanvasBlueprint blueprint;
                    afxRaster tex = raster;
                    AfxGetRasterExtent(tex, 0, extent);
                    AfxBeginCanvasBlueprint(&blueprint, extent);
                    AfxCanvasBlueprintAddExistingRaster(&blueprint, raster);
                    AfxCanvasBlueprintSetDepth(&blueprint, dout->auxDsFmt[0], dout->auxDsFmt[1]);
                    blueprint.udd[0] = (void*)dout;
                    blueprint.udd[1] = (void*)i;

                    if (AfxBuildCanvases(dctx, 1, &canv, &blueprint))
                    {
                        AfxThrowError();
                        break;
                    }
                    else
                    {
                        AfxAssertObjects(1, &canv, afxFcc_CANV);
                        dout->buffers[i].canv = canv;
                    }
                }
            }
        }
#endif
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// EXTENT                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX void AfxGetDrawOutputExtentNdc(afxDrawOutput dout, afxV3d extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(extent);
    AfxSetV3d(extent, AfxToNdc(dout->extent[0], dout->resolution[0]), AfxToNdc(dout->extent[1], dout->resolution[1]), (afxReal)1);
}

_AFX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(extent);
    extent[0] = dout->extent[0];
    extent[1] = dout->extent[1];
    extent[2] = dout->extent[2];
}

_AFX afxError AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(extent);
    AfxAssertExtent(dout->resolution[0], extent[0]);
    AfxAssertExtent(dout->resolution[1], extent[1]);
    AfxAssertExtent(dout->resolution[2], extent[2]);

    afxBool changed = (dout->extent[0] != extent[0] || dout->extent[1] != extent[1] || dout->extent[2] != extent[2]);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        dout->extent[0] = AfxMaxi(extent[0], 1);
        dout->extent[1] = AfxMaxi(extent[1], 1);
        dout->extent[2] = AfxMaxi(extent[2], 1);

        AfxReadjustDrawOutputProportion(dout, dout->wpOverHp, dout->resolution);

        AfxRegenerateDrawOutputBuffers(dout);

        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->extent);
        //AfxObjectEmitEvent(&dout->obj, &ev);

        afxV3d extent2;
        AfxGetDrawOutputExtentNdc(dout, extent2);
        AfxEcho("Draw output (%p) extent readjusted to %ux%u; %.0f:%.0f%% of %ux%u.", dout, extent2[0], extent2[1], extent2[0] / dout->resolution[0], extent2[1] / dout->resolution[1], dout->resolution[0], dout->resolution[1]);

        --dout->resizing;
    }
    return err;
}

_AFX afxError AfxReadjustDrawOutputNdc(afxDrawOutput dout, afxV3d const extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(extent);
    afxWhd const whd = { (afxNat)AfxFromNdc(extent[0], dout->resolution[0]), (afxNat)AfxFromNdc(extent[1], dout->resolution[1]), 1 };
    return AfxReadjustDrawOutput(dout, whd);
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
        dout->resolution[0] = AfxMaxi(resolution[0], 1);
        dout->resolution[1] = AfxMaxi(resolution[1], 1);
        dout->resolution[2] = AfxMaxi(resolution[2], 1);
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
    dout->wwOverHw = (afxReal64)dout->extent[0] / (afxReal64)dout->extent[1];
    AfxEcho("Draw output (%p) porportions readjusted. P %f, R %f W %f", dout, dout->wpOverHp, dout->wrOverHr, dout->wwOverHw);
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxDrawDevice AfxGetDrawOutputDevice(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetObjectProvider(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AFX afxError AfxOpenDrawOutputs(afxDrawSystem dsys, afxNat ddevId, afxNat cnt, afxDrawOutputConfig const config[], afxDrawOutput dout[]) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    AfxEntry("cnt=%u,config=%p,dout=%p", cnt, config, dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(dsys, ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = AfxGetDrawOutputClass(ddev);
        AfxAssertClass(cls, afxFcc_DOUT);

        if (AfxAcquireObjects(cls, cnt, (afxHandle*)dout, (void*[]) { &ddevId, (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, dout, afxFcc_DOUT);
    }
    return err;
}

_AFX afxNat AfxCurateDrawOutputs(afxDrawSystem dsys, afxNat ddevId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass* cls = AfxGetDrawDeviceClass(dsys);
    AfxAssertClass(cls, afxFcc_DDEV);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((ddevId != AFX_INVALID_INDEX) && !(AfxGetInstance(cls, ddevId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((ddevId == AFX_INVALID_INDEX)) cls = &dsys->outputs;
        else
        {
            AfxAssertObjects(1, &ddev, afxFcc_DDEV);
            cls = AfxGetDrawInputClass(ddev);
        }

        AfxAssertClass(cls, afxFcc_DOUT);
        rslt = AfxCurateInstances(cls, first, cnt, (void*)f, udd);
    }
    return rslt;
}

_AFX afxNat AfxEnumerateDrawOutputs(afxDrawSystem dsys, afxNat ddevId, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dout);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass* cls = AfxGetDrawDeviceClass(dsys);
    AfxAssertClass(cls, afxFcc_DDEV);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((ddevId != AFX_INVALID_INDEX) && !(AfxGetInstance(cls, ddevId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((ddevId == AFX_INVALID_INDEX)) cls = &dsys->outputs;
        else
        {
            AfxAssertObjects(1, &ddev, afxFcc_DDEV);
            cls = AfxGetDrawOutputClass(ddev);
        }

        AfxAssertClass(cls, afxFcc_DOUT);
        rslt = AfxEnumerateInstances(cls, first, cnt, (afxHandle*)dout);
    }
    return rslt;
}

_AFX afxNat AfxCountDrawOutputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxCountInstances(cls);
}