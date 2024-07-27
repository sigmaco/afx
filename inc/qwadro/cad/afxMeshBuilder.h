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

#ifndef AKX_MESH_BUILDER_H
#define AKX_MESH_BUILDER_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxBox.h"
#include "qwadro/base/afxObject.h"
#include "qwadro/cad/afxMaterial.h"
#include "qwadro/space/afxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/base/afxFixedString.h"

AFX_DEFINE_STRUCT(akxVertex)
{
    afxNat              baseBiasIdx;
    afxNat              biasCnt;
};

AFX_DEFINE_STRUCT(akxCoveredTriangle)
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
    akxVertex*          vtx;
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

AKX afxMesh             AfxBuildCubeMesh(afxSimulation sim, afxReal scale, afxNat secCnt);
AKX afxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxV3d whd, afxReal slantX, afxReal slantY);
AKX afxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices);

AKX afxMesh             AfxBuildDomeMesh2(afxSimulation sim, afxReal radius, afxNat stacks, afxNat slices, afxBool inv);
AKX afxMesh             AfxBuildSphereMesh(afxSimulation sim, afxReal radius, afxNat stacks, afxNat slices, afxBool inv);
AKX afxMesh             AfxBuildCapsuleMesh(afxSimulation sim, afxReal radius, afxReal height, afxNat stacks, afxNat slices, afxNat cylinderSlices, afxBool inv);
AKX afxMesh             AfxBuildPlaneMesh(afxSimulation sim, afxNat gridSizeX, afxNat gridSizeY, afxReal width, afxReal height);

#endif//AKX_MESH_BUILDER_H
