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

#ifndef AFX_TXD_H
#define AFX_TXD_H

#include "qwadro/draw/io/afxRaster.h"

AFX_DEFINE_STRUCT(afxTexture)
{
    afxUri32    name;
};

AFX_DEFINE_HANDLE(afxTxd);

#ifdef _AVX_DRAW_C
#ifdef _AFX_TXD_C
#ifndef _AFX_TXD_IMPL
AFX_OBJECT(afxTxd)
#else
struct afxBaseTxd
#endif
{
    afxUri128   uri;
    afxNat      texCnt;
    afxString*  texName;
    afxRaster*  texRas;
    afxColor*   texColor;
};
#endif//_AFX_TXD_C
#endif//_AVX_DRAW_C

////////////////////////////////////////////////////////////////////////////////

AVX afxError    AfxAcquireTxd(afxNat texCnt, afxString const texNames[], afxTxd* dict);

AVX afxError    AfxLoadTxd(afxUri const* uri, afxTxd* dict);

#endif//AFX_TXD_H
