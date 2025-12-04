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

_AVX afxClass const* _AvxDctxGetDctxClass(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    afxClass const* cls = &dctx->dctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
    return cls;
}

_AVX avxContextStatus _AvxDctxGetStatus(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->state;
}

_AVX afxMask AvxGetCommandPort(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->exuMask;
}

_AVX afxDrawContext AvxGetCommandPool(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->pool;
}

_AVX afxError AvxTraverseDrawCommands(afxDrawContext dctx, afxCmdId(*f)(void* udd, void* cmd), void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    return err;
}

_AVX afxError AvxPrepareDrawCommands(afxDrawContext dctx, afxBool purge, avxCmdFlags flags)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must not be in the PENDING state.
    AFX_ASSERT(dctx->state != avxContextStatus_PENDING);

    AFX_ASSERT(dctx->ddi->prepare);
    if (dctx->ddi->prepare(dctx, purge, flags))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(dctx->state == avxContextStatus_RECORDING);
        AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
    }

    return err;
}

_AVX afxError AvxCompileDrawCommands(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the RECORDING state.
    AFX_ASSERT(dctx->state == avxContextStatus_RECORDING);
    // If dctx is a prime context, there must not be an active draw scope.
    AFX_ASSERT(!dctx->inDrawScope || (dctx->cmdFlags & avxCmdFlag_DEFERRED));
    // Any and every query activated during the recording of dctx must have been deactivated.
    // TODO: Do it.
    // If dctx is a aux context, there must not be any remaining BeginDebugUtilsLabelEXT command recorded to dctx that has not previously been ended by a call to EndDebugUtilsLabelEXT.
    // TODO: Do it.

    AFX_ASSERT(dctx->ddi->compile);
    if (dctx->ddi->compile(dctx))
    {
        AfxThrowError();
        
        // If there was an error during recording, the application will be notified by an unsuccessful return 
        // code returned by AfxCompileDrawCommands, and the draw context will be moved to the invalid state.

        AFX_ASSERT(dctx->state == avxContextStatus_INVALID);
    }
    else
    {
        // The draw context must have been in the recording state, and, if successful, is moved to the executable state.
        AFX_ASSERT(dctx->state == avxContextStatus_EXECUTABLE);
        //AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
    }
    return err;
}

_AVX afxError AvxExhaustDrawContext(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be a pool context.
    AFX_ASSERT(!dctx->pool || (dctx->pool == dctx));

    AFX_ASSERT(dctx->ddi->exhaust);
    if (dctx->ddi->exhaust(dctx, freeMem))
    {
        AfxThrowError();
    }
    else
    {
        //AFX_ASSERT(dctx->state == avxContextStatus_INITIAL);
        AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
    }
    return err;
}

_AVX afxError _AvxDctxDtorCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AvxExhaustDrawContext(dctx, TRUE);
    AFX_ASSERT(dctx->objsToBeDisposed.pop == 0);
    AfxEmptyArray(&dctx->objsToBeDisposed, FALSE, FALSE);

    AfxExhaustInterlockedQueue(&dctx->recycQue);

    AfxDismantleArena(&dctx->cmdArena);

    AfxDeregisterChainedClasses(&dctx->classes);

    return err;
}

_AVX afxError _AvxDctxCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawContext pool = AFX_CAST(afxDrawContext, args[1]);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
    AFX_ASSERT(args[2]);
    avxContextInfo const* info = AFX_CAST(avxContextInfo const*, args[2]);
    afxClassConfig const* pDctxClsCfg = NIL;/* AFX_CAST(afxClassConfig const*, args[3]);*/

    dctx->ddi = &_AVX_DDI_DCTX;
    dctx->udd = info->udd;
    dctx->tag = info->tag;
    dctx->devLimits = _AvxDsysGetLimits(dsys);
    dctx->enabledFeatures = _AvxDsysGetReqFeatures(dsys);

    AfxMakeChain(&dctx->classes, dctx);

    if (!pool)
    {
        dctx->pool = dctx;

        dctx->caps = info->caps;
        dctx->exuMask = info->exuMask;

        afxClassConfig dctxClsCfg = pDctxClsCfg ? *pDctxClsCfg : _AVX_CLASS_CONFIG_DCTX;
        AFX_ASSERT(dctxClsCfg.fcc == afxFcc_DCTX);
        dctxClsCfg.unitsPerPage = AFX_OR(AFX_MIN(info->auxCnt, 32), dctxClsCfg.unitsPerPage);
        AfxMountClass(&dctx->dctxCls, NIL, &dctx->classes, &dctxClsCfg);

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&dctx->recycQue, sizeof(afxDrawContext), AFX_MAX(AFX_MIN(info->recycCap, 32), 1));
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
        
        // If there is a base, the base must be its own base.
        // Effectively, a aux context can only be acquired from a base context.
        // By allowing it to be itself, we allow the code to proceed normally avoid testing against NIL.
        if (pool->pool && (pool->pool != pool))
        {
            AfxThrowError();
            return err;
        }
        dctx->pool = pool;

        dctx->caps = (pool->caps & info->caps);
        dctx->exuMask = (pool->exuMask & info->exuMask);

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&dctx->recycQue, sizeof(afxDrawContext), 1);
    }


    dctx->flags = info->flags;
    dctx->cmdFlags = info->cmdFlags;
    dctx->state = avxContextStatus_INITIAL;

    AfxMakeChain(&dctx->commands, dctx);
    AfxMakeArena(&dctx->cmdArena, NIL, AfxHere());

    AfxMakeArray(&dctx->objsToBeDisposed, sizeof(afxObject), 4, NIL, 0);

    dctx->inVideoCoding = FALSE;
    dctx->inDrawScope = FALSE;

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

_AVX afxError AvxAcquireDrawContexts(afxDrawSystem dsys, afxDrawContext pool, avxContextInfo const* info, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // contexts must be a valid pointer to an array of @cnt * afxDrawContext handles.
    AFX_ASSERT(contexts);
    // @info must be a valid pointer to a valid avxContextInfo structure.
    AFX_ASSERT(info);
    // @cnt must be greater than 0.
    AFX_ASSERT(cnt);

    AFX_ASSERT(32 >= info->auxCnt);
    AFX_ASSERT(32 >= info->recycCap);

    afxClass* cls;

    if (!pool)
    {
        cls = (afxClass*)_AvxDsysGetDctxClassCb_SW(dsys);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { dsys, pool, info }))
        {
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);
        return err;
    }

    // If @pool is not NIL, @pool must be a valid afxDrawContext handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);

    if (pool->pool != pool)
    {
        AFX_ASSERT(pool->pool != pool);
        AfxThrowError();
        return err;
    }

    afxBool once = (info->cmdFlags & avxCmdFlag_ONCE);
    afxBool deferred = (info->cmdFlags & avxCmdFlag_DEFERRED);

    afxUnit recycCnt = 0;
    while (1)
    {
        // Firstly, try recycling contexts.

        afxDrawContext aux;
        if (AfxPopInterlockedQueue(&pool->recycQue, &aux))
        {
            AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &aux);
            AFX_ASSERT(aux->submCnt == 0);
            AFX_ASSERT(aux->submQueMask == 0);

            AvxPrepareDrawCommands(aux, TRUE, info->cmdFlags);

            AfxMakeChain(&aux->commands, aux);

            AFX_ASSERT(AfxLoadAtom32(&aux->submCnt) == 0);
            aux->submCnt = 0;
            aux->submQueMask = NIL;

            aux->flags = info->flags ? (pool->flags & info->flags) : pool->flags;
            aux->cmdFlags = info->cmdFlags ? (pool->cmdFlags & info->cmdFlags) : pool->cmdFlags;
            aux->exuMask = info->exuMask ? (pool->exuMask & info->exuMask) : pool->exuMask;
            aux->caps = info->caps ? (pool->caps & info->caps) : pool->caps;

            contexts[recycCnt] = aux;

            ++recycCnt;
            if (recycCnt >= cnt)
                break;
        }
    }

    afxUnit remainCnt = (cnt - recycCnt);
    if (remainCnt)
    {
        cls = (afxClass*)_AvxDctxGetDctxClass(pool);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
        if (AfxAcquireObjects(cls, remainCnt, (afxObject*)&contexts[recycCnt], (void const*[]) { dsys, pool, info }))
        {
            AfxThrowError();

            // If acquisition failed, dispose all recycled contexts.
            // TODO: Recycle it.
            AfxDisposeObjects(recycCnt, contexts);
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);
    return err;
}

_AVX afxError AvxRecycleDrawContexts(afxBool freeRes, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = { 0 };

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxDrawContext dctx = contexts[iter];
        if (!dctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
        AFX_ASSERT(dctx->state != avxContextStatus_PENDING);

        AFX_ASSERT(dctx->ddi->recycle);
        if (dctx->ddi->recycle(dctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(dctx->state == avxContextStatus_INITIAL);
            AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
        }
    }
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

        AFX_ASSERT(!(dctx->cmdFlags & avxCmdFlag_DEFERRED));

        if (dctx->state != avxContextStatus_EXECUTABLE)
        {
            if ((dctx->state == avxContextStatus_PENDING) ||
                (dctx->state == avxContextStatus_INTERNAL_EXECUTING))
            {
                if (!(dctx->cmdFlags & avxCmdFlag_SHARED))
                {
                    AFX_ASSERT(!(dctx->cmdFlags & avxCmdFlag_SHARED));
                    AfxThrowError();
                }
            }
        }

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

#if 0

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
            //AFX_ASSERT(dctx->state == avxContextStatus_EXECUTABLE);
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

        dctx->state = avxContextStatus_RECORDING;
        AFX_ASSERT(dctx->state == avxContextStatus_RECORDING);

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
#endif
