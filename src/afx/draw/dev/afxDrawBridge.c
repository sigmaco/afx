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
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#include "AvxDevKit.h"

_AVX afxDrawDevice AfxGetDrawBridgeDevice(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxDrawDevice ddev = AfxGetObjectProvider(ddge);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxDrawContext AfxGetDrawBridgeContext(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxDrawContext dctx = ddge->dctx;
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxNat AfxGetDrawBridgePort(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    return ddge->portIdx;
}

_AVX afxClass const* _AvxGetDrawQueueClass(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    /// ddge must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxClass const* cls = &ddge->dqueCls;
    AfxAssertClass(cls, afxFcc_DQUE);
    return cls;
}

_AVX afxNat _AvxCountDrawQueues(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    /// ddge must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    return ddge->queCnt;
}

_AVX afxBool _AvxGetDrawQueue(afxDrawBridge ddge, afxNat queIdx, afxDrawQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// ddge must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    /// queIdx must be less than the value of queCnt for the bridge indicated by portIdx when device context was created.
    AfxAssertRange(ddge->queCnt, queIdx, 1);
    afxBool rslt;

    if (!(rslt = (queIdx < ddge->queCnt))) AfxThrowError();
    else
    {
        afxDrawQueue dque = ddge->queues[queIdx];
        AfxAssertObjects(1, &dque, afxFcc_DQUE);
        /// queue must be a valid pointer to a afxDrawQueue handle.        
        *queue = dque;
    }
    return rslt;
}

_AVX afxError _AvxWaitForIdleDrawQueue(afxDrawBridge ddge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// ddge must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxDrawQueue dque;

    if (!_AvxGetDrawQueue(ddge, queIdx, &dque)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dque, afxFcc_DQUE);

        if (!ddge->waitCb)
        {
            AfxLockMutex(&dque->idleCndMtx);

            while (dque->workChn.cnt)
                AfxWaitCondition(&dque->idleCnd, &dque->idleCndMtx);

            AfxUnlockMutex(&dque->idleCndMtx);
        }
        else if (ddge->waitCb(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError _AvxWaitForIdleDrawBridge(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    /// ddge must be a valid afxDrawBridge handle.
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxNat queCnt = ddge->queCnt;

    for (afxNat i = 0; i < queCnt; i++)
        _AvxWaitForIdleDrawQueue(ddge, i);
    
    return err;
}

_AVX afxNat AfxEnqueueExecutionRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->executeCb(ddge, fenc, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AfxEnqueueTransferRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->transferCb(ddge, fenc, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AfxEnqueuePresentRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->presentCb(ddge, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AfxEnqueueStampRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->stampCb(ddge, cnt, req, origin, caption)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AvxRequestDrawWork(afxDrawBridge ddge, afxNat cnt, avxSubmission const subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(subm);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->submCb(ddge, NIL, cnt, subm)))
        AfxThrowError();

    return queIdx;
}

_AVX afxError _AvxDqueStdDtorCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    AfxCleanUpMutex(&dque->workChnMtx);
    AfxDismantleArena(&dque->workArena);
    AfxCleanUpSlock(&dque->workArenaSlock);
    AfxCleanUpCondition(&dque->idleCnd);
    AfxCleanUpMutex(&dque->idleCndMtx);

    return err;
}

_AVX afxError _AvxDdgeStdDtorCb(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    AfxWaitForDrawContext(ddge->dctx);
    AfxWaitForDrawContext(ddge->dctx); // yes, two times.

    AfxAssertObjects(ddge->queCnt, ddge->queues, afxFcc_DQUE);
    AfxReleaseObjects(ddge->queCnt, ddge->queues);
    AfxCleanUpChainedClasses(&ddge->classes);
    AfxDeallocate(ddge->queues);

    return err;
}

_AVX afxError _AvxDqueStdCtorCb(afxDrawQueue dque, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    afxDrawContext dctx = cookie->udd[0];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = cookie->udd[1];
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxDrawBridgeConfig const *cfg = ((afxDrawBridgeConfig const *)cookie->udd[2]);
    AfxAssert(cfg);

    dque->ddge = ddge;
    dque->dctx = dctx;

    dque->immediate = 0;// !!spec->immedate;

    AfxSetUpSlock(&dque->workArenaSlock);
    AfxDeployArena(&dque->workArena, NIL, AfxHere());

    AfxSetUpMutex(&dque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&dque->workChn, ddge);
    AfxSetUpMutex(&dque->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&dque->idleCnd);

    dque->closed = FALSE;

    return err;
}

_AVX afxError _AvxDdgeStdCtorCb(afxDrawBridge ddge, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawContext dctx = cookie->udd[1];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridgeConfig const *cfg = ((afxDrawBridgeConfig const *)cookie->udd[2]);
    AfxAssert(cfg);
    
    AfxAssertRange(AfxCountDrawPorts(ddev), cfg->portIdx, 1);
    ddge->portIdx = cfg->portIdx;
    ddge->dctx = dctx;
    
    ddge->waitCb = NIL;
    ddge->executeCb = NIL;
    ddge->transferCb = NIL;
    ddge->presentCb = NIL;
    ddge->stampCb = NIL;

    ddge->queCnt = AfxMax(1, AfxMax(_AvxDqueStdImplementation.unitsPerPage, cfg->queueCnt));

    AfxDeployChain(&ddge->classes, ddge);

    afxClassConfig clsCfg = _AvxDqueStdImplementation;
    clsCfg.maxCnt = ddge->queCnt;
    clsCfg.unitsPerPage = ddge->queCnt;
    AfxRegisterClass(&ddge->dqueCls, NIL, &ddge->classes, &clsCfg);
    // what about size of device-dependent draw queues?

    if (!(ddge->queues = AfxAllocate(ddge->queCnt, sizeof(ddge->queues[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxClass* cls = (afxClass*)_AvxGetDrawQueueClass(ddge);
        AfxAssertClass(cls, afxFcc_DQUE);

        if (AfxAcquireObjects(cls, ddge->queCnt, (afxObject*)ddge->queues, (void const*[]) { dctx, ddge, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(ddge->queCnt, ddge->queues, afxFcc_DQUE);
        }

        if (err)
            AfxDeallocate(ddge->queues);
    }
    
    if (err)
        AfxCleanUpChainedClasses(&ddge->classes);

    return err;
}

_AVX afxClassConfig const _AvxDqueStdImplementation =
{
    .fcc = afxFcc_DQUE,
    .name = "DrawQueue",
    .desc = "Draw Device Execution Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawQueue)),
    .ctor = (void*)_AvxDqueStdCtorCb,
    .dtor = (void*)_AvxDqueStdDtorCb
};

_AVX afxClassConfig const _AvxDdgeStdImplementation =
{
    .fcc = afxFcc_DDGE,
    .name = "DrawBrige",
    .desc = "Draw Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawBridge)),
    .ctor = (void*)_AvxDdgeStdCtorCb,
    .dtor = (void*)_AvxDdgeStdDtorCb
};
