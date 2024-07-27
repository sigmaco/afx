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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// Textures in Qwadro are stored in as straightforward a manner as possible.
/// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
/// It then has a list of images, and each image lists its MIP levels.
/// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

#ifndef AVX_TXD_H
#define AVX_TXD_H

#include "qwadro/draw/afxRaster.h"

AFX_DEFINE_STRUCT(afxFileChunkTxd)
{
    afxNat              texCnt;
    afxUrdReference    names; // segment containing a list of texCnt strings specifying texture names.
    afxUrdReference    rasInfos; // segment containing a list of texCnt raster infos specifying raster parameters.
    afxUrdReference    rasData;
    // afxString;
    // afxRasterInfo;
};

AFX_DEFINE_HANDLE(avxTxd);

AFX_DEFINE_STRUCT(afxTexture)
{
    afxUri          name;
    avxTexelAddress uvw[3];
    avxTexelFilter  minFlt;
    avxSampler      samp;
    afxRaster       ras;
};

AFX_DEFINE_STRUCT(avxTxdInfo)
{
    afxNat                  texCnt;
    afxString const*        names;
    afxRasterInfo const*    rasters;
    avxSamplerConfig const* samplers;
};

////////////////////////////////////////////////////////////////////////////////

AVX afxError    AfxAcquireTxd(afxDrawInput din, avxTxdInfo const* info, avxTxd* dict);

AVX afxError    AfxOpenTxd(afxDrawInput din, afxUri const* uri, avxTxd* dict);

#endif//AVX_TXD_H
