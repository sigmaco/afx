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
 // QWADRO FIXED FUNCTION TEXTURE SAMPLER                                    //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/**
    A avxSampler object represent the state of an texture sampler which is used by the implementation to read image data and apply filtering and other transformations for the shader.
*/

#ifndef AVX_SAMPLER_H
#define AVX_SAMPLER_H

#include "qwadro/inc/draw/io/avxFormat.h"
#include "qwadro/inc/draw/math/avxColor.h"

typedef enum avxSamplerFlag
{
    avxSamplerFlag_SUBSAMPLED = AFX_BITMASK(0),
    avxSamplerFlag_NONSEAMLESS= AFX_BITMASK(1)
} avxSamplerFlags;

typedef enum avxTexelWrap
// Specify behavior of sampling with texture coordinates outside an texel map.
{
    // Tile the texture at every (u,v) integer junction. 
    // For example, for u values between 0 and 3, the texture is repeated three times.
    avxTexelWrap_REPEAT,

    // Flip the texture at every (u,v) integer junction. 
    // For u values between 0 and 1, for example, the texture is addressed normally; 
    // between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
    avxTexelWrap_MIRROR,

    // Clamp to edge
    // Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
    avxTexelWrap_EDGE,

    // Clamp to border
    // Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in sampler or shader code.
    avxTexelWrap_BORDER,

    // MIRROR and CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), 
    // and then clamps to the maximum value.
    avxTexelWrap_REFLECT,

    avxTexelWrap_TOTAL
} avxTexelWrap;

typedef enum avxTexelFilter
// Specify filters used for texel map lookups.
{
    avxTexelFilter_NEAREST, // Use point sampling (aka NEAREST). Supported for Lods.
    avxTexelFilter_LINEAR, // Use linear interpolation. Supported for Lods.
    avxTexelFilter_ANISOTROPIC, // Use anisotropic interpolation.

    avxTexelFilter_TOTAL
} avxTexelFilter;

typedef enum avxTexelReduction
// Specify reduction mode for texture filtering.
{
    // Texel values are combined by computing a weighted average of values in the footprint, 
    // using weights as specified in the image operations chapter.
    avxTexelReduction_AVG,

    // Texel values are combined by taking the component-wise minimum of values in the footprint with non-zero weights.
    avxTexelReduction_MIN,

    // Texel values are combined by taking the component-wise maximum of values in the footprint with non-zero weights.
    avxTexelReduction_MAX,

    avxTexelReduction_TOTAL
}avxTexelReduction;



typedef enum avxColorMatrix
/*
    Color model component of a color space.

    The Y (luma) component corresponds to the G component of an RGB image.
    The U (chroma blue color difference) component corresponds to the B component of an RGB image.
    The V (chroma red color difference) component corresponds to the R component of an RGB image.
    The alpha component, if present, is not modified by color model conversion.
*/
{
    // The input values to the conversion are unmodified.
    avxColorMatrix_RGB,
    // Nno model conversion but the inputs are range expanded as for YUV.
    avxColorMatrix_YUV,
    // The color model conversion from YUV to RGB defined in BT.709.
    avxColorMatrix_YUV709,
    // The color model conversion from YUV to RGB defined in BT.601.
    avxColorMatrix_YUV601,
    // The color model conversion from YUV to RGB defined in BT.2020.
    avxColorMatrix_YUV2020,
    
    avxColorMatrix_TOTAL
} avxColorMatrix;

AFX_DEFINE_UNION(avxClearValue)
// Union specifying a clear value.
{
    union
    {
        // The clear values to use when clearing a color buffer.
        afxV4d  rgba;
        afxInt  rgbai[4];
        afxUnit rgban[4];
    };
    struct
    {
        // The clear values to use when clearing a depth and/or stencil buffer.
        afxReal depth; // 1.f (for avxCompareOp_LESS) or 0.f (for avxCompareOp_GREATER).
        afxUnit stencil; // 0
    };
};

AFX_DEFINE_STRUCT(avxSamplerInfo)
{
    avxSamplerFlags     flags;

    // Sets the wrap parameter for texture coordinates.
    avxTexelWrap        uvw[3]; // REPEAT.
    
    //  The texture magnification function is used whenever the level-of-detail function used when 
    // sampling from the texture determines that the texture should be magified. Aka upsampling filter.
    avxTexelFilter      magnify; // LINEAR.
    //  The texture minifying function is used whenever the level-of-detail function used when sampling 
    // from the texture determines that the texture should be minified. There are six defined minifying functions. Aka downsampling filter.
    avxTexelFilter      minify; // NEAREST.
    // Mipmap filtering mode.
    avxTexelFilter      mipFlt; // LINEAR.
    // How texture filtering combines texel values.
    avxTexelReduction   reductionMode; // avxTexelReduction_AVG.
    afxBool             anisotropyEnabled; // FALSE
    afxReal             anisotropyMaxDegree; // 1

    afxBool             compareEnabled;
    avxCompareOp        compareOp; // LEQUAL
    // Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
    afxReal             lodBias; // 0.
    // Sets the minimum level-of-detail parameter. 
    // This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level).
    afxReal             minLod; // -1000.
    // Sets the maximum level-of-detail parameter. 
    // This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level).
    afxReal             maxLod; // 1000.
    
    // Specifies the color that should be used for border texels. 
    // If a texel is sampled from the border of the texture, this value is used for the non-existent texel data. 
    // If the texture contains depth components, the first component of this color is interpreted as a depth value.
    avxClearValue       borderColor; // (0, 0, 0, 0).
    afxBool             unnormalizedCoords;

    // YUV CONVERSION

    afxBool             isYuv;
    // The format of the raster from which color information will be retrieved.
    avxFormat           yuvFmt;
    // The color matrix for conversion between color models.
    avxColorMatrix      yuvMtx;

    // Specifies that headroom and foot room are reserved in the numerical range of encoded values, 
    // and the remaining values are expanded according to the ITU "narrow range" quantization rules.
    afxBool             useNarrowRange;
    // Swizzle to be applied prior to range expansion and color model conversion.
    avxSwizzling        yuvSizzle;
    // The sample location associated with downsampled chroma components in the x dimension.
    // Has no effect for formats in which chroma components are not downsampled horizontally.
    afxBool             xChromaOffset;
    // The sample location associated with downsampled chroma components in the y dimension.
    // Has no effect for formats in which the chroma components are not downsampled vertically.
    afxBool             yChromaOffset;
    // The filter for chroma reconstruction.
    avxTexelFilter      chromaFilter;
    // Ensure that reconstruction is done explicitly, if supported.
    afxBool             forceExplicitReconstr;

    afxString           tag;
};

#define AVX_COLOR_VALUE(r_, g_, b_, a_) (avxClearValue const){ { .rgba = { (r_), (g_), (b_), (a_) } } }
#define AVX_DEPTH_VALUE(d_, s_) (avxClearValue const){ { .depth = (d_), .stencil = (s_) } }

AVX avxSamplerInfo const AVX_SAMPLER_DEFAULT;

AVX void            AvxDescribeSampler(avxSampler samp, avxSamplerInfo* desc);

AVX void            AvxGetSamplerBorderColor(avxSampler samp, avxClearValue* val);

AVX afxBool         AvxIsSamplerYuvCapable(avxSampler samp);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AvxDeclareSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[]);

AVX afxBool         AvxFindSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[]);

#endif//AVX_SAMPLER_H
