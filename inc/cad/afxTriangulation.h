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

AVXINL afxError         AfxBeginMeshBuilding(afxTriangulation* mshb, afxString const* id, afxUnit vtxCnt, afxUnit triCnt, afxUnit secCnt, afxUnit artCnt);
AVXINL void             AfxEndMeshBuilding(afxTriangulation* mshb);

AVXINL void             AfxUpdateVertices(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxUnit const baseBiasIdx[], afxUnit const biasCnt[]);
AVXINL void             AfxUpdateVertexPositions(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const posn[], afxUnit srcStride);
AVXINL void             AfxUpdateVertexPositions4(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV4d const posn[], afxUnit srcStride);
AVXINL afxError         AfxUpdateVertexNormals(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const nrm[], afxUnit srcStride);
AVXINL afxError         AfxUpdateVertexWraps(afxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV2d const uv[], afxUnit srcStride);

AVXINL void             AfxEmitTriangles(afxTriangulation* mshb, afxUnit surfIdx, afxUnit baseTriIdx, afxUnit triCnt, afxUnit const triVtxIdx[][3]);

AVXINL afxUnit           AfxAddVertexBiases(afxTriangulation* mshb, afxUnit cnt, afxUnit const jntIdx[], afxReal const weight[]);

AVXINL void             AfxRenameVertexPivots(afxTriangulation* mshb, afxUnit basePivotIdx, afxUnit cnt, afxString const name[]);

AVX afxMesh             AfxBuildParallelepipedMesh(afxDrawInput din, afxV3d whd, afxReal slantX, afxReal slantY);
AVX afxMesh             AfxBuildDomeMesh(afxDrawInput din, afxReal radius, afxUnit slices);

AVX afxMesh             AfxBuildDomeMesh2(afxDrawInput din, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
AVX afxMesh             AfxBuildSphereMesh(afxDrawInput din, afxReal radius, afxUnit stacks, afxUnit slices, afxBool inv);
AVX afxMesh             AfxBuildCapsuleMesh(afxDrawInput din, afxReal radius, afxReal height, afxUnit stacks, afxUnit slices, afxUnit cylinderSlices, afxBool inv);
AVX afxMesh             AfxBuildPlaneMesh(afxDrawInput din, afxUnit gridSizeX, afxUnit gridSizeY, afxReal width, afxReal height);

AVX afxError            AfxBuildBoxMesh(afxDrawInput din, afxV3d const whd, afxUnit secCnt, afxMesh* mesh);
AVX afxMesh             AfxBuildDiscMesh(afxDrawInput din, afxReal radius, afxUnit secCnt);
AVX afxError            AfxBuildGridMesh(afxDrawInput din, afxUnit rows, afxUnit layers, afxReal width, afxReal depth, afxMesh* mesh);

AVX afxError AfxDestripifyTriangles(afxUnit triCnt, void const* src, afxUnit srcIdxSiz, void* dst, afxUnit dstIdxSiz);
AVX afxError AfxRecomputeTriangularNormals(afxUnit triCnt, afxUnit const indices[], afxV3d const pos[], afxV3d const nrm[]);
AVX afxError AfxRecomputeTriangularTangents(afxUnit triCnt, afxUnit const indices[], afxV3d const pos[], afxV2d const uv[], afxUnit distro, afxV3d tan[], afxV3d bit[]);

#endif//AVX_MESH_BUILDER_H
