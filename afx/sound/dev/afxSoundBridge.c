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
#include "../../dev/AsxImplKit.h"

_ASX afxSoundDevice AfxGetSoundBridgeDevice(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxSoundDevice sdev = AfxGetParent(sdge);
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

_ASX afxNat _AsxCountSoundQueues(afxSoundBridge sdge, afxNat baseQueIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sdge must be a valid afxSoundBridge handle.
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssertRange(sdge->queCnt, baseQueIdx, 1);
    return sdge->queCnt - baseQueIdx;
}

_ASX afxBool _AsxGetSoundQueue(afxSoundBridge sdge, afxNat queIdx, afxSoundQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// sdge must be a valid afxSoundBridge handle.
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    /// queIdx must be less than the value of queCnt for the bridge indicated by portIdx when device context was created.
    AfxAssertRange(sdge->queCnt, queIdx, 1);
    afxBool rslt;

    if (!(rslt = (queIdx < sdge->queCnt))) AfxThrowError();
    else
    {
        afxSoundQueue dque = sdge->queues[queIdx];
        AfxAssertObjects(1, &dque, afxFcc_DQUE);
        /// queue must be a valid pointer to a afxSoundQueue handle.        
        *queue = dque;
    }
    return rslt;
}

_ASX afxError _AsxWaitForIdleSoundQueue(afxSoundBridge sdge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// sdge must be a valid afxSoundBridge handle.
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxSoundQueue sque;

    if (!_AsxGetSoundQueue(sdge, queIdx, &sque)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sque, afxFcc_SQUE);

        if (!sdge->waitCb)
        {
            AfxLockMutex(&sque->idleCndMtx);

            while (sque->workChn.cnt)
                AfxWaitCondition(&sque->idleCnd, &sque->idleCndMtx);

            AfxUnlockMutex(&sque->idleCndMtx);
        }
        else if (sdge->waitCb(sdge, queIdx))
            AfxThrowError();
    }
    return err;
}

_ASX afxError _AsxWaitForIdleSoundBridge(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    /// sdge must be a valid afxSoundBridge handle.
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxNat queCnt = sdge->queCnt;

    for (afxNat i = 0; i < queCnt; i++)
        _AsxWaitForIdleSoundQueue(sdge, i);

    return err;
}

_ASX afxNat _AsxSubmitSoundCommands(afxSoundBridge sdge, asxSubmission const* ctrl, afxNat cnt, asxTracker trackers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssert(cnt);
    AfxAssert(trackers);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = sdge->submitCb(sdge, ctrl, cnt, trackers)))
        AfxThrowError();

    return queIdx;
}

_ASX afxNat _AsxSubmitSoundOutputFlushes(afxSoundBridge sdge, asxFlush const* ctrl, afxNat cnt, afxSoundOutput outputs[], afxNat const bufIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssert(cnt);
    AfxAssert(outputs);
    AfxAssert(bufIdx);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = sdge->flushCb(sdge, ctrl, cnt, outputs, bufIdx)))
        AfxThrowError();

    return queIdx;
}

_ASX afxNat _AsxSubmitTransferences(afxSoundBridge sdge, asxTransference const* ctrl, afxNat opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = sdge->transferCb(sdge, ctrl, opCnt, ops)))
        AfxThrowError();

    return queIdx;
}

_ASX afxError _AsxSqueStdDtorCb(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sque, afxFcc_SQUE);

    AfxCleanUpMutex(&sque->workChnMtx);
    AfxDismantleArena(&sque->workArena);
    AfxDismantleSlock(&sque->workArenaSlock);
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

_ASX afxError _AsxSqueStdCtorCb(afxSoundQueue sque, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sque, afxFcc_SQUE);

    afxSoundContext sctx = args[0];
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundBridge sdge = args[1];
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxSoundBridgeConfig const *cfg = ((afxSoundBridgeConfig const *)args[2]);
    AfxAssert(cfg);

    sque->sdge = sdge;
    sque->sctx = sctx;

    sque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&sque->workArenaSlock);
    AfxDeployArena(&sque->workArena, NIL, AfxHere());

    AfxSetUpMutex(&sque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&sque->workChn, sdge);
    AfxSetUpMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&sque->idleCnd);

    sque->closed = FALSE;

    return err;
}

_ASX afxError _AsxSdgeStdCtorCb(afxSoundBridge sdge, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    afxSoundDevice sdev = args[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundContext sctx = args[1];
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundBridgeConfig const *cfg = ((afxSoundBridgeConfig const *)args[2]);
    AfxAssert(cfg);

    AfxAssertRange(AfxCountSoundPorts(sdev), cfg->portIdx, 1);
    sdge->portIdx = cfg->portIdx;
    sdge->sctx = sctx;

    sdge->waitCb = NIL;
    sdge->submitCb = NIL;
    sdge->transferCb = NIL;

    sdge->queCnt = AfxMax(_AsxSqueStdImplementation.unitsPerPage, cfg->queueCnt);

    AfxDeployChain(&sdge->classes, sdge);

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
    .desc = "Sound Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundQueue)),
    .ctor = (void*)_AsxSqueStdCtorCb,
    .dtor = (void*)_AsxSqueStdDtorCb
};

_ASX afxClassConfig const _AsxSdgeStdImplementation =
{
    .fcc = afxFcc_SDGE,
    .name = "SoundBrige",
    .desc = "Sound Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundBridge)),
    .ctor = (void*)_AsxSdgeStdCtorCb,
    .dtor = (void*)_AsxSdgeStdDtorCb
};
