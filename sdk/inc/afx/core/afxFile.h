/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "afx/core/afxStream.h"
#include "afx/core/afxUri.h"

typedef enum afxFileFlags
{
    // permissions
    AFX_FILE_FLAG_R         = AFX_BIT_OFFSET(0), // Readable
    AFX_FILE_FLAG_W         = AFX_BIT_OFFSET(1), // Writeable
    AFX_FILE_FLAG_X         = AFX_BIT_OFFSET(2), // Executable (seekable)
    AFX_FILE_FLAG_RW        = (AFX_FILE_FLAG_R | AFX_FILE_FLAG_W),
    AFX_FILE_FLAG_RX        = (AFX_FILE_FLAG_R | AFX_FILE_FLAG_X),
    AFX_FILE_FLAG_WX        = (AFX_FILE_FLAG_W | AFX_FILE_FLAG_X),
    AFX_FILE_FLAG_RWX       = (AFX_FILE_FLAG_R | AFX_FILE_FLAG_W | AFX_FILE_FLAG_X),
    AFX_FILE_PERM_MASK        = AFX_FILE_FLAG_RWX,

    // attributes
    AFX_FILE_FLAG_U         = AFX_BIT_OFFSET(3), // Unbuffered.
    AFX_FILE_FLAG_L         = AFX_BIT_OFFSET(4), // Literal
    AFX_FILE_FLAG_V         = AFX_BIT_OFFSET(5), // Virtual. This flag is reserved for system use.
    AFX_FILE_FLAG_Q         = AFX_BIT_OFFSET(6), // Qwadro. This flag is reserved for system use.
    AFX_FILE_FLAG_D         = AFX_BIT_OFFSET(7),  // Device. This flag is reserved for system use.
    AFX_FILE_FLAG_H         = AFX_BIT_OFFSET(8),  // Hidden. The stream is hidden. It shouldn't be included in an ordinary listing.
    AFX_FILE_FLAG_A         = AFX_BIT_OFFSET(9),  // Archived. The stream is an archive-derived stream.
    AFX_FILE_FLAG_T         = AFX_BIT_OFFSET(10), // Temporary. Used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed. This is used by SIGMA Future Storage.
    AFX_FILE_FLAG_Z         = AFX_BIT_OFFSET(11), // Compressed. The data is compressed. For a file, all of the data in the file is compressed. This is used by SIGMA Future Storage.
    AFX_FILE_FLAG_E         = AFX_BIT_OFFSET(12), // Encrypted. The data is encrypted. For a file, all data streams in the file are encrypted. This is used by SIGMA Future Storage.
    AFX_FILE_FLAG_O         = AFX_BIT_OFFSET(13), // Offline. The data isn't available immediately. This flag indicates that the data is physically moved to remote/offline storage. This is used by SIGMA Future Storage.
    AFX_FILE_FLAG_S         = AFX_BIT_OFFSET(14), // Sparse. The data is a sparse file. Empty ranges (zeroed ranges) are generated dynamically by some algorithm. This is used by SIGMA Future Storage.

    AFX_FILE_FLAG_FORCE_NAT = AFX_N32_MAX
}
afxFileFlags;

#ifdef _AFX_FILE_C
AFX_OBJECT(afxFile)
{
    afxStream               ios;
    void                    *fd;
    afxUri                  path;
    afxBool                 shouldBeFlushed;
    afxFileFlags            flags;
};
#endif

AFX afxNat                  AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile file[]);

AFX afxError                AfxOpenFiles(afxNat cnt, afxFile file[], afxUri const uri[], afxFileFlags const flags[]);
AFX afxFile                 AfxOpenReadableFile(afxUri const *uri);
AFX afxFile                 AfxOpenWritableFile(afxUri const *uri);
AFX afxStream               AfxLoadFile(afxIoFlags flags, afxUri const *uri); // will fully load the opened file then close it.

AFX void*                   AfxGetFileHostDescriptor(afxFile file);

AFX afxError                AfxReopenFile(afxFile file, afxRwx const rwx, afxUri const *uri);
AFX afxError                AfxReloadFile(afxFile file, afxRwx const rwx, afxUri const *uri);

AFX afxResult               AfxExtractFilePath(afxFile file, afxString *str);
AFX afxString const*        AfxGetFilePathString(afxFile file);
AFX afxUri const*           AfxGetFilePath(afxFile file);

AFX afxStream*              AfxGetFileStream(afxFile file);

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

//AFX afxBool                 _AfxGetFileD(afxFile file, struct _afxFileD **fileD, struct _afxSysD* sysD);

#endif//AFX_FILE_H