/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _ASX_SOUND_C
#define _ASX_SOUND_BRIDGE_C
#define _ASX_SOUND_QUEUE_C
#include "../impl/asxImplementation.h"

_ASX afxError _AsxUnlockSoundQueue(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxUnlockMutex(&sque->workChnMtx);
    return err;
}

_ASX afxError _AsxLockSoundQueue(afxSoundQueue sque, afxBool wait, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (wait)
    {
        if (!AfxTryLockMutex(&sque->workChnMtx))
            return 1;
    }
    else
    {
        if (ts)
            err = AfxLockMutexTimed(&sque->workChnMtx, ts);
        else
            if (AfxLockMutex(&sque->workChnMtx))
                AfxThrowError();
    }
    return err;
}

_ASX afxError _AsxSquePopBlob(afxSoundQueue sque, void* blob, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxRecycleArenaUnit(&sque->workArena, blob, siz);
    return err;
}

_ASX void* _AsxSquePushBlob(afxSoundQueue sque, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    void* blob = AfxRequestArenaUnit(&sque->workArena, siz);
    AFX_ASSERT(blob);
    return blob;
}

_ASX afxError _AsxSquePopWork(afxSoundQueue sque, asxWork* work)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxPopLink(&work->hdr.chain);
    AfxRecycleArenaUnit(&sque->workArena, work, work->hdr.siz);
    return err;
}

_ASX asxWork* _AsxSquePushWork(afxSoundQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    asxWork* work = AfxRequestArenaUnit(&sque->workArena, siz);
    AFX_ASSERT(work);
    work->hdr.id = id;
    work->hdr.siz = siz;
    AfxGetTime(&work->hdr.pushTime);
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&work->hdr.chain, &sque->workChn);
    return work;
}

////////////////////////////////////////////////////////////////////////////////

_ASX afxSoundDevice AfxGetSoundQueueDevice(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxSoundBridge sexu = AfxGetProvider(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxSoundDevice sdev = AfxGetSoundBridgeDevice(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev;
}

_ASX afxSoundSystem AfxGetSoundQueueContext(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxSoundSystem ssys = sque->ssys;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    return ssys;
}

_ASX afxUnit AfxGetSoundQueuePort(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    return sque->portId;
}

_ASX afxError _AsxSqueStdDtorCb(afxSoundQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    AfxDismantleMutex(&sque->workChnMtx);
    AfxDismantleArena(&sque->workArena);
    AfxDismantleSlock(&sque->workArenaSlock);
    AfxDismantleCondition(&sque->idleCnd);
    AfxDismantleMutex(&sque->idleCndMtx);

    return err;
}

_ASX afxError _AsxSqueStdCtorCb(afxSoundQueue sque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    afxSoundBridge sexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _asxSoundBridgeAcquisition const* cfg = AFX_CAST(_asxSoundBridgeAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    sque->sdev = cfg->sdev;
    sque->portId = cfg->portId;
    sque->exuIdx = cfg->exuIdx;
    sque->ssys = AfxGetSoundBridgeSystem(sexu);

    sque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&sque->workArenaSlock);
    AfxDeployArena(&sque->workArena, NIL, AfxHere());

    AfxDeployMutex(&sque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&sque->workChn, sexu);
    AfxDeployMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sque->idleCnd);

    sque->closed = FALSE;

    return err;
}

_ASX afxClassConfig const _ASX_SQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_SQUE,
    .name = "SoundQueue",
    .desc = "Sound Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundQueue)),
    .ctor = (void*)_AsxSqueStdCtorCb,
    .dtor = (void*)_AsxSqueStdDtorCb
};
