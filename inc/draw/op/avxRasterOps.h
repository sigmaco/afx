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

#ifndef AVX_RASTER_OPS_H
#define AVX_RASTER_OPS_H

//#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/math/afxWhd.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"

AFX_DEFINE_UNION(avxClearValue)
{
    union
    {
        afxV4d  rgba;
        afxInt  rgbai[4];
        afxUnit rgban[4];
    };
    struct
    {
        afxReal depth; /// default is 1.f
        afxUnit stencil; /// default is 0
    };
};

AFX_DEFINE_STRUCT(afxRasterBlock)
/// Structure specifying an afxRaster subresource range.
{
    afxUnit baseLod; /// the first mipmap level accessible to the view.
    afxUnit lodCnt; /// the number of mipmap levels (starting from @baseLodIdx) accessible to the view.
    afxUnit baseLayer; /// the first array layer accessible to the view.
    afxUnit layerCnt; /// the number of array layers (starting from @baseLayer) accessible to the view.
};

AFX_DEFINE_STRUCT(afxRasterRegion)
{
    afxUnit lodIdx;
    afxWhd  origin; /// is the initial x, y, z offsets in texels of the sub-region of the source or destination afxRaster data.
    afxWhd  extent; /// is the size in texels of the afxRaster to copy in width, height and depth.
};

AFX_DEFINE_STRUCT(afxRasterCopy)
/// Especificação de operação de cópia de afxRaster.
{
    afxRasterRegion src;
    afxUnit         dstLodIdx;
    afxWhd          dstOrigin; /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination afxRaster data.
};

AFX_DEFINE_STRUCT(afxRasterBlit)
/// Structure specifying an afxRaster blit operation.
{
    afxRasterRegion src;
    afxRasterRegion dst;
};

AFX_DEFINE_STRUCT(afxRasterIo)
/// Especificação de operação de transferência arbitrária de afxRaster.
{
    afxRasterRegion rgn;
    afxSize         offset; /// is the offset in bytes from the start of the buffer object where the afxRaster data is copied from or to.
    afxUnit         rowStride; /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxUnit         rowsPerImg; /// (aka rows per layer) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    ///  rowsPerImage * rowStride is the stride, in bytes, between the beginning of each texel image of data and the subsequent texel image.
    afxCodec        codec;
    afxUnit         decSiz;
    afxUnit         encSiz;
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDOS                                                            ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AvxCmdClearRaster
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    afxDrawContext          dctx,
    afxRaster               ras, /// the afxRaster to be cleared.
    avxClearValue const*    value, /// a structure containing the values that the afxRaster subresource ranges will be cleared to.
    afxUnit                 opCnt, /// the number of afxRaster subresource ranges.
    afxRasterBlock const    ops[] /// an array of structures describing a range of mipmap levels, array layers, and aspects to be cleared.
);

AVX afxCmdId                AvxCmdCopyRaster
/// Copy data between afxRaster's.
(
    afxDrawContext          dctx,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxUnit                 opCnt, /// the number of regions to copy.
    afxRasterCopy const     ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AvxCmdResolveRaster
/// Resolve regions of an afxRaster.
(
    afxDrawContext          dctx,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxUnit                 opCnt, /// the number of regions to resolve.
    afxRasterCopy const     ops[] /// an array of structures specifying the regions to resolve.
);

AVX afxCmdId                AvxCmdBlitRaster
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    afxDrawContext          dctx,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxUnit                 opCnt, /// the number of regions to blit.
    afxRasterBlit const     ops[], /// an array of structures specifying the regions to blit.
    avxTexelFilter          flt /// a avxTexelFilter specifying the filter to apply if the blits require scaling.
);

AVX afxCmdId                AvxCmdPackRaster
/// Copy afxRaster data into a afxBuffer.
(
    afxDrawContext          dctx,
    afxRaster               ras, /// the source afxRaster.
    afxBuffer               buf, /// the destination buffer.
    afxUnit                 opCnt, /// the number of regions to copy.
    afxRasterIo const       ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AvxCmdUnpackRaster
/// Copy data from a afxBuffer into an afxRaster.
(
    afxDrawContext          dctx,
    afxRaster               ras, /// the destination afxRaster.
    afxBuffer               buf, /// the source buffer.
    afxUnit                 opCnt, /// the number of regions to copy.
    afxRasterIo const       ops[] /// an array of structures specifying the regions to copy.
);



AVX afxCmdId                AvxCmdSubsampleRaster
/// Generate mipmaps for a afxRaster.
(
    afxDrawContext          dctx,
    afxRaster               ras,
    afxUnit                 baseLod,
    afxUnit                 lodCnt
);

#endif//AVX_RASTER_OPS_H
