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
#include "qwadro/inc/io/afxIoBridge.h"

_AFX afxError _AfxXquePopWork(afxIoQueue xque, afxStdWork* work)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxPopLink(&work->hdr.chain);
    AfxRecycleArenaUnit(&xque->workArena, work, work->hdr.siz);
    return err;
}

_AFX afxStdWork* _AfxXquePushWork(afxIoQueue xque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    afxStdWork* work = AfxRequestArenaUnit(&xque->workArena, siz);
    AFX_ASSERT(work);
    work->hdr.id = id;
    work->hdr.siz = siz;
    AfxGetTime(&work->hdr.pushTime);
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&work->hdr.chain, &xque->workChn);
    return work;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxUnit AfxGetIoQueuePort(afxIoQueue xque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    return xque->portId;
}

_AFX afxDevLink AfxGetIoQueueContext(afxIoQueue xque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    afxDevLink ctx = xque->ctx;
    AFX_ASSERT_OBJECTS(afxFcc_DEVK, 1, &ctx);
    return ctx;
}

_AFX afxError _AfxXqueStdDtorCb(afxIoQueue xque)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    AfxDeregisterChainedClasses(&xque->classes);

    AfxDismantleMutex(&xque->workChnMtx);
    AfxDismantleArena(&xque->workArena);
    AfxDismantleSlock(&xque->workArenaSlock);
    AfxDismantleCondition(&xque->idleCnd);
    AfxDismantleMutex(&xque->idleCndMtx);

    return err;
}

_AFX afxError _AfxXqueStdCtorCb(afxIoQueue xque, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    afxIoBridge exu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxIoBridgeConfig const *cfg = ((afxIoBridgeConfig const *)args[1]);
    AFX_ASSERT(cfg);

    xque->portId = AfxGetIoBridgePort(exu);
    xque->exu = exu;
    xque->ctx = AfxGetIoBridgeContext(exu);

    xque->immediate = 0;// !!spec->immedate;

    AfxDeploySlock(&xque->workArenaSlock);
    AfxDeployArena(&xque->workArena, NIL, AfxHere());

    AfxDeployMutex(&xque->workChnMtx, AFX_MTX_PLAIN);
    AfxDeployChain(&xque->workChn, exu);
    AfxDeployMutex(&xque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&xque->idleCnd);

    xque->closed = FALSE;

    AfxDeployChain(&xque->classes, (void*)xque);

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
