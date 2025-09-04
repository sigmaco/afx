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
 // QWADRO TEXTURE EXCHANGE DICTIONARY                                       //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/*
    The Texture Exchange Dictionary (TXD) is a mechanism inspired in another from RenderWare to efficiently manage textures for rendering. 
    It's part of an major optimization design aimed at improving the performance and flexibility of texture usage, particularly 
    in more advanced or complex rendering scenarios, such as those found in multi-pass rendering operations.

    The "texture exchange dictionary" is a peculiarity of Qwadro that leverage this idea for more efficient texture management. 
    In a more general context, it refers to a way of organizing, caching, or managing textures for faster access or reusability.

    TEDs are used to keep track of textures in a way that optimizes their exchange or swapping during rendering operations.
    Instead of repeatedly loading and unloading textures from memory, which is time-consuming, the TED enables more efficient reuse and management.

    As suggests the term "dictionary", this mechanism may store textures identified by a unique key or reference, and its associated texture 
    data (such as the actual texture pixels, format, size, etc.) is stored alongside it.
    The idea is to reduce redundancy and facilitate quick access to textures that may be needed during different stages of rendering.

    By using a TXD, textures that are used repeatedly (for example, in different rendering passes or for various object states) can be kept in 
    memory or reused across multiple frames without needing to reload them from disk. This also allows for faster switching between textures because the system can swap textures efficiently.

    By caching and efficiently managing textures, TEDs help reduce latency and overhead when rendering objects or paths that require complex texture operations.
    Efficiently organizing textures and minimizing redundant loading/unloading operations can prevent texture-related bottlenecks and resource exhaustion.

*/

#ifndef AVX_TXD_H
#define AVX_TXD_H

#include "qwadro/inc/draw/avxRaster.h"

#define AFX_MAX_TEXTURE_NAME_LENGTH (32)

AFX_DEFINE_STRUCT(afxTexture)
{
    afxUri          name;
    avxTexelWrap    uvw[3];
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

AVX afxCmdId    AvxCmdUseTxds(afxDrawContext dctx, afxUnit baseSlotIdx, afxUnit cnt, avxTxd txds[]);
AVX afxCmdId    AvxCmdBindTextures(afxDrawContext dctx, afxUnit txdIdx, afxUnit cnt, afxUnit const indices[]);

AVX afxError    AvxAddTextures(avxTxd txd, afxUnit cnt, afxString const names[]);
AVX afxError    AvxFindTextures(avxTxd txd, afxUnit cnt, afxString const names[], afxUnit indices[]);
AVX afxError    AvxRequestTextures(avxTxd txd, afxUnit cnt, afxUnit indices[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError    AvxAcquireTxd(afxDrawSystem dsys, avxTxdInfo const* info, avxTxd* dict);

AVX afxError    AvxOpenTxd(afxDrawSystem dsys, afxUri const* url, avxTxd* dict);

#endif//AVX_TXD_H
