/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
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
#include "../exec/afxSystemDDK.h"

#define _AFX_STRING_RESILIENCE_ENABLED TRUE

_AFX afxString strEmptyData = AFX_STRING("");
_AFX afxString const AFX_STRING_EMPTY = AFX_STRING("");

_AFXINL afxChar const* AfxGetStringData(afxString const* s, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(!base || s->len >= base);
    return (s->len > base) ? &s->start[base] : NIL;
};

_AFXINL afxChar* AfxGetStringStorage(afxString const* s, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(!base || s->cap >= base);
    return (s->cap > base) ? (afxChar*)&s->start[base] : NIL;
};

_AFXINL afxUnit AfxGetStringLength(afxString const* s)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    return s->len;
}

_AFXINL afxUnit AfxGetStringCapacity(afxString const* s, afxUnit from)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    return from < s->cap ? s->cap - from : 0;
}

_AFXINL afxError AfxGetStringAsHex(afxString const* s, afxUnit32 *value)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(value);
    AfxScanString(s, "%x", value);
    return err;
}

_AFXINL afxError AfxGetStringAsReal(afxString const* s, afxReal *value)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(value);
    void const *src = AfxGetStringData(s, 0);
    AFX_ASSERT(src);
    *value = strtod(src, NIL);
    return err;
}

_AFXINL afxBool AfxIsStringEmpty(afxString const* s)
{
    afxError err = { 0 };
    return (!s || (0 == s->len));
}

_AFXINL afxString* AfxMakeString(afxString* s, afxUnit cap, void const *start, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    //AFX_ASSERT(!len || (len && start));
    s->len = !len && start && !cap ? AfxStrlen(start) : len;
    s->cap = cap;
    s->start = start;
    return s;
}

_AFXINL afxString AfxExcerptString(afxString const* src, afxUnit from, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    afxString sub = { 0 };

    if (!src || !src->start || from >= src->len)
        return sub;

    sub.len = AFX_MIN(len, src->len - from);
    sub.cap = src->cap > from ? src->cap - from : 0;
    sub.start = src->start + from;
    return sub;
}

_AFXINL afxString AfxExcerptStringLine(afxString const* s, afxUnit from)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(s->start);
    AFX_ASSERT(from <= s->len);

    afxString line = { 0 };

    if (!s || !s->start || from >= s->len)
        return line;

    afxChar const* str = s->start;
    afxChar const* end = s->start + s->len;

    // Use your fast search function
    afxChar const* pos;
    afxUnit posIdx;

    if (AfxFindChar(s, from, '\n', FALSE, FALSE, &posIdx))
    {
        pos = s->start + posIdx;
    }
    else
    {
        if (AfxFindChar(s, from, '\r', FALSE, FALSE, &posIdx))
            pos = s->start + posIdx;
        else
            pos = NIL;
    }

    afxUnit len = (pos) ? (afxUnit)(pos + 1 - (str + from)) : (afxUnit)(end - (str + from));

    line.start = str + from;
    line.len = len;
    line.cap = 0;

    return line;
}

_AFXINL afxBool AfxFindChar(afxString const* s, afxUnit from, afxInt ch, afxBool ci, afxBool reversely, afxUnit* posn)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    afxBool rslt = FALSE;
    afxUnit strLen = s->len;

    if (strLen <= from)
        return rslt;

    afxChar* data = (afxChar*)s->start;

    // If case-insensitive, we can convert character to lowercase for comparison
    if (ci)
    {
        // Convert the search character to lowercase (as AfxTolower likely does)
        ch = AfxTolower((afxUnit)ch);

        if (reversely)
        {
            // Use strrchr for reverse search (case-insensitive)
            afxChar* found = strrchr(data + from, ch);
            if (found)
            {
                if (posn)
                    *posn = found - data;
                return TRUE;
            }
        }
        else
        {
            // Forward search using strchr (case-insensitive)
            afxChar* found = strchr(data + from, ch);
            if (found)
            {
                if (posn)
                    *posn = found - data;
                return TRUE;
            }
        }
    }
    else
    {
        // Case-sensitive search
        if (reversely)
        {
            // Reverse search using strrchr
            afxChar* found = strrchr(data + from, ch);
            if (found)
            {
                if (posn)
                    *posn = found - data;

                return TRUE;
            }
        }
        else
        {
            // Forward search using strchr
            afxChar* found = strchr(data + from, ch);
            if (found)
            {
                if (posn)
                    *posn = found - data;

                return TRUE;
            }
        }
    }

    return FALSE;
}

_AFXINL afxString AfxFindSubstrings(afxString const* s, afxUnit from, afxBool ci, afxUnit cnt, afxString const substrings[], afxUnit* matchedIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(s);

    if (!s->len)
        return (afxString) { 0 };

    AFX_ASSERT(s->start);
    AFX_ASSERT_RANGE(s->len, from, 1);
    
    afxChar const* src = s->start;
    afxUnit slen = s->len;

    if (from >= slen)
        return (afxString) { 0, 0, NIL };

    afxUnit bestPos = slen;
    afxUnit bestIdx = AFX_INVALID_INDEX;
    afxUnit bestLen = 0;

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxString const* sub = &substrings[i];
        
        // Skip any substring that's obviously too long to ever match.
        if (sub->len > slen)
            continue;

        afxUnit maxSearch = slen - sub->len;

        for (afxUnit j = from; j <= maxSearch; ++j)
        {
            afxBool match = 1;

            for (afxUnit k = 0; k < sub->len; ++k)
            {
                afxChar ch1 = src[j + k];
                afxChar ch2 = sub->start[k];

                if (ci)
                {
                    ch1 = AfxTolower(ch1);
                    ch2 = AfxTolower(ch2);
                }

                if (ch1 != ch2)
                {
                    match = 0;
                    break;
                }
            }

            if (match && j < bestPos)
            {
                bestPos = j;
                bestIdx = i;
                bestLen = sub->len;
                break; // no need to check this substring further
            }
        }
    }

    if (bestIdx != AFX_INVALID_INDEX)
    {
        if (matchedIdx)
            *matchedIdx = bestIdx;

        return (afxString) {
            .len = bestLen,
            .cap = 0,
            .start = &src[bestPos]
        };
    }

    if (matchedIdx) *matchedIdx = AFX_INVALID_INDEX;

    return (afxString) { 0 }; // Not found
}

_AFXINL afxString AfxFindSubstring(afxString const* s, afxUnit from, afxBool ci, afxString const* substring)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    afxUnit idx = 0;
    return AfxFindSubstrings(s, from, ci, 1, substring, &idx);
}

_AFXINL afxResult AfxDumpString(afxString const* s, afxUnit base, afxUnit len, void *dst)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT_RANGE(s->len, base, len);

    // sanitize arguments
    base = AFX_MIN(base, s->len - 1);
    len = AFX_MIN(len, s->len - base);

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
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(s);

    va_list args;
    AFX_ASSERT(fmt);
    va_start(args, fmt);
    afxUnit rslt = AfxFormatStringArg(s, fmt, args);
    AFX_ASSERT(rslt == s->len);
    va_end(args);
    return rslt;
}

AFX afxUnit AfxCopySubstring(afxString* dst, afxUnit at, afxString const* src, afxUnit from, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(dst);
    AFX_ASSERT(!src->cap || dst->start);
    AFX_ASSERT(src);
    AFX_ASSERT(!src->len || src->start);
    AFX_ASSERT(from <= src->len);
    AFX_ASSERT(at <= dst->cap);

    // Remaining characters in source from 'from'
    afxUnit availSrc = src->len - from;
    afxUnit toCopy = (len < availSrc) ? len : availSrc;

    // Available room in destination from 'at'
    afxUnit availDst = (at < dst->cap) ? (dst->cap - at) : 0;
    afxUnit actualCopy = (toCopy < availDst) ? toCopy : availDst;

    if (actualCopy > 0)
        AfxCopy(dst->buf + at, src->start + from, actualCopy);

    // Update string length
    if ((at + actualCopy) > dst->len)
        dst->len = at + actualCopy;

#ifndef AFX_DONT_TRY_TO_ZERO_TERMINATE
    // Null-terminate if room
    if (dst->len < dst->cap)
        (dst->buf)[dst->len] = '\0';
#endif//AFX_DONT_TRY_TO_ZERO_TERMINATE

    return toCopy - actualCopy; // Return remaining uncopied length
}

_AFXINL afxUnit AfxCopyString(afxString* dst, afxUnit at, afxString const* src, afxUnit from)
{
    afxError err = { 0 };
    AFX_ASSERT(dst);
    AFX_ASSERT(!dst->cap || dst->start);
    
    if (!src)
    {
        AfxResetStrings(1, dst);
        return 0;
    }
    AFX_ASSERT(!src->len || src->start);
    AFX_ASSERT(at <= dst->cap);
    AFX_ASSERT(from <= src->len);
    return AfxCopySubstring(dst, at, src, from, src->len - from);
}

_AFXINL afxUnit AfxMoveSubstring(afxString* dst, afxUnit at, afxString const* src, afxUnit from, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(dst);
    AFX_ASSERT(!src->cap || dst->start);
    AFX_ASSERT(src);
    AFX_ASSERT(!src->len || src->start);
    AFX_ASSERT(at <= dst->cap);
    AFX_ASSERT(from <= src->len);

    // Calculate how much can actually be copied
    afxUnit availSrc = src->len - from;
    afxUnit toCopy = (len < availSrc) ? len : availSrc;

    afxUnit availDst = (at < dst->cap) ? (dst->cap - at) : 0;
    afxUnit actualCopy = (toCopy < availDst) ? toCopy : availDst;

    if (actualCopy > 0)
        AfxMove(dst->buf + at, src->start + from, actualCopy);

    // Update dst->len if the move extended it
    if ((at + actualCopy) > dst->len)
        dst->len = at + actualCopy;

#ifndef AFX_DONT_TRY_TO_ZERO_TERMINATE
    // Null-terminate if possible
    if (dst->len < dst->cap)
        (dst->buf)[dst->len] = '\0';
#endif//AFX_DONT_TRY_TO_ZERO_TERMINATE

    // Characters that couldn't be copied.
    return toCopy - actualCopy;
}

_AFXINL afxUnit AfxMoveString(afxString* dst, afxUnit at, afxString const* src, afxUnit from)
{
    afxError err = { 0 };
    AFX_ASSERT(dst);
    AFX_ASSERT(!src->cap || dst->start);
    AFX_ASSERT(src);
    AFX_ASSERT(!src->len || src->start);
    AFX_ASSERT(at <= dst->cap);
    AFX_ASSERT(from <= src->len);
    return AfxMoveSubstring(dst, at, src, from, src->len - from);
}

_AFXINL afxUnit AfxCatenateStrings(afxString* s, afxUnit cnt, afxString const src[])
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(s->buf);
    AFX_ASSERT(src);
    afxUnit excluded = 0;

    afxByte* dst = (afxByte*)s->buf;
    afxUnit written = s->len;
    afxUnit remaining = s->cap > written ? (s->cap - written) : 0;
    afxUnit dropped = 0;

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxUnit copyLen = src[i].len;
        if (!copyLen) continue;

        if (remaining >= copyLen)
        {
            AfxCopy(dst + written, src[i].start, copyLen);
            written += copyLen;
            remaining -= copyLen;
        }
        else
        {
            if (remaining)
            {
                AfxCopy(dst + written, src[i].start, remaining);
                written += remaining;
                copyLen -= remaining;
                remaining = 0;
            }
            dropped += copyLen; // count what we couldn't write.
        }
    }

#ifndef AFX_DONT_TRY_TO_ZERO_TERMINATE
    // If there's still room, null-terminate
    if (s->cap > written)
        dst[written] = '\0';
#endif//AFX_DONT_TRY_TO_ZERO_TERMINATE

    s->len = written;
    return dropped;
}

_AFXINL afxUnit AfxCatenateString(afxString* s, afxChar const* start, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(s);

    if (!len && start)
        len = AfxStrlen(start);

    afxString tmp;
    AfxMakeString(&tmp, 0, start, len);
    return AfxCatenateStrings(s, 1, &tmp);
}

_AFXINL afxUnit AfxInsertString(afxString* dst, afxUnit at, afxUnit cnt, afxString const src[])
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(dst);

    if (!dst || !src || at > dst->len)
        return 0;

    afxUnit failedLen = 0;

    // Total length available in the destination buffer.
    afxUnit remainingCap = (dst->cap > dst->len) ? dst->cap - dst->len : 0;

    // Compute total insert size.
    afxUnit insertLen = 0;
    for (afxUnit i = 0; i < cnt; ++i)
        insertLen += src[i].len;

    // Adjust insert length to what's possible.
    afxUnit actualInsertLen = AFX_MIN(insertLen, remainingCap);

    // Shift the tail (if needed) to make room.
    afxUnit tailLen = dst->len - at;
    if (actualInsertLen > 0 && tailLen > 0)
    {
        afxChar* data = dst->buf;
        AfxMove(data + at + actualInsertLen, data + at, tailLen);
    }

    // Insert strings.
    afxChar* dest = dst->buf + at;
    afxUnit copied = 0;
    for (afxUnit i = 0; i < cnt && copied < actualInsertLen; ++i)
    {
        afxUnit n = AFX_MIN(src[i].len, actualInsertLen - copied);
        AfxCopy(dest + copied, src[i].start, n);
        copied += n;
    }

    dst->len += copied;

#ifndef AFX_DONT_TRY_TO_ZERO_TERMINATE
    // Null-terminate if there's still room
    if (dst->len < dst->cap)
        (dst->buf)[dst->len] = '\0';
#endif//AFX_DONT_TRY_TO_ZERO_TERMINATE

    failedLen = insertLen - copied;
    return failedLen;
}

_AFXINL afxUnit AfxEraseString(afxString* s, afxUnit from, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    AFX_ASSERT_RANGE(s->cap, from, range);

    if (!s || !s->start || from >= s->len || range == 0)
        return 0;

    afxUnit actualLen = AFX_MIN(range, s->len - from);
    afxChar* data = s->buf;

    // Move tail over erased segment
    afxUnit tailLen = s->len - (from + actualLen);
    if (tailLen > 0)
        AfxMove(data + from, data + from + actualLen, tailLen);

    s->len -= actualLen;

    // Null-terminate if possible
    if (s->len < s->cap)
        data[s->len] = '\0';

    return actualLen;
}

_AFXINL afxResult AfxCompareString(afxString const* s, afxUnit from, afxChar const* start, afxUnit len, afxBool ci)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(start);

    if (!len)
        len = AfxStrlen(start);

    if (s->len + len == 0) return 0;
    else if ((s->len != len) || ((!s->len && len) || (!len && s->len))) // if have different lenghts or any of them is blank.
        return s->len - len;

    afxString b;
    AfxMakeString(&b, 0, start, len);
    return (!AfxCompareSubstrings(s, from, b.len, ci, 1, &b, NIL));
}

_AFXINL afxResult _AfxCompareStringRange2(afxString const* s, afxUnit lenA, afxString const* other, afxUnit lenB)
{
    afxError err = { 0 };
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

_AFXINL afxBool AfxCompareSubstrings(afxString const* s, afxUnit from, afxUnit len, afxBool ci, afxUnit cnt, afxString const others[], afxUnit* matchedIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(s);

    afxChar const* srcStart = s->start;

    // Bail out if source has not data.
    if (!srcStart) return FALSE;
    AFX_ASSERT(from <= s->len);
    AFX_ASSERT((from + len) <= s->len);
#ifndef _AFX_STRING_RESILIENCE_ENABLED
    from = AFX_MIN(from, s->len - 1);
    len = AFX_MIN(len, s->len - from);
#endif

    afxBool matched = FALSE;

    if (ci)
    {
        AFX_ASSERT(ci == TRUE);
        AFX_ASSERT(others);

        for (afxUnit i = 0; i < cnt; ++i)
        {
            afxString const* o = &others[i];

            // Fast discard for different lengths.
            if (o->len != len) continue;
            
            // Discard empty strings.
            if (!o->start) continue;

            matched = (0 == AfxStrnicmp(o->start, srcStart, len));

            if (matched)
            {
                if (matchedIdx) *matchedIdx = i;
                break;
            }
        }
    }
    else
    {
        AFX_ASSERT(others);

        for (afxUnit i = 0; i < cnt; ++i)
        {
            afxString const* o = &others[i];

            // Fast discard for different lengths.
            if (o->len != len) continue;

            // Discard empty strings.
            if (!o->start) continue;

            matched = (0 == AfxStrncmp(o->start, srcStart, len));

            if (matched)
            {
                if (matchedIdx) *matchedIdx = i;
                break;
            }
        }
    }
    return matched;
}

_AFXINL afxBool AfxCompareStrings(afxString const* s, afxUnit from, afxBool ci, afxUnit cnt, afxString const others[], afxUnit* matchedIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(others);
    AFX_ASSERT(s);
    return AfxCompareSubstrings(s, from, s->len - from, ci, cnt, others, matchedIdx);
}

_AFXINL afxError AfxCloneSubstring(afxString* s, afxString const* in, afxUnit base, afxUnit len)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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

                    while (!AfxCompareStrings(&t1, 0, FALSE, 1, &t2, NIL) || !WildCardMatch(&t2, v4, v6))
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT(!len || (len && start));

    afxBool hasLen = !!len;

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
            AFX_ASSERT(src.len == len);
            AfxCopyString(s, 0, &src, 0);
            AFX_ASSERT(s->len == len);
        }
        AFX_ASSERT(s->len == len);
    }
    return err;
}

#if 0
_AFXINL afxError AfxReallocateString(afxString* s, afxUnit cap, void const *start, afxUnit len)
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxUnit effectiveRange = AFX_MIN(len, effectiveCap); // truncate to capacity when string has space.
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

_AFXINL afxError AfxReadString(afxString* s, afxStream in, afxUnit len)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AFX_ASSERT(AfxGetStringCapacity(s, 0));
    AFX_ASSERT_RANGE(s->cap, 0, len);
    afxUnit errLen = 0;

    if ((errLen = AfxReadStream(in, len, 0, s->buf)))
        AfxThrowError();

    s->len = len - errLen;
    return errLen;
}

_AFXINL afxUnit AfxDumpStringAsUtf16(afxString const* s, afxUnit bufCap, wchar_t* wideBuf)
// Helper function to convert UTF-8 to UTF-16
{
    afxError err = { 0 };
    AFX_ASSERT(s);

    // Estimate the maximum size of the wide char string (UTF-16)
    // wideBuf is the maximum size (worst case each UTF-8 byte is 1 UTF-16 character)

    afxUnit utf8Index = 0;
    afxUnit wideIndex = 0;
    afxChar const* utf8Str = s->start;

    while (utf8Str[utf8Index] != '\0')
    {
        afxByte c = utf8Str[utf8Index];

        if (c <= 0x7F)
        {  // 1-byte character (ASCII)
            wideBuf[wideIndex++] = (wchar_t)c;
            utf8Index++;
        }
        else if (c >= 0xC2 && c <= 0xDF)
        {  // 2-byte character (UTF-8)
            afxByte c2 = utf8Str[utf8Index + 1];
            wideBuf[wideIndex++] = ((wchar_t)(c & 0x1F) << 6) | (c2 & 0x3F);
            utf8Index += 2;
        }
        else if (c >= 0xE0 && c <= 0xEF)
        {  // 3-byte character (UTF-8)
            afxByte c2 = utf8Str[utf8Index + 1];
            afxByte c3 = utf8Str[utf8Index + 2];
            wideBuf[wideIndex++] = ((wchar_t)(c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            utf8Index += 3;
        }
        else if (c >= 0xF0 && c <= 0xF4)
        {  // 4-byte character (UTF-8)
            afxByte c2 = utf8Str[utf8Index + 1];
            afxByte c3 = utf8Str[utf8Index + 2];
            afxByte c4 = utf8Str[utf8Index + 3];
            afxInt32 wc = ((wchar_t)(c & 0x07) << 18) | ((c2 & 0x3F) << 12) | ((c3 & 0x3F) << 6) | (c4 & 0x3F);
            
            // Handle surrogate pairs for characters beyond 0xFFFF
            if (wc >= 0x10000 && wc <= 0x10FFFF)
            {
                // UTF-16 surrogate pair encoding
                wc -= 0x10000;
                wchar_t highSurrogate = (wchar_t)((wc >> 10) + 0xD800);  // High surrogate
                wchar_t lowSurrogate = (wchar_t)((wc & 0x3FF) + 0xDC00);  // Low surrogate
                wideBuf[wideIndex++] = highSurrogate;
                wideBuf[wideIndex++] = lowSurrogate;
            }
            else
            {
                // For characters within the BMP (<= 0xFFFF), just store as normal
                wideBuf[wideIndex++] = wc;
            }
            utf8Index += 4;
        }
        else
        {
            // Invalid UTF-8 byte sequence
            return 0;
        }
    }
    wideBuf[wideIndex] = L'\0';  // Null-terminate the wide character string
    return utf8Index;
}

_AFXINL afxUnit AfxLoadStringAsUtf16(afxString* s, wchar_t const* wideStr, afxUnit wideStrLen)
// Function to convert wide character (UTF-16) to UTF-8 string
{
    afxError err = { 0 };
    AFX_ASSERT(s);

    // Estimate the maximum size of the UTF-8 string
    if (!wideStrLen)
        wideStrLen = wcslen(wideStr);

    afxUnit utf8Size = wideStrLen * 4 + 1;  // UTF-8 can use up to 4 bytes per character, plus null terminator

    // Allocate memory for the UTF-8 string
    afxChar* utf8Str = s->buf;

    afxUnit utf8Index = 0;
    for (afxUnit i = 0; i < wideStrLen; i++)
    {
        afxInt32 wc = wideStr[i];

        // Check for surrogate pairs (characters outside the BMP)
        if (wc >= 0xD800 && wc <= 0xDBFF && i + 1 < wideStrLen)
        {
            // High surrogate (0xD800 - 0xDBFF)
            wchar_t lowSurrogate = wideStr[i + 1];
            if (lowSurrogate >= 0xDC00 && lowSurrogate <= 0xDFFF)
            {
                // Low surrogate (0xDC00 - 0xDFFF)
                // Combine high and low surrogates to form a code point (32-bit value)
                wc = 0x10000 + ((wc - 0xD800) << 10) + (lowSurrogate - 0xDC00);
                i++;  // Skip the next code unit (low surrogate)
            }
            else
            {
                return 0;  // Invalid surrogate pair (low surrogate missing or incorrect)
            }
        }

        if (wc <= 0x7F)
        {  // 1-byte UTF-8 (ASCII)
            utf8Str[utf8Index++] = (afxChar)wc;
        }
        else if (wc <= 0x7FF)
        {  // 2-byte UTF-8
            utf8Str[utf8Index++] = (afxChar)((wc >> 6) | 0xC0);
            utf8Str[utf8Index++] = (afxChar)((wc & 0x3F) | 0x80);
        }
        else if (wc <= 0xFFFF)
        {  // 3-byte UTF-8
            utf8Str[utf8Index++] = (afxChar)((wc >> 12) | 0xE0);
            utf8Str[utf8Index++] = (afxChar)(((wc >> 6) & 0x3F) | 0x80);
            utf8Str[utf8Index++] = (afxChar)((wc & 0x3F) | 0x80);
        }
        else if (wc <= 0x10FFFF)
        {  // 4-byte UTF-8 (for supplementary characters)
            utf8Str[utf8Index++] = (afxChar)((wc >> 18) | 0xF0);
            utf8Str[utf8Index++] = (afxChar)(((wc >> 12) & 0x3F) | 0x80);
            utf8Str[utf8Index++] = (afxChar)(((wc >> 6) & 0x3F) | 0x80);
            utf8Str[utf8Index++] = (afxChar)((wc & 0x3F) | 0x80);
        }
        else
        {
            // Invalid UTF-16 code unit (out of range)
            return 0;
        }
    }

    utf8Str[utf8Index] = '\0';  // Null-terminate the UTF-8 string
    return utf8Index;
}

////////////////////////////////////////////////////////////////////////////////


_AFXINL void AfxClearStrings(afxUnit cnt, afxString strings[])
{
    afxError err = { 0 };
    AFX_ASSERT(strings);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];
        // Optionally null-terminate if capacity allows.
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
    afxError err = { 0 };
    AFX_ASSERT(strings);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];
        s->len = 0;
        s->cap = 0;
        s->start = NIL;
    }
}

