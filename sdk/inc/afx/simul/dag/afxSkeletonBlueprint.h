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

#ifndef AFX_SKELETON_BLUEPRINT_H
#define AFX_SKELETON_BLUEPRINT_H

#include "afxSkeleton.h"

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um afxSkeleton.

AFX_DEFINE_STRUCT(afxBoneBlueprint)
{
    afxFcc              fcc;
    afxString32         name;
    afxNat              parentIdx;
    afxQuat             rotation;
    afxV4d              position;
    afxV3d              scale;
    afxM4d              iw;
    afxReal             lodError;
};

AFX_DEFINE_STRUCT(afxSkeletonBlueprint)
{
    afxFcc              fcc;
    afxString32         name;
    afxNat              boneCnt;
    afxBoneBlueprint    *bones;
};

// Inicializa uma blueprint, e reserva recursos.
AFX afxError            AfxSkeletonBlueprintDeploy(afxSkeletonBlueprint *sklb, afxString const *name, afxNat boneCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX afxError            AfxSkeletonBlueprintDrop(afxSkeletonBlueprint *sklb);

// Checa se um osso sob especificado índice já está adicionado.
AFX afxBool             AfxSkeletonBlueprintHasBone(afxSkeletonBlueprint *sklb, afxNat idx);

// Adiciona um osso a um esqueleto. Ao menos um deve existir.
AFX afxError            AfxSkeletonBlueprintDefineBone(afxSkeletonBlueprint *sklb, afxNat idx, afxString const *name, afxNat parentIdx, afxQuat const rotation, afxV4d const position, afxV3d const scale);

// Define uma tolerância a erro de LOD para um especificado osso.
AFX afxError            AfxSkeletonBlueprintAssignBoneLodError(afxSkeletonBlueprint *sklb, afxNat idx, afxReal lodError);

// Associa um osso como filho de outro osso numa blueprint.
AFX afxError            AfxSkeletonBlueprintBindBoneParent(afxSkeletonBlueprint *sklb, afxNat idx, afxNat parentIdx);

// Tenta completar uma blueprint com os dados já fornecidos, se suficentes para serem usados para gerar um novo afxSkeleton.
AFX afxError            AfxSkeletonBlueprintValidate(afxSkeletonBlueprint *sklb);

#endif//AFX_SKELETON_BLUEPRINT_H