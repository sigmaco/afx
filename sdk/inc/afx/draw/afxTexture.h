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

#ifndef AFX_TEXTURE_H
#define AFX_TEXTURE_H

#include "afx/draw/afxColor.h"
#include "afx/core/afxResource.h"

/// Textures in Qwadro are stored in as straightforward a manner as possible.
/// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
/// It then has a list of images, and each image lists its MIP levels.
/// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

typedef enum afxEventTexture
{
    AFX_EVENT_TEX_RESIZ
} afxEventTexture;

typedef enum afxTextureFlag
{
    afxTextureFlag_SRC       = AfxGetBitOffset(0), /// The texture can be used as the source of a copy operation.
    afxTextureFlag_DST       = AfxGetBitOffset(1), /// The texture can be used as the destination of a copy or write operation.
    afxTextureFlag_TRANSFER  = (afxTextureFlag_SRC | afxTextureFlag_DST),
    afxTextureFlag_SAMPLING  = AfxGetBitOffset(2), /// The texture can be bound for use as a sampled texture in a shader.
    afxTextureFlag_STORAGE   = AfxGetBitOffset(3), /// The texture can be bound for use as a storage texture in a shader.
    afxTextureFlag_DRAW      = AfxGetBitOffset(4), /// The texture can be used as a color or depth/stencil attachment in a render pass.
    afxTextureFlag_USAGE    = afxTextureFlag_TRANSFER | afxTextureFlag_SAMPLING | afxTextureFlag_STORAGE | afxTextureFlag_DRAW,

    AFX_TEX_TILING_LINEAR   = AfxGetBitOffset(9), /// specifies linear tiling (texels are laid out in memory in row-major order, possibly with some padding on each row).
    AFX_TEX_TILING_OPTIMAL  = AfxGetBitOffset(10), /// specifies optimal tiling (texels are laid out in an implementation-dependent arrangement, for more efficient memory access).
    AFX_TEX_TILING          = AFX_TEX_TILING_LINEAR | AFX_TEX_TILING_OPTIMAL,

    AFX_TEX_FLAG_CUBEMAP    = AfxGetBitOffset(30),
    AFX_TEX_FLAG_REVALIDATE = AfxGetBitOffset(31)
} afxTextureFlags;

typedef enum afxTextureOpenFlags
{
    AFX_TEX_OPEN_R  = AfxGetBitOffset(0),
    AFX_TEX_OPEN_W  = AfxGetBitOffset(1),
    AFX_TEX_OPEN_RW = (AFX_TEX_OPEN_R | AFX_TEX_OPEN_W),
    AFX_TEX_OPEN_X  = AfxGetBitOffset(2) /// especial caso que força a reconstrução do recurso junto ao OpenGL
} afxTextureOpenFlags;

AFX_DEFINE_STRUCT(afxTextureRegion)
{
    afxNat                  lodIdx;
    afxNat                  baseImg;
    afxNat                  imgCnt;
    afxWhd                  offset;
    afxWhd                  extent;
};

AFX_DEFINE_STRUCT(afxTextureInfo)
{
    afxWhd                  whd;
    afxPixelFormat          fmt;
    afxNat                  lodCnt;
    afxNat                  imgCnt;
    afxNat                  sampleCnt;
    afxFlags                usage;
};

AFX_DEFINE_STRUCT(afxTextureBuilder)
{
    void                    (*GetInfo)(void* data, afxTextureInfo* info);
    void                    (*GetImage)(void* data, afxTexture tex, afxNat lodIdx, afxNat imgIdx, afxNat imgCnt);
};

AFX_DEFINE_STRUCT(afxTextureIo)
{
    
    afxNat      lodIdx; // the mipmap level to copy
    afxNat      baseImg; // the starting layer to copy.
    afxNat      imgCnt; // the number of layers to copy.
    afxNat      offset[3]; // the initial x, y, z offsets in texels of the sub-region of the source or destination image data.
    afxWhd      extent; // the size in texels of the image to copy in width, height and depth.
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_TEXTURE_C
AFX_OBJECT(afxTexture)
#else
struct afxBaseTexture
#endif
{
    afxTextureFlags         flags;
    afxPixelFormat          fmt;
    afxWhd                  whd; // extent of image
    afxNat                  lodCnt; // mip level cnt
    afxNat                  imgCnt;
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxColorSwizzling const*swizzling;
    afxByte*                maps;
    afxError(*map)(afxTexture, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *siz, void**ptr);
    afxError(*unmap)(afxTexture, afxTextureRegion const *rgn);
};
#endif

AFX afxError        AfxAcquireTextures(afxDrawContext dctx, afxNat cnt, afxTextureInfo const info[], afxTexture tex[]);
AFX afxError        AfxBuildTextures(afxDrawContext dctx, afxTextureBuilder const* texb, afxNat cnt, void* data[], afxTexture tex[]);

AFX afxError        AfxLoadTexturesFromTarga(afxDrawContext dctx, afxTextureFlags flags, afxNat cnt, afxUri const uri[], afxTexture tex[]);

AFX afxTexture      AfxAssembleTexture(afxDrawContext dctx, afxTextureFlags flags, afxNat cnt, afxUri const uri[]);
AFX afxError        AfxAssembleTexturesFromTarga(afxDrawContext dctx, afxTextureFlags flags, afxNat cnt, afxUri const uri[], afxTexture* tex);
AFX afxTexture      AfxAssembleCubemapTexture(afxDrawContext dctx, afxTextureFlags flags, afxUri const uri[6]);

AFX void            AfxGetTextureExtent(afxTexture tex, afxNat lodIdx, afxWhd extent);
#if 0
AFX afxError        AfxReadjustTexture(afxTexture tex, afxWhd const extent);
#endif

AFX afxPixelFormat  AfxGetTextureFormat(afxTexture tex);
AFX afxNat          AfxGetTextureLodCount(afxTexture tex);
AFX afxNat          AfxGetTextureImageCount(afxTexture tex);
AFX afxNat          AfxGetTextureSampleCount(afxTexture tex);

AFX afxBool         AfxTextureGetColorSwizzling(afxTexture tex, afxColorSwizzling const** csw);
AFX afxError        AfxTextureSwapChannels(afxTexture tex, afxColorSwizzle a, afxColorSwizzle b, afxTextureRegion const *rgn);

AFX afxResult       AfxTestTexture(afxTexture tex, afxTextureFlags bitmask);
AFX void            AfxFlagTexture(afxTexture tex, afxTextureFlags bitmask);
AFX void            AfxUnflagTexture(afxTexture tex, afxTextureFlags bitmask);

AFX afxError        AfxGenerateTextureLods(afxTexture tex, afxNat firstLod, afxNat lodCnt);

AFX afxError        AfxBufferizeTexture(afxTexture tex);
AFX afxNat          AfxMeasureTextureRegion(afxTexture tex, afxTextureRegion const *rgn);
AFX afxNat          AfxLocateTextureRegion(afxTexture tex, afxNat lodIdx, afxNat imgIdx, afxWhd const offset);

AFX void*           AfxOpenTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *size);
AFX void            AfxCloseTextureRegion(afxTexture tex, afxTextureRegion const *rgn);

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

// Update texture image data from arbitrary raw allocation. A safe way of copying.
AFX afxError                    AfxUpdateTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, void const *src, afxNat siz, afxPixelFormat fmt);
AFX afxError                    AfxUpdateTextureRegion(afxTexture tex, afxTextureRegion const *rgn, void const *src, afxNat siz, afxPixelFormat fmt);
AFX afxError                    AfxUpdateTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], void const *src[], afxNat const siz[], afxPixelFormat const fmt[]);

// Copy texture image data from another texture.
AFX afxError                    AfxCopyTexture(afxTexture tex, afxTexture src);
AFX afxError                    AfxCopyTextureRegion(afxTexture tex, afxTexture src, afxTextureRegion const *rgn);
AFX afxError                    AfxCopyTextureRegions(afxTexture tex, afxTexture src, afxNat cnt, afxTextureRegion const rgn[]);

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

// Stream in texture image data from a stream.
AFX afxError                    AfxInputTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxStream *in);
AFX afxError                    AfxInputTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxStream *in);
AFX afxError                    AfxInputTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxStream *in);

// Stream in texture image data from a file.
AFX afxError                    AfxFetchTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
AFX afxError                    AfxFetchTextureFromTarga(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
AFX afxError                    AfxFetchTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxUri const *uri);
AFX afxError                    AfxFetchTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
AFX afxError                    AfxFetchTextureRegionsFromTarga(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

// Stream out texture image data to a stream.
AFX afxError                    AfxOutputTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxStream *out);
AFX afxError                    AfxOutputTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxStream *out);
AFX afxError                    AfxOutputTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxStream *out);

// Stream out texture image data to a file.
AFX afxError                    AfxPrintTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
AFX afxError                    AfxPrintTextureToTarga(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
AFX afxError                    AfxPrintTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxUri const *uri);
AFX afxError                    AfxPrintTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
AFX afxError                    AfxPrintTextureRegionsToTarga(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#endif//AFX_TEXTURE_H