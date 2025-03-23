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

_AVX afxCmdId AvxCmdSwitchDepthBias(afxDrawContext dctx, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(EnableDepthBias), sizeof(cmd->EnableDepthBias), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthBias.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthBias(afxDrawContext dctx, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetDepthBias), sizeof(cmd->SetDepthBias), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthBias.constFactor = constFactor;
    cmd->SetDepthBias.clamp = clamp;
    cmd->SetDepthBias.slopeFactor = slopeFactor;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetLineWidth(afxDrawContext dctx, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetLineWidth), sizeof(cmd->SetLineWidth), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetLineWidth.lineWidth = lineWidth;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchRasterization(afxDrawContext dctx, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(disable, FALSE, TRUE);
    disable = AfxClamp(disable, FALSE, TRUE);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DisableRasterization), sizeof(cmd->DisableRasterization), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DisableRasterization.disable = disable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchDepthTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(EnableDepthTest), sizeof(cmd->EnableDepthTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthTest.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthCompareOp(afxDrawContext dctx, avxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_BOUNDS(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    op = AfxClamp(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetDepthCompareOp), sizeof(cmd->SetDepthCompareOp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthCompareOp.compareOp = op;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchDepthWrites(afxDrawContext dctx, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    AFX_ASSERT_RANGE(disable, FALSE, TRUE);
    disable = AfxClamp(disable, FALSE, TRUE);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DisableDepthWrite), sizeof(cmd->DisableDepthWrite), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DisableDepthWrite.disable = disable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchStencilTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(EnableStencilTest), sizeof(cmd->EnableStencilTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableStencilTest.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilCompareMask(afxDrawContext dctx, avxFaceMask faceMask, afxMask compareMask)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetStencilCompareMask), sizeof(cmd->SetStencilCompareMask), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilCompareMask.faceMask = faceMask;
    cmd->SetStencilCompareMask.compareMask = compareMask;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilWriteMask(afxDrawContext dctx, avxFaceMask faceMask, afxMask writeMask)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetStencilWriteMask), sizeof(cmd->SetStencilWriteMask), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilWriteMask.faceMask = faceMask;
    cmd->SetStencilWriteMask.writeMask = writeMask;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilReference(afxDrawContext dctx, avxFaceMask faceMask, afxUnit32 reference)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetStencilReference), sizeof(cmd->SetStencilReference), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilReference.faceMask = faceMask;
    cmd->SetStencilReference.reference = reference;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilOp(afxDrawContext dctx, avxFaceMask faceMask, avxStencilOp failOp, avxStencilOp passOp, avxStencilOp depthFailOp, avxCompareOp compareOp)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetStencilOp), sizeof(cmd->SetStencilOp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilOp.faceMask = faceMask;
    cmd->SetStencilOp.failOp = failOp;
    cmd->SetStencilOp.passOp = passOp;
    cmd->SetStencilOp.depthFailOp = depthFailOp;
    cmd->SetStencilOp.compareOp = compareOp;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchDepthBoundsTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AfxClamp(enable, FALSE, TRUE);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(EnableDepthBoundsTest), sizeof(cmd->EnableDepthBoundsTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthBoundsTest.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthBounds(afxDrawContext dctx, afxV2d const bounds)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetDepthBounds), sizeof(cmd->SetDepthBounds), &cmdId);
    AFX_ASSERT(cmd);
    AfxV2dCopy(cmd->SetDepthBounds.bounds, bounds ? bounds : AFX_V2D(0, 1));
    return cmdId;
}

_AVX afxCmdId AvxCmdSetBlendConstants(afxDrawContext dctx, afxV4d const blendConstants)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetBlendConstants), sizeof(cmd->SetBlendConstants), &cmdId);
    AFX_ASSERT(cmd);
    AfxV4dCopy(cmd->SetBlendConstants.blendContants, blendConstants ? blendConstants : AFX_V4D(1, 1, 1, 1));
    return cmdId;
}

_AVX afxCmdId AvxCmdAdjustScissors(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // cnt must be greater than 0.
    AFX_ASSERT(cnt);

    // rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(8, baseIdx, cnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(AdjustScissors), sizeof(cmd->AdjustScissors) + (cnt * sizeof(cmd->AdjustScissors.rects[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->AdjustScissors.baseIdx = baseIdx;
    cmd->AdjustScissors.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRect const* rc = &rects[i];

        // The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AFX_ASSERT(rc->x >= 0);
        AFX_ASSERT(rc->y >= 0);

        // Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AFX_ASSERT(rc->x + (afxInt)rc->w > 0);
        AFX_ASSERT(rc->y + (afxInt)rc->h > 0);

        cmd->AdjustScissors.rects[i] = *rc;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdAdjustCurtainsSIGMA(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // cnt must be greater than 0.
    AFX_ASSERT(cnt);

    // rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(8, baseIdx, cnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(AdjustCurtainsSIGMA), sizeof(cmd->AdjustCurtainsSIGMA) + (cnt * sizeof(cmd->AdjustCurtainsSIGMA.rects[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->AdjustCurtainsSIGMA.baseIdx = baseIdx;
    cmd->AdjustCurtainsSIGMA.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRect const* rc = &rects[i];

        // The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AFX_ASSERT(rc->x >= 0);
        AFX_ASSERT(rc->y >= 0);

        // Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AFX_ASSERT(rc->x + (afxInt)rc->w > 0);
        AFX_ASSERT(rc->y + (afxInt)rc->h > 0);

        cmd->AdjustCurtainsSIGMA.rects[i] = *rc;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdCommenceDrawScope(afxDrawContext dctx, avxDrawScope const* cfg)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inRenderPass);
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
        avxRange whd = AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO);
        AFX_ASSERT_RANGE(whd.w, cfg->area.x, cfg->area.w);
        AFX_ASSERT_RANGE(whd.h, cfg->area.y, cfg->area.h);
        
        cfg2.area.x = AfxClamp(cfg->area.x, 0, (afxInt)whd.w);
        cfg2.area.y = AfxClamp(cfg->area.y, 0, (afxInt)whd.h);
        cfg2.area.w = cfg->area.w ? AfxClamp(cfg->area.w, 1, whd.w - cfg2.area.x) : whd.w;
        cfg2.area.h = cfg->area.h ? AfxClamp(cfg->area.h, 1, whd.h - cfg2.area.y) : whd.h;
    }

    dctx->inRenderPass = TRUE;

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(CommenceDrawScope), sizeof(cmd->CommenceDrawScope) + (cfg2.targetCnt * sizeof(cmd->CommenceDrawScope.targets[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CommenceDrawScope.baseLayer = cfg2.baseLayer;
    cmd->CommenceDrawScope.layerCnt = cfg2.layerCnt;
    cmd->CommenceDrawScope.targetCnt = cfg2.targetCnt;
    cmd->CommenceDrawScope.canv = canv;
    cmd->CommenceDrawScope.area = cfg2.area;

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
        AFX_ASSERT_BOUNDS(cfg2.depth.loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg2.depth.storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.depth = cfg2.depth;
        cmd->CommenceDrawScope.hasD = TRUE;
    }

    //if (!cfg2.stencil) cmd->CommenceDrawScope.hasS = FALSE;
    //else
    {
        AFX_ASSERT_BOUNDS(cfg2.stencil.loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg2.stencil.storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.stencil = cfg2.stencil;
        cmd->CommenceDrawScope.hasS = TRUE;
    }

    cmd->CommenceDrawScope.dbgTag = cfg2.tag;

    return cmdId;
}

_AVX afxCmdId AvxCmdConcludeDrawScope(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(ConcludeDrawScope), sizeof(cmd->ConcludeDrawScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ConcludeDrawScope.nothing = NIL;

    dctx->inRenderPass = FALSE;
    return cmdId;
}

_AVX afxCmdId AvxCmdNextPass(afxDrawContext dctx, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(NextPass), sizeof(cmd->NextPass), &cmdId);
    AFX_ASSERT(cmd);
    cmd->NextPass.useAuxContexts = !!useAuxScripts;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetRasterizationSamplesEXT(afxDrawContext dctx, afxUnit sampleLvl)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetRasterizationSamplesEXT), sizeof(cmd->SetRasterizationSamplesEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetRasterizationSamplesEXT.sampleLvl = sampleLvl;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetSampleMaskEXT(afxDrawContext dctx, afxUnit sampleLvl, afxMask const sampleMasks[AVX_MAX_SAMPLE_MASKS])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetSampleMaskEXT), sizeof(cmd->SetSampleMaskEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetSampleMaskEXT.sampleLvl = sampleLvl;
    AfxCopy(cmd->SetSampleMaskEXT.sampleMasks, sampleMasks, sampleLvl * sizeof(sampleMasks[0]));
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchAlphaToCoverageEXT(afxDrawContext dctx, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SwitchAlphaToCoverageEXT), sizeof(cmd->SwitchAlphaToCoverageEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchAlphaToCoverageEXT.enable = !!enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchAlphaToOneEXT(afxDrawContext dctx, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SwitchAlphaToOneEXT), sizeof(cmd->SwitchAlphaToOneEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchAlphaToOneEXT.enable = !!enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchColorBlendingEXT(afxDrawContext dctx, afxUnit first, afxUnit cnt, afxBool const enabled[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SwitchColorBlendingEXT), sizeof(cmd->SwitchColorBlendingEXT) + (cnt * sizeof(cmd->SwitchColorBlendingEXT.enabled[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchColorBlendingEXT.first = first;
    cmd->SwitchColorBlendingEXT.cnt = cnt;
    AfxCopy(cmd->SwitchColorBlendingEXT.enabled, enabled, (cnt * sizeof(cmd->SwitchColorBlendingEXT.enabled[0])));
    return cmdId;
}

_AVX afxCmdId AvxCmdChangeColorBlendEquationEXT(afxDrawContext dctx, afxUnit first, afxUnit cnt, avxColorBlend const equations[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(ChangeColorBlendEquationEXT), sizeof(cmd->ChangeColorBlendEquationEXT) + (cnt * sizeof(cmd->ChangeColorBlendEquationEXT.equations[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ChangeColorBlendEquationEXT.first = first;
    cmd->ChangeColorBlendEquationEXT.cnt = cnt;
    AfxCopy(cmd->ChangeColorBlendEquationEXT.equations, equations, (cnt * sizeof(cmd->ChangeColorBlendEquationEXT.equations[0])));

    return cmdId;
}

_AVX afxCmdId AvxCmdSetColorWriteMaskEXT(afxDrawContext dctx, afxUnit first, afxUnit cnt, avxColorMask const writeMasks[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetColorWriteMaskEXT), sizeof(cmd->SetColorWriteMaskEXT) + (cnt * sizeof(cmd->SetColorWriteMaskEXT.masks[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetColorWriteMaskEXT.first = first;
    cmd->SetColorWriteMaskEXT.cnt = cnt;
    AfxCopy(cmd->SetColorWriteMaskEXT.masks, writeMasks, (cnt * sizeof(cmd->SetColorWriteMaskEXT.masks[0])));
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchLogicOpEXT(afxDrawContext dctx, afxBool enabled)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SwitchLogicOpEXT), sizeof(cmd->SwitchLogicOpEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchLogicOpEXT.enabled = !!enabled;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthClampEnableEXT(afxDrawContext dctx, afxBool enabled)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetDepthClampEnableEXT), sizeof(cmd->SetDepthClampEnableEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthClampEnableEXT.enabled = !!enabled;
    return cmdId;
}

_AVX afxCmdId AvxCmdDiscardRectanglesEXT(afxDrawContext dctx, afxUnit first, afxUnit cnt, afxRect const* rects)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inRenderPass);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);


    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(DiscardRectanglesEXT), sizeof(cmd->DiscardRectanglesEXT) + (cnt * sizeof(cmd->DiscardRectanglesEXT.rects[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DiscardRectanglesEXT.first = first;
    cmd->DiscardRectanglesEXT.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
        cmd->DiscardRectanglesEXT.rects[i] = rects[i];

    return cmdId;
}
