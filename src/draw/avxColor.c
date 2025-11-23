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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#include "avxIcd.h"

_AVX afxReal AfxToSrgba(afxReal linear)
{
    return linear <= 0.0031308f ? linear * 12.92f : AfxPowf(linear, 1.0f / 2.4f) * 1.055f - 0.055f;
}

_AVX afxReal AfxFromSrgba(afxReal sRgb)
{
    return sRgb <= 0.04045f ? sRgb / 12.92f : AfxPowf((sRgb + 0.055f) / 1.055f, 2.4f);
}

_AVXINL void AvxResetColor(avxColor c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    AfxResetV4d(c);
}

_AVXINL void AvxMakeColor(avxColor c, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    AfxV4dSet(c, r, g, b, a);
}

_AVXINL void AvxMakeColor8(avxColor c, afxByte r, afxByte g, afxByte b, afxByte a)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    c[0] = r / 255.f;
    c[1] = g / 255.f;
    c[2] = b / 255.f;
    c[3] = a / 255.f;
}

_AVXINL void AvxCopyColor(avxColor c, avxColor const src)
{
    afxError err = { 0 };
    AFX_ASSERT2(c, src);
    AfxV4dCopy(c, src);
}

_AVXINL void AvxCopyColorA(avxColor c, avxColor const src, afxReal a)
{
    afxError err = { 0 };
    AFX_ASSERT2(c, src);
    AvxMakeColor(c, src[0], src[1], src[2], a);
}

_AVXINL void AvxCopyColorA8(avxColor c, avxColor const src, afxByte a)
{
    afxError err = { 0 };
    AFX_ASSERT2(c, src);
    AvxMakeColor(c, src[0], src[1], src[2], a / 255.f);
}

_AVXINL void AvxMakeColorRgba8(avxColor c, afxUnit rgba)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    c[0] = (afxReal)((rgba >> 24) & 0xFF) / 255.0f;
    c[1] = (afxReal)((rgba >> 16) & 0xFF) / 255.0f;
    c[2] = (afxReal)((rgba >> 8) & 0xFF) / 255.0f;
    c[3] = (afxReal)((rgba & 0xFF) / 255.0f);
}

_AVXINL void AvxMakeColorArgb8(avxColor c, afxUnit argb)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    c[3] = (afxReal)((argb >> 24) & 0xFF) / 255.0f;
    c[0] = (afxReal)((argb >> 16) & 0xFF) / 255.0f;
    c[1] = (afxReal)((argb >> 8) & 0xFF) / 255.0f;
    c[2] = (afxReal)((argb & 0xFF) / 255.0f);
}

_AVXINL void AvxAddColor(avxColor c, avxColor const a, avxColor const b)
{
    afxError err = { 0 };
    AFX_ASSERT3(c, a, b);
    AfxV4dAdd(c, a, b);
}

_AVXINL void AvxSubColor(avxColor c, avxColor const a, avxColor const b)
{
    afxError err = { 0 };
    AFX_ASSERT3(c, a, b);
    AfxV4dSub(c, a, b);
}

_AVXINL void AvxScaleColor(avxColor c, avxColor const from, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT2(c, from);
    AfxV4dScale(c, from, lambda);
}

_AVXINL afxUnit32 AvxGetColorRgba8(avxColor const c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    return ((((afxUnit32)(c[0] * 255.0f)) << 24) | (((afxUnit32)(c[1] * 255.0f)) << 16) | (((afxUnit32)(c[2] * 255.0f)) << 8) | ((afxUnit32)(c[3] * 255.0f)));
}

_AVXINL afxUnit32 AvxGetColorArgb8(avxColor const c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    return ((((afxUnit32)(c[3] * 255.0f)) << 24) | (((afxUnit32)(c[0] * 255.0f)) << 16) | (((afxUnit32)(c[1] * 255.0f)) << 8) | ((afxUnit32)(c[2] * 255.0f)));
}

_AVXINL void AvxPremulColorAlpha(avxColor c, avxColor const in)
{
    afxError err = { 0 };
    AFX_ASSERT2(c, in);
    c[0] = in[0] * in[3];
    c[1] = in[1] * in[3];
    c[2] = in[2] * in[3];
    c[3] = in[3];
}

_AVXINL void AvxMixColor(avxColor c, avxColor const c0, avxColor const c1, afxReal u)
{
    afxError err = { 0 };
    AFX_ASSERT3(c, c0, c1);
    u = AfxClampf(u, 0.f, 1.f);
    AfxV4dMix(c, c0, c1, u);
}

_AVXINL afxReal AvxSetHue(afxReal h, afxReal m1, afxReal m2)
{
    if (h < 0.f)
        h += 1.f;

    if (h > 1.f)
        h -= 1.f;

    if (h < 1.f / 6.f)
        return m1 + (m2 - m1) * h * 6.f;
    else if (h < 3.f / 6.f)
        return m2;
    else if (h < 4.f / 6.f)
        return m1 + (m2 - m1) * (2.f / 3.f - h) * 6.f;

    return m1;
}

_AVXINL void AvxMakeColorHsla(avxColor c, afxReal h, afxReal s, afxReal l, afxByte a)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    h = AfxModf(h, 1.f);

    if (h < 0.0f)
        h += 1.f;

    s = AfxClampf(s, 0.f, 1.f);
    l = AfxClampf(l, 0.f, 1.f);
    afxReal m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
    afxReal m1 = 2.f * l - m2;
    c[0] = AfxClampf(AvxSetHue(h + 1.f / 3.f, m1, m2), 0.f, 1.f);
    c[1] = AfxClampf(AvxSetHue(h, m1, m2), 0.f, 1.f);
    c[2] = AfxClampf(AvxSetHue(h - 1.f / 3.f, m1, m2), 0.f, 1.f);
    c[3] = a / 255.f;
}

_AVXINL void AvxMakeColorHsl(avxColor c, afxReal h, afxReal s, afxReal l)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    AvxMakeColorHsla(c, h, s, l, 0xFF);
}
