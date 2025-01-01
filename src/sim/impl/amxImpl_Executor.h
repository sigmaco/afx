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

#ifndef AMX_IMPL___EXECUTOR_H
#define AMX_IMPL___EXECUTOR_H

#include "qwadro/inc/sim/afxSimulation.h"

#define AMX_MAX_SIM_QUEUE_PER_BRIDGE 32

AFX_DECLARE_UNION(amxWork);
AFX_DECLARE_UNION(amxWorkList);
AFX_DECLARE_UNION(amxCmdList);

AFX_DEFINE_STRUCT(amxMpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxSimBridge    mexu;
    afxThread       thr;
    afxSimulation   sim;

    afxBool         running;
    afxBool         instanced;
    
    afxPose         pose;
    afxUnit         basXform;
    afxUnit         xformCnt;
    
    afxModel        skl;
    afxUnit         baseJnt;
    afxUnit         jntCnt;
    afxReal         allowedErr;
    afxUnit const*  sparseJntMap;
    afxM4d          displacement;

    struct
    {
        afxBuffer   buf;
        afxUnit     base;
        afxUnit     range;
        afxByte*    bytemap;
    } bufBindings[16];
    afxUnit bufBindCnt;
};

AFX_DEFINE_STRUCT(_amxSimBridgeAcquisition)
{
    afxEngine               mdev;
    afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   mqueClsCfg;
    afxClassConfig const*   mctxClsCfg;
};

#ifdef _AMX_SIM_BRIDGE_C
#ifdef _AMX_SIM_BRIDGE_IMPL
AFX_OBJECT(_amxSimBridge)
#else
AFX_OBJECT(afxSimBridge)
#endif
{
    afxEngine mdev;
    afxUnit portId;
    afxUnit exuIdx;
    afxChain classes;
    afxClass mqueCls;

    afxError(*pingCb)(afxSimBridge,afxUnit);

    // MPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt(*workerProc)(afxSimBridge);
    amxWorkList const*workVmt;
    amxCmdList const*cmdVmt;
    afxError(*endOfCtxCb)(amxMpu*);
    afxBool(*procCb)(amxMpu*);
    amxMpu*         mpu;
};
#endif//_AMX_SIM_BRIDGE_C

#ifdef _AMX_SIM_QUEUE_C
#ifdef _AMX_SIM_QUEUE_IMPL
AFX_OBJECT(_amxSimQueue)
#else
AFX_OBJECT(afxSimQueue)
#endif
{
    afxEngine       mdev;
    afxUnit         portId;
    afxUnit         exuIdx;
    afxSimulation   sim;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxCondition idleCnd;
    afxMutex idleCndMtx;
    afxSlock workArenaSlock;
    afxArena workArena; // used by submission of queue operations, not stream commands.        
    afxChain workChn;
    afxMutex workChnMtx;


    afxChain        classes;
    // one command pool per queue to avoid thread interation.
    afxClass        cmdbCls;
    // one stream manager per queue to avoid thread interaction
    afxQueue        cmdbRecycQue;
    afxSlock        cmdbReqLock;
    afxBool         cmdbLockedForReq;

    afxError(*waitCb)(afxSimQueue, afxTime);
};
#endif//_AMX_SIM_QUEUE_C

AFX_DEFINE_UNION(amxWorkList)
{
    struct
    {
        void* Callback;
        void* Execute;
    };
    void(*f[])(void*, amxWork const*);
};

#define ASX_GET_STD_WORK_ID(cmdName_) (offsetof(amxWorkList, cmdName_) / sizeof(void*))

AFX_DEFINE_STRUCT(amxWorkHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxTime complTime; // completation time
    afxError(*exec)(void*, afxSimBridge, afxUnit queIdx, amxWork*);
};

AFX_DEFINE_UNION(amxWork)
{
    amxWorkHdr hdr;
    struct
    {
        amxWorkHdr hdr;

        afxUnit         submType;
        void(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        amxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        afxFence        fence;
        afxUnit         cmdbCnt;
        afxCatalyst AFX_SIMD cmdbs[];
    } Execute;
};

AMX afxError _AmxMquePopWork(afxSimQueue mque, amxWork* work);
AMX amxWork* _AmxMquePushWork(afxSimQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId);
AMX afxError _AmxMquePopBlob(afxSimQueue mque, void* blob, afxUnit siz);
AMX void* _AmxMquePushBlob(afxSimQueue mque, afxUnit siz);

AMX afxClass const* _AmxGetCatalystClass(afxSimQueue mque);

AMX afxClass const* _AmxGetSimQueueClass(afxSimBridge mdge);
AMX afxUnit _AmxCountMathQueues(afxSimBridge mdge, afxUnit baseQueIdx);
AMX afxBool _AmxGetMathQueue(afxSimBridge mdge, afxUnit queIdx, afxSimQueue* queue);
AMX afxError _AmxWaitForIdleMathQueue(afxSimBridge mdge, afxUnit queIdx);
AMX afxError _AmxWaitForIdleMathBridge(afxSimBridge mdge);

AMX afxEngine AfxGetSimQueueDevice(afxSimQueue mque);
AMX afxSimulation AfxGetSimQueueContext(afxSimQueue mque);
AMX afxUnit AfxGetSimQueuePort(afxSimQueue mque);

AMX amxWorkList const _AMX_MPU_WORK_VMT;
AMX afxInt   _AMX_MPU_THREAD_PROC(afxSimBridge mexu);
AMX afxBool  _AmxMpu_ProcCb(amxMpu*);
AMX afxError _AmxMexu_PingCb(afxSimBridge mexu, afxUnit queIdx);

AMX afxError _AmxMpuRollContext(amxMpu* mpu, afxCatalyst cyst);

AMX afxClassConfig const _AMX_MEXU_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG;

AMX afxError _AmxSubmitSimCommands(afxSimQueue mque, avxSubmission const* ctrl, afxUnit cnt, afxCatalyst contexts[]);

#endif//AMX_IMPL___EXECUTOR_H
