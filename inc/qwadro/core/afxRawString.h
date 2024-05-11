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

#ifndef AFX_RAW_STRING_H
#define AFX_RAW_STRING_H

#include "qwadro/core/afxCoreDefs.h"

AFXINL afxResult    AfxStrlen(afxChar const* str);

AFXINL afxChar*     AfxStrchr(afxChar const* str, afxInt ch);
AFXINL afxChar*     AfxStrrchr(afxChar const* str, afxInt ch);

AFXINL afxChar*     AfxStrstr(afxChar const* str, afxChar const* excerpt);
AFXINL afxChar const*AfxStrnstr(afxChar const* str, afxNat strLen, afxChar const* substr, afxNat substrLen);

AFXINL afxResult    AfxStrnicmp(afxChar const* str, afxChar const* str2, afxSize len);
AFXINL afxResult    AfxStricmp(afxChar const* str, afxChar const* str2);

AFXINL void         AfxStrcat(afxChar* str, afxChar const* str2);
AFXINL afxChar*     AfxStrdup(afxChar const* src);

AFXINL void         AfxStrcpy(afxChar* str, afxChar const* from);
AFXINL afxResult    AfxStrncpy(afxChar const* str, afxSize len, afxChar* to);

AFXINL afxResult    AfxStrcmp(afxChar const* str, afxChar const* str2);
AFXINL afxResult    AfxStrncmp(afxChar const* str, afxChar const* str2, afxNat len);

AFXINL afxResult    AfxSprintf(afxChar* buf, afxChar const* fmt, ...);
AFXINL afxResult    AfxSnprintf(afxChar* buf, afxNat size, afxChar const* fmt, ...);
AFXINL afxResult    AfxVsprintf(afxChar* buf, afxChar const* fmt, va_list args);
AFXINL afxResult    AfxVsnprintf(afxChar* buf, size_t size, afxChar const* fmt, va_list args);

AFXINL afxResult    AfxSscanf(afxChar const* str, afxChar const* fmt, ...);
AFXINL afxResult    AfxVsscanf(afxChar const* buf, afxChar const* fmt, va_list args);

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
    AfxStrncpy((b_), n, (a_)); \
    \
    if (n <= (afxSize)AfxStrlen((afxChar*)(b_))) \
    { \
        afxSize const trunc = n - 1; \
        (a_)[trunc] = ((afxChar)0); \
        /*AfxLogAdvertence("String truncated!\n    %s", (a_)); */\
    } \
} while(0)

#endif//AFX_RAW_STRING_H
