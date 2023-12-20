/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

_SGL afxCmdId _SglEncodeCmdCommand(afxDrawScript dscr, afxNat id, afxNat siz, _sglCmd *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    cmd->id = id;
    AfxAssertRange(SGL_CMD_TOTAL, cmd->id, 1);

    if (err)
    {
        int a = 0;
    }

    cmd->siz = siz;
    return AfxPushLinkage(&cmd->script, &dscr->commands);
}

///////////////////////////////////////////

_SGL afxCmdId FlushNextCachedPipelineBinding(afxDrawScript dscr, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);
    afxPipeline curr = dscr->levelCaches[segment].pip.curr;
    afxPipeline next = dscr->levelCaches[segment].pip.next;

    if ((curr != next) && next)
    {
        dscr->levelCaches[segment].pip.curr = next;
        dscr->levelCaches[segment].pip.next = NIL;

        _sglCmdPipeline *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
        AfxAssert(cmd);
        cmd->segment = segment;
        cmd->pip = next;
        return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, BindPipeline) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
    }
    return 0;
}

_SGL afxCmdId FlushCachedDrawStuff(afxDrawScript dscr, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    return FlushNextCachedPipelineBinding(dscr, segment);
}

_SGL afxCmdId FlushCachedDrawIndexedStuff(afxDrawScript dscr, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);
    return FlushCachedDrawStuff(dscr, segment);
}

_SGL afxCmdId FlushCachedStuff(afxDrawScript dscr, afxNat segment)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);
    return FlushCachedDrawIndexedStuff(dscr, segment);
}

_SGL afxCmdId _SglEncodeCmdBindPipeline(afxDrawScript dscr, afxNat segment, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);
#if 0
    afxPipeline next = dscr->levelCaches[line].pip.next;

    if (next != pip)
    {
        afxCmdId cmdId = FlushNextCachedPipelineBinding(dscr, line);
        dscr->levelCaches[line].pip.next = pip;
        return cmdId;
    }
#else

    _sglCmdPipeline *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->segment = segment;
    cmd->pip = pip;
    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, BindPipeline) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
#endif
    return 0;
}

_SGL afxCmdId _SglEncodeCmdBindVertexInput(afxDrawScript dscr, afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);
    
    _sglCmdVertexInput *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vin = vin;
    return _SglEncodeCmdCommand(dscr, (offsetof(afxCmd, BindVertexInput) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

// Sources and resources

_SGL afxCmdId _SglEncodeCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBindBuffers *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->buf[i] = buf ? buf[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
        cmd->range[i] = range ? range[i] : 0;
    }
    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, BindBuffers) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBindRasters(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxRaster tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBindRasters *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->set = set;
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->smp[i] = smp ? smp[i] : 0;
        cmd->tex[i] = tex ? tex[i] : 0;
    }

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, BindRasters) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBindVertexSources(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdVertexSources *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        if ((cmd->buf[i] = buf ? buf[i] : NIL))
        {
            AfxAssertObjects(1, &cmd->buf[i], afxFcc_BUF);
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

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, BindVertexSources) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

#if 0
_SGL afxCmdId _SglEncodeCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdVertexStreams *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->srcIdx[i] = srcIdx ? srcIdx[i] : 0;
        cmd->stride[i] = stride ? stride[i] : 0;
        cmd->instance[i] = instance ? instance[i] : FALSE;
    }

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, ResetVertexStreams) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdVertexAttributes *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->location[i] = location ? location[i] : 0;
        cmd->fmt[i] = fmt ? fmt[i] : NIL;
        AfxAssert(cmd->fmt[i]);
        cmd->srcIdx[i] = srcIdx ? srcIdx[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
    }

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, ResetVertexAttributes) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}
#endif

_SGL afxCmdId _SglEncodeCmdBindIndexSource(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBufferRange *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);

    cmd->offset = offset;
    cmd->range = range;
    cmd->stride = idxSiz;

    if ((cmd->buf = buf))
    {
        AfxAssertObjects(1, &cmd->buf, afxFcc_BUF);
        AfxAssertRange(AfxGetBufferSize(cmd->buf), cmd->offset, cmd->range);
    }

    return _SglEncodeCmdCommand(dscr, SGL_CMD_BIND_INDEX_SOURCE, sizeof(cmd), &cmd->cmd);
}

// Fixed-function vertex post-processing

_SGL afxCmdId _SglEncodeCmdSetPrimitiveTopology(afxDrawScript dscr, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = topology;

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, SetPrimitiveTopology) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSwitchFrontFace(afxDrawScript dscr, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = cw;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SWITCH_FRONT_FACE, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetCullMode(afxDrawScript dscr, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = mode;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_CULL_MODE, sizeof(cmd), &cmd->cmd);
}

// Viewports, synthesis areas and scissors.

_SGL afxCmdId _SglEncodeCmdReadjustViewports(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdViewport *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, ReadjustViewports) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetViewports(afxDrawScript dscr, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdViewport *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->vp[i] = vp[i];

    return _SglEncodeCmdCommand(dscr, offsetof(afxCmd, ResetViewports) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdReadjustScissors(afxDrawScript dscr, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_READJUST_SCISSORS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetScissors(afxDrawScript dscr, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdScissor *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_RESET_SCISSORS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdReadjustAreas(afxDrawScript dscr, afxBool exclusive, afxNat32 first, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdArea *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->exclusive = exclusive;
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_READJUST_AREAS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetAreas(afxDrawScript dscr, afxBool exclusive, afxNat32 cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdArea *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->exclusive = exclusive;
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;

    for (afxNat i = 0; i < cnt; i++)
        cmd->rect[i] = rect[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_RESET_AREAS, sizeof(cmd), &cmd->cmd);
}

// Rasterization

_SGL afxCmdId _SglEncodeCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = disable;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_DISABLE_RASTERIZATION, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_ENABLE_DEPTH_BIAS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal3 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = constFactor;
    cmd->value[1] = clamp;
    cmd->value[2] = slopeFactor;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_DEPTH_BIAS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = lineWidth;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_LINE_WIDTH, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_ENABLE_DEPTH_BOUNDS_TEST, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal2 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = bounds[0];
    cmd->value[1] = bounds[1];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_DEPTH_BOUNDS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_ENABLE_STENCIL_TEST, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetStencilCompareMask(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = compareMask;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_STENCIL_COMPARE_MASK, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetStencilWriteMask(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = writeMask;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_STENCIL_WRITE_MASK, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetStencilReference(afxDrawScript dscr, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBitmaskNat32 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->mask = faceMask;
    cmd->value = reference;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_STENCIL_REFERENCE, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_ENABLE_DEPTH_TEST, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdNat *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = op;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_DEPTH_COMPARE_OP, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdEnableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = enable;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_ENABLE_DEPTH_WRITE, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdReal4 *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value[0] = blendConstants[0];
    cmd->value[1] = blendConstants[1];
    cmd->value[2] = blendConstants[2];
    cmd->value[3] = blendConstants[3];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_BLEND_CONSTANTS, sizeof(cmd), &cmd->cmd);
}

// Raster

_SGL afxCmdId _SglEncodeCmdPackRaster(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    _sglCmdPackRasterRegions *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->unpack = FALSE;
    cmd->ras = ras;
    cmd->buf = buf;
    cmd->opCnt = opCnt;
    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_PACK_RASTER, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdUnpackRaster(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    _sglCmdPackRasterRegions *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->unpack = TRUE;
    cmd->ras = ras;
    cmd->buf = buf;
    cmd->opCnt = opCnt;
    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_UNPACK_RASTER, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdCopyRaster(afxDrawScript dscr, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdCopyRasterRegions *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->src = src;
    cmd->dst = dst;
    cmd->opCnt = opCnt;
    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_COPY_RASTER, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdRegenerateMipmaps(afxDrawScript dscr, afxRaster ras, afxNat baseLodIdx, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdRegenerateMipmaps *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->ras = ras;
    cmd->baseLodIdx = baseLodIdx;
    cmd->lodCnt = lodCnt;
    
    return _SglEncodeCmdCommand(dscr, SGL_CMD_REGENERATE_MIPMAPS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSwizzleRaster(afxDrawScript dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdSwizzleRasterRegions *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->ras = ras;
    cmd->a = a;
    cmd->b = b;
    cmd->rgnCnt = rgnCnt;
    AfxAssert(8 > rgnCnt);

    for (afxNat i = 0; i < rgnCnt; i++)
        cmd->rgn[i] = rgn[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_SWIZZLE_RASTER, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdFlipRaster(afxDrawScript dscr, afxRaster ras, afxBool rows, afxBool columns, afxNat rgnCnt, afxRasterRegion const rgn[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdFlipRasterRegions *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->ras = ras;
    cmd->rows = rows;
    cmd->columns = columns;
    cmd->rgnCnt = rgnCnt;
    AfxAssert(8 > rgnCnt);

    for (afxNat i = 0; i < rgnCnt; i++)
        cmd->rgn[i] = rgn[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_FLIP_RASTER, sizeof(cmd), &cmd->cmd);
}



// Canvas

_SGL afxCmdId _SglEncodeCmdBeginSynthesis(afxDrawScript dscr, afxSynthesisConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdBeginSynthesis *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);

    AfxRectCopy(&cmd->area, &state->area);
    cmd->layerCnt = state->layerCnt;
    cmd->rasterCnt = state->rasterCnt;
    cmd->canv = state->canv;
    cmd->defFbo = FALSE;

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

    return _SglEncodeCmdCommand(dscr, SGL_CMD_BEGIN_SYNTHESIS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdFinsihSynthesis(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmd *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    return _SglEncodeCmdCommand(dscr, SGL_CMD_FINISH_SYNTHESIS, sizeof(cmd), cmd);
}

_SGL afxCmdId _SglEncodeCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdNextPass *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->useAuxScripts = !!useAuxScripts;
    return _SglEncodeCmdCommand(dscr, SGL_CMD_NEXT_PASS, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript subsets[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdExecCmds *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
        cmd->subsets[i] = subsets[i];

    return _SglEncodeCmdCommand(dscr, SGL_CMD_EXECUTE_COMMANDS, sizeof(cmd), &cmd->cmd);
}

// Draw

_SGL afxCmdId _SglEncodeCmdDraw(afxDrawScript dscr, afxNat32 vtxCnt, afxNat32 instCnt, afxNat32 firstVtx, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    FlushCachedDrawStuff(dscr, 0);

    _sglCmdDraw *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vtxCnt = vtxCnt;
    cmd->instCnt = instCnt;
    cmd->firstVtx = firstVtx;
    cmd->firstInst = firstInst;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_DRAW, sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdDrawIndexed(afxDrawScript dscr, afxNat32 idxCnt, afxNat32 instCnt, afxNat32 firstIdx, afxNat32 vtxOff, afxNat32 firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    FlushCachedDrawIndexedStuff(dscr, 0);

    _sglCmdDrawIndexed *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->idxCnt = idxCnt;
    cmd->instCnt = instCnt;
    cmd->firstIdx = firstIdx;
    cmd->vtxOff = vtxOff;
    cmd->firstInst = firstInst;

    return _SglEncodeCmdCommand(dscr, SGL_CMD_DRAW_INDEXED, sizeof(cmd), &cmd->cmd);
}

#if 0
_SGL afxCmdId _SglEncodeCmdSetVertexInputLayout(afxDrawScript dscr, afxNat cnt, afxVertexInputPoint const spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dscr->base.state == afxDrawScriptState_RECORDING);

    _sglCmdSetVtxInLayout *cmd = AfxRequestArenaUnit(&dscr->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->spec[i] = spec[i];
    }
    return _SglEncodeCmdCommand(dscr, SGL_CMD_SET_VERTEX_INPUT_LAYOUT, sizeof(cmd), &cmd->cmd);
}
#endif

_SGL afxError _SglDscrEndCb(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    FlushCachedStuff(dscr, 0);
    return err;
}

_SGL afxCmd _SglEncodeCmdVmt =
{
    .BindPipeline = _SglEncodeCmdBindPipeline,
    .BindVertexInput = _SglEncodeCmdBindVertexInput,

    .BindVertexSources = _SglEncodeCmdBindVertexSources,
    //.ResetVertexStreams = _SglEncodeCmdResetVertexStreams,
    //.ResetVertexAttributes = _SglEncodeCmdResetVertexAttributes,
    .BindIndexSource = _SglEncodeCmdBindIndexSource,
    .SetPrimitiveTopology = _SglEncodeCmdSetPrimitiveTopology,

    .ResetViewports = _SglEncodeCmdResetViewports,
    .ReadjustViewports = _SglEncodeCmdReadjustViewports,

    .DisableRasterization = _SglEncodeCmdDisableRasterization,
    .SwitchFrontFace = _SglEncodeCmdSwitchFrontFace,
    .SetCullMode = _SglEncodeCmdSetCullMode,
    .EnableDepthBias = _SglEncodeCmdEnableDepthBias,
    .SetDepthBias = _SglEncodeCmdSetDepthBias,
    .SetLineWidth = _SglEncodeCmdSetLineWidth,

    .ResetScissors = _SglEncodeCmdResetScissors,
    .ReadjustScissors = _SglEncodeCmdReadjustScissors,

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
    .BeginSynthesis = _SglEncodeCmdBeginSynthesis,
    .NextPass = _SglEncodeCmdNextPass,
    .FinishSynthesis = _SglEncodeCmdFinsihSynthesis,

    .BindBuffers = _SglEncodeCmdBindBuffers,    
    .BindRasters = _SglEncodeCmdBindRasters,

    .SwizzleRaster = _SglEncodeCmdSwizzleRaster,
    .FlipRaster = _SglEncodeCmdFlipRaster,
    .RegenerateMipmaps = _SglEncodeCmdRegenerateMipmaps,
    .CopyRaster = _SglEncodeCmdCopyRaster,
    .UnpackRaster = _SglEncodeCmdUnpackRaster,
    .PackRaster = _SglEncodeCmdPackRaster,
};
