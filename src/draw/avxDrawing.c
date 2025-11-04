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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "ddi/avxImplementation.h"

// Draw

_AVX afxCmdId AvxCmdDraw(afxDrawContext dctx, afxUnit vtxCnt, afxUnit instCnt, afxUnit baseVtx, afxUnit baseInst)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(vtxCnt);
    //AFX_ASSERT(instCnt);

#if 0
#if _AFX_DEBUG
    AFX_ASSERT(dctx->vertices.vin);
    avxVertexLayout vil;
    AvxDescribeVertexLayout(dctx->vertices.vin, &vil);
    for (afxUnit i = 0; i < vil.binCnt; i++)
    {
        avxBuffer buf = dctx->vertices.streams[i].buf;
        afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
        afxSize bufBase = dctx->vertices.streams[i].offset;
        afxUnit bufRange = dctx->vertices.streams[i].range;
        afxUnit bufStride = dctx->vertices.streams[i].stride;

        AFX_ASSERT_RANGE(bufCap, baseVtx * bufStride + bufBase, 1);
        AFX_ASSERT_RANGE(bufCap, bufBase, vtxCnt * bufStride);
    }
#endif
#endif

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(Draw), sizeof(cmd->Draw), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Draw.data.vtxCnt = vtxCnt;
    cmd->Draw.data.instCnt = instCnt;
    cmd->Draw.data.baseVtx = baseVtx;
    cmd->Draw.data.baseInst = baseInst;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndirect(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    AFX_ASSERT(drawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndirect), sizeof(cmd->DrawIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirect.buf = buf;
    cmd->DrawIndirect.drawCnt = drawCnt;
    cmd->DrawIndirect.offset = offset;
    cmd->DrawIndirect.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndirect2(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // cntBuf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);

    AFX_ASSERT(cntBuf);
    AFX_ASSERT(maxDrawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndirect2), sizeof(cmd->DrawIndirect2), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirect2.buf = buf;
    cmd->DrawIndirect2.cntBuf = cntBuf;
    cmd->DrawIndirect2.cntBufOff = cntBufOff;
    cmd->DrawIndirect2.maxDrawCnt = maxDrawCnt;
    cmd->DrawIndirect2.offset = offset;
    cmd->DrawIndirect2.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndexed(afxDrawContext dctx, afxUnit idxCnt, afxUnit instCnt, afxUnit baseIdx, afxUnit vtxOffset, afxUnit baseInst)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(idxCnt);
    //AFX_ASSERT(instCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndexed), sizeof(cmd->DrawIndexed), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexed.data.baseIdx = baseIdx;
    cmd->DrawIndexed.data.baseInst = baseInst;
    cmd->DrawIndexed.data.idxCnt = idxCnt;
    cmd->DrawIndexed.data.instCnt = instCnt;
    cmd->DrawIndexed.data.vtxOffset = vtxOffset;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndexedIndirect(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /*
        @buf must have been created with the avxBufferUsage_INDIRECT bit set.

        @offset must be a multiple of 4.

        If the multiDrawIndirect feature is not enabled, 
            @drawCnt must be 0 or 1.

        @drawCnt must be less than or equal to device's maxDrawIndirectCnt.

        If @drawCnt is greater than 1, 
            @stride must be a multiple of 4 and must be greater than or equal to sizeof(avxDrawIndexedIndirect).

        If @drawCnt is equal to 1, 
            (offset + sizeof(avxDrawIndexedIndirect)) must be less than or equal to the size of @buf.

        If @drawCnt is greater than 1, 
            (@stride * (@drawCnt - 1) + offset + sizeof(avxDrawIndexedIndirect)) must be less than or equal to the size of @buf.
    */

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferUsage(buf, avxBufferUsage_INDIRECT));
    afxSize bufCap = AvxGetBufferCapacity(buf, 0);
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    AFX_ASSERT(stride % sizeof(afxUnit32) == 0);
    AFX_ASSERT_RANGE(bufCap, offset, (stride * (drawCnt ? drawCnt - 1 : 0) + sizeof(avxDrawIndexedIndirect)));
    AFX_ASSERT(!stride || (stride >= sizeof(avxDrawIndexedIndirect)));
    AFX_ASSERT_RANGE(drawCnt, 0, dctx->devLimits->maxDrawIndirectCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndexedIndirect), sizeof(cmd->DrawIndexedIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexedIndirect.buf = buf;
    cmd->DrawIndexedIndirect.drawCnt = drawCnt;
    cmd->DrawIndexedIndirect.offset = offset;
    cmd->DrawIndexedIndirect.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndexedIndirect2(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // cntBuf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);

    AFX_ASSERT(maxDrawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndexedIndirect2), sizeof(cmd->DrawIndexedIndirect2), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexedIndirect2.buf = buf;
    cmd->DrawIndexedIndirect2.cntBuf = cntBuf;
    cmd->DrawIndexedIndirect2.cntBufOff = cntBufOff;
    cmd->DrawIndexedIndirect2.maxDrawCnt = maxDrawCnt;
    cmd->DrawIndexedIndirect2.offset = offset;
    cmd->DrawIndexedIndirect2.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDispatch(afxDrawContext dctx, afxUnit w, afxUnit h, afxUnit d)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(Dispatch), sizeof(cmd->Dispatch), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Dispatch.data.w = w;
    cmd->Dispatch.data.h = h;
    cmd->Dispatch.data.d = d;
    return cmdId;
}

_AVX afxCmdId AvxCmdDispatchIndirect(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DispatchIndirect), sizeof(cmd->DispatchIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DispatchIndirect.buf = buf;
    cmd->DispatchIndirect.offset = offset;
    return cmdId;
}

_AVX afxCmdId AvxCmdPushDebugScope(afxDrawContext dctx, afxString const* name, avxColor const color)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PushDebugScope), sizeof(cmd->PushDebugScope), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->PushDebugScope.label, name);

    if (color)
        AvxCopyColor(cmd->PushDebugScope.color, color);
    else
        AvxResetColor(cmd->PushDebugScope.color);

    return cmdId;
}

_AVX afxCmdId AvxCmdPopDebugScope(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PopDebugScope), sizeof(cmd->PopDebugScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PopDebugScope.nothing = NIL;
    return cmdId;
}

_AVX afxCmdId AvxCmdMarkDebugStep(afxDrawContext dctx, afxString const* name, avxColor const color)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(MarkDebugStep), sizeof(cmd->MarkDebugStep), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->MarkDebugStep.label, name);

    if (color)
        AvxCopyColor(cmd->MarkDebugStep.color, color);
    else
        AvxResetColor(cmd->MarkDebugStep.color);

    return cmdId;
}

_AVX afxError AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(at);
    AFX_ASSERT(v);
    AFX_ASSERT(caption);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(StampDebug), sizeof(cmd->StampDebug) + AFX_ALIGN_SIZE(caption->len, 16), &cmdId);
    AFX_ASSERT(cmd);
    AfxM4dCopyAtm(cmd->StampDebug.v, v);
    AfxV2dCopy(cmd->StampDebug.at, at);
    AfxCopy(cmd->StampDebug.data, caption->start, caption->len);

    return err;
}
