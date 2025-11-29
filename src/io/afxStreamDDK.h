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

#ifndef AFX_STREAM_DDK_H
#define AFX_STREAM_DDK_H

#include "qwadro/exec/afxSystem.h"

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
        afxBool disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

        afxUnit state;
        afxChain commands;
        afxArena cmdArena; // owned by ssys data for specific port

        afxError(*endCb)(afxStream);
        afxError(*resetCb)(afxStream, afxBool freeMem, afxBool permanent);
        afxAtom32 submCnt; // number of submissions
        afxMask64 submQueMask; // one for each queue where this dctx was submitted into.
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

#endif//AFX_STREAM_DDK_H
