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

#include "qwadro/inc/draw/kit/afxMesh.h"
#include "qwadro/inc/sim/body/afxPose.h"
#include "qwadro/inc/sim/body/afxPlacement.h"

/// O objeto afxModel descreve uma coleção de afxMesh'es que são todas ligadas ao mesmo afxSkeleton.
/// Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
/// Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo é feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

/// Rigging is the process of connecting a mesh with an internal poseable skeleton rig and bone structure. 
/// Rigged meshes allow mesh surfaces to rotate and move where internal bones are placed within the model during the modeling process.

/// O objeto afxSkeleton é uma coleção hierárquica de articulações que descrevem a estrutura articular interna de um afxModel, e auxilia no manejo e na animação do mesmo. 

/// Cada articulação no arranjo do afxSkeleton aloja o transforme para aquela articulação em dois meios diferentes. 
/// Primeiro, afxTransform "local" é o transforme decomposto da articulação relativa a seu parente imediato (dado pelo índice do parente).
/// Se esta não houver parente (isto é, o índex do parente é igual a AFX_INVALID_INDEX), então esta é relativa a origem do afxSkeleton.

/// Segundo, "iw" é uma afxM4d que é o transforme invertido no world-space para a articulação na postura padrão do afxSkeleton (isto é, a postura na qual o afxSkeleton foi originalmente modelado).

/// O afxTransform "local" é usado primariamente na composição de animações, e a afxM4d "iw" é usada primariamente para deformação de malha, razão pela qual esta informação é alojada em dois diferentes formatos.

/// Cada articulação também opcionalmente contém dados definidos pelo usuário "UDD".
/// Este campo aponta para algum dado, estranho à lógica do Qwadro, associado com a articulação, se houver qualquer.

/// Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
/// Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
/// The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.

typedef enum afxModelFlag
{
    afxModelFlag_NONE
} afxModelFlags;

typedef enum afxMeshRigFlag
{
    afxMeshRigFlag_TRANSPLANTED = AFX_BIT(0)
} afxMeshRigFlags;

AFX_DEFINE_STRUCT(afxModelInfo)
{
    afxUnit         jntCnt;
    afxString*      joints;
    afxUnit*        jntPi;
    afxTransform*   jntLt;
    afxM4d*         jntIw;
    afxReal*        jntLe;

    afxUnit         lodType;
    afxReal         allowedLodErrFadingFactor;
    afxTransform    displacement;

    afxUnit         rigCnt;
    afxUnit         riggedMeshCnt;
};

AFX_DEFINE_STRUCT(afxMeshRigInfo)
{
    afxMeshRigFlags flags;
    afxMesh         msh;
    afxUnit const*  biasToJnt;
    afxUnit const*  biasFromJnt;
    afxModel        origSkl;
    afxMaterial     txd;
};

AFX_DEFINE_STRUCT(afxModelBlueprint)
/// Data needed for model assembly
{
    afxString32         urn;
    afxUnit             lodType;
    afxBool             scalable;
    afxBool             deformable;
    afxUnit             jointCnt;
    afxString const*    joints;
    //afxUnit const*      parents;
    //afxTransform const* transforms; // local
    //afxM4d const*       matrices; // inverse world
    //afxReal const*      lodErrors;
    afxTransform        displacement;
    afxUnit             rigCnt;
};

AMX afxBool     AfxGetModelUrn(afxModel mdl, afxString* id);

AMX void        AfxDescribeModel(afxModel mdl, afxModelInfo* info);

// DISPLACEMENT

AMX void        AfxDisplaceModel(afxModel mdl, afxTransform const* t);
AMX void        AfxGetModelDisplacement(afxModel mdl, afxTransform* t);
AMX void        AfxComputeModelDisplacement(afxModel mdl, afxM4d m);

// MESH RIGGING

AMX void        AfxDescribeMeshRig(afxModel mdl, afxUnit rigIdx, afxMeshRigInfo* info);

AMX afxUnit     AfxGetRiggedMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit cnt, afxMesh meshes[]);
AMX afxError    AfxRigMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit cnt, afxMesh const meshes[], afxModel origSkl);

AMX afxBool     AfxIsMeshRigTransplanted(afxModel mdl, afxUnit rigIdx);

AMX void        AfxSetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial mtl);
AMX afxBool     AfxFindMeshRigMaterial(afxModel mdl, afxUnit rigIdx, afxString const* id, afxMaterial* material);

AMX void        AfxComputeMeshRigMatrices(afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[]);

// SKELETON

AMX afxError    AfxReparentJoints(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride);
AMX afxError    AfxResetJointTransforms(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[]);
AMX afxError    AfxResetJointMatrices(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride);
AMX afxError    AfxResetJointLodErrors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[]);

AMX afxUnit     AfxCountJoints(afxModel mdl, afxReal allowedErr);
AMX afxUnit     AfxFindJoints(afxModel mdl, afxUnit cnt, afxString const ids[], afxUnit indices[]);
AMX void        AfxQueryModelErrorTolerance(afxModel mdl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler);

AMX void        AfxComputeAttachmentWorldMatrix(afxModel mdl, afxUnit jntIdx, afxPose const pose, afxM4d const offset, afxUnit const* sparseJntMap, afxUnit const* sparseJntMapRev, afxM4d m);
AMX void        AfxComputeAttachmentOffset(afxModel mdl, afxUnit jntIdx, afxPose const pose, afxM4d const offset, afxUnit const* sparseJntMap, afxUnit const* sparseJntMapRev, afxM4d m);

AMX void        AfxBuildIndexedCompositeBuffer(afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxM4d buffer[]);
AMX void        AfxBuildIndexedCompositeBufferTransposed(afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxReal buffer[][3][4]);

AMX void        AfxComputeRestPose(afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxPose pose);

// Pass NIL as atti to AfxComputePlacement() to compute the rest placement.
// Pass TRUE as composite to AfxComputePlacement() to compute the placement's composite buffer.

AMX void        AfxComputePlacement(afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxPose const pose, afxM4d const offset, afxBool noComposite, afxPlacement plce);

// extract the pose
AMX void        AfxRebuildPose(afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const offset, afxPlacement const plce, afxBool rigid, afxPose pose);

AMX afxError    AfxArchiveModel(afxModel mdl, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

AMX afxUnit     AfxEnumerateModels(afxSimulation sim, afxUnit first, afxUnit cnt, afxModel models[]);

AMX void        AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxUnit cnt, afxModel models[]);

AMX afxError    AfxAssembleModels(afxSimulation sim, afxUnit cnt, afxModelBlueprint const blueprints[], afxModel models[]);

AMX afxError    AfxLoadModel(afxSimulation sim, afxString const* urn, afxUri const* uri, afxModel* model);

#endif//AMX_MODEL_H
