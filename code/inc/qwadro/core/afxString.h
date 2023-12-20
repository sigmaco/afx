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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_STRING_H
#define AFX_STRING_H

#include "qwadro/core/afxRawString.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"

#define AFX_STR_MAX_LEN AFX_N16_MAX

// UTF-8 everywhere

enum
{
    _AFX_STR_FLAG_ALLOCED = AfxGetBitOffset(0)
};

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

AFX_DEFINE_STRUCT(afxBufferedString)
{
    afxString           str;
    afxNat16            cap; // buffered
    afxNat16            flags; // 0 = dynamic, 1 = remote source, 2 = string source
};

AFX_DEFINE_STRUCT(afxFixedString4)
{
    afxBufferedString   str;
    afxChar             buf[4];
};

AFX_DEFINE_STRUCT(afxFixedString8)
{
    afxBufferedString   str;
    afxChar             buf[8];
};

AFX_DEFINE_STRUCT(afxFixedString16)
{
    afxBufferedString   str;
    afxChar             buf[16];
};

AFX_DEFINE_STRUCT(afxFixedString32)
{
    afxBufferedString   str;
    afxChar             buf[32];
};

AFX_DEFINE_STRUCT(afxFixedString128)
{
    afxBufferedString   str;
    afxChar             buf[128];
};

AFX_DEFINE_STRUCT(afxFixedString2048)
{
    afxBufferedString   str;
    afxChar             buf[2048];
};

AFX_DEFINE_STRUCT(afxFixedString4096)
{
    afxBufferedString   str;
    afxChar             buf[4096];
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

// used for formatted string input with %.*s specifier.
// %.*s
#define AfxPushString(str_) (str_) ? AfxGetStringLength((str_)) : 1, (str_) ? AfxGetStringData((str_), 0) : ""

// FIXED STRINGS ///////////////////////////////////////////////////////////////

AFX afxBufferedString*  AfxMakeFixedString4(afxFixedString4 *str, afxString const* src);
AFX afxBufferedString*  AfxMakeFixedString8(afxFixedString8 *str, afxString const* src);
AFX afxBufferedString*  AfxMakeFixedString16(afxFixedString16 *str, afxString const* src);
AFX afxBufferedString*  AfxMakeFixedString32(afxFixedString32 *str, afxString const* src);
AFX afxBufferedString*  AfxMakeFixedString128(afxFixedString128 *str, afxString const* src);
AFX afxBufferedString*  AfxMakeFixedString2048(afxFixedString2048 *str, afxString const* src);
AFX afxBufferedString*  AfxMakeFixedString4096(afxFixedString4096 *str, afxString const* src);

// BUFFERED STRINGS ////////////////////////////////////////////////////////////

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

// STRING //////////////////////////////////////////////////////////////////////

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

AFX afxResult           AfxCompareString(afxString const* str, afxString const* other);
AFX afxResult           AfxCompareStringCi(afxString const* str, afxString const* other);
AFX afxResult           AfxCompareStringRange(afxString const* str, afxString const* other, afxNat base, afxNat len);
AFX afxResult           AfxCompareStringRangeCi(afxString const* str, afxString const* other, afxNat base, afxNat len);
AFX afxResult           AfxCompareStringLiteral(afxString const* str, afxNat base, afxChar const *start, afxNat len);
AFX afxResult           AfxCompareStringLiteralCi(afxString const* str, afxNat base, afxChar const *start, afxNat len);

AFX afxBool             AfxExtractExcerpt(afxString const* str, afxString const* substring, afxString* excerpt);

AFX afxChar*            AfxFindStringChar(afxString const* str, afxNat base, afxInt ch);
AFX afxChar*            AfxFindStringCharB2F(afxString const* str, afxNat base, afxInt ch);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const* str, afxNat32 *value);
AFX afxError            AfxGetStringAsReal(afxString const* str, afxReal *value);

AFX afxString const     AFX_STR_EMPTY;

#endif//AFX_STRING_H
