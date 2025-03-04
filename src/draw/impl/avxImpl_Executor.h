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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_IMPL___EXECUTOR_H
#define AVX_IMPL___EXECUTOR_H

#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/mem/afxInterlockedQueue.h"
#include "qwadro/inc/mem/afxSlabAllocator.h"

AFX_DECLARE_UNION(avxWork);
AFX_DECLARE_UNION(avxWorkList);

AFX_DEFINE_STRUCT(avxDpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxDrawBridge   dexu;
    afxThread       thr;
    afxDrawSystem   dsys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedVertices;
    afxSize         numOfFedIndices;
    afxSize         numOfFedInstances;

    // draw scope
    avxCanvas           canv;
    afxRect             area; //  the area that is affected by the draw scope.
    afxUnit             baseLayer; // the index of the first attachment layer that will be drawn.
    afxUnit             layerCnt; // the number of layers drawn to in each attachment when viewMask is 0.
    afxUnit             targetCnt; // the number of video surfaces.
    avxDrawTarget       targets[8]; // structures describing any color attachments used.
    afxBool             useDepth;
    avxDrawTarget       depth; // structure describing a depth attachment.
    afxBool             useStencil;
    avxDrawTarget       stencil; // structure describing a stencil attachment.

    // transform scope
    afxUnit     vpCnt;
    afxViewport vps[8];
    afxRect     scissors[8];
};

AFX_DEFINE_STRUCT(_avxDrawBridgeAcquisition)
{
    afxDrawDevice           ddev;
    afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   dqueClsCfg;
    afxClassConfig const*   dctxClsCfg;
};

#ifdef _AVX_DRAW_BRIDGE_C
#ifdef _AVX_DRAW_BRIDGE_IMPL
AFX_OBJECT(_avxDrawBridge)
#else
AFX_OBJECT(afxDrawBridge)
#endif
{
    afxDrawDevice   ddev;
    afxUnit         portId;
    afxUnit         exuIdx;    
    afxChain        classes;
    afxClass        dqueCls;

    afxError(*pingCb)(afxDrawBridge, afxUnit);

    // DPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxDrawBridge);
    avxWorkList const*workVmt;
    avxCmdList const*cmdVmt;
    afxBool         (*procCb)(avxDpu*);
    avxDpu*         dpu;
};
#endif//_AVX_DRAW_BRIDGE_C

#ifdef _AVX_FENCE_C
#ifdef _AVX_FENCE_IMPL
AFX_OBJECT(_avxFence)
#else
AFX_OBJECT(avxFence)
#endif
{
    afxAtom32 signaled;
};
#endif//_AVX_FENCE_C

#ifdef _AVX_DRAW_QUEUE_C
#ifdef _AVX_DRAW_QUEUE_IMPL
AFX_OBJECT(_avxDrawQueue)
#else
AFX_OBJECT(afxDrawQueue)
#endif
{
    afxDrawDevice   ddev;
    afxUnit         portId;
    afxUnit         exuIdx;
    afxDrawSystem   dsys; // owner context
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;
    afxSlock        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxChain        classes;
    // one command pool per queue to avoid thread interation.
    afxClass        cmdbCls;
    // one stream manager per queue to avoid thread interaction
    afxQueue        cmdbRecycQue;
    afxSlock        cmdbReqLock;
    afxBool         cmdbLockedForReq;

    afxError(*waitCb)(afxDrawQueue, afxTime);
};
#endif//_AVX_DRAW_QUEUE_C

AFX_DEFINE_UNION(avxWorkList)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Present;
        void* Remap;
        void* SyncMaps;
    };
    void(*f[])(void*, avxWork*);
};

#define AVX_GET_STD_WORK_ID(cmdName_) (offsetof(avxWorkList, cmdName_) / sizeof(void*))

AFX_DEFINE_STRUCT(avxWorkHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxTime complTime; // completation time
    afxError(*exec)(void*, afxDrawBridge, afxUnit queIdx, avxWork*);
    avxFence completionFence;
    afxSize idd[4];
};

AFX_DEFINE_UNION(avxWork)
{
    avxWorkHdr hdr;
    struct
    {
        avxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        //avxFence        fence;
        afxUnit         cmdbCnt;
        afxDrawContext AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        avxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        //avxFence        fence;

        union
        {
            afxRaster   ras;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            afxRaster   ras;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               dst;
        afxFcc          dstFcc;
        afxCodec        codec;
        afxUnit         decSiz;
        afxUnit         opCnt;
        union
        {
            afxRasterIo AFX_SIMD rasOps[];
            avxBufferIo AFX_SIMD bufOps[];
        };
    } Transfer;
    struct
    {
        avxWorkHdr hdr;

        afxSemaphore    wait;
        afxUnit         opCnt;
        struct
        {
            afxDrawOutput   dout;
            afxUnit         bufIdx;
        } AFX_SIMD          ops[];
    } Present;
    struct
    {
        avxWorkHdr hdr;
        afxUnit     mapCnt;
        afxUnit     unmapCnt;
        union
        {
            avxBufferRemap mapOps[];
            avxBuffer unmapOps[];
        };
    } Remap;
    struct
    {
        avxWorkHdr  hdr;
        afxUnit     baseFlushIdx;
        afxUnit     flushCnt;
        afxUnit     baseFetchIdx;
        afxUnit     fetchCnt;
        union
        {
            avxBufferMap ops[];
        };
    } SyncMaps;
    struct
    {
        avxWorkHdr hdr;

        afxUnit         submType;
        void            (*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        avxWorkHdr hdr;

        afxBool waitAll;
        afxUnit64 timeout;
        afxUnit cnt;
        avxFence fences[];
    } WaitForFences;
};

AVX afxClass const* _AvxGetDrawThreadClass(afxDrawBridge dexu);
AVX afxClass const* _AvxGetDrawQueueClass(afxDrawBridge dexu);
AVX afxClassConfig const _AVX_DQUE_CLASS_CONFIG;
AVX afxClassConfig const _AVX_DEXU_CLASS_CONFIG;
AVX afxClassConfig const _AVX_DTHR_CLASS_CONFIG;
AVX afxClassConfig const _AVX_FENC_CLASS_CONFIG;

AVX afxError _AvxUnlockDrawQueue(afxDrawQueue dque);
AVX afxError _AvxLockDrawQueue(afxDrawQueue dque, afxBool wait, afxTimeSpec const* ts);
AVX afxError _AvxDquePopWork(afxDrawQueue dque, avxWork* work);
AVX avxWork* _AvxDquePushWork(afxDrawQueue dque, afxUnit id, afxUnit siz, afxCmdId* cmdId);

//AVX afxUnit _AvxCountDrawQueues(afxDrawBridge dexu, afxUnit baseQueIdx);
AVX afxError AfxWaitForEmptyDrawQueue(afxDrawQueue dque, afxTime timeout);
AVX afxError AfxWaitForIdleDrawBridge(afxDrawBridge dexu, afxTime timeout);

AVX afxClass const* _AvxGetDrawBatchClass(afxDrawQueue dque);

// standard sound port methods

AVX avxWorkList const _AVX_DPU_WORK_VMT;
AVX avxCmdList const _AVX_DPU_CMD_VMT;

AVX afxError _AvxDpuWork_CallbackCb(avxDpu* dpu, avxWork* work);
AVX afxError _AvxDpuWork_ExecuteCb(avxDpu* dpu, avxWork* work);

AVX afxInt   _AVX_DPU_THREAD_PROC(afxDrawBridge dexu);
AVX afxBool  _AvxDpu_ProcCb(avxDpu* dpu);
AVX afxError _AvxDexu_PingCb(afxDrawBridge dexu, afxUnit queIdx);

AVX afxError _AvxDpuRollContext(avxDpu* dpu, afxDrawContext dctx);

// Common queued operations
AVX afxError _AvxExecuteDrawCommands(afxDrawQueue dque, avxSubmission const* ctrl, afxUnit cnt, afxDrawContext contexts[], avxFence fence);
AVX afxError _AvxSubmitTransferences(afxDrawQueue dque, avxTransference const* ctrl, afxUnit opCnt, void const* ops);
AVX afxError _AvxSubmitRemapping(afxDrawQueue dque, afxUnit mapCnt, avxBufferRemap const maps[], afxUnit unmapCnt, avxBuffer const unmaps[]);
AVX afxError _AvxSubmitSyncMaps(afxDrawQueue dque, afxUnit flushCnt, avxBufferMap const flushes[], afxUnit fetchCnt, avxBufferMap const fetches[]);
AVX afxError _AvxSubmitCallback(afxDrawQueue dque, void(*f)(void*, void*), void* udd);

AVX void _AvxBeginDrawQueueDebugScope(afxDrawQueue dque, afxString const* name, afxColor const color);
AVX void _AvxPushDrawQueueDebugLabel(afxDrawQueue dque, afxString const* name, afxColor const color);
AVX void _AvxEndDrawQueueDebugScope(afxDrawQueue dque);

#endif//AVX_IMPL___EXECUTOR_H
