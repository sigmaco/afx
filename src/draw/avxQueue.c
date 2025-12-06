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
//#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "avxIcd.h"

_AVX afxError _AvxDqueUnlockIoReqChain(afxDrawQueue dque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxUnlockMutex(&dque->iorpChnMtx);
    return err;
}

_AVX afxError _AvxDqueLockIoReqChain(afxDrawQueue dque, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    
    if (timeout == 0)
    {
        if (!AfxTryLockMutex(&dque->iorpChnMtx))
            return afxError_TIMEOUT;
    }
    else
    {
        afxTimeSpec ts;
        AfxMakeTimeSpec(&ts, timeout);

        if (afxError_TIMEOUT == AfxLockMutexTimed(&dque->iorpChnMtx, &ts))
            return afxError_TIMEOUT;
        else
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDquePopIoReqPacket(afxDrawQueue dque, _avxIoReqPacket* iorp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxPopLink(&iorp->hdr.chain);
    AfxReclaimArena(&dque->iorpArena, iorp, iorp->hdr.siz);
    return err;
}

_AVX afxError _AvxDquePushIoReqPacket(afxDrawQueue dque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _avxIoReqPacket** pIorp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    _avxIoReqPacket* iorp = AfxRequestArena(&dque->iorpArena, siz, 1, NIL, 0);
    
    if (!iorp) AfxThrowError();
    else
    {
        AfxZero(iorp, sizeof(iorp->hdr));
        iorp->hdr.id = id;
        iorp->hdr.siz = siz;
        AfxGetClock(&iorp->hdr.pushTime);
        AFX_ASSERT(cmdId);
        *cmdId = AfxPushLink(&iorp->hdr.chain, &dque->iorpChn);
        AFX_ASSERT(pIorp);
        *pIorp = iorp;
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawSystem AvxGetDrawQueueDock(afxDrawQueue dque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    afxDrawSystem dsys = dque->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX void _AvxBeginDrawQueueDebugScope(afxDrawQueue dque, afxString const* name, avxColor const color)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxThrowError();
}

_AVX void _AvxEndDrawQueueDebugScope(afxDrawQueue dque)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxThrowError();
}

_AVX void _AvxPushDrawQueueDebugLabel(afxDrawQueue dque, afxString const* name, avxColor const color)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AfxThrowError();
}

_AVX afxError AvxWaitForEmptyDrawQueue(afxDrawQueue dque, afxUnit64 timeout)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (dque->ddi->waitCb)
    {
        if (dque->ddi->waitCb(dque, timeout))
            AfxThrowError();
    }
    else
    {
#if 0
        afxBool again = TRUE;

        while (again)
        {
            afxTimeSpec ts = { 0 };
            if (timeout && (timeout != AFX_TIMEOUT_INFINITE))
                AfxMakeTimeSpec(&ts, timeout);

            AfxLockMutex(&dque->idleCndMtx);

            if (dque->iorpChn.cnt)
            {
                if (timeout == AFX_TIMEOUT_INFINITE)
                {
                    AfxWaitCondition(&dque->idleCnd, &dque->idleCndMtx);
                    again = FALSE;
                    break;
                }
                else if (afxError_TIMEOUT == AfxWaitTimedCondition(&dque->idleCnd, &dque->idleCndMtx, &ts))
                    if (timeout != AFX_TIMEOUT_INFINITE)
                        break;

                if (!timeout)
                {
                    again = FALSE;
                    break; // do not stall; return immediately.
                }
                else if (timeout == AFX_TIMEOUT_INFINITE)
                    continue; // cycle again
            }
            else again = FALSE;

            AfxUnlockMutex(&dque->idleCndMtx);
        }
#else
        if (!timeout)
        {
            if (!AfxTryLockMutex(&dque->iorpChnMtx)) err = afxError_TIMEOUT;
            else
            {
                while (!AfxIsChainEmpty(&dque->iorpChn))
                {
                    err = afxError_TIMEOUT;
                    break; // while
                }

                if (!err)
                {
                    AFX_ASSERT(AfxIsChainEmpty(&dque->iorpChn));
                    AFX_ASSERT(dque->iorpChn.cnt == 0);
                }

                AfxUnlockMutex(&dque->iorpChnMtx);
            }
        }
        else if ((timeout == AFX_TIMEOUT_INFINITE))
        {
            if (AfxLockMutex(&dque->iorpChnMtx))
                AfxThrowError();

            while (!AfxIsChainEmpty(&dque->iorpChn))
            {
                if (AfxWaitCondition(&dque->idleCnd, &dque->iorpChnMtx))
                {
                    AfxThrowError();
                    break;
                }
            }

            AFX_ASSERT(AfxIsChainEmpty(&dque->iorpChn));
            AFX_ASSERT(dque->iorpChn.cnt == 0);

            AfxUnlockMutex(&dque->iorpChnMtx);
        }
        else
        {
            // TODO: timed?
            AfxLockMutex(&dque->iorpChnMtx);

            afxTimeSpec ts = { 0 };
            AfxMakeTimeSpec(&ts, timeout);

            while (!AfxIsChainEmpty(&dque->iorpChn))
            {
                afxError cndErr = AfxWaitTimedCondition(&dque->idleCnd, &dque->iorpChnMtx, &ts);

                if (cndErr)
                {
                    if (cndErr == afxError_TIMEOUT)
                        err = afxError_TIMEOUT;
                    else AfxThrowError();

                    break;
                }
            }

            if (!err)
            {
                AFX_ASSERT(AfxIsChainEmpty(&dque->iorpChn));
            }

            AfxUnlockMutex(&dque->iorpChnMtx);
        }
#endif
    }
    return err;
}

_AVX afxError _AvxDqueSubmitCallback(afxDrawQueue dque, afxError(*f)(void*, void*), void* udd)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AFX_ASSERT(f);

    if (!AfxTryLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _avxIoReqPacket* iorp;
    
    if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Callback), sizeof(iorp->Callback), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        iorp->Callback.f = f;
        iorp->Callback.udd = udd;
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX afxError AvxSubmitDrawCommands(afxDrawQueue dque, afxUnit cnt, avxSubmission subms[])
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(subms);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    if (!AfxTryLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _avxIoReqPacket* iorp;

    if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Execute), sizeof(iorp->Execute) + (cnt * sizeof(iorp->Execute.cmdbs[0])), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        for (afxUnit i = 0; i < cnt; i++)
        {
            iorp->Execute.cmdbCnt = 1;

            afxDrawContext dctx = subms[i].dctx;
            AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
            AfxReacquireObjects(1, &dctx);
            AFX_ASSERT(!(dctx->cmdFlags & avxCmdFlag_DEFERRED));
            
            if (dctx->state != avxContextState_INTERNAL_EXECUTING)
            {
                if (dctx->state = avxContextState_EXECUTABLE)
                    dctx->state = avxContextState_PENDING;
            }
            else
            {
                AFX_ASSERT(dctx->cmdFlags & avxCmdFlag_SHARED);
            }

            iorp->Execute.cmdbs[i].dctx = dctx;

            iorp->Execute.cmdbs[i].wait = subms[i].wait;
            iorp->Execute.cmdbs[i].waitValue = subms[i].waitValue;
            iorp->Execute.cmdbs[i].waitReserved = subms[i].waitReserved;
            iorp->Execute.cmdbs[i].waitStageMask = subms[i].waitStageMask;
            iorp->Execute.cmdbs[i].signal = subms[i].signal;
            iorp->Execute.cmdbs[i].signalValue = subms[i].signalValue;
            iorp->Execute.cmdbs[i].signalReserved = subms[i].signalReserved;
            iorp->Execute.cmdbs[i].signalStageMask = subms[i].signalStageMask;
        }
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX afxError _AvxDqueTransferResources(afxDrawQueue dque, avxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    if (!AfxTryLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _avxIoReqPacket* iorp;

    if ((ctrl->dstFcc == afxFcc_RAS) || (ctrl->srcFcc == afxFcc_RAS))
    {
        if ((ctrl->dstFcc == afxFcc_RAS) && (ctrl->srcFcc == afxFcc_RAS))
        {
            if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.rasCpyOps[0])), &cmdId, &iorp))
                AfxThrowError();
        }
        else if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.rasOps[0])), &cmdId, &iorp))
            AfxThrowError();
    }
    else
    {
        if ((ctrl->dstFcc == afxFcc_BUF) && (ctrl->srcFcc == afxFcc_BUF))
        {
            if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.bufCpyOps[0])), &cmdId, &iorp))
                AfxThrowError();
        }
        else if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.bufOps[0])), &cmdId, &iorp))
            AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT(iorp);

        iorp->Transfer.wait = ctrl->wait;
        iorp->Transfer.waitValue = ctrl->waitValue;
        iorp->Transfer.waitReserved = ctrl->waitReserved;
        iorp->Transfer.waitStageMask = ctrl->waitStageMask;
        iorp->Transfer.signal = ctrl->signal;
        iorp->Transfer.signalValue = ctrl->signalValue;
        iorp->Transfer.signalReserved = ctrl->signalReserved;
        iorp->Transfer.signalStageMask = ctrl->signalStageMask;

        iorp->Transfer.srcFcc = ctrl->srcFcc;
        iorp->Transfer.dstFcc = ctrl->dstFcc;
        iorp->Transfer.codec = ctrl->codec;
        iorp->Transfer.decSiz = ctrl->decSiz;

        iorp->Transfer.opCnt = opCnt;

        switch (ctrl->srcFcc)
        {
        case afxFcc_RAS:
        {
            iorp->Transfer.src.ras = ctrl->src.ras;
            AfxReacquireObjects(1, &iorp->Transfer.src.ras);
            break;
        }
        case afxFcc_BUF:
        {
            iorp->Transfer.src.buf = ctrl->src.buf;
            AfxReacquireObjects(1, &iorp->Transfer.src.buf);
            break;
        }
        case afxFcc_IOB:
        {
            iorp->Transfer.src.iob = ctrl->src.iob;
            AfxReacquireObjects(1, &iorp->Transfer.src.iob);
            break;
        }
        case NIL:
        {
            iorp->Transfer.src.src = ctrl->src.src;
            break;
        }
        default: AfxThrowError(); break;
        }

        switch (ctrl->dstFcc)
        {
        case afxFcc_RAS:
        {
            iorp->Transfer.dst.ras = ctrl->dst.ras;
            AfxReacquireObjects(1, &iorp->Transfer.dst.ras);
            break;
        }
        case afxFcc_BUF:
        {
            iorp->Transfer.dst.buf = ctrl->dst.buf;
            AfxReacquireObjects(1, &iorp->Transfer.dst.buf);
            break;
        }
        case afxFcc_IOB:
        {
            iorp->Transfer.dst.iob = ctrl->dst.iob;
            AfxReacquireObjects(1, &iorp->Transfer.dst.iob);
            break;
        }
        case NIL:
        {
            iorp->Transfer.dst.dst = ctrl->dst.dst;
            break;
        }
        default: AfxThrowError(); break;
        }

        if ((ctrl->dstFcc == afxFcc_RAS) || (ctrl->srcFcc == afxFcc_RAS))
        {
            if ((ctrl->dstFcc == afxFcc_RAS) && (ctrl->srcFcc == afxFcc_RAS))
            {
                avxRasterCopy const* riops = ops;
                for (afxUnit i = 0; i < opCnt; i++)
                {
                    iorp->Transfer.rasCpyOps[i] = riops[i];
                }
            }
            else
            {
                avxRasterIo const* riops = ops;
                for (afxUnit i = 0; i < opCnt; i++)
                {
                    iorp->Transfer.rasOps[i] = riops[i];
                }
            }
            int a = 1;
        }
        else if ((ctrl->srcFcc == afxFcc_BUF) || (ctrl->dstFcc == afxFcc_BUF))
        {
            if ((ctrl->srcFcc == afxFcc_BUF) && (ctrl->dstFcc == afxFcc_BUF))
            {
                avxBufferCopy const* biops = ops;
                for (afxUnit i = 0; i < opCnt; i++)
                {
                    iorp->Transfer.bufCpyOps[i] = biops[i];
                }
            }
            else
            {
                avxBufferIo const* biops = ops;
                for (afxUnit i = 0; i < opCnt; i++)
                {
                    iorp->Transfer.bufOps[i] = biops[i];
                }
            }
            int a = 1;
        }
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX afxError _AvxDqueRemapBuffers(afxDrawQueue dque, afxUnit mapCnt, _avxBufferRemapping const maps[], afxUnit unmapCnt, _avxBufferRemapping const unmaps[])
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (mapCnt)
    {
        afxCmdId cmdId;
        _avxIoReqPacket* iorp;
        
        if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (mapCnt * sizeof(iorp->Remap.mapOps[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->Remap.firstMapIdx = 0;
            iorp->Remap.mapCnt = mapCnt;
            iorp->Remap.firstUnmapIdx = 0;
            iorp->Remap.unmapCnt = 0;

            for (afxUnit i = 0; i < mapCnt; i++)
            {
                _avxBufferRemapping const* map = &maps[i];

                iorp->Remap.mapOps[i].buf = map->buf;
                iorp->Remap.mapOps[i].offset = map->offset;
                iorp->Remap.mapOps[i].range = map->range;
                iorp->Remap.mapOps[i].flags = map->flags;
                iorp->Remap.mapOps[i].placeholder = map->placeholder;

                AfxReacquireObjects(1, &map->buf);
                AfxIncAtom32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    if (unmapCnt)
    {
        afxCmdId cmdId;
        _avxIoReqPacket* iorp;
        
        if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (unmapCnt * sizeof(iorp->Remap.unmapOps[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->Remap.firstMapIdx = 0;
            iorp->Remap.firstUnmapIdx = 0;
            iorp->Remap.mapCnt = 0;
            iorp->Remap.unmapCnt = unmapCnt;

            for (afxUnit i = 0; i < unmapCnt; i++)
            {
                _avxBufferRemapping const* map = &unmaps[i];

                iorp->Remap.unmapOps[i].buf = map->buf;

                AfxReacquireObjects(1, &map->buf);
                AfxIncAtom32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX afxError _AvxDqueCohereMappedBuffers(afxDrawQueue dque, afxUnit flushCnt, avxBufferedMap const flushes[], afxUnit fetchCnt, avxBufferedMap const fetches[])
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (flushCnt)
    {
        afxCmdId cmdId;
        _avxIoReqPacket* iorp;
        
        if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (flushCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->SyncMaps.baseFetchIdx = 0;
            iorp->SyncMaps.fetchCnt = 0;
            iorp->SyncMaps.baseFlushIdx = 0;
            iorp->SyncMaps.flushCnt = flushCnt;

            for (afxUnit i = 0; i < flushCnt; i++)
            {
                avxBufferedMap const* map = &flushes[i];

                iorp->SyncMaps.ops[i].buf = map->buf;
                iorp->SyncMaps.ops[i].offset = map->offset;
                iorp->SyncMaps.ops[i].range = map->range;

                AfxReacquireObjects(1, &map->buf);
            }
        }
    }

    if (fetchCnt)
    {
        afxCmdId cmdId;
        _avxIoReqPacket* iorp;
        
        if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (fetchCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->SyncMaps.baseFetchIdx = 0;
            iorp->SyncMaps.flushCnt = 0;
            iorp->SyncMaps.baseFlushIdx = 0;
            iorp->SyncMaps.fetchCnt = fetchCnt;

            for (afxUnit i = 0; i < fetchCnt; i++)
            {
                avxBufferedMap const* map = &fetches[i];

                iorp->SyncMaps.ops[i].buf = map->buf;
                iorp->SyncMaps.ops[i].offset = map->offset;
                iorp->SyncMaps.ops[i].range = map->range;

                AfxReacquireObjects(1, &map->buf);
            }
        }
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX afxError _AvxDqueResetQueries(afxDrawQueue dque, avxQueryPool qryp, afxUnit base, afxUnit cnt)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _avxIoReqPacket* iorp;

    if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(ResetQueries), sizeof(iorp->ResetQueries), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        iorp->ResetQueries.base = base;
        iorp->ResetQueries.cnt = cnt;
        iorp->ResetQueries.qryp = qryp;
        AfxReacquireObjects(1, &qryp);
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX afxError _AvxDqueQueryResults(afxDrawQueue dque, avxQueryPool qryp, avxQueryResultFlags flags, afxUnit base, afxUnit cnt, afxSize dstCap, void* dst, afxSize stride)
{
    afxError err = { 0 };
    // dque must be a valid afxDrawQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AfxLockMutex(&dque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _avxIoReqPacket* iorp;

    if (_AvxDquePushIoReqPacket(dque, _AVX_GET_STD_IORP_ID(QueryResults), sizeof(iorp->QueryResults), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        iorp->QueryResults.qryp = qryp;
        iorp->QueryResults.flags = flags;
        iorp->QueryResults.base = base;
        iorp->QueryResults.cnt = cnt;
        iorp->QueryResults.dstCap = dstCap;
        iorp->QueryResults.dst = dst;
        iorp->QueryResults.stride = stride;
    }

    AfxUnlockMutex(&dque->iorpChnMtx);

    afxDrawBridge dexu = AfxGetHost(dque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AvxDexu_PingCb(dexu, 0);

    return err;
}

_AVX _avxDdiDque const _AVX_DDI_DQUE =
{

};

_AVX afxError _AvxDqueDtorCb(afxDrawQueue dque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    AfxDeregisterChainedClasses(&dque->classes);

    AfxDismantleMutex(&dque->iorpChnMtx);
    AfxDismantleArena(&dque->iorpArena);
    AfxCleanUpFutex(&dque->iorpArenaSlock);
    AfxDismantleCondition(&dque->idleCnd);
    //AfxDismantleMutex(&dque->idleCndMtx);

    return err;
}

_AVX afxError _AvxDqueCtorCb(afxDrawQueue dque, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    afxDrawBridge dexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _avxDexuAcq const* cfg = AFX_CAST(_avxDexuAcq const*, args[1]);

#if 0
    afxMask const* bridges = AFX_CAST(afxMask const*, args[1]);
    AFX_ASSERT(bridges);
    afxMask exuMask = bridges[invokeNo];

    afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, AFX_INVALID_INDEX, NIL, 0, 0, NIL);
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxDrawBridge dexu;
        if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }
    }
    dque->exuMask = exuMask;
#else
    //AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    dque->ddev = cfg->ddev;
    //dque->portId = cfg->portId;
    dque->exuIdx = cfg->exuIdx;
#endif
    dque->dsys = AvxGetBridgedDrawSystem(dexu, NIL);

    dque->immediate = 0;// !!spec->immedate;

    AfxSetUpFutex(&dque->iorpArenaSlock);
    AfxMakeArena(&dque->iorpArena, NIL, AfxHere());

    AfxDeployMutex(&dque->iorpChnMtx, AFX_MTX_PLAIN);
    AfxMakeChain(&dque->iorpChn, dque);
    //AfxDeployMutex(&dque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&dque->idleCnd);

    dque->closed = FALSE;

    AfxMakeChain(&dque->classes, (void*)dque);

    dque->ddi = &_AVX_DDI_DQUE;
    dque->idd = NIL;

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_DQUE =
{
    .fcc = afxFcc_DQUE,
    .name = "DrawQueue",
    .desc = "Draw Device Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawQueue)),
    .ctor = (void*)_AvxDqueCtorCb,
    .dtor = (void*)_AvxDqueDtorCb
};

////////////////////////////////////////////////////////////////////////////////

#if 0
_AVX afxError _AvxAcquireDrawQueues(afxDrawSystem dsys, afxUnit cnt, afxMask const bridges[], afxDrawQueue queues[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClass* dqueCls = (afxClass*)_AvxDexuGetDqueClass(dsys);
    AFX_ASSERT_CLASS(dqueCls, afxFcc_DQUE);

    AFX_ASSERT(AVX_MAX_QUEUES_PER_BRIDGE >= cnt);

    if (AfxAcquireObjects(dqueCls, cnt, (afxObject*)queues, (void const*[]) { dsys, bridges }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, cnt, queues);
    }

    return err;
}
#endif
