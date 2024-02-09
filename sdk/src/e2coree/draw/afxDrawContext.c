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

// This section is part of SIGMA GL/2.

#define _AFX_DRAW_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/core/afxSystem.h"

extern afxClassConfig const _vbufClsConfig;
extern afxClassConfig const _ibufClsConfig;
AFXINL afxDrawSystem _AfxGetDsysData(void);

_AFX afxClass* AfxGetVertexInputClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->vinputs;
    AfxAssertClass(cls, afxFcc_VIN);
    return cls;
}

_AFX afxClass* AfxGetRasterClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->rasters;
    AfxAssertClass(cls, afxFcc_RAS);
    return cls;
}

_AFX afxClass* AfxGetBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->buffers;
    AfxAssertClass(cls, afxFcc_BUF);
    return cls;
}

_AFX afxClass* AfxGetSamplerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->samplers;
    AfxAssertClass(cls, afxFcc_SAMP);
    return cls;
}

_AFX afxClass* AfxGetPipelineClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->pipelines;
    AfxAssertClass(cls, afxFcc_PIP);
    return cls;
}

_AFX afxClass* AfxGetCanvasClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->canvases;
    AfxAssertClass(cls, afxFcc_CANV);
    return cls;
}

_AFX afxClass* AfxGetRasterizerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->rasterizers;
    AfxAssertClass(cls, afxFcc_RAZR);
    return cls;
}

_AFX afxClass* AfxGetShaderClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->shaders;
    AfxAssertClass(cls, afxFcc_SHD);
    return cls;
}

_AFX afxClass* AfxGetBindSchemaClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->schemas;
    AfxAssertClass(cls, afxFcc_BSCH);
    return cls;
}

_AFX afxClass* AfxGetQueryPoolClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->queries;
    AfxAssertClass(cls, afxFcc_QRYP);
    return cls;
}

_AFX afxClass* AfxGetFenceClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->fences;
    AfxAssertClass(cls, afxFcc_FENC);
    return cls;
}

_AFX afxClass* AfxGetSemaphoreClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->semaphores;
    AfxAssertClass(cls, afxFcc_SEM);
    return cls;
}

_AFX afxNat AfxCountDrawInputConnections(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->dinSlotCnt;
}

_AFX afxNat AfxCountDrawOutputConnections(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->doutSlotCnt;
}

_AFX afxNat AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(inputs);
    AfxAssert(cnt);

    afxNat rslt = 0;
    afxNat total = 0;
    afxNat slotCnt = dctx->dinSlotCnt;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxDrawInput din = dctx->dinSlots[i].din;
        
        if (din)
        {
            AfxAssertObjects(1, &din, afxFcc_DIN);

            if (total >= first)
            {
                inputs[rslt] = din;
                ++rslt;
            }
            ++total;
        }
    }
    return rslt;
}

_AFX afxNat AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(outputs);
    AfxAssert(cnt);

    afxNat rslt = 0;
    afxNat total = 0;
    afxNat slotCnt = dctx->doutSlotCnt;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxDrawOutput dout = dctx->doutSlots[i].dout;

        if (dout)
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);

            if (total >= first)
            {
                outputs[rslt] = dout;
                ++rslt;
            }
            ++total;
        }
    }
    return rslt;
}

_AFX afxNat AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(f);

    afxNat rslt = 0;
    afxNat total = 0;
    afxNat slotCnt = dctx->dinSlotCnt;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxDrawInput din = dctx->dinSlots[i].din;

        if (din)
        {
            AfxAssertObjects(1, &din, afxFcc_DIN);

            if (total >= first)
            {
                ++rslt;

                if (f(din, udd))
                    break;

                if (rslt == cnt)
                    break;
            }
            ++total;
        }
    }
    return rslt;
}

_AFX afxNat AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(f);
    
    afxNat rslt = 0;
    afxNat total = 0;
    afxNat slotCnt = dctx->doutSlotCnt;

    for (afxNat i = 0; i < slotCnt; i++)
    {
        afxDrawOutput dout = dctx->doutSlots[i].dout;

        if (dout)
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);

            if (total >= first)
            {
                ++rslt;

                if (f(dout, udd))
                    break;

                if (rslt == cnt)
                    break;
            }
            ++total;
        }
    }
    return rslt;
}

_AFX afxDrawInput AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat no)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountDrawInputConnections(dctx), no, 1);    
    afxDrawInput din = NIL;
    if (AfxEnumerateConnectedDrawInputs(dctx, no, 1, &din))
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);
    }
    return din;
}

_AFX afxDrawOutput AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat no)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountDrawOutputConnections(dctx), no, 1);
    afxDrawOutput dout = NIL;
    if (AfxEnumerateConnectedDrawOutputs(dctx, no, 1, &dout))
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
    }
    return dout;
}

_AFX afxError AfxDisconnectDrawInputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawInput din;
    while ((din = AfxGetConnectedDrawInput(dctx, 0)))
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);
        AfxDisconnectDrawInput(din);
    }
    return err;
}

_AFX afxError AfxDisconnectDrawOutputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);   
    afxDrawOutput dout;
    while ((dout = AfxGetConnectedDrawOutput(dctx, 0)))
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxDisconnectDrawOutput(dout);
    }
    return err;
}

_AFX afxDrawQueue AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(dctx->openPortCnt, portIdx, 1);
    AfxAssertRange(dctx->openPorts[portIdx].dqueCnt, queIdx, 1);
    afxDrawQueue dque = dctx->openPorts[portIdx].queues[queIdx];
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    return dque;
}

_AFX afxNat AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(dctx->openPortCnt, portIdx, 1);
    return dctx->openPorts[portIdx].dqueCnt;
}

_AFX afxMmu AfxGetDrawContextMmu(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxMmu mmu = dctx->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxDrawDevice AfxGetDrawContextDevice(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxCountDrawContexts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxCountInstances(cls);
}

_AFX afxNat AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    afxClass* cls;

    if (ddev)
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        cls = AfxGetDrawContextClass(ddev);
    }
    else
    {
        afxDrawSystem dsys = _AfxGetDsysData();
        AfxAssertType(dsys, afxFcc_DSYS);
        cls = &dsys->dcontexts;            
    }
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext dctx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dctx);
    afxClass* cls;

    if (ddev)
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        cls = AfxGetDrawContextClass(ddev);
    }
    else
    {
        afxDrawSystem dsys = _AfxGetDsysData();
        AfxAssertType(dsys, afxFcc_DSYS);
        cls = &dsys->dcontexts;            
    }
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)dctx);
}

_AFX afxError AfxAcquireDrawContexts(afxNat ddevId, afxNat cnt, afxDrawContextConfig const cfg[], afxDrawContext dctx[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(ddev = AfxGetDrawDevice(ddevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = AfxGetDrawContextClass(ddev);
        AfxAssertClass(cls, afxFcc_DCTX);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)dctx, (void const*[]) { ddev, cfg }))
            AfxThrowError();

        AfxAssertObjects(cnt, dctx, afxFcc_DCTX);
    }
    return err;
}
