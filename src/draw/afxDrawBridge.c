/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_OUTPUT_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "impl/avxImplementation.h"

_AVX afxDrawSystem AvxGetBridgedDrawSystem(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxDrawSystem dsys = AfxGetProvider(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxDrawDevice AvxGetBridgedDrawDevice(afxDrawBridge dexu, afxUnit* portId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxDrawDevice ddev = dexu->ddev;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    if (portId)
    {
        *portId = dexu->portId;
    }
    return ddev;
}

_AVX afxClass const* _AvxGetDrawQueueClass(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    // dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxClass const* cls = &dexu->dqueCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DQUE);
    return cls;
}

_AVX afxUnit AvxGetDrawQueues(afxDrawBridge dexu, afxUnit baseQueIdx, afxUnit cnt, afxDrawQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    // dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    // queues must be a valid pointer to the afxDrawQueue handles.
    AFX_ASSERT(queues);

    afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);
    AFX_ASSERT_CLASS(dqueCls, afxFcc_DQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(dqueCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, rslt, queues);
    return rslt;
}

_AVX afxError AvxWaitForIdleDrawBridge(afxDrawBridge dexu, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    if (dexu->pingCb)
        dexu->pingCb(dexu, 0);

    afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

    for (afxUnit i = 0; i < dqueCls->instCnt; i++)
    {
        afxDrawQueue dque;
        AvxGetDrawQueues(dexu, i, 1, &dque);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
        
        afxError err2;
        if ((err2 = AvxWaitForEmptyDrawQueue(dque, timeout)) && (err2 != afxError_TIMEOUT))
        {
            AfxThrowError();
        }
    }

    return err;
}

_AVX afxError _AvxDexuDtorCb(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AvxGetBridgedDrawSystem(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    //AvxWaitForDrawSystem(dsys, AFX_TIME_INFINITE);
    //AvxWaitForDrawSystem(dsys, AFX_TIME_INFINITE); // yes, two times.

    if (dexu->worker)
    {
        while (AfxIsThreadRunning(dexu->worker))
        {
            AfxRequestThreadInterruption(dexu->worker);
        }
        afxInt exitCode;
        AfxWaitForThreadExit(dexu->worker, &exitCode);
        AfxDisposeObjects(1, &dexu->worker);
    }

    AfxDeregisterChainedClasses(&dexu->classes);

    AfxDismantleCondition(&dexu->schedCnd);
    AfxDismantleMutex(&dexu->schedCndMtx);

    return err;
}

_AVX afxError _AvxDexuCtorCb(afxDrawBridge dexu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AFX_CAST(afxDrawSystem, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    _avxDexuAcquisition const* cfg = AFX_CAST(_avxDexuAcquisition const*, args[1]) + invokeNo;
    
    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    dexu->ddev = cfg->ddev;
    AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    dexu->portId = cfg->portId;
    dexu->exuIdx = cfg->exuIdx;

    dexu->pingCb = _AvxDexu_PingCb;
    dexu->workerProc = _AVX_DPU_THREAD_PROC;
    dexu->procCb = _AvxDpu_ProcCb;

    dexu->iorpVmt = &_AVX_DPU_IORP_VMT;
    dexu->cmdVmt = &_AVX_DPU_CMD_VMT;

    AfxDeployMutex(&dexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dexu->schedCnd);
    dexu->schedCnt = 0;

    AfxDeployChain(&dexu->classes, dexu);
    afxClassConfig clsCfg = cfg->dqueClsCfg ? *cfg->dqueClsCfg : _AVX_DQUE_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_DQUE);
    AfxMountClass(&dexu->dqueCls, NIL, &dexu->classes, &clsCfg);

    afxClass* dqueCls = (afxClass*)_AvxGetDrawQueueClass(dexu);
    AFX_ASSERT_CLASS(dqueCls, afxFcc_DQUE);
    afxDrawQueue queues[AVX_MAX_QUEUES_PER_BRIDGE];
    AFX_ASSERT(AVX_MAX_QUEUES_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(dqueCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { dexu, cfg }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = DrawThreadProc;
        thrCfg.usage = afxThreadUsage_DRAW;
        thrCfg.udd[0] = dexu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &dexu->worker))
        {
            AfxThrowError();
        }
    }

    if (err)
    {
        AfxDeregisterChainedClasses(&dexu->classes);
        AfxDismantleCondition(&dexu->schedCnd);
        AfxDismantleMutex(&dexu->schedCndMtx);
    }
    return err;
}

_AVX afxClassConfig const _AVX_DEXU_CLASS_CONFIG =
{
    .fcc = afxFcc_DEXU,
    .name = "DrawBridge",
    .desc = "Draw Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawBridge)),
    .ctor = (void*)_AvxDexuCtorCb,
    .dtor = (void*)_AvxDexuDtorCb
};
