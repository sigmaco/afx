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
#include "qwadro/inc/sim/io/afxMesh.h"

ASX afxCmdId AsxCmdBindBuffers(afxContext ctx, afxUnit baseSlot, afxUnit cnt, afxBuffer buffers[], afxUnit const offsets[], afxUnit const ranges[]);

// MESH OPS

ASX afxCmdId        AmxCmdChangeMeshMorphes(afxContext mctx, afxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, afxMeshMorph const morphes[]);

ASX afxCmdId        AmxCmdResetMeshSections(afxContext mctx, afxMesh msh, afxUnit baseSecIdx, afxUnit cnt, afxMeshSection const sections[]);

ASX afxCmdId        AmxCmdInvertMeshTopology(afxContext mctx, afxMesh msh);

ASX afxCmdId        AmxCmdRemapMeshCoverage(afxContext mctx, afxMesh msh, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[]);

ASX afxCmdId        AmxCmdUpdateMeshIndices(afxContext mctx, afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz);
ASX afxCmdId        AmxCmdDumpMeshIndices(afxContext mctx, afxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz);

ASX afxCmdId        AmxCmdRecomputeMeshBounds(afxContext mctx, afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx);
ASX afxCmdId        AmxCmdUpdateMeshBounds(afxContext mctx, afxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[]);

ASX afxCmdId        AmxCmdRecomputeMeshNormals(afxContext mctx, afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx);
ASX afxCmdId        AmxCmdRecomputeMeshTangents(afxContext mctx, afxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx);

ASX afxCmdId        AmxCmdTransformMeshes(afxContext mctx, afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxMesh meshes[]);

#endif//ASX_MISC_OPS_H
