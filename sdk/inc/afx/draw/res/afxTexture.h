/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_TEXTURE_H
#define AFX_TEXTURE_H

#include "afx/draw/afxColor.h"
#include "afx/core/io/afxResource.h"

// Textures in Granny are stored in as straightforward a manner as possible.
// Each texture says what kind it is (such as color map or cube map), what encoding it is (such as raw pixels or S3TC), and how many images it has (1 for a color map, 6 for a cube map, etc.).
// It then has a list of images, and each image lists its MIP levels.
// That's about all there is to textures. How the texture is meant to be used (ie., as a diffuse map or a normal map or something else) is not specified in the texture itself, since it might be used differently in different materials.

typedef enum afxTextureFlag
{
    AFX_TEX_FLAG_DUMMY = (1 << 0)
} afxTextureFlag;

typedef enum afxTextureUsage
{
    AFX_TEX_USAGE_TRANSFER_SRC      = AFX_FLAG(0),
    AFX_TEX_USAGE_TRANSFER_DST      = AFX_FLAG(1),
    AFX_TEX_USAGE_SAMPLED           = AFX_FLAG(2),
    AFX_TEX_USAGE_STORAGE           = AFX_FLAG(3),
    AFX_TEX_USAGE_SURFACE_RASTER    = AFX_FLAG(4), // used as color buffer; a surface of canvases.
    AFX_TEX_USAGE_SURFACE_DEPTH     = AFX_FLAG(5), // used as depth/stencil buffer; a surface of canvases.
    AFX_TEX_USAGE_SURFACE           = (AFX_TEX_USAGE_SURFACE_RASTER | AFX_TEX_USAGE_SURFACE_DEPTH),
    AFX_TEX_USAGE_TRANSIENT_BUFFER  = AFX_FLAG(6),
    AFX_TEX_USAGE_INPUT_BUFFER      = AFX_FLAG(7),

    AFX_TEX_CUBEMAP                 = AFX_FLAG(30),
    AFX_TEX_VOLUME                  = AFX_FLAG(31) // to be used as 3D instead of layered
} afxTextureUsage;

AFX_DEFINE_HANDLE(afxTexture);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxTexture)
{
    afxObject               obj;
};

#endif

AFX_DEFINE_STRUCT(afxTextureRegion)
{
    afxNat                      lod; // mip level
    afxNat                      baseLayer;
    afxNat                      layerCnt;
    afxWhd                      offset;
    afxWhd                      extent;
};

AFX_DEFINE_STRUCT(afxTextureSource) // source for blueprints
{
    afxFcc                      type; // NIL (raw data), URI or IOS
    union
    {
        struct
        {
            afxPixelFormat      fmt;
            afxWhd              extent; // d always 1
            void const*         start;
            afxSize             range;
        }                       data;
        struct
        {
            afxPixelFormat      fmt;
            afxWhd              extent; // d always 1
            afxStream           ios;
            afxSize             offset;
            afxNat              range;
        }                       stream;
        afxObject               *obj;
        afxUri*                 uri; // map subentry uri to don't use in-place URI storage.
    };
};

AFX_DEFINE_STRUCT(afxTextureBlueprint)
{
    afxFcc                      fcc;
    void*                       dctx;
    afxUri128                   name; // 128
    afxPixelFormat              fmt;
    afxNat                      whd[3];
    afxNat                      layerCnt;
    afxTextureSource            images[16];
    afxNat                      lodCnt; // aka mip level. // at least 1.
    afxBool                     generateLods;
    afxFlags                    usage; // CUBEMAP, etc
};

AFX void*                       AfxTextureGetContext(afxTexture tex);
AFX void*                       AfxTextureGetDriver(afxTexture tex);
AFX void*                       AfxTextureGetDrawSystem(afxTexture tex);

AFX afxNat*                     AfxTextureGetExtent(afxTexture tex, afxNat lod, afxWhd extent);
AFX afxError                    AfxTextureSetExtent(afxTexture tex, afxNat layerCnt, afxWhd const extent);

AFX afxUri const*               AfxTextureGetUri(afxTexture tex);
AFX afxPixelFormat              AfxTextureGetFormat(afxTexture tex);
AFX afxNat                      AfxTextureGetLodCount(afxTexture tex);
AFX afxNat                      AfxTextureGetLayerCount(afxTexture tex);
AFX afxNat                      AfxTextureGetSampleCount(afxTexture tex);
AFX afxColorSwizzling const*    AfxTextureGetColorSwizzling(afxTexture tex);
AFX afxError                    AfxTextureSwapChannels(afxTexture tex, afxColorSwizzle a, afxColorSwizzle b);

AFX afxError                    AfxTextureGenerateLods(afxTexture tex, afxNat base, afxNat cnt);

AFX void const*                 AfxTextureGetData(afxTexture tex, afxNat lod, afxNat layer, afxWhd const offset);
//AFX afxSize                     AfxTextureMeasure(afxTexture tex, afxTextureRegion const *rgn);
AFX void*                       AfxTextureMap(afxTexture tex, afxTextureRegion const *rgn, afxSize *size);
AFX void                        AfxTextureUnmap(afxTexture tex);
AFX afxError                    AfxTextureUpdateRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], void const *src[], afxPixelFormat const fmt[]);

AFX afxError                    AfxTextureDownload(afxTexture tex, afxUri const *uri);
AFX afxError                    AfxTextureDownloadRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

AFX afxError                    AfxTextureUpload(afxTexture tex, afxUri const *uri);
AFX afxError                    AfxTextureUploadRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);

AFX afxBool                     AfxTextureTestFlags(afxTexture tex, afxTextureFlag flags);
AFX afxResult                   AfxTextureTestUsageFlags(afxTexture tex, afxFlags flags);

////////////////////////////////////////////////////////////////////////////////
// TEXTURE BLUEPRINT                                                          //
////////////////////////////////////////////////////////////////////////////////

AFX afxError                    AfxTextureBlueprintBegin(afxTextureBlueprint *blueprint, void* dctx, afxUri const *name, afxPixelFormat fmt, afxWhd const extent, afxFlags usage);
AFX afxError                    AfxTextureBlueprintEnd(afxTextureBlueprint *blueprint, afxNat cnt, afxTexture tex[]);

AFX afxError                    AfxTextureBlueprintAddImage(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, void const *start, afxNat range);
AFX afxError                    AfxTextureBlueprintAddImageFromStream(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, afxStream ios, afxSize offset, afxNat range);
AFX afxError                    AfxTextureBlueprintAddImagesFromResource(afxTextureBlueprint *blueprint, afxNat cnt, afxUri const uri[]);

#endif//AFX_TEXTURE_H