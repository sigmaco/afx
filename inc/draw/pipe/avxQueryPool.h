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

#ifndef AVX_QUERY_POOL_H
#define AVX_QUERY_POOL_H

#include "qwadro/inc/draw/pipe/avxPipeline.h"

typedef enum afxQueryResultFlag
{
    afxQueryResultFlag_64 = AFX_BIT(0), /// the results will be written as an array of 64-bit unsigned integer values. If this bit is not set, the results will be written as an array of 32-bit unsigned integer values.
    afxQueryResultFlag_WAIT = AFX_BIT(1), /// will wait for each query’s status to become available before retrieving its results.
    afxQueryResultFlag_WITH_AVAIL = AFX_BIT(2), /// the availability status accompanies the results.
    afxQueryResultFlag_PARTIAL = AFX_BIT(3) /// returning partial results is acceptable.
} afxQueryResultFlags;

typedef enum afxQueryType
{
    afxQueryType_OCCLUSION, /// an occlusion query.
    afxQueryType_PIPELINE, /// a pipeline statistics query.
    afxQueryType_TIMESTAMP, /// a timestamp query.

    afxQueryType_TOTAL
} afxQueryType;

AVX afxDrawContext      AfxGetQueryPoolContext(avxQueryPool qryp);

AVX afxResult           AfxGetQueryResults
/// Copy results of queries in a query qryp to a host memory region.
(
    avxQueryPool        qryp,
    afxUnit              baseQuery, /// the initial query index.
    afxUnit              queryCnt, /// the number of queries to read.
    void*               dst, /// a user-allocated buffer where the results will be written.
    afxSize             cap, /// the size in bytes of the buffer pointed to by @dst.
    afxSize             stride, /// the stride in bytes between results for individual queries within @dst.
    afxQueryResultFlags flags /// how and when results are returned.
);

AVX void                AfxResetQueries
/// Reset queries in a query qryp. This command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    avxQueryPool        qryp,
    afxUnit              baseQuery, /// the initial query index to reset.
    afxUnit              queryCnt /// the number of queries to reset.
);

#endif//AVX_QUERY_POOL_H
