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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "qwadro/core/afxString.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"

_AFX afxString strEmptyData = AFX_STRING("");
_AFX afxString const AFX_STR_EMPTY = AFX_STRING("");

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

_AFXINL void const* AfxGetStringData(afxString const* str, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(str->len >= base);
    afxChar const* data;

    if ((data = str->start))
        data += base;

    return data;
};

_AFXINL afxNat AfxGetStringLength(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    return str->len;
}

_AFXINL afxNat AfxGetStringCapacity(afxBufferedString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    return str->cap;
}

_AFXINL afxError AfxGetStringAsHex(afxString const* str, afxNat32 *value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(value);
    AfxScanString(str, "%x", value);
    return err;
}

_AFXINL afxError AfxGetStringAsReal(afxString const* str, afxReal *value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(value);
    void const *src = AfxGetStringData(str, 0);
    AfxAssert(src);
    *value = strtod(src, NIL);
    return err;
}

_AFXINL afxBool AfxStringIsEmpty(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    return (0 == str->len);
}

AFX afxBool AfxStringIsWriteable(afxBufferedString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    afxNat cap = AfxGetStringCapacity(str);
    return (0 < cap) && (str->cap);
}

_AFXINL afxChar* AfxFindStringCharB2F(afxString const* str, afxNat base, afxInt ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    void const *src = AfxGetStringData(str, base);
    AfxAssert(src);
    AfxAssert(0 < str->len - base);
    return (0 < str->len - base) && src ? strrchr(src, ch) : NIL; // can fault if str isn't null terminated
}

_AFXINL afxChar* AfxFindStringChar(afxString const* str, afxNat base, afxInt ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    void const *src = AfxGetStringData(str, base);
    AfxAssert(src);
    AfxAssert(0 < str->len - base);
    return (0 < str->len - base) && src ? memchr(src, ch, str->len - base) : NIL;
}

_AFX afxBool AfxExtractExcerpt(afxString const* str, afxString const* substring, afxString* excerpt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssertType(substring, afxFcc_STR);

    afxFixedString2048 tmp;
    AfxMakeFixedString2048(&tmp, substring);
    AfxMakeString(excerpt, strstr(str->start, tmp.buf), substring->len);
    return !!excerpt->len;
}

_AFXINL afxResult AfxCompareStringLiteralCi(afxString const* str, afxNat base, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(start);

    if (!len)
        len = AfxMeasureRawString(start);

    if (str->len + len == 0) return 0;
    else if ((str->len != len) || ((!str->len && len) || (!len && str->len))) // if have different lenghts or any of them is blank.
        return str->len - len;

    afxString a, b;
    AfxExcerptString(&a, str, base, len);
    AfxMakeString(&b, start, len);
    return AfxCompareStringCi(&a, &b);
}

_AFXINL afxResult AfxCompareStringLiteral(afxString const* str, afxNat base, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(start);

    if (!len)
        len = AfxMeasureRawString(start);

    if (str->len + len == 0) return 0;
    else if ((str->len != len) || ((!str->len && len) || (!len && str->len))) // if have different lenghts or any of them is blank.
        return str->len - len;

    afxString a, b;
    AfxExcerptString(&a, str, base, len);
    AfxMakeString(&b, start, len);
    return AfxCompareString(&a, &b);
}

_AFXINL afxResult AfxCompareStringRangeCi(afxString const* str, afxString const* other, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);

    afxString a, b;
    AfxMakeString(&a, AfxGetStringData(str, base), len);
    AfxMakeString(&b, AfxGetStringData(other, base), len);
    return AfxCompareStringCi(&a, &b);
}

_AFXINL afxResult AfxCompareStringRange(afxString const* str, afxString const* other, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);

    afxString a, b;
    AfxMakeString(&a, AfxGetStringData(str, base), len);
    AfxMakeString(&b, AfxGetStringData(other, base), len);
    return AfxCompareString(&a, &b);
}

_AFXINL afxResult AfxCompareStringCi(afxString const* str, afxString const* other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssertType(other, afxFcc_STR);

    afxNat lenA = str->len, lenB = other->len;

    if (lenA + lenB == 0) return 0;
    else if ((lenA != lenB) || ((!lenA && lenB) || (!lenB && lenA))) // if have different lenghts or any of them is blank.
        return lenA - lenB;

    void const *a = AfxGetStringData(str, 0);
    void const *b = AfxGetStringData(other, 0);
    AfxAssert(a);
    AfxAssert(b);
    return _strnicmp(a, b, lenA);
}

_AFXINL afxResult AfxCompareString(afxString const* str, afxString const* other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssertType(other, afxFcc_STR);

    afxNat lenA = str->len, lenB = other->len;

    if (lenA + lenB == 0) return 0;
    else if ((lenA != lenB) || ((!lenA && lenB) || (!lenB && lenA))) // if have different lenghts or any of them is blank.
        return lenA - lenB;

    void const *a = AfxGetStringData(str, 0);
    void const *b = AfxGetStringData(other, 0);
    AfxAssert(a);
    AfxAssert(b);
    return strncmp(a, b, lenA);
}

_AFXINL afxNat AfxStringsAreEqual(afxString const* str, afxNat cnt, afxString const others[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(cnt);
    AfxAssert(others);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxString const* other = &others[i];
        AfxAssertType(other, afxFcc_STR);

        afxNat lenA = str->len, lenB = other->len;

        if (lenA + lenB == 0) return 0;
        else if ((lenA != lenB) || ((!lenA && lenB) || (!lenB && lenA))) // if have different lenghts or any of them is blank.
            return lenA - lenB;

        void const *a = AfxGetStringData(str, 0);
        void const *b = AfxGetStringData(other, 0);
        AfxAssert(a);
        AfxAssert(b);
        
        if (0 == strncmp(a, b, lenA))
            return i;
    }
    return AFX_INVALID_INDEX;
}

_AFXINL afxResult AfxScanStringArg(afxString const* str, afxChar const *fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    void const *src = AfxGetStringData(str, 0);
    AfxAssert(src);
    AfxAssert(fmt);
    afxResult rslt = vsscanf(src, fmt, args);
    return rslt;
}

_AFXINL afxResult AfxScanString(afxString const* str, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    void const *src = AfxGetStringData(str, 0);
    AfxAssert(src);
    AfxAssert(fmt);
    va_list args;
    va_start(args, fmt);
    afxResult rslt = vsscanf(src, fmt, args);
    va_end(args);
    return rslt;
}

_AFXINL afxResult AfxDumpString(afxString const* str, afxNat base, afxNat len, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssertRange(str->len, base, len);
    afxNat maxRange = AfxGetStringLength(str);
    AfxAssert(base + len <= maxRange);
    void const *src = AfxGetStringData(str, base);
    AfxAssert(src);
    AfxAssert(len);
    AfxAssert(dst);
    AfxCopy(1, len > maxRange - base ? maxRange - base : len, src, dst);
    return 0;
}

_AFXINL afxBool WildCardMatch(afxString const* str, const char *Wildcard, char *Out)
{
    // bool WildCardMatch(const char *Name, const char *Wildcard, char *Out)

    afxBool result;

    afxChar const* v3 = AfxGetStringData(str, 0);
    
    if (Wildcard)
    {
        afxChar const* i;
        afxChar* v6 = Out;

        while (1)
        {
            afxChar const* v4 = Wildcard;
            afxChar v7;

            while (1)
            {
            LABEL_4:
                while (1)
                {
                    v7 = *v4;

                    if (*v4 != 63)
                        break;

                    if (*v3)
                        *v6++ = *v3++;

                    ++v4;
                }

                if (v7 != 42)
                    break;

                afxChar v8 = (v4++)[1];

                for (i = v4; v8; v8 = (i++)[1])
                {
                    if (v8 == 59)
                        break;

                    if (v8 == 63)
                        break;

                    if (v8 == 42)
                        break;
                }

                afxNat v10 = i - v4;

                if (i == v4)
                {
                    AfxCopyRawString(v6, v3);
                    return 1;
                }

                if (*v3)
                {
                    afxString t1, t2;
                    AfxMakeString(&t1, v4, v10);
                    AfxMakeString(&t2, v3, v10);

                    while (AfxCompareString(&t1, &t2) || !WildCardMatch(&t2, v4, v6))
                    {
                        *v6 = *v3;
                        afxChar v11 = v3[1];
                        ++v6;
                        ++v3;
                        if (!v11)
                            goto LABEL_4;
                    }
                    return 1;
                }
            }
            
            if (!v7)
                break;

            afxChar v12 = tolower(v7);
            
            if (v12 != tolower(*v3))
                return 0;

            ++v3;
            ++v4;
        }

        *v6 = 0;
        result = *v3 == 0;
    }
    else
    {
        AfxCopyRawString(Out, v3);
        result = 1;
    }
    return result;
}

// READ/WRITE METHODS /////////////////////////////////////////////////////////

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
        len = AfxMeasureRawString(start);

    //AfxAssert(len);
    afxNat effectiveCap = cap ? cap : len;
    //AfxAssert(effectiveCap);
    AfxAssert(len <= effectiveCap);
    afxNat effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
    //AfxAssert(effectiveRange);

    void *buf = NIL;
    
    if (effectiveCap && !(buf = AfxAllocate(NIL, effectiveCap, 1, 0, AfxHint())))
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
        len = AfxMeasureRawString(start);

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
        len = AfxMeasureRawString(src);

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
    return (str->str.len = vsnprintf(map, cap, fmt, args));
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

    str->str.len = AfxMeasureRawString(str->str.start);

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
        len = AfxMeasureRawString(start);

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

_AFXINL afxNat AfxExcerptString(afxString* str, afxString const* src, afxNat offset, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    
    AfxAssertType(src, afxFcc_STR);
    afxNat maxRange = AfxGetStringLength(src);
    AfxAssertRange(maxRange, offset, len);
    afxNat clampedRange = len > maxRange ? maxRange : len;
    
    AfxAssert(str);
    AfxAssignFcc(str, afxFcc_STR);
    str->len = clampedRange;
    str->start = AfxGetStringData(src, offset);
    return len - clampedRange; // return clipped off amount
}

_AFXINL void AfxReplicateString(afxString* str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssertType(src, afxFcc_STR);
    
    if (AfxExcerptString(str, src, 0, AfxGetStringLength(src)))
        AfxThrowError();
}

_AFXINL void AfxMakeString(afxString* str, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(!len || (len && start));

    AfxAssignFcc(str, afxFcc_STR);
    str->len = len && start ? len : (start ? AfxMeasureRawString(start) : 0);
    str->start = start;
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

_AFXINL void AfxResetString(afxString* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssignFcc(str, afxFcc_STR);
    str->len = 0;
    str->start = NIL;
}

_AFXINL void AfxResetBufferedString(afxBufferedString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxResetString(&str->str);
    str->flags = NIL;
    str->cap = 0;
}

#if 0
_AFXINL afxString* AfxString128DeployRaw(afxFixedString128 *str, afxChar const *src, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(!len || (len && src));
    AfxMakeBufferedString(&str->str, str->buf, sizeof(str->buf));

    if (src && src[0])
    {
        if (!len)
            len = AfxMeasureRawString(src);

        if (len > str->str._cap)
            len = str->str._cap;

        AfxCopy(&str->str._data, src, len);
        str->str._bytemap[len] = '\0';
    }
    return &str->str;
}

_AFXINL afxString* AfxMakeFixedString128(afxFixedString128 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxMakeBufferedString(&str->str, str->buf, sizeof(str->buf));

    if (src && !AfxStringIsEmpty(src))
        AfxCopyString(&str->str, src);

    return &str->str;
}
#endif

_AFX afxBufferedString* AfxMakeFixedString4(afxFixedString4 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxBufferedString* AfxMakeFixedString8(afxFixedString8 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxBufferedString* AfxMakeFixedString16(afxFixedString16 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxBufferedString* AfxMakeFixedString32(afxFixedString32 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxBufferedString* AfxMakeFixedString128(afxFixedString128 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxBufferedString* AfxMakeFixedString2048(afxFixedString2048 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxBufferedString* AfxMakeFixedString4096(afxFixedString4096 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeBufferedString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}
