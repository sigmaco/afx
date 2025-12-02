/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_QUERY_POOL_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

_AVX afxCmdId AvxCmdDeclareDependency(afxDrawContext dctx, avxBusStage dstStage, avxBusAccess dstAcc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PipelineBarrier), sizeof(cmd->PipelineBarrier), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PipelineBarrier.dstStage = dstStage;
    cmd->PipelineBarrier.dstAccess = dstAcc;
    return cmdId;
}

_AVX afxCmdId AvxCmdDeclareBarrier(afxDrawContext dctx, avxBusStage dstStage, avxBusAccess dstAcc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PipelineBarrier), sizeof(cmd->PipelineBarrier), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PipelineBarrier.dstStage = dstStage;
    cmd->PipelineBarrier.dstAccess = dstAcc;
    return cmdId;
}

AVX afxCmdId AvxCmdBeginQuery(afxDrawContext dctx, avxQueryPool pool, afxUnit slot, afxBool precise)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, slot, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BeginQuery), sizeof(cmd->BeginQuery), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BeginQuery.pool = pool;
    cmd->BeginQuery.slot = slot;
    cmd->BeginQuery.precise = precise;
    return cmdId;
}

AVX afxCmdId AvxCmdEndQuery(afxDrawContext dctx, avxQueryPool pool, afxUnit slot)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, slot, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EndQuery), sizeof(cmd->EndQuery), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EndQuery.pool = pool;
    cmd->EndQuery.slot = slot;
    return cmdId;
}

AVX afxCmdId AvxCmdCopyQueryResults(afxDrawContext dctx, avxQueryPool pool, afxUnit baseSlot, afxUnit slotCnt, avxBuffer buf, afxSize offset, afxSize stride, avxQueryResultFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, baseSlot, slotCnt);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), offset, stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyQueryResults), sizeof(cmd->CopyQueryResults), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyQueryResults.pool = pool;
    cmd->CopyQueryResults.baseSlot = baseSlot;
    cmd->CopyQueryResults.slotCnt = slotCnt;
    cmd->CopyQueryResults.buf = buf;
    cmd->CopyQueryResults.offset = offset;
    cmd->CopyQueryResults.stride = stride;
    cmd->CopyQueryResults.flags = flags;
    return cmdId;
}

AVX afxCmdId AvxCmdResetQueries(afxDrawContext dctx, avxQueryPool pool, afxUnit baseSlot, afxUnit slotCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, baseSlot, slotCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ResetQueries), sizeof(cmd->ResetQueries), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResetQueries.pool = pool;
    cmd->ResetQueries.baseSlot = baseSlot;
    cmd->ResetQueries.slotCnt = slotCnt;
    return cmdId;
}

AVX afxCmdId AvxCmdQueryTimestamp(afxDrawContext dctx, avxQueryPool pool, afxUnit slot, avxBusStage stage)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, slot, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(QueryTimestamp), sizeof(cmd->QueryTimestamp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->QueryTimestamp.pool = pool;
    cmd->QueryTimestamp.slot = slot;
    cmd->QueryTimestamp.stage = stage;
    return cmdId;
}
