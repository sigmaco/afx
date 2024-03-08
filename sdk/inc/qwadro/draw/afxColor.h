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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_COLOR_H
#define AFX_COLOR_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/math/afxVector.h"

typedef afxV4d      afxColor;
typedef afxNat32    afxRgb8, afxBgr8;
typedef afxNat32    afxRgba8;
typedef afxByte     afxBgra8;
typedef afxByte     afxArgb8;

typedef enum afxColorSpace
{
    afxColorSpace_LINEAR = AfxGetBitOffset(0),
    afxColorSpace_SRGB = AfxGetBitOffset(1)
} afxColorSpace;

typedef enum afxColorSwizzle
{
    afxColorSwizzle_ZERO,
    afxColorSwizzle_ONE,
    afxColorSwizzle_A,
    afxColorSwizzle_R,
    afxColorSwizzle_G,
    afxColorSwizzle_B
} afxColorSwizzle;

typedef enum afxPixelFormat
{
    // non-specified types are expected to be natural number (unsigned int). R = real number (float). I = integer (int)

    afxPixelFormat_R8 = 1,
    afxPixelFormat_RG8,
    afxPixelFormat_RGB8,
    afxPixelFormat_RGBA8, // 0xAABBGGRR
    afxPixelFormat_ABGR8 = afxPixelFormat_RGBA8,

    afxPixelFormat_GR8,
    afxPixelFormat_BGR8,
    afxPixelFormat_BGRA8, // 0xAARRGGBB
    afxPixelFormat_ARGB8 = afxPixelFormat_BGRA8,

    afxPixelFormat_R32F,
    afxPixelFormat_RG32F,
    afxPixelFormat_RGB32F,
    afxPixelFormat_RGBA32F,

    afxPixelFormat_RGB8_SRGB,
    afxPixelFormat_RGBA8_SRGB, // A is not color, it is always linear.
    afxPixelFormat_RGB9E5,

    afxPixelFormat_RGBA4,
    afxPixelFormat_RGB5A1,
    afxPixelFormat_R5G6B5,
    afxPixelFormat_RGB10A2,

    afxPixelFormat_S8,
    afxPixelFormat_D16,
    afxPixelFormat_D24,
    afxPixelFormat_D32,
    afxPixelFormat_D32F,
    afxPixelFormat_D24S8,
    afxPixelFormat_D32FS8,

    afxPixelFormat_RGB_DXT1,
    afxPixelFormat_RGBA_DXT1,
    afxPixelFormat_RGBA_DXT3,
    afxPixelFormat_RGBA_DXT5,

	afxPixelFormat_TOTAL
} afxPixelFormat;

typedef enum afxPixelFlag
{
    AFX_PIXEL_FLAG_REAL         = AfxGetBitOffset(0),
    AFX_PIXEL_FLAG_DEPTH        = AfxGetBitOffset(1),
    AFX_PIXEL_FLAG_SIGNED       = AfxGetBitOffset(2),
    AFX_PIXEL_FLAG_NORMALIZED   = AfxGetBitOffset(3),
    AFX_PIXEL_FLAG_COMPRESSED   = AfxGetBitOffset(4),
    AFX_PIXEL_FLAG_INDEXED      = AfxGetBitOffset(5),
    AFX_PIXEL_FLAG_LUMINANCE    = AfxGetBitOffset(6),
} afxPixelFlag;

AFX_DEFINE_STRUCT(afxPixelFormatSpecification)
{
    afxNat  stride; // bytes per pixel
    afxNat  bpp[4];
    afxNat  shift[4];
    afxNat  swizzle[4];
};

AFX_DEFINE_STRUCT(afxColorSwizzling)
{
    afxColorSwizzle r, g, b, a;
};

AFX_DEFINE_STRUCT(afxPixelLayout)
{
    afxNat  bpp; // bis per pixel
    afxNat  bpc[4]; // bits per component. { R/D/S/I, RG/DS, RGB, RGBA/RGBE }
    afxNat  shift[4]; // bit shift per component
    afxBool real;
    afxBool depth;
    afxBool stencil;
    afxBool block;
    afxBool unused2;
    afxBool unused3;
    afxBool unused4;
    //afxBool unused5;
};

AVXINL void AfxDescribePixelFormat(afxPixelFormat fmt, afxPixelLayout *layout);

AVXINL afxBool AfxPixelFormatIsDepth(afxPixelFormat fmt);
AVXINL afxBool AfxPixelFormatIsStencil(afxPixelFormat fmt);
AVXINL afxBool AfxPixelFormatIsCombinedDepthStencil(afxPixelFormat fmt);
AVXINL afxBool AfxPixelFormatIsReal(afxPixelFormat fmt);
AVXINL afxBool AfxPixelFormatIsSrgb(afxPixelFormat fmt);
AVXINL afxBool AfxPixelFormatIsCompressed(afxPixelFormat fmt);



#define AfxPackColor(x_, y_, z_, w_) (afxColor const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxPackColorConst(x_, y_, z_, w_) (afxColor const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }

AVX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING;

AVXINL void AfxSetColor(afxColor color, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxSetV4d(color, r, g, b, a);
}

AVXINL void AfxCopyColor(afxColor color, afxColor const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxAssert(in);
    AfxCopyV4d(color, in);
}

AVXINL void AfxResetColor(afxColor color)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxResetV4d(color);
}

// ((RwUInt32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

#endif//AFX_COLOR_H
