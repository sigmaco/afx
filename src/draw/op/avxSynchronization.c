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

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
#define _AVX_DRAW_CONTEXT_C
#include "../ddi/avxImplementation.h"

_AVX afxCmdId AvxCmdPipelineBarrier(afxDrawContext dctx, avxPipelineStage dstStage, avxPipelineAccess dstAcc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PipelineBarrier), sizeof(cmd->PipelineBarrier), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PipelineBarrier.dstStage = dstStage;
    cmd->PipelineBarrier.dstAccess = dstAcc;
    return cmdId;
}

AVX afxCmdId AvxCmdBeginQuery(afxDrawContext dctx, avxQueryPool pool, afxUnit queryIdx, afxBool precise)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->cap, queryIdx, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BeginQuery), sizeof(cmd->BeginQuery), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BeginQuery.pool = pool;
    cmd->BeginQuery.queryIdx = queryIdx;
    cmd->BeginQuery.precise = precise;
    return cmdId;
}

AVX afxCmdId AvxCmdEndQuery(afxDrawContext dctx, avxQueryPool pool, afxUnit queryIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->cap, queryIdx, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EndQuery), sizeof(cmd->EndQuery), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EndQuery.pool = pool;
    cmd->EndQuery.queryIdx = queryIdx;
    return cmdId;
}

AVX afxCmdId AvxCmdCopyQueryResults(afxDrawContext dctx, avxQueryPool pool, afxUnit baseQuery, afxUnit queryCnt, avxBuffer buf, afxSize offset, afxSize stride, avxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->cap, baseQuery, queryCnt);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), offset, stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyQueryResults), sizeof(cmd->CopyQueryResults), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyQueryResults.pool = pool;
    cmd->CopyQueryResults.baseQuery = baseQuery;
    cmd->CopyQueryResults.queryCnt = queryCnt;
    cmd->CopyQueryResults.buf = buf;
    cmd->CopyQueryResults.offset = offset;
    cmd->CopyQueryResults.stride = stride;
    cmd->CopyQueryResults.flags = flags;
    return cmdId;
}

AVX afxCmdId AvxCmdResetQueries(afxDrawContext dctx, avxQueryPool pool, afxUnit baseQuery, afxUnit queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->cap, baseQuery, queryCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ResetQueries), sizeof(cmd->ResetQueries), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResetQueries.pool = pool;
    cmd->ResetQueries.baseQuery = baseQuery;
    cmd->ResetQueries.queryCnt = queryCnt;
    return cmdId;
}

AVX afxCmdId AvxCmdWriteTimestamp(afxDrawContext dctx, avxQueryPool pool, afxUnit queryIdx, avxPipelineStage stage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->cap, queryIdx, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(WriteTimestamp), sizeof(cmd->WriteTimestamp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->WriteTimestamp.pool = pool;
    cmd->WriteTimestamp.queryIdx = queryIdx;
    cmd->WriteTimestamp.stage = stage;
    return cmdId;
}
