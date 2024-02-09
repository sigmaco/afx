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

#ifndef AFX_MESH_BUILDER_H
#define AFX_MESH_BUILDER_H

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/sim/afxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/core/afxFixedString.h"

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
    _AFX_DBG_FCC;
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
    afxFixedString32    id;
};

AFXINL afxError         AfxBeginMeshBuilding(afxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt);
AFXINL void             AfxEndMeshBuilding(afxMeshBuilder* mshb);

AFXINL void             AfxUpdateVertices(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[]);
AFXINL void             AfxUpdateVertexPositions(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][3], afxNat srcStride);
AFXINL void             AfxUpdateVertexPositions4(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][4], afxNat srcStride);
AFXINL afxError         AfxUpdateVertexNormals(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const nrm[][3], afxNat srcStride);
AFXINL afxError         AfxUpdateVertexWraps(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const uv[][2], afxNat srcStride);

AFXINL void             AfxEmitTriangles(afxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3]);

AFXINL afxNat           AfxAddVertexBiases(afxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[]);

AFXINL void             AfxRenameVertexPivots(afxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[]);

AFX afxMesh             AfxBuildCubeMesh(afxSimulation sim, afxReal scale);
AFX afxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxReal width, afxReal height);
AFX afxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices);

AFX_DEFINE_STRUCT(afxModelAssembler)
{
    afxSimulation _sim;

    afxStringCatalog pvtCatalog;
    afxStringCatalog attrCatalog;
    afxStringCatalog idCatalog;

    afxArray        bones;
    afxArray        skeletons;
    afxArray        models;
    afxArray        meshes;
};

// afxStringCatalog strc, afxMesh const meshes[], afxNat cnt, afxModelBlueprint const blueprints[]
AFX afxError    _AfxAssembleModel(afxSimulation sim, afxModelAssembler* assembler, afxNat cnt, afxNat const blueIndices[], afxModel models[]);

AFX afxError    AfxPushMeshBlueprint(afxModelAssembler mdla, afxString const* id, afxNat* blueIdx);



AFX afxError    AfxPushModelBlueprint(afxModelAssembler mdla, afxNat meshCnt, afxString const* id, afxNat* blueIdx);

AFX void        AfxPushModelInitialPlacement(afxModelAssembler mdla, afxTransform const* t);
AFX void        AfxPushLinkedMesh(afxModelAssembler mdla, afxNat slotIdx, afxMesh msh);

AFX afxError    AfxPushSkeletonBlueprint(afxModelAssembler mdla, afxNat boneCnt, afxString const* id, afxNat* blueIdx);

AFX void        AfxPushBoneTransform(afxModelAssembler mdla, afxNat boneIdx, afxTransform const* lt);
AFX void        AfxPushBoneWorldTransform(afxModelAssembler mdla, afxNat boneIdx, afxTransform const* wt);
AFX void        AfxPushBoneIwtm(afxModelAssembler mdla, afxNat boneIdx, afxReal const iw[4][4]);
AFX void        AfxPushBoneId(afxModelAssembler mdla, afxNat boneIdx, afxString const* id);
AFX void        AfxPushBoneParent(afxModelAssembler mdla, afxNat boneIdx, afxNat parentIdx);

AFX void        AfxPushBoneTransforms(afxModelAssembler mdla, afxNat boneIdx, afxTransform const* lt, afxTransform const* wt, afxReal const iw[4][4]);

AFX void        AfxSetPivotNameCatalog(afxModelAssembler mdla, afxStringCatalog strc);
AFX void        AfxSetVertexAttributeNameCatalog(afxModelAssembler mdla, afxStringCatalog strc);

AFX void        AfxPushVertexData(afxModelAssembler mdla, awxVertexData vtd);
AFX void        AddPushMeshTopology(afxModelAssembler mdla, afxMeshTopology msht);

AFX afxNat      AfxBlueprintMesh(afxModelAssembler mdla, afxString const* id, afxNat vtxIdx, afxNat mshtIdx, afxNat pvtCnt);

#endif//AFX_MESH_BUILDER_H
