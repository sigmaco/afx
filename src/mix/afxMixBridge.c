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
#define _AMX_MIX_CONTEXT_C
#include "impl/amxImplementation.h"

_AMX afxMixSystem AmxGetBridgedMixSystem(afxMixBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixSystem msys = AfxGetProvider(mexu);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMX afxMixDevice AmxGetBridgedMixDevice(afxMixBridge mexu, afxUnit* portId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixDevice mdev = mexu->mdev;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    if (portId)
    {
        *portId = mexu->portId;
    }
    return mdev;
}

_AMX afxClass const* _AmxGetMixQueueClass(afxMixBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxClass const* cls = &mexu->mqueCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MQUE);
    return cls;
}

_AMX afxUnit AmxGetMixQueues(afxMixBridge mexu, afxUnit first, afxUnit cnt, afxMixQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    // queues must be a valid pointer to the afxMixQueue handles.
    AFX_ASSERT(queues);

    afxClass const* mqueCls = _AmxGetMixQueueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(mqueCls, FALSE, first, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, rslt, queues);
    return rslt;
}

_AMX afxError AmxWaitForEmptyMixQueue(afxMixBridge mexu, afxUnit queIdx, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixQueue mque;
    if (!AmxGetMixQueues(mexu, queIdx, 1, &mque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (!mque->waitCb)
    {
        AfxLockMutex(&mque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsecs = AFX_MAX(1, timeout) * 100000; // 100000 = 0.0001 sec

        while (mque->iorpChn.cnt)
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

_AMX afxError AmxWaitForIdleMixBridge(afxMixBridge mexu, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (mexu->pingCb)
        mexu->pingCb(mexu, 0);

    afxClass const* mqueCls = _AmxGetMixQueueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxUnit queCnt = mqueCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AmxWaitForEmptyMixQueue(mexu, i, timeout);

    return err;
}

_AMX afxError _AmxMexuSubmitCallback(afxMixBridge mexu, void(*f)(void*, void*), void* udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(f);

    afxClass const* mqueCls = _AmxGetMixQueueClass(mexu);

    // sanitize arguments
    afxUnit totalQueCnt = mqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(0, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(1, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxMixQueue mque;
        if (!AmxGetMixQueues(mexu, baseQueIdx + queIdx, 1, &mque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
            if (!AfxTryLockMutex(&mque->iorpChnMtx))
                continue;
        }

        afxCmdId cmdId;
        _amxIoReqPacket* work;
        _AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Callback), sizeof(work->Callback), &cmdId, &work);
        AFX_ASSERT(work);

        work->Callback.f = f;
        work->Callback.udd = udd;

        AfxUnlockMutex(&mque->iorpChnMtx);

        mexu->pingCb(mexu, 0);
        break;
    }
    return err;
}

_AMX afxError _AmxMexuTransferBuffers(afxMixBridge mexu, amxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxClass const* mqueCls = _AmxGetMixQueueClass(mexu);

    // sanitize arguments
    afxUnit totalQueCnt = mqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxMixQueue mque;
        if (!AmxGetMixQueues(mexu, baseQueIdx + queIdx, 1, &mque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
            if (!AfxTryLockMutex(&mque->iorpChnMtx))
                continue;
        }

        afxCmdId cmdId;
        _amxIoReqPacket* work;
        _AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Transfer), sizeof(work->Transfer) + (opCnt * sizeof(work->Transfer.wavOps[0])), &cmdId, &work);
        AFX_ASSERT(work);

        work->Transfer.srcFcc = ctrl->srcFcc;
        work->Transfer.dstFcc = ctrl->dstFcc;
        work->Transfer.codec = ctrl->codec;
        work->Transfer.decSiz = ctrl->decSiz;

        work->Transfer.opCnt = opCnt;

        switch (ctrl->srcFcc)
        {
        case afxFcc_AUD:
        {
            work->Transfer.src.aud = ctrl->src.aud;
            AfxReacquireObjects(1, &work->Transfer.src.aud);
            break;
        }
        case afxFcc_IOB:
        {
            work->Transfer.src.iob = ctrl->src.iob;
            AfxReacquireObjects(1, &work->Transfer.src.iob);
            break;
        }
        case NIL:
        {
            work->Transfer.src.src = ctrl->src.src;
            break;
        }
        default: AfxThrowError(); break;
        }

        switch (ctrl->dstFcc)
        {
        case afxFcc_AUD:
        {
            work->Transfer.dst.aud = ctrl->dst.aud;
            AfxReacquireObjects(1, &work->Transfer.dst.aud);
            break;
        }
        case afxFcc_IOB:
        {
            work->Transfer.dst.iob = ctrl->dst.iob;
            AfxReacquireObjects(1, &work->Transfer.dst.iob);
            break;
        }
        case NIL:
        {
            work->Transfer.dst.dst = ctrl->dst.dst;
            break;
        }
        default: AfxThrowError(); break;
        }

        amxAudioIo const* riops = ops;
        for (afxUnit i = 0; i < opCnt; i++)
        {
            work->Transfer.wavOps[i] = riops[i];
        }
        int a = 1;

        AfxUnlockMutex(&mque->iorpChnMtx);

        mexu->pingCb(mexu, 0);
        break;
    }
    return err;
}

_AMX afxError _AmxMexuSubmitSink(afxMixBridge mexu, void* ctrl, afxSink sink, amxAudio aud, amxAudioPeriod const* seg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(sink);
    AFX_ASSERT(seg);
    AFX_ASSERT(aud);

    afxClass const* mqueCls = _AmxGetMixQueueClass(mexu);

    // sanitize arguments
    afxUnit baseQueIdx = 0;
    afxUnit queCnt = mqueCls->instCnt;
    AFX_ASSERT(queCnt);

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxMixQueue mque;
        if (!AmxGetMixQueues(mexu, baseQueIdx + queIdx, 1, &mque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
            if (!AfxTryLockMutex(&mque->iorpChnMtx))
                continue;
        }

        afxCmdId cmdId;
        _amxIoReqPacket* work;
        _AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Sink), sizeof(work->Sink), &cmdId, &work);
        AFX_ASSERT(work);

        work->Sink.buf = aud;
        work->Sink.sink = sink;
        work->Sink.seg = *seg;

        AfxUnlockMutex(&mque->iorpChnMtx);

        mexu->pingCb(mexu, 0);
        break;
    }
    return err;
}

_AMX afxError _AmxMexuRollMixers(afxMixBridge mexu, afxReal startTime, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixContext mixers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMixContext mix = mixers[i];
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        
        AfxReacquireObjects(1, &mix);

        if (mix->executor.chain)
            AfxPopLink(&mix->executor);
        
        AfxPushLink(&mix->executor, &mexu->activeMixers);
        mix->state = amxMixState_PENDING;
        mix->motor.localClock = startTime;
        mix->motor.localDur = dur;
        mix->motor.iterCnt = iterCnt;
    }
    return err;
}

_AMX afxError _AmxMexuDtorCb(afxMixBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixSystem msys = AmxGetBridgedMixSystem(mexu);

    AfxWaitForMixSystem(msys, AFX_TIME_INFINITE);
    AfxWaitForMixSystem(msys, AFX_TIME_INFINITE); // yes, two times.
    
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

_AMX afxError _AmxMexuCtorCb(afxMixBridge mexu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixSystem msys = AFX_CAST(afxMixSystem, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    _amxMexuAcquisition const* cfg = AFX_CAST(_amxMexuAcquisition const*, args[1]) + invokeNo;

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

    mexu->procCb = _AmxMpu_ProcCb;
    mexu->workerProc = _AMX_MPU_THREAD_PROC;
    mexu->iorpVmt = &_AMX_MPU_IORP_VMT;
    mexu->pingCb = _AmxMexu_PingCb;

    AfxDeployMutex(&mexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mexu->schedCnd);
    mexu->schedCnt = 0;

    AfxDeployChain(&mexu->classes, mexu);
    afxClassConfig clsCfg;
    clsCfg = cfg->squeClsCfg ? *cfg->squeClsCfg : _AMX_MQUE_CLASS_CONFIG;
    AfxMountClass(&mexu->mqueCls, NIL, &mexu->classes, &clsCfg);

    afxClass* squeCls = (afxClass*)_AmxGetMixQueueClass(mexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_MQUE);
    afxDrawQueue queues[AMX_MAX_QUEUES_PER_BRIDGE];
    AFX_ASSERT(AMX_MAX_QUEUES_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(squeCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { mexu, cfg }))
    {
        AfxThrowError();
    }
    
    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = SoundThreadProc;
        thrCfg.usage = afxThreadUsage_MIX;
        thrCfg.udd[0] = mexu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &mexu->worker))
        {
            AfxThrowError();
        }

        AfxDeployChain(&mexu->activeMixers, mexu);
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
    .name = "MixBridge",
    .desc = "Mix Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixBridge)),
    .ctor = (void*)_AmxMexuCtorCb,
    .dtor = (void*)_AmxMexuDtorCb
};
