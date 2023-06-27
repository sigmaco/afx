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

#ifndef AFX_SAMPLER_H
#define AFX_SAMPLER_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/draw/res/afxTexture.h"

typedef enum afxTexelAddress
{
    AFX_TEXEL_ADDR_REPEAT, // Tile the texture at every (u,v) integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
    AFX_TEXEL_ADDR_MIRROR, // Flip the texture at every (u,v) integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
    AFX_TEXEL_ADDR_CLAMP, // Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
    AFX_TEXEL_ADDR_BORDER, // Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in sampler or shader code.
    AFX_TEXEL_ADDR_REFLECT, // MIRROR and CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.

    AFX_TEXEL_ADDR_TOTAL
} afxTexelAddress;

typedef enum afxTexelFilter
{
    AFX_TEXEL_FLT_POINT, // Use point sampling (aka NEAREST). Supported for Lods.
    AFX_TEXEL_FLT_LINEAR, // Use linear interpolation. Supported for Lods.
    AFX_TEXEL_FLT_ANISOTROPIC, // Use anisotropic interpolation.

    AFX_TEXEL_FLT_TOTAL
} afxTexelFilter;

// A afxSampler object represent the state of an image sampler which is used by the implementation to read image data and apply filtering and other transformations for the shader.

AFX_DEFINE_STRUCT(afxSamplerSpecification)
{
    afxTexelFilter      magFilter; // LINEAR. The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified. Aka upsampling filter.
    afxTexelFilter      minFilter; // NEAREST. The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions. Aka downsampling filter.
    afxTexelFilter      mipmapFilter; // LINEAR.
    afxTexelAddress     uvw[3]; // REPEAT. Sets the wrap parameter for texture coordinates.
    afxBool             anisotropyEnabled; // FALSE
    afxReal             anisotropyMaxDegree; // 1
    afxBool             compareEnabled;
    afxCompareOp        compareOp; // LEQUAL
    afxReal             lodBias; // 0. Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
    afxReal             minLod; // -1000. Sets the minimum level-of-detail parameter. This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level).
    afxReal             maxLod; // 1000. Sets the maximum level-of-detail parameter. This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level).
    afxColor            borderColor; // (0, 0, 0, 0). Specifies the color that should be used for border texels. If a texel is sampled from the border of the texture, this value is used for the non-existent texel data. If the texture contains depth components, the first component of this color is interpreted as a depth value.
    afxBool             unnormalizedCoords;
};

AFX_DEFINE_HANDLE(afxSampler);

AFX void*               AfxSamplerGetContext(afxSampler smp);
AFX void*               AfxSamplerGetDriver(afxSampler smp);
AFX void*               AfxSamplerGetDrawSystem(afxSampler smp);

AFX void                AfxSamplerDescribe(afxSampler smp, afxSamplerSpecification *spec);


#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxSampler)
{
    afxObject           obj;
};

#endif

#endif//AFX_SAMPLER_H