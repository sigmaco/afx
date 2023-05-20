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

// UTF-8 everywhere

#define AFX_STRING(text_) { AFX_FCC_STR, (sizeof((text_)) / sizeof((text_)[0])) - sizeof((text_)[0]), 0, ._dataConst = (void const*const)(text_) }

AFX_DEFINE_STRUCT(afxString)
{
    afxFcc          fcc;
    afxNat32        _range; // measured range in data bytemap.
    afxNat32        _cap;
    union
    {
        afxByte     *_bytemap;
        afxChar     *_charmap;
        void        *_data; // points to a abstract source.
        void const  *_dataConst;
    };
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

AFX_DEFINE_STRUCT(afxString64)
{
    afxString   str;
    afxChar     buf[64];
};

AFX_DEFINE_STRUCT(afxString128)
{
    afxString   str;
    afxChar     buf[128];
};

AFX_DEFINE_STRUCT(afxString256)
{
    afxString   str;
    afxChar     buf[256];
};

AFX_DEFINE_STRUCT(afxString512)
{
    afxString   str;
    afxChar     buf[512];
};

AFX_DEFINE_STRUCT(afxString1024)
{
    afxString   str;
    afxChar     buf[1024];
};

AFX_DEFINE_STRUCT(afxString2048)
{
    afxString   str;
    afxChar     buf[2048];
};

AFX_DEFINE_STRUCT(afxString4096)
{
    afxString   str;
    afxChar     buf[4096];
};

// NOTE: When 'cap' is zero, a string can't be modified.

//#define AfxAssertString(str_) AfxAssert3(((afxChar*)&((str_)->fcc))[0] == 's', ((afxChar*)&((str_)->fcc))[1] == 't', ((afxChar*)&((str_)->fcc))[2] == 'r')

// used for formatted string input with %.*s specifier.
// %.*s
#define AfxPushString(str_) (str_) ? AfxStringGetSize((str_)) : 1, (str_) ? AfxStringGetDataConst((str_)) : ""
#define AfxSpawnStaticString(text_) (afxString const[])AFX_STRING(text_);

AFX afxString*          AfxString8(afxString8 *str, afxString const *src);
AFX afxString*          AfxString16(afxString16 *str, afxString const *src);
AFX afxString*          AfxString32(afxString32 *str, afxString const *src);
AFX afxString*          AfxString64(afxString64 *str, afxString const *src);
AFX afxString*          AfxString128(afxString128 *str, afxString const *src);
AFX afxString*          AfxString256(afxString256 *str, afxString const *src);
AFX afxString*          AfxString512(afxString512 *str, afxString const *src);
AFX afxString*          AfxString1024(afxString1024 *str, afxString const *src);
AFX afxString*          AfxString2048(afxString2048 *str, afxString const *src);
AFX afxString*          AfxString4096(afxString4096 *str, afxString const *src);

AFX afxString*          AfxString8DeployRaw(afxString8 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString16DeployRaw(afxString16 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString32DeployRaw(afxString32 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString64DeployRaw(afxString64 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString128DeployRaw(afxString128 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString256DeployRaw(afxString256 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString512DeployRaw(afxString512 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString1024DeployRaw(afxString1024 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString2048DeployRaw(afxString2048 *str, afxChar const *src, afxNat range);
AFX afxString*          AfxString4096DeployRaw(afxString4096 *str, afxChar const *src, afxNat range);

AFX afxString const*    AfxStringMap(afxString *str, void const *start, afxNat range); // wraps constant (read-only) data as a Qwadro string.
AFX afxString*          AfxStringMapWriteable(afxString *str, void *start, afxNat range); // wraps dynamic (writeable) data as a Qwadro string.

AFX afxString*          AfxStringReflect(afxString *str, afxString const *in); // wraps the same content mapped or buffered by other string as read-only data.
AFX afxString*          AfxStringReflectWriteable(afxString *str, afxString const *in); // wraps the same content mapped or buffered by other string as writeable data.
AFX afxString*          AfxStringReflectRange(afxString *str, afxString const *in, afxNat base, afxNat range); // wraps the same content mapped or buffered by other string as read-only data.
AFX afxString*          AfxStringReflectRangeWriteable(afxString *str, afxString const *in, afxNat base, afxNat range); // wraps the same content mapped or buffered by other string as writeable data.

AFX afxChar*            AfxStringGetData(afxString *str);
AFX afxChar const*      AfxStringGetDataConst(afxString const *str);
AFX afxNat              AfxStringGetSize(afxString const *str);
AFX afxNat              AfxStringGetCap(afxString const *str);

AFX void                AfxStringReset(afxString *str);
AFX void                AfxStringClear(afxString *str);
AFX afxBool             AfxStringIsEmpty(afxString const *str);

AFX afxNat              AfxStringCopy(afxString *str, afxString const *in);
AFX afxNat              AfxStringCopyRange(afxString *str, afxString const *in, afxNat base, afxNat range);
AFX afxResult           AfxStringAppend(afxString *str, afxNat cnt, afxString const src[]);
AFX afxResult           AfxStringAppendRaw(afxString *str, afxChar const *start, afxNat range);
    
AFX afxResult           AfxStringUpdate(afxString *str, afxNat base, afxNat range, void const *src); // copy into
AFX afxResult           AfxStringDump(afxString const *str, afxNat base, afxNat range, void *dst); // copy out

AFX afxResult           AfxStringFormat(afxString *str, afxChar const *fmt, ...);
AFX afxResult           AfxStringFormatArg(afxString *str, afxChar const *fmt, va_list args);

AFX afxResult           AfxStringScan(afxString const *str, afxChar const *fmt, ...);
AFX afxResult           AfxStringScanArg(afxString const *str, afxChar const *fmt, va_list args);

AFX afxResult           AfxStringCompare(afxString const *str, afxString const *other);
AFX afxResult           AfxStringCompareCi(afxString const *str, afxString const *other);
AFX afxResult           AfxStringCompareRange(afxString const *str, afxString const *other, afxNat base, afxNat range);
AFX afxResult           AfxStringCompareRangeCi(afxString const *str, afxString const *other, afxNat base, afxNat range);
AFX afxResult           AfxStringCompareRaw(afxString const *str, afxNat base, afxNat range, afxChar const *start);
AFX afxResult           AfxStringCompareRawCi(afxString const *str, afxNat base, afxNat range, afxChar const *start);

AFX afxChar*            AfxStringFindChar(afxString const *str, afxNat base, afxInt ch);
AFX afxChar*            AfxStringFindCharB2F(afxString const *str, afxNat base, afxInt ch);

    // TODO GetAs(str, szScheme, pvData) -> GetAs(str, "%x", data)
    // TODO GetAsMeasured(str, szScheme, nLen, pvData) -> GetAs(str, "%.*s", data)
AFX afxResult           AfxStringGetAsHex(afxString const *str, afxNat32 *value);
AFX afxResult           AfxStringGetAsReal(afxString const *str, afxReal *value);

AFX afxString const*    AFX_STR_EMPTY;

//#define AfxAssertString(var_, code_) ((!!((var_) && ((var_)->h.fcc == AFX_FCC_STRC || (var_)->h.fcc == AFX_FCC_STRE || ((var_)->h.fcc >= AFX_FCC_STR0 && (var_)->h.fcc <= AFX_FCC_STR9))))||(AfxThrowError(rslt),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[(code_)]),0))
//#define AfxTryAssertString(var_, code_) ((!!(!(var_) || ((var_)->h.fcc == AFX_FCC_STRC || (var_)->h.fcc == AFX_FCC_STRE || ((var_)->h.fcc >= AFX_FCC_STR0 && (var_)->h.fcc <= AFX_FCC_STR9))))||(AfxThrowError(rslt),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[(code_)]),0))

#define AfxAssertString(str_) (err = _AfxStringAssert((str_), AfxSpawnHint(), AfxStr((str_))));
#define AfxTryAssertString(str_) ((!str_) || (err = _AfxStringAssert((str_), AfxSpawnHint(), AfxStr((str_)))));

AFXINL afxError _AfxStringAssert(afxString const *str_, afxHint const hint_, afxChar const *exp_)
{
    afxChar const *msg;

    if (!(str_)) msg = errorMsg[AFXERR_NIL];
    else if (!((str_)->fcc)) msg = errorMsg[AFXERR_UNINITIALIZED];
    else if (!((str_)->fcc == AFX_FCC_STR || (str_)->fcc == AFX_FCC_STRC || (str_)->fcc == AFX_FCC_STRE || ((str_)->fcc >= AFX_FCC_STR0 && (str_)->fcc <= AFX_FCC_STR9))) msg = errorMsg[AFXERR_INVALID];
    else msg = NIL;

    if (!msg) return 0;
    else
    {
        AfxOutputError((hint_), "%s\n    %s", (exp_), msg);
        return (-((afxError)hint_[1]));
    }
}

#endif//AFX_STRING_H