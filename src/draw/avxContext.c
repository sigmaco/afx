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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

_AVX _avxCmdBatch* _AvxDctxGetCmdBatch(afxDrawContext dctx, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    _avxCmdBatch* batch = NIL;
    afxBool valid = AfxGetPoolUnit(&dctx->batches, idx, (void**)&batch);
    if ((!valid) || (!batch)) return NIL;
    AFX_ASSERT(batch->fcc == afxFcc_DCMD);
    return batch;
}

_AVX _avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(siz >= sizeof(_avxCmdHdr));

    _avxCmd* cmd = AfxRequestArena(&dctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &dctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AVX avxDrawContextState _AvxDctxGetStatus(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->state;
}

_AVX afxUnit AvxGetCommandPort(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->exuIdx;
}

_AVX afxUnit AvxGetCommandPool(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->poolIdx;
}

_AVX afxError AvxTraverseDrawCommands(afxDrawContext dctx, afxCmdId(*f)(void* udd, void* cmd), void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    return err;
}

_AVX afxError AvxExhaustDrawContext(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AFX_ASSERT(dctx->ddi->exhaust);
    if (dctx->ddi->exhaust(dctx, freeMem))
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

_AVX afxError AvxRecordDrawCommands(afxDrawContext dctx, afxBool once, afxBool deferred)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AFX_ASSERT(dctx->ddi->compose);
    if (dctx->ddi->compose(dctx, once, deferred))
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

_AVX afxError AvxDiscardDrawCommands(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AFX_ASSERT(dctx->ddi->discard);
    if (dctx->ddi->discard(dctx, freeRes))
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

_AVX afxError AvxCompileDrawCommands(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    AFX_ASSERT(dctx->ddi->compile);
    if (dctx->ddi->compile(dctx))
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

_AVX afxError AvxRecycleDrawCommands(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);


    AFX_ASSERT(dctx->ddi->recycle);
    if (dctx->ddi->recycle(dctx, freeRes))
    {
        AfxThrowError();
    }
    return err;
}

_AVX afxBool AvxDoesDrawCommandsExist_(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxUnit batchId = dctx->batchId;
    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return FALSE;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    _avxCmdBatch* batch = _AvxDctxGetCmdBatch(dctx, batchId);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return !!batch;
}

#if 0
_AVX afxError AvxAcquireAuxContexts(afxDrawContext dctx, avxContextInfo const* info, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(contexts);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDexuGetDctxClass(dctx);
    AFX_ASSERT_CLASS(cls, afxFcc_DCTX);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { dexu, info }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);

    return err;
}
#endif

_AVX afxError _AvxDctxDtorCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AvxExhaustDrawContext(dctx, TRUE);
    AFX_ASSERT(dctx->objsToBeDisposed.pop == 0);
    AfxEmptyArray(&dctx->objsToBeDisposed, FALSE, FALSE);

    AfxCleanUpFutex(&dctx->cmdbReqLock);
    AfxMakeChain(&dctx->cmdbRecycChain, dctx);

    AfxDismantleArena(&dctx->cmdArena);
    AfxExhaustPool(&dctx->batches, TRUE);

    return err;
}

_AVX afxError _AvxDctxCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawBridge dexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    AFX_ASSERT(args[1]);
    avxContextInfo const* info = AFX_CAST(avxContextInfo const*, args[1]);
    
    afxDrawSystem dsys = AfxGetHost(dexu);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    dctx->exuIdx = dexu->exuIdx;
    dctx->poolIdx = AfxGetObjectId(dctx);

    dctx->cmdbLockedForReq = FALSE;
    AfxSetUpFutex(&dctx->cmdbReqLock);
    AfxMakeChain(&dctx->cmdbRecycChain, dctx);

    AfxMakeArena(&dctx->cmdArena, NIL, AfxHere());

    dctx->devLimits = _AvxDdevGetLimits(ddev);
    dctx->enabledFeatures = _AvxDsysAccessReqFeatures(dsys);

    dctx->transient = FALSE;
    dctx->deferred = FALSE;
    dctx->once = FALSE;
    
    dctx->ddi = &_AVX_DDI_DCTX;
    
    AfxSetUpPool(&dctx->batches, sizeof(_avxCmdBatch), 3, 0);


    AfxMakeChain(&dctx->commands, dctx);

    AfxMakeArray(&dctx->objsToBeDisposed, sizeof(afxObject), 4, NIL, 0);

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_DCTX =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "AVX Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawContext)),
    .unitsPerPage = 4,
    .ctor = (void*)_AvxDctxCtorCb,
    .dtor = (void*)_AvxDctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireDrawContexts(afxDrawSystem dsys, avxContextInfo const* info, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(contexts);
    AFX_ASSERT(cnt);

    afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, info->caps, info->exuMask, 0, 0, NIL);

    if (!exuCnt)
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit exuIter = 0; exuIter < exuCnt; exuIter++)
    {
        afxDrawBridge dexu;
        if (!AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, info->caps, info->exuMask, exuIter, 1, &dexu))
        {
            AfxThrowError();
            continue;
        }

        afxDrawQueue dque;
        afxUnit nextQueIdx = 0;
        AvxGetDrawQueues(dexu, nextQueIdx, 1, &dque);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        afxClass* cls = (afxClass*)_AvxDexuGetDctxClass(dexu);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { dexu, info }))
        {
            AfxThrowError();
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);

    return err;
}

_AVX afxError AvxExecuteDrawCommands(afxDrawSystem dsys, afxUnit cnt, avxSubmission submissions[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(submissions);
    AFX_ASSERT(cnt);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        avxSubmission* subm = &submissions[ctxIt];

        afxDrawContext dctx = subm->dctx;
        if (!dctx)
        {
            AFX_ASSERT(subm->dctx);
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

        afxMask exuMask = subm->exuMask;
        afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxDrawBridge dexu;
                if (!AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxDrawQueue dque;
                    afxUnit nextQueIdx = 0;
                    while (AvxGetDrawQueues(dexu, nextQueIdx, 1, &dque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
                        ++nextQueIdx;

                        afxError err2 = AvxSubmitDrawCommands(dque, 1, subm);

                        if (!err2)
                        {
                            queued = TRUE;
                            break; // while --- get queue
                        }

                        if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                            break; // while --- get queue

                        AfxThrowError();
                    }
                    if (err || queued) break; // while --- find queues
                }
                if (err || queued) break; // for --- iterate bridge
            }
            if (err || queued) break; // while --- find bridges
        }
    }
    return err;
}



_AVX afxError AvxPrepareDrawCommandsNEXT(afxDrawContext dctx, afxUnit bin, afxBool once, afxBool deferred)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_RANGE(dctx->binCnt, bin, 1);

    if (dctx->ddi->compose)
    {
        if (dctx->ddi->compose(dctx, once, deferred))
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
    AFX_ITERATE_CHAIN_B2F(cmdb, recyc, &dctx->cmdbRecycChain)
    {
        AFX_ASSERT(cmdb->fcc == afxFcc_DCMD);

        AfxPopLink(&cmdb->recyc);

        AfxMakeChain(&cmdb->commands, dctx);

        // Move the allocated resource to the context.
        if (AfxMergeArenas(&dctx->cmdArena, &cmdb->cmdArenaCompiled))
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

        if (AfxRequestPoolUnits(&dctx->batches, AfxHere(), 1, &cmdbId, (void**)&cmdb))
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

        AfxMakeChain(&cmdb->commands, dctx);

        AfxMakeArena(&cmdb->cmdArenaCompiled, NIL, AfxHere());
        break;
    }

    if (!err)
    {
        cmdb->inDrawScope = FALSE;
        cmdb->inVideoCoding = FALSE;

        dctx->state = avxDrawContextState_RECORDING;
        AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);

    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError AvxPurgeDrawCommandsNEXT(afxDrawContext dctx, afxUnit bin, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    if (bin == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(bin != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (dctx->ddi->recycle)
    {
        if (dctx->ddi->recycle(dctx, freeRes))
        {
            AfxThrowError();
        }
        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&dctx->batches, bin));
    _avxCmdBatch* cmdb = _AvxDctxGetCmdBatch(dctx, bin);

    if (!cmdb)
    {
        AFX_ASSERT(cmdb);
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(cmdb->fcc == afxFcc_DCMD);
    AFX_ASSERT(cmdb->uniqId == bin);

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
            AfxReclaimArena(&cmdb->cmdArenaCompiled, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&cmdb->cmdArenaCompiled);
#endif

    AfxMakeChain(&cmdb->commands, cmdb);

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

        if (AfxReclaimPoolUnits(&dctx->batches, AfxHere(), 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&dctx->batches, FALSE);
    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);
    return err;
}
