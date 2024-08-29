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

#ifndef AVX_MESH_BUILDER_H
#define AVX_MESH_BUILDER_H

#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
#include "qwadro/inc/base/afxFixedString.h"

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

AVXINL afxError         AfxBeginMeshBuilding(afxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt);
AVXINL void             AfxEndMeshBuilding(afxMeshBuilder* mshb);

AVXINL void             AfxUpdateVertices(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[]);
AVXINL void             AfxUpdateVertexPositions(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV3d const posn[], afxNat srcStride);
AVXINL void             AfxUpdateVertexPositions4(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV4d const posn[], afxNat srcStride);
AVXINL afxError         AfxUpdateVertexNormals(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV3d const nrm[], afxNat srcStride);
AVXINL afxError         AfxUpdateVertexWraps(afxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxV2d const uv[], afxNat srcStride);

AVXINL void             AfxEmitTriangles(afxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3]);

AVXINL afxNat           AfxAddVertexBiases(afxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[]);

AVXINL void             AfxRenameVertexPivots(afxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[]);

AVX afxMesh             AfxBuildCubeMesh(afxDrawInput din, afxReal scale, afxNat secCnt);
AVX afxMesh             AfxBuildParallelepipedMesh(afxDrawInput din, afxV3d whd, afxReal slantX, afxReal slantY);
AVX afxMesh             AfxBuildDomeMesh(afxDrawInput din, afxReal radius, afxNat slices);

AVX afxMesh             AfxBuildDomeMesh2(afxDrawInput din, afxReal radius, afxNat stacks, afxNat slices, afxBool inv);
AVX afxMesh             AfxBuildSphereMesh(afxDrawInput din, afxReal radius, afxNat stacks, afxNat slices, afxBool inv);
AVX afxMesh             AfxBuildCapsuleMesh(afxDrawInput din, afxReal radius, afxReal height, afxNat stacks, afxNat slices, afxNat cylinderSlices, afxBool inv);
AVX afxMesh             AfxBuildPlaneMesh(afxDrawInput din, afxNat gridSizeX, afxNat gridSizeY, afxReal width, afxReal height);

#endif//AVX_MESH_BUILDER_H
