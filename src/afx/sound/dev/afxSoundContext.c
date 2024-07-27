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
#include "AsxDevKit.h"

_ASX afxSoundDevice AfxGetSoundContextDevice(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundDevice sdev = AfxGetObjectProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxNat AfxCountSoundBridges(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx->ownedBridgeCnt;
}

_ASX afxBool AfxGetSoundBridge(afxSoundContext sctx, afxNat portIdx, afxSoundBridge* bridge)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be one of the bridge indices specified when device was created.
    AfxAssertRange(sctx->ownedBridgeCnt, portIdx, 1);
    afxBool rslt;

    if (!(rslt = (portIdx < sctx->ownedBridgeCnt))) AfxThrowError();
    else
    {
        afxSoundBridge sdge = sctx->ownedBridges[portIdx];
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        /// bridge must be a valid pointer to a afxSoundBridge handle.
        *bridge = sdge;
    }
    return rslt;
}

_ASX afxBool AfxGetSoundQueue(afxSoundContext sctx, afxNat portIdx, afxNat queIdx, afxSoundQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be one of the bridge indices specified when device was created.
    AfxAssertRange(sctx->ownedBridgeCnt, portIdx, 1);
    afxSoundBridge sdge;
    afxBool rslt;

    if (!(rslt = AfxGetSoundBridge(sctx, portIdx, &sdge))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        /// queIdx must be less than the value of queCnt for the bridge indicated by portIdx when device context was created.
        AfxAssertRange(sdge->queCnt, queIdx, 1);

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

_ASX afxNat AfxCountSoundQueues(afxSoundContext sctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange((afxNat)sctx->ownedBridgeCnt, portIdx, 1);
    afxNat queCnt = 0;
    afxSoundBridge sdge;

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        queCnt = sdge->queCnt;
    }
    return queCnt;
}

_ASX afxError AfxWaitForIdleSoundQueue(afxSoundContext sctx, afxNat portIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange((afxNat)sctx->ownedBridgeCnt, portIdx, 1);
    afxSoundBridge sdge;

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        /// queIdx must be a valid index to a queue.
        AfxAssertRange(sdge->queCnt, queIdx, 1);

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

_ASX afxError AfxWaitForIdleSoundBridge(afxSoundContext sctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sctx must be a valid afxSoundContext handle.
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    /// portIdx must be a valid index to a bridge.
    AfxAssertRange((afxNat)sctx->ownedBridgeCnt, portIdx, 1);
    afxSoundBridge sdge;

    if (!AfxGetSoundBridge(sctx, portIdx, &sdge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        afxNat squeCnt = sdge->queCnt;

        for (afxNat squeIdx = 0; squeIdx < squeCnt; squeIdx++)
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
        afxNat sdgeCnt = sctx->ownedBridgeCnt;

        for (afxNat portIdx = 0; portIdx < sdgeCnt; portIdx++)
            AfxWaitForIdleSoundBridge(sctx, portIdx);
    }
    else if (sctx->waitCb(sctx))
        AfxThrowError();

    return err;
}

_ASX afxNat AfxCountSoundInputConnections(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx->inputs.cnt;
}

_ASX afxNat AfxCountSoundOutputConnections(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx->outputs.cnt;
}

_ASX afxNat AfxEnumerateConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(inputs);
    AfxAssert(conCnt);

    afxNat rslt = 0;
    afxNat total = 0;
    afxSoundInput sin;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundInput), sin, &sctx->inputs, sctx)
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);

        if (total >= baseCon)
        {
            inputs[rslt] = sin;
            ++rslt;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxNat AfxEnumerateConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(outputs);
    AfxAssert(conCnt);

    afxNat rslt = 0;
    afxNat total = 0;
    afxSoundOutput sout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundOutput), sout, &sctx->inputs, sctx)
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);

        if (total >= baseCon)
        {
            outputs[rslt] = sout;
            ++rslt;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxNat AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(conCnt);
    AfxAssert(f);

    afxNat rslt = 0;
    afxNat total = 0;
    afxSoundInput sin;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundInput), sin, &sctx->inputs, sctx)
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(sin, udd))
                break;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxNat AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(conCnt);
    AfxAssert(f);

    afxNat rslt = 0;
    afxNat total = 0;
    afxSoundOutput sout;
    AfxIterateLinkageB2F(AFX_OBJECT(afxSoundOutput), sout, &sctx->outputs, sctx)
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);

        if (total >= baseCon)
        {
            ++rslt;

            if (f(sout, udd))
                break;

            if (rslt == conCnt)
                break;
        }
        ++total;
    }
    return rslt;
}

_ASX afxBool AfxGetConnectedSoundInput(afxSoundContext sctx, afxNat conNo, afxSoundInput* input)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxBool rslt = FALSE;
    afxSoundInput sin = NIL;
    while (AfxEnumerateConnectedSoundInputs(sctx, conNo, 1, &sin))
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);
        AfxAssert(input);
        *input = sin;
        rslt = TRUE;
        break;
    }
    return rslt;
}

_ASX afxBool AfxGetConnectedSoundOutput(afxSoundContext sctx, afxNat conNo, afxSoundOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxBool rslt = FALSE;
    afxSoundOutput sout = NIL;
    while (AfxEnumerateConnectedSoundOutputs(sctx, conNo, 1, &sout))
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);
        AfxAssert(output);
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
    AfxAssert((!keepInputs) || (!keepOutputs));

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

    AfxAssert(sdev->closeCb);

    afxNat ownedBridgeCnt = sctx->ownedBridgeCnt;

    if (sdev->closeCb(sdev, sctx))
        AfxThrowError();

    AfxCleanUpChainedClasses(&sctx->ctx.classes);

    if (sctx->ownedBridges)
    {
        AfxAssertObjects(ownedBridgeCnt, sctx->ownedBridges, afxFcc_SDGE);
        AfxReleaseObjects(ownedBridgeCnt, sctx->ownedBridges);
        AfxDeallocate(sctx->ownedBridges);
        sctx->ownedBridges = NIL;
    }

    AfxAssert(AfxChainIsEmpty(&sctx->ctx.classes));

    AfxDismantleArena(&sctx->aren);

    return err;
}

_ASX afxError _AsxSctxStdCtorCb(afxSoundContext sctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AfxAssert(spec);

    afxSoundDevice sdev = cookie->udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundContextConfig const *cfg = ((afxSoundContextConfig const *)cookie->udd[1]) + cookie->no;

    sctx->running = FALSE;

    AfxDeployArena(&sctx->aren, NIL, AfxHere());

    AfxDeployChain(&sctx->inputs, sctx);
    AfxDeployChain(&sctx->outputs, sctx);

    afxChain *classes = &sctx->ctx.classes;
    AfxDeployChain(classes, (void*)sctx);

    // Acquire bridges and queues

    // Acquire bridges and queues
    afxNat bridgeCnt = 0;
    afxNat totalQueCnt = 0;
    afxSoundBridgeConfig bridgeCfg[16] = { 0 };

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
        AfxAssert(bridgeCnt);

        for (afxNat i = 0; i < bridgeCnt; i++)
        {
            bridgeCfg[i].queueCnt = _AsxSqueStdImplementation.unitsPerPage;
            bridgeCfg[i].queuePriority = NIL;
            bridgeCfg[i].flags = NIL;

            totalQueCnt += bridgeCfg[i].queueCnt;
        }
    }

    afxSoundBridge* ownedBridges;
    sctx->ownedBridges = NIL;

    if (!(ownedBridges = AfxAllocate(bridgeCnt, sizeof(ownedBridges[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat j = 0; j < bridgeCnt; j++)
        {
            afxClass* cls = &sdev->sdgeCls;
            AfxAssertClass(cls, afxFcc_SDGE);

            if (AfxAcquireObjects(cls, 1, (afxObject*)&ownedBridges[j], (void const*[]) { sdev, sctx, &bridgeCfg[j] }))
            {
                AfxThrowError();
                AfxReleaseObjects(j, ownedBridges);
            }
            else
            {
                AfxAssertObjects(1, &ownedBridges[j], afxFcc_SDGE);
            }
        }

        if (!err)
        {
            AfxAssertObjects(bridgeCnt, ownedBridges, afxFcc_SDGE);
            sctx->ownedBridgeCnt = bridgeCnt;
            sctx->ownedBridges = ownedBridges;

            if (AfxDoDeviceService(&sdev->dev)) AfxThrowError(); // let the device build its SPUs.
            else
            {
                AfxAssert(sdev->openCb);
                AfxAssert(sdev->closeCb);

                if (!err)
                    if (sdev->openCb(sdev, sctx, cookie))
                        AfxThrowError();
            }
        }

        if (err)
        {
            AfxCleanUpChainedClasses(classes);
            AfxAssert(AfxChainIsEmpty(classes));
            AfxDeallocate(sctx->ownedBridges);
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

_ASX afxError AfxConfigureSoundDevice(afxNat sdevId, afxSoundContextConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sdevId != AFX_INVALID_INDEX);
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        AfxAssert(cfg);
        *cfg = (afxSoundContextConfig const) { 0 };
        cfg->mainBridge.flags = NIL;
        cfg->mainBridge.portIdx = 0;
        cfg->mainBridge.queueCnt = 3;
    }
    return err;
}

_ASX afxError AfxOpenSoundDevice(afxNat sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = AfxGetSoundContextClass(sdev);
        AfxAssertClass(cls, afxFcc_SCTX);
        AfxAssert(context);

        if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { sdev, (void*)cfg }))
            AfxThrowError();

        AfxAssertObjects(1, context, afxFcc_SCTX);
    }
    return err;
}

_ASX afxNat AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(contexts);
    AfxAssert(cnt);
    afxClass* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)contexts);
}

_ASX afxNat AfxEvokeSoundContexts(afxSoundDevice sdev, afxBool(*flt)(afxSoundContext, void*), void* fdd, afxNat first, afxNat cnt, afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(contexts);
    AfxAssert(flt);
    AfxAssert(cnt);
    afxClass* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)contexts);
}

_ASX afxNat AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_ASX afxNat AfxInvokeSoundContexts2(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*flt)(afxSoundContext, void*), void *fdd, afxBool(*f)(afxSoundContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(cnt);
    AfxAssert(flt);
    AfxAssert(f);
    afxClass* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxInvokeClassInstances2(cls, first, cnt, (void*)flt, fdd, (void*)f, udd);
}
