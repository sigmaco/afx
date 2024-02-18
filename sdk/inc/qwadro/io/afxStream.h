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

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "qwadro/core/afxObject.h"
#include "qwadro/mem/afxMmu.h"

typedef enum afxIoFlag
{
    // permissions
    afxIoFlag_R         = AfxGetBitOffset(0), // Readable
    afxIoFlag_W         = AfxGetBitOffset(1), // Writeable
    afxIoFlag_X         = AfxGetBitOffset(2), // Executable (seekable)
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

AFX_DEFINE_STRUCT(afxStreamIoCtrl)
{
    afxError(*read)(afxStream, void *dst, afxSize range);
    afxResult(*readFeedback)(afxStream, afxNat32, void*);
    afxError(*write)(afxStream, void const * const src, afxSize range);
    afxResult(*writeFeedback)(afxStream, afxNat32, void*);
    afxNat(*tell)(afxStream);
    afxError(*seek)(afxStream, afxInt, afxSeekMode);
    afxBool(*eos)(afxStream);
};

#ifdef _AFX_CORE_C
#ifdef _AFX_STREAM_C
AFX_OBJECT(afxStream)
{
    _AFX_DBG_FCC;
    afxStreamIoCtrl         ioctl;
    void                    (*dtor)(afxStream);
    afxIoFlags              flags;
    afxSize                 range;
    afxSize                 posn;
    struct
    {
        afxSize             cap; // when buffered
        afxByte*            start; // when wrapped or externally buffered.
    }                       src;
    afxBool                 isUserBuf; // when buffered
    void*                   idd;
};
#endif//_AFX_STREAM_C
#endif//_AFX_CORE_C

AFX afxStream               AfxAcquireStream(afxNat cap, afxIoFlags flags, void const* start, afxSize range);
AFX afxStream               AfxAcquireInputStream(afxNat cap, void const* start, afxSize range);
AFX afxStream               AfxAcquireOutputStream(afxNat cap, void* start, afxSize range);

// Make a stream fork from another stream.
AFX afxStream               AfxForkStream(afxStream ios); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxStream               AfxForkStreamRange(afxStream ios, afxSize offset, afxNat range); // em caso de erro, retorna o comprimento deixado de fora.

// Reads data from a stream

AFX_DEFINE_STRUCT(afxStreamSeg)
{
    afxSize     offset;
    afxNat      range;
    afxNat      stride;
};

AFX afxError                AfxRead(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride);
AFX afxError                AfxReadReversed(afxStream in, afxNat cnt, afxNat siz, void* dst, afxNat dstStride);

AFX afxError                AfxReadAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride);
AFX afxError                AfxReadReversedAt(afxStream in, afxSize offset, afxNat cnt, afxNat siz, void *dst, afxNat dstStride);

AFX afxError                AfxWrite(afxStream out, afxNat cnt, afxNat siz, void const* src, afxNat srcStride);
AFX afxError                AfxWriteAt(afxStream out, afxSize offset, afxNat cnt, afxNat siz, void const* src, afxNat srcStride);

AFX afxError                AfxReadStream2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride);
AFX afxError                AfxReadStreamAt2(afxStream in, afxSize offset, afxNat range, afxNat stride, void* dst, afxNat dstStride);
AFX afxError                AfxReadStreamReversed2(afxStream in, afxNat range, afxNat stride, void* dst, afxNat dstStride);
AFX afxError                AfxReadStreamReversedAt2(afxStream in, afxSize offset, afxNat range, afxNat stride, void* dst, afxNat dstStride);

AFX afxError                AfxWriteStream2(afxStream out, afxNat range, afxNat stride, void const* src, afxNat srcStride); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxWriteStreamAt2(afxStream out, afxSize offset, afxNat range, afxNat stride, void const* src, afxNat srcStride); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError                AfxReadStream(afxStream in, void* dst, afxNat range, afxNat rate); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxReadStreamAt(afxStream in, afxSize offset, void* dst, afxNat range, afxNat rate); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxReadStreamReversed(afxStream in, void* dst, afxNat range, afxNat rate); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxReadStreamReversedAt(afxStream in, afxSize offset, void* dst, afxNat range, afxNat rate); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError                AfxWriteStream(afxStream out, void const* src, afxNat range, afxNat rate); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxWriteStreamAt(afxStream out, afxSize offset, void const* src, afxNat range, afxNat rate); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxSize                 AfxAskStreamPosn(afxStream ios);
AFX afxSize                 AfxMeasureStream(afxStream ios);
AFX afxBool                 AfxTestStreamEndCondition(afxStream ios);

AFX afxError                AfxGoToStreamBegin(afxStream ios, afxInt offset); // if offset is zero, rewind.
AFX afxError                AfxGoToStreamEnd(afxStream ios, afxInt offset);
AFX afxError                AfxSkipStream(afxStream ios, afxInt range);
AFX afxError                AfxSeekStream(afxStream ios, afxInt offset, afxSeekMode origin);
AFX afxError                AfxRecedeStream(afxStream ios, afxNat range);

AFX afxError                AfxCopyStream(afxStream in, afxNat rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxCopyStreamRange(afxStream in, afxSize base, afxNat range, afxNat rate, afxStream out); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError                AfxReadjustStreamBuffer(afxStream ios, afxNat siz);

AFXINL afxBool              AfxStreamIsReadOnly(afxStream const ios);
AFXINL afxBool              AfxStreamIsReadable(afxStream const ios);
AFXINL afxBool              AfxStreamIsWriteable(afxStream const ios);
AFXINL afxBool              AfxStreamIsExecutable(afxStream const ios);

AFXINL void const*          AfxGetStreamData(afxStream const ios, afxSize offset);
AFXINL afxSize              AfxGetStreamLength(afxStream const ios);
AFXINL afxNat               AfxGetStreamBufferCap(afxStream const ios);

AFX afxError                _AfxStandardStreamReadCallback(afxStream ios, void* dst, afxNat siz);
AFX afxError                _AfxStandardStreamWriteCallback(afxStream ios, void const* const src, afxNat siz);
AFX afxNat                  _AfxStandardStreamAskCursorCallback(afxStream ios);
AFX afxError                _AfxStandardStreamMoveCursorCallback(afxStream ios, afxInt offset, afxSeekMode origin);
AFX afxBool                 _AfxStandardTestStreamEndConditionCallback(afxStream ios);

#endif//AFX_STREAM_H
