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

#include "../base/afxDrawDefs.h"
#include "../res/afxTexture.h"

typedef enum afxTexelAddress
{
    AFX_TEXEL_ADDR_REPEAT, // the repeat wrap mode will be used.
    AFX_TEXEL_ADDR_MIRROR, // the mirrored repeat wrap mode will be used.
    AFX_TEXEL_ADDR_EDGE, // the clamp to edge wrap mode will be used.
    AFX_TEXEL_ADDR_BORDER, // the clamp to border wrap mode will be used.
    AFX_TEXEL_ADDR_WRAP, // mirror clamp to edge?

    AFX_TEXEL_ADDR_TOTAL
} afxTexelAddress;

typedef enum afxTexelFilter
{
    AFX_TEXEL_FLT_NEAREST, // supported for Lods
    AFX_TEXEL_FLT_LINEAR, // supported for Lods
    AFX_TEXEL_FLT_ANISOTROPIC,

    AFX_TEXEL_FLT_TOTAL
} afxTexelFilter;

// A afxSampler object represent the state of an image sampler which is used by the implementation to read image data and apply filtering and other transformations for the shader.

AFX_DEFINE_STRUCT(afxSamplerSpecification)
{
    afxTexelFilter      magFilter; // LINEAR. The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified.
    afxTexelFilter      minFilter; // NEAREST. The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions.
    afxTexelFilter      mipmapFilter; // LINEAR.
    afxTexelAddress     uvw[3]; // REPEAT. Sets the wrap parameter for texture coordinates.
    afxBool             anisotropyEnabled; // FALSE
    afxReal             anisotropyMaxDegree; // 1
    afxBool             compareEnabled;
    afxCompareOp        compareOp; // LEQUAL
    afxReal             mipLodBias; // 0. Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
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