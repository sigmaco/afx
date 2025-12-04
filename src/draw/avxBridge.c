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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "avxIcd.h"

_AVX afxClass const* _AvxDexuGetDqueClass(afxDrawBridge dexu)
{
    afxError err = { 0 };
    // dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxClass const* cls = &dexu->dqueCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DQUE);
    return cls;
}

_AVX afxDrawSystem AvxGetBridgedDrawSystem(afxDrawBridge dexu, afxUnit* bridgeId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxDrawSystem dsys = AfxGetHost(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (bridgeId) *bridgeId = AfxGetObjectId(dexu);
    return dsys;
}

_AVX afxDrawDevice AvxGetBridgedDrawDevice(afxDrawBridge dexu, afxUnit* ddevId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    afxDrawDevice ddev = dexu->ddev;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    if (ddevId) *ddevId = AfxGetObjectId(ddev);
    return ddev;
}

_AVX afxUnit AvxGetDrawQueues(afxDrawBridge dexu, afxUnit baseQueIdx, afxUnit cnt, afxDrawQueue queues[])
{
    afxError err = { 0 };
    // dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    // queues must be a valid pointer to the afxDrawQueue handles.
    AFX_ASSERT(queues);

    afxClass const* dqueCls = _AvxDexuGetDqueClass(dexu);
    AFX_ASSERT_CLASS(dqueCls, afxFcc_DQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(dqueCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, rslt, queues);
    return rslt;
}

_AVX afxError AvxWaitForIdleDrawBridge(afxDrawBridge dexu, afxUnit64 timeout)
{
    afxError err = { 0 };
    // dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    if (dexu->ddi->pingCb)
        dexu->ddi->pingCb(dexu, 0);

    if (!dexu->ddi->waitCb)
    {
        afxClass const* dqueCls = _AvxDexuGetDqueClass(dexu);

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
    }
    else if (dexu->ddi->waitCb(dexu, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDexuTransferVideoMemory(afxDrawBridge dexu, avxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxBool queued = FALSE;
    afxClass const* dqueCls = _AvxDexuGetDqueClass(dexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);

    while (1)
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxDrawQueue dque;
            if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque))
            {
                AfxThrowError();
                continue; // for
            }

            afxError err2 = _AvxDqueTransferResources(dque, ctrl, opCnt, ops);

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

_AVX afxError _AvxDexuRemapBuffers(afxDrawBridge dexu, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxDrawQueue dque;
    afxUnit queIdx = 0;
    while (AvxGetDrawQueues(dexu, queIdx++, 1, &dque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (unmap)
        {
            afxError err2 = _AvxDqueRemapBuffers(dque, 0, NIL, cnt, maps);
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
            afxError err2 = _AvxDqueRemapBuffers(dque, cnt, maps, 0, NIL);
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
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
        // Wait for the draw queue to finish the operation, otherwise we will have not pointers.
        if (AvxWaitForEmptyDrawQueue(dque, AFX_TIMEOUT_INFINITE))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDexuCohereMappedBuffers(afxDrawBridge dexu, afxBool invalidate, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxDrawQueue dque;
    afxUnit queIdx = 0;
    while (AvxGetDrawQueues(dexu, queIdx++, 1, &dque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (invalidate)
        {
            afxError err2 = _AvxDqueCohereMappedBuffers(dque, 0, NIL, cnt, maps);
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
            afxError err2 = _AvxDqueCohereMappedBuffers(dque, cnt, maps, 0, NIL);
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

_AVX afxError _AvxDexuResetQueries(afxDrawBridge dexu, avxQueryPool qryp, afxUnit base, afxUnit cnt)
{
    afxError err = { 0 };
    // @dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT(cnt);

    afxBool queued = FALSE;
    afxDrawQueue dque;
    afxUnit queIdx = 0;
    while (AvxGetDrawQueues(dexu, queIdx++, 1, &dque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        afxError err2 = _AvxDqueResetQueries(dque, qryp, base, cnt);
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

    if (queued)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
        // Wait for the draw queue to finish the operation, otherwise we will have not pointers.
        if (AvxWaitForEmptyDrawQueue(dque, AFX_TIMEOUT_INFINITE))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDexuQueryResults(afxDrawBridge dexu, avxQueryPool qryp, avxQueryResultFlags flags, afxUnit base, afxUnit cnt, afxSize dstCap, void* dst, afxSize stride)
{
    afxError err = { 0 };
    // @dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT(cnt);
    AFX_ASSERT(dstCap);
    AFX_ASSERT(dst);
    AFX_ASSERT(stride);

    afxBool queued = FALSE;
    afxDrawQueue dque;
    afxUnit queIdx = 0;
    while (AvxGetDrawQueues(dexu, queIdx++, 1, &dque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        afxError err2 = _AvxDqueQueryResults(dque, qryp, flags, base, cnt, dstCap, dst, stride);
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

    if (queued)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
        // Wait for the draw queue to finish the operation, otherwise we will have not pointers.
        if (AvxWaitForEmptyDrawQueue(dque, AFX_TIMEOUT_INFINITE))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDexuPresentBuffers(afxDrawBridge dexu, afxUnit cnt, avxPresentation presentations[])
{
    afxError err = { 0 };
    // @dexu must be a valid afxDrawBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(presentations);

    afxDrawQueue dque;
    afxUnit queIdx = 0;
    afxBool queued = FALSE;
    while (AvxGetDrawQueues(dexu, queIdx++, 1, &dque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        afxError err2 = _AvxDquePresentBuffers(dque, cnt, presentations);

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

_AVX _avxDdiDexu const _AVX_DDI_DEXU =
{
    .pingCb = _AvxDexu_PingCb
};

_AVX afxError _AvxDexuDtorCb(afxDrawBridge dexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AvxGetBridgedDrawSystem(dexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE);
    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE); // yes, two times.

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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AFX_CAST(afxDrawSystem, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    _avxDexuAcq const* cfg = AFX_CAST(_avxDexuAcq const*, args[1]) + invokeNo;
    
    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    dexu->ddev = cfg->ddev;
    //AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    //dexu->portId = cfg->portId;
    dexu->exuIdx = cfg->exuIdx;
    AFX_ASSERT(dexu->exuIdx == AfxGetObjectId(dexu));

    dexu->ddi = &_AVX_DDI_DEXU;
    dexu->workerProc = _AVX_DPU_THREAD_PROC;
    dexu->procCb = _AvxDpu_ProcCb;

    dexu->iorpVmt = &_AVX_DPU_IORP_VMT;
    dexu->cmdVmt = &_AVX_DPU_CMD_VMT;

    AfxDeployMutex(&dexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dexu->schedCnd);
    dexu->schedCnt = 0;

    AfxMakeChain(&dexu->classes, dexu);

    afxClassConfig dqueClsCfg = cfg->dqueClsCfg ? *cfg->dqueClsCfg : _AVX_CLASS_CONFIG_DQUE;
    AFX_ASSERT(dqueClsCfg.fcc == afxFcc_DQUE);
    AfxMountClass(&dexu->dqueCls, NIL, &dexu->classes, &dqueClsCfg);

    afxClass* dqueCls = (afxClass*)_AvxDexuGetDqueClass(dexu);
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

_AVX afxClassConfig const _AVX_CLASS_CONFIG_DEXU =
{
    .fcc = afxFcc_DEXU,
    .name = "DrawBridge",
    .desc = "Draw Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawBridge)),
    .ctor = (void*)_AvxDexuCtorCb,
    .dtor = (void*)_AvxDexuDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxDsysBridgeDevices(afxDrawSystem dsys, afxUnit cnt, _avxDexuAcq const configs[], afxDrawBridge bridges[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(configs);
    AFX_ASSERT(bridges);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetDexuClassCb_SW(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bridges, (void const*[]) { dsys, configs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, cnt, bridges);
    }

    return err;
}
