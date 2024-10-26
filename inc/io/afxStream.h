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

// This code is part of SIGMA Future Storage.

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "qwadro/inc/exec/afxAtomic.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/mem/afxMemory.h"
#include "qwadro/inc/io/afxUri.h"

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

typedef enum afxSeekOrigin
{
    afxSeekOrigin_BEGIN, // from begin
    afxSeekOrigin_CURR, // from current posn
    afxSeekOrigin_END // from end to begin
} afxSeekOrigin;

typedef afxUnit afxRwx[3];

AFX_DECLARE_STRUCT(_afxIobIdd);

AFX_DEFINE_STRUCT(afxIobImpl)
{
    afxError    (*read)(afxStream, void *dst, afxUnit32 range);
    afxResult   (*readFeedback)(afxStream, afxUnit32, void*);
    afxError    (*write)(afxStream, void const * const src, afxUnit32 range);
    afxResult   (*writeFeedback)(afxStream, afxUnit32, void*);
    afxUnit     (*tell)(afxStream);
    afxError    (*seek)(afxStream, afxSize, afxSeekOrigin);
    afxBool     (*eos)(afxStream);
    afxResult   (*dtor)(afxStream);
};

// Reads data from a stream

AFX_DEFINE_STRUCT(afxStreamSeg)
{
    afxSize offset;
    afxUnit range;
    afxUnit stride;
};

AFX afxBool         AfxIsStreamWriteable(afxStream const iob);
AFX afxBool         AfxIsStreamReadable(afxStream const iob);
AFX afxBool         AfxIsStreamReadOnly(afxStream const iob);
AFX afxBool         AfxStreamIsSeekable(afxStream const iob);

AFX afxUnit         AfxGetStreamCapacity(afxStream const iob); // total of bytes of a stream.
AFX afxSize         AfxGetStreamLength(afxStream const iob); // number of bytes available to be read.
AFX afxSize         AfxGetStreamRoom(afxStream const iob); // number of bytes available to be written.

AFX afxSize         AfxAskStreamPosn(afxStream iob);
AFX afxSize         AfxMeasureStream(afxStream iob); // = end - begin
AFX afxBool         AfxHasStreamReachedEnd(afxStream iob);

AFX afxError        AfxSeekStream(afxStream iob, afxSize offset, afxSeekOrigin origin);
AFX afxError        AfxRewindStream(afxStream iob);
AFX afxError        AfxRecedeStream(afxStream iob, afxUnit range);
AFX afxError        AfxAdvanceStream(afxStream iob, afxUnit range);

AFX void const*     AfxGetStreamBuffer(afxStream const iob, afxSize offset);
AFX afxError        AfxAdjustStreamBuffer(afxStream iob, afxUnit bufCap);


AFX afxBool         AfxResetStream(afxStream iob);

// Make a stream fork from another stream.
AFX afxStream       AfxForkStream(afxStream iob); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxStream       AfxForkStreamRange(afxStream iob, afxSize offset, afxUnit range); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError        AfxMergeStream(afxStream out, afxStream in, afxSize base, afxUnit range, afxUnit rate);

AFX afxError        AfxRead(afxStream in, afxUnit cnt, afxUnit siz, void* dst, afxUnit dstStride);
AFX afxError        AfxReadReversed(afxStream in, afxUnit cnt, afxUnit siz, void* dst, afxUnit dstStride);

AFX afxError        AfxReadAt(afxStream in, afxSize offset, afxUnit cnt, afxUnit siz, void *dst, afxUnit dstStride);
AFX afxError        AfxReadReversedAt(afxStream in, afxSize offset, afxUnit cnt, afxUnit siz, void *dst, afxUnit dstStride);

AFX afxError        AfxWrite(afxStream out, afxUnit cnt, afxUnit siz, void const* src, afxUnit srcStride);
AFX afxError        AfxWriteAt(afxStream out, afxSize offset, afxUnit cnt, afxUnit siz, void const* src, afxUnit srcStride);

AFX afxError        AfxReadStream2(afxStream in, afxUnit range, afxUnit stride, void* dst, afxUnit dstStride);
AFX afxError        AfxReadStreamAt2(afxStream in, afxSize offset, afxUnit range, afxUnit stride, void* dst, afxUnit dstStride);
AFX afxError        AfxReadStreamReversed2(afxStream in, afxUnit range, afxUnit stride, void* dst, afxUnit dstStride);
AFX afxError        AfxReadStreamReversedAt2(afxStream in, afxSize offset, afxUnit range, afxUnit stride, void* dst, afxUnit dstStride);

AFX afxError        AfxWriteStream2(afxStream out, afxUnit range, afxUnit stride, void const* src, afxUnit srcStride); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxWriteStreamAt2(afxStream out, afxSize offset, afxUnit range, afxUnit stride, void const* src, afxUnit srcStride); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError        AfxDoStreamInput(afxStream in, afxUnit rowStride, afxUnit rowCnt, void* dst, afxUnit dstStride); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxDoStreamInputAt(afxStream in, afxSize offset, afxUnit rowStride, afxUnit rowCnt, void* dst, afxUnit dstStride);
AFX afxError        AfxDoStreamOutput(afxStream out, afxUnit rowStride, afxUnit rowCnt, void const* src, afxUnit srcStride); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxDoStreamOutputAt(afxStream out, afxSize offset, afxUnit rowStride, afxUnit rowCnt, void const* src, afxUnit srcStride);

AFX afxError        AfxReadStream(afxStream in, afxUnit range, afxUnit rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxReadStreamAt(afxStream in, afxSize offset, afxUnit range, afxUnit rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxReadStreamReversed(afxStream in, afxUnit range, afxUnit rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxReadStreamReversedAt(afxStream in, afxSize offset, afxUnit range, afxUnit rate, void* dst); // em caso de erro, retorna o comprimento deixado de fora.

AFX_DEFINE_STRUCT(afxDataIo)
{
    afxSize     offset;
    afxUnit      rowStride;
    afxUnit      rowCnt;
};

AFX afxError        AfxDecodeStream(afxStream in, afxSize offset, afxUnit rowStride, afxUnit rowCnt);
AFX afxError        AfxDecodeStream2(afxStream in, afxSize offset, afxUnit encSiz, afxFcc codec, afxUnit stop0, afxUnit stop1, afxUnit stop2, void* dst);

AFX afxError        AfxWriteStream(afxStream out, afxUnit range, afxUnit rate, void const* src); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxWriteStreamAt(afxStream out, afxSize offset, afxUnit range, afxUnit rate, void const* src); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError        AfxCopyStream(afxStream in, afxUnit rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxCopyStreamRange(afxStream in, afxSize base, afxUnit range, afxUnit rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.

////////////////////////////////////////////////////////////////////////////////

typedef enum afxStreamUsage
{
    afxStreamUsage_FILE     = AFX_BIT(0), // will be used as file I/O stream/buffer.
    afxStreamUsage_PIPE     = AFX_BIT(1),
    afxStreamUsage_MEM      = AFX_BIT(2), // will be used as I/O memory buffer.
    afxStreamUsage_TCP      = AFX_BIT(3), // will be used as TCP communication stream/buffer.
    afxStreamUsage_TTY      = AFX_BIT(4) // will be used as console I/O stream/buffer.
} afxStreamUsage;

typedef enum afxStreamFlag
{
    afxStreamFlag_READABLE  = AFX_BIT(0),
    afxStreamFlag_WRITEABLE = AFX_BIT(1),
    afxStreamFlag_SEEKABLE  = AFX_BIT(2),
    afxStreamFlag_RESIZABLE = AFX_BIT(3),
    afxStreamFlag_RING      = AFX_BIT(4)
} afxStreamFlags;

AFX_DEFINE_STRUCT(afxStreamInfo)
{
    afxUnit          bufCap;
    afxStreamUsage  usage;
    afxStreamFlags  flags;
};

AFX afxIobImpl const    stdStreamImpl;

AFX afxError            AfxAcquireImplementedStream(afxIobImpl const* pimpl, afxUnit cnt, afxStreamInfo const infos[], afxStream streams[]);
AFX afxError            AfxAcquireStream(afxUnit cnt, afxStreamInfo const infos[], afxStream streams[]);

AFX afxError            AfxReopenStream(afxStream iob, void* buf, afxSize siz);
AFX afxError            AfxReopenInputStream(afxStream iob, void const* start, afxSize len);
AFX afxError            AfxReopenOutputStream(afxStream iob, void* buf, afxUnit bufCap);

#endif//AFX_STREAM_H
