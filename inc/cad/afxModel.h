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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AMX_MODEL_H
#define AMX_MODEL_H

#include "qwadro/inc/cad/afxMesh.h"
#include "qwadro/inc/cad/afxSkeleton.h"

/// O objeto afxModel descreve uma coleção de afxMesh'es que são todas ligadas ao mesmo afxSkeleton.
/// Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
/// Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo é feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

/// Rigging is the process of connecting a mesh with an internal poseable skeleton rig and bone structure. 
/// Rigged meshes allow mesh surfaces to rotate and move where internal bones are placed within the model during the modeling process.

typedef enum afxModelFlag
{
    afxModelFlag_NONE
} afxModelFlags;

typedef enum afxMeshRigFlag
{
    afxMeshRigFlag_TRANSPLANTED = AFX_BIT(0)
} afxMeshRigFlags;

AFX_DEFINE_STRUCT(afxModelBlueprint)
/// Data needed for model assembly
{
    afxSkeletonBlueprint skl;
    afxTransform    displacement;
    afxUnit         rigCnt;
};

AMX afxBool         AfxGetModelUrn(afxModel mdl, afxString* id);

AMX afxBool         AfxGetModelSkeleton(afxModel mdl, afxSkeleton* skeleton);

AMX void            AfxDisplaceModel(afxModel mdl, afxTransform const* t);
AMX void            AfxGetModelDisplacement(afxModel mdl, afxTransform* t);
AMX void            AfxComputeModelDisplacement(afxModel mdl, afxM4d m);

AMX afxUnit         AfxCountMeshRigs(afxModel mdl);
AMX afxUnit         AfxCountRiggedMeshes(afxModel mdl);
AMX afxUnit         AfxGetRiggedMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit cnt, afxMesh meshes[]);
AMX afxError        AfxRigMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit cnt, afxMesh const meshes[], afxSkeleton origSkl);

AMX afxBool         AfxIsMeshRigTransplanted(afxModel mdl, afxUnit rigIdx);
AMX afxBool         AfxGetMeshRigBaseSkeleton(afxModel mdl, afxUnit rigIdx, afxSkeleton* skeleton);

AMX afxBool         AfxGetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial* material);
AMX void            AfxSetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial mtl);
AMX afxBool         AfxFindMeshRigMaterial(afxModel mdl, afxUnit rigIdx, afxString const* id, afxMaterial* material);

// TODO? Make some kind of binding for these datas to avoid expose/demilitarized/direct access? We can benefit of small integer types.
AMX afxUnit const*  AfxGetMeshRigBiasToJointMapping(afxModel mdl, afxUnit rigIdx);
AMX afxUnit const*  AfxGetMeshRigBiasFromJointMapping(afxModel mdl, afxUnit rigIdx);

AMX void            AfxComputeMeshRigMatrices(afxModel mdl, afxUnit rigIdx, afxPoseBuffer const posb, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxUnit         AfxEnumerateModels(afxSimulation sim, afxUnit first, afxUnit cnt, afxModel models[]);

AMX void            AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxUnit cnt, afxModel models[]);

AMX afxError        AfxAssembleModels(afxSimulation sim, afxSkeleton skl, afxUnit cnt, afxModelBlueprint const blueprints[], afxModel models[]);

#endif//AMX_MODEL_H
