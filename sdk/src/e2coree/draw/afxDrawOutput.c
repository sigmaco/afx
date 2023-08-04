/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_OUTPUT_C
//#define _AFX_SURFACE_C

#include "_classified/afxDrawClassified.h"

_AFX afxNat _AfxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    AfxEnterSlockShared(&dout->buffersLock);
    afxNat bufferLockCnt = dout->bufferLockCnt;
    AfxExitSlockShared(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = ++dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->buffersLock);
    afxNat bufferLockCnt = --dout->bufferLockCnt;
    AfxExitSlockExclusive(&dout->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AFX afxError _AfxDoutFreeAllBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    for (afxNat i = 0; i < dout->bufCnt; i++)
    {
        afxCanvas canv = dout->buffers[i].canv;
        afxSurface surf = dout->buffers[i].surf;

        if (!surf)
        {
            AfxAssert(_AfxDoutIsSuspended(dout)); // dout sem surfaces é inoperante
        }
        else
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssertObject(canv, AFX_FCC_CANV);
            AfxReleaseObject(&surf->tex.obj);
            AfxReleaseObject(&canv->obj);
            dout->buffers[i].surf = NIL;
            dout->buffers[i].canv = NIL;
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
    AfxAssertObject(dout, AFX_FCC_DOUT);

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
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(canv);

    _AfxDoutLockBuffers(dout);

    afxDrawContext dctx;    
    if (!(AfxGetConnectedDrawOutputContext(dout, &dctx))) AfxThrowError();
    else
    {
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxResult rslt = 0;

        for (afxNat i = 0; i < AfxMin(cnt, dout->bufCnt); i++)
        {
            afxSurface raster;
            
            if (!(AfxGetDrawOutputBuffer(dout, first + i, &raster))) AfxThrowError();
            else
            {
                AfxAssertObject(raster, AFX_FCC_SURF);

                afxWhd extent;
                AfxGetTextureExtent(&raster->tex, 0, extent);

                afxCanvasBlueprint blueprint;
                AfxBeginCanvasBlueprint(&blueprint, dctx, extent);
                AfxCanvasBlueprintSetDepth(&blueprint, depth, stencil);
                AfxCanvasBlueprintAddExistingRaster(&blueprint, raster);
                blueprint.udd[0] = dout;
                blueprint.udd[1] = (void*)i;

                if (AfxBuildCanvases(&blueprint, 1, &canv[i]))
                {
                    AfxThrowError();

                    for (afxNat j = 0; j < i; j++)
                    {
                        AfxObjectRemoveEventFilter(&dout->obj, &canv[j]->obj);
                        AfxReleaseObject(&canv[j]->obj);
                        canv[j] = NIL;
                    }
                    break;
                }
                else
                {
                    AfxAssertObject(canv[i], AFX_FCC_CANV);
                    AfxObjectInstallEventFilter(&dout->obj, &canv[i]->obj);
                    rslt++;
                }
            }
        }
    }

    _AfxDoutUnlockBuffers(dout);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetConnectedDrawOutputContext(afxDrawOutput dout, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    afxDrawContext dctx2 = dout->dctx;
    afxBool rslt;

    if ((rslt = !!dctx2))
    {
        AfxAssertObject(dctx2, AFX_FCC_DCTX);
        
        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawOutputIsConnected(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    return !!(AfxGetConnectedDrawOutputContext(dout, NIL));
}

_AFX afxError AfxDisconnectDrawOutput(afxDrawOutput dout, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    if (dout->vmt->cctx(dout, dout->dctx, NIL, slotIdx))
    {
        AfxThrowError();
    }
    else
    {
        dout->dctx = NIL;
        AfxRegenerateDrawOutputBuffers(dout);
    }

    AfxAssert(!dout->dctx);

    return err;
}

_AFX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxGetDrawOutputDriver(dout);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawDriver ddrv2 = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv2, AFX_FCC_DDRV);

    if (ddrv != ddrv2) AfxThrowError();
    else
    {
        if (dout->vmt->cctx(dout, dout->dctx, dctx, slotIdx))
        {
            AfxThrowError();
        }
        else
        {
            dout->dctx = dctx;
            AfxRegenerateDrawOutputBuffers(dout);
        }
    }
    AfxAssert(dout->dctx == dctx);
    return !err;
}

////////////////////////////////////////////////////////////////////////////////
// BUFFER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat idx, afxSurface *surf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    afxBool rslt;
    afxSurface surf2;
    if ((rslt = !!(surf2 = dout->buffers[idx].surf)))
    {
        AfxAssertObject(surf2, AFX_FCC_SURF);

        if (surf)
            *surf = surf2;
    }
    return rslt;
}

_AFX afxBool AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat idx, afxCanvas *canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, idx, 1);

    afxBool rslt;
    afxCanvas canv2;    
    if ((rslt = !!(canv2 = dout->buffers[idx].canv)))
    {
        AfxAssertObject(canv2, AFX_FCC_CANV);

        if (canv)
            *canv = canv2;
    }
    return rslt;
}

_AFX afxNat AfxGetDrawOutputCapacity(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    return dout->bufCnt;
}

_AFX afxError AfxRequestNextDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    *bufIdx = AFX_INVALID_INDEX;

    if (dout->vmt->req(dout, timeout, bufIdx)) AfxThrowError();
    else
    {
        AfxAssertRange(dout->bufCnt, *bufIdx, 1);
        AfxAssert(AFX_INVALID_INDEX != *bufIdx);
    }
    return err;
}

_AFX afxResult AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssertRange(dout->bufCnt, first, cnt);
    AfxAssert(surf);
    afxResult hitcnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSurface surf2 = dout->buffers[first + i].surf;
        AfxTryAssertObject(surf2, AFX_FCC_SURF);
        surf[i] = surf2;
        ++hitcnt;
    }

    return hitcnt;
}

_AFX afxError AfxRegenerateDrawOutputBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    if (dout->buffers)
    {
        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
            afxSurface surf = dout->buffers[i].surf;

            if (surf)
            {
                AfxAssertObject(surf, AFX_FCC_SURF);
                AfxReleaseObject(&surf->tex.obj);
                dout->buffers[i].surf = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetConnectedDrawOutputContext(dout, &dctx))
            {
                AfxAssertObject(dctx, AFX_FCC_DCTX);
                afxSurface surf2;
                dout->buffers[i].surf = (surf2 = AfxAcquireSurface(dctx, dout->pixelFmt, dout->extent, dout->bufUsage));

                if (!(surf2)) AfxThrowError();
                else
                {
                    AfxAssertObject(surf2, AFX_FCC_SURF);

                    afxEvent ev;
                    AfxEventDeploy(&ev, AFX_EVENT_DOUT_REBUF, &dout->obj, NIL);
                    ev.udd[0] = (void*)i; // from
                    AfxObjectEmitEvent(&dout->obj, &ev);
                }
            }
        }

        for (afxNat i = 0; i < dout->bufCnt; i++)
        {
            afxCanvas canv = dout->buffers[i].canv;

            if (canv)
            {
                AfxAssertObject(canv, AFX_FCC_CANV);
                AfxReleaseObject(&canv->obj);
                dout->buffers[i].canv = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetConnectedDrawOutputContext(dout, &dctx))
            {
                AfxAssertObject(dctx, AFX_FCC_DCTX);

                afxSurface raster = dout->buffers[i].surf;
                AfxTryAssertObject(raster, AFX_FCC_SURF);

                if (!raster) AfxThrowError();
                else
                {
                    afxWhd extent;
                    afxCanvasBlueprint blueprint;
                    AfxGetTextureExtent(&raster->tex, 0, extent);
                    AfxBeginCanvasBlueprint(&blueprint, dctx, extent);
                    AfxCanvasBlueprintAddExistingRaster(&blueprint, raster);
                    AfxCanvasBlueprintSetDepth(&blueprint, dout->auxDsFmt[0], dout->auxDsFmt[1]);
                    blueprint.udd[0] = dout;
                    blueprint.udd[1] = (void*)i;

                    if (AfxBuildCanvases(&blueprint, 1, &canv))
                    {
                        AfxThrowError();
                        break;
                    }
                    else
                    {
                        AfxAssertObject(canv, AFX_FCC_CANV);
                        dout->buffers[i].canv = canv;
                    }
                }
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// EXTENT                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX void AfxGetDrawOutputExtentNdc(afxDrawOutput dout, afxV3d extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    AfxV3dSet(extent, AfxToNdc(dout->extent[0], dout->resolution[0]), AfxToNdc(dout->extent[1], dout->resolution[1]), (afxReal)1);
}

_AFX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    extent[0] = dout->extent[0];
    extent[1] = dout->extent[1];
    extent[2] = dout->extent[2];
}

_AFX afxError AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    AfxAssertExtent(dout->resolution[0], extent[0]);
    AfxAssertExtent(dout->resolution[1], extent[1]);
    AfxAssertExtent(dout->resolution[2], extent[2]);

    afxBool changed = (dout->extent[0] != extent[0] || dout->extent[1] != extent[1] || dout->extent[2] != extent[2]);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        dout->extent[0] = AfxMax(extent[0], 1);
        dout->extent[1] = AfxMax(extent[1], 1);
        dout->extent[2] = AfxMax(extent[2], 1);

        AfxReadjustDrawOutputProportion(dout, dout->wpOverHp, dout->resolution);

        AfxRegenerateDrawOutputBuffers(dout);

        afxEvent ev;
        AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->extent);
        AfxObjectEmitEvent(&dout->obj, &ev);

        afxV3d extent;
        AfxGetDrawOutputExtentNdc(dout, extent);
        AfxEcho("Draw output (%p) extent readjusted to %ux%u; %0f%%:%0f%% of %ux%u.", dout, dout->extent[0], dout->extent[1], extent[0], extent[1], dout->resolution[0], dout->resolution[1]);

        --dout->resizing;
    }
    return err;
}

_AFX afxError AfxReadjustDrawOutputNdc(afxDrawOutput dout, afxV3d const extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    AfxAssert(extent);
    afxWhd const whd = { (afxNat)AfxFromNdc(extent[0], dout->resolution[0]), (afxNat)AfxFromNdc(extent[1], dout->resolution[1]), 1 };
    return AfxReadjustDrawOutput(dout, whd);
}

_AFX void AfxReadjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

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
    dout->wwOverHw = (afxReal64)dout->extent[0] / (afxReal64)dout->extent[1];
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxDrawDriver AfxGetDrawOutputDriver(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    afxDrawDriver ddrv = AfxObjectGetProvider(&dout->obj);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError AfxAcquireDrawOutputs(afxDrawOutputSpecification const *spec, afxNat cnt, afxDrawOutput dout[]) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    AfxEntry("spec=%p,cnt=%u", spec, cnt);
    afxError err = AFX_ERR_NONE;
    afxDrawDriver ddrv;

    if (!AfxGetDrawDriver(spec ? spec->drvIdx : 0, &ddrv)) AfxThrowError();
    else
    {
        AfxAssertObject(ddrv, AFX_FCC_DDRV);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxClassAcquireObjects(AfxGetDrawOutputClass(ddrv), NIL, 1, spec, (afxObject**)&dout[i], AfxSpawnHint()))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxAssertObject(dout[j], AFX_FCC_DOUT);
                    AfxReleaseObject(&dout[j]->obj);
                    dout[j] = NIL;
                }
                break;
            }
            else
            {
                AfxAssertObject(dout[i], AFX_FCC_DOUT);
            }
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dout[i] = NIL;

    return err;
}

_AFX afxBool _AfxDoutEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (void*)obj;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDoutEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (void*)obj;
    AfxAssertObject(dout, AFX_FCC_DOUT);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDoutDtor(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dout, AFX_FCC_DOUT);

    AfxDisconnectDrawOutput(dout, NIL);

    _AfxDoutSuspendFunction(dout);
    _AfxDoutLockBuffers(dout);

    _AfxDoutFreeAllBuffers(dout);

    if (dout->vmt->dtor)
        if (dout->vmt->dtor(dout))
            AfxThrowError();
    
    AfxAssert(!dout->idd); // imp->dtor must dealloc it

    AfxAssert(dout->swapchain.cnt == 0);

    if (dout->caption)
        AfxDeallocateString(dout->caption);

    AfxReleaseSlock(&dout->buffersLock);
    AfxReleaseSlock(&dout->suspendSlock);

    return err;
}

_AFX afxError _AfxDoutCtor(void *cache, afxNat idx, afxDrawOutput dout, afxDrawOutputSpecification const *specs)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    (void)cache;

    afxDrawOutputSpecification const *spec = &specs[idx];

    afxDrawDriver ddrv = AfxGetDrawOutputDriver(dout);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    dout->dctx = NIL;
    afxMemory mem = AfxGetDrawMemory();

    dout->suspendCnt = 1;
    AfxAcquireSlock(&dout->suspendSlock);

    // absolute extent
    dout->resolution[0] = 1280;
    dout->resolution[1] = 720;
    dout->resolution[2] = 1;

    dout->extent[0] = 1280;
    dout->extent[1] = 720;
    dout->extent[2] = 1;
    dout->resizable = TRUE;

    // swapchain-related data
    dout->flags = NIL;
    dout->pixelFmt = AFX_PFD_RGBA8; // or AFX_PFD_RGBA8R ?
    dout->colorSpc = AFX_COLOR_SPACE_SRGB; // sRGB is the default
    dout->bufUsage = AFX_TEX_USAGE_DRAW;
    dout->bufferLockCnt = 1;
    dout->bufCnt = 2;// 3; // 2 or 3; double or triple buffered for via-memory presentation.
    dout->lastReqBufIdx = dout->bufCnt - 1; // to start at 0 instead of 1 we set to last one.
    AfxAcquireChain(&dout->swapchain, dout);
    AfxAcquireSlock(&dout->buffersLock);

    dout->presentAlpha = TRUE; // consider transparency for window composing.
    dout->presentTransform = AFX_PRESENT_TRANSFORM_FLIP_V; // NIL leaves it as it is.
    dout->presentMode = AFX_PRESENT_MODE_FIFO;
    dout->clipped = TRUE; // usually true to don't spend resources doing off-screen draw.
    dout->swapping = FALSE;

    AfxV2dZero(dout->absCursorPos);
    AfxV2dZero(dout->absCursorMove);
    AfxV2dZero(dout->ndcCursorPos);
    AfxV2dZero(dout->ndcCursorMove);

    if (spec)
    {
        dout->extent[0] = spec->whd[0];
        dout->extent[1] = spec->whd[1];
        dout->extent[2] = spec->whd[2];

        // swapchain-related data
        dout->pixelFmt = spec->pixelFmt;
        dout->colorSpc = spec->colorSpc;
        dout->bufUsage |= spec->bufUsage;
        dout->bufCnt = spec->bufCnt;

        dout->presentAlpha = spec->presentAlpha;
        dout->presentTransform = spec->presentTransform;
        dout->presentMode = spec->presentMode;
        dout->clipped = !!spec->clipped;
    }

    dout->auxDsFmt[0] = NIL;
    dout->auxDsFmt[1] = NIL;

    dout->idd = NIL;
    dout->vmt = NIL;

    dout->caption = AfxAllocateString(512, NIL, 0);
    dout->buffers = NIL;

    if (spec && spec->udd)
        dout->udd = spec->udd;

    if (ddrv->vmt->dout && ddrv->vmt->dout(dout, spec->endpoint, spec)) AfxThrowError();
    else
    {
        AfxAssert(dout->vmt);

        AfxAssert(dout->resolution[0]);
        AfxAssert(dout->resolution[1]);
        AfxAssert(dout->resolution[2]);
        AfxAssertExtent(dout->resolution[0], dout->extent[0]);
        AfxAssertExtent(dout->resolution[1], dout->extent[1]);
        AfxAssertExtent(dout->resolution[2], dout->extent[2]);

        AfxAssert(dout->bufCnt);
        AfxAssert(dout->bufUsage & AFX_TEX_USAGE_DRAW);
        AfxAssert(dout->refreshRate);
        AfxAssert(dout->wpOverHp);
        AfxAssert(dout->wrOverHr);
        AfxAssert(dout->wwOverHw);

        if (!dout->buffers && !(dout->buffers = AfxAllocate(mem, dout->bufCnt * sizeof(dout->buffers[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else for(afxNat i = 0; i < dout->bufCnt; i++)
            dout->buffers[i].surf = NIL, dout->buffers[i].canv = NIL;;

        if (err && dout->buffers)
        {
            _AfxDoutFreeAllBuffers(dout);
            AfxDeallocate(mem, dout->buffers);
        }
    }
    return err;
}

_AFX afxClassSpecification const _AfxDoutClassSpec;

afxClassSpecification const _AfxDoutClassSpec =
{
    AFX_FCC_DOUT,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawOutput)),
    NIL,
    (void*)_AfxDoutCtor,
    (void*)_AfxDoutDtor,
    .event = _AfxDoutEventHandler,
    .eventFilter = _AfxDoutEventFilter,
    "afxDrawOutput",
    NIL
};
