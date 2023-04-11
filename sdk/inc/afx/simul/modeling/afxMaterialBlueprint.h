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

#ifndef AFX_MATERIAL_BLUEPRINT_H
#define AFX_MATERIAL_BLUEPRINT_H

#include "afxMaterial.h"

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um afxMaterial.

AFX_DECLARE_STRUCT(afxMaterialBlueprint);

AFX_DEFINE_STRUCT(afxMaterialMapBlueprint)
{
    afxString16         usage;
    afxBool             isUri;
    union
    {
        //afxMaterialBlueprint    mtlb;
        afxUri128       uri;
    };
};

AFX_DEFINE_STRUCT(afxMaterialBlueprint)
{
    afxFcc                      fcc;
    afxString32                 name;
    afxNat                      mapCnt;
    afxMaterialMapBlueprint     *maps;
    afxUri128                   tex;
};

// Inicializa uma blueprint, e reserva recursos.
AFX afxError                    AfxMaterialBlueprintDeploy(afxMaterialBlueprint *mtlb, afxString const *name, afxNat mapCnt, afxUri const *tex);

// Retira de uso uma blueprint, e libera recursos.
AFX afxError                    AfxMaterialBlueprintDrop(afxMaterialBlueprint *mtlb);

// Tenta completar uma blueprint com os dados já fornecidos, se suficentes para serem usados para gerar um novo afxMaterial.
AFX afxError                    AfxMaterialBlueprintValidate(afxMaterialBlueprint *mtlb);

AFX afxMaterialMapBlueprint*    AfxMaterialBlueprintGetMap(afxMaterialBlueprint *mtlb, afxNat mapIdx);

AFX afxError                    AfxMaterialBlueprintDefineUri(afxMaterialBlueprint *mtlb, afxNat mapIdx, afxUri const *uri);

#endif//AFX_MATERIAL_BLUEPRINT_H