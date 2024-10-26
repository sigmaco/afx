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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ASX_SOUND_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_CONTEXT_C
#define _ASX_SOUND_BRIDGE_C
#define _ASX_SOUND_QUEUE_C
#define _ASX_SOUND_INPUT_C
#define _ASX_SOUND_OUTPUT_C
#define _ASX_SOUND_SYSTEM_C
#include "../../dev/AsxImplKit.h"

_ASX afxSoundDevice AfxGetSoundContextDevice(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundDevice sdev = AfxGetProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxUnit AfxCountSoundBridges(afxSoundContext sctx, afxUnit baseBridIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT_RANGE(sctx->exuCnt, baseBridIdx, 1);
    return sctx->exuCnt - baseBridIdx;
}

_ASX afxClass const* AfxGetTrackerClass(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxClass const* cls = &sctx->traxCls;
    AfxAssertClass(cls, afxFcc_TRAX);
    return cls;
}

_ASX afxClass const* AfxGetWaveformClass(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxClass const* cls = &sctx->wavCls;
    AfxAssertClass(cls, afxFcc_WAV);
    return cls;
}

_ASX afxBool AfxGetSoundBridge(afxSoundContext sctx, afxUnit portIdx, afxSoundBridge* bridge)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be one of the bridge indices specified when device was created.
    AFX_ASSERT_RANGE(sctx->exuCnt, portIdx, 1);
    afxBool rslt;

    if (!(rslt = (portIdx < sctx->exuCnt))) AfxThrowError();
    else
    {
        afxSoundBridge sdge = sctx->exus[portIdx];
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        /// bridge must be a valid pointer to a afxSoundBridge handle.
        *bridge = sdge;
    }
    return rslt;
}

_ASX afxBool AfxGetSoundQueue(afxSoundContext sctx, afxUnit portIdx, afxUnit queIdx, afxSoundQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be one of the bridge indices specified when device was created.
    AFX_ASSERT_RANGE(sctx->exuCnt, portIdx, 1);
    afxSoundBridge sdge;
    afxBool rslt;

    if (!(rslt = AfxGetSoundBridge(sctx, portIdx, &sdge))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        /// queIdx must be less than the value of queCnt for the bridge indicated by portIdx when device context was created.
        AFX_ASSERT_RANGE(sdge->queCnt, queIdx, 1);

        if (!(rslt = (queIdx < sdge->queCnt))) AfxThrowError();
        else
        {
            afxSoundQueue sque = sdge->queues[queIdx];
            AfxAssertObjects(1, &sque, afxFcc_SQUE);
            /// queue must be a valid pointer to a afxSoundQueue handle.        
            *queue = sque;
        }
    }
    return rslt;
}

_ASX afxUnit AfxCountSoundQueues(afxSoundContext sctx, afxUnit portIdx, afxUnit baseQueIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)sctx->exuCnt, portIdx, 1);
    afxUnit queCnt = 0;
    afxSoundBridge sdge;

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        AFX_ASSERT_RANGE(sdge->queCnt, baseQueIdx, 1);
        queCnt = sdge->queCnt - baseQueIdx;
    }
    return queCnt;
}

_ASX afxError AfxWaitForIdleSoundQueue(afxSoundContext sctx, afxUnit portIdx, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)sctx->exuCnt, portIdx, 1);
    afxSoundBridge sdge;

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        /// queIdx must be a valid index to a queue.
        AFX_ASSERT_RANGE(sdge->queCnt, queIdx, 1);

        if (sdge->queCnt > queIdx)
        {
            if (!sdge->waitCb)
            {
                afxSoundQueue sque = sdge->queues[queIdx];
                AfxLockMutex(&sque->idleCndMtx);

                while (sque->workChn.cnt)
                    AfxWaitCondition(&sque->idleCnd, &sque->idleCndMtx);

                AfxUnlockMutex(&sque->idleCndMtx);
            }
            else if (sdge->waitCb(sdge, queIdx))
                AfxThrowError();
        }
    }
    return err;
}

_ASX afxError AfxWaitForIdleSoundBridge(afxSoundContext sctx, afxUnit portIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)sctx->exuCnt, portIdx, 1);
    afxSoundBridge sdge;

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        afxUnit squeCnt = sdge->queCnt;

        for (afxUnit squeIdx = 0; squeIdx < squeCnt; squeIdx++)
            AfxWaitForIdleSoundQueue(sctx, portIdx, squeIdx);
    }
    return err;
}

_ASX afxError AfxWaitForSoundContext(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    if (!sctx->waitCb)
    {
        afxUnit sdgeCnt = sctx->exuCnt;

        for (afxUnit portIdx = 0; portIdx < sdgeCnt; portIdx++)
            AfxWaitForIdleSoundBridge(sctx, portIdx);
    }
    else if (sctx->waitCb(sctx))
        AfxThrowError();

    return err;
}

_ASX afxUnit AfxCountSoundInputConnections(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx->inputs.cnt;
}

_ASX void AfxCountSoundContextConnections(afxSoundContext sctx, afxUnit* inputCnt, afxUnit* outputCnt)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(inputCnt || outputCnt);

    if (inputCnt)
        *inputCnt = sctx->inputs.cnt;

    if (outputCnt)
        *outputCnt = sctx->outputs.cnt;
}

_ASX afxUnit AfxGetConnectedSoundInputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxSoundInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(inputs);
    AFX_ASSERT(cnt);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxSoundInput sin;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundInput), sin, &sctx->inputs, sctx)
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);

        if (total >= baseCon)
        {
            inputs[rslt] = sin;
            ++rslt;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxUnit AfxGetConnectedSoundOutputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxSoundOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(outputs);
    AFX_ASSERT(cnt);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxSoundOutput sout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundOutput), sout, &sctx->inputs, sctx)
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);

        if (total >= baseCon)
        {
            outputs[rslt] = sout;
            ++rslt;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxUnit AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxBool(*f)(afxSoundInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxSoundInput sin;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundInput), sin, &sctx->inputs, sctx)
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(sin, udd))
                break;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxUnit AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxUnit baseCon, afxUnit cnt, afxBool(*f)(afxSoundOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);

    afxUnit rslt = 0;
    afxUnit total = 0;
    afxSoundOutput sout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundOutput), sout, &sctx->outputs, sctx)
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(sout, udd))
                break;

            if (rslt == cnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxBool AfxGetConnectedSoundInput(afxSoundContext sctx, afxUnit conNo, afxSoundInput* input)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxBool rslt = FALSE;
    afxSoundInput sin = NIL;
    while (AfxGetConnectedSoundInputs(sctx, conNo, 1, &sin))
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);
        AFX_ASSERT(input);
        *input = sin;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_ASX afxBool AfxGetConnectedSoundOutput(afxSoundContext sctx, afxUnit conNo, afxSoundOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxBool rslt = FALSE;
    afxSoundOutput sout = NIL;
    while (AfxGetConnectedSoundOutputs(sctx, conNo, 1, &sout))
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);
        AFX_ASSERT(output);
        *output = sout;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_ASX afxError AfxDisconnectSoundContext(afxSoundContext sctx, afxBool keepInputs, afxBool keepOutputs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT((!keepInputs) || (!keepOutputs));

    if (!keepInputs)
    {
        afxSoundInput sin;
        while (AfxGetConnectedSoundInput(sctx, 0, &sin))
        {
            AfxAssertObjects(1, &sin, afxFcc_SIN);
            AfxDisconnectSoundInput(sin);
            break;
        }
    }

    if (!keepOutputs)
    {
        afxSoundOutput sout;
        while (AfxGetConnectedSoundOutput(sctx, 0, &sout))
        {
            AfxAssertObjects(1, &sout, afxFcc_SOUT);
            AfxDisconnectSoundOutput(sout);
            break;
        }
    }
    return err;
}

_ASX afxError _AsxSctxStdDtorCb(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundDevice sdev = AfxGetSoundContextDevice(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxDisconnectSoundContext(sctx, FALSE, FALSE);
    AfxWaitForSoundContext(sctx);
    AfxWaitForSoundContext(sctx);

    AFX_ASSERT(sdev->closeCb);

    afxUnit exuCnt = sctx->exuCnt;

    if (sdev->closeCb(sdev, sctx))
        AfxThrowError();

    AfxDeregisterChainedClasses(&sctx->ctx.classes);

    if (sctx->exus)
    {
        AfxAssertObjects(exuCnt, sctx->exus, afxFcc_SDGE);
        AfxReleaseObjects(exuCnt, sctx->exus);
        AfxDeallocate(sctx->exus);
        sctx->exus = NIL;
    }

    AFX_ASSERT(AfxChainIsEmpty(&sctx->ctx.classes));

    AfxDismantleArena(&sctx->aren);

    return err;
}

_ASX afxError _AsxSctxStdCtorCb(afxSoundContext sctx, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AFX_ASSERT(spec);

    afxSoundDevice sdev = args[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundContextConfig const *cfg = ((afxSoundContextConfig const *)args[1]) + invokeNo;

    sctx->running = FALSE;

    AfxDeployArena(&sctx->aren, NIL, AfxHere());

    AfxDeployChain(&sctx->inputs, sctx);
    AfxDeployChain(&sctx->outputs, sctx);

    afxChain *classes = &sctx->ctx.classes;
    AfxDeployChain(classes, (void*)sctx);

    // Acquire bridges and queues

    // Acquire bridges and queues
    afxUnit bridgeCnt = 0;
    afxUnit totalQueCnt = 0;
    afxSoundBridgeConfig bridgeCfg[16] = { 0 };

    if (cfg)
    {
        bridgeCfg[bridgeCnt] = cfg->mainBridge;
        ++bridgeCnt;

        if (cfg->auxBridgeCnt && cfg->auxBridges)
        {
            AFX_ASSERT(15 >= cfg->auxBridgeCnt);

            for (afxUnit i = 0; i < cfg->auxBridgeCnt; i++)
            {
                afxUnit bridgeIdx = AFX_INVALID_INDEX;

                for (afxUnit j = 0; j < bridgeCnt; j++)
                {
                    if (cfg->auxBridges[j].portIdx == bridgeCfg[j].portIdx)
                    {
                        bridgeCfg[bridgeIdx].queueCnt += cfg->auxBridges[i].queueCnt;
                        bridgeCfg[bridgeIdx].queuePriority = NIL;
                        bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                        totalQueCnt += bridgeCfg[bridgeIdx].queueCnt;
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
        bridgeCnt = AfxCountSoundPorts(sdev);
        AFX_ASSERT(bridgeCnt);

        for (afxUnit i = 0; i < bridgeCnt; i++)
        {
            bridgeCfg[i].queueCnt = _AsxSqueStdImplementation.unitsPerPage;
            bridgeCfg[i].queuePriority = NIL;
            bridgeCfg[i].flags = NIL;

            totalQueCnt += bridgeCfg[i].queueCnt;
        }
    }

    afxSoundBridge* exus;
    sctx->exus = NIL;

    if (!(exus = AfxAllocate(bridgeCnt, sizeof(exus[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            afxClass* cls = &sdev->sdgeCls;
            AfxAssertClass(cls, afxFcc_SDGE);

            if (AfxAcquireObjects(cls, 1, (afxObject*)&exus[j], (void const*[]) { sdev, sctx, &bridgeCfg[j] }))
            {
                AfxThrowError();
                AfxReleaseObjects(j, exus);
            }
            else
            {
                AfxAssertObjects(1, &exus[j], afxFcc_SDGE);
            }
        }

        if (!err)
        {
            AfxAssertObjects(bridgeCnt, exus, afxFcc_SDGE);
            sctx->exuCnt = bridgeCnt;
            sctx->exus = exus;

            if (AfxDoDeviceService(&sdev->dev)) AfxThrowError(); // let the device build its SPUs.
            else
            {
                AFX_ASSERT(sdev->openCb);
                AFX_ASSERT(sdev->closeCb);

                if (!err)
                    if (sdev->openCb(sdev, sctx, args, invokeNo))
                        AfxThrowError();
            }
        }

        if (err)
        {
            AfxDeregisterChainedClasses(classes);
            AFX_ASSERT(AfxChainIsEmpty(classes));
            AfxDeallocate(sctx->exus);
        }
    }
    return err;
}

_ASX afxClassConfig const _AsxSctxStdImplementation =
{
    .fcc = afxFcc_SCTX,
    .name = "SoundContext",
    .desc = "Sound Device Management Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundContext)),
    .ctor = (void*)_AsxSctxStdCtorCb,
    .dtor = (void*)_AsxSctxStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxConfigureSoundDevice(afxUnit sdevId, afxSoundContextConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(sdevId != AFX_INVALID_INDEX);
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        AFX_ASSERT(cfg);
        *cfg = (afxSoundContextConfig const) { 0 };
        cfg->mainBridge.flags = NIL;
        cfg->mainBridge.portIdx = 0;
        cfg->mainBridge.queueCnt = 3;
    }
    return err;
}

_ASX afxError AfxOpenSoundDevice(afxUnit sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = (afxClass*)AfxGetSoundContextClass(sdev);
        AfxAssertClass(cls, afxFcc_SCTX);
        AFX_ASSERT(context);

        if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { sdev, (void*)cfg }))
            AfxThrowError();

        AfxAssertObjects(1, context, afxFcc_SCTX);
    }
    return err;
}

_ASX afxUnit AfxEnumerateSoundContexts(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(contexts);
    AFX_ASSERT(cnt);
    afxClass const* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)contexts);
}

_ASX afxUnit AfxEvokeSoundContexts(afxSoundDevice sdev, afxBool(*flt)(afxSoundContext, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(contexts);
    AFX_ASSERT(flt);
    AFX_ASSERT(cnt);
    afxClass const* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)contexts);
}

_ASX afxUnit AfxInvokeSoundContexts(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}
