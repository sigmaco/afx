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
    afxUnit              len;
    afxUnit              cap; // buffered
    union
    {
        afxChar const*  start; // wrapped or externally buffered.
        afxSize         offset;
        afxChar*        buf;
    };
};

#define AFX_STRING(text_) { .len = (afxUnit16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .start = (afxChar const*)(text_) }
#define AfxString(text_) (afxString const)AFX_STRING((text_))
#define AfxStaticString(text_) (afxString const)AFX_STRING((text_))

// NOTE: When 'cap' is zero, a string can't be modified.
//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier. // %.*s
#define AfxPushString(str_) ((str_) ? AfxGetStringLength((str_)) : 0), ((str_) ? AfxGetStringData((str_), 0) : "")

AFXINL void             AfxResetStrings(afxUnit cnt, afxString strings[]);

AFXINL void             AfxClearStrings(afxUnit cnt, afxString strings[]);

AFXINL void             AfxMakeString(afxString* s, afxUnit cap, void const *start, afxUnit len); // wraps constant (read-only) data as a Qwadro string.

// Makes a excerpt string from another string.
AFXINL void             AfxReflectString(afxString const* s, afxString* reflection); // wraps the same content mapped or buffered by other string as read-only data.
AFXINL afxUnit          AfxExcerptString(afxString const* src, afxUnit base, afxUnit len, afxString* selection); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.
AFX afxUnit             AfxExcerptStringLine(afxString const* s, afxUnit base, afxString* excerpt); // wraps the same content mapped or buffered by %s until a '\n' char has been reached.

AFXINL afxChar const*   AfxGetStringData(afxString const* s, afxUnit base);
AFXINL afxChar*         AfxGetStringStorage(afxString const* s, afxUnit base);

AFXINL afxUnit          AfxGetStringLength(afxString const* s);
AFXINL afxUnit          AfxGetStringCapacity(afxString const* s, afxUnit from);

AFXINL afxBool          AfxIsStringEmpty(afxString const* s);

AFX afxResult           AfxDumpString(afxString const* s, afxUnit base, afxUnit range, void *dst); // copy out

AFX afxResult           AfxScanString(afxString const* s, afxChar const* fmt, ...);
AFX afxResult           AfxScanStringArg(afxString const* s, afxChar const* fmt, va_list args);

AFX afxUnit             AfxFormatString(afxString* s, afxChar const* fmt, ...);
AFX afxUnit             AfxFormatStringArg(afxString* s, afxChar const* fmt, va_list args);

AFXINL afxResult        AfxCompareString(afxString const* s, afxUnit base, afxChar const* start, afxUnit len, afxBool ci);

// return the index of the first matched other string or AFX_INVALID_INDEX. Can be used in switch/case statements.
AFX afxUnit             AfxCompareStrings(afxString const* s, afxUnit base, afxBool ci, afxUnit cnt, afxString const others[]);
AFXINL afxUnit          AfxCompareSubstrings(afxString const* s, afxUnit base, afxUnit len, afxBool ci, afxUnit cnt, afxString const others[]);

AFX afxUnit             AfxCatenateStrings(afxString* s, afxUnit cnt, afxString const src[]); // Return clamped off (non-copied) length if any.
AFX afxUnit             AfxCatenateString(afxString* s, afxChar const* start, afxUnit len); // Return clamped off (non-copied) length if any.

AFX afxUnit             AfxSpanString(afxString* s, afxString const* excerpt);
AFX afxUnit             AfxSpanString2(afxString* s, afxString const* stopset);

AFX afxUnit             AfxCopyString(afxString* s, afxString const* in); // Return clamped off (non-copied) length if any.
AFX afxUnit             AfxCopySubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len); // Return clamped off (non-copied) length if any.

AFX afxError            AfxCloneString(afxString* s, afxString const* in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneSubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len); // wraps the same content mapped or buffered by other string as writeable data.

AFX void                AfxDeallocateString(afxString* s);
AFX afxError            AfxAllocateString(afxString* s, afxUnit cap, void const *start, afxUnit len);
AFX afxError            AfxReallocateString(afxString* s, afxUnit cap, void const *start, afxUnit len);

AFX afxUnit             _AfxWriteString(afxString* s, afxUnit base, void const *src, afxUnit len); // copy into

AFX afxUnit             AfxInsertString(afxString* s, afxUnit at, afxString const* include);
AFX afxUnit             AfxEraseString(afxString* s, afxUnit at, afxUnit len);

// return the position of found char/substring, in else case, it should be AFX_INVALID_INDEX.
AFXINL afxBool          AfxFindFirstChar(afxString const* s, afxUnit from, afxInt ch, afxUnit* posn);
AFXINL afxBool          AfxFindLastChar(afxString const* s, afxUnit from, afxInt ch, afxUnit* posn);
AFXINL afxUnit          AfxFindSubstring(afxString const* s, afxString const* excerpt);

    // TODO GetAs(s, szScheme, pvData) -> GetAs(s, "%x", data)
    // TODO GetAsMeasured(s, szScheme, nLen, pvData) -> GetAs(s, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* s, afxUnit32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* s, afxReal *value);

AFXINL afxError         AfxReadString(afxString* s, afxStream in, afxUnit len);

AFX afxString const     AFX_STR_EMPTY;

#endif//AFX_STRING_H
