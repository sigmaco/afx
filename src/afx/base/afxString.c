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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "../src/afx/dev/afxDevCoreBase.h"

_AFX afxString strEmptyData = AFX_STRING("");
_AFX afxString const AFX_STR_EMPTY = AFX_STRING("");

_AFXINL afxChar const* AfxGetStringData(afxString const* str, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(!base || str->len >= base);
    return (str->len > base) ? &str->start[base] : NIL;
};

_AFXINL afxChar* AfxGetStringStorage(afxString const* str, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(!base || str->cap >= base);
    return (str->cap > base) ? (afxChar*)&str->start[base] : NIL;
};

_AFXINL afxNat AfxGetStringLength(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    return str->len;
}

_AFXINL afxNat AfxGetStringCapacity(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    return str->cap;
}

_AFXINL afxError AfxGetStringAsHex(afxString const* str, afxNat32 *value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(value);
    AfxScanString(str, "%x", value);
    return err;
}

_AFXINL afxError AfxGetStringAsReal(afxString const* str, afxReal *value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(value);
    void const *src = AfxGetStringData(str, 0);
    AfxAssert(src);
    *value = strtod(src, NIL);
    return err;
}

_AFXINL afxBool AfxStringIsEmpty(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    return (!str || (0 == str->len));
}

_AFXINL afxBool AfxStringIsWriteable(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    return (0 < str->cap);
}

_AFXINL void AfxClearString(afxString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    if (str->cap && str->buf)
        str->buf[0] = '\0';

    str->len = 0;
}

_AFXINL void AfxResetString(afxString* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->len = 0;
    str->cap = 0;
    str->start = NIL;
}

_AFXINL void AfxMakeString(afxString* str, afxNat cap, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    //AfxAssert(!len || (len && start));
    str->len = !len && start && !cap ? AfxStrlen(start) : len;
    str->cap = cap;
    str->start = start;
}

_AFXINL void AfxReflectString(afxString const* str, afxString* reflection)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(reflection);

    if (AfxExcerptString(str, 0, AfxGetStringLength(str), reflection))
        AfxThrowError();
}

_AFXINL afxNat AfxExcerptString(afxString const* src, afxNat base, afxNat len, afxString* selection)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(src);
    afxNat maxRange = AfxGetStringLength(src);
    //AfxAssertRange(maxRange, base, len);
    afxNat clampedRange = AfxMin(len, maxRange);

    AfxAssert(selection);
    selection->len = clampedRange;
    selection->cap = src->cap > base ? src->cap - base : 0;
    selection->start = AfxGetStringData(src, base);
    return len - clampedRange; // return clipped off amount
}

_AFXINL afxNat AfxFindFirstChar(afxString const* str, afxNat from, afxInt ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    afxNat posn = AFX_INVALID_INDEX;
    afxNat strLen = str->len;
    AfxAssertRange(strLen, from, 1);

    if (strLen > from)
    {
        afxChar const *src = AfxGetStringData(str, from);

        if (src)
        {
            afxChar* p = (afxChar*)memchr(src, ch, str->len - from);
            posn = p ? (p - (afxChar*)AfxGetStringData(str, 0)) : AFX_INVALID_INDEX;
        }
    }
    return posn;
}

_AFXINL afxNat AfxFindLastChar(afxString const* str, afxNat from, afxInt ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    afxNat posn = AFX_INVALID_INDEX;

    afxNat strLen = str->len;
    AfxAssertRange(strLen, from, 1);

    if (strLen > from)
    {
        afxChar const *src = AfxGetStringData(str, 0);
        
        if (src)
        {
            for (afxNat i = strLen - from; i-- > 0;)
            {
                if (ch == src[i])
                {
                    posn = i;
                    break;
                }
            }
        }
    }
    return posn;
}

_AFXINL afxNat AfxFindSubstring(afxString const* str, afxString const* excerpt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(excerpt);
    afxChar const* start = str->start ? AfxStrnstr(str->start, str->len, excerpt->start, excerpt->len) : NIL;
    return start ? (afxNat)(afxSize)start : AFX_INVALID_INDEX;
}

_AFXINL afxResult AfxCompareStringCil(afxString const* str, afxNat base, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(start);

    if (!len)
        len = AfxStrlen(start);

    if (str->len + len == 0) return 0;
    else if ((str->len != len) || ((!str->len && len) || (!len && str->len))) // if have different lenghts or any of them is blank.
        return str->len - len;

    afxString a, b;
    AfxExcerptString(str, base, len, &a);
    AfxMakeString(&b, 0, start, len);
    return AfxCompareStringCi(&a, &b);
}

_AFXINL afxResult AfxCompareStringL(afxString const* str, afxNat base, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(start);

    if (!len)
        len = AfxStrlen(start);

    if (str->len + len == 0) return 0;
    else if ((str->len != len) || ((!str->len && len) || (!len && str->len))) // if have different lenghts or any of them is blank.
        return str->len - len;
    
    afxString a, b;
    AfxExcerptString(str, base, len, &a);
    AfxMakeString(&b, 0, start, len);
    return AfxCompareString(&a, &b);
}

_AFXINL afxResult AfxCompareStringRangeCi(afxString const* str, afxNat base, afxString const* other, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    afxString a, b;
    AfxExcerptString(str, base, len, &a);
    AfxExcerptString(other, 0, len, &b);
    return AfxCompareStringCi(&a, &b);
}

_AFXINL afxResult _AfxCompareStringRange2(afxString const* str, afxNat lenA, afxString const* other, afxNat lenB)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    
    if (lenA + lenB == 0) return 0;
    else if ((lenA != lenB) || ((!lenA && lenB) || (!lenB && lenA))) // if have different lenghts or any of them is blank.
        return lenA - lenB;

    void const *a = AfxGetStringData(str, 0);
    void const *b = AfxGetStringData(other, 0);
    AfxAssert(a);
    AfxAssert(b);
    return strncmp(a, b, lenA);
}

_AFXINL afxResult AfxCompareStringRange(afxString const* str, afxNat base, afxString const* other, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    if (len > str->len - base)
        return -1;

    void const* a = AfxGetStringData(str, base);
    void const* b = AfxGetStringData(other, 0);
    return strncmp(a, b, len);
}

_AFXINL afxResult AfxCompareStringCi(afxString const* str, afxString const* other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(other);

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
    AfxAssert(str);
    AfxAssert(other);

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
    AfxAssert(str);
    AfxAssert(cnt);
    AfxAssert(others);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxString const* other = &others[i];
        AfxAssert(other);

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

_AFXINL afxResult AfxDumpString(afxString const* str, afxNat base, afxNat len, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssertRange(str->len, base, len);
    afxNat maxRange = AfxGetStringLength(str);
    AfxAssert(base + len <= maxRange);
    void const *src = AfxGetStringData(str, base);
    AfxAssert(src);
    AfxAssert(len);
    AfxAssert(dst);
    AfxCopy2(1, len > maxRange - base ? maxRange - base : len, src, dst);
    return 0;
}

_AFXINL afxResult AfxScanStringArg(afxString const* str, afxChar const *fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    void const *src = AfxGetStringData(str, 0);
    AfxAssert(src);
    AfxAssert(fmt);
    afxResult rslt = vsscanf(src, fmt, args);
    return rslt;
}

_AFXINL afxResult AfxScanString(afxString const* str, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    void const *src = AfxGetStringData(str, 0);
    AfxAssert(src);
    AfxAssert(fmt);
    va_list args;
    va_start(args, fmt);
    afxResult rslt = vsscanf(src, fmt, args);
    va_end(args);
    return rslt;
}

_AFXINL afxNat AfxFormatStringArg(afxString* str, afxChar const *fmt, va_list args)
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
    return (str->len = AfxVsnprintf(map, cap, fmt, args));
}

_AFXINL afxNat AfxFormatString(afxString* str, afxChar const *fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));

    va_list args;
    AfxAssert(fmt);
    va_start(args, fmt);
    afxNat rslt = AfxFormatStringArg(str, fmt, args);
    AfxAssert(rslt == str->len);
    va_end(args);
    return rslt;
}

_AFXINL afxNat AfxCatenateString(afxString* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssert(in);
    afxNat srcLen = AfxGetStringLength(in);
    afxNat effectiveRange = 0;

    if (srcLen)
    {
        afxNat base = AfxGetStringLength(str);
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
                afxString tmp;
                AfxMakeString(&tmp, leftCap, dstData, effectiveRange);
                AfxCopyString(&tmp, in);
                str->len += tmp.len;
            }
        }
    }
    return srcLen - effectiveRange;
}

_AFXINL afxNat AfxCatenateStringL(afxString* str, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));

    if (!len && start)
        len = AfxStrlen(start);

    afxString tmp;
    AfxMakeString(&tmp, 0, start, len);
    afxNat overflow = AfxCatenateString(str, &tmp);
    AfxAssert(!overflow);
    return overflow;
}

_AFXINL afxNat AfxCopyStringRange(afxString* str, afxString const* in, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    afxNat clippedRange = 0;

    if (!in || !len) AfxClearString(str);
    else
    {
        AfxAssert(in);
        afxNat maxRange = AfxGetStringLength(in);
        AfxAssertRange(maxRange, base, len);
        afxNat base2 = AfxMin(base, in->len - 1);
        afxNat len2 = AfxMin(len, in->len);

        afxNat cap = AfxGetStringCapacity(str);
        afxNat effectiveRange = AfxClamp(len2, maxRange - base2, len2);
        AfxAssert(effectiveRange == len2);
        effectiveRange = AfxClamp(effectiveRange, effectiveRange, cap);

        // TODO averiguar essa jabuticaba aqui

        afxChar *start = AfxGetStringStorage(str, 0);
        AfxAssert(start);

        if (start)
        {
            if (effectiveRange)
            {
                afxChar const *src = AfxGetStringData(in, base);
                AfxAssert(src); // if it has len it must have data.
                AfxAssert(start); // if it is writeable it must have data.
                AfxCopy2(1, effectiveRange, src, start);
            }

            if (cap > effectiveRange) // opportunistic attempt to use left space to zero-terminate string.
                start[effectiveRange] = '\0';
        }

        str->len = effectiveRange;
        clippedRange = len - effectiveRange;
        AfxAssertAbs(clippedRange);
    }
    return clippedRange;
}

_AFXINL afxNat AfxCopyString(afxString* str, afxString const* in)
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

        if (!len) clippedRange = AfxGetStringLength(in), AfxClearString(str);
        else if ((clippedRange = AfxCopyStringRange(str, in, 0, len)))
            AfxThrowError();
    }
    AfxAssertAbs(clippedRange);
    return clippedRange;
}

_AFXINL afxError AfxCloneStringRange(afxString* str, afxString const* in, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    if (!in || !len) AfxResetString(str);
    else
    {
        AfxAssert(in);
        AfxAssertRange(in->len, base, len);

        void const *data = AfxGetStringData(in, base);
        //AfxAssert(data); // não pode ter tamanyo e não ter dado.

        if (!data) AfxResetString(str);
        else if (AfxAllocateString(str, len, data, len))
            AfxThrowError();
    }
    return err;
}

_AFXINL afxError AfxCloneString(afxString* str, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    
    if (!in) AfxResetString(str);
    else
    {
        AfxAssert(in);
        afxNat size = AfxGetStringLength(in);

        if (!size) AfxResetString(str);
        else if (AfxCloneStringRange(str, in, 0, size))
            AfxThrowError();
    }
    return err;
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
                    AfxStrcpy(v6, v3);
                    return 1;
                }

                if (*v3)
                {
                    afxString t1, t2;
                    AfxMakeString(&t1, 0, v4, v10);
                    AfxMakeString(&t2, 0, v3, v10);

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
        AfxStrcpy(Out, v3);
        result = 1;
    }
    return result;
}

_AFXINL void AfxDeallocateString(afxString *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    
    if (str->cap && str->buf)
    {
        AfxDeallocate(str->buf);
        str->buf = NIL;
    }
    AfxResetString(str);
}

_AFXINL afxError AfxAllocateString(afxString* str, afxNat cap, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(!len || (len && start));

    if (!len && start)
        len = AfxStrlen(start);

    //AfxAssert(len);
    afxNat effectiveCap = cap ? cap : len;
    //AfxAssert(effectiveCap);

    if (!effectiveCap) AfxResetString(str);
    else
    {
        AfxAssert(len <= effectiveCap);
        afxNat effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
        //AfxAssert(effectiveRange);

        void *buf = NIL;

        if (effectiveCap && !(buf = AfxAllocate(1, effectiveCap, 0, AfxHere())))
            AfxThrowError();

        AfxMakeString(str, effectiveCap, buf, 0);
        //str->flags = _AFX_STR_FLAG_ALLOCED;

        if (effectiveRange)
        {
            afxString src;
            AfxMakeString(&src, 0, start, effectiveRange);
            AfxCopyString(str, &src);
        }
    }
    return err;
}

#if 0
_AFXINL afxError AfxReallocateString(afxString* str, afxNat cap, void const *start, afxNat len)
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

_AFXINL afxNat _AfxWriteString(afxString* str, afxNat base, void const *src, afxNat len)
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
    AfxAssertRange(cap, base, len);
    AfxAssert(cap >= len);
    afxNat effectiveCap = (cap - base);
    AfxAssert(len <= effectiveCap);
    afxNat effectiveRange = AfxMin(len, effectiveCap); // truncate to capacity when string has space.
    AfxAssert(effectiveRange);

    afxChar *start = AfxGetStringStorage(str, base);
    AfxAssert(start);
    AfxCopy2(effectiveRange, 1, src, start);

    if (base + effectiveRange > str->len)
        str->len = (base + effectiveRange) - str->len;


    if (effectiveCap > effectiveRange)
        start[effectiveRange] = '\0';

    return rslt;
}

_AFXINL afxNat AfxInsertString(afxString* str, afxNat at, afxString const* include)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(include);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertRange(str->cap, at, include->len);

    afxString area;
    AfxMakeString(&area, str->cap > at ? at - str->cap : 0, AfxGetStringData(str, at), str->len > at ? at - str->len : 0);

    afxString2048 remaining;
    AfxMakeString2048(&remaining, &area);

    afxNat ret = AfxFormatString(&area, "%.*s%.*s", AfxPushString(include), AfxPushString(&remaining.str));

    str->len = AfxStrlen(str->start);

    return ret;
}

_AFXINL afxNat AfxEraseString(afxString* str, afxNat at, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(AfxStringIsWriteable(str));
    AfxAssertRange(str->cap, at, len);

    afxString area;
    AfxMakeString(&area, str->cap > at ? at - str->cap : 0, AfxGetStringData(str, at), str->len > at ? at - str->len : 0);

    afxString2048 remaining;
    AfxMakeString2048(&remaining, &area);

    return AfxFormatString(&area, "%.*s%c", AfxPushString(&remaining.str), '\0');
}
