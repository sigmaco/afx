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

#ifndef AFX_BUFFERED_STRING_H
#define AFX_BUFFERED_STRING_H

#include "qwadro/core/afxString.h"

#define AFX_BUFSTR_MAX_CAP AFX_N16_MAX

enum
{
    _AFX_STR_FLAG_ALLOCED = AfxGetBitOffset(0)
};

AFX_DEFINE_STRUCT(afxBufferedString)
{
    afxString           str;
    afxNat16            cap; // buffered
    afxNat16            flags; // 0 = dynamic, 1 = remote source, 2 = string source
};

AFX void                AfxResetBufferedString(afxBufferedString *str);
AFX void                AfxMakeBufferedString(afxBufferedString *str, afxNat cap, void *start, afxNat len);  // wraps dynamic (writeable) data as a Qwadro string.

AFX void                AfxDeallocateString(afxBufferedString* str);
AFX afxError            AfxAllocateString(afxBufferedString* str, afxNat cap, void const *start, afxNat len);
AFX afxError            AfxReallocateString(afxBufferedString* str, afxNat cap, void const *start, afxNat len);

AFX void*               AfxGetBufferedStringData(afxBufferedString* str, afxNat base);

AFX afxNat              AfxGetStringCapacity(afxBufferedString const* str);
AFX afxBool             AfxStringIsWriteable(afxBufferedString const* str);

AFX void                AfxClearBufferedString(afxBufferedString *str);

AFX afxError            AfxCloneString(afxBufferedString* str, afxString const* in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneStringRange(afxBufferedString* str, afxString const* in, afxNat base, afxNat len); // wraps the same content mapped or buffered by other string as writeable data.

AFX afxNat              AfxCopyString(afxBufferedString* str, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxCopyStringRange(afxBufferedString* str, afxString const* in, afxNat base, afxNat len); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxAppendString(afxBufferedString* str, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxAppendStringLiteral(afxBufferedString* str, afxChar const *start, afxNat len); // Return clamped off (non-copied) length if any.

AFX afxNat              _AfxWriteString(afxBufferedString* str, afxNat base, void const *src, afxNat len); // copy into

AFX afxNat              AfxFormatString(afxBufferedString* str, afxChar const *fmt, ...);
AFX afxNat              AfxFormatStringArg(afxBufferedString* str, afxChar const *fmt, va_list args);

AFX afxNat              AfxInsertString(afxBufferedString* str, afxNat at, afxString const* include);
AFX afxNat              AfxEraseString(afxBufferedString* str, afxNat at, afxNat len);

#endif//AFX_BUFFERED_STRING_H
