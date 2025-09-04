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
#include "ddi/avxImplementation.h"

_AVX afxCmdId AvxCmdAdjustCurtainsSIGMA(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // cnt must be greater than 0.
    AFX_ASSERT(cnt);

    // rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(AVX_MAX_VIEWPORTS, baseIdx, cnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(AdjustCurtainsSIGMA), sizeof(cmd->AdjustCurtainsSIGMA) + (cnt * sizeof(cmd->AdjustCurtainsSIGMA.rects[0])), &cmdId);
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

_AVX afxCmdId AvxCmdSetRasterizationSamplesEXT(afxDrawContext dctx, afxUnit sampleLvl)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetRasterizationSamplesEXT), sizeof(cmd->SetRasterizationSamplesEXT), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetSampleMaskEXT), sizeof(cmd->SetSampleMaskEXT), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SwitchAlphaToCoverageEXT), sizeof(cmd->SwitchAlphaToCoverageEXT), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SwitchAlphaToOneEXT), sizeof(cmd->SwitchAlphaToOneEXT), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SwitchColorBlendingEXT), sizeof(cmd->SwitchColorBlendingEXT) + (cnt * sizeof(cmd->SwitchColorBlendingEXT.enabled[0])), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ChangeColorBlendEquationEXT), sizeof(cmd->ChangeColorBlendEquationEXT) + (cnt * sizeof(cmd->ChangeColorBlendEquationEXT.equations[0])), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetColorWriteMaskEXT), sizeof(cmd->SetColorWriteMaskEXT) + (cnt * sizeof(cmd->SetColorWriteMaskEXT.masks[0])), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SwitchLogicOpEXT), sizeof(cmd->SwitchLogicOpEXT), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthClampEnableEXT), sizeof(cmd->SetDepthClampEnableEXT), &cmdId);
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
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);


    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DiscardRectanglesEXT), sizeof(cmd->DiscardRectanglesEXT) + (cnt * sizeof(cmd->DiscardRectanglesEXT.rects[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DiscardRectanglesEXT.first = first;
    cmd->DiscardRectanglesEXT.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
        cmd->DiscardRectanglesEXT.rects[i] = rects[i];

    return cmdId;
}
