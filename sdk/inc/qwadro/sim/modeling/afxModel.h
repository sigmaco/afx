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
AFX_DEFINE_STRUCT(awxLinkedMesh)
{
    afxMesh             msh;
    afxMaterial         txd;
    afxNat*             boneMap;
    afxNat*             origBoneMap;
    afxSkeleton         origSkl;
};

AFX_OBJECT(afxModel)
{
    afxNat              idStrIdx;
    afxFixedString32    id; // 128
    afxSkeleton         skl;
    afxTransform        init;
    afxAabb             aabb;
    afxNat              slotCnt;
    awxLinkedMesh*      slots;
    afxStringCatalog    strc;
};
#endif

AFX_DEFINE_STRUCT(afxModelBlueprint)
/// Data needed for model assembly
{
    afxFixedString32    id;
    afxSkeleton         skl;
    afxTransform        init;
    afxNat              baseMshIdx;
    afxNat              mshCnt;
};

AFX afxBool             AfxGetModelId(afxModel mdl, afxString* id);

AFX afxSkeleton         AfxGetModelSkeleton(afxModel mdl);

AFX void                AfxGetModelInitialPlacement(afxModel mdl, afxReal m[4][4]);
AFX void                AfxResetModelInitialPlacement(afxModel mdl, afxTransform const* xform);

AFX afxNat              AfxGetModelCapacity(afxModel mdl);
AFX afxNat              AfxCountLinkedMeshes(afxModel mdl);
AFX afxNat              AfxEnumerateLinkedMeshes(afxModel mdl, afxNat baseSlot, afxNat slotCnt, afxMesh msh[]);
AFX afxError            AfxRelinkMeshes(afxModel mdl, afxSkeleton origSkl, afxNat baseSlot, afxNat slotCnt, afxMesh const meshes[]);

AFX afxBool             AfxMeshIsTransplanted(afxModel mdl, afxNat slotIdx);
AFX afxSkeleton         AfxGetMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx);
AFX afxNat const*       AfxGetMeshOriginalBoneMap(afxModel mdl, afxNat slotIdx);

AFX afxMaterial         AfxGetMeshTxd(afxModel mdl, afxNat slotIdx);
AFX void                AfxSetMeshTxd(afxModel mdl, afxNat slotIdx, afxMaterial mtl);
AFX afxMaterial         AfxFindMeshMaterial(afxModel mdl, afxNat slotIdx, afxString const* id);

AFX afxNat const*       AfxGetMeshBoneMap(afxModel mdl, afxNat slotIdx);

AFX void                AwxBuildLinkedMeshMatrixArray(afxModel mdl, afxNat slotIdx, awxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxAssembleModel(afxSimulation sim, afxStringCatalog strc, afxMesh const meshes[], afxNat cnt, afxModelBlueprint const blueprints[], afxModel models[]);

AFX void                AfxTransformModels(afxReal const linear[3][3], afxReal const invLinear[3][3], afxReal linearTol, afxReal const affine[3], afxReal affineTol, afxFlags flags, afxNat cnt, afxModel models[]);

#endif//AFX_MODEL_H
