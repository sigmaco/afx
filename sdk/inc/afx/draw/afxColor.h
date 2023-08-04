/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_COLOR_H
#define AFX_COLOR_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/math/afxMathDefs.h"

typedef afxV4d      afxColor;
typedef afxNat32    afxRgb8, afxBgr8;
typedef afxNat32    afxRgba8;
typedef afxByte     afxBgra8;
typedef afxByte     afxArgb8;

typedef enum afxColorSwizzle
{
    AFX_COLOR_SIZZLE_R,
    AFX_COLOR_SIZZLE_G,
    AFX_COLOR_SIZZLE_B,
    AFX_COLOR_SIZZLE_A,
    AFX_COLOR_SIZZLE_ZERO,
    AFX_COLOR_SIZZLE_ONE,
} afxColorSwizzle;

typedef enum afxPixelFormat
{
    // non-specified types are expected to be natural number (unsigned int). R = real number (float). I = integer (int)

    AFX_PFD_R8 = 1,
    AFX_PFD_RG8,
    AFX_PFD_RGB8,
    AFX_PFD_RGBA8, // 0xAABBGGRR
    AFX_PFD_ABGR8 = AFX_PFD_RGBA8,

    AFX_PFD_GR8,
    AFX_PFD_BGR8,
    AFX_PFD_BGRA8, // 0xAARRGGBB
    AFX_PFD_ARGB8 = AFX_PFD_BGRA8,

    AFX_PFD_R32F,
    AFX_PFD_RG32F,
    AFX_PFD_RGB32F,
    AFX_PFD_RGBA32F,

    AFX_PFD_RGB8_SRGB,
    AFX_PFD_RGBA8_SRGB, // A is not color, it is always linear.
    AFX_PFD_RGB9E5,

    AFX_PFD_RGBA4,
    AFX_PFD_RGB5A1,
    AFX_PFD_R5G6B5,
    AFX_PFD_RGB10A2,

    AFX_PFD_S8,
    AFX_PFD_D16,
    AFX_PFD_D24,
    AFX_PFD_D32,
    AFX_PFD_D32F,
    AFX_PFD_D24S8,
    AFX_PFD_D32FS8,

    AFX_PFD_RGB_DXT1,
    AFX_PFD_RGBA_DXT1,
    AFX_PFD_RGBA_DXT3,
    AFX_PFD_RGBA_DXT5,

	AFX_PFD_TOTAL
} afxPixelFormat;

typedef enum afxPixelFlag
{
    AFX_PIXEL_FLAG_REAL         = AFX_FLAG(0),
    AFX_PIXEL_FLAG_DEPTH        = AFX_FLAG(1),
    AFX_PIXEL_FLAG_SIGNED       = AFX_FLAG(2),
    AFX_PIXEL_FLAG_NORMALIZED   = AFX_FLAG(3),
    AFX_PIXEL_FLAG_COMPRESSED   = AFX_FLAG(4),
    AFX_PIXEL_FLAG_INDEXED      = AFX_FLAG(5),
    AFX_PIXEL_FLAG_LUMINANCE    = AFX_FLAG(6),
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
    afxBool unused5;
};

AFXINL void AfxDescribePixelFormat(afxPixelFormat fmt, afxPixelLayout *layout);

AFXINL afxBool AfxPixelFormatIsDepth(afxPixelFormat fmt);
AFXINL afxBool AfxPixelFormatIsStencil(afxPixelFormat fmt);
AFXINL afxBool AfxPixelFormatIsCombinedDepthStencil(afxPixelFormat fmt);
AFXINL afxBool AfxPixelFormatIsReal(afxPixelFormat fmt);
AFXINL afxBool AfxPixelFormatIsSrgb(afxPixelFormat fmt);
AFXINL afxBool AfxPixelFormatIsCompressed(afxPixelFormat fmt);



#define AfxPackColor(x_, y_, z_, w_) (afxColor){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxPackColorConst(x_, y_, z_, w_) (afxColor const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }

AFX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING;

AFXINL void AfxSetColor(afxColor color, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxV4dSet(color, r, g, b, a);
}

AFXINL void AfxCopyColor(afxColor color, afxColor const in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxAssert(in);
    AfxV4dCopy(color, in);
}

AFXINL void AfxResetColor(afxColor color)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(color);
    AfxV4dMakeIdentity(color);
}

// ((RwUInt32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

#endif//AFX_COLOR_H