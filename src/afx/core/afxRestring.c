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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/core/afxSystem.h"

_AFXINL void* AfxGetStringStorage(afxRestring const* str, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(str->cap >= base);
    afxChar* data;

    if ((data = str->str.buf))
        data += base;

    AfxCatchError(!(str->cap >= base));
    return data;
};

_AFXINL afxNat AfxGetStringCapacity(afxRestring const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    return str->cap;
}

AFX afxBool AfxStringIsWriteable(afxRestring const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    afxNat cap = AfxGetStringCapacity(str);
    return (0 < cap) && (str->cap);
}

_AFXINL void AfxDeallocateString(afxRestring *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    //AfxAssert(str->flags & _AFX_STR_FLAG_ALLOCED);
    
    if (str->flags & _AFX_STR_FLAG_ALLOCED)
    {
        if (str->str.buf)
            AfxDeallocate(str->str.buf);

        str->str.buf = NIL;
        AfxResetRestring(str);
    }
}

_AFXINL afxError AfxAllocateString(afxRestring* str, afxNat cap, void const *start, afxNat len)
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
    
    if (effectiveCap && !(buf = AfxAllocate(1, effectiveCap, 0, AfxHere())))
        AfxThrowError();

    AfxMakeRestring(str, effectiveCap, buf, 0);
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
_AFXINL afxError AfxReallocateString(afxRestring* str, afxNat cap, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(!len || (len && start));
    void *buf = NIL;

    if (start && !len)
        len = AfxStrlen(start);

    {
        AfxAssert(str);
        AfxAssert(str->flags & _AFX_STR_FLAG_ALLOCED);
        AfxAssert(str->flags & _AFX_STR_FLAG_BUFFERED);
        afxNat effectiveCap = cap ? cap : len;
        AfxAssert(effectiveCap);
        AfxAssert(len <= effectiveCap);
        afxNat effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
        AfxAssert(effectiveRange);

        if (!(buf = AfxReallocate(NIL, str->startRW, effectiveCap, 0, AfxHere()))) AfxThrowError();
        else
        {
            str->len = effectiveRange;
            str->cap = effectiveCap;
            str->startRW = buf;

            AfxCopy2(str->startRW, start, str->len);

            if (str->cap > str->len)
                str->startRW[str->len] = '\0';
        }
    }
    return err;
}
#endif

_AFXINL afxNat _AfxWriteString(afxRestring* str, afxNat base, void const *src, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
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

    afxChar *start = AfxGetStringStorage(str, base);
    AfxAssert(start);
    AfxCopy2(1, effectiveRange, src, start);


    if (base + effectiveRange > str->str.len)
        str->str.len = base + effectiveRange;


    if (effectiveCap > effectiveRange)
        start[effectiveRange] = '\0';
    
    return rslt;
}

_AFXINL afxNat AfxFormatStringArg(afxRestring* str, afxChar const *fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat cap = AfxGetStringCapacity(str);
    AfxAssert(cap);
    AfxAssert(fmt);
    AfxAssert(args);
    void *map = AfxGetStringStorage(str, 0);
    AfxAssert(map);
    return (str->str.len = AfxVsnprintf(map, cap, fmt, args));
}

_AFXINL afxNat AfxFormatString(afxRestring* str, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));

    va_list args;
    AfxAssert(fmt);
    va_start(args, fmt);
    afxNat rslt = AfxFormatStringArg(str, fmt, args);
    AfxAssert(rslt == str->str.len);
    va_end(args);
    return rslt;
}

_AFXINL afxNat AfxInsertString(afxRestring* str, afxNat at, afxString const* include)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(include);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertRange(str->cap, at, include->len);

    afxRestring area;
    AfxMakeRestring(&area, str->cap > at ? at - str->cap : 0, AfxGetStringStorage(str, at), str->str.len > at ? at - str->str.len : 0);
    
    afxString2048 remaining;
    AfxMakeString2048(&remaining, &area.str);

    afxNat ret = AfxFormatString(&area, "%.*s%.*s", AfxPushString(include), AfxPushString(&remaining.str.str));

    str->str.len = AfxStrlen(str->str.start);

    return ret;
}

_AFXINL afxNat AfxEraseString(afxRestring* str, afxNat at, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertRange(str->cap, at, len);

    afxRestring area;
    AfxMakeRestring(&area, str->cap > at ? at - str->cap: 0, AfxGetStringStorage(str, at), str->str.len > at ? at - str->str.len : 0);

    afxString2048 remaining;
    AfxMakeString2048(&remaining, &area.str);

    return AfxFormatString(&area, "%.*s%c", AfxPushString(&remaining.str.str), '\0');
}

_AFXINL afxNat AfxConcatenateStringL(afxRestring* str, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));

    if (!len && start)
        len = AfxStrlen(start);

    afxString tmp;
    AfxMakeString(&tmp, start, len);
    afxNat overflow = AfxConcatenateString(str, &tmp);
    AfxAssert(!overflow);
    return overflow;
}

_AFXINL afxNat AfxConcatenateString(afxRestring* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssert(in);
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
            void *dstData = AfxGetStringStorage(str, base);
            AfxAssert(dstData);
            AfxAssert(leftCap >= srcLen);
            effectiveRange = srcLen > leftCap ? leftCap : srcLen; // truncate to capacity when string has space.
            AfxAssert(effectiveRange == srcLen);

            if (!dstData) AfxThrowError();
            else
            {
                afxRestring tmp;
                AfxMakeRestring(&tmp, leftCap, dstData, effectiveRange);
                AfxCopyString(&tmp, in);
                str->str.len += tmp.str.len;
            }
        }
    }
    return srcLen - effectiveRange;
}

_AFXINL afxNat AfxCopyStringRange(afxRestring* str, afxString const* in, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat clippedRange = 0;

    if (!in) AfxClearString(str);
    else
    {
        AfxAssert(in);

        afxNat cap = AfxGetStringCapacity(str);
        afxNat maxRange = AfxGetStringLength(in);
        AfxAssertRange(maxRange, 0, len - base);
        afxNat effectiveRange = len - base > maxRange ? maxRange : len;
        AfxAssert(effectiveRange == len);

        afxChar *start = AfxGetStringStorage(str, base);
        AfxAssert(start);

        if (start)
        {
            if (effectiveRange)
            {
                afxChar const *src = AfxGetStringData(in, 0);
                AfxAssert(src); // if it has len it must have data.
                AfxAssert(start); // if it is writeable it must have data.
                AfxCopy2(1, effectiveRange, src, start);
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

_AFXINL afxNat AfxCopyString(afxRestring* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat clippedRange = 0;

    if (!in) AfxClearString(str);
    else
    {
        AfxAssert(in);
        afxNat len = AfxMin(AfxGetStringLength(in), AfxGetStringCapacity(str));

        if (!len) AfxResetRestring(str);
        else if ((clippedRange = AfxCopyStringRange(str, in, 0, len)))
            AfxThrowError();
    }
    AfxAssertAbs(clippedRange);
    return clippedRange;
}

_AFXINL void AfxClearString(afxRestring *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    if (str->cap && str->str.buf)
        str->str.buf[0] = '\0';

    str->str.len = 0;
}

_AFXINL afxError AfxCloneStringRange(afxRestring* str, afxString const* in, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;

    if (!in || !len) AfxResetRestring(str);
    else
    {
        AfxAssert(in);
        AfxAssertRange(in->len, base, len);
        
        void const *data = AfxGetStringData(in, base);
        //AfxAssert(data); // não pode ter tamanyo e não ter dado.

        if (!data) AfxResetRestring(str);
        else if (AfxAllocateString(str, 0, data, len))
            AfxThrowError();
    }
    return err;
}

_AFXINL afxError AfxCloneString(afxRestring* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    
    if (!in) AfxResetRestring(str);
    else
    {
        AfxAssert(in);
        afxNat size = AfxGetStringLength(in);

        if (!size) AfxResetRestring(str);
        else if (AfxCloneStringRange(str, in, 0, size))
            AfxThrowError();
    }
    return err;
}

_AFXINL void AfxReflectRestring(afxRestring const* str, afxRestring* reflection)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(reflection);
    *reflection = *str;
}

_AFXINL afxNat AfxExcerptRestring(afxRestring const* str, afxNat base, afxNat len, afxRestring* excerpt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    afxNat maxRange = AfxGetStringLength(&str->str);
    AfxAssertRange(maxRange, base, len);
    afxNat clampedRange = len > maxRange ? maxRange : len;

    AfxAssert(excerpt);
    excerpt->str.len = clampedRange;
    excerpt->str.start = AfxGetStringData(&str->str, base);
    excerpt->cap = base < str->cap ? str->cap - base : 0;
    return len - clampedRange; // return clipped off amount
}

_AFXINL void AfxMakeRestring(afxRestring *str, afxNat cap, void *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(start);
    
    AfxMakeString(&str->str, start, len);
    str->cap = cap;
    str->flags = 0;

    AfxCatchError(err);
}

_AFXINL void AfxResetRestring(afxRestring *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxResetString(&str->str);
    str->flags = NIL;
    str->cap = 0;
}
