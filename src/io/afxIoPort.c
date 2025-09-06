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
#include "../impl/afxIoImplKit.h"
#include "qwadro/io/afxIoBridge.h"

_AFX afxError _AfxXpuStdWorkCallbackCb(afxIoBridge exu, afxStdWork* work)
{
    afxError err = AFX_ERR_NONE;
    work->Callback.f(exu, work->Callback.udd);
    return err;
}

_AFX afxError _AfxXpuStdWorkExecuteCb(afxIoBridge exu, afxStdWork* work)
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < work->Execute.cmdbCnt; i++)
    {
        afxStream iob = work->Execute.cmdbs[i];
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
        _XpuRollStreams(exu, 1, &iob);
    }
    return err;
}

_AFX afxStdWorkList _AfxStdXpuWorkCbVmt =
{
    .Callback = _AfxXpuStdWorkCallbackCb,
    .Execute = _AfxXpuStdWorkExecuteCb,
};

_AFX afxBool _AfxExuStdIoProcCb(afxIoBridge exu, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    AFX_ASSERT_OBJECTS(afxFcc_THR, 1, &thr);

    for (afxUnit queIdx = 0; queIdx < exu->queCnt; queIdx++)
    {
        afxIoQueue xque;
        if (!AfxGetIoQueues(exu, queIdx, 1, &xque) || !xque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

        if (AfxTryLockMutex(&xque->workChnMtx))
        {
            afxStdWork* work;
            AFX_ITERATE_CHAIN_B2F(afxStdWork, work, hdr.chain, &xque->workChn)
            {
                AFX_ASSERT(xque->workChn.cnt);
                AfxGetTime(&work->hdr.pullTime);

                exu->workExecFn[work->hdr.id](exu, work);

                AfxGetTime(&work->hdr.complTime);
                _AfxXquePopWork(xque, work);
            }
            AfxSignalCondition(&xque->idleCnd);
            AfxUnlockMutex(&xque->workChnMtx);
        }
    }
    return TRUE;
}

_AFX afxInt _AfxExuStdIoThreadProc(afxIoBridge exu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    afxDevice ddev = AfxGetIoBridgeDevice(exu);
    afxDevLink ctx = AfxGetIoBridgeContext(exu);
    afxUnit portId = exu->portId;

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == exu->worker);

    do
    {
        AfxLockMutex(&exu->schedCndMtx);

        while (!exu->schedCnt)
            AfxWaitCondition(&exu->schedCnd, &exu->schedCndMtx);

        exu->schedCnt = 0;
        AfxUnlockMutex(&exu->schedCndMtx);

        exu->procCb(exu, exu->worker);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    return 0;
}

_AFX afxError _AfxExuStdIoPingCb(afxIoBridge exu, afxUnit queIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    exu->schedCnt += 1;
    AfxSignalCondition(&exu->schedCnd);
    return err;
}

_AFX afxError _AfxExuStdIoCtrlCb(afxIoBridge exu, afxInt code)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    switch (code)
    {
    case 0: // start
    {
        if (!AfxIsThreadRunning(exu->worker))
        {
            AfxRunThread(exu->worker, (void*)exu->workerProc, exu);
        }
        break;
    }
    case 1: // pause/suspend
    {
        break;
    }
    case 2: // resume
    {
        break;
    }
    case 3: // stop
    {
        afxInt exitCode = 0;
        do
        {
            AfxRequestThreadInterruption(exu->worker);
            _AfxWaitForIdleIoBridge(exu, AFX_TIMEOUT_INFINITE);
        } while (!AfxWaitForThreadExit(exu->worker, &exitCode));

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}
