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

  //////////////////////////////////////////////////////////////////////////////
 // DRAWING PIPELINE EXECUTION SYNCHRONIZATION OPERATIONS                    //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_SYNCHRONIZATION_H
#define AVX_SYNCHRONIZATION_H

#include "qwadro/inc/draw/op/avxQueryPool.h"

typedef enum avxPipelineAccess
{
    avxPipelineAccess_NONE,
    // All read accesses, always valid in any access mask, and is treated as equivalent 
    // to setting all READ access flags that are valid where it is used.
    avxPipelineAccess_MEMORY_R = AFX_BITMASK(0),
    // All write accesses, always valid in any access mask, and is treated as equivalent 
    // to setting all WRITE access flags that are valid where it is used.
    avxPipelineAccess_MEMORY_W = AFX_BITMASK(1),
    // read access to indirect command data read as part of an indirect build, trace, drawing or 
    // dispatching command. Such access occurs in the DRAW_INDIRECT pipeline stage.
    avxPipelineAccess_INDIRECT_R = AFX_BITMASK(2),
    // read access to an index buffer as part of an indexed drawing command. 
    // Such access occurs in the VERTEX_INPUT pipeline stage.
    avxPipelineAccess_INDEX_R = AFX_BITMASK(3),
    // read access to a vertex buffer as part of a drawing command. 
    // Such access occurs in the VERTEX_INPUT_BIT pipeline stage.
    avxPipelineAccess_VERTEX_R = AFX_BITMASK(4),
    // read access to a uniform buffer in any shader pipeline stage.
    avxPipelineAccess_UNIFORM_R = AFX_BITMASK(5),
    // read access to an input attachment within a render pass during subpass shading or fragment shading.
    avxPipelineAccess_INPUT_ATTACH_R = AFX_BITMASK(6),
    // read access to a uniform texel buffer, sampled image, storage buffer, physical storage buffer, 
    // shader binding table, storage texel buffer, or storage image in any shader pipeline stage.
    avxPipelineAccess_SHADER_R = AFX_BITMASK(7),
    // write access to a storage buffer, physical storage buffer, storage texel buffer, or storage image in any shader pipeline stage.
    avxPipelineAccess_SHADER_W = AFX_BITMASK(8),
    avxPipelineAccess_SHADER = (avxPipelineAccess_SHADER_R | avxPipelineAccess_SHADER_W),

    // read access to a color attachment, such as via blending (other than advanced blend operations), 
    // logic operations or certain render pass load operations in the COLOR_OUT pipeline stage or via 
    // fragment shader tile image reads in the FRAGMENT_SHADER pipeline stage.
    avxPipelineAccess_COLOR_OUT_R = AFX_BITMASK(9),
    // write access to a color, resolve, or depth/stencil resolve attachment during a render pass or via 
    // certain render pass load and store operations. Such access occurs in the COLOR_OUT pipeline stage.
    avxPipelineAccess_COLOR_OUT_W = AFX_BITMASK(10),
    avxPipelineAccess_COLOR_OUT = (avxPipelineAccess_COLOR_OUT_R | avxPipelineAccess_COLOR_OUT_W),

    // read access to a depth/stencil attachment, via depth or stencil operations or certain render pass load 
    // operations in the EARLY_TESTS or LATE_TESTS pipeline stages or via fragment shader tile image reads 
    // in the FRAGMENT_SHADER pipeline stage.
    avxPipelineAccess_DEPTH_OUT_R = AFX_BITMASK(11),
    // write access to a depth/stencil attachment, via depth or stencil operations or certain render pass load and store operations. 
    // Such access occurs in the EARLY_TESTS or LATE_TESTS pipeline stages.
    avxPipelineAccess_DEPTH_OUT_W = AFX_BITMASK(12),
    avxPipelineAccess_DEPTH_OUT = (avxPipelineAccess_DEPTH_OUT_R | avxPipelineAccess_DEPTH_OUT_W),

    // read access to an image or buffer in a copy operation. Such access occurs in ALL_TRANSFER pipeline stage.
    avxPipelineAccess_TRANSFER_R = AFX_BITMASK(13),
    // write access to an image or buffer in a clear or copy operation. Such access occurs in the ALL_TRANSFER_BIT pipeline stage.
    avxPipelineAccess_TRANSFER_W = AFX_BITMASK(14),
    avxPipelineAccess_TRANSFER = (avxPipelineAccess_TRANSFER_R | avxPipelineAccess_TRANSFER_W),

    // read access by a host operation. Accesses of this type are not performed through a resource, but directly on memory. 
    // Such access occurs in the HOST pipeline stage.
    avxPipelineAccess_HOST_R = AFX_BITMASK(15),
    // write access by a host operation. Accesses of this type are not performed through a resource, but directly on memory. 
    // Such access occurs in the HOST pipeline stage.
    avxPipelineAccess_HOST_W = AFX_BITMASK(16),
    avxPipelineAccess_HOST = (avxPipelineAccess_HOST_R | avxPipelineAccess_HOST_W)
} avxPipelineAccess;

AVX afxCmdId            AvxCmdPipelineBarrier
(
    afxDrawContext      dctx,
    avxPipelineStage    dstStage,
    avxPipelineAccess   dstAcc
);

AVX afxCmdId            AvxCmdBeginQuery
/// Begin a query. After beginning a query, that query is considered active within the draw context it was called in until that same query is ended.
(
    afxDrawContext      dctx,
    avxQueryPool        pool, // the query pool that will manage the results of the query.
    afxUnit             queryIdx, // the index within the query pool that will contain the results.
    afxBool             precise // specifies the precision of occlusion queries.
); 

AVX afxCmdId            AvxCmdEndQuery
/// Ends a query. After ending a query, that query is marked as available.
(
    afxDrawContext      dctx,
    avxQueryPool        pool, // the pool that is managing the results of the query.
    afxUnit             queryIdx // the index within the query pool where the result is stored.
);

AVX afxCmdId            AvxCmdCopyQueryResults
/// Copy the results of queries in a query pool to a buffer.
(
    afxDrawContext      dctx,
    avxQueryPool        pool, // the query pool managing the queries containing the desired results.
    afxUnit             baseQuery, // the initial query index.
    afxUnit             queryCnt, // the number of queries. @baseQuery and @queryCnt together define a range of queries.
    avxBuffer           buf, // a avxBuffer object that will receive the results of the copy command.
    afxSize             offset, // an offset into dstBuffer.
    afxSize             stride, // the stride in bytes between results for individual queries within dstBuffer.
    avxQueryResultFlags flags
);

AVX afxCmdId            AvxCmdResetQueries
/// Reset queries in a query pool. When executed on a queue, this command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    afxDrawContext      dctx,
    avxQueryPool        pool, // the pool managing the queries being reset.
    afxUnit             baseQuery, // the initial query index to reset.
    afxUnit             queryCnt // the number of queries to reset.
);

AVX afxCmdId            AvxCmdWriteTimestamp
/// Write a device timestamp into a query object. When AvxCmdWriteTimestamp is submitted to a queue, it defines an execution dependency on commands that were submitted before it, and writes a timestamp to a query pool.
(
    afxDrawContext      dctx,
    avxQueryPool        pool,
    afxUnit             queryIdx, // the query that will contain the timestamp.
    avxPipelineStage    stage // the stage of the pipeline.
);

#endif//AVX_SYNCHRONIZATION_H
