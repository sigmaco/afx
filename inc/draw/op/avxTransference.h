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
 // VIDEO MEMORY TRANSFERENCE OPERATIONS                                     //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_TRANSFERENCE_H
#define AVX_TRANSFERENCE_H

//#include "qwadro/inc/draw/io/avxRaster.h"
#include "qwadro/inc/math/afxWarp.h"
#include "qwadro/inc/draw/op/avxSampler.h"
#include "qwadro/inc/draw/io/avxBuffer.h"
#include "qwadro/inc/math/avxViewport.h"

#define AVX_BUFFER_UPDATE_CAPACITY (65536)

AFX_DEFINE_STRUCT(avxRasterBlock)
// Structure specifying an avxRaster subresource range.
{
    // The first mipmap level accessible to the view.
    afxUnit baseLod;
    // The number of mipmap levels (starting from @baseLodIdx) accessible to the view.
    afxUnit lodCnt;
    // The first array layer accessible to the view.
    afxUnit baseLayer;
    // The number of array layers (starting from @baseLayer) accessible to the view.
    afxUnit layerCnt;
};

AFX_DEFINE_STRUCT(avxRasterRegion)
{
    afxUnit         lodIdx;
    // The initial x, y, z offsets in texels of the sub-region of the source or destination avxRaster data.
    avxOrigin       origin;
    // The size in texels of the avxRaster to copy in width, height and depth.
    avxRange        whd;
};

AFX_DEFINE_STRUCT(avxRasterCopy)
// Especificação de operação de cópia de avxRaster.
{
    avxRasterRegion src;
    afxUnit         dstLodIdx;
    avxOrigin       dstOrigin; // select the initial x, y, and z offsets in texels of the sub-regions of the source and destination avxRaster data.
};

AFX_DEFINE_STRUCT(avxRasterBlit)
// Structure specifying an avxRaster blit operation.
{
    avxRasterRegion src;
    avxRasterRegion dst;
};

AFX_DEFINE_STRUCT(avxRasterIo)
// Especificação de operação de transferência arbitrária de avxRaster.
{
    avxRasterRegion rgn;
    // The offset in bytes from the start of the buffer object where the avxRaster data is copied from or to.
    afxSize         offset;
    // (aka bytes per row) specify in texels a subregion of an image, and control the addressing calculations. 
    // If either of these values is zero, that aspect of the image is considered to be tightly packed according to the raster's extent.
    afxUnit         rowStride;
    // (aka rows per layer) specify in texels a subregion of an image, and control the addressing calculations. 
    // If either of these values is zero, that aspect of the image is considered to be tightly packed according to the raster's extent.
    afxUnit         rowsPerImg;
    //  rowsPerImage * rowStride is the stride, in bytes, between the beginning of each texel image of data and the subsequent texel image.
    afxCodec        codec;
    afxUnit         decSiz;
    afxUnit         encSiz;
};


// COMMANDOS


AVX afxCmdId                AvxCmdClearBuffer
// Fill a region of a avxBuffer with zeroes.
(
    afxDrawContext          dctx,
    // The buffer to be zeroed.
    avxBuffer               buf,
    // The byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxSize                 offset,
    // The number of bytes to zero, and must be either a multiple of 4, or 0 to zero the range from offset to the end of the buffer. 
    // If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.    
    afxUnit                 range
);

AVX afxCmdId                AvxCmdFillBuffer
// Fill a region of a avxBuffer with a fixed value.
(
    afxDrawContext          dctx,
    // The buffer to be filled.
    avxBuffer               buf,
    // The byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxSize                 offset,
    // The number of bytes to fill, and must be either a multiple of 4, or 0 to fill the range from offset to the end of the buffer. 
    // If 0 is used and the remaining size of the buffer is not a multiple of 4, then the nearest smaller multiple is used.
    afxUnit                 range,
    // The 4-byte word written repeatedly to the buffer to fill size bytes of data. 
    // The data word is written to memory according to the host endianness.
    afxUnit                 value
);

AVX afxCmdId                AvxCmdUpdateBuffer
// Update a avxBuffer's contents from host memory.
(
    afxDrawContext          dctx,
    // The buffer to be updated.
    avxBuffer               buf,
    // The byte offset into the buffer to start updating, and must be a multiple of 4.
    afxSize                 offset,
    // The number of bytes to update, and must be a multiple of 4.
    afxUnit                 range,
    // The source data for the buffer update, and must be at least @range bytes in size.
    void const*             src
);

AVX afxCmdId                AvxCmdCopyBuffer
// Copy data between avxBuffer regions.
(
    afxDrawContext          dctx,
    // The destination buffer.
    avxBuffer               dst,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    avxBufferCopy const     ops[],
    // The source buffer.
    avxBuffer               src
);

AVX afxCmdId                AvxCmdClearRaster
// Clear regions of a color avxRaster or fill regions of a combined depth/stencil avxRaster.
(
    afxDrawContext          dctx,
    // The avxRaster to be cleared.
    avxRaster               ras,
    // A structure containing the values that the avxRaster subresource ranges will be cleared to.
    avxClearValue const*    value,
    afxUnit                 baseLod,
    afxUnit                 lodCnt,
    afxUnit                 baseLayer,
    afxUnit                 layerCnt
);

AVX afxCmdId                AvxCmdCopyRaster
// Copy data between avxRaster's.
(
    afxDrawContext          dctx,
    // The destination avxRaster.
    avxRaster               dst,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterCopy const     ops[],
    // The source avxRaster.
    avxRaster               src
);

AVX afxCmdId                AvxCmdResolveRaster
// Resolve regions of an avxRaster.
(
    afxDrawContext          dctx,
    // The destination avxRaster.
    avxRaster               dst,
    // The number of regions to resolve.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to resolve.
    avxRasterCopy const     ops[],
    // The source avxRaster.
    avxRaster               src
);

AVX afxCmdId                AvxCmdBlitRaster
// Copy regions of an avxRaster, potentially performing format conversion.
(
    afxDrawContext          dctx,
    // The destination avxRaster.
    avxRaster               dst,
    // The number of regions to blit.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to blit.
    avxRasterBlit const     ops[],
    // The source avxRaster.
    avxRaster               src,
    // A avxTexelFilter specifying the filter to apply if the blits require scaling.
    avxTexelFilter          flt
);

AVX afxCmdId                AvxCmdPackRaster
// Copy avxRaster data into a avxBuffer.
(
    afxDrawContext          dctx,
    // The source avxRaster.
    avxRaster               ras,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const       ops[],
    // The destination buffer.
    avxBuffer               buf
);

AVX afxCmdId                AvxCmdUnpackRaster
// Copy data from a avxBuffer into an avxRaster.
(
    afxDrawContext          dctx,
    // The destination avxRaster.
    avxRaster               ras,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const       ops[],
    // The source buffer.
    avxBuffer               buf
);



AVX afxCmdId                AvxCmdRegenerateMipmapsSIGMA
// Regenerate mipmaps for an array of avxRaster.
(
    afxDrawContext          dctx,
    afxFlags                flags,
    afxUnit                 rasCnt,
    avxRaster               rasters[]
);

#endif//AVX_TRANSFERENCE_H
