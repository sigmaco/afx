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

#include "../exec/afxSystemDDK.h"

_AFX afxString* AfxMakeString4(afxString4 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString8(afxString8 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString16(afxString16 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString32(afxString32 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString64(afxString64 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString128(afxString128 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString256(afxString256 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString512(afxString512 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString1024(afxString1024 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString2048(afxString2048 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}

_AFX afxString* AfxMakeString4096(afxString4096 *s, afxString const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(s);
    s->buf[0] = '\0';
    AfxMakeString(&s->s, sizeof(s->buf), s->buf, 0);

    if (src)
        AfxCopyString(&s->s, 0, src, 0);

    return &s->s;
}
