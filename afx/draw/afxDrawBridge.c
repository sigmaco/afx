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
#include "../dev/AvxImplKit.h"

_AVX afxDrawDevice AfxGetDrawQueueDevice(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    afxDrawDevice ddev = AfxGetProvider(dque);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxDrawContext AfxGetDrawQueueContext(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    afxDrawContext dctx = dque->dctx;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxNat AfxGetDrawQueuePort(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    return dque->portId;
}

_AVX afxClass const* _AvxGetDrawBatchClass(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    afxClass const* cls = &dque->cmdbCls;
    AfxAssertClass(cls, afxFcc_CMDB);
    return cls;
}

_AVX afxError _AvxWaitForEmptyDrawQueue(afxDrawQueue dque, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dque must be a valid afxDrawQueue handle.
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    
    if (!dque->waitCb)
    {
        AfxLockMutex(&dque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsec = AfxMax(1, timeout) * 100000; // 100000 = 0.0001 sec
        
        while (dque->workChn.cnt)
        {
            AfxWaitTimedCondition(&dque->idleCnd, &dque->idleCndMtx, &ts);
            dque->dexu->pingCb(dque->dexu, 0);
        }
        AfxUnlockMutex(&dque->idleCndMtx);
    }
    else if (dque->waitCb(dque, timeout))
        AfxThrowError();

    return err;
}

_AVX void _AvxBeginDrawQueueDebugScope(afxDrawQueue dque, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dque must be a valid afxDrawQueue handle.
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
}

_AVX void _AvxEndDrawQueueDebugScope(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    /// dque must be a valid afxDrawQueue handle.
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
}

_AVX void _AvxPushDrawQueueDebugLabel(afxDrawQueue dque, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// dque must be a valid afxDrawQueue handle.
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
}

_AVX afxError _AvxDqueStdDtorCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    AfxDeregisterChainedClasses(&dque->classes);

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

    dque->portId = cfg->portId;
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
    afxDrawDevice ddev = AfxGetProvider(dexu);
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
    return dexu->portId;
}

_AVX void AfxQueryDrawQueues(afxDrawBridge dexu, afxNat* baseQueIdx, afxNat* queCnt)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssert(baseQueIdx || queCnt);

    if (baseQueIdx)
        *baseQueIdx = dexu->baseQueIdx;

    if (queCnt)
        *queCnt = dexu->queCnt;
}

_AVX afxError _AvxWaitForIdleDrawBridge(afxDrawBridge dexu, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// dexu must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    
    if (!dexu->waitCb)
    {
        if (dexu->pingCb)
            dexu->pingCb(dexu, 0);

        afxNat queCnt = dexu->queCnt;

        for (afxNat i = 0; i < queCnt; i++)
        {
            afxDrawQueue dque;
            AfxGetDrawQueues(dexu->dctx, i, 1, &dque);
            AfxAssertObjects(1, &dque, afxFcc_DQUE);
            _AvxWaitForEmptyDrawQueue(dque, timeout);
        }
    }
    else if (dexu->waitCb(dexu, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxSubmitDrawCommands(afxDrawBridge dexu, avxSubmission const* ctrl, afxNat cnt, avxCmdb cmdbs[])
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

    afxDrawContext dctx = AfxGetDrawBridgeContext(dexu);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat queCnt = AfxMin(dexu->queCnt, AfxMax(1, ctrl->queCnt));

    for (afxNat j = 0; j < queCnt; j++)
    {
        afxDrawQueue dque;
        AfxGetDrawQueues(dctx, dexu->baseQueIdx + j, 1, &dque);
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        if ((err = dque->submitCb(dque, ctrl, cnt, cmdbs)))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxSubmitTransferences(afxDrawBridge dexu, avxTransference const* ctrl, afxNat opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssert(opCnt);
    AfxAssert(ops);

    afxDrawContext dctx = AfxGetDrawBridgeContext(dexu);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat queCnt = AfxMin(dexu->queCnt, AfxMax(1, ctrl->queCnt));

    for (afxNat j = 0; j < queCnt; j++)
    {
        afxDrawQueue dque;
        AfxGetDrawQueues(dctx, dexu->baseQueIdx + j, 1, &dque);
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        if ((err = dque->transferCb(dque, ctrl, opCnt, ops)))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxSubmitDrawWorkRequest(afxDrawBridge dexu, afxNat cnt, avxQueueOp const subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);
    AfxAssert(cnt);
    AfxAssert(subm);

    afxDrawContext dctx = AfxGetDrawBridgeContext(dexu);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat queCnt = 1;

    for (afxNat j = 0; j < queCnt; j++)
    {
        afxDrawQueue dque;
        AfxGetDrawQueues(dctx, dexu->baseQueIdx + j, 1, &dque);
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        if ((err = dque->submCb(dque, NIL, cnt, subm)))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxDexuStdDtorCb(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dexu, afxFcc_DEXU);

    AfxWaitForDrawContext(dexu->dctx, AFX_TIME_INFINITE);
    AfxWaitForDrawContext(dexu->dctx, AFX_TIME_INFINITE); // yes, two times.

    afxDrawDevice ddev = AfxGetDrawBridgeDevice(dexu);

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
    afxNat* baseQueIdxs = (afxNat*)(args[3]);
    AfxAssert(baseQueIdxs);

    if (!cfg) AfxThrowError();
    else
    {
        dexu->dctx = dctx;
        AfxAssertRange(AfxCountDrawPorts(ddev), cfg->portId, 1);
        dexu->portId = cfg->portId;
        dexu->exuIdx = invokeNo;

        dexu->waitCb = NIL;

        dexu->baseQueIdx = baseQueIdxs[invokeNo];
        dexu->queCnt = cfg->minQueCnt;
        //AfxAssert(dexu->queCnt);
    }
    return err;
}

_AVX afxClassConfig const _AvxDexuStdImplementation =
{
    .fcc = afxFcc_DEXU,
    .name = "DrawBridge",
    .desc = "Draw Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawBridge)),
    .ctor = (void*)_AvxDexuStdCtorCb,
    .dtor = (void*)_AvxDexuStdDtorCb
};
