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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_QUERY_POOL_H
#define AVX_QUERY_POOL_H

#include "qwadro/inc/draw/avxPipeline.h"

typedef enum avxQueryResultFlag
{
    avxQueryResultFlag_64 = AFX_BITMASK(0), // the results will be written as an array of 64-bit unsigned integer values. If this bit is not set, the results will be written as an array of 32-bit unsigned integer values.
    avxQueryResultFlag_WAIT = AFX_BITMASK(1), // will wait for each query’s status to become available before retrieving its results.
    avxQueryResultFlag_WITH_AVAIL = AFX_BITMASK(2), // the availability status accompanies the results.
    avxQueryResultFlag_PARTIAL = AFX_BITMASK(3) // returning partial results is acceptable.
} avxQueryResultFlags;

typedef enum avxQueryType
{
    avxQueryType_OCCLUSION, // an occlusion query.
    avxQueryType_PIPELINE, // a pipeline statistics query.
    avxQueryType_TIMESTAMP, // a timestamp query.

    avxQueryType_TOTAL
} avxQueryType;

AVX afxDrawSystem      AvxGetQueryPoolContext(avxQueryPool qryp);

AVX afxResult           AvxGetQueryResults
/// Copy results of queries in a query qryp to a host memory region.
(
    avxQueryPool        qryp,
    afxUnit             baseQuery, // the initial query index.
    afxUnit             queryCnt, // the number of queries to read.
    void*               dst, // a user-allocated buffer where the results will be written.
    afxSize             cap, // the size in bytes of the buffer pointed to by @dst.
    afxSize             stride, // the stride in bytes between results for individual queries within @dst.
    avxQueryResultFlags flags // how and when results are returned.
);

AVX void                AvxResetQueries
/// Reset queries in a query qryp. This command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    avxQueryPool        qryp,
    afxUnit             baseQuery, // the initial query index to reset.
    afxUnit             queryCnt // the number of queries to reset.
);

AVX afxError AfxAcquireQueryPools(afxDrawSystem dsys, avxQueryType type, afxUnit cap, afxUnit cnt, avxQueryPool pools[]);

#endif//AVX_QUERY_POOL_H
