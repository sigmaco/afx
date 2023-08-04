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

#define _AFX_DRAW_CONTEXT_C
//#define _AFX_DRAW_DRIVER_C
#include "_classified/afxDrawClassified.h"

AFX afxClassSpecification const _AfxDopClassSpec;
AFX afxClassSpecification const _AfxCanvClassSpec;
AFX afxClassSpecification const _AfxPipClassSpec;
AFX afxClassSpecification const _AfxLegtClassSpec;

AFX afxClassSpecification const _AfxVbufClassSpec;
AFX afxClassSpecification const _AfxIbufClassSpec;
AFX afxClassSpecification const _AfxBufClassSpec;
AFX afxClassSpecification const _AfxSampClassSpec;
AFX afxClassSpecification const _AfxTexClassSpec;
AFX afxClassSpecification const _AfxSurfClassSpec;
AFX afxClassSpecification const _AfxShdClassSpec;

_AFX void _AfxDctxFreeAllQueueSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    if (dctx->ports)
    {
        for (afxNat i = 0; i < dctx->cachedPortCnt; i++)
        {
            for (afxNat j = 0; j < dctx->ports[i].queueCnt; j++)
            {
                afxDrawQueue dque = dctx->ports[i].queues[j];

                if (dque)
                {
                    AfxReleaseObject(&dque->obj);
                    dctx->ports[i].queues[j] = NIL;
                }
            }
            AfxDeallocate(dctx->mem, dctx->ports[i].queues);
        }
        AfxDeallocate(dctx->mem, dctx->ports);
    }
}

_AFX void _AfxDctxFreeAllInputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    if (dctx->inputs)
    {
        for (afxNat i = 0; i < dctx->inputCap; i++)
        {
            afxDrawInput din = dctx->inputs[i];

            if (din)
            {
                AfxDisconnectDrawInput(din, NIL);
                dctx->inputs[i] = NIL;
                --dctx->inputCnt;
            }
        }
        dctx->inputCap = 0;
        AfxDeallocate(dctx->mem, dctx->inputs);
        dctx->inputs = NIL;
    }
}

_AFX void _AfxDctxFreeAllOutputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    if (dctx->outputs)
    {
        for (afxNat i = 0; i < dctx->outputCap; i++)
        {
            afxDrawOutput dout = dctx->outputs[i];

            if (dout)
            {
                AfxDisconnectDrawOutput(dout, NIL);
                dctx->outputs[i] = NIL;
                --dctx->outputCnt;
            }
        }
        dctx->outputCap = 0;
        AfxDeallocate(dctx->mem, dctx->outputs);
        dctx->outputs = NIL;
    }
}

_AFX afxClass* AfxGetTextureClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->texClass;
    AfxAssertClass(cls, AFX_FCC_TEX);
    return cls;
}

_AFX afxClass* AfxGetIndexBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->ibufClass;
    AfxAssertClass(cls, AFX_FCC_IBUF);
    return cls;
}

_AFX afxClass* AfxGetVertexBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->vbufClass;
    AfxAssertClass(cls, AFX_FCC_VBUF);
    return cls;
}

_AFX afxClass* AfxGetBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->bufClass;
    AfxAssertClass(cls, AFX_FCC_BUF);
    return cls;
}

_AFX afxClass* AfxGetSamplerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->smpClass;
    AfxAssertClass(cls, AFX_FCC_SAMP);
    return cls;
}

_AFX afxClass* AfxGetSurfaceClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->surfClass;
    AfxAssertClass(cls, AFX_FCC_SURF);
    return cls;
}

_AFX afxClass* AfxGetShaderClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->shdClass;
    AfxAssertClass(cls, AFX_FCC_SHD);
    return cls;
}

_AFX afxClass* AfxGetDrawOperationClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->dopClass;
    AfxAssertClass(cls, AFX_FCC_DOP);
    return cls;
}

_AFX afxClass* AfxGetLegoClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->legtClass;
    AfxAssertClass(cls, AFX_FCC_LEGO);
    return cls;
}

_AFX afxClass* AfxGetPipelineClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->pipClass;
    AfxAssertClass(cls, AFX_FCC_PIP);
    return cls;
}

_AFX afxClass* AfxGetCanvasClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = &dctx->canvClass;
    AfxAssertClass(cls, AFX_FCC_CANV);
    return cls;
}

_AFX afxNat AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCnt, first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;

    for (afxNat i = 0; i < dctx->inputCap; i++)
    {
        afxDrawInput din = dctx->inputs[i];

        if (din)
        {
            AfxAssertObject(din, AFX_FCC_DIN);

            ++cnt2;

            if (f(din, udd))
                break;

            if (cnt2 == cnt)
                break;
        }

    }
    return cnt2;
}

_AFX afxNat AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCnt, first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;

    for (afxNat i = 0; i < dctx->outputCap; i++)
    {
        afxDrawOutput dout = dctx->outputs[i];

        if (dout)
        {
            AfxAssertObject(dout, AFX_FCC_DOUT);

            ++cnt2;

            if (f(dout, udd))
                break;

            if (cnt2 == cnt)
                break;
        }

    }
    return cnt2;
}

_AFX afxNat AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetTextureClass(dctx);
    AfxAssertClass(cls, AFX_FCC_TEX);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)tex);
}

_AFX afxNat AfxEnumerateIndexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetIndexBufferClass(dctx);
    AfxAssertClass(cls, AFX_FCC_IBUF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)ibuf);
}

_AFX afxNat AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetVertexBufferClass(dctx);
    AfxAssertClass(cls, AFX_FCC_VBUF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)vbuf);
}

_AFX afxNat AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, AFX_FCC_BUF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)buf);
}

_AFX afxNat AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler smp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, AFX_FCC_SAMP);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)smp);
}

_AFX afxNat AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetShaderClass(dctx);
    AfxAssertClass(cls, AFX_FCC_SHD);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)shd);
}

_AFX afxNat AfxEnumerateSurfaces(afxDrawContext dctx, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetSurfaceClass(dctx);
    AfxAssertClass(cls, AFX_FCC_SURF);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)surf);
}

_AFX afxNat AfxEnumerateDrawOperations(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetDrawOperationClass(dctx);
    AfxAssertClass(cls, AFX_FCC_DOP);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)dop);
}

_AFX afxNat AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetPipelineClass(dctx);
    AfxAssertClass(cls, AFX_FCC_PIP);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)pip);
}

_AFX afxNat AfxEnumerateLegos(afxDrawContext dctx, afxNat first, afxNat cnt, afxLego lego[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetLegoClass(dctx);
    AfxAssertClass(cls, AFX_FCC_LEGO);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)lego);
}

_AFX afxNat AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxClass *cls = AfxGetCanvasClass(dctx);
    AfxAssertClass(cls, AFX_FCC_CANV);
    //AfxAssertRange(AfxGetClassInstanceCount(cls), first, cnt);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)canv);
}

_AFX afxNat AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCnt, first, cnt);
    AfxAssert(dout);
    afxNat cnt2 = 0;
    afxNat total = 0;

    for (afxNat i = 0; i < dctx->outputCap; i++)
    {
        afxDrawOutput dout2 = dctx->outputs[i];

        if (dout2)
        {
            if (total >= first)
            {
                AfxAssertObject(dout2, AFX_FCC_DOUT);
                dout[i] = dout2;
                ++cnt2;

            }
            ++total;
        }

    }
    return cnt2;
}

_AFX afxNat AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->inputCnt, first, cnt);
    AfxAssert(din);
    afxNat cnt2 = 0;
    afxNat total = 0;

    for (afxNat i = 0; i < dctx->inputCap; i++)
    {
        afxDrawInput din2 = dctx->inputs[i];

        if (din2)
        {
            if (total >= first)
            {
                AfxAssertObject(din2, AFX_FCC_DIN);
                din[i] = din2;
                ++cnt2;

            }
            ++total;
        }

    }
    return cnt2;
}

_AFX afxError AfxDisconnectAllDrawInputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    _AfxDctxFreeAllInputSlots(dctx);
    return err;
}

_AFX afxError AfxDisconnectAllDrawOutputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    _AfxDctxFreeAllOutputSlots(dctx);
    return err;
}

_AFX afxDrawOutput AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCap, slotIdx, 1);
    return dctx->outputs[slotIdx];
}

_AFX afxDrawInput AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->inputCap, slotIdx, 1);
    return dctx->inputs[slotIdx];
}

_AFX afxNat AfxGetConnectedDrawOutputCount(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx->outputCnt;
}

_AFX afxNat AfxGetConnectedDrawInputCount(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    return dctx->inputCnt;
}

_AFX afxDrawQueue AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(portIdx == 0);
    AfxAssertRange(dctx->cachedPortCnt, portIdx, 1);
    AfxAssertRange(dctx->ports[portIdx].queueCnt, queIdx, 1);
    return dctx->ports[portIdx].queues[queIdx];
}

_AFX afxError AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(portIdx == 0);
    AfxAssertRange(dctx->cachedPortCnt, portIdx, 1);
    AfxAssert(queIdx);
    //AfxAssertRange(dctx->ports[portIdx].queueCnt, queIdx, 1);
    
    (void)timeout;
    *queIdx = AFX_INVALID_INDEX;

    afxNat idx = dctx->ports[portIdx].lastReqQueIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % dctx->ports[portIdx].queueCnt;
        afxDrawQueue dque = dctx->ports[portIdx].queues[idx];
        AfxAssertObject(dque, AFX_FCC_DQUE);

        if (dque)
        {
            //AfxAssert(!surf->swapchain.chain);
            *queIdx = idx;
            break;
        }

        if (timeout > AfxGetTime(&t2) - time)
        {
            AfxThrowError();
            *queIdx = AFX_INVALID_INDEX;
            break;
        }
    }

    if (!err)
        dctx->ports[portIdx].lastReqQueIdx = idx;

    return err;
}

_AFX afxNat AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(portIdx == 0);
    AfxAssertRange(dctx->cachedPortCnt, portIdx, 1);
    return dctx->ports[portIdx].queueCnt;
}

_AFX afxMemory AfxGetDrawContextMemory(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = dctx->mem;
    AfxAssertObject(mem, AFX_FCC_MEM);
    return mem;
}

_AFX afxDrawDriver AfxGetDrawContextDriver(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawDriver ddrv = AfxObjectGetProvider(&dctx->obj);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError AfxAcquireDrawContexts(afxDrawContextSpecification const *spec, afxNat cnt, afxDrawContext dctx[])
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
            if (AfxClassAcquireObjects(AfxGetDrawContextClass(ddrv), NIL, 1, spec, (afxObject**)&dctx[i], AfxSpawnHint()))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxAssertObject(dctx[j], AFX_FCC_DCTX);
                    AfxReleaseObject(&dctx[j]->obj);
                    dctx[j] = NIL;
                }
                break;
            }
            else
            {
                AfxAssertObject(dctx[i], AFX_FCC_DCTX);
            }
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dctx[i] = NIL;

    return err;
}

_AFX afxBool _AfxDctxEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (void*)obj;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDctxEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (void*)obj;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDctxDtor(afxDrawContext dctx)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    AfxDisconnectAllDrawInputs(dctx);
    AfxDisconnectAllDrawOutputs(dctx);

    _AfxDropClassChain(&dctx->provisions);

    _AfxDctxFreeAllQueueSlots(dctx);
    _AfxDctxFreeAllInputSlots(dctx);
    _AfxDctxFreeAllOutputSlots(dctx);
    
    AfxReleaseArena(&dctx->aren);
    AfxReleaseObject(&(dctx->mem->obj));

    return AFX_SUCCESS;
}

_AFX afxError _AfxDctxCtor(void *cache, afxNat idx, afxDrawContext dctx, afxDrawContextSpecification const *specs)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawContextSpecification const *spec = &specs[idx];
    AfxAssert(spec);
    (void)cache;

    afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    dctx->running = FALSE;

    if (!(dctx->mem = spec->genrlMem))
    {
        dctx->mem = AfxGetDrawMemory();
    }

    AfxAssertObject(dctx->mem, AFX_FCC_MEM);

    AfxAcquireArena(dctx->mem, &dctx->aren, NIL, AfxSpawnHint());

    if (!(AfxObjectReacquire(&dctx->mem->obj, &dctx->obj, NIL, NIL, NIL))) AfxThrowError();
    else
    {
        afxChain *provisions = &dctx->provisions;
        AfxAcquireChain(provisions, dctx);

        AfxMountClass(&dctx->bufClass, provisions, &_AfxBufClassSpec);
        AfxMountClassInherited(&dctx->vbufClass, AfxGetBufferClass(dctx), provisions, &_AfxVbufClassSpec);
        AfxMountClassInherited(&dctx->ibufClass, AfxGetBufferClass(dctx), provisions, &_AfxIbufClassSpec);
        AfxMountClass(&dctx->smpClass, provisions, &_AfxSampClassSpec);
        AfxMountClass(&dctx->texClass, provisions, &_AfxTexClassSpec);
        AfxMountClassInherited(&dctx->surfClass, AfxGetTextureClass(dctx), provisions, &_AfxSurfClassSpec);
        AfxMountClass(&dctx->shdClass, provisions, &_AfxShdClassSpec);

        AfxMountClass(&dctx->legtClass, provisions, &_AfxLegtClassSpec);
        AfxMountClass(&dctx->canvClass, provisions, &_AfxCanvClassSpec);
        AfxMountClass(&dctx->pipClass, provisions, &_AfxPipClassSpec);
        AfxMountClass(&dctx->dopClass, provisions, &_AfxDopClassSpec);

        dctx->vmt = NIL;
        dctx->idd = NIL;

        dctx->cachedPortCnt = AfxGetDrawDriverPortCount(ddrv); //spec && spec->portCnt ? spec->portCnt : 1;
        AfxAssert(dctx->cachedPortCnt);
        dctx->inputCnt = 0;
        dctx->inputCap = 1;
        dctx->outputCnt = 0;
        dctx->outputCap = 1;

        dctx->inputs = NIL;
        dctx->outputs = NIL;
        dctx->ports = NIL;

        if (dctx->inputCap && !(dctx->inputs = AfxAllocate(dctx->mem, dctx->inputCap * sizeof(dctx->inputs[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < dctx->inputCap; i++)
                dctx->inputs[i] = NIL;

            if (dctx->outputCap && !(dctx->outputs = AfxAllocate(dctx->mem, dctx->outputCap * sizeof(dctx->outputs[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < dctx->outputCap; i++)
                    dctx->outputs[i] = NIL;

                if (dctx->cachedPortCnt && !(dctx->ports = AfxAllocate(dctx->mem, dctx->cachedPortCnt * sizeof(dctx->ports[0]), 0, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < dctx->cachedPortCnt; i++)
                    {
                        dctx->ports[i].queueCnt = 1;// spec && spec->queuesPerPort && spec->queuesPerPort[i] ? spec->queuesPerPort[i] : 1;
                        dctx->ports[i].lastReqQueIdx = (dctx->ports[i].lastReqQueIdx + 1) % dctx->ports[i].queueCnt;
                        //dctx->ports[i].minRecyclSubmCnt = 2;
                        dctx->ports[i].queues = NIL;

                        if (dctx->ports[i].queueCnt && !(dctx->ports[i].queues = AfxAllocate(dctx->mem, dctx->ports[i].queueCnt * sizeof(dctx->ports[0].queues), 0, AfxSpawnHint())))
                        {
                            AfxThrowError();

                            for (afxNat j = 0; j < i; j++)
                            {
                                AfxDeallocate(dctx->mem, dctx->ports[j].queues);
                                dctx->ports[j].queues = NIL;
                            }
                        }
                        else
                        {
                            for (afxNat j = 0; j < dctx->ports[i].queueCnt; j++)
                                dctx->ports[i].queues[j] = NIL;
                        }
                    }

                    if (!err)
                    {
                        if (ddrv->vmt->dctx && ddrv->vmt->dctx(dctx, spec)) AfxThrowError();
                        else
                        {
                            AfxAssert(dctx->vmt);

                            for (afxNat i = 0; i < dctx->cachedPortCnt; i++)
                            {
                                afxDrawQueueSpecification dqueSpec = { 0 };
                                dqueSpec.portIdx = i;
                                dqueSpec.owner = dctx;

                                if (AfxAcquireDrawQueues(ddrv, &dqueSpec, dctx->ports[i].queueCnt, dctx->ports[i].queues))
                                {
                                    AfxThrowError();

                                    for (afxNat j = 0; j < i; j++)
                                        for (afxNat k = 0; k < dctx->ports[i].queueCnt; k++)
                                            AfxReleaseObject(&dctx->ports[j].queues[k]->obj);
                                }
                            }

                            if (!err)
                            {
                                dctx->running = TRUE;
                            }

                        }
                    }

                    if (err && dctx->ports)
                        _AfxDctxFreeAllQueueSlots(dctx);
                }

                if (err && dctx->outputs)
                    _AfxDctxFreeAllOutputSlots(dctx);
            }

            if (err && dctx->inputs)
                _AfxDctxFreeAllInputSlots(dctx);
        }

        if (err)
        {
            _AfxDropClassChain(&dctx->provisions);

            AfxReleaseObject(&(dctx->mem->obj));
        }
    }
    return err;
}

_AFX afxClassSpecification const _AfxDctxClassSpec;

afxClassSpecification const _AfxDctxClassSpec =
{
    AFX_FCC_DCTX,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawContext)),
    NIL,
    (void*)_AfxDctxCtor,
    (void*)_AfxDctxDtor,
    .event = _AfxDctxEventHandler,
    .eventFilter = _AfxDctxEventFilter,
    "afxDrawContext",
    NIL
};
