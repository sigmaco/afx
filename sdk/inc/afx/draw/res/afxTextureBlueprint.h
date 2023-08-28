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
        afxInstance               *obj;
        afxUri                  uri; // map subentry uri to don't use in-place URI storage.
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

AFX void                        AfxAcquireTextureBlueprint(afxTextureBlueprint *blueprint, afxWhd const extent, afxPixelFormat fmt, afxFlags usage);
AFX afxError                    AfxTextureBlueprintEnd(afxTextureBlueprint *blueprint, afxNat cnt, afxTexture tex[]);

AFX afxError                    AfxTextureBlueprintAddImage(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, void const *start, afxNat range);
AFX afxError                    AfxTextureBlueprintAddImageFromStream(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, afxStream *ios, afxSize offset, afxNat range);
AFX afxError                    AfxTextureBlueprintAddImagesFromResource(afxTextureBlueprint *blueprint, afxNat cnt, afxUri const uri[]);

AFX afxError                    AfxBuildTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxTextureBlueprint const *blueprint);

#endif//AFX_TEXTURE_BLUEPRINT_H