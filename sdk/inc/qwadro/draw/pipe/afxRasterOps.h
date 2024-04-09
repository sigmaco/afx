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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_RASTER_OPS_H
#define AFX_RASTER_OPS_H

#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/pipe/afxSampler.h"

AFX_DEFINE_UNION(afxClearValue)
{
    union
    {
        afxV4d              color;
        afxInt              colori[4];
        afxNat              colorn[4];
    };
    struct
    {
        afxReal             depth; /// default is 1.f
        afxNat              stencil; /// default is 0
    };
};

AFX_DEFINE_STRUCT(afxRasterCopyOp)
/// Especificação de operação de cópia de afxRaster.
{
    afxNat                  srcLodIdx;
    afxNat                  srcBaseLayer;
    afxNat                  srcLayerCnt;
    afxWhd                  srcOffset; /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination afxRaster data.
    afxRasterRegion         dst;
};

AFX_DEFINE_STRUCT(afxRasterBlitOp)
/// Structure specifying an afxRaster blit operation.
{
    afxRasterRegion         src;
    afxRasterRegion         dst;
};

AFX_DEFINE_STRUCT(afxRasterSubset)
/// Structure specifying an afxRaster subresource range.
{
    afxNat                  baseLod; /// the first mipmap level accessible to the view.
    afxNat                  lodCnt; /// the number of mipmap levels (starting from @baseLodIdx) accessible to the view.
    afxNat                  baseLayer; /// the first array layer accessible to the view.
    afxNat                  layerCnt; /// the number of array layers (starting from @baseLayer) accessible to the view.
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDOS                                                            ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AfxCmdInputRaster
/// Load data from a afxStream into an afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the destination afxRaster.
    afxStream               src, /// the source afxStream.
    afxNat                  opCnt, /// the number of regions to load.
    afxRasterIoOp const     ops[], /// an array of structures specifying the regions to load.
    afxCodec                cdc
);

AVX afxCmdId                AfxCmdInputRasterRegion
/// Load data from a afxStream into an afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the destination afxRaster.
    afxRasterRegion const*  rgn,
    afxStream               src, /// the source afxStream.
    afxNat                  srcOffset, /// is the offset in bytes from the start of the stream object where the afxRaster data is loaded from.
    afxNat                  srcRowSiz, /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional afxRaster in stream memory, and control the addressing calculations. If either of these values is zero, that aspect of the stream memory is considered to be tightly packed according to the imageExtent.
    afxNat                  srcRowCnt, /// (aka rows per afxRaster) specify in texels a subregion of a larger two- or three-dimensional afxRaster in stream memory, and control the addressing calculations. If either of these values is zero, that aspect of the stream memory is considered to be tightly packed according to the imageExtent.
    afxCodec                cdc
);

AVX afxCmdId                AfxCmdOutputRaster
/// Store afxRaster data into a afxStream.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the source afxRaster.
    afxStream               dst, /// the destination afxStream.
    afxNat                  opCnt, /// the number of regions to store.
    afxRasterIoOp const     ops[], /// an array of structures specifying the regions to store.
    afxCodec                cdc
);

AVX afxCmdId                AfxCmdOutputRasterRegion
/// Store afxRaster data into a afxStream.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the source afxRaster.
    afxRasterRegion const*  rgn,
    afxStream               dst, /// the destination afxStream.
    afxNat                  dstOffset, /// is the offset in bytes from the start of the stream object where the afxRaster data is stored to.
    afxNat                  dstRowSiz, /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional afxRaster in stream memory, and control the addressing calculations. If either of these values is zero, that aspect of the stream memory is considered to be tightly packed according to the imageExtent.
    afxNat                  dstRowCnt, /// (aka rows per afxRaster) specify in texels a subregion of a larger two- or three-dimensional afxRaster in stream memory, and control the addressing calculations. If either of these values is zero, that aspect of the stream memory is considered to be tightly packed according to the imageExtent.
    afxCodec                cdc
);

AVX afxCmdId                AfxCmdPackRaster
/// Copy afxRaster data into a afxBuffer.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the source afxRaster.
    afxBuffer               buf, /// the destination buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterIoOp const     ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AfxCmdPackRasterRegion
/// Copy afxRaster data into a afxBuffer.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the source afxRaster.
    afxRasterRegion const*  rgn,
    afxBuffer               buf, /// the destination buffer.
    afxNat                  bufOffset, /// is the offset in bytes from the start of the buffer object where the afxRaster data is copied to.
    afxNat                  bufRowSiz, /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxNat                  bufRowCnt /// (aka rows per afxRaster) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
);

AVX afxCmdId                AfxCmdUnpackRaster
/// Copy data from a afxBuffer into an afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the destination afxRaster.
    afxBuffer               buf, /// the source buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterIoOp const     ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AfxCmdUnpackRasterRegion
/// Copy data from a afxBuffer into an afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the destination afxRaster.
    afxRasterRegion const*  rgn,
    afxBuffer               buf, /// the source buffer.
    afxNat                  bufOffset, /// is the offset in bytes from the start of the buffer object where the afxRaster data is copied from.
    afxNat                  bufRowSiz, /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxNat                  bufRowCnt /// (aka rows per afxRaster) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
);

AVX afxCmdId                AfxCmdCopyRaster
/// Copy data between afxRaster's.
(
    afxDrawStream           diob,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterCopyOp const   ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AfxCmdCopyRasterRegion
/// Copy data between afxRaster's.
(
    afxDrawStream           diob,
    afxRaster               src, /// the source afxRaster.
    afxNat                  srcLodIdx,
    afxNat                  srcBaseLayer,
    afxNat                  srcLayerCnt,
    afxWhd                  srcOffset, /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination afxRaster data.
    afxRaster               dst, /// the destination afxRaster.
    afxRasterRegion const*  dstRgn
);

AVX afxCmdId                AfxCmdSwizzleRaster
/// Permute color components of a raster.
(
    afxDrawStream           diob,
    afxRaster               ras,
    afxColorSwizzle         a,
    afxColorSwizzle         b,
    afxNat                  rgnCnt,
    afxRasterRegion const   regions[]
);

AVX afxCmdId                AfxCmdSwizzleRasterRegion
(
    afxDrawStream           diob,
    afxRaster               ras,
    afxColorSwizzle         a,
    afxColorSwizzle         b,
    afxRasterRegion const*  rgn
);

AVX afxCmdId                AfxCmdTransformRaster
(
    afxDrawStream           diob,
    afxRaster               ras,
    afxM4d const            m,
    afxNat                  rgnCnt,
    afxRasterRegion const   regions[]
);

AVX afxCmdId                AfxCmdTransformRasterRegion
(
    afxDrawStream           diob,
    afxRaster               ras,
    afxM4d const            m,
    afxRasterRegion const*  rgn
);

AVX afxCmdId                AfxCmdClearRaster
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the afxRaster to be cleared.
    afxNat                  subsetCnt, /// the number of afxRaster subresource ranges.
    afxRasterSubset const   subsets[], /// an array of structures describing a range of mipmap levels, array layers, and aspects to be cleared.
    afxClearValue const*    value /// a structure containing the values that the afxRaster subresource ranges will be cleared to.
);

AVX afxCmdId                AfxCmdClearRasterRegion
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras, /// the afxRaster to be cleared.
    afxNat                  baseLod, /// the first mipmap level accessible to the view.
    afxNat                  lodCnt, /// the number of mipmap levels (starting from @baseLod) accessible to the view.
    afxNat                  baseLayer, /// the first array layer accessible to the view.
    afxNat                  layerCnt, /// the number of array layers (starting from @baseLayer) accessible to the view.
    afxClearValue const*    value /// a structure containing the values that the afxRaster subresource ranges will be cleared to.
);

AVX afxCmdId                AfxCmdSubsampleRaster
/// Generate mipmaps for a afxRaster.
(
    afxDrawStream           diob,
    afxRaster               ras,
    afxNat                  baseLod,
    afxNat                  lodCnt
);

AVX afxCmdId                AfxCmdResolveRaster
/// Resolve regions of an afxRaster.
(
    afxDrawStream           diob,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxNat                  opCnt, /// the number of regions to resolve.
    afxRasterCopyOp const   ops[] /// an array of structures specifying the regions to resolve.
);

AVX afxCmdId                AfxCmdResolveRasterRegion
/// Resolve regions of an afxRaster.
(
    afxDrawStream           diob,
    afxRaster               src, /// the source afxRaster.
    afxNat                  srcLodIdx,
    afxNat                  srcBaseLayer,
    afxNat                  srcLayerCnt,
    afxWhd                  srcOffset, /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination afxRaster data.
    afxRaster               dst, /// the destination afxRaster.
    afxRasterRegion const*  dstRgn
);

AVX afxCmdId                AfxCmdBlitRaster
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    afxDrawStream           diob,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxNat                  opCnt, /// the number of regions to blit.
    afxRasterBlitOp const   ops[], /// an array of structures specifying the regions to blit.
    afxTexelFilter          flt /// a afxTexelFilter specifying the filter to apply if the blits require scaling.
);

AVX afxCmdId                AfxCmdBlitRasterRegion
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    afxDrawStream           diob,
    afxRaster               src, /// the source afxRaster.
    afxRasterRegion const*  srcRgn, /// a structures specifying the source region to blit from.
    afxRaster               dst, /// the destination afxRaster.
    afxRasterRegion const*  dstRgn, /// a structures specifying the destination region to blit to.
    afxTexelFilter          flt /// a afxTexelFilter specifying the filter to apply if the blits require scaling.
);


#endif//AFX_RASTER_OPS_H
