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

// This code is part of Qwadro Draw System <https://sigmaco.org/qwadro>
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_TARGA_H
#define AFX_TARGA_H

#include "qwadro/inc/draw/afxRaster.h"

// provisory as Qwadro formats are inherited from Vulkan.

typedef enum afxTargaCodec
{
    afxTargaCodec_NONE,
    afxTargaCodec_RLE8 = 1,
    afxTargaCodec_GDEFLATE,
} afxTargaCodec;

typedef enum afxTargaFlag
{
    afxTargaFlag_X_FLIPPED, // vertical origin is right to left.
    afxTargaFlag_Y_FLIPPED, // horizontal origin is top to bottom.
    afxTargaFlag_CUBEMAP, // layered image is treated as cubemap.
    afxTargaFlag_3D, // depth is treated as 3D depth instead of layers.
} afxTargaFlags;

AFX_DEFINE_STRUCT(afxTarga)
{
    afxTargaFlags   flags; // 0x01 = Cubemap
    afxPixelFormat  fmt; // raw pixel format or texel block compressed format (aka S3TC/DXT/ASTC)
    afxNat          baseLod;
    afxNat          lodCnt;
    afxWhd          origin;
    afxWhd          whd;
    union
    {
        afxSize     offset; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
        void const* src;
        void*       dst;
    }               data;
    afxTargaCodec   codec; // storage compression (not pixel compression), for example, RLE.
    afxNat          encSiz; // the size in bytes of stored image data. If not compressed by any codec, encSize is same as decSize.
    afxNat          decSiz; // the size in bytes of the image subresource. size includes any extra memory that is required based on rowStride.
    afxNat          rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
    afxNat          depthStride; // bytes per layer or (3D) slice.
    afxNat          udd[2];
};

// (x,y,z,layer) are in texel coordinates
// address(x, y, z, layer) = layer * arrayPitch + z * depthPitch + y * rowPitch + x * elementSize + offset

// (x,y,z,layer) are in compressed texel block coordinates
// address(x, y, z, layer) = layer * arrayPitch + z * depthPitch + y * rowPitch + x * compressedTexelBlockByteSize + offset;

AVX afxBool     AfxPrepareTarga(afxTarga* tga, afxRaster ras);
AVX afxBool     AfxReadTarga(afxTarga* tga, afxStream in);
AVX afxError    AfxReadTargaData(afxTarga* tga, afxStream in, void* dst);

AVX afxError    AfxWriteTarga(afxTarga* tga, void const* src, afxStream out);

AVX afxBool     AfxSetUpTarga(afxTarga* tga, afxRaster ras, afxRasterIo const* op, afxNat lodCnt);
AVX afxBool     AfxBeginTargaOutput(afxTarga* tga, afxStream out);
AVX afxBool     AfxEndTargaOutput(afxTarga* tga, afxStream out);

#endif//AFX_TARGA_H
