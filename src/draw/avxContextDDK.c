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

_AVX afxError _AvxDctxExhaustCb(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    if (dctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(dctx->objsToBeDisposed.pop, dctx->objsToBeDisposed.items);

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
            dctx->batchId = i;
            if (AvxRecycleDrawCommands(dctx, freeMem))
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

    AfxMakeChain(&dctx->commands, dctx);
    AFX_ASSERT(dctx->batches.totalUsedCnt == 0);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError _AvxDctxComposeCb(afxDrawContext dctx, afxBool once, afxBool deferred)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.

    if (dctx->batches.totalUsedCnt)
        return afxError_BUSY;

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

        dctx->batchId = cmdbId;
    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError _AvxDctxDiscardCb(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

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
            AfxReclaimArena(&dctx->cmdArena, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&dctx->cmdArena);
#endif

    AfxMakeChain(&dctx->commands, dctx);

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError _AvxDctxCompileCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    afxUnit batchId = dctx->batchId;
    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&dctx->batches, batchId));
    _avxCmdBatch* batch = _AvxDctxGetCmdBatch(dctx, batchId);

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
                AfxMergeArenas(&batch->cmdArenaCompiled, &dctx->cmdArena);
                AfxAppendChain(&batch->commands, &dctx->commands);
                AfxExhaustArena(&dctx->cmdArena);
                AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
            }
        }
    }

    AfxUnlockFutex(&dctx->cmdbReqLock, FALSE);

    return err;
}

_AVX afxError _AvxDctxRecycleCb(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxUnit batchId = dctx->batchId;
    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    // AVX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&dctx->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&dctx->batches, batchId));
    _avxCmdBatch* cmdb = _AvxDctxGetCmdBatch(dctx, batchId);

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

_AVX _avxDdiDctx const _AVX_DDI_DCTX =
{
    .compose = _AvxDctxComposeCb,
    .discard = _AvxDctxDiscardCb,
    .compile = _AvxDctxCompileCb,
    .recycle = _AvxDctxRecycleCb,
    .exhaust = _AvxDctxExhaustCb
};
