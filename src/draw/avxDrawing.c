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
#include "avxIcd.h"

_AVXINL void AfxMinLayeredRect(afxLayeredRect* rc, afxLayeredRect const* a, afxLayeredRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(b);
    AFX_ASSERT(a);
    rc->area.x = AFX_MIN(a->area.x, b->area.x);
    rc->area.y = AFX_MIN(a->area.y, b->area.y);
    rc->area.w = AFX_MIN(a->area.w, (b->area.w - rc->area.x));
    rc->area.h = AFX_MIN(a->area.h, (b->area.h - rc->area.y));
    rc->baseLayer = AFX_MIN(a->baseLayer, b->baseLayer);
    rc->layerCnt = AFX_MIN(a->layerCnt, b->layerCnt);
}

_AVXINL void AfxMaxLayeredRect(afxLayeredRect* rc, afxLayeredRect const* a, afxLayeredRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(b);
    AFX_ASSERT(a);
    rc->area.x = AFX_MAX(a->area.x, b->area.x);
    rc->area.y = AFX_MAX(a->area.y, b->area.y);
    rc->area.w = AFX_MAX(a->area.w, (b->area.w - rc->area.x));
    rc->area.h = AFX_MAX(a->area.h, (b->area.h - rc->area.y));
    rc->baseLayer = AFX_MAX(a->baseLayer, b->baseLayer);
    rc->layerCnt = AFX_MAX(a->layerCnt, b->layerCnt);
}

_AVX afxCmdId AvxCmdCommenceDrawScope(afxDrawContext dctx, avxDrawScope const* cfg)
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

    // cfg must be a valid pointer to a valid avxDrawScope structure.
    AFX_ASSERT(cfg);
    AFX_ASSERT(cfg->targets);
    avxDrawScope cfg2 = *cfg;
    avxCanvas canv = cfg->canv;

    if (canv)
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        afxLayeredRect areaMax;
        AvxGetCanvasExtent(canv, NIL, &areaMax);
        AFX_ASSERT_RANGE(areaMax.area.w, cfg->bounds.area.x, cfg->bounds.area.w);
        AFX_ASSERT_RANGE(areaMax.area.h, cfg->bounds.area.y, cfg->bounds.area.h);
        AFX_ASSERT_RANGE(areaMax.layerCnt, cfg->bounds.baseLayer, cfg->bounds.layerCnt);

        cfg2.bounds.area.x = AFX_CLAMP(cfg->bounds.area.x, 0, (afxInt)areaMax.area.w);
        cfg2.bounds.area.y = AFX_CLAMP(cfg->bounds.area.y, 0, (afxInt)areaMax.area.h);
        cfg2.bounds.area.w = cfg->bounds.area.w ? AFX_CLAMP(cfg->bounds.area.w, 1, areaMax.area.w - cfg2.bounds.area.x) : areaMax.area.w;
        cfg2.bounds.area.h = cfg->bounds.area.h ? AFX_CLAMP(cfg->bounds.area.h, 1, areaMax.area.h - cfg2.bounds.area.y) : areaMax.area.h;
        cfg2.bounds.baseLayer = AFX_CLAMP(cfg->bounds.baseLayer, areaMax.baseLayer, areaMax.layerCnt - 1);
        cfg2.bounds.layerCnt = AFX_CLAMP(cfg->bounds.layerCnt, areaMax.baseLayer, areaMax.layerCnt);
    }

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CommenceDrawScope), sizeof(cmd->CommenceDrawScope) + (cfg2.targetCnt * sizeof(cmd->CommenceDrawScope.targets[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CommenceDrawScope.targetCnt = cfg2.targetCnt;
    cmd->CommenceDrawScope.canv = canv;
    cmd->CommenceDrawScope.bounds = cfg2.bounds;

    for (afxUnit i = 0; i < cfg2.targetCnt; i++)
    {
        AFX_ASSERT(cfg->targets);
        AFX_ASSERT_BOUNDS(cfg->targets[i].loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg->targets[i].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.targets[i] = cfg->targets[i];
    }

    //if (!cfg2.depth) cmd->CommenceDrawScope.hasD = FALSE;
    //else
    {
        AFX_ASSERT_BOUNDS(cfg2.ds[0].loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg2.ds[0].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.ds[0] = cfg2.ds[0];
        cmd->CommenceDrawScope.hasD = TRUE;
    }

    //if (!cfg2.stencil) cmd->CommenceDrawScope.hasS = FALSE;
    //else
    {
        AFX_ASSERT_BOUNDS(cfg2.ds[1].loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg2.ds[1].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.ds[1] = cfg2.ds[1];
        cmd->CommenceDrawScope.hasS = TRUE;
    }

    cmd->CommenceDrawScope.flags = cfg2.flags;
    cmd->CommenceDrawScope.dbgTag = cfg2.tag;

    dctx->inDrawScope = TRUE;
    dctx->inDrawScopeCmd = cmd;
    dctx->canv = canv;
    dctx->ccfg = cfg2;

    return cmdId;
}

_AVX afxCmdId AvxCmdConcludeDrawScope(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ConcludeDrawScope), sizeof(cmd->ConcludeDrawScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ConcludeDrawScope.nothing = NIL;

    dctx->inDrawScope = FALSE;
    dctx->inDrawScopeCmd = cmd;
    dctx->canv = NIL;
    dctx->ccfg = (avxDrawScope) { 0 };

    return cmdId;
}

_AVX afxCmdId AvxCmdClearCanvas(afxDrawContext dctx, afxUnit bufCnt, afxUnit const bins[], avxClearValue const values[], afxUnit areaCnt, afxLayeredRect const areas[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ClearCanvas), sizeof(cmd->ClearCanvas) + (areaCnt * sizeof(cmd->ClearCanvas.areas[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ClearCanvas.bufCnt = bufCnt;
    cmd->ClearCanvas.areaCnt = areaCnt;

    for (afxUnit i = 0; i < bufCnt; i++)
    {
        cmd->ClearCanvas.bins[i] = bins ? bins[i] : i;
        cmd->ClearCanvas.values[i] = values[i];
    }

    for (afxUnit i = 0; i < areaCnt; i++)
    {
        AfxMinLayeredRect(&cmd->ClearCanvas.areas[i], &areas[i], &dctx->ccfg.bounds);
    }

    return cmdId;
}

_AVX afxCmdId AvxCmdNextPass(afxDrawContext dctx, afxBool useAuxScripts)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(NextPass), sizeof(cmd->NextPass), &cmdId);
    AFX_ASSERT(cmd);
    cmd->NextPass.useAuxContexts = !!useAuxScripts;
    return cmdId;
}

// Draw

_AVX afxCmdId AvxCmdDraw(afxDrawContext dctx, afxUnit vtxCnt, afxUnit instCnt, afxUnit baseVtx, afxUnit baseInst)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DispatchIndirect), sizeof(cmd->DispatchIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DispatchIndirect.buf = buf;
    cmd->DispatchIndirect.offset = offset;
    return cmdId;
}
