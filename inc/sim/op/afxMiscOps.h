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

#ifndef ASX_MISC_OPS_H
#define ASX_MISC_OPS_H

#include "afxSampleContext.h"
#include "qwadro/inc/render/cad/arxMesh.h"

ASX afxCmdId AsxCmdBindBuffers(afxContext ctx, afxUnit baseSlot, afxUnit cnt, avxBuffer buffers[], afxUnit const offsets[], afxUnit const ranges[]);

// MESH OPS

ASX afxCmdId        AsxCmdChangeMeshMorphes(afxContext sctx, arxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, arxMeshMorph const morphes[]);

ASX afxCmdId        AsxCmdResetMeshSections(afxContext sctx, arxMesh msh, afxUnit baseSecIdx, afxUnit cnt, arxMeshSection const sections[]);

ASX afxCmdId        AsxCmdInvertMeshTopology(afxContext sctx, arxMesh msh);

ASX afxCmdId        AsxCmdRemapMeshCoverage(afxContext sctx, arxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

ASX afxCmdId        AsxCmdUpdateMeshIndices(afxContext sctx, arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);
ASX afxCmdId        AsxCmdDumpMeshIndices(afxContext sctx, arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz);

ASX afxCmdId        AsxCmdRecomputeMeshBounds(afxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx);
ASX afxCmdId        AsxCmdUpdateMeshBounds(afxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[]);

ASX afxCmdId        AsxCmdRecomputeMeshNormals(afxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx);
ASX afxCmdId        AsxCmdRecomputeMeshTangents(afxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx);

ASX afxCmdId        AsxCmdTransformMeshes(afxContext sctx, afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, arxMesh meshes[]);

// MESH OPT EXT

// Reorders indices to reduce the number of GPU vertex shader invocations.
ASX afxCmdId        AsxCmdOptimizeVertexCacheEXT(afxContext sctx, asxBuffer vbo, afxSize vtxOffset, afxUnit vtxCnt, asxBuffer ibo, afxSize idxOffset, afxUnit idxCnt, avxTopology top);

// Reorders indices to reduce the number of GPU vertex shader invocations and the pixel overdraw.
ASX afxCmdId        AsxCmdOptimizeVertexOverdrawEXT(afxContext sctx, asxBuffer vbo, afxUnit vtxCnt, afxUnit vtxStride, asxBuffer ibo, afxSize idxOffset, afxUnit idxCnt, asxBuffer dst, afxSize dstBase, afxReal threshold);

// Reorders vertices and changes indices to reduce the amount of GPU memory fetches during vertex processing.
ASX afxCmdId        AsxCmdOptimizeVertexFetchEXT(afxContext sctx, asxBuffer vbo, afxSize vtxOffset, afxUnit vtxSiz, asxBuffer ibo, afxSize idxOffset, afxUnit idxCnt, asxBuffer dstVbo, afxSize dstVboOffset);

// Reduces the number of triangles in the mesh, attempting to preserve mesh appearance as much as possible.
ASX afxCmdId        AsxCmdSimplifyTrianglesEXT(afxContext sctx, asxBuffer ibo, afxSize iboOffset, afxUnit idxCnt, asxBuffer vbo, afxSize vboPosOffset, afxUnit vtxPosStride, afxUnit vtxCnt, afxReal tol, afxFlags flags, asxBuffer dst, afxSize dstOffset);

#endif//ASX_MISC_OPS_H
