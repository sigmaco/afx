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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "../dev/AvxImplKit.h"

_AVX avxColorSwizzling const AFX_STD_COLOR_SWIZZLING =
{
    avxColorSwizzle_R,
    avxColorSwizzle_G,
    avxColorSwizzle_B,
    avxColorSwizzle_A
};

_AVX afxReal AfxToSrgba(afxReal linear)
{
    return linear <= 0.0031308f ? linear * 12.92f : AfxPowf(linear, 1.0f / 2.4f) * 1.055f - 0.055f;
}

_AVX afxReal AfxFromSrgba(afxReal sRgb)
{
    return sRgb <= 0.04045f ? sRgb / 12.92f : AfxPowf((sRgb + 0.055f) / 1.055f, 2.4f);
}

_AVXINL void AfxColorReset(afxColor c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    AfxResetV4d(c);
}

_AVXINL void AfxColorSet(afxColor c, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    AfxV4dSet(c, r, g, b, a);
}

_AVXINL void AfxColorSet8(afxColor c, afxByte r, afxByte g, afxByte b, afxByte a)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    c[0] = r / 255.f;
    c[1] = g / 255.f;
    c[2] = b / 255.f;
    c[3] = a / 255.f;
}

_AVXINL void AfxColorCopy(afxColor c, afxColor const src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(c, src);
    AfxV4dCopy(c, src);
}

_AVXINL void AfxColorCopyA(afxColor c, afxColor const src, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(c, src);
    AfxColorSet(c, src[0], src[1], src[2], a);
}

_AVXINL void AfxColorCopyA8(afxColor c, afxColor const src, afxByte a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(c, src);
    AfxColorSet(c, src[0], src[1], src[2], a / 255.f);
}

_AVXINL void AfxColorCopyRgba8(afxColor c, afxUnit rgba)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    c[0] = (afxReal)((rgba >> 24) & 0xFF) / 255.0f;
    c[1] = (afxReal)((rgba >> 16) & 0xFF) / 255.0f;
    c[2] = (afxReal)((rgba >> 8) & 0xFF) / 255.0f;
    c[3] = (afxReal)((rgba & 0xFF) / 255.0f);
}

_AVXINL void AfxColorCopyArgb8(afxColor c, afxUnit argb)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    c[3] = (afxReal)((argb >> 24) & 0xFF) / 255.0f;
    c[0] = (afxReal)((argb >> 16) & 0xFF) / 255.0f;
    c[1] = (afxReal)((argb >> 8) & 0xFF) / 255.0f;
    c[2] = (afxReal)((argb & 0xFF) / 255.0f);
}

_AVXINL void AfxColorAdd(afxColor c, afxColor const a, afxColor const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert3(c, a, b);
    AfxV4dAdd(c, a, b);
}

_AVXINL void AfxColorSub(afxColor c, afxColor const a, afxColor const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert3(c, a, b);
    AfxV4dSub(c, a, b);
}

_AVXINL void AfxColorScale(afxColor c, afxColor const from, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(c, from);
    AfxV4dScale(c, from, lambda);
}

_AVXINL afxUnit32 AfxGetColorAsRgba8(afxColor const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    return ((((afxUnit32)(c[0] * 255.0f)) << 24) | (((afxUnit32)(c[1] * 255.0f)) << 16) | (((afxUnit32)(c[2] * 255.0f)) << 8) | ((afxUnit32)(c[3] * 255.0f)));
}

_AVXINL afxUnit32 AfxGetColorAsArgb8(afxColor const c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    return ((((afxUnit32)(c[3] * 255.0f)) << 24) | (((afxUnit32)(c[0] * 255.0f)) << 16) | (((afxUnit32)(c[1] * 255.0f)) << 8) | ((afxUnit32)(c[2] * 255.0f)));
}

_AVXINL void AfxColorPremulAlpha(afxColor c, afxColor const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(c, in);
    c[0] = in[0] * in[3];
    c[1] = in[1] * in[3];
    c[2] = in[2] * in[3];
    c[3] = in[3];
}

_AVXINL void AfxColorMix(afxColor c, afxColor const c0, afxColor const c1, afxReal u)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert3(c, c0, c1);
    u = AfxClampf(u, 0.f, 1.f);
    AfxV4dMix(c, c0, c1, u);
}

_AVXINL afxReal AfxColorSetHue(afxReal h, afxReal m1, afxReal m2)
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

_AVXINL void AfxColorSetHsla(afxColor c, afxReal h, afxReal s, afxReal l, afxByte a)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    h = AfxModf(h, 1.f);

    if (h < 0.0f)
        h += 1.f;

    s = AfxClampf(s, 0.f, 1.f);
    l = AfxClampf(l, 0.f, 1.f);
    afxReal m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
    afxReal m1 = 2.f * l - m2;
    c[0] = AfxClampf(AfxColorSetHue(h + 1.f / 3.f, m1, m2), 0.f, 1.f);
    c[1] = AfxClampf(AfxColorSetHue(h, m1, m2), 0.f, 1.f);
    c[2] = AfxClampf(AfxColorSetHue(h - 1.f / 3.f, m1, m2), 0.f, 1.f);
    c[3] = a / 255.f;
}

_AVXINL void AfxColorSetHsl(afxColor c, afxReal h, afxReal s, afxReal l)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(c);
    AfxColorSetHsla(c, h, s, l, 0xFF);
}
