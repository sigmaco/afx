/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ARX_MESH_IO_H
#define ARX_MESH_IO_H

#include "qwadro/cad/arxMesh.h"

ARX afxError ArxUpdateMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    void const* src, 
    afxUnit srcIdxSiz
);

ARX afxError ArxDumpMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    void* dst, 
    afxUnit dstIdxSiz
);

ARX afxError ArxUploadMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    afxStream in, 
    afxUnit srcIdxSiz
);

ARX afxError ArxDownloadMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    afxStream out, 
    afxUnit dstIdxSiz
);

ARX afxError ArxUpdateVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, void const* src, afxUnit32 srcStride);
ARX afxError ArxUploadVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream in, afxUnit stride);
ARX afxError ArxDownloadVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream out, afxUnit stride);

/*
    The ArxExportMeshTopology() function exports and potentially processes a mesh's topology in different formats 
    (like triangle lists or line lists).
*/

ARX afxError ArxExportMeshTopology
(
    // The mesh object from which the topology is being extracted.
    arxMesh msh,
    // The desired topology format for the export (could be a triangle list or line list).
    avxTopology top,
    // The index of the base triangle to start exporting from.
    afxUnit baseTriIdx,
    // The number of triangles to export.
    afxUnit triCnt,
    // A pointer to the destination buffer where the processed topology will be stored.
    void* dst,
    // The size of the indices to be stored in the destination buffer.
    afxUnit dstIdxSiz
);

ARX afxError ArxExportMeshData
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseVtx, 
    afxUnit vtxCnt, 
    void* dst, 
    afxUnit stride
);

ARX afxError ArxExportMeshDataDeindexed
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseIdx, 
    afxUnit idxCnt, 
    void* dst, 
    afxUnit stride
);

////////////////////////////////////////////////////////////////////////////////

#endif//ARX_MESH_IO_H
