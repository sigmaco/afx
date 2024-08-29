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

#ifndef AFX_STRING_H
#define AFX_STRING_H

#include "qwadro/inc/base/afxRawString.h"
#include "qwadro/inc/base/afxFcc.h"

#define AFX_STR_MAX_LEN AFX_N32_MAX

// UTF-8 everywhere

AFX_DEFINE_STRUCT(afxString)
{
    afxNat              len;
    afxNat              cap; // buffered
    union
    {
        afxChar const*  start; // wrapped or externally buffered.
        afxSize         offset;
        afxChar*        buf;
    };
};

#define AFX_STRING(text_) { .len = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .start = (afxChar const*)(text_) }
#define AfxString(text_) (afxString const)AFX_STRING((text_))
#define AfxStaticString(text_) (afxString const)AFX_STRING((text_))

// NOTE: When 'cap' is zero, a string can't be modified.
//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier. // %.*s
#define AfxPushString(str_) ((str_) ? AfxGetStringLength((str_)) : 0), ((str_) ? AfxGetStringData((str_), 0) : "")

AFXINL void             AfxResetString(afxString* str);

AFXINL void             AfxMakeString(afxString* str, afxNat cap, void const *start, afxNat len); // wraps constant (read-only) data as a Qwadro string.

AFXINL void             AfxClearString(afxString* str);

// Makes a excerpt string from another string.
AFXINL void             AfxReflectString(afxString const* str, afxString* reflection); // wraps the same content mapped or buffered by other string as read-only data.
AFXINL afxNat           AfxExcerptString(afxString const* src, afxNat base, afxNat len, afxString* selection); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.

AFXINL afxChar const*   AfxGetStringData(afxString const* str, afxNat base);
AFXINL afxChar*         AfxGetStringStorage(afxString const* str, afxNat base);

AFXINL afxNat           AfxGetStringLength(afxString const* str);
AFXINL afxNat           AfxGetStringCapacity(afxString const* str);

AFXINL afxBool          AfxStringIsEmpty(afxString const* str);
AFXINL afxBool          AfxStringIsWriteable(afxString const* str);

AFX afxResult           AfxDumpString(afxString const* str, afxNat base, afxNat range, void *dst); // copy out

AFX afxResult           AfxScanString(afxString const* str, afxChar const *fmt, ...);
AFX afxResult           AfxScanStringArg(afxString const* str, afxChar const *fmt, va_list args);

AFX afxNat              AfxFormatString(afxString* str, afxChar const *fmt, ...);
AFX afxNat              AfxFormatStringArg(afxString* str, afxChar const *fmt, va_list args);

AFX afxNat              AfxStringsAreEqual(afxString const* str, afxNat cnt, afxString const others[]);

AFX afxResult           AfxCompareString(afxString const* str, afxString const* other);
AFX afxResult           AfxCompareStringCi(afxString const* str, afxString const* other);
AFX afxResult           AfxCompareStringRange(afxString const* str, afxNat base, afxString const* other, afxNat len);
AFX afxResult           AfxCompareStringRangeCi(afxString const* str, afxNat base, afxString const* other, afxNat len);
AFX afxResult           AfxCompareStringL(afxString const* str, afxNat base, afxChar const *start, afxNat len);
AFX afxResult           AfxCompareStringCil(afxString const* str, afxNat base, afxChar const *start, afxNat len);

AFX afxNat              AfxCatenateString(afxString* str, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxCatenateStringL(afxString* str, afxChar const *start, afxNat len); // Return clamped off (non-copied) length if any.

AFX afxNat              AfxSpanString(afxString* str, afxString const* excerpt);
AFX afxNat              AfxSpanString2(afxString* str, afxString const* stopset);

AFX afxNat              AfxCopyString(afxString* str, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxCopyStringRange(afxString* str, afxString const* in, afxNat base, afxNat len); // Return clamped off (non-copied) length if any.

AFX afxError            AfxCloneString(afxString* str, afxString const* in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneStringRange(afxString* str, afxString const* in, afxNat base, afxNat len); // wraps the same content mapped or buffered by other string as writeable data.

AFX void                AfxDeallocateString(afxString* str);
AFX afxError            AfxAllocateString(afxString* str, afxNat cap, void const *start, afxNat len);
AFX afxError            AfxReallocateString(afxString* str, afxNat cap, void const *start, afxNat len);

AFX afxNat              _AfxWriteString(afxString* str, afxNat base, void const *src, afxNat len); // copy into

AFX afxNat              AfxInsertString(afxString* str, afxNat at, afxString const* include);
AFX afxNat              AfxEraseString(afxString* str, afxNat at, afxNat len);

// return the position of found char/substring, in else case, it should be AFX_INVALID_INDEX.
AFXINL afxNat           AfxFindFirstChar(afxString const* str, afxNat base, afxInt ch);
AFXINL afxNat           AfxFindLastChar(afxString const* str, afxNat base, afxInt ch);
AFXINL afxNat           AfxFindSubstring(afxString const* str, afxString const* excerpt);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* str, afxNat32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* str, afxReal *value);

AFX afxString const     AFX_STR_EMPTY;

#endif//AFX_STRING_H
