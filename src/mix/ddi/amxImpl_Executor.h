/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_IMPL___EXECUTOR_H
#define AMX_IMPL___EXECUTOR_H

#include "qwadro/mix/afxMixSystem.h"

AFX_DEFINE_STRUCT(_amxBufferRemapping)
{
    amxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    afxFlags        flags;
    afxBool         unmap;
    void**          placeholder;
};

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
    afxUnit         queuedFrameCnt;

    afxUnit         mixFreq; // the mix frequency used in a mix scope.
    amxAudio        sinkBuf; // the audio's sink buffer object.
    afxUnit         sinkBufIdx;
    afxUnit         sinkBaseSamp;
    afxUnit         sinkBaseSeg;
    afxUnit         sinkSampCnt;
    afxUnit         sinkSegCnt;
    afxUnit         sinkBaseChan;
    afxUnit         sinkChanCnt;
    amxMixTarget      sinkChans[32]; // our SPU supports at least 32 sink channels.
    //arxSampleContext  sampCtx;
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
    afxUnit dpuId;
    afxUnit syncUnitId;
    void* syncIdd0;
    afxSize syncIdd1;
    afxError(*exec)(void*, afxMixBridge, afxUnit queIdx, _amxIoReqPacket*);
    afxTime complTime; // completation time
    avxFence completionFence;
    afxSize idd[4];
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
        void* SyncMaps;
    };
    afxError(*f[])(void*, _amxIoReqPacket const*);
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
        //avxFence        fence;
        afxUnit         cmdbCnt;
        struct
        {
            afxMixContext mctx;
            afxUnit batchId;
        } AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        //avxFence        fence;

        union
        {
            amxAudio    aud;
            amxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            amxAudio    aud;
            amxBuffer   buf;
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
            amxAudioIo AFX_SIMD wavOps[];
            amxAudioCopy AFX_SIMD wavCpyOps[];
            amxBufferIo AFX_SIMD bufOps[];
            amxBufferCopy AFX_SIMD bufCpyOps[];
        };
    } Transfer;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxSemaphore    wait;
        afxUnit         opCnt;
        struct
        {
            afxSink     sink;
            afxUnit     bufIdx;
        } AFX_SIMD      ops[];
    } Sink;
    struct
    {
        _amxIoReqPacketHdr hdr;
        afxUnit     firstMapIdx;
        afxUnit     mapCnt;
        afxUnit     firstUnmapIdx;
        afxUnit     unmapCnt;
        union
        {
            _amxBufferRemapping mapOps[];
            _amxBufferRemapping unmapOps[];
        };
    } Remap;
    struct
    {
        _amxIoReqPacketHdr  hdr;
        afxUnit     baseFlushIdx;
        afxUnit     flushCnt;
        afxUnit     baseFetchIdx;
        afxUnit     fetchCnt;
        union
        {
            amxBufferedMap ops[];
        };
    } SyncMaps;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxUnit         submType;
        afxError(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        _amxIoReqPacketHdr hdr;

        afxBool waitAll;
        afxUnit64 timeout;
        afxUnit cnt;
        /*amxFence*/void* fences[];
    } WaitForFences;
};

AMX afxError _AmxMquePopIoReqPacket(afxMixQueue mque, _amxIoReqPacket* work);
AMX afxError _AmxMquePushIoReqPacket(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxIoReqPacket** iorp);
AMX afxError _AmxMquePopBlob(afxMixQueue mque, void* blob, afxUnit siz);
AMX void* _AmxMquePushBlob(afxMixQueue mque, afxUnit siz);

AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MEXU_CLASS_CONFIG;

AMX afxClass const* _AmxMexuGetMqueClass(afxMixBridge mexu);

AMX afxError _AmxMsysBridgeDevices(afxMixSystem msys, afxUnit cnt, _amxMexuAcquisition const configs[], afxMixBridge bridges[]);

AMX afxError _AmxMexuRemapBuffers(afxMixBridge mexu, afxBool unmap, afxUnit cnt, _amxBufferRemapping const maps[]);
AMX afxError _AmxMexuCohereMappedBuffers(afxMixBridge mexu, afxBool discard, afxUnit cnt, amxBufferedMap const maps[]);
AMX afxError _AmxMexuTransferMixMemory(afxMixBridge mexu, amxTransference* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxMexuSinkBuffers(afxMixBridge mexu, afxUnit cnt, amxFlush presentations[]);

// standard sound port methods

AMX _amxIoReqLut const _AMX_MPU_IORP_VMT;
AMX afxInt   _AMX_MPU_THREAD_PROC(afxMixBridge mexu);
AMX afxBool  _AmxMpu_ProcCb(amxMpu*);
AMX afxError _AmxMexu_PingCb(afxMixBridge mexu, afxUnit queIdx);

AMX afxError _AmxMpuRollMixContexts(amxMpu* mpu, afxMixContext mix);

// Common queued operations

//AMX afxError _AmxMexuSubmitCallback(afxMixBridge mexu, afxError(*f)(void*, void*), void* udd);
AMX afxError _AmxMexuExecuteMixCommands(afxMixBridge mexu, afxUnit frameCnt, afxUnit cnt, afxMixContext mixers[]);

AMX afxError _AmxMqueSubmitCallback(afxMixQueue mque, afxError(*f)(void*, void*), void* udd);
AMX afxError _AmxMqueExecuteMixCommands(afxMixQueue mque, afxUnit cnt, amxSubmission subms[]);
AMX afxError _AmxMqueTransferResources(afxMixQueue mque, amxTransference const* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxMqueRemapBuffers(afxMixQueue mque, afxUnit mapCnt, _amxBufferRemapping const maps[], afxUnit unmapCnt, _amxBufferRemapping const unmaps[]);
AMX afxError _AmxMqueCohereMappedBuffers(afxMixQueue mque, afxUnit flushCnt, amxBufferedMap const flushes[], afxUnit fetchCnt, amxBufferedMap const fetches[]);

AMX afxError _AmxMqueSinkBuffers(afxMixQueue mque, afxUnit cnt, amxFlush presentations[]);

#endif//AMX_IMPL___EXECUTOR_H
