/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "sgl.h"
#include "afx/afxQwadro.h"
#include "afx/draw/afxDrawCommands.h"

_SGL void _SglEncodeCmdCommand(afxDrawScript dscr, afxNat id, afxNat siz, _afxDscrCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    cmd->id = id;
    cmd->siz = siz;
    AfxPushLinkage(&cmd->script, &dscr->commands);
}

///////////////////////////////////////////

_SGL void _SglEncodeCmdBindPipeline(afxDrawScript dscr, afxNat level, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdPipeline *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->level = level;
    cmd->pip = pip;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_BIND_PIPELINE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBindBuffers *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf[i];
        cmd->offset[i] = offset[i];
        cmd->range[i] = range[i];
    }
    _SglEncodeCmdCommand(dscr, AFX_DCMD_BIND_BUFFERS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBindTextures *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->smp[i] = smp[i];
        cmd->tex[i] = tex[i];
    }

    _SglEncodeCmdCommand(dscr, AFX_DCMD_BIND_TEXTURES, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdBindVertexSources(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdVertexSources *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        if ((cmd->buf[i] = buf ? buf[i] : NIL))
        {
            AfxAssertObjects(1, &cmd->buf[i], AFX_FCC_BUF);
        }

        cmd->offset[i] = offset ? offset[i] : 0;
        cmd->range[i] = range ? range[i] : (cmd->buf[i] ? AfxGetBufferSize(cmd->buf[i]) : 0);
        //cmd->stride[i] = stride ? stride[i] : 0;
        //cmd->spec[i].instance = FALSE;
        //cmd->spec[i].instDivisor = 1;

        if (cmd->buf[i])
        {
            AfxAssertRange(AfxGetBufferSize(cmd->buf[i]), cmd->offset[i], cmd->range[i]);
        }
    }

    _SglEncodeCmdCommand(dscr, AFX_DCMD_BIND_VERTEX_SOURCES, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdVertexStreams *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->srcIdx[i] = srcIdx ? srcIdx[i] : 0;
        cmd->stride[i] = stride ? stride[i] : 0;
        cmd->instance[i] = instance ? instance[i] : FALSE;
    }

    cmd->reset = TRUE;
    _SglEncodeCmdCommand(dscr, AFX_DCMD_RESET_VERTEX_STREAMS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdUpdateVertexStreams(afxDrawScript dscr, afxNat first, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdVertexStreams *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->srcIdx[i] = srcIdx ? srcIdx[i] : 0;
        cmd->stride[i] = stride ? stride[i] : 0;
        cmd->instance[i] = instance ? instance[i] : FALSE;
    }

    cmd->reset = FALSE;
    _SglEncodeCmdCommand(dscr, AFX_DCMD_UPDATE_VERTEX_STREAMS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const streamIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdVertexAttributes *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->location[i] = location ? location[i] : 0;
        cmd->fmt[i] = fmt ? fmt[i] : NIL;
        AfxAssert(cmd->fmt[i]);
        cmd->streamIdx[i] = streamIdx ? streamIdx[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
    }

    cmd->reset = TRUE;
    _SglEncodeCmdCommand(dscr, AFX_DCMD_RESET_VERTEX_ATTRIBUTES, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdUpdateVertexAttributes(afxDrawScript dscr, afxNat first, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const streamIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdVertexAttributes *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->location[i] = location ? location[i] : 0;
        cmd->fmt[i] = fmt ? fmt[i] : NIL;
        AfxAssert(cmd->fmt[i]);
        cmd->streamIdx[i] = streamIdx ? streamIdx[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
    }

    cmd->reset = FALSE;
    _SglEncodeCmdCommand(dscr, AFX_DCMD_UPDATE_VERTEX_ATTRIBUTES, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdBindIndexSource(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBufferRange *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);

    cmd->offset = offset;
    cmd->idxSiz = idxSiz;

    if ((cmd->buf = buf))
    {
        AfxAssertObjects(1, &cmd->buf, AFX_FCC_BUF);
        AfxAssertRange(AfxGetBufferSize(cmd->buf), cmd->offset, 1);
    }

    _SglEncodeCmdCommand(dscr, AFX_DCMD_BIND_INDEX_SOURCE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetPrimitiveTopology(afxDrawScript dscr, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = topology;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_PRIMITIVE_TOPOLOGY, sizeof(cmd), &cmd->cmd);
}

// Fixed-function vertex post-processing

_SGL void _SglEncodeCmdUpdateViewports(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdViewport *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_UPDATE_VIEWPORTS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdResetViewports(afxDrawScript dscr, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdViewport *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_RESET_VIEWPORTS, sizeof(cmd), &cmd->cmd);
}

// Rasterization

_SGL void _SglEncodeCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = disable;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_DISABLE_RASTERIZATION, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSwitchFrontFace(afxDrawScript dscr, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = cw;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SWITCH_FRONT_FACE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetCullMode(afxDrawScript dscr, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = mode;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_CULL_MODE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_ENABLE_DEPTH_BIAS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdReal3 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = constFactor;
    cmd->value[1] = clamp;
    cmd->value[2] = slopeFactor;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_DEPTH_BIAS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdReal *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = lineWidth;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_LINE_WIDTH, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdUpdateScissors(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_UPDATE_SCISSORS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdResetScissors(afxDrawScript dscr, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_RESET_SCISSORS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_ENABLE_DEPTH_BOUNDS_TEST, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdReal2 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = bounds[0];
    cmd->value[1] = bounds[1];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_DEPTH_BOUNDS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_ENABLE_STENCIL_TEST, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetStencilCompareMask(afxDrawScript dscr, afxBitmask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = compareMask;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_STENCIL_COMPARE_MASK, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetStencilWriteMask(afxDrawScript dscr, afxBitmask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = writeMask;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_STENCIL_WRITE_MASK, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetStencilReference(afxDrawScript dscr, afxBitmask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = reference;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_STENCIL_REFERENCE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_ENABLE_DEPTH_TEST, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = op;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_DEPTH_COMPARE_OP, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdEnableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_ENABLE_DEPTH_WRITE, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdReal4 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = blendConstants[0];
    cmd->value[1] = blendConstants[1];
    cmd->value[2] = blendConstants[2];
    cmd->value[3] = blendConstants[3];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_BLEND_CONSTANTS, sizeof(cmd), &cmd->cmd);
}

// Canvas

_SGL void _SglEncodeCmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdBeginCanvas *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    AfxRectCopy(&cmd->area, &state->area);
    cmd->layerCnt = state->layerCnt;
    cmd->rasterCnt = state->rasterCnt;
    //cmd->canv = state->canv;

    for (afxNat i = 0; i < state->rasterCnt; i++)
        cmd->rasters[i] = state->rasters[i];

    if (state->depth)
        cmd->depth = *state->depth;
    else
        cmd->depth = (afxDrawTarget) { 0 };

    if (state->stencil)
        cmd->stencil = *state->stencil;
    else
        cmd->stencil = (afxDrawTarget) { 0 };

    cmd->fboHandle = 0;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_BEGIN_CANVAS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdEndCanvas(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmd *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    _SglEncodeCmdCommand(dscr, AFX_DCMD_END_CANVAS, sizeof(cmd), cmd);
}

_SGL void _SglEncodeCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdNextPass *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    _SglEncodeCmdCommand(dscr, AFX_DCMD_NEXT_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript subsets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdExecCmds *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->subsets[i] = subsets[i];

    _SglEncodeCmdCommand(dscr, AFX_DCMD_EXECUTE_COMMANDS, sizeof(cmd), &cmd->cmd);
}

// Draw

_SGL void _SglEncodeCmdDraw(afxDrawScript dscr, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdDraw *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_DRAW, sizeof(cmd), &cmd->cmd);
}

_SGL void _SglEncodeCmdDrawIndexed(afxDrawScript dscr, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdDrawIndexed *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    _SglEncodeCmdCommand(dscr, AFX_DCMD_DRAW_INDEXED, sizeof(cmd), &cmd->cmd);
}








#if 0
_SGL void _SglEncodeCmdSetVertexInputLayout(afxDrawScript dscr, afxNat cnt, afxVertexInputPoint const spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _afxDscrCmdSetVtxInLayout *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->spec[i] = spec[i];
    }
    _SglEncodeCmdCommand(dscr, AFX_DCMD_SET_VERTEX_INPUT_LAYOUT, sizeof(cmd), &cmd->cmd);
}
#endif

_SGL afxCmd _SglEncodeCmdVmt =
{
    .BindPipeline = _SglEncodeCmdBindPipeline,

    .BindVertexSources = _SglEncodeCmdBindVertexSources,
    .ResetVertexStreams = _SglEncodeCmdResetVertexStreams,
    .UpdateVertexStreams = _SglEncodeCmdUpdateVertexStreams,
    .ResetVertexAttributes = _SglEncodeCmdResetVertexAttributes,
    .UpdateVertexAttributes = _SglEncodeCmdUpdateVertexAttributes,
    .BindIndexSource = _SglEncodeCmdBindIndexSource,
    .SetPrimitiveTopology = _SglEncodeCmdSetPrimitiveTopology,

    .ResetViewports = _SglEncodeCmdResetViewports,
    .UpdateViewports = _SglEncodeCmdUpdateViewports,

    .DisableRasterization = _SglEncodeCmdDisableRasterization,
    .SwitchFrontFace = _SglEncodeCmdSwitchFrontFace,
    .SetCullMode = _SglEncodeCmdSetCullMode,
    .EnableDepthBias = _SglEncodeCmdEnableDepthBias,
    .SetDepthBias = _SglEncodeCmdSetDepthBias,
    .SetLineWidth = _SglEncodeCmdSetLineWidth,

    .ResetScissors = _SglEncodeCmdResetScissors,
    .UpdateScissors = _SglEncodeCmdUpdateScissors,

    .EnableDepthBoundsTest = _SglEncodeCmdEnableDepthBoundsTest,
    .SetDepthBounds = _SglEncodeCmdSetDepthBounds,

    .EnableStencilTest = _SglEncodeCmdEnableStencilTest,
    .SetStencilCompareMask = _SglEncodeCmdSetStencilCompareMask,
    .SetStencilWriteMask = _SglEncodeCmdSetStencilWriteMask,
    .SetStencilReference = _SglEncodeCmdSetStencilReference,

    .EnableDepthTest = _SglEncodeCmdEnableDepthTest,
    .SetDepthCompareOp = _SglEncodeCmdSetDepthCompareOp,
    .EnableDepthWrite = _SglEncodeCmdEnableDepthWrite,

    .SetBlendConstants = _SglEncodeCmdSetBlendConstants,

    .Draw = _SglEncodeCmdDraw,
    .DrawIndexed = _SglEncodeCmdDrawIndexed,

    .ExecuteCommands = _SglEncodeCmdExecuteCommands,
    .BeginCanvas = _SglEncodeCmdBeginCanvas,
    .NextPass = _SglEncodeCmdNextPass,
    .EndCanvas = _SglEncodeCmdEndCanvas,

    .BindBuffers = _SglEncodeCmdBindBuffers,    
    .BindTextures = _SglEncodeCmdBindTextures,
};
