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

_AVX afxCmdId AvxCmdPackRaster(afxDrawContext dctx, afxRaster ras, afxBuffer buf, afxUnit opCnt, afxRasterIo const ops[])
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

    /// ras must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(PackRaster), sizeof(cmd->PackRaster) + (opCnt * sizeof(cmd->PackRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PackRaster.buf = buf;
    cmd->PackRaster.ras = ras;
    cmd->PackRaster.opCnt = opCnt;

    afxUnit bufCap = AfxGetBufferCapacity(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        afxWhd whd = AfxGetRasterExtent(ras, op->rgn.lodIdx);
        AFX_ASSERT_RANGE_WHD(whd, op->rgn.origin, op->rgn.extent);
        _AvxSanitizeRasterIo(ras, bufCap, op, &cmd->UnpackRaster.ops[i]);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdUnpackRaster(afxDrawContext dctx, afxRaster ras, afxBuffer buf, afxUnit opCnt, afxRasterIo const ops[])
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

    /// ras must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(UnpackRaster), sizeof(cmd->UnpackRaster) + (opCnt * sizeof(cmd->UnpackRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UnpackRaster.buf = buf;
    cmd->UnpackRaster.ras = ras;
    cmd->UnpackRaster.opCnt = opCnt;

    afxUnit bufCap = AfxGetBufferCapacity(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        afxWhd whd = AfxGetRasterExtent(ras, op->rgn.lodIdx);
        AFX_ASSERT_RANGE_WHD(whd, op->rgn.origin, op->rgn.extent);
        _AvxSanitizeRasterIo(ras, bufCap, op, &cmd->UnpackRaster.ops[i]);
        int a = 1;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdCopyRaster(afxDrawContext dctx, afxRaster src, afxRaster dst, afxUnit opCnt, afxRasterCopy const ops[])
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

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    /// dst must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    /// src must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    afxRasterInfo rasi, rasi2;
    AfxDescribeRaster(src, &rasi);
    AfxDescribeRaster(dst, &rasi2);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(CopyRaster), sizeof(cmd->CopyRaster) + (opCnt * sizeof(cmd->CopyRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyRaster.src = src;
    cmd->CopyRaster.dst = dst;
    cmd->CopyRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.extent, op->src.origin, op->src.extent);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.extent, op->dstOrigin, op->src.extent);
        
        _AvxSanitizeRasterCopy(src, dst, op, &cmd->CopyRaster.ops[i]);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdResolveRaster(afxDrawContext dctx, afxRaster src, afxRaster dst, afxUnit opCnt, afxRasterCopy const ops[])
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

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    /// dst must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    /// src must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    afxRasterInfo rasi, rasi2;
    AfxDescribeRaster(src, &rasi);
    AfxDescribeRaster(dst, &rasi2);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(ResolveRaster), sizeof(cmd->ResolveRaster) + (opCnt * sizeof(cmd->ResolveRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResolveRaster.src = src;
    cmd->ResolveRaster.dst = dst;
    cmd->ResolveRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.extent, op->src.origin, op->src.extent);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.extent, op->dstOrigin, op->src.extent);

        _AvxSanitizeRasterCopy(src, dst, op, &cmd->ResolveRaster.ops[i]);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBlitRaster(afxDrawContext dctx, afxRaster src, afxRaster dst, afxUnit opCnt, afxRasterBlit const ops[], avxTexelFilter flt)
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

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    /// dst must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    /// src must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    afxRasterInfo rasi, rasi2;
    AfxDescribeRaster(src, &rasi);
    AfxDescribeRaster(dst, &rasi2);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BlitRaster), sizeof(cmd->BlitRaster) + (opCnt * sizeof(cmd->BlitRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BlitRaster.src = src;
    cmd->BlitRaster.dst = dst;
    cmd->BlitRaster.flt = flt;
    cmd->BlitRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterBlit const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.extent, op->src.origin, op->dst.extent);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dst.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.extent, op->dst.origin, op->dst.extent);

        _AvxSanitizeRasterRegion(src, &op->src, &cmd->BlitRaster.ops[i].src);
        _AvxSanitizeRasterRegion(dst, &op->dst, &cmd->BlitRaster.ops[i].dst);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdClearRaster(afxDrawContext dctx, afxRaster ras, avxClearValue const* value, afxUnit opCnt, afxRasterBlock const ops[])
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

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    /// ras must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(ClearRaster), sizeof(cmd->ClearRaster) + (opCnt * sizeof(cmd->ClearRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ClearRaster.ras = ras;
    cmd->ClearRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxRasterBlock const* op = &ops[i];
        
        AFX_ASSERT_RANGE(rasi.lodCnt, op->baseLod, op->lodCnt);
        AFX_ASSERT_RANGE(rasi.extent.d, op->baseLayer, op->layerCnt);

        afxRasterBlock op2 = { 0 };
        op2.baseLayer = AfxMin(op->baseLayer, rasi.extent.z - 1);
        op2.layerCnt = AfxMin(op->layerCnt, op2.baseLayer);
        op2.baseLod = AfxMin(op->baseLod, rasi.lodCnt - 1);
        op2.lodCnt = AfxMin(op->lodCnt, rasi.lodCnt - op2.baseLod);

        cmd->ClearRaster.ops[i] = *op;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdSubsampleRaster(afxDrawContext dctx, afxRaster ras, afxUnit baseLod, afxUnit lodCnt)
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

    /// ras must be a valid afxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxRasterInfo rasi;
    AfxDescribeRaster(ras, &rasi);

    AFX_ASSERT_RANGE(rasi.lodCnt, baseLod, lodCnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SubsampleRaster), sizeof(cmd->SubsampleRaster), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SubsampleRaster.baseLod = baseLod;
    cmd->SubsampleRaster.lodCnt = lodCnt;
    cmd->SubsampleRaster.ras = ras;
    return cmdId;
}
