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

#include "src/impl/afxExecImplKit.h"

_AFX afxString* AfxMakeString4(afxString4 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString8(afxString8 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString16(afxString16 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString32(afxString32 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString128(afxString128 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString2048(afxString2048 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString4096(afxString4096 *s, afxString const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}
