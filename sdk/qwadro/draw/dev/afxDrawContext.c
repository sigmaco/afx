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
#define _AFX_DRAW_CONTEXT_C
//#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_BRIDGE_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/draw/afxDrawSystem.h"

extern afxClassConfig const _vbufMgrCfg;
extern afxClassConfig const _ibufMgrCfg;

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

_AVX afxDrawInput AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat conNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AfxAssertRange(AfxCountDrawInputConnections(dctx), conNo, 1);
    afxDrawInput din = NIL;
    if (AfxEnumerateConnectedDrawInputs(dctx, conNo, 1, &din))
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);
    }
    return din;
}

_AVX afxDrawOutput AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat conNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AfxAssertRange(AfxCountDrawOutputConnections(dctx), conNo, 1);
    afxDrawOutput dout = NIL;
    if (AfxEnumerateConnectedDrawOutputs(dctx, conNo, 1, &dout))
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
    }
    return dout;
}

_AVX afxError AfxDisconnectDrawInputs(afxDrawContext dctx)
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

_AVX afxError AfxDisconnectDrawOutputs(afxDrawContext dctx)
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

_AVX afxDrawBridge AfxGetDrawBridge(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange((afxNat)dctx->ownedBridges.cnt, portIdx, 1);
    afxDrawBridge ddge = AFX_REBASE(AfxFindFirstLinkage(&dctx->ownedBridges, portIdx), AFX_OBJECT(afxDrawBridge), dctx);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    return ddge;
}

_AVX afxNat AfxCountDrawBridges(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->ownedBridges.cnt;
}

_AVX afxDrawDevice AfxGetDrawContextDevice(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireDrawContexts(afxNat ddevId, afxNat cnt, afxDrawContextConfig const cfg[], afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(contexts);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxManager* cls = AfxGetDrawContextClass(ddev);
        AfxAssertClass(cls, afxFcc_DCTX);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { ddev, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(cnt, contexts, afxFcc_DCTX);
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
#if 0
    afxNat rslt = 0;

    // This function is used during draw context acquisition to process default resource loading. Due to generic manager method not processing uncounted 

    //if (mgr->instCnt)
    {
        AfxEnterSlockShared((void*)&mgr->poolLock);
        rslt = _AfxClsCurateObjects(mgr, FALSE, first, cnt, f, udd);
        AfxExitSlockShared((void*)&mgr->poolLock);
    }
    return rslt;
#endif
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
