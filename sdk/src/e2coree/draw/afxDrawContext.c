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

// This section is part of SIGMA GL.

#define _AFX_DRAW_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_SYSTEM_C
//#define _AFX_DRAW_DRIVER_C

#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C
#define _AFX_CANVAS_C
#define _AFX_BUFFER_C
#define _AFX_INDEX_BUFFER_C
#define _AFX_VERTEX_BUFFER_C
#define _AFX_TEXTURE_C
#define _AFX_SURFACE_C
#define _AFX_PIPELINE_C
#define _AFX_PIPELINE_RIG_C
#define _AFX_SHADER_C
#include "afxDrawClassified.h"

_AFX afxNat AfxEnumerateBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxBuffer buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->buffers, first, cnt, (afxHandle*)buf);
}

_AFX afxNat AfxEnumerateCanvases(afxDrawContext dctx, afxNat first, afxNat cnt, afxCanvas canv[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(canv);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->canvases, first, cnt, (afxHandle*)canv);
}

_AFX afxNat AfxEnumerateIndexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ibuf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->indices, first, cnt, (afxHandle*)ibuf);
}

_AFX afxNat AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(pip);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->pipelines, first, cnt, (afxHandle*)pip);
}

_AFX afxNat AfxEnumeratePipelineRigs(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipelineRig lego[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(lego);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->legos, first, cnt, (afxHandle*)lego);
}

_AFX afxNat AfxEnumerateSamplers(afxDrawContext dctx, afxNat first, afxNat cnt, afxSampler samp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(samp);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->samplers, first, cnt, (afxHandle*)samp);
}

_AFX afxNat AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(shd);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->shaders, first, cnt, (afxHandle*)shd);
}

_AFX afxNat AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(vbuf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->vertices, first, cnt, (afxHandle*)vbuf);
}

_AFX afxNat AfxEnumerateSurfaces(afxDrawContext dctx, afxNat first, afxNat cnt, afxSurface surf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(surf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->surfaces, first, cnt, (afxHandle*)surf);
}

_AFX afxNat AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(tex);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return AfxEnumerateInstances(&dctx->textures, first, cnt, (afxHandle*)tex);
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
    AfxAssertRange(AfxCountInstances(&dctx->openPorts[portIdx].queues), queIdx, 1);
    afxDrawQueue dque;
    
    if (!AfxEnumerateInstances(&dctx->openPorts[portIdx].queues, queIdx, 1, (afxHandle*)&dque))
        dque = NIL;

    return dque;
}

_AFX afxNat AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(dctx->openPortCnt, portIdx, 1);
    return dctx->openPorts[portIdx].queues.pool.totalUsedCnt;
}

_AFX afxContext AfxGetDrawContextMemory(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxContext ctx = dctx->ctx;
    AfxAssertObjects(1, &ctx, afxFcc_CTX);
    return ctx;
}

_AFX afxDrawDevice AfxGetDrawContextDevice(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AFX afxError AfxAcquireDrawContexts(afxDrawSystem dsys, afxNat devId, afxNat cnt, afxDrawContextConfig const config[], afxDrawContext dctx[])
{
    AfxEntry("cnt=%u,config=%p", cnt, config);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxDrawDevice ddev;
    
    if (!(AfxGetDrawDevice(dsys, devId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxAcquireObjects(AfxGetDrawContextClass(ddev), cnt, (afxHandle*)dctx, (void*[]) { &devId, (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, dctx, afxFcc_DCTX);
    }
    return err;
}
