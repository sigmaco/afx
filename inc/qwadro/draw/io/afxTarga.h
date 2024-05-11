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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of Qwadro Draw System <https://sigmaco.org/qwadro>
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_TARGA_H
#define AFX_TARGA_H

#include "qwadro/draw/io/afxRaster.h"

// provisory as Qwadro formats are inherited from Vulkan.

typedef enum RwRasterFormat
{
    rwRASTERFORMATDEFAULT = 0x0000, /* Whatever the hardware likes best */

    rwRASTERFORMAT1555 = 0x0100,    /**<16 bits - 1 bit alpha, 5 bits red, green and blue */
    rwRASTERFORMAT565 = 0x0200,     /**<16 bits - 5 bits red and blue, 6 bits green */
    rwRASTERFORMAT4444 = 0x0300,    /**<16 bits - 4 bits per component */
    rwRASTERFORMATLUM8 = 0x0400,    /**<Gray scale */
    rwRASTERFORMAT8888 = 0x0500,    /**<32 bits - 8 bits per component */
    rwRASTERFORMAT888 = 0x0600,     /**<24 bits - 8 bits per component */
    rwRASTERFORMAT16 = 0x0700,      /**<16 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT24 = 0x0800,      /**<24 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT32 = 0x0900,      /**<32 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT555 = 0x0a00,     /**<16 bits - 5 bits red, green and blue */

    rwRASTERFORMATAUTOMIPMAP = 0x1000, /**<RenderWare generated the mip levels */

    rwRASTERFORMATPAL8 = 0x2000,    /**<8 bit palettised */
    rwRASTERFORMATPAL4 = 0x4000,    /**<4 bit palettised */

    rwRASTERFORMATMIPMAP = 0x8000,  /**<Mip mapping on */

    rwRASTERFORMATPIXELFORMATMASK = 0x0f00, /**<The pixel color format
                                             *  (excluding palettised bits) */
    rwRASTERFORMATMASK = 0xff00     /**<The whole format */
} RwRasterFormat;

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

typedef enum RwRasterFormat RwRasterFormat;

AFX_DEFINE_STRUCT(afxTarga)
{
    afxNat          udd[2];
    afxTargaFlags   flags; // 0x01 = Cubemap
    afxTargaCodec   codec; // storage compression (not DXT or other pixel compression)
    afxPixelFormat  fmt; // raw pixel format or texel block compressed format (aka S3TC/DXT/ASTC)
    afxNat          lodCnt;
    afxWhd          origin;
    afxWhd          whd;
    union
    {
        afxSize     offset; // where start (above this header) the raster data // for each lod, sizeof lod is rowLen * whd[1] * whd[2] * layerCnt 
        void const* src;
        void*       dst;
    }               data;
    afxNat          encSiz; // the size in bytes of stored image data. If not compressed by any codec, encSize is same as decSize.
    afxNat          decSiz; // the size in bytes of the image subresource. size includes any extra memory that is required based on rowStride.
    afxNat          rowStride; // bytes per row/scanline (W usually aligned/padded to some memory boundary)
    afxNat          depthStride; // bytes per layer or (3D) slice.
};

// (x,y,z,layer) are in texel coordinates
// address(x, y, z, layer) = layer * arrayPitch + z * depthPitch + y * rowPitch + x * elementSize + offset

// (x,y,z,layer) are in compressed texel block coordinates
// address(x, y, z, layer) = layer * arrayPitch + z * depthPitch + y * rowPitch + x * compressedTexelBlockByteSize + offset;

AVX afxBool     AfxReadTarga(afxTarga* tga, afxStream in);
AVX afxError    AfxReadTargaData(afxTarga* tga, afxStream in, void* dst);

AVX afxError    AfxWriteTarga(afxTarga* tga, void const* src, afxStream out);

#endif//AFX_TARGA_H
