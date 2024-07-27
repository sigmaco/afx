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

typedef enum avxFormat
/// Available pixel formats.
/// Despite being read as vectors, pixels are stored as packs. Do not see channels as vector units.
{
    avxFormat_UNDEFINED,

    avxFormat_RG4 = 1, // pack 8
    avxFormat_RGBA4, // pack 16
    avxFormat_BGRA4, // pack 16
    avxFormat_R5G6B5, // pack 16
    avxFormat_B5G6R5, // pack 16
    avxFormat_RGB5A1, // pack 16
    avxFormat_BGR5A1, // pack 16
    avxFormat_A1RGB5, // pack 16

    // omitted color type is unsigned normalized (UN); omitted.
    // omitted float qualifier is signed.

    avxFormat_R8 = 9,
    avxFormat_R8sn,
    avxFormat_R8us,
    avxFormat_R8ss,
    avxFormat_R8u,
    avxFormat_R8i,
    avxFormat_R8_sRGB,
    
    avxFormat_RG8 = 16,
    avxFormat_RG8sn,
    avxFormat_RG8us,
    avxFormat_RG8ss,
    avxFormat_RG8u,
    avxFormat_RG8i,
    avxFormat_RG8_sRGB,

    avxFormat_RGB8 = 23,
    avxFormat_RGB8sn,
    avxFormat_RGB8us,
    avxFormat_RGB8ss,
    avxFormat_RGB8u,
    avxFormat_RGB8i,
    avxFormat_RGB8_sRGB,
    
    avxFormat_BGR8,
    avxFormat_BGR8sn,
    avxFormat_BGR8us,
    avxFormat_BGR8ss,
    avxFormat_BGR8u,
    avxFormat_BGR8i,
    avxFormat_BGR8_sRGB,

    avxFormat_RGBA8 = 37,
    avxFormat_RGBA8sn,
    avxFormat_RGBA8us,
    avxFormat_RGBA8ss,
    avxFormat_RGBA8u,
    avxFormat_RGBA8i,
    avxFormat_RGBA8_sRGB,

    avxFormat_BGRA8 = 44,
    avxFormat_BGRA8sn,
    avxFormat_BGRA8us,
    avxFormat_BGRA8ss,
    avxFormat_BGRA8u,
    avxFormat_BGRA8i,
    avxFormat_BGRA8_sRGB,

    avxFormat_ABGR8 = 51, // pack 32
    avxFormat_ABGR8sn, // pack 32
    avxFormat_ABGR8us, // pack 32
    avxFormat_ABGR8ss, // pack 32
    avxFormat_ABGR8u, // pack 32
    avxFormat_ABGR8i, // pack 32
    avxFormat_ABGR8_sRGB, // pack 32

    avxFormat_A2RGB10 = 58, // pack 32
    avxFormat_A2RGB10sn, // pack 32
    avxFormat_A2RGB10us, // pack 32
    avxFormat_A2RGB10ss, // pack 32
    avxFormat_A2RGB10u, // pack 32
    avxFormat_A2RGB10i, // pack 32
    
    avxFormat_A2BGR10 = 64, // pack 32
    avxFormat_A2BGR10sn, // pack 32
    avxFormat_A2BGR10us, // pack 32
    avxFormat_A2BGR10ss, // pack 32
    avxFormat_A2BGR10u, // pack 32
    avxFormat_A2BGR10i, // pack 32

    avxFormat_R16 = 70,
    avxFormat_R16sn,
    avxFormat_R16us,
    avxFormat_R16ss,
    avxFormat_R16u,
    avxFormat_R16i,    
    avxFormat_R16f,
    
    avxFormat_RG16 = 77,
    avxFormat_RG16sn,
    avxFormat_RG16us,
    avxFormat_RG16ss,
    avxFormat_RG16u,
    avxFormat_RG16i,
    avxFormat_RG16f,
        
    avxFormat_RGB16 = 84,
    avxFormat_RGB16sn,
    avxFormat_RGB16us,
    avxFormat_RGB16ss,
    avxFormat_RGB16u,
    avxFormat_RGB16i,
    avxFormat_RGB16f,
    
    avxFormat_RGBA16 = 91,
    avxFormat_RGBA16sn,
    avxFormat_RGBA16us,
    avxFormat_RGBA16ss,
    avxFormat_RGBA16u,
    avxFormat_RGBA16i,
    avxFormat_RGBA16f,

    avxFormat_R32u = 98,
    avxFormat_R32i,
    avxFormat_R32f,
    
    avxFormat_RG32u = 101,
    avxFormat_RG32i,
    avxFormat_RG32f,
    
    avxFormat_RGB32u = 104,
    avxFormat_RGB32i,
    avxFormat_RGB32f,
    
    avxFormat_RGBA32u = 107,
    avxFormat_RGBA32i,
    avxFormat_RGBA32f,
    
    avxFormat_R64u = 110,
    avxFormat_R64i,
    avxFormat_R64f,
    
    avxFormat_RG64u = 113,
    avxFormat_RG64i,
    avxFormat_RG64f,
    
    avxFormat_RGB64u = 116,
    avxFormat_RGB64i,
    avxFormat_RGB64f,

    avxFormat_RGBA64u = 119,
    avxFormat_RGBA64i,
    avxFormat_RGBA64f,

    avxFormat_B10GR11uf = 122, // unsigned float --- pack 32
    avxFormat_E5BGR9uf, // unsigned float --- pack 32

    // omitted stencil type is unsigned int (U).
    // omitted depth type is unsigned normalized (UN), when int, or signed float (SF), when float.

    avxFormat_D16 = 124, // Dun
    avxFormat_X8D24, // Dun --- pack 32
    avxFormat_D32f, // Df
    avxFormat_S8, // Su
    avxFormat_D16S8, // Dun, Su
    avxFormat_D24S8, // Dun
    avxFormat_D32fS8, // Df, Su

    // S3TC/DXT1 (BC1), S3TC/DXT3 (BC2) and S3TC/DXT5 (BC3)
    avxFormat_DXT1 = 131,
    avxFormat_DXT1_sRGB,
    avxFormat_DXT1A,
    avxFormat_DXT1A_sRGB,
    avxFormat_DXT3,
    avxFormat_DXT3_sRGB,
    avxFormat_DXT5,
    avxFormat_DXT5_sRGB,
    avxFormat_RGTC1, // RGTC1 (aka BC4 or ATI1N)
    avxFormat_RGTC1sn, // RGTC1 (aka BC4 or ATI1N)
    avxFormat_RGTC2, // RGTC2 (aka BC5 or ATI2N)
    avxFormat_RGTC2sn, // RGTC2 (aka BC5 or ATI2N)
    avxFormat_BPTCuf, // BPTC (aka BC6H)
    avxFormat_BPTCf, // BPTC (aka BC6H)
    avxFormat_BPTC7, // BPTC (aka BC7)
    avxFormat_BPTC7_sRGB, // BPTC (aka BC7)
    
    avxFormat_ETC2_RGB8 = 147,
    avxFormat_ETC2_RGB8_sRGB,
    avxFormat_ETC2_RGB8A1,
    avxFormat_ETC2_RGB8A1_sRGB,
    avxFormat_ETC2_RGBA8,
    avxFormat_ETC2_RGBA8_sRGB,

    avxFormat_EAC_R11 = 153,
    avxFormat_EAC_R11sn,
    avxFormat_EAC_R11G11,
    avxFormat_EAC_R11G11sn,

    avxFormat_ASTC_4x4 = 157,
    avxFormat_ASTC_4x4_sRGB,
    avxFormat_ASTC_5x4,
    avxFormat_ASTC_5x4_sRGB,
    avxFormat_ASTC_5x5,
    avxFormat_ASTC_5x5_sRGB,
    avxFormat_ASTC_6x5,
    avxFormat_ASTC_6x5_sRGB,
    avxFormat_ASTC_6x6,
    avxFormat_ASTC_6x6_sRGB,
    avxFormat_ASTC_8x5,
    avxFormat_ASTC_8x5_sRGB,
    avxFormat_ASTC_8x6,
    avxFormat_ASTC_8x6_sRGB,
    avxFormat_ASTC_8x8,
    avxFormat_ASTC_8x8_sRGB,
    avxFormat_ASTC_10x5,
    avxFormat_ASTC_10x5_sRGB,
    avxFormat_ASTC_10x6,
    avxFormat_ASTC_10x6_sRGB,
    avxFormat_ASTC_10x8,
    avxFormat_ASTC_10x8_sRGB,
    avxFormat_ASTC_10x10,
    avxFormat_ASTC_10x10_sRGB,
    avxFormat_ASTC_12x10,
    avxFormat_ASTC_12x10_sRGB,
    avxFormat_ASTC_12x12,
    avxFormat_ASTC_12x12_sRGB,

    avxFormat_TOTAL
} avxFormat;

typedef enum afxPixelFormat
/// Available pixel formats.
/// Despite being read as vectors, pixels are stored as packs. Do not see channels as vector units.
{
    // non-specified types are expected to be natural number (unsigned int). R = real number (float). I = integer (int)

    afxPixelFormat_R8 = 1,
    afxPixelFormat_RG8,
    afxPixelFormat_RGB8,
    afxPixelFormat_RGBA8, // AAAAAAAA BBBBBBBB GGGGGGGG RRRRRRRR
    afxPixelFormat_ABGR8 = afxPixelFormat_RGBA8, // [ R, G, B, A ]

    afxPixelFormat_GR8,
    afxPixelFormat_BGR8,
    afxPixelFormat_BGRA8, // AAAAAAAA RRRRRRRR GGGGGGGG BBBBBBBB
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

    afxPixelFormat_DXT1,
    afxPixelFormat_DXT1A,
    afxPixelFormat_DXT3,
    afxPixelFormat_DXT5,

    afxPixelFormat_TOTAL
} afxPixelFormat;

typedef enum afxPixelFlag
{
    AFX_PIXEL_FLAG_REAL = AFX_BIT(0),
    AFX_PIXEL_FLAG_DEPTH = AFX_BIT(1),
    AFX_PIXEL_FLAG_SIGNED = AFX_BIT(2),
    AFX_PIXEL_FLAG_NORMALIZED = AFX_BIT(3),
    AFX_PIXEL_FLAG_COMPRESSED = AFX_BIT(4),
    AFX_PIXEL_FLAG_INDEXED = AFX_BIT(5),
    AFX_PIXEL_FLAG_LUMINANCE = AFX_BIT(6),
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

AVXINL void     AfxDescribePixelFormat(afxPixelFormat fmt, afxPixelLayout *layout);

AVXINL afxNat   AfxGetBpp(afxPixelFormat fmt);

AVXINL afxBool  AfxPixelFormatIsDepth(afxPixelFormat fmt);
AVXINL afxBool  AfxPixelFormatIsStencil(afxPixelFormat fmt);
AVXINL afxBool  AfxPixelFormatIsCombinedDepthStencil(afxPixelFormat fmt);
AVXINL afxBool  AfxPixelFormatIsReal(afxPixelFormat fmt);
AVXINL afxBool  AfxPixelFormatIsSrgb(afxPixelFormat fmt);
AVXINL afxBool  AfxPixelFormatIsCompressed(afxPixelFormat fmt);



#endif//AFX_PIXEL_H
