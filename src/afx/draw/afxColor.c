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

#include "qwadro/draw/afxDrawSystem.h"

_AVX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING =
{
    afxColorSwizzle_R,
    afxColorSwizzle_G,
    afxColorSwizzle_B,
    afxColorSwizzle_A,
    
};

_AVX afxReal AfxToSrgba(afxReal linear)
{
    return linear <= 0.0031308f ? linear * 12.92f : AfxPowf(linear, 1.0f / 2.4f) * 1.055f - 0.055f;
}

_AVX afxReal AfxFromSrgba(afxReal sRgb)
{
    return sRgb <= 0.04045f ? sRgb / 12.92f : AfxPowf((sRgb + 0.055f) / 1.055f, 2.4f);
}

_AVXINL void AfxResetColor(afxColor color)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxResetV4d(color);
}

_AVXINL void AfxCopyColor(afxColor color, afxColor const from)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxAssert(from);
    AfxCopyV4d(color, from);
}

_AVXINL void AfxMakeRgba(afxColor col, afxByte r, afxByte g, afxByte b, afxByte a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    col[0] = r / 255.f;
    col[1] = g / 255.f;
    col[2] = b / 255.f;
    col[3] = a / 255.f;
}

_AVXINL void AfxMakeRgb(afxColor col, afxByte r, afxByte g, afxByte b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxMakeRgba(col, r, g, b, 0xFF);
}

_AVXINL void AfxSetRgba(afxColor col, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxSetV4d(col, r, g, b, a);
}

_AVXINL void AfxSetRgb(afxColor col, afxReal r, afxReal g, afxReal b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxSetRgba(col, r, g, b, 1.f);
}

_AVXINL void AfxRepackAlpha(afxColor col, afxColor const src, afxByte a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxAssert(src);
    AfxSetRgba(col, src[0], src[1], src[2], a / 255.f);
}

_AVXINL void AfxReplaceAlpha(afxColor col, afxColor const src, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxAssert(src);
    AfxSetRgba(col, src[0], src[1], src[2], a);
}

_AVXINL void AfxAddColor(afxColor c, afxColor const a, afxColor const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(c);
    AfxAssert(a);
    AfxAssert(b);
    AfxAddV4d(c, a, b);
}

_AVXINL void AfxSubColor(afxColor c, afxColor const a, afxColor const b)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(c);
    AfxAssert(a);
    AfxAssert(b);
    AfxSubV4d(c, a, b);
}

_AVXINL void AfxScaleColor(afxColor c, afxColor const from, afxReal lambda)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(c);
    AfxAssert(from);
    AfxScaleV4d(c, from, lambda);
}

_AVXINL void AfxPackRgba8(afxColor const c, afxRgba8 out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(c);
    // Quantize color to rgba8
    out[0] = (afxNat8)((afxInt32)((c[0] * (afxReal)255.0) + (afxReal)0.5));
    out[1] = (afxNat8)((afxInt32)((c[1] * (afxReal)255.0) + (afxReal)0.5));
    out[2] = (afxNat8)((afxInt32)((c[2] * (afxReal)255.0) + (afxReal)0.5));
    out[3] = (afxNat8)((afxInt32)((c[3] * (afxReal)255.0) + (afxReal)0.5));
}

_AVXINL void AfxUnpackRgba8(afxColor c, afxRgba8 const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(c);
    // Quantize rgba8 from color.

    c[0] = ((afxReal)in[0]) * (1.f / 255.f);
    c[1] = ((afxReal)in[1]) * (1.f / 255.f);
    c[2] = ((afxReal)in[2]) * (1.f / 255.f);
    c[3] = ((afxReal)in[3]) * (1.f / 255.f);
}

_AVXINL void AfxPremulAlpha(afxColor c)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(c);
    c[0] *= c[3];
    c[1] *= c[3];
    c[2] *= c[3];
}

_AVXINL void AfxLerpColor(afxColor col, afxColor const c0, afxColor const c1, afxReal u)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxAssert(c0);
    AfxAssert(c1);
    u = AfxClampf(u, 0.f, 1.f);
    afxReal oneMinusU = 1.f - u;
    AfxSetRgba(col, c0[0] * oneMinusU + c1[0] * u, c0[1] * oneMinusU + c1[1] * u, c0[2] * oneMinusU + c1[2] * u, c0[3] * oneMinusU + c1[3] * u);
}

_AVXINL afxReal AfxMakeHue(afxReal h, afxReal m1, afxReal m2)
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

_AVXINL void AfxMakeHsla(afxColor col, afxReal h, afxReal s, afxReal l, afxByte a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    h = AfxModf(h, 1.f);

    if (h < 0.0f)
        h += 1.f;

    s = AfxClampf(s, 0.f, 1.f);
    l = AfxClampf(l, 0.f, 1.f);
    afxReal m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
    afxReal m1 = 2.f * l - m2;
    col[0] = AfxClampf(AfxMakeHue(h + 1.f / 3.f, m1, m2), 0.f, 1.f);
    col[1] = AfxClampf(AfxMakeHue(h, m1, m2), 0.f, 1.f);
    col[2] = AfxClampf(AfxMakeHue(h - 1.f / 3.f, m1, m2), 0.f, 1.f);
    col[3] = a / 255.f;
}

_AVXINL void AfxMakeHsl(afxColor col, afxReal h, afxReal s, afxReal l)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(col);
    AfxMakeHsla(col, h, s, l, 0xFF);
}
