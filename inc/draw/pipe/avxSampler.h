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

/**
    A avxSampler object represent the state of an image sampler which is used by the implementation to read image data and apply filtering and other transformations for the shader.
*/

#ifndef AVX_SAMPLER_H
#define AVX_SAMPLER_H

#include "qwadro/inc/draw/io/afxPixel.h"
#include "qwadro/inc/draw/math/afxColor.h"

typedef enum avxTexelAddress
/// Specify behavior of sampling with texture coordinates outside an texel map.
{
    avxTexelAddress_REPEAT, // Tile the texture at every (u,v) integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
    avxTexelAddress_MIRROR, // Flip the texture at every (u,v) integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
    avxTexelAddress_CLAMP, // Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
    avxTexelAddress_BORDER, // Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in sampler or shader code.
    avxTexelAddress_REFLECT, // MIRROR and CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.

    avxTexelAddress_TOTAL
} avxTexelAddress;

typedef enum avxTexelFilter
/// Specify filters used for texel map lookups.
{
    avxTexelFilter_POINT, // Use point sampling (aka NEAREST). Supported for Lods.
    avxTexelFilter_LINEAR, // Use linear interpolation. Supported for Lods.
    avxTexelFilter_ANISOTROPIC, // Use anisotropic interpolation.

    avxTexelFilter_TOTAL
} avxTexelFilter;

typedef enum avxYuvModelConversion
{
    AFX_YUV_MODEL_CONV_RGB_IDENTITY = 0,
    AFX_YUV_MODEL_CONV_IDENTITY = 1,
    AFX_YUV_MODEL_CONV_709 = 2,
    AFX_YUV_MODEL_CONV_601 = 3,
    AFX_YUV_MODEL_CONV_2020 = 4,
    
    /*
        the Y (luma) component corresponds to the G component of an RGB image.
        the U (chroma blue color difference) component corresponds to the B component of an RGB image.
        the V (chroma red color difference) component corresponds to the R component of an RGB image.
        the alpha component, if present, is not modified by color model conversion.
    */
} avxYuvModelConversion;

AFX_DEFINE_STRUCT(avxSamplerInfo)
{
    avxTexelFilter          magnify; // LINEAR. The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified. Aka upsampling filter.
    avxTexelFilter          minify; // NEAREST. The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions. Aka downsampling filter.
    avxTexelFilter          mipify; // Mipmap filtering mode. Default is LINEAR.
    avxTexelAddress         uvw[3]; // REPEAT. Sets the wrap parameter for texture coordinates.
    afxBool                 anisotropyEnabled; // FALSE
    afxReal                 anisotropyMaxDegree; // 1
    afxBool                 compareEnabled;
    avxCompareOp            compareOp; // LEQUAL
    afxReal                 lodBias; // 0. Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
    afxReal                 minLod; // -1000. Sets the minimum level-of-detail parameter. This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level).
    afxReal                 maxLod; // 1000. Sets the maximum level-of-detail parameter. This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level).
    
    //
    afxColor                borderColor; // (0, 0, 0, 0). Specifies the color that should be used for border texels. If a texel is sampled from the border of the texture, this value is used for the non-existent texel data. If the texture contains depth components, the first component of this color is interpreted as a depth value.
    afxBool                 unnormalizedCoords;
};

AFX_DEFINE_STRUCT(avxYuvSamplerInfo)
{
    avxSamplerInfo          base;
    avxFormat               fmt;
    avxYuvModelConversion   ycbcrModel;
    afxBool                 useNarrowRange;
    avxColorSwizzling       components;
    afxBool                 xChromaOffsetAtMidpoint;
    afxBool                 yChromaOffsetAtMidpoint;
    avxTexelFilter          chromaFilter;
    afxBool                 forceExplicitReconstruction;
};

AVX void            AfxDescribeSampler(avxSampler samp, avxSamplerInfo* config);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxDeclareSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[]);
AVX afxError        AfxDeclareYuvSamplers(afxDrawSystem dsys, afxUnit cnt, avxYuvSamplerInfo const cfg[], avxSampler samplers[]);

AVX afxBool         AfxFindSamplers(afxDrawSystem dsys, afxUnit cnt, avxSamplerInfo const cfg[], avxSampler samplers[]);
AVX afxBool         AfxFindYuvSamplers(afxDrawSystem dsys, afxUnit cnt, avxYuvSamplerInfo const cfg[], avxSampler samplers[]);

AVX void            AfxDescribeDefaultSampler(avxSamplerInfo* config);

#endif//AVX_SAMPLER_H
