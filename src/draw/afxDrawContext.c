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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>


#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_INPUT_C
#include "../dev/AvxImplKit.h"

#define AFX_MAX_DRAW_BRIDGE_PER_CONTEXT 16

_AVX afxDrawDevice AfxGetDrawContextDevice(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxClass* AfxGetQueryPoolClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->qrypCls;
    AfxAssertClass(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxClass* AfxGetVertexInputClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->vinCls;
    AfxAssertClass(cls, afxFcc_VIN);
    return cls;
}

_AVX afxClass* AfxGetRasterClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->rasCls;
    AfxAssertClass(cls, afxFcc_RAS);
    return cls;
}

_AVX afxClass const* AfxGetBufferClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->bufCls;
    AfxAssertClass(cls, afxFcc_BUF);
    return cls;
}

_AVX afxClass* AfxGetSamplerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->sampCls;
    AfxAssertClass(cls, afxFcc_SAMP);
    return cls;
}

_AVX afxClass* AfxGetPipelineClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->pipCls;
    AfxAssertClass(cls, afxFcc_PIP);
    return cls;
}

_AVX afxClass* AfxGetCanvasClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->canvCls;
    AfxAssertClass(cls, afxFcc_CANV);
    return cls;
}

_AVX afxClass* AfxGetShaderClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->shadCls;
    AfxAssertClass(cls, afxFcc_SHD);
    return cls;
}

_AVX afxClass* AfxGetLigatureClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->ligaCls;
    AfxAssertClass(cls, afxFcc_LIGA);
    return cls;
}

_AVX afxBool AfxGetShaderStringBase(afxDrawContext dctx, afxStringBase* base)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(base);
    afxStringBase strb = dctx->shdStrb;
    AfxTryAssertObjects(1, &strb, afxFcc_STRB);
    *base = dctx->shdStrb;
    return !!strb;
}

_AVX void AfxCountDrawContextConnections(afxDrawContext dctx, afxUnit* inputCnt, afxUnit* outputCnt)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(inputCnt || outputCnt);

    if (inputCnt)
        *inputCnt = dctx->inputs.cnt;

    if (outputCnt)
        *outputCnt = dctx->outputs.cnt;
}

_AVX void AfxQueryDrawBridges(afxDrawContext dctx, afxUnit portId, afxUnit* baseExuIdx, afxUnit* exuCnt)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(portId < AfxCountDrawPorts(AfxGetDrawContextDevice(dctx)));
    AFX_ASSERT(baseExuIdx || exuCnt);

    afxUnit dexuCnt = dctx->exuCnt;
    afxUnit first = 0, found = 0;

    for (afxUnit i = 0; i < dexuCnt; i++)
    {
        afxDrawBridge dexu = dctx->exus[i];
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        /// dexu must be a valid pointer to a afxDrawBridge handle.

        if (portId == AfxGetDrawBridgePort(dexu))
        {
            if (!found++)
                first = i;
        }
    }

    if (baseExuIdx)
        *baseExuIdx = first;

    if (exuCnt)
        *exuCnt = found;
}

_AVX afxUnit AfxGetDrawBridges(afxDrawContext dctx, afxUnit first, afxUnit cnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// bridges must be a valid pointer to afxDrawBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit dexuCnt = dctx->exuCnt;
    AFX_ASSERT_RANGE(dexuCnt, first, cnt);

    if (first < dexuCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxDrawBridge dexu = dctx->exus[first + i];
            AfxAssertObjects(1, &dexu, afxFcc_DEXU);
            bridges[rslt++] = dexu;
        }
    }
    return rslt;
}

_AVX afxUnit AfxGetDrawQueues(afxDrawContext dctx, afxUnit first, afxUnit cnt, afxDrawQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// queues must be a valid pointer to the afxDrawQueue handles.
    AFX_ASSERT(queues);
    afxUnit rslt = 0;

    afxUnit dqueCnt = dctx->queCnt;
    AFX_ASSERT_RANGE(dqueCnt, first, cnt);

    if (first < dctx->queCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxDrawQueue dque = dctx->queues[first + i];
            AfxAssertObjects(1, &dque, afxFcc_DQUE);
            queues[rslt++] = dque;
        }
    }
    return rslt;
}

_AVX afxError AfxWaitForDrawQueue(afxDrawContext dctx, afxUnit queIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// queIdx must be a valid index to a queue.
    AFX_ASSERT_RANGE(dctx->queCnt, queIdx, 1);
    afxDrawQueue dque;
    
    if (!AfxGetDrawQueues(dctx, queIdx, 1, &dque)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        if (_AvxWaitForEmptyDrawQueue(dque, timeout))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxWaitForDrawBridge(afxDrawContext dctx, afxUnit exuIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)dctx->exuCnt, exuIdx, 1);
    afxDrawBridge dexu;
    
    if (!AfxGetDrawBridges(dctx, exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        _AvxWaitForIdleDrawBridge(dexu, timeout);
    }
    return err;
}

_AVX afxError AfxWaitForDrawContext(afxDrawContext dctx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (!dctx->waitCb)
    {
        afxUnit exuCnt = dctx->exuCnt;

        for (afxUnit i = 0; i < exuCnt; i++)
            AfxWaitForDrawBridge(dctx, i, timeout);
    }
    else if (dctx->waitCb(dctx, timeout))
        AfxThrowError();

    return err;
}

_AVX afxUnit AfxGetConnectedDrawInputs(afxDrawContext dctx, afxUnit baseCon, afxUnit cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(inputs);
    AFX_ASSERT(cnt);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxDrawInput din;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din, &dctx->inputs, dctx)
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);

        if (total >= baseCon)
        {
            inputs[rslt] = din;
            ++rslt;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxUnit AfxGetConnectedDrawOutputs(afxDrawContext dctx, afxUnit baseCon, afxUnit cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(outputs);
    AFX_ASSERT(cnt);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxDrawOutput dout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawOutput), dout, &dctx->outputs, dctx)
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        if (total >= baseCon)
        {
            outputs[rslt] = dout;
            ++rslt;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxUnit AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxUnit baseCon, afxUnit cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxDrawInput din;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din, &dctx->inputs, dctx)
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(din, udd))
                break;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxUnit AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxUnit baseCon, afxUnit cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxDrawOutput dout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxDrawOutput), dout, &dctx->outputs, dctx)
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(dout, udd))
                break;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxBool AfxGetConnectedDrawInput(afxDrawContext dctx, afxUnit conNo, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxBool rslt = FALSE;
    afxDrawInput din = NIL;
    while (AfxGetConnectedDrawInputs(dctx, conNo, 1, &din))
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);
        AFX_ASSERT(input);
        *input = din;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_AVX afxBool AfxGetConnectedDrawOutput(afxDrawContext dctx, afxUnit conNo, afxDrawOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxBool rslt = FALSE;
    afxDrawOutput dout = NIL;
    while (AfxGetConnectedDrawOutputs(dctx, conNo, 1, &dout))
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AFX_ASSERT(output);
        *output = dout;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_AVX afxError AfxDisconnectDrawContext(afxDrawContext dctx, afxBool keepInputs, afxBool keepOutputs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT((!keepInputs) || (!keepOutputs));

    if (!keepInputs)
    {
        afxDrawInput din;
        while (AfxGetConnectedDrawInput(dctx, 0, &din))
        {
            AfxAssertObjects(1, &din, afxFcc_DIN);
            AfxDisconnectDrawInput(din);
            break;
        }
    }

    if (!keepOutputs)
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

_AVX afxError AfxSubmitDrawCommands(afxDrawContext dctx, avxSubmission* ctrl, afxUnit cnt, avxCmdb cmdbs[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(cmdbs);
    AFX_ASSERT(cnt);

    AFX_ASSERT(ctrl);
    AFX_ASSERT_RANGE(dctx->exuCnt, ctrl->exuIdx, ctrl->exuCnt);

    for (afxUnit i = 0; i < AfxMax(1, ctrl->exuCnt); i++)
    {
        afxDrawBridge dexu;

        if (!AfxGetDrawBridges(dctx, i, 1, &dexu)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &dexu, afxFcc_DEXU);

            /*
                If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
                Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
                If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
            */

            if (_AvxSubmitDrawCommands(dexu, ctrl, cnt, cmdbs))
                AfxThrowError();

            //ctrl->exuCnt = err ? 0 : 1;
            //ctrl->queCnt = err ? 0 : 1;
        }
    }
    return err;
}

_AVX afxError AfxPresentDrawOutput(afxDrawContext dctx, avxPresentation* ctrl, afxUnit cnt, afxDrawOutput outputs[], afxUnit const bufIdx[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(ctrl);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    AFX_ASSERT(ctrl);
    AFX_ASSERT_RANGE(dctx->exuCnt, ctrl->exuIdx, ctrl->exuCnt);

    for (afxUnit i = 0; i < AfxMax(1, ctrl->exuCnt); i++)
    {
        afxDrawBridge dexu;

        if (!AfxGetDrawBridges(dctx, i, 1, &dexu)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &dexu, afxFcc_DEXU);

            for (afxUnit j = 0; j < cnt; j++)
            {
                afxDrawOutput dout = outputs[j];
                AfxAssertObjects(1, &dout, afxFcc_DOUT);
                afxDrawContext con;

                if (!AfxGetDrawOutputContext(dout, &con)) AfxThrowError();
                else
                {
                    avxPresentation ctrl2 = *ctrl;

                    afxUnit baseQueIdx, totalQueCnt;
                    AfxQueryDrawQueues(dexu, &baseQueIdx, &totalQueCnt);
                    afxUnit queCnt = AfxMin(totalQueCnt, AfxMax(1, ctrl->queCnt));

                    afxUnit queIdx = baseQueIdx + j;
                    AfxAssertObjects(1, &dctx->queues[queIdx], afxFcc_DQUE);

                    if (_AvxPresentDrawOutput(dout, bufIdx[j], queCnt, &dctx->queues[queIdx]))
                        AfxThrowError();
                }
            }

            //ctrl->exuCnt = err ? 0 : 1;
            //ctrl->queCnt = err ? 0 : 0;
            //ctrl->queIdx = 0;
        }
    }
    return err;
}

_AVX afxError _AvxDctxStdDtorCb(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxDisconnectDrawContext(dctx, FALSE, FALSE);
    AfxWaitForDrawContext(dctx, AFX_TIME_INFINITE);
    AfxWaitForDrawContext(dctx, AFX_TIME_INFINITE);

    afxUnit exuCnt = dctx->exuCnt;
    afxUnit queCnt = dctx->queCnt;

    AfxDeregisterChainedClasses(&dctx->ctx.classes);
#if 0
    AfxAssertObjects(dctx->exuCnt, dctx->exus, afxFcc_DEXU);

    for (afxUnit j = dctx->exuCnt; j-- > 0;)
        while (!AfxReleaseObjects(1, &dctx->exus[j]));
#endif

    if (dctx->queues || dctx->exus)
    {
        if (queCnt)
        {
            AfxAssertObjects(queCnt, dctx->queues, afxFcc_DQUE);
            AfxReleaseObjects(queCnt, dctx->queues);
        }
        
        if (exuCnt)
        {
            AfxAssertObjects(exuCnt, dctx->exus, afxFcc_DEXU);
            AfxReleaseObjects(exuCnt, dctx->exus);
        }

        afxObjectStash const stashes[] =
        {
            {
                .cnt = dctx->queCnt,
                .siz = sizeof(dctx->queues[0]),
                .var = (void**)&dctx->queues
            },
            {
                .cnt = dctx->exuCnt,
                .siz = sizeof(dctx->exus[0]),
                .var = (void**)&dctx->exus
            }
        };

        if (AfxDeallocateInstanceData(dctx, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    AFX_ASSERT(AfxChainIsEmpty(&dctx->ctx.classes));

    AfxDismantleArena(&dctx->aren);

    if (dctx->shdStrb)
        AfxReleaseObjects(1, &dctx->shdStrb);

    return err;
}

_AVX afxError _AvxDctxStdCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AFX_ASSERT(spec);

    afxDrawDevice ddev = args[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawContextConfig const *cfg = ((afxDrawContextConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    afxDrawBridgeConfig const* bridgeCfgs = args[2];
    AFX_ASSERT(bridgeCfgs);
    afxUnit* baseBridgeQueIdxs = args[3];
    AFX_ASSERT(baseBridgeQueIdxs);

    dctx->running = FALSE;

    AfxDeployArena(&dctx->aren, NIL, AfxHere());

    dctx->clipSpace = ddev->clipSpace;

    AfxDeployChain(&dctx->inputs, dctx);
    AfxDeployChain(&dctx->outputs, dctx);

    afxChain *classes = &dctx->ctx.classes;
    AfxDeployChain(classes, (void*)dctx);

    if (AfxAcquireStringCatalogs(1, &dctx->shdStrb))
        AfxThrowError();

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = 1 + cfg->auxBridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = totalDqueCnt,
            .siz = sizeof(dctx->queues[0]),
            .var = (void**)&dctx->queues
        },
        {
            .cnt = bridgeCnt,
            .siz = sizeof(dctx->exus[0]),
            .var = (void**)&dctx->exus
        }
    };

    if (AfxAllocateInstanceData(dctx, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        dctx->exuCnt = bridgeCnt;
        dctx->queCnt = totalDqueCnt;
        afxUnit baseQueIdx = 0;

        afxClass* cls = &ddev->dexuCls;
        AfxAssertClass(cls, afxFcc_DEXU);

        if (AfxAcquireObjects(cls, dctx->exuCnt, (afxObject*)dctx->exus, (void const*[]) { ddev, dctx, bridgeCfgs, baseBridgeQueIdxs }))
        {
            AfxThrowError();
        }
        else
        {
            AfxAssertObjects(bridgeCnt, dctx->exus, afxFcc_DEXU);

            for (afxUnit j = 0; j < bridgeCnt; j++)
            {
                afxClass* cls2 = (afxClass*)_AvxGetDrawQueueClass(ddev);
                AfxAssertClass(cls2, afxFcc_DQUE);

                if (AfxAcquireObjects(cls2, bridgeCfgs[j].minQueCnt, (afxObject*)&dctx->queues[baseQueIdx], (void const*[]) { dctx, dctx->exus[j], &bridgeCfgs[j] })) AfxThrowError();
                else
                {
                    AfxAssertObjects(bridgeCfgs[j].minQueCnt, &dctx->queues[baseQueIdx], afxFcc_DQUE);

                }
            }
        }

        if (!err)
        {
            if (AfxDoDeviceService(&ddev->dev)) AfxThrowError(); // let the device build its DPUs.
            else
            {
                afxClassConfig tmpClsConf;

                //tmpClsConf = ;
                //AfxRegisterClass(&dctx->ctx.semaphores, NIL, classes, &tmpClsConf);

                //tmpClsConf = ;
                //AfxRegisterClass(&dctx->ctx.fences, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxBufStdImplementation;
                AfxRegisterClass(&dctx->bufCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxRasStdImplementation;
                AfxRegisterClass(&dctx->rasCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxCanvStdImplementation;
                AfxRegisterClass(&dctx->canvCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxSampStdImplementation;
                AfxRegisterClass(&dctx->sampCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxLigaStdImplementation;
                AfxRegisterClass(&dctx->ligaCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxVinStdImplementation;
                AfxRegisterClass(&dctx->vinCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxShdStdImplementation;
                AfxRegisterClass(&dctx->shadCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxPipStdImplementation;
                AfxRegisterClass(&dctx->pipCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxQrypStdImplementation;
                AfxRegisterClass(&dctx->qrypCls, NIL, classes, &tmpClsConf);

                dctx->running = TRUE;
            }
        }

        if (err)
        {
            AfxDeregisterChainedClasses(classes);
            AFX_ASSERT(AfxChainIsEmpty(classes));
            AfxDeallocate(dctx->exus);
        }

        if (err)
            AfxDeallocateInstanceData(dctx, ARRAY_SIZE(stashes), stashes);
    }

    if (err && dctx->shdStrb)
        AfxReleaseObjects(1, &dctx->shdStrb);

    return err;
}

_AVX afxClassConfig const _AvxDctxStdImplementation =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Draw Device Management Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawContext)),
    .ctor = (void*)_AvxDctxStdCtorCb,
    .dtor = (void*)_AvxDctxStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxConfigureDrawDevice(afxUnit ddevId, afxDrawContextConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ddevId != AFX_INVALID_INDEX);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        AFX_ASSERT(cfg);
        *cfg = (afxDrawContextConfig const) { 0 };
        //cfg->mainBridge.flags = NIL;
        cfg->mainBridge.portId = 0;
        cfg->mainBridge.minQueCnt = 3;
    }
    return err;
}

_AVX afxError AfxOpenDrawDevice(afxUnit ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxCallDevice(&ddev->dev, 1, NIL)) AfxThrowError(); // let the device build its DPUs.
        else
        {
            AFX_ASSERT(context);

            if (!cfg)
                AfxThrowError();
        }
    }

    if (!err)
    {
        // Acquire bridges and queues
        afxUnit bridgeCnt = 0;
        afxUnit totalDqueCnt = 0;
        afxDrawBridgeConfig bridgeCfg[AFX_MAX_DRAW_BRIDGE_PER_CONTEXT] = { 0 };
        afxUnit baseQueIdx[AFX_MAX_DRAW_BRIDGE_PER_CONTEXT] = { 0 };

        if (cfg)
        {
            bridgeCfg[bridgeCnt] = cfg->mainBridge;
            bridgeCfg[bridgeCnt].minQueCnt = bridgeCfg[bridgeCnt].minQueCnt ? bridgeCfg[bridgeCnt].minQueCnt : AfxMax(1, _AvxDqueStdImplementation.unitsPerPage);
            ++bridgeCnt;

            if (cfg->auxBridgeCnt && cfg->auxBridges)
            {
                AFX_ASSERT(15 >= cfg->auxBridgeCnt);

                for (afxUnit i = 0; i < cfg->auxBridgeCnt; i++)
                {
                    afxUnit bridgeIdx = AFX_INVALID_INDEX;

                    for (afxUnit j = 0; j < bridgeCnt; j++)
                    {
                        if (cfg->auxBridges[j].portId == bridgeCfg[j].portId)
                        {
                            bridgeCfg[bridgeIdx].minQueCnt += cfg->auxBridges[i].minQueCnt;
                            bridgeCfg[bridgeIdx].queuePriority = NIL;
                            //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                            baseQueIdx[bridgeIdx] = totalDqueCnt;
                            totalDqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                            break;
                        }
                    }

                    if (bridgeIdx == AFX_INVALID_INDEX)
                    {
                        bridgeCfg[bridgeCnt] = cfg->auxBridges[i];
                        ++bridgeCnt;
                    }
                }
            }
        }

        if (!bridgeCnt)
        {
            bridgeCnt = AfxCountDrawPorts(ddev);
            AFX_ASSERT(bridgeCnt);

            for (afxUnit i = 0; i < bridgeCnt; i++)
            {
                bridgeCfg[i].minQueCnt = AfxMax(1, _AvxDqueStdImplementation.unitsPerPage);
                bridgeCfg[i].queuePriority = NIL;
                //bridgeCfg[i].flags = NIL;
                baseQueIdx[i] = totalDqueCnt;
                totalDqueCnt += bridgeCfg[i].minQueCnt;
            }
        }

        afxClass* cls = (afxClass*)AfxGetDrawContextClass(ddev);
        AfxAssertClass(cls, afxFcc_DCTX);

        afxDrawContextConfig cfg2 = *cfg;
        cfg2.mainBridge = bridgeCfg[0];
        cfg2.auxBridgeCnt = bridgeCnt - 1;
        cfg2.auxBridges = &bridgeCfg[1];

        if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { ddev, &cfg2, &bridgeCfg[0], &baseQueIdx[0] })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, context, afxFcc_DCTX);
        }
    }
    return err;
}
