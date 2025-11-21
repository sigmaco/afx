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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_DRAW_QUEUE_IMPL_H
#define AVX_DRAW_QUEUE_IMPL_H

#include "avxStorageImpl.h"
#include "avxContextImpl.h"
#include "avxFenceImpl.h"

AFX_DECLARE_UNION(_avxIoReqPacket);
AFX_DECLARE_UNION(_avxIoReqLut);

AFX_DECLARE_STRUCT(_avxIddDque);
AFX_DEFINE_STRUCT(_avxDdiDque)
{
    afxError(*waitCb)(afxDrawQueue, afxUnit64);
};

#ifdef _AVX_DRAW_QUEUE_C
#ifdef _AVX_DRAW_QUEUE_IMPL
AFX_OBJECT(_avxDrawQueue)
#else
AFX_OBJECT(afxDrawQueue)
#endif
{
    _avxIddDque*        idd;
    _avxDdiDque const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxDrawDevice       ddev;
    //afxUnit           portId;
    afxUnit             exuIdx;
    afxMask             exuMask;
    afxDrawSystem       dsys; // owner context
    afxBool             immediate; // 0 = deferred, 1 = immediate
    afxBool             closed; // can't enqueue

    afxCondition        idleCnd;
    //afxMutex          idleCndMtx;
    afxFutex            iorpArenaSlock;
    afxArena            iorpArena; // used by submission of queue operations, not stream commands.        
    afxChain            iorpChn;
    afxMutex            iorpChnMtx;

    afxChain            classes;
    // one stream manager per queue to avoid thread interaction
};
#endif//_AVX_DRAW_QUEUE_C

AFX_DEFINE_UNION(_avxIoReqLut)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Present;
        void* Remap;
        void* SyncMaps;
        void* Stamp;
    };
    afxError(*f[])(void*, _avxIoReqPacket*);
};

#define _AVX_GET_STD_IORP_ID(cmdName_) (offsetof(_avxIoReqLut, cmdName_) / sizeof(void*))

AFX_DEFINE_STRUCT(_avxIoReqPacketHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxClock pushTime; // submission (into input) time
    afxClock pullTime; // fecth (by queue) time
    afxBool pulled;
    afxUnit dpuId;
    afxUnit syncUnitId;
    void* syncIdd0;
    afxSize syncIdd1;
    afxError(*exec)(void*, afxDrawBridge, afxUnit queIdx, _avxIoReqPacket*);
    afxClock complTime; // completation time
    afxBool completed;
    avxFence completionFence;
    afxSize idd[4];
};

AFX_DEFINE_UNION(_avxIoReqPacket)
{
    _avxIoReqPacketHdr hdr;
    struct
    {
        _avxIoReqPacketHdr hdr;

        avxFence    wait;
        avxFence    signal;
        //avxFence        fence;
        afxUnit         cmdbCnt;
        struct
        {
            afxDrawContext dctx;
            afxUnit batchId;
        } AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        _avxIoReqPacketHdr hdr;

        avxFence    wait;
        avxFence    signal;
        //avxFence        fence;

        union
        {
            avxRaster   ras;
            avxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            avxRaster   ras;
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
            avxRasterCopy AFX_SIMD rasCpyOps[];
            avxRasterIo AFX_SIMD rasOps[];
            avxBufferCopy AFX_SIMD bufCpyOps[];
            avxBufferIo AFX_SIMD bufOps[];
        };
    } Transfer;
    struct
    {
        _avxIoReqPacketHdr hdr;

        avxFence    wait;
        afxUnit         opCnt;
        struct
        {
            afxSurface   dout;
            afxUnit         bufIdx;
        } AFX_SIMD          ops[];
    } Present;
    struct
    {
        _avxIoReqPacketHdr hdr;
        afxUnit     firstMapIdx;
        afxUnit     mapCnt;
        afxUnit     firstUnmapIdx;
        afxUnit     unmapCnt;
        union
        {
            _avxBufferRemapping mapOps[];
            _avxBufferRemapping unmapOps[];
        };
    } Remap;
    struct
    {
        _avxIoReqPacketHdr  hdr;
        afxUnit     baseFlushIdx;
        afxUnit     flushCnt;
        afxUnit     baseFetchIdx;
        afxUnit     fetchCnt;
        union
        {
            avxBufferedMap ops[];
        };
    } SyncMaps;
    struct
    {
        _avxIoReqPacketHdr hdr;

        afxUnit         submType;
        afxError        (*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        _avxIoReqPacketHdr hdr;

        afxBool waitAll;
        afxUnit64 timeout;
        afxUnit cnt;
        avxFence fences[];
    } WaitForFences;
    struct
    {
        _avxIoReqPacketHdr hdr;

        avxCanvas canv;
        avxOrigin origin;
        afxString4096 caption;
    } Stamp;
};

AVX afxClassConfig const _AVX_DQUE_CLASS_CONFIG;

AVX _avxIoReqLut const _AVX_DPU_IORP_VMT;

AVX afxError        _AvxAcquireDrawQueues
(
    afxDrawSystem   dsys,
    afxUnit         cnt,
    afxMask const   bridges[],
    afxDrawQueue    queues[]
);

AVX afxError _AvxDqueUnlockIoReqChain(afxDrawQueue dque);
AVX afxError _AvxDqueLockIoReqChain(afxDrawQueue dque, afxUnit64 timeout);
AVX afxError _AvxDquePopIoReqPacket(afxDrawQueue dque, _avxIoReqPacket* work);
AVX afxError _AvxDquePushIoReqPacket(afxDrawQueue dque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _avxIoReqPacket** pIorp);

// Common queued operations
AVX afxError _AvxDqueExecuteDrawCommands(afxDrawQueue dque, afxUnit cnt, avxSubmission subms[]);
AVX afxError _AvxDqueTransferResources(afxDrawQueue dque, avxTransference const* ctrl, afxUnit opCnt, void const* ops);
AVX afxError _AvxDqueRemapBuffers(afxDrawQueue dque, afxUnit mapCnt, _avxBufferRemapping const maps[], afxUnit unmapCnt, _avxBufferRemapping const unmaps[]);
AVX afxError _AvxDqueCohereMappedBuffers(afxDrawQueue dque, afxUnit flushCnt, avxBufferedMap const flushes[], afxUnit fetchCnt, avxBufferedMap const fetches[]);
AVX afxError _AvxDqueSubmitCallback(afxDrawQueue dque, afxError(*f)(void*, void*), void* udd);

AVX void _AvxBeginDrawQueueDebugScope(afxDrawQueue dque, afxString const* name, avxColor const color);
AVX void _AvxPushDrawQueueDebugLabel(afxDrawQueue dque, afxString const* name, avxColor const color);
AVX void _AvxEndDrawQueueDebugScope(afxDrawQueue dque);

#endif//AVX_DRAW_QUEUE_IMPL_H
