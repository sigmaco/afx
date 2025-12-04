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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_SINK_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_TRACK_C
#define _AMX_AUDIO_C
#define _AMX_AUDIO_C
#include "amxIcd.h"

_AMX afxError _AmxMctxExhaustCb(afxMixContext mix, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    AfxLockFutex(&mix->cmdbReqLock, FALSE);
#if 0
    if (mix->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(mix->objsToBeDisposed.pop, mix->objsToBeDisposed.items);

        AfxEmptyArray(&mix->objsToBeDisposed, !freeMem, FALSE);
    }
#endif

    while (1)
    {
        afxUnit leftCnt = 0;
        _amxCmdBatch* batch;
        for (afxUnit i = 0; AfxEnumeratePoolItems(&mix->batches, i, 1, (void**)&batch); i++)
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
            mix->batchId = i;
            if (AmxRecycleMixCommands(mix, freeMem))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (freeMem)
    {
        AfxExhaustArena(&mix->cmdArena);
        AfxExhaustPool(&mix->batches, !freeMem);
    }

    AfxMakeChain(&mix->commands, mix);
    AFX_ASSERT(mix->batches.totalUsedCnt == 0);

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError _AmxMctxComposeCb(afxMixContext mix, afxBool once, afxBool deferred)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    // AMX assumes the compilation when ICD does not take the front.

    if (mix->batches.totalUsedCnt)
        return afxError_BUSY;

#if 0
    if (!AfxTryLockFutex(&mix->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        return err;
    }
#else
    AfxLockFutex(&mix->cmdbReqLock, FALSE);
#endif

    // Firstly, try recycling batches.
    afxBool recycled = FALSE;

    afxUnit cmdbId;
    _amxCmdBatch* cmdb;
    AFX_ITERATE_CHAIN_B2F(cmdb, recyc, &mix->cmdbRecycChain)
    {
        AFX_ASSERT(cmdb->fcc == afxFcc_MCMD);

        AfxPopLink(&cmdb->recyc);

        AfxMakeChain(&cmdb->commands, mix);

        // Move the allocated resource to the context.
        if (AfxMergeArenas(&mix->cmdArena, &cmdb->cmdArenaCompiled))
        {
            AfxThrowError();
        }

        if (mix->cmdArena.cleanupCnt > 3)
            AfxExhaustArena(&mix->cmdArena);

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

        if (mix->batches.totalUsedCnt >= 100)
        {
            AfxThrowError();
            break;
        }

        if (AfxRequestPoolUnits(&mix->batches, AfxHere(), 1, &cmdbId, (void**)&cmdb))
        {
            AfxThrowError();
            break;
        }

        cmdb->fcc = afxFcc_MCMD;
        cmdb->uniqId = cmdbId;
        cmdb->submCnt = 0;
        cmdb->submQueMask = NIL;

        AfxPushLink(&cmdb->recyc, NIL);

        AFX_ASSERT(!once || (once == TRUE));
        cmdb->once = !!once;
        AFX_ASSERT(!deferred || (deferred == TRUE));
        cmdb->deferred = !!deferred;

        AfxMakeChain(&cmdb->commands, mix);

        AfxMakeArena(&cmdb->cmdArenaCompiled, NIL, AfxHere());
        break;
    }

    if (!err)
    {
#if 0
        cmdb->inMixScope = FALSE;
        cmdb->inVideoCoding = FALSE;
#endif
        mix->state = amxMixState_RECORDING;
        AFX_ASSERT(mix->state == amxMixState_RECORDING);

        mix->batchId = cmdbId;
    }

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError _AmxMctxDiscardCb(afxMixContext mix, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    // AMX assumes the compilation when ICD does not take the front.

    if (AfxIsChainEmpty(&mix->commands))
        return err;

#if 0
    if (!AfxTryLockFutex(&mix->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        // Don't care for robustness here because these resources will be reused anyway if this context is kept operational.
        AfxThrowError();
        return err;
    }
#else
    AfxLockFutex(&mix->cmdbReqLock, FALSE);
#endif

    // How to use @freeRes here? It is mandatory to reclaim every allocation to arena as we could purge it all at once.

#if 0
    if (freeRes)
    {
        AfxExhaustArena(&mix->cmdArena);
    }
    else
    {
        _amxCmd* cmd;
        AFX_ITERATE_CHAIN(_amxCmd, cmd, hdr.script, &mix->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimArena(&mix->cmdArena, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&mix->cmdArena);
#endif

    AfxMakeChain(&mix->commands, mix);

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError _AmxMctxCompileCb(afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    afxUnit batchId = mix->batchId;
    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    // AMX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&mix->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&mix->batches, batchId));
    _amxCmdBatch* batch = _AmxMctxGetCmdBatch(mix, batchId);

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
                //AfxAssumeArena(&batch->cmdArenaCompiled, &mix->cmdArena);
                AfxMergeArenas(&batch->cmdArenaCompiled, &mix->cmdArena);
                AfxAppendChain(&batch->commands, &mix->commands);
                AfxExhaustArena(&mix->cmdArena);
                AFX_ASSERT(AfxIsChainEmpty(&mix->commands));
            }
        }
    }

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);

    return err;
}

_AMX afxError _AmxMctxRecycleCb(afxMixContext mix, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxUnit batchId = mix->batchId;
    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    // AMX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&mix->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&mix->batches, batchId));
    _amxCmdBatch* cmdb = _AmxMctxGetCmdBatch(mix, batchId);

    if (!cmdb)
    {
        AFX_ASSERT(cmdb);
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(cmdb->fcc == afxFcc_MCMD);
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
        AfxUnlockFutex(&mix->cmdbReqLock, FALSE);
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from MPU as there not a lock mechanism for arena and batches' pool.

#if 0
    if (freeRes)
    {
        AFX_ASSERT(freeRes == TRUE);
        AfxExhaustArena(&cmdb->cmdArenaCompiled);
    }
    else
    {
        _amxCmd* cmd;
        AFX_ITERATE_CHAIN(_amxCmd, cmd, hdr.script, &cmdb->commands)
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
    if (3 > mix->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &mix->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&mix->batches, AfxHere(), 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&mix->batches, FALSE);
    }

    AfxUnlockFutex(&mix->cmdbReqLock, FALSE);
    return err;
}

_AMX _amxDdiMctx const _AMX_MCTX_DDI =
{
    .compose = _AmxMctxComposeCb,
    .discard = _AmxMctxDiscardCb,
    .compile = _AmxMctxCompileCb,
    .recycle = _AmxMctxRecycleCb,
    .exhaust = _AmxMctxExhaustCb
};
