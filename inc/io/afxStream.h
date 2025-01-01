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
#include "qwadro/inc/io/afxRing.h"

#define AFX_DEFAULT_IOB_SIZE 512

typedef enum afxIoUsage
{
    afxIoUsage_MEM      = AFX_BIT(0), // will be used as I/O memory buffer.
    afxIoUsage_FILE     = AFX_BIT(1), // will be used as file I/O stream/buffer.
    afxIoUsage_PIPE     = AFX_BIT(2),
    afxIoUsage_TCP      = AFX_BIT(3), // will be used as TCP communication stream/buffer.
    afxIoUsage_TTY      = AFX_BIT(4) // will be used as console I/O stream/buffer.
} afxIoUsage;

typedef enum afxIoFlag
{
    // permissions
    afxIoFlag_R         = AFX_BIT(0), // Readable (read-only)
    afxIoFlag_READABLE  = afxIoFlag_R,
    afxIoFlag_W         = AFX_BIT(1), // Writeable
    afxIoFlag_WRITEABLE = afxIoFlag_W,
    afxIoFlag_X         = AFX_BIT(2), // Executable (seekable)
    afxIoFlag_SEEKABLE  = afxIoFlag_X,

    afxIoFlag_RW        = (afxIoFlag_R | afxIoFlag_W), // (read/write, create if not exists)
    afxIoFlag_RX        = (afxIoFlag_R | afxIoFlag_X), // read or create
    afxIoFlag_WX        = (afxIoFlag_W | afxIoFlag_X), // 
    afxIoFlag_RWX       = (afxIoFlag_R | afxIoFlag_W | afxIoFlag_X), // 
    afxIoModeMask       = afxIoFlag_RWX,

    afxIoFlag_RESIZABLE = AFX_BIT(3),
    afxIoFlag_RING      = AFX_BIT(4)
} afxIoFlags;

typedef enum afxSeekOrigin
{
    afxSeekOrigin_BEGIN, // from begin
    afxSeekOrigin_CURR, // from current posn
    afxSeekOrigin_END // from end to begin
} afxSeekOrigin;

AFX_DEFINE_STRUCT(afxStreamInfo)
{
    afxUnit          bufCap;
    afxIoUsage  usage;
    afxIoFlags  flags;
};

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
    afxError    (*buf)(afxStream,afxUnit,void*);
};

AFX afxIobImpl const stdStreamImpl;

AFX afxBool         AfxIsStreamWriteable(afxStream const iob);
AFX afxBool         AfxIsStreamReadable(afxStream const iob);
AFX afxBool         AfxIsStreamReadOnly(afxStream const iob);
AFX afxBool         AfxStreamIsSeekable(afxStream const iob);

AFX afxUnit         AfxGetStreamCapacity(afxStream const iob); // total of bytes of a stream.
AFX afxSize         AfxGetStreamLength(afxStream const iob); // number of bytes available to be read.
AFX afxSize         AfxGetStreamRoom(afxStream const iob); // number of bytes available to be written.

/**
    The AfxAskStreamPosn() method gets the current position in a stream.

    @param iob the stream object that identifies the stream.
    @return Returns the current value of the position indicator of the stream.
*/

AFX afxSize         AfxAskStreamPosn(afxStream iob);

AFX afxSize         AfxMeasureStream(afxStream iob); // = end - begin

/**
    The AfxHasStreamReachedEnd() method checks for end-of-file indicator.

    Checks whether the end-of-File indicator associated with stream is set, returning a value different from zero if it is.
    This indicator is generally set by a previous operation on the stream that attempted to read at or past the end-of-file.
    Notice that stream's internal position indicator may point to the end-of-file for the next operation, but still, the 
    end-of-file indicator may not be set until an operation attempts to read at that point.

    @param iob the stream object that identifies the stream.
    @return A non-zero value is returned in the case that the end-of-file indicator associated with the stream is set.
    Otherwise, zero is returned.
*/

AFX afxBool         AfxHasStreamReachedEnd(afxStream iob);

/**
    The AfxSeekStream() method repositions a stream position indicator.
    
    Sets the position indicator associated with the stream to a new position.

    The new position is defined by adding offset to a reference position specified by origin.

    @param iob is the stream object that identifies the stream.
    @param offset is the umber of bytes to offset from origin.
    @param origin is the position used as reference for the offset.
    @return If successful, the function returns zero. Otherwise, it returns non-zero value.
*/

AFX afxError        AfxSeekStream(afxStream iob, afxSize offset, afxSeekOrigin origin);

/**
    The AfxRewindStream() method sets the position of a stream to the beginning.

    Sets the position indicator associated with stream to the beginning of the file.

    @param iob the stream object that identifies the stream.
    @return Returns any error occuried.
*/

AFX afxError        AfxRewindStream(afxStream iob);

AFX afxError        AfxRecedeStream(afxStream iob, afxUnit range);

AFX afxError        AfxAdvanceStream(afxStream iob, afxUnit range);

AFX void const*     AfxGetStreamBuffer(afxStream const iob, afxSize offset);

/**
    The AfxBufferizeStream() method changes the stream buffering.

    Specifies a buffer for stream. The function allows to specify the mode and size of the buffer (in bytes).
    
    If buffer is a nil, the function automatically allocates a buffer (using bufCap as a hint on the size to use). 
    
    Otherwise, the array pointed by buf may be used as a buffer of bufCap bytes.

    This function should be called once the stream has been associated with an open file, 
    but before any input or output operation is performed with it.

    @param iob the stream object that identifies the stream.
    @param bufCap is the buffer size, in bytes.
    @param buf is the user-allocated buffer. Shall be at least bufCap bytes long.
    @return Returns any error occuried.
*/

AFX afxError        AfxBufferizeStream(afxStream iob, afxUnit bufCap, void* buf);

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

AFX afxError        AfxDecodeStream(afxStream in, afxSize offset, afxUnit rowStride, afxUnit rowCnt);
AFX afxError        AfxDecodeStream2(afxStream in, afxSize offset, afxUnit encSiz, afxFcc codec, afxUnit stop0, afxUnit stop1, afxUnit stop2, void* dst);

AFX afxError        AfxWriteStream(afxStream out, afxUnit range, afxUnit rate, void const* src); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxWriteStreamAt(afxStream out, afxSize offset, afxUnit range, afxUnit rate, void const* src); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError        AfxCopyStream(afxStream in, afxUnit rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError        AfxCopyStreamRange(afxStream in, afxSize base, afxUnit range, afxUnit rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError        AfxReopenStream(afxStream iob, afxUnit bufCap, void* start, afxSize len);

////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxOpenStream(afxUnit bufCap, void* start, afxUnit len, afxIoFlags flags, afxStream* stream);

AFX afxError        AfxAcquireStream(afxUnit cnt, afxStreamInfo const infos[], afxStream streams[]);

AFX afxError        AfxAcquireImplementedStream(afxIobImpl const* pimpl, afxUnit cnt, afxStreamInfo const infos[], afxStream streams[]);

#endif//AFX_STREAM_H
