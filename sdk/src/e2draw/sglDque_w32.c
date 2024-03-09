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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#include "sgl.h"

#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

extern void _SglDpuSwitchFrontFace(sglDpuIdd* dpu, _sglCmdBool const *cmd);
extern void _SglDpuSetCullMode(sglDpuIdd* dpu, _sglCmdNat const *cmd);
extern void _SglDpuSetPrimitiveTopology(sglDpuIdd* dpu, _sglCmdNat const *cmd);
extern void _SglDpuSetViewports(sglDpuIdd* dpu, _sglCmdViewport const *cmd);
extern void _SglDpuBindVertexSources(sglDpuIdd* dpu, _sglCmdVertexSources const *cmd);
extern void _SglDpuBindIndexSource(sglDpuIdd* dpu, _sglCmdBufferRange const *cmd);
extern void _SglDpuBindVertexInput(sglDpuIdd* dpu, _sglCmdVertexInput *cmd);

extern void _SglDpuDisableRasterization(sglDpuIdd* dpu, _sglCmdBool const *cmd);
extern void _SglDpuEnableDepthBias(sglDpuIdd* dpu, _sglCmdBool const *cmd);
extern void _SglDpuSetDepthBias(sglDpuIdd* dpu, _sglCmdReal3 const *cmd);
extern void _SglDpuSetLineWidth(sglDpuIdd* dpu, _sglCmdReal const *cmd);
extern void _SglDpuEnableStencilTest(sglDpuIdd* dpu, _sglCmdBool const *cmd);
extern void _SglDpuSetStencilCompareMask(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd);
extern void _SglDpuSetStencilWriteMask(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd);
extern void _SglDpuSetStencilReference(sglDpuIdd* dpu, _sglCmdBitmaskNat32 const *cmd);
extern void _SglDpuEnableDepthTest(sglDpuIdd* dpu, _sglCmdBool const *cmd);
extern void _SglDpuSetDepthCompareOp(sglDpuIdd* dpu, _sglCmdNat const *cmd);
extern void _SglDpuDisableDepthWrite(sglDpuIdd* dpu, _sglCmdBool const *cmd);
extern void _SglDpuSetBlendConstants(sglDpuIdd* dpu, _sglCmdReal4 const *cmd);
extern void _SglDpuSetScissors(sglDpuIdd* dpu, _sglCmdScissor const *cmd);
extern void _SglDpuSetAreas(sglDpuIdd* dpu, _sglCmdArea const *cmd);

extern void _SglDpuRasSubsample(sglDpuIdd* dpu, _sglCmdRegenerateMipmaps const* cmd);
extern void _SglDpuRasPack(sglDpuIdd* dpu, _sglCmdPackRasterRegions const* cmd);
extern void _SglDpuRasCopy(sglDpuIdd* dpu, _sglCmdCopyRasterRegions const* cmd);
extern void _SglDpuRasSwizzle(sglDpuIdd* dpu, _sglCmdSwizzleRasterRegions const* cmd);
extern void _SglDpuRasXform(sglDpuIdd* dpu, _sglCmdFlipRasterRegions const* cmd);

extern void _SglDpuBufCpy(sglDpuIdd* dpu, _sglCmdBufCpy const* cmd);
extern void _SglDpuBufSet(sglDpuIdd* dpu, _sglCmdBufSet const* cmd);
extern void _SglDpuBufRw(sglDpuIdd* dpu, _sglCmdBufRw const* cmd);
extern void _SglDpuBufIo(sglDpuIdd* dpu, _sglCmdBufRw const* cmd);

extern void SglFlushXformStateChanges(sglDpuIdd* dpu);
extern void SglFlushRasterizationStateChanges(sglDpuIdd* dpu);

// RENDERING SCOPE
 
_SGL void _SglDpuFinishSynthesis(sglDpuIdd* dpu, _sglCmd const *cmd)
{
    (void)cmd;
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    //_SglDpuBindAndSyncCanv(gl, FALSE, GL_READ_FRAMEBUFFER, 0);
    //_SglDpuBindAndSyncCanv(gl, FALSE, GL_DRAW_FRAMEBUFFER, 0);
    //gl->Flush(); _SglThrowErrorOccuried();

    afxCanvas canv = dpu->activeRasterState.pass.canv;

    if (canv)
    {
        afxNat surCnt = AfxCountSurfaces(canv);

        if (surCnt)
        {
            AfxAssert(_SGL_MAX_SURF_PER_CANV >= surCnt);
            afxRaster surfaces[_SGL_MAX_SURF_PER_CANV];
            AfxGetDrawBuffers(canv, 0, surCnt, surfaces);

            for (afxNat i = 0; i < surCnt; i++)
            {
                afxRaster ras = surfaces[i];

                if (ras)
                {
                    AfxAssertObjects(1, &ras, afxFcc_RAS);
                    ras->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
                }
            }
        }
    }
}

_SGL void _SglDpuBeginSynthesis(sglDpuIdd* dpu, _sglCmdBeginSynthesis const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxCanvas canv = cmd->canv;
    afxDrawTarget const* dt;
    
    //_SglDpuBindAndSyncCanv(gl, bindCanv, TRUE, GL_READ_FRAMEBUFFER, canv);
    _SglDpuBindAndSyncCanv(dpu, (dpu->activeRasterState.pass.canv != canv), TRUE, GL_DRAW_FRAMEBUFFER, canv);
    //dpu->activeRasterState.pass.canv = canv;

    if (!canv)
    {
        if (cmd->defFbo)
        {
            dt = &cmd->rasters[0];

            switch (dt->loadOp)
            {
            case afxSurfaceLoadOp_CLEAR:
            {
                afxReal const *color = dt->clearValue.color;
                gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
                gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();
                break;
            }
            case afxSurfaceLoadOp_LOAD:
            {
                break;
            }
            case afxSurfaceLoadOp_DONT_CARE:
            {
                afxWhd whd;
                AfxGetCanvasExtent(canv, whd);

                if ((cmd->area.x == 0) && (cmd->area.y == 0) && (cmd->area.w == whd[0]) && (cmd->area.h == whd[1]))
                {
                    gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK_LEFT }); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK_LEFT }, cmd->area.x, cmd->area.y, cmd->area.w, cmd->area.h); _SglThrowErrorOccuried();
                }
                break;
            }
            }
        }
    }
    else
    {
        GLuint glHandle = canv->glHandle;
        AfxAssert(gl->IsFramebuffer(glHandle));

        afxNat colorCnt = cmd->rasterCnt;
        afxNat maxSurCnt = AfxCountDrawSurfaces(canv);
        colorCnt = AfxMin(colorCnt, maxSurCnt);

        afxNat enabledToDrawCnt = 0;
        GLenum enabledToDraw[_SGL_MAX_SURF_PER_CANV];
        afxMask storeBitmask = NIL;

        for (afxNat i = 0; i < colorCnt; i++)
        {
            dt = &cmd->rasters[i];

            switch (dt->storeOp)
            {
            case afxSurfaceStoreOp_STORE:
            {
                enabledToDraw[enabledToDrawCnt] = GL_COLOR_ATTACHMENT0 + i;
                enabledToDrawCnt++;
                storeBitmask |= AfxGetBitOffset(i);
                break;
            }
            case afxSurfaceStoreOp_DISCARD:
            {
                enabledToDraw[enabledToDrawCnt] = GL_NONE;
                enabledToDrawCnt++;
                storeBitmask |= AfxGetBitOffset(8 + i);
                break;
            }
            case afxSurfaceStoreOp_DONT_CARE:
            default:
            {
                enabledToDraw[enabledToDrawCnt] = GL_NONE;
                enabledToDrawCnt++;
                storeBitmask |= AfxGetBitOffset(16 + i);
                break;
            }
            }
        }

        if (canv->storeBitmask != storeBitmask)
        {
            canv->storeBitmask = storeBitmask;
            gl->DrawBuffers(enabledToDrawCnt, enabledToDraw); _SglThrowErrorOccuried();
        }
        
        afxNat enabledToDontLoadCnt = 0;
        GLenum enabledToDontLoad[_SGL_MAX_SURF_PER_CANV];

        for (afxNat i = 0; i < colorCnt; i++)
        {
            dt = &cmd->rasters[i];

            switch (dt->loadOp)
            {
            case afxSurfaceLoadOp_CLEAR:
            {
                afxReal const *color = dt->clearValue.color;
                gl->ClearBufferfv(GL_COLOR, i, color); _SglThrowErrorOccuried();
                break;
            }
            case afxSurfaceLoadOp_LOAD:
            {
                break;
            }
            case afxSurfaceLoadOp_DONT_CARE:
            default:
            {
                enabledToDontLoad[enabledToDontLoadCnt] = GL_COLOR_ATTACHMENT0 + i;
                ++enabledToDontLoadCnt;
                break;
            }
            }
        }

        if (enabledToDontLoadCnt)
        {
            afxWhd whd;
            AfxGetCanvasExtent(canv, whd);

            if ((cmd->area.x == 0) && (cmd->area.y == 0) && (cmd->area.w == whd[0]) && (cmd->area.h == whd[1]))
            {
                gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, enabledToDontLoadCnt, enabledToDontLoad); _SglThrowErrorOccuried();
            }
            else
            {
                gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, enabledToDontLoadCnt, enabledToDontLoad, cmd->area.x, cmd->area.y, cmd->area.w, cmd->area.h); _SglThrowErrorOccuried();
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

_SGL void _SglDpuBindPipeline(sglDpuIdd* dpu, _sglCmdPipeline *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxPipeline pip = cmd->pip;

    dpu->nextPip = pip;
    
    if (!pip)
    {
        //dpu->nextVin = NIL;
    }
    else
    {
        dpu->nextVin = pip->base.vin;

        dpu->nextXformState.primTop = pip->base.primTop;
        dpu->nextXformState.primRestartEnabled = pip->base.primRestartEnabled;
        dpu->nextXformState.depthClampEnabled = pip->base.depthClampEnabled;
        
        if ((dpu->nextXformState.cullMode = pip->base.cullMode))
        {
            dpu->nextXformState.cwFrontFacing = pip->base.frontFacingInverted;
        }

        if (!pip->base.razr)
        {
            dpu->nextRasterState.rasterizationDisabled = TRUE;
        }
        else
        {
            dpu->nextRasterState.rasterizationDisabled = FALSE;

            afxRasterizationConfig config;
            AfxDescribeRasterizerConfiguration(pip->base.razr, &config);

            if ((dpu->nextRasterState.outCnt = config.colorOutCnt))
            {
                AfxCopy(config.colorOutCnt, sizeof(config.colorOuts[0]), config.colorOuts, dpu->nextRasterState.outs);
            
                AfxCopyColor(dpu->nextRasterState.blendConstants, config.blendConstants);
            }

            if ((dpu->nextRasterState.depthBiasEnabled = config.depthBiasEnabled))
            {
                dpu->nextRasterState.depthBiasClamp = config.depthBiasClamp;
                dpu->nextRasterState.depthBiasConstFactor = config.depthBiasConstFactor;
                dpu->nextRasterState.depthBiasSlopeScale = config.depthBiasSlopeScale;
            }
            
            if ((dpu->nextRasterState.depthBoundsTestEnabled = config.depthBoundsTestEnabled))
                AfxCopyV2d(dpu->nextRasterState.depthBounds, config.depthBounds);
            
            if ((dpu->nextRasterState.depthTestEnabled = config.depthTestEnabled))
            {
                dpu->nextRasterState.depthCompareOp = config.depthCompareOp;
            }

            dpu->nextRasterState.depthWriteDisabled = config.depthWriteDisabled;
            
            dpu->nextRasterState.fillMode = config.fillMode;
            dpu->nextRasterState.lineWidth = config.lineWidth;
            
            if ((dpu->nextRasterState.minSampleShadingValue = config.minSampleShadingValue))
            {
                dpu->nextRasterState.sampleShadingEnabled = config.sampleShadingEnabled;
            }

            dpu->nextRasterState.msEnabled = config.msEnabled;

            dpu->nextRasterState.alphaToCoverageEnabled = config.alphaToCoverageEnabled;
            dpu->nextRasterState.alphaToOneEnabled = config.alphaToOneEnabled;

            if ((dpu->nextRasterState.logicOpEnabled = config.pixelLogicOpEnabled))
            {
                dpu->nextRasterState.logicOp = config.pixelLogicOp;
            }

            if ((dpu->nextRasterState.sampleCnt = config.sampleCnt))
            {
                AfxCopy(config.sampleCnt, sizeof(config.sampleMasks), config.sampleMasks, dpu->nextRasterState.sampleMasks);
            }

            if ((dpu->nextRasterState.stencilTestEnabled = config.stencilTestEnabled))
            {
                dpu->nextRasterState.stencilBack = config.stencilBack;
                dpu->nextRasterState.stencilFront = config.stencilFront;
            }
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
        AfxAssertObjects(1, &lego, afxFcc_BSCH);

        for (afxNat j = 0; j < lego->base.entryCnt; j++)
        {
            afxBindSchemaEntry const *entry = &lego->base.entries[j];
            AfxAssert(entry->type);
            afxNat glUnit = (setIdx * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding;
            afxBool reqUpd = FALSE, reqUpd2 = FALSE;
            GLuint glHandle = 0, glHandle2 = 0;
            afxSize bufSiz = 0;
                    
            switch (entry->type)
            {
            case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
            case AFX_SHD_RES_TYPE_STORAGE_BUFFER:
            {
                afxNat offset = dpu->nextResBind[setIdx][j].offset;
                afxNat range = dpu->nextResBind[setIdx][j].range;
                afxBuffer buf = dpu->nextResBind[setIdx][j].buf;
                afxBool rebind = FALSE;
                GLenum glTarget = GL_INVALID_ENUM;

                if (entry->type == AFX_SHD_RES_TYPE_CONSTANT_BUFFER)
                    glTarget = GL_UNIFORM_BUFFER;
                else if (entry->type == AFX_SHD_RES_TYPE_STORAGE_BUFFER)
                    glTarget = GL_SHADER_STORAGE_BUFFER;
                else AfxThrowError();
                
                //if (updMask & AfxGetBitOffset(j))
                {
                    if ((dpu->activeResBind[setIdx][j].buf != buf) ||
                        (dpu->activeResBind[setIdx][j].offset != offset) ||
                        (dpu->activeResBind[setIdx][j].range != range)
                        )
                    {
                        dpu->activeResBind[setIdx][j].buf = buf;
                        dpu->activeResBind[setIdx][j].offset = offset;
                        dpu->activeResBind[setIdx][j].range = range;
                        rebind = TRUE;
                    }
                }

                if (!buf)
                {
                    gl->BindBufferBase(glTarget, glUnit, 0); _SglThrowErrorOccuried();
                }
                else
                {
                    _SglBindAndSyncBuf(dpu, sglBindFlag_SYNC, glTarget, buf, offset, range, 0, /*buf->glUsage ? buf->glUsage : */GL_DYNAMIC_DRAW);
                    bufSiz = AfxGetBufferCapacity(buf);

                    if (offset || range)
                    {
                        AfxAssert(range);
                        AfxAssertRange(bufSiz, offset, range);
                        gl->BindBufferRange(glTarget, glUnit, buf->glHandle, offset, range); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        gl->BindBufferBase(glTarget, glUnit, buf->glHandle); _SglThrowErrorOccuried();
                    }
                }
                break;
            }
            case AFX_SHD_RES_TYPE_SAMPLER:
            case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
            case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
            {
                afxSampler samp = dpu->nextResBind[setIdx][j].smp;
                afxRaster ras = dpu->nextResBind[setIdx][j].ras;
                afxBool rebindRas = FALSE;
                afxBool rebindSamp = FALSE;

                //if (updMask & AfxGetBitOffset(j))
                {
                    if (dpu->activeResBind[setIdx][j].ras != ras)
                    {
                        dpu->activeResBind[setIdx][j].ras = ras;
                        rebindRas = TRUE;
                    }

                    if (dpu->activeResBind[setIdx][j].smp != samp)
                    {
                        dpu->activeResBind[setIdx][j].smp = samp;
                        rebindSamp = TRUE;
                    }
                }

                if (entry->type == AFX_SHD_RES_TYPE_SAMPLED_IMAGE || entry->type == AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER)
                {
                    if (!ras)
                    {
                        gl->ActiveTexture(GL_TEXTURE0 + glUnit); _SglThrowErrorOccuried();
                        gl->BindTexture(GL_TEXTURE_2D, 0); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        _SglBindAndSyncRas(dpu, sglBindFlag_SYNC | sglBindFlag_KEEP | sglBindFlag_BIND, glUnit, ras);
                        gl->ActiveTexture(GL_TEXTURE0 + glUnit); _SglThrowErrorOccuried();
                        gl->BindTexture(ras->glTarget, ras->glHandle); _SglThrowErrorOccuried();
                    }
                }

                if (entry->type == AFX_SHD_RES_TYPE_SAMPLER || entry->type == AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER)
                {
                    if (!samp)
                    {
                        gl->BindSampler(glUnit, 0); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        _SglDpuBindAndSyncSamp(dpu, sglBindFlag_SYNC | sglBindFlag_KEEP | sglBindFlag_BIND, glUnit, samp);
                        gl->BindSampler(glUnit, samp->glHandle); _SglThrowErrorOccuried();
                    }
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

_SGL void _SglDpuDraw(sglDpuIdd* dpu, _sglCmdDraw const *cmd)
{
    afxError err = AFX_ERR_NONE;    
    glVmt const* gl = &dpu->gl;
    afxNat cnt;

    {
        _SglDpuBindAndSyncPip(dpu, (dpu->activePip != dpu->nextPip), TRUE, dpu->nextPip);
        dpu->activePip = dpu->nextPip;
        //dpu->activeVertexInput = &dpu->activePip->vertexInput;
    }
    
    //if (dpu->activeVin != dpu->nextVin)
    {
        dpu->activeVin = dpu->nextVin;
        _SglDpuBindAndSyncVin(dpu, dpu->activeVin, &dpu->nextVinBindings);
    }
    SglFlushXformStateChanges(dpu);
    SglFlushRasterizationStateChanges(dpu);    
    _SglFlushResourcingStateChanges(dpu);

    //if (dpu->nextVtxInStreamUpdMask || dpu->nextVtxInAttribUpdMask || dpu->flushIbb)
        //_SglFlushVertexInputStateChanges(dpu, FALSE);

    //AfxAssert(!dpu->nextVinBindingsMask);
    //AfxAssert(!dpu->nextVtxInAttribUpdMask);
    //AfxAssert(!dpu->flushIbb);

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

    {
        _SglDpuBindAndSyncPip(dpu, (dpu->activePip != dpu->nextPip), TRUE, dpu->nextPip);
        dpu->activePip = dpu->nextPip;
        //dpu->activeVertexInput = &dpu->activePip->vertexInput;        
    }

    //if (dpu->activeVin != dpu->nextVin)
    {
        dpu->activeVin = dpu->nextVin;
        _SglDpuBindAndSyncVin(dpu, dpu->activeVin, &dpu->nextVinBindings);
    }

    SglFlushXformStateChanges(dpu);
    SglFlushRasterizationStateChanges(dpu);
    _SglFlushResourcingStateChanges(dpu);

    //if (dpu->nextVtxInStreamUpdMask || dpu->nextVtxInAttribUpdMask || dpu->flushIbb)
        //_SglFlushVertexInputStateChanges(dpu, TRUE);

    //AfxAssert(!dpu->nextVinBindingsMask);
    //AfxAssert(!dpu->nextVtxInAttribUpdMask);
    //AfxAssert(!dpu->flushIbb);

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
    afxSize dataOff = (idxBaseOff + (idxSiz * cmd->firstIdx));

    GLenum top = AfxToGlTopology(dpu->activeXformState.primTop);

    glVmt const* gl = &dpu->gl;

    if (cmd->instCnt)
    {
        if (cmd->firstInst)
        {
            AfxAssert(gl->DrawElementsInstancedBaseVertexBaseInstance);
            gl->DrawElementsInstancedBaseVertexBaseInstance(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)dataOff, cmd->instCnt, vtxOff2, cmd->firstInst); _SglThrowErrorOccuried();
        }
        else
        {
            gl->DrawElementsInstancedBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)dataOff, cmd->instCnt, vtxOff2); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->DrawElementsBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)dataOff, vtxOff2); _SglThrowErrorOccuried();
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
    .cmd.BindBuffers = (void*)_SglDpuBindBuffers,
    .cmd.BindRasters = (void*)_SglDpuBindRasters,
    .cmd.ExecuteCommands = (void*)_SglDpuExecuteCommands,

    .cmd.Draw = (void*)_SglDpuDraw,
    .cmd.DrawIndexed = (void*)_SglDpuDrawIndexed,

    .cmd.Transformation.BindVertexInput = (void*)_SglDpuBindVertexInput,
    .cmd.Transformation.BindVertexSources = (void*)_SglDpuBindVertexSources,
    //.cmd.ResetVertexStreams = (void*)_SglDpuResetVertexStreams,
    //.cmd.ResetVertexAttributes = (void*)_SglDpuResetVertexAttributes,
    .cmd.Transformation.BindIndexSource = (void*)_SglDpuBindIndexSource,
    .cmd.Transformation.SetPrimitiveTopology = (void*)_SglDpuSetPrimitiveTopology,
    .cmd.Transformation.ResetViewports = (void*)_SglDpuSetViewports,
    .cmd.Transformation.ReadjustViewports = (void*)_SglDpuSetViewports,
    .cmd.Transformation.SwitchFrontFace = (void*)_SglDpuSwitchFrontFace,
    .cmd.Transformation.SetCullMode = (void*)_SglDpuSetCullMode,

    .cmd.Rasterization.BeginSynthesis = (void*)_SglDpuBeginSynthesis,
    .cmd.Rasterization.FinishSynthesis = (void*)_SglDpuFinishSynthesis,
    .cmd.Rasterization.NextPass = (void*)_SglDpuNextPass,

    .cmd.Rasterization.DisableRasterization = (void*)_SglDpuDisableRasterization,
    .cmd.Rasterization.EnableDepthBias = (void*)_SglDpuEnableDepthBias,
    .cmd.Rasterization.SetDepthBias = (void*)_SglDpuSetDepthBias,
    .cmd.Rasterization.SetLineWidth = (void*)_SglDpuSetLineWidth,
    .cmd.Rasterization.ResetScissors = (void*)_SglDpuSetScissors,
    .cmd.Rasterization.ReadjustScissors = (void*)_SglDpuSetScissors,
    .cmd.Rasterization.ResetAreas = (void*)_SglDpuSetAreas,
    .cmd.Rasterization.ReadjustAreas = (void*)_SglDpuSetAreas,
    .cmd.Rasterization.EnableDepthBoundsTest = NIL,
    .cmd.Rasterization.SetDepthBounds = NIL,
    .cmd.Rasterization.EnableStencilTest = (void*)_SglDpuEnableStencilTest,
    .cmd.Rasterization.SetStencilCompareMask = (void*)_SglDpuSetStencilCompareMask,
    .cmd.Rasterization.SetStencilReference = (void*)_SglDpuSetStencilReference,
    .cmd.Rasterization.SetStencilWriteMask = (void*)_SglDpuSetStencilWriteMask,
    .cmd.Rasterization.EnableDepthTest = (void*)_SglDpuEnableDepthTest,
    .cmd.Rasterization.SetDepthCompareOp = (void*)_SglDpuSetDepthCompareOp,
    .cmd.Rasterization.DisableDepthWrite = (void*)_SglDpuDisableDepthWrite,
    .cmd.Rasterization.SetBlendConstants = (void*)_SglDpuSetBlendConstants,

    .cmd.ras.xform = (void*)_SglDpuRasXform,
    .cmd.ras.swizzle = (void*)_SglDpuRasSwizzle,
    .cmd.ras.mip = (void*)_SglDpuRasSubsample,
    .cmd.ras.cpy = (void*)_SglDpuRasCopy,
    .cmd.ras.pak = (void*)_SglDpuRasPack,

    .cmd.buf.cpy = (void*)_SglDpuBufCpy,
    .cmd.buf.set = (void*)_SglDpuBufSet,
    .cmd.buf.rw = (void*)_SglDpuBufRw,
    .cmd.buf.io = (void*)_SglDpuBufIo
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
                if (cmdHdr->id == NIL/*SGL_CMD_END*/)
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
            if (cmdHdr->id == NIL/*SGL_CMD_END*/)
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

        HDC dc = dpu->wgl.GetCurrentDC();
        HGLRC rc = dpu->wgl.GetCurrentContext();

        if (rc != dpu->glrc || dc != dout->dc)
        {
            if (!dpu->wgl.MakeCurrent(dout->dc, dpu->glrc))
            {
                AfxThrowError();
                AfxError("DPU %u failed to set to draw output device %p.", dpu->portIdx, dout);
                dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc);
            }
            dpu->wgl.SwapIntervalEXT(0); // causes lag when 1
        }


        afxRaster surf;
        AfxGetDrawOutputBuffer(dout, outBufIdx, 1, &surf);
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
        dpu->activeRasterState.pass.canv = NIL;
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, canv->glHandle); _SglThrowErrorOccuried();
#endif
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Clear(GL_COLOR_BUFFER_BIT);  _SglThrowErrorOccuried();
        gl->BlitFramebuffer(0, 0, surf->base.whd[0], surf->base.whd[1], 0, 0, surf->base.whd[0], surf->base.whd[1], GL_COLOR_BUFFER_BIT, GL_LINEAR); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
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
        
        SglSwapBuffers(dout->dc, dpu); // deadlocks all
        //gl->Flush();
        
        //gl->ClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE); _SglThrowErrorOccuried(); // reset GL default

        afxNat currIter, lastFreq;
        AfxGetExecutionCounter(&currIter, &lastFreq);

        if (0 == currIter)
        {
            afxReal64 currTime, deltaTime;
            AfxGetExecutionTime(&currTime, &deltaTime);
            AfxFormatString(&dout->base.caption, "Delta time %0f, IPS %u --- OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build", deltaTime, lastFreq);
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

_SGL afxError _SglDdevProcDpuCb(afxDrawThread dthr, afxDrawContext dctx, afxNat basePort, afxNat portCnt, afxNat baseQue, afxNat queCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(ddev->idd->dpuCnt, basePort, portCnt);

    afxBool ctxEntered = FALSE;

    for (afxNat portIter = 0; portIter < portCnt; portIter++)
    {
        afxNat portIdx = basePort + portIter;
        AfxAssertRange(ddev->idd->dpuCnt, portIdx, 1);
        sglDpuIdd *dpu = &ddev->idd->dpus[portIdx];
        AfxAssertRange(dctx->base.openPortCnt, portIdx, 1);
        //AfxAssertRange(dctx->base.openPorts[portIdx].dqueCnt, baseQue, queCnt);

        queCnt = AfxMin(queCnt, dctx->base.openPorts[portIdx].dqueCnt);
        
        for (afxNat queIter = 0; queIter < queCnt; queIter++)
        {
            afxNat queIdx = baseQue + queIter;
            AfxAssertRange(dctx->base.openPorts[portIdx].dqueCnt, queIdx, 1);
            afxDrawQueue dque = dctx->base.openPorts[portIdx].queues[queIdx];
            AfxAssertObjects(1, &dque, afxFcc_DQUE);
            AfxAssert(portIdx == dque->base.portIdx);

            if (AfxTryEnterSlockExclusive(&dque->base.pendingChainSlock))
            {
                dthr->dque = dque;
                dthr->queueIdx = queIdx;

                if (!ctxEntered)
                {
                    HGLRC rc = dpu->wgl.GetCurrentContext();

                    if (rc == dpu->glrc) ctxEntered = TRUE;
                    else
                    {
                        if (!(ctxEntered = !!dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc)))
                        {
                            AfxThrowError();
                            AfxError("DPU %u failed to take active context.", portIdx);
                        }
                    }

                    if (ctxEntered)
                        dpu->activeDctx = dctx;
                }

                if (ctxEntered)
                {
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
                }
                AfxExitSlockExclusive(&dque->base.pendingChainSlock);
            }
        }
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

    afxDrawContext dctx = cookie->udd[0];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxNat portIdx = *(afxNat const *)cookie->udd[1];
    sglDrawQueueSpecification const *spec = ((sglDrawQueueSpecification const *)cookie->udd[2]);
    afxDrawQueue dque2;

    if (!AfxGetInstance(&ddev->ports[portIdx].queues, AfxIdentifyObject(dque), (afxObject*)&dque2) || dque2 != dque) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dque2, afxFcc_DQUE);
        AfxAssert(spec);

        afxMmu mmu = AfxGetDrawSystemMmu();

        dque->base.portIdx = portIdx;
        dque->base.dctx = dctx;

        dque->base.immediate = 0;// !!spec->immedate;

        AfxTakeSlock(&dque->base.arenaSlock);
        AfxAllocateArena(mmu, &dque->base.cmdArena, NIL, AfxHint());

        dque->base.locked = FALSE;

        AfxSetUpChain(&dque->base.pendingChain, dque);
        AfxTakeSlock(&dque->base.pendingChainSlock);
        AfxTakeSlock(&dque->base.reqLock);

        dque->base.waitCb = _SglDqueWaitCb;

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
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxDrawQueue)),
    .mmu = NIL,
    .ctor = (void*)_SglDqueCtor,
    .dtor = (void*)_SglDqueDtor
};
