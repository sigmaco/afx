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

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ASX_MISC_OPS_H
#define ASX_MISC_OPS_H

#include "arxSampleContext.h"
#include "qwadro/cad/arxMesh.h"

ASX afxCmdId AsxCmdBindBuffers(arxContext ctx, afxUnit baseSlot, afxUnit cnt, avxBuffer buffers[], afxUnit const offsets[], afxUnit const ranges[]);

// MESH OPS

ASX afxCmdId        AsxCmdChangeMeshMorphes(arxContext sctx, arxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, arxMeshMorph const morphes[]);

ASX afxCmdId        AsxCmdResetMeshSections(arxContext sctx, arxMesh msh, afxUnit baseSecIdx, afxUnit cnt, arxMeshSection const sections[]);

ASX afxCmdId        AsxCmdInvertMeshTopology(arxContext sctx, arxMesh msh);

ASX afxCmdId        AsxCmdRemapMeshCoverage(arxContext sctx, arxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

ASX afxCmdId        AsxCmdUpdateMeshIndices(arxContext sctx, arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);
ASX afxCmdId        AsxCmdDumpMeshIndices(arxContext sctx, arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz);

ASX afxCmdId        AsxCmdRecomputeMeshBounds(arxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx);
ASX afxCmdId        AsxCmdUpdateMeshBounds(arxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[]);

ASX afxCmdId        AsxCmdRecomputeMeshNormals(arxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx);
ASX afxCmdId        AsxCmdRecomputeMeshTangents(arxContext sctx, arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx);

ASX afxCmdId        AsxCmdTransformMeshes(arxContext sctx, afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, arxMesh meshes[]);

// MESH OPT EXT

// Reorders indices to reduce the number of GPU vertex shader invocations.
ASX afxCmdId        AsxCmdOptimizeVertexCacheEXT(arxContext sctx, arxBuffer vbo, afxSize vtxOffset, afxUnit vtxCnt, arxBuffer ibo, afxSize idxOffset, afxUnit idxCnt, avxTopology top);

// Reorders indices to reduce the number of GPU vertex shader invocations and the pixel overdraw.
ASX afxCmdId        AsxCmdOptimizeVertexOverdrawEXT(arxContext sctx, arxBuffer vbo, afxUnit vtxCnt, afxUnit vtxStride, arxBuffer ibo, afxSize idxOffset, afxUnit idxCnt, arxBuffer dst, afxSize dstBase, afxReal threshold);

// Reorders vertices and changes indices to reduce the amount of GPU memory fetches during vertex processing.
ASX afxCmdId        AsxCmdOptimizeVertexFetchEXT(arxContext sctx, arxBuffer vbo, afxSize vtxOffset, afxUnit vtxSiz, arxBuffer ibo, afxSize idxOffset, afxUnit idxCnt, arxBuffer dstVbo, afxSize dstVboOffset);

// Reduces the number of triangles in the mesh, attempting to preserve mesh appearance as much as possible.
ASX afxCmdId        AsxCmdSimplifyTrianglesEXT(arxContext sctx, arxBuffer ibo, afxSize iboOffset, afxUnit idxCnt, arxBuffer vbo, afxSize vboPosOffset, afxUnit vtxPosStride, afxUnit vtxCnt, afxReal tol, afxFlags flags, arxBuffer dst, afxSize dstOffset);

#endif//ASX_MISC_OPS_H
