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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_TEXTURE_BLUEPRINT_H
#define AFX_TEXTURE_BLUEPRINT_H

#include "afx/draw/res/afxTexture.h"

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
            afxStream*          ios;
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
    afxDrawContext              dctx;
    afxWhd                      extent;
    afxPixelFormat              fmt;
    afxNat                      lodCnt; // aka mip level. // at least 1.
    afxNat                      imgCnt;
    afxTextureSource            images[16];
    afxNat                      sampleCnt;
    afxBool                     generateLods;
    afxFlags                    usage; // CUBEMAP, etc
};

AFX void                        AfxAcquireTextureBlueprint(afxTextureBlueprint *blueprint, afxDrawContext dctx, afxWhd const extent, afxPixelFormat fmt, afxFlags usage);
AFX afxError                    AfxTextureBlueprintEnd(afxTextureBlueprint *blueprint, afxNat cnt, afxTexture tex[]);

AFX afxError                    AfxTextureBlueprintAddImage(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, void const *start, afxNat range);
AFX afxError                    AfxTextureBlueprintAddImageFromStream(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, afxStream *ios, afxSize offset, afxNat range);
AFX afxError                    AfxTextureBlueprintAddImagesFromResource(afxTextureBlueprint *blueprint, afxNat cnt, afxUri const uri[]);

AFX afxError                    AfxBuildTextures(afxTextureBlueprint const *blueprint, afxNat cnt, afxTexture tex[]);

#endif//AFX_TEXTURE_BLUEPRINT_H