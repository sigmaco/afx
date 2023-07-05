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

#ifndef AFX_SKELETON_BLUEPRINT_H
#define AFX_SKELETON_BLUEPRINT_H

#include "afx/core/io/afxUri.h"
#include "afx/math/afxTransform.h"
#include "afx/core/mem/afxArray.h"

AFX_DEFINE_HANDLE(afxSkeleton);

/// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de um afxSkeleton.

AFX_DEFINE_STRUCT(afxSkeletonBlueprintBone)
{
    afxString*                  name; // 32
    afxNat                      parentIdx;
    afxTransform                local;
    afxM4d                      iw;
    afxReal                     lodError;
};

AFX_DEFINE_STRUCT(afxSkeletonBlueprint)
{
    afxFcc                      fcc;
    void*                       sim;
    afxArray                    bones; // afxSkeletonBlueprintBone    
    afxNat                      lodType;
    afxNat                      *remappingTable;
};

// Inicializa uma blueprint, e reserva recursos.
AFX void                AfxSkeletonBlueprintDeploy(afxSkeletonBlueprint *sklb, void *sim);

// Retira de uso uma blueprint, e libera recursos.
AFX void                AfxSkeletonBlueprintDiscard(afxSkeletonBlueprint *sklb);

// Adiciona um osso a um esqueleto. Ao menos um deve existir.
AFX void                AfxSkeletonBlueprintAddBoneWithIw(afxSkeletonBlueprint *sklb, afxV3d const pos, afxQuat const orientation, afxM3d const scaleShear, afxM4d const iw);
AFX void                AfxSkeletonBlueprintAddBone(afxSkeletonBlueprint *sklb, afxV3d const pos, afxQuat const orientation, afxM3d const scaleShear, afxV3d const worldPos, afxQuat const worldOrientation, afxM3d const worldScaleShear);

// Define uma tolerância a erro de LOD para um especificado osso.
AFX void                AfxSkeletonBlueprintSetBoneLodError(afxSkeletonBlueprint *sklb, afxNat idx, afxReal lodError);

// Associa um osso como filho de outro osso numa blueprint.
AFX void                AfxSkeletonBlueprintReparentBone(afxSkeletonBlueprint *sklb, afxNat idx, afxNat parentIdx);
AFX void                AfxSkeletonBlueprintReparentLastBone(afxSkeletonBlueprint *sklb, afxNat parentIdx);

AFX void                AfxSkeletonBlueprintRenameBone(afxSkeletonBlueprint *sklb, afxNat idx, afxString const *name);
AFX void                AfxSkeletonBlueprintRenameLastBone(afxSkeletonBlueprint *sklb, afxString const *name);

AFX void                AfxSkeletonBlueprintSetRemappingTable(afxSkeletonBlueprint *sklb, afxNat *remappingTable);

#endif//AFX_SKELETON_BLUEPRINT_H