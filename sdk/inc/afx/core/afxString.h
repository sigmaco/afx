/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
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
    _AFX_DBG_FCC;
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
            afxString const*str; // excerpt
            afxString*      strRW;
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

#define AFX_STRING_LITERAL(text_) { .fcc = AFX_FCC_STR, .range = (afxNat16)((sizeof((text_)) / sizeof((text_)[0])) - sizeof(afxChar)), .flags = NIL, .src.cap = 0, .src.start = (afxChar const*)(text_) }
#define AfxSpawnString(text_) (afxString const[])AFX_STRING_LITERAL(text_)

// NOTE: When 'cap' is zero, a string can't be modified.

//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier.
// %.*s
#define AfxPushString(str_) (str_) ? AfxStringGetSize((str_)) : 1, (str_) ? AfxStringGetDataConst((str_), 0) : ""
#define AfxSpawnStaticString(text_) (afxString const[])AFX_STRING(text_);

AFX void                AfxString8(afxString8 *str);
AFX void                AfxString16(afxString16 *str);
AFX void                AfxString32(afxString32 *str);
AFX void                AfxString128(afxString128 *str);

AFX void                AfxStringDeallocate(afxString *str);
AFX afxString*          AfxStringAllocate(afxNat cap, void const *start, afxNat range);
AFX afxString*          AfxStringReallocate(afxString *str, afxNat cap, void const *start, afxNat range);

AFX void                AfxStringWrapLiteral(afxString *str, void const *start, afxNat range); // wraps constant (read-only) data as a Qwadro string.
AFX void                AfxStringWrapLiteralRW(afxString *str, void *start, afxNat range); // wraps dynamic (writeable) data as a Qwadro string.
AFX void                AfxStringWrapBuffer(afxString *str, void *start, afxNat cap);

AFX void                AfxStringExcerpt(afxString const *str, afxString *excerpt); // wraps the same content mapped or buffered by other string as read-only data.
AFX afxNat              AfxStringExcerptRange(afxString const *str, afxNat base, afxNat range, afxString *excerpt); // wraps the same content mapped or buffered by other string as read-only data. Return clamped off range if any.

AFX afxString*          AfxStringClone(afxString const *str); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxString*          AfxStringCloneRange(afxString const *str, afxNat base, afxNat range); // wraps the same content mapped or buffered by other string as writeable data.

AFX void*               AfxStringGetData(afxString *str, afxNat base);
AFX void const*         AfxStringGetDataConst(afxString const *str, afxNat base);
AFX afxNat              AfxStringGetSize(afxString const *str);
AFX afxNat              AfxStringGetBufferCap(afxString const *str);

AFX void                AfxStringReset(afxString *str);
AFX void                AfxStringClear(afxString *str);
AFX afxBool             AfxStringIsEmpty(afxString const *str);
AFX afxBool             AfxStringIsWriteable(afxString const *str);

AFX afxNat              AfxStringCopy(afxString *str, afxString const *in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxStringCopyRange(afxString *str, afxString const *in, afxNat base, afxNat range); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxStringAppend(afxString *str, afxString const *in); // Return clamped off (non-copied) length if any.
AFX afxNat              AfxStringAppendLiteral(afxString *str, afxChar const *start, afxNat range); // Return clamped off (non-copied) length if any.
    
AFX afxResult           _AfxStringWrite(afxString *str, afxNat base, void const *src, afxNat range); // copy into
AFX afxResult           AfxStringDump(afxString const *str, afxNat base, afxNat range, void *dst); // copy out

AFX afxResult           AfxStringFormat(afxString *str, afxChar const *fmt, ...);
AFX afxResult           AfxStringFormatArg(afxString *str, afxChar const *fmt, va_list args);

AFX afxResult           AfxStringScan(afxString const *str, afxChar const *fmt, ...);
AFX afxResult           AfxStringScanArg(afxString const *str, afxChar const *fmt, va_list args);

AFX afxResult           AfxStringDiffer(afxString const *str, afxString const *other);
AFX afxResult           AfxStringDifferCi(afxString const *str, afxString const *other);
AFX afxResult           AfxStringDifferRange(afxString const *str, afxString const *other, afxNat base, afxNat range);
AFX afxResult           AfxStringDifferRangeCi(afxString const *str, afxString const *other, afxNat base, afxNat range);
AFX afxResult           AfxStringDifferLiteral(afxString const *str, afxNat base, afxChar const *start, afxNat range);
AFX afxResult           AfxStringDifferLiteralCi(afxString const *str, afxNat base, afxChar const *start, afxNat range);

AFX afxChar*            AfxStringFindChar(afxString const *str, afxNat base, afxInt ch);
AFX afxChar*            AfxStringFindCharB2F(afxString const *str, afxNat base, afxInt ch);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxError            AfxStringGetAsHex(afxString const *str, afxNat32 *value);
AFX afxError            AfxStringGetAsReal(afxString const *str, afxReal *value);

AFX afxString const     AFX_STR_EMPTY;

//#define AfxAssertString(var_, code_) ((!!((var_) && ((var_)->h.fcc == AFX_FCC_STRC || (var_)->h.fcc == AFX_FCC_STRE || ((var_)->h.fcc >= AFX_FCC_STR0 && (var_)->h.fcc <= AFX_FCC_STR9))))||(AfxThrowError(rslt),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[(code_)]),0))
//#define AfxTryAssertString(var_, code_) ((!!(!(var_) || ((var_)->h.fcc == AFX_FCC_STRC || (var_)->h.fcc == AFX_FCC_STRE || ((var_)->h.fcc >= AFX_FCC_STR0 && (var_)->h.fcc <= AFX_FCC_STR9))))||(AfxThrowError(rslt),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[(code_)]),0))

#define AfxAssertString(str_) (err = _AfxStringAssert((str_), AfxSpawnHint(), AfxStr((str_))));
#define AfxTryAssertString(str_) ((!str_) || (err = _AfxStringAssert((str_), AfxSpawnHint(), AfxStr((str_)))));

AFXINL afxError _AfxStringAssert(afxString const *str_, afxHint const hint_, afxChar const *exp_)
{
    afxChar const *msg;

    if (!(str_)) msg = errorMsg[AFXERR_NIL];
    else if (!((str_)->fcc)) msg = errorMsg[AFXERR_UNINITIALIZED];
    else if (!((str_)->fcc == AFX_FCC_STR)) msg = errorMsg[AFXERR_INVALID];
    else msg = NIL;

    if (!msg) return 0;
    else
    {
        AfxOutputError((hint_), "%s\n    %s", (exp_), msg);
        return (-((afxError)hint_[1]));
    }
}

#endif//AFX_STRING_H