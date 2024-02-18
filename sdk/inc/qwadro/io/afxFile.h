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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "qwadro/io/afxStream.h"
#include "qwadro/io/afxUri.h"

typedef enum afxFileFlags
{
    // permissions
    afxFileFlag_R         = AfxGetBitOffset(0), // Readable
    afxFileFlag_W         = AfxGetBitOffset(1), // Writeable
    afxFileFlag_X         = AfxGetBitOffset(2), // Executable (seekable)
    afxFileFlag_RW        = (afxFileFlag_R | afxFileFlag_W),
    afxFileFlag_RX        = (afxFileFlag_R | afxFileFlag_X),
    afxFileFlag_WX        = (afxFileFlag_W | afxFileFlag_X),
    afxFileFlag_RWX       = (afxFileFlag_R | afxFileFlag_W | afxFileFlag_X),
    AFX_FILE_PERM_MASK      = afxFileFlag_RWX,

    // attributes
    afxFileFlag_U         = AfxGetBitOffset(3), // Unbuffered.
    afxFileFlag_L         = AfxGetBitOffset(4), // Literal
    afxFileFlag_V         = AfxGetBitOffset(5), // Virtual. This flag is reserved for system use.
    afxFileFlag_Q         = AfxGetBitOffset(6), // Qwadro. This flag is reserved for system use.
    afxFileFlag_D         = AfxGetBitOffset(7),  // Device. This flag is reserved for system use.
    afxFileFlag_H         = AfxGetBitOffset(8),  // Hidden. The stream is hidden. It shouldn't be included in an ordinary listing.
    afxFileFlag_A         = AfxGetBitOffset(9),  // Archived. The stream is an archive-derived stream.
    afxFileFlag_T         = AfxGetBitOffset(10), // Temporary. Used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed. This is used by SIGMA Future Storage.
    afxFileFlag_Z         = AfxGetBitOffset(11), // Compressed. The data is compressed. For a file, all of the data in the file is compressed. This is used by SIGMA Future Storage.
    afxFileFlag_E         = AfxGetBitOffset(12), // Encrypted. The data is encrypted. For a file, all data streams in the file are encrypted. This is used by SIGMA Future Storage.
    afxFileFlag_O         = AfxGetBitOffset(13), // Offline. The data isn't available immediately. This flag indicates that the data is physically moved to remote/offline storage. This is used by SIGMA Future Storage.
    afxFileFlag_S         = AfxGetBitOffset(14), // Sparse. The data is a sparse file. Empty ranges (zeroed ranges) are generated dynamically by some algorithm. This is used by SIGMA Future Storage.
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

AFX afxError                AfxOpenFiles(afxFileFlags flags, afxNat cnt, afxUri const uri[], afxFile files[]);
AFX afxFile                 AfxOpenReadableFile(afxUri const *uri);
AFX afxFile                 AfxOpenWritableFile(afxUri const *uri);
AFX afxError                AfxReloadFile(afxStream ios, afxFileFlags flags, afxUri const *uri); // will fully load the opened file then close it.

AFX void*                   AfxGetFileHostDescriptor(afxFile file);

//AFX afxError                AfxReopenFile(afxFile file, afxRwx const rwx, afxUri const *uri);
//AFX afxError                AfxReloadFile(afxFile file, afxRwx const rwx, afxUri const *uri);

AFX afxUri const*           AfxGetFilePath(afxFile file);
AFX afxResult               AfxCopyFilePath(afxFile file, afxUri* uri);

AFX afxString const*        AfxGetFilePathString(afxFile file);
AFX afxResult               AfxCopyFilePathString(afxFile file, afxRestring *str);

AFX afxStream               AfxGetFileStream(afxFile file);
AFX afxError                AfxReadFileString(afxFile file, afxRestring* str);

AFX afxResult               AfxFlushFile(afxFile file);
AFX afxBool                 AfxFileShouldBeFlushed(afxFile file);
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
