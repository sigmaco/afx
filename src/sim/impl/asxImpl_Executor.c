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

#define _ASX_SIM_QUEUE_C
#define _ASX_SIM_BRIDGE_C
#include "afx/src/sim/impl/asxImplementation.h"

_ASX afxError _AsxSpuWork_CallbackCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;
    work->Callback.f(spu, work->Callback.udd);
    return err;
}

_ASX afxError _AsxSpuWork_ExecuteCb(asxSpu* spu, asxWork* work)
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < work->Execute.cmdbCnt; i++)
    {
        afxContext sctx = work->Execute.cmdbs[i];
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &sctx);
        _AsxSpuRollContext(spu, sctx);
    }
    return err;
}

_ASX asxWorkList const _ASX_SPU_WORK_VMT =
{
    .Callback = _AsxSpuWork_CallbackCb,
    .Execute = _AsxSpuWork_ExecuteCb,
};

_ASX afxBool _AsxSpu_ProcCb(asxSpu* spu)
{
    afxError err = AFX_ERR_NONE;

    afxSimBridge sexu = spu->sexu;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    asxWorkList const* workVmt = sexu->workVmt;

    afxClass const* squeCls = _AsxGetSimQueueClass(sexu);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxSimQueue sque;
        if (!AfxGetSimQueues(sexu, queIdx, 1, &sque) || !sque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        if (AfxTryLockMutex(&sque->workChnMtx))
        {
            asxWork* work;
            AFX_ITERATE_CHAIN_B2F(asxWork, work, hdr.chain, &sque->workChn)
            {
                AFX_ASSERT(sque->workChn.cnt);
                AfxGetTime(&work->hdr.pullTime);

                workVmt->f[work->hdr.id](spu, work);

                AfxGetTime(&work->hdr.complTime);
                _AsxSquePopWork(sque, work);
            }
            AfxSignalCondition(&sque->idleCnd);
            AfxUnlockMutex(&sque->workChnMtx);
        }
    }
    return TRUE;
}

_ASX afxInt _ASX_SPU_THREAD_PROC(afxSimBridge sexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == sexu->worker);

    afxSimulation sim = AfxGetMathBridgeContext(sexu);
    afxUnit portId = sexu->portId;

    asxSpu* spu;
    if (AfxAllocate(sizeof(*spu), 0, AfxHere(), (void**)&spu))
        AfxThrowError();

    AfxZero(spu, sizeof(*spu));
    spu->sexu = sexu;
    spu->sim = sim;
    spu->exuIdx = portId;
    spu->running = TRUE;
    spu->thr = thr;

    AfxReportf(6, NIL, "Sim Execution Unit %.03.", portId);

    do
    {
#if 0
        AfxLockMutex(&sexu->schedCndMtx);

        while (!sexu->schedCnt)
            AfxWaitCondition(&sexu->schedCnd, &sexu->schedCndMtx);

        sexu->schedCnt = 0;
        AfxUnlockMutex(&sexu->schedCndMtx);
#endif

        sexu->procCb(spu);

        AfxSleep(1);

        //_ZalProcessDeletionQueue(al, &sexu->deletionQueue);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    AFX_ASSERT(spu == sexu->spu);
    AfxDeallocate((void**)&spu, AfxHere());

    return 0;
}

_ASX afxError _AsxSexu_PingCb(afxSimBridge sexu, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    sexu->schedCnt += 1;
    AfxSignalCondition(&sexu->schedCnd);
    return err;
}
