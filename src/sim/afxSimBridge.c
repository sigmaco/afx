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
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#include "impl/asxImplementation.h"

_ASX afxClass const* _AsxGetSimQueueClass(afxSimBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxClass const* cls = &sexu->squeCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SQUE);
    return cls;
}

_ASX afxEngine AfxGetMathBridgeDevice(afxSimBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxEngine seng = sexu->seng;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    return seng;
}

_ASX afxSimulation AfxGetMathBridgeContext(afxSimBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxSimulation sim = AfxGetProvider(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_ASX afxUnit AfxQuerySimBridgePort(afxSimBridge sexu, afxEngine* engine)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (engine)
    {
        afxEngine seng = sexu->seng;
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
        *engine = seng;
    }
    return sexu->portId;
}

_ASX afxUnit AfxGetSimQueues(afxSimBridge sexu, afxUnit baseQueIdx, afxUnit cnt, afxSimQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    /// queues must be a valid pointer to the afxSimQueue handles.
    AFX_ASSERT(queues);

    afxClass const* squeCls = _AsxGetSimQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(squeCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, rslt, queues);
    return rslt;
}

_ASX afxError AfxWaitForEmptySimQueue(afxSimBridge sexu, afxUnit queIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSimQueue sque;
    if (!AfxGetSimQueues(sexu, queIdx, 1, &sque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (!sque->waitCb)
    {
        AfxLockMutex(&sque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsec = AfxMax(1, timeout) * 100000; // 100000 = 0.0001 sec

        while (sque->workChn.cnt)
        {
            sexu->pingCb(sexu, 0);
            AfxWaitTimedCondition(&sque->idleCnd, &sque->idleCndMtx, &ts);
        }
        AfxUnlockMutex(&sque->idleCndMtx);
    }
    else if (sque->waitCb(sque, timeout))
        AfxThrowError();

    return err;
}

_ASX afxError AfxWaitForIdleSimBridge(afxSimBridge sexu, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (sexu->pingCb)
        sexu->pingCb(sexu, 0);

    afxClass const* squeCls = _AsxGetSimQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AfxWaitForEmptySimQueue(sexu, i, timeout);

    return err;
}

_ASX afxError _AsxMexuDtorCb(afxSimBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSimulation sim = AfxGetMathBridgeContext(sexu);

    AfxWaitForSimulation(sim, 0);
    AfxWaitForSimulation(sim, 0); // yes, two times.

    AfxDeregisterChainedClasses(&sexu->classes);

    if (sexu->worker)
    {
        afxInt exitCode;
        AfxWaitForThreadExit(sexu->worker, &exitCode);
        AfxDisposeObjects(1, &sexu->worker);
    }

    AfxDismantleCondition(&sexu->schedCnd);
    AfxDismantleMutex(&sexu->schedCndMtx);

    return err;
}

_ASX afxError _AsxMexuCtorCb(afxSimBridge sexu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    _asxSimBridgeAcquisition const* cfg = AFX_CAST(_asxSimBridgeAcquisition const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    sexu->seng = cfg->seng;
    AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    sexu->portId = cfg->portId;
    sexu->exuIdx = cfg->exuIdx;

    sexu->pingCb = _AsxSexu_PingCb;
    sexu->workerProc = _ASX_SPU_THREAD_PROC;
    sexu->procCb = _AsxSpu_ProcCb;

    sexu->workVmt = &_ASX_SPU_WORK_VMT;
    //sexu->cmdVmt = &_ASX_MPU_CMD_VMT;

    AfxDeployMutex(&sexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sexu->schedCnd);
    sexu->schedCnt = 0;

    AfxDeployChain(&sexu->classes, sexu);
    afxClassConfig clsCfg = cfg->squeClsCfg ? *cfg->squeClsCfg : _ASX_SQUE_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SQUE);
    AfxMountClass(&sexu->squeCls, NIL, &sexu->classes, &clsCfg);

    afxClass* squeCls = (afxClass*)_AsxGetSimQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxSimQueue queues[ASX_MAX_SIM_QUEUE_PER_BRIDGE];
    AFX_ASSERT(ASX_MAX_SIM_QUEUE_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(squeCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { sexu, cfg }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = DrawThreadProc;
        thrCfg.purpose = afxThreadPurpose_SIM;
        thrCfg.udd[0] = sexu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &sexu->worker))
        {
            AfxThrowError();
        }
    }

    if (err)
    {
        AfxDeregisterChainedClasses(&sexu->classes);
        AfxDismantleCondition(&sexu->schedCnd);
        AfxDismantleMutex(&sexu->schedCndMtx);
    }
    return err;
}

_ASX afxClassConfig const _ASX_SEXU_CLASS_CONFIG =
{
    .fcc = afxFcc_SEXU,
    .name = "MathBrige",
    .desc = "Math Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxSimBridge)),
    .ctor = (void*)_AsxMexuCtorCb,
    .dtor = (void*)_AsxMexuDtorCb
};
