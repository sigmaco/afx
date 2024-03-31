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

#ifndef AFX_PIXEL_H
#define AFX_PIXEL_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum afxPixelElementType
{
    afxPixelElementType_UNKNOWN,
    afxPixelElementType_UNUSED,
    afxPixelElementType_UNORM,
    afxPixelElementType_SNORM,
    afxPixelElementType_UINT,
    afxPixelElementType_SINT,
    afxPixelElementType_FLOAT,
    afxPixelElementType_SSCALED,
    afxPixelElementType_USCALED,
    afxPixelElementType_SFIXED,
} afxPixelElementType;

typedef enum afxPixelFormat
{
    // non-specified types are expected to be natural number (unsigned int). R = real number (float). I = integer (int)

    afxPixelFormat_R8 = 1,
    afxPixelFormat_RG8,
    afxPixelFormat_RGB8,
    afxPixelFormat_RGBA8, // 0xAABBGGRR
    afxPixelFormat_ABGR8 = afxPixelFormat_RGBA8, // [ R, G, B, A ]

    afxPixelFormat_GR8,
    afxPixelFormat_BGR8,
    afxPixelFormat_BGRA8, // 0xAARRGGBB
    afxPixelFormat_ARGB8 = afxPixelFormat_BGRA8, // [ B, G, R, A ]

    afxPixelFormat_R32F,
    afxPixelFormat_RG32F,
    afxPixelFormat_RGB32F,
    afxPixelFormat_RGBA32F,

    afxPixelFormat_RGB8_SRGB,
    afxPixelFormat_RGBA8_SRGB, // A is not color, so it is always linear.
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
    AFX_PIXEL_FLAG_REAL = AFX_BIT_OFFSET(0),
    AFX_PIXEL_FLAG_DEPTH = AFX_BIT_OFFSET(1),
    AFX_PIXEL_FLAG_SIGNED = AFX_BIT_OFFSET(2),
    AFX_PIXEL_FLAG_NORMALIZED = AFX_BIT_OFFSET(3),
    AFX_PIXEL_FLAG_COMPRESSED = AFX_BIT_OFFSET(4),
    AFX_PIXEL_FLAG_INDEXED = AFX_BIT_OFFSET(5),
    AFX_PIXEL_FLAG_LUMINANCE = AFX_BIT_OFFSET(6),
} afxPixelFlag;

AFX_DEFINE_STRUCT(afxPixelFormatSpecification)
{
    afxNat  stride; // bytes per pixel
    afxNat  bpp[4];
    afxNat  shift[4];
    afxNat  swizzle[4];
};

AFX_DEFINE_STRUCT(afxPfd)
/// Pixel format descriptor
{
    afxNat32            bpp; /// bits per pixel
    afxNat32            bypp; /// bytes per pixel
    afxNat32            compCnt; /// number of components
    afxPixelElementType type[4];
    afxNat32            defaults[4]; /// Default values for missing components.
    afxNat32            swizzle[4]; /// swizzle per component
    afxNat32            bpc[4]; /// bits per component
    afxBool             isNormalized[4];
    afxReal             tof[4]; /// To float scale factor
    afxNat32            bcWh[2];
    afxBool             isSrgb;
    afxBool             isBc;
    afxBool             isSubsampled;
    afxBool             isLuminance;    
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



#endif//AFX_PIXEL_H
