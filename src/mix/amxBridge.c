/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_MIX_QUEUE_C
#define _AMX_MIX_CONTEXT_C
#include "amxIcd.h"

_AMX afxClass const* _AmxMexuGetMqueClass(afxMixBridge mexu)
{
    afxError err = { 0 };
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxClass const* cls = &mexu->mqueCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MQUE);
    return cls;
}

_AMX afxMixSystem AmxGetBridgedMixSystem(afxMixBridge mexu, afxUnit* bridgeId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixSystem msys = AfxGetHost(mexu);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (bridgeId) *bridgeId = AfxGetObjectId(mexu);
    return msys;
}

_AMX afxMixDevice AmxGetBridgedMixDevice(afxMixBridge mexu, afxUnit* mdevId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixDevice mdev = mexu->mdev;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    if (mdevId) *mdevId = AfxGetObjectId(mdev);
    return mdev;
}

_AMX afxUnit AmxGetMixQueues(afxMixBridge mexu, afxUnit baseQueIdx, afxUnit cnt, afxMixQueue queues[])
{
    afxError err = { 0 };
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    // queues must be a valid pointer to the afxMixQueue handles.
    AFX_ASSERT(queues);

    afxClass const* mqueCls = _AmxMexuGetMqueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(mqueCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, rslt, queues);
    return rslt;
}

_AMX afxError AmxWaitForEmptyMixQueue(afxMixBridge mexu, afxUnit queIdx, afxUnit64 timeout)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
    // mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (mexu->pingCb)
        mexu->pingCb(mexu, 0);

    afxClass const* mqueCls = _AmxMexuGetMqueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxUnit queCnt = mqueCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AmxWaitForEmptyMixQueue(mexu, i, timeout);

    return err;
}
#if 0
_AMX afxError _AmxMexuSubmitCallback(afxMixBridge mexu, afxError(*f)(void*, void*), void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(f);

    afxClass const* mqueCls = _AmxMexuGetMqueClass(mexu);

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

_AMX afxError _AmxMexuExecuteMixCommands(afxMixBridge mexu, afxUnit frameCnt, afxUnit cnt, afxMixContext mixers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMixContext mix = mixers[i];
        AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
        
        AfxReacquireObjects(1, &mix);

        mix->queuedFrameCnt = frameCnt;

    }
    return err;
}
#endif

_AMX afxError _AmxMexuTransferMixMemory(afxMixBridge mexu, amxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxBool queued = FALSE;
    afxClass const* mqueCls = _AmxMexuGetMqueClass(mexu);

    // sanitize arguments
    afxUnit totalQueCnt = mqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);

    while (1)
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxMixQueue mque;
            if (!AmxGetMixQueues(mexu, queIdx, 1, &mque))
            {
                AfxThrowError();
                continue; // for
            }

            afxError err2 = _AmxMqueTransferResources(mque, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }

        if (err || queued) break; // while --- reiterate if not queue for timeout?
    }
    return err;
}

_AMX afxError _AmxMexuRemapBuffers(afxMixBridge mexu, afxBool unmap, afxUnit cnt, _amxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxMixQueue mque;
    afxUnit queIdx = 0;
    while (AmxGetMixQueues(mexu, queIdx++, 1, &mque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

        if (unmap)
        {
            afxError err2 = _AmxMqueRemapBuffers(mque, 0, NIL, cnt, maps);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If synchronization fails, throw an error.
            AfxThrowError();
        }
        else
        {
            afxError err2 = _AmxMqueRemapBuffers(mque, cnt, maps, 0, NIL);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If flushing fails, throw an error
            AfxThrowError();
        }
    }

    if (queued)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
        // Wait for the draw queue to finish the operation, otherwise we will have not pointers.
        //if (AmxWaitForEmptyMixQueue(mque, AFX_TIMEOUT_INFINITE))
        if (AmxWaitForEmptyMixQueue(mexu, queIdx - 1, AFX_TIMEOUT_INFINITE))
            AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxMexuCohereMappedBuffers(afxMixBridge mexu, afxBool discard, afxUnit cnt, amxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxMixQueue mque;
    afxUnit queIdx = 0;
    while (AmxGetMixQueues(mexu, queIdx++, 1, &mque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

        if (discard)
        {
            afxError err2 = _AmxMqueCohereMappedBuffers(mque, 0, NIL, cnt, maps);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If synchronization fails, throw an error.
            AfxThrowError();
        }
        else
        {
            afxError err2 = _AmxMqueCohereMappedBuffers(mque, cnt, maps, 0, NIL);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If flushing fails, throw an error
            AfxThrowError();
        }
    }
    return err;
}

_AMX afxError _AmxMexuSinkBuffers(afxMixBridge mexu, afxUnit cnt, amxFlush presentations[])
{
    afxError err = { 0 };
    // @mexu must be a valid afxMixBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(presentations);

    afxMixQueue mque;
    afxUnit queIdx = 0;
    afxBool queued = FALSE;
    while (AmxGetMixQueues(mexu, queIdx++, 1, &mque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

        afxError err2 = _AmxMqueSinkBuffers(mque, cnt, presentations);

        if (!err2)
        {
            queued = TRUE;
            break; // while
        }

        if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
            continue; // while

        err = err2;
        AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxMexuDtorCb(afxMixBridge mexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixSystem msys = AmxGetBridgedMixSystem(mexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE);
    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE); // yes, two times.
    
    AfxDeregisterChainedClasses(&mexu->classes);

    if (mexu->worker)
    {
        while (AfxIsThreadRunning(mexu->worker))
        {
            AfxRequestThreadInterruption(mexu->worker);
        }
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixSystem msys = AFX_CAST(afxMixSystem, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    _amxMexuAcq const* cfg = AFX_CAST(_amxMexuAcq const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    mexu->mdev = cfg->mdev;
    //AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    //mexu->portId = cfg->portId;
    mexu->exuIdx = cfg->exuIdx;

    mexu->procCb = _AmxMpu_ProcCb;
    mexu->workerProc = _AMX_MPU_THREAD_PROC;
    mexu->iorpVmt = &_AMX_MPU_IORP_VMT;
    mexu->pingCb = _AmxMexu_PingCb;

    AfxDeployMutex(&mexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mexu->schedCnd);
    mexu->schedCnt = 0;

    AfxMakeChain(&mexu->classes, mexu);

    afxClassConfig mqueClsCfg = cfg->mqueClsCfg ? *cfg->mqueClsCfg : _AMX_MQUE_CLASS_CONFIG;
    AFX_ASSERT(mqueClsCfg.fcc == afxFcc_MQUE);
    AfxMountClass(&mexu->mqueCls, NIL, &mexu->classes, &mqueClsCfg);

    afxClassConfig mctxClsCfg = cfg->mixClsCfg ? *cfg->mixClsCfg : _AMX_MCTX_CLASS_CONFIG;
    AFX_ASSERT(mctxClsCfg.fcc == afxFcc_MCTX);
    //AfxMountClass(&mexu->mctxCls, NIL, &mexu->classes, &mctxClsCfg);

    afxClass* mqueCls = (afxClass*)_AmxMexuGetMqueClass(mexu);
    AFX_ASSERT_CLASS(mqueCls, afxFcc_MQUE);
    afxDrawQueue queues[AMX_MAX_QUEUES_PER_BRIDGE];
    AFX_ASSERT(AMX_MAX_QUEUES_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(mqueCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { mexu, cfg }))
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

////////////////////////////////////////////////////////////////////////////////

_AMX afxError _AmxMsysBridgeDevices(afxMixSystem msys, afxUnit cnt, _amxMexuAcq const configs[], afxMixBridge bridges[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(configs);
    AFX_ASSERT(bridges);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetMexuClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bridges, (void const*[]) { msys, configs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, cnt, bridges);
    }

    return err;
}
