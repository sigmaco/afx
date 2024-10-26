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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
#define _AFX_DRAW_OUTPUT_C
#include "../dev/AvxImplKit.h"
#include "qwadro/inc/io/afxIoBridge.h"

#ifdef _AFX_IO_QUEUE_C
AFX_OBJECT(afxIoQueue)
{
    afxUnit          portId;
    afxIoBridge     xexu; // owner bridge
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxSlock        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxError        (*waitCb)(afxIoQueue, afxTime);
    afxError        (*submitCb)(afxIoQueue, afxSubmission const*, afxUnit, afxObject[]);
    afxError        (*transferCb)(afxIoQueue, afxTransference const*, afxUnit, void const*);
};
#endif//_AFX_IO_QUEUE_C

#ifdef _AFX_IO_BRIDGE_C
AFX_OBJECT(afxIoBridge)
{
    afxUnit          exuIdx;
    afxUnit          portId;

    afxChain        classes;
    afxClass        xqueCls;
    afxUnit          queCnt;
    afxIoQueue*     queues;

    afxError        (*waitCb)(afxIoBridge, afxUnit);
    afxError        (*pingCb)(afxIoBridge, afxUnit);
};
#endif//_AFX_IO_BRIDGE_C

_AFX afxUnit AfxGetIoQueuePort(afxIoQueue xque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xque, afxFcc_XQUE);
    return xque->portId;
}

_AFX afxError _AfxWaitForEmptyIoQueue(afxIoQueue xque, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// xque must be a valid afxIoQueue handle.
    AfxAssertObjects(1, &xque, afxFcc_XQUE);
    
    if (!xque->waitCb)
    {
        AfxLockMutex(&xque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsec = AfxMax(1, timeout) * 100000; // 100000 = 0.0001 sec
        
        while (xque->workChn.cnt)
        {
            AfxWaitTimedCondition(&xque->idleCnd, &xque->idleCndMtx, &ts);
            xque->xexu->pingCb(xque->xexu, 0);
        }
        AfxUnlockMutex(&xque->idleCndMtx);
    }
    else if (xque->waitCb(xque, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxXqueStdDtorCb(afxIoQueue xque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xque, afxFcc_XQUE);

    AfxCleanUpMutex(&xque->workChnMtx);
    AfxDismantleArena(&xque->workArena);
    AfxDismantleSlock(&xque->workArenaSlock);
    AfxCleanUpCondition(&xque->idleCnd);
    AfxCleanUpMutex(&xque->idleCndMtx);

    return err;
}

_AFX afxError _AfxXqueStdCtorCb(afxIoQueue xque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xque, afxFcc_XQUE);

    afxIoBridge xexu = args[0];
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);
    afxIoBridgeConfig const *cfg = ((afxIoBridgeConfig const *)args[1]);
    AFX_ASSERT(cfg);

    xque->portId = cfg->portId;
    xque->xexu = xexu;

    xque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&xque->workArenaSlock);
    AfxDeployArena(&xque->workArena, NIL, AfxHere());

    AfxSetUpMutex(&xque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&xque->workChn, xexu);
    AfxSetUpMutex(&xque->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&xque->idleCnd);

    xque->closed = FALSE;

    return err;
}

_AFX afxClassConfig const _AfxXqueStdImplementation =
{
    .fcc = afxFcc_XQUE,
    .name = "IoQueue",
    .desc = "I/O Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoQueue)),
    .ctor = (void*)_AfxXqueStdCtorCb,
    .dtor = (void*)_AfxXqueStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxUnit AfxGetIoBridgePort(afxIoBridge xexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);
    return xexu->portId;
}

_AFX afxUnit AfxGetIoQueues(afxIoBridge xexu, afxUnit first, afxUnit cnt, afxIoQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    /// xexu must be a valid afxIoBridge handle.
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);
    /// queues must be a valid pointer to the afxIoQueue handles.
    AFX_ASSERT(queues);
    afxUnit rslt = 0;

    afxUnit xqueCnt = xexu->queCnt;
    AFX_ASSERT_RANGE(xqueCnt, first, cnt);

    if (first < xexu->queCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxIoQueue xque = xexu->queues[first + i];
            AfxAssertObjects(1, &xque, afxFcc_XQUE);
            queues[rslt++] = xque;
        }
    }
    return rslt;
}

_AFX afxError _AfxWaitForIdleIoBridge(afxIoBridge xexu, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    /// xexu must be a valid afxIoBridge handle.
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);
    
    if (!xexu->waitCb)
    {
        if (xexu->pingCb)
            xexu->pingCb(xexu, 0);

        afxUnit queCnt = xexu->queCnt;

        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxIoQueue xque;
            AfxGetIoQueues(xexu, i, 1, &xque);
            AfxAssertObjects(1, &xque, afxFcc_XQUE);
            _AfxWaitForEmptyIoQueue(xque, timeout);
        }
    }
    else if (xexu->waitCb(xexu, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxSubmitIoCommands(afxIoBridge xexu, afxSubmission const* ctrl, afxUnit cnt, afxObject cmdbs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);
    AFX_ASSERT(cnt);
    AFX_ASSERT(cmdbs);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxUnit queCnt = AfxMin(xexu->queCnt, AfxMax(1, ctrl->queCnt));

    for (afxUnit j = 0; j < queCnt; j++)
    {
        afxIoQueue xque;
        AfxGetIoQueues(xexu, j, 1, &xque);
        AfxAssertObjects(1, &xque, afxFcc_XQUE);

        if ((err = xque->submitCb(xque, ctrl, cnt, cmdbs)))
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxSubmitTransferences(afxIoBridge xexu, afxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxUnit queCnt = AfxMin(xexu->queCnt, AfxMax(1, ctrl->queCnt));

    for (afxUnit j = 0; j < queCnt; j++)
    {
        afxIoQueue xque;
        AfxGetIoQueues(xexu, j, 1, &xque);
        AfxAssertObjects(1, &xque, afxFcc_XQUE);

        if ((err = xque->transferCb(xque, ctrl, opCnt, ops)))
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxXexuStdDtorCb(afxIoBridge xexu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = xexu->queCnt,
            .siz = sizeof(xexu->queues[0]),
            .var = (void**)&xexu->queues
        }
    };
    AfxDeallocateInstanceData(xexu, ARRAY_SIZE(stashes), stashes);

    AfxDeregisterChainedClasses(&xexu->classes);

    return err;
}

_AFX afxError _AfxXexuStdCtorCb(afxIoBridge xexu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &xexu, afxFcc_XEXU);

    afxIoBridgeConfig const *cfg = ((afxIoBridgeConfig const *)args[1]);
    AFX_ASSERT(cfg);

    if (!cfg) AfxThrowError();
    else
    {
        afxObjectStash const stashes[] =
        {
            {
                .cnt = cfg->minQueCnt,
                .siz = sizeof(xexu->queues[0]),
                .var = (void**)&xexu->queues
            }
        };

        if (AfxAllocateInstanceData(xexu, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            xexu->portId = cfg->portId;
            xexu->exuIdx = invokeNo;

            xexu->waitCb = NIL;

            xexu->queCnt = cfg->minQueCnt;
            //AFX_ASSERT(xexu->queCnt);

            AfxDeployChain(&xexu->classes, xexu);

            AfxRegisterClass(&xexu->xqueCls, NIL, &xexu->classes, &_AfxXqueStdImplementation);

            afxClass* cls2 = (afxClass*)&xexu->xqueCls;
            AfxAssertClass(cls2, afxFcc_XQUE);

            if (AfxAcquireObjects(cls2, cfg->minQueCnt, (afxObject*)xexu->queues, (void const*[]) { xexu, cfg })) AfxThrowError();
            else
            {
                AfxAssertObjects(cfg->minQueCnt, xexu->queues, afxFcc_XQUE);

            }

            if (err)
                AfxDeallocateInstanceData(xexu, ARRAY_SIZE(stashes), stashes);
        }
    }
    return err;
}

_AFX afxClassConfig const _AfxXexuStdImplementation =
{
    .fcc = afxFcc_XEXU,
    .name = "IoBridge",
    .desc = "I/O Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoBridge)),
    .ctor = (void*)_AfxXexuStdCtorCb,
    .dtor = (void*)_AfxXexuStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireIoBridge(afxIoBridgeConfig const* cfg, afxIoBridge* bridge)
{
    afxError err = AFX_ERR_NONE;
    
    // Acquire bridges and queues
    afxUnit totalDqueCnt = 0;
    afxIoBridgeConfig bridgeCfg = { 0 };

    if (cfg)
    {
        bridgeCfg = *cfg;
        bridgeCfg.minQueCnt = bridgeCfg.minQueCnt ? bridgeCfg.minQueCnt : AfxMax(1, _AfxXqueStdImplementation.unitsPerPage);            
    }
    else
    {
        bridgeCfg.minQueCnt = AfxMax(1, _AfxXqueStdImplementation.unitsPerPage);
        bridgeCfg.queuePriority = NIL;
        //bridgeCfg[i].flags = NIL;
        totalDqueCnt += bridgeCfg.minQueCnt;
    }

    afxClass* cls = (afxClass*)AfxGetIoBridgeClass();
    AfxAssertClass(cls, afxFcc_XEXU);

    if (AfxAcquireObjects(cls, 1, (afxObject*)bridge, (void const*[]) { NIL, &bridgeCfg })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, bridge, afxFcc_XEXU);
    }
    return err;
}
