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

#ifndef ASX_IMPL___EXECUTOR_H
#define ASX_IMPL___EXECUTOR_H

#include "qwadro/inc/sound/afxSoundSystem.h"

AFX_DECLARE_UNION(asxWork);
AFX_DECLARE_UNION(asxWorkList);

AFX_DEFINE_STRUCT(asxSpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxSoundBridge  sexu;
    afxThread       thr;
    afxSoundSystem  ssys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedSamples;

    afxMixer        mix;
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
    asxMixSink      sinkChans[32]; // our SPU supports at least 32 sink channels.
    afxSampleContext  sampCtx;
};

AFX_DEFINE_STRUCT(_asxSoundBridgeAcquisition)
{
    afxSoundDevice          sdev;
    afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   squeClsCfg;
    afxClassConfig const*   mixClsCfg;
};

#ifdef _ASX_SOUND_BRIDGE_C
#ifdef _ASX_SOUND_BRIDGE_IMPL
AFX_OBJECT(_asxSoundBridge)
#else
AFX_OBJECT(afxSoundBridge)
#endif
{
    afxSoundDevice  sdev;
    afxUnit         portId;
    afxUnit         exuIdx;
    
    afxChain        classes;
    afxClass        squeCls;

    afxChain        activeMixers;

    afxError        (*pingCb)(afxSoundBridge, afxUnit);

    // SPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxSoundBridge);
    asxWorkList const*workVmt;
    afxBool         (*procCb)(asxSpu*);
    asxSpu*         spu;
};
#endif//_ASX_SOUND_BRIDGE_C

#ifdef _ASX_SOUND_QUEUE_C
#ifdef _ASX_SOUND_QUEUE_IMPL
AFX_OBJECT(_asxSoundQueue)
#else
AFX_OBJECT(afxSoundQueue)
#endif
{
    afxSoundDevice  sdev;
    afxUnit         portId;
    afxUnit         exuIdx;
    afxSoundSystem  ssys;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxSlock        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxError(*waitCb)(afxSoundQueue, afxTime);
    //afxError(*submCb)(afxSoundQueue, afxFence fenc, afxUnit cnt, asxQueueOp const req[]);
};
#endif//_ASX_SOUND_QUEUE_C

AFX_DEFINE_STRUCT(asxWorkHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxTime complTime; // completation time
    afxError(*exec)(void*, afxSoundBridge, afxUnit queIdx, asxWork*);
};

AFX_DEFINE_UNION(asxWorkList)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Sink;
        void* Remap;
    };
    void(*f[])(void*, asxWork const*);
};

#define ASX_GET_STD_WORK_ID(cmdName_) (offsetof(asxWorkList, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(asxWork)
{
    asxWorkHdr hdr;
    struct
    {
        asxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        afxFence        fence;
        afxUnit         mixCnt;
        afxMixer AFX_SIMD mixers[];
    } Execute;
    struct
    {
        asxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        afxFence        fence;

        union
        {
            afxAudio    wav;
            afxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            afxAudio    wav;
            afxBuffer   buf;
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
        asxWorkHdr hdr;

        afxSemaphore    wait;
        afxSink         sink;
        afxAudio        buf;
        afxWaveInterval seg;
    } Sink;
    struct
    {
        asxWorkHdr hdr;

        afxAudio        wav;
        afxSize         off;
        afxUnit         ran;
        afxFlags        flags;
        void**          placeholder;
    } Remap;
    struct
    {
        asxWorkHdr hdr;

        afxUnit         submType;
        void(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
};

ASX afxError _AsxSquePopWork(afxSoundQueue sque, asxWork* work);
ASX asxWork* _AsxSquePushWork(afxSoundQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId);
ASX afxError _AsxSquePopBlob(afxSoundQueue sque, void* blob, afxUnit siz);
ASX void* _AsxSquePushBlob(afxSoundQueue sque, afxUnit siz);

ASX afxClassConfig const _ASX_SQUE_CLASS_CONFIG;
ASX afxClassConfig const _ASX_SEXU_CLASS_CONFIG;

ASX afxClass const* _AsxGetSoundQueueClass(afxSoundBridge sexu);
ASX afxClass const* _AsxGetSoundQueueClass(afxSoundBridge sexu);

// standard sound port methods

ASX asxWorkList const _ASX_SPU_WORK_VMT;
ASX afxInt   _ASX_SPU_THREAD_PROC(afxSoundBridge sexu);
ASX afxBool  _AsxSpu_ProcCb(asxSpu*);
ASX afxError _AsxSexu_PingCb(afxSoundBridge sexu, afxUnit queIdx);

ASX afxError _AsxSpuRollMixers(asxSpu* spu, afxMixer mix);

// Common queued operations

ASX afxError _AsxSubmitCallback(afxSoundBridge sexu, void(*f)(void*, void*), void* udd);
ASX afxError _AsxSubmitSoundCommands(afxSoundBridge sexu, asxSubmission const* ctrl, afxUnit cnt, asxTracker trackers[]);
ASX afxError _AsxSubmitTransferences(afxSoundBridge sexu, asxTransference const* ctrl, afxUnit opCnt, void const* ops);
ASX afxError _AsxSubmitSink(afxSoundBridge sexu, void* ctrl, afxSink sink, afxAudio aud, afxWaveInterval const* seg);
ASX afxError _AsxRollMixers(afxSoundBridge sexu, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixer mixers[]);

#endif//ASX_IMPL___EXECUTOR_H
