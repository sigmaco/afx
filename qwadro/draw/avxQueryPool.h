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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_QUERY_POOL_H
#define AVX_QUERY_POOL_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum avxQueryResultFlag
{
    avxQueryResultFlag_64 = AFX_BITMASK(0), // the results will be written as an array of 64-bit unsigned integer values. If this bit is not set, the results will be written as an array of 32-bit unsigned integer values.
    avxQueryResultFlag_WAIT = AFX_BITMASK(1), // will wait for each query’s status to become available before retrieving its results.
    avxQueryResultFlag_WITH_AVAIL = AFX_BITMASK(2), // the availability status accompanies the results.
    avxQueryResultFlag_PARTIAL = AFX_BITMASK(3) // returning partial results is acceptable.
} avxQueryResultFlags;

typedef enum avxQueryType
{
    // an occlusion query.
    avxQueryType_OCCLUSION,
    // a pipeline statistics query.
    avxQueryType_PIPELINE,
    // a timestamp query.
    avxQueryType_TIMESTAMP,
    // Transform feedback stream
    avxQueryType_XFB_OUT_EXT,
    // Performance query,
    avxQueryType_PERF,
    // Performance query for Intel,
    avxQueryType_PERF_INTEL,

    avxQueryType_TOTAL
} avxQueryType;

typedef enum avxBusStat
{
    avxBusStat_VTX_INS = AFX_BITMASK(0),
    avxBusStat_PRIM_INS = AFX_BITMASK(1),
    avxBusStat_VSH_EXECS = AFX_BITMASK(2),
    avxBusStat_GSH_EXECS = AFX_BITMASK(3),
    avxBusStat_GSH_PRIMITIVES = AFX_BITMASK(4),
    avxBusStat_CLIPPING_EXECS = AFX_BITMASK(5),
    avxBusStat_CLIPPING_PRIMITIVES = AFX_BITMASK(6),
    avxBusStat_FSH_EXECS = AFX_BITMASK(7),
    avxBusStat_TESSCTRL_PATCHES = AFX_BITMASK(8),
    avxBusStat_TESSEVAL_EXECS = AFX_BITMASK(9),
    avxBusStat_CSH_INVOKES = AFX_BITMASK(10),
    avxBusStat_TSH_EXECS = AFX_BITMASK(11),
    avxBusStat_MSH_EXECS = AFX_BITMASK(12),
} avxBusStats;

AFX_DEFINE_STRUCT(avxQueryPoolConfig)
{
    afxFlags        flags;
    avxQueryType    type;
    afxUnit         slotCnt;
    avxBusStats     stats;
};

AVX afxError AfxAcquireQueryPools
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    avxQueryPoolConfig const cfg[], 
    avxQueryPool pools[]
);

////////////////////////////////////////////////////////////////////////////////

AVX afxDrawSystem       AvxGetQueryPoolHost
(
    avxQueryPool        qryp
);

AVX afxResult           AvxGetQueryResults
/// Copy results of queries in a query qryp to a host memory region.
(
    avxQueryPool        qryp,
    avxQueryResultFlags flags, // how and when results are returned.
    afxUnit             baseSlot, // the initial query index.
    afxUnit             slotCnt, // the number of queries to read.
    afxSize             dstCap, // the size in bytes of the buffer pointed to by @dst.
    void*               dst, // a user-allocated buffer where the results will be written.
    afxSize             stride // the stride in bytes between results for individual queries within @dst.
);

AVX void                AvxResetQueries
/// Reset queries in a query qryp. This command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    avxQueryPool        qryp,
    afxUnit             baseSlot, // the initial query index to reset.
    afxUnit             slotCnt // the number of queries to reset.
);

#endif//AVX_QUERY_POOL_H
