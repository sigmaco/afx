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
#define _AVX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "../avxIcd.h"

#define _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC TRUE

_AVX afxError _AvxDpuWork_CallbackCb(avxDpu* dpu, _avxIoReqPacket* work)
{
    afxError err = { 0 };
    return work->Callback.f(dpu, work->Callback.udd);
}

_AVX afxError _AvxDpuWork_ExecuteCb(avxDpu* dpu, _avxIoReqPacket* work)
{
    afxError err = { 0 };
    afxUnit cnt = work->Execute.cmdbCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxDrawContext dctx = work->Execute.cmdbs[i].dctx;
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
        afxUnit batchId = dctx->batchId;
        _avxCmdBatch* cmdb = _AvxDctxGetCmdBatch(dctx, batchId);

        if (!cmdb)
        {
            AfxThrowError();
            return err;
        }

        //AFX_ASSERT(cmdb->state == avxDrawContextState_PENDING);
        _AvxDpuRollContext(dpu, dctx, batchId);
        
        // Must be disposed because AvxSubmitDrawCommands() reacquires it.
        AfxDecAtom32(&cmdb->submCnt);
        AvxRecycleDrawCommands(dctx, /*batchId,*/ FALSE);
#if 0
        AFX_ASSERT(!AvxDoesDrawCommandsExist_(dctx, batchId));
        AfxReportf(0, AfxHere(),"%d dpu %d", batchId, dpu->exuIdx);
#endif
        AfxDisposeObjects(1, &dctx);
    }
    return err;
}

_AVX _avxIoReqLut const _AVX_DPU_IORP_VMT =
{
    .Callback = _AvxDpuWork_CallbackCb,
    .Execute = _AvxDpuWork_ExecuteCb,
};

_AVX afxBool _PullDque(afxDrawQueue dque, avxDpu* dpu)
{
    afxError err = { 0 };
    AFX_ASSERT(dpu);
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    afxDrawBridge dexu = dpu->dexu;
    _avxIoReqLut const* iorpVmt = dexu->iorpVmt;

    if (dque->exuIdx != dexu->exuIdx)
        return FALSE;

    if (AfxTryLockMutex(&dque->iorpChnMtx))
    {
        _avxIoReqPacket* iorp;
        AFX_ITERATE_CHAIN_B2F(iorp, hdr.chain, &dque->iorpChn)
        {
            AFX_ASSERT(dque->iorpChn.cnt);
            AfxGetClock(&iorp->hdr.pullTime);

            AFX_ASSERT(iorpVmt->f[iorp->hdr.id]);
            if (afxError_TIMEOUT == iorpVmt->f[iorp->hdr.id](dpu, iorp))
                continue;

            AfxGetClock(&iorp->hdr.complTime);
            _AvxDquePopIoReqPacket(dque, iorp);
        }
        AfxSignalCondition(&dque->idleCnd);
        AfxUnlockMutex(&dque->iorpChnMtx);
        //AfxYield();
    }
    return TRUE;
}

_AVX afxBool _AvxDpu_ProcCb(avxDpu* dpu)
{
    afxError err = { 0 };
    AFX_ASSERT(dpu);

    afxDrawBridge dexu = dpu->dexu;
    _avxIoReqLut const* iorpVmt = dexu->iorpVmt;

    afxClass const* dqueCls = _AvxDexuGetDqueClass(dexu);

    //AfxInvokeObjects(dqueCls, 0, AFX_U32_MAX, (void*)_PullDque, dpu);

    afxUnit totalQueCnt = dqueCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < totalQueCnt; queIdx++)
    {
        afxDrawQueue dque;
        if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque) || !dque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (dque->exuIdx != dexu->exuIdx)
            continue;

        if (AfxTryLockMutex(&dque->iorpChnMtx))
        {
            _avxIoReqPacket* iorp;
            AFX_ITERATE_CHAIN_B2F(iorp, hdr.chain, &dque->iorpChn)
            {
                AFX_ASSERT(dque->iorpChn.cnt);
                AfxGetClock(&iorp->hdr.pullTime);

                AFX_ASSERT(iorpVmt->f[iorp->hdr.id]);
                if (afxError_TIMEOUT == iorpVmt->f[iorp->hdr.id](dpu, iorp))
                    continue;

                AfxGetClock(&iorp->hdr.complTime);
                _AvxDquePopIoReqPacket(dque, iorp);
            }
            AfxSignalCondition(&dque->idleCnd);
            AfxUnlockMutex(&dque->iorpChnMtx);
            //AfxYield();
        }
    }
    return TRUE;
}

_AVX afxInt _AVX_DPU_THREAD_PROC(afxDrawBridge dexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == dexu->worker);

    afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
    afxDrawSystem dsys = AvxGetBridgedDrawSystem(dexu, NIL);
    afxUnit portId = dexu->exuIdx;

    avxDpu* dpu;
    if (AfxAllocate(sizeof(*dpu), 0, AfxHere(), (void**)&dpu))
        AfxThrowError();

    AfxZero(dpu, sizeof(*dpu));
    dpu->dexu = dexu;
    dpu->dsys = dsys;
    dpu->exuIdx = portId;
    dpu->running = TRUE;
    dpu->thr = thr;
    dexu->dpu = dpu;

    while (1)
    {
        AfxLockMutex(&dexu->schedCndMtx);

        while (!dexu->schedCnt)
            AfxWaitCondition(&dexu->schedCnd, &dexu->schedCndMtx);

        dexu->schedCnt = 0;
        AfxUnlockMutex(&dexu->schedCndMtx);

        dexu->procCb(dpu);

        if (AfxShouldThreadBeInterrupted())
            break;

#ifdef _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC
        AfxSleep(1);
#else
        AfxYield();
#endif//_YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC
    }

    AFX_ASSERT(dpu == dexu->dpu);
    AfxDeallocate((void**)&dpu, AfxHere());

    return 0;
}

_AVX afxError _AvxDexu_PingCb(afxDrawBridge dexu, afxUnit queIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
#if 0
    if (!AfxTryLockMutex(&dexu->schedCndMtx)) err = afxError_BUSY;
    else
#else
    AfxLockMutex(&dexu->schedCndMtx);
#endif
    {
        dexu->schedCnt += 1;
        AfxSignalCondition(&dexu->schedCnd);
        AfxUnlockMutex(&dexu->schedCndMtx);
    }
    return err;
}
