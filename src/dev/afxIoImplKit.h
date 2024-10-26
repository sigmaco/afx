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
    afxStreamUsage      usage;
    afxStreamFlags      flags;
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
            afxSize     rPosn;
            afxSize     wPosn;
        }               m2;
        struct
        {
            void*       fd;
            afxBool     isUserFd;
            afxBool     shouldBeFlushed;
            afxFileFlags fflags;
            afxUri2048  url; // ---- //./dev/dir/to/
            afxUri2048  resolvedUrl; // if in search, it is only the base URI.
            afxLinkage  disk;
        }               f;
    }                   idd;
    afxFcc              typeFcc; // current type. Must be one of usage.
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
    afxLinkage          fsys;
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

#endif//AFX_IO_IMPL_KIT_H
