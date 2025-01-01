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

// This section is part of SIGMA Simulation Infrastructure.

#define _AMX_SIM_QUEUE_C
#define _AMX_SIM_BRIDGE_C
#include "afx/src/sim/impl/amxImplementation.h"

_AMX afxError _AmxMpuWork_CallbackCb(amxMpu* mpu, amxWork* work)
{
    afxError err = AFX_ERR_NONE;
    work->Callback.f(mpu, work->Callback.udd);
    return err;
}

_AMX afxError _AmxMpuWork_ExecuteCb(amxMpu* mpu, amxWork* work)
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < work->Execute.cmdbCnt; i++)
    {
        afxCatalyst mctx = work->Execute.cmdbs[i];
        AFX_ASSERT_OBJECTS(afxFcc_CYST, 1, &mctx);
        _AmxMpuRollContext(mpu, mctx);
    }
    return err;
}

_AMX amxWorkList const _AMX_MPU_WORK_VMT =
{
    .Callback = _AmxMpuWork_CallbackCb,
    .Execute = _AmxMpuWork_ExecuteCb,
};

_AMX afxBool _AmxMpu_ProcCb(amxMpu* mpu)
{
    afxError err = AFX_ERR_NONE;

    afxSimBridge mexu = mpu->mexu;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    amxWorkList const* workVmt = mexu->workVmt;

    afxClass const* mqueCls = _AmxGetSimQueueClass(mexu);
    afxUnit queCnt = mqueCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxSimQueue mque;
        if (!AfxGetSimQueues(mexu, queIdx, 1, &mque) || !mque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

        if (AfxTryLockMutex(&mque->workChnMtx))
        {
            amxWork* work;
            AfxChainForEveryLinkageB2F(&mque->workChn, amxWork, hdr.chain, work)
            {
                AFX_ASSERT(mque->workChn.cnt);
                AfxGetTime(&work->hdr.pullTime);

                workVmt->f[work->hdr.id](mpu, work);

                AfxGetTime(&work->hdr.complTime);
                _AmxMquePopWork(mque, work);
            }
            AfxUnlockMutex(&mque->workChnMtx);
            AfxSignalCondition(&mque->idleCnd);
        }
    }
    return TRUE;
}

_AMX afxInt _AMX_MPU_THREAD_PROC(afxSimBridge mexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == mexu->worker);

    afxSimulation sim = AfxGetMathBridgeContext(mexu);
    afxUnit portId = mexu->portId;

    amxMpu* mpu;
    if (AfxAllocate(sizeof(*mpu), 0, AfxHere(), (void**)&mpu))
        AfxThrowError();

    AfxZero(mpu, sizeof(*mpu));
    mpu->mexu = mexu;
    mpu->sim = sim;
    mpu->exuIdx = portId;
    mpu->running = TRUE;
    mpu->thr = thr;

    AfxDbgLogf(6, NIL, "Sim Execution Unit %.03.", portId);

    do
    {
#if 0
        AfxLockMutex(&mexu->schedCndMtx);

        while (!mexu->schedCnt)
            AfxWaitCondition(&mexu->schedCnd, &mexu->schedCndMtx);

        mexu->schedCnt = 0;
        AfxUnlockMutex(&mexu->schedCndMtx);
#endif

        mexu->procCb(mpu);

        AfxSleep(1);

        //_ZalProcessDeletionQueue(al, &mexu->deletionQueue);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    AFX_ASSERT(mpu == mexu->mpu);
    AfxDeallocate((void**)&mpu, AfxHere());

    return 0;
}

_AMX afxError _AmxMexu_PingCb(afxSimBridge mexu, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
    mexu->schedCnt += 1;
    AfxSignalCondition(&mexu->schedCnd);
    return err;
}
