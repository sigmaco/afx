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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _ASX_SOUND_C
#define _ASX_SOUND_BRIDGE_C
#define _ASX_SOUND_QUEUE_C
#include "../src/afx/sound/dev/asxDevKit.h"

_ASX afxSoundDevice AfxGetSoundBridgeDevice(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxSoundDevice sdev = AfxGetObjectProvider(sdge);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxSoundContext AfxGetSoundBridgeContext(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxSoundContext sctx = sdge->sctx;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx;
}

_ASX afxNat AfxGetSoundBridgePort(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    return sdge->portIdx;
}

_ASX afxClass const* _AsxGetSoundQueueClass(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    /// sdge must be a valid afxSoundBridge handle.
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxClass const* cls = &sdge->squeCls;
    AfxAssertClass(cls, afxFcc_SQUE);
    return cls;
}

_ASX afxNat AfxEnqueueExecutionRequest(afxSoundBridge sdge, afxFence fenc, afxNat cnt, afxExecutionRequest2 const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = sdge->executeCb(sdge, fenc, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_ASX afxNat AfxEnqueueTransferRequest(afxSoundBridge sdge, afxFence fenc, afxNat cnt, afxTransferRequest2 const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = sdge->transferCb(sdge, fenc, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_ASX afxError _AsxSqueStdDtorCb(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sque, afxFcc_SQUE);

    AfxCleanUpMutex(&sque->workChnMtx);
    AfxDeallocateArena(&sque->workArena);
    AfxCleanUpSlock(&sque->workArenaSlock);
    AfxCleanUpCondition(&sque->idleCnd);
    AfxCleanUpMutex(&sque->idleCndMtx);

    return err;
}

_ASX afxError _AsxSdgeStdDtorCb(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    AfxWaitForSoundContext(sdge->sctx);
    AfxWaitForSoundContext(sdge->sctx); // yes, two times.

    AfxAssertObjects(sdge->queCnt, sdge->queues, afxFcc_SQUE);
    AfxReleaseObjects(sdge->queCnt, sdge->queues);
    AfxCleanUpChainedClasses(&sdge->classes);
    AfxDeallocate(sdge->queues);

    return err;
}

_ASX afxError _AsxSqueStdCtorCb(afxSoundQueue sque, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sque, afxFcc_SQUE);

    afxSoundContext sctx = cookie->udd[0];
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundBridge sdge = cookie->udd[1];
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxSoundBridgeConfig const *cfg = ((afxSoundBridgeConfig const *)cookie->udd[2]);
    AfxAssert(cfg);

    sque->sdge = sdge;
    sque->sctx = sctx;

    sque->immediate = 0;// !!spec->immedate;

    AfxSetUpSlock(&sque->workArenaSlock);
    AfxAllocateArena(NIL, &sque->workArena, NIL, AfxHere());

    AfxSetUpMutex(&sque->workChnMtx, AFX_MTX_PLAIN);
    AfxSetUpChain(&sque->workChn, sdge);
    AfxSetUpMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&sque->idleCnd);

    sque->closed = FALSE;

    return err;
}

_ASX afxError _AsxSdgeStdCtorCb(afxSoundBridge sdge, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    afxSoundDevice sdev = cookie->udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundContext sctx = cookie->udd[1];
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundBridgeConfig const *cfg = ((afxSoundBridgeConfig const *)cookie->udd[2]);
    AfxAssert(cfg);

    AfxAssertRange(AfxCountSoundPorts(sdev), cfg->portIdx, 1);
    sdge->portIdx = cfg->portIdx;
    sdge->sctx = sctx;

    sdge->waitCb = NIL;
    sdge->executeCb = NIL;
    sdge->transferCb = NIL;

    sdge->queCnt = AfxMax(_AsxSqueStdImplementation.unitsPerPage, cfg->queueCnt);

    AfxSetUpChain(&sdge->classes, sdge);

    sdge->queCnt = AfxMax(3, cfg->queueCnt);

    afxClassConfig clsCfg = _AsxSqueStdImplementation;
    clsCfg.maxCnt = sdge->queCnt;
    clsCfg.unitsPerPage = sdge->queCnt;
    AfxRegisterClass(&sdge->squeCls, NIL, &sdge->classes, &clsCfg);

    if (!(sdge->queues = AfxAllocate(sdge->queCnt, sizeof(sdge->queues[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxClass* cls = (afxClass*)_AsxGetSoundQueueClass(sdge);
        AfxAssertClass(cls, afxFcc_SQUE);

        if (AfxAcquireObjects(cls, sdge->queCnt, (afxObject*)sdge->queues, (void const*[]) { sctx, sdge, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(sdge->queCnt, sdge->queues, afxFcc_SQUE);
        }

        if (err)
            AfxDeallocate(sdge->queues);
    }

    if (err)
        AfxCleanUpChainedClasses(&sdge->classes);

    return err;
}

_ASX afxClassConfig const _AsxSqueStdImplementation =
{
    .fcc = afxFcc_SQUE,
    .name = "SoundQueue",
    .desc = "Sound Device Execution Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundQueue)),
    .ctor = (void*)_AsxSqueStdCtorCb,
    .dtor = (void*)_AsxSqueStdDtorCb
};

_ASX afxClassConfig const _AsxSdgeStdImplementation =
{
    .fcc = afxFcc_SDGE,
    .name = "SoundBrige",
    .desc = "Sound Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundBridge)),
    .ctor = (void*)_AsxSdgeStdCtorCb,
    .dtor = (void*)_AsxSdgeStdDtorCb
};
