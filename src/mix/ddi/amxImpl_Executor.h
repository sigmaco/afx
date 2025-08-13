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

AFX_DECLARE_UNION(_amxIoReqPacket);
AFX_DECLARE_UNION(_amxIoReqLut);

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
    amxAudio        a, b;

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

AFX_DEFINE_STRUCT(_amxMexuAcquisition)
{
    afxMixDevice            mdev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   mqueClsCfg;
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
    //afxUnit         portId;
    afxUnit         exuIdx;
    
    afxChain        classes;
    afxClass        mqueCls;

    afxChain        activeMixers;

    afxError        (*pingCb)(afxMixBridge, afxUnit);

    // SPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxMixBridge);
    _amxIoReqLut const*iorpVmt;
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
    //afxUnit         portId;
    afxUnit         exuIdx;
    afxMixSystem    msys;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxFutex        iorpArenaSlock;
    afxArena        iorpArena; // used by submission of queue operations, not stream commands.        
    afxChain        iorpChn;
    afxMutex        iorpChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxError(*waitCb)(afxMixQueue, afxTime);
    //afxError(*submCb)(afxMixQueue, avxFence fenc, afxUnit cnt, amxQueueOp const req[]);
};
#endif//_AMX_MIX_QUEUE_C

AFX_DEFINE_STRUCT(_amxIoReqPacketHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxTime complTime; // completation time
    afxError(*exec)(void*, afxMixBridge, afxUnit queIdx, _amxIoReqPacket*);
};

AFX_DEFINE_UNION(_amxIoReqLut)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Sink;
        void* Remap;
    };
    void(*f[])(void*, _amxIoReqPacket const*);
};

#define _AMX_GET_STD_IORP_ID(cmdName_) (offsetof(_amxIoReqLut, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(_amxIoReqPacket)
{
    _amxIoReqPacketHdr hdr;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        avxFence        fence;
        afxUnit         mixCnt;
        afxMixContext AFX_SIMD mixers[];
    } Execute;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        avxFence        fence;

        union
        {
            amxAudio    aud;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            amxAudio    aud;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               dst;
        afxFcc          dstFcc;
        afxCodec        codec;
        afxUnit         decSiz;
        afxUnit         opCnt;
        amxAudioIo AFX_SIMD wavOps[];
    } Transfer;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxSemaphore    wait;
        afxSink         sink;
        amxAudio        buf;
        amxAudioPeriod seg;
    } Sink;
    struct
    {
        _amxIoReqPacketHdr hdr;

        amxAudio        wav;
        afxSize         off;
        afxUnit         ran;
        afxFlags        flags;
        void**          placeholder;
    } Remap;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxUnit         submType;
        void(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
};

AMX afxError _AmxMquePopIoReqPacket(afxMixQueue mque, _amxIoReqPacket* work);
AMX afxError _AmxMquePushIoReqPacket(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxIoReqPacket** iorp);
AMX afxError _AmxMquePopBlob(afxMixQueue mque, void* blob, afxUnit siz);
AMX void* _AmxMquePushBlob(afxMixQueue mque, afxUnit siz);

AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MEXU_CLASS_CONFIG;

AMX afxClass const* _AmxGetMixQueueClass(afxMixBridge mexu);

AMX afxError _AmxMsysBridgeDevices(afxMixSystem msys, afxUnit cnt, _amxMexuAcquisition const configs[], afxMixBridge bridges[]);

// standard sound port methods

AMX _amxIoReqLut const _AMX_MPU_IORP_VMT;
AMX afxInt   _AMX_MPU_THREAD_PROC(afxMixBridge mexu);
AMX afxBool  _AmxMpu_ProcCb(amxMpu*);
AMX afxError _AmxMexu_PingCb(afxMixBridge mexu, afxUnit queIdx);

AMX afxError _AmxMpuRollMixContexts(amxMpu* mpu, afxMixContext mix);

// Common queued operations

AMX afxError _AmxMexuSubmitCallback(afxMixBridge mexu, void(*f)(void*, void*), void* udd);
AMX afxError _AmxSubmitSoundCommands(afxMixBridge mexu, amxSubmission const* ctrl, afxUnit cnt, amxTracker trackers[]);
AMX afxError _AmxMexuTransferBuffers(afxMixBridge mexu, amxTransference const* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxMexuSubmitSink(afxMixBridge mexu, void* ctrl, afxSink sink, amxAudio aud, amxAudioPeriod const* seg);
AMX afxError _AmxMexuRollMixers(afxMixBridge mexu, afxReal clock, afxReal dur, afxUnit iterCnt, afxUnit cnt, afxMixContext mixers[]);

#endif//AMX_IMPL___EXECUTOR_H
