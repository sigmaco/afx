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
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

_AVX afxCmdId AvxCmdChangeFillModeEXT(afxDrawContext dctx, avxFillMode mode)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(mode < avxFillMode_TOTAL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetFillModeEXT), sizeof(cmd->SetFillModeEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetFillModeEXT.mode = mode;
    return cmdId;
}

_AFX afxCmdId AvxCmdBeginTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cntBufCnt,
    avxBuffer const cntBufs[],
    afxSize const cntBufOffsets[]
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BeginTransformFeedbackEXT), sizeof(cmd->BeginTransformFeedbackEXT) + (cntBufCnt * sizeof(cmd->BeginTransformFeedbackEXT.srcs[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BeginTransformFeedbackEXT.firstCounterBuffer = firstCntBuf;
    cmd->BeginTransformFeedbackEXT.counterBufferCount = cntBufCnt;

    for (afxUnit i = 0; i < cntBufCnt; i++)
    {
        cmd->BeginTransformFeedbackEXT.srcs[i].cntBuf = cntBufs ? cntBufs[i] : NIL;
        cmd->BeginTransformFeedbackEXT.srcs[i].cntBufOff = cntBufOffsets ? cntBufOffsets[i] : 0;
    }
    return cmdId;
}

_AFX afxCmdId AvxCmdEndTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cntBufCnt,
    avxBuffer const cntBufs[],
    afxSize const cntBufOffsets[]
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EndTransformFeedbackEXT), sizeof(cmd->EndTransformFeedbackEXT) + (cntBufCnt * sizeof(cmd->EndTransformFeedbackEXT.srcs[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EndTransformFeedbackEXT.firstCntBuf = firstCntBuf;
    cmd->EndTransformFeedbackEXT.cnterBufCnt = cntBufCnt;

    for (afxUnit i = 0; i < cntBufCnt; i++)
    {
        cmd->EndTransformFeedbackEXT.srcs[i].cntBuf = cntBufs ? cntBufs[i] : NIL;
        cmd->EndTransformFeedbackEXT.srcs[i].cntBufOff = cntBufOffsets ? cntBufOffsets[i] : 0;
    }
    return cmdId;
}

_AFX afxCmdId AvxCmdBeginQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxBool precise,
    afxUnit index
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BeginQueryIndexedEXT), sizeof(cmd->BeginQueryIndexedEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BeginQueryIndexedEXT.qryp = qryp;
    cmd->BeginQueryIndexedEXT.query = query;
    cmd->BeginQueryIndexedEXT.precise = precise;
    cmd->BeginQueryIndexedEXT.index = index;
    return cmdId;
}

_AFX afxCmdId AvxCmdEndQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxUnit index
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EndQueryIndexedEXT), sizeof(cmd->EndQueryIndexedEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EndQueryIndexedEXT.qryp = qryp;
    cmd->EndQueryIndexedEXT.query = query;
    cmd->EndQueryIndexedEXT.index = index;
    return cmdId;
}

_AFX afxCmdId AvxCmdBindTransformFeedbackBuffersEXT
(
    afxDrawContext dctx,
    afxUnit first,
    afxUnit cnt,
    avxBuffer const buffers[],
    afxSize const offsets[],
    afxSize const ranges[]
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindTransformFeedbackBuffersEXT), sizeof(cmd->BindTransformFeedbackBuffersEXT) + (cnt * sizeof(cmd->BindTransformFeedbackBuffersEXT.srcs[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindTransformFeedbackBuffersEXT.firstBinding = first;
    cmd->BindTransformFeedbackBuffersEXT.bindingCount = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->BindTransformFeedbackBuffersEXT.srcs[i].buf = buffers ? buffers[i] : NIL;
        cmd->BindTransformFeedbackBuffersEXT.srcs[i].offset = offsets ? offsets[i] : 0;
        cmd->BindTransformFeedbackBuffersEXT.srcs[i].range = ranges ? ranges[i] : 0;
    }
    return cmdId;
}

_AFX afxCmdId AvxCmdDrawIndirectByteCountEXT
(
    afxDrawContext dctx,
    afxUnit instCnt,
    afxUnit firstInst,
    avxBuffer cntBuf,
    afxSize cntBufOffset,
    afxUnit cntOffset,
    afxUnit vtxStride
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndirectByteCountEXT), sizeof(cmd->DrawIndirectByteCountEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirectByteCountEXT.instCnt = instCnt;
    cmd->DrawIndirectByteCountEXT.firstInst = firstInst;
    cmd->DrawIndirectByteCountEXT.cnterBuf = cntBuf;
    cmd->DrawIndirectByteCountEXT.cnterBufOffset = cntBufOffset;
    cmd->DrawIndirectByteCountEXT.cnterOffset = cntOffset;
    cmd->DrawIndirectByteCountEXT.vtxStride = vtxStride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawMeshTasksEXT
(
    afxDrawContext dctx,
    // The number of local workgroups to dispatch in the X dimension.
    afxUnit w,
    // The number of local workgroups to dispatch in the Y dimension.
    afxUnit h,
    // The number of local workgroups to dispatch in the Z dimension.
    afxUnit d
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawMeshTasksEXT), sizeof(cmd->DrawMeshTasksEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawMeshTasksEXT.w = w;
    cmd->DrawMeshTasksEXT.h = h;
    cmd->DrawMeshTasksEXT.d = d;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawMeshTasksIndirectEXT
(
    afxDrawContext dctx,
    // The buffer containing draw parameters.
    avxBuffer buf,
    // The byte offset into buffer where parameters begin.
    afxSize offset,
    // The number of draws to execute, and can be zero.
    afxUnit drawCnt,
    // The byte stride between successive sets of draw parameters.
    afxUnit stride
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawMeshTasksIndirectEXT), sizeof(cmd->DrawMeshTasksIndirectEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawMeshTasksIndirectEXT.buf = buf;
    cmd->DrawMeshTasksIndirectEXT.offset = offset;
    cmd->DrawMeshTasksIndirectEXT.drawCnt = drawCnt;
    cmd->DrawMeshTasksIndirectEXT.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawMeshTasksIndirectCountEXT
(
    afxDrawContext dctx,
    // The buffer containing draw parameters.
    avxBuffer buf,
    // The byte offset into buffer where parameters begin.
    afxSize offset,
    // The buffer containing the draw count.
    avxBuffer cntBuf,
    // The byte offset into countBuffer where the draw count begins.
    afxSize cntBufOffset,
    // The maximum number of draws that will be executed. 
    // The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit maxDrawCnt,
    // The byte stride between successive sets of draw parameters.
    afxUnit stride
)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawMeshTasksIndirectCountEXT), sizeof(cmd->DrawMeshTasksIndirectCountEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawMeshTasksIndirectCountEXT.buf = buf;
    cmd->DrawMeshTasksIndirectCountEXT.offset = offset;
    cmd->DrawMeshTasksIndirectCountEXT.cntBuf = cntBuf;
    cmd->DrawMeshTasksIndirectCountEXT.cntBufOffset = cntBufOffset;
    cmd->DrawMeshTasksIndirectCountEXT.maxDrawCnt = maxDrawCnt;
    cmd->DrawMeshTasksIndirectCountEXT.stride = stride;
    return cmdId;
}
