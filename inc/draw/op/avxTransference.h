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

#ifndef AVX_TRANSFERENCE_H
#define AVX_TRANSFERENCE_H

//#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/math/afxWhd.h"
#include "qwadro/inc/draw/op/avxSampler.h"
#include "qwadro/inc/draw/io/avxBuffer.h"
#include "qwadro/inc/draw/math/afxViewport.h"

#define AVX_BUFFER_UPDATE_CAPACITY (65536)

AFX_DEFINE_STRUCT(afxRasterBlock)
// Structure specifying an afxRaster subresource range.
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

AFX_DEFINE_STRUCT(afxRasterRegion)
{
    afxUnit         lodIdx;
    // The initial x, y, z offsets in texels of the sub-region of the source or destination afxRaster data.
    avxOrigin       origin;
    // The size in texels of the afxRaster to copy in width, height and depth.
    avxRange        extent;
};

AFX_DEFINE_STRUCT(afxRasterCopy)
// Especificação de operação de cópia de afxRaster.
{
    afxRasterRegion src;
    afxUnit         dstLodIdx;
    avxOrigin       dstOrigin; // select the initial x, y, and z offsets in texels of the sub-regions of the source and destination afxRaster data.
};

AFX_DEFINE_STRUCT(afxRasterBlit)
// Structure specifying an afxRaster blit operation.
{
    afxRasterRegion src;
    afxRasterRegion dst;
};

AFX_DEFINE_STRUCT(afxRasterIo)
// Especificação de operação de transferência arbitrária de afxRaster.
{
    afxRasterRegion rgn;
    // The offset in bytes from the start of the buffer object where the afxRaster data is copied from or to.
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

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDOS                                                            ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AvxCmdClearBuffer
// Fill a region of a avxBuffer with zeroes.
(
    afxDrawContext          dctx,
    // The buffer to be zeroed.
    avxBuffer               buf,
    // The byte offset into the buffer at which to start filling, and must be a multiple of 4.
    afxUnit                 offset,
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
    afxUnit                 offset,
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
    afxUnit                 offset,
    // The number of bytes to update, and must be a multiple of 4.
    afxUnit                 range,
    // The source data for the buffer update, and must be at least @range bytes in size.
    void const*             src
);

AVX afxCmdId                AvxCmdCopyBuffer
// Copy data between avxBuffer regions.
(
    afxDrawContext          dctx,
    // The source buffer.
    avxBuffer               src,
    // The destination buffer.
    avxBuffer               dst,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    avxBufferCopy const     ops[]
);

AVX afxCmdId                AvxCmdClearRaster
// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    afxDrawContext          dctx,
    // The afxRaster to be cleared.
    afxRaster               ras,
    // A structure containing the values that the afxRaster subresource ranges will be cleared to.
    avxClearValue const*    value,
    // The number of afxRaster subresource ranges.
    afxUnit                 opCnt,
    // An array of structures describing a range of mipmap levels, array layers, and aspects to be cleared.
    afxRasterBlock const    ops[]
);

AVX afxCmdId                AvxCmdCopyRaster
// Copy data between afxRaster's.
(
    afxDrawContext          dctx,
    // The source afxRaster.
    afxRaster               src,
    // The destination afxRaster.
    afxRaster               dst,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    afxRasterCopy const     ops[]
);

AVX afxCmdId                AvxCmdResolveRaster
// Resolve regions of an afxRaster.
(
    afxDrawContext          dctx,
    // The source afxRaster.
    afxRaster               src,
    // The destination afxRaster.
    afxRaster               dst,
    // The number of regions to resolve.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to resolve.
    afxRasterCopy const     ops[]
);

AVX afxCmdId                AvxCmdBlitRaster
// Copy regions of an afxRaster, potentially performing format conversion.
(
    afxDrawContext          dctx,
    // The source afxRaster.
    afxRaster               src,
    // The destination afxRaster.
    afxRaster               dst,
    // The number of regions to blit.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to blit.
    afxRasterBlit const     ops[],
    // A avxTexelFilter specifying the filter to apply if the blits require scaling.
    avxTexelFilter          flt
);

AVX afxCmdId                AvxCmdPackRaster
// Copy afxRaster data into a avxBuffer.
(
    afxDrawContext          dctx,
    // The source afxRaster.
    afxRaster               ras,
    // The destination buffer.
    avxBuffer               buf,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    afxRasterIo const       ops[]
);

AVX afxCmdId                AvxCmdUnpackRaster
// Copy data from a avxBuffer into an afxRaster.
(
    afxDrawContext          dctx,
    // The destination afxRaster.
    afxRaster               ras,
    // The source buffer.
    avxBuffer               buf,
    // The number of regions to copy.
    afxUnit                 opCnt,
    // An array of structures specifying the regions to copy.
    afxRasterIo const       ops[]
);



AVX afxCmdId                AvxCmdSubsampleRaster
// Generate mipmaps for a afxRaster.
(
    afxDrawContext          dctx,
    afxRaster               ras,
    afxUnit                 baseLod,
    afxUnit                 lodCnt
);

#endif//AVX_TRANSFERENCE_H
