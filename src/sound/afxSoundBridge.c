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
#define _ASX_MIXER_C
#include "impl/asxImplementation.h"

_ASX afxSoundDevice AfxGetSoundBridgeDevice(afxSoundBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxSoundDevice sdev = sexu->sdev;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev;
}

_ASX afxSoundSystem AfxGetSoundBridgeSystem(afxSoundBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxSoundSystem ssys = AfxGetProvider(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    return ssys;
}

_ASX afxUnit AfxQuerySoundBridgePort(afxSoundBridge sexu, afxSoundDevice* device)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (device)
    {
        afxSoundDevice sdev = sexu->sdev;
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
        *device = sdev;
    }
    return sexu->portId;
}

_ASX afxClass const* _AsxGetSoundQueueClass(afxSoundBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSoundBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxClass const* cls = &sexu->squeCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SQUE);
    return cls;
}

_ASX afxUnit AfxGetSoundQueues(afxSoundBridge sexu, afxUnit first, afxUnit cnt, afxSoundQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSoundBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    /// queues must be a valid pointer to the afxSoundQueue handles.
    AFX_ASSERT(queues);

    afxClass const* squeCls = _AsxGetSoundQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(squeCls, FALSE, first, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, rslt, queues);
    return rslt;
}

_ASX afxError AfxWaitForEmptySoundQueue(afxSoundBridge sexu, afxUnit queIdx, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSoundBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSoundQueue sque;
    if (!AfxGetSoundQueues(sexu, queIdx, 1, &sque))
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

_ASX afxError AfxWaitForIdleSoundBridge(afxSoundBridge sexu, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// sexu must be a valid afxSoundBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (sexu->pingCb)
        sexu->pingCb(sexu, 0);

    afxClass const* squeCls = _AsxGetSoundQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AfxWaitForEmptySoundQueue(sexu, i, timeout);

    return err;
}

_ASX afxError _AsxSubmitCallback(afxSoundBridge sexu, void(*f)(void*, void*), void* udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AFX_ASSERT(f);

    afxClass const* squeCls = _AsxGetSoundQueueClass(sexu);

    // sanitize arguments
    afxUnit totalQueCnt = squeCls->instCnt;
    afxUnit baseQueIdx = AfxMin(0, totalQueCnt - 1);
    afxUnit queCnt = AfxClamp(1, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxSoundQueue sque;
        if (!AfxGetSoundQueues(sexu, baseQueIdx + queIdx, 1, &sque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
            if (!AfxTryLockMutex(&sque->workChnMtx))
                continue;
        }

        afxCmdId cmdId;
        asxWork* work = _AsxSquePushWork(sque, ASX_GET_STD_WORK_ID(Callback), sizeof(work->Callback), &cmdId);
        AFX_ASSERT(work);

        work->Callback.f = f;
        work->Callback.udd = udd;

        AfxUnlockMutex(&sque->workChnMtx);

        sexu->pingCb(sexu, 0);
        break;
    }
    return err;
}

_ASX afxError _AsxSubmitTransferences(afxSoundBridge sexu, asxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxClass const* squeCls = _AsxGetSoundQueueClass(sexu);

    // sanitize arguments
    afxUnit totalQueCnt = squeCls->instCnt;
    afxUnit baseQueIdx = AfxMin(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AfxClamp(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxSoundQueue sque;
        if (!AfxGetSoundQueues(sexu, baseQueIdx + queIdx, 1, &sque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
            if (!AfxTryLockMutex(&sque->workChnMtx))
                continue;
        }

        afxCmdId cmdId;
        asxWork* work = _AsxSquePushWork(sque, ASX_GET_STD_WORK_ID(Transfer), sizeof(work->Transfer) + (opCnt * sizeof(work->Transfer.wavOps[0])), &cmdId);
        AFX_ASSERT(work);

        work->Transfer.srcFcc = ctrl->srcFcc;
        work->Transfer.dstFcc = ctrl->dstFcc;
        work->Transfer.codec = ctrl->codec;
        work->Transfer.decSiz = ctrl->decSiz;

        work->Transfer.opCnt = opCnt;

        switch (ctrl->srcFcc)
        {
        case afxFcc_WAV:
        {
            work->Transfer.src.wav = ctrl->src.wav;
            AfxReacquireObjects(1, &work->Transfer.src.wav);
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
        case afxFcc_WAV:
        {
            work->Transfer.dst.wav = ctrl->dst.wav;
            AfxReacquireObjects(1, &work->Transfer.dst.wav);
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

        afxWaveIo const* riops = ops;
        for (afxUnit i = 0; i < opCnt; i++)
        {
            work->Transfer.wavOps[i] = riops[i];
        }
        int a = 1;

        AfxUnlockMutex(&sque->workChnMtx);

        sexu->pingCb(sexu, 0);
        break;
    }
    return err;
}

_ASX afxError _AsxSubmitSink(afxSoundBridge sexu, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AFX_ASSERT(sink);
    AFX_ASSERT(seg);
    AFX_ASSERT(aud);

    afxClass const* squeCls = _AsxGetSoundQueueClass(sexu);

    // sanitize arguments
    afxUnit baseQueIdx = 0;
    afxUnit queCnt = squeCls->instCnt;
    AFX_ASSERT(queCnt);

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxSoundQueue sque;
        if (!AfxGetSoundQueues(sexu, baseQueIdx + queIdx, 1, &sque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
            if (!AfxTryLockMutex(&sque->workChnMtx))
                continue;
        }

        afxCmdId cmdId;
        asxWork* work = _AsxSquePushWork(sque, ASX_GET_STD_WORK_ID(Sink), sizeof(work->Sink), &cmdId);
        AFX_ASSERT(work);

        work->Sink.buf = aud;
        work->Sink.sink = sink;
        work->Sink.seg = *seg;

        AfxUnlockMutex(&sque->workChnMtx);

        sexu->pingCb(sexu, 0);
        break;
    }
    return err;
}

_ASX afxError _AsxRollMixers(afxSoundBridge sexu, afxReal startTime, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixer mixers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMixer mix = mixers[i];
        AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
        
        AfxReacquireObjects(1, &mix);

        if (mix->executor.chain)
            AfxPopLink(&mix->executor);
        
        AfxPushLink(&mix->executor, &sexu->activeMixers);
        mix->state = asxMixState_PENDING;
        mix->motor.localClock = startTime;
        mix->motor.localDur = dur;
        mix->motor.iterCnt = iterCnt;
    }
    return err;
}

_ASX afxError _AsxSexuDtorCb(afxSoundBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSoundSystem ssys = AfxGetSoundBridgeSystem(sexu);

    AfxWaitForSoundSystem(ssys, AFX_TIME_INFINITE);
    AfxWaitForSoundSystem(ssys, AFX_TIME_INFINITE); // yes, two times.
    
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

_ASX afxError _AsxSexuCtorCb(afxSoundBridge sexu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxSoundSystem ssys = AFX_CAST(afxSoundSystem, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    _asxSoundBridgeAcquisition const* cfg = AFX_CAST(_asxSoundBridgeAcquisition const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    sexu->sdev = cfg->sdev;
    AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    sexu->portId = cfg->portId;
    sexu->exuIdx = cfg->exuIdx;

    sexu->procCb = _AsxSpu_ProcCb;
    sexu->workerProc = _ASX_SPU_THREAD_PROC;
    sexu->workVmt = &_ASX_SPU_WORK_VMT;
    sexu->pingCb = _AsxSexu_PingCb;

    AfxDeployMutex(&sexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sexu->schedCnd);
    sexu->schedCnt = 0;

    AfxDeployChain(&sexu->classes, sexu);
    afxClassConfig clsCfg;
    clsCfg = cfg->squeClsCfg ? *cfg->squeClsCfg : _ASX_SQUE_CLASS_CONFIG;
    AfxMountClass(&sexu->squeCls, NIL, &sexu->classes, &clsCfg);

    afxClass* squeCls = (afxClass*)_AsxGetSoundQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxDrawQueue queues[AFX_MAX_SOUND_QUEUE_PER_BRIDGE];
    AFX_ASSERT(AFX_MAX_SOUND_QUEUE_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(squeCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { sexu, cfg }))
    {
        AfxThrowError();
    }
    
    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = SoundThreadProc;
        thrCfg.purpose = afxThreadPurpose_SOUND;
        thrCfg.udd[0] = sexu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &sexu->worker))
        {
            AfxThrowError();
        }

        AfxDeployChain(&sexu->activeMixers, sexu);
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
    .name = "SoundBridge",
    .desc = "Sound Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundBridge)),
    .ctor = (void*)_AsxSexuCtorCb,
    .dtor = (void*)_AsxSexuDtorCb
};
