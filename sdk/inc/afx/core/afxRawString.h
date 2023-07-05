/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_RAW_STRING_H
#define AFX_RAW_STRING_H

#include "afx/core/afxCoreDefs.h"

AFX afxChar* AfxFindCharRaw(afxChar const* str, afxInt ch);
AFX afxChar* AfxFindCharRawB2F(afxChar const* str, afxInt ch);
AFX afxChar* AfxFindExcerptRaw(afxChar const* str, afxChar const* excerpt);
AFX afxResult AfxCompareMeasuredRawStringCi(afxChar const* str, afxChar const* str2, afxSize len);
AFX afxResult AfxCopyMeasuredRawString(afxChar const* str, afxSize len, afxChar* to);
AFX afxChar* AfxConvertRawStringCpToUtf8(afxChar* str, afxInt cp);
AFX afxResult AfxCompareRawStringCi(afxChar const* str, afxChar const* str2);
AFX void AfxConcatRawString(afxChar* str, afxChar const* str2);
AFX afxResult AfxMeasureRawString(afxChar const* str);
AFX afxResult AfxCountRawStringCodepoints(afxChar const* str);
AFX afxResult AfxCountMeasuredRawStringCodepoints(afxChar const* str, afxSize siz);
AFX void AfxCopyRawString(afxChar* str, afxChar const* from);
AFX afxResult AfxCompareRawString(afxChar const* str, afxChar const* str2);
AFX afxResult AfxCompareMeasuredRawString(afxChar const* str, afxChar const* str2, afxNat len);

AFX afxResult AfxFormatRawString(afxChar* buf, afxChar const* fmt, ...);
AFX afxResult AfxFormatMeasuredRawString(afxChar* buf, afxNat size, afxChar const* fmt, ...);
AFX afxResult AfxFormatMeasuredRawStringFromVaList(afxChar* buf, size_t size, afxChar const* fmt, va_list args);
AFX afxResult AfxFormatRawStringFromVaList(afxChar* buf, afxChar const* fmt, va_list args);
AFX afxResult AfxScanRawString(afxChar const* str, afxChar const* fmt, ...);
AFX afxResult AfxScanRawStringIntoVaList(afxChar const* buf, afxChar const* fmt, va_list args);

AFX void* utf8codepoint(const void* str, afxInt32* out_codepoint);
AFX size_t utf8codepointsize(afxInt32 ch);
AFX afxInt32 utf8lwrcodepoint(afxInt32 cp);
AFX afxInt32 utf8uprcodepoint(afxInt32 cp);

#define AfxPopulateRawString(a_, b_) \
do { \
    afxSize const n = sizeof((a_)) / sizeof((a_)[0]); \
    AfxCopyMeasuredRawString((b_), n, (a_)); \
    \
    if (n <= (afxSize)AfxMeasureRawString((afxChar*)(b_))) \
    { \
        afxSize const trunc = n - 1; \
        (a_)[trunc] = ((afxChar)0); \
        /*AfxAdvertise("String truncated!\n    %s", (a_)); */\
    } \
} while(0)

#endif//AFX_RAW_STRING_H