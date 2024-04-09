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

#include "qwadro/core/afxSystem.h"

_AFX afxRestring* AfxMakeString4(afxString4 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxRestring* AfxMakeString8(afxString8 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxRestring* AfxMakeString16(afxString16 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxRestring* AfxMakeString32(afxString32 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxRestring* AfxMakeString128(afxString128 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxRestring* AfxMakeString2048(afxString2048 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxRestring* AfxMakeString4096(afxString4096 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeRestring(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}
