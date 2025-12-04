/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ARX_MODEL_H
#define ARX_MODEL_H

/*
    O objeto arxModel descreve uma coleção de arxMesh'es que são todas ligadas ao mesmo arxSkeleton.
    Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
    Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
    Um arxModel completo é feito de um arxSkeleton e um conjunto de arxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do arxModel.

    Rigging is the process of connecting a mesh with an internal poseable skeleton rig and bone structure. 
    Rigged meshes allow mesh surfaces to rotate and move where internal bones are placed within the model during the modeling process.
*/

#include "qwadro/cad/arxMeshIo.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxPlacement.h"
#include "qwadro/cad/arxSkeleton.h"

typedef enum arxModelFlag
{
    arxModelFlag_NONE
} arxModelFlags;

typedef enum arxMeshRigFlag
{
    arxMeshRigFlag_TRANSPLANTED = AFX_BITMASK(0)
} arxMeshRigFlags;

AFX_DEFINE_STRUCT(arxModelInfo)
{
    arxSkeleton     skl;
    arxSkeletonInfo skli;

    afxUnit         lodType;
    afxReal         allowedLodErrFadingFactor;
    afxTransform    displace;

    arxMtd          mtd;
    afxUnit         rigCnt;
    afxUnit         riggedMeshCnt;
};

AFX_DEFINE_STRUCT(arxMeshRigging)
{
    arxMeshRigFlags flags;
    arxMesh         msh;
    arxSkeleton     origSkl;
};

AFX_DEFINE_STRUCT(arxMeshRigInfo)
{
    arxMeshRigFlags flags;
    arxMesh         msh;
    arxSkeleton     origSkl;
    afxUnit const*  biasFromJntMap;
    afxUnit const*  biasToJntMap;
    afxUnit const*  mtlMap;
};

AFX_DEFINE_STRUCT(arxModelBlueprint)
// Data needed for model assembly
{
    arxSkeleton         skl;
    afxUnit             boneCnt;
    afxString const*    bones;
    afxBool             scalable;
    afxBool             deformable;
    afxUnit             lodType;
    arxMtd              mtd;
    afxUnit             rigCnt;
    afxTransform        displace;
};

ARX afxBool ArxGetModelUrn
(
    arxModel mdl, 
    afxString* id
);

ARX arxSkeleton ArxGetSkeleton
(
    arxModel mdl
);

ARX void ArxDescribeModel
(
    arxModel mdl, 
    arxModelInfo* info
);

// DISPLACEMENT

ARX void ArxDisplaceModel
(
    arxModel mdl, 
    afxTransform const* t
);

ARX void ArxGetModelDisplacement
(
    arxModel mdl, 
    afxTransform* t
);

ARX void ArxComputeModelDisplacement
(
    arxModel mdl, 
    afxM4d m
);

// MESH RIGGING

ARX void ArxDescribeMeshRigs
(
    arxModel mdl, 
    afxUnit baseRigIdx, 
    afxUnit cnt, 
    arxMeshRigInfo info[]
);

ARX afxUnit ArxGetRiggedMeshes
(
    arxModel mdl, 
    afxUnit baseRigIdx, 
    afxUnit cnt, 
    arxMesh meshes[]
);

ARX afxError ArxRigMeshes
(
    arxModel mdl, 
    afxUnit baseRigIdx, 
    afxUnit rigCnt, 
    arxMeshRigging const meshes[]
);

ARX afxBool ArxIsRiggedMeshTransplanted
(
    arxModel mdl, 
    afxUnit rigIdx
);

ARX void ArxSetMeshRigTxd
(
    arxModel mdl, 
    afxUnit rigIdx, 
    arxMaterial mtl
);

ARX afxBool ArxFindMeshRigMaterial
(
    arxModel mdl, 
    afxUnit rigIdx, 
    afxString const* id, 
    arxMaterial* material
);

ARX void ArxComputeRiggedMeshMatrices
(
    arxModel mdl, 
    afxUnit rigIdx, 
    arxPlacement plce, 
    afxUnit baseBiasIdx, 
    afxUnit biasCnt, 
    afxM4d matrices[]
);

// SKELETON

ARX afxUnit ArxCountJoints
(
    arxModel mdl, 
    afxReal allowedErr
);

ARX void ArxQueryModelErrorTolerance
(
    arxModel mdl, 
    afxReal allowedErr, 
    afxReal* allowedErrEnd, 
    afxReal* allowedErrScaler
);

ARX void ArxBuildRiggedMeshCompositeMatrices
(
    arxModel mdl, 
    afxUnit rigIdx, 
    arxPlacement plce, 
    afxUnit cnt, 
    afxM4d matrices[]
);

ARX afxError ArxArchiveModel
(
    arxModel mdl, 
    afxUri const* uri
);

////////////////////////////////////////////////////////////////////////////////

ARX void ArxTransformModels
(
    afxM3d const ltm, 
    afxM3d const iltm, 
    afxReal ltmTol, 
    afxV3d const atv, 
    afxReal atvTol, 
    afxFlags flags, 
    afxUnit cnt, 
    arxModel models[]
);

ARX afxUnit ArxEnumerateModels
(
    arxScenario scio, 
    afxUnit first, 
    afxUnit cnt, 
    arxModel models[]
);

ARX afxError ArxAssembleModels
(
    arxScenario scio, 
    afxUnit cnt, 
    afxString const urns[], 
    arxModelBlueprint const blueprints[], 
    arxModel models[]
);

ARX afxError ArxLoadModel
(
    arxScenario scio, 
    arxMtd mtd,
    arxSkeleton skl,
    afxString const* urn, 
    afxUri const* uri, 
    arxModel* model
);

ARX afxBool ArxIdentifyModels
(
    arxScenario scio,
    afxUnit cnt,
    afxString const ids[],
    arxModel models[]
);

#endif//ARX_MODEL_H
