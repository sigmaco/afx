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
#include "qwadro/inc/math/coll/afxBox.h"
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

ASX afxError AfxDestripifyTriangles(afxUnit triCnt, void const* src, afxUnit srcIdxSiz, void* dst, afxUnit dstIdxSiz);

ASX afxError AfxComputeTriangleTangents(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV2d const uv[], afxUnit uvStride, afxBool perTriOut, afxV3d tan[], afxUnit tanStride, afxV3d bit[], afxUnit bitStride);

ASX afxError AfxComputeTriangleNormals(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV3d nrm[], afxUnit nrmStride);

ASX afxError AfxComputeTriangleSkins(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV2d uv[], afxUnit uvStride);

ASX afxError AfxGenerateTriangleSkins(afxUnit triCnt, void const* indices, afxUnit idxStride, afxUnit stride, afxV2d uv[]);

ASX void AsxExtractIndexedLines(afxUnit triCnt, afxUnit triIndices[], afxUnit triIdxStride, afxUnit bufCap, afxUnit edgeIndices[], afxUnit* numLines);

ASX void AsxInvertIndexedTriangles(afxUnit triCnt, afxUnit indices[], afxUnit idxStride);

#endif//ASX_MESH_BUILDER_H
