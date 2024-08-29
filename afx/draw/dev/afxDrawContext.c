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
#include "../../dev/AvxImplKit.h"

_AVX afxDrawDevice AfxGetDrawContextDevice(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetParent(dctx);
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

_AVX afxClass* AfxGetBufferClass(afxDrawContext dctx)
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

_AVX afxClass* AfxGetRasterizerClass(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass *cls = &dctx->razrCls;
    AfxAssertClass(cls, afxFcc_RAZR);
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
    AfxAssertClass(cls, afxFcc_BSCH);
    return cls;
}

_AVX afxBool AfxGetShaderStringBase(afxDrawContext dctx, afxStringBase* base)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(base);
    afxStringBase strb = dctx->shdStrb;
    AfxTryAssertObjects(1, &strb, afxFcc_STRB);
    *base = dctx->shdStrb;
    return !!strb;
}

_AVX void AfxCountDrawContextConnections(afxDrawContext dctx, afxNat* inputCnt, afxNat* outputCnt)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(inputCnt || outputCnt);

    if (inputCnt)
        *inputCnt = dctx->inputs.cnt;

    if (outputCnt)
        *outputCnt = dctx->outputs.cnt;
}

_AVX afxNat AfxCountDrawBridges(afxDrawContext dctx, afxNat portId, afxNat baseExu)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat dexuCnt = dctx->exuCnt;
    afxNat rslt = 0, found = 0;

    for (afxNat i = 0; i < dexuCnt; i++)
    {
        afxDrawBridge dexu = dctx->exus[i];
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        /// dexu must be a valid pointer to a afxDrawBridge handle.

        if (portId == AfxGetDrawBridgePort(dexu))
        {
            ++found;

            if (found > baseExu)
                rslt++;
        }
    }
    return rslt;
}

_AVX afxNat AfxQueryDrawBridges(afxDrawContext dctx, afxNat portId, afxNat first, afxNat cnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat dexuCnt = dctx->exuCnt;
    afxNat rslt = 0, found = 0;

    for (afxNat i = 0; i < dexuCnt; i++)
    {
        if (rslt == cnt)
            break;

        afxDrawBridge dexu = dctx->exus[i];
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        /// dexu must be a valid pointer to a afxDrawBridge handle.

        if (portId == AfxGetDrawBridgePort(dexu))
        {
            ++found;

            if (found > first)
                bridges[rslt++] = dexu;
        }
    }
    return rslt;
}

_AVX afxNat AfxQueryDrawQueues(afxDrawContext dctx, afxNat portId, afxNat first, afxNat cnt, afxDrawQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat dexuCnt = dctx->exuCnt;
    afxNat rslt = 0, found = 0;

    for (afxNat i = 0; i < dexuCnt; i++)
    {
        if (rslt == cnt)
            break;

        afxDrawBridge dexu = dctx->exus[i];
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        /// dexu must be a valid pointer to a afxDrawBridge handle.

        if (portId == AfxGetDrawBridgePort(dexu))
        {
            afxNat queCnt = AfxCountDrawQueues(dexu, 0);

            for (afxNat j = 0; j < queCnt; j++)
            {
                afxDrawQueue dque;
                AfxGetDrawQueues(dexu, j, 1, &dque);
                AfxAssertObjects(1, &dque, afxFcc_DQUE);

                ++found;

                if (found > first)
                    queues[rslt++] = dque;

                if (rslt == cnt)
                    break;
            }
        }

        if (rslt == cnt)
            break;
    }
    return rslt;
}

_AVX afxNat AfxGetDrawBridges(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawBridge bridges[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat dexuCnt = dctx->exuCnt;
    AfxAssertRange(dexuCnt, first, cnt);
    afxNat rslt = 0;

    if (first < dexuCnt)
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            afxDrawBridge dexu = dctx->exus[first + i];
            AfxAssertObjects(1, &dexu, afxFcc_DEXU);
            /// dexu must be a valid pointer to a afxDrawBridge handle.
            bridges[rslt++] = dexu;
        }
    }
    return rslt;
}

_AVX afxBool AfxGetDrawQueue(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx, afxDrawQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// exuIdx must be one of the bridge indices specified when device was created.
    AfxAssertRange(dctx->exuCnt, exuIdx, 1);
    afxBool rslt = FALSE;

    if (exuIdx < dctx->exuCnt)
    {
        afxDrawBridge dexu = dctx->exus[exuIdx];
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        afxDrawQueue dque;
        
        if ((rslt = !!AfxGetDrawQueues(dexu, queIdx, 1, &dque)))
        {
            /// queue must be a valid pointer to a afxDrawQueue handle.
            AfxAssertObjects(1, &dque, afxFcc_DQUE);
            AfxAssert(queue);
            *queue = dque;
        }
    }
    return rslt;
}

_AVX afxError AfxWaitForDrawQueue(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange(dctx->exuCnt, exuIdx, 1);
    afxDrawBridge dexu;
    
    if (!AfxGetDrawBridges(dctx, exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        if (_AvxWaitForIdleDrawQueue(dexu, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxWaitForDrawBridge(afxDrawContext dctx, afxNat exuIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange((afxNat)dctx->exuCnt, exuIdx, 1);
    afxDrawBridge dexu;
    
    if (!AfxGetDrawBridges(dctx, exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        _AvxWaitForIdleDrawBridge(dexu);
    }
    return err;
}

_AVX afxError AfxWaitForDrawContext(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (!dctx->waitCb)
    {
        afxNat ddgeCnt = dctx->exuCnt;

        for (afxNat i = 0; i < ddgeCnt; i++)
            AfxWaitForDrawBridge(dctx, i);
    }
    else if (dctx->waitCb(dctx))
        AfxThrowError();

    return err;
}

_AVX void AfxBeginDrawQueueDebugScope(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange(dctx->exuCnt, exuIdx, 1);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        _AvxBeginDrawQueueDebugScope(dexu, queIdx, name, color);
    }
}

_AVX void AfxEndDrawQueueDebugScope(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange(dctx->exuCnt, exuIdx, 1);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        _AvxEndDrawQueueDebugScope(dexu, queIdx);
    }
}

_AVX void AfxPushDrawQueueDebugLabel(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange(dctx->exuCnt, exuIdx, 1);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        _AvxPushDrawQueueDebugLabel(dexu, queIdx, name, color);
    }
}

_AVX afxNat AfxGetConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(inputs);
    AfxAssert(cnt);

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

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxNat AfxGetConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(outputs);
    AfxAssert(cnt);

    afxNat rslt = 0;
    afxNat total = 0;
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

_AVX afxNat AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
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

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_AVX afxNat AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
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

            if (rslt == cnt)
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
    while (AfxGetConnectedDrawInputs(dctx, conNo, 1, &din))
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
    while (AfxGetConnectedDrawOutputs(dctx, conNo, 1, &dout))
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        AfxAssert(output);
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
    AfxAssert((!keepInputs) || (!keepOutputs));

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

_AVX afxError AfxSubmitDrawCommands(afxDrawContext dctx, avxSubmission* ctrl, afxNat cnt, avxCmdb cmdbs[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(dctx->exuCnt, ctrl->exuIdx, ctrl->exuCnt);
    afxDrawBridge dexu;

    if (!AfxQueryDrawBridges(dctx, ctrl->portId, ctrl->exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        AfxAssert(cnt);
        AfxAssert(cmdbs);

        /*
            If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
            Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
            If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
        */

        if (AFX_INVALID_INDEX == (ctrl->queIdx = _AvxSubmitDrawCommands(dexu, ctrl, cnt, cmdbs)))
            AfxThrowError();

        ctrl->exuCnt = err ? 0 : 1;
        ctrl->queCnt = err ? 0 : 1;
    }
    return err;
}

_AVX afxError AfxPresentDrawOutputBuffers(afxDrawContext dctx, avxPresentation* ctrl, afxNat cnt, afxDrawOutput outputs[], afxNat const bufIdx[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(ctrl);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    afxDrawBridge dexu;

    if (!AfxQueryDrawBridges(dctx, ctrl->portId, ctrl->exuIdx, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxDrawOutput dout = outputs[i];
            AfxAssertObjects(1, &dout, afxFcc_DOUT);
            afxDrawContext con;

            if (!AfxGetDrawOutputContext(dout, &con)) AfxThrowError();
            else
            {
                avxPresentation ctrl2 = *ctrl;
                
                if (_AvxPresentDrawOutput(dout, bufIdx[i], NIL))
                    AfxThrowError();
            }
        }

        ctrl->exuCnt = err ? 0 : 1;
        ctrl->queCnt = err ? 0 : 0;
        ctrl->queIdx = 0;
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
    AfxWaitForDrawContext(dctx);
    AfxWaitForDrawContext(dctx);

    afxNat exuCnt = dctx->exuCnt;

    AfxCleanUpChainedClasses(&dctx->ctx.classes);
#if 0
    AfxAssertObjects(dctx->exuCnt, dctx->exus, afxFcc_DEXU);

    for (afxNat j = dctx->exuCnt; j-- > 0;)
        while (!AfxReleaseObjects(1, &dctx->exus[j]));
#endif

    if (dctx->exus)
    {
        AfxAssertObjects(exuCnt, dctx->exus, afxFcc_DEXU);
        AfxReleaseObjects(exuCnt, dctx->exus);
        AfxDeallocate(dctx->exus);
        dctx->exus = NIL;
    }

    AfxAssert(AfxChainIsEmpty(&dctx->ctx.classes));

    AfxDismantleArena(&dctx->aren);

    if (dctx->shdStrb)
        AfxReleaseObjects(1, &dctx->shdStrb);

    return err;
}

_AVX afxError _AvxDctxStdCtorCb(afxDrawContext dctx, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    //AfxAssert(spec);

    afxDrawDevice ddev = args[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawContextConfig const *cfg = ((afxDrawContextConfig const *)args[1]) + invokeNo;

    dctx->running = FALSE;

    AfxDeployArena(&dctx->aren, NIL, AfxHere());

    dctx->clipSpace = ddev->clipSpace;

    AfxDeployChain(&dctx->inputs, dctx);
    AfxDeployChain(&dctx->outputs, dctx);

    afxChain *classes = &dctx->ctx.classes;
    AfxDeployChain(classes, (void*)dctx);

    if (AfxAcquireStringCatalogs(1, &dctx->shdStrb))
        AfxThrowError();

    // Acquire bridges and queues
    afxNat bridgeCnt = 0;
    afxNat totalDqueCnt = 0;
    afxDrawBridgeConfig bridgeCfg[4] = { 0 };
    
    if (cfg)
    {
        bridgeCfg[bridgeCnt] = cfg->mainBridge;
        ++bridgeCnt;

        if (cfg->auxBridgeCnt && cfg->auxBridges)
        {
            AfxAssert(15 >= cfg->auxBridgeCnt);

            for (afxNat i = 0; i < cfg->auxBridgeCnt; i++)
            {
                afxNat bridgeIdx = AFX_INVALID_INDEX;

                for (afxNat j = 0; j < bridgeCnt; j++)
                {
                    if (cfg->auxBridges[j].portIdx == bridgeCfg[j].portIdx)
                    {
                        bridgeCfg[bridgeIdx].minQueCnt += cfg->auxBridges[i].minQueCnt;
                        bridgeCfg[bridgeIdx].queuePriority = NIL;
                        //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

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
        AfxAssert(bridgeCnt);

        for (afxNat i = 0; i < bridgeCnt; i++)
        {
            bridgeCfg[i].minQueCnt = _AvxDqueStdImplementation.unitsPerPage;
            bridgeCfg[i].queuePriority = NIL;
            //bridgeCfg[i].flags = NIL;

            totalDqueCnt += bridgeCfg[i].minQueCnt;
        }
    }

    afxDrawBridge* exus;
    dctx->exus = NIL;

    if (!(exus = AfxAllocate(bridgeCnt, sizeof(exus[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat j = 0; j < bridgeCnt; j++)
        {
            afxClass* cls = &ddev->dexuCls;
            AfxAssertClass(cls, afxFcc_DEXU);

            if (AfxAcquireObjects(cls, 1, (afxObject*)&exus[j], (void const*[]) { ddev, dctx, &bridgeCfg[j], &j }))
            {
                AfxThrowError();
                AfxReleaseObjects(j, exus);
            }
            else
            {
                AfxAssertObjects(1, &exus[j], afxFcc_DEXU);
            }
        }

        if (!err)
        {
            AfxAssertObjects(bridgeCnt, exus, afxFcc_DEXU);
            dctx->exuCnt = bridgeCnt;
            dctx->exus = exus;

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

                tmpClsConf = _AvxRazrStdImplementation;
                AfxRegisterClass(&dctx->razrCls, NIL, classes, &tmpClsConf);

                tmpClsConf = _AvxQrypStdImplementation;
                AfxRegisterClass(&dctx->qrypCls, NIL, classes, &tmpClsConf);
                    
                dctx->running = TRUE;
            }
        }

        if (err)
        {
            AfxCleanUpChainedClasses(classes);
            AfxAssert(AfxChainIsEmpty(classes));
            AfxDeallocate(dctx->exus);
        }
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

_AVX afxError AfxConfigureDrawDevice(afxNat ddevId, afxDrawContextConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ddevId != AFX_INVALID_INDEX);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        AfxAssert(cfg);
        *cfg = (afxDrawContextConfig const) { 0 };
        //cfg->mainBridge.flags = NIL;
        cfg->mainBridge.portIdx = 0;
        cfg->mainBridge.minQueCnt = 3;
    }
    return err;
}

_AVX afxError AfxOpenDrawDevice(afxNat ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context)
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
            afxClass* cls = (afxClass*)AfxGetDrawContextClass(ddev);
            AfxAssertClass(cls, afxFcc_DCTX);
            AfxAssert(context);

            if (!cfg) AfxThrowError();
            else
            {
                if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { ddev, cfg })) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, context, afxFcc_DCTX);
                }
            }
        }
    }
    return err;
}
