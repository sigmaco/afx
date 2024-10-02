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
#define _AMX_MATH_DEVICE_C
#define _AMX_MATH_BRIDGE_C
#define _AMX_MATH_QUEUE_C
#include "../../dev/AmxImplKit.h"

_AMX afxComboDevice AfxGetMathBridgeDevice(afxComboBridge mdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxComboDevice ddev = AfxGetProvider(mdge);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AMX afxSimulation AfxGetMathBridgeContext(afxComboBridge mdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxSimulation sim = mdge->sim;
    AfxTryAssertObjects(1, &sim, afxFcc_SIM);
    return sim;
}

_AMX afxNat AfxGetMathBridgePort(afxComboBridge mdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    return mdge->portIdx;
}

_AMX afxClass const* _AmxGetMathQueueClass(afxComboBridge mdge)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxClass const* cls = &mdge->mqueCls;
    AfxAssertClass(cls, afxFcc_MQUE);
    return cls;
}

_AMX afxNat _AmxCountMathQueues(afxComboBridge mdge, afxNat baseQueIdx)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    AfxAssertRange(mdge->queCnt, baseQueIdx, 1);
    return mdge->queCnt - baseQueIdx;
}

_AMX afxBool _AmxGetMathQueue(afxComboBridge mdge, afxNat queIdx, afxMathQueue* queue)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    /// queIdx must be less than the value of queCnt for the bridge indicated by portIdx when device context was created.
    AfxAssertRange(mdge->queCnt, queIdx, 1);
    afxBool rslt;

    if (!(rslt = (queIdx < mdge->queCnt))) AfxThrowError();
    else
    {
        afxMathQueue mque = mdge->queues[queIdx];
        AfxAssertObjects(1, &mque, afxFcc_MQUE);
        /// queue must be a valid pointer to a afxMathQueue handle.        
        *queue = mque;
    }
    return rslt;
}

_AMX afxError _AmxWaitForIdleMathQueue(afxComboBridge mdge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxMathQueue mque;

    if (!_AmxGetMathQueue(mdge, queIdx, &mque)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &mque, afxFcc_MQUE);

        if (!mdge->waitCb)
        {
            AfxLockMutex(&mque->idleCndMtx);

            while (mque->workChn.cnt)
                AfxWaitCondition(&mque->idleCnd, &mque->idleCndMtx);

            AfxUnlockMutex(&mque->idleCndMtx);
        }
        else if (mdge->waitCb(mdge, queIdx))
            AfxThrowError();
    }
    return err;
}

_AMX afxError _AmxWaitForIdleMathBridge(afxComboBridge mdge)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxNat queCnt = mdge->queCnt;

    for (afxNat i = 0; i < queCnt; i++)
        _AmxWaitForIdleMathQueue(mdge, i);
    
    return err;
}

_AMX void _AmxBeginMathQueueDebugScope(afxComboBridge mdge, afxNat queIdx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxNat queCnt = mdge->queCnt;
    AfxAssertRange(queCnt, queIdx, 1);
    

}

_AMX void _AmxEndMathQueueDebugScope(afxComboBridge mdge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxNat queCnt = mdge->queCnt;
    AfxAssertRange(queCnt, queIdx, 1);
    
}

_AMX void _AmxPushMathQueueDebugLabel(afxComboBridge mdge, afxNat queIdx, afxString const* name, afxColor const color)
{
    afxError err = AFX_ERR_NONE;
    /// mdge must be a valid afxComboBridge handle.
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxNat queCnt = mdge->queCnt;
    AfxAssertRange(queCnt, queIdx, 1);
    
    
}

_AMX afxNat _AmxSubmitMathCommands(afxComboBridge mdge, amxSubmission const* ctrl, afxNat cnt, avxCmdb cmdbs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    AfxAssert(cnt);
    AfxAssert(cmdbs);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = mdge->submitCb(mdge, ctrl, cnt, cmdbs)))
        AfxThrowError();

    return queIdx;
}

_AMX afxNat _AmxSubmitMathWorkRequest(afxComboBridge mdge, afxNat cnt, amxQueueOp const subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    AfxAssert(cnt);
    AfxAssert(subm);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = mdge->submCb(mdge, NIL, cnt, subm)))
        AfxThrowError();

    return queIdx;
}

_AMX afxError _AmxMqueStdDtorCb(afxMathQueue mque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mque, afxFcc_MQUE);

    AfxCleanUpMutex(&mque->workChnMtx);
    AfxDismantleArena(&mque->workArena);
    AfxDismantleSlock(&mque->workArenaSlock);
    AfxCleanUpCondition(&mque->idleCnd);
    AfxCleanUpMutex(&mque->idleCndMtx);

    return err;
}

_AMX afxError _AmxMdgeStdDtorCb(afxComboBridge mdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);

    AfxWaitForSimulation(mdge->sim, AFX_INVALID_INDEX, 0, 0);
    AfxWaitForSimulation(mdge->sim, AFX_INVALID_INDEX, 0, 0); // yes, two times.

    afxComboDevice mdev = AfxGetMathBridgeDevice(mdge);

    if (mdev->closeMdgeCb && mdev->closeMdgeCb(mdev, mdge))
        AfxThrowError();

    AfxAssertObjects(mdge->queCnt, mdge->queues, afxFcc_MQUE);
    AfxReleaseObjects(mdge->queCnt, mdge->queues);
    AfxDeregisterChainedClasses(&mdge->classes);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdge->queCnt,
            .siz = sizeof(mdge->queues[0]),
            .var = (void**)&mdge->queues
        }
    };
    AfxDeallocateInstanceData(mdge, AFX_COUNTOF(stashes), stashes);

    return err;
}

_AMX afxError _AmxMqueStdCtorCb(afxMathQueue mque, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mque, afxFcc_MQUE);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxComboBridge mdge = args[1];
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);
    afxComboBridgeConfig const *cfg = ((afxComboBridgeConfig const *)args[2]);
    AfxAssert(cfg);

    mque->mdge = mdge;
    mque->sim = sim;

    mque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&mque->workArenaSlock);
    AfxDeployArena(&mque->workArena, NIL, AfxHere());

    AfxSetUpMutex(&mque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&mque->workChn, mdge);
    AfxSetUpMutex(&mque->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&mque->idleCnd);

    mque->closed = FALSE;

    return err;
}

_AMX afxError _AmxMdgeStdCtorCb(afxComboBridge mdge, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdge, afxFcc_MDGE);

    afxComboDevice mdev = args[0];
    AfxAssertObjects(1, &mdev, afxFcc_MDEV);
    afxSimulation sim = args[1];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxComboBridgeConfig const *cfg = ((afxComboBridgeConfig const *)args[2]);
    AfxAssert(cfg);
    
    AfxAssertRange(AfxCountPragmaPorts(mdev), cfg->portIdx, 1);
    mdge->portIdx = cfg->portIdx;
    mdge->sim = sim;
    
    mdge->waitCb = NIL;
    mdge->submitCb = NIL;

    mdge->queCnt = AfxMax(1, AfxMax(_AmxMqueStdImplementation.unitsPerPage, cfg->queueCnt));

    AfxDeployChain(&mdge->classes, mdge);

    afxClassConfig clsCfg = _AmxMqueStdImplementation;
    clsCfg.maxCnt = mdge->queCnt;
    clsCfg.unitsPerPage = mdge->queCnt;
    AfxRegisterClass(&mdge->mqueCls, NIL, &mdge->classes, &clsCfg);
    // what about size of device-dependent draw queues?

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdge->queCnt,
            .siz = sizeof(mdge->queues[0]),
            .var = (void**)&mdge->queues
        }
    };

    if (AfxAllocateInstanceData(mdge, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
    else
    {
        if (mdev->openMdgeCb && mdev->openMdgeCb(mdev, mdge, cfg)) AfxThrowError();
        else
        {
            afxClass* cls = (afxClass*)_AmxGetMathQueueClass(mdge);
            AfxAssertClass(cls, afxFcc_MQUE);

            if (AfxAcquireObjects(cls, mdge->queCnt, (afxObject*)mdge->queues, (void const*[]) { sim, mdge, cfg })) AfxThrowError();
            else
            {
                AfxAssertObjects(mdge->queCnt, mdge->queues, afxFcc_MQUE);
            }

            if (err && mdev->closeMdgeCb && mdev->closeMdgeCb(mdev, mdge))
                AfxThrowError();
        }

        if (err)
            AfxDeallocateInstanceData(mdge, AFX_COUNTOF(stashes), stashes);
    }
    
    if (err)
        AfxDeregisterChainedClasses(&mdge->classes);

    return err;
}

_AMX afxClassConfig const _AmxMqueStdImplementation =
{
    .fcc = afxFcc_MQUE,
    .name = "MathQueue",
    .desc = "Math Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxMathQueue)),
    .ctor = (void*)_AmxMqueStdCtorCb,
    .dtor = (void*)_AmxMqueStdDtorCb
};

_AMX afxClassConfig const _AmxMdgeStdImplementation =
{
    .fcc = afxFcc_MDGE,
    .name = "MathBrige",
    .desc = "Math Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxComboBridge)),
    .ctor = (void*)_AmxMdgeStdCtorCb,
    .dtor = (void*)_AmxMdgeStdDtorCb
};
