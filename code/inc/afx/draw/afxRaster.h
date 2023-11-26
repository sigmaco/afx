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

#include "afx/draw/afxColor.h"
#include "afx/core/afxResource.h"

/// Textures in Qwadro are stored in as straightforward a manner as possible.
/// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
/// It then has a list of images, and each image lists its MIP levels.
/// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

typedef enum afxRasterFlag
{
    afxRasterFlag_SRC       = AfxGetBitOffset(0), /// The texture can be used as the source of a copy operation.
    afxRasterFlag_DST       = AfxGetBitOffset(1), /// The texture can be used as the destination of a copy or write operation.
    afxRasterFlag_TRANSFER  = (afxRasterFlag_SRC | afxRasterFlag_DST),
    afxRasterFlag_SAMPLING  = AfxGetBitOffset(2), /// The texture can be bound for use as a sampled texture in a shader.
    afxRasterFlag_STORAGE   = AfxGetBitOffset(3), /// The texture can be bound for use as a storage texture in a shader.
    afxRasterFlag_DRAW      = AfxGetBitOffset(4), /// The texture can be used as a color or depth/stencil attachment in a render pass.
    afxRasterFlag_USAGE    = afxRasterFlag_TRANSFER | afxRasterFlag_SAMPLING | afxRasterFlag_STORAGE | afxRasterFlag_DRAW,
#if 0
    AFX_TEX_TILING_LINEAR   = AfxGetBitOffset(9), /// specifies linear tiling (texels are laid out in memory in row-major order, possibly with some padding on each row).
    AFX_TEX_TILING_OPTIMAL  = AfxGetBitOffset(10), /// specifies optimal tiling (texels are laid out in an implementation-dependent arrangement, for more efficient memory access).
    AFX_TEX_TILING          = AFX_TEX_TILING_LINEAR | AFX_TEX_TILING_OPTIMAL,
#endif
    AFX_TEX_FLAG_CUBEMAP    = AfxGetBitOffset(30),
    AFX_TEX_FLAG_REVALIDATE = AfxGetBitOffset(31)
} afxRasterFlags;

typedef enum afxRasterOpenFlags
{
    AFX_TEX_OPEN_R  = AfxGetBitOffset(0),
    AFX_TEX_OPEN_W  = AfxGetBitOffset(1),
    AFX_TEX_OPEN_RW = (AFX_TEX_OPEN_R | AFX_TEX_OPEN_W),
    AFX_TEX_OPEN_X  = AfxGetBitOffset(2) /// especial caso que força a reconstrução do recurso junto ao OpenGL
} afxRasterOpenFlags;

AFX_DEFINE_STRUCT(afxRasterRegion)
{
    afxNat                  lodIdx;
    afxNat                  baseLayer;
    afxNat                  layerCnt;
    afxWhd                  offset;
    afxWhd                  extent;
};

AFX_DEFINE_STRUCT(afxRasterInfo)
{
    afxWhd                  whd;
    afxPixelFormat          fmt;
    afxNat                  lodCnt;
    afxNat                  layerCnt;
    afxNat                  sampleCnt;
    afxFlags                usage;
};

AFX_DEFINE_STRUCT(afxRasterIo)
{
    
    afxNat      lodIdx; // the mipmap level to copy
    afxNat      baseLayer; // the starting layer to copy.
    afxNat      layerCnt; // the number of layers to copy.
    afxNat      offset[3]; // the initial x, y, z offsets in texels of the sub-region of the source or destination image data.
    afxWhd      extent; // the size in texels of the image to copy in width, height and depth.
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_RASTER_C
AFX_OBJECT(afxRaster)
#else
struct afxBaseRaster
#endif
{
    afxRasterFlags         flags;
    afxPixelFormat          fmt;
    afxWhd                  whd; // extent of image
    afxNat                  lodCnt; // mip level cnt
    afxNat                  layerCnt;
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxColorSwizzling const*swizzling;
    afxByte*                maps;
    afxError(*map)(afxRaster, afxRasterRegion const *rgn, afxRasterOpenFlags flags, afxNat *siz, void**ptr);
    afxError(*unmap)(afxRaster, afxRasterRegion const *rgn);
};
#endif

AFX afxError        AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster ras[]);

AFX afxError        AfxLoadRastersFromTarga(afxDrawContext dctx, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster ras[]);

AFX afxRaster       AfxAssembleRaster(afxDrawContext dctx, afxRasterFlags flags, afxNat cnt, afxUri const uri[]);
AFX afxError        AfxAssembleRastersFromTarga(afxDrawContext dctx, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster* ras);
AFX afxRaster       AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterFlags flags, afxUri const uri[6]);

AFX void            AfxGetRasterExtent(afxRaster ras, afxNat lodIdx, afxWhd extent);

AFX afxPixelFormat  AfxGetRasterFormat(afxRaster ras);
AFX afxNat          AfxCountRasterLods(afxRaster ras);
AFX afxNat          AfxCountRasterLayers(afxRaster ras);
AFX afxNat          AfxGetRasterSampleCount(afxRaster ras);
AFX afxNat          AfxMeasureRasterRow(afxRaster ras);
AFX afxNat          AfxMeasureRasterLayer(afxRaster ras);

AFX afxBool         AfxGetRasterSwizzling(afxRaster ras, afxColorSwizzling const** csw);
AFX void            AfxFlipRasterVertically(afxRaster ras);
AFX afxError        AfxSwapRasterChannels(afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxRasterRegion const *rgn);

AFX afxResult       AfxTestRaster(afxRaster ras, afxRasterFlags bitmask);
AFX void            AfxFlagRaster(afxRaster ras, afxRasterFlags bitmask);
AFX void            AfxUnflagRaster(afxRaster ras, afxRasterFlags bitmask);

AFX afxError        AfxGenerateRasterLods(afxRaster ras, afxNat firstLod, afxNat lodCnt);

AFX afxError        AfxBufferizeRaster(afxRaster ras);
AFX afxNat          AfxMeasureRasterRegion(afxRaster ras, afxRasterRegion const *rgn);
AFX afxNat          AfxLocateRasterRegion(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxWhd const offset);

AFX void*           AfxOpenRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxRasterOpenFlags flags, afxNat *size);
AFX void            AfxCloseRasterRegion(afxRaster ras, afxRasterRegion const *rgn);

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

// Update texture image data from arbitrary raw allocation. A safe way of copying.
AFX afxError                    AfxUpdateRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, void const *src, afxNat siz, afxPixelFormat fmt);
AFX afxError                    AfxUpdateRasterRegion(afxRaster ras, afxRasterRegion const *rgn, void const *src, afxNat siz, afxPixelFormat fmt);
AFX afxError                    AfxUpdateRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], void const *src[], afxNat const siz[], afxPixelFormat const fmt[]);

// Copy texture image data from another texture.
AFX afxError                    AfxCopyRaster(afxRaster ras, afxRaster src);
AFX afxError                    AfxCopyRasterRegion(afxRaster ras, afxRaster src, afxRasterRegion const *rgn);
AFX afxError                    AfxCopyRasterRegions(afxRaster ras, afxRaster src, afxNat cnt, afxRasterRegion const rgn[]);

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

// Stream in texture image data from a stream.
AFX afxError                    AfxInputRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxStream *in);
AFX afxError                    AfxInputRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxStream *in);
AFX afxError                    AfxInputRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxStream *in);

// Stream in texture image data from a file.
AFX afxError                    AfxFetchRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError                    AfxFetchRasterFromTarga(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError                    AfxFetchRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxUri const *uri);
AFX afxError                    AfxFetchRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);
AFX afxError                    AfxFetchRasterRegionsFromTarga(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

// Stream out texture image data to a stream.
AFX afxError                    AfxOutputRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxStream *out);
AFX afxError                    AfxOutputRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxStream *out);
AFX afxError                    AfxOutputRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxStream *out);

// Stream out texture image data to a file.
AFX afxError                    AfxPrintRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError                    AfxPrintRasterToTarga(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri);
AFX afxError                    AfxPrintRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxUri const *uri);
AFX afxError                    AfxPrintRasterRegions(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);
AFX afxError                    AfxPrintRasterRegionsToTarga(afxRaster ras, afxNat cnt, afxRasterRegion const rgn[], afxUri const uri[]);

#endif//AFX_RASTER_H
