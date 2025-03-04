/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_MESH_BUILDER_H
#define ASX_MESH_BUILDER_H

#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/bound/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/sim/afxSimDefs.h"

AFX_DEFINE_STRUCT(akxVertex)
{
    afxUnit              baseBiasIdx;
    afxUnit              biasCnt;
};

AFX_DEFINE_STRUCT(akxCoveredTriangle)
{
    afxUnit              vtxIdx[3];
    afxUnit              surfIdx;
};

AFX_DEFINE_STRUCT(afxTriangulation)
{
    afxArray            faces2;
    afxUnit              triCnt;
    struct
    {
        afxUnit          vtxIdx[3];
        afxUnit          secIdx;
        afxUnit          artCnt;
    }*                  faces;
    afxUnit              secCnt;
    afxArray            biases;

    afxUnit              artCnt;
    afxString*          pivots;
    struct
    {
        afxString       name;
        afxUnit          triCnt;
    }*                  pivots2;

    afxUnit              vtxCnt;
    akxVertex*          vtx;
    afxV4d*             posn;
    afxV3d*             nrm;
    afxV2d*             uv;
    afxString32    id;
};

ASXINL afxError         AfxBeginMeshBuilding(afxTriangulation* mshb, afxString const* id, afxUnit vtxCnt, afxUnit triCnt, afxUnit secCnt, afxUnit artCnt);
ASXINL void             AfxEndMeshBuilding(afxTriangulation* mshb);

ASXINL void             AfxUpdateVertices(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxUnit const baseBiasIdx[], afxUnit const biasCnt[]);
ASXINL void             AfxUpdateVertexPositions(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const posn[], afxUnit srcStride);
ASXINL void             AfxUpdateVertexPositions4(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV4d const posn[], afxUnit srcStride);
ASXINL afxError         AfxUpdateVertexNormals(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const nrm[], afxUnit srcStride);
ASXINL afxError         AfxUpdateVertexWraps(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV2d const uv[], afxUnit srcStride);

ASXINL void             AfxEmitTriangles(afxTriangulation* mshb, afxUnit surfIdx, afxUnit baseTriIdx, afxUnit triCnt, afxUnit const triVtxIdx[][3]);

ASXINL afxUnit           AfxAddVertexBiases(afxTriangulation* mshb, afxUnit cnt, afxUnit const jntIdx[], afxReal const weight[]);

ASXINL void             AfxRenameVertexPivots(afxTriangulation* mshb, afxUnit basePivotIdx, afxUnit cnt, afxString const name[]);

ASX afxMesh             AfxBuildParallelepipedMesh(afxSimulation sim, afxV3d whd, afxReal slantX, afxReal slantY);
ASX afxMesh             AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxUnit slices);

ASX afxMesh             AfxBuildDomeMesh2(afxSimulation sim, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
ASX afxMesh             AfxBuildSphereMesh(afxSimulation sim, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
ASX afxMesh             AfxBuildCapsuleMesh(afxSimulation sim, afxReal radius, afxReal height, afxUnit stacks, afxUnit slices, afxUnit cylinderSlices, afxBool inv);
ASX afxMesh             AfxBuildPlaneMesh(afxSimulation sim, afxUnit gridSizeX, afxUnit gridSizeY, afxReal width, afxReal height);

ASX afxError            AfxBuildBoxMesh(afxSimulation sim, afxV3d const whd, afxUnit secCnt, afxMesh* mesh);
ASX afxMesh             AfxBuildDiscMesh(afxSimulation sim, afxReal radius, afxUnit secCnt);
ASX afxError            AfxBuildGridMesh(afxSimulation sim, afxUnit rows, afxUnit layers, afxReal width, afxReal depth, afxMesh* mesh);

ASX afxError AfxDestripifyTriangles(afxUnit triCnt, void const* src, afxUnit srcIdxSiz, void* dst, afxUnit dstIdxSiz);
ASX afxError AfxRecomputeTriangularNormals(afxUnit triCnt, afxUnit const indices[], afxV3d const pos[], afxV3d nrm[]);
ASX afxError AfxRecomputeTriangularTangents(afxUnit triCnt, afxUnit const indices[], afxV3d const pos[], afxV2d const uv[], afxUnit distro, afxV3d tan[], afxV3d bit[]);

#endif//ASX_MESH_BUILDER_H
