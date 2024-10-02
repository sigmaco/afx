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

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro Data Access And Storage Infrastructure                            //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA Future Storage.

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "qwadro/inc/io/afxStream.h"
#include "qwadro/inc/io/afxUri.h"

typedef enum afxFileFlag
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
} afxFileFlags;

AFX afxError        AfxReadFileString(afxStream file, afxString* str);

AFX afxResult       AfxFlushFileCache(afxStream file);

AFX afxBool         AfxTestFileFlags(afxStream file, afxFileFlags flags);

AFX afxBool         AfxGetFileUri(afxStream file, afxUri* location);
AFX afxBool         AfxGetResolvedFileUri(afxStream file, afxUri* location);

AFX afxError        AfxReopenFile(afxStream file, afxUri const* uri, afxFileFlags flags);
AFX afxError        AfxReloadFile(afxStream file, afxUri const* uri); // will upload the entire file data into RAM and close the file.

#endif//AFX_FILE_H
