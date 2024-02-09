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

#ifndef AFX_QUERY_POOL_H
#define AFX_QUERY_POOL_H

#include "qwadro/draw/pipe/afxPipeline.h"

typedef enum afxQueryResultFlag
{
    afxQueryResultFlag_64 = AfxGetBitOffset(0), /// the results will be written as an array of 64-bit unsigned integer values. If this bit is not set, the results will be written as an array of 32-bit unsigned integer values.
    afxQueryResultFlag_WAIT = AfxGetBitOffset(1), /// will wait for each query’s status to become available before retrieving its results.
    afxQueryResultFlag_WITH_AVAIL = AfxGetBitOffset(2), /// the availability status accompanies the results.
    afxQueryResultFlag_PARTIAL = AfxGetBitOffset(3) /// returning partial results is acceptable.
} afxQueryResultFlags;

typedef enum afxQueryType
{
    afxQueryType_OCCLUSION, /// an occlusion query.
    afxQueryType_PIPELINE, /// a pipeline statistics query.
    afxQueryType_TIMESTAMP, /// a timestamp query.
} afxQueryType;

#ifdef _AFX_DRAW_C
#ifdef _AFX_QUERY_POOL_C
#ifndef _AFX_QUERY_POOL_IMPL
AFX_OBJECT(afxQueryPool)
#else
struct _afxBaseQueryPool
#endif
{
    afxQueryType    type;
    afxNat          cap;
};
#endif
#endif

AFX afxDrawContext      AfxGetQueryPoolContext(afxQueryPool qryp);

AFX afxResult           AfxGetQueryResults
/// Copy results of queries in a query pool to a host memory region.
(
    afxQueryPool        pool,
    afxNat              baseQuery, /// the initial query index.
    afxNat              queryCnt, /// the number of queries to read.
    void*               dst, /// a user-allocated buffer where the results will be written.
    afxSize             cap, /// the size in bytes of the buffer pointed to by @dst.
    afxSize             stride, /// the stride in bytes between results for individual queries within @dst.
    afxQueryResultFlags flags /// how and when results are returned.
);

AFX void                AfxResetQueries
/// Reset queries in a query pool. This command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    afxQueryPool        pool,
    afxNat              baseQuery, /// the initial query index to reset.
    afxNat              queryCnt /// the number of queries to reset.
);

////////////////////////////////////////////////////////////////////////////////

AFX afxCmdId            AfxCmdBeginQuery
/// Begin a query. After beginning a query, that query is considered active within the command buffer it was called in until that same query is ended.
(
    afxDrawScript       dscr,
    afxQueryPool        pool, /// the query pool that will manage the results of the query.
    afxNat              queryIdx, /// the index within the query pool that will contain the results.
    afxBool             precise /// specifies the precision of occlusion queries.
); 

AFX afxCmdId            AfxCmdEndQuery
/// Ends a query. After ending a query, that query is marked as available.
(
    afxDrawScript       dscr,
    afxQueryPool        pool, /// the pool that is managing the results of the query.
    afxNat              queryIdx /// the index within the query pool where the result is stored.
);

AFX afxCmdId            AfxCmdCopyQueryResults
/// Copy the results of queries in a query pool to a buffer.
(
    afxDrawScript       dscr,
    afxQueryPool        pool, /// the query pool managing the queries containing the desired results.
    afxNat              baseQuery, /// the initial query index.
    afxNat              queryCnt, /// the number of queries. @baseQuery and @queryCnt together define a range of queries.
    afxBuffer           buf, /// a afxBuffer object that will receive the results of the copy command.
    afxSize             offset, /// an offset into dstBuffer.
    afxSize             stride, /// the stride in bytes between results for individual queries within dstBuffer.
    afxQueryResultFlags flags
);

AFX afxCmdId            AfxCmdResetQueries
/// Reset queries in a query pool. When executed on a queue, this command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    afxDrawScript       dscr,
    afxQueryPool        pool, /// the pool managing the queries being reset.
    afxNat              baseQuery, /// the initial query index to reset.
    afxNat              queryCnt /// the number of queries to reset.
);

AFX afxCmdId            AfxCmdWriteTimestamp
/// Write a device timestamp into a query object. When AfxCmdWriteTimestamp is submitted to a queue, it defines an execution dependency on commands that were submitted before it, and writes a timestamp to a query pool.
(
    afxDrawScript       dscr,
    afxQueryPool        pool,
    afxNat              queryIdx, /// the query that will contain the timestamp.
    afxPipelineStage    stage /// the stage of the pipeline.
);

#endif//AFX_QUERY_POOL_H
