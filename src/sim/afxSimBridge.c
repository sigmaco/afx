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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AMX_ENGINE_C
#define _AMX_SIM_BRIDGE_C
#define _AMX_SIM_QUEUE_C
#include "impl/amxImplementation.h"

_AMX afxClass const* _AmxGetSimQueueClass(afxSimBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    /// mexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxClass const* cls = &mexu->mqueCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MQUE);
    return cls;
}

_AMX afxEngine AfxGetMathBridgeDevice(afxSimBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxEngine mdev = mexu->mdev;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev;
}

_AMX afxSimulation AfxGetMathBridgeContext(afxSimBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxSimulation sim = AfxGetProvider(mexu);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_AMX afxUnit AfxQuerySimBridgePort(afxSimBridge mexu, afxEngine* engine)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (engine)
    {
        afxEngine mdev = mexu->mdev;
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
        *engine = mdev;
    }
    return mexu->portId;
}

_AMX afxUnit AfxGetSimQueues(afxSimBridge mexu, afxUnit baseQueIdx, afxUnit cnt, afxSimQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// mexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    /// queues must be a valid pointer to the afxSimQueue handles.
    AFX_ASSERT(queues);

    afxClass const* mqueCls = _AmxGetSimQueueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(mqueCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, rslt, queues);
    return rslt;
}

_AMX afxError AfxWaitForEmptySimQueue(afxSimBridge mexu, afxUnit queIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// mexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxSimQueue mque;
    if (!AfxGetSimQueues(mexu, queIdx, 1, &mque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (!mque->waitCb)
    {
        AfxLockMutex(&mque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsec = AfxMax(1, timeout) * 100000; // 100000 = 0.0001 sec

        while (mque->workChn.cnt)
        {
            mexu->pingCb(mexu, 0);
            AfxWaitTimedCondition(&mque->idleCnd, &mque->idleCndMtx, &ts);
        }
        AfxUnlockMutex(&mque->idleCndMtx);
    }
    else if (mque->waitCb(mque, timeout))
        AfxThrowError();

    return err;
}

_AMX afxError AfxWaitForIdleSimBridge(afxSimBridge mexu, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// mexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (mexu->pingCb)
        mexu->pingCb(mexu, 0);

    afxClass const* mqueCls = _AmxGetSimQueueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxUnit queCnt = mqueCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AfxWaitForEmptySimQueue(mexu, i, timeout);

    return err;
}

_AMX afxError _AmxMexuDtorCb(afxSimBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxSimulation sim = AfxGetMathBridgeContext(mexu);

    AfxWaitForSimulation(sim, 0);
    AfxWaitForSimulation(sim, 0); // yes, two times.

    AfxDeregisterChainedClasses(&mexu->classes);

    if (mexu->worker)
    {
        afxInt exitCode;
        AfxWaitForThreadExit(mexu->worker, &exitCode);
        AfxDisposeObjects(1, &mexu->worker);
    }

    AfxDismantleCondition(&mexu->schedCnd);
    AfxDismantleMutex(&mexu->schedCndMtx);

    return err;
}

_AMX afxError _AmxMexuCtorCb(afxSimBridge mexu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    _amxSimBridgeAcquisition const* cfg = AFX_CAST(_amxSimBridgeAcquisition const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    mexu->mdev = cfg->mdev;
    AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    mexu->portId = cfg->portId;
    mexu->exuIdx = cfg->exuIdx;

    mexu->pingCb = _AmxMexu_PingCb;
    mexu->workerProc = _AMX_MPU_THREAD_PROC;
    mexu->procCb = _AmxMpu_ProcCb;

    mexu->workVmt = &_AMX_MPU_WORK_VMT;
    //mexu->cmdVmt = &_AMX_MPU_CMD_VMT;

    AfxDeployMutex(&mexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mexu->schedCnd);
    mexu->schedCnt = 0;

    AfxDeployChain(&mexu->classes, mexu);
    afxClassConfig clsCfg = cfg->mqueClsCfg ? *cfg->mqueClsCfg : _AMX_MQUE_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MQUE);
    AfxMountClass(&mexu->mqueCls, NIL, &mexu->classes, &clsCfg);

    afxClass* mqueCls = (afxClass*)_AmxGetSimQueueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxSimQueue queues[AMX_MAX_SIM_QUEUE_PER_BRIDGE];
    AFX_ASSERT(AMX_MAX_SIM_QUEUE_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(mqueCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { mexu, cfg }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = DrawThreadProc;
        thrCfg.purpose = afxThreadPurpose_SIM;
        thrCfg.udd[0] = mexu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &mexu->worker))
        {
            AfxThrowError();
        }
    }

    if (err)
    {
        AfxDeregisterChainedClasses(&mexu->classes);
        AfxDismantleCondition(&mexu->schedCnd);
        AfxDismantleMutex(&mexu->schedCndMtx);
    }
    return err;
}

_AMX afxClassConfig const _AMX_MEXU_CLASS_CONFIG =
{
    .fcc = afxFcc_MEXU,
    .name = "MathBrige",
    .desc = "Math Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimBridge)),
    .ctor = (void*)_AmxMexuCtorCb,
    .dtor = (void*)_AmxMexuDtorCb
};
