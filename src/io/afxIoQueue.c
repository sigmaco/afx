/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
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
#include "afxIoDDK.h"
#include "../exec/afxSystemDDK.h"
#include "qwadro/io/afxIoBridge.h"

_AFX afxError _AfxXquePopWork(afxIoQueue xque, afxStdWork* work)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxPopLink(&work->hdr.chain);
    AfxReclaimArena(&xque->workArena, work, work->hdr.siz);
    return err;
}

_AFX afxStdWork* _AfxXquePushWork(afxIoQueue xque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    afxStdWork* work = AfxRequestArena(&xque->workArena, siz, 1, NIL, 0);
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    return xque->portId;
}

_AFX afxDevLink AfxGetIoQueueContext(afxIoQueue xque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    afxDevLink ctx = xque->ctx;
    AFX_ASSERT_OBJECTS(afxFcc_DEVK, 1, &ctx);
    return ctx;
}

_AFX afxError _AfxXqueDtorCb(afxIoQueue xque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    AfxDeregisterChainedClasses(&xque->classes);

    AfxDismantleMutex(&xque->workChnMtx);
    AfxDismantleArena(&xque->workArena);
    AfxCleanUpFutex(&xque->workArenaSlock);
    AfxDismantleCondition(&xque->idleCnd);
    AfxDismantleMutex(&xque->idleCndMtx);

    return err;
}

_AFX afxError _AfxXqueCtorCb(afxIoQueue xque, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    afxIoBridge exu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxIoBridgeConfig const *cfg = ((afxIoBridgeConfig const *)args[1]);
    AFX_ASSERT(cfg);

    xque->portId = AfxGetIoBridgePort(exu);
    xque->exu = exu;
    xque->ctx = AfxGetIoBridgeContext(exu);

    xque->immediate = 0;// !!spec->immedate;

    AfxSetUpFutex(&xque->workArenaSlock);
    AfxMakeArena(&xque->workArena, NIL, AfxHere());

    AfxDeployMutex(&xque->workChnMtx, AFX_MTX_PLAIN);
    AfxMakeChain(&xque->workChn, exu);
    AfxDeployMutex(&xque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&xque->idleCnd);

    xque->closed = FALSE;

    AfxMakeChain(&xque->classes, (void*)xque);

    return err;
}

_AFX afxClassConfig const _AfxXqueStdImplementation =
{
    .fcc = afxFcc_XQUE,
    .name = "IoQueue",
    .desc = "I/O Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoQueue)),
    .ctor = (void*)_AfxXqueCtorCb,
    .dtor = (void*)_AfxXqueDtorCb
};
