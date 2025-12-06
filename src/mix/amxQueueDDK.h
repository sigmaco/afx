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

#ifndef AMX_QUEUE_DDK_H
#define AMX_QUEUE_DDK_H

#include "amxStorageDDK.h"

AFX_DECLARE_UNION(_amxIoReqPacket);
AFX_DECLARE_UNION(_amxIoReqLut);

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

        afxUnit         cmdbCnt;
        struct
        {
            amxFence    wait;
            afxUnit64   waitValue;
            amxBusStage waitStageMask;
            afxUnit32   waitReserved;
            amxFence    signal;
            afxUnit64   signalValue;
            amxBusStage signalStageMask;
            afxUnit32   signalReserved;

            afxMixContext mctx;
        } AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        _amxIoReqPacketHdr hdr;

        amxFence    wait;
        afxUnit64   waitValue;
        amxBusStage waitStageMask;
        afxUnit32   waitReserved;
        amxFence    signal;
        afxUnit64   signalValue;
        amxBusStage signalStageMask;
        afxUnit32   signalReserved;

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

        afxUnit         opCnt;
        struct
        {
            amxFence    wait;
            afxUnit64   waitValue;
            amxBusStage waitStageMask;
            afxUnit32   waitReserved;
            amxFence    signal;
            afxUnit64   signalValue;
            amxBusStage signalStageMask;
            afxUnit32   signalReserved;

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
        amxFence fences[];
    } WaitForFences;
};

AMX afxError _AmxMquePopIoReqPacket(afxMixQueue mque, _amxIoReqPacket* work);
AMX afxError _AmxMquePushIoReqPacket(afxMixQueue mque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxIoReqPacket** iorp);
AMX afxError _AmxMquePopBlob(afxMixQueue mque, void* blob, afxUnit siz);
AMX void* _AmxMquePushBlob(afxMixQueue mque, afxUnit siz);

AMX afxClassConfig const _AMX_MQUE_CLASS_CONFIG;

// Common queued operations

AMX afxError _AmxMqueSubmitCallback(afxMixQueue mque, afxError(*f)(void*, void*), void* udd);
AMX afxError _AmxMqueSubmitMixCommands(afxMixQueue mque, afxUnit cnt, amxSubmission subms[]);
AMX afxError _AmxMqueTransferResources(afxMixQueue mque, amxTransference const* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxMqueRemapBuffers(afxMixQueue mque, afxUnit mapCnt, _amxBufferRemapping const maps[], afxUnit unmapCnt, _amxBufferRemapping const unmaps[]);
AMX afxError _AmxMqueCohereMappedBuffers(afxMixQueue mque, afxUnit flushCnt, amxBufferedMap const flushes[], afxUnit fetchCnt, amxBufferedMap const fetches[]);

AMX afxError _AmxMqueSinkBuffers(afxMixQueue mque, afxUnit cnt, amxFlush presentations[]);

#endif//AMX_QUEUE_DDK_H
