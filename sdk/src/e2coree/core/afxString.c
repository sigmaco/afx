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
#include "qwadro/core/afxString.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxStream.h"

_AFX afxString strEmptyData = AFX_STRING("");
_AFX afxString const AFX_STR_EMPTY = AFX_STRING("");

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
    AfxMakeString(excerpt, AfxStrnstr(str->start, str->len, substring->start, substring->len), substring->len);
    return !!excerpt->len;
}

_AFXINL afxResult AfxTestStringEquivalenceLiteral(afxString const* str, afxNat base, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(start);

    if (!len)
        len = AfxStrlen(start);

    if (str->len + len == 0) return 0;
    else if ((str->len != len) || ((!str->len && len) || (!len && str->len))) // if have different lenghts or any of them is blank.
        return str->len - len;

    afxString a, b;
    AfxExcerptString(&a, str, base, len);
    AfxMakeString(&b, start, len);
    return AfxTestStringEquivalence(&a, &b);
}

_AFXINL afxResult AfxTestStringEqualityLiteral(afxString const* str, afxNat base, afxChar const *start, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);
    AfxAssert(start);

    if (!len)
        len = AfxStrlen(start);

    if (str->len + len == 0) return 0;
    else if ((str->len != len) || ((!str->len && len) || (!len && str->len))) // if have different lenghts or any of them is blank.
        return str->len - len;

    afxString a, b;
    AfxExcerptString(&a, str, base, len);
    AfxMakeString(&b, start, len);
    return AfxTestStringEquality(&a, &b);
}

_AFXINL afxResult AfxTestStringRangeEquivalence(afxString const* str, afxString const* other, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);

    afxString a, b;
    AfxMakeString(&a, AfxGetStringData(str, base), len);
    AfxMakeString(&b, AfxGetStringData(other, base), len);
    return AfxTestStringEquivalence(&a, &b);
}

_AFXINL afxResult AfxTestStringRangeEquality(afxString const* str, afxString const* other, afxNat base, afxNat len)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(str, afxFcc_STR);

    afxString a, b;
    AfxMakeString(&a, AfxGetStringData(str, base), len);
    AfxMakeString(&b, AfxGetStringData(other, base), len);
    return AfxTestStringEquality(&a, &b);
}

_AFXINL afxResult AfxTestStringEquivalence(afxString const* str, afxString const* other)
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

_AFXINL afxResult AfxTestStringEquality(afxString const* str, afxString const* other)
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
                    AfxStrcpy(v6, v3);
                    return 1;
                }

                if (*v3)
                {
                    afxString t1, t2;
                    AfxMakeString(&t1, v4, v10);
                    AfxMakeString(&t2, v3, v10);

                    while (AfxTestStringEquality(&t1, &t2) || !WildCardMatch(&t2, v4, v6))
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

_AFXINL afxError AfxReadStrings(afxStream in, afxNat cnt, afxString dst[], afxNat dstStride)
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(dst);
    AfxAssert(cnt);

    AfxReadStream2(in, cnt * sizeof(dst[0]), sizeof(dst[0]), dst, dstStride);

    return err;
}

_AFXINL afxError AfxWriteStrings(afxStream out, afxNat cnt, afxString const src[], afxNat srcStride)
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(src);
    AfxAssert(cnt);

    AfxWriteStream2(out, cnt * sizeof(src[0]), sizeof(src[0]), src, srcStride);

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
    //AfxAssert(!len || (len && start));

    AfxAssignFcc(str, afxFcc_STR);
    str->len = len ? len : (start ? AfxStrlen(start) : 0);
    str->start = start;
}

_AFXINL void AfxResetString(afxString* str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxAssignFcc(str, afxFcc_STR);
    str->len = 0;
    str->start = NIL;
}
