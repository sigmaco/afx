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

#define _AFX_DRAW_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#include "sgl.h"

#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

#define _SGL_DBG_IGNORE_DEPTH_RANGE
#define _SGL_DBG_IGNORE_PRIM_RESTART
#define _SGL_DBG_IGNORE_DEPTH_CLAMP
#define _SGL_DBG_IGNORE_RASTERIZER_DISCARD
#define _SGL_DBG_IGNORE_DEPTH_BIAS
//#define _SGL_DBG_IGNORE_DEPTH_TEST
//#define _SGL_DBG_IGNORE_DEPTH_WRITE
#define _SGL_DBG_IGNORE_DEPTH_BOUNDS_TEST
#define _SGL_DBG_IGNORE_LINE_WIDTH
#define _SGL_DBG_IGNORE_STENCIL_TEST
#define _SGL_DBG_IGNORE_MULTISAMPLING
#define _SGL_DBG_IGNORE_BLEND
#define _SGL_DBG_IGNORE_LOGICAL_OP
#define _SGL_DBG_IGNORE_COLOR_MASK
#define _SGL_DBG_IGNORE_SCISSOR_TEST

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

// RENDERING SCOPE

_SGL void _SglDpuFinishSynthesis(sglDpuIdd* dpu, _sglCmd const *cmd)
{
    (void)cmd;
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    _SglDpuBindAndSyncCanv(gl, FALSE, GL_DRAW_FRAMEBUFFER, 0);
    gl->Flush(); _SglThrowErrorOccuried();

    afxCanvas canv = dpu->state.renderPass.canv;

    if (canv)
    {
        afxNat surCnt = AfxCountSurfaces(canv);

        for (afxNat i = 0; i < surCnt; i++)
        {
            afxRaster ras;

            if (AfxGetSurfaceRasters(canv, i, 1, &ras))
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);
                ras->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
            }
        }
    }
};

_SGL void _SglDpuBeginSynthesis(sglDpuIdd* dpu, _sglCmdBeginSynthesis const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxCanvas canv = cmd->canv;
    afxDrawTarget const* dt;

    _SglDpuBindAndSyncCanv(gl, FALSE, GL_DRAW_FRAMEBUFFER, canv);

    if (!canv)
    {
        if (cmd->defFbo)
        {
            dt = &cmd->rasters[0];

            if (dt->storeOp == afxSurfaceStoreOp_DISCARD) // nem faz sentido descartar o default FB
            {
                afxNat wh[2];
                AfxGetCanvasExtent(canv, wh);

                if ((cmd->area.x == 0) && (cmd->area.y == 0) && (cmd->area.w == wh[0]) && (cmd->area.h == wh[1]))
                {
                    gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK_LEFT }); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK_LEFT }, cmd->area.x, cmd->area.y, cmd->area.w, cmd->area.h); _SglThrowErrorOccuried();
                }
            }

            if (dt->loadOp == afxSurfaceLoadOp_CLEAR)
            {
                afxReal const *color = dt->clearValue.color;
                gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
                gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else if (dt->loadOp == afxSurfaceLoadOp_LOAD)
            {
                _SglDpuBindAndSyncCanv(gl, FALSE, GL_READ_FRAMEBUFFER, 0);
            }
        }
    }
    else
    {
        GLuint glHandle = canv->glHandle;
        AfxAssert(gl->IsFramebuffer(glHandle));

        afxNat surCnt = cmd->rasterCnt;
        afxNat maxSurCnt = AfxCountColorSurfaces(canv);
        surCnt = AfxMin(surCnt, maxSurCnt);

        afxNat colorAttchEnabledCnt = 0;
        afxNat colorAttchStoreCnt = 0;
        afxNat colorAttchDiscardCnt = 0;
        GLenum drawBufferIndices[_SGL_MAX_SURF_PER_CANV];
        GLenum drawBufferStoreIndices[_SGL_MAX_SURF_PER_CANV];
        GLenum drawBufferDiscardIndices[_SGL_MAX_SURF_PER_CANV];

        for (afxNat i = 0; i < surCnt; i++)
        {
            dt = &cmd->rasters[i];

            if (dt->storeOp == afxSurfaceStoreOp_STORE)
            {
                drawBufferStoreIndices[colorAttchStoreCnt] = GL_COLOR_ATTACHMENT0 + i;
                ++colorAttchStoreCnt;
            }
            else if (dt->storeOp == afxSurfaceStoreOp_DISCARD)
            {
                drawBufferDiscardIndices[colorAttchDiscardCnt] = GL_COLOR_ATTACHMENT0 + i;
                ++colorAttchDiscardCnt;
            }
            drawBufferDiscardIndices[colorAttchEnabledCnt] = GL_COLOR_ATTACHMENT0 + i;
            colorAttchEnabledCnt++;
        }

        gl->DrawBuffers(colorAttchEnabledCnt, drawBufferIndices); _SglThrowErrorOccuried();

        if (colorAttchDiscardCnt)
        {
            afxNat wh[2];
            AfxGetCanvasExtent(canv, wh);

            if ((cmd->area.x == 0) && (cmd->area.y == 0) && (cmd->area.w == wh[0]) && (cmd->area.h == wh[1]))
            {
                gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, colorAttchDiscardCnt, drawBufferDiscardIndices); _SglThrowErrorOccuried();
            }
            else
            {
                gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, colorAttchDiscardCnt, drawBufferDiscardIndices, cmd->area.x, cmd->area.y, cmd->area.w, cmd->area.h); _SglThrowErrorOccuried();
            }
        }

        for (afxNat i = 0; i < colorAttchEnabledCnt; i++)
        {
            dt = &cmd->rasters[i];

            if (dt->loadOp == afxSurfaceLoadOp_CLEAR)
            {
                afxReal const *color = dt->clearValue.color;
                gl->ClearBufferfv(GL_COLOR, GL_DRAW_BUFFER0 + i, color); _SglThrowErrorOccuried();
            }
        }

        afxNat dsSurIdx[2];
        AfxGetDepthSurface(canv, &dsSurIdx[0]);
        afxBool combinedDs = (AfxGetStencilSurface(canv, &dsSurIdx[1]) && (dsSurIdx[1] == dsSurIdx[0]));
        
        if (dsSurIdx[0] != AFX_INVALID_INDEX)
        {
            dt = &cmd->depth;

            if (dt->loadOp == afxSurfaceLoadOp_CLEAR)
            {
                if (!combinedDs)
                {
                    gl->ClearBufferfv(GL_DEPTH, 0, &dt->clearValue.depth); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->ClearBufferfi(GL_DEPTH_STENCIL, 0, dt->clearValue.depth, dt->clearValue.stencil); _SglThrowErrorOccuried();
                }
            }
        }

        if (!combinedDs && (dsSurIdx[1] != AFX_INVALID_INDEX))
        {
            dt = &cmd->stencil;

            if (dt->loadOp == afxSurfaceLoadOp_CLEAR)
            {
                GLint sCv = dt->clearValue.stencil;
                gl->ClearBufferiv(GL_STENCIL, 0, &sCv); _SglThrowErrorOccuried();
            }
        }
    }
}

_SGL void _SglDpuNextPass(sglDpuIdd* dpu, _sglCmd const *cmd)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    (void)cmd;
    //++(dpu->state.renderPass.activeSubpass);
}

// STATE SETTING

_SGL void _SglDpuDisableRasterization(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.rasterizationDisabled = cmd->value;
}

_SGL void _SglDpuSwitchFrontFace(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextXformState.cwFrontFacing = cmd->value;
}

_SGL void _SglDpuSetCullMode(sglDpuIdd* dpu, _sglCmdNat const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextXformState.cullMode = cmd->value;
}

_SGL void _SglDpuEnableDepthBias(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthBiasEnabled = cmd->value;
}

_SGL void _SglDpuSetDepthBias(sglDpuIdd* dpu, _sglCmdReal3 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthBiasConstFactor = cmd->value[0];
    dpu->nextRasterState.depthBiasClamp = cmd->value[1];
    dpu->nextRasterState.depthBiasSlopeScale = cmd->value[2];
}

_SGL void _SglDpuSetLineWidth(sglDpuIdd* dpu, _sglCmdReal const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.lineWidth = cmd->value;
}

_SGL void _SglDpuEnableStencilTest(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.stencilTestEnabled = cmd->value;
}

_SGL void _SglDpuSetStencilCompareMask(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange((AfxGetBitOffset(0) | AfxGetBitOffset(1)), AfxGetBitOffset(0), cmd->mask);

    if (cmd->mask & AfxGetBitOffset(0))
        dpu->nextRasterState.stencilFront.compareMask = cmd->value;

    if (cmd->mask & AfxGetBitOffset(1))
        dpu->nextRasterState.stencilBack.compareMask = cmd->value;
}

_SGL void _SglDpuSetStencilWriteMask(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange((AfxGetBitOffset(0) | AfxGetBitOffset(1)), AfxGetBitOffset(0), cmd->mask);

    if (cmd->mask & AfxGetBitOffset(0))
        dpu->nextRasterState.stencilFront.writeMask = cmd->value;

    if (cmd->mask & AfxGetBitOffset(1))
        dpu->nextRasterState.stencilBack.writeMask = cmd->value;
}

_SGL void _SglDpuSetStencilReference(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange((AfxGetBitOffset(0) | AfxGetBitOffset(1)), AfxGetBitOffset(0), cmd->mask);

    if (cmd->mask & AfxGetBitOffset(0))
        dpu->nextRasterState.stencilFront.reference = cmd->value;

    if (cmd->mask & AfxGetBitOffset(1))
        dpu->nextRasterState.stencilBack.reference = cmd->value;
}

_SGL void _SglDpuEnableDepthTest(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthTestEnabled = cmd->value;
}

_SGL void _SglDpuSetDepthCompareOp(sglDpuIdd* dpu, _sglCmdNat const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthCompareOp = cmd->value;    
}

_SGL void _SglDpuEnableDepthWrite(sglDpuIdd* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextRasterState.depthWriteEnabled = cmd->value;    
}

_SGL void _SglDpuSetBlendConstants(sglDpuIdd* dpu, _sglCmdReal4 const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxCopyV4d(dpu->nextRasterState.blendConstants, cmd->value);
    dpu->nextBlendConstUpd = TRUE;
}

_SGL void _SglDpuSetPrimitiveTopology(sglDpuIdd* dpu, _sglCmdNat const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextXformState.primTop = cmd->value;
}

_SGL void _SglDpuSetViewports(sglDpuIdd* dpu, _sglCmdViewport const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VIEWPORTS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
        dpu->nextXformState.vps[first + i] = cmd->vp[i], dpu->nextViewportUpdMask |= AfxGetBitOffset(first + i);

    dpu->nextViewportUpdCnt = AfxMax(dpu->nextViewportUpdCnt, cnt);

    if (cmd->reset)
        dpu->nextXformState.vpCnt = cnt;
}

_SGL void _SglDpuSetScissors(sglDpuIdd* dpu, _sglCmdScissor const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VIEWPORTS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
        dpu->nextRasterState.scisRects[first + i] = cmd->rect[i], dpu->nextScissorUpdMask |= AfxGetBitOffset(first + i);

    dpu->nextScissorUpdCnt = AfxMax(dpu->nextScissorUpdCnt, cnt);

    if (cmd->reset)
        dpu->nextRasterState.scisCnt = cnt;
}

_SGL void _SglDpuSetAreas(sglDpuIdd* dpu, _sglCmdArea const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VIEWPORTS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
        dpu->nextRasterState.areaRects[first + i] = cmd->rect[i], dpu->nextAreaUpdMask |= AfxGetBitOffset(first + i);

    dpu->nextAreaUpdCnt = AfxMax(dpu->nextAreaUpdCnt, cnt);

    if (cmd->reset)
        dpu->nextRasterState.areaCnt = cnt;
}

_SGL void _SglDpuBindVertexSources(sglDpuIdd* dpu, _sglCmdVertexSources const *cmd)
{
    /*
        The values taken from elements i of pBuffers and pOffsets replace the current state for the vertex input binding firstBinding + i, for i in[0, bindingCount).
        The vertex input binding is updated to start at the offset indicated by pOffsets[i] from the start of the buffer pBuffers[i].
        If pSizes is not NULL then pSizes[i] specifies the bound size of the vertex buffer starting from the corresponding elements of pBuffers[i] plus pOffsets[i].
        All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
        If the nullDescriptor feature is enabled, elements of pBuffers can be VK_NULL_HANDLE, and can be used by the vertex shader.
        If a vertex input attribute is bound to a vertex input binding that is VK_NULL_HANDLE, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with(0, 0, 1).

        This command also dynamically sets the byte strides between consecutive elements within buffer pBuffers[i] to the corresponding pStrides[i] value when drawing using shader objects, or when the graphics pipeline is created with VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE set in VkPipelineDynamicStateCreateInfo::pDynamicStates.Otherwise, strides are specified by the VkVertexInputBindingDescription::stride values used to create the currently active pipeline.

        If drawing using shader objects or if the bound pipeline state object was also created with the VK_DYNAMIC_STATE_VERTEX_INPUT_EXT dynamic state enabled then vkCmdSetVertexInputEXT can be used instead of vkCmdBindVertexBuffers2 to set the stride.
    */

    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat bindingIdx = first + i;
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, bindingIdx, 1);

        afxBuffer buf = cmd->buf[i];
        afxNat32 offset = cmd->offset[i];
        afxNat32 range = cmd->range[i];
        
        dpu->nextVinBindings.sources[bindingIdx].buf = buf;
        dpu->nextVinBindings.sources[bindingIdx].offset = offset;
        dpu->nextVinBindings.sources[bindingIdx].range = range;
        dpu->nextVinBindingsMask |= AfxGetBitOffset(bindingIdx);
    }
    dpu->nextVinBindingsCnt = AfxMax(dpu->nextVinBindingsCnt, cnt);
}

#if 0
_SGL void _SglDpuResetVertexStreams(sglDpuIdd* dpu, _sglCmdVertexStreams const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, 0, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        //afxNat streamIdx = i;

        afxNat srcIdx = cmd->srcIdx[i];
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, srcIdx, 1);
        afxNat stride = cmd->stride[i];
        afxBool instance = cmd->instance[i];
        
        dpu->nextVertexInput.streams[srcIdx].srcIdx = srcIdx;
        dpu->nextVertexInput.streams[srcIdx].stride = stride;
        dpu->nextVertexInput.streams[srcIdx].instance = instance;
        dpu->nextVtxInStreamUpdMask |= AfxGetBitOffset(srcIdx);
    }
    dpu->nextVtxInStreamUpdCnt = AfxMax(dpu->nextVtxInStreamUpdCnt, cnt);
}

_SGL void _SglDpuResetVertexAttributes(sglDpuIdd* dpu, _sglCmdVertexAttributes const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VERTEX_ATTRIBS, 0, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat location = cmd->location[i];
        afxVertexFormat fmt = cmd->fmt[i];
        afxNat srcIdx = cmd->srcIdx[i];
        afxNat32 offset = cmd->offset[i];
        
        afxNat attrIdx = location;
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIBS, attrIdx, 1);
        dpu->nextVertexInput.attrs[attrIdx].location = location;
        dpu->nextVertexInput.attrs[attrIdx].fmt = fmt;
        dpu->nextVertexInput.attrs[attrIdx].srcIdx = srcIdx;
        dpu->nextVertexInput.attrs[attrIdx].offset = offset;
        dpu->nextVtxInAttribUpdMask |= AfxGetBitOffset(attrIdx);
    }
    dpu->nextVtxInAttribUpdCnt = AfxMax(dpu->nextVtxInAttribUpdCnt, cnt);
}
#endif

_SGL void _SglDpuBindIndexSource(sglDpuIdd* dpu, _sglCmdBufferRange const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextVinBindings.idxSrcBuf = cmd->buf;
    dpu->nextVinBindings.idxSrcOff = cmd->offset;
    dpu->nextVinBindings.idxSrcRange = cmd->range;
    dpu->nextVinBindings.idxSrcSiz = cmd->stride;
    dpu->flushIbb = TRUE;
}

_SGL void _SglDpuBindVertexInput(sglDpuIdd* dpu, _sglCmdVertexInput *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextVin = cmd->vin;
    afxVertexInput vin = cmd->vin;

    if (vin)
    {

    }
}

_SGL void _SglDpuBindPipeline(sglDpuIdd* dpu, _sglCmdPipeline *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextPip = cmd->pip;
    afxPipeline pip = cmd->pip;

    if (pip)
    {
        dpu->nextVin = pip->base.vin;

        dpu->nextXformState.primTop = pip->base.primTop;
        dpu->nextXformState.primRestartEnabled = pip->base.primRestartEnabled;
        dpu->nextXformState.depthClampEnabled = pip->base.depthClampEnabled;
        dpu->nextXformState.cullMode = pip->base.cullMode;
        dpu->nextXformState.cwFrontFacing = pip->base.frontFacingInverted;

        if (pip->base.razr)
        {
            afxRasterizationConfig config;
            AfxDescribeRasterizerConfiguration(pip->base.razr, &config);

            dpu->nextRasterState.alphaToCoverageEnabled = config.alphaToCoverageEnabled;
            dpu->nextRasterState.alphaToOneEnabled = config.alphaToOneEnabled;
            AfxCopyColor(dpu->nextRasterState.blendConstants, config.blendConstants);
            dpu->nextRasterState.outCnt = config.colorOutCnt;
            AfxCopy(config.colorOutCnt, sizeof(config.colorOuts[0]), config.colorOuts, dpu->nextRasterState.outs);
            dpu->nextRasterState.depthBiasClamp = config.depthBiasClamp;
            dpu->nextRasterState.depthBiasConstFactor = config.depthBiasConstFactor;
            dpu->nextRasterState.depthBiasClamp = config.depthBiasEnabled;
            dpu->nextRasterState.depthBiasSlopeScale = config.depthBiasSlopeScale;
            AfxCopyV2d(dpu->nextRasterState.depthBounds, config.depthBounds);
            dpu->nextRasterState.depthBoundsTestEnabled = config.depthBoundsTestEnabled;
            dpu->nextRasterState.depthCompareOp = config.depthCompareOp;
            dpu->nextRasterState.depthTestEnabled = config.depthTestEnabled;
            dpu->nextRasterState.depthWriteEnabled = config.depthWriteEnabled;
            dpu->nextRasterState.fillMode = config.fillMode;
            dpu->nextRasterState.lineWidth = config.lineWidth;
            dpu->nextRasterState.minSampleShadingValue = config.minSampleShadingValue;
            dpu->nextRasterState.msEnabled = config.msEnabled;
            dpu->nextRasterState.logicOp = config.pixelLogicOp;
            dpu->nextRasterState.logicOpEnabled = config.pixelLogicOpEnabled;
            dpu->nextRasterState.sampleCnt = config.sampleCnt;
            AfxCopy(dpu->nextRasterState.sampleCnt, sizeof(config.sampleMasks), config.sampleMasks, dpu->nextRasterState.sampleMasks);
            dpu->nextRasterState.sampleShadingEnabled = config.sampleShadingEnabled;
            dpu->nextRasterState.stencilBack = config.stencilBack;
            dpu->nextRasterState.stencilFront = config.stencilFront;
            dpu->nextRasterState.stencilTestEnabled = config.stencilTestEnabled;
        }
    }
}

// RESOURCE BINDING

_SGL void _SglDpuBindBuffers(sglDpuIdd* dpu, _sglCmdBindBuffers const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    AfxAssertRange(_SGL_MAX_ENTRY_PER_LEGO, first, cmd->cnt);
    AfxAssertRange(_SGL_MAX_LEGO_PER_BIND, cmd->set, 1);

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxNat setIdx = cmd->set;
        afxNat entryIdx = first + i;
        afxBuffer buf = cmd->buf[i];
        afxNat32 offset = cmd->offset[i];
        afxNat32 range = cmd->range[i];
        
        dpu->nextResBind[setIdx][entryIdx].buf = buf;
        dpu->nextResBind[setIdx][entryIdx].offset = offset;
        dpu->nextResBind[setIdx][entryIdx].range = range;
        dpu->nextResBindUpdMask[setIdx] |= AfxGetBitOffset(entryIdx);
    }
}

_SGL void _SglDpuBindRasters(sglDpuIdd* dpu, _sglCmdBindRasters const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    AfxAssertRange(_SGL_MAX_ENTRY_PER_LEGO, first, cmd->cnt);
    AfxAssertRange(_SGL_MAX_LEGO_PER_BIND, cmd->set, 1);

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxNat setIdx = cmd->set;
        afxNat entryIdx = first + i;
        afxRaster ras = cmd->tex[i];
        afxSampler smp = cmd->smp[i];
        dpu->nextResBind[setIdx][entryIdx].ras = ras;
        dpu->nextResBind[setIdx][entryIdx].smp = smp;
        dpu->nextResBindUpdMask[setIdx] |= AfxGetBitOffset(entryIdx);
    }
}

_SGL void _SglDpuRegenerateMipmaps(sglDpuIdd* dpu, _sglCmdRegenerateMipmaps const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat lodCnt = cmd->lodCnt;
    afxNat baseLodIdx = cmd->baseLodIdx;
    AfxAssert(lodCnt);

    afxWhd rasWhd;
    AfxGetRasterExtent(ras, 0, rasWhd);
    afxNat rasLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasLodCnt = AfxCountRasterLods(ras);

    _SglBindAndSyncTex(gl, FALSE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
    gl->GenerateMipmap(ras->glTarget); _SglThrowErrorOccuried();
    _SglBindAndSyncTex(gl, FALSE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);

    AfxAssertRange(rasLodCnt, baseLodIdx, lodCnt);
    AfxThrowError();
}

_SGL void _SglDpuPackRaster(sglDpuIdd* dpu, _sglCmdPackRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster ras = cmd->ras;
    afxBuffer buf = cmd->buf;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxNat opCnt = cmd->opCnt;
    AfxAssert(opCnt);
    afxRasterIoOp const* ops = cmd->ops;
    AfxAssert(ops);

    afxPixelLayout pdf;
    AfxDescribePixelFormat(AfxGetRasterFormat(ras), &pdf);
    afxNat bpp = pdf.bpp;
    afxWhd rasMaxWhd;
    AfxGetRasterExtent(ras, 0, rasMaxWhd);
    afxNat rasMaxLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasMaxLodCnt = AfxCountRasterLods(ras);
    afxNat rasMaxRowSiz = AfxMeasureRasterRow(ras);
    afxNat rasMaxRowCnt = rasMaxWhd[1];

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion rgn = op->rgn;
        AfxAssertRange(rasMaxLodCnt, rgn.lodIdx, 1);
        AfxAssertRange(rasMaxLayerCnt, rgn.baseLayer, rgn.layerCnt);
        AfxAssertRange(rasMaxWhd[0], rgn.offset[0], rgn.whd[0]);
        AfxAssertRange(rasMaxWhd[1], rgn.offset[1], rgn.whd[1]);
        AfxAssertRange(rasMaxWhd[2], rgn.offset[2], rgn.whd[2]);
        
        afxNat rasRowCnt = rgn.whd[1];
        afxSize bufOffset = op->bufOffset;
        afxNat bufRowSiz = op->bufRowSiz;
        afxNat bufRowCnt = op->bufRowCnt;

        AfxAssert(rasMaxRowSiz >= bufRowSiz);
        AfxAssert(rasMaxRowCnt >= bufRowCnt);

        if (cmd->unpack)
        {
            /*
                GL_UNPACK_ALIGNMENT
                Specifies the alignment requirements for the start of each pixel row in memory. 
                The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 4 (word-alignment), and 8 (rows start on double-word boundaries).
            
                GL_UNPACK_SKIP_PIXELS and GL_UNPACK_SKIP_ROWS
                These values are provided as a convenience to the programmer; they provide no functionality that cannot be duplicated by incrementing the pointer passed to glTexImage1D, glTexImage2D, glTexSubImage1D or glTexSubImage2D. 
                Setting GL_UNPACK_SKIP_PIXELS to i is equivalent to incrementing the pointer by in components or indices, where n is the number of components or indices in each pixel. 
                Setting GL_UNPACK_SKIP_ROWS to j is equivalent to incrementing the pointer by jk components or indices, where k is the number of components or indices per row, as just computed in the GL_UNPACK_ROW_LENGTH section.
            
                GL_UNPACK_IMAGE_HEIGHT
                If greater than 0, GL_UNPACK_IMAGE_HEIGHT defines the number of pixels in an image of a three-dimensional texture volume. 
                Where "image" is defined by all pixel sharing the same third dimension index. 
                
                The word component in this description refers to the nonindex values red, green, blue, alpha, and depth. Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.
            
                GL_UNPACK_ROW_LENGTH
                If greater than 0, GL_UNPACK_ROW_LENGTH defines the number of pixels in a row. 
                
                The word component in this description refers to the nonindex values red, green, blue, alpha, and depth. 
                Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.
            
                GL_UNPACK_LSB_FIRST
                If true, bits are ordered within a byte from least significant to most significant; otherwise, the first bit in each byte is the most significant one.
            
                GL_UNPACK_SWAP_BYTES
                If true, byte ordering for multibyte color components, depth components, or stencil indices is reversed. 
                That is, if a four-byte component consists of bytes b0, b1, b2, b3, it is taken from memory as b3, b2, b1, b0 if GL_UNPACK_SWAP_BYTES is true.
                GL_UNPACK_SWAP_BYTES has no effect on the memory order of components within a pixel, only on the order of bytes within components or indices. 
                For example, the three components of a GL_RGB format pixel are always stored with red first, green second, and blue third, regardless of the value of GL_UNPACK_SWAP_BYTES.
            */

            //afxNat rowSiz = rasRowSiz ? rasRowSiz : bufRowSiz;
            //afxNat rowCnt = rasRowCnt ? rasRowCnt : bufRowCnt;
            //afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            
            _SglBindAndSyncBuf(gl, FALSE, GL_PIXEL_UNPACK_BUFFER, 0, cmd->buf, 0, 0, 0, GL_STREAM_COPY);
            _SglBindAndSyncTex(gl, FALSE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

            gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_SKIP_PIXELS, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_SKIP_ROWS, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_SKIP_IMAGES, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_ROW_LENGTH, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_LSB_FIRST, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_UNPACK_SWAP_BYTES, ); _SglThrowErrorOccuried();

            if (ras->glTarget == GL_TEXTURE_CUBE_MAP)
            {
                for (afxNat j = 0; j < rgn.layerCnt; j++)
                    _SglTexSubImage(gl, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, rgn.lodIdx, rgn.baseLayer, 1, rgn.offset, rgn.whd, ras->glFmt, ras->glType, (const void*)bufOffset);
            }
            else
            {
                _SglTexSubImage(gl, ras->glTarget, rgn.lodIdx, rgn.baseLayer, rgn.layerCnt, rgn.offset, rgn.whd, ras->glFmt, ras->glType, (const void*)bufOffset);
            }

            _SglBindAndSyncTex(gl, FALSE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);
            _SglBindAndSyncBuf(gl, FALSE, GL_PIXEL_UNPACK_BUFFER, 0, NIL, 0, 0, 0, GL_STREAM_COPY);
        }
        else
        {
            /*
                GL_PACK_ALIGNMENT
                Specifies the alignment requirements for the start of each pixel row in memory. 
                The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 4 (word-alignment), and 8 (rows start on double-word boundaries).
            
                GL_PACK_SKIP_PIXELS and GL_PACK_SKIP_ROWS            
                These values are provided as a convenience to the programmer; they provide no functionality that cannot be duplicated simply by incrementing the pointer passed to glReadPixels. 
                Setting GL_PACK_SKIP_PIXELS to i is equivalent to incrementing the pointer by in components or indices, where n is the number of components or indices in each pixel. 
                Setting GL_PACK_SKIP_ROWS to j is equivalent to incrementing the pointer by jm components or indices, where m is the number of components or indices per row, as just computed in the GL_PACK_ROW_LENGTH section. 
                Setting GL_PACK_SKIP_IMAGES to k is equivalent to incrementing the pointer by kp, where p is the number of components or indices per image, as computed in the GL_PACK_IMAGE_HEIGHT section.

                GL_PACK_IMAGE_HEIGHT
                If greater than 0, GL_PACK_IMAGE_HEIGHT defines the number of pixels in an image three-dimensional texture volume, where "image" is defined by all pixels sharing the same third dimension index.

                The word component in this description refers to the nonindex values red, green, blue, alpha, and depth.
                Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.
            
                GL_PACK_ROW_LENGTH
                If greater than 0, GL_PACK_ROW_LENGTH defines the number of pixels in a row.

                The word component in this description refers to the nonindex values red, green, blue, alpha, and depth. 
                Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.
            
                GL_PACK_LSB_FIRST
                If true, bits are ordered within a byte from least significant to most significant; otherwise, the first bit in each byte is the most significant one.
            
                GL_PACK_SWAP_BYTES
                If true, byte ordering for multibyte color components, depth components, or stencil indices is reversed. 
                That is, if a four-byte component consists of bytes b0, b1, b2, b3, it is stored in memory as b3, b2, b1, b0 if GL_PACK_SWAP_BYTES is true. 
                GL_PACK_SWAP_BYTES has no effect on the memory order of components within a pixel, only on the order of bytes within components or indices. 
                For example, the three components of a GL_RGB format pixel are always stored with red first, green second, and blue third, regardless of the value of GL_PACK_SWAP_BYTES.
            */

            //afxNat rowSiz = dstRowSiz ? dstRowSiz : srcRowSiz;
            //afxNat rowCnt = dstRowCnt ? dstRowCnt : srcRowCnt;
            //afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            //AfxAssert(rgnSiz >= dataSiz);

            _SglBindAndSyncBuf(gl, FALSE, GL_PIXEL_PACK_BUFFER, 0, cmd->buf, 0, 0, 0, GL_STREAM_COPY);
            _SglBindAndSyncTex(gl, FALSE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

            gl->PixelStorei(GL_PACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_SKIP_PIXELS, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_SKIP_ROWS, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_SKIP_IMAGES, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_ROW_LENGTH, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_LSB_FIRST, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_SWAP_BYTES, ); _SglThrowErrorOccuried();

            if (ras->glTarget == GL_TEXTURE_CUBE_MAP)
            {
                for (afxNat j = 0; j < rgn.layerCnt; j++)
                {
                    gl->GetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, rgn.lodIdx, ras->glFmt, ras->glType, (void*)bufOffset); _SglThrowErrorOccuried();
                }
            }
            else
            {
                gl->GetTexImage(ras->glTarget, rgn.lodIdx, ras->glFmt, ras->glType, (void*)bufOffset); _SglThrowErrorOccuried();
            }

            _SglBindAndSyncTex(gl, FALSE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);
            _SglBindAndSyncBuf(gl, FALSE, GL_PIXEL_PACK_BUFFER, 0, NIL, 0, 0, 0, GL_STREAM_COPY);
        }
    }
}

_SGL void _SglDpuCopyRaster(sglDpuIdd* dpu, _sglCmdCopyRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster dst = cmd->dst;
    afxRaster src = cmd->src;
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    afxNat opCnt = cmd->opCnt;
    AfxAssert(opCnt);
    afxRasterCopyOp const* ops = cmd->ops;
    AfxAssert(ops);

    afxWhd srcMaxWhd, dstMaxWhd;
    AfxGetRasterExtent(src, 0, srcMaxWhd);
    AfxGetRasterExtent(dst, 0, dstMaxWhd);
    afxNat srcMaxLayerCnt = AfxCountRasterLayers(src);
    afxNat dstMaxLayerCnt = AfxCountRasterLayers(dst);
    afxNat srcMaxLodCnt = AfxCountRasterLods(src);
    afxNat dstMaxLodCnt = AfxCountRasterLods(dst);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterCopyOp const *op = &ops[i];
        afxRasterRegion dstRgn = op->dst;
        AfxAssertRange(dstMaxLodCnt, dstRgn.lodIdx, 1);
        AfxAssertRange(dstMaxLayerCnt, dstRgn.baseLayer, dstRgn.layerCnt);
        AfxAssertRange(dstMaxWhd[0], dstRgn.offset[0], dstRgn.whd[0]);
        AfxAssertRange(dstMaxWhd[1], dstRgn.offset[1], dstRgn.whd[1]);
        AfxAssertRange(dstMaxWhd[2], dstRgn.offset[2], dstRgn.whd[2]);
        
        afxRasterRegion srcRgn;
        srcRgn.lodIdx = op->srcLodIdx;
        srcRgn.baseLayer = op->srcBaseLayer;
        srcRgn.layerCnt = op->srcLayerCnt;
        AfxCopyWhd(srcRgn.offset, op->srcOffset);
        AfxCopyWhd(srcRgn.whd, dstRgn.whd);
        AfxAssertRange(srcMaxLodCnt, srcRgn.lodIdx, 1);
        AfxAssertRange(srcMaxLayerCnt, srcRgn.baseLayer, srcRgn.layerCnt);
        AfxAssertRange(srcMaxWhd[0], srcRgn.offset[0], srcRgn.whd[0]);
        AfxAssertRange(srcMaxWhd[1], srcRgn.offset[1], srcRgn.whd[1]);
        AfxAssertRange(srcMaxWhd[2], srcRgn.offset[2], srcRgn.whd[2]);

        //_SglBindAndSyncTex(gl, 79, ras, );
        //gl->CopyTexImage2D();
        //gl->CopyTexSubImage2D();
        //gl->CopyTextureSubImage2D(, );

        afxNat dstRgnSiz;
        afxNat dstRowSiz;
        void *dstData = AfxOpenRasterRegion(dst, &dstRgn, AFX_TEX_OPEN_W, &dstRgnSiz, &dstRowSiz);

        if (!dstData) AfxThrowError();
        else
        {
            afxNat srcRowSiz;
            afxNat srcRgnSiz;
            void *srcData = AfxOpenRasterRegion(src, &srcRgn, AFX_TEX_OPEN_R, &srcRgnSiz, &srcRowSiz);

            if (!srcData) AfxThrowError();
            else
            {
                afxNat rowCnt = dstRgn.whd[1] * dstRgn.whd[2] * dstRgn.layerCnt;

                AfxCopy(rowCnt, AfxMin(srcRowSiz, dstRowSiz), srcData, dstData);
                
                AfxCloseRasterRegion(src, &srcRgn);
            }
            AfxCloseRasterRegion(dst, &dstRgn);
        }
    }
}

_SGL void _SglDpuSwizzleRaster(sglDpuIdd* dpu, _sglCmdSwizzleRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat rgnCnt = cmd->rgnCnt;
    afxRasterRegion const* rgns = cmd->rgn;
    AfxAssert(rgnCnt);
    afxColorSwizzle a = cmd->a;
    afxColorSwizzle b = cmd->b;

    afxWhd rasWhd;
    AfxGetRasterExtent(ras, 0, rasWhd);
    afxNat rasLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasLodCnt = AfxCountRasterLods(ras);

    for (afxNat j = 0; j < rgnCnt; j++)
    {
        afxRasterRegion const* rgn = &rgns[j];
        AfxAssert(rgn);
        AfxAssert(rasLodCnt > rgn->lodIdx);
        AfxAssertRange(rasLayerCnt, rgn->baseLayer, rgn->layerCnt);
        AfxAssertRange(rasWhd[0], rgn->offset[0], rgn->whd[0]);
        AfxAssertRange(rasWhd[1], rgn->offset[1], rgn->whd[1]);
        AfxAssertRange(rasWhd[2], rgn->offset[2], rgn->whd[2]);

        afxPixelFormat fmt = AfxGetRasterFormat(ras);
        AfxAssert(fmt == afxPixelFormat_BGR8 || fmt == afxPixelFormat_BGRA8 || fmt == afxPixelFormat_RGB8 || fmt == afxPixelFormat_RGBA8);

        afxPixelLayout pfd;
        AfxDescribePixelFormat(fmt, &pfd);
        afxNat pixelSiz = AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE) / AFX_BYTE_SIZE;

        AfxAssert(a < afxColorSwizzle_ZERO);

        afxNat rgnSize;
        afxNat rowLen = 0;
        afxByte *bytemap = AfxOpenRasterRegion(ras, rgn, AFX_TEX_OPEN_RW, &rgnSize, &rowLen);

        // swap the R and B values to get RGB since the bitmap color format is in BGR
        for (afxNat k = 0; k < rgn->whd[0] * rgn->whd[1] * pixelSiz; k += pixelSiz)
        {
            afxByte temp = (b != afxColorSwizzle_ZERO ? (b != afxColorSwizzle_ONE ? bytemap[k + b] : 1) : 0);
            bytemap[k + b] = bytemap[k + a];
            bytemap[k + a] = temp;
        }

        AfxCloseRasterRegion(ras, rgn);
    }
}

_SGL void _SglDpuFlipRaster(sglDpuIdd* dpu, _sglCmdFlipRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat rgnCnt = cmd->rgnCnt;
    afxRasterRegion const* rgns = cmd->rgn;
    afxBool rows = cmd->rows;
    afxBool columns = cmd->columns;
    AfxAssert(rows || columns);
    
    afxWhd rasWhd;
    AfxGetRasterExtent(ras, 0, rasWhd);
    afxNat rasLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasLodCnt = AfxCountRasterLods(ras);

    for (afxNat j = 0; j < rgnCnt; j++)
    {
        afxRasterRegion const* rgn = &rgns[j];
        afxRasterRegion srcRgn = *rgn;
        srcRgn.layerCnt = 1;
        AfxGetRasterExtent(ras, 0, srcRgn.whd);

        afxNat size;
        afxNat rowLen = 0;

        afxNat bpl = AfxMeasureRasterLayer(ras);
        //afxNat bpr = AfxMeasureRasterRow(ras);
        afxNat rowCnt = (rasWhd[1] >> 1);
        afxByte tmp[2048]; // used to be max texture size supported in old days

        for (afxNat layer = 0; layer < srcRgn.layerCnt; ++layer)
        {
            srcRgn.baseLayer = layer;
            afxByte* bytemap = AfxOpenRasterRegion(ras, &srcRgn, AFX_TEX_OPEN_RW, &size, &rowLen);

            for (afxNat row = 0, row2 = rowCnt; row < rowCnt; row++)
            {
                afxByte *first = &bytemap[row * rowLen];
                afxByte *next = &bytemap[--row2 * rowLen];
                // swap row0 with row1
                afxNat bytesLeft = rowLen;

                while (bytesLeft)
                {
                    afxSize bytesCpy = (bytesLeft < sizeof(tmp)) ? bytesLeft : sizeof(tmp);
                    AfxCopy(bytesCpy, 1, first, tmp);
                    AfxCopy(bytesCpy, 1, next, first);
                    AfxCopy(bytesCpy, 1, tmp, next);
                    first += bytesCpy;
                    next += bytesCpy;
                    bytesLeft -= bytesCpy;
                }
            }
            AfxCloseRasterRegion(ras, &srcRgn);
            //bytemap += bpl;
        }
    }

#if 0

    afxNat bpr = AfxMeasureRasterRow(ras);
    AfxAssert(bpr);
    afxByte tmp[2048]; // used to be max texture size supported in old days
    afxByte *bytes = ras->maps;

    afxNat layerCnt = ras->layerCnt;
    afxNat bpl = AfxMeasureRasterLayer(ras);
    AfxAssert(bpl);
    afxNat rowCnt = (ras->whd[1] >> 1);

    for (afxNat layer = 0; layer < layerCnt; ++layer)
    {
        for (afxNat row = 0; row < rowCnt; row++)
        {
            afxByte *first = bytes + (row * bpr);
            afxByte *next = bytes + (ras->whd[1] - row - 1) * bpr;
            // swap row0 with row1
            afxNat bytesLeft = bpr;

            while (bytesLeft)
            {
                afxSize bytesCpy = (bytesLeft < sizeof(tmp)) ? bytesLeft : sizeof(tmp);
                AfxCopy(bytesCpy, 1, first, tmp);
                AfxCopy(bytesCpy, 1, next, first);
                AfxCopy(bytesCpy, 1, tmp, next);
                first += bytesCpy;
                next += bytesCpy;
                bytesLeft -= bytesCpy;
            }
        }
        bytes += bpl;
    }
#endif
}



_SGL void _SglDpuUpdateUniformVector(sglDpuIdd* dpu, _sglCmdUniformVectorEXT const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLuint loc = gl->GetUniformLocation(dpu->activePip->glHandle, cmd->name);
    gl->Uniform4fv(loc, cmd->cnt, cmd->v);
}

_SGL void _SglDpuUpdateUniformMatrix(sglDpuIdd* dpu, _sglCmdUniformMatrixEXT const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLuint loc = gl->GetUniformLocation(dpu->activePip->glHandle, cmd->name);
    gl->UniformMatrix4fv(loc, cmd->cnt, GL_FALSE, cmd->m);
}

// DO WORK

_SGL void SglFlushXformStateChanges(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    
    dpu->activeXformState.primTop = dpu->nextXformState.primTop;

    afxCullMode cullMode = dpu->nextXformState.cullMode;

    if (dpu->activeXformState.cullMode != cullMode)
    {
        /*
            GL_CULL_FACE
            If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
        */

        if (cullMode)
        {
            AfxAssert(!dpu->activeXformState.cullMode);
            gl->Enable(GL_CULL_FACE); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeXformState.cullMode);
            gl->Disable(GL_CULL_FACE); _SglThrowErrorOccuried();
        }

        if (cullMode)
        {
            /*
                glCullFace — specify whether front- or back-facing facets can be culled
                void glCullFace(GLenum mode);

                mode Specifies whether front- or back-facing facets are candidates for culling. Symbolic constants GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK are accepted. The initial value is GL_BACK.

                glCullFace specifies whether front- or back-facing facets are culled (as specified by mode) when facet culling is enabled.
                Facet culling is initially disabled. To enable and disable facet culling, call the glEnable and glDisable commands with the argument GL_CULL_FACE.
                Facets include triangles, quadrilaterals, polygons, and rectangles.

                glFrontFace specifies which of the clockwise and counterclockwise facets are front-facing and back-facing. See glFrontFace.

                If mode is GL_FRONT_AND_BACK, no facets are drawn, but other primitives such as points and lines are drawn.
            */
            gl->CullFace(AfxToGlCullMode(cullMode)); _SglThrowErrorOccuried();
        }
        dpu->activeXformState.cullMode = cullMode;
    }

    afxBool cwFrontFacing = dpu->nextXformState.cwFrontFacing;

    if (dpu->activeXformState.cwFrontFacing != cwFrontFacing)
    {
        /*
            glFrontFace — define front- and back-facing polygons
            void glFrontFace(GLenum mode);

            mode Specifies the orientation of front-facing polygons. GL_CW and GL_CCW are accepted. The initial value is GL_CCW.

            In a scene composed entirely of opaque closed surfaces, back-facing polygons are never visible.
            Eliminating these invisible polygons has the obvious benefit of speeding up the rendering of the image.
            To enable and disable elimination of back-facing polygons, call glEnable and glDisable with argument GL_CULL_FACE.

            The projection of a polygon to window coordinates is said to have clockwise winding if an imaginary object following the path from its first vertex, its second vertex, and so on, to its last vertex, and finally back to its first vertex, moves in a clockwise direction about the interior of the polygon.
            The polygon's winding is said to be counterclockwise if the imaginary object following the same path moves in a counterclockwise direction about the interior of the polygon.
            glFrontFace specifies whether polygons with clockwise winding in window coordinates, or counterclockwise winding in window coordinates, are taken to be front-facing.
            Passing GL_CCW to mode selects counterclockwise polygons as front-facing; GL_CW selects clockwise polygons as front-facing.
            By default, counterclockwise polygons are taken to be front-facing.
        */
        gl->FrontFace(cwFrontFacing ? GL_CW : GL_CCW); _SglThrowErrorOccuried();
        dpu->activeXformState.cwFrontFacing = cwFrontFacing;
    }

    if (dpu->nextViewportUpdMask)
    {
        afxNat cnt = dpu->nextViewportUpdCnt;
        AfxAssert(cnt);

        if (cnt)
        {
            afxMask updMask = dpu->nextViewportUpdMask;

            for (afxNat i = 0; i < SGL_MAX_VIEWPORTS; i++)
            {
                if (AfxTestBitEnabled(updMask, i))
                {
                    if (gl->ViewportArrayv)
                    {
                        GLfloat v[SGL_MAX_VIEWPORTS][4];
                        AfxAssert(SGL_MAX_VIEWPORTS >= cnt);

                        v[0][0] = dpu->nextXformState.vps[i].offset[0],
                        v[0][1] = dpu->nextXformState.vps[i].offset[1],
                        v[0][2] = dpu->nextXformState.vps[i].extent[0],
                        v[0][3] = dpu->nextXformState.vps[i].extent[1];

                        gl->ViewportArrayv(i, 1, &v[0][0]); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        GLint x = (GLint)(dpu->nextXformState.vps[0].offset[0]);
                        GLint y = (GLint)(dpu->nextXformState.vps[0].offset[1]);
                        GLsizei w = (GLsizei)(dpu->nextXformState.vps[0].extent[0]);
                        GLsizei h = (GLsizei)(dpu->nextXformState.vps[0].extent[1]);
                        AfxAssert(i == 0);
                        gl->Viewport(x, y, w, h); _SglThrowErrorOccuried();
                    }
#ifndef _SGL_DBG_IGNORE_DEPTH_RANGE
                    if (gl->DepthRangeArrayv)
                    {
                        GLdouble v[SGL_MAX_VIEWPORTS][2];
                        AfxAssert(SGL_MAX_VIEWPORTS >= cnt);

                        v[0][0] = dpu->nextXformState.vps[i].depth[0],
                        v[0][1] = dpu->nextXformState.vps[i].depth[1];

                        gl->DepthRangeArrayv(0, 1, &v[0][0]); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        GLdouble n = dpu->nextXformState.vps[0].depth[0];
                        GLdouble f = dpu->nextXformState.vps[0].depth[1];

                        AfxAssert(i == 0);
                        gl->DepthRange(n, f); _SglThrowErrorOccuried();
                    }
#endif
                    dpu->activeXformState.vps[i] = dpu->nextXformState.vps[i];
                }
            }
        }
        dpu->nextViewportUpdMask = NIL;
    }


#ifndef _SGL_DBG_IGNORE_PRIM_RESTART

    afxBool primRestartEnabled = dpu->nextXformState.primTop;

    if (dpu->activeXformState.primRestartEnabled != primRestartEnabled)
    {
        /*
            GL_PRIMITIVE_RESTART
            Enables primitive restarting.
            If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the primitive restart index.
            See glPrimitiveRestartIndex.
        */

        if (primRestartEnabled)
        {
            AfxAssert(!dpu->activeXformState.primRestartEnabled);
            gl->Enable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();

            /*
                glPrimitiveRestartIndex — specify the primitive restart index
                void glPrimitiveRestartIndex(GLuint index);
                index Specifies the value to be interpreted as the primitive restart index.

                glPrimitiveRestartIndex specifies a vertex array element that is treated specially when primitive restarting is enabled. This is known as the primitive restart index.

                When one of the Draw* commands transfers a set of generic attribute array elements to the GL, if the index within the vertex arrays corresponding to that set is equal to the primitive restart index, then the GL does not process those elements as a vertex.
                Instead, it is as if the drawing command ended with the immediately preceding transfer, and another drawing command is immediately started with the same parameters, but only transferring the immediately following element through the end of the originally specified elements.

                When either glDrawElementsBaseVertex, glDrawElementsInstancedBaseVertex or glMultiDrawElementsBaseVertex is used, the primitive restart comparison occurs before the basevertex offset is added to the array index.
            */

            //gl->PrimitiveRestartIndex(); _SglThrowErrorOccuried();

            /*
                GL_PRIMITIVE_RESTART_FIXED_INDEX
                Enables primitive restarting with a fixed index.
                If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the fixed primitive index for the specified index type.
                The fixed index is equal to 2^n - 1
                where n is equal to 8 for GL_UNSIGNED_BYTE, 16 for GL_UNSIGNED_SHORT and 32 for GL_UNSIGNED_INT.
            */

            //gl->Enable(GL_PRIMITIVE_RESTART_FIXED_INDEX); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeXformState.primRestartEnabled);
            gl->Disable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
        }
        dpu->activeXformState.primRestartEnabled = primRestartEnabled;
    }
#endif

#ifndef _SGL_DBG_IGNORE_DEPTH_CLAMP

    afxBool depthClampEnabled = dpu->nextXformState.depthClampEnabled;

    if (dpu->activeXformState.depthClampEnabled != depthClampEnabled)
    {
        /*
            GL_DEPTH_CLAMP
            If enabled, the -wc =< zc =< wc plane equation is ignored by view volume clipping (effectively, there is no near or far plane clipping). See glDepthRange.
        */

        if (depthClampEnabled)
        {
            AfxAssert(!dpu->activeXformState.depthClampEnabled);
            gl->Enable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeXformState.depthClampEnabled);
            gl->Disable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
        }
        dpu->activeXformState.depthClampEnabled = depthClampEnabled;
    }
#endif
}

_SGL void SglFlushRasterizationStateChanges(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

#ifndef _SGL_DBG_IGNORE_RASTERIZER_DISCARD

    afxBool rasterizationDisabled = dpu->nextRasterState.rasterizationDisabled;

    if (dpu->activeRasterState.rasterizationDisabled != rasterizationDisabled)
    {
        /*
            GL_RASTERIZER_DISCARD
            If enabled, primitives are discarded after the optional transform feedback stage, but before rasterization.
            Furthermore, when enabled, glClear, glClearBufferData, glClearBufferSubData, glClearTexImage, and glClearTexSubImage are ignored.
        */

        if (rasterizationDisabled)
        {
            gl->Enable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.rasterizationDisabled = rasterizationDisabled;
    }
#endif

    afxFillMode fillMode = dpu->nextRasterState.fillMode;

    if (dpu->activeRasterState.fillMode != fillMode)
    {
        /*
            glPolygonMode — select a polygon rasterization mode.
            void glPolygonMode(	GLenum face, GLenum mode);

            face = Specifies the polygons that mode applies to. Must be GL_FRONT_AND_BACK for front- and back-facing polygons.
            mode = Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL. The initial value is GL_FILL for both front- and back-facing polygons.

            glPolygonMode controls the interpretation of polygons for rasterization.
            face describes which polygons mode applies to: both front and back-facing polygons (GL_FRONT_AND_BACK).
            The polygon mode affects only the final rasterization of polygons.
            In particular, a polygon's vertices are lit and the polygon is clipped and possibly culled before these modes are applied.

            Three modes are defined and can be specified in mode:

            GL_POINT Polygon vertices that are marked as the start of a boundary edge are drawn as points. Point attributes such as GL_POINT_SIZE and GL_POINT_SMOOTH control the rasterization of the points. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
            GL_LINE Boundary edges of the polygon are drawn as line segments. Line attributes such as GL_LINE_WIDTH and GL_LINE_SMOOTH control the rasterization of the lines. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
            GL_FILL The interior of the polygon is filled. Polygon attributes such as GL_POLYGON_SMOOTH control the rasterization of the polygon.

            Vertices are marked as boundary or nonboundary with an edge flag. Edge flags are generated internally by the GL when it decomposes triangle stips and fans.
        */
        gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlFillMode(fillMode)); _SglThrowErrorOccuried();
        dpu->activeRasterState.fillMode = fillMode;
    }

    // DEPTH BIAS
#ifndef _SGL_DBG_IGNORE_DEPTH_BIAS

    afxBool depthBiasEnabled = dpu->nextRasterState.depthBiasEnabled;

    if (dpu->activeRasterState.depthBiasEnabled != depthBiasEnabled)
    {
        /*
            GL_POLYGON_OFFSET_FILL If enabled, and if the polygon is rendered in GL_FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed.
            See glPolygonOffset.

            GL_POLYGON_OFFSET_LINE If enabled, and if the polygon is rendered in GL_LINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed.
            See glPolygonOffset.

            GL_POLYGON_OFFSET_POINT If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, if the polygon is rendered in GL_POINT mode.
            See glPolygonOffset.
        */

        static int const QwadroToGlPolygonModeBasedOffset[] =
        {
            GL_POLYGON_OFFSET_FILL,
            GL_POLYGON_OFFSET_LINE,
            GL_POLYGON_OFFSET_POINT
        };

        if (depthBiasEnabled)
        {
            gl->Enable(QwadroToGlPolygonModeBasedOffset[dpu->activeRasterState.fillMode]); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(QwadroToGlPolygonModeBasedOffset[dpu->activeRasterState.fillMode]); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.depthBiasEnabled = depthBiasEnabled;
    }

    if  (
        (dpu->activeRasterState.depthBiasConstFactor != dpu->nextRasterState.depthBiasConstFactor) ||
        (dpu->activeRasterState.depthBiasSlopeScale != dpu->nextRasterState.depthBiasSlopeScale) ||
        (dpu->activeRasterState.depthBiasClamp != dpu->nextRasterState.depthBiasClamp)
        )
    {
        /*
            glPolygonOffset — set the scale and units used to calculate depth values
            void glPolygonOffset(GLfloat factor, GLfloat units);

            factor Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is 0.
            units Is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0.

            When GL_POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_LINE, or GL_POLYGON_OFFSET_POINT is enabled, each fragment's depth value will be offset after it is interpolated from the depth values of the appropriate vertices.
            The value of the offset is factor×DZ+r×units, where DZ is a measurement of the change in depth relative to the screen area of the polygon, and r is the smallest value that is guaranteed to produce a resolvable offset for a given implementation.
            The offset is added before the depth test is performed and before the value is written into the depth buffer.

            glPolygonOffset is useful for rendering hidden-line images, for applying decals to surfaces, and for rendering solids with highlighted edges.
        */

        if (gl->PolygonOffsetClamp)
        {
            gl->PolygonOffsetClamp(dpu->nextRasterState.depthBiasSlopeScale, dpu->nextRasterState.depthBiasConstFactor, dpu->nextRasterState.depthBiasClamp); _SglThrowErrorOccuried();
        }
        else
        {
            gl->PolygonOffset(dpu->nextRasterState.depthBiasSlopeScale, dpu->nextRasterState.depthBiasConstFactor); _SglThrowErrorOccuried();
        }

        dpu->activeRasterState.depthBiasConstFactor != dpu->nextRasterState.depthBiasConstFactor;
        dpu->activeRasterState.depthBiasSlopeScale != dpu->nextRasterState.depthBiasSlopeScale;
        dpu->activeRasterState.depthBiasClamp != dpu->nextRasterState.depthBiasClamp;
    }
#endif

#ifndef _SGL_DBG_IGNORE_LINE_WIDTH
    /*
        glLineWidth — specify the width of rasterized lines.
        void glLineWidth(GLfloat width);

        width = Specifies the width of rasterized lines. The initial value is 1.

        glLineWidth specifies the rasterized width of both aliased and antialiased lines.
        Using a line width other than 1 has different effects, depending on whether line antialiasing is enabled.
        To enable and disable line antialiasing, call glEnable and glDisable with argument GL_LINE_SMOOTH.
        Line antialiasing is initially disabled.

        If line antialiasing is disabled, the actual width is determined by rounding the supplied width to the nearest integer.
        (If the rounding results in the value 0, it is as if the line width were 1.) If |delta x|>=|delta y|, i pixels are filled in each column that is rasterized, where i is the rounded value of width.
        Otherwise, i pixels are filled in each row that is rasterized.

        If antialiasing is enabled, line rasterization produces a fragment for each pixel square that intersects the region lying within the rectangle having width equal to the current line width, length equal to the actual length of the line, and centered on the mathematical line segment.
        The coverage value for each fragment is the window coordinate area of the intersection of the rectangular region with the corresponding pixel square.
        This value is saved and used in the final rasterization step.

        Not all widths can be supported when line antialiasing is enabled.
        If an unsupported width is requested, the nearest supported width is used.
        Only width 1 is guaranteed to be supported; others depend on the implementation.
        Likewise, there is a range for aliased line widths as well.
        To query the range of supported widths and the size difference between supported widths within the range, call glGet with arguments GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.

        The line width specified by glLineWidth is always returned when GL_LINE_WIDTH is queried.
        Clamping and rounding for aliased and antialiased lines have no effect on the specified value.

        Nonantialiased line width may be clamped to an implementation-dependent maximum.
        Call glGet with GL_ALIASED_LINE_WIDTH_RANGE to determine the maximum width.

        In OpenGL 1.2, the tokens GL_LINE_WIDTH_RANGE and GL_LINE_WIDTH_GRANULARITY were replaced by GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.
        The old names are retained for backward compatibility, but should not be used in new code.
    */

    afxReal lineWidth = dpu->nextRasterState.lineWidth;

    if (dpu->activeRasterState.lineWidth != lineWidth)
    {
        gl->LineWidth(lineWidth); _SglThrowErrorOccuried();
        dpu->activeRasterState.lineWidth = lineWidth;
    }
#endif

    // DEPTH TEST
#ifndef _SGL_DBG_IGNORE_DEPTH_TEST

    afxBool depthTestEnabled = dpu->nextRasterState.depthTestEnabled;

    if (dpu->activeRasterState.depthTestEnabled != depthTestEnabled)
    {
        /*
            GL_DEPTH_TEST
            If enabled, do depth comparisons and update the depth buffer.
            Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
            See glDepthFunc and glDepthRange.
        */

        if (depthTestEnabled)
        {
            AfxAssert(!dpu->activeRasterState.depthTestEnabled);
            gl->Enable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.depthTestEnabled);
            gl->Disable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.depthTestEnabled = depthTestEnabled;
    }

    if (dpu->activeRasterState.depthTestEnabled)
    {
        afxCompareOp depthCompareOp = dpu->nextRasterState.depthCompareOp;

        if (dpu->activeRasterState.depthCompareOp != depthCompareOp)
        {
            /*
                glDepthFunc — specify the value used for depth buffer comparisons
                void glDepthFunc(GLenum func);

                func = Specifies the depth comparison function. Symbolic constants GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS are accepted. The initial value is GL_LESS.

                glDepthFunc specifies the function used to compare each incoming pixel depth value with the depth value present in the depth buffer.
                The comparison is performed only if depth testing is enabled. (See glEnable and glDisable of GL_DEPTH_TEST.)

                func specifies the conditions under which the pixel will be drawn. The comparison functions are as follows:

                GL_NEVER Never passes.
                GL_LESS Passes if the incoming depth value is less than the stored depth value.
                GL_EQUAL Passes if the incoming depth value is equal to the stored depth value.
                GL_LEQUAL Passes if the incoming depth value is less than or equal to the stored depth value.
                GL_GREATER Passes if the incoming depth value is greater than the stored depth value.
                GL_NOTEQUAL Passes if the incoming depth value is not equal to the stored depth value.
                GL_GEQUAL Passes if the incoming depth value is greater than or equal to the stored depth value.
                GL_ALWAYS Always passes.

                The initial value of func is GL_LESS. Initially, depth testing is disabled. If depth testing is disabled or if no depth buffer exists, it is as if the depth test always passes.

                Even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
                In order to unconditionally write to the depth buffer, the depth test should be enabled and set to GL_ALWAYS.
            */

            AfxAssert(depthCompareOp < afxCompareOp_TOTAL);
            gl->DepthFunc(SglToGlCompareOp(depthCompareOp)); _SglThrowErrorOccuried();
            dpu->activeRasterState.depthCompareOp = depthCompareOp;
        }
    }
#endif

#ifndef _SGL_DBG_IGNORE_DEPTH_WRITE
    afxBool depthWriteEnabled = dpu->nextRasterState.depthWriteEnabled;

    if (dpu->activeRasterState.depthWriteEnabled != depthWriteEnabled)
    {
        /*
            glDepthMask — enable or disable writing into the depth buffer.
            void glDepthMask(GLboolean flag);

            flag = Specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

            glDepthMask specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

            Even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
            In order to unconditionally write to the depth buffer, the depth test should be enabled and set to GL_ALWAYS (see glDepthFunc).
        */

        if (depthWriteEnabled)
        {
            AfxAssert(!dpu->activeRasterState.depthWriteEnabled);
            gl->DepthMask(GL_TRUE); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.depthWriteEnabled);
            gl->DepthMask(GL_FALSE); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.depthWriteEnabled = depthWriteEnabled;
    }
#endif

#ifndef _SGL_DBG_IGNORE_DEPTH_BOUNDS_TEST

    afxBool depthBoundsTestEnabled = dpu->nextRasterState.depthBoundsTestEnabled;

    if (dpu->activeRasterState.depthBoundsTestEnabled != depthBoundsTestEnabled)
    {
        /*
            EXT_depth_bounds_test

            The depth bounds test determines whether the depth value (Zpixel) stored at the location given by the incoming fragment's (xw,yw) location lies within the depth bounds range defined by two values.
            These values are set with

            void DepthBoundsEXT(clampd zmin, clampd zmax);

            Each of zmin and zmax are clamped to lie within [0,1] (being of type clampd).  If zmin <= Zpixel <= zmax, then the depth bounds test passes.  Otherwise, the test fails and the fragment is discarded.
            The test is enabled or disabled using Enable or Disable using the constant DEPTH_BOUNDS_TEST_EXT.  When disabled, it is as if the depth bounds test always passes.  If zmin is greater than zmax, then the error INVALID_VALUE is generated.
            The state required consists of two floating-point values and a bit indicating whether the test is enabled or disabled.  In the initial state, zmin and zmax are set to 0.0 and 1.0 respectively; and the depth bounds test is disabled.

            If there is no depth buffer, it is as if the depth bounds test always passes.
        */

        if (depthBoundsTestEnabled)
        {
            gl->Enable(DEPTH_BOUNDS_TEST_EXT); _SglThrowErrorOccuried();
            gl->DepthBoundsEXT(dpu->state.depthBounds[0], dpu->state.depthBounds[1]); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(DEPTH_BOUNDS_TEST_EXT); _SglThrowErrorOccuried();
        }
    }
#endif

    // STENCIL TEST

#ifndef _SGL_DBG_IGNORE_STENCIL_TEST

    afxBool stencilTestEnabled = dpu->nextRasterState.stencilTestEnabled;

    if (dpu->activeRasterState.stencilTestEnabled != stencilTestEnabled)
    {
        /*
            GL_STENCIL_TEST
            If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
        */

        if (stencilTestEnabled)
        {
            AfxAssert(!dpu->activeRasterState.stencilTestEnabled);
            gl->Enable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.stencilTestEnabled);
            gl->Disable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.stencilTestEnabled = stencilTestEnabled;
    }

    // STENCIL FUNC

    {
        /*
            glStencilFuncSeparate — set front and/or back function and reference value for stencil testing
            void glStencilFuncSeparate(	GLenum face, GLenum func, GLint ref, GLuint mask);

            face = Specifies whether front and/or back stencil state is updated. Three symbolic constants are valid: GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
            func = Specifies the test function. Eight symbolic constants are valid: GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, and GL_ALWAYS. The initial value is GL_ALWAYS.
            ref = Specifies the reference value for the stencil test. ref is clamped to the range [0, 2^n - 1], where n is the number of bitplanes in the stencil buffer. The initial value is 0.
            mask = Specifies a mask that is ANDed with both the reference value and the stored stencil value when the test is done. The initial value is all 1's.

            Stenciling, like depth-buffering, enables and disables drawing on a per-pixel basis.
            You draw into the stencil planes using GL drawing primitives, then render geometry and images, using the stencil planes to mask out portions of the screen.
            Stenciling is typically used in multipass rendering algorithms to achieve special effects, such as decals, outlining, and constructive solid geometry rendering.

            The stencil test conditionally eliminates a pixel based on the outcome of a comparison between the reference value and the value in the stencil buffer.
            To enable and disable the test, call glEnable and glDisable with argument GL_STENCIL_TEST.
            To specify actions based on the outcome of the stencil test, call glStencilOp or glStencilOpSeparate.

            There can be two separate sets of func, ref, and mask parameters; one affects back-facing polygons, and the other affects front-facing polygons as well as other non-polygon primitives.
            glStencilFunc sets both front and back stencil state to the same values, as if glStencilFuncSeparate were called with face set to GL_FRONT_AND_BACK.

            func is a symbolic constant that determines the stencil comparison function. It accepts one of eight values, shown in the following list.
            ref is an integer reference value that is used in the stencil comparison. It is clamped to the range [0, 2^n - 1], where n is the number of bitplanes in the stencil buffer.
            mask is bitwise ANDed with both the reference value and the stored stencil value, with the ANDed values participating in the comparison.

            If stencil represents the value stored in the corresponding stencil buffer location, the following list shows the effect of each comparison function that can be specified by func.
            Only if the comparison succeeds is the pixel passed through to the next stage in the rasterization process (see glStencilOp).
            All tests treat stencil values as unsigned integers in the range [0,2^n - 1], where n is the number of bitplanes in the stencil buffer.

            The following values are accepted by func:

            GL_NEVER Always fails.
            GL_LESS Passes if ( ref & mask ) < ( stencil & mask ).
            GL_LEQUAL Passes if ( ref & mask ) <= ( stencil & mask ).
            GL_GREATER Passes if ( ref & mask ) > ( stencil & mask ).
            GL_GEQUAL Passes if ( ref & mask ) >= ( stencil & mask ).
            GL_EQUAL Passes if ( ref & mask ) = ( stencil & mask ).
            GL_NOTEQUAL Passes if ( ref & mask ) != ( stencil & mask ).
            GL_ALWAYS Always passes.

            Initially, the stencil test is disabled.
            If there is no stencil buffer, no stencil modification can occur and it is as if the stencil test always passes.
        */

        afxNat32 compareMask = dpu->nextRasterState.stencilFront.compareMask;
        afxCompareOp compareOp = dpu->nextRasterState.stencilFront.compareOp;
        afxNat32 reference = dpu->nextRasterState.stencilFront.reference;

        if (
            (dpu->activeRasterState.stencilFront.compareMask != compareMask) ||
            (dpu->activeRasterState.stencilFront.compareOp != compareOp) ||
            (dpu->activeRasterState.stencilFront.reference != reference)
            )
        {
            gl->StencilFuncSeparate(GL_FRONT, SglToGlCompareOp(compareOp), reference, compareMask); _SglThrowErrorOccuried();

            dpu->activeRasterState.stencilFront.compareMask = compareMask;
            dpu->activeRasterState.stencilFront.compareOp = compareOp;
            dpu->activeRasterState.stencilFront.reference = reference;
        }

        compareMask = dpu->nextRasterState.stencilBack.compareMask;
        compareOp = dpu->nextRasterState.stencilBack.compareOp;
        reference = dpu->nextRasterState.stencilBack.reference;

        if (
            (dpu->activeRasterState.stencilBack.compareMask != compareMask) ||
            (dpu->activeRasterState.stencilBack.compareOp != compareOp) ||
            (dpu->activeRasterState.stencilBack.reference != reference)
            )
        {
            gl->StencilFuncSeparate(GL_BACK, SglToGlCompareOp(compareOp), reference, compareMask); _SglThrowErrorOccuried();

            dpu->activeRasterState.stencilBack.compareMask = compareMask;
            dpu->activeRasterState.stencilBack.compareOp = compareOp;
            dpu->activeRasterState.stencilBack.reference = reference;
        }
    }

    // STENCIL WRITE

    {
        /*
            glStencilMaskSeparate — control the front and/or back writing of individual bits in the stencil planes.
            void glStencilMaskSeparate(	GLenum face, GLuint mask);

            face = Specifies whether the front and/or back stencil writemask is updated. Three symbolic constants are valid: GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
            mask = Specifies a bit mask to enable and disable writing of individual bits in the stencil planes. Initially, the mask is all 1's.

            glStencilMaskSeparate controls the writing of individual bits in the stencil planes.
            The least significant n bits of mask, where n is the number of bits in the stencil buffer, specify a mask.
            Where a 1 appears in the mask, it's possible to write to the corresponding bit in the stencil buffer.
            Where a 0 appears, the corresponding bit is write-protected.
            Initially, all bits are enabled for writing.

            There can be two separate mask writemasks; one affects back-facing polygons, and the other affects front-facing polygons as well as other non-polygon primitives.
            glStencilMask sets both front and back stencil writemasks to the same values, as if glStencilMaskSeparate were called with face set to GL_FRONT_AND_BACK.
        */

        static GLenum const faces[] =
        {
            GL_INVALID_ENUM,
            GL_FRONT,
            GL_BACK,
            GL_FRONT_AND_BACK
        };

        afxMask facesAffected;
        afxNat32 writeMask = dpu->nextRasterState.stencilFront.writeMask;
        
        if (dpu->activeRasterState.stencilFront.writeMask != writeMask)
        {
            gl->StencilMaskSeparate(GL_FRONT, writeMask); _SglThrowErrorOccuried();
            dpu->activeRasterState.stencilFront.writeMask = writeMask;
        }

        writeMask = dpu->nextRasterState.stencilBack.writeMask;

        if (dpu->activeRasterState.stencilBack.writeMask != writeMask)
        {
            gl->StencilMaskSeparate(GL_BACK, writeMask); _SglThrowErrorOccuried();
            dpu->activeRasterState.stencilBack.writeMask = writeMask;
        }
    }
#endif


#ifndef _SGL_DBG_IGNORE_MULTISAMPLING
    
    afxBool msEnabled = dpu->nextRasterState.msEnabled;

    if (dpu->activeRasterState.msEnabled != msEnabled)
    {
        /*
            GL_MULTISAMPLE
            If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
        */

        if (msEnabled)
        {
            AfxAssert(!dpu->activeRasterState.msEnabled);
            gl->Enable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.msEnabled);
            gl->Disable(GL_MULTISAMPLE); _SglThrowErrorOccuried();

        }
        dpu->activeRasterState.msEnabled = msEnabled;
    }

    if (msEnabled)
    {
        afxNat sampleCnt = dpu->nextRasterState.sampleCnt;

        if (dpu->activeRasterState.sampleCnt != sampleCnt)
        {
            /*
                GL_SAMPLE_MASK
                If enabled, the sample coverage mask generated for a fragment during rasterization will be ANDed with the value of GL_SAMPLE_MASK_VALUE before shading occurs. See glSampleMaski.
            */

            if (sampleCnt)
            {
                AfxAssert(!dpu->activeRasterState.sampleCnt);
                gl->Enable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
                
                for (afxNat i = 0; i < sampleCnt; i++)
                {
                    dpu->activeRasterState.sampleMasks[i] = dpu->nextRasterState.sampleMasks[i];
                    gl->SampleMaski(i, dpu->nextRasterState.sampleMasks[i]); _SglThrowErrorOccuried();
                }
            }
            else
            {
                AfxAssert(dpu->activeRasterState.sampleCnt);
                gl->Disable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
            }
            dpu->activeRasterState.sampleCnt = sampleCnt;
        }

        afxReal minSampleShadingValue = dpu->nextRasterState.minSampleShadingValue;
        afxBool sampleShadingEnabled = dpu->nextRasterState.sampleShadingEnabled;

        if (sampleShadingEnabled != dpu->activeRasterState.sampleShadingEnabled)
        {
            /*
                GL_SAMPLE_SHADING
                If enabled, the active fragment shader is run once for each covered sample, or at fraction of this rate as determined by the current value of GL_MIN_SAMPLE_SHADING_VALUE. See glMinSampleShading.
            */

            if (sampleShadingEnabled)
            {
                AfxAssert(dpu->activeRasterState.sampleShadingEnabled);
                gl->Enable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(!dpu->activeRasterState.sampleShadingEnabled);
                gl->Disable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
            }

            if (gl->MinSampleShading) // gl v4.0
            {
                gl->MinSampleShading(minSampleShadingValue); _SglThrowErrorOccuried();
            }

            dpu->activeRasterState.sampleShadingEnabled = sampleShadingEnabled;
            dpu->activeRasterState.minSampleShadingValue = minSampleShadingValue;
        }

        afxBool alphaToOneEnabled = dpu->nextRasterState.alphaToOneEnabled;

        if (dpu->activeRasterState.alphaToOneEnabled != alphaToOneEnabled)
        {
            /*
                GL_SAMPLE_ALPHA_TO_ONE
                If enabled, each sample alpha value is replaced by the maximum representable alpha value.
            */

            if (alphaToOneEnabled)
            {
                AfxAssert(!dpu->activeRasterState.alphaToOneEnabled);
                gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(dpu->activeRasterState.alphaToOneEnabled);
                gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
            }
            dpu->activeRasterState.alphaToOneEnabled = alphaToOneEnabled;
        }

        afxBool alphaToCoverageEnabled = dpu->nextRasterState.alphaToCoverageEnabled;

        if (dpu->activeRasterState.alphaToCoverageEnabled != alphaToCoverageEnabled)
        {
            /*
                GL_SAMPLE_ALPHA_TO_COVERAGE
                If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location.
                The temporary coverage value is then ANDed with the fragment coverage value.
            */

            if (alphaToCoverageEnabled)
            {
                AfxAssert(!dpu->activeRasterState.alphaToCoverageEnabled);
                gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(dpu->activeRasterState.alphaToCoverageEnabled);
                gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
            }
            dpu->activeRasterState.alphaToCoverageEnabled = alphaToCoverageEnabled;
        }

    }
#endif

    // SCISSOR
#ifndef _SGL_DBG_IGNORE_SCISSOR_TEST

    if (dpu->nextScissorUpdMask)
    {
        afxNat cnt = dpu->nextScissorUpdCnt;
        afxBool enabled = dpu->scissorTestEnabled;

        if (!cnt)
        {
            if (enabled)
            {
                gl->Disable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
                dpu->scissorTestEnabled = TRUE;
            }
        }
        else
        {
            if (!enabled)
            {
                gl->Enable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
                dpu->scissorTestEnabled = TRUE;
            }

            afxMask updMask = dpu->nextScissorUpdMask;

            for (afxNat i = 0; i < SGL_MAX_VIEWPORTS; i++)
            {
                AfxAssertRange(SGL_MAX_VIEWPORTS, i, 1);

                if (AfxTestBitEnabled(updMask, i))
                {
                    if (gl->ScissorArrayv)
                    {
                        GLint v[SGL_MAX_VIEWPORTS][4];
                        AfxAssert(SGL_MAX_VIEWPORTS >= cnt);
                        v[0][0] = dpu->nextRasterState.scisRects[i].offset[0],
                        v[0][1] = dpu->nextRasterState.scisRects[i].offset[1],
                        v[0][2] = dpu->nextRasterState.scisRects[i].extent[0],
                        v[0][3] = dpu->nextRasterState.scisRects[i].extent[1];

                        gl->ScissorArrayv(i, 1, &v[0][0]); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        GLint x = (GLint)(dpu->nextRasterState.scisRects[0].offset[0]);
                        GLint y = (GLint)(dpu->nextRasterState.scisRects[0].offset[1]);
                        GLsizei w = (GLsizei)(dpu->nextRasterState.scisRects[0].extent[0]);
                        GLsizei h = (GLsizei)(dpu->nextRasterState.scisRects[0].extent[1]);

                        AfxAssert(i == 0);
                        gl->Scissor(x, y, w, h); _SglThrowErrorOccuried();
                    }
                    AfxRectCopy(&(dpu->activeRasterState.scisRects[i]), &dpu->nextRasterState.scisRects[i]);
                }
            }
        }
        dpu->nextScissorUpdMask = NIL;
    }

#endif

    // BLEND CONSTANT
#ifndef _SGL_DBG_IGNORE_BLEND

    if (dpu->nextBlendConstUpd)
    {
        gl->BlendColor(dpu->activeRasterState.blendConstants[0], dpu->activeRasterState.blendConstants[1], dpu->activeRasterState.blendConstants[2], dpu->activeRasterState.blendConstants[3]);
        AfxCopyColor(dpu->activeRasterState.blendConstants, dpu->nextRasterState.blendConstants);
        dpu->nextBlendConstUpd = NIL;
    }
#endif

    afxBool blendNoUsed = TRUE;

    afxNat outCnt = dpu->nextRasterState.outCnt;

    for (afxNat i = 0; i < outCnt; i++)
    {
        afxColorOutputChannel const*ch = &dpu->nextRasterState.outs[i];
        //AfxGetColorOutputChannels(ras, 0, outCnt, ch);

#ifndef _SGL_DBG_IGNORE_BLEND
        if (ch->blendEnabled)
        {
            if (ch->blendEnabled != dpu->activeRasterState.outs[i].blendEnabled)
            {
                if (ch->blendEnabled)
                {
                    if (blendNoUsed)
                    {
                        blendNoUsed = FALSE;

                        /*
                            GL_BLEND
                            If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
                        */

                        if (!dpu->state.anyBlendEnabled)
                        {
                            gl->Enable(GL_BLEND); _SglThrowErrorOccuried();
                            dpu->state.anyBlendEnabled = TRUE;
                        }
                        /*
                            glBlendColor — set the blend color.

                            The GL_BLEND_COLOR may be used to calculate the source and destination blending factors. The color components are clamped to the range [0,1] before being stored.
                            See glBlendFunc for a complete description of the blending operations. Initially the GL_BLEND_COLOR is set to (0, 0, 0, 0).
                        */
                        if (AfxGetColorBlendConstants(ras, dpu->state.blendConstants))
                        {
                            gl->BlendColor(dpu->state.blendConstants[0], dpu->state.blendConstants[1], dpu->state.blendConstants[2], dpu->state.blendConstants[3]); _SglThrowErrorOccuried();
                        }
                    }

                    if ((dpu->state.outs[i].blendConfig.rgbBlendOp != ch->blendConfig.rgbBlendOp) || (dpu->state.outs[i].blendConfig.aBlendOp != ch->blendConfig.aBlendOp))
                    {
                        /*
                            glBlendEquationSeparate — set the RGB blend equation and the alpha blend equation separately.

                            The blend equations determines how a new pixel (the source color) is combined with a pixel already in the framebuffer (the destination color).
                            These functions specify one blend equation for the RGB-color components and one blend equation for the alpha component.
                            glBlendEquationSeparatei specifies the blend equations for a single draw buffer whereas glBlendEquationSeparate sets the blend equations for all draw buffers.

                            The blend equations use the source and destination blend factors specified by either glBlendFunc or glBlendFuncSeparate.
                            See glBlendFunc or glBlendFuncSeparate for a description of the various blend factors.
                        */
                        gl->BlendEquationSeparate(AfxToGlBlendOp(ch->blendConfig.rgbBlendOp), AfxToGlBlendOp(ch->blendConfig.aBlendOp)); _SglThrowErrorOccuried();
                        dpu->state.outs[i].blendConfig.rgbBlendOp = ch->blendConfig.rgbBlendOp;
                        dpu->state.outs[i].blendConfig.aBlendOp = ch->blendConfig.aBlendOp;
                    }

                    if ((dpu->state.outs[i].blendConfig.rgbSrcFactor != ch->blendConfig.rgbSrcFactor) || (dpu->state.outs[i].blendConfig.rgbDstFactor != ch->blendConfig.rgbDstFactor) || (dpu->state.outs[i].blendConfig.aSrcFactor != ch->blendConfig.aSrcFactor) || (dpu->state.outs[i].blendConfig.aDstFactor != ch->blendConfig.aDstFactor))
                    {
                        /*
                            glBlendFuncSeparate — specify pixel arithmetic for RGB and alpha components separately.

                            Pixels can be drawn using a function that blends the incoming (source) RGBA values with the RGBA values that are already in the frame buffer (the destination values).
                            Blending is initially disabled. Use glEnable and glDisable with argument GL_BLEND to enable and disable blending.

                            glBlendFuncSeparate defines the operation of blending for all draw buffers when it is enabled.
                            glBlendFuncSeparatei defines the operation of blending for a single draw buffer specified by buf when enabled for that draw buffer.
                            srcRGB specifies which method is used to scale the source RGB-color components. dstRGB specifies which method is used to scale the destination RGB-color components.
                            Likewise, srcAlpha specifies which method is used to scale the source alpha color component, and dstAlpha specifies which method is used to scale the destination alpha component.
                            The possible methods are described in the following table. Each method defines four scale factors, one each for red, green, blue, and alpha.
                        */
                        gl->BlendFuncSeparate(AfxToGlBlendFactor(ch->blendConfig.rgbSrcFactor), AfxToGlBlendFactor(ch->blendConfig.rgbDstFactor), AfxToGlBlendFactor(ch->blendConfig.aSrcFactor), AfxToGlBlendFactor(ch->blendConfig.aDstFactor)); _SglThrowErrorOccuried();
                        dpu->state.outs[i].blendConfig.rgbSrcFactor = ch->blendConfig.rgbSrcFactor;
                        dpu->state.outs[i].blendConfig.rgbDstFactor = ch->blendConfig.rgbDstFactor;
                        dpu->state.outs[i].blendConfig.aSrcFactor = ch->blendConfig.aSrcFactor;
                        dpu->state.outs[i].blendConfig.aDstFactor = ch->blendConfig.aDstFactor;
                    }
                }
                dpu->activeRasterState.outs[i].blendEnabled = ch->blendEnabled;
            }

        }
#endif

#ifndef _SGL_DBG_IGNORE_COLOR_MASK

        if (ch->discardMask != dpu->nextRasterState.outs[i].discardMask)
        {
            /*
                glColorMask, glColorMaski — enable and disable writing of frame buffer color components.

                glColorMask and glColorMaski specify whether the individual color components in the frame buffer can or cannot be written.
                glColorMaski sets the mask for a specific draw buffer, whereas glColorMask sets the mask for all draw buffers.
                If red is GL_FALSE, for example, no change is made to the red component of any pixel in any of the color buffers, regardless of the drawing operation attempted.

                Changes to individual bits of components cannot be controlled. Rather, changes are either enabled or disabled for entire color components.
            */
            gl->ColorMaski(i, !(ch->discardMask & afxRgbaMask_R), !(ch->discardMask & afxRgbaMask_G), !(ch->discardMask & afxRgbaMask_B), !(ch->discardMask & afxRgbaMask_A)); _SglThrowErrorOccuried();
            dpu->nextRasterState.outs[i].discardMask = ch->discardMask;
        }
#endif
    }

#ifndef _SGL_DBG_IGNORE_BLEND
    if (blendNoUsed)
    {
        if (dpu->activeRasterState.anyBlendEnabled)
        {
            gl->Disable(GL_BLEND); _SglThrowErrorOccuried();
            dpu->activeRasterState.anyBlendEnabled = FALSE;
        }
    }
#endif

#ifndef _SGL_DBG_IGNORE_LOGICAL_OP

    afxLogicOp logicOp = dpu->nextRasterState.logicOp;
    afxBool logicOpEnabled = dpu->nextRasterState.logicOpEnabled;

    if (dpu->activeRasterState.logicOpEnabled != logicOpEnabled)
    {
        if (logicOpEnabled)
        {
            AfxAssert(!dpu->activeRasterState.logicOpEnabled);
            gl->Enable(GL_COLOR_LOGIC_OP); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeRasterState.logicOpEnabled);
            gl->Disable(GL_COLOR_LOGIC_OP); _SglThrowErrorOccuried();
        }
        dpu->activeRasterState.logicOpEnabled = logicOpEnabled;
    }

    if (logicOpEnabled)
    {
        if (dpu->activeRasterState.logicOp != logicOp)
        {
            /*
                glLogicOp — specify a logical pixel operation for rendering.

                glLogicOp specifies a logical operation that, when enabled, is applied between the incoming RGBA color and the RGBA color at the corresponding location in the frame buffer.
                To enable or disable the logical operation, call glEnable and glDisable using the symbolic constant GL_COLOR_LOGIC_OP. The initial value is disabled.

                Th opcode is a symbolic constant. The logical operation is applied independently to each bit pair of the source and destination colors.

                When more than one RGBA color buffer is enabled for drawing, logical operations are performed separately for each enabled buffer, using for the destination value the contents of that buffer (see glDrawBuffer).

                Logic operations have no effect on floating point draw buffers.
                However, if GL_COLOR_LOGIC_OP is enabled, blending is still disabled in this case.
            */
            gl->LogicOp(AfxToGlLogicOp(logicOp)); _SglThrowErrorOccuried();
            dpu->activeRasterState.logicOp = logicOp;
        }
    }
#endif
}

_SGL void _SglFlushResourcingStateChanges(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    // BIND RESOURCES (TEXTURES, SAMPLERS AND BUFFERS)

    for (afxNat i = 0; i < dpu->activePip->base.wiringCnt; i++)
    {
        afxNat setIdx = dpu->activePip->base.wiring[i].set;
        afxMask updMask = dpu->nextResBindUpdMask[setIdx];

        afxBindSchema lego = dpu->activePip->base.wiring->legt;
        AfxAssert(lego);
        AfxAssertObjects(1, &lego, afxFcc_LEGO);

        for (afxNat j = 0; j < lego->base.entryCnt; j++)
        {
            afxBindSchemaEntry const *entry = &lego->base.entries[j];
            AfxAssert(entry->type);
            afxNat binding = (setIdx * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding;
            afxBool reqUpd = FALSE, reqUpd2 = FALSE;
            GLuint glHandle = 0, glHandle2 = 0;
                    
            switch (entry->type)
            {
            case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
            {
                afxNat offset = dpu->nextResBind[setIdx][j].offset;
                afxNat range = dpu->nextResBind[setIdx][j].range;
                afxBuffer buf = dpu->nextResBind[setIdx][j].buf;
                
                if (updMask & AfxGetBitOffset(j))
                {
                    if (dpu->activeResBind[setIdx][j].buf != buf)
                    {
                        dpu->activeResBind[setIdx][j].buf = buf;
                        reqUpd = TRUE;
                    }

                    if (dpu->activeResBind[setIdx][j].offset != offset)
                    {
                        dpu->activeResBind[setIdx][j].offset = offset;
                        reqUpd = TRUE;
                    }

                    if (dpu->activeResBind[setIdx][j].range != range)
                    {
                        dpu->activeResBind[setIdx][j].range = range;
                        reqUpd = TRUE;
                    }
                }

                _SglBindAndSyncBuf(gl, !reqUpd, GL_UNIFORM_BUFFER, binding, buf, offset, range, 0, /*buf->glUsage ? buf->glUsage : */GL_DYNAMIC_DRAW);
                break;
            }
            case AFX_SHD_RES_TYPE_SAMPLER:
            case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
            case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
            {
                afxSampler samp = dpu->nextResBind[setIdx][j].smp;
                afxRaster ras = dpu->nextResBind[setIdx][j].ras;

                if (updMask & AfxGetBitOffset(j))
                {
                    if (dpu->activeResBind[setIdx][j].ras != ras)
                    {
                        dpu->activeResBind[setIdx][j].ras = ras;
                        reqUpd = TRUE;
                    }

                    if (dpu->activeResBind[setIdx][j].smp != samp)
                    {
                        dpu->activeResBind[setIdx][j].smp = samp;
                        reqUpd2 = TRUE;
                    }
                }

                if (entry->type == AFX_SHD_RES_TYPE_SAMPLED_IMAGE || entry->type == AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER)
                {
                    _SglBindAndSyncTex(gl, !reqUpd, binding, ras);
                }

                if (entry->type == AFX_SHD_RES_TYPE_SAMPLER || entry->type == AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER)
                {
                    _SglDpuBindAndSyncSamp(gl, !reqUpd2, binding, samp);
                }
                break;
            }
            default:
            {
                AfxError("");
            }
            }
        }
        dpu->nextResBindUpdMask[setIdx] = NIL;
    }
}

_SGL void _SglFlushVertexInputStateChanges(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxBool vaoBoundHere = FALSE;
#if 0
    if (dpu->state.vtxInAttribUpdMask)
    {
        afxBool vaoGerated = FALSE;
        afxNat streamCnt = dpu->state.vertexInput.streamCnt;

        for (afxNat attrIdx = 0; attrIdx < SGL_MAX_VERTEX_ATTRIBS/* dpu->state.vertexInput.attrCnt*/; attrIdx++)
        {
            if (dpu->state.vtxInAttribUpdMask & AfxGetBitOffset(attrIdx))
            {
                if (!vaoGerated)
                {
                    vaoGerated = TRUE;
                    dpu->activeVaoIdx = (dpu->activeVaoIdx + 1) % _SGL_MAX_VAO_PER_TIME;
                    vaoBoundHere = TRUE;
#if !0
                    if (dpu->vao[dpu->activeVaoIdx])
                    {
                        gl->DeleteVertexArrays(1, &(dpu->vao[dpu->activeVaoIdx])); _SglThrowErrorOccuried();
                    }
                    gl->GenVertexArrays(1, &(dpu->vao[dpu->activeVaoIdx])); _SglThrowErrorOccuried();
                    gl->BindVertexArray(dpu->vao[dpu->activeVaoIdx]); _SglThrowErrorOccuried();
#endif
                }

                afxNat location = dpu->state.vertexInput.attrs[attrIdx].location;
                AfxAssert(location == attrIdx);
                afxVertexFormat fmt = dpu->state.vertexInput.attrs[attrIdx].fmt;
                afxNat32 offset = dpu->state.vertexInput.attrs[attrIdx].offset;
                afxNat srcIdx = dpu->state.vertexInput.attrs[attrIdx].srcIdx;

                GLint glsiz;
                GLenum gltype;
                GLuint glStride;
                AfxToGlVertexFormat(fmt, &glsiz, &gltype, &glStride);

                AfxAssert(16 > location);  // max vertex attrib
                gl->EnableVertexAttribArray(location); _SglThrowErrorOccuried();
                AfxAssert(gl->BindVertexBuffer);
                gl->VertexAttribFormat(location, glsiz, gltype, FALSE, offset); _SglThrowErrorOccuried();
                //afxNat srcIdx = streamIdx;// dpu->state.vertexInput.streams[streamIdx].srcIdx;
                //AfxAssertRange(_SGL_MAX_VBO_PER_BIND, srcIdx, 1);
                AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, srcIdx, 1);
                gl->VertexAttribBinding(location, srcIdx); _SglThrowErrorOccuried();

                // TODO mover VAO para Pipeline. A indireção de stream/source supostamente permite
            }
        }
        dpu->state.vtxInAttribUpdMask = NIL;
    }
#endif
    if (dpu->nextVinBindingsMask)
    {
        afxMask updMask = dpu->nextVinBindingsMask;
        afxNat streamCnt = dpu->nextVinBindingsCnt;
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, 0, streamCnt);

        for (afxNat streamIdx = 0; streamIdx < SGL_MAX_VERTEX_ATTRIB_BINDINGS; streamIdx++)
        {
            AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, streamIdx, 1);

            if (dpu->activeVin->base.streamCnt > streamIdx)
            {

                afxBool updReq = FALSE, updReq2 = FALSE;
#if 0
                if (!vaoBoundHere)
                {
                    vaoBoundHere = TRUE;
                    gl->BindVertexArray(dpu->vao[dpu->activeVaoIdx]); _SglThrowErrorOccuried();
                }
#endif
#if 0
                afxBool instance = dpu->nextVertexInput.streams[streamIdx].instance;
                afxNat32 stride = dpu->nextVertexInput.streams[streamIdx].stride;
                afxNat srcIdx = dpu->nextVertexInput.streams[streamIdx].srcIdx;
#endif       
                afxNat32 range = dpu->nextVinBindings.sources[streamIdx].range;
                afxNat32 offset = dpu->nextVinBindings.sources[streamIdx].offset;
                afxBuffer buf = dpu->nextVinBindings.sources[streamIdx].buf;
                afxNat32 stride;

                if (!(updMask & AfxGetBitOffset(streamIdx))) stride = 0;
                else
                {
                    stride = dpu->activeVin->base.streams[streamIdx].stride;
                    //AfxAssert(srcIdx == streamIdx);

                    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, streamIdx, 1);

#if 0
                    if (dpu->activeVertexInput->streams[streamIdx].srcIdx != srcIdx)
                    {
                        dpu->activeVertexInput->streams[streamIdx].srcIdx = srcIdx;
                        updReq = TRUE;
                    }

                    if (dpu->activeVertexInput->streams[streamIdx].stride != stride)
                    {
                        dpu->activeVertexInput->streams[streamIdx].stride = stride;
                        updReq = TRUE;
                    }

                    if (dpu->activeVertexInput->streams[streamIdx].instance != instance)
                    {
                        dpu->activeVertexInput->streams[streamIdx].instance = instance;
                        updReq = TRUE;

                        // !!!!!!! GAMBIARRA
                    }

                    if (updReq)
                    {

                    }
#endif
                    if (dpu->activeVin->bindings.sources[streamIdx].buf != buf)
                    {
                        dpu->activeVin->bindings.sources[streamIdx].buf = buf;
                        updReq = TRUE;
                    }

                    if (dpu->activeVin->bindings.sources[streamIdx].offset != offset)
                    {
                        dpu->activeVin->bindings.sources[streamIdx].offset = offset;
                        updReq = TRUE;
                    }

                    if (dpu->activeVin->bindings.sources[streamIdx].range != range)
                    {
                        dpu->activeVin->bindings.sources[streamIdx].range = range;
                        updReq = TRUE;
                    }
                }

                _SglBindAndSyncBuf(gl, !updReq, GL_ARRAY_BUFFER, streamIdx, buf, offset, range, stride, /*buf->glUsage ? buf->glUsage : */GL_STATIC_DRAW);
            }
        }
        dpu->nextVinBindingsMask = NIL;
    }

    {
        afxBuffer buf = dpu->nextVinBindings.idxSrcBuf;
        afxNat32 off = dpu->nextVinBindings.idxSrcOff;
        afxNat32 range = dpu->nextVinBindings.idxSrcRange;
        afxNat32 idxSrcSiz = dpu->nextVinBindings.idxSrcSiz;
        afxBool updReq = FALSE;
        
        if (dpu->flushIbb)
        {
            dpu->flushIbb = FALSE;

            if (dpu->activeVin->bindings.idxSrcBuf != buf)
            {
                dpu->activeVin->bindings.idxSrcBuf = buf;
                updReq = TRUE;
            }

            if (dpu->activeVin->bindings.idxSrcOff != off)
            {
                dpu->activeVin->bindings.idxSrcOff = off;
                updReq = TRUE;
            }

            if (dpu->activeVin->bindings.idxSrcRange != range)
            {
                dpu->activeVin->bindings.idxSrcRange = range;
                updReq = TRUE;
            }

            if (dpu->activeVin->bindings.idxSrcSiz != idxSrcSiz)
            {
                dpu->activeVin->bindings.idxSrcSiz = idxSrcSiz;
                updReq = TRUE;
            }
        }

        _SglBindAndSyncBuf(gl, !updReq, GL_ELEMENT_ARRAY_BUFFER, 0, buf, off, range, idxSrcSiz, /*buf->glUsage ? buf->glUsage : */GL_STATIC_DRAW);
    }
}

_SGL void _SglDpuDraw(sglDpuIdd* dpu, _sglCmdDraw const *cmd)
{
    afxError err = AFX_ERR_NONE;    
    glVmt const* gl = &dpu->gl;
    afxNat cnt;

    if (dpu->activePip != dpu->nextPip)
    {
        dpu->activePip = dpu->nextPip;
        _SglDpuBindAndSyncPip(dpu, FALSE, dpu->activePip);
        dpu->activeVin = dpu->nextVin;
        _SglDpuBindAndSyncVin(dpu, FALSE, dpu->activeVin);
        //dpu->activeVertexInput = &dpu->activePip->vertexInput;
    }
    SglFlushXformStateChanges(dpu);
    SglFlushRasterizationStateChanges(dpu);    
    _SglFlushResourcingStateChanges(dpu);

    //if (dpu->nextVtxInStreamUpdMask || dpu->nextVtxInAttribUpdMask || dpu->flushIbb)
        _SglFlushVertexInputStateChanges(dpu);

    AfxAssert(!dpu->nextVinBindingsMask);
    //AfxAssert(!dpu->nextVtxInAttribUpdMask);
    AfxAssert(!dpu->flushIbb);

    AfxAssert(dpu->activePip);
    AfxAssert(cmd->vtxCnt);
    //AfxAssert(cmd->instCnt);
    GLenum top = AfxToGlTopology(dpu->activeXformState.primTop);

    if (cmd->instCnt)
    {
        if (cmd->firstInst)
        {
            AfxAssert(gl->DrawArraysInstancedBaseInstance);
            gl->DrawArraysInstancedBaseInstance(top, cmd->firstVtx, cmd->vtxCnt, cmd->instCnt, cmd->firstInst); _SglThrowErrorOccuried();
        }
        else
        {
            gl->DrawArraysInstanced(top, cmd->firstVtx, cmd->vtxCnt, cmd->instCnt); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->DrawArrays(top, cmd->firstVtx, cmd->vtxCnt); _SglThrowErrorOccuried();
    }
    dpu->numOfFedVertices += cmd->vtxCnt;
    dpu->numOfFedInstances += cmd->instCnt;

    //AfxEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _SglDpuDrawIndexed(sglDpuIdd* dpu, _sglCmdDrawIndexed const* cmd)
{
    /*
        When the command is executed, primitives are assembled using the current primitive topology and indexCount vertices whose indices are retrieved from the index buffer. 
        The index buffer is treated as an array of tightly packed unsigned integers of size defined by the vkCmdBindIndexStream::indexType parameter with which the buffer was bound.

        The first vertex index is at an offset of firstIndex × indexSize + offset within the bound index buffer, where offset is the offset specified by CmdBindIndexStream and indexSize is the byte size of the type specified by indexType. 
        Subsequent index values are retrieved from consecutive locations in the index buffer. 
        Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the indexType is VK_INDEX_TYPE_UINT8_EXT or VK_INDEX_TYPE_UINT16) and have vertexOffset added to them, before being supplied as the vertexIndex value.

        The primitives are drawn instanceCount times with instanceIndex starting with firstInstance and increasing sequentially for each instance. 
        The assembled primitives execute the bound graphics pipeline.
    */

    afxError err = AFX_ERR_NONE;

    if (dpu->activePip != dpu->nextPip)
    {
        dpu->activePip = dpu->nextPip;
        _SglDpuBindAndSyncPip(dpu, FALSE, dpu->activePip);
        //dpu->activeVertexInput = &dpu->activePip->vertexInput;
        dpu->activeVin = dpu->nextVin;
        _SglDpuBindAndSyncVin(dpu, FALSE, dpu->activeVin);
    }
    SglFlushXformStateChanges(dpu);
    SglFlushRasterizationStateChanges(dpu);
    _SglFlushResourcingStateChanges(dpu);

    //if (dpu->nextVtxInStreamUpdMask || dpu->nextVtxInAttribUpdMask || dpu->flushIbb)
        _SglFlushVertexInputStateChanges(dpu);

    AfxAssert(!dpu->nextVinBindingsMask);
    //AfxAssert(!dpu->nextVtxInAttribUpdMask);
    AfxAssert(!dpu->flushIbb);

    AfxAssertObjects(1, &dpu->activeVin->bindings.idxSrcBuf, afxFcc_BUF);
    //AfxAssert(dpu->state.vertexBindingCnt);

    AfxAssert(cmd->idxCnt);
    //AfxAssert(cmd->instCnt);

    static const GLenum idxSizGl[] =
    {
        0,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_SHORT,
        0,
        GL_UNSIGNED_INT
    };

    afxSize idxSiz = dpu->activeVin->bindings.idxSrcSiz;
    afxSize idxBaseOff = dpu->activeVin->bindings.idxSrcOff;
    
    GLint vtxOff2 = cmd->vtxOff;

    GLenum top = AfxToGlTopology(dpu->activeXformState.primTop);

    glVmt const* gl = &dpu->gl;

    if (cmd->instCnt)
    {
        if (cmd->firstInst)
        {
            AfxAssert(gl->DrawElementsInstancedBaseVertexBaseInstance);
            gl->DrawElementsInstancedBaseVertexBaseInstance(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), cmd->instCnt, cmd->vtxOff, cmd->firstInst); _SglThrowErrorOccuried();
        }
        else
        {
            gl->DrawElementsInstancedBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), cmd->instCnt, vtxOff2); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->DrawElementsBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), vtxOff2); _SglThrowErrorOccuried();
    }
    //dpu->numOfFedVertices += cmd->idxCnt;
    dpu->numOfFedIndices += cmd->idxCnt;
    dpu->numOfFedInstances += cmd->instCnt;
    
    //AfxEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_SGL void _SglDpuExecuteCommands(sglDpuIdd* dpu, _sglCmdExecCmds const* cmd);

union
{
    afxCmd  cmd;
    void(*f[SGL_CMD_TOTAL])(sglDpuIdd* dpu, _sglCmd *cmd);
}
_SglDpuVmt =
{
    .cmd.BindPipeline = (void*)_SglDpuBindPipeline,
    .cmd.BindVertexInput = (void*)_SglDpuBindVertexInput,
    .cmd.BindBuffers = (void*)_SglDpuBindBuffers,
    .cmd.BindRasters = (void*)_SglDpuBindRasters,

    .cmd.BindVertexSources = (void*)_SglDpuBindVertexSources,
    //.cmd.ResetVertexStreams = (void*)_SglDpuResetVertexStreams,
    //.cmd.ResetVertexAttributes = (void*)_SglDpuResetVertexAttributes,
    .cmd.BindIndexSource = (void*)_SglDpuBindIndexSource,
    .cmd.SetPrimitiveTopology = (void*)_SglDpuSetPrimitiveTopology,

    .cmd.ResetViewports = (void*)_SglDpuSetViewports,
    .cmd.ReadjustViewports = (void*)_SglDpuSetViewports,

    .cmd.DisableRasterization = (void*)_SglDpuDisableRasterization,
    .cmd.SwitchFrontFace = (void*)_SglDpuSwitchFrontFace,
    .cmd.SetCullMode = (void*)_SglDpuSetCullMode,

    .cmd.EnableDepthBias = (void*)_SglDpuEnableDepthBias,
    .cmd.SetDepthBias = (void*)_SglDpuSetDepthBias,

    .cmd.SetLineWidth = (void*)_SglDpuSetLineWidth,

    .cmd.ResetScissors = (void*)_SglDpuSetScissors,
    .cmd.ReadjustScissors = (void*)_SglDpuSetScissors,

    .cmd.ResetAreas = (void*)_SglDpuSetAreas,
    .cmd.ReadjustAreas = (void*)_SglDpuSetAreas,

    .cmd.EnableDepthBoundsTest = NIL,
    .cmd.SetDepthBounds = NIL,

    .cmd.EnableStencilTest = (void*)_SglDpuEnableStencilTest,
    .cmd.SetStencilCompareMask = (void*)_SglDpuSetStencilCompareMask,
    .cmd.SetStencilReference = (void*)_SglDpuSetStencilReference,
    .cmd.SetStencilWriteMask = (void*)_SglDpuSetStencilWriteMask,

    .cmd.EnableDepthTest = (void*)_SglDpuEnableDepthTest,
    .cmd.SetDepthCompareOp = (void*)_SglDpuSetDepthCompareOp,
    .cmd.EnableDepthWrite = (void*)_SglDpuEnableDepthWrite,

    .cmd.SetBlendConstants = (void*)_SglDpuSetBlendConstants,

    .cmd.BeginSynthesis = (void*)_SglDpuBeginSynthesis,
    .cmd.FinishSynthesis = (void*)_SglDpuFinishSynthesis,
    .cmd.NextPass = (void*)_SglDpuNextPass,
    .cmd.ExecuteCommands = (void*)_SglDpuExecuteCommands,

    .cmd.Draw = (void*)_SglDpuDraw,
    .cmd.DrawIndexed = (void*)_SglDpuDrawIndexed,

    .cmd.FlipRaster = (void*)_SglDpuFlipRaster,
    .cmd.SwizzleRaster = (void*)_SglDpuSwizzleRaster,
    .cmd.RegenerateMipmaps = (void*)_SglDpuRegenerateMipmaps,
    .cmd.CopyRaster = (void*)_SglDpuCopyRaster,
};

_SGL void _SglDpuExecuteCommands(sglDpuIdd* dpu, _sglCmdExecCmds const* cmd)
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxDrawScript dscr = cmd->subsets[i];
        AfxAssertObjects(1, &dscr, afxFcc_DSCR);
        AfxAssert(dscr->base.state == afxDrawScriptState_PENDING);

        if (dscr->base.state == afxDrawScriptState_PENDING)
        {
            _sglCmd *cmdHdr;
            AfxChainForEveryLinkageB2F(&dscr->commands, _sglCmd, script, cmdHdr)
            {
                if (cmdHdr->id == SGL_CMD_END)
                    break;

                if (dscr->base.state != afxDrawScriptState_PENDING)
                {
                    AfxThrowError();
                    break;
                }

                _SglDpuVmt.f[cmdHdr->id](dpu, cmdHdr);
            }

            if (!err)
            {
                dscr->base.state = afxDrawScriptState_EXECUTABLE;
            }

            if (err || dscr->base.disposable)
            {
                AfxAssert(dscr->base.portIdx == dpu->portIdx);
                dscr->base.state = afxDrawScriptState_INVALID;
            }
        }
    }
}

// QUEUE STUFF

_SGL afxError _SglDpuExecuteDscr(sglDpuIdd* dpu, afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(dque, afxFcc_DQUE);
    //glVmt const* gl = &thdIdd->wglVmt;

    AfxAssertObjects(1, &dpu->activeDctx, afxFcc_DCTX);
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    
    AfxAssert(dscr->base.state == afxDrawScriptState_PENDING);

    if (dscr->base.state == afxDrawScriptState_PENDING)
    {
        _sglCmd * cmdHdr;
        AfxChainForEveryLinkageB2F(&dscr->commands, _sglCmd, script, cmdHdr)
        {
            if (cmdHdr->id == SGL_CMD_END)
                break;

            if (dscr->base.state != afxDrawScriptState_PENDING)
            {
                AfxThrowError();
                break;
            }

            AfxAssertRange(SGL_CMD_TOTAL, cmdHdr->id, 1);

            // I know. This shit breaks CPU prediction. Just ignore it for now.

            if (err)
            {
                int a = 0;
            }

            _SglDpuVmt.f[cmdHdr->id](dpu, cmdHdr);
        }

        if (!err)
        {
            dscr->base.state = afxDrawScriptState_EXECUTABLE;
        }

        if (err || dscr->base.disposable)
        {
            AfxAssert(dscr->base.portIdx == dpu->portIdx);
            dscr->base.state = afxDrawScriptState_INVALID;
        }
    }
    return err;
}

_SGL afxError _SglDpuPresentSurf(sglDpuIdd* dpu, afxDrawOutput dout, afxNat outBufIdx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(dque, afxFcc_DQUE);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    afxDrawContext dctx = dpu->activeDctx;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    AfxAssert(!dout->base.swapping);

    {
        dout->base.swapping = TRUE;

        //wglVmt const*wgl = &dpu->wgl;

        HDC dc = dpu->GetCurrentDC();
        HGLRC rc = dpu->GetCurrentContext();

        if (rc != dpu->glrc || dc != dout->dc)
        {
            if (!dpu->MakeCurrent(dout->dc, dpu->glrc))
            {
                AfxThrowError();
                AfxError("DPU %u failed to set to draw output device %p.", dpu->portIdx, dout);
                dpu->MakeCurrent(dpu->dc, dpu->glrc);
            }
        }

        afxRaster surf;
        AfxGetDrawOutputSurface(dout, outBufIdx, 1, &surf);
        afxCanvas canv;
        AfxGetDrawOutputCanvas(dout, outBufIdx, 1 , &canv);
        AfxAssertObjects(1, &surf, afxFcc_RAS);
        //AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
        glVmt const* gl = &dpu->gl;
#if !0
#if 0
        dpu->activeTmpFboIdx = (dpu->activeTmpFboIdx + 1) % (sizeof(dpu->tmpFbo) / sizeof(dpu->tmpFbo[0]));
        GLuint tmpFbo = dpu->tmpFbo[dpu->activeTmpFboIdx];

        if (!tmpFbo)
        {
            gl->GenFramebuffers(1, &(tmpFbo)); _SglThrowErrorOccuried();
        }
        else
        {
            gl->DeleteFramebuffers(1, &(tmpFbo)); _SglThrowErrorOccuried();
            gl->GenFramebuffers(1, &(tmpFbo)); _SglThrowErrorOccuried();
        }
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, tmpFbo); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(tmpFbo));
        gl->FramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, surf->glTarget, surf->glHandle, 0); _SglThrowErrorOccuried();
#else
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, canv->glHandle); _SglThrowErrorOccuried();
#endif
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  _SglThrowErrorOccuried();
        gl->BlitFramebuffer(0, 0, surf->base.whd[0], surf->base.whd[1], 0, 0, surf->base.whd[0], surf->base.whd[1], GL_COLOR_BUFFER_BIT, GL_LINEAR); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Flush();
#else

#if !0
        AfxAssert(gl->ClipControl);

        //gl->ClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE); _SglThrowErrorOccuried();// set screen origin to top to bottm, and set depth to [ 0, 1 ]

        afxWhd extent;
        AfxGetRasterExtent(surf, 0, extent);
        
        _sglCmdBeginSynthesis cmdBeginOp = { 0 };
        cmdBeginOp.defFbo = TRUE;
        cmdBeginOp.canv = NIL;
        cmdBeginOp.area = (afxRect const){ { { 0, 0 } }, { { extent[0], extent[1] } } };
        cmdBeginOp.rasterCnt = 1;
        cmdBeginOp.rasters[0] = (afxDrawTarget const){ .loadOp = afxSurfaceLoadOp_CLEAR, .storeOp = afxSurfaceStoreOp_STORE, .clearValue = { .color = { 0.3, 0.1, 0.3, 1 } } };

        _SglDpuBeginSynthesis(dpu, &cmdBeginOp);

        _sglCmdPipeline cmdBindPip = { 0 };
        cmdBindPip.pip = dctx->presentPip;
        _SglDpuBindPipeline(dpu, &cmdBindPip);

#else
        afxDrawTarget const rasterRt = { NIL, afxSurfaceLoadOp_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _SglDpuBeginCombination(dpu, NIL, 1, 0, &rasterRt, NIL, NIL);

        afxPipeline pip = AfxDrawOperationGetPipeline(idd->presentDop, 0, 0);
        AfxAssertObjects(1, &pip, afxFcc_PIP);
        _SglDpuBindPipeline(dpu, pip);
#endif   
        //_SglDpuEmployTechnique(dque, 0);

        _sglCmdViewport cmdSetVp = { 0 };
        cmdSetVp.cnt = 1;
        cmdSetVp.vp[0].extent[0] = extent[0];
        cmdSetVp.vp[0].extent[1] = extent[1];
        cmdSetVp.vp[0].depth[1] = 1;
        cmdSetVp.reset = TRUE;

        _SglDpuSetViewports(dpu, &cmdSetVp);

#if 0 // already set by pipeline
        afxPipelineRasterizerState const ras = { FALSE, FALSE, afxFillMode_SOLID, afxCullMode_BACK, afxFrontFace_CCW, FALSE, 0, 0, 0, 1 };
        _SglDpuSetRasterizerState(dque, &ras);

        afxPipelineDepthState const depth = { 0 };
        _SglDpuSetDepthState(dque, &depth);
#endif

#if 0 // already set by pipeline
        afxPipelinePrimitiveState ia;
        ia.topology = afxPrimTopology_TRI_STRIP;
        ia.primRestartEnable = FALSE;
        _SglDpuSetInputAssemblyState(dque, &ia);
#endif
#if 0
        afxNat const baseVtxs[] = { 0, 0 };
        afxNat const vtxArrs[] = { 0, 1 };
        afxVertexBuffer vbufs[] = { idd->rgbRectVbo, idd->rgbRectVbo };
        _SglDpuBindVertexBuffers(dque, 0, 2, vbufs, baseVtxs, vtxArrs);
#endif

#if 0
        _sglCmdVertexAttributes const vtxAttrs =
        {
            .cnt = 1,
            .location[0] = 0,
            .fmt[0] = afxVertexFormat_V2D,
            .srcIdx[0] = 7,
            .offset[0] = 0
        };
        _SglDpuResetVertexAttributes(dpu, &vtxAttrs);
        _sglCmdVertexStreams const vtxStreams =
        {
            .cnt = 1,
            .srcIdx[0] = 7,
            .stride[0] = sizeof(afxV2d),
            .instance[0] = FALSE
        };
        _SglDpuResetVertexStreams(dpu, &vtxStreams);
        _sglCmdVertexSources bindVbuf = { .first = 7, .cnt = 1, .offset[0] = 0, .range[0] = sizeof(afxV2d) * 4 };
        bindVbuf.buf[0] = dctx->tristrippedQuad2dPosBuf;
        _SglDpuBindVertexSources(dpu, &bindVbuf);
#endif
        //_SglDpuBindLegos(dque, 0, 1, &idd->presentLego);
        _sglCmdBindRasters cmdBindTex = { 0 };
        cmdBindTex.first = 0;
        cmdBindTex.cnt = 1;
        cmdBindTex.smp[0] = dctx->presentSmp;
        cmdBindTex.tex[0] = surf;
        _SglDpuBindRasters(dpu, &cmdBindTex);

        _sglCmdDraw cmdDraw = { 0 };
        cmdDraw.vtxCnt = 4;
        cmdDraw.instCnt = 1;
        _SglDpuDraw(dpu, &cmdDraw);

#if !0
        _SglDpuFinishSynthesis(dpu, NIL);
#else
        _SglDpuEndCombination(dpu, NIL);
#endif
#endif
        
        // deadlocks all
        SglSwapBuffers(dout->dc, dpu);
        
        //gl->ClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE); _SglThrowErrorOccuried(); // reset GL default

        afxReal64 currTime, deltaTime;
        AfxGetExecutionTime(&currTime, &deltaTime);
        afxNat currIter, lastFreq;
        AfxGetExecutionCounter(&currIter,&lastFreq);

        if (0 == currIter)
        {
            AfxFormatString(&dout->base.caption, "Delta %0f, IPS %u/%u --- OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build", deltaTime, lastFreq, currIter);
            SetWindowTextA(dout->wnd, AfxGetStringData(&dout->base.caption.str, 0));
        }
        dout->base.swapping = FALSE;
        dout->base.buffers[outBufIdx].booked = FALSE;        
    }
    return err;
}

_SGL void* _AfxDqueRequestArenaSpace(afxDrawContext dctx, afxDrawQueue dque, afxNat siz)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    afxNat unitIdx = dque->base.portIdx;

    AfxEnterSlockExclusive(&dque->base.arenaSlock);

    void *block = AfxRequestArenaUnit(&dque->base.cmdArena, siz);

    if (!block)
        AfxThrowError();

    AfxExitSlockExclusive(&dque->base.arenaSlock);

    return block;
}

_SGL void _AfxDqueRecycleArenaSpace(afxDrawContext dctx, afxDrawQueue dque, void *block, afxNat siz)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertObjects(1, &dque, afxFcc_DQUE);


    afxNat unitIdx = dque->base.portIdx;

    AfxEnterSlockExclusive(&dque->base.arenaSlock);

    AfxAssert(block);

    AfxRecycleArenaUnit(&dque->base.cmdArena, block, siz);

    AfxExitSlockExclusive(&dque->base.arenaSlock);
}

_SGL afxError _SglDqueProcCb(afxDrawQueue dque, afxDrawContext dctx, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    if (AfxTryEnterSlockExclusive(&dque->base.pendingChainSlock))
    {
        afxDrawDevice ddev = AfxGetObjectProvider(dctx);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
   
        afxNat unitIdx;
        AfxGetThreadingUnit(&unitIdx);
        sglDpuIdd *dpu = &ddev->dpus[unitIdx];
        //wglVmt const*wgl = &dpu->wgl;
    
        AfxAssert(unitIdx == dque->base.portIdx);
    
        dpu->activeDctx = dctx;
    
        HGLRC rc = dpu->GetCurrentContext();

        if (rc != dpu->glrc)
        {
            if (!dpu->MakeCurrent(dpu->dc, dpu->glrc))
            {
                AfxThrowError();
                AfxError("DPU %u failed to be set.", unitIdx);
                dpu->MakeCurrent(dpu->dc, dpu->glrc);
            }
        }

        dpu->numOfFedVertices = 0;
        dpu->numOfFedIndices = 0;
        dpu->numOfFedInstances = 0;

        _afxDqueSubm* subm;
        AfxChainForEveryLinkageB2F(&dque->base.pendingChain, _afxDqueSubm, chain, subm)
        {
            AfxGetTime(&subm->pullTime);

            //if (subm->isScript)
            {
                for (afxNat i = 0; i < subm->scriptCnt; i++)
                {
                    afxDrawScript dscr = subm->scripts[i];
                    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
                    
                    if (_SglDpuExecuteDscr(dpu, dscr))
                        AfxThrowError();
                }
            }
            //else
            {
                for (afxNat i = 0; i < subm->outputCnt; i++)
                {
                    afxDrawOutput dout = subm->outputs[i];

                    if (_SglDpuPresentSurf(dpu, dout, subm->outBufIdx[i]))
                        AfxThrowError();
                }
            }

            AfxGetTime(&subm->complTime);

            AfxPopLinkage(&subm->chain);

            //AfxEnterSlockExclusive(&dctxD->ports[dque->portIdx].recyclSubmChainSlock);

            //if (dctxD->ports[dque->portIdx].minRecyclSubmCnt > AfxGetChainLength(&dctxD->ports[dque->portIdx].recyclSubmChain))
            {
                //AfxPushLinkage(&subm->chain, &dctxD->ports[dque->portIdx].recyclSubmChain);
            }
            //else
            {
                _AfxDqueRecycleArenaSpace(dctx, dque, subm, sizeof(*subm));
            }
            //AfxExitSlockExclusive(&dctxD->ports[dque->portIdx].recyclSubmChainSlock);
        }

        AfxExitSlockExclusive(&dque->base.pendingChainSlock);
    }
    return err;
}

_SGL afxBool _SglDqueVmtSubmitCb(afxDrawContext dctx, afxDrawQueue dque, afxDrawSubmissionSpecification const *spec, afxNat *submNo) // called by din to enqueue submission
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    _afxDqueSubm* subm = _AfxDqueRequestArenaSpace(dctx, dque, sizeof(*subm));

    *subm = (_afxDqueSubm) { 0 };

    subm->submNo = 0;
    subm->exec = NIL;
    subm->reqSubmNo = 0;
    AfxGetTime(&subm->pushTime);
    subm->pullTime = 0;
    subm->complTime = 0;

    subm->scriptCnt = AfxMin(spec->scriptCnt, 4);

    for (afxNat i = 0; i < subm->scriptCnt; i++)
    {
        subm->scripts[i] = spec->scripts[i];
        subm->scripts[i]->base.state = afxDrawScriptState_PENDING;
    }

    subm->outputCnt = AfxMin(spec->outputCnt, 4);
    
    for (afxNat i = 0; i < subm->outputCnt; i++)
    {
        subm->outputs[i] = spec->outputs[i];
        subm->outBufIdx[i] = spec->outBufIdx[i];
        //subm->outputs[i]->buffers[subm->outBufIdx[i]]->state = AFX_SURF_STATE_PENDING;
    }
    subm->data[0] = spec->data;


    AfxEnterSlockExclusive(&dque->base.pendingChainSlock);

    AfxPushLinkage(&subm->chain, &dque->base.pendingChain);

    AfxExitSlockExclusive(&dque->base.pendingChainSlock);

    return TRUE;
}

_SGL afxError _SglDqueWaitCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    return err;
}

_SGL afxError _SglDqueDtor(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    //if (dque->vmt->dtor && dque->vmt->dtor(dque))
    //    AfxThrowError();

    //AfxAssert(!dque->idd);

    AfxReleaseSlock(&dque->base.pendingChainSlock);
    AfxReleaseSlock(&dque->base.reqLock);
    AfxDeallocateArena(&dque->base.cmdArena);
    AfxReleaseSlock(&dque->base.arenaSlock);

    return err;
}

_SGL afxError _SglDqueCtor(afxDrawQueue dque, afxCookie const* cookie)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxNat portIdx = *(afxNat const *)cookie->udd[1];
    afxDrawQueueSpecification const *spec = ((afxDrawQueueSpecification const *)cookie->udd[2]);
    afxDrawQueue dque2;

    if (!AfxGetInstance(&ddev->base.ports[portIdx].queues, AfxIdentifyObject(dque), (afxObject*)&dque2) || dque2 != dque) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dque2, afxFcc_DQUE);
        AfxAssert(spec);

        afxMmu mmu = AfxGetDrawSystemMmu();

        dque->base.portIdx = portIdx;
        dque->base.dctx = spec->dctx;
        AfxAssertObjects(1, &spec->dctx, afxFcc_DCTX);

        dque->base.immediate = 0;// !!spec->immedate;

        AfxTakeSlock(&dque->base.arenaSlock);
        AfxAllocateArena(mmu, &dque->base.cmdArena, NIL, AfxHint());

        dque->base.locked = FALSE;

        AfxTakeChain(&dque->base.pendingChain, dque);
        AfxTakeSlock(&dque->base.pendingChainSlock);
        AfxTakeSlock(&dque->base.reqLock);

        dque->base.waitCb = _SglDqueWaitCb;
        dque->base.procCb = _SglDqueProcCb;

        //if (ddev->vmt->dque && ddev->vmt->dque(dque, spec)) AfxThrowError();
        //else
        {
            //AfxAssert(dque->vmt);
        }

        if (err)
        {
            AfxReleaseSlock(&dque->base.pendingChainSlock);
            AfxReleaseSlock(&dque->base.reqLock);

            AfxDeallocateArena(&dque->base.cmdArena);
        }
    }
    return err;
}

_SGL afxClassConfig _SglDqueClsConfig =
{
    .fcc = afxFcc_DQUE,
    .name = "Draw Queue",
    .unitsPerPage = 3,
    .size = sizeof(AFX_OBJECT(afxDrawQueue)),
    .mmu = NIL,
    .ctor = (void*)_SglDqueCtor,
    .dtor = (void*)_SglDqueDtor
};
