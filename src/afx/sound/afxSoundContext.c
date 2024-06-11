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
#define _ASX_SOUND_INPUT_C
#define _ASX_SOUND_OUTPUT_C
#define _ASX_SOUND_SYSTEM_C
#include "qwadro/sound/afxSoundSystem.h"

_ASX afxSoundDevice AfxGetSoundContextDevice(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundDevice sdev = AfxGetObjectProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxSoundBridge AfxGetSoundBridge(afxSoundContext sctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssertRange((afxNat)sctx->ownedBridgeCnt, portIdx, 1);
    afxSoundBridge sdge = sctx->ownedBridges[portIdx];
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    return sdge;
}

_ASX afxNat AfxCountSoundBridges(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx->ownedBridgeCnt;
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

_ASX afxError AfxDisconnectSoundContext(afxSoundContext sctx, afxBool inputs, afxBool outputs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    if (inputs)
    {
        afxSoundInput sin;
        while (AfxGetConnectedSoundInput(sctx, 0, &sin))
        {
            AfxAssertObjects(1, &sin, afxFcc_SIN);
            AfxDisconnectSoundInput(sin);
            break;
        }
    }

    if (outputs)
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

_ASX afxError AfxWaitForIdleSoundContext(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    if (!sctx->waitCb)
    {
        afxNat bridgeCnt = sctx->ownedBridgeCnt;

        for (afxNat i = 0; i < bridgeCnt; i++)
        {
            afxSoundBridge sdge = sctx->ownedBridges[i];
            AfxAssertObjects(1, &sctx, afxFcc_SCTX);
            afxNat queCnt = AfxCountSoundQueues(sdge);

            for (afxNat j = 0; j < queCnt; j++)
                AfxWaitForIdleSoundQueue(sdge, j);
        }
    }
    else if (sctx->waitCb(sctx))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxSctxStdDtor(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    AfxDisconnectSoundContext(sctx, TRUE, TRUE);
    AfxWaitForIdleSoundContext(sctx);

    AfxCleanUpChainedManagers(&sctx->ctx.classes);

    AfxAssertObjects(sctx->ownedBridgeCnt, sctx->ownedBridges, afxFcc_SDGE);
    AfxDeallocate(sctx->ownedBridges);

    AfxAssert(AfxChainIsEmpty(&sctx->ctx.classes));

    AfxDeallocateArena(&sctx->aren);

    return err;
}

_ASX afxError _AsxSctxStdCtor(afxSoundContext sctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AfxAssert(spec);

    afxSoundDevice sdev = cookie->udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundContextConfig const *config = ((afxSoundContextConfig const *)cookie->udd[1]) + cookie->no;

    sctx->running = FALSE;

    AfxAllocateArena(NIL, &sctx->aren, NIL, AfxHere());

    AfxSetUpChain(&sctx->inputs, sctx);
    AfxSetUpChain(&sctx->outputs, sctx);

    afxChain *classes = &sctx->ctx.classes;
    AfxSetUpChain(classes, (void*)sctx);

    // Acquire bridges and queues

    afxNat bridgeCnt = AfxCountSoundPorts(sdev);
    AfxAssert(bridgeCnt);
    afxSoundBridge* ownedBridges;
    sctx->ownedBridges = NIL;

    if (!(ownedBridges = AfxAllocate(bridgeCnt, sizeof(ownedBridges[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat j = 0; j < bridgeCnt; j++)
        {
            afxSoundBridgeConfig sdgeSpec = { 0 };
            sdgeSpec.portIdx = j;

            if (AfxAcquireObjects(&sdev->ports[j].sdgeMgr, 1, (afxObject*)&ownedBridges[j], (void const*[]) { sdev, sctx, &sdgeSpec }))
            {
                AfxThrowError();
                AfxReleaseObjects(j, ownedBridges);
            }
            else
            {
                AfxAssertObjects(1, &ownedBridges[j], afxFcc_SDGE);
            }
        }

        if (err) AfxDeallocate(ownedBridges);
        else
        {
            AfxAssertObjects(bridgeCnt, ownedBridges, afxFcc_SDGE);
            sctx->ownedBridgeCnt = bridgeCnt;
            sctx->ownedBridges = ownedBridges;
        }
    }

    if (err)
    {
        AfxAssertObjects(sctx->ownedBridgeCnt, sctx->ownedBridges, afxFcc_DDGE);
        AfxDeallocate(sctx->ownedBridges);
    }

    return err;
}

_ASX afxClassConfig const _AsxSctxStdImplementation =
{
    .fcc = afxFcc_SCTX,
    .name = "SoundContext",
    .desc = "Sound Device Context Management",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundContext)),
    .ctor = (void*)_AsxSctxStdCtor,
    .dtor = (void*)_AsxSctxStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireSoundContext(afxNat sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxManager* cls = AfxGetSoundContextManager(sdev);
        AfxAssertClass(cls, afxFcc_SCTX);
        AfxAssert(context);

        if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { sdev, (void*)cfg }))
            AfxThrowError();

        AfxAssertObjects(1, context, afxFcc_SCTX);
    }
    return err;
}

_ASX afxNat AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetSoundContextManager(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_ASX afxNat AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(contexts);
    AfxAssert(cnt);
    afxManager* cls = AfxGetSoundContextManager(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)contexts);
}

_ASX afxNat AfxCountSoundContexts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxManager* cls = AfxGetSoundContextManager(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxCountObjects(cls);
}
