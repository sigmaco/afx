/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_COLOR_H
#define AVX_COLOR_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/math/afxVector.h"

typedef afxV4d      AFX_SIMD avxColor;

typedef afxUnit8     AFX_SIMD afxRgb8[3];
typedef afxUnit8     AFX_SIMD afxRgba8[4];

typedef enum avxColorSpace
{
    avxColorSpace_LINEAR = 1,
    avxColorSpace_STANDARD // sRGB
} avxColorSpace;

typedef enum avxColorChannel
{
    avxColorChannel_R,
    avxColorChannel_G,
    avxColorChannel_B,
    avxColorChannel_A
}avxColorChannel;

typedef enum avxColorSwizzle
{
    avxColorSwizzle_R,
    avxColorSwizzle_G,
    avxColorSwizzle_B,
    avxColorSwizzle_A,
    avxColorSwizzle_ONE,
    avxColorSwizzle_ZERO,

    avxColorSwizzle_TOTAL
} avxColorSwizzle;

AFX_DEFINE_STRUCT(avxSwizzling)
{
    avxColorSwizzle r, g, b, a;
};

#define AVX_COLOR(x_, y_, z_, w_) (avxColor const){ (afxReal)(x_), (afxReal)(y_), (afxReal)(z_), (afxReal)(w_) }

#ifdef AFX_LE
#   define AVX_ARGB(a, r, g, b) (afxUnit32)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#else
#   define AVX_ARGB(a, r, g, b) (afxUnit32)(((b) << 24) | ((g) << 16) | ((r) << 8) | (a))
#endif

AVX avxSwizzling const AVX_COLOR_SWIZZLING_IDENTITY;

AVXINL void     AvxResetColor(avxColor c);


AVXINL void     AvxMakeColor(avxColor c, afxReal r, afxReal g, afxReal b, afxReal a);
AVXINL void     AvxMakeColor8(avxColor col, afxByte r, afxByte g, afxByte b, afxByte a);

AVXINL void     AvxCopyColor(avxColor c, avxColor const src);
AVXINL void     AvxCopyColorA(avxColor col, avxColor const src, afxReal a);
AVXINL void     AvxCopyColorA8(avxColor col, avxColor const src, afxByte a);

AVXINL void     AvxAddColor(avxColor c, avxColor const a, avxColor const b);
AVXINL void     AvxSubColor(avxColor c, avxColor const a, avxColor const b);

AVXINL void     AvxScaleColor(avxColor c, avxColor const from, afxReal lambda);

AVXINL afxUnit32 AvxGetColorRgba8(avxColor const c);
AVXINL afxUnit32 AvxGetColorArgb8(avxColor const c);
AVXINL void     AvxMakeColorRgba8(avxColor c, afxUnit rgba);
AVXINL void     AvxMakeColorArgb8(avxColor c, afxUnit argb);

AVXINL void     AvxPremulColorAlpha(avxColor c, avxColor const in);

AVXINL void     AvxMixColor(avxColor c, avxColor const c0, avxColor const c1, afxReal u);

AVXINL afxReal  AvxSetHue(afxReal h, afxReal m1, afxReal m2);
AVXINL void     AvxMakeColorHsl(avxColor c, afxReal h, afxReal s, afxReal l);
AVXINL void     AvxMakeColorHsla(avxColor c, afxReal h, afxReal s, afxReal l, afxByte a);

#endif//AVX_COLOR_H
