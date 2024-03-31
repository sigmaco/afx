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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_QUERY_OPS_H
#define AFX_QUERY_OPS_H

#include "qwadro/draw/pipe/afxQueryPool.h"

AVX afxCmdId            AfxCmdBeginQuery
/// Begin a query. After beginning a query, that query is considered active within the command buffer it was called in until that same query is ended.
(
    afxDrawStream       diob,
    afxQueryPool        pool, /// the query pool that will manage the results of the query.
    afxNat              queryIdx, /// the index within the query pool that will contain the results.
    afxBool             precise /// specifies the precision of occlusion queries.
); 

AVX afxCmdId            AfxCmdEndQuery
/// Ends a query. After ending a query, that query is marked as available.
(
    afxDrawStream       diob,
    afxQueryPool        pool, /// the pool that is managing the results of the query.
    afxNat              queryIdx /// the index within the query pool where the result is stored.
);

AVX afxCmdId            AfxCmdCopyQueryResults
/// Copy the results of queries in a query pool to a buffer.
(
    afxDrawStream       diob,
    afxQueryPool        pool, /// the query pool managing the queries containing the desired results.
    afxNat              baseQuery, /// the initial query index.
    afxNat              queryCnt, /// the number of queries. @baseQuery and @queryCnt together define a range of queries.
    afxBuffer           buf, /// a afxBuffer object that will receive the results of the copy command.
    afxSize             offset, /// an offset into dstBuffer.
    afxSize             stride, /// the stride in bytes between results for individual queries within dstBuffer.
    afxQueryResultFlags flags
);

AVX afxCmdId            AfxCmdResetQueries
/// Reset queries in a query pool. When executed on a queue, this command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    afxDrawStream       diob,
    afxQueryPool        pool, /// the pool managing the queries being reset.
    afxNat              baseQuery, /// the initial query index to reset.
    afxNat              queryCnt /// the number of queries to reset.
);

AVX afxCmdId            AfxCmdWriteTimestamp
/// Write a device timestamp into a query object. When AfxCmdWriteTimestamp is submitted to a queue, it defines an execution dependency on commands that were submitted before it, and writes a timestamp to a query pool.
(
    afxDrawStream       diob,
    afxQueryPool        pool,
    afxNat              queryIdx, /// the query that will contain the timestamp.
    afxPipelineStage    stage /// the stage of the pipeline.
);

#endif//AFX_QUERY_OPS_H
