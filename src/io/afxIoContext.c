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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
#define _AFX_IO_CONTEXT_C
#include "afxIoDDK.h"
#include "../exec/afxSystemDDK.h"

#ifndef _AFX_CORE_C
AFX_DECLARE_STRUCT(_afxIocDdi);
#else
AFX_DEFINE_STRUCT(_afxIocDdi)
{
    afxError(*compose)(afxIoContext, afxBool once, afxBool deferred, afxUnit* bid);
    afxError(*compile)(afxIoContext, afxUnit bid);
    afxError(*recycle)(afxIoContext, afxUnit bid, afxBool freeMem);
    afxError(*discard)(afxIoContext, afxBool freeMem);
    afxError(*exhaust)(afxIoContext, afxBool freeMem);
};
#endif

AFX_DEFINE_STRUCT(_afxCmdBatch)
{
    afxFcc          fcc;
    afxUnit         uniqId;
    afxLink         recyc;
    afxBool         once; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    afxBool         deferred;
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.
    afxChain        commands;
    afxArena        cmdArenaCompiled;
};

#ifdef _AFX_IO_CONTEXT_C
#ifdef _AFX_IO_CONTEXT_IMPL
AFX_OBJECT(_afxIoContext)
#else
AFX_OBJECT(afxIoContext)
#endif
{
    _afxIocDdi const*  pimpl;
    /*afxDrawContextState*/afxUnit state;
    afxBool             once; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    afxBool             deferred;
    afxPool             batches;

    afxUnit         exuIdx;
    afxUnit         poolIdx;
    // Is short-lived? That is, does not recycle batches, etc.
    afxBool         transient;
    afxChain        cmdbRecycChain;
    afxFutex        cmdbReqLock;
    afxBool         cmdbLockedForReq;

    afxChain        commands;
    afxArena        cmdArena; // owned by dsys data for specific port
};
#endif//_AFX_IO_CONTEXT_C

AFX_DEFINE_STRUCT(_afxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(_afxCmd)
{
    _afxCmdHdr hdr;

    // BUFFER OPERATIONS

    struct
    {
        _afxCmdHdr hdr;

    } CopyBuffer;
};

AFX_DEFINE_UNION(_afxCmdLut)
{
    struct
    {
        void* CopyBuffer;
    };
    void(*f[])(void*, _afxCmd const*);
};

#define _AFX_CMD_ID(cmdName_) (offsetof(_afxCmdLut, cmdName_) / sizeof(void*))

_AFX _afxCmdBatch* _AfxGetCmdBatch(afxIoContext ioc, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);
    _afxCmdBatch* batch = NIL;
    afxBool valid = AfxGetPoolUnit(&ioc->batches, idx, (void**)&batch);
    if ((!valid) || (!batch)) return NIL;
    AFX_ASSERT(batch->fcc == afxFcc_XCMD);
    return batch;
}

_AFX _afxCmd* _AfxIocPushCmd(afxIoContext ioc, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);
    AFX_ASSERT(siz >= sizeof(_afxCmdHdr));

    _afxCmd* cmd = AfxRequestArena(&ioc->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &ioc->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX afxUnit _AfxGetCommandStatus(afxIoContext ioc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);
    return ioc->state;
}

_AFX afxUnit AfxGetCommandPort(afxIoContext ioc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);
    return ioc->exuIdx;
}

_AFX afxUnit AfxGetCommandPool(afxIoContext ioc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);
    return ioc->poolIdx;
}

_AFX afxError AfxExhaustIoContext(afxIoContext ioc, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    if (ioc->pimpl->exhaust)
    {
        if (ioc->pimpl->exhaust(ioc, freeMem))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(AfxIsChainEmpty(&ioc->commands));
            AFX_ASSERT(ioc->batches.totalUsedCnt == 0);
        }
        return err;
    }

    AfxLockFutex(&ioc->cmdbReqLock, FALSE);

    while (1)
    {
        afxUnit leftCnt = 0;
        _afxCmdBatch* batch;
        for (afxUnit i = 0; AfxEnumeratePoolItems(&ioc->batches, i, 1, (void**)&batch); i++)
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
            if (AfxRecycleIoCommands(ioc, i, freeMem))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (freeMem)
    {
        AfxExhaustArena(&ioc->cmdArena);
        AfxExhaustPool(&ioc->batches, !freeMem);
    }

    AfxMakeChain(&ioc->commands, ioc);
    AFX_ASSERT(ioc->batches.totalUsedCnt == 0);

    AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);

    return err;
}

_AFX afxError AfxRecordIoCommands(afxIoContext ioc, afxBool once, afxBool deferred, afxUnit* batchId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);
    AFX_ASSERT(batchId);

    if (ioc->pimpl->compose)
    {
        afxUnit bId;
        if (ioc->pimpl->compose(ioc, once, deferred, &bId))
        {
            AfxThrowError();
            bId = AFX_INVALID_INDEX;
        }
        else
        {
            //AFX_ASSERT(ioc->state == afxIoContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&ioc->commands));
        }

        AFX_ASSERT(batchId);
        *batchId = bId;

        return err;
    }

    // AFX assumes the compilation when ICD does not take the front.

#if 0
    if (!AfxTryLockFutex(&ioc->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        return err;
    }
#else
    AfxLockFutex(&ioc->cmdbReqLock, FALSE);
#endif

    // Firstly, try recycling batches.
    afxBool recycled = FALSE;

    afxUnit cmdbId;
    _afxCmdBatch* cmdb;
    AFX_ITERATE_CHAIN_B2F(_afxCmdBatch, cmdb, recyc, &ioc->cmdbRecycChain)
    {
        AFX_ASSERT(cmdb->fcc == afxFcc_XCMD);

        AfxPopLink(&cmdb->recyc);

        AfxMakeChain(&cmdb->commands, ioc);

        // Move the allocated resource to the context.
        if (AfxMergeArenas(&ioc->cmdArena, &cmdb->cmdArenaCompiled))
        {
            AfxThrowError();
        }

        if (ioc->cmdArena.cleanupCnt > 3)
            AfxExhaustArena(&ioc->cmdArena);

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

        if (ioc->batches.totalUsedCnt >= 100)
        {
            AfxThrowError();
            break;
        }

        if (AfxRequestPoolUnits(&ioc->batches, 1, &cmdbId, (void**)&cmdb))
        {
            AfxThrowError();
            break;
        }

        cmdb->fcc = afxFcc_XCMD;
        cmdb->uniqId = cmdbId;
        cmdb->submCnt = 0;
        cmdb->submQueMask = NIL;

        AfxPushLink(&cmdb->recyc, NIL);

        AFX_ASSERT(!once || (once == TRUE));
        cmdb->once = !!once;
        AFX_ASSERT(!deferred || (deferred == TRUE));
        cmdb->deferred = !!deferred;

        AfxMakeChain(&cmdb->commands, ioc);

        AfxMakeArena(&cmdb->cmdArenaCompiled, NIL, AfxHere());
        break;
    }

    if (!err)
    {
        ioc->state = 1;
        AFX_ASSERT(ioc->state == 1);

        AFX_ASSERT(batchId);
        *batchId = cmdbId;
    }

    AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);

    return err;
}

_AFX afxError AfxDiscardIoCommands(afxIoContext ioc, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    if (ioc->pimpl->discard)
    {
        if (ioc->pimpl->discard(ioc, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            //AFX_ASSERT(ioc->state == afxIoContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&ioc->commands));
        }
        return err;
    }

    // AFX assumes the compilation when ICD does not take the front.

    if (AfxIsChainEmpty(&ioc->commands))
        return err;

#if 0
    if (!AfxTryLockFutex(&ioc->cmdbReqLock, FALSE))
    {
        // Bail out immediately.
        // Don't care for robustness here because these resources will be reused anyway if this context is kept operational.
        AfxThrowError();
        return err;
    }
#else
    AfxLockFutex(&ioc->cmdbReqLock, FALSE);
#endif
    
    // How to use @freeRes here? It is mandatory to reclaim every allocation to arena as we could purge it all at once.

#if 0
    if (freeRes)
    {
        AfxExhaustArena(&ioc->cmdArena);
    }
    else
    {
        _afxCmd* cmd;
        AFX_ITERATE_CHAIN(_afxCmd, cmd, hdr.script, &ioc->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimArena(&ioc->cmdArena, cmd, cmd->hdr.siz);
        }
    }
#else
    AfxExhaustArena(&ioc->cmdArena);
#endif

    AfxMakeChain(&ioc->commands, ioc);

    AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);

    return err;
}

_AFX afxError AfxCompileIoCommands(afxIoContext ioc, afxUnit batchId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the draw context will be moved to the invalid state.
    // The draw context must have been in the recording state, and, if successful, is moved to the executable state.

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (ioc->pimpl->compile)
    {
        if (ioc->pimpl->compile(ioc, batchId))
        {
            AfxThrowError();
        }
        else
        {
            //AFX_ASSERT(ioc->state == afxIoContextState_EXECUTABLE);
            AFX_ASSERT(AfxIsChainEmpty(&ioc->commands));
        }
        return err;
    }

    // AFX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&ioc->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&ioc->batches, batchId));
    _afxCmdBatch* batch = _AfxGetCmdBatch(ioc, batchId);

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
                //AfxAssumeArena(&batch->cmdArenaCompiled, &ioc->cmdArena);
                AfxMergeArenas(&batch->cmdArenaCompiled, &ioc->cmdArena);
                AfxAppendChain(&batch->commands, &ioc->commands);
                AfxExhaustArena(&ioc->cmdArena);
                AFX_ASSERT(AfxIsChainEmpty(&ioc->commands));
            }
        }
    }

    AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);

    return err;
}

_AFX afxError AfxRecycleIoCommands(afxIoContext ioc, afxUnit batchId, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return err;
    }

    if (ioc->pimpl->recycle)
    {
        if (ioc->pimpl->recycle(ioc, batchId, freeRes))
        {
            AfxThrowError();
        }
        return err;
    }

    // AFX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&ioc->cmdbReqLock, FALSE);

    AFX_ASSERT(AfxIsAnValidPoolUnit(&ioc->batches, batchId));
    _afxCmdBatch* cmdb = _AfxGetCmdBatch(ioc, batchId);

    if (!cmdb)
    {
        AFX_ASSERT(cmdb);
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(cmdb->fcc == afxFcc_XCMD);
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
        AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);
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
        _afxCmd* cmd;
        AFX_ITERATE_CHAIN(_afxCmd, cmd, hdr.script, &cmdb->commands)
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
    if (3 > ioc->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &ioc->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&ioc->batches, 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&ioc->batches, FALSE);
    }

    AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);
    return err;
}

_AFX afxBool AfxDoesIoCommandsExist(afxIoContext ioc, afxUnit batchId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    if (batchId == AFX_INVALID_INDEX)
    {
        AFX_ASSERT(batchId != AFX_INVALID_INDEX);
        AfxThrowError();
        return FALSE;
    }

    // AFX assumes the compilation when ICD does not take the front.

    AfxLockFutex(&ioc->cmdbReqLock, FALSE);

    _afxCmdBatch* batch = _AfxGetCmdBatch(ioc, batchId);

    AfxUnlockFutex(&ioc->cmdbReqLock, FALSE);

    return !!batch;
}

_AFX afxError _AfxIocDtorCb(afxIoContext ioc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    AfxExhaustIoContext(ioc, TRUE);

    AfxCleanUpFutex(&ioc->cmdbReqLock);
    AfxMakeChain(&ioc->cmdbRecycChain, ioc);

    AfxDismantleArena(&ioc->cmdArena);
    AfxExhaustPool(&ioc->batches, TRUE);

    return err;
}

_AFX afxError _AfxIocCtorCb(afxIoContext ioc, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

    afxIoBridge dexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &dexu);
    afxBool once = *AFX_CAST(afxBool const*, args[1]);
    afxBool deferred = *AFX_CAST(afxBool const*, args[2]);

    ioc->exuIdx = dexu->exuIdx;
    ioc->poolIdx = AfxGetObjectId(ioc);

    ioc->cmdbLockedForReq = FALSE;
    AfxSetUpFutex(&ioc->cmdbReqLock);
    AfxMakeChain(&ioc->cmdbRecycChain, ioc);

    AfxMakeArena(&ioc->cmdArena, NIL, AfxHere());

    ioc->transient = FALSE;
    ioc->deferred = FALSE;
    ioc->once = FALSE;
    
    static _afxIocDdi _AFX_IOC_DDI = { 0 };
    ioc->pimpl = &_AFX_IOC_DDI;
    
    AfxSetUpPool(&ioc->batches, sizeof(_afxCmdBatch), 3, 0);

    AfxMakeChain(&ioc->commands, ioc);

    return err;
}

_AFX afxClassConfig const _AFX_IOC_CLASS_CONFIG =
{
    .fcc = afxFcc_IOC,
    .name = "IoContext",
    .desc = "Input/Output Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoContext)),
    .unitsPerPage = 4,
    .ctor = (void*)_AfxIocCtorCb,
    .dtor = (void*)_AfxIocDtorCb
};

#endif

////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX afxError AfxAcquireIoContexts(afxIoCaps caps, afxMask exuMask, afxUnit cnt, afxIoContext contexts[])
{
    afxError err = { 0 };
    AFX_ASSERT(contexts);
    AFX_ASSERT(cnt);

    afxUnit exuCnt = AfxChooseIoBridges(AFX_INVALID_INDEX, caps, exuMask, 0, 0, NIL);

    if (!exuCnt)
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit exuIter = 0; exuIter < exuCnt; exuIter++)
    {
        afxIoBridge exu;
        if (!AfxChooseIoBridges(AFX_INVALID_INDEX, caps, exuMask, exuIter, 1, &exu))
        {
            AfxThrowError();
            continue;
        }

        afxIoQueue ioq;
        afxUnit nextQueIdx = 0;
        AfxGetIoQueues(exu, nextQueIdx, 1, &ioq);
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &ioq);

        afxClass* cls = (afxClass*)_AfxXqueGetIocClass(exu);
        AFX_ASSERT_CLASS(cls, afxFcc_IOC);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { exu }))
        {
            AfxThrowError();
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_IOC, cnt, contexts);

    return err;
}

_AFX afxError AfxExecuteIoCommands(afxIoSystem dsys, afxSubmission* ctrl, afxUnit cnt, afxIoContext contexts[], afxUnit const batches[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxIoSystem handle.
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

    afxMask exuMask = 0;// ctrl->exuMask;
    afxUnit exuCnt = AfxChooseIoBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
    afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        afxIoContext ioc = contexts[ctxIt];
        AFX_ASSERT_OBJECTS(afxFcc_IOC, 1, &ioc);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxIoBridge dexu;
                if (!AfxChooseIoBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxIoQueue dque;
                    afxUnit nextQueIdx = 0;
                    while (AfxGetIoQueues(dexu, nextQueIdx, 1, &dque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
                        ++nextQueIdx;

                        afxError err2 = _AfxDqueExecuteIoCommands(dque, ctrl, 1, &ioc, &batches[ctxIt]);

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

AFX afxError    AfxCmdOpenFile(afxIoContext ioc, afxUnit slot, afxUri const* uri, afxFileFlags flags);
AFX afxError    AfxCmdLoadFile(afxIoContext ioc, afxUnit slot, afxUri const* uri);

AFX afxBool     AfxCmdOpenFileSegments(afxIoContext ioc, afxUnit slot, afxUnit baseSegIdx, afxUnit segCnt);
AFX void        AfxCmdCloseFileSegments(afxIoContext ioc, afxUnit slot, afxUnit baseSegIdx, afxUnit segCnt);
AFX afxError    AfxCmdRealizeFileSegments(afxIoContext ioc, afxUnit slot, afxUnit32 baseSegOffset, afxUnit segCnt);

AFX afxError    AfxReadFileLine(afxIoContext ioc, afxUnit slot, afxString* str);
AFX afxResult   AfxFlushFileCache(afxIoContext ioc, afxUnit slot);
#endif