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

typedef enum avxColorChannel
{
    avxColorChannel_R,
    avxColorChannel_G,
    avxColorChannel_B,
    avxColorChannel_A
}avxColorChannel;

typedef afxV4d      AFX_SIMD afxColor;

typedef afxUnit8     AFX_SIMD afxRgb8[3];
typedef afxUnit8     AFX_SIMD afxRgba8[4];

typedef enum avxColorSpace
{
    avxColorSpace_LINEAR = 1,
    avxColorSpace_STANDARD // sRGB
} avxColorSpace;

typedef enum avxColorSwizzle
{
    avxColorSwizzle_ZERO,
    avxColorSwizzle_ONE,
    avxColorSwizzle_A,
    avxColorSwizzle_R,
    avxColorSwizzle_G,
    avxColorSwizzle_B
} avxColorSwizzle;

AFX_DEFINE_STRUCT(avxColorSwizzling)
{
    avxColorSwizzle r, g, b, a;
};

#define AFX_COLOR(x_, y_, z_, w_) (afxColor const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }

#define AFX_ARGB(a, r, g, b) (afxUnit32)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

AVX avxColorSwizzling const AFX_STD_COLOR_SWIZZLING;

AVXINL void     AfxColorReset(afxColor c);


AVXINL void     AfxColorSet(afxColor c, afxReal r, afxReal g, afxReal b, afxReal a);
AVXINL void     AfxColorSet8(afxColor col, afxByte r, afxByte g, afxByte b, afxByte a);

AVXINL void     AfxColorCopy(afxColor c, afxColor const src);
AVXINL void     AfxColorCopyA(afxColor col, afxColor const src, afxReal a);
AVXINL void     AfxColorCopyA8(afxColor col, afxColor const src, afxByte a);

AVXINL void     AfxColorAdd(afxColor c, afxColor const a, afxColor const b);
AVXINL void     AfxColorSub(afxColor c, afxColor const a, afxColor const b);

AVXINL void     AfxColorScale(afxColor c, afxColor const from, afxReal lambda);

AVXINL afxUnit32 AfxGetColorAsRgba8(afxColor const c);
AVXINL afxUnit32 AfxGetColorAsArgb8(afxColor const c);
AVXINL void     AfxColorCopyRgba8(afxColor c, afxUnit rgba);
AVXINL void     AfxColorCopyArgb8(afxColor c, afxUnit argb);

AVXINL void     AfxColorPremulAlpha(afxColor c, afxColor const in);

AVXINL void     AfxColorMix(afxColor c, afxColor const c0, afxColor const c1, afxReal u);

AVXINL afxReal  AfxColorSetHue(afxReal h, afxReal m1, afxReal m2);
AVXINL void     AfxColorSetHsl(afxColor c, afxReal h, afxReal s, afxReal l);
AVXINL void     AfxColorSetHsla(afxColor c, afxReal h, afxReal s, afxReal l, afxByte a);

#endif//AVX_COLOR_H
