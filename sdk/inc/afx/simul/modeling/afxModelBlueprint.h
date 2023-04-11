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

#ifndef AFX_MODEL_BLUEPRINT_H
#define AFX_MODEL_BLUEPRINT_H

#include "afxModel.h"
#include "../dag/afxSkeletonBlueprint.h"
#include "afxMeshBlueprint.h"

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um modelo.

typedef enum afxModelBlueprintFlag
{
    AFX_MDLP_FLAG_DONT_SHARE_VBUF   = AFX_FLAG(0),
    AFX_MDLP_FLAG_DONT_SHARE_IBUF   = AFX_FLAG(1)
} afxModelBlueprintFlag;

AFX_DEFINE_STRUCT(afxModelBlueprint)
{
    afxFcc                  fcc;
    afxString32             name;
    afxSkeleton             skl; // if exist, sklb will be ignored.
    afxSkeletonBlueprint    sklb;
    afxM4d                  init;
    afxNat                  mshCnt;
    afxMeshBlueprint        *meshes;
};

// Inicializa uma blueprint, e reserva recursos.
AFX afxError                AfxModelBlueprintDeploy(afxModelBlueprint *mdlb, afxString const *name, afxM4d const m, afxNat mshCnt, afxNat boneCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX afxError                AfxModelBlueprintDrop(afxModelBlueprint *mdlb);

// Obtém um blueprint para um afxMesh sob o índice especificado.
AFX afxMeshBlueprint*       AfxModelBlueprintGetMesh(afxModelBlueprint *mdlb, afxNat mshIdx);

// Obtém o blueprint do afxSkeleton.
AFX afxSkeletonBlueprint*   AfxModelBlueprintGetSkeleton(afxModelBlueprint *mdlb);

// Tenta completar uma blueprint com os dados já fornecidos, se suficentes para serem usados para gerar um novo afxModel.
AFX afxError                AfxModelBlueprintValidate(afxModelBlueprint *mdlb);

#endif//AFX_MODEL_BLUEPRINT_H