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

_AVXINL void AfxSetColor(afxColor color, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxSetV4d(color, r, g, b, a);
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

_AVXINL void AfxToRgba8(afxColor const c, afxRgba8 out)
{
    // Quantize color to rgba8
    out[0] = (afxNat8)((afxInt32)((c[0] * (afxReal)255.0) + (afxReal)0.5));
    out[1] = (afxNat8)((afxInt32)((c[1] * (afxReal)255.0) + (afxReal)0.5));
    out[2] = (afxNat8)((afxInt32)((c[2] * (afxReal)255.0) + (afxReal)0.5));
    out[3] = (afxNat8)((afxInt32)((c[3] * (afxReal)255.0) + (afxReal)0.5));
}

_AVXINL void AfxFromRgba8(afxColor c, afxRgba8 const in)
{
    // Quantize rgba8 from color.

    c[0] = ((afxReal)in[0]) * (1.0 / 255.0);
    c[1] = ((afxReal)in[1]) * (1.0 / 255.0);
    c[2] = ((afxReal)in[2]) * (1.0 / 255.0);
    c[3] = ((afxReal)in[3]) * (1.0 / 255.0);
}
