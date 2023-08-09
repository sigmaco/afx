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

#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_SYSTEM_C
//#define _AFX_SURFACE_C

#include "_classified/afxDrawClassified.h"

_AFX afxBool _AfxGetDoutD(afxDrawOutput dout, struct _afxDoutD **doutD, struct _afxDsysD* dsysD)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxGetObjectsResidency(1, &dout, (void**)doutD, &dsysD->outputs);
}

_AFX afxNat _AfxDoutBuffersAreLocked(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    AfxEnterSlockShared(&doutD->buffersLock);
    afxNat bufferLockCnt = doutD->bufferLockCnt;
    AfxExitSlockShared(&doutD->buffersLock);
    
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutLockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&doutD->buffersLock);
    afxNat bufferLockCnt = ++doutD->bufferLockCnt;
    AfxExitSlockExclusive(&doutD->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutUnlockBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&doutD->buffersLock);
    afxNat bufferLockCnt = --doutD->bufferLockCnt;
    AfxExitSlockExclusive(&doutD->buffersLock);
    return bufferLockCnt;
}

_AFX afxNat _AfxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxEnterSlockShared(&doutD->suspendSlock);
    afxNat suspendCnt = doutD->suspendCnt;
    AfxExitSlockShared(&doutD->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&doutD->suspendSlock);
    afxNat suspendCnt = ++doutD->suspendCnt;
    AfxExitSlockExclusive(&doutD->suspendSlock);
    return suspendCnt;
}

_AFX afxNat _AfxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxEnterSlockExclusive(&doutD->suspendSlock);
    afxNat suspendCnt = --doutD->suspendCnt;
    AfxExitSlockExclusive(&doutD->suspendSlock);
    return suspendCnt;
}

_AFX afxError _AfxDoutFreeAllBuffers(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    for (afxNat i = 0; i < doutD->bufCnt; i++)
    {
        afxCanvas canv = doutD->buffers[i].canv;
        afxSurface surf = doutD->buffers[i].surf;

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
            doutD->buffers[i].surf = NIL;
            doutD->buffers[i].canv = NIL;
        }
    }

    //doutD->bufCnt = 0;
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
        doutD->auxDsFmt[0] = depth;
    
    if (stencil && !AfxPixelFormatIsStencil(stencil))
        AfxThrowError();
    else
        doutD->auxDsFmt[1] = stencil;

    AfxRegenerateDrawOutputBuffers(dout);

    return err;
}
#endif

_AFX afxError AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxPixelFormat depth, afxPixelFormat stencil, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssertRange(doutD->bufCnt, first, cnt);
    AfxAssert(canv);

    _AfxDoutLockBuffers(dout);

    afxDrawContext dctx;    
    if (!(AfxGetConnectedDrawOutputContext(dout, &dctx))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, AFX_FCC_DCTX);
        afxResult rslt = 0;

        for (afxNat i = 0; i < AfxMini(cnt, doutD->bufCnt); i++)
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
                        //AfxObjectRemoveEventFilter(&doutD->obj, &canv[j]->obj);
                        AfxReleaseObject(&canv[j]->obj);
                        canv[j] = NIL;
                    }
                    break;
                }
                else
                {
                    AfxAssertObject(canv[i], AFX_FCC_CANV);
                    //AfxObjectInstallEventFilter(&doutD->obj, &canv[i]->obj);
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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    afxDrawContext dctx2 = doutD->dctx;
    afxBool rslt;

    if ((rslt = !!dctx2))
    {
        //AfxAssertObjects(1, &dctx2, AFX_FCC_DCTX);
        
        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawOutputIsConnected(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    return !!(AfxGetConnectedDrawOutputContext(dout, NIL));
}

_AFX afxError AfxDisconnectDrawOutput(afxDrawOutput dout, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    if (doutD->vmt->cctx(dout, doutD->dctx, NIL, slotIdx))
    {
        AfxThrowError();
    }
    else
    {
        doutD->dctx = NIL;
        AfxRegenerateDrawOutputBuffers(dout);
    }

    AfxAssert(!doutD->dctx);

    return err;
}

_AFX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    //AfxAssertType(dctxD, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxGetDrawOutputDriver(dout);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawDriver ddrv2 = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv2, AFX_FCC_DDRV);

    if (ddrv != ddrv2) AfxThrowError();
    else
    {
        if (doutD->vmt->cctx(dout, doutD->dctx, dctx, slotIdx))
        {
            AfxThrowError();
        }
        else
        {
            doutD->dctx = dctx;
            AfxRegenerateDrawOutputBuffers(dout);
        }
    }
    AfxAssert(doutD->dctx == dctx);
    return !err;
}

////////////////////////////////////////////////////////////////////////////////
// BUFFER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat idx, afxSurface *surf)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    afxBool rslt;
    afxSurface surf2;
    if ((rslt = !!(surf2 = doutD->buffers[idx].surf)))
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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssertRange(doutD->bufCnt, idx, 1);

    afxBool rslt;
    afxCanvas canv2;    
    if ((rslt = !!(canv2 = doutD->buffers[idx].canv)))
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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    return doutD->bufCnt;
}

_AFX afxError AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    *bufIdx = AFX_INVALID_INDEX;

    if (doutD->vmt->req(dout, timeout, bufIdx)) AfxThrowError();
    else
    {
        AfxAssertRange(doutD->bufCnt, *bufIdx, 1);
        AfxAssert(AFX_INVALID_INDEX != *bufIdx);
    }
    return err;
}

_AFX afxNat AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssertRange(doutD->bufCnt, first, cnt);
    AfxAssert(surf);
    afxNat hitcnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxSurface surf2 = doutD->buffers[first + i].surf;
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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    if (doutD->buffers)
    {
        for (afxNat i = 0; i < doutD->bufCnt; i++)
        {
            afxSurface surf = doutD->buffers[i].surf;

            if (surf)
            {
                AfxAssertObject(surf, AFX_FCC_SURF);
                AfxReleaseObject(&surf->tex.obj);
                doutD->buffers[i].surf = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetConnectedDrawOutputContext(dout, &dctx))
            {
                //AfxAssertType(dctxD, AFX_FCC_DCTX);
                afxSurface surf2;
                doutD->buffers[i].surf = (surf2 = AfxAcquireSurface(dctx, doutD->pixelFmt, doutD->extent, doutD->bufUsage));

                if (!(surf2)) AfxThrowError();
                else
                {
                    AfxAssertObject(surf2, AFX_FCC_SURF);

                    //afxEvent ev;
                    //AfxEventDeploy(&ev, AFX_EVENT_DOUT_REBUF, &doutD->obj, NIL);
                    //ev.udd[0] = (void*)i; // from
                    //AfxObjectEmitEvent(&doutD->obj, &ev);
                }
            }
        }

        for (afxNat i = 0; i < doutD->bufCnt; i++)
        {
            afxCanvas canv = doutD->buffers[i].canv;

            if (canv)
            {
                AfxAssertObject(canv, AFX_FCC_CANV);
                AfxReleaseObject(&canv->obj);
                doutD->buffers[i].canv = NIL;
            }

            afxDrawContext dctx;
            
            if (AfxGetConnectedDrawOutputContext(dout, &dctx))
            {
                //AfxAssertType(dctxD, AFX_FCC_DCTX);

                afxSurface raster = doutD->buffers[i].surf;
                AfxTryAssertObject(raster, AFX_FCC_SURF);

                if (!raster) AfxThrowError();
                else
                {
                    afxWhd extent;
                    afxCanvasBlueprint blueprint;
                    AfxGetTextureExtent(&raster->tex, 0, extent);
                    AfxBeginCanvasBlueprint(&blueprint, dctx, extent);
                    AfxCanvasBlueprintAddExistingRaster(&blueprint, raster);
                    AfxCanvasBlueprintSetDepth(&blueprint, doutD->auxDsFmt[0], doutD->auxDsFmt[1]);
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
                        doutD->buffers[i].canv = canv;
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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssert(extent);
    AfxV3dSet(extent, AfxToNdc(doutD->extent[0], doutD->resolution[0]), AfxToNdc(doutD->extent[1], doutD->resolution[1]), (afxReal)1);
}

_AFX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssert(extent);
    extent[0] = doutD->extent[0];
    extent[1] = doutD->extent[1];
    extent[2] = doutD->extent[2];
}

_AFX afxError AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssert(extent);
    AfxAssertExtent(doutD->resolution[0], extent[0]);
    AfxAssertExtent(doutD->resolution[1], extent[1]);
    AfxAssertExtent(doutD->resolution[2], extent[2]);

    afxBool changed = (doutD->extent[0] != extent[0] || doutD->extent[1] != extent[1] || doutD->extent[2] != extent[2]);

    if (changed)
    {
        ++doutD->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        doutD->extent[0] = AfxMaxi(extent[0], 1);
        doutD->extent[1] = AfxMaxi(extent[1], 1);
        doutD->extent[2] = AfxMaxi(extent[2], 1);

        AfxReadjustDrawOutputProportion(dout, doutD->wpOverHp, doutD->resolution);

        AfxRegenerateDrawOutputBuffers(dout);

        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &doutD->obj, doutD->extent);
        //AfxObjectEmitEvent(&doutD->obj, &ev);

        afxV3d extent;
        AfxGetDrawOutputExtentNdc(dout, extent);
        AfxEcho("Draw output (%p) extent readjusted to %ux%u; %0f%%:%0f%% of %ux%u.", dout, doutD->extent[0], doutD->extent[1], extent[0], extent[1], doutD->resolution[0], doutD->resolution[1]);

        --doutD->resizing;
    }
    return err;
}

_AFX afxError AfxReadjustDrawOutputNdc(afxDrawOutput dout, afxV3d const extent) // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    AfxAssert(extent);
    afxWhd const whd = { (afxNat)AfxFromNdc(extent[0], doutD->resolution[0]), (afxNat)AfxFromNdc(extent[1], doutD->resolution[1]), 1 };
    return AfxReadjustDrawOutput(dout, whd);
}

_AFX void AfxReadjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    if (resolution)
    {
        AfxAssert(resolution[0]);
        AfxAssert(resolution[1]);
        AfxAssert(resolution[2]);
        doutD->resolution[0] = AfxMaxi(resolution[0], 1);
        doutD->resolution[1] = AfxMaxi(resolution[1], 1);
        doutD->resolution[2] = AfxMaxi(resolution[2], 1);
    }

    if (!physicalAspectRatio)
    {
        afxReal64 div = (afxReal64)doutD->resolution[0] / (afxReal64)doutD->resolution[1];

        if (div <= 1.4)
            physicalAspectRatio = 1.33;
        else if (div <= 1.6)
            physicalAspectRatio = 1.5599999;
        else
            physicalAspectRatio = 1.78;
    }

    doutD->wpOverHp = physicalAspectRatio;
    doutD->wrOverHr = (afxReal64)doutD->resolution[0] / (afxReal64)doutD->resolution[1];
    doutD->wwOverHw = (afxReal64)doutD->extent[0] / (afxReal64)doutD->extent[1];
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxDrawDriver AfxGetDrawOutputDriver(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);
    afxDrawDriver ddrv = doutD->ddrv;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError _AfxDoutDtor(afxDrawOutput dout, struct _afxDoutD*doutD)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    AfxDisconnectDrawOutput(dout, NIL);

    _AfxDoutSuspendFunction(dout);
    _AfxDoutLockBuffers(dout);

    _AfxDoutFreeAllBuffers(dout);

    if (doutD->vmt->dtor)
        if (doutD->vmt->dtor(dout))
            AfxThrowError();
    
    AfxAssert(!doutD->idd); // imp->dtor must dealloc it

    AfxAssert(doutD->swapchain.cnt == 0);

    if (doutD->caption)
        AfxDeallocateString(doutD->caption);

    AfxReleaseSlock(&doutD->buffersLock);
    AfxReleaseSlock(&doutD->suspendSlock);

    return err;
}

_AFX void _AfxObjDeallocDout(afxDrawOutput dout)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDoutD *doutD;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    _AfxGetDoutD(dout, &doutD, dsysD);
    AfxAssertType(doutD, AFX_FCC_DOUT);

    _AfxDoutDtor(dout, doutD);
    AfxDeallocateObjects(1, &dout, &dsysD->outputs);
}

_AFX void AfxReleaseDrawOutputs(afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, dout, AFX_FCC_DOUT);
    AfxReleaseObjects(cnt, dout, _AfxObjDeallocDout);
}

_AFX afxError _AfxDoutCtor(afxDrawOutput dout, struct _afxDoutD*doutD, afxDrawOutputConfig const *config)
{
    AfxEntry("dout=%p", dout);
    afxError err = AFX_ERR_NONE;

    //afxDrawDriver ddrv = AfxGetDrawOutputDriver(dout);
    //AfxAssertObject(ddrv, AFX_FCC_DDRV);

    afxDrawDriver ddrv;
    AfxGetDrawDriver(config ? config->drvIdx : 0, &ddrv);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    doutD->ddrv = ddrv;
    doutD->doutObj = dout;
    doutD->dctx = NIL;
    afxContext mem = AfxGetDrawMemory();

    AfxAssignFcc(doutD, AFX_FCC_DOUT);

    doutD->suspendCnt = 1;
    AfxAcquireSlock(&doutD->suspendSlock);

    // absolute extent
    doutD->resolution[0] = 1280;
    doutD->resolution[1] = 720;
    doutD->resolution[2] = 1;

    doutD->extent[0] = 1280;
    doutD->extent[1] = 720;
    doutD->extent[2] = 1;
    doutD->resizable = TRUE;

    // swapchain-related data
    doutD->flags = NIL;
    doutD->pixelFmt = AFX_PFD_RGBA8; // or AFX_PFD_RGBA8R ?
    doutD->colorSpc = AFX_COLOR_SPACE_SRGB; // sRGB is the default
    doutD->bufUsage = AFX_TEX_USAGE_DRAW;
    doutD->bufferLockCnt = 1;
    doutD->bufCnt = 2;// 3; // 2 or 3; double or triple buffered for via-memory presentation.
    doutD->lastReqBufIdx = doutD->bufCnt - 1; // to start at 0 instead of 1 we set to last one.
    AfxAcquireChain(&doutD->swapchain, dout);
    AfxAcquireSlock(&doutD->buffersLock);

    doutD->presentAlpha = TRUE; // consider transparency for window composing.
    doutD->presentTransform = AFX_PRESENT_TRANSFORM_FLIP_V; // NIL leaves it as it is.
    doutD->presentMode = AFX_PRESENT_MODE_FIFO;
    doutD->clipped = TRUE; // usually true to don't spend resources doing off-screen draw.
    doutD->swapping = FALSE;

    AfxV2dZero(doutD->absCursorPos);
    AfxV2dZero(doutD->absCursorMove);
    AfxV2dZero(doutD->ndcCursorPos);
    AfxV2dZero(doutD->ndcCursorMove);

    if (config)
    {
        doutD->extent[0] = config->whd[0];
        doutD->extent[1] = config->whd[1];
        doutD->extent[2] = config->whd[2];

        // swapchain-related data
        doutD->pixelFmt = config->pixelFmt;
        doutD->colorSpc = config->colorSpc;
        doutD->bufUsage |= config->bufUsage;
        doutD->bufCnt = config->bufCnt;

        doutD->presentAlpha = config->presentAlpha;
        doutD->presentTransform = config->presentTransform;
        doutD->presentMode = config->presentMode;
        doutD->clipped = !!config->clipped;
    }

    doutD->auxDsFmt[0] = NIL;
    doutD->auxDsFmt[1] = NIL;

    doutD->idd = NIL;
    doutD->vmt = NIL;

    doutD->caption = AfxAllocateString(512, NIL, 0);
    doutD->buffers = NIL;

    if (config && config->udd)
        doutD->udd = config->udd;

    if (ddrv->vmt->dout && ddrv->vmt->dout(dout, config->endpoint, config)) AfxThrowError();
    else
    {
        AfxAssert(doutD->vmt);

        AfxAssert(doutD->resolution[0]);
        AfxAssert(doutD->resolution[1]);
        AfxAssert(doutD->resolution[2]);
        AfxAssertExtent(doutD->resolution[0], doutD->extent[0]);
        AfxAssertExtent(doutD->resolution[1], doutD->extent[1]);
        AfxAssertExtent(doutD->resolution[2], doutD->extent[2]);

        AfxAssert(doutD->bufCnt);
        AfxAssert(doutD->bufUsage & AFX_TEX_USAGE_DRAW);
        AfxAssert(doutD->refreshRate);
        AfxAssert(doutD->wpOverHp);
        AfxAssert(doutD->wrOverHr);
        AfxAssert(doutD->wwOverHw);

        if (!doutD->buffers && !(doutD->buffers = AfxAllocate(mem, doutD->bufCnt * sizeof(doutD->buffers[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else for(afxNat i = 0; i < doutD->bufCnt; i++)
            doutD->buffers[i].surf = NIL, doutD->buffers[i].canv = NIL;;

        if (err && doutD->buffers)
        {
            _AfxDoutFreeAllBuffers(dout);
            AfxDeallocate(mem, doutD->buffers);
        }
    }
    return err;
}

_AFX afxError AfxAcquireDrawOutputs(afxNat cnt, afxDrawOutput dout[], afxDrawOutputConfig const config[]) // file, window, desktop, widget, etc; physical or virtual VDUs.
{
    AfxEntry("cnt=%u,config=%p,dout=%p", cnt, config, dout);
    afxError err = AFX_ERR_NONE;
    afxDrawDriver ddrv;

    if (!AfxGetDrawDriver(config ? config->drvIdx : 0, &ddrv)) AfxThrowError();
    else
    {
        AfxAssertObject(ddrv, AFX_FCC_DDRV);

        afxDrawSystem dsys;
        AfxGetDrawSystem(&dsys);
        AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
        struct _afxDsysD* dsysD;
        _AfxGetDsysD(dsys, &dsysD);
        AfxAssertType(dsysD, AFX_FCC_DSYS);

        if (AfxAcquireNamedObjects(cnt, dout, AFX_FCC_DOUT, "Draw Output")) AfxThrowError();
        else
        {
            afxNat slots[32] = { 0 };

            if (AfxAllocateObjects(cnt, dout, &dsysD->outputs)) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < cnt; i++)
                {
                    struct _afxDoutD *doutD;
                    _AfxGetDoutD(dout[i], &doutD, dsysD);
                    
                    if (_AfxDoutCtor(dout[i], doutD, &config[i]))
                    {
                        AfxThrowError();
                        afxNat j = i;

                        for (afxNat k = 0; k < i; k++)
                            if (_AfxDoutDtor(dout[j--], doutD))
                                AfxThrowError();
                    }
                }
            }

            if (err)
                AfxReleaseObjects(cnt, dout, NIL);
        }
    }

    if (err)
    {
        for (afxNat i = 0; i < cnt; i++)
            dout[i] = NIL;
    }
    else
    {
        AfxAssertObjects(cnt, dout, AFX_FCC_DOUT);
    }
    return err;
}
