/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_TEXTURE_H
#define AFX_TEXTURE_H

#include "afx/draw/afxColor.h"
#include "afx/core/io/afxResource.h"

/// Textures in Qwadro are stored in as straightforward a manner as possible.
/// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
/// It then has a list of images, and each image lists its MIP levels.
/// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

typedef enum afxEventTexture
{
    AFX_EVENT_TEX_RESIZ
} afxEventTexture;

typedef enum afxTextureFlags
{
    AFX_TEX_USAGE_SRC               = AFX_FLAG(0), // The texture can be used as the source of a copy operation.
    AFX_TEX_USAGE_DST               = AFX_FLAG(1), // The texture can be used as the destination of a copy or write operation.
    AFX_TEX_USAGE_TRANSFER          = (AFX_TEX_USAGE_SRC | AFX_TEX_USAGE_DST),
    AFX_TEX_USAGE_SAMPLING          = AFX_FLAG(2), // The texture can be bound for use as a sampled texture in a shader.
    AFX_TEX_USAGE_STORAGE           = AFX_FLAG(3), // The texture can be bound for use as a storage texture in a shader.
    AFX_TEX_USAGE_DRAW              = AFX_FLAG(4), // The texture can be used as a color or depth/stencil attachment in a render pass.
    AFX_TEX_USAGE                   = 0x000000FF,

    AFX_TEX_FLAG_CUBEMAP            = AFX_FLAG(30),
    AFX_TEX_FLAG_REVALIDATE         = AFX_FLAG(31)
} afxTextureFlags;

typedef enum afxTextureOpenFlags
{
    AFX_TEX_OPEN_R  = AFX_FLAG(0),
    AFX_TEX_OPEN_W  = AFX_FLAG(1),
    AFX_TEX_OPEN_RW = (AFX_TEX_OPEN_R | AFX_TEX_OPEN_W),
    AFX_TEX_OPEN_X  = AFX_FLAG(2) // especial caso que força a reconstrução do recurso junto ao OpenGL
} afxTextureOpenFlags;

AFX_DEFINE_STRUCT(afxTextureRegion)
{
    afxNat                  lodIdx;
    afxNat                  baseImg;
    afxNat                  imgCnt;
    afxWhd                  offset;
    afxWhd                  extent;
};

// 1d               =   1 x w11 >> lod
// 1d array         = img x w11 >> lod
// 2d               =   1 x wh1 >> lod
// 2d array         = img x wh1
// cube             =   1 x wh1 >> lod
// cube array       = img x wh1 >> lod
// 3d               =   1 x whd >> lod

AFX_DECLARE_STRUCT(_afxTexVmt);

#ifdef _AFX_TEXTURE_C
AFX_OBJECT(afxTexture)
#else
struct afxBaseTexture
#endif
{
    _afxTexVmt const*       vmt;
    afxTextureFlags         flags;

    afxPixelFormat          fmt;
    afxWhd                  whd; // extent of image
    afxNat                  lodCnt; // mip level cnt
    afxNat                  imgCnt;
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxBool                 linearlyTiled; // optimal tiling (for driver) if false.
    afxColorSwizzling const*swizzling;

    afxByte*                maps;
};

AFX afxNat                      AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[]);

AFX afxResult                   AfxFindTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const name[]);
AFX afxError                    AfxLoadTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[]);

AFX afxError                    AfxAcquireTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[]);

AFX void                        AfxGetTextureExtent(afxTexture tex, afxNat lodIdx, afxWhd extent);
#if 0
AFX afxError                    AfxReadjustTexture(afxTexture tex, afxWhd const extent);
#endif

AFX afxPixelFormat              AfxGetTextureFormat(afxTexture tex);
AFX afxNat                      AfxGetTextureLodCount(afxTexture tex);
AFX afxNat                      AfxGetTextureImageCount(afxTexture tex);
AFX afxNat                      AfxGetTextureSampleCount(afxTexture tex);

AFX afxColorSwizzling const*    AfxTextureGetColorSwizzling(afxTexture tex);
AFX afxError                    AfxTextureSwapChannels(afxTexture tex, afxColorSwizzle a, afxColorSwizzle b, afxTextureRegion const *rgn);

AFX afxResult                   AfxTestTexture(afxTexture tex, afxTextureFlags bitmask);
AFX void                        AfxFlagTexture(afxTexture tex, afxTextureFlags bitmask);
AFX void                        AfxUnflagTexture(afxTexture tex, afxTextureFlags bitmask);

AFX afxError                    AfxGenerateTextureLods(afxTexture tex, afxNat firstLod, afxNat lodCnt);

AFX afxError                    AfxBufferizeTexture(afxTexture tex);
AFX afxNat                      AfxMeasureTextureRegion(afxTexture tex, afxTextureRegion const *rgn);
AFX afxNat                      AfxLocateTextureRegion(afxTexture tex, afxNat lodIdx, afxNat imgIdx, afxWhd const offset);

AFX void*                       AfxOpenTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *size);
AFX void                        AfxCloseTextureRegion(afxTexture tex, afxTextureRegion const *rgn);

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(afxUpdateTexture)
{
    afxTexture      tex;
    afxNat          lodIdx;
    afxNat          baseImg;
    afxNat          imgCnt;
    void const*     src;
    afxNat          siz;
    afxPixelFormat  fmt;
};

AFX_DEFINE_STRUCT(afxUpdateTextureRegion)
{
    afxTexture              tex;
    afxNat                  cnt;
    afxTextureRegion const* rgn;
    void const**            src;
    afxNat const*           siz;
    afxPixelFormat const*   fmt;
};

AFX afxError                    AfxCmdUpdateTextureRegion(afxDrawInput din, afxUpdateTextureRegion const* instruction);

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

AFX_DEFINE_STRUCT(afxInputTexture)
{
    afxTexture      tex;
    afxNat          lodIdx;
    afxNat          baseImg;
    afxNat          imgCnt;
    afxStream*      in;
};

// Stream in texture image data from a stream.
AFX afxError                    AfxInputTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxStream *in);
AFX afxError                    AfxInputTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxStream *in);
AFX afxError                    AfxInputTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxStream *in);

// Stream in texture image data from a file.
AFX afxError                    AfxFetchTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
AFX afxError                    AfxFetchTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxUri const *uri);
AFX afxError                    AfxFetchTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

AFX_DEFINE_STRUCT(afxOutputTexture)
{
    afxTexture      tex;
    afxNat          lodIdx;
    afxNat          baseImg;
    afxNat          imgCnt;
    afxStream*      out;
};

// Stream out texture image data to a stream.
AFX afxError                    AfxOutputTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxStream *out);
AFX afxError                    AfxOutputTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxStream *out);
AFX afxError                    AfxOutputTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxStream *out);

AFX_DEFINE_STRUCT(afxPrintTexture)
{
    afxTexture      tex;
    afxNat          lodIdx;
    afxNat          baseImg;
    afxNat          imgCnt;
    afxUri const*   uri;
};

AFX_DEFINE_STRUCT(afxPrintTextureRegions)
{
    afxTexture      tex;
    afxNat          lodIdx;
    afxNat          baseImg;
    afxNat          imgCnt;
    afxUri const*   uri;
};

// Stream out texture image data to a file.
AFX afxError                    AfxPrintTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
AFX afxError                    AfxPrintTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxUri const *uri);
AFX afxError                    AfxPrintTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

#endif//AFX_TEXTURE_H