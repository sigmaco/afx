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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>


#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
#define _AVX_DRAW_OUTPUT_C
#include "qwadro/draw/afxDrawSystem.h"

extern afxClassConfig const _vbufMgrCfg;
extern afxClassConfig const _ibufMgrCfg;

_AVX afxManager* AfxGetQueryPoolClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->queries;
    AfxAssertClass(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxManager* AfxGetVertexInputClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->vinputs;
    AfxAssertClass(cls, afxFcc_VIN);
    return cls;
}

_AVX afxManager* AfxGetRasterClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->rasters;
    AfxAssertClass(cls, afxFcc_RAS);
    return cls;
}

_AVX afxManager* AfxGetBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->buffers;
    AfxAssertClass(cls, afxFcc_BUF);
    return cls;
}

_AVX afxManager* AfxGetSamplerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->samplers;
    AfxAssertClass(cls, afxFcc_SAMP);
    return cls;
}

_AVX afxManager* AfxGetPipelineClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->pipelines;
    AfxAssertClass(cls, afxFcc_PIP);
    return cls;
}

_AVX afxManager* AfxGetCanvasClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->canvases;
    AfxAssertClass(cls, afxFcc_CANV);
    return cls;
}

_AVX afxManager* AfxGetRasterizerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->rasterizers;
    AfxAssertClass(cls, afxFcc_RAZR);
    return cls;
}

_AVX afxManager* AfxGetShaderClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->shaders;
    AfxAssertClass(cls, afxFcc_SHD);
    return cls;
}

_AVX afxManager* AfxGetBindSchemaClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxManager *cls = &dctx->schemas;
    AfxAssertClass(cls, afxFcc_BSCH);
    return cls;
}

_AVX afxNat AfxCountDrawInputConnections(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->inputs.cnt;
}

_AVX afxNat AfxCountDrawOutputConnections(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->outputs.cnt;
}

_AVX afxDrawBridge AfxGetDrawBridge(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange((afxNat)dctx->ownedBridgeCnt, portIdx, 1);
    afxDrawBridge ddge = dctx->ownedBridges[portIdx];
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    return ddge;
}

_AVX afxNat AfxCountDrawBridges(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->ownedBridgeCnt;
}

_AVX afxDrawDevice AfxGetDrawContextDevice(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxNat AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(inputs);
    AfxAssert(conCnt);

    afxNat rslt = 0;
    afxNat total = 0;
    afxDrawInput din;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din, &dctx->inputs, dctx)
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);

        if (total >= baseCon)
        {
            inputs[rslt] = din;
            ++rslt;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxNat AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(outputs);
    AfxAssert(conCnt);

    afxNat rslt = 0;
    afxNat total = 0;
    afxDrawOutput dout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawOutput), dout, &dctx->inputs, dctx)
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        if (total >= baseCon)
        {
            outputs[rslt] = dout;
            ++rslt;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxNat AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(conCnt);
    AfxAssert(f);

    afxNat rslt = 0;
    afxNat total = 0;
    afxDrawInput din;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din, &dctx->inputs, dctx)
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(din, udd))
                break;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxNat AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(conCnt);
    AfxAssert(f);

    afxNat rslt = 0;
    afxNat total = 0;
    afxDrawOutput dout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawOutput), dout, &dctx->outputs, dctx)
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(dout, udd))
                break;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxBool AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat conNo, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxBool rslt = FALSE;
    afxDrawInput din = NIL;
    while (AfxEnumerateConnectedDrawInputs(dctx, conNo, 1, &din))
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);
        AfxAssert(input);
        *input = din;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_AVX afxBool AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat conNo, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxBool rslt = FALSE;
    afxDrawOutput dout = NIL;
    while (AfxEnumerateConnectedDrawOutputs(dctx, conNo, 1, &dout))
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxAssert(output);
        *output = dout;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_AVX afxError AfxDisconnectDrawContext(afxDrawContext dctx, afxBool inputs, afxBool outputs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (inputs)
    {
        afxDrawInput din;
        while (AfxGetConnectedDrawInput(dctx, 0, &din))
        {
            AfxAssertObjects(1, &din, afxFcc_DIN);
            AfxDisconnectDrawInput(din);
            break;
        }
    }

    if (outputs)
    {
        afxDrawOutput dout;
        while (AfxGetConnectedDrawOutput(dctx, 0, &dout))
        {
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            AfxDisconnectDrawOutput(dout);
            break;
        }
    }
    return err;
}

_AVX afxError AfxWaitForIdleDrawContext(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (!dctx->waitCb)
    {
        afxNat bridgeCnt = dctx->ownedBridgeCnt;

        for (afxNat i = 0; i < bridgeCnt; i++)
        {
            afxDrawBridge ddge = dctx->ownedBridges[i];
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxNat queCnt = AfxCountDrawQueues(ddge);

            for (afxNat j = 0; j < queCnt; j++)
                AfxWaitForIdleDrawQueue(ddge, j);
        }
    }
    else if (dctx->waitCb(dctx))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDctxStdDtor(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    AfxDisconnectDrawContext(dctx, TRUE, TRUE);
    AfxWaitForIdleDrawContext(dctx);

    AfxCleanUpChainedManagers(&dctx->ctx.classes);

    AfxAssertObjects(dctx->ownedBridgeCnt, dctx->ownedBridges, afxFcc_DDGE);
    AfxDeallocate(dctx->ownedBridges);

    AfxAssert(AfxChainIsEmpty(&dctx->ctx.classes));

    AfxDeallocateArena(&dctx->aren);

    return err;
}

_AVX afxError _AvxDctxStdCtor(afxDrawContext dctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AfxAssert(spec);

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawContextConfig const *config = ((afxDrawContextConfig const *)cookie->udd[1]) + cookie->no;

    dctx->running = FALSE;

    AfxAllocateArena(NIL, &dctx->aren, NIL, AfxHere());

    dctx->clipCfg = ddev->clipCfg;

    AfxSetUpChain(&dctx->inputs, dctx);
    AfxSetUpChain(&dctx->outputs, dctx);

    afxChain *classes = &dctx->ctx.classes;
    AfxSetUpChain(classes, (void*)dctx);

    // Acquire bridges and queues

    afxNat bridgeCnt = AfxCountDrawPorts(ddev);
    AfxAssert(bridgeCnt);
    afxDrawBridge* ownedBridges;
    dctx->ownedBridges = NIL;

    if (!(ownedBridges = AfxAllocate(bridgeCnt, sizeof(ownedBridges[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat j = 0; j < bridgeCnt; j++)
        {
            afxDrawBridgeConfig ddgeSpec = { 0 };
            ddgeSpec.portIdx = j;
            
            if (AfxAcquireObjects(&ddev->ports[j].ddgeMgr, 1, (afxObject*)&ownedBridges[j], (void const*[]) { ddev, dctx, &ddgeSpec }))
            {
                AfxThrowError();
                AfxReleaseObjects(j, ownedBridges);
            }
            else
            {
                AfxAssertObjects(1, &ownedBridges[j], afxFcc_DDGE);
            }
        }

        if (err) AfxDeallocate(ownedBridges);
        else
        {
            AfxAssertObjects(bridgeCnt, ownedBridges, afxFcc_DDGE);
            dctx->ownedBridgeCnt = bridgeCnt;
            dctx->ownedBridges = ownedBridges;
        }
    }

    if (err)
    {
        AfxAssertObjects(dctx->ownedBridgeCnt, dctx->ownedBridges, afxFcc_DDGE);
        AfxDeallocate(dctx->ownedBridges);
    }

    return err;
}

_AVX afxClassConfig const _AvxDctxStdImplementation =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Draw Device Context Management",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawContext)),
    .ctor = (void*)_AvxDctxStdCtor,
    .dtor = (void*)_AvxDctxStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireDrawContext(afxNat ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxManager* cls = AfxGetDrawContextClass(ddev);
        AfxAssertClass(cls, afxFcc_DCTX);
        AfxAssert(context);

        if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { ddev, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, context, afxFcc_DCTX);
        }
    }
    return err;
}

_AVX afxNat AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* mgr = AfxGetDrawContextClass(ddev);
    AfxAssertClass(mgr, afxFcc_DCTX);
    return AfxInvokeObjects(mgr, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(contexts);
    AfxAssert(cnt);
    afxManager* cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)contexts);
}

_AVX afxNat AfxCountDrawContexts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager*cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxCountObjects(cls);
}
