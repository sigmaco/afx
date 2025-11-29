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

#ifndef AFX_IO_DDK_H
#define AFX_IO_DDK_H

#include "qwadro/exec/afxSystem.h"
#include "afxArchiveDDK.h"
#include "afxStorageDDK.h"
#include "afxStreamDDK.h"

AFX_DECLARE_UNION(afxStdWork);

#ifdef _AFX_IO_BRIDGE_C
AFX_OBJECT(afxIoBridge)
{
    afxUnit         exuIdx;
    afxUnit         portId;
    afxDevLink      ctx; // owner
    afxChain        classes;
    afxClass        xqueCls;
    afxUnit         queCnt; // common queues? (non-private)
    afxIoQueue*     queues;

    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxIoBridge);
    afxError        (**workExecFn)(afxIoBridge,afxStdWork*);

    afxBool         (*procCb)(afxIoBridge, afxThread);
    afxError        (*waitCb)(afxIoBridge, afxUnit);
    afxError        (*pingCb)(afxIoBridge, afxUnit);
    afxError        (*ctrlCb)(afxIoBridge, afxInt);
};
#endif//_AFX_IO_BRIDGE_C

#ifdef _AFX_IO_QUEUE_C
AFX_OBJECT(afxIoQueue)
{
    afxUnit         portId;
    afxIoBridge     exu; // owner bridge
    afxDevLink      ctx;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxFutex        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxChain        classes;

    afxError(*waitCb)(afxIoQueue, afxTime);
};
#endif//_AFX_IO_QUEUE_C

AFX_DEFINE_STRUCT(afxStdWorkHeader)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxTime complTime; // completation time
    afxError(*exec)(void*, afxIoBridge, afxUnit queIdx, afxStdWork*);
};

AFX_DEFINE_UNION(afxStdWork)
{
    afxStdWorkHeader hdr;
    struct
    {
        afxStdWorkHeader hdr;

        afxUnit         submType;
        void(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        afxStdWorkHeader hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        avxFence        fence;
        afxUnit         cmdbCnt;
        afxStream AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        afxStdWorkHeader hdr;

        void*           buf;
        afxSize         off;
        afxUnit         ran;
        afxFlags        flags;
        void**          placeholder;
    } Remap;
};

AFX_DEFINE_UNION(afxStdWorkList)
{
    struct
    {
        void* Callback;
        void* Execute;
    };
    void(*f[])(void*, afxStdWork const*);
};

#define AFX_GET_STD_WORK_ID(cmdName_) (offsetof(afxStdWorkList, cmdName_) / sizeof(void*))

AFX_DEFINE_STRUCT(afxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(afxStdCmd)
{
    afxCmdHdr hdr;
    struct
    {
        afxCmdHdr hdr;

        afxStream iob;
        afxSize at;
        afxUnit rowStride;
        afxUnit rowCnt;
        afxUnit dstCap;
        void *dst;

        afxBool seek;
    } Read;
    struct
    {
        afxCmdHdr hdr;

        afxStream iob;
        afxSize at;
        afxUnit rowStride;
        afxUnit rowCnt;
        void const* src;
        afxUnit srcSiz;

        afxBool seek;
    } Write;
    struct
    {
        afxCmdHdr hdr;

        afxStream iob;
        afxSize at;
        afxStream src;
        afxSize from;
        afxUnit range;

        afxBool seek;
    } Merge;
    struct
    {
        afxCmdHdr hdr;

        afxStream iob;
        afxSize at;
        afxUnit encSiz;
        afxFcc codec;
        afxUnit stop0;
        afxUnit stop1;
        afxUnit stop2;
        void* dst;

        afxBool seek;
    } Decode;
    struct
    {
        afxCmdHdr hdr;

        afxStream iob;
        afxSize at;
        afxUnit decSiz;
        afxFcc codec;
        afxUnit stop0;
        afxUnit stop1;
        afxUnit stop2;
        void const* src;

        afxBool seek;
    } Encode;
};

AFX_DEFINE_STRUCT(afxStdCmdList)
{
    struct
    {
        void* Read;
        void* Write;
        void* Merge;
        void* Decode;
        void* Encode;
    };
    void(*f[])(void*, afxStdCmd const*);
};

#define AFX_GET_STD_CMD_ID(cmdName_) (offsetof(afxStdCmdList, cmdName_) / sizeof(void*))

AFX afxClassConfig const _AFX_EXU_CLASS_CONFIG;
AFX afxClassConfig const _AfxXqueStdImplementation;

AFX afxError _AfxXquePopWork(afxIoQueue xque, afxStdWork* work);
AFX afxStdWork* _AfxXquePushWork(afxIoQueue xque, afxUnit id, afxUnit siz, afxCmdId* cmdId);

AFX afxBool _AfxExuStdIoProcCb(afxIoBridge exu, afxThread thr);
AFX afxInt _AfxExuStdIoThreadProc(afxIoBridge exu);
AFX afxError _AfxExuStdIoPingCb(afxIoBridge exu, afxUnit queIdx);
AFX afxError _AfxExuStdIoCtrlCb(afxIoBridge exu, afxInt code);
AFX afxError _XpuRollStreams(afxIoBridge exu, afxUnit cnt, afxStream streams[]);

#endif//AFX_IO_DDK_H
