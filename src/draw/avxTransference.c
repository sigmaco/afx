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

// These are the common transfer operations. That is why they forbids inDrawScope and inVideoCoding.

// buffer ops

_AVX afxCmdId AvxCmdCopyBuffer(afxDrawContext dctx, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[], avxBuffer src)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    AFX_ASSERT(AvxGetBufferFlags(src, avxBufferFlag_R));
    // dst must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferFlags(dst, avxBufferFlag_W));

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyBuffer), sizeof(cmd->CopyBuffer) + (opCnt * sizeof(cmd->CopyBuffer.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyBuffer.src = src;
    cmd->CopyBuffer.dst = dst;
    cmd->CopyBuffer.opCnt = opCnt;

    _AvxSanitizeBufferCopy(dst, src, opCnt, ops, cmd->CopyBuffer.ops);

    return cmdId;
}

_AVX afxCmdId AvxCmdFillBuffer(afxDrawContext dctx, avxBuffer buf, afxSize offset, afxUnit range, afxUnit value)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // buf must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferFlags(buf, avxBufferFlag_W));

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

_AVX afxCmdId AvxCmdClearBuffer(afxDrawContext dctx, avxBuffer buf, afxSize offset, afxUnit range)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // buf must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferFlags(buf, avxBufferFlag_W));

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

_AVX afxCmdId AvxCmdUpdateBuffer(afxDrawContext dctx, avxBuffer buf, afxSize offset, afxUnit range, void const* src)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // buf must have been created with avxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AvxGetBufferFlags(buf, avxBufferFlag_W));

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

_AVX afxCmdId AvxCmdPackRaster(afxDrawContext dctx, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    AvxDescribeRaster(ras, &rasi, NIL, NIL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PackRaster), sizeof(cmd->PackRaster) + (opCnt * sizeof(cmd->PackRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PackRaster.buf = buf;
    cmd->PackRaster.ras = ras;
    cmd->PackRaster.opCnt = opCnt;

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
#if !0 // debug
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        avxRange whd = AvxGetRasterExtent(ras, op->rgn.lodIdx);
        AFX_ASSERT_RANGE_WHD(whd, op->rgn.origin, op->rgn.whd);
    }
#endif
    _AvxSanitizeRasterIo(ras, bufCap, opCnt, ops, cmd->UnpackRaster.ops);
    return cmdId;
}

_AVX afxCmdId AvxCmdUnpackRaster(afxDrawContext dctx, avxRaster ras, afxUnit opCnt, avxRasterIo const ops[], avxBuffer buf)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    AvxDescribeRaster(ras, &rasi, NIL, NIL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UnpackRaster), sizeof(cmd->UnpackRaster) + (opCnt * sizeof(cmd->UnpackRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UnpackRaster.buf = buf;
    cmd->UnpackRaster.ras = ras;
    cmd->UnpackRaster.opCnt = opCnt;

    afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
#if !0 // debug
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterIo const* op = &ops[i];
        AFX_ASSERT_RANGE(rasi.lodCnt, op->rgn.lodIdx, 1);
        avxRange whd = AvxGetRasterExtent(ras, op->rgn.lodIdx);
        AFX_ASSERT_RANGE_WHD(whd, op->rgn.origin, op->rgn.whd);
    }
#endif
    _AvxSanitizeRasterIo(ras, bufCap, opCnt, ops, cmd->UnpackRaster.ops);
    return cmdId;
}

_AVX afxCmdId AvxCmdCopyRaster(afxDrawContext dctx, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    AvxDescribeRaster(src, &rasi, NIL, NIL);
    AvxDescribeRaster(dst, &rasi2, NIL, NIL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyRaster), sizeof(cmd->CopyRaster) + (opCnt * sizeof(cmd->CopyRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyRaster.src = src;
    cmd->CopyRaster.dst = dst;
    cmd->CopyRaster.opCnt = opCnt;
#if !0 // debug
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.whd, op->src.origin, op->src.whd);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.whd, op->dstOrigin, op->src.whd);
        
    }
#endif
    _AvxSanitizeRasterCopy(dst, src, opCnt, ops, cmd->CopyRaster.ops);
    return cmdId;
}

_AVX afxCmdId AvxCmdResolveRaster(afxDrawContext dctx, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[], avxRaster src)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    AvxDescribeRaster(src, &rasi, NIL, NIL);
    AvxDescribeRaster(dst, &rasi2, NIL, NIL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ResolveRaster), sizeof(cmd->ResolveRaster) + (opCnt * sizeof(cmd->ResolveRaster.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResolveRaster.src = src;
    cmd->ResolveRaster.dst = dst;
    cmd->ResolveRaster.opCnt = opCnt;
#if !0 // debug
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];

        AFX_ASSERT_RANGE(rasi.lodCnt, op->src.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi.whd, op->src.origin, op->src.whd);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dstLodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.whd, op->dstOrigin, op->src.whd);
    }
#endif
    _AvxSanitizeRasterCopy(dst, src, opCnt, ops, cmd->ResolveRaster.ops);
    return cmdId;
}

_AVX afxCmdId AvxCmdBlitRaster(afxDrawContext dctx, avxRaster dst, afxUnit opCnt, avxRasterBlit const ops[], avxRaster src, avxTexelFilter flt)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    AvxDescribeRaster(src, &rasi, NIL, NIL);
    AvxDescribeRaster(dst, &rasi2, NIL, NIL);

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
        AFX_ASSERT_RANGE_WHD(rasi.whd, op->src.origin, op->dst.whd);

        AFX_ASSERT_RANGE(rasi2.lodCnt, op->dst.lodIdx, 1);
        AFX_ASSERT_RANGE_WHD(rasi2.whd, op->dst.origin, op->dst.whd);

        _AvxSanitizeRasterRegion(src, 1, &op->src, &cmd->BlitRaster.ops[i].src);
        _AvxSanitizeRasterRegion(dst, 1, &op->dst, &cmd->BlitRaster.ops[i].dst);
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdClearRaster(afxDrawContext dctx, avxRaster ras, avxClearValue const* value, afxUnit baseLod, afxUnit lodCnt, afxUnit baseLayer, afxUnit layerCnt)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // opCnt must be greater than 0.
    //AFX_ASSERT(opCnt);
    //AFX_ASSERT(ops);

    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    avxRasterInfo rasi;
    AvxDescribeRaster(ras, &rasi, NIL, NIL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ClearRaster), sizeof(cmd->ClearRaster), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ClearRaster.ras = ras;
    cmd->ClearRaster.lodCnt = lodCnt;
    cmd->ClearRaster.baseLod = baseLod;
    cmd->ClearRaster.layerCnt = layerCnt;
    cmd->ClearRaster.baseLayer = baseLayer;
    cmd->ClearRaster.value = *value;

    return cmdId;
}

_AVX afxCmdId AvxCmdRegenerateMipmapsSIGMA(afxDrawContext dctx, afxFlags flags, afxUnit rasCnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, rasCnt, rasters);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(RegenerateMipmapsSIGMA), sizeof(cmd->RegenerateMipmapsSIGMA) + (rasCnt * sizeof(cmd->RegenerateMipmapsSIGMA.rasters[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->RegenerateMipmapsSIGMA.flags = flags;
    cmd->RegenerateMipmapsSIGMA.rasCnt = rasCnt;
    AfxCopy(cmd->RegenerateMipmapsSIGMA.rasters, rasters, sizeof(cmd->RegenerateMipmapsSIGMA.rasters[0]) * rasCnt);

    return cmdId;
}
