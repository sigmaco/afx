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

_AFX afxDevice AfxGetIoBridgeDevice(afxIoBridge exu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxDevice dev = AfxGetHost(exu);
    AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);
    return dev;
}

_AFX afxDevLink AfxGetIoBridgeContext(afxIoBridge exu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxDevLink ctx = exu->ctx;
    //AFX_ASSERT_OBJECTS(afxFcc_DEVK, 1, &ctx);
    return ctx;
}

_AFX afxUnit AfxGetIoBridgePort(afxIoBridge exu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    return exu->portId;
}

_AFX afxUnit AfxGetIoQueues(afxIoBridge exu, afxUnit baseQueIdx, afxUnit cnt, afxIoQueue queues[])
{
    afxError err = { 0 };
    // exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    // queues must be a valid pointer to the afxIoQueue handles.
    AFX_ASSERT(queues);
    afxUnit rslt = 0;

    afxUnit xqueCnt = exu->queCnt;
    AFX_ASSERT_RANGE(xqueCnt, baseQueIdx, cnt);

    if (baseQueIdx < exu->queCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxIoQueue xque = exu->queues[baseQueIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
            queues[rslt++] = xque;
        }
    }
    return rslt;
}

_AFX afxError _AfxWaitForEmptyIoQueue(afxIoBridge exu, afxUnit queIdx, afxTime timeout)
{
    afxError err = { 0 };
    // exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    afxIoQueue xque;
    AFX_ASSERT_RANGE(exu->queCnt, queIdx, 1);
    AfxGetIoQueues(exu, queIdx, 1, &xque);
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    if (!xque->waitCb)
    {
        AfxLockMutex(&xque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsecs = AFX_MAX(1, timeout) * 100000; // 100000 = 0.0001 sec

        while (xque->workChn.cnt)
        {
            if (exu->pingCb)
                exu->pingCb(exu, 0);

            AfxWaitTimedCondition(&xque->idleCnd, &xque->idleCndMtx, &ts);
        }
        AfxUnlockMutex(&xque->idleCndMtx);
    }
    else if (xque->waitCb(xque, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxWaitForIdleIoBridge(afxIoBridge exu, afxTime timeout)
{
    afxError err = { 0 };
    // exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    
    if (!exu->waitCb)
    {
        if (exu->pingCb)
            exu->pingCb(exu, 0);

        afxUnit queCnt = exu->queCnt;

        for (afxUnit i = 0; i < queCnt; i++)
            _AfxWaitForEmptyIoQueue(exu, i, timeout);
    }
    else if (exu->waitCb(exu, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxSubmitIoOperation(afxIoBridge exu, afxSubmission const* ctrl, afxUnit cnt, afxObject cmdbs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(cmdbs);

    // sanitize arguments
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, exu->queCnt - 1);
    afxUnit queCnt = AFX_MIN(exu->queCnt - baseQueIdx, AFX_MAX(1, ctrl->queCnt));
    AFX_ASSERT(queCnt);

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxIoQueue xque;
        if (!AfxGetIoQueues(exu, baseQueIdx + queIdx, 1, &xque)) continue;
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
            if (!AfxTryLockMutex(&xque->workChnMtx))
                continue;
        }

        afxCmdId cmdId;
        afxStdWork* work = _AfxXquePushWork(xque, AFX_GET_STD_WORK_ID(Execute), sizeof(work->Execute) + (cnt * sizeof(work->Execute.cmdbs[0])), &cmdId);
        AFX_ASSERT(work);

        AfxUnlockMutex(&xque->workChnMtx);

        exu->pingCb(exu, 0);
        break;
    }
    return err;
}

_AFX afxError _AfxExuDtorCb(afxIoBridge exu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    _AfxWaitForIdleIoBridge(exu, AFX_TIMEOUT_INFINITE);
    _AfxWaitForIdleIoBridge(exu, AFX_TIMEOUT_INFINITE); // yes, two times.

    AfxDeregisterChainedClasses(&exu->classes);
    
    _AfxWaitForIdleIoBridge(exu, AFX_TIMEOUT_INFINITE);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = exu->queCnt,
            .siz = sizeof(exu->queues[0]),
            .var = (void**)&exu->queues
        }
    };
    AfxDeallocateInstanceData(exu, ARRAY_SIZE(stashes), stashes);

    if (exu->worker)
    {
        afxInt exitCode;
        AfxWaitForThreadExit(exu->worker, &exitCode);
        AfxDisposeObjects(1, &exu->worker);
    }
    AfxDismantleCondition(&exu->schedCnd);
    AfxDismantleMutex(&exu->schedCndMtx);

    return err;
}

_AFX afxError _AfxExuCtorCb(afxIoBridge exu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    afxDevice dev = args[0];
    //AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);
    afxDevLink ctx = args[1];
    //AFX_ASSERT_OBJECTS(afxFcc_DEVK, 1, &ctx);
    afxIoBridgeConfig const *cfg = ((afxIoBridgeConfig const *)args[2]);
    AFX_ASSERT(cfg);
    afxClassConfig const* xqueClsCfg = args[3];
    AFX_ASSERT(xqueClsCfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    exu->queCnt = cfg->minQueCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = cfg->minQueCnt,
            .siz = sizeof(exu->queues[0]),
            .var = (void**)&exu->queues
        }
    };

    if (AfxAllocateInstanceData(exu, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    exu->ctx = ctx;
    exu->portId = exu->portId;
    exu->exuIdx = invokeNo;

    AfxDeployMutex(&exu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&exu->schedCnd);
    exu->schedCnt = 0;

    afxThreadConfig thrCfg = { 0 };
    //thrCfg.procCb = DrawThreadProc;
    thrCfg.usage = afxThreadUsage_IO;
    thrCfg.udd[0] = exu;
    if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &exu->worker))
        AfxThrowError();

    exu->procCb = _AfxExuStdIoProcCb;
    exu->pingCb = _AfxExuStdIoPingCb;
    exu->ctrlCb = _AfxExuStdIoCtrlCb;
    exu->workerProc = _AfxExuStdIoThreadProc;
    exu->waitCb = NIL;

    AfxMakeChain(&exu->classes, exu);

    afxClassConfig clsCfg;
    clsCfg = *xqueClsCfg;
    AfxMountClass(&exu->xqueCls, NIL, &exu->classes, &clsCfg);

    afxClass* xqueCls = (afxClass*)&exu->xqueCls;
    AFX_ASSERT_CLASS(xqueCls, afxFcc_XQUE);

    if (AfxAcquireObjects(xqueCls, exu->queCnt, (afxObject*)exu->queues, (void const*[]) { exu, cfg })) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, exu->queCnt, exu->queues);
    }

    if (err)
        AfxDeregisterChainedClasses(&exu->classes);

    if (err && AfxDeallocateInstanceData(exu, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AFX afxClassConfig const _AFX_EXU_CLASS_CONFIG =
{
    .fcc = afxFcc_EXU,
    .name = "IoBridge",
    .desc = "I/O Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoBridge)),
    .ctor = (void*)_AfxExuCtorCb,
    .dtor = (void*)_AfxExuDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireIoBridge(afxIoBridgeConfig const* cfg, afxIoBridge* bridge)
{
    afxError err = { 0 };
    
    // Acquire bridges and queues
    afxUnit totalDqueCnt = 0;
    afxIoBridgeConfig bridgeCfg = { 0 };

    if (cfg)
    {
        bridgeCfg = *cfg;
        bridgeCfg.minQueCnt = bridgeCfg.minQueCnt ? bridgeCfg.minQueCnt : AFX_MAX(1, _AfxXqueStdImplementation.unitsPerPage);            
    }
    else
    {
        bridgeCfg.minQueCnt = AFX_MAX(1, _AfxXqueStdImplementation.unitsPerPage);
        bridgeCfg.queuePriority = NIL;
        //bridgeCfg[i].flags = NIL;
        totalDqueCnt += bridgeCfg.minQueCnt;
    }

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass* cls = (afxClass*)_AfxSysGetExuClass(sys);
    AFX_ASSERT_CLASS(cls, afxFcc_EXU);

    if (AfxAcquireObjects(cls, 1, (afxObject*)bridge, (void const*[]) { NIL, NIL, &bridgeCfg, &_AfxXqueStdImplementation })) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, bridge);
    }
    return err;
}
