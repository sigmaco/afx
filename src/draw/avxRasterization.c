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

_AVX afxCmdId AvxCmdSwitchRasterization(afxDrawContext dctx, afxBool disabled)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(disabled, FALSE, TRUE);
    disabled = AFX_CLAMP(disabled, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DisableRasterization), sizeof(cmd->DisableRasterization), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DisableRasterization.disable = disabled;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetLineWidth(afxDrawContext dctx, afxReal lineWidth)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetLineWidth), sizeof(cmd->SetLineWidth), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetLineWidth.lineWidth = lineWidth;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchDepthBias(afxDrawContext dctx, afxBool enabled)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enabled, FALSE, TRUE);
    enabled = AFX_CLAMP(enabled, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableDepthBias), sizeof(cmd->EnableDepthBias), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthBias.enable = enabled;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthBias(afxDrawContext dctx, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthBias), sizeof(cmd->SetDepthBias), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthBias.constFactor = constFactor;
    cmd->SetDepthBias.clamp = clamp;
    cmd->SetDepthBias.slopeFactor = slopeFactor;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchDepthTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AFX_CLAMP(enable, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableDepthTest), sizeof(cmd->EnableDepthTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthTest.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthCompareOp(afxDrawContext dctx, avxCompareOp op)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_BOUNDS(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    op = AFX_CLAMP(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthCompareOp), sizeof(cmd->SetDepthCompareOp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthCompareOp.compareOp = op;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchDepthWrites(afxDrawContext dctx, afxBool disable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    AFX_ASSERT_RANGE(disable, FALSE, TRUE);
    disable = AFX_CLAMP(disable, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DisableDepthWrite), sizeof(cmd->DisableDepthWrite), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DisableDepthWrite.disable = disable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchStencilTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableStencilTest), sizeof(cmd->EnableStencilTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableStencilTest.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilCompareMask(afxDrawContext dctx, avxFaceMask faceMask, afxMask compareMask)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilCompareMask), sizeof(cmd->SetStencilCompareMask), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilCompareMask.faceMask = faceMask;
    cmd->SetStencilCompareMask.compareMask = compareMask;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilWriteMask(afxDrawContext dctx, avxFaceMask faceMask, afxMask writeMask)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilWriteMask), sizeof(cmd->SetStencilWriteMask), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilWriteMask.faceMask = faceMask;
    cmd->SetStencilWriteMask.writeMask = writeMask;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilReference(afxDrawContext dctx, avxFaceMask faceMask, afxUnit32 reference)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilReference), sizeof(cmd->SetStencilReference), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilReference.faceMask = faceMask;
    cmd->SetStencilReference.reference = reference;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetStencilOp(afxDrawContext dctx, avxFaceMask faceMask, avxStencilOp failOp, avxStencilOp passOp, avxStencilOp depthFailOp, avxCompareOp compareOp)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilOp), sizeof(cmd->SetStencilOp), &cmdId);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AFX_CLAMP(enable, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableDepthBoundsTest), sizeof(cmd->EnableDepthBoundsTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthBoundsTest.enable = enable;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetDepthBounds(afxDrawContext dctx, afxV2d const bounds)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthBounds), sizeof(cmd->SetDepthBounds), &cmdId);
    AFX_ASSERT(cmd);
    AfxV2dCopy(cmd->SetDepthBounds.bounds, bounds ? bounds : AFX_V2D(0, 1));
    return cmdId;
}

_AVX afxCmdId AvxCmdSetBlendConstants(afxDrawContext dctx, afxV4d const blendConstants)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetBlendConstants), sizeof(cmd->SetBlendConstants), &cmdId);
    AFX_ASSERT(cmd);
    AfxV4dCopy(cmd->SetBlendConstants.blendContants, blendConstants ? blendConstants : AFX_V4D(1, 1, 1, 1));
    return cmdId;
}

_AVX afxCmdId AvxCmdAdjustScissors(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // cnt must be greater than 0.
    AFX_ASSERT(cnt);

    // rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(AVX_MAX_VIEWPORTS, baseIdx, cnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(AdjustScissors), sizeof(cmd->AdjustScissors) + (cnt * sizeof(cmd->AdjustScissors.rects[0])), &cmdId);
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
