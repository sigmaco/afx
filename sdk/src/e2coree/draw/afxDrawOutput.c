/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_DEVICE_C
//#define _AFX_SURFACE_C

#include "afxDrawClassified.h"

_AFX afxNat _AfxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxEnterSlockShared(&dout->buffersLock);
    afxNat bufferLockCnt = dout->bufferLockCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = ++dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = --dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxError _AfxDoutFreeAllBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        //afxCanvas canv = dout->buffers[i].canv;
        afxTexture tex = dout->buffers[i].tex;

        if (!tex)
        {
            AfxAssert(_AfxDoutIsSuspended(dout)); // dout sem surfaces é inoperante
        }
        else
        {
            AfxAssertObjects(1, &tex, AFX_FCC_TEX);
            //AfxAssertObjects(1, &canv, AFX_FCC_CANV);
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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(canv);

    afxDrawContext dctx;

    if (!(AfxGetDrawOutputConnection(dout, &dctx))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, AFX_FCC_DCTX);
        afxResult rslt = 0;

        for (afxNat i = 0; i < AfxMini(cnt, dout->bufCnt); i++)
        {
            afxTexture raster;
            
            if (!(AfxGetDrawOutputBuffer(dout, first + i, &raster))) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &raster, AFX_FCC_TEX);
                //afxTexture tex = AfxGetSurfaceTexture(raster);

                afxWhd extent;
                AfxGetTextureExtent(raster, 0, extent);

                afxCanvasBlueprint blueprint;
                AfxBeginCanvasBlueprint(&blueprint, extent);
                AfxCanvasBlueprintSetDepth(&blueprint, depth, stencil);
                AfxCanvasBlueprintAddExistingRaster(&blueprint, raster);
                blueprint.udd[0] = (void*)dout;
                blueprint.udd[1] = (void*)i;

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
                    AfxAssertObjects(1, &canv[i], AFX_FCC_CANV);
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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    
    afxBool rslt;
    afxDrawContext dctx2 = AfxGetLinker(&dout->dctx);
    
    if ((rslt = !!dctx2))
    {
        AfxAssertObjects(1, &dctx2, AFX_FCC_DCTX);
        
        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawOutputIsConnected(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    return !!(AfxGetDrawOutputConnection(dout, NIL));
}

_AFX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    afxDrawContext curr = NIL;
    AfxGetDrawOutputConnection(dout, &curr);

    if (dctx != curr)
    {
        afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
        AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, AFX_FCC_DDEV);

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
                AfxAssertObjects(1, &curr, AFX_FCC_DCTX);
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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawOutputIsConnected(dout));
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// BUFFER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat idx, afxTexture *tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, idx, 1);

    afxBool rslt;
    afxTexture tex2;
    if ((rslt = !!(tex2 = dout->buffers[idx].tex)))
    {
        AfxAssertObjects(1, &tex2, AFX_FCC_TEX);

        if (tex)
            *tex = tex2;
    }
    return rslt;
}

#if 0
_AFX afxBool AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat idx, afxCanvas *canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, idx, 1);

    afxBool rslt;
    afxCanvas canv2;    
    if ((rslt = !!(canv2 = dout->buffers[idx].canv)))
    {
        AfxAssertObjects(1, &canv2, AFX_FCC_CANV);

        if (canv)
            *canv = canv2;
    }
    return rslt;
}
#endif

_AFX afxNat AfxGetDrawOutputCapacity(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    return dout->bufCnt;
}

_AFX afxError AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    *bufIdx = AFX_INVALID_INDEX;

    if (dout->vmt->req(dout, timeout, bufIdx)) AfxThrowError();
    else
    {
        AfxAssertRange(dout->bufCnt, *bufIdx, 1);
        AfxAssert(AFX_INVALID_INDEX != *bufIdx);
    }
    return err;
}

_AFX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(tex);
    afxNat hitcnt = 0;

    for (afxNat i = 0; i < AfxMini(dout->bufCnt, cnt); i++)
    {
        afxTexture tex2 = dout->buffers[first + i].tex;

        if (tex2)
        {
            AfxAssertObjects(1, &tex2, AFX_FCC_TEX);
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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    if (dout->buffers)
    {
        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
            afxTexture tex = dout->buffers[i].tex;

            if (tex)
            {
                AfxAssertObjects(1, &tex, AFX_FCC_TEX);
                AfxReleaseObjects(1, (void*[]) { tex });
                dout->buffers[i].tex = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetDrawOutputConnection(dout, &dctx))
            {
                //AfxAssertType(dctxD, AFX_FCC_DCTX);
                afxTexture tex2;
                afxTextureInfo texi = { 0 };
                texi.fmt = dout->pixelFmt;
                texi.whd[0] = dout->extent[0];
                texi.whd[1] = dout->extent[1];
                texi.whd[2] = dout->extent[2];
                texi.imgCnt = 1;
                texi.lodCnt = 1;
                texi.usage = dout->bufUsage;

                if (AfxAcquireTextures(dctx, 1, &texi, &tex2)) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &tex2, AFX_FCC_TEX);
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
                AfxAssertObjects(1, &canv, AFX_FCC_CANV);
                AfxReleaseObjects(1, (void*[]) { canv });
                dout->buffers[i].canv = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetDrawOutputConnection(dout, &dctx))
            {
                //AfxAssertType(dctxD, AFX_FCC_DCTX);

                afxTexture raster = dout->buffers[i].tex;
                
                if (!raster) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &raster, AFX_FCC_TEX);

                    afxWhd extent;
                    afxCanvasBlueprint blueprint;
                    afxTexture tex = raster;
                    AfxGetTextureExtent(tex, 0, extent);
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
                        AfxAssertObjects(1, &canv, AFX_FCC_CANV);
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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    AfxSetV3d(extent, AfxToNdc(dout->extent[0], dout->resolution[0]), AfxToNdc(dout->extent[1], dout->resolution[1]), (afxReal)1);
}

_AFX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    extent[0] = dout->extent[0];
    extent[1] = dout->extent[1];
    extent[2] = dout->extent[2];
}

_AFX afxError AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
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

        afxV3d extent;
        AfxGetDrawOutputExtentNdc(dout, extent);
        AfxEcho("Draw output (%p) extent readjusted to %ux%u; %.0f:%.0f%% of %ux%u.", dout, dout->extent[0], dout->extent[1], dout->extent[0] / dout->resolution[0], dout->extent[1] / dout->resolution[1], dout->resolution[0], dout->resolution[1]);

        --dout->resizing;
    }
    return err;
}

_AFX afxError AfxReadjustDrawOutputNdc(afxDrawOutput dout, afxV3d const extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    afxWhd const whd = { (afxNat)AfxFromNdc(extent[0], dout->resolution[0]), (afxNat)AfxFromNdc(extent[1], dout->resolution[1]), 1 };
    return AfxReadjustDrawOutput(dout, whd);
}

_AFX void AfxReadjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

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
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    afxDrawDevice ddev = AfxGetObjectProvider(dout);
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return ddev;
}

_AFX afxError AfxOpenDrawOutputs(afxDrawSystem dsys, afxNat devId, afxNat cnt, afxDrawOutputConfig const config[], afxDrawOutput dout[]) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    AfxEntry("cnt=%u,config=%p,dout=%p", cnt, config, dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(dsys, devId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);

        if (AfxAcquireObjects(AfxGetDrawOutputClass(ddev), cnt, (afxHandle*)dout, (void*[]) { &devId, (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, dout, AFX_FCC_DOUT);
    }
    return err;
}
