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

#ifndef AFX_IO_IMPL_KIT_H
#define AFX_IO_IMPL_KIT_H

#include "qwadro/inc/exec/afxSystem.h"

AFX_DECLARE_UNION(afxStdWork);

#ifdef _AFX_IO_BRIDGE_C
AFX_OBJECT(afxIoBridge)
{
    afxUnit         exuIdx;
    afxUnit         portId;
    afxContext      ctx; // owner
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
    afxContext      ctx;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxSlock        workArenaSlock;
    afxArena        workArena; // used by submission of queue operations, not stream commands.        
    afxChain        workChn;
    afxMutex        workChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxChain        classes;

    afxError(*waitCb)(afxIoQueue, afxTime);
};
#endif//_AFX_IO_QUEUE_C

#ifdef _AFX_ARCHIVE_C
AFX_OBJECT(afxArchive)
{
    afxStream   file;
    afxArray    entries;
};
#endif//_AFX_ARCHIVE_C

#ifdef _AFX_STREAM_C

AFX_DEFINE_STRUCT(_afxIobIdd)
{
    struct
    {
        afxByte*        buf; // when wrapped or externally buffered.
        afxSize         bufCap; // when buffered
        afxSize         posn;
        afxBool         isUserBuf; // when buffered
    }                   m;
    struct
    {
        afxByte*        buf;
        afxUnit32        bufCap;
        afxUnit32        itemSiz;
        afxUnit32        wPosn;
        afxUnit32        rPosn;
        afxAtom32       rItemCnt;
    }                   m2;
    struct
    {
        void*           fd;
        afxBool         isUserFd;
        //afxUri          path;
        afxBool         shouldBeFlushed;
    }                   f;
    struct
    {
        void*           hFile;
    }                   w32;
};

AFX_OBJECT(afxStream)
{
    afxIobImpl const*   pimpl;
    afxIoUsage      usage;
    afxIoFlags      flags;
    afxSize             bufCap; // total of bytes.
    afxSize             length; // readable bytes
    // writeable bytes is reached subtracting bufCap by bufLen;
    afxSize             posn;
    afxByte*            buf;
    afxBool             isUserBuf;
    union
    {
        struct
        {
            afxSize         rPosn;
            afxSize         wPosn;
        }                   m2;
        struct
        {
            void*           fd;
            afxBool         isUserFd;
            afxBool         shouldBeFlushed;
            afxFileFlags    fflags;
            afxUri2048      url; // ---- //./dev/dir/to/
            afxUri2048      resolvedUrl; // if in search, it is only the base URI.
            afxLink      disk;

            afxUnit         segCnt;
            afxFileSegment* segHdrs;
            struct
            {
                afxUnit     refCnt;
                afxByte*    data;
            }*              segBufs; // [segCnt]
        }                   f;
    }                       idd;
    afxFcc                  typeFcc; // current type. Must be one of usage.

    struct
    {
        afxUnit portId;
        afxUnit poolIdx;
        afxBool disposable; /// if true, at execution end, it is moved to invalid state and considered in recycle chain.

        afxUnit state;
        afxChain commands;
        afxArena cmdArena; /// owned by ssys data for specific port

        afxError(*endCb)(afxStream);
        afxError(*resetCb)(afxStream, afxBool freeMem, afxBool permanent);
        afxAtom32 submCnt; /// number of submissions
        afxMask64 submQueMask; /// one for each queue where this dctx was submitted into.
    };
};

#endif//_AFX_STREAM_C

#if 0
#ifdef _AFX_FILE_C
AFX_OBJECT(afxFile)
{
    afxStream               ios;
    void                    *fd;
    afxUri                  path;
    afxBool                 shouldBeFlushed;
    afxFileFlags            flags;

    afxFileHeader*          hdr;
    afxUnit                  openSecCnt;
    void**                  openSections;
    afxBool8*               marshalled;
    afxBool8*               isUserMem;
    afxBool                 byteReserved;
};
#endif//_AFX_FILE_C
#endif

#ifdef _AFX_STORAGE_C

AFX_DEFINE_STRUCT(afxStorageUnit)
{
    afxLink          fsys;
    afxFileFlags        flags;
    afxUri              rootPath;  // path of exchange point
    afxArchive          arc;
};

AFX_OBJECT(afxStorage)
{
    afxChar             diskId; // single letter identifying this file system.
    afxUri8             baseUrl; // qualified name of exchange point <//./z/>
    afxChain            storages;
    afxChain            classes;
    afxClass            fileCls;
    afxClass            archCls;
    afxClass            urdCls;
    afxChain            fileChain;
};

#endif//_AFX_STORAGE_C


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
        afxFence        fence;
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

AFX afxClass const*     AfxGetFileClass(afxStorage fsys);
AFX afxClass const*     AfxGetArchiveClass(afxStorage fsys);

AFX afxClassConfig const _AfxExuStdImplementation;
AFX afxClassConfig const _AfxXqueStdImplementation;

AFX afxError _AfxXquePopWork(afxIoQueue xque, afxStdWork* work);
AFX afxStdWork* _AfxXquePushWork(afxIoQueue xque, afxUnit id, afxUnit siz, afxCmdId* cmdId);

AFX afxBool _AfxExuStdIoProcCb(afxIoBridge exu, afxThread thr);
AFX afxInt _AfxExuStdIoThreadProc(afxIoBridge exu);
AFX afxError _AfxExuStdIoPingCb(afxIoBridge exu, afxUnit queIdx);
AFX afxError _AfxExuStdIoCtrlCb(afxIoBridge exu, afxInt code);
AFX afxError _XpuRollStreams(afxIoBridge exu, afxUnit cnt, afxStream streams[]);

#endif//AFX_IO_IMPL_KIT_H
