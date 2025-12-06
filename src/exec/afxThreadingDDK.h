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

#ifndef AFX_THREADING_H
#define AFX_THREADING_H

#include "qwadro/exec/afxAtomic.h"
#include "qwadro/exec/afxFutex.h"
#include "qwadro/exec/afxMutex.h"
#include "qwadro/exec/afxCondition.h"
#include "qwadro/exec/afxThread.h"
#include "qwadro/exec/afxSystem.h"

#ifdef _AFX_SEMAPHORE_C
#ifdef _AFX_SEMAPHORE_IMPL
AFX_OBJECT(_afxSemaphore)
#else
AFX_OBJECT(afxSemaphore)
#endif
{
    afxUnit64            value;
};
#endif//_AFX_SEMAPHORE_C

#ifdef _AFX_THREAD_C
AFX_OBJECT(afxThread)
{
    afxUnit              unitIdx;
    afxUnit32            tid;
    void*               osHandle;
    //afxThreadProc       procCb;
    void*               udd[4];

    afxThreadUsage      usage;
    //afxUnit          affineProcUnitIdx; // if not equal to AFX_INVALID_INDEX, this thread can be ran by any system processor unit, else case, will only be ran by the unit specified by this index.
    //afxUnit          affineThrUnitIdx; // if set bit set, only such processor will can run this thread.
    afxClock            startClock;
    afxClock            lastClock;
    afxClock            execCntSwapClock;
    afxUnit              lastExecCnt;
    afxUnit              execNo;
    afxBool             started;
    afxBool             exited;
    afxBool             running;
    afxUnit              suspendCnt;
    afxBool             isInFinish;
    afxBool             finished;
    afxBool             interruptionRequested;
    afxInt              exitCode;

    afxCondition        statusCnd;
    afxMutex            statusCndMtx;

    afxFutex            evSlock;
    afxArena            evArena;
    afxQueue            events;
    afxInterlockedQueue events2;
    afxBool             isUxThread;

    afxChar const*      _file_;
    afxSize             _line_;
    afxChar const*      _func_;

#ifdef _WIN32
#ifdef _INC_WINDOWS
    HANDLE              hAvrt;
    DWORD               avrtIdx;
    //AVRT_PRIORITY       avrtPrio;
#endif
#endif
};
#endif//_AFX_THREAD_C

#ifdef _AFX_THREAD_POOL_C
AFX_OBJECT(afxThreadPool)
{
    afxMutex lock;
    afxCondition notify;
    afxThread* threads;
    afxThreadedTask* workQue;
    afxInt thrCnt;
    afxInt workQueCap;
    afxInt head;
    afxInt tail;
    afxInt cnt;
    afxBool shutdown;
    afxInt started;
};

#endif//_AFX_THREAD_POOL_C

AFX afxClassConfig const _AFX_THR_CLASS_CONFIG;
AFX afxClassConfig const _AFX_THRP_CLASS_CONFIG;

AFX afxClass*           _AfxGetSemaphoreClass(afxDevLink ctx);

AFX afxClassConfig const _AfxSemStdImplementation;

AFX afxClass const*     _AfxSysGetThrClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetThrpClass(afxSystem sys);

#endif//AFX_THREADING_H
