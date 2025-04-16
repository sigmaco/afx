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

// These are the common transfer operations. That is why they forbids inDrawScope and inVideoCoding.

// buffer ops

_AVX afxCmdId AvxCmdCopyBuffer(afxDrawContext dctx, avxBuffer src, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[])
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

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);

    // src must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    // dst must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &dst);

    // src must have been created with avxBufferUsage_UPLOAD usage flag.
    AFX_ASSERT(AvxGetBufferAccess(src, avxBufferFlag_R));
    // dst must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferAccess(dst, avxBufferFlag_W));

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyBuffer), sizeof(cmd->CopyBuffer) + (opCnt * sizeof(cmd->CopyBuffer.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyBuffer.src = src;
    cmd->CopyBuffer.dst = dst;
    cmd->CopyBuffer.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
        _AvxSanitizeBufferCopy(src, dst, &ops[i], &cmd->CopyBuffer.ops[i]);

    return cmdId;
}

_AVX afxCmdId AvxCmdFillBuffer(afxDrawContext dctx, avxBuffer buf, afxUnit offset, afxUnit range, afxUnit value)
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
    // buf must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_W));

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
    // offset must be less than the size of buf.
    // If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AFX_ASSERT_RANGE(bufCap, offset, range);

    // offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    // If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(FillBuffer), sizeof(cmd->FillBuffer), &cmdId);
    AFX_ASSERT(cmd);
    cmd->FillBuffer.buf = buf;
    cmd->FillBuffer.offset = AFX_MIN(offset, bufCap - 1);
    cmd->FillBuffer.range = AFX_MIN(range, bufCap - cmd->FillBuffer.offset);
    cmd->FillBuffer.value = value;
    return cmdId;
}

_AVX afxCmdId AvxCmdClearBuffer(afxDrawContext dctx, avxBuffer buf, afxUnit offset, afxUnit range)
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
    // buf must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_W));

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
    // offset must be less than the size of buf.
    // If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AFX_ASSERT_RANGE(bufCap, offset, range);

    // offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    // If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    afxCmdId cmdId = AvxCmdFillBuffer(dctx, buf, offset, range, 0);
    AFX_ASSERT(cmdId != AFX_INVALID_INDEX);
    return cmdId;
}

_AVX afxCmdId AvxCmdUpdateBuffer(afxDrawContext dctx, avxBuffer buf, afxUnit offset, afxUnit range, void const* src)
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
    // buf must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_W));

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
    // offset must be less than the size of buf.
    // If range is not equal to zero, range must be less than or equal to the size of buf minus offset.
    AFX_ASSERT_RANGE(bufCap, offset, range);

    // offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    // If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    // the src data for the buffer update, and must be at least range bytes in size.
    AFX_ASSERT(src);

    AFX_ASSERT(AVX_BUFFER_UPDATE_CAPACITY >= range);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UpdateBuffer), sizeof(cmd->UpdateBuffer) + range, &cmdId);
    AFX_ASSERT(cmd);
    cmd->UpdateBuffer.buf = buf;
    cmd->UpdateBuffer.offset = AFX_MIN(offset, bufCap - 1);
    cmd->UpdateBuffer.range = AFX_MIN(range, bufCap - cmd->UpdateBuffer.offset);
    AfxCopy(cmd->UpdateBuffer.data, src, cmd->UpdateBuffer.range);

    return cmdId;
}

_AVX afxCmdId AvxCmdPackRaster(afxDrawContext dctx, avxRaster ras, avxBuffer buf, afxUnit opCnt, avxRasterIo const ops[])
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

    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    avxRasterInfo rasi;
    AvxDescribeRaster(ras, &rasi);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PackRaster), sizeof(cmd->PackRaster) + (opCnt * sizeof(cmd->PackRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PackRaster.buf = buf;
    cmd->PackRaster.ras = ras;
    cmd->PackRaster.opCnt = opCnt;

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        avxRange whd = AvxGetRasterExtent(ras, op->rgn.lodIdx);
        AFX_ASSERT_RANGE_WHD(whd, op->rgn.origin, op->rgn.extent);
        _AvxSanitizeRasterIo(ras, bufCap, op, &cmd->UnpackRaster.ops[i]);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdUnpackRaster(afxDrawContext dctx, avxRaster ras, avxBuffer buf, afxUnit opCnt, avxRasterIo const ops[])
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

    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    avxRasterInfo rasi;
    AvxDescribeRaster(ras, &rasi);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UnpackRaster), sizeof(cmd->UnpackRaster) + (opCnt * sizeof(cmd->UnpackRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UnpackRaster.buf = buf;
    cmd->UnpackRaster.ras = ras;
    cmd->UnpackRaster.opCnt = opCnt;

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        avxRange whd = AvxGetRasterExtent(ras, op->rgn.lodIdx);
        AFX_ASSERT_RANGE_WHD(whd, op->rgn.origin, op->rgn.extent);
        _AvxSanitizeRasterIo(ras, bufCap, op, &cmd->UnpackRaster.ops[i]);
        int a = 1;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdCopyRaster(afxDrawContext dctx, avxRaster src, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[])
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

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    // dst must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    avxRasterInfo rasi, rasi2;
    AvxDescribeRaster(src, &rasi);
    AvxDescribeRaster(dst, &rasi2);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyRaster), sizeof(cmd->CopyRaster) + (opCnt * sizeof(cmd->CopyRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyRaster.src = src;
    cmd->CopyRaster.dst = dst;
    cmd->CopyRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.extent, op->src.origin, op->src.extent);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.extent, op->dstOrigin, op->src.extent);
        
        _AvxSanitizeRasterCopy(src, dst, op, &cmd->CopyRaster.ops[i]);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdResolveRaster(afxDrawContext dctx, avxRaster src, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[])
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

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    // dst must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    avxRasterInfo rasi, rasi2;
    AvxDescribeRaster(src, &rasi);
    AvxDescribeRaster(dst, &rasi2);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ResolveRaster), sizeof(cmd->ResolveRaster) + (opCnt * sizeof(cmd->ResolveRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResolveRaster.src = src;
    cmd->ResolveRaster.dst = dst;
    cmd->ResolveRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.extent, op->src.origin, op->src.extent);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.extent, op->dstOrigin, op->src.extent);

        _AvxSanitizeRasterCopy(src, dst, op, &cmd->ResolveRaster.ops[i]);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBlitRaster(afxDrawContext dctx, avxRaster src, avxRaster dst, afxUnit opCnt, avxRasterBlit const ops[], avxTexelFilter flt)
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

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    // dst must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &dst);
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);

    avxRasterInfo rasi, rasi2;
    AvxDescribeRaster(src, &rasi);
    AvxDescribeRaster(dst, &rasi2);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BlitRaster), sizeof(cmd->BlitRaster) + (opCnt * sizeof(cmd->BlitRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BlitRaster.src = src;
    cmd->BlitRaster.dst = dst;
    cmd->BlitRaster.flt = flt;
    cmd->BlitRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterBlit const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.extent, op->src.origin, op->dst.extent);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dst.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.extent, op->dst.origin, op->dst.extent);

        _AvxSanitizeRasterRegion(src, &op->src, &cmd->BlitRaster.ops[i].src);
        _AvxSanitizeRasterRegion(dst, &op->dst, &cmd->BlitRaster.ops[i].dst);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdClearRaster(afxDrawContext dctx, avxRaster ras, avxClearValue const* value, afxUnit opCnt, avxRasterBlock const ops[])
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

    // opCnt must be greater than 0.
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    avxRasterInfo rasi;
    AvxDescribeRaster(ras, &rasi);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ClearRaster), sizeof(cmd->ClearRaster) + (opCnt * sizeof(cmd->ClearRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ClearRaster.ras = ras;
    cmd->ClearRaster.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterBlock const* op = &ops[i];
        
        AFX_ASSERT_RANGE(rasi.lodCnt, op->baseLod, op->lodCnt);
        AFX_ASSERT_RANGE(rasi.extent.d, op->baseLayer, op->layerCnt);

        avxRasterBlock op2 = { 0 };
        op2.baseLayer = AFX_MIN(op->baseLayer, rasi.extent.d - 1);
        op2.layerCnt = AFX_MIN(op->layerCnt, op2.baseLayer);
        op2.baseLod = AFX_MIN(op->baseLod, rasi.lodCnt - 1);
        op2.lodCnt = AFX_MIN(op->lodCnt, rasi.lodCnt - op2.baseLod);

        cmd->ClearRaster.ops[i] = *op;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdSubsampleRaster(afxDrawContext dctx, avxRaster ras, afxUnit baseLod, afxUnit lodCnt)
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

    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    avxRasterInfo rasi;
    AvxDescribeRaster(ras, &rasi);

    AFX_ASSERT_RANGE(rasi.lodCnt, baseLod, lodCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SubsampleRaster), sizeof(cmd->SubsampleRaster), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SubsampleRaster.baseLod = baseLod;
    cmd->SubsampleRaster.lodCnt = lodCnt;
    cmd->SubsampleRaster.ras = ras;
    return cmdId;
}
