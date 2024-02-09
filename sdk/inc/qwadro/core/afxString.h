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
    _AFX_DBG_FCC;
    afxNat              len;
    union
    {
        afxChar const*  start; // wrapped or externally buffered.
        afxChar*        buf;
    };
};

#if _AFX_DEBUG
#define AFX_STRING(text_) { .fcc = afxFcc_STR, .len = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .start = (afxChar const*)(text_) }
#else
#define AFX_STRING(text_) { .len = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .start = (afxChar const*)(text_) }
#endif
#define AfxString(text_) (afxString const)AFX_STRING((text_))
#define AfxStaticString(text_) (afxString const)AFX_STRING((text_))

// NOTE: When 'cap' is zero, a string can't be modified.
//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier. // %.*s
#define AfxPushString(str_) (str_) ? AfxGetStringLength((str_)) : 1, (str_) ? AfxGetStringData((str_), 0) : ""

AFX void                AfxResetString(afxString* str);

AFX void                AfxMakeString(afxString* str, void const *start, afxNat len); // wraps constant (read-only) data as a Qwadro string.

// Makes a excerpt string from another string.
AFX void                AfxReplicateString(afxString* str, afxString const* src); // wraps the same content mapped or buffered by other string as read-only data.
AFX afxNat              AfxExcerptString(afxString* str, afxString const* src, afxNat offset, afxNat len); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.

AFX void const*         AfxGetStringData(afxString const* str, afxNat base);
AFX afxNat              AfxGetStringLength(afxString const* str);

AFX afxBool             AfxStringIsEmpty(afxString const* str);

AFX afxResult           AfxDumpString(afxString const* str, afxNat base, afxNat range, void *dst); // copy out

AFX afxResult           AfxScanString(afxString const* str, afxChar const *fmt, ...);
AFX afxResult           AfxScanStringArg(afxString const* str, afxChar const *fmt, va_list args);

AFX afxResult           AfxTestStringEquality(afxString const* str, afxString const* other);
AFX afxNat              AfxStringsAreEqual(afxString const* str, afxNat cnt, afxString const others[]);
AFX afxResult           AfxTestStringEquivalence(afxString const* str, afxString const* other);
AFX afxResult           AfxTestStringRangeEquality(afxString const* str, afxString const* other, afxNat base, afxNat len);
AFX afxResult           AfxTestStringRangeEquivalence(afxString const* str, afxString const* other, afxNat base, afxNat len);
AFX afxResult           AfxTestStringEqualityLiteral(afxString const* str, afxNat base, afxChar const *start, afxNat len);
AFX afxResult           AfxTestStringEquivalenceLiteral(afxString const* str, afxNat base, afxChar const *start, afxNat len);

AFX afxBool             AfxExtractExcerpt(afxString const* str, afxString const* substring, afxString* excerpt);

AFX afxChar*            AfxFindStringChar(afxString const* str, afxNat base, afxInt ch);
AFX afxChar*            AfxFindStringCharB2F(afxString const* str, afxNat base, afxInt ch);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* str, afxNat32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* str, afxReal *value);

AFX afxError            AfxReadStrings(afxStream in, afxNat cnt, afxString dst[], afxNat dstStride);
AFX afxError            AfxWriteStrings(afxStream out, afxNat cnt, afxString const src[], afxNat srcStride);

AFX afxString const     AFX_STR_EMPTY;

#endif//AFX_STRING_H
