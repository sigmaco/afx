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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "qwadro/io/afxStream.h"
#include "qwadro/io/afxUri.h"

typedef enum afxFileFlags
{
    // permissions
    afxFileFlag_R       = AFX_BIT(0), // Readable
    afxFileFlag_W       = AFX_BIT(1), // Writeable
    afxFileFlag_X       = AFX_BIT(2), // Executable (seekable)
    afxFileFlag_RW      = (afxFileFlag_R | afxFileFlag_W),
    afxFileFlag_RX      = (afxFileFlag_R | afxFileFlag_X),
    afxFileFlag_WX      = (afxFileFlag_W | afxFileFlag_X),
    afxFileFlag_RWX     = (afxFileFlag_R | afxFileFlag_WX),
    AFX_FILE_PERM_MASK  = afxFileFlag_RWX,

    // attributes
    afxFileFlag_U       = AFX_BIT(3), // Unbuffered.
    afxFileFlag_L       = AFX_BIT(4), // Literal
    afxFileFlag_V       = AFX_BIT(5), // Virtual. This flag is reserved for system use.
    afxFileFlag_Q       = AFX_BIT(6), // Qwadro. This flag is reserved for system use.
    afxFileFlag_D       = AFX_BIT(7),  // Device. This flag is reserved for system use.
    afxFileFlag_H       = AFX_BIT(8),  // Hidden. The stream is hidden. It shouldn't be included in an ordinary listing.
    afxFileFlag_A       = AFX_BIT(9),  // Archived. The stream is an archive-derived stream.
    afxFileFlag_T       = AFX_BIT(10), // Temporary. Used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed. This is used by SIGMA Future Storage.
    afxFileFlag_Z       = AFX_BIT(11), // Compressed. The data is compressed. For a file, all of the data in the file is compressed. This is used by SIGMA Future Storage.
    afxFileFlag_E       = AFX_BIT(12), // Encrypted. The data is encrypted. For a file, all data streams in the file are encrypted. This is used by SIGMA Future Storage.
    afxFileFlag_O       = AFX_BIT(13), // Offline. The data isn't available immediately. This flag indicates that the data is physically moved to remote/offline storage. This is used by SIGMA Future Storage.
    afxFileFlag_S       = AFX_BIT(14), // Sparse. The data is a sparse file. Empty ranges (zeroed ranges) are generated dynamically by some algorithm. This is used by SIGMA Future Storage.
}
afxFileFlags;

AFX_DEFINE_STRUCT(afxFileReference)
{
    afxNat              secIdx;
    afxSize             offset;
};

AFX_DEFINE_STRUCT(afxFileSection)
{
    afxNat              codec; // codec used to compress this section
    afxNat              dataOffset;
    afxNat              dataSizStored;
    afxNat              dataSiz;
    afxNat              intAlign;
};

AFX_DEFINE_STRUCT(afxFileHeader)
{
    afxNat              ver;
    afxNat              totalSiz;
    afxNat32            crc;
    afxNat              secOffset;
    afxNat              secCnt;
    afxFileReference    rootObjectTypeDef;
    afxFileReference    rootObject;
    afxNat              typeTag;
    afxNat              extraTags[4];
    afxNat              stringDatabaseCrc;
    afxNat              reserved[3];
};

AFX_DEFINE_STRUCT(afxFileMagicValue)
{
    afxNat              magicVal[4];
    afxNat              hdrSiz;
    afxNat              hdrFmt;
    afxNat              reserved[2];
};

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

AFX void*                   AfxGetFileDescriptor(afxStream file);

//AFX afxError                AfxReopenFile(afxFile file, afxRwx const rwx, afxUri const *uri);
//AFX afxError                AfxReloadFile(afxFile file, afxRwx const rwx, afxUri const *uri);

AFX afxUri const*           AfxGetFilePath(afxStream file);
AFX afxResult               AfxCopyFilePath(afxStream file, afxUri* uri);

AFX afxString const*        AfxGetFilePathString(afxStream file);
AFX afxResult               AfxCopyFilePathString(afxStream file, afxRestring *str);

AFX afxError                AfxReadFileString(afxStream file, afxRestring* str);

AFX afxResult               AfxFlushFile(afxStream file);
AFX afxBool                 AfxFileShouldBeFlushed(afxStream file);

AFX afxError                AfxBufferizeFile(afxFile file, afxNat siz);

AFX afxBool                 AfxFileIsVirtual(afxFile file);
AFX afxBool                 AfxFileIsTemporary(afxFile file);
AFX afxBool                 AfxFileIsSparse(afxFile file);
AFX afxBool                 AfxFileIsCompressed(afxFile file);
AFX afxBool                 AfxFileIsEncrypted(afxFile file);
AFX afxBool                 AfxFileIsArchive(afxFile file);
AFX afxBool                 AfxFileIsHidden(afxFile file);
AFX afxBool                 AfxFileIsReserved(afxFile file);

#endif//AFX_FILE_H
