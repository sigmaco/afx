/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "afx/core/afxObject.h"
#include "afx/core/mem/afxMemory.h"

typedef enum
{
    // permissions
    AFX_IO_FLAG_R           = AFX_FLAG(0), // Readable
    AFX_IO_FLAG_W           = AFX_FLAG(1), // Writeable
    AFX_IO_FLAG_X           = AFX_FLAG(2), // Executable (seekable)
    AFX_IO_FLAG_RW          = (AFX_IO_FLAG_R | AFX_IO_FLAG_W),
    AFX_IO_FLAG_RX          = (AFX_IO_FLAG_R | AFX_IO_FLAG_X),
    AFX_IO_FLAG_WX          = (AFX_IO_FLAG_W | AFX_IO_FLAG_X),
    AFX_IO_FLAG_RWX         = (AFX_IO_FLAG_R | AFX_IO_FLAG_W | AFX_IO_FLAG_X),
    AFX_IO_PERM_MASK        = AFX_IO_FLAG_RWX,

    // attributes
    AFX_IO_FLAG_U           = AFX_FLAG(3), // Unbuffered.
    AFX_IO_FLAG_L           = AFX_FLAG(4), // Literal
    AFX_IO_FLAG_V           = AFX_FLAG(5), // Virtual. This flag is reserved for system use.
    AFX_IO_FLAG_Q           = AFX_FLAG(6), // Qwadro. This flag is reserved for system use.
    AFX_IO_FLAG_D           = AFX_FLAG(7),  // Device. This flag is reserved for system use.
    AFX_IO_FLAG_H           = AFX_FLAG(8),  // Hidden. The stream is hidden. It shouldn't be included in an ordinary listing.
    AFX_IO_FLAG_A           = AFX_FLAG(9),  // Archived. The stream is an archive-derived stream.
    AFX_IO_FLAG_T           = AFX_FLAG(10), // Temporary. Used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed. This is used by SIGMA Future Storage.
    AFX_IO_FLAG_Z           = AFX_FLAG(11), // Compressed. The data is compressed. For a file, all of the data in the file is compressed. This is used by SIGMA Future Storage.
    AFX_IO_FLAG_E           = AFX_FLAG(12), // Encrypted. The data is encrypted. For a file, all data streams in the file are encrypted. This is used by SIGMA Future Storage.
    AFX_IO_FLAG_O           = AFX_FLAG(13), // Offline. The data isn't available immediately. This flag indicates that the data is physically moved to remote/offline storage. This is used by SIGMA Future Storage.
    AFX_IO_FLAG_S           = AFX_FLAG(14), // Sparse. The data is a sparse file. Empty ranges (zeroed ranges) are generated dynamically by some algorithm. This is used by SIGMA Future Storage.
}
afxIoFlags;

typedef enum
{
    AFX_STRM_ORIGIN_BEGIN,
    AFX_STRM_ORIGIN_CURRENT,
    AFX_STRM_ORIGIN_END
}
afxStreamOrigin;

typedef afxNat afxRwx[3];

AFX_DEFINE_HANDLE(afxStream);

AFX_DEFINE_STRUCT(afxStreamImplementation)
{
    afxError    (*read)(afxStream, void *dst, afxNat len);
    afxResult   (*readProgressFb)(afxStream, afxNat32, void*);
    afxError    (*write)(afxStream, void const * const src, afxNat len);
    afxResult   (*writeProgressFb)(afxStream, afxNat32, void*);
    afxNat      (*tell)(afxStream);
    afxResult   (*seek)(afxStream, afxInt, afxStreamOrigin);
    afxBool     (*eos)(afxStream);
};

AFX_OBJECT(afxStream)
{
    afxObject                       obj;
#ifdef _AFX_STREAM_C
    afxStreamImplementation const   *impl;
    afxIoFlags                      flags;
    afxNat32                        bufCap;
    afxByte                         *buffer;
    afxNat32                        currPosn;
#endif
};

AFX void*               AfxStreamGetSystem(afxStream ios);
AFX void*               AfxStreamGetFileSystem(afxStream ios);

AFX afxNat              AfxStreamAsk(afxStream ios);
AFX afxBool             AfxStreamHasReachedEnd(afxStream ios);
AFX afxNat              AfxStreamMeasure(afxStream ios);
AFX afxResult           AfxStreamGoToBegin(afxStream ios, afxInt offset); // if offset is zero, rewind.
AFX afxResult           AfxStreamGoToEnd(afxStream ios, afxInt offset);
AFX afxResult           AfxStreamSkip(afxStream ios, afxInt range);
AFX afxResult           AfxStreamSeek(afxStream ios, afxInt offset, afxStreamOrigin origin);
AFX afxResult           AfxStreamRecede(afxStream ios, afxInt range);

AFX afxBool             AfxStreamIsReadOnly(afxStream ios);
AFX afxBool             AfxStreamIsReadable(afxStream ios);
AFX afxBool             AfxStreamIsWriteable(afxStream ios);
AFX afxBool             AfxStreamIsExecutable(afxStream ios);
AFX afxBool             AfxStreamIsVirtual(afxStream ios);
AFX afxBool             AfxStreamIsTemporary(afxStream ios);
AFX afxBool             AfxStreamIsSparse(afxStream ios);
AFX afxBool             AfxStreamIsCompressed(afxStream ios);
AFX afxBool             AfxStreamIsEncrypted(afxStream ios);
AFX afxBool             AfxStreamIsArchive(afxStream ios);
AFX afxBool             AfxStreamIsHidden(afxStream ios);
AFX afxBool             AfxStreamIsReserved(afxStream ios);

AFX afxResult           AfxStreamRead(afxStream ios, void *dst, afxNat len);
AFX afxResult           AfxStreamResizeBuffer(afxStream ios, afxNat siz);
AFX afxResult           AfxStreamWrite(afxStream ios, void const *src, afxNat len);

#endif//AFX_STREAM_H