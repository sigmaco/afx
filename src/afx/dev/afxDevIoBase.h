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

#ifndef AFX_DEV_IO_BASE_H
#define AFX_DEV_IO_BASE_H

#include "qwadro/exec/afxSystem.h"

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
        afxNat32        bufCap;
        afxNat32        itemSiz;
        afxNat32        wPosn;
        afxNat32        rPosn;
        afxAtom32       rItemCnt;
    }                   m2;
    struct
    {
        void*           fd;
        afxBool         isUserFd;
        afxUri          path;
        afxBool         shouldBeFlushed;
    }                   f;
};

AFX_OBJECT(afxStream)
{
    _afxIobImpl const*  pimpl;
    afxIoFlags          flags;
    _afxIobIdd          idd;
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
    afxNat                  openSecCnt;
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
    afxUri8             point; // name of exchange point // AFX_FS_SYM_LEN // can't have control chars. Actually works like a variable without $().
    afxChain            storages;
};
#endif//_AFX_STORAGE_C

#endif//AFX_DEV_IO_BASE_H
