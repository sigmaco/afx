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

AFX_DEFINE_STRUCT(akxModelBlueprint)
/// Data needed for model assembly
{
    afxString32         id;
    afxSkeleton         skl;
    afxTransform        displacement;
    afxNat              rigCnt;
    afxMesh*            meshes;
};

AMX afxBool             AfxGetModelUrn(afxModel mdl, afxString* id);

AMX afxSkeleton         AfxGetModelSkeleton(afxModel mdl);

AMX void                AfxComputeModelDisplacement(afxModel mdl, afxM4d m);
AMX void                AfxUpdateModelDisplacement(afxModel mdl, afxTransform const* t);

AMX afxNat              AfxCountModelRigs(afxModel mdl);
AMX afxNat              AfxCountRiggedMeshes(afxModel mdl);
AMX afxNat              AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseRig, afxNat rigCnt, afxMesh meshes[]);
AMX afxError            AfxRigMeshes(afxModel mdl, afxSkeleton origSkl, afxNat baseRig, afxNat rigCnt, afxMesh const meshes[]);

AMX afxBool             AfxRiggedMeshIsTransplanted(afxModel mdl, afxNat rigIdx);
AMX afxSkeleton         AfxGetRiggedMeshSkeleton(afxModel mdl, afxNat rigIdx);

AMX afxMaterial         AfxGetRiggedMeshTxd(afxModel mdl, afxNat rigIdx);
AMX void                AfxSetRiggedMeshTxd(afxModel mdl, afxNat rigIdx, afxMaterial mtl);
AMX afxMaterial         AfxFindRiggedMeshMaterial(afxModel mdl, afxNat rigIdx, afxString const* id);

AMX afxNat const*       AfxGetRiggedMeshBiasToJointMapping(afxModel mdl, afxNat rigIdx);
AMX afxNat const*       AfxGetRiggedMeshBiasFromJointMapping(afxModel mdl, afxNat rigIdx);

AMX void                AfxComputeRiggedMeshMatrices(afxModel mdl, afxNat rigIdx, afxPoseBuffer const posb, afxNat baseBias, afxNat jointCnt, afxM4d m[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxError            AfxAssembleModels(afxSimulation sim, afxNat cnt, akxModelBlueprint const blueprints[], afxModel models[]);

AMX afxNat              AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel models[]);

AMX void                AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxNat cnt, afxModel models[]);

#endif//AMX_MODEL_H
