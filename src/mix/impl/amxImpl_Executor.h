/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                     Q W A D R O   S O U N D   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AMX_IMPL___EXECUTOR_H
#define AMX_IMPL___EXECUTOR_H

#include "qwadro/inc/mix/afxMixSystem.h"

AFX_DECLARE_UNION(amxWork);
AFX_DECLARE_UNION(amxWorkList);

AFX_DEFINE_STRUCT(amxMpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxMixBridge  mexu;
    afxThread       thr;
    afxMixSystem  msys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedSamples;

    afxMixContext        mix;
    afxReal         bufA[48000];
    afxReal         bufB[48000];
    
    afxUnit         samplesPerFrame;
    afxUnit         tmpBufCap;
    afxAudio        a, b;

    afxUnit         mixFreq; // the mix frequency used in a mix scope.
    afxSink         sinkBuf; // the audio's sink buffer object.
    afxUnit         sinkBufIdx;
    afxUnit         sinkBaseSamp;
    afxUnit         sinkSampCnt;
    afxUnit         sinkBaseChan;
    afxUnit         sinkChanCnt;
    amxMixSink      sinkChans[32]; // our SPU supports at least 32 sink channels.
    afxSampleContext  sampCtx;
};

AFX_DEFINE_STRUCT(_amxMixBridgeAcquisition)
{
    afxMixDevice          mdev;
    afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   squeClsCfg;
    afxClassConfig const*   mixClsCfg;
};

#ifdef _AMX_MIX_BRIDGE_C
#ifdef _AMX_MIX_BRIDGE_IMPL
AFX_OBJECT(_amxMixBridge)
#else
AFX_OBJECT(afxMixBridge)
#endif
{
    afxMixDevice  mdev;
    afxUnit         portId;
    afxUnit         exuIdx;
    
    afxChain        classes;
    afxClass        squeCls;

    afxChain        activeMixers;

    afxError        (*pingCb)(afxMixBridge, afxUnit);

    // SPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxMixBridge);
    amxWorkList const*workVmt;
    afxBool         (*procCb)(amxMpu*);
    amxMpu*         mpu;
};
#endif//_AMX_MIX_BRIDGE_C

#ifdef _AMX_MIX_QUEUE_C
#ifdef _AMX_MIX_QUEUE_IMPL
AFX_OBJECT(_amxMixQueue)
#else
AFX_OBJECT(afxMixQueue)
#endif
{
    afxMixDevice    mdev;
    afxUnit         portId;
    afxUnit         exuIdx;
    afxMixSystem    msys;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxSlock        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxError(*waitCb)(afxMixQueue, afxTime);
    //afxError(*submCb)(afxMixQueue, avxFence fenc, afxUnit cnt, amxQueueOp const req[]);
};
#endif//_AMX_MIX_QUEUE_C

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
    afxError(*exec)(void*, afxMixBridge, afxUnit queIdx, amxWork*);
};

AFX_DEFINE_UNION(amxWorkList)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Sink;
        void* Remap;
    };
    void(*f[])(void*, amxWork const*);
};

#define AMX_GET_STD_WORK_ID(cmdName_) (offsetof(amxWorkList, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(amxWork)
{
    amxWorkHdr hdr;
    struct
    {
        amxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        avxFence        fence;
        afxUnit         mixCnt;
        afxMixContext AFX_SIMD mixers[];
    } Execute;
    struct
    {
        amxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        avxFence        fence;

        union
        {
            afxAudio    wav;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            afxAudio    wav;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               dst;
        afxFcc          dstFcc;
        afxCodec        codec;
        afxUnit         decSiz;
        afxUnit         opCnt;
        afxWaveIo AFX_SIMD wavOps[];
    } Transfer;
    struct
    {
        amxWorkHdr hdr;

        afxSemaphore    wait;
        afxSink         sink;
        afxAudio        buf;
        afxWaveInterval seg;
    } Sink;
    struct
    {
        amxWorkHdr hdr;

        afxAudio        wav;
        afxSize         off;
        afxUnit         ran;
        afxFlags        flags;
        void**          placeholder;
    } Remap;
    struct
    {
        amxWorkHdr hdr;

        afxUnit         submType;
        void(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
};

AMX afxError _AmxSquePopWork(afxMixQueue mque, amxWork* work);
AMX amxWork* _AmxSquePushWork(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId);
AMX afxError _AmxSquePopBlob(afxMixQueue mque, void* blob, afxUnit siz);
AMX void* _AmxSquePushBlob(afxMixQueue mque, afxUnit siz);

AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MEXU_CLASS_CONFIG;

AMX afxClass const* _AmxGetMixQueueClass(afxMixBridge mexu);

// standard sound port methods

AMX amxWorkList const _AMX_MPU_WORK_VMT;
AMX afxInt   _AMX_MPU_THREAD_PROC(afxMixBridge mexu);
AMX afxBool  _AmxMpu_ProcCb(amxMpu*);
AMX afxError _AmxMexu_PingCb(afxMixBridge mexu, afxUnit queIdx);

AMX afxError _AmxMpuRollMixContexts(amxMpu* mpu, afxMixContext mix);

// Common queued operations

AMX afxError _AmxSubmitCallback(afxMixBridge mexu, void(*f)(void*, void*), void* udd);
AMX afxError _AmxSubmitSoundCommands(afxMixBridge mexu, amxSubmission const* ctrl, afxUnit cnt, amxTracker trackers[]);
AMX afxError _AmxSubmitTransferences(afxMixBridge mexu, amxTransference const* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxSubmitSink(afxMixBridge mexu, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg);
AMX afxError _AmxRollMixers(afxMixBridge mexu, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixContext mixers[]);

#endif//AMX_IMPL___EXECUTOR_H
