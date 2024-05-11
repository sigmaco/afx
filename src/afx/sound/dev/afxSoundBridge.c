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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _ASX_SOUND_C
#define _ASX_SOUND_BRIDGE_C
#define _ASX_SOUND_QUEUE_C
#include "qwadro/sound/afxSoundSystem.h"

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

_ASX afxNat AfxCountSoundQueues(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    return sdge->queCnt;
}

_ASX afxError AfxWaitForIdleSoundQueue(afxSoundBridge sdge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    if (!sdge->waitCb)
    {
        AfxAssertRange(sdge->queCnt, queIdx, 1);
        afxSoundQueue* sque = &sdge->queues[queIdx];

        AfxLockMutex(&sque->idleCndMtx);

        while (sque->workChn.cnt)
            AfxWaitCondition(&sque->idleCnd, &sque->idleCndMtx);

        AfxUnlockMutex(&sque->idleCndMtx);
    }
    else if (sdge->waitCb(sdge, queIdx))
        AfxThrowError();

    return err;
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

_ASX afxError _AsxSdgeStdDtor(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    for (afxNat i = 0; i < sdge->queCnt; i++)
    {
        afxSoundQueue* sque = &sdge->queues[i];

        AfxAbolishManager(&sque->streams);

        AfxCleanUpMutex(&sque->workChnMtx);
        AfxCleanUpSlock(&sque->reqLock);
        AfxDeallocateArena(&sque->workArena);
        AfxCleanUpSlock(&sque->workArenaSlock);
        AfxCleanUpCondition(&sque->idleCnd);
        AfxCleanUpMutex(&sque->idleCndMtx);
        AfxDeallocateQueue(&sque->recycQue);
    }

    AfxCleanUpChainedManagers(&sdge->managers);

    AfxDeallocate(sdge->queues);

    return err;
}

_ASX afxError _AsxSdgeStdCtor(afxSoundBridge sdge, afxCookie const* cookie)
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

    AfxSetUpChain(&sdge->managers, sdge);

    sdge->queCnt = AfxMax(3, cfg->queueCnt);

    if (!(sdge->queues = AfxAllocate(sdge->queCnt, sizeof(sdge->queues[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxClassConfig tmpClsCfg;

        for (afxNat i = 0; i < sdge->queCnt; i++)
        {
            afxSoundQueue* sque = &sdge->queues[i];

            sque->sdge = sdge;

            sque->immediate = 0;// !!spec->immedate;

            AfxSetUpSlock(&sque->workArenaSlock);
            AfxAllocateArena(NIL, &sque->workArena, NIL, AfxHere());

            sque->lockedForReq = FALSE;

            AfxSetUpMutex(&sque->workChnMtx, AFX_MTX_PLAIN);
            AfxSetUpChain(&sque->workChn, sdge);
            AfxSetUpMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
            AfxSetUpCondition(&sque->idleCnd);

            AfxSetUpSlock(&sque->reqLock);
            AfxAllocateQueue(&sque->recycQue, sizeof(void*/*afxSoundStream*/), 3);

            //tmpClsCfg = _AsxSiobStdImplementation;
            //AfxEstablishManager(&sque->streams, NIL, &sdge->managers, &tmpClsCfg);

            sque->closed = FALSE;
        }
    }
    return err;
}

_ASX afxClassConfig const _AsxSdgeStdImplementation =
{
    .fcc = afxFcc_SDGE,
    .name = "SoundBrige",
    .desc = "Sound Execution Bridge",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxSoundBridge)),
    .ctor = (void*)_AsxSdgeStdCtor,
    .dtor = (void*)_AsxSdgeStdDtor
};
