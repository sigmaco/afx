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

#include "qwadro/core/afxBufferedString.h"
#include "qwadro/core/afxFixedString.h"
#include "qwadro/mem/afxMmu.h"

_AFXINL void* AfxGetBufferedStringData(afxBufferedString* str, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(str->cap >= base);
    afxChar* data;

    if ((data = str->str.buf))
        data += base;

    return data;
};

_AFXINL afxNat AfxGetStringCapacity(afxBufferedString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    return str->cap;
}

AFX afxBool AfxStringIsWriteable(afxBufferedString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    afxNat cap = AfxGetStringCapacity(str);
    return (0 < cap) && (str->cap);
}

_AFXINL void AfxDeallocateString(afxBufferedString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    //AfxAssert(str->flags & _AFX_STR_FLAG_ALLOCED);
    
    if (str->flags & _AFX_STR_FLAG_ALLOCED)
    {
        if (str->str.buf)
            AfxDeallocate(NIL, str->str.buf);

        str->str.buf = NIL;
        AfxResetBufferedString(str);
    }
}

_AFXINL afxError AfxAllocateString(afxBufferedString* str, afxNat cap, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(!len || (len && start));
    
    if (start && !len)
        len = AfxStrlen(start);

    //AfxAssert(len);
    afxNat effectiveCap = cap ? cap : len;
    //AfxAssert(effectiveCap);
    AfxAssert(len <= effectiveCap);
    afxNat effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
    //AfxAssert(effectiveRange);

    void *buf = NIL;
    
    if (effectiveCap && !(buf = AfxAllocate(NIL, 1, effectiveCap, 0, AfxHint())))
        AfxThrowError();

    AfxMakeBufferedString(str, effectiveCap, buf, 0);
    str->flags = _AFX_STR_FLAG_ALLOCED;

    if (effectiveRange)
    {
        afxString src;
        AfxMakeString(&src, start, effectiveRange);
        AfxCopyString(str, &src);
    }
    return err;
}

#if 0
_AFXINL afxError AfxReallocateString(afxBufferedString* str, afxNat cap, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(!len || (len && start));
    void *buf = NIL;

    if (start && !len)
        len = AfxStrlen(start);

    {
        AfxAssertType(str, afxFcc_STR);
        AfxAssert(str->flags & _AFX_STR_FLAG_ALLOCED);
        AfxAssert(str->flags & _AFX_STR_FLAG_BUFFERED);
        afxNat effectiveCap = cap ? cap : len;
        AfxAssert(effectiveCap);
        AfxAssert(len <= effectiveCap);
        afxNat effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
        AfxAssert(effectiveRange);

        if (!(buf = AfxReallocate(NIL, str->startRW, effectiveCap, 0, AfxHint()))) AfxThrowError();
        else
        {
            str->len = effectiveRange;
            str->cap = effectiveCap;
            str->startRW = buf;

            AfxCopy(str->startRW, start, str->len);

            if (str->cap > str->len)
                str->startRW[str->len] = '\0';
        }
    }
    return err;
}
#endif

_AFXINL afxNat _AfxWriteString(afxBufferedString* str, afxNat base, void const *src, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat rslt = 0;

    AfxAssert(src);

    if (!len)
        len = AfxStrlen(src);

    AfxAssert(len);
    afxNat cap = AfxGetStringCapacity(str);
    AfxAssert(cap >= len);
    afxNat effectiveCap = (cap - base);
    AfxAssert(len <= effectiveCap);
    afxNat effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
    AfxAssert(effectiveRange);

    afxChar *start = AfxGetBufferedStringData(str, base);
    AfxAssert(start);
    AfxCopy(1, effectiveRange, src, start);


    if (base + effectiveRange > str->str.len)
        str->str.len = base + effectiveRange;


    if (effectiveCap > effectiveRange)
        start[effectiveRange] = '\0';
    
    return rslt;
}

_AFXINL afxNat AfxFormatStringArg(afxBufferedString* str, afxChar const *fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat cap = AfxGetStringCapacity(str);
    AfxAssert(cap);
    AfxAssert(fmt);
    AfxAssert(args);
    void *map = AfxGetBufferedStringData(str, 0);
    AfxAssert(map);
    return (str->str.len = AfxVsnprintf(map, cap, fmt, args));
}

_AFXINL afxNat AfxFormatString(afxBufferedString* str, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));

    va_list args;
    AfxAssert(fmt);
    va_start(args, fmt);
    afxNat rslt = AfxFormatStringArg(str, fmt, args);
    AfxAssert(rslt == str->str.len);
    va_end(args);
    return rslt;
}

_AFXINL afxNat AfxInsertString(afxBufferedString* str, afxNat at, afxString const* include)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssertType(include, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertRange(str->cap, at, include->len);

    afxBufferedString area;
    AfxMakeBufferedString(&area, str->cap > at ? at - str->cap : 0, AfxGetBufferedStringData(str, at), str->str.len > at ? at - str->str.len : 0);
    
    afxFixedString2048 remaining;
    AfxMakeFixedString2048(&remaining, &area.str);

    afxNat ret = AfxFormatString(&area, "%.*s%.*s", AfxPushString(include), AfxPushString(&remaining.str.str));

    str->str.len = AfxStrlen(str->str.start);

    return ret;
}

_AFXINL afxNat AfxEraseString(afxBufferedString* str, afxNat at, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertRange(str->cap, at, len);

    afxBufferedString area;
    AfxMakeBufferedString(&area, str->cap > at ? at - str->cap: 0, AfxGetBufferedStringData(str, at), str->str.len > at ? at - str->str.len : 0);

    afxFixedString2048 remaining;
    AfxMakeFixedString2048(&remaining, &area.str);

    return AfxFormatString(&area, "%.*s%c", AfxPushString(&remaining.str.str), '\0');
}

_AFXINL afxNat AfxAppendStringLiteral(afxBufferedString* str, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));

    if (!len && start)
        len = AfxStrlen(start);

    afxString tmp;
    AfxMakeString(&tmp, start, len);
    afxNat overflow = AfxAppendString(str, &tmp);
    AfxAssert(!overflow);
    return overflow;
}

_AFXINL afxNat AfxAppendString(afxBufferedString* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertType(in, afxFcc_STR);
    afxNat srcLen = AfxGetStringLength(in);
    afxNat effectiveRange = 0;

    if (srcLen)
    {
        afxNat base = AfxGetStringLength(&str->str);
        afxNat cap = AfxGetStringCapacity(str);
        AfxAssert(cap); // writeable strings must have capacity
        afxNat leftCap = (cap - base);

        if (!leftCap) AfxThrowError();
        else
        {
            void *dstData = AfxGetBufferedStringData(str, base);
            AfxAssert(dstData);
            AfxAssert(leftCap >= srcLen);
            effectiveRange = srcLen > leftCap ? leftCap : srcLen; // truncate to capacity when string has space.
            AfxAssert(effectiveRange == srcLen);

            if (!dstData) AfxThrowError();
            else
            {
                afxBufferedString tmp;
                AfxMakeBufferedString(&tmp, effectiveRange, dstData, effectiveRange);
                AfxCopyString(&tmp, in);
                str->str.len += tmp.str.len;
            }
        }
    }
    return srcLen - effectiveRange;
}

_AFXINL afxNat AfxCopyStringRange(afxBufferedString* str, afxString const* in, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat clippedRange = 0;

    if (!in) AfxClearBufferedString(str);
    else
    {
        AfxAssertType(in, afxFcc_STR);

        afxNat cap = AfxGetStringCapacity(str);
        afxNat maxRange = AfxGetStringLength(in);
        AfxAssertRange(maxRange, 0, len - base);
        afxNat effectiveRange = len - base > maxRange ? maxRange : len;
        AfxAssert(effectiveRange == len);

        afxChar *start = AfxGetBufferedStringData(str, base);
        AfxAssert(start);

        if (start)
        {
            if (effectiveRange)
            {
                afxChar const *src = AfxGetStringData(in, 0);
                AfxAssert(src); // if it has len it must have data.
                AfxAssert(start); // if it is writeable it must have data.
                AfxCopy(1, effectiveRange, src, start);
            }

            if (cap > effectiveRange) // opportunistic attempt to use left space to zero-terminate string.
                start[effectiveRange] = '\0';
        }

        str->str.len = effectiveRange;
        clippedRange = len - effectiveRange;
        AfxAssertAbs(clippedRange);
    }
    return clippedRange;
}

_AFXINL afxNat AfxCopyString(afxBufferedString* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat clippedRange = 0;

    if (!in) AfxClearBufferedString(str);
    else
    {
        AfxAssertType(in, afxFcc_STR);
        afxNat len = AfxGetStringLength(in);

        if (!len) AfxResetBufferedString(str);
        else if ((clippedRange = AfxCopyStringRange(str, in, 0, len)))
            AfxThrowError();
    }
    AfxAssertAbs(clippedRange);
    return clippedRange;
}

_AFXINL void AfxClearBufferedString(afxBufferedString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);

    if (str->cap && str->str.buf)
        str->str.buf[0] = '\0';

    str->str.len = 0;
}

_AFXINL afxError AfxCloneStringRange(afxBufferedString* str, afxString const* in, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;

    if (!in || !len) AfxResetBufferedString(str);
    else
    {
        AfxAssertType(in, afxFcc_STR);
        AfxAssertRange(in->len, base, len);
        
        void const *data = AfxGetStringData(in, base);
        //AfxAssert(data); // não pode ter tamanyo e não ter dado.

        if (!data) AfxResetBufferedString(str);
        else if (AfxAllocateString(str, 0, data, len))
            AfxThrowError();
    }
    return err;
}

_AFXINL afxError AfxCloneString(afxBufferedString* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    
    if (!in) AfxResetBufferedString(str);
    else
    {
        AfxAssertType(in, afxFcc_STR);
        afxNat size = AfxGetStringLength(in);

        if (!size) AfxResetBufferedString(str);
        else if (AfxCloneStringRange(str, in, 0, size))
            AfxThrowError();
    }
    return err;
}

_AFXINL void AfxMakeBufferedString(afxBufferedString *str, afxNat cap, void *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(start);
    
    AfxMakeString(&str->str, start, len);
    str->cap = cap;
    str->flags = 0;

    AfxCatchError(err);
}

_AFXINL void AfxResetBufferedString(afxBufferedString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxResetString(&str->str);
    str->flags = NIL;
    str->cap = 0;
}
