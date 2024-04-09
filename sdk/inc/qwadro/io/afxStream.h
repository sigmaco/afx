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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "qwadro/core/afxObject.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxUri.h"

typedef enum afxIoFlag
{
    // permissions
    afxIoFlag_R         = AFX_BIT_OFFSET(0), // Readable
    afxIoFlag_W         = AFX_BIT_OFFSET(1), // Writeable
    afxIoFlag_X         = AFX_BIT_OFFSET(2), // Executable (seekable)
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

AFX_DEFINE_STRUCT(_afxIobImpl)
{
    afxError    (*read)(afxStream, void *dst, afxSize range);
    afxResult   (*readFeedback)(afxStream, afxNat32, void*);
    afxError    (*write)(afxStream, void const * const src, afxSize range);
    afxResult   (*writeFeedback)(afxStream, afxNat32, void*);
    afxNat      (*tell)(afxStream);
    afxError    (*seek)(afxStream, afxInt, afxSeekMode);
    afxBool     (*eos)(afxStream);
    afxResult   (*dtor)(afxStream);
};

AFX_DECLARE_STRUCT(_afxIobIdd);

#ifdef _AFX_CORE_C
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
#endif//_AFX_CORE_C

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

AFX afxError            AfxSeekStream(afxStream iob, afxInt offset, afxSeekMode origin);
AFX afxError            AfxSeekStreamFromBegin(afxStream iob, afxNat offset); // if offset is zero, rewind.
AFX afxError            AfxSeekStreamFromEnd(afxStream iob, afxNat offset);
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

AFX afxStream           AfxAcquireImplementedStream(afxIoFlags flags, _afxIobImpl const* pimpl, _afxIobIdd const* idd);
AFX afxStream           AfxAcquireStream(afxIoFlags flags, afxNat bufCap);

AFX afxStream           AfxLoadFile(afxUri const *uri); // will upload the entire file data into RAM and close the file.
AFX afxError            AfxReloadFile(afxStream iob, afxUri const *uri);

AFX afxStream           AfxOpenStream(afxIoFlags flags, void const* start, afxSize siz);
AFX afxStream           AfxOpenInputStream(void const* start, afxSize len);
AFX afxStream           AfxOpenOutputStream(void* buf, afxNat bufCap);

AFX afxStream           AfxWrapFile(void* fd, afxIoFlags flags);
AFX afxStream           AfxOpenFile(afxUri const* uri, afxIoFlags flags);
AFX afxError            AfxReopenFile(afxStream file, afxUri const* uri, afxIoFlags flags);

#endif//AFX_STREAM_H
