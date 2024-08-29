/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_OUTPUT_C
#include "../../dev/AvxImplKit.h"

_AVX afxError _AvxDqueStdDtorCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    AfxCleanUpChainedClasses(&dque->classes);

    AfxDeregisterClass(&dque->cmdbCls);
    AfxDismantleSlock(&dque->cmdbReqLock);
    AfxCleanUpQueue(&dque->cmdbRecycQue);

    AfxCleanUpMutex(&dque->workChnMtx);
    AfxDismantleArena(&dque->workArena);
    AfxDismantleSlock(&dque->workArenaSlock);
    AfxCleanUpCondition(&dque->idleCnd);
    AfxCleanUpMutex(&dque->idleCndMtx);

    return err;
}

_AVX afxError _AvxDqueStdCtorCb(afxDrawQueue dque, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    afxDrawContext dctx = args[0];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu = args[1];
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxDrawBridgeConfig const *cfg = ((afxDrawBridgeConfig const *)args[2]);
    AfxAssert(cfg);

    dque->dexu = dexu;
    dque->dctx = dctx;

    dque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&dque->workArenaSlock);
    AfxDeployArena(&dque->workArena, NIL, AfxHere());

    AfxSetUpMutex(&dque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&dque->workChn, dexu);
    AfxSetUpMutex(&dque->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&dque->idleCnd);

    dque->closed = FALSE;

    AfxDeployChain(&dque->classes, (void*)dque);

    dque->cmdbLockedForReq = FALSE;
    AfxDeploySlock(&dque->cmdbReqLock);
    AfxSetUpQueue(&dque->cmdbRecycQue, sizeof(avxCmdb), 3);

    afxClassConfig tmpClsCfg = _AvxCmdbStdImplementation;
    AfxRegisterClass(&dque->cmdbCls, NIL, &dque->classes, &tmpClsCfg);

    return err;
}

_AVX afxClassConfig const _AvxDqueStdImplementation =
{
    .fcc = afxFcc_DQUE,
    .name = "DrawQueue",
    .desc = "Draw Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawQueue)),
    .ctor = (void*)_AvxDqueStdCtorCb,
    .dtor = (void*)_AvxDqueStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawDevice AfxGetDrawBridgeDevice(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxDrawDevice ddev = AfxGetParent(dexu);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxDrawContext AfxGetDrawBridgeContext(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxDrawContext dctx = dexu->dctx;
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxNat AfxGetDrawBridgePort(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    return dexu->portIdx;
}

_AVX afxClass const* _AvxGetDrawQueueClass(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxClass const* cls = &dexu->dqueCls;
    AfxAssertClass(cls, afxFcc_DQUE);
    return cls;
}

_AVX afxNat AfxCountDrawQueues(afxDrawBridge dexu, afxNat first)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssertRange(dexu->queCnt, first, 1);
    return dexu->queCnt - first;
}

_AVX afxNat AfxGetDrawQueues(afxDrawBridge dexu, afxNat first, afxNat cnt, afxDrawQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    /// queIdx must be less than the value of queCnt for the bridge indicated by portIdx when device context was created.
    AfxAssertRange(dexu->queCnt, first, cnt);
    AfxAssert(queues);
    afxNat rslt = 0;

    if (first < dexu->queCnt)
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            afxDrawQueue dque = dexu->queues[first + i];
            AfxAssertObjects(1, &dque, afxFcc_DQUE);
            /// queue must be a valid pointer to a afxDrawQueue handle.        
            queues[rslt++] = dque;
        }
    }
    return rslt;
}

_AVX afxError _AvxWaitForIdleDrawQueue(afxDrawBridge dexu, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxDrawQueue dque;

    if (!AfxGetDrawQueues(dexu, queIdx, 1, &dque))
        AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        if (!dexu->waitCb)
        {
            AfxLockMutex(&dque->idleCndMtx);

            afxTimeSpec ts = { 0 };
            ts.nsec = 100000; // 100000 = 0.0001 sec

            while (dque->workChn.cnt)
            {
                AfxWaitTimedCondition(&dque->idleCnd, &dque->idleCndMtx, &ts);
                dexu->pingCb(dexu, queIdx);
            }
            AfxUnlockMutex(&dque->idleCndMtx);
        }
        else if (dexu->waitCb(dexu, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxWaitForIdleDrawBridge(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxNat queCnt = dexu->queCnt;

    for (afxNat i = 0; i < queCnt; i++)
        _AvxWaitForIdleDrawQueue(dexu, i);
    
    return err;
}

_AVX void _AvxBeginDrawQueueDebugScope(afxDrawBridge dexu, afxNat queIdx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxNat queCnt = dexu->queCnt;
    AfxAssertRange(queCnt, queIdx, 1);
    

}

_AVX void _AvxEndDrawQueueDebugScope(afxDrawBridge dexu, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxNat queCnt = dexu->queCnt;
    AfxAssertRange(queCnt, queIdx, 1);
    
}

_AVX void _AvxPushDrawQueueDebugLabel(afxDrawBridge dexu, afxNat queIdx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    afxNat queCnt = dexu->queCnt;
    AfxAssertRange(queCnt, queIdx, 1);
    
    
}

_AVX afxNat _AvxSubmitDrawCommands(afxDrawBridge dexu, avxSubmission const* ctrl, afxNat cnt, avxCmdb cmdbs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssert(cnt);
    AfxAssert(cmdbs);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = dexu->submitCb(dexu, ctrl, cnt, cmdbs)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat _AvxSubmitTransferences(afxDrawBridge dexu, avxTransference const* ctrl, afxNat opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = dexu->transferCb(dexu, ctrl, opCnt, ops)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat _AvxSubmitDrawWorkRequest(afxDrawBridge dexu, afxNat cnt, avxQueueOp const subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssert(cnt);
    AfxAssert(subm);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = dexu->submCb(dexu, NIL, cnt, subm)))
        AfxThrowError();

    return queIdx;
}

_AVX afxError _AvxDexuStdDtorCb(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);

    AfxWaitForDrawContext(dexu->dctx);
    AfxWaitForDrawContext(dexu->dctx); // yes, two times.

    afxDrawDevice ddev = AfxGetDrawBridgeDevice(dexu);

    AfxAssertObjects(dexu->queCnt, dexu->queues, afxFcc_DQUE);
    AfxReleaseObjects(dexu->queCnt, dexu->queues);
    AfxCleanUpChainedClasses(&dexu->classes);

    afxObjectStash stashes[] =
    {
        {
            .cnt = dexu->queCnt,
            .siz = sizeof(dexu->queues[0]),
            .var = (void**)&dexu->queues
        }
    };
    AfxDeallocateInstanceData(dexu, AFX_COUNTOF(stashes), stashes);

    return err;
}

_AVX afxError _AvxDexuStdCtorCb(afxDrawBridge dexu, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);

    afxDrawDevice ddev = args[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawContext dctx = args[1];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridgeConfig const *cfg = ((afxDrawBridgeConfig const *)args[2]);
    AfxAssert(cfg);
    afxNat exuIdx = *(afxNat*)(args[3]);
    
    if (!cfg) AfxThrowError();
    else
    {
        dexu->dctx = dctx;
        AfxAssertRange(AfxCountDrawPorts(ddev), cfg->portIdx, 1);
        dexu->portIdx = cfg->portIdx;
        dexu->exuIdx = exuIdx;

        dexu->waitCb = NIL;
        dexu->submitCb = NIL;
        dexu->transferCb = NIL;

        dexu->queCnt = AfxMax(1, AfxMax(_AvxDqueStdImplementation.unitsPerPage, cfg->minQueCnt));

        AfxDeployChain(&dexu->classes, dexu);

        afxClassConfig clsCfg = _AvxDqueStdImplementation;
        clsCfg.maxCnt = dexu->queCnt;
        clsCfg.unitsPerPage = dexu->queCnt;
        AfxRegisterClass(&dexu->dqueCls, NIL, &dexu->classes, &clsCfg);
        // what about size of device-dependent draw queues?

        afxObjectStash stashes[] =
        {
            {
                .cnt = dexu->queCnt,
                .siz = sizeof(dexu->queues[0]),
                .var = (void**)&dexu->queues
            }
        };

        if (AfxAllocateInstanceData(dexu, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
        else
        {
            afxClass* cls = (afxClass*)_AvxGetDrawQueueClass(dexu);
            AfxAssertClass(cls, afxFcc_DQUE);

            if (AfxAcquireObjects(cls, dexu->queCnt, (afxObject*)dexu->queues, (void const*[]) { dctx, dexu, cfg })) AfxThrowError();
            else
            {
                AfxAssertObjects(dexu->queCnt, dexu->queues, afxFcc_DQUE);
            }

            if (err)
                AfxDeallocateInstanceData(dexu, AFX_COUNTOF(stashes), stashes);
        }

        if (err)
            AfxCleanUpChainedClasses(&dexu->classes);

    }
    return err;
}

_AVX afxClassConfig const _AvxDexuStdImplementation =
{
    .fcc = afxFcc_DEXU,
    .name = "DrawBrige",
    .desc = "Draw Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawBridge)),
    .ctor = (void*)_AvxDexuStdCtorCb,
    .dtor = (void*)_AvxDexuStdDtorCb
};
