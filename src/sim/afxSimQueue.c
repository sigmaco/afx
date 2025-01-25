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
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#include "impl/asxImplementation.h"

_ASX afxError _AsxUnlockSimQueue(afxSimQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxUnlockMutex(&sque->workChnMtx);
    return err;
}

_ASX afxError _AsxLockSimQueue(afxSimQueue sque, afxBool wait, afxTimeSpec const* ts)
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

_ASX afxError _AsxSquePopBlob(afxSimQueue sque, void* blob, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxRecycleArenaUnit(&sque->workArena, blob, siz);
    return err;
}

_ASX void* _AsxSquePushBlob(afxSimQueue sque, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    void* blob = AfxRequestArenaUnit(&sque->workArena, siz);
    AFX_ASSERT(blob);
    return blob;
}

_ASX afxError _AsxSquePopWork(afxSimQueue sque, asxWork* work)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxPopLink(&work->hdr.chain);
    AfxRecycleArenaUnit(&sque->workArena, work, work->hdr.siz);
    return err;
}

_ASX asxWork* _AsxSquePushWork(afxSimQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
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

_ASX afxClass const* _AsxGetCatalystClass(afxSimQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxClass const* cls = &sque->cmdbCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CTX);
    return cls;
}

_ASX afxEngine AfxGetSampleQueueDevice(afxSimQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxSimBridge sexu = AfxGetProvider(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxEngine sdev = AfxGetMathBridgeDevice(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev;
}

_ASX afxSimulation AfxGetSampleQueueContext(afxSimQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxSimulation sim = sque->sim;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_ASX afxUnit AfxGetSampleQueuePort(afxSimQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    return sque->portId;
}

_ASX afxError _AsxSubmitSimCommands(afxSimQueue sque, asxSubmission const* ctrl, afxUnit cnt, afxContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    /// sque must be a valid afxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(contexts);

    if (!AfxTryLockMutex(&sque->workChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    asxWork* work = _AsxSquePushWork(sque, AVX_GET_STD_WORK_ID(Execute), sizeof(work->Execute) + (cnt * sizeof(work->Execute.cmdbs[0])), &cmdId);
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
        afxContext mctx = contexts[i];
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &mctx);

        AfxReacquireObjects(1, &mctx);
        work->Execute.cmdbs[i] = mctx;
        AfxIncAtom32(&mctx->submCnt);
        work->Execute.cmdbs[i]->state = asxContextState_PENDING;
    }

    AfxUnlockMutex(&sque->workChnMtx);

    return err;
}

_ASX afxError _AsxSqueDtorCb(afxSimQueue sque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    AfxDeregisterChainedClasses(&sque->classes);

    AfxDismountClass(&sque->cmdbCls);
    AfxDismantleSlock(&sque->cmdbReqLock);
    AfxDismantleQueue(&sque->cmdbRecycQue);

    AfxDismantleMutex(&sque->workChnMtx);
    AfxDismantleArena(&sque->workArena);
    AfxDismantleSlock(&sque->workArenaSlock);
    AfxDismantleCondition(&sque->idleCnd);
    AfxDismantleMutex(&sque->idleCndMtx);

    return err;
}

_ASX afxError _AsxSqueCtorCb(afxSimQueue sque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    afxSimBridge sexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _asxSimBridgeAcquisition const* cfg = AFX_CAST(_asxSimBridgeAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    sque->seng = cfg->seng;
    sque->portId = cfg->portId;
    sque->exuIdx = cfg->exuIdx;
    sque->sim = AfxGetMathBridgeContext(sexu);

    sque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&sque->workArenaSlock);
    AfxDeployArena(&sque->workArena, NIL, AfxHere());

    AfxDeployMutex(&sque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&sque->workChn, sexu);
    AfxDeployMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sque->idleCnd);

    sque->closed = FALSE;

    AfxDeployChain(&sque->classes, (void*)sque);

    sque->cmdbLockedForReq = FALSE;
    AfxDeploySlock(&sque->cmdbReqLock);
    AfxMakeQueue(&sque->cmdbRecycQue, sizeof(afxDrawContext), 3, NIL, 0);

    afxClassConfig tmpClsCfg = { 0 };
    tmpClsCfg = cfg->mctxClsCfg ? *cfg->mctxClsCfg : _ASX_CTX_CLASS_CONFIG;
    AFX_ASSERT(tmpClsCfg.fcc == afxFcc_CTX);
    AfxMountClass(&sque->cmdbCls, NIL, &sque->classes, &tmpClsCfg);

    return err;
}

_ASX afxClassConfig const _ASX_SQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_SQUE,
    .name = "MathQueue",
    .desc = "Math Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimQueue)),
    .ctor = (void*)_AsxSqueCtorCb,
    .dtor = (void*)_AsxSqueDtorCb
};
