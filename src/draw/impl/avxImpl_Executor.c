/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVX afxError _AvxDpuWork_CallbackCb(avxDpu* dpu, avxWork* work)
{
    afxError err = AFX_ERR_NONE;
    work->Callback.f(dpu, work->Callback.udd);
    return err;
}

_AVX afxError _AvxDpuWork_ExecuteCb(avxDpu* dpu, avxWork* work)
{
    afxError err = AFX_ERR_NONE;
    afxUnit cnt = work->Execute.cmdbCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxDrawContext dctx = work->Execute.cmdbs[i];
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
        AFX_ASSERT(dctx->state == avxCmdbState_PENDING);
        _AvxDpuRollContext(dpu, dctx);
    }
    return err;
}

_AVX avxWorkList const _AVX_DPU_WORK_VMT =
{
    .Callback = _AvxDpuWork_CallbackCb,
    .Execute = _AvxDpuWork_ExecuteCb,
};

_AVX afxBool _AvxDpu_ProcCb(avxDpu* dpu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu);

    afxDrawBridge dexu = dpu->dexu;
    avxWorkList const* workVmt = dexu->workVmt;

    afxClass const* dqueCls = _AvxGetDrawQueueClass(dexu);
    afxUnit totalQueCnt = dqueCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < totalQueCnt; queIdx++)
    {
        afxDrawQueue dque;
        if (!AfxGetDrawQueues(dexu, queIdx, 1, &dque) || !dque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (dque->portId != dexu->portId)
            continue;

        if (AfxTryLockMutex(&dque->workChnMtx))
        {
            avxWork* work;
            AfxChainForEveryLinkageB2F(&dque->workChn, avxWork, hdr.chain, work)
            {
                AFX_ASSERT(dque->workChn.cnt);
                AfxGetTime(&work->hdr.pullTime);

                AFX_ASSERT(workVmt->f[work->hdr.id]);
                workVmt->f[work->hdr.id](dpu, work);

                AfxGetTime(&work->hdr.complTime);
                _AvxDquePopWork(dque, work);
            }
            AfxUnlockMutex(&dque->workChnMtx);
            AfxSignalCondition(&dque->idleCnd);
        }
    }
    return TRUE;
}

_AVX afxInt _AVX_DPU_THREAD_PROC(afxDrawBridge dexu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == dexu->worker);

    afxDrawDevice ddev = AfxGetDrawBridgeDevice(dexu);
    afxDrawSystem dsys = AfxGetDrawBridgeContext(dexu);
    afxUnit portId = dexu->portId;

    avxDpu* dpu;
    if (AfxAllocate(sizeof(*dpu), 0, AfxHere(), (void**)&dpu))
        AfxThrowError();

    AfxZero(dpu, sizeof(*dpu));
    dpu->dexu = dexu;
    dpu->dsys = dsys;
    dpu->exuIdx = portId;
    dpu->running = TRUE;
    dpu->thr = thr;
    dexu->dpu = dpu;

    do
    {
        AfxLockMutex(&dexu->schedCndMtx);

        while (!dexu->schedCnt)
            AfxWaitCondition(&dexu->schedCnd, &dexu->schedCndMtx);

        dexu->schedCnt = 0;
        AfxUnlockMutex(&dexu->schedCndMtx);

        dexu->procCb(dpu);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    AFX_ASSERT(dpu == dexu->dpu);
    AfxDeallocate((void**)&dpu, AfxHere());

    return 0;
}

_AVX afxError _AvxDexu_PingCb(afxDrawBridge dexu, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    dexu->schedCnt += 1;
    AfxSignalCondition(&dexu->schedCnd);
    return err;
}
