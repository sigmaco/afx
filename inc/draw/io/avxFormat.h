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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO FIXED FUNCTION VIDEO DATA FORMAT                                  //
//////////////////////////////////////////////////////////////////////////////

#ifndef AVX_FORMAT_H
#define AVX_FORMAT_H

#include "qwadro/inc/draw/afxDrawDefs.h"

typedef enum avxFormat
// Available pixel formats.
// Despite being read as vectors, pixels are stored as packs. Do not see channels as vector units.
// non-specified types are expected to be natural number (unsigned int). 
// The F suffix means signed float.
// The UF suffix means unsigned float.
// The I suffix means signed integer.
// The U suffix means unsigned integer.
// The UN suffix means unsigned normalized integer.
// The SN suffix means signed normalized integer.
// The US suffix means unsigned scaled integer.
// The SS suffix means signed scaled integer.
// The V suffix means sRGB (proper for video display).
{
    avxFormat_UNDEFINED, // nil; used to autoselection when viable.

    // 8-bit precise sRGB
    avxFormat_R8v, // sRGB
    avxFormat_RG8v, // sRGB
    avxFormat_RGB8v, // sRGB
    avxFormat_RGBA8v, // sRGB
    avxFormat_ABGR8v, // sRGB --- pack 32
    avxFormat_BGRA8v, // sRGB
    avxFormat_BGR8v, // sRGB

    // 8-bit precise unsigned normalized integer
    avxFormat_R8un,
    avxFormat_RG8un,
    avxFormat_RGB8un,
    avxFormat_RGBA8un,
    avxFormat_ABGR8un, // pack 32
    avxFormat_BGRA8un,
    avxFormat_BGR8un,

    // 8-bit precise signed normalized integer
    avxFormat_R8sn,
    avxFormat_RG8sn,
    avxFormat_RGB8sn,
    avxFormat_RGBA8sn,
    avxFormat_ABGR8sn, // pack 32
    avxFormat_BGRA8sn,
    avxFormat_BGR8sn,

    // 8-bit precise unsigned scaled integer
    avxFormat_R8us,
    avxFormat_RG8us,
    avxFormat_RGB8us,
    avxFormat_RGBA8us,
    avxFormat_ABGR8us, // pack 32
    avxFormat_BGRA8us,
    avxFormat_BGR8us,

    // 8-bit precise signed scaled integer
    avxFormat_R8ss,
    avxFormat_RG8ss,
    avxFormat_RGB8ss,
    avxFormat_RGBA8ss,
    avxFormat_ABGR8ss, // pack 32
    avxFormat_BGRA8ss,
    avxFormat_BGR8ss,

    // 8-bit precise unsigned integer
    avxFormat_R8u,
    avxFormat_RG8u,
    avxFormat_RGB8u,
    avxFormat_RGBA8u,
    avxFormat_ABGR8u, // pack 32
    avxFormat_BGRA8u,
    avxFormat_BGR8u,

    // 8-bit precise signed integer 
    avxFormat_R8i,
    avxFormat_RG8i,
    avxFormat_RGB8i,
    avxFormat_RGBA8i,
    avxFormat_ABGR8i, // pack 32
    avxFormat_BGRA8i,
    avxFormat_BGR8i,

    // 16-bit precise unsigned normalized integer
    avxFormat_R16un,
    avxFormat_RG16un,
    avxFormat_RGB16un,
    avxFormat_RGBA16un,

    // 16-bit precise signed normalized integer
    avxFormat_R16sn,
    avxFormat_RG16sn,
    avxFormat_RGB16sn,
    avxFormat_RGBA16sn,

    // 16-bit precise unsigned scaled integer
    avxFormat_R16us,
    avxFormat_RG16us,
    avxFormat_RGB16us,
    avxFormat_RGBA16us,

    // 16-bit precise signed scaled integer
    avxFormat_R16ss,
    avxFormat_RG16ss,
    avxFormat_RGB16ss,
    avxFormat_RGBA16ss,

    // 16-bit precise unsigned integer
    avxFormat_R16u,
    avxFormat_RG16u,
    avxFormat_RGB16u,
    avxFormat_RGBA16u,

    // 16-bit precise signed integer
    avxFormat_R16i,
    avxFormat_RG16i,
    avxFormat_RGB16i,
    avxFormat_RGBA16i,

    avxFormat_R16f,    
    avxFormat_RG16f,
    avxFormat_RGB16f,
    avxFormat_RGBA16f,

    // 32-bit precise unsigned integer
    avxFormat_R32u,
    avxFormat_RG32u,
    avxFormat_RGB32u,
    avxFormat_RGBA32u,
    
    // 32-bit precise signed integer
    avxFormat_R32i,
    avxFormat_RG32i,
    avxFormat_RGB32i,
    avxFormat_RGBA32i,
    
    // 32-bit precise floating point
    avxFormat_R32f,
    avxFormat_RG32f,
    avxFormat_RGB32f,
    avxFormat_RGBA32f,
    
    // 64-bit precise unsigned integer
    avxFormat_R64u,
    avxFormat_RG64u,
    avxFormat_RGB64u,
    avxFormat_RGBA64u,

    // 64-bit precise signed integer
    avxFormat_R64i,
    avxFormat_RG64i,
    avxFormat_RGB64i,
    avxFormat_RGBA64i,

    // 64-bit precise floating point
    avxFormat_R64f,
    avxFormat_RG64f,
    avxFormat_RGB64f,
    avxFormat_RGBA64f,





    // 4-bit precise unsigned normalized integer
    avxFormat_RG4un, // pack 8
    avxFormat_RGBA4un, // pack 16
    avxFormat_BGRA4un, // pack 16
    avxFormat_ARGB4un, // pack 16
    avxFormat_ABGR4un, // pack 16

    // 5/6-bit precise unsigned normalized integer
    avxFormat_R5G6B5un, // pack 16
    avxFormat_B5G6R5un, // pack 16

    // 5/1-bit precise unsigned normalized integer
    avxFormat_RGB5A1un, // pack 16
    avxFormat_A1RGB5un, // pack 16
    avxFormat_BGR5A1un, // pack 16

    // omitted color type is unsigned normalized (UN); omitted.
    // omitted float qualifier is signed.

    avxFormat_A2RGB10un, // pack 32
    avxFormat_A2BGR10un, // pack 32

    avxFormat_A2RGB10sn, // pack 32
    avxFormat_A2BGR10sn, // pack 32

    avxFormat_A2RGB10us, // pack 32
    avxFormat_A2BGR10us, // pack 32

    avxFormat_A2RGB10ss, // pack 32
    avxFormat_A2BGR10ss, // pack 32

    avxFormat_A2RGB10u, // pack 32
    avxFormat_A2BGR10u, // pack 32

    avxFormat_A2RGB10i, // pack 32
    avxFormat_A2BGR10i, // pack 32

    avxFormat_B10GR11uf, // unsigned float --- pack 32
    avxFormat_E5BGR9uf, // unsigned float --- pack 32

    // omitted stencil type is unsigned int (U).
    // omitted depth type is unsigned normalized (UN), when int, or signed float (SF), when float.

    avxFormat_S8u, // Su
    avxFormat_D16un, // Dun
    avxFormat_D16unS8u, // Dun, Su
    avxFormat_X8D24un, // Dun --- pack 32
    avxFormat_D24unS8u, // Dun
    avxFormat_D32f, // Df
    avxFormat_D32fS8u, // Df, Su

    // S3TC/DXT1 (BC1), S3TC/DXT3 (BC2) and S3TC/DXT5 (BC3)
    avxFormat_DXT1un, // RGB UN
    avxFormat_DXT1v, // RGB sRGB
    avxFormat_DXT1A, // RGBA UN
    avxFormat_DXT1Av, // RGBA sRGB
    avxFormat_DXT3un, // RGBA UN
    avxFormat_DXT3v, // RGBA sRGB
    avxFormat_DXT5un, // RGBA UN
    avxFormat_DXT5v, // RGBA sRGB
    avxFormat_RGTC1un, // RGTC1 (aka BC4 or ATI1N)
    avxFormat_RGTC1sn, // RGTC1 (aka BC4 or ATI1N)
    avxFormat_RGTC2un, // RGTC2 (aka BC5 or ATI2N)
    avxFormat_RGTC2sn, // RGTC2 (aka BC5 or ATI2N)
    avxFormat_BPTCuf, // BPTC (aka BC6H)
    avxFormat_BPTCf, // BPTC (aka BC6H)
    avxFormat_BPTCun, // BPTC (aka BC7)
    avxFormat_BPTCv, // BPTC (aka BC7)
    
    avxFormat_ETC2un, // RGB8 UN
    avxFormat_ETC2std, // RGB8 sRGB
    avxFormat_ETC2A1un, // RGB8A1 UN
    avxFormat_ETC2A1v, // RGB8A1 sRGB
    avxFormat_ETC2Aun, // RGBA8 UN
    avxFormat_ETC2Av, // RGBA8 sRGB

    avxFormat_EAC_R11un,
    avxFormat_EAC_R11sn,
    avxFormat_EAC_R11G11un,
    avxFormat_EAC_R11G11sn,

    avxFormat_ASTC4,
    avxFormat_ASTC4v, // sRGB
    avxFormat_ASTC5x4,
    avxFormat_ASTC5x4v, // sRGB
    avxFormat_ASTC5,
    avxFormat_ASTC5v, // sRGB
    avxFormat_ASTC6x5,
    avxFormat_ASTC6x5v, // sRGB
    avxFormat_ASTC6,
    avxFormat_ASTC6v, // sRGB
    avxFormat_ASTC8x5,
    avxFormat_ASTC8x5v, // sRGB
    avxFormat_ASTC8x6,
    avxFormat_ASTC8x6v, // sRGB
    avxFormat_ASTC8,
    avxFormat_ASTC8v, // sRGB
    avxFormat_ASTC10x5,
    avxFormat_ASTC10x5v, // sRGB
    avxFormat_ASTC10x6,
    avxFormat_ASTC10x6v, // sRGB
    avxFormat_ASTC10x8,
    avxFormat_ASTC10x8v, // sRGB
    avxFormat_ASTC10,
    avxFormat_ASTC10v, // sRGB
    avxFormat_ASTC12x10,
    avxFormat_ASTC12x10v, // sRGB
    avxFormat_ASTC12,
    avxFormat_ASTC12v, // sRGB

    avxFormat_TOTAL
} avxFormat;

typedef enum avxFormatType
{
    avxFormatType_UNK,
    // Usually padding.
    avxFormatType_UNUSED,
    // Unsigned normalized.
    avxFormatType_UN,
    // Signed normalized.
    avxFormatType_SN,
    // Unsigned int.
    avxFormatType_U,
    // Signed int.
    avxFormatType_I,
    // Single precision floating point; (32-bit)
    avxFormatType_F,
    // Signed scaled.
    avxFormatType_SS,
    // Unsigned scaled.
    avxFormatType_US,
    // Fixed point.
    avxFormatType_SFIXED,
} avxFormatType;

typedef enum avxFormatFlag
// Bitmasl flags specifying characteristics of a data format.
{
    // Format supports color channels (e.g., RGBA).
    avxFormatFlag_COLOR         = AFX_BITMASK(0),
    // Format supports depth data (e.g., for depth buffering).
    avxFormatFlag_DEPTH         = AFX_BITMASK(1),
    // Format supports stencil data.
    avxFormatFlag_STENCIL       = AFX_BITMASK(2),
    // Format includes luminance information.
    avxFormatFlag_LUMINANCE     = AFX_BITMASK(3),
    // Format includes chrominance (color difference) info.
    avxFormatFlag_CHROMINANCE   = AFX_BITMASK(4),
    // Format uses sRGB color space.
    avxFormatFlag_sRGB          = AFX_BITMASK(5),
    // Format is compressed (e.g., DXT, ASTC).
    avxFormatFlag_COMPRESSED    = AFX_BITMASK(6),
    // Format supports or includes mipmaps.
    avxFormatFlag_MIPMAPPED     = AFX_BITMASK(7),
    // Data is normalized (e.g., unsigned/signed normalized).
    avxFormatFlag_NORMALIZED    = AFX_BITMASK(8),
} avxFormatFlags;

typedef enum avxFormatCap
// Bitmask flags specifying capabilities (or possible usages) of a data format,
// essentially, how a format can be used, in contrast to avxFormatFlags, which describes what a format represents.
{
    // Copy operations.
    // Can be used as a source for copy operations.
    avxFormatCap_COPY_SRC   = AFX_BITMASK(0),
    // Can be used as a destination for copy operations.
    avxFormatCap_COPY_DST   = AFX_BITMASK(1),
    avxFormatCap_COPY       = (avxFormatCap_COPY_SRC | avxFormatCap_COPY_DST),

    // Blitting (pixel transfer with format conversion or filtering).
    avxFormatCap_BLIT_SRC   = AFX_BITMASK(2),
    avxFormatCap_BLIT_DST   = AFX_BITMASK(3),
    avxFormatCap_BLIT       = (avxFormatCap_BLIT_SRC | avxFormatCap_BLIT_DST),

    // Rendering and composition.
    // Can be rendered to (as a render target).
    avxFormatCap_DRAW       = AFX_BITMASK(4),
    // Supports blending.
    avxFormatCap_BLEND      = AFX_BITMASK(5),
    avxFormatCap_COMPOSE    = (avxFormatCap_DRAW | avxFormatCap_BLEND),

    // Usable as a (read-only) sampled image/texture.
    avxFormatCap_RASTER     = AFX_BITMASK(6),
    // Supports shader image fetch/load/store.
    avxFormatCap_IMAGE      = AFX_BITMASK(7),
    // Usable as a (read-only) tensor fetch buffer, aka texel buffer.
    avxFormatCap_FETCH      = AFX_BITMASK(8),
    // Usable as a (RW) tensor buffer, aka storage texel buffer.
    avxFormatCap_TENSOR     = AFX_BITMASK(9),
    // Usable as vertex buffer format.
    avxFormatCap_VERTEX     = AFX_BITMASK(10),
} avxFormatCaps;

AFX_DEFINE_STRUCT(avxFormatDescription)
// Structure describing a data format; how data is laid out, interpreted, and how it can be used.
{
    // Total bits per pixel (across all components). For compressed formats, this might be per block.
    afxUnit32       bpp;
    // Bytes per pixel (total size per element, including padding).
    afxUnit32       stride;
    // Number of actual components (1�4); e.g., 3 for RGB, 4 for RGBA.
    afxUnit32       compCnt;
    // Type of each component (e.g., UINT8, FLOAT32, etc.).
    avxFormatType   type[4];
    // Mapping of RGBA channels (e.g., {0,1,2,3} for identity).
    afxUnit32       swizzle[4];
    // Default values for missing components (e.g., Alpha = 1).
    afxUnit32       defaults[4];
    // Whether each component is normalized to [0,1] or [-1,1].
    afxBool         isNormalized[4];
    // To-float scale factor (e.g., for unnormalized integers).
    afxReal         tof[4];
    // Bits per component.
    afxUnit32       bpc[4];
    // Block-compression width/height (e.g., {4,4} for BC/DXT formats).
    afxUnit32       bcWh[2];
    // What the format represents (e.g., COLOR, DEPTH, etc.).
    avxFormatFlags  flags;
    // Capabilities as a sampled image/texture.
    avxFormatCaps   rasterCaps;
    // Capabilities as a buffer (e.g., vertex, uniform, etc.).
    avxFormatCaps   bufferCaps;
    // Human-readable name (e.g., "RGBA8_UNORM").
    afxString       name;
};

#if !0
AVX void            AvxDescribeFormats(afxUnit cnt, avxFormat const formats[], avxFormatDescription pfd[]);

AVX afxUnit         AvxChooseFormats(avxFormatDescription const* pfd, afxUnit maxCnt, avxFormat formats[]);

AVX void AvxConvertFormat(afxUnit32 rowSize, afxUnit32 rowCnt,
    void const* srcBuffer, afxUnit32 rowStrideSrc, avxFormat srcFmt, avxFormat dstFmt,
    void* dstBuffer, afxUnit32 rowStrideDst);

AVXINL afxUnit      AfxGetBpp(avxFormat fmt);

AVXINL afxBool      AfxIsPixelFormatDepth(avxFormat fmt);
AVXINL afxBool      AfxIsPixelFormatStencil(avxFormat fmt);
AVXINL afxBool      AfxIsPixelFormatCombinedDepthStencil(avxFormat fmt);
AVXINL afxBool      AfxIsSrgb(avxFormat fmt);
AVXINL afxBool      AfxIsPixelFormatCompressed(avxFormat fmt);
#endif


#endif//AVX_FORMAT_H
