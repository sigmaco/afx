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

#ifndef AFX_STRING_H
#define AFX_STRING_H

#include "afx/core/afxRawString.h"
#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxFcc.h"

#define AFX_STR_MAX_LEN AFX_N16_MAX

// UTF-8 everywhere

#define AFX_STRING(text_) { AFX_FCC_STR, (sizeof((text_)) / sizeof((text_)[0])) - sizeof((text_)[0]), 0, ._dataConst = (void const*const)(text_) }

enum
{
    _AFX_STR_FLAG_ALLOCED = AFX_FLAG(0),
    _AFX_STR_FLAG_BUFFERED = AFX_FLAG(1),
    _AFX_STR_FLAG_EXCERPT = AFX_FLAG(2),
    _AFX_STR_FLAG_WRAP = AFX_FLAG(3),
};

AFX_DEFINE_STRUCT(afxString)
{
    _AFX_DBG_FCC
    afxNat16                range;
    afxNat16                flags; // 0 = dynamic, 1 = remote source, 2 = string source
    struct
    {
        union
        {
            afxNat16        cap; // buffered
            afxNat16        base; // excerpt
        };
        union
        {
            afxString const*parent; // excerpt
            afxString*      parentRW;
            afxChar const*  start; // wrapped or externally buffered.
            afxChar*        startRW;
            //afxSimd(afxByte)byteseq[]; // internally buffered.
        };
    }                       src;
};

AFX_DEFINE_STRUCT(afxString8)
{
    afxString   str;
    afxChar     buf[8];
};

AFX_DEFINE_STRUCT(afxString16)
{
    afxString   str;
    afxChar     buf[16];
};

AFX_DEFINE_STRUCT(afxString32)
{
    afxString   str;
    afxChar     buf[32];
};

AFX_DEFINE_STRUCT(afxString128)
{
    afxString   str;
    afxChar     buf[128];
};

#if _AFX_DEBUG
#define AFX_STRING_LITERAL(text_) { .fcc = AFX_FCC_STR, .range = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .flags = NIL, .src.cap = 0, .src.start = (afxChar const*)(text_) }
#else
#define AFX_STRING_LITERAL(text_) { .range = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .flags = NIL, .src.cap = 0, .src.start = (afxChar const*)(text_) }
#endif
#define AfxSpawnString(text_) (afxString const[])AFX_STRING_LITERAL(text_)

// NOTE: When 'cap' is zero, a string can't be modified.

//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier.
// %.*s
#define AfxPushString(str_) (str_) ? AfxGetStringSize((str_)) : 1, (str_) ? AfxGetStringDataConst((str_), 0) : ""
#define AfxSpawnStaticString(text_) (afxString const[])AFX_STRING(text_);

AFX void                AfxString8(afxString8 *str);
AFX void                AfxString16(afxString16 *str);
AFX void                AfxString32(afxString32 *str);
AFX void                AfxString128(afxString128 *str);

AFX void                AfxDeallocateString(afxString *str);
AFX afxError            AfxAllocateString(afxString* str, afxNat cap, void const *start, afxNat range);
AFX afxError            AfxReallocateString(afxString *str, afxNat cap, void const *start, afxNat range);

AFX void                AfxWrapStringLiteral(afxString *str, void const *start, afxNat range); // wraps constant (read-only) data as a Qwadro string.
AFX void                AfxWrapStringLiteralRW(afxString *str, void *start, afxNat range); // wraps dynamic (writeable) data as a Qwadro string.
AFX void                AfxWrapStringBuffer(afxString *str, void *start, afxNat cap);

// Makes a excerpt string from another string.
AFX void                AfxExcerptString(afxString *str, afxString const *parent); // wraps the same content mapped or buffered by other string as read-only data.
AFX afxNat              AfxExcerptStringRange(afxString *str, afxString const *parent, afxNat offset, afxNat range); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.

AFX afxError            AfxCloneString(afxString* str, afxString const *in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxError            AfxCloneStringRange(afxString* str, afxString const *in, afxNat base, afxNat range); // wraps the same content mapped or buffered by other string as writeable data.

AFX void*               AfxGetStringData(afxString *str, afxNat base);
AFX void const*         AfxGetStringDataConst(afxString const *str, afxNat base);
AFX afxNat              AfxGetStringSize(afxString const *str);
AFX afxNat              AfxGetStringBufferCap(afxString const *str);

AFX void                AfxResetString(afxString *str);
AFX void                AfxClearString(afxString *str);
AFX afxBool             AfxStringIsEmpty(afxString const *str);
AFX afxBool             AfxStringIsWriteable(afxString const *str);

AFX afxNat              AfxCopyString(afxString *str, afxString const *in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxCopyStringRange(afxString *str, afxString const *in, afxNat base, afxNat range); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxAppendString(afxString *str, afxString const *in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxAppendStringLiteral(afxString *str, afxChar const *start, afxNat range); // Return clamped off (non-copied) length if any.
    
AFX afxResult           _AfxWriteString(afxString *str, afxNat base, void const *src, afxNat range); // copy into
AFX afxResult           AfxDumpString(afxString const *str, afxNat base, afxNat range, void *dst); // copy out

AFX afxResult           AfxFormatString(afxString *str, afxChar const *fmt, ...);
AFX afxResult           AfxFormatStringArg(afxString *str, afxChar const *fmt, va_list args);

AFX afxResult           AfxScanString(afxString const *str, afxChar const *fmt, ...);
AFX afxResult           AfxScanStringArg(afxString const *str, afxChar const *fmt, va_list args);

AFX afxResult           AfxCompareString(afxString const *str, afxString const *other);
AFX afxResult           AfxCompareStringCi(afxString const *str, afxString const *other);
AFX afxResult           AfxCompareStringRange(afxString const *str, afxString const *other, afxNat base, afxNat range);
AFX afxResult           AfxCompareStringRangeCi(afxString const *str, afxString const *other, afxNat base, afxNat range);
AFX afxResult           AfxCompareStringLiteral(afxString const *str, afxNat base, afxChar const *start, afxNat range);
AFX afxResult           AfxCompareStringLiteralCi(afxString const *str, afxNat base, afxChar const *start, afxNat range);

AFX afxChar*            AfxFindStringChar(afxString const *str, afxNat base, afxInt ch);
AFX afxChar*            AfxFindStringCharB2F(afxString const *str, afxNat base, afxInt ch);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxError            AfxGetStringAsHex(afxString const *str, afxNat32 *value);
AFX afxError            AfxGetStringAsReal(afxString const *str, afxReal *value);

AFX afxString const     AFX_STR_EMPTY;

//#define AfxAssertString(var_, code_) ((!!((var_) && ((var_)->h.fcc == AFX_FCC_STRC || (var_)->h.fcc == AFX_FCC_STRE || ((var_)->h.fcc >= AFX_FCC_STR0 && (var_)->h.fcc <= AFX_FCC_STR9))))||(AfxThrowError(rslt),AfxLogError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[(code_)]),0))
//#define AfxTryAssertString(var_, code_) ((!!(!(var_) || ((var_)->h.fcc == AFX_FCC_STRC || (var_)->h.fcc == AFX_FCC_STRE || ((var_)->h.fcc >= AFX_FCC_STR0 && (var_)->h.fcc <= AFX_FCC_STR9))))||(AfxThrowError(rslt),AfxLogError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[(code_)]),0))

#define AfxAssertString(str_) (err = _AfxStringAssert((str_), AfxSpawnHint(), AfxStr((str_))));
#define AfxTryAssertString(str_) ((!str_) || (err = _AfxStringAssert((str_), AfxSpawnHint(), AfxStr((str_)))));

AFXINL afxError _AfxStringAssert(afxString const *str_, afxHint const hint_, afxChar const *exp_)
{
    afxChar const *msg;

    if (!(str_)) msg = errorMsg[AFXERR_NIL];
#if _AFX_DEBUG
    else if (!((str_)->fcc)) msg = errorMsg[AFXERR_UNINITIALIZED];
    else if (!((str_)->fcc == AFX_FCC_STR)) msg = errorMsg[AFXERR_INVALID];
#endif
    else msg = NIL;

    if (!msg) return 0;
    else
    {
        AfxLogError((hint_), "%s\n    %s", (exp_), msg);
        return (-((afxError)hint_[1]));
    }
}

#endif//AFX_STRING_H