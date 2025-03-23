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
//#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "../impl/avxImplementation.h"

_AVX afxError _AvxUnlockDrawQueue(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxUnlockMutex(&dque->workChnMtx);
    return err;
}

_AVX afxError _AvxLockDrawQueue(afxDrawQueue dque, afxBool wait, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    
    if (wait)
    {
        if (!AfxTryLockMutex(&dque->workChnMtx))
            return 1;
    }
    else
    {
        if (ts)
            err = AfxLockMutexTimed(&dque->workChnMtx, ts);
        else
            if (AfxLockMutex(&dque->workChnMtx))
                AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDquePopWork(afxDrawQueue dque, avxWork* work)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxPopLink(&work->hdr.chain);
    AfxRecycleArenaUnit(&dque->workArena, work, work->hdr.siz);
    return err;
}

_AVX avxWork* _AvxDquePushWork(afxDrawQueue dque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    avxWork* work = AfxRequestArenaUnit(&dque->workArena, siz);
    AFX_ASSERT(work);
    AfxZero(work, sizeof(work->hdr));
    work->hdr.id = id;
    work->hdr.siz = siz;
    AfxGetTime(&work->hdr.pushTime);
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&work->hdr.chain, &dque->workChn);
    return work;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawSystem AfxGetDrawQueueContext(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    afxDrawSystem dsys = dque->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxUnit AfxGetDrawQueuePort(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    return dque->portId;
}

_AVX afxClass const* _AvxGetDrawBatchClass(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    afxClass const* cls = &dque->cmdbCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
    return cls;
}

_AVX void _AvxBeginDrawQueueDebugScope(afxDrawQueue dque, afxString const* name, avxColor const color)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
}

_AVX void _AvxEndDrawQueueDebugScope(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
}

_AVX void _AvxPushDrawQueueDebugLabel(afxDrawQueue dque, afxString const* name, avxColor const color)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
}

_AVX afxError AvxWaitForEmptyDrawQueue(afxDrawQueue dque, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (dque->waitCb)
    {
        if (dque->waitCb(dque, timeout))
            AfxThrowError();
    }
    else
    {
#if 0
        afxBool again = TRUE;

        while (again)
        {
            afxTimeSpec ts = { 0 };
            if (timeout && (timeout != AFX_TIME_INFINITE))
                AfxMakeTimeSpec(&ts, timeout);

            AfxLockMutex(&dque->idleCndMtx);

            if (dque->workChn.cnt)
            {
                if (timeout == AFX_TIME_INFINITE)
                {
                    AfxWaitCondition(&dque->idleCnd, &dque->idleCndMtx);
                    again = FALSE;
                    break;
                }
                else if (afxError_TIMEOUT == AfxWaitTimedCondition(&dque->idleCnd, &dque->idleCndMtx, &ts))
                    if (timeout != AFX_TIME_INFINITE)
                        break;

                if (!timeout)
                {
                    again = FALSE;
                    break; // do not stall; return immediately.
                }
                else if (timeout == AFX_TIME_INFINITE)
                    continue; // cycle again
            }
            else again = FALSE;

            AfxUnlockMutex(&dque->idleCndMtx);
        }
#else
        if (timeout == AFX_TIME_INFINITE)
        {
            AfxLockMutex(&dque->idleCndMtx);
            
            while (dque->workChn.cnt)
            {
                if (AfxWaitCondition(&dque->idleCnd, &dque->idleCndMtx))
                {
                    AfxThrowError();
                    break;
                }
            }
            AfxUnlockMutex(&dque->idleCndMtx);
        }
        else
        {
            AfxLockMutex(&dque->idleCndMtx);

            afxTimeSpec ts = { 0 };
            if (timeout && (timeout != AFX_TIME_INFINITE))
                AfxMakeTimeSpec(&ts, timeout);

            while (dque->workChn.cnt)
            {
                afxError cndErr = AfxWaitTimedCondition(&dque->idleCnd, &dque->idleCndMtx, &ts);

                if (cndErr)
                {
                    if (cndErr == afxError_TIMEOUT)
                    {
                        err = afxError_TIMEOUT;
                        break;
                    }
                    AfxThrowError();
                    break;
                }

                if (!timeout)
                    break; // if there is not timeout specicied, it can not stall; return imediately.
            }
            AfxUnlockMutex(&dque->idleCndMtx);
        }
#endif
    }
    return err;
}

_AVX afxError _AvxSubmitCallback(afxDrawQueue dque, void(*f)(void*, void*), void* udd)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AFX_ASSERT(f);

    if (!AfxTryLockMutex(&dque->workChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    avxWork* work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(Callback), sizeof(work->Callback), &cmdId);
    AFX_ASSERT(work);

    work->Callback.f = f;
    work->Callback.udd = udd;

    AfxUnlockMutex(&dque->workChnMtx);

    return err;
}

_AVX afxError _AvxExecuteDrawCommands(afxDrawQueue dque, avxSubmission const* ctrl, afxUnit cnt, afxDrawContext contexts[], avxFence fence)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(contexts);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    if (!AfxTryLockMutex(&dque->workChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    avxWork* work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(Execute), sizeof(work->Execute) + (cnt * sizeof(work->Execute.cmdbs[0])), &cmdId);
    AFX_ASSERT(work);

    if (ctrl)
    {
        work->Execute.signal = ctrl->signalSems;
        work->Execute.wait = ctrl->waitSems;
    }
    work->Execute.hdr.completionFence = fence;

    work->Execute.cmdbCnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxDrawContext dctx = contexts[i];
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

        AfxReacquireObjects(1, &dctx);
        work->Execute.cmdbs[i] = dctx;
        AfxIncAtom32(&dctx->submCnt);
        work->Execute.cmdbs[i]->state = avxCmdbState_PENDING;
    }

    AfxUnlockMutex(&dque->workChnMtx);

    return err;
}

_AVX afxError _AvxSubmitTransferences(afxDrawQueue dque, avxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    if (!AfxTryLockMutex(&dque->workChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    avxWork* work;

    if (ctrl->dstFcc == afxFcc_RAS || ctrl->srcFcc == afxFcc_RAS)
    {
        work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(Transfer), sizeof(work->Transfer) + (opCnt * sizeof(work->Transfer.rasOps[0])), &cmdId);
        AFX_ASSERT(work);
    }
    else
    {
        work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(Transfer), sizeof(work->Transfer) + (opCnt * sizeof(work->Transfer.bufOps[0])), &cmdId);
        AFX_ASSERT(work);
    }

    work->Transfer.srcFcc = ctrl->srcFcc;
    work->Transfer.dstFcc = ctrl->dstFcc;
    work->Transfer.codec = ctrl->codec;
    work->Transfer.decSiz = ctrl->decSiz;

    work->Transfer.opCnt = opCnt;

    switch (ctrl->srcFcc)
    {
    case afxFcc_RAS:
    {
        work->Transfer.src.ras = ctrl->src.ras;
        AfxReacquireObjects(1, &work->Transfer.src.ras);
        break;
    }
    case afxFcc_BUF:
    {
        work->Transfer.src.buf = ctrl->src.buf;
        AfxReacquireObjects(1, &work->Transfer.src.buf);
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
    case afxFcc_RAS:
    {
        work->Transfer.dst.ras = ctrl->dst.ras;
        AfxReacquireObjects(1, &work->Transfer.dst.ras);
        break;
    }
    case afxFcc_BUF:
    {
        work->Transfer.dst.buf = ctrl->dst.buf;
        AfxReacquireObjects(1, &work->Transfer.dst.buf);
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

    if (ctrl->dstFcc == afxFcc_RAS || ctrl->srcFcc == afxFcc_RAS)
    {
        avxRasterIo const* riops = ops;
        for (afxUnit i = 0; i < opCnt; i++)
        {
            work->Transfer.rasOps[i] = riops[i];
        }
        int a = 1;
    }
    else
    {
        avxBufferIo const* biops = ops;
        for (afxUnit i = 0; i < opCnt; i++)
        {
            AFX_ASSERT((ctrl->srcFcc == afxFcc_BUF || ctrl->dstFcc == afxFcc_BUF));
            work->Transfer.bufOps[i] = biops[i];
        }
        int a = 1;
    }

    AfxUnlockMutex(&dque->workChnMtx);

    return err;
}

_AVX afxError _AvxSubmitRemapping(afxDrawQueue dque, afxUnit mapCnt, avxBufferRemap const maps[], afxUnit unmapCnt, avxBuffer const unmaps[])
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxLockMutex(&dque->workChnMtx))
        return afxError_TIMEOUT;

    if (mapCnt)
    {
        afxCmdId cmdId;
        avxWork* work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(Remap), sizeof(work->Remap) + (mapCnt * sizeof(work->Remap.mapOps[0])), &cmdId);
        AFX_ASSERT(work);

        work->Remap.mapCnt = mapCnt;
        work->Remap.unmapCnt = 0;

        for (afxUnit i = 0; i < mapCnt; i++)
        {
            avxBufferRemap const* map = &maps[i];

            work->Remap.mapOps[i].buf = map->buf;
            work->Remap.mapOps[i].offset = map->offset;
            work->Remap.mapOps[i].range = map->range;
            work->Remap.mapOps[i].flags = map->flags;
            work->Remap.mapOps[i].placeholder = map->placeholder;

            AfxReacquireObjects(1, &map->buf);
            AfxIncAtom32(&map->buf->pendingRemap);
        }
    }

    if (unmapCnt)
    {
        afxCmdId cmdId;
        avxWork* work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(Remap), sizeof(work->Remap) + (unmapCnt * sizeof(work->Remap.unmapOps[0])), &cmdId);
        AFX_ASSERT(work);

        work->Remap.mapCnt = 0;
        work->Remap.unmapCnt = unmapCnt;

        for (afxUnit i = 0; i < unmapCnt; i++)
        {
            avxBuffer buf = unmaps[i];

            work->Remap.unmapOps[i] = buf;
            
            AfxReacquireObjects(1, &buf);
            AfxIncAtom32(&buf->pendingRemap);
        }
    }

    AfxUnlockMutex(&dque->workChnMtx);

    return err;
}

_AVX afxError _AvxSubmitSyncMaps(afxDrawQueue dque, afxUnit flushCnt, avxBufferedMap const flushes[], afxUnit fetchCnt, avxBufferedMap const fetches[])
{
    afxError err = AFX_ERR_NONE;
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxLockMutex(&dque->workChnMtx))
        return afxError_TIMEOUT;

    if (flushCnt)
    {
        afxCmdId cmdId;
        avxWork* work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(SyncMaps), sizeof(work->SyncMaps) + (flushCnt * sizeof(work->SyncMaps.ops[0])), &cmdId);
        AFX_ASSERT(work);

        work->SyncMaps.baseFetchIdx = 0;
        work->SyncMaps.fetchCnt = 0;
        work->SyncMaps.baseFlushIdx = 0;
        work->SyncMaps.flushCnt = flushCnt;

        for (afxUnit i = 0; i < flushCnt; i++)
        {
            avxBufferedMap const* map = &flushes[i];

            work->SyncMaps.ops[i].buf = map->buf;
            work->SyncMaps.ops[i].offset = map->offset;
            work->SyncMaps.ops[i].range = map->range;

            AfxReacquireObjects(1, &map->buf);
        }
    }

    if (fetchCnt)
    {
        afxCmdId cmdId;
        avxWork* work = _AvxDquePushWork(dque, AVX_GET_STD_WORK_ID(SyncMaps), sizeof(work->SyncMaps) + (fetchCnt * sizeof(work->SyncMaps.ops[0])), &cmdId);
        AFX_ASSERT(work);

        work->SyncMaps.baseFetchIdx = 0;
        work->SyncMaps.flushCnt = 0;
        work->SyncMaps.baseFlushIdx = 0;
        work->SyncMaps.fetchCnt = fetchCnt;

        for (afxUnit i = 0; i < fetchCnt; i++)
        {
            avxBufferedMap const* map = &fetches[i];

            work->SyncMaps.ops[i].buf = map->buf;
            work->SyncMaps.ops[i].offset = map->offset;
            work->SyncMaps.ops[i].range = map->range;

            AfxReacquireObjects(1, &map->buf);
        }
    }

    AfxUnlockMutex(&dque->workChnMtx);

    return err;
}

_AVX afxError _AvxDqueDtorCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    AfxDeregisterChainedClasses(&dque->classes);

    AfxDismountClass(&dque->cmdbCls);
    AfxDismantleSlock(&dque->cmdbReqLock);
    AfxDismantleQueue(&dque->cmdbRecycQue);

    AfxDismantleMutex(&dque->workChnMtx);
    AfxDismantleArena(&dque->workArena);
    AfxDismantleSlock(&dque->workArenaSlock);
    AfxDismantleCondition(&dque->idleCnd);
    AfxDismantleMutex(&dque->idleCndMtx);

    return err;
}

_AVX afxError _AvxDqueCtorCb(afxDrawQueue dque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    afxDrawBridge dexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _avxDexuAcquisition const* cfg = AFX_CAST(_avxDexuAcquisition const*, args[1]);
    AFX_ASSERT(cfg);
    
    AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    dque->ddev = cfg->ddev;
    dque->portId = cfg->portId;
    dque->exuIdx = cfg->exuIdx;
    dque->dsys = AfxGetBridgedDrawSystem(dexu);

    dque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&dque->workArenaSlock);
    AfxDeployArena(&dque->workArena, NIL, AfxHere());

    AfxDeployMutex(&dque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&dque->workChn, dque);
    AfxDeployMutex(&dque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dque->idleCnd);

    dque->closed = FALSE;

    AfxDeployChain(&dque->classes, (void*)dque);

    dque->cmdbLockedForReq = FALSE;
    AfxDeploySlock(&dque->cmdbReqLock);
    AfxMakeQueue(&dque->cmdbRecycQue, sizeof(afxDrawContext), 3, NIL, 0);

    afxClassConfig tmpClsCfg = { 0 };
    tmpClsCfg = cfg->dctxClsCfg ? *cfg->dctxClsCfg : _AVX_DCTX_CLASS_CONFIG;
    AFX_ASSERT(tmpClsCfg.fcc == afxFcc_DCTX);
    AfxMountClass(&dque->cmdbCls, NIL, &dque->classes, &tmpClsCfg);

    return err;
}

_AVX afxClassConfig const _AVX_DQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_DQUE,
    .name = "DrawQueue",
    .desc = "Draw Bridge Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawQueue)),
    .ctor = (void*)_AvxDqueCtorCb,
    .dtor = (void*)_AvxDqueDtorCb
};
