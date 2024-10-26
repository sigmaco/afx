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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_QUERY_OPS_H
#define AFX_QUERY_OPS_H

#include "qwadro/inc/draw/pipe/avxQueryPool.h"

AVX afxCmdId            AvxCmdBeginQuery
/// Begin a query. After beginning a query, that query is considered active within the command buffer it was called in until that same query is ended.
(
    avxCmdb             cmdb,
    avxQueryPool        pool, /// the query pool that will manage the results of the query.
    afxUnit              queryIdx, /// the index within the query pool that will contain the results.
    afxBool             precise /// specifies the precision of occlusion queries.
); 

AVX afxCmdId            AvxCmdEndQuery
/// Ends a query. After ending a query, that query is marked as available.
(
    avxCmdb             cmdb,
    avxQueryPool        pool, /// the pool that is managing the results of the query.
    afxUnit              queryIdx /// the index within the query pool where the result is stored.
);

AVX afxCmdId            AvxCmdCopyQueryResults
/// Copy the results of queries in a query pool to a buffer.
(
    avxCmdb             cmdb,
    avxQueryPool        pool, /// the query pool managing the queries containing the desired results.
    afxUnit              baseQuery, /// the initial query index.
    afxUnit              queryCnt, /// the number of queries. @baseQuery and @queryCnt together define a range of queries.
    afxBuffer           buf, /// a afxBuffer object that will receive the results of the copy command.
    afxSize             offset, /// an offset into dstBuffer.
    afxSize             stride, /// the stride in bytes between results for individual queries within dstBuffer.
    afxQueryResultFlags flags
);

AVX afxCmdId            AvxCmdResetQueries
/// Reset queries in a query pool. When executed on a queue, this command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    avxCmdb             cmdb,
    avxQueryPool        pool, /// the pool managing the queries being reset.
    afxUnit              baseQuery, /// the initial query index to reset.
    afxUnit              queryCnt /// the number of queries to reset.
);

AVX afxCmdId            AvxCmdWriteTimestamp
/// Write a device timestamp into a query object. When AvxCmdWriteTimestamp is submitted to a queue, it defines an execution dependency on commands that were submitted before it, and writes a timestamp to a query pool.
(
    avxCmdb             cmdb,
    avxQueryPool        pool,
    afxUnit              queryIdx, /// the query that will contain the timestamp.
    avxPipelineStage    stage /// the stage of the pipeline.
);

#endif//AFX_QUERY_OPS_H
