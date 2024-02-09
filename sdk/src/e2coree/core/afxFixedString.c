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

#include "qwadro/core/afxFixedString.h"

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
