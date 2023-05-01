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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
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

    AFX_PIXEL_FMT_R8 = 1,
    AFX_PIXEL_FMT_RG8,
    AFX_PIXEL_FMT_RGB8,
    AFX_PIXEL_FMT_RGBA8,

    AFX_PIXEL_FMT_GR8,
    AFX_PIXEL_FMT_BGR8,
    AFX_PIXEL_FMT_BGRA8,

    AFX_PIXEL_FMT_R32F,
    AFX_PIXEL_FMT_RG32F,
    AFX_PIXEL_FMT_RGB32F,
    AFX_PIXEL_FMT_RGBA32F,

    AFX_PIXEL_FMT_RGB8_SRGB,
    AFX_PIXEL_FMT_RGBA8_SRGB,
    AFX_PIXEL_FMT_RGB9E5,

    AFX_PIXEL_FMT_RGBA4,
    AFX_PIXEL_FMT_RGB5A1,
    AFX_PIXEL_FMT_R5G6B5,
    AFX_PIXEL_FMT_RGB10A2,

    AFX_PIXEL_FMT_S8,
    AFX_PIXEL_FMT_D16,
    AFX_PIXEL_FMT_D24,
    AFX_PIXEL_FMT_D32,
    AFX_PIXEL_FMT_D32F,
    AFX_PIXEL_FMT_D24S8,
    AFX_PIXEL_FMT_D32FS8,

    AFX_PIXEL_FMT_RGB_DXT1,
    AFX_PIXEL_FMT_RGBA_DXT1,
    AFX_PIXEL_FMT_RGBA_DXT3,
    AFX_PIXEL_FMT_RGBA_DXT5,

	AFX_PIXEL_FMT_TOTAL
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
    afxNat  stride;
    afxNat  chCnt;
    afxNat  bpp[4];
    afxNat  shift[4];
    afxNat  swizzle[4];
};

AFX_DEFINE_STRUCT(afxColorSwizzling)
{
    afxColorSwizzle r, g, b, a;
};

AFX_DEFINE_STRUCT(afxColorLayout)
{
    afxNat stride; // sizeof, in bits. Must be divived by AFX_BYTE_SIZE
    afxNat size[4]; // # of bits
    afxNat shift[4];
};

#define AfxPackColor(x_, y_, z_, w_) (afxColor){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }
#define AfxPackColorConst(x_, y_, z_, w_) (afxColor const){ (afxReal)x_, (afxReal)y_, (afxReal)z_, (afxReal)w_ }

AFX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING;

//AFX const int sizeOfPixel[];

AFXINL afxNat AfxPixelFormatGetBpp(afxPixelFormat pf);

AFXINL afxReal* AfxColorSet(afxColor color, afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = NIL;
    AfxAssert(color);
    return AfxV4dSet(color, r, g, b, a);
}

AFXINL afxReal* AfxColorCopy(afxColor color, afxColor const in)
{
    afxError err = NIL;
    AfxAssert(color);
    AfxAssert(in);
    return AfxV4dCopy(color, in);
}

AFXINL afxReal* AfxColorReset(afxColor color)
{
    afxError err = NIL;
    AfxAssert(color);
    return AfxV4dReset(color);
}

AFXINL afxReal* AfxColorCopyRaw(afxColor color, afxColorLayout const* layout, void* raw)
{
    afxError err = NIL;
    AfxAssert(color);
    AfxAssert(layout);
    AfxAssert(raw);
    (void)raw;
    (void)layout;

    afxNat8 rgba8[4] =
    {
        (afxNat8)((color[0] * AfxScalar(255)) + AfxScalar(0.5)),
        (afxNat8)((color[1] * AfxScalar(255)) + AfxScalar(0.5)),
        (afxNat8)((color[2] * AfxScalar(255)) + AfxScalar(0.5)),
        (afxNat8)((color[3] * AfxScalar(255)) + AfxScalar(0.5)),
    };
    (void)rgba8;
    return color;
}

// ((RwUInt32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

#if 0
AFX_COLOR_P8 = 1, // palettized/indexed color

// THE RGBA COLOR SPACE

// 8 bits
AFX_COLOR_R8,

// 15-bit high color
AFX_COLOR_RGB5,
AFX_COLOR_BGR5,

// 16-bit high color
AFX_COLOR_RGBA4,
AFX_COLOR_BGRA4,

AFX_COLOR_RGB5A1,
AFX_COLOR_BGR5A1,

AFX_COLOR_R5G6B5,
AFX_COLOR_B5G6R5,

// 24-bit true color
AFX_COLOR_RGB8,
AFX_COLOR_BGR8,

// 32-bit true color
AFX_COLOR_RGBA8,
AFX_COLOR_BGRA8,
AFX_COLOR_ARGB8,
#endif//0

// 8 bits
AFX afxColorLayout const* AFX_R8;

// 15 bits
AFX afxColorLayout const* AFX_RGB5;
AFX afxColorLayout const* AFX_BGR5;

// 16 bits
AFX afxColorLayout const* AFX_RGBA4;
AFX afxColorLayout const* AFX_BGRA4;

AFX afxColorLayout const* AFX_RGB5A1;
AFX afxColorLayout const* AFX_BGR5A1;

AFX afxColorLayout const* AFX_R5G6B5;
AFX afxColorLayout const* AFX_B5G6R5;

// 24 bits
AFX afxColorLayout const* AFX_RGB8;
AFX afxColorLayout const* AFX_BGR8;

// 32 bits
AFX afxColorLayout const* AFX_RGBA8;
AFX afxColorLayout const* AFX_BGRA8;
AFX afxColorLayout const* AFX_ARGB8;

#endif//AFX_COLOR_H