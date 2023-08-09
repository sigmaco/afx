/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_TARGA_H
#define AFX_TARGA_H

#include "afx/draw/afxDrawContext.h"

AFX afxError    AfxLoadTexturesTarga(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);

AFX afxError    AfxFetchTextureFromTarga(afxTexture tex, afxUri const *uri);
AFX afxError    AfxFetchTextureRegionsFromTarga(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

AFX afxError    AfxPrintTextureToTarga(afxTexture tex, afxUri const *uri);
AFX afxError    AfxPrintTextureRegionsToTarga(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#if 0

hkRaster;
hkTexture;
hkSurface;

dictionary GPUTextureViewDescriptor
    : GPUObjectDescriptorBase{
GPUTextureFormat format;
GPUTextureViewDimension dimension;
GPUTextureAspect aspect = "all";
GPUIntegerCoordinate baseMipLevel = 0;
GPUIntegerCoordinate mipLevelCount;
GPUIntegerCoordinate baseArrayLayer = 0;
GPUIntegerCoordinate arrayLayerCount;
};

dictionary GPUTextureDescriptor
    : GPUObjectDescriptorBase{
required GPUExtent3D size;
GPUIntegerCoordinate mipLevelCount = 1;
GPUSize32 sampleCount = 1;
GPUTextureDimension dimension = "2d";
required GPUTextureFormat format;
required GPUTextureUsageFlags usage;
sequence<GPUTextureFormat> viewFormats = [];
};

AFX_DEFINE_STRUCT()
{
    afxTextureFlags         flags;

    afxPixelFormat          fmt;
    afxWhd                  whd; // extent of image
    afxNat                  lodCnt; // mip level cnt
    afxNat                  imgCnt;
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxBool                 linearlyTiled; // optimal tiling (for driver) if false.
    afxColorSwizzling const*swizzling;

    afxByte*                maps;
};

AFX_DEFINE_STRUCT()
{
    afxTextureFlags         flags;

    afxPixelFormat          fmt;
    afxWhd                  whd; // extent of image
    afxNat                  lodCnt; // mip level cnt
    afxNat                  imgCnt;
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxBool                 linearlyTiled; // optimal tiling (for driver) if false.
    afxColorSwizzling const*swizzling;

    afxByte*                maps;
};

#endif

#endif//AFX_TARGA_H