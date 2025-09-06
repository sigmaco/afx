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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_COLOR_H
#define AVX_COLOR_H

#include "qwadro/draw/avxFormat.h"
#include "qwadro/math/afxVector.h"

#ifdef AFX_LE
#   define AVX_ARGB(a, r, g, b) (afxUnit32)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#else
#   define AVX_ARGB(a, r, g, b) (afxUnit32)(((b) << 24) | ((g) << 16) | ((r) << 8) | (a))
#endif

typedef afxV4d      AFX_SIMD avxColor;

typedef afxUnit8     AFX_SIMD afxRgb8[3];
typedef afxUnit8     AFX_SIMD afxRgba8[4];

#define AVX_COLOR(x_, y_, z_, w_) (avxColor){ \
    (afxReal)(x_), (afxReal)(y_), (afxReal)(z_), (afxReal)(w_) }

#define AVX_DEFAULT_BLEND_CONSTANTS AVX_COLOR(0, 0, 0, 0)

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

typedef enum avxColorMask
// Bitmask controlling which components are written to the framebuffer.
// The color write mask operation is applied regardless of whether blending is enabled.
// The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
{
    // The R value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_R = AFX_BITMASK(0),

    // The G value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_G = AFX_BITMASK(1),

    // The B value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_B = AFX_BITMASK(2),

    // The A value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_A = AFX_BITMASK(3),

    avxColorMask_RG = avxColorMask_R | avxColorMask_G,
    avxColorMask_RGB = avxColorMask_RG | avxColorMask_B,
    avxColorMask_RGBA = avxColorMask_RGB | avxColorMask_A
} avxColorMask;

AFX_DEFINE_STRUCT(avxColorBlend)
// Describes how the color and/or alpha components of a fragment are blended.
{
    // Blend operation is used to calculate the RGB values to write to the color attachment. 
    avxBlendOp          rgbBlendOp; // ADD

    // Blend factor is used to determine the source factors (Sr,Sg,Sb). 
    avxBlendFactor      rgbSrcFactor; // ONE

    // Blend factor is used to determine the destination factors (Dr,Dg,Db). 
    avxBlendFactor      rgbDstFactor; // ZERO

    // Blend operation is used to calculate the alpha values to write to the color attachment.
    avxBlendOp          aBlendOp; // ADD

    // Blend factor is used to determine the source factor Sa.
    avxBlendFactor      aSrcFactor; // ONE

    // Blend factor is used to determine the destination factor Da.
    avxBlendFactor      aDstFactor; // ZERO
};

#define AVX_DEFAULT_COLOR_BLEND (avxColorBlend) { \
    .rgbBlendOp = avxBlendOp_ADD, \
    .rgbSrcFactor = avxBlendFactor_ONE, \
    .rgbDstFactor = avxBlendFactor_ZERO, \
    .aBlendOp = avxBlendOp_ADD, \
    .aSrcFactor = avxBlendFactor_ONE, \
    .aDstFactor = avxBlendFactor_ZERO }

AFX_DEFINE_STRUCT(avxColorOutput)
{
    // Surface format compatible with pixel output.
    avxFormat           fmt;

    // controls whether blending is enabled for the corresponding color attachment. 
    // If blending is not enabled, the source fragment’s color for that attachment is passed through unmodified.
    afxBool             blendEnabled; // FALSE

    // Describes how the color or alpha components of a fragment are blended.
    avxColorBlend       blendConfig;

    // A bitmask to allow discarding of some or all color components.
    avxColorMask        discardMask; // NIL = write all
};

#define AVX_DEFAULT_COLOR_OUTPUT (avxColorOutput) { \
    .fmt = avxFormat_UNDEFINED, \
    .blendEnabled = FALSE, \
    .blendConfig = AVX_DEFAULT_COLOR_BLEND, \
    .discardMask = 0x00000000 }

AFX_DEFINE_STRUCT(avxSwizzling)
{
    avxColorSwizzle r, g, b, a;
};

#define AVX_COLOR_SWIZZLING_IDENTITY (avxSwizzling) { \
    .r = avxColorSwizzle_R, \
    .g = avxColorSwizzle_G, \
    .b = avxColorSwizzle_B, \
    .a = avxColorSwizzle_A }

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

// HSV (Hue, Saturation, Value) or HSL (Hue, Saturation, Lightness) color model.
// A hue value of 256 would be 256 degrees on the color wheel.

AVXINL afxReal  AvxSetHue(afxReal h, afxReal m1, afxReal m2);
AVXINL void     AvxMakeColorHsl(avxColor c, afxReal h, afxReal s, afxReal l);
AVXINL void     AvxMakeColorHsla(avxColor c, afxReal h, afxReal s, afxReal l, afxByte a);

#endif//AVX_COLOR_H
