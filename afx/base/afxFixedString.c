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

#include "../dev/afxExecImplKit.h"

_AFX afxString* AfxMakeString4(afxString4 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxString* AfxMakeString8(afxString8 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxString* AfxMakeString16(afxString16 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxString* AfxMakeString32(afxString32 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxString* AfxMakeString128(afxString128 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxString* AfxMakeString2048(afxString2048 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}

_AFX afxString* AfxMakeString4096(afxString4096 *str, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(str);
    str->buf[0] = '\0';
    AfxMakeString(&str->str, sizeof(str->buf), str->buf, 0);

    if (src)
        AfxCopyString(&str->str, src);

    return &str->str;
}
