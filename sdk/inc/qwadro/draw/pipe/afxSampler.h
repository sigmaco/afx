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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_SAMPLER_H
#define AFX_SAMPLER_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum afxTexelAddress
{
    afxTexelAddress_REPEAT, // Tile the texture at every (u,v) integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
    afxTexelAddress_MIRROR, // Flip the texture at every (u,v) integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
    afxTexelAddress_CLAMP, // Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
    afxTexelAddress_BORDER, // Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in sampler or shader code.
    afxTexelAddress_REFLECT, // MIRROR and CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.

    afxTexelAddress_TOTAL
} afxTexelAddress;

typedef enum afxTexelFilter
{
    afxTexelFilter_POINT, // Use point sampling (aka NEAREST). Supported for Lods.
    afxTexelFilter_LINEAR, // Use linear interpolation. Supported for Lods.
    afxTexelFilter_ANISOTROPIC, // Use anisotropic interpolation.

    afxTexelFilter_TOTAL
} afxTexelFilter;

typedef enum afxYuvModelConversion
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
} afxYuvModelConversion;

AFX_DEFINE_STRUCT(afxSamplerConfigYuv)
{
    afxPixelFormat          fmt;
    afxYuvModelConversion   ycbcrModel;
    afxBool                 useNarrowRange; 
    afxColorSwizzling       components;
    afxBool                 xChromaOffsetAtMidpoint;
    afxBool                 yChromaOffsetAtMidpoint;
    afxTexelFilter          chromaFilter;
    afxBool                 forceExplicitReconstruction;
};

// A afxSampler object represent the state of an image sampler which is used by the implementation to read image data and apply filtering and other transformations for the shader.

AFX_DEFINE_STRUCT(afxSamplerConfig)
{
    afxTexelFilter  magFilter; // LINEAR. The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified. Aka upsampling filter.
    afxTexelFilter  minFilter; // NEAREST. The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions. Aka downsampling filter.
    afxTexelFilter  mipmapFilter; // LINEAR.
    afxTexelAddress uvw[3]; // REPEAT. Sets the wrap parameter for texture coordinates.
    afxBool         anisotropyEnabled; // FALSE
    afxReal         anisotropyMaxDegree; // 1
    afxBool         compareEnabled;
    afxCompareOp    compareOp; // LEQUAL
    afxReal         lodBias; // 0. Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
    afxReal         minLod; // -1000. Sets the minimum level-of-detail parameter. This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level).
    afxReal         maxLod; // 1000. Sets the maximum level-of-detail parameter. This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level).
    afxColor        borderColor; // (0, 0, 0, 0). Specifies the color that should be used for border texels. If a texel is sampled from the border of the texture, this value is used for the non-existent texel data. If the texture contains depth components, the first component of this color is interpreted as a depth value.
    afxBool         unnormalizedCoords;
    void*           ext;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_SAMPLER_C
#ifndef _AFX_SAMPLER_IMPL
AFX_OBJECT(afxSampler)
#else
struct afxBaseSampler
#endif
{
    afxNat32        crc32;
    afxTexelFilter  magFilter; // LINEAR. The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified.
    afxTexelFilter  minFilter; // NEAREST. The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions.
    afxTexelFilter  mipmapFilter; // LINEAR.
    afxTexelAddress uvw[3]; // REPEAT. Sets the wrap parameter for texture coordinates.
    afxBool         anisotropyEnabled; // FALSE
    afxReal         anisotropyMaxDegree; // 1
    afxBool         compareEnabled;
    afxCompareOp    compareOp; // LEQUAL
    afxReal         lodBias; // 0. Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
    afxReal         minLod; // -1000. Sets the minimum level-of-detail parameter. This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level).
    afxReal         maxLod; // 1000. Sets the maximum level-of-detail parameter. This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level).
    afxColor        borderColor; // (0, 0, 0, 0). Specifies the color that should be used for border texels. If a texel is sampled from the border of the texture, this value is used for the non-existent texel data. If the texture contains depth components, the first component of this color is interpreted as a depth value.
    afxBool         unnormalizedCoords;
};
#endif
#endif

AFX afxSampler      AfxAcquireBilinearSampler(afxDrawContext dctx);
AFX afxSampler      AfxAcquireTrilinearSampler(afxDrawContext dctx);

AFX afxError        AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerConfig const config[], afxSampler samp[]);

AFX void            AfxDescribeSampler(afxSampler samp, afxSamplerConfig* config);

AFX void            AfxDescribeDefaultSampler(afxSamplerConfig* config);

#endif//AFX_SAMPLER_H
