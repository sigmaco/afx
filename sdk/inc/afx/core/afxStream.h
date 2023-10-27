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

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "afx/core/afxInstance.h"
#include "afx/core/afxContext.h"

typedef enum afxIoFlags
{
    // permissions
    AFX_IO_FLAG_R           = AFX_BIT_OFFSET(0), // Readable
    AFX_IO_FLAG_W           = AFX_BIT_OFFSET(1), // Writeable
    AFX_IO_FLAG_X           = AFX_BIT_OFFSET(2), // Executable (seekable)
    AFX_IO_FLAG_RW          = (AFX_IO_FLAG_R | AFX_IO_FLAG_W),
    AFX_IO_FLAG_RX          = (AFX_IO_FLAG_R | AFX_IO_FLAG_X),
    AFX_IO_FLAG_WX          = (AFX_IO_FLAG_W | AFX_IO_FLAG_X),
    AFX_IO_FLAG_RWX         = (AFX_IO_FLAG_R | AFX_IO_FLAG_W | AFX_IO_FLAG_X),
    AFX_IO_PERM_MASK        = AFX_IO_FLAG_RWX,

    AFX_IO_FLAG_FORCE_NAT   = AFX_N32_MAX
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

AFX_DECLARE_STRUCT(afxStream);

AFX_DEFINE_STRUCT(afxStreamIoCtrl)
{
    afxError(*read)(afxStream*, void *dst, afxNat len);
    afxResult(*readFeedback)(afxStream*, afxNat32, void*);
    afxError(*write)(afxStream*, void const * const src, afxNat len);
    afxResult(*writeFeedback)(afxStream*, afxNat32, void*);
    afxNat(*tell)(afxStream*);
    afxError(*seek)(afxStream*, afxInt, afxStreamOrigin);
    afxBool(*eos)(afxStream*);
};

AFX_DEFINE_STRUCT(afxStream)
{
    _AFX_DBG_FCC
    afxStreamIoCtrl         ioctl;
    void                    (*dtor)(afxStream*);
    afxIoFlags              flags;
    afxNat32                len;
    afxNat32                posn;
    afxBool                 forked;
    struct
    {
        union
        {
            afxNat          cap; // when buffered
            afxNat          base; // when forked
        };
        union
        {
            afxByte*        start; // when wrapped or externally buffered.
            afxStream*      parent; // when forked
        };
    }                       src;
    afxBool8                isUserBuf; // when buffered
    void*                   idd;
};

AFX void                    AfxReleaseStream(afxStream* ios);

AFX afxError                AfxAcquireStream(afxStream *ios, afxIoFlags flags, void *start, afxNat len);
AFX afxError                AfxAcquireInputStream(afxStream *in, void const *start, afxNat len);
AFX afxError                AfxAcquireOutputStream(afxStream *out, void *start, afxNat len);

// Make a stream fork from another stream.
AFX afxError                AfxForkStream(afxStream* ios, afxStream* parent); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxForkStreamRange(afxStream* ios, afxStream* parent, afxNat base, afxNat range); // em caso de erro, retorna o comprimento deixado de fora.

// Reads data from a stream
AFX afxError                AfxReadStream(afxStream* in, void *dst, afxNat len, afxNat feedback); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxReadStreamAt(afxStream* in, afxNat offset, void *dst, afxNat len, afxNat feedback); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError                AfxWriteStream(afxStream* out, void const *src, afxNat len, afxNat feedback); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxWriteStreamAt(afxStream* out, afxNat offset, void const *src, afxNat len, afxNat feedback); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxNat                  AfxAskStreamPosn(afxStream* ios);
AFX afxNat                  AfxMeasureStream(afxStream* ios);
AFX afxBool                 AfxTestStreamEndCondition(afxStream* ios);

AFX afxError                AfxGoToStreamBegin(afxStream* ios, afxInt offset); // if offset is zero, rewind.
AFX afxError                AfxGoToStreamEnd(afxStream* ios, afxInt offset);
AFX afxError                AfxSkipStream(afxStream* ios, afxInt range);
AFX afxError                AfxSeekStream(afxStream* ios, afxInt offset, afxStreamOrigin origin);
AFX afxError                AfxRecedeStream(afxStream* ios, afxInt range);

AFX afxError                AfxCopyStream(afxStream *out, afxStream *in, afxNat feedback); // em caso de erro, retorna o comprimento deixado de fora.
AFX afxError                AfxCopyStreamRange(afxStream *out, afxStream *in, afxNat base, afxNat range, afxNat feedback); // em caso de erro, retorna o comprimento deixado de fora.

AFX afxError                AfxResizeStreamBuffer(afxStream* ios, afxNat siz);

AFXINL afxBool              AfxStreamIsReadOnly(afxStream const* ios);
AFXINL afxBool              AfxStreamIsReadable(afxStream const* ios);
AFXINL afxBool              AfxStreamIsWriteable(afxStream const* ios);
AFXINL afxBool              AfxStreamIsExecutable(afxStream const* ios);

AFXINL void const*          AfxGetStreamData(afxStream const* ios, afxNat offset);
AFXINL afxNat               AfxGetStreamLength(afxStream const* ios);
AFXINL afxNat               AfxGetStreamBufferCap(afxStream const* ios);

AFX afxError                _AfxStandardStreamReadCallback(afxStream* ios, void *dst, afxNat siz);
AFX afxError                _AfxStandardStreamWriteCallback(afxStream* ios, void const* const src, afxNat siz);
AFX afxNat                  _AfxStandardStreamAskCursorCallback(afxStream* ios);
AFX afxError                _AfxStandardStreamMoveCursorCallback(afxStream* ios, afxInt offset, afxStreamOrigin origin);
AFX afxBool                 _AfxStandardTestStreamEndConditionCallback(afxStream* ios);

#endif//AFX_STREAM_H