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
#include "../impl/afxExecImplKit.h"

_AFX afxString strEmptyData = AFX_STRING("");
_AFX afxString const AFX_STR_EMPTY = AFX_STRING("");

_AFXINL afxChar const* AfxGetStringData(afxString const* s, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(!base || s->len >= base);
    return (s->len > base) ? &s->start[base] : NIL;
};

_AFXINL afxChar* AfxGetStringStorage(afxString const* s, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(!base || s->cap >= base);
    return (s->cap > base) ? (afxChar*)&s->start[base] : NIL;
};

_AFXINL afxUnit AfxGetStringLength(afxString const* s)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    return s->len;
}

_AFXINL afxUnit AfxGetStringCapacity(afxString const* s, afxUnit from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    return from < s->cap ? s->cap - from : 0;
}

_AFXINL afxError AfxGetStringAsHex(afxString const* s, afxUnit32 *value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(value);
    AfxScanString(s, "%x", value);
    return err;
}

_AFXINL afxError AfxGetStringAsReal(afxString const* s, afxReal *value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(value);
    void const *src = AfxGetStringData(s, 0);
    AFX_ASSERT(src);
    *value = strtod(src, NIL);
    return err;
}

_AFXINL afxBool AfxIsStringEmpty(afxString const* s)
{
    afxError err = AFX_ERR_NONE;
    return (!s || (0 == s->len));
}

_AFXINL void AfxClearStrings(afxUnit cnt, afxString strings[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(strings);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];

        if (s->cap)
        {
            AFX_ASSERT(s->buf);
            s->buf[0] = '\0';
        }
        s->len = 0;
    }
}

_AFXINL void AfxResetStrings(afxUnit cnt, afxString strings[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(strings);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];
        s->len = 0;
        s->cap = 0;
        s->start = NIL;
    }
}

_AFXINL void AfxMakeString(afxString* s, afxUnit cap, void const *start, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    //AFX_ASSERT(!len || (len && start));
    s->len = !len && start && !cap ? AfxStrlen(start) : len;
    s->cap = cap;
    s->start = start;
}

_AFXINL void AfxReflectString(afxString const* s, afxString* reflection)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(reflection);

    if (AfxExcerptString(s, 0, AfxGetStringLength(s), reflection))
        AfxThrowError();
}

_AFXINL afxUnit AfxExcerptString(afxString const* src, afxUnit base, afxUnit len, afxString* selection)
{
    afxError err = AFX_ERR_NONE;

    AFX_ASSERT(src);
    afxUnit maxRange = AfxGetStringLength(src);
    //AFX_ASSERT_RANGE(maxRange, base, len);
    afxUnit clampedRange = AfxMin(len, maxRange);

    AFX_ASSERT(selection);
    selection->len = clampedRange;
    selection->cap = src->cap > base ? src->cap - base : 0;
    selection->start = AfxGetStringData(src, base);
    return len - clampedRange; // return clipped off amount
}

_AFX afxUnit AfxExcerptStringLine(afxString const* s, afxUnit base, afxString* excerpt)
{
    afxError err = NIL;
    AFX_ASSERT(excerpt);
    AFX_ASSERT(s);
    afxUnit llen = 0, posn = 0;

    if (AfxFindFirstChar(s, base, '\n', &posn))
        AfxExcerptString(s, base, (posn - base) + 1, excerpt), llen = excerpt->len;
    else
        AfxExcerptString(s, base, s->len - base, excerpt), llen = excerpt->len;

    return llen;
}

_AFXINL afxBool AfxFindFirstChar(afxString const* s, afxUnit from, afxInt ch, afxUnit* posn)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    afxBool rslt = FALSE;
    afxUnit strLen = s->len;

    if (strLen > from)
    {
        afxChar const* src = AfxGetStringData(s, from);

        if (src)
        {
            afxChar* p = (afxChar*)memchr(src, ch, s->len - from);
            rslt = !!p;

            if (rslt && posn)
            {
                *posn = (p - (afxChar*)AfxGetStringData(s, 0));
            }
        }
    }
    return rslt;
}

_AFXINL afxBool AfxFindLastChar(afxString const* s, afxUnit from, afxInt ch, afxUnit* posn)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    afxBool rslt = FALSE;
    afxUnit strLen = s->len;

    if (strLen > from)
    {
        afxChar const* src = AfxGetStringData(s, 0);
        
        if (src)
        {
            for (afxUnit i = strLen - from; i-- > 0;)
            {
                if (ch == src[i])
                {
                    rslt = TRUE;
                    
                    if (posn)
                        *posn = i;

                    break;
                }
            }
        }
    }
    return rslt;
}

_AFXINL afxUnit AfxFindSubstring(afxString const* s, afxString const* excerpt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(excerpt);
    afxChar const* start = s->start ? AfxStrnstr(s->start, s->len, excerpt->start, excerpt->len) : NIL;
    return start ? (afxUnit)(afxSize)start : AFX_INVALID_INDEX;
}

_AFXINL afxResult AfxCompareString(afxString const* s, afxUnit base, afxChar const* start, afxUnit len, afxBool ci)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(start);

    if (!len)
        len = AfxStrlen(start);

    if (s->len + len == 0) return 0;
    else if ((s->len != len) || ((!s->len && len) || (!len && s->len))) // if have different lenghts or any of them is blank.
        return s->len - len;

    afxString b;
    AfxMakeString(&b, 0, start, len);
    return (0 != AfxCompareStrings(s, base, ci, 1, &b));
}

_AFXINL afxResult _AfxCompareStringRange2(afxString const* s, afxUnit lenA, afxString const* other, afxUnit lenB)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    
    if (lenA + lenB == 0) return 0;
    else if ((lenA != lenB) || ((!lenA && lenB) || (!lenB && lenA))) // if have different lenghts or any of them is blank.
        return lenA - lenB;

    void const *a = AfxGetStringData(s, 0);
    void const *b = AfxGetStringData(other, 0);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return strncmp(a, b, lenA);
}

_AFXINL afxUnit AfxCompareSubstrings(afxString const* s, afxUnit base, afxUnit len, afxBool ci, afxUnit cnt, afxString const others[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(others);
    AFX_ASSERT(s);

    afxString ss;
    AfxExcerptString(s, base, s->len - base, &ss);
    afxUnit lenA = ss.len;

    void const *a = AfxGetStringData(&ss, 0); // base was already considered by excerption

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* other = &others[i];

        void const* b = AfxGetStringData(other, 0);

        if ((ci && (0 == _strnicmp(a, b, len))) || (0 == strncmp(a, b, len)))
        {
            return i;
        }
    }
    return AFX_INVALID_INDEX;
}

_AFXINL afxUnit AfxCompareStrings(afxString const* s, afxUnit base, afxBool ci, afxUnit cnt, afxString const others[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(others);
    AFX_ASSERT(s);

    afxString ss;
    AfxExcerptString(s, base, s->len - base, &ss);
    afxUnit lenA = ss.len;

    void const *a = AfxGetStringData(&ss, 0); // base was already considered by excerption

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* other = &others[i];
        afxUnit lenB = other->len;

        if (lenA + lenB == 0) return i;
        else if ((lenA != lenB) || ((!lenA && lenB) || (!lenB && lenA))) // if have different lenghts or any of them is blank.
            continue;

        void const *b = AfxGetStringData(other, 0);
        AFX_ASSERT(b);
        AFX_ASSERT(a);

        if ((ci && (0 == _strnicmp(a, b, lenA))) || (0 == strncmp(a, b, lenA)))
        {
            return i;
        }
    }
    return AFX_INVALID_INDEX;
}

_AFXINL afxResult AfxDumpString(afxString const* s, afxUnit base, afxUnit len, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT_RANGE(s->len, base, len);

    // sanitize arguments
    base = AfxMin(base, s->len - 1);
    len = AfxMin(len, s->len - base);

    if (!len)
        return 0;

    void const *src = AfxGetStringData(s, base);
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    AfxCopy(dst, src, len);
    return len;
}

_AFXINL afxResult AfxScanStringArg(afxString const* s, afxChar const* fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);

    if (!s->len)
        return 0;

    void const *src = AfxGetStringData(s, 0);
    AFX_ASSERT(src);
    AFX_ASSERT(fmt);
    afxResult rslt = vsscanf(src, fmt, args);
    return rslt;
}

_AFXINL afxResult AfxScanString(afxString const* s, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);

    if (!s->len)
        return 0;

    void const *src = AfxGetStringData(s, 0);
    AFX_ASSERT(src);

    va_list args;
    AFX_ASSERT(fmt);
    va_start(args, fmt);
    afxResult rslt = AfxScanStringArg(s, fmt, args);
    va_end(args);
    return rslt;
}

_AFXINL afxUnit AfxFormatStringArg(afxString* s, afxChar const* fmt, va_list args)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);

    afxUnit cap = AfxGetStringCapacity(s, 0);

    if (!cap)
        return 0;

    void *map = AfxGetStringStorage(s, 0);
    AFX_ASSERT(map);
    AFX_ASSERT(fmt);
    AFX_ASSERT(args);
    return (s->len = AfxVsnprintf(map, cap, fmt, args));
}

_AFXINL afxUnit AfxFormatString(afxString* s, afxChar const* fmt, ...)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);

    va_list args;
    AFX_ASSERT(fmt);
    va_start(args, fmt);
    afxUnit rslt = AfxFormatStringArg(s, fmt, args);
    AFX_ASSERT(rslt == s->len);
    va_end(args);
    return rslt;
}

_AFXINL afxUnit AfxCatenateStrings(afxString* s, afxUnit cnt, afxString const src[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    AFX_ASSERT(src);
    afxUnit excluded = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* in = &src[i];
        afxUnit srcLen = AfxGetStringLength(in);
        afxUnit effectiveRange = 0;

        if (!srcLen)
            continue;

        afxUnit base = AfxGetStringLength(s);
        afxUnit room = AfxGetStringCapacity(s, base);

        if (room)
        {
            void *dst = AfxGetStringStorage(s, base);
            AFX_ASSERT(dst);
            AFX_ASSERT(room >= srcLen);
            effectiveRange = AfxClamp(effectiveRange, srcLen, room); // truncate to capacity when string has space.
            AFX_ASSERT(effectiveRange == srcLen);

            afxString tmp;
            AfxMakeString(&tmp, room, dst, effectiveRange);
            AfxCopyString(&tmp, in);
            s->len += tmp.len;
        }
        excluded += (srcLen - effectiveRange);
    }
    return excluded;
}

_AFXINL afxUnit AfxCatenateString(afxString* s, afxChar const* start, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);

    if (!len && start)
        len = AfxStrlen(start);

    afxString tmp;
    AfxMakeString(&tmp, 0, start, len);
    return AfxCatenateStrings(s, 1, &tmp);
}

_AFXINL afxUnit AfxCopySubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    afxUnit clippedRange = 0;

    if (!in || !len) AfxClearStrings(1, s);
    else
    {
        AFX_ASSERT(in);
        afxUnit maxRange = AfxGetStringLength(in);
        AFX_ASSERT_RANGE(maxRange, base, len);
        afxUnit base2 = AfxMin(base, in->len - 1);
        afxUnit len2 = AfxMin(len, in->len);

        afxUnit cap = AfxGetStringCapacity(s, 0);
        afxUnit effectiveRange = AfxClamp(len2, maxRange - base2, len2);
        AFX_ASSERT(effectiveRange == len2);
        effectiveRange = AfxClamp(effectiveRange, effectiveRange, cap);

        // TODO averiguar essa jabuticaba aqui

        afxChar *start = AfxGetStringStorage(s, 0);
        AFX_ASSERT(start);

        if (start)
        {
            if (effectiveRange)
            {
                afxChar const* src = AfxGetStringData(in, base);
                AFX_ASSERT(src); // if it has len it must have data.
                AFX_ASSERT(start); // if it is writeable it must have data.
                AfxCopy(start, src, effectiveRange);
            }

            if (cap > effectiveRange) // opportunistic attempt to use left space to zero-terminate string.
                start[effectiveRange] = '\0';
        }

        s->len = effectiveRange;
        clippedRange = len - effectiveRange;
        AfxAssertAbs(clippedRange);
    }
    return clippedRange;
}

_AFXINL afxUnit AfxCopyString(afxString* s, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    afxUnit clippedRange = 0;

    if (!in)
        AfxClearStrings(1, s);
    else if ((clippedRange = AfxCopySubstring(s, in, 0, in->len)))
        AfxThrowError();

    return clippedRange;
}

_AFXINL afxError AfxCloneSubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);

    if (!in || !len) AfxResetStrings(1, s);
    else
    {
        AFX_ASSERT(in);
        AFX_ASSERT_RANGE(in->len, base, len);

        void const *data = AfxGetStringData(in, base);
        //AFX_ASSERT(data); // não pode ter tamanyo e não ter dado.

        if (!data) AfxResetStrings(1, s);
        else if (AfxAllocateString(s, len, data, len))
            AfxThrowError();
    }
    return err;
}

_AFXINL afxError AfxCloneString(afxString* s, afxString const* in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    
    if (!in) AfxResetStrings(1, s);
    else
    {
        AFX_ASSERT(in);
        afxUnit size = AfxGetStringLength(in);

        if (!size) AfxResetStrings(1, s);
        else if (AfxCloneSubstring(s, in, 0, size))
            AfxThrowError();
    }
    return err;
}

_AFXINL afxBool WildCardMatch(afxString const* s, const char *Wildcard, char *Out)
{
    // bool WildCardMatch(const char *Name, const char *Wildcard, char *Out)

    afxBool result;

    afxChar const* v3 = AfxGetStringData(s, 0);
    
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

                afxUnit v10 = i - v4;

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

                    while (!AfxIsAnValidIndex(AfxCompareStrings(&t1, 0, FALSE, 1, &t2)) || !WildCardMatch(&t2, v4, v6))
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

_AFXINL void AfxDeallocateString(afxString *s)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    
    if (s->cap && s->buf)
    {
        AfxDeallocate((void**)&s->buf, AfxHere());
        s->buf = NIL;
    }
    AfxResetStrings(1, s);
}

_AFXINL afxError AfxAllocateString(afxString* s, afxUnit cap, void const *start, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(!len || (len && start));

    if (!len && start)
        len = AfxStrlen(start);

    //AFX_ASSERT(len);
    afxUnit effectiveCap = cap ? cap : len;
    //AFX_ASSERT(effectiveCap);

    if (!effectiveCap) AfxResetStrings(1, s);
    else
    {
        AFX_ASSERT(len <= effectiveCap);
        afxUnit effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
        //AFX_ASSERT(effectiveRange);

        void *buf = NIL;

        if (effectiveCap && AfxAllocate(effectiveCap, 0, AfxHere(), (void**)&buf))
            AfxThrowError();

        AfxMakeString(s, effectiveCap, buf, 0);
        //s->flags = _AFX_STR_FLAG_ALLOCED;

        if (effectiveRange)
        {
            afxString src;
            AfxMakeString(&src, 0, start, effectiveRange);
            AfxCopyString(s, &src);
        }
    }
    return err;
}

#if 0
_AFXINL afxError AfxReallocateString(afxString* s, afxUnit cap, void const *start, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(!len || (len && start));
    void *buf = NIL;

    if (start && !len)
        len = AfxStrlen(start);

    {
        AFX_ASSERT(s);
        AFX_ASSERT(s->flags & _AFX_STR_FLAG_ALLOCED);
        AFX_ASSERT(s->flags & _AFX_STR_FLAG_BUFFERED);
        afxUnit effectiveCap = cap ? cap : len;
        AFX_ASSERT(effectiveCap);
        AFX_ASSERT(len <= effectiveCap);
        afxUnit effectiveRange = len > effectiveCap ? effectiveCap : len; // truncate to capacity when string has space.
        AFX_ASSERT(effectiveRange);

        if (!(buf = AfxReallocate(NIL, s->startRW, effectiveCap, 0, AfxHere()))) AfxThrowError();
        else
        {
            s->len = effectiveRange;
            s->cap = effectiveCap;
            s->startRW = buf;

            AfxCopy2(s->startRW, start, s->len);

            if (s->cap > s->len)
                s->startRW[s->len] = '\0';
        }
    }
    return err;
}
#endif

_AFXINL afxUnit _AfxWriteString(afxString* s, afxUnit base, void const *src, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    afxUnit rslt = 0;

    AFX_ASSERT(src);

    if (!len)
        len = AfxStrlen(src);

    AFX_ASSERT(len);
    afxUnit cap = AfxGetStringCapacity(s, 0);
    AFX_ASSERT_RANGE(cap, base, len);
    AFX_ASSERT(cap >= len);
    afxUnit effectiveCap = (cap - base);
    AFX_ASSERT(len <= effectiveCap);
    afxUnit effectiveRange = AfxMin(len, effectiveCap); // truncate to capacity when string has space.
    AFX_ASSERT(effectiveRange);

    afxChar *start = AfxGetStringStorage(s, base);
    AFX_ASSERT(start);
    AfxCopy(start, src, effectiveRange);

    if (base + effectiveRange > s->len)
        s->len = (base + effectiveRange) - s->len;


    if (effectiveCap > effectiveRange)
        start[effectiveRange] = '\0';

    return rslt;
}

_AFXINL afxUnit AfxInsertString(afxString* s, afxUnit at, afxString const* include)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(include);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    AFX_ASSERT_RANGE(s->cap, at, include->len);

    afxString area;
    AfxMakeString(&area, s->cap > at ? at - s->cap : 0, AfxGetStringData(s, at), s->len > at ? at - s->len : 0);

    afxString2048 remaining;
    AfxMakeString2048(&remaining, &area);

    afxUnit ret = AfxFormatString(&area, "%.*s%.*s", AfxPushString(include), AfxPushString(&remaining.str));

    s->len = AfxStrlen(s->start);

    return ret;
}

_AFXINL afxUnit AfxEraseString(afxString* s, afxUnit at, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    AFX_ASSERT_RANGE(s->cap, at, len);

    afxString area;
    AfxMakeString(&area, s->cap > at ? at - s->cap : 0, AfxGetStringData(s, at), s->len > at ? at - s->len : 0);

    afxString2048 remaining;
    AfxMakeString2048(&remaining, &area);

    return AfxFormatString(&area, "%.*s%c", AfxPushString(&remaining.str), '\0');
}

_AFXINL afxError AfxReadString(afxString* s, afxStream in, afxUnit len)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    AFX_ASSERT_RANGE(s->cap, 0, len);
    afxUnit errLen = 0;

    if ((errLen = AfxReadStream(in, len, 0, s->buf)))
        AfxThrowError();

    s->len = len - errLen;
    return errLen;
}
