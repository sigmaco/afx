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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "impl/avxImplementation.h"

_AFX _avxCmdBatch* _AvxGetCmdBatch(afxDrawContext dctx, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    _avxCmdBatch* batch = NIL;
    afxBool valid = AfxGetPoolUnit(&dctx->batches, idx, (void**)&batch);
    if ((!valid) || (!batch)) return NIL;
    AFX_ASSERT(batch->fcc == afxFcc_DCMD);
    return batch;
}

_AVX _avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(siz >= sizeof(avxCmdHdr));

    _avxCmd* cmd = AfxRequestFromArena(&dctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &dctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AVX avxDrawContextState _AvxGetCommandStatus(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    dctx->state;
}

_AVX afxUnit AvxGetCommandPort(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->portId;
}

_AVX afxUnit AvxGetCommandPool(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->poolIdx;
}

_AVX afxError AvxExhaustDrawContext(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    if (dctx->pimpl->exhaust)
    {
        if (dctx->pimpl->exhaust(dctx, freeMem))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
            AFX_ASSERT(dctx->batches.totalUsedCnt == 0);
        }
        return err;
    }

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    if (dctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(dctx->objsToBeDisposed.pop, dctx->objsToBeDisposed.data);

        AfxEmptyArray(&dctx->objsToBeDisposed, !freeMem, FALSE);
    }

    while (1)
    {
        afxUnit leftCnt = 0;
        _avxCmdBatch* batch;
        for (afxUnit i = 0; AfxEnumeratePoolItems(&dctx->batches, i, 1, (void**)&batch); i++)
        {
#if 0
            while (AfxLoadAtom32(&batch->submCnt))
            {
                AfxYield();
            }
#else
            if (AfxLoadAtom32(&batch->submCnt))
            {
                ++leftCnt;
                continue;
            }
#endif
            if (AvxRecycleDrawCommands(dctx, i, freeMem))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (freeMem)
    {
        AfxExhaustArena(&dctx->cmdArena);
        AfxExhaustPool(&dctx->batches, !freeMem);
    }

    AfxDeployChain(&dctx->commands, dctx);
    AFX_ASSERT(dctx->batches.totalUsedCnt == 0);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError AvxRecordDrawCommands(afxDrawContext dctx, afxBool once, afxBool deferred, afxUnit* batchId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(batchId);

    if (dctx->pimpl->compose)
    {
        afxUnit bId;
        if (dctx->pimpl->compose(dctx, once, deferred, &bId))
        {
            AfxThrowError();
            bId = AFX_INVALID_INDEX;
        }
        else
        {
            //AFX_ASSERT(dctx->state == avxDrawContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
        }

        AFX_ASSERT(batchId);
        *batchId = bId;

        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

#if 0
    if (!AfxTryLockFutex(&dctx->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        return err;
    }
#else
    AfxLockFutex(&dctx->cmdbReqLock, FALSE);
#endif

    // Firstly, try recycling batches.
    afxBool recycled = FALSE;

    afxUnit cmdbId;
    _avxCmdBatch* cmdb;
    AFX_ITERATE_CHAIN_B2F(_avxCmdBatch, cmdb, recyc, &dctx->cmdbRecycChain)
    {
        AFX_ASSERT(cmdb->fcc == afxFcc_DCMD);

        AfxPopLink(&cmdb->recyc);

        AfxDeployChain(&cmdb->commands, dctx);

        // Move the allocated resource to the context.
        if (AfxMergeArena(&dctx->cmdArena, &cmdb->cmdArenaCompiled))
        {
            AfxThrowError();
        }

        if (dctx->cmdArena.cleanupCnt > 3)
            AfxExhaustArena(&dctx->cmdArena);

        AFX_ASSERT(AfxLoadAtom32(&cmdb->submCnt) == 0);
        cmdb->submCnt = 0;
        cmdb->submQueMask = NIL;

        AFX_ASSERT(!once || (once == TRUE));
        cmdb->once = !!once;
        AFX_ASSERT(!deferred || (deferred == TRUE));
        cmdb->deferred = !!deferred;

        cmdbId = cmdb->uniqId;

        recycled = TRUE;
        break;
    }

    while (!recycled)
    {
        // If could not recycle a batch, request a new one from the pool.

        if (dctx->batches.totalUsedCnt >= 100)
        {
            AfxThrowError();
            break;
        }

        if (AfxRequestPoolUnits(&dctx->batches, 1, &cmdbId, (void**)&cmdb))
        {
            AfxThrowError();
            break;
        }

        cmdb->fcc = afxFcc_DCMD;
        cmdb->uniqId = cmdbId;
        cmdb->submCnt = 0;
        cmdb->submQueMask = NIL;

        AfxPushLink(&cmdb->recyc, NIL);

        AFX_ASSERT(!once || (once == TRUE));
        cmdb->once = !!once;
        AFX_ASSERT(!deferred || (deferred == TRUE));
        cmdb->deferred = !!deferred;

        AfxDeployChain(&cmdb->commands, dctx);

        AfxMakeArena(&cmdb->cmdArenaCompiled, NIL, AfxHere());
        break;
    }

    if (!err)
    {
        cmdb->inDrawScope = FALSE;
        cmdb->inVideoCoding = FALSE;

        dctx->state = avxDrawContextState_RECORDING;
        AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);

        AFX_ASSERT(batchId);
        *batchId = cmdbId;
    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError AvxDiscardDrawCommands(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    if (dctx->pimpl->discard)
    {
        if (dctx->pimpl->discard(dctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            //AFX_ASSERT(dctx->state == avxDrawContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
        }
        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

    if (AfxIsChainEmpty(&dctx->commands))
        return err;

#if 0
    if (!AfxTryLockFutex(&dctx->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        // Don't care for robustness here because these resources will be reused anyway if this context is kept operational.
        AfxThrowError();
        return err;
    }
#else
    AfxLockFutex(&dctx->cmdbReqLock, FALSE);
#endif
    
    // How to use @freeRes here? It is mandatory to reclaim every allocation to arena as we could purge it all at once.

#if 0
    if (freeRes)
    {
        AfxExhaustArena(&dctx->cmdArena);
    }
    else
    {
        _avxCmd* cmd;
        AFX_ITERATE_CHAIN(_avxCmd, cmd, hdr.script, &dctx->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimToArena(&dctx->cmdArena, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&dctx->cmdArena);
#endif

    AfxDeployChain(&dctx->commands, dctx);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError AvxCompileDrawCommands(afxDrawContext dctx, afxUnit batchId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (dctx->pimpl->compile)
    {
        if (dctx->pimpl->compile(dctx, batchId))
        {
            AfxThrowError();
        }
        else
        {
            //AFX_ASSERT(dctx->state == avxDrawContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
        }
        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&dctx->batches, batchId));
    _avxCmdBatch* batch = _AvxGetCmdBatch(dctx, batchId);

    if (!batch)
    {
        AFX_ASSERT(batch);
    }
    else
    {
        if (AfxLoadAtom32(&batch->submCnt))
        {
            AFX_ASSERT(batch->submCnt == 0);
            AfxThrowError();
        }
        else
        {
            if (batch->recyc.next != &batch->recyc)
            {
                AFX_ASSERT(!(batch->recyc.next != &batch->recyc));
                AfxThrowError();
            }
            else
            {
                //AfxAssumeArena(&batch->cmdArenaCompiled, &dctx->cmdArena);
                AfxMergeArena(&batch->cmdArenaCompiled, &dctx->cmdArena);
                AfxAppendChain(&batch->commands, &dctx->commands);
                AfxExhaustArena(&dctx->cmdArena);
                AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
            }
        }
    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError AvxRecycleDrawCommands(afxDrawContext dctx, afxUnit batchId, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (dctx->pimpl->recycle)
    {
        if (dctx->pimpl->recycle(dctx, batchId, freeRes))
        {
            AfxThrowError();
        }
        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&dctx->batches, batchId));
    _avxCmdBatch* cmdb = _AvxGetCmdBatch(dctx, batchId);

    if (!cmdb)
    {
        AFX_ASSERT(cmdb);
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(cmdb->fcc == afxFcc_DCMD);
    AFX_ASSERT(cmdb->uniqId == batchId);

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from DPU as there not a lock mechanism for arena and batches' pool.

#if 0
    if (freeRes)
    {
        AFX_ASSERT(freeRes == TRUE);
        AfxExhaustArena(&cmdb->cmdArenaCompiled);
    }
    else
    {
        _avxCmd* cmd;
        AFX_ITERATE_CHAIN(_avxCmd, cmd, hdr.script, &cmdb->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimToArena(&cmdb->cmdArenaCompiled, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&cmdb->cmdArenaCompiled);
#endif

    AfxDeployChain(&cmdb->commands, cmdb);

    afxBool recycled = FALSE;
#if 0
    if (3 > dctx->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &dctx->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&dctx->batches, 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&dctx->batches, FALSE);
    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);
    return err;
}

_AVX afxBool AvxDoesDrawCommandsExist(afxDrawContext dctx, afxUnit batchId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return FALSE;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    _avxCmdBatch* batch = _AvxGetCmdBatch(dctx, batchId);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return !!batch;
}

_AVX afxError _AvxDctxDtorCb(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AvxExhaustDrawContext(dctx, TRUE);
    AFX_ASSERT(dctx->objsToBeDisposed.pop == 0);
    AfxEmptyArray(&dctx->objsToBeDisposed, FALSE, FALSE);

    AfxDismantleFutex(&dctx->cmdbReqLock);
    AfxDeployChain(&dctx->cmdbRecycChain, dctx);

    AfxDismantleArena(&dctx->cmdArena);
    AfxExhaustPool(&dctx->batches, TRUE);

    return err;
}

_AVX afxError _AvxDctxCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawBridge dexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AfxGetProvider(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit portId;
    afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, &portId);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    dctx->portId = portId;
    dctx->poolIdx = AfxGetObjectId(dctx);

    dctx->cmdbLockedForReq = FALSE;
    AfxDeployFutex(&dctx->cmdbReqLock);
    AfxDeployChain(&dctx->cmdbRecycChain, dctx);

    AfxMakeArena(&dctx->cmdArena, NIL, AfxHere());

    dctx->devLimits = _AvxDdevGetLimits(ddev);
    dctx->enabledFeatures = _AvxDsysAccessReqFeatures(dsys);

    dctx->transient = FALSE;
    dctx->deferred = FALSE;
    dctx->once = FALSE;
    

    dctx->pimpl = &_AVX_DCTX_DDI;
    
    AfxDeployPool(&dctx->batches, sizeof(_avxCmdBatch), 3, 0);


    AfxDeployChain(&dctx->commands, dctx);

    AfxMakeArray(&dctx->objsToBeDisposed, sizeof(afxObject), 4, NIL, 0);

    return err;
}

_AVX afxClassConfig const _AVX_DCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Draw Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawContext)),
    .unitsPerPage = 4,
    .ctor = (void*)_AvxDctxCtorCb,
    .dtor = (void*)_AvxDctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireDrawContexts(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(contexts);
    AFX_ASSERT(cnt);

    afxDrawBridge dexu;
    if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxClass* cls = (afxClass*)_AvxGetDrawContextClass(dexu);
    AFX_ASSERT_CLASS(cls, afxFcc_DCTX);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { dexu }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);

    return err;
}

_AVX afxError AvxExecuteDrawCommands(afxDrawSystem dsys, avxSubmission* ctrl, afxUnit cnt, afxDrawContext contexts[], afxUnit const batches[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(contexts);
    AFX_ASSERT(batches);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(cnt);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */
    
    afxMask exuMask = ctrl->exuMask;
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

        afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);

        // sanitize arguments
        afxUnit totalQueCnt = dqueCls->instCnt;
        afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
        afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
        AFX_ASSERT(queCnt);
        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit queIt = 0; queIt < queCnt; queIt++)
            {
                afxUnit queIdx = baseQueIdx + queIt;

                afxDrawQueue dque;
                if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque))
                {
                    AfxThrowError();
                    break; // for
                }
                AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

                afxError err2 = _AvxDqueExecuteDrawCommands(dque, ctrl, cnt, contexts, batches);

                if (!err2)
                {
                    queued = TRUE;
                    break; // for
                }
                
                if (err2 == afxError_TIMEOUT)
                {
                    //err = afxError_TIMEOUT;
                    continue; // for
                }

                AfxThrowError();
                break; // for
            }

            if (err || queued)
                break; // while --- reiterate if not queue for timeout?
        }
    }
    return err;
}
