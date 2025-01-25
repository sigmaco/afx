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
#define _AVX_DRAW_CONTEXT_C
#include "../impl/avxImplementation.h"


_AVX afxString const afxDrawCmdStrings[] =
{
    AFX_STRING("PushPipeline"),
    AFX_STRING("PopPipeline"),

    AFX_STRING("EnableRasterization"),

    AFX_STRING("SwitchFrontFace"),
    AFX_STRING("SetCullMode"),

    AFX_STRING("EnableDepthBias"),
    AFX_STRING("SetDepthBias"),

    AFX_STRING("SetLineWidth"),
    
    AFX_STRING("EnableDepthTest"),
    AFX_STRING("EnableDepthWrite"),

    AFX_STRING("EnableStencilTest"),
    AFX_STRING("SetStencilCompareMask"),
    AFX_STRING("SetStencilWriteMask"),
    AFX_STRING("SetStencilReference"),

    AFX_STRING("EnableDepthBoundsTest"),
    AFX_STRING("SetDepthBounds"),

    AFX_STRING("SetBlendConstants"),

    AFX_STRING("ResetViewports"),
    AFX_STRING("UpdateViewports"),
    AFX_STRING("ResetScissors"),
    AFX_STRING("UpdateScissors"),
    AFX_STRING("ResetAreas"),
    AFX_STRING("UpdateAreas"),

    AFX_STRING("BindIndexBuffer"),
    AFX_STRING("BindVertexBuffers"),

    AFX_STRING("Draw"),
    AFX_STRING("DrawIndirect"),
    AFX_STRING("DrawIndirectCount"),
    AFX_STRING("DrawIndexed"),
    AFX_STRING("DrawIndexedIndirect"),
    AFX_STRING("DrawIndexedIndirectCount"),
};

_AVX afxCmdId AvxCmdBindPipeline(afxDrawContext dctx, afxUnit segment, avxPipeline pip, avxVertexDecl vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindPipeline), sizeof(cmd->BindPipeline), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindPipeline.pip = pip;
    cmd->BindPipeline.segment = segment;
    cmd->BindPipeline.vin = vin;
    cmd->BindPipeline.dynamics = dynamics;
    return cmdId;
}

_AVX afxCmdId AvxCmdBindBuffers(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, afxBufferMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindBuffers), sizeof(cmd->BindBuffers) + (cnt * sizeof(cmd->BindBuffers.maps[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindBuffers.set = set;
    cmd->BindBuffers.baseIdx = baseIdx;
    cmd->BindBuffers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferMap const* map = &maps[i];
        cmd->BindBuffers.maps[i].buf = map->buf;
        cmd->BindBuffers.maps[i].offset = map->offset;
        cmd->BindBuffers.maps[i].range = map->range;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindRasters(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, afxRaster const rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindRasters), sizeof(cmd->BindRasters) + (cnt * sizeof(cmd->BindRasters.rasters[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindRasters.set = set;
    cmd->BindRasters.baseIdx = baseIdx;
    cmd->BindRasters.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->BindRasters.rasters[i] = rasters ? rasters[i] : NIL;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindSamplers(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, avxSampler const samplers[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindSamplers), sizeof(cmd->BindSamplers) + (cnt * sizeof(cmd->BindSamplers.samplers[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindSamplers.set = set;
    cmd->BindSamplers.baseIdx = baseIdx;
    cmd->BindSamplers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
        cmd->BindSamplers.samplers[i] = samplers[i];

    return cmdId;
}

_AVX afxCmdId AvxCmdExecuteCommands(afxDrawContext dctx, afxUnit cnt, afxDrawContext aux[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(cnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(ExecuteCommands), sizeof(cmd->ExecuteCommands) + (cnt * sizeof(cmd->ExecuteCommands.contexts[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ExecuteCommands.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
        cmd->ExecuteCommands.contexts[i] = aux[i];

    return cmdId;
}

// Draw

_AVX afxCmdId AvxCmdDraw(afxDrawContext dctx, afxUnit vtxCnt, afxUnit instCnt, afxUnit firstVtxIdx, afxUnit firstInstIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AFX_ASSERT(dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(vtxCnt);
    //AFX_ASSERT(instCnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(Draw), sizeof(cmd->Draw), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Draw.data.vtxCnt = vtxCnt;
    cmd->Draw.data.instCnt = instCnt;
    cmd->Draw.data.baseVtx = firstVtxIdx;
    cmd->Draw.data.baseInst = firstInstIdx;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndirect(afxDrawContext dctx, afxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AFX_ASSERT(dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    AFX_ASSERT(drawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DrawIndirect), sizeof(cmd->DrawIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirect.buf = buf;
    cmd->DrawIndirect.drawCnt = drawCnt;
    cmd->DrawIndirect.offset = offset;
    cmd->DrawIndirect.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndirectCount(afxDrawContext dctx, afxBuffer buf, afxUnit32 offset, afxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AFX_ASSERT(dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    /// cntBuf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);

    AFX_ASSERT(cntBuf);
    AFX_ASSERT(maxDrawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DrawIndirectCount), sizeof(cmd->DrawIndirectCount), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirectCount.buf = buf;
    cmd->DrawIndirectCount.cntBuf = cntBuf;
    cmd->DrawIndirectCount.cntBufOff = cntBufOff;
    cmd->DrawIndirectCount.maxDrawCnt = maxDrawCnt;
    cmd->DrawIndirectCount.offset = offset;
    cmd->DrawIndirectCount.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndexed(afxDrawContext dctx, afxUnit idxCnt, afxUnit instCnt, afxUnit firstIdx, afxUnit vtxOff, afxUnit firstInstIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AFX_ASSERT(dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(idxCnt);
    //AFX_ASSERT(instCnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DrawIndexed), sizeof(cmd->DrawIndexed), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexed.data.baseIdx = firstIdx;
    cmd->DrawIndexed.data.baseInst = firstInstIdx;
    cmd->DrawIndexed.data.idxCnt = idxCnt;
    cmd->DrawIndexed.data.instCnt = instCnt;
    cmd->DrawIndexed.data.vtxOff = vtxOff;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndexedIndirect(afxDrawContext dctx, afxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AFX_ASSERT(dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /*
        @buf must have been created with the afxBufferUsage_INDIRECT bit set.

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

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxTestBufferUsage(buf, afxBufferUsage_INDIRECT));
    afxSize bufCap = AfxGetBufferCapacity(buf, 0);
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    AFX_ASSERT(stride % sizeof(afxUnit32) == 0);
    AFX_ASSERT_RANGE(bufCap, offset, (stride * (drawCnt ? drawCnt - 1 : 0) + sizeof(avxDrawIndexedIndirect)));
    AFX_ASSERT(!stride || (stride >= sizeof(avxDrawIndexedIndirect)));
    AFX_ASSERT_RANGE(drawCnt, 0, dctx->devLimits->maxDrawIndirectCnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DrawIndexedIndirect), sizeof(cmd->DrawIndexedIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexedIndirect.buf = buf;
    cmd->DrawIndexedIndirect.drawCnt = drawCnt;
    cmd->DrawIndexedIndirect.offset = offset;
    cmd->DrawIndexedIndirect.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDrawIndexedIndirectCount(afxDrawContext dctx, afxBuffer buf, afxUnit32 offset, afxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called inside of a render pass instance.
    AFX_ASSERT(dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    /// cntBuf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);

    AFX_ASSERT(maxDrawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DrawIndexedIndirectCount), sizeof(cmd->DrawIndexedIndirectCount), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexedIndirectCount.buf = buf;
    cmd->DrawIndexedIndirectCount.cntBuf = cntBuf;
    cmd->DrawIndexedIndirectCount.cntBufOff = cntBufOff;
    cmd->DrawIndexedIndirectCount.maxDrawCnt = maxDrawCnt;
    cmd->DrawIndexedIndirectCount.offset = offset;
    cmd->DrawIndexedIndirectCount.stride = stride;
    return cmdId;
}

_AVX afxCmdId AvxCmdDispatch(afxDrawContext dctx, afxUnit grpCntX, afxUnit grpCntY, afxUnit grpCntZ)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(Dispatch), sizeof(cmd->Dispatch), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Dispatch.data.x = grpCntX;
    cmd->Dispatch.data.y = grpCntY;
    cmd->Dispatch.data.z = grpCntZ;
    return cmdId;
}

_AVX afxCmdId AvxCmdDispatchIndirect(afxDrawContext dctx, afxBuffer buf, afxUnit32 offset)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DispatchIndirect), sizeof(cmd->DispatchIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DispatchIndirect.buf = buf;
    cmd->DispatchIndirect.offset = offset;
    return cmdId;
}

_AVX afxCmdId AvxCmdPushConstants(afxDrawContext dctx, afxUnit offset, afxUnit siz, void const* value)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    AFX_ASSERT(siz % sizeof(afxUnit32) == 0);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(PushConstants), sizeof(cmd->PushConstants) + siz, &cmdId);
    AFX_ASSERT(cmd);
    cmd->PushConstants.offset = offset;
    cmd->PushConstants.siz = siz;
    AfxCopy(cmd->PushConstants.data, value, siz);
    return cmdId;
}

_AVX afxError AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(at);
    AFX_ASSERT(v);
    AFX_ASSERT(caption);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(StampDebug), sizeof(cmd->StampDebug) + AFX_ALIGNED_SIZE(caption->len, 16), &cmdId);
    AFX_ASSERT(cmd);
    AfxM4dCopyAtm(cmd->StampDebug.v, v);
    AfxV2dCopy(cmd->StampDebug.at, at);
    AfxCopy(cmd->StampDebug.data, caption->start, caption->len);

    return err;
}
