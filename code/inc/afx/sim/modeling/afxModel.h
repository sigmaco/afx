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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MODEL_H
#define AFX_MODEL_H

#include "afxMesh.h"
#include "afx/sim/afxSkeleton.h"

/// O objeto afxModel descreve uma coleção de afxMesh'es que são todas ligadas ao mesmo afxSkeleton.
/// Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
/// Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo é feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

#ifdef _AFX_MODEL_C
AFX_DEFINE_STRUCT(afxMeshSlot)
{
    afxMesh             msh;
    afxNat*             boneIndices;
    afxNat*             srcBoneIndices;
    afxSkeleton         srcSkl;
};

AFX_OBJECT(afxModel)
{
    afxString           id; // 128
    afxSkeleton         skl;
    afxTransform        init;
    afxAabb             aabb;
    afxNat              cap;
    afxMeshSlot        *meshSlots;
};
#endif

AFX afxString const*    AfxGetModelId(afxModel mdl);

AFX afxSkeleton         AfxGetModelSkeleton(afxModel mdl);
AFX void                AfxRelinkModelSkeleton(afxModel mdl, afxSkeleton skl);

AFX void                AfxGetModelInitialPlacement(afxModel mdl, afxReal m[4][4]);
AFX void                AfxResetModelInitialPlacement(afxModel mdl, afxTransform const* xform);

AFX afxNat              AfxGetModelCapacity(afxModel mdl);
AFX afxNat              AfxCountAttachedMeshes(afxModel mdl);
AFX afxNat              AfxGetAttachedMeshes(afxModel mdl, afxNat baseSlotIdx, afxNat slotCnt, afxMesh msh[]);
AFX afxError            AfxAttachMeshes(afxModel mdl, afxSkeleton srcSkl, afxNat first, afxNat cnt, afxMesh meshes[]);

AFX afxBool             AfxAttachedMeshIsTransferred(afxModel mdl, afxNat slotIdx);
AFX afxSkeleton         AfxGetAttachedMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx);
AFX afxNat const*       AfxGetAttachedMeshOriginalBoneIndices(afxModel mdl, afxNat slotIdx);

AFX afxNat const*       AfxGetAttachedMeshBoneIndices(afxModel mdl, afxNat slotIdx);

AFX void                AfxBuildAttachedMeshMatrixArray(afxModel mdl, afxNat slotIdx, afxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(afxModelAssembler)
{
    afxString       id;
    afxSkeleton     skl;
    afxTransform    init;
    afxNat          mshCnt;
    afxMesh*        meshes;
};

AFX afxModel            AfxAssembleModel(afxSimulation sim, afxString const* id, afxSkeleton skl, afxTransform const* init, afxNat mshCnt, afxMesh msh[]);

AFX void                AfxTransformModels(afxReal const linear[3][3], afxReal const invLinear[3][3], afxReal const affine[3], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxModel mdl[]);

#endif//AFX_MODEL_H
