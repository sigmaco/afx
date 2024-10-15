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

#include "qwadro/inc/draw/afxDrawDefs.h"

typedef enum avxFormat
/// Available pixel formats.
/// Despite being read as vectors, pixels are stored as packs. Do not see channels as vector units.
/// non-specified types are expected to be natural number (unsigned int). R = real number (float). I = integer (int)
{
    avxFormat_UNDEFINED, // nil; used to autoselection when viable.

    avxFormat_RG4un = 1, // pack 8
    avxFormat_RG4 = avxFormat_RG4un,
    avxFormat_RGBA4un, // pack 16
    avxFormat_RGBA4 = avxFormat_RGBA4un,
    avxFormat_BGRA4un, // pack 16
    avxFormat_BGRA4 = avxFormat_BGRA4un,
    
    avxFormat_R5G6B5un, // pack 16
    avxFormat_R5G6B5 = avxFormat_R5G6B5un,
    avxFormat_B5G6R5un, // pack 16
    avxFormat_B5G6R5 = avxFormat_B5G6R5un,
    
    avxFormat_RGB5A1un, // pack 16
    avxFormat_RGB5A1 = avxFormat_RGB5A1un,
    avxFormat_BGR5A1un, // pack 16
    avxFormat_BGR5A1 = avxFormat_BGR5A1un,
    avxFormat_A1RGB5un, // pack 16
    avxFormat_A1RGB5 = avxFormat_A1RGB5un,

    // omitted color type is unsigned normalized (UN); omitted.
    // omitted float qualifier is signed.

    avxFormat_R8un = 9,
    avxFormat_R8 = avxFormat_R8un,
    avxFormat_R8sn,
    avxFormat_R8us,
    avxFormat_R8ss,
    avxFormat_R8u,
    avxFormat_R8i,
    avxFormat_R8_sRGB,
    
    avxFormat_RG8un = 16,
    avxFormat_RG8 = avxFormat_RG8un,
    avxFormat_RG8sn,
    avxFormat_RG8us,
    avxFormat_RG8ss,
    avxFormat_RG8u,
    avxFormat_RG8i,
    avxFormat_RG8_sRGB,

    avxFormat_RGB8un = 23,
    avxFormat_RGB8 = avxFormat_RGB8un,
    avxFormat_RGB8sn,
    avxFormat_RGB8us,
    avxFormat_RGB8ss,
    avxFormat_RGB8u,
    avxFormat_RGB8i,
    avxFormat_RGB8_sRGB,
    
    avxFormat_BGR8un,
    avxFormat_BGR8 = avxFormat_BGR8un,
    avxFormat_BGR8sn,
    avxFormat_BGR8us,
    avxFormat_BGR8ss,
    avxFormat_BGR8u,
    avxFormat_BGR8i,
    avxFormat_BGR8_sRGB,

    avxFormat_RGBA8un = 37,
    avxFormat_RGBA8 = avxFormat_RGBA8un,
    avxFormat_RGBA8sn,
    avxFormat_RGBA8us,
    avxFormat_RGBA8ss,
    avxFormat_RGBA8u,
    avxFormat_RGBA8i,
    avxFormat_RGBA8_sRGB,

    avxFormat_BGRA8un = 44,
    avxFormat_BGRA8 = avxFormat_BGRA8un,
    avxFormat_BGRA8sn,
    avxFormat_BGRA8us,
    avxFormat_BGRA8ss,
    avxFormat_BGRA8u,
    avxFormat_BGRA8i,
    avxFormat_BGRA8_sRGB,

    avxFormat_ABGR8un = 51, // pack 32
    avxFormat_ABGR8 = avxFormat_ABGR8un,
    avxFormat_ABGR8sn, // pack 32
    avxFormat_ABGR8us, // pack 32
    avxFormat_ABGR8ss, // pack 32
    avxFormat_ABGR8u, // pack 32
    avxFormat_ABGR8i, // pack 32
    avxFormat_ABGR8_sRGB, // pack 32

    avxFormat_A2RGB10un = 58, // pack 32
    avxFormat_A2RGB10 = avxFormat_A2RGB10un,
    avxFormat_A2RGB10sn, // pack 32
    avxFormat_A2RGB10us, // pack 32
    avxFormat_A2RGB10ss, // pack 32
    avxFormat_A2RGB10u, // pack 32
    avxFormat_A2RGB10i, // pack 32
    
    avxFormat_A2BGR10un = 64, // pack 32
    avxFormat_A2BGR10 = avxFormat_A2BGR10un,
    avxFormat_A2BGR10sn, // pack 32
    avxFormat_A2BGR10us, // pack 32
    avxFormat_A2BGR10ss, // pack 32
    avxFormat_A2BGR10u, // pack 32
    avxFormat_A2BGR10i, // pack 32

    avxFormat_R16un = 70,
    avxFormat_R16 = avxFormat_R16un,
    avxFormat_R16sn,
    avxFormat_R16us,
    avxFormat_R16ss,
    avxFormat_R16u,
    avxFormat_R16i,    
    avxFormat_R16f,
    
    avxFormat_RG16un = 77,
    avxFormat_RG16 = avxFormat_RG16un,
    avxFormat_RG16sn,
    avxFormat_RG16us,
    avxFormat_RG16ss,
    avxFormat_RG16u,
    avxFormat_RG16i,
    avxFormat_RG16f,
        
    avxFormat_RGB16un = 84,
    avxFormat_RGB16 = avxFormat_RGB16un,
    avxFormat_RGB16sn,
    avxFormat_RGB16us,
    avxFormat_RGB16ss,
    avxFormat_RGB16u,
    avxFormat_RGB16i,
    avxFormat_RGB16f,
    
    avxFormat_RGBA16un = 91,
    avxFormat_RGBA16 = avxFormat_RGBA16un,
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
    avxFormat_B10GR11 = avxFormat_B10GR11uf,
    avxFormat_E5BGR9uf, // unsigned float --- pack 32
    avxFormat_E5BGR9 = avxFormat_E5BGR9uf,

    // omitted stencil type is unsigned int (U).
    // omitted depth type is unsigned normalized (UN), when int, or signed float (SF), when float.

    avxFormat_D16un = 124, // Dun
    avxFormat_D16 = avxFormat_D16un,
    avxFormat_X8D24un, // Dun --- pack 32
    avxFormat_X8D24 = avxFormat_X8D24un,
    avxFormat_D24 = avxFormat_X8D24,
    avxFormat_D32f, // Df
    avxFormat_D32 = avxFormat_D32f,
    avxFormat_S8u, // Su
    avxFormat_S8 = avxFormat_S8u,
    avxFormat_D16unS8u, // Dun, Su
    avxFormat_D16S8 = avxFormat_D16unS8u,
    avxFormat_D24unS8u, // Dun
    avxFormat_D24S8 = avxFormat_D24unS8u,
    avxFormat_D32fS8u, // Df, Su
    avxFormat_D32S8 = avxFormat_D32fS8u,

    // S3TC/DXT1 (BC1), S3TC/DXT3 (BC2) and S3TC/DXT5 (BC3)
    avxFormat_DXT1un = 131,
    avxFormat_DXT1 = avxFormat_DXT1un,
    avxFormat_DXT1_sRGB,
    avxFormat_DXT1Aun,
    avxFormat_DXT1A = avxFormat_DXT1Aun,
    avxFormat_DXT1A_sRGB,
    avxFormat_DXT3un,
    avxFormat_DXT3 = avxFormat_DXT3un,
    avxFormat_DXT3_sRGB,
    avxFormat_DXT5un,
    avxFormat_DXT5 = avxFormat_DXT5un,
    avxFormat_DXT5_sRGB,
    avxFormat_RGTC1un, // RGTC1 (aka BC4 or ATI1N)
    avxFormat_RGTC1 = avxFormat_RGTC1un,
    avxFormat_RGTC1sn, // RGTC1 (aka BC4 or ATI1N)
    avxFormat_RGTC2un, // RGTC2 (aka BC5 or ATI2N)
    avxFormat_RGTC2 = avxFormat_RGTC2un,
    avxFormat_RGTC2sn, // RGTC2 (aka BC5 or ATI2N)
    avxFormat_BPTCuf, // BPTC (aka BC6H)
    avxFormat_BPTCf, // BPTC (aka BC6H)
    avxFormat_BPTC7un, // BPTC (aka BC7)
    avxFormat_BPTC7 = avxFormat_BPTC7un,
    avxFormat_BPTC7_sRGB, // BPTC (aka BC7)
    
    avxFormat_ETC2_RGB8un = 147,
    avxFormat_ETC2_RGB8 = avxFormat_ETC2_RGB8un,
    avxFormat_ETC2_RGB8_sRGB,
    avxFormat_ETC2_RGB8A1un,
    avxFormat_ETC2_RGB8A1 = avxFormat_ETC2_RGB8A1un,
    avxFormat_ETC2_RGB8A1_sRGB,
    avxFormat_ETC2_RGBA8un,
    avxFormat_ETC2_RGBA8 = avxFormat_ETC2_RGBA8un,
    avxFormat_ETC2_RGBA8_sRGB,

    avxFormat_EAC_R11un = 153,
    avxFormat_EAC_R11 = avxFormat_EAC_R11un,
    avxFormat_EAC_R11sn,
    avxFormat_EAC_R11G11un,
    avxFormat_EAC_R11G11 = avxFormat_EAC_R11G11un,
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

typedef enum avxFormatComponent
{
    avxFormatComponent_UNK,
    avxFormatComponent_UNUSED,
    avxFormatComponent_UNORM,
    avxFormatComponent_SNORM,
    avxFormatComponent_UINT,
    avxFormatComponent_SINT,
    avxFormatComponent_FLOAT,
    avxFormatComponent_SSCALED,
    avxFormatComponent_USCALED,
    avxFormatComponent_SFIXED,
} avxFormatComponent;

typedef enum avxFormatFlag
{
    avxFormatFlag_DRAW       = AFX_BIT(0),
    avxFormatFlag_DEPTH      = AFX_BIT(1),
    avxFormatFlag_STENCIL    = AFX_BIT(2),

    avxFormatFlag_sRGB       = AFX_BIT(3),
    avxFormatFlag_COMPRESSED = AFX_BIT(4),
    avxFormatFlag_SUBSAMPLED = AFX_BIT(5),
    avxFormatFlag_LUMINANCE  = AFX_BIT(6),
    avxFormatFlag_NORMALIZED = AFX_BIT(7),
} avxFormatFlags;

AFX_DEFINE_STRUCT(avxFormatDescription)
/// Pixel format descriptor
{
    afxNat32            bpp; /// bits per pixel
    afxNat32            stride; /// bytes per pixel
    afxNat32            compCnt; /// number of components
    avxFormatComponent  type[4];
    afxNat32            swizzle[4]; /// swizzle per component
    afxNat32            defaults[4]; /// Default values for missing components.
    afxBool             isNormalized[4];
    afxReal             tof[4]; /// To float scale factor
    afxNat32            bpc[4]; /// bits per component
    afxNat32            bcWh[2];
    avxFormatFlags      flags;
};

AVXINL void     AfxDescribePixelFormat(avxFormat fmt, avxFormatDescription* pfd);

AVXINL afxNat   AfxGetBpp(avxFormat fmt);

AVXINL afxBool  AfxIsPixelFormatDepth(avxFormat fmt);
AVXINL afxBool  AfxIsPixelFormatStencil(avxFormat fmt);
AVXINL afxBool  AfxIsPixelFormatCombinedDepthStencil(avxFormat fmt);
AVXINL afxBool  AfxIsSrgb(avxFormat fmt);
AVXINL afxBool  AfxIsPixelFormatCompressed(avxFormat fmt);



#endif//AFX_PIXEL_H
