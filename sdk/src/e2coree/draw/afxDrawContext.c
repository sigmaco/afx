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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
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
    afxClass *cls = &dctx->legos;
    AfxAssertClass(cls, afxFcc_LEGO);
    return cls;
}

_AFX afxNat AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->buffers, first, cnt, (afxObject*)buf);
}

_AFX afxNat AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(canv);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->canvases, first, cnt, (afxObject*)canv);
}

_AFX afxNat AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(pipelines);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->pipelines, first, cnt, (afxObject*)pipelines);
}

_AFX afxNat AfxEnumerateBindSchemas(afxDrawContext dctx, afxNat first, afxNat cnt, afxBindSchema schemas[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(schemas);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->legos, first, cnt, (afxObject*)schemas);
}

_AFX afxNat AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samplers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(samplers);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->samplers, first, cnt, (afxObject*)samplers);
}

_AFX afxNat AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(shaders);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->shaders, first, cnt, (afxObject*)shaders);
}

_AFX afxNat AfxEnumerateRasters(afxDrawContext dctx, afxNat first, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(rasters);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->rasters, first, cnt, (afxObject*)rasters);
}

_AFX afxNat AfxCountConnectedDrawInputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->inlinks.cnt;
}

_AFX afxNat AfxCountConnectedDrawOutputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx->outlinks.cnt;
}

_AFX afxNat AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountConnectedDrawInputs(dctx), first, cnt);
    AfxAssert(din);
    afxNat cnt2 = 0;
    afxNat total = 0;
    afxLinkage* lnk = AfxGetLastLinkage(&dctx->inlinks);
    while (lnk)
    {
        afxDrawInput din2 = AfxRebase(lnk, afxDrawInput, dctx);
        AfxAssertObjects(1, &din2, afxFcc_DIN);
        
        if (total >= first)
        {
            din[cnt2] = din2;
            ++cnt2;

        }
        ++total;

        lnk = AfxGetNextLinkage(lnk);
    }
    return cnt2;
}

_AFX afxNat AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountConnectedDrawOutputs(dctx), first, cnt);
    AfxAssert(dout);
    afxNat cnt2 = 0;
    afxNat total = 0;
    afxLinkage* lnk = AfxGetLastLinkage(&dctx->outlinks);
    while (lnk)
    {
        afxDrawOutput dout2 = AfxRebase(lnk, afxDrawOutput, dctx);
        AfxAssertObjects(1, &dout2, afxFcc_DOUT);

        if (total >= first)
        {
            dout[cnt2] = dout2;
            ++cnt2;

        }
        ++total;

        lnk = AfxGetNextLinkage(lnk);
    }
    return cnt2;
}

_AFX afxNat AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountConnectedDrawInputs(dctx), first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;
    afxNat total = 0;
    afxLinkage* lnk = AfxGetLastLinkage(&dctx->inlinks);
    while (lnk)
    {
        afxDrawInput din = AfxRebase(lnk, afxDrawInput, dctx);
        AfxAssertObjects(1, &din, afxFcc_DIN);
        lnk = AfxGetNextLinkage(lnk); // prevent possible unlink

        if (total >= first)
        {
            ++cnt2;

            if (f(din, udd))
                break;

            if (cnt2 == cnt)
                break;
        }
        ++total;
    }
    return cnt2;
}

_AFX afxNat AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountConnectedDrawOutputs(dctx), first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;
    afxNat total = 0;
    afxLinkage* lnk = AfxGetLastLinkage(&dctx->outlinks);
    while (lnk)
    {
        afxDrawOutput dout = AfxRebase(lnk, afxDrawOutput, dctx);
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        lnk = AfxGetNextLinkage(lnk); // prevent possible unlink

        if (total >= first)
        {
            ++cnt2;

            if (f(dout, udd))
                break;

            if (cnt2 == cnt)
                break;
        }
        ++total;
    }
    return cnt2;
}

_AFX afxDrawInput AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat no)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(AfxCountConnectedDrawInputs(dctx), no, 1);    
    afxDrawInput din;
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
    AfxAssertRange(AfxCountConnectedDrawOutputs(dctx), no, 1);
    afxDrawOutput dout;
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

_AFX afxNat AfxCurateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
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
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
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

_AFX afxNat AfxCountDrawContexts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxCountInstances(cls);
}
