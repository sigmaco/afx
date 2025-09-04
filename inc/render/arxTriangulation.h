/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ARX_MESH_BUILDER_H
#define ARX_MESH_BUILDER_H

#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/render/arxRenderable.h"

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

AFX_DEFINE_STRUCT(arxTriangulation)
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

ARXINL afxError         ArxBeginMeshBuilding(arxTriangulation* mshb, afxString const* id, afxUnit vtxCnt, afxUnit triCnt, afxUnit secCnt, afxUnit artCnt);
ARXINL void             ArxEndMeshBuilding(arxTriangulation* mshb);

ARXINL void             ArxUpdateVertices(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxUnit const baseBiasIdx[], afxUnit const biasCnt[]);
ARXINL void             ArxUpdateVertexPositions(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const posn[], afxUnit srcStride);
ARXINL void             ArxUpdateVertexPositions4(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV4d const posn[], afxUnit srcStride);
ARXINL afxError         ArxUpdateVertexNormals(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const nrm[], afxUnit srcStride);
ARXINL afxError         ArxUpdateVertexWraps(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV2d const uv[], afxUnit srcStride);

ARXINL void             ArxEmitTriangles(arxTriangulation* mshb, afxUnit surfIdx, afxUnit baseTriIdx, afxUnit triCnt, afxUnit const triVtxIdx[][3]);

ARXINL afxUnit           ArxAddVertexBiases(arxTriangulation* mshb, afxUnit cnt, afxUnit const jntIdx[], afxReal const weight[]);

ARXINL void             ArxRenameVertexPivots(arxTriangulation* mshb, afxUnit basePivotIdx, afxUnit cnt, afxString const name[]);

ARX afxError ArxDestripifyTriangles(afxUnit triCnt, void const* src, afxUnit srcIdxSiz, void* dst, afxUnit dstIdxSiz);

ARX afxError ArxComputeTriangleTangents(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV2d const uv[], afxUnit uvStride, afxBool perTriOut, afxV3d tan[], afxUnit tanStride, afxV3d bit[], afxUnit bitStride);

ARX afxError ArxComputeTriangleNormals(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV3d nrm[], afxUnit nrmStride);

ARX afxError ArxComputeTriangleSkins(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV2d uv[], afxUnit uvStride);

ARX afxError ArxGenerateTriangleSkins(afxUnit triCnt, void const* indices, afxUnit idxStride, afxUnit stride, afxV2d uv[]);

ARX void ArxExtractIndexedLines(afxUnit triCnt, afxUnit triIndices[], afxUnit triIdxStride, afxUnit bufCap, afxUnit edgeIndices[], afxUnit* numLines);

ARX void ArxInvertIndexedTriangles(afxUnit triCnt, afxUnit indices[], afxUnit idxStride);

#endif//ARX_MESH_BUILDER_H
