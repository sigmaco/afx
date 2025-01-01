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
#define _AMX_ENGINE_C
#define _AMX_SIM_BRIDGE_C
#define _AMX_SIM_QUEUE_C
#define _AMX_CATALYST_C
#include "impl/amxImplementation.h"

_AMX afxError _AmxUnlockSimQueue(afxSimQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxUnlockMutex(&mque->workChnMtx);
    return err;
}

_AMX afxError _AmxLockSimQueue(afxSimQueue mque, afxBool wait, afxTimeSpec const* ts)
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

_AMX afxError _AmxMquePopBlob(afxSimQueue mque, void* blob, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxRecycleArenaUnit(&mque->workArena, blob, siz);
    return err;
}

_AMX void* _AmxMquePushBlob(afxSimQueue mque, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    void* blob = AfxRequestArenaUnit(&mque->workArena, siz);
    AFX_ASSERT(blob);
    return blob;
}

_AMX afxError _AmxMquePopWork(afxSimQueue mque, amxWork* work)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxPopLink(&work->hdr.chain);
    AfxRecycleArenaUnit(&mque->workArena, work, work->hdr.siz);
    return err;
}

_AMX amxWork* _AmxMquePushWork(afxSimQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
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

_AMX afxClass const* _AmxGetCatalystClass(afxSimQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxClass const* cls = &mque->cmdbCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CYST);
    return cls;
}

_AMX afxEngine AfxGetSimQueueDevice(afxSimQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxSimBridge mexu = AfxGetProvider(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxEngine sdev = AfxGetMathBridgeDevice(mexu);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &sdev);
    return sdev;
}

_AMX afxSimulation AfxGetSimQueueContext(afxSimQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxSimulation sim = mque->sim;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_AMX afxUnit AfxGetSimQueuePort(afxSimQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    return mque->portId;
}

_AMX afxError _AmxSubmitSimCommands(afxSimQueue mque, avxSubmission const* ctrl, afxUnit cnt, afxCatalyst contexts[])
{
    afxError err = AFX_ERR_NONE;
    /// mque must be a valid afxCatalyst handle.
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(contexts);

    if (!AfxTryLockMutex(&mque->workChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    amxWork* work = _AmxMquePushWork(mque, AVX_GET_STD_WORK_ID(Execute), sizeof(work->Execute) + (cnt * sizeof(work->Execute.cmdbs[0])), &cmdId);
    AFX_ASSERT(work);

    if (ctrl)
    {
        work->Execute.fence = ctrl->fence;
        work->Execute.signal = ctrl->signalSems;
        work->Execute.wait = ctrl->waitSems;
    }

    work->Execute.cmdbCnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxCatalyst mctx = contexts[i];
        AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &mctx);

        AfxReacquireObjects(1, &mctx);
        work->Execute.cmdbs[i] = mctx;
        AfxIncAtom32(&mctx->submCnt);
        work->Execute.cmdbs[i]->state = amxCatalystState_PENDING;
    }

    AfxUnlockMutex(&mque->workChnMtx);

    return err;
}

_AMX afxError _AmxMqueDtorCb(afxSimQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    AfxDeregisterChainedClasses(&mque->classes);

    AfxDismountClass(&mque->cmdbCls);
    AfxDismantleSlock(&mque->cmdbReqLock);
    AfxDismantleQueue(&mque->cmdbRecycQue);

    AfxDismantleMutex(&mque->workChnMtx);
    AfxDismantleArena(&mque->workArena);
    AfxDismantleSlock(&mque->workArenaSlock);
    AfxDismantleCondition(&mque->idleCnd);
    AfxDismantleMutex(&mque->idleCndMtx);

    return err;
}

_AMX afxError _AmxMqueCtorCb(afxSimQueue mque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    afxSimBridge mexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _amxSimBridgeAcquisition const* cfg = AFX_CAST(_amxSimBridgeAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    mque->mdev = cfg->mdev;
    mque->portId = cfg->portId;
    mque->exuIdx = cfg->exuIdx;
    mque->sim = AfxGetMathBridgeContext(mexu);

    mque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&mque->workArenaSlock);
    AfxDeployArena(&mque->workArena, NIL, AfxHere());

    AfxDeployMutex(&mque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&mque->workChn, mexu);
    AfxDeployMutex(&mque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mque->idleCnd);

    mque->closed = FALSE;

    AfxDeployChain(&mque->classes, (void*)mque);

    mque->cmdbLockedForReq = FALSE;
    AfxDeploySlock(&mque->cmdbReqLock);
    AfxMakeQueue(&mque->cmdbRecycQue, sizeof(afxDrawContext), 3, NIL, 0);

    afxClassConfig tmpClsCfg = { 0 };
    tmpClsCfg = cfg->mctxClsCfg ? *cfg->mctxClsCfg : _AMX_CYST_CLASS_CONFIG;
    AFX_ASSERT(tmpClsCfg.fcc == afxFcc_CYST);
    AfxMountClass(&mque->cmdbCls, NIL, &mque->classes, &tmpClsCfg);

    return err;
}

_AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_MQUE,
    .name = "MathQueue",
    .desc = "Math Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimQueue)),
    .ctor = (void*)_AmxMqueCtorCb,
    .dtor = (void*)_AmxMqueDtorCb
};
