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

#define _AMX_SOUND_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_MIX_QUEUE_C
#include "../impl/amxImplementation.h"

_AMX afxError _AmxUnlockSoundQueue(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxUnlockMutex(&mque->workChnMtx);
    return err;
}

_AMX afxError _AmxLockSoundQueue(afxMixQueue mque, afxBool wait, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (wait)
    {
        if (!AfxTryLockMutex(&mque->workChnMtx))
            return 1;
    }
    else
    {
        if (ts)
            err = AfxLockMutexTimed(&mque->workChnMtx, ts);
        else
            if (AfxLockMutex(&mque->workChnMtx))
                AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxSquePopBlob(afxMixQueue mque, void* blob, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxRecycleArenaUnit(&mque->workArena, blob, siz);
    return err;
}

_AMX void* _AmxSquePushBlob(afxMixQueue mque, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    void* blob = AfxRequestArenaUnit(&mque->workArena, siz);
    AFX_ASSERT(blob);
    return blob;
}

_AMX afxError _AmxSquePopWork(afxMixQueue mque, amxWork* work)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxPopLink(&work->hdr.chain);
    AfxRecycleArenaUnit(&mque->workArena, work, work->hdr.siz);
    return err;
}

_AMX amxWork* _AmxSquePushWork(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    amxWork* work = AfxRequestArenaUnit(&mque->workArena, siz);
    AFX_ASSERT(work);
    work->hdr.id = id;
    work->hdr.siz = siz;
    AfxGetTime(&work->hdr.pushTime);
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&work->hdr.chain, &mque->workChn);
    return work;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxMixDevice AfxGetMixQueueDevice(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxMixBridge mexu = AfxGetProvider(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixDevice mdev = AfxGetMixBridgeDevice(mexu);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev;
}

_AMX afxMixSystem AfxGetMixQueueContext(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxMixSystem msys = mque->msys;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMX afxUnit AfxGetMixQueuePort(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    return mque->portId;
}

_AMX afxError _AmxSqueStdDtorCb(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    AfxDismantleMutex(&mque->workChnMtx);
    AfxDismantleArena(&mque->workArena);
    AfxDismantleSlock(&mque->workArenaSlock);
    AfxDismantleCondition(&mque->idleCnd);
    AfxDismantleMutex(&mque->idleCndMtx);

    return err;
}

_AMX afxError _AmxSqueStdCtorCb(afxMixQueue mque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    afxMixBridge mexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _amxMixBridgeAcquisition const* cfg = AFX_CAST(_amxMixBridgeAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    mque->mdev = cfg->mdev;
    mque->portId = cfg->portId;
    mque->exuIdx = cfg->exuIdx;
    mque->msys = AfxGetMixBridgeSystem(mexu);

    mque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&mque->workArenaSlock);
    AfxDeployArena(&mque->workArena, NIL, AfxHere());

    AfxDeployMutex(&mque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&mque->workChn, mexu);
    AfxDeployMutex(&mque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mque->idleCnd);

    mque->closed = FALSE;

    return err;
}

_AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_MQUE,
    .name = "SoundQueue",
    .desc = "Sound Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixQueue)),
    .ctor = (void*)_AmxSqueStdCtorCb,
    .dtor = (void*)_AmxSqueStdDtorCb
};
