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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_RASTER_H
#define AFX_RASTER_H

#include "qwadro/draw/afxColor.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/pipe/afxSampler.h"

/// Textures in Qwadro are stored in as straightforward a manner as possible.
/// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
/// It then has a list of images, and each image lists its MIP levels.
/// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

typedef enum afxRasterFlag
{
#if !0
    afxRasterFlag_LINEAR_TILING = AfxGetBitOffset(9), /// specifies linear tiling (texels are laid out in memory in row-major order, possibly with some padding on each row).
    //afxRasterFlag_OPTIMAL  = AfxGetBitOffset(10), /// specifies optimal tiling (texels are laid out in an implementation-dependent arrangement, for more efficient memory access).
    //afxRasterFlag          = afxRasterFlag_LINEAR | afxRasterFlag_OPTIMAL,
#endif
    afxRasterFlag_CUBEMAP       = AfxGetBitOffset(30),
    afxRasterFlag_REVALIDATE    = AfxGetBitOffset(31)
} afxRasterFlags;

typedef enum afxRasterUsage
{
    afxRasterUsage_SRC      = AfxGetBitOffset(0), /// The texture can be used as the source of a copy operation.
    afxRasterUsage_DST      = AfxGetBitOffset(1), /// The texture can be used as the destination of a copy or write operation.
    afxRasterUsage_TRANSFER = (afxRasterUsage_SRC | afxRasterUsage_DST),
    afxRasterUsage_SAMPLING = AfxGetBitOffset(2), /// The texture can be bound for use as a sampled texture in a shader.
    afxRasterUsage_STORAGE  = AfxGetBitOffset(3), /// The texture can be bound for use as a storage texture in a shader.
    afxRasterUsage_DRAW     = AfxGetBitOffset(4), /// The texture can be used as a color or depth/stencil attachment in a render pass.
    
    afxRasterFlag_USAGE     = afxRasterUsage_TRANSFER | afxRasterUsage_SAMPLING | afxRasterUsage_STORAGE | afxRasterUsage_DRAW,
} afxRasterUsage;

typedef enum afxRasterAccess
{
    afxRasterAccess_R   = AfxGetBitOffset(0),
    afxRasterAccess_W   = AfxGetBitOffset(1),
    afxRasterAccess_RW  = (afxRasterAccess_R | afxRasterAccess_W),
    afxRasterAccess_X   = AfxGetBitOffset(2) /// especial caso que força a reconstrução do recurso junto ao OpenGL
} afxRasterAccess;

AFX_DEFINE_UNION(afxClearValue)
{
    union
    {
        afxV4d          color;
        afxInt          colori[4];
        afxNat          colorn[4];
    };
    struct
    {
        afxReal         depth; /// default is 1.f
        afxNat          stencil; /// default is 0
    };
};

AFX_DEFINE_STRUCT(afxRasterRegion)
{
    afxNat                  lodIdx;
    afxNat                  baseLayer;
    afxNat                  layerCnt;
    afxWhd                  offset; /// is the initial x, y, z offsets in texels of the sub-region of the source or destination image data.
    afxWhd                  whd; /// is the size in texels of the image to copy in width, height and depth.
};

AFX_DEFINE_STRUCT(afxRasterIoOp)
/// Especificação de operação de transferência arbitrária de afxRaster.
{
    afxRasterRegion         rgn;
    afxSize                 bufOffset; /// is the offset in bytes from the start of the buffer object where the image data is copied from or to.
    afxNat                  bufRowSiz; /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional image in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxNat                  bufRowCnt; /// (aka rows per image) specify in texels a subregion of a larger two- or three-dimensional image in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
};

AFX_DEFINE_STRUCT(afxRasterCopyOp)
/// Especificação de operação de cópia de afxRaster.
{
    afxNat                  srcLodIdx;
    afxNat                  srcBaseLayer;
    afxNat                  srcLayerCnt;
    afxWhd                  srcOffset; /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination image data.
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
    afxNat                  baseLodIdx; /// the first mipmap level accessible to the view.
    afxNat                  lodCnt; /// the number of mipmap levels (starting from @baseLodIdx) accessible to the view.
    afxNat                  baseLayer; /// the first array layer accessible to the view.
    afxNat                  layerCnt; /// the number of array layers (starting from @baseLayer) accessible to the view.
};

AFX_DEFINE_STRUCT(afxRasterInfo)
{
    afxNat                  lodCnt;
    afxNat                  layerCnt;
    afxWhd                  whd;
    afxPixelFormat          fmt;
    afxNat                  sampleCnt;
    afxRasterUsage          usage;
    afxRasterFlags          flags;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_RASTER_C
#ifndef _AFX_RASTER_IMPL
AFX_OBJECT(afxRaster)
#else
struct afxBaseRaster
#endif
{
    afxNat                  lodCnt; // mip level cnt
    afxNat                  layerCnt;
    afxWhd                  whd; // extent of image
    
    afxPixelFormat          fmt;
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxColorSwizzling const*swizzling;
    afxByte*                maps;
    afxByte*                sidemap; // aux map (usually for indexed/palettized)
    
    afxRasterFlags          flags;
    afxRasterUsage          usage;

    afxError                (*map)(afxRaster, afxRasterRegion const *rgn, afxRasterAccess flags, afxNat *siz, afxNat* rowSiz, void**ptr);
    afxError                (*unmap)(afxRaster, afxRasterRegion const *rgn);
};
#endif
#endif//_AFX_DRAW_C

AFX void            AfxGetRasterExtent(afxRaster ras, afxNat lodIdx, afxWhd whd);

AFX afxPixelFormat  AfxGetRasterFormat(afxRaster ras);

AFX afxNat          AfxCountRasterLods(afxRaster ras);
AFX afxNat          AfxCountRasterLayers(afxRaster ras);
AFX afxNat          AfxCountRasterSamples(afxRaster ras);

AFX afxNat          AfxMeasureRasterRow(afxRaster ras, afxNat lodIdx);
AFX afxNat          AfxMeasureRasterLayer(afxRaster ras, afxNat lodIdx);

AFX afxBool         AfxGetRasterSwizzling(afxRaster ras, afxColorSwizzling const** csw);

AFX afxRasterUsage  AfxGetRasterUsage(afxRaster ras);
AFX afxRasterFlags  AfxGetRasterFlags(afxRaster ras);

AFX afxRasterUsage  AfxTestRasterUsage(afxRaster ras, afxRasterUsage bitmask);
AFX afxRasterFlags  AfxTestRasterFlags(afxRaster ras, afxRasterFlags bitmask);

AFX afxError        AfxBufferizeRaster(afxRaster ras);
AFX afxNat          AfxMeasureRasterRegion(afxRaster ras, afxRasterRegion const *rgn);
AFX afxNat          AfxGetRasterOffset(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxWhd const offset);

AFX void*           AfxOpenRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxRasterAccess flags, afxNat* rgnSiz, afxNat* rowSiz);
AFX void            AfxCloseRasterRegion(afxRaster ras, afxRasterRegion const *rgn);

// Update texture image data from arbitrary raw allocation. A safe way of copying.
AFX afxError        AfxUpdateRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], void const *src);
AFX afxError        AfxDumpRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], void *dst);

// Stream in/out texture image data from a stream.
AFX afxError        AfxInputRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxStream in);
AFX afxError        AfxOutputRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxStream out);

// Stream in texture image data from a file.
AFX afxError        AfxFetchRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError        AfxFetchRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[]);

AFX afxError        AfxFetchRasterFromTarga(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError        AfxFetchRasterRegionsFromTarga(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[]);

// Stream out texture image data to a file.
AFX afxError        AfxPrintRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError        AfxPrintRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[]);

AFX afxError        AfxPrintRasterToTarga(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError        AfxPrintRasterRegionsToTarga(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster ras[]);

AFX afxError        AfxLoadRastersFromTarga(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster ras[]);

AFX afxRaster       AfxAssembleRaster(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[]);
AFX afxError        AfxAssembleRastersFromTarga(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster* ras);
AFX afxRaster       AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const uri[6]);

AFX afxError        AfxReadjustRasters(afxDrawInput din, afxWhd const whd, afxNat cnt, afxRaster rasters[]);

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AFX afxCmdId                AfxCmdPackRaster
/// Copy afxRaster data into a afxBuffer.
(
    afxDrawScript           dscr,
    afxRaster               ras, /// the source image.
    afxBuffer               buf, /// the destination buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterIoOp const     ops[] /// an array of structures specifying the regions to copy.
);

AFX afxCmdId                AfxCmdPackRasterRegion
/// Copy afxRaster data into a afxBuffer.
(
    afxDrawScript           dscr,
    afxRaster               ras, /// the source image.
    afxBuffer               buf, /// the destination buffer.
    afxRasterRegion const*  rgn,
    afxSize                 bufOffset, /// is the offset in bytes from the start of the buffer object where the image data is copied from or to.
    afxNat                  bufRowSiz, /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional image in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxNat                  bufRowCnt /// (aka rows per image) specify in texels a subregion of a larger two- or three-dimensional image in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
);

AFX afxCmdId                AfxCmdUnpackRaster
/// Copy data from a afxBuffer into an afxRaster.
(
    afxDrawScript           dscr,
    afxRaster               ras, /// the destination image.
    afxBuffer               buf, /// the source buffer.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterIoOp const     ops[] /// an array of structures specifying the regions to copy.
);

AFX afxCmdId                AfxCmdUnpackRasterRegion
/// Copy data from a afxBuffer into an afxRaster.
(
    afxDrawScript           dscr,
    afxRaster               ras, /// the destination image.
    afxBuffer               buf, /// the source buffer.
    afxRasterRegion const*  rgn,
    afxSize                 bufOffset, /// is the offset in bytes from the start of the buffer object where the image data is copied from or to.
    afxNat                  bufRowSiz, /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional image in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxNat                  bufRowCnt /// (aka rows per image) specify in texels a subregion of a larger two- or three-dimensional image in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
);

AFX afxCmdId                AfxCmdCopyRaster
/// Copy data between afxRaster's.
(
    afxDrawScript           dscr,
    afxRaster               src, /// the source image.
    afxRaster               dst, /// the destination image.
    afxNat                  opCnt, /// the number of regions to copy.
    afxRasterCopyOp const   ops[] /// an array of structures specifying the regions to copy.
);

AFX afxCmdId                AfxCmdCopyRasterRegion
/// Copy data between afxRaster's.
(
    afxDrawScript           dscr,
    afxRaster               src, /// the source image.
    afxNat                  srcLodIdx,
    afxNat                  srcBaseLayer,
    afxNat                  srcLayerCnt,
    afxWhd                  srcOffset, /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination image data.
    afxRaster               dst, /// the destination image.
    afxRasterRegion const*  dstRgn
);

AFX afxCmdId                AfxCmdRegenerateMipmaps
(
    afxDrawScript           dscr,
    afxRaster               ras,
    afxNat                  baseLodIdx,
    afxNat                  lodCnt
);

/// When you need to permute color components of a raster, call;
/// SwizzleRasters(din, afxColorSwizzle_R, afxColorSwizzle_B, &rgn, 1, &ras)

AFX afxCmdId                AfxCmdSwizzleRaster
(
    afxDrawScript           dscr,
    afxRaster               ras,
    afxColorSwizzle         a,
    afxColorSwizzle         b,
    afxNat                  rgnCnt,
    afxRasterRegion const   rgns[]
);

AFX afxCmdId                AfxCmdSwizzleRasterRegion
(
    afxDrawScript           dscr,
    afxRaster               ras,
    afxColorSwizzle         a,
    afxColorSwizzle         b,
    afxRasterRegion const*  rgn
);

AFX afxCmdId                AfxCmdFlipRaster
(
    afxDrawScript           dscr,
    afxRaster               ras,
    afxBool                 rows,
    afxBool                 columns,
    afxNat                  rgnCnt,
    afxRasterRegion const   rgns[]
); // TODO flip flags

AFX afxCmdId                AfxCmdFlipRasterRegion
(
    afxDrawScript           dscr,
    afxRaster               ras,
    afxBool                 rows,
    afxBool                 columns,
    afxRasterRegion const*  rgn
); // TODO flip flags

AFX afxCmdId                AfxCmdClearRaster
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    afxDrawScript           dscr,
    afxRaster               ras, /// the image to be cleared.
    afxClearValue const*    value, /// a structure containing the values that the image subresource ranges will be cleared to.
    afxNat                  subsetCnt, /// the number of image subresource ranges.
    afxRasterSubset const   subsets[] /// an array of structures describing a range of mipmap levels, array layers, and aspects to be cleared.
);

AFX afxCmdId                AfxCmdClearRasterRegion
/// Clear regions of a color afxRaster or fill regions of a combined depth/stencil afxRaster.
(
    afxDrawScript           dscr,
    afxRaster               ras, /// the image to be cleared.
    afxClearValue const*    value, /// a structure containing the values that the image subresource ranges will be cleared to.
    afxNat                  baseLodIdx, /// the first mipmap level accessible to the view.
    afxNat                  lodCnt, /// the number of mipmap levels (starting from @baseLodIdx) accessible to the view.
    afxNat                  baseLayer, /// the first array layer accessible to the view.
    afxNat                  layerCnt /// the number of array layers (starting from @baseLayer) accessible to the view.
);

AFX afxCmdId                AfxCmdResolveRaster
/// Resolve regions of an image.
(
    afxDrawScript           dscr,
    afxRaster               src, /// the source image.
    afxRaster               dst, /// the destination image.
    afxNat                  opCnt, /// the number of regions to resolve.
    afxRasterCopyOp const   ops[] /// an array of structures specifying the regions to resolve.
);

AFX afxCmdId                AfxCmdResolveRasterRegion
/// Resolve regions of an image.
(
    afxDrawScript           dscr,
    afxRaster               src, /// the source image.
    afxNat                  srcLodIdx,
    afxNat                  srcBaseLayer,
    afxNat                  srcLayerCnt,
    afxWhd                  srcOffset, /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination image data.
    afxRaster               dst, /// the destination image.
    afxRasterRegion const*  dstRgn
);

AFX afxCmdId                AfxCmdBlitRaster
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    afxDrawScript           dscr,
    afxRaster               src, /// the source image.
    afxRaster               dst, /// the destination image.
    afxNat                  opCnt, /// the number of regions to blit.
    afxRasterBlitOp const   ops[], /// an array of structures specifying the regions to blit.
    afxTexelFilter          flt /// a afxTexelFilter specifying the filter to apply if the blits require scaling.
);

AFX afxCmdId                AfxCmdBlitRasterRegion
/// Copy regions of an afxRaster, potentially performing format conversion.
(
    afxDrawScript           dscr,
    afxRaster               src, /// the source image.
    afxRasterRegion const*  srcRgn, /// a structures specifying the source region to blit from.
    afxRaster               dst, /// the destination image.
    afxRasterRegion const*  dstRgn, /// a structures specifying the destination region to blit to.
    afxTexelFilter          flt /// a afxTexelFilter specifying the filter to apply if the blits require scaling.
);


#endif//AFX_RASTER_H
