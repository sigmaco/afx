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

/// Textures in Qwadro are stored in as straightforward a manner as possible.
/// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
/// It then has a list of images, and each image lists its MIP levels.
/// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

/*
    A texture dictionary is a collection or database of textures, typically used in computer graphics, game development, and 3D modeling. 
    It serves as a centralized repository where various textures—like images that simulate surfaces (e.g., wood, stone, fabric)—are stored and organized for easy access.

    In practical terms, a texture dictionary can help artists and developers quickly find and apply the right textures to their models or scenes. 
    It often includes metadata about each texture, such as resolution, file format, and usage guidelines. 
    This helps streamline workflows and ensures consistency across projects.
*/

#ifndef AVX_TXD_H
#define AVX_TXD_H

#include "qwadro/inc/draw/io/avxRaster.h"

#define AFX_MAX_TEX_ID_LEN 32

AFX_DEFINE_STRUCT(afxTexture)
{
    afxUri          name;
    avxTexelWrap uvw[3];
    avxTexelFilter  minFlt;
    avxSampler      samp;
    avxRaster       ras;
};

AFX_DEFINE_STRUCT(avxTxdInfo)
{
    afxUri const*           url;
    afxUnit                  texCnt;
    afxString const*        names;
    avxRasterInfo const*    rasters;
    avxSamplerInfo const*   samplers;
};

////////////////////////////////////////////////////////////////////////////////

AVX afxError    AfxAcquireTxd(afxDrawInput din, avxTxdInfo const* info, afxTxd* dict);

AVX afxError    AfxOpenTxd(afxDrawInput din, afxUri const* url, afxTxd* dict);
AVX afxError    AfxLoadTxd(afxDrawInput din, afxStream in, afxTxd* dict);

#endif//AVX_TXD_H
