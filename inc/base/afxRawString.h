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

#ifndef AFX_RAW_STRING_H
#define AFX_RAW_STRING_H

#include "qwadro/inc/base/afxCoreDefs.h"

/// This function checks whether the passed character is alphabetic.
AFXINL afxBool      AfxIsalpha(afxInt ch);
/// This function checks whether the passed character is alphanumeric.
AFXINL afxBool      AfxIsalnum(afxInt ch);
/// This function checks whether the passed character is decimal digit.
AFXINL afxBool      AfxIsdigit(afxInt ch);
/// This function checks whether the passed character is a hexadecimal digit.
AFXINL afxBool      AfxIsxdigit(afxInt ch);
/// This function checks whether the passed character is control character.
AFXINL afxBool      AfxIscntrl(afxInt ch);
/// This function checks whether the passed character is lowercase letter.
AFXINL afxBool      AfxIslower(afxInt ch);
/// This function checks whether the passed character is an uppercase letter.
AFXINL afxBool      AfxIsupper(afxInt ch);
/// This function checks whether the passed character is white-space.
AFXINL afxBool      AfxIsspace(afxInt ch);

AFXINL afxInt       AfxTolower(afxInt ch);
AFXINL afxInt       AfxToupper(afxInt ch);

/// Computes the length of the string str up to but not including the terminating null character.
AFXINL afxResult    AfxStrlen(afxChar const* str);
/// Calculates the length of the initial segment of str1 which consists entirely of characters in str2.
AFXINL afxSize      AfxStrspn(afxChar const* str, afxChar const* control);
/// Calculates the length of the initial segment of str1 which consists entirely of characters in str2.
AFXINL afxSize      AfxStrnspn(afxChar const* str, afxSize strLen, afxChar const* control, afxSize controlLen);
/// Calculates the length of the initial segment of str which consists entirely of characters not in control.
AFXINL afxSize      AfxStrcspn(afxChar const* str, afxChar const* control);
/// Calculates the length of the initial segment of str which consists entirely of characters not in control.
AFXINL afxSize      AfxStrncspn(afxChar const* str, afxSize strLen, afxChar const* control, afxSize controlLen);

/// Searches for the first occurrence of the character ch (an unsigned char) in the string pointed to, by the argument str.
AFXINL afxChar*     AfxStrchr(afxChar const* str, afxInt ch);
/// Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str.
AFXINL afxChar*     AfxStrrchr(afxChar const* str, afxInt ch);
/// Finds the first character in the string str1 that matches any character specified in control.
AFXINL afxChar*     AfxStrpbrk(afxChar const* str1, afxChar const* control);
/// Finds the first occurrence of the entire excerpt (not including the terminating null character) which appears in the str.
AFXINL afxChar*     AfxStrstr(afxChar const* str, afxChar const* excerpt);
AFXINL afxChar*     AfxStrnstr(afxChar const* str, afxUnit strLen, afxChar const* substr, afxUnit substrLen);


/// Appends the string pointed to, by src to the end of the string pointed to by dst.
AFXINL afxChar*     AfxStrcat(afxChar* dst, afxChar const* src);
/// Appends the string pointed to, by src to the end of the string pointed to, by dst up to len characters long.
AFXINL afxChar*     AfxStrncat(afxChar* dst, afxChar const* src, afxUnit len);


/// Copies the string pointed to, by src to dst.
AFXINL afxChar*     AfxStrcpy(afxChar* dst, afxChar const* src);
/// Copies up to n characters from the string pointed to, by src to dst.
AFXINL afxChar*     AfxStrncpy(afxChar* dst, afxChar const* src, afxSize len);


/// Compares the string pointed to, by str1 to the string pointed to by str2.
AFXINL afxResult    AfxStrcmp(afxChar const* str, afxChar const* str2);
/// Compares at most the first len bytes of str1 and str2.
AFXINL afxResult    AfxStrncmp(afxChar const* str, afxChar const* str2, afxUnit len);

AFXINL afxResult    AfxStricmp(afxChar const* str, afxChar const* str2);

AFXINL afxResult    AfxStrnicmp(afxChar const* str, afxChar const* str2, afxSize len);

/// Compares string str1 to str2.The result is dependent on the LC_COLLATE setting of the location.
AFXINL afxInt       AfxStrcoll(afxChar const* str1, afxChar const* str2);


/// Sends formatted output to a string.
AFXINL afxResult    AfxSprintf(afxChar* buf, afxChar const* fmt, ...);
AFXINL afxResult    AfxSnprintf(afxChar* buf, afxUnit size, afxChar const* fmt, ...);

/// Sends formatted output to a string using an argument list.
AFXINL afxResult    AfxVsprintf(afxChar* buf, afxChar const* fmt, va_list args);
AFXINL afxResult    AfxVsnprintf(afxChar* buf, size_t size, afxChar const* fmt, va_list args);

/// Reads formatted input from a string.
AFXINL afxResult    AfxSscanf(afxChar const* str, afxChar const* fmt, ...);
AFXINL afxResult    AfxVsscanf(afxChar const* buf, afxChar const* fmt, va_list args);

AFXINL afxChar*     AfxStrdup(afxChar const* src);

AFX void* utf8codepoint(const void* str, afxInt32* out_codepoint);
AFX size_t utf8codepointsize(afxInt32 ch);
AFX afxInt32 utf8lwrcodepoint(afxInt32 cp);
AFX afxInt32 utf8uprcodepoint(afxInt32 cp);

AFXINL afxChar*     AfxConvertRawStringCpToUtf8(afxChar* str, afxInt cp);
AFXINL afxResult    AfxCountRawStringCodepoints(afxChar const* str);
AFXINL afxResult    AfxCountMeasuredRawStringCodepoints(afxChar const* str, afxSize siz);

#define AfxPopulateRawString(a_, b_) \
do { \
    afxSize const n = sizeof((a_)) / sizeof((a_)[0]); \
    AfxStrncpy((a_), n, (b_)); \
    \
    if (n <= (afxSize)AfxStrlen((afxChar*)(b_))) \
    { \
        afxSize const trunc = n - 1; \
        (a_)[trunc] = ((afxChar)0); \
        /*AfxLogAdvertence("String truncated!\n    %s", (a_)); */\
    } \
} while(0)

#endif//AFX_RAW_STRING_H
