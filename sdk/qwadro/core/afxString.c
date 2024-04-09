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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "qwadro/core/afxSystem.h"

_AFX afxString strEmptyData = AFX_STRING("");
_AFX afxString const AFX_STR_EMPTY = AFX_STRING("");

_AFXINL afxChar const* AfxGetStringData(afxString const* str, afxNat base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(str->len >= base);
    afxChar const* data;

    if ((data = str->start))
        data += base;

    return data;
};

_AFXINL afxNat AfxGetStringLength(afxString const* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    return str->len;
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
    AfxAssert(str);
    return (0 == str->len);
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
    AfxMakeString(&b, start, len);
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
    AfxMakeString(&b, start, len);
    return AfxCompareString(&a, &b);
}

_AFXINL afxResult AfxCompareStringRangeCi(afxString const* str, afxString const* other, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    afxString a, b;
    AfxMakeString(&a, AfxGetStringData(str, base), len);
    AfxMakeString(&b, AfxGetStringData(other, base), len);
    return AfxCompareStringCi(&a, &b);
}

_AFXINL afxResult AfxCompareStringRange(afxString const* str, afxString const* other, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);

    afxString a, b;
    AfxMakeString(&a, AfxGetStringData(str, base), len);
    AfxMakeString(&b, AfxGetStringData(other, base), len);
    return AfxCompareString(&a, &b);
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
        AfxStrcpy(Out, v3);
        result = 1;
    }
    return result;
}

_AFXINL afxNat AfxExcerptString(afxString const* src, afxNat offset, afxNat len, afxString* selection)
{
    afxError err = AFX_ERR_NONE;
    
    AfxAssert(src);
    afxNat maxRange = AfxGetStringLength(src);
    AfxAssertRange(maxRange, offset, len);
    afxNat clampedRange = len > maxRange ? maxRange : len;
    
    AfxAssert(selection);
    selection->len = clampedRange;
    selection->start = AfxGetStringData(src, offset);
    return len - clampedRange; // return clipped off amount
}

_AFXINL void AfxReflectString(afxString const* str, afxString* reflection)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssert(reflection);
    
    if (AfxExcerptString(str, 0, AfxGetStringLength(str), reflection))
        AfxThrowError();
}

_AFXINL void AfxMakeString(afxString* str, void const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    //AfxAssert(!len || (len && start));
    str->len = len ? len : (start ? AfxStrlen(start) : 0);
    str->start = start;
}

_AFXINL void AfxResetString(afxString* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->len = 0;
    str->start = NIL;
}
