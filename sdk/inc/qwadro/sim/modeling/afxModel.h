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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_MODEL_H
#define AFX_MODEL_H

#include "afxMesh.h"
#include "qwadro/sim/afxSkeleton.h"

/// O objeto afxModel descreve uma coleção de afxMesh'es que são todas ligadas ao mesmo afxSkeleton.
/// Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
/// Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo é feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

#ifdef _AFX_MODEL_C
AFX_DEFINE_STRUCT(afxMeshRig)
{
    afxMesh             msh;
    afxMaterial         txd;
    afxNat*             biasToJointMap; // to assembled skeleton.
    afxNat*             biasFromJointMap; // from original skeleton.
    afxSkeleton         sklOrig;
};

AFX_OBJECT(afxModel)
{
    afxSkeleton         skl;
    afxNat              rigCnt;
    afxMeshRig*         rigs;
    afxAabb             aabb;
    afxTransform        displacement;
    afxString           id; // 32
    afxStringBase       strb;
};
#endif

AFX_DEFINE_STRUCT(afxModelBlueprint)
/// Data needed for model assembly
{
    afxString32         id;
    afxSkeleton         skl;
    afxTransform        displacement;
    afxNat              rigCnt;
    afxMesh*            meshes;
    afxStringBase       strb;
};

AKX afxBool             AfxGetModelId(afxModel mdl, afxString* id);

AKX afxSkeleton         AfxGetModelSkeleton(afxModel mdl);

AKX void                AfxComputeModelDisplacement(afxModel mdl, afxM4d m);
AKX void                AfxUpdateModelDisplacement(afxModel mdl, afxTransform const* xform);

AKX afxNat              AfxCountModelRigs(afxModel mdl);
AKX afxNat              AfxCountRiggedMeshes(afxModel mdl);
AKX afxNat              AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseRig, afxNat rigCnt, afxMesh meshes[]);
AKX afxError            AfxRigMeshes(afxModel mdl, afxSkeleton origSkl, afxNat baseRig, afxNat rigCnt, afxMesh const meshes[]);

AKX afxBool             AfxRiggedMeshIsTransplanted(afxModel mdl, afxNat rigIdx);
AKX afxSkeleton         AfxGetRiggedMeshDonor(afxModel mdl, afxNat rigIdx);

AKX afxMaterial         AfxGetRiggedMeshTxd(afxModel mdl, afxNat rigIdx);
AKX void                AfxSetRiggedMeshTxd(afxModel mdl, afxNat rigIdx, afxMaterial mtl);
AKX afxMaterial         AfxFindRiggedMeshMaterial(afxModel mdl, afxNat rigIdx, afxString const* id);

AKX afxNat const*       AfxGetRiggedMeshBiasToJointMapping(afxModel mdl, afxNat rigIdx);
AKX afxNat const*       AfxGetRiggedMeshBiasFromJointMapping(afxModel mdl, afxNat rigIdx);

AKX void                AfxComputeRiggedMeshMatrices(afxModel mdl, afxNat rigIdx, awxPoseBuffer const* posb, afxNat baseBias, afxNat jointCnt, afxM4d m[]);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleModel(afxSimulation sim, afxNat cnt, afxModelBlueprint const blueprints[], afxModel models[]);

AKX void                AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxNat cnt, afxModel models[]);

#endif//AFX_MODEL_H
