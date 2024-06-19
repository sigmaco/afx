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

#ifndef AVX_BUFFERED_STRING_H
#define AVX_BUFFERED_STRING_H

#include "qwadro/base/afxString.h"

#define AFX_BUFSTR_MAX_CAP AFX_N16_MAX

enum
{
    _AFX_STR_FLAG_ALLOCED = AFX_BIT(0)
};

AFX_DEFINE_STRUCT(afxRestring)
{
    afxString           str;
    afxNat16            cap; // buffered
    afxNat16            flags; // 0 = dynamic, 1 = remote source, 2 = string source
};

AFXINL void             AfxResetRestring(afxRestring *str);
AFXINL void             AfxMakeRestring(afxRestring *str, afxNat cap, void *start, afxNat len);  // wraps dynamic (writeable) data as a Qwadro string.
AFXINL afxNat           AfxExcerptRestring(afxRestring const* str, afxNat base, afxNat len, afxRestring* excerpt);
AFXINL void             AfxReflectRestring(afxRestring const* str, afxRestring* reflection);

AFX void                AfxDeallocateString(afxRestring* str);
AFX afxError            AfxAllocateString(afxRestring* str, afxNat cap, void const *start, afxNat len);
AFX afxError            AfxReallocateString(afxRestring* str, afxNat cap, void const *start, afxNat len);

AFX void*               AfxGetStringStorage(afxRestring const* str, afxNat base);

AFX afxNat              AfxGetStringCapacity(afxRestring const* str);
AFX afxBool             AfxStringIsWriteable(afxRestring const* str);

AFX void                AfxClearString(afxRestring *str);

AFX afxError            AfxCloneString(afxRestring* str, afxString const* in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneStringRange(afxRestring* str, afxString const* in, afxNat base, afxNat len); // wraps the same content mapped or buffered by other string as writeable data.

AFX afxNat              AfxCopyString(afxRestring* str, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxCopyStringRange(afxRestring* str, afxString const* in, afxNat base, afxNat len); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxConcatenateString(afxRestring* str, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxConcatenateStringL(afxRestring* str, afxChar const *start, afxNat len); // Return clamped off (non-copied) length if any.

AFX afxNat              _AfxWriteString(afxRestring* str, afxNat base, void const *src, afxNat len); // copy into

AFX afxNat              AfxFormatString(afxRestring* str, afxChar const *fmt, ...);
AFX afxNat              AfxFormatStringArg(afxRestring* str, afxChar const *fmt, va_list args);

AFX afxNat              AfxInsertString(afxRestring* str, afxNat at, afxString const* include);
AFX afxNat              AfxEraseString(afxRestring* str, afxNat at, afxNat len);

#endif//AVX_BUFFERED_STRING_H
