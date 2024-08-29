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

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "qwadro/exec/afxAtomic.h"
#include "qwadro/base/afxObject.h"
#include "qwadro/mem/afxMemory.h"
#include "qwadro/io/afxUri.h"

typedef enum afxIoFlag
{
    // permissions
    afxIoFlag_R         = AFX_BIT(0), // Readable
    afxIoFlag_W         = AFX_BIT(1), // Writeable
    afxIoFlag_X         = AFX_BIT(2), // Executable (seekable)
    afxIoFlag_RW        = (afxIoFlag_R | afxIoFlag_W),
    afxIoFlag_RX        = (afxIoFlag_R | afxIoFlag_X),
    afxIoFlag_WX        = (afxIoFlag_W | afxIoFlag_X),
    afxIoFlag_RWX       = (afxIoFlag_R | afxIoFlag_W | afxIoFlag_X),
    AFX_IO_PERM_MASK    = afxIoFlag_RWX
} afxIoFlags;

typedef enum afxSeekMode
{
    afxSeekMode_ABSOLUTE, // from begin
    afxSeekMode_RELATIVE, // from current posn
    afxSeekMode_INVERSE // from end to begin
} afxSeekMode;

typedef afxNat afxRwx[3];

AFX_DECLARE_STRUCT(_afxIobIdd);

AFX_DEFINE_STRUCT(_afxIobImpl)
{
    afxError    (*read)(afxStream, void *dst, afxNat32 range);
    afxResult   (*readFeedback)(afxStream, afxNat32, void*);
    afxError    (*write)(afxStream, void const * const src, afxNat32 range);
    afxResult   (*writeFeedback)(afxStream, afxNat32, void*);
    afxNat      (*tell)(afxStream);
    afxError    (*seek)(afxStream, afxSize, afxSeekMode);
    afxBool     (*eos)(afxStream);
    afxResult   (*dtor)(afxStream);
};

// Reads data from a stream

AFX_DEFINE_STRUCT(afxStreamSeg)
{
    afxSize     offset;
    afxNat      range;
    afxNat      stride;
};

AFX afxBool             AfxCloseStream(afxStream iob);

// Make a stream fork from another stream.
AFX afxStream           AfxForkStream(afxStream iob); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxStream           AfxForkStreamRange(afxStream iob, afxSize offset, afxNat range); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError            AfxRead(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride);
AFX afxError            AfxReadReversed(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride);

AFX afxError            AfxReadAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride);
AFX afxError            AfxReadReversedAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride);

AFX afxError            AfxWrite(afxStream out, afxNat cnt, afxNat siz, void const* src, afxNat srcStride);
AFX afxError            AfxWriteAt(afxStream out, afxSize offset, afxNat cnt, afxNat siz, void const* src, afxNat srcStride);

AFX afxError            AfxReadStream2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride);
AFX afxError            AfxReadStreamAt2(afxStream in, afxSize offset, afxNat range, afxNat stride, void* dst, afxNat dstStride);
AFX afxError            AfxReadStreamReversed2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride);
AFX afxError            AfxReadStreamReversedAt2(afxStream in, afxSize offset, afxNat range, afxNat stride, void* dst, afxNat dstStride);

AFX afxError            AfxWriteStream2(afxStream out, afxNat range, afxNat stride, void const* src, afxNat srcStride); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError            AfxWriteStreamAt2(afxStream out, afxSize offset, afxNat range, afxNat stride, void const* src, afxNat srcStride); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError            AfxReadStream(afxStream in, afxNat range, afxNat rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError            AfxReadStreamAt(afxStream in, afxSize offset, afxNat range, afxNat rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError            AfxReadStreamReversed(afxStream in, afxNat range, afxNat rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError            AfxReadStreamReversedAt(afxStream in, afxSize offset, afxNat range, afxNat rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError            AfxWriteStream(afxStream out, afxNat range, afxNat rate, void const* src); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError            AfxWriteStreamAt(afxStream out, afxSize offset, afxNat range, afxNat rate, void const* src); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxSize             AfxGetStreamPosn(afxStream iob);
AFX afxSize             AfxMeasureStream(afxStream iob);
AFX afxBool             AfxStreamReachedEnd(afxStream iob);

AFX afxError            AfxSeekStream(afxStream iob, afxSize offset, afxSeekMode origin);
AFX afxError            AfxSeekStreamFromBegin(afxStream iob, afxSize offset); // if offset is zero, rewind.
AFX afxError            AfxSeekStreamFromEnd(afxStream iob, afxSize offset);
AFX afxError            AfxAdvanceStream(afxStream iob, afxNat range);
AFX afxError            AfxRecedeStream(afxStream iob, afxNat range);
AFXINL afxError         AfxRewindStream(afxStream iob);

AFX afxError            AfxCopyStream(afxStream in, afxNat rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError            AfxCopyStreamRange(afxStream in, afxSize base, afxNat range, afxNat rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError            AfxAdjustStreamBuffer(afxStream iob, afxNat siz);

AFXINL afxBool          AfxStreamIsReadOnly(afxStream const iob);
AFXINL afxBool          AfxStreamIsReadable(afxStream const iob);
AFXINL afxBool          AfxStreamIsWriteable(afxStream const iob);
AFXINL afxBool          AfxStreamIsExecutable(afxStream const iob);

AFXINL afxSize          AfxGetStreamLength(afxStream const iob);
AFXINL void const*      AfxGetStreamBuffer(afxStream const iob, afxSize offset);
AFXINL afxNat           AfxGetStreamBufferCap(afxStream const iob);

////////////////////////////////////////////////////////////////////////////////

AFX _afxIobImpl const   stdStreamImpl;

AFX afxError            AfxAcquireImplementedStream(afxNat diskId, afxIoFlags flags, _afxIobImpl const* pimpl, _afxIobIdd const* idd, afxStream* stream);
AFX afxError            AfxAcquireStream(afxNat diskId, afxIoFlags flags, afxNat bufCap, afxStream* stream);

AFX afxError            AfxLoadFile(afxUri const *uri, afxStream* stream); // will upload the entire file data into RAM and close the file.
AFX afxError            AfxReloadFile(afxStream iob, afxUri const *uri);

AFX afxError            AfxOpenStream(afxIoFlags flags, void const* start, afxSize siz, afxStream* stream);
AFX afxError            AfxOpenInputStream(void const* start, afxSize len, afxStream* stream);
AFX afxError            AfxOpenOutputStream(void* buf, afxNat bufCap, afxStream* stream);

AFX afxError            AfxWrapFile(void* fd, afxIoFlags flags, afxStream* stream);
AFX afxError            AfxOpenFile(afxUri const* uri, afxIoFlags flags, afxStream* stream);
AFX afxError            AfxReopenFile(afxStream file, afxUri const* uri, afxIoFlags flags);

#endif//AFX_STREAM_H
