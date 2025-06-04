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

#define _AMX_MIX_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_MIX_QUEUE_C
#include "../impl/amxImplementation.h"

_AMX afxError _AmxMqueUnlockIoReqChain(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxUnlockMutex(&mque->iorpChnMtx);
    return err;
}

_AMX afxError _AmxMqueLockIoReqChain(afxMixQueue mque, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (timeout == 0)
    {
        if (!AfxTryLockMutex(&mque->iorpChnMtx))
            return afxError_TIMEOUT;
    }
    else
    {
        afxTimeSpec ts;
        AfxMakeTimeSpec(&ts, timeout);

        if (afxError_TIMEOUT == AfxLockMutexTimed(&mque->iorpChnMtx, &ts))
            return afxError_TIMEOUT;
        else
            AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxMquePopBlob(afxMixQueue mque, void* blob, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxReclaimToArena(&mque->iorpArena, blob, siz);
    return err;
}

_AMX void* _AmxMquePushBlob(afxMixQueue mque, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    void* blob = AfxRequestFromArena(&mque->iorpArena, siz, 1, NIL, 0);
    AFX_ASSERT(blob);
    return blob;
}

_AMX afxError _AmxMquePopIoReqPacket(afxMixQueue mque, _amxIoReqPacket* iorp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AFX_ASSERT(iorp);
    AfxPopLink(&iorp->hdr.chain);
    AfxReclaimToArena(&mque->iorpArena, iorp, iorp->hdr.siz);
    return err;
}

_AMX afxError _AmxMquePushIoReqPacket(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxIoReqPacket** pIorp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    _amxIoReqPacket* iorp = AfxRequestFromArena(&mque->iorpArena, siz, 1, NIL, 0);
    AFX_ASSERT(iorp);
    iorp->hdr.id = id;
    iorp->hdr.siz = siz;
    AfxGetTime(&iorp->hdr.pushTime);

    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&iorp->hdr.chain, &mque->iorpChn);
    
    AFX_ASSERT(pIorp);
    *pIorp = iorp;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxMixDevice AfxGetMixQueueDevice(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxMixBridge mexu = AfxGetProvider(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, NIL);
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

_AMX afxError _AmxMqueDtorCb(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    AfxDismantleMutex(&mque->iorpChnMtx);
    AfxDismantleArena(&mque->iorpArena);
    AfxDismantleFutex(&mque->iorpArenaSlock);
    AfxDismantleCondition(&mque->idleCnd);
    AfxDismantleMutex(&mque->idleCndMtx);

    return err;
}

_AMX afxError _AmxMqueCtorCb(afxMixQueue mque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    afxMixBridge mexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _amxMexuAcquisition const* cfg = AFX_CAST(_amxMexuAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    mque->mdev = cfg->mdev;
    mque->portId = cfg->portId;
    mque->exuIdx = cfg->exuIdx;
    mque->msys = AmxGetBridgedMixSystem(mexu);

    mque->immediate = 0;// !!spec->immedate;

    AfxDeployFutex(&mque->iorpArenaSlock);
    AfxMakeArena(&mque->iorpArena, NIL, AfxHere());

    AfxDeployMutex(&mque->iorpChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&mque->iorpChn, mexu);
    AfxDeployMutex(&mque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mque->idleCnd);

    mque->closed = FALSE;

    return err;
}

_AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_MQUE,
    .name = "MixQueue",
    .desc = "Mix Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixQueue)),
    .ctor = (void*)_AmxMqueCtorCb,
    .dtor = (void*)_AmxMqueDtorCb
};
