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

#ifndef AFX_TEXTURE_BLUEPRINT_H
#define AFX_TEXTURE_BLUEPRINT_H

#include "afxTexture.h"

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de uma afxTexture.

AFX_DEFINE_STRUCT(afxTextureLayerBlueprint)
{
    afxBool                     srcIsUrl;
    union
    {
        struct
        {
            void const          *src;
            afxPixelFormat      fmt;
            afxNat              stride; // pixel stride
        };
        afxUri128               uri;
    };
};

AFX_DEFINE_STRUCT(afxTextureBlueprint)
{
    afxFcc                      fcc;
    afxString32                 name;
    afxPixelFormat              fmt;
    afxNat                      whd[3];
    afxNat                      layerCnt;
    afxTextureLayerBlueprint    layers[16];
    afxNat                      lodCnt; // aka mip level. // at least 1.
    afxFlags                    usage; // CUBEMAP, etc
};

// Inicializa uma blueprint, e reserva recursos.
AFX afxError                    AfxTextureBlueprintDeploy(afxTextureBlueprint *texb, afxString const *name, afxPixelFormat fmt, afxWhd const extent, afxNat lodCnt, afxNat layerCnt, afxFlags usage);

// Retira de uso uma blueprint, e libera recursos.
AFX afxError                    AfxTextureBlueprintDrop(afxTextureBlueprint *texb);

// Tenta completar uma blueprint com os dados já fornecidos, se suficentes para serem usados para gerar uma nova afxTexture.
AFX afxError                    AfxTextureBlueprintValidate(afxTextureBlueprint *texb);

AFX afxError                    AfxTextureBlueprintDefineLayer(afxTextureBlueprint *texb, afxNat layerIdx, void const *src, afxPixelFormat fmt, afxNat stride);

AFX afxError                    AfxTextureBlueprintSetSourceUrl(afxTextureBlueprint *texb, afxNat base, afxNat cnt, afxUri const uri[]);

#endif//AFX_TEXTURE_BLUEPRINT_H