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

#include "awxMesh.h"
#include "qwadro/sim/awxSkeleton.h"

/// O objeto awxModel descreve uma coleção de awxMesh'es que são todas ligadas ao mesmo awxSkeleton.
/// Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
/// Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
/// Um awxModel completo é feito de um awxSkeleton e um conjunto de awxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do awxModel.

#ifdef _AFX_MODEL_C
AFX_DEFINE_STRUCT(awxLinkedMesh)
{
    awxMesh             msh;
    awxMaterial         txd;
    afxNat*             boneMap;
    afxNat*             origBoneMap;
    awxSkeleton         origSkl;
};

AFX_OBJECT(awxModel)
{
    afxBufferedString   id; // 128
    awxSkeleton         skl;
    afxTransform        init;
    afxAabb             aabb;
    afxNat              slotCnt;
    awxLinkedMesh*      slots;
};
#endif

AFX afxString const*    AwxGetModelId(awxModel mdl);

AFX awxSkeleton         AwxGetModelSkeleton(awxModel mdl);
AFX void                AwxRelinkModelSkeleton(awxModel mdl, awxSkeleton skl);

AFX void                AwxGetModelInitialPlacement(awxModel mdl, afxReal m[4][4]);
AFX void                AwxResetModelInitialPlacement(awxModel mdl, afxTransform const* xform);

AFX afxNat              AwxGetMeshLinkageCapacity(awxModel mdl);
AFX afxNat              AwxCountLinkedMeshes(awxModel mdl);
AFX afxNat              AwxGetLinkedMeshes(awxModel mdl, afxNat baseSlot, afxNat slotCnt, awxMesh msh[]);
AFX afxError            AwxRelinkMeshes(awxModel mdl, awxSkeleton origSkl, afxNat first, afxNat cnt, awxMesh meshes[]);

AFX afxBool             AwxLinkedMeshIsTransplanted(awxModel mdl, afxNat slotIdx);
AFX awxSkeleton         AwxGetLinkedMeshOriginalSkeleton(awxModel mdl, afxNat slotIdx);
AFX afxNat const*       AwxGetLinkedMeshOriginalBoneMap(awxModel mdl, afxNat slotIdx);

AFX awxMaterial         AwxGetLinkedMeshTxd(awxModel mdl, afxNat slotIdx);
AFX void                AwxSetLinkedMeshTxd(awxModel mdl, afxNat slotIdx, awxMaterial mtl);
AFX awxMaterial         AwxFindLinkedMeshMaterial(awxModel mdl, afxNat slotIdx, afxString const* id);

AFX afxNat const*       AwxGetLinkedMeshBoneMap(awxModel mdl, afxNat slotIdx);

AFX void                AwxBuildLinkedMeshMatrixArray(awxModel mdl, afxNat slotIdx, awxWorldPose const* WorldPose, afxNat firstBoneIdx, afxNat boneCnt, afxReal xformBuffer[][4][4]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(awxModelAssembler)
{
    afxString       id;
    awxSkeleton     skl;
    afxTransform    init;
    afxNat          mshCnt;
    awxMesh*        meshes;
};

AFX awxModel            AwxAssembleModel(afxSimulation sim, afxString const* id, awxSkeleton skl, afxTransform const* init, afxNat mshCnt, awxMesh msh[]);

AFX void                AwxTransformModels(afxReal const linear[3][3], afxReal const invLinear[3][3], afxReal const affine[3], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxModel mdl[]);

#endif//AFX_MODEL_H
