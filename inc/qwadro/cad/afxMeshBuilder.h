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

#ifndef AFX_MESH_BUILDER_H
#define AFX_MESH_BUILDER_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxBox.h"
#include "qwadro/base/afxObject.h"
#include "qwadro/cad/afxMaterial.h"
#include "qwadro/space/afxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/base/afxFixedString.h"

AFX_DEFINE_STRUCT(afxVertex)
{
    afxNat              baseBiasIdx;
    afxNat              biasCnt;
};

AFX_DEFINE_STRUCT(afxCoveredTriangle)
{
    afxNat              vtxIdx[3];
    afxNat              surfIdx;
};

AFX_DEFINE_STRUCT(afxMeshBuilder)
{
    afxArray            faces2;
    afxNat              triCnt;
    struct
    {
        afxNat          vtxIdx[3];
        afxNat          surfIdx;
        afxNat          artCnt;
    }*                  faces;
    afxNat              surfCnt;
    afxArray            biases;

    afxNat              artCnt;
    afxString*          pivots;
    struct
    {
        afxString       name;
        afxNat          triCnt;
    }*                  pivots2;

    afxNat              vtxCnt;
    afxVertex*          vtx;
    afxV4d*             posn;
    afxV3d*             nrm;
    afxV2d*             uv;
    afxString32    id;
};

AKXINL afxError         AfxBeginMeshBuilding(afxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt);
AKXINL void             AfxEndMeshBuilding(afxMeshBuilder* mshb);

AKXINL void             AfxUpdateVertices(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[]);
AKXINL void             AfxUpdateVertexPositions(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV3d const posn[], afxNat srcStride);
AKXINL void             AfxUpdateVertexPositions4(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV4d const posn[], afxNat srcStride);
AKXINL afxError         AfxUpdateVertexNormals(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV3d const nrm[], afxNat srcStride);
AKXINL afxError         AfxUpdateVertexWraps(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV2d const uv[], afxNat srcStride);

AKXINL void             AfxEmitTriangles(afxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3]);

AKXINL afxNat           AfxAddVertexBiases(afxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[]);

AKXINL void             AfxRenameVertexPivots(afxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[]);

AKX afxMesh             AfxBuildCubeMesh(afxSimulation sim, afxReal scale);
AKX afxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxReal width, afxReal height);
AKX afxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices);

AFX_DEFINE_STRUCT(afxModelAssembler)
{
    afxSimulation _sim;

    afxStringBase pvtCatalog;
    afxStringBase attrCatalog;
    afxStringBase idCatalog;

    afxArray        bones;
    afxArray        skeletons;
    afxArray        models;
    afxArray        meshes;
};

AKX afxError    AfxAssembleModels(afxModelAssembler mdla, afxNat cnt, afxString const id[], afxNat sklIdx[], afxNat mshCnt[], afxNat mshIdx[][1], afxTransform init[]);

// afxStringBase strb, afxMesh const meshes[], afxNat cnt, afxModelBlueprint const blueprints[]
AKX afxError    _AfxAssembleModel(afxSimulation sim, afxModelAssembler* assembler, afxNat cnt, afxNat const blueIndices[], afxModel models[]);

AKX afxError    AfxPushMeshBlueprint(afxModelAssembler mdla, afxString const* id, afxNat* blueIdx);



AKX afxError    AfxPushModelBlueprint(afxModelAssembler mdla, afxNat meshCnt, afxString const* id, afxNat* blueIdx);

AKX void        AfxPushModelInitialPlacement(afxModelAssembler mdla, afxTransform const* t);
AKX void        AfxPushLinkedMesh(afxModelAssembler mdla, afxNat slotIdx, afxMesh msh);

AKX afxError    AfxPushSkeletonBlueprint(afxModelAssembler mdla, afxNat boneCnt, afxString const* id, afxNat* blueIdx);

AKX void        AfxPushBoneTransform(afxModelAssembler mdla, afxNat boneIdx, afxTransform const* lt);
AKX void        AfxPushBoneWorldTransform(afxModelAssembler mdla, afxNat boneIdx, afxTransform const* wt);
AKX void        AfxPushBoneIwtm(afxModelAssembler mdla, afxNat boneIdx, afxReal const iw[4][4]);
AKX void        AfxPushBoneId(afxModelAssembler mdla, afxNat boneIdx, afxString const* id);
AKX void        AfxPushBoneParent(afxModelAssembler mdla, afxNat boneIdx, afxNat parentIdx);

AKX void        AfxPushBoneTransforms(afxModelAssembler mdla, afxNat boneIdx, afxTransform const* lt, afxTransform const* wt, afxReal const iw[4][4]);

AKX void        AfxSetPivotNameCatalog(afxModelAssembler mdla, afxStringBase strb);
AKX void        AfxSetVertexAttributeNameCatalog(afxModelAssembler mdla, afxStringBase strb);

AKX void        AfxPushVertexData(afxModelAssembler mdla, akxVertexData vtd);
AKX void        AddPushMeshTopology(afxModelAssembler mdla, afxMeshTopology msht);

AKX afxNat      AfxBlueprintMesh(afxModelAssembler mdla, afxString const* id, afxNat vtxIdx, afxNat mshtIdx, afxNat pvtCnt);

#endif//AFX_MESH_BUILDER_H
