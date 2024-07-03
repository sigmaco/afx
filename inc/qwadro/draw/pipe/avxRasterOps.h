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

#ifndef AVX_RASTER_OPS_H
#define AVX_RASTER_OPS_H

#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/pipe/avxSampler.h"

AFX_DEFINE_UNION(afxClearValue)
{
    union
    {
        afxV4d      color;
        afxInt      colori[4];
        afxNat      colorn[4];
    };
    struct
    {
        afxReal     depth; /// default is 1.f
        afxNat      stencil; /// default is 0
    };
};

AFX_DEFINE_STRUCT(afxRasterBlit)
/// Structure specifying an afxRaster blit operation.
{
    afxRasterRegion src;
    afxRasterRegion dst;
};

AFX_DEFINE_STRUCT(afxRasterSubset)
/// Structure specifying an afxRaster subresource range.
{
    afxNat          baseLod; /// the first mipmap level accessible to the view.
    afxNat          lodCnt; /// the number of mipmap levels (starting from @baseLodIdx) accessible to the view.
    afxNat          baseLayer; /// the first array layer accessible to the view.
    afxNat          layerCnt; /// the number of array layers (starting from @baseLayer) accessible to the view.
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDOS                                                            ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId                AvxCmdPackRaster
/// Copy afxRaster data into a afxBuffer.
(
    avxCmdb            cmdb,
    afxRaster               ras, /// the source afxRaster.
    afxRasterIo const*      op, /// the structure specifying the region to pack.
    afxBuffer               buf /// the destination buffer.
);

AVX afxCmdId                AvxCmdPackRasterRegions
/// Copy afxRaster data into a afxBuffer.
(
    avxCmdb            cmdb,
    afxRaster               ras, /// the source afxRaster.
    afxBuffer               buf, /// the destination buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterIo const       ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AvxCmdUnpackRaster
/// Copy data from a afxBuffer into an afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               ras, /// the destination afxRaster.
    afxRasterIo const*      op, /// the structure specifying the region to unpack.
    afxBuffer               buf /// the source buffer.
);

AVX afxCmdId                AvxCmdUnpackRasterRegions
/// Copy data from a afxBuffer into an afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               ras, /// the destination afxRaster.
    afxBuffer               buf, /// the source buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterIo const       ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AvxCmdCopyRaster
/// Copy data between afxRaster's.
(
    avxCmdb            cmdb,
    afxRaster               src, /// the source afxRaster.
    afxRasterCopy const*    copy, /// the structure specifying the region to copy.
    afxRaster               dst /// the destination afxRaster.
);

AVX afxCmdId                AvxCmdCopyRasterRegions
/// Copy data between afxRaster's.
(
    avxCmdb            cmdb,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterCopy const     ops[] /// an array of structures specifying the regions to copy.
);

AVX afxCmdId                AvxCmdSwizzleRaster
(
    avxCmdb            cmdb,
    afxRaster               ras,
    avxColorSwizzle         a,
    avxColorSwizzle         b,
    afxRasterRegion const*  rgn
);

AVX afxCmdId                AvxCmdSwizzleRasterRegions
/// Permute color components of a raster.
(
    avxCmdb            cmdb,
    afxRaster               ras,
    avxColorSwizzle         a,
    avxColorSwizzle         b,
    afxNat                  rgnCnt,
    afxRasterRegion const   regions[]
);

AVX afxCmdId                AvxCmdTransformRaster
(
    avxCmdb            cmdb,
    afxRaster               ras,
    afxM4d const            m,
    afxRasterRegion const*  rgn
);

AVX afxCmdId                AvxCmdTransformRasterRegions
(
    avxCmdb            cmdb,
    afxRaster               ras,
    afxM4d const            m,
    afxNat                  rgnCnt,
    afxRasterRegion const   regions[]
);

AVX afxCmdId                AvxCmdClearRaster
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               ras, /// the afxRaster to be cleared.
    afxRasterSubset const*  subset, /// the structure describing a range of mipmap levels, array layers, and aspects to be cleared.
    afxClearValue const*    value /// a structure containing the values that the afxRaster subresource ranges will be cleared to.
);

AVX afxCmdId                AvxCmdClearRasterRegions
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               ras, /// the afxRaster to be cleared.
    afxNat                  subsetCnt, /// the number of afxRaster subresource ranges.
    afxRasterSubset const   subsets[], /// an array of structures describing a range of mipmap levels, array layers, and aspects to be cleared.
    afxClearValue const*    value /// a structure containing the values that the afxRaster subresource ranges will be cleared to.
);

AVX afxCmdId                AvxCmdSubsampleRaster
/// Generate mipmaps for a afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               ras,
    afxNat                  baseLod,
    afxNat                  lodCnt
);

AVX afxCmdId                AvxCmdResolveRaster
/// Resolve regions of an afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               src, /// the source afxRaster.
    afxRasterCopy const*    copy, /// the structure specifying the region to resolve.
    afxRaster               dst /// the destination afxRaster.
);

AVX afxCmdId                AvxCmdResolveRasterRegions
/// Resolve regions of an afxRaster.
(
    avxCmdb            cmdb,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxNat                  opCnt, /// the number of regions to resolve.
    afxRasterCopy const     ops[] /// an array of structures specifying the regions to resolve.
);

AVX afxCmdId                AvxCmdBlitRaster
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    avxCmdb            cmdb,
    afxRaster               src, /// the source afxRaster.
    afxRasterBlit const*    blit, /// the structure specifying the region to blit.
    afxRaster               dst, /// the destination afxRaster.
    avxTexelFilter          flt /// a avxTexelFilter specifying the filter to apply if the blits require scaling.
);

AVX afxCmdId                AvxCmdBlitRasterRegions
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    avxCmdb            cmdb,
    afxRaster               src, /// the source afxRaster.
    afxRaster               dst, /// the destination afxRaster.
    afxNat                  opCnt, /// the number of regions to blit.
    afxRasterBlit const     ops[], /// an array of structures specifying the regions to blit.
    avxTexelFilter          flt /// a avxTexelFilter specifying the filter to apply if the blits require scaling.
);

#endif//AVX_RASTER_OPS_H
