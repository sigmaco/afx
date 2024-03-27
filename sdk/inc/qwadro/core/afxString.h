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

#ifndef AFX_STRING_H
#define AFX_STRING_H

#include "qwadro/core/afxRawString.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"

#define AFX_STR_MAX_LEN AFX_N32_MAX

// UTF-8 everywhere

AFX_DEFINE_STRUCT(afxString)
{
    afxNat              len;
    union
    {
        afxSize         offset;
        afxChar const*  start; // wrapped or externally buffered.
        afxChar*        buf;
    };
};

#define AFX_STRING(text_) { .len = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .start = (afxChar const*)(text_) }
#define AfxString(text_) (afxString const)AFX_STRING((text_))
#define AfxStaticString(text_) (afxString const)AFX_STRING((text_))

// NOTE: When 'cap' is zero, a string can't be modified.
//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier. // %.*s
#define AfxPushString(str_) (str_) ? AfxGetStringLength((str_)) : 1, (str_) ? AfxGetStringData((str_), 0) : ""

AFX void                AfxResetString(afxString* str);

AFX void                AfxMakeString(afxString* str, void const *start, afxNat len); // wraps constant (read-only) data as a Qwadro string.

// Makes a excerpt string from another string.
AFX void                AfxReflectString(afxString const* str, afxString* reflection); // wraps the same content mapped or buffered by other string as read-only data.
AFX afxNat              AfxExcerptString(afxString const* src, afxNat offset, afxNat len, afxString* selection); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.

AFX afxChar const*      AfxGetStringData(afxString const* str, afxNat base);
AFX afxNat              AfxGetStringLength(afxString const* str);

AFX afxBool             AfxStringIsEmpty(afxString const* str);

AFX afxResult           AfxDumpString(afxString const* str, afxNat base, afxNat range, void *dst); // copy out

AFX afxResult           AfxScanString(afxString const* str, afxChar const *fmt, ...);
AFX afxResult           AfxScanStringArg(afxString const* str, afxChar const *fmt, va_list args);

AFX afxResult           AfxCompareString(afxString const* str, afxString const* other);
AFX afxNat              AfxStringsAreEqual(afxString const* str, afxNat cnt, afxString const others[]);
AFX afxResult           AfxCompareStringCi(afxString const* str, afxString const* other);
AFX afxResult           AfxCompareStringRange(afxString const* str, afxString const* other, afxNat base, afxNat len);
AFX afxResult           AfxCompareStringRangeCi(afxString const* str, afxString const* other, afxNat base, afxNat len);
AFX afxResult           AfxCompareStringL(afxString const* str, afxNat base, afxChar const *start, afxNat len);
AFX afxResult           AfxCompareStringCil(afxString const* str, afxNat base, afxChar const *start, afxNat len);

// return the position of found char/substring, in else case, it should be AFX_INVALID_INDEX.
AFXINL afxNat           AfxFindFirstChar(afxString const* str, afxNat from, afxInt ch);
AFXINL afxNat           AfxFindLastChar(afxString const* str, afxNat from, afxInt ch);
AFXINL afxNat           AfxFindSubstring(afxString const* str, afxString const* excerpt);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* str, afxNat32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* str, afxReal *value);

AFX afxString const     AFX_STR_EMPTY;

#endif//AFX_STRING_H
