/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
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
#define _AMX_BUFFER_C
#include "ddi/amxImplementation.h"

_AMX afxError _AmxMqueUnlockIoReqChain(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxUnlockMutex(&mque->iorpChnMtx);
    return err;
}

_AMX afxError _AmxMqueLockIoReqChain(afxMixQueue mque, afxUnit64 timeout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (timeout == 0)
    {
        if (!AfxTryLockMutex(&mque->iorpChnMtx))
            return afxError_TIMEOUT;
    }
    else
    {
        afxTimeSpec ts;
        AfxMakeTimeSpec(&ts, timeout);

        if (afxError_TIMEOUT == AfxLockMutexTimed(&mque->iorpChnMtx, &ts))
            return afxError_TIMEOUT;
        else
            AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxMquePopBlob(afxMixQueue mque, void* blob, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AfxReclaimToArena(&mque->iorpArena, blob, siz);
    return err;
}

_AMX void* _AmxMquePushBlob(afxMixQueue mque, afxUnit siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    void* blob = AfxRequestFromArena(&mque->iorpArena, siz, 1, NIL, 0);
    AFX_ASSERT(blob);
    return blob;
}

_AMX afxError _AmxMquePopIoReqPacket(afxMixQueue mque, _amxIoReqPacket* iorp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AFX_ASSERT(iorp);
    AfxPopLink(&iorp->hdr.chain);
    AfxReclaimToArena(&mque->iorpArena, iorp, iorp->hdr.siz);
    return err;
}

_AMX afxError _AmxMquePushIoReqPacket(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxIoReqPacket** pIorp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    _amxIoReqPacket* iorp = AfxRequestFromArena(&mque->iorpArena, siz, 1, NIL, 0);
    AFX_ASSERT(iorp);
    iorp->hdr.id = id;
    iorp->hdr.siz = siz;
    AfxGetTime(&iorp->hdr.pushTime);

    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&iorp->hdr.chain, &mque->iorpChn);
    
    AFX_ASSERT(pIorp);
    *pIorp = iorp;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxMixDevice AmxGetMixQueueDevice(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxMixBridge mexu = AfxGetHost(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev;
}

_AMX afxMixSystem AmxGetMixQueueSystem(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    afxMixSystem msys = mque->msys;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMX afxError _AmxMqueSubmitCallback(afxMixQueue mque, afxError(*f)(void*, void*), void* udd)
{
    afxError err = AFX_ERR_NONE;
    // mque must be a valid afxMixQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AFX_ASSERT(f);

    if (!AfxTryLockMutex(&mque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _amxIoReqPacket* iorp;

    if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Callback), sizeof(iorp->Callback), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        iorp->Callback.f = f;
        iorp->Callback.udd = udd;
    }

    AfxUnlockMutex(&mque->iorpChnMtx);

    afxMixBridge mexu = AfxGetHost(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _AmxMexu_PingCb(mexu, 0);

    return err;
}

_AMX afxError _AmxMqueExecuteMixCommands(afxMixQueue mque, afxUnit cnt, amxSubmission subms[])
{
    afxError err = AFX_ERR_NONE;
    // mque must be a valid afxMixQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(subms);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    if (!AfxTryLockMutex(&mque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _amxIoReqPacket* iorp;

    if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Execute), sizeof(iorp->Execute) + (cnt * sizeof(iorp->Execute.cmdbs[0])), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        for (afxUnit i = 0; i < cnt; i++)
        {
            iorp->Execute.signal = subms[i].signalSems;
            iorp->Execute.wait = subms[i].waitSems;
            iorp->Execute.hdr.completionFence = subms[i].fence;
            iorp->Execute.cmdbCnt = 1;

            afxMixContext mctx = subms[i].mctx;
            AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mctx);
            _amxCmdBatch* batch = _AmxGetCmdBatch(mctx, subms[i].batchId);

            AfxReacquireObjects(1, &mctx);
            AfxIncAtom32(&batch->submCnt);
            //batch->state = amxMixContextState_PENDING;

            iorp->Execute.cmdbs[i].mctx = mctx;
            iorp->Execute.cmdbs[i].batchId = subms[i].batchId;
        }
    }

    AfxUnlockMutex(&mque->iorpChnMtx);

    afxMixBridge mexu = AfxGetHost(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _AmxMexu_PingCb(mexu, 0);

    return err;
}

_AMX afxError _AmxMqueTransferResources(afxMixQueue mque, amxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    // mque must be a valid afxMixQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    if (!AfxTryLockMutex(&mque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _amxIoReqPacket* iorp;

    if ((ctrl->dstFcc == afxFcc_AUD) && (ctrl->srcFcc == afxFcc_AUD))
    {
#if 0
        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.rasCpyOps[0])), &cmdId, &iorp))
#endif
            AfxThrowError();
    }
    else if ((ctrl->dstFcc == afxFcc_AUD) || (ctrl->srcFcc == afxFcc_AUD))
    {
        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.wavOps[0])), &cmdId, &iorp))
            AfxThrowError();
    }
    else
    {
        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.bufOps[0])), &cmdId, &iorp))
            AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT(iorp);

        iorp->Transfer.srcFcc = ctrl->srcFcc;
        iorp->Transfer.dstFcc = ctrl->dstFcc;
        iorp->Transfer.codec = ctrl->codec;
        iorp->Transfer.decSiz = ctrl->decSiz;

        iorp->Transfer.opCnt = opCnt;

        switch (ctrl->srcFcc)
        {
        case afxFcc_MBUF:
        {
            iorp->Transfer.src.buf = ctrl->src.buf;
            AfxReacquireObjects(1, &iorp->Transfer.src.buf);
            break;
        }
        case afxFcc_AUD:
        {
            iorp->Transfer.src.aud = ctrl->src.aud;
            AfxReacquireObjects(1, &iorp->Transfer.src.aud);
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
        case afxFcc_MBUF:
        {
            iorp->Transfer.dst.buf = ctrl->dst.buf;
            AfxReacquireObjects(1, &iorp->Transfer.dst.buf);
            break;
        }
        case afxFcc_AUD:
        {
            iorp->Transfer.dst.aud = ctrl->dst.aud;
            AfxReacquireObjects(1, &iorp->Transfer.dst.aud);
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

        amxAudioIo const* riops = ops;
        for (afxUnit i = 0; i < opCnt; i++)
        {
            iorp->Transfer.wavOps[i] = riops[i];
        }
    }

    AfxUnlockMutex(&mque->iorpChnMtx);

    afxMixBridge mexu = AfxGetHost(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _AmxMexu_PingCb(mexu, 0);

    return err;
}

_AMX afxError _AmxMqueRemapBuffers(afxMixQueue mque, afxUnit mapCnt, _amxBufferRemapping const maps[], afxUnit unmapCnt, _amxBufferRemapping const unmaps[])
{
    afxError err = AFX_ERR_NONE;
    // mque must be a valid afxMixQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (AfxLockMutex(&mque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (mapCnt)
    {
        afxCmdId cmdId;
        _amxIoReqPacket* iorp;

        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (mapCnt * sizeof(iorp->Remap.mapOps[0])), &cmdId, &iorp))
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
                _amxBufferRemapping const* map = &maps[i];

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
        _amxIoReqPacket* iorp;

        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (unmapCnt * sizeof(iorp->Remap.unmapOps[0])), &cmdId, &iorp))
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
                _amxBufferRemapping const* map = &unmaps[i];

                iorp->Remap.unmapOps[i].buf = map->buf;

                AfxReacquireObjects(1, &map->buf);
                AfxIncAtom32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    AfxUnlockMutex(&mque->iorpChnMtx);

    afxMixBridge mexu = AfxGetHost(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _AmxMexu_PingCb(mexu, 0);

    return err;
}

_AMX afxError _AmxMqueCohereMappedBuffers(afxMixQueue mque, afxUnit flushCnt, amxBufferedMap const flushes[], afxUnit fetchCnt, amxBufferedMap const fetches[])
{
    afxError err = AFX_ERR_NONE;
    // mque must be a valid afxMixQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (AfxLockMutex(&mque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (flushCnt)
    {
        afxCmdId cmdId;
        _amxIoReqPacket* iorp;

        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (flushCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
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
                amxBufferedMap const* map = &flushes[i];

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
        _amxIoReqPacket* iorp;

        if (_AmxMquePushIoReqPacket(mque, _AMX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (fetchCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
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
                amxBufferedMap const* map = &fetches[i];

                iorp->SyncMaps.ops[i].buf = map->buf;
                iorp->SyncMaps.ops[i].offset = map->offset;
                iorp->SyncMaps.ops[i].range = map->range;

                AfxReacquireObjects(1, &map->buf);
            }
        }
    }

    AfxUnlockMutex(&mque->iorpChnMtx);

    afxMixBridge mexu = AfxGetHost(mque);
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _AmxMexu_PingCb(mexu, 0);

    return err;
}

_AMX afxError _AmxMqueDtorCb(afxMixQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    AfxDismantleMutex(&mque->iorpChnMtx);
    AfxDismantleArena(&mque->iorpArena);
    AfxDismantleFutex(&mque->iorpArenaSlock);
    AfxDismantleCondition(&mque->idleCnd);
    AfxDismantleMutex(&mque->idleCndMtx);

    return err;
}

_AMX afxError _AmxMqueCtorCb(afxMixQueue mque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    afxMixBridge mexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    _amxMexuAcquisition const* cfg = AFX_CAST(_amxMexuAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    mque->mdev = cfg->mdev;
    //mque->portId = cfg->portId;
    mque->exuIdx = cfg->exuIdx;
    mque->msys = AmxGetBridgedMixSystem(mexu, NIL);

    mque->immediate = 0;// !!spec->immedate;

    AfxDeployFutex(&mque->iorpArenaSlock);
    AfxMakeArena(&mque->iorpArena, NIL, AfxHere());

    AfxDeployMutex(&mque->iorpChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&mque->iorpChn, mexu);
    AfxDeployMutex(&mque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&mque->idleCnd);

    mque->closed = FALSE;

    return err;
}

_AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_MQUE,
    .name = "MixQueue",
    .desc = "Mix Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixQueue)),
    .ctor = (void*)_AmxMqueCtorCb,
    .dtor = (void*)_AmxMqueDtorCb
};
