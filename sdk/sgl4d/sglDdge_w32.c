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
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/draw/pipe/afxDrawOps.h"

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <dwmapi.h>

extern afxClassConfig _SglDiobMgrCfg;

extern void _DpuSwitchFrontFace(sglDpu* dpu, _sglCmdBool const *cmd);
extern void _DpuSetCullMode(sglDpu* dpu, _sglCmdNat const *cmd);
extern void _DpuSetPrimitiveTopology(sglDpu* dpu, _sglCmdNat const *cmd);
extern void _DpuSetViewports(sglDpu* dpu, _sglCmdViewport const *cmd);
extern void _DpuBindVertexSources(sglDpu* dpu, _sglCmdVertexSources const *cmd);
extern void _DpuBindIndexSource(sglDpu* dpu, _sglCmdBufferRange const *cmd);
extern void _DpuBindVertexInput(sglDpu* dpu, _sglCmdVertexInput *cmd);

extern void _DpuDisableRasterization(sglDpu* dpu, _sglCmdBool const *cmd);
extern void _DpuEnableDepthBias(sglDpu* dpu, _sglCmdBool const *cmd);
extern void _DpuSetDepthBias(sglDpu* dpu, _sglCmdReal3 const *cmd);
extern void _DpuSetLineWidth(sglDpu* dpu, _sglCmdReal const *cmd);
extern void _DpuEnableStencilTest(sglDpu* dpu, _sglCmdBool const *cmd);
extern void _DpuSetStencilCompareMask(sglDpu* dpu, _sglCmdBitmaskNat32 const *cmd);
extern void _DpuSetStencilWriteMask(sglDpu* dpu, _sglCmdBitmaskNat32 const *cmd);
extern void _DpuSetStencilReference(sglDpu* dpu, _sglCmdBitmaskNat32 const *cmd);
extern void _DpuEnableDepthTest(sglDpu* dpu, _sglCmdBool const *cmd);
extern void _DpuSetDepthCompareOp(sglDpu* dpu, _sglCmdNat const *cmd);
extern void _DpuDisableDepthWrite(sglDpu* dpu, _sglCmdBool const *cmd);
extern void _DpuSetBlendConstants(sglDpu* dpu, _sglCmdReal4 const *cmd);
extern void _DpuSetScissors(sglDpu* dpu, _sglCmdScissor const *cmd);
extern void _DpuSetAreas(sglDpu* dpu, _sglCmdArea const *cmd);

extern void _DpuRasSubsample(sglDpu* dpu, _sglCmdRegenerateMipmaps const* cmd);
extern void _DpuRasPack(sglDpu* dpu, _sglCmdPackRasterRegions const* cmd);
extern void _DpuRasCopy(sglDpu* dpu, _sglCmdCopyRasterRegions const* cmd);
extern void _DpuRasSwizzle(sglDpu* dpu, _sglCmdSwizzleRasterRegions const* cmd);
extern void _DpuRasXform(sglDpu* dpu, _sglCmdFlipRasterRegions const* cmd);

extern void _SglDecodeBufCpy(sglDpu* dpu, _sglCmdBufCpy const* cmd);
extern void _SglDecodeBufSet(sglDpu* dpu, _sglCmdBufSet const* cmd);
extern void _SglDecodeBufRw(sglDpu* dpu, _sglCmdBufRw const* cmd);
extern void _SglDecodeBufIo(sglDpu* dpu, _sglCmdBufIo const* cmd);

extern void SglFlushXformStateChanges(sglDpu* dpu);
extern void SglFlushRasterizationStateChanges(sglDpu* dpu);

// RENDERING SCOPE
 
_SGL void _DpuFinishSynthesis(sglDpu* dpu, _sglCmd const *cmd)
{
    (void)cmd;
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    //_DpuBindAndSyncCanv(gl, FALSE, GL_READ_FRAMEBUFFER, 0);
    //_DpuBindAndSyncCanv(gl, FALSE, GL_DRAW_FRAMEBUFFER, 0);
    //gl->Flush(); _SglThrowErrorOccuried();

    afxCanvas canv = dpu->activeRasterState.pass.canv;

    if (canv)
    {
        afxNat surCnt = AfxCountSurfaces(canv);

        if (surCnt)
        {
            AfxAssert(_SGL_MAX_SURF_PER_CANV >= surCnt);
            afxRaster surfaces[_SGL_MAX_SURF_PER_CANV];
            AfxEnumerateDrawBuffers(canv, 0, surCnt, surfaces);

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

_SGL void _DpuBeginSynthesis(sglDpu* dpu, _sglCmdBeginSynthesis const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxCanvas canv = cmd->canv;
    afxDrawTarget const* dt;
    
    //_DpuBindAndSyncCanv(gl, bindCanv, TRUE, GL_READ_FRAMEBUFFER, canv);
    _DpuBindAndSyncCanv(dpu, /*(dpu->activeRasterState.pass.canv != canv)*/TRUE, TRUE, GL_DRAW_FRAMEBUFFER, canv);
    //dpu->activeRasterState.pass.canv = canv;

    if (!canv)
    {
        if (cmd->defFbo)
        {
            dt = &cmd->raster0;

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
            dt = i ? &cmd->raster_[i] : &cmd->raster0;

            switch (dt->storeOp)
            {
            case afxSurfaceStoreOp_STORE:
            {
                enabledToDraw[enabledToDrawCnt] = GL_COLOR_ATTACHMENT0 + i;
                enabledToDrawCnt++;
                storeBitmask |= AFX_BIT_OFFSET(i);
                break;
            }
            case afxSurfaceStoreOp_DISCARD:
            {
                enabledToDraw[enabledToDrawCnt] = GL_NONE;
                enabledToDrawCnt++;
                storeBitmask |= AFX_BIT_OFFSET(8 + i);
                break;
            }
            case afxSurfaceStoreOp_DONT_CARE:
            default:
            {
                enabledToDraw[enabledToDrawCnt] = GL_NONE;
                enabledToDrawCnt++;
                storeBitmask |= AFX_BIT_OFFSET(16 + i);
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
            dt = i ? &cmd->raster_[i] : &cmd->raster0;

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

_SGL void _DpuNextPass(sglDpu* dpu, _sglCmd const *cmd)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = ddge->dpu;
    (void)cmd;
    //++(dpu->state.renderPass.activeSubpass);
}

// STATE SETTING

_SGL void _DpuBindPipeline(sglDpu* dpu, _sglCmdPipeline *cmd)
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
                AfxCopy2(config.colorOutCnt, sizeof(config.colorOuts[0]), config.colorOuts, dpu->nextRasterState.outs);
            
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
                AfxCopy2(config.sampleCnt, sizeof(config.sampleMasks), config.sampleMasks, dpu->nextRasterState.sampleMasks);
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

_SGL void _DpuBindBuffers(sglDpu* dpu, _sglCmdBindBuffers const *cmd)
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
        dpu->nextResBindUpdMask[setIdx] |= AFX_BIT_OFFSET(entryIdx);
    }
}

_SGL void _DpuBindRasters(sglDpu* dpu, _sglCmdBindRasters const *cmd)
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
        dpu->nextResBindUpdMask[setIdx] |= AFX_BIT_OFFSET(entryIdx);
    }
}



_SGL void _DpuUpdateUniformVector(sglDpu* dpu, _sglCmdUniformVectorEXT const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLuint loc = gl->GetUniformLocation(dpu->activePip->glHandle, cmd->name);
    gl->Uniform4fv(loc, cmd->cnt, cmd->v);
}

_SGL void _DpuUpdateUniformMatrix(sglDpu* dpu, _sglCmdUniformMatrixEXT const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    GLuint loc = gl->GetUniformLocation(dpu->activePip->glHandle, cmd->name);
    //gl->UniformMatrix4fv(loc, cmd->cnt, GL_FALSE, cmd->m);
    AfxThrowError();
}

// DO WORK

_SGL void _SglFlushResourcingStateChanges(sglDpu* dpu)
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
                
                //if (updMask & AFX_BIT_OFFSET(j))
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
                    DpuBindAndSyncBuf(dpu, sglBindFlag_SYNC, glTarget, buf, offset, range, 0, /*buf->glUsage ? buf->glUsage : */GL_DYNAMIC_DRAW);
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

                //if (updMask & AFX_BIT_OFFSET(j))
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
                        DpuBindAndSyncRas(dpu, sglBindFlag_SYNC | sglBindFlag_KEEP | sglBindFlag_BIND, glUnit, ras);
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
                        _DpuBindAndSyncSamp(dpu, sglBindFlag_SYNC | sglBindFlag_KEEP | sglBindFlag_BIND, glUnit, samp);
                        gl->BindSampler(glUnit, samp->glHandle); _SglThrowErrorOccuried();
                    }
                }
                break;
            }
            default:
            {
                AfxLogError("");
            }
            }
        }
        dpu->nextResBindUpdMask[setIdx] = NIL;
    }
}

_SGL void _DpuDraw(sglDpu* dpu, _sglCmdDraw const *cmd)
{
    afxError err = AFX_ERR_NONE;    
    glVmt const* gl = &dpu->gl;
    afxNat cnt;

    {
        _DpuBindAndSyncPip(dpu, (dpu->activePip != dpu->nextPip), TRUE, dpu->nextPip);
        dpu->activePip = dpu->nextPip;
        //dpu->activeVertexInput = &dpu->activePip->vertexInput;
    }
    
    //if (dpu->activeVin != dpu->nextVin)
    {
        dpu->activeVin = dpu->nextVin;
        _DpuBindAndSyncVin(dpu, dpu->activeVin, &dpu->nextVinBindings);
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

    //AfxLogEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _DpuDrawIndexed(sglDpu* dpu, _sglCmdDrawIndexed const* cmd)
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
        _DpuBindAndSyncPip(dpu, (dpu->activePip != dpu->nextPip), TRUE, dpu->nextPip);
        dpu->activePip = dpu->nextPip;
        //dpu->activeVertexInput = &dpu->activePip->vertexInput;        
    }

    //if (dpu->activeVin != dpu->nextVin)
    {
        dpu->activeVin = dpu->nextVin;
        _DpuBindAndSyncVin(dpu, dpu->activeVin, &dpu->nextVinBindings);
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
    
    //AfxLogEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_SGL void _DpuExecuteCommands(sglDpu* dpu, _sglCmdExecCmds const* cmd);

union
{
    afxCmd  cmd;
    void(*f[SGL_CMD_TOTAL])(sglDpu* dpu, _sglCmd *cmd);
}
_DpuVmt =
{
    .cmd.BindPipeline = (void*)_DpuBindPipeline,
    .cmd.BindBuffers = (void*)_DpuBindBuffers,
    .cmd.BindRasters = (void*)_DpuBindRasters,
    .cmd.ExecuteCommands = (void*)_DpuExecuteCommands,

    .cmd.Draw = (void*)_DpuDraw,
    .cmd.DrawIndexed = (void*)_DpuDrawIndexed,

    .cmd.Transformation.BindVertexInput = (void*)_DpuBindVertexInput,
    .cmd.Transformation.BindVertexSources = (void*)_DpuBindVertexSources,
    //.cmd.ResetVertexStreams = (void*)_DpuResetVertexStreams,
    //.cmd.ResetVertexAttributes = (void*)_DpuResetVertexAttributes,
    .cmd.Transformation.BindIndexSource = (void*)_DpuBindIndexSource,
    .cmd.Transformation.SetPrimitiveTopology = (void*)_DpuSetPrimitiveTopology,
    .cmd.Transformation.ResetViewports = (void*)_DpuSetViewports,
    .cmd.Transformation.ReadjustViewports = (void*)_DpuSetViewports,
    .cmd.Transformation.SwitchFrontFace = (void*)_DpuSwitchFrontFace,
    .cmd.Transformation.SetCullMode = (void*)_DpuSetCullMode,

    .cmd.Rasterization.BeginSynthesis = (void*)_DpuBeginSynthesis,
    .cmd.Rasterization.FinishSynthesis = (void*)_DpuFinishSynthesis,
    .cmd.Rasterization.NextPass = (void*)_DpuNextPass,

    .cmd.Rasterization.DisableRasterization = (void*)_DpuDisableRasterization,
    .cmd.Rasterization.EnableDepthBias = (void*)_DpuEnableDepthBias,
    .cmd.Rasterization.SetDepthBias = (void*)_DpuSetDepthBias,
    .cmd.Rasterization.SetLineWidth = (void*)_DpuSetLineWidth,
    .cmd.Rasterization.ResetScissors = (void*)_DpuSetScissors,
    .cmd.Rasterization.ReadjustScissors = (void*)_DpuSetScissors,
    .cmd.Rasterization.ResetAreas = (void*)_DpuSetAreas,
    .cmd.Rasterization.ReadjustAreas = (void*)_DpuSetAreas,
    .cmd.Rasterization.EnableDepthBoundsTest = NIL,
    .cmd.Rasterization.SetDepthBounds = NIL,
    .cmd.Rasterization.EnableStencilTest = (void*)_DpuEnableStencilTest,
    .cmd.Rasterization.SetStencilCompareMask = (void*)_DpuSetStencilCompareMask,
    .cmd.Rasterization.SetStencilReference = (void*)_DpuSetStencilReference,
    .cmd.Rasterization.SetStencilWriteMask = (void*)_DpuSetStencilWriteMask,
    .cmd.Rasterization.EnableDepthTest = (void*)_DpuEnableDepthTest,
    .cmd.Rasterization.SetDepthCompareOp = (void*)_DpuSetDepthCompareOp,
    .cmd.Rasterization.DisableDepthWrite = (void*)_DpuDisableDepthWrite,
    .cmd.Rasterization.SetBlendConstants = (void*)_DpuSetBlendConstants,

    .cmd.ras.xform = (void*)_DpuRasXform,
    .cmd.ras.swizzle = (void*)_DpuRasSwizzle,
    .cmd.ras.mip = (void*)_DpuRasSubsample,
    .cmd.ras.cpy = (void*)_DpuRasCopy,
    .cmd.ras.pak = (void*)_DpuRasPack,

    .cmd.buf.cpy = (void*)_SglDecodeBufCpy,
    .cmd.buf.set = (void*)_SglDecodeBufSet,
    .cmd.buf.rw = (void*)_SglDecodeBufRw
};

_SGL void _DpuExecuteCommands(sglDpu* dpu, _sglCmdExecCmds const* cmd)
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxDrawStream diob = cmd->subsets[i];
        AfxAssertObjects(1, &diob, afxFcc_DIOB);
        AfxAssert(diob->base.state == afxDrawStreamState_PENDING);

        if (diob->base.state == afxDrawStreamState_PENDING)
        {
            _sglCmd *cmdHdr;
            AfxChainForEveryLinkageB2F(&diob->commands, _sglCmd, script, cmdHdr)
            {
                if (cmdHdr->id == NIL/*SGL_CMD_END*/)
                    break;

                if (diob->base.state != afxDrawStreamState_PENDING)
                {
                    AfxThrowError();
                    break;
                }

                _DpuVmt.f[cmdHdr->id](dpu, cmdHdr);
            }

            if (!err)
            {
                diob->base.state = afxDrawStreamState_EXECUTABLE;
            }

            if (err || diob->base.disposable)
            {
                AfxAssert(diob->base.portIdx == dpu->portIdx);
                diob->base.state = afxDrawStreamState_INVALID;
            }
        }
    }
}

// QUEUE STUFF

_SGL afxError _DpuExecute(sglDpu* dpu, afxDrawBridge ddge, afxNat queIdx, _sglQueueingExecute* subm)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(ddge, afxFcc_DDGE);
    //glVmt const* gl = &thdIdd->wglVmt;

    for (afxNat i = 0; i < subm->itemCnt; i++)
    {
        afxDrawStream diob = subm->items[i].diob;
        AfxAssertObjects(1, &diob, afxFcc_DIOB);

        if (diob->base.state == afxDrawStreamState_PENDING)
        {
            AfxAssert(diob->base.state == afxDrawStreamState_PENDING);

            _sglCmd * cmdHdr;
            AfxChainForEveryLinkageB2F(&diob->commands, _sglCmd, script, cmdHdr)
            {
                if (cmdHdr->id == NIL/*SGL_CMD_END*/)
                    break;

                if (diob->base.state != afxDrawStreamState_PENDING)
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

                _DpuVmt.f[cmdHdr->id](dpu, cmdHdr);
            }

            if (!err)
            {
                diob->base.state = afxDrawStreamState_EXECUTABLE;
            }

            if (err || diob->base.disposable)
            {
                AfxAssert(diob->base.portIdx == dpu->portIdx);
                diob->base.state = afxDrawStreamState_INVALID;

                afxNat diobQueIdx = diob->base.queIdx;

                AfxEnterSlockExclusive(&ddge->base.queues[diobQueIdx].reqLock);

                if (AfxPushQueueUnit(&ddge->base.queues[diobQueIdx].recycQue, &diob))
                {
                    AfxReleaseObjects(1, (void**)&diob);
                }

                AfxExitSlockExclusive(&ddge->base.queues[diobQueIdx].reqLock);
            }
        }
    }
    return err;
}

_SGL afxError _DpuMmap(sglDpu* dpu, afxDrawBridge ddge, afxNat queIdx, _sglQueueingMmap* subm)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(ddge, afxFcc_DDGE);
    glVmt const* gl = &dpu->gl;
    
    GLenum glTarget = NIL;

    if (subm->buf->base.access & afxBufferAccess_W)
        glTarget = GL_COPY_WRITE_BUFFER;
    else
        glTarget = GL_COPY_READ_BUFFER;

    DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_SYNC | sglBindFlag_KEEP, glTarget, subm->buf, subm->off, subm->ran, 0, GL_INVALID_ENUM);
    
    if (subm->ran)
    {
        GLenum glAccess = NIL;
        AfxAssert(!subm->buf->base.bytemap);

#if !0
        if (subm->ran == AfxGetBufferCapacity(subm->buf))
        {
            if (!(subm->buf->base.access & afxBufferAccess_R))
            {
                if (subm->buf->base.access & afxBufferAccess_W)
                    glAccess = GL_WRITE_ONLY;
            }
            else
            {
                if (subm->buf->base.access & afxBufferAccess_W)
                    glAccess = GL_READ_WRITE;
                else
                    glAccess = GL_READ_ONLY;
            }

            subm->buf->base.bytemap = gl->MapBuffer(glTarget, glAccess); _SglThrowErrorOccuried();
        }
        else
#endif
        {
            subm->buf->base.bytemap = gl->MapBufferRange(glTarget, subm->off, subm->ran, subm->buf->glAccess); _SglThrowErrorOccuried();
        }
        AfxAssert(subm->buf->base.bytemap);
    }
    else
    {
        AfxAssert(subm->buf->base.bytemap);
        gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
        //gl->FlushMappedBuffer(glTarget, subm->off, subm->ran); _SglThrowErrorOccuried();
        subm->buf->base.bytemap = NIL;
    }

    return err;
}

_SGL afxError _DpuTransfer(sglDpu* dpu, afxDrawBridge ddge, afxNat queIdx, _sglQueueingTransfer* subm)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(ddge, afxFcc_DDGE);
    glVmt const* gl = &dpu->gl;

    for (afxNat i = 0; i < subm->itemCnt; i++)
    {
        afxTransferRequest* req = &subm->items[i];
        
        switch (req->srcFcc)
        {
        case NIL:
        {
            switch (req->dstFcc)
            {
            case afxFcc_BUF: // raw to buf
            {
                GLenum glTarget = GL_COPY_WRITE_BUFFER;
                GLenum glAccess = GL_MAP_WRITE_BIT;
                DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, req->data.buf, req->data.offset, req->data.range, 0, GL_INVALID_ENUM);
                gl->BufferSubData(glTarget, req->data.offset, req->data.range, &(((afxByte*)req->data.src)[req->data.dataOff])); _SglThrowErrorOccuried();
                break;
            }
            case afxFcc_RAS: // raw to ras
            {
                afxRaster ras = req->img.ras;
                DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
                gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_UNPACK_ROW_LENGTH, req->img.rowSiz); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, req->img.rowCnt); _SglThrowErrorOccuried();
                _SglTexSubImage(gl, ras->glTarget, req->img.rgn.lodIdx, req->img.rgn.baseLayer, req->img.rgn.layerCnt, req->img.rgn.origin, req->img.rgn.whd, ras->glFmt, ras->glType, &(((afxByte*)req->img.src)[req->img.offset]));
                break;
            }
            default: AfxThrowError(); break;
            }
            break;
        }
        case afxFcc_BUF:
        {
            switch (req->dstFcc)
            {
            case NIL: // buf to raw
            {
                GLenum glTarget = GL_COPY_READ_BUFFER;
                GLenum glAccess = GL_MAP_READ_BIT;
                DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, req->data.buf, req->data.offset, req->data.range, 0, GL_INVALID_ENUM);
                gl->GetBufferSubData(glTarget, req->data.offset, req->data.range, &(((afxByte*)req->data.dst)[req->data.dataOff])); _SglThrowErrorOccuried();
                break;
            }
            case afxFcc_IOB: // buf to iob
            {
                GLenum glTarget = GL_COPY_READ_BUFFER;
                GLenum glAccess = GL_MAP_READ_BIT;
                DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, req->data.buf, req->data.offset, req->data.range, 0, GL_INVALID_ENUM);

                void* src = gl->MapBufferRange(glTarget, req->data.offset, req->data.range, glAccess); _SglThrowErrorOccuried();
                AfxAssert(src);

                AfxWriteStreamAt(req->data.iob, req->data.dataOff, req->data.range, 0, src);

                gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
                break;
            }
            default: AfxThrowError(); break;
            }
            break;
        }
        case afxFcc_RAS:
        {
            switch (req->dstFcc)
            {
            case NIL: // ras to raw
            case afxFcc_IOB: // ras to iob
            {
                afxRaster ras = req->img.ras;
                afxRasterRegion const* rgn = &req->img.rgn;
                DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

                afxNat bufSiz = req->img.rowSiz * req->img.rowCnt * req->img.rgn.whd[2] * req->img.rgn.layerCnt;

                GLuint pbo;
                GLenum pboTarget = GL_PIXEL_PACK_BUFFER;
                gl->GenBuffers(1, &pbo); _SglThrowErrorOccuried();
                gl->BindBuffer(pboTarget, pbo); _SglThrowErrorOccuried();
                gl->BufferStorage(pboTarget, bufSiz, NIL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT); _SglThrowErrorOccuried();

                gl->PixelStorei(GL_PACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_PACK_ROW_LENGTH, req->img.rowSiz); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, req->img.rowCnt); _SglThrowErrorOccuried();

                gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc); _SglThrowErrorOccuried();
                _SglBindFboAttachment(&dpu->gl, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ras->glTarget, ras->glHandle, rgn->lodIdx, rgn->baseLayer, rgn->origin[2]);
                gl->ReadBuffer(GL_COLOR_ATTACHMENT0); _SglThrowErrorOccuried();
                gl->ReadPixels(rgn->origin[0], rgn->origin[1], rgn->whd[0], rgn->whd[1], ras->glFmt, ras->glType, NIL); _SglThrowErrorOccuried();

                if (req->dstFcc == NIL)
                {
                    gl->GetBufferSubData(pboTarget, 0, bufSiz, &(((afxByte*)req->img.dst)[req->img.offset])); _SglThrowErrorOccuried();
                }
                else
                {
                    afxByte* bytemap = gl->MapBuffer(pboTarget, GL_READ_ONLY); _SglThrowErrorOccuried();
                    AfxSeekStreamFromBegin(req->img.iob, req->img.offset);
                    AfxWriteStream(req->img.iob, bufSiz, 0, bytemap);
                    gl->UnmapBuffer(pboTarget); _SglThrowErrorOccuried();
                }

                gl->BindBuffer(pboTarget, 0); _SglThrowErrorOccuried();
                gl->DeleteBuffers(1, &pbo); _SglThrowErrorOccuried();
                break;
            }
            default: AfxThrowError(); break;
            }
            break;
        }
        case afxFcc_IOB:
        {
            switch (req->dstFcc)
            {
            case afxFcc_BUF: // iob to buf
            {
                GLenum glTarget = GL_COPY_WRITE_BUFFER;
                GLenum glAccess = GL_MAP_WRITE_BIT;
                DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, glTarget, req->data.buf, req->data.offset, req->data.range, 0, GL_INVALID_ENUM);

                afxByte* dst = gl->MapBufferRange(glTarget, req->data.offset, req->data.range, glAccess); _SglThrowErrorOccuried();
                AfxAssert(dst);

                AfxReadStreamAt(req->data.iob, req->data.dataOff, req->data.range, 0, dst);

                gl->UnmapBuffer(glTarget); _SglThrowErrorOccuried();
                //gl->FlushMappedBufferRange(glTarget, op->dstOffset, op->range); _SglThrowErrorOccuried();
                break;
            }
            case afxFcc_RAS: // iob to ras
            {
                afxRaster ras = req->img.ras;
                afxRasterRegion const* rgn = &req->img.rgn;
                DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

                afxNat bufSiz = req->img.rowSiz * req->img.rowCnt * req->img.rgn.whd[2] * req->img.rgn.layerCnt;

                GLuint pbo;
                GLenum pboTarget = GL_PIXEL_UNPACK_BUFFER;
                gl->GenBuffers(1, &pbo); _SglThrowErrorOccuried();
                gl->BindBuffer(pboTarget, pbo); _SglThrowErrorOccuried();
                gl->BufferStorage(pboTarget, bufSiz, NIL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT); _SglThrowErrorOccuried();
                afxByte* bytemap = gl->MapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY); _SglThrowErrorOccuried();
                AfxSeekStreamFromBegin(req->img.iob, req->img.offset);
                AfxReadStream(req->img.iob, bufSiz, 0, bytemap);
                gl->UnmapBuffer(pboTarget); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_UNPACK_ROW_LENGTH, req->img.rowSiz); _SglThrowErrorOccuried();
                gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, req->img.rowCnt); _SglThrowErrorOccuried();
                _SglTexSubImage(gl, ras->glTarget, rgn->lodIdx, rgn->baseLayer, rgn->layerCnt, rgn->origin, rgn->whd, ras->glFmt, ras->glType, NIL);
                gl->BindBuffer(pboTarget, 0); _SglThrowErrorOccuried();
                gl->DeleteBuffers(1, &pbo); _SglThrowErrorOccuried();
                break;
            }
            default: AfxThrowError(); break;
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_SGL afxError _DpuPresent(sglDpu* dpu, afxDrawBridge ddge, afxNat queIdx, _sglQueueingPresent* subm)
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < subm->itemCnt; i++)
    {
        afxDrawOutput dout = subm->items[i].dout;
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        afxNat outBufIdx = subm->items[i].bufIdx;

        afxDrawContext dctx = dpu->activeDctx;
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        AfxAssert(!dout->swapping);

        if (_SglActivateDout(dpu, dout)) AfxThrowError();
        else
        {
            dout->swapping = TRUE;

            //wglVmt const*wgl = &dpu->wgl;


            afxRaster surf;
            AfxEnumerateDrawOutputBuffers(dout, outBufIdx, 1, &surf);
            afxCanvas canv;
            AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
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
            afxBool useLerp = (surf->base.whd[0] != canv->base.wh[0]) && (surf->base.whd[1] != canv->base.wh[1]);
            gl->BlitFramebuffer(0, 0, surf->base.whd[0], surf->base.whd[1], 0, 0, surf->base.whd[0], surf->base.whd[1], GL_COLOR_BUFFER_BIT, useLerp ? GL_LINEAR : GL_NEAREST); _SglThrowErrorOccuried();
            gl->BindFramebuffer(GL_READ_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
            gl->Flush(); _SglThrowErrorOccuried();
#else

#if !0
            AfxAssert(gl->ClipControl);

            //gl->ClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE); _SglThrowErrorOccuried();// set screen origin to top to bottm, and set depth to [ 0, 1 ]

            afxWhd extent;
            AfxGetRasterExtent(surf, 0, extent);

            _sglCmdBeginSynthesis cmdBeginOp = { 0 };
            cmdBeginOp.defFbo = TRUE;
            cmdBeginOp.canv = NIL;
            cmdBeginOp.area = (afxRect const) { { { 0, 0 } }, { { extent[0], extent[1] } } };
            cmdBeginOp.rasterCnt = 1;
            cmdBeginOp.rasters[0] = (afxDrawTarget const) { .loadOp = afxSurfaceLoadOp_CLEAR, .storeOp = afxSurfaceStoreOp_STORE, .clearValue = { .color = { 0.3, 0.1, 0.3, 1 } } };

            _DpuBeginSynthesis(dpu, &cmdBeginOp);

            _sglCmdPipeline cmdBindPip = { 0 };
            cmdBindPip.pip = dctx->presentPip;
            _DpuBindPipeline(dpu, &cmdBindPip);

#else
            afxDrawTarget const rasterRt = { NIL, afxSurfaceLoadOp_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
            _DpuBeginCombination(dpu, NIL, 1, 0, &rasterRt, NIL, NIL);

            afxPipeline pip = AfxDrawOperationGetPipeline(idd->presentDop, 0, 0);
            AfxAssertObjects(1, &pip, afxFcc_PIP);
            _DpuBindPipeline(dpu, pip);
#endif   
            //_DpuEmployTechnique(ddge, 0);

            _sglCmdViewport cmdSetVp = { 0 };
            cmdSetVp.cnt = 1;
            cmdSetVp.vp[0].extent[0] = extent[0];
            cmdSetVp.vp[0].extent[1] = extent[1];
            cmdSetVp.vp[0].depth[1] = 1;
            cmdSetVp.reset = TRUE;

            _DpuSetViewports(dpu, &cmdSetVp);

#if 0 // already set by pipeline
            afxPipelineRasterizerState const ras = { FALSE, FALSE, afxFillMode_SOLID, afxCullMode_BACK, afxFrontFace_CCW, FALSE, 0, 0, 0, 1 };
            _DpuSetRasterizerState(ddge, &ras);

            afxPipelineDepthState const depth = { 0 };
            _DpuSetDepthState(ddge, &depth);
#endif

#if 0 // already set by pipeline
            afxPipelinePrimitiveState ia;
            ia.topology = afxPrimTopology_TRI_STRIP;
            ia.primRestartEnable = FALSE;
            _DpuSetInputAssemblyState(ddge, &ia);
#endif
#if 0
            afxNat const baseVtxs[] = { 0, 0 };
            afxNat const vtxArrs[] = { 0, 1 };
            afxVertexBuffer vbufs[] = { idd->rgbRectVbo, idd->rgbRectVbo };
            _DpuBindVertexBuffers(ddge, 0, 2, vbufs, baseVtxs, vtxArrs);
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
            _DpuResetVertexAttributes(dpu, &vtxAttrs);
            _sglCmdVertexStreams const vtxStreams =
            {
                .cnt = 1,
                .srcIdx[0] = 7,
                .stride[0] = sizeof(afxV2d),
                .instance[0] = FALSE
            };
            _DpuResetVertexStreams(dpu, &vtxStreams);
            _sglCmdVertexSources bindVbuf = { .first = 7,.cnt = 1,.offset[0] = 0,.range[0] = sizeof(afxV2d) * 4 };
            bindVbuf.buf[0] = dctx->tristrippedQuad2dPosBuf;
            _DpuBindVertexSources(dpu, &bindVbuf);
#endif
            //_DpuBindLegos(ddge, 0, 1, &idd->presentLego);
            _sglCmdBindRasters cmdBindTex = { 0 };
            cmdBindTex.first = 0;
            cmdBindTex.cnt = 1;
            cmdBindTex.smp[0] = dctx->presentSmp;
            cmdBindTex.tex[0] = surf;
            _DpuBindRasters(dpu, &cmdBindTex);

            _sglCmdDraw cmdDraw = { 0 };
            cmdDraw.vtxCnt = 4;
            cmdDraw.instCnt = 1;
            _DpuDraw(dpu, &cmdDraw);

#if !0
            _DpuFinishSynthesis(dpu, NIL);
#else
            _DpuEndCombination(dpu, NIL);
#endif
#endif

            HDC dc = dout->w32.hDc;

            if (dc)
            {
                //afxNat cnt = dout->refreshRate;
                //while (--cnt) DwmFlush();
                SglSwapBuffers(dc, &dpu->wgl); // deadlocks all
                Sleep(1);
                AfxYield();
            }
            //gl->Flush();

            //gl->ClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE); _SglThrowErrorOccuried(); // reset GL default

            afxClock currClock;
            AfxGetClock(&currClock);

            if (1.0 > AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)) ++dout->outNo;
            else
            {
                dout->outCntResetClock = currClock;
                dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
                dout->outNo = 0;
            }
            afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
            afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
            dout->lastClock = currClock;

            if (0 == dout->outNo)
            {
                //AfxFormatOverlayCaption(ovly, "Delta time %0f, IPS %u --- OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build", deltaTime, lastFreq);
            }

            if (dout->endpointNotifier)
            {
                dout->endpointNotifier(dout->endpointObj, outBufIdx);
            }
            dout->swapping = FALSE;
            dout->buffers[outBufIdx].booked = FALSE;
            dout->buffers[outBufIdx].booked = FALSE;
            dout->buffers[outBufIdx].booked = FALSE;
        }
    }
    return err;
}

_SGL afxError _DpuStamp(sglDpu* dpu, afxDrawBridge ddge, afxNat queIdx, _sglQueueingStamp* subm)
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < subm->itemCnt; i++)
    {
        afxDrawOutput dout = subm->items[i].dout;
        AfxAssertObjects(1, &dout, afxFcc_DOUT);
        afxNat outBufIdx = subm->items[i].bufIdx;

        afxDrawContext dctx = dpu->activeDctx;
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxWhd whd;
        afxCanvas canv;
        AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
        AfxGetCanvasExtent(canv, whd);

        _sglCmdBeginSynthesis cmdBeginOp = { 0 };
        cmdBeginOp.canv = canv;
        cmdBeginOp.layerCnt = 1;
        cmdBeginOp.rasterCnt = 1;
        cmdBeginOp.raster0 = (afxDrawTarget const) { .loadOp = afxSurfaceLoadOp_LOAD, .storeOp = afxSurfaceStoreOp_STORE };
        cmdBeginOp.area.extent[0] = whd[0];
        cmdBeginOp.area.extent[1] = whd[1];
        _DpuBeginSynthesis(dpu, &cmdBeginOp);

        _sglCmdViewport cmdSetVp = { 0 };
        cmdSetVp.cnt = 1;
        cmdSetVp.vp0.extent[0] = whd[0];
        cmdSetVp.vp0.extent[1] = whd[1];
        cmdSetVp.vp0.depth[1] = 1;
        cmdSetVp.reset = TRUE;
        _DpuSetViewports(dpu, &cmdSetVp);

        _sglCmdPipeline cmdBindPip = { 0 };
        cmdBindPip.pip = dctx->fntPip;
        _DpuBindPipeline(dpu, &cmdBindPip);

        awxViewConstants vc;
        AfxResetM4d(vc.v);
        AfxComputeOffcenterOrthographicMatrix(vc.p, 0, cmdSetVp.vp0.extent[0], 0, cmdSetVp.vp0.extent[1], -1.f, 1.f, &AFX_CLIP_SPACE_OPENGL);
        AfxUpdateBuffer(dctx->fntUnifBuf, 0, sizeof(vc), &vc);

        _sglCmdBindBuffers cmdBindBufs = { 0 };
        cmdBindBufs.cnt = 1;
        cmdBindBufs.buf[0] = dctx->fntUnifBuf;
        _DpuBindBuffers(dpu, &cmdBindBufs);

        _sglCmdBindRasters cmdBindTex = { 0 };
        cmdBindTex.first = 1;
        cmdBindTex.cnt = 1;
        cmdBindTex.smp[0] = dctx->fntSamp;
        cmdBindTex.tex[0] = dctx->fntRas;
        _DpuBindRasters(dpu, &cmdBindTex);

        afxReal x = subm->origin[0];
        afxReal y = subm->origin[1];
        afxNat numchar = subm->caption.str.str.len;
        afxReal r = 1, g = 1, b = 1;
        afxReal x2 = x;

        afxReal* verts = AfxMapBuffer(dctx->fntDataBuf, 0, 2048, afxBufferAccess_W);
        void const* bufStart = verts;

        for (char const *ptr = subm->caption.str.str.start, i = 0; *ptr != '\0'; ptr++)
        {
            // Decrement 'y' for any CR's
            if (*ptr == '\n')
            {
                x = x2;
                y -= 16;
                continue;
            }

#if 0
            // Just advance spaces instead of rendering empty quads
            if (*ptr == ' ')
            {
                x += 16;
                numchar--;
                continue;
            }
#endif

            // ANSI color escape codes
            // I'm sure there's a better way to do this!
            // But it works, so whatever.
            if (*ptr == '\x1B')
            {
                ptr++;

                if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '0'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.0f;	b = 0.0f; }	// BLACK
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '1'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.0f;	b = 0.0f; }	// DARK RED
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '2'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.5f;	b = 0.0f; }	// DARK GREEN
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '3'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.5f;	b = 0.0f; }	// DARK YELLOW
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '4'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.0f;	b = 0.5f; }	// DARK BLUE
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '5'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.0f;	b = 0.5f; }	// DARK MAGENTA
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '6'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.5f;	b = 0.5f; }	// DARK CYAN
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '7'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.5f;	b = 0.5f; }	// GREY
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '0'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.5f;	b = 0.5f; }	// GREY
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '1'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 0.0f;	b = 0.0f; }	// RED
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '2'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 1.0f;	b = 0.0f; }	// GREEN
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '3'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 1.0f;	b = 0.0f; }	// YELLOW
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '4'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.0f;	b = 1.0f; }	// BLUE
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '5'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 0.0f;	b = 1.0f; }	// MAGENTA
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '6'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 1.0f;	b = 1.0f; }	// CYAN
                else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '7'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 1.0f;	b = 1.0f; }	// WHITE
                ptr += 4;
            }

            // Emit position, atlas offset, and color for this character
            *verts++ = x;
            *verts++ = y;
            *verts++ = (float)(*ptr % 16)*0.0625f; // *verts++ = (float)(*ptr % 16)*0.0625f;
            *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f; // *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f;
            //*verts++ = r;
            //*verts++ = g;
            //*verts++ = b;
            *verts++ = 1.f;

            // Advance one character
            x += 16;
        }

        AfxUnmapBuffer(dctx->fntDataBuf);

        _sglCmdVertexSources cmdVtxSrcs = { 0 };
        cmdVtxSrcs.cnt = 1;
        cmdVtxSrcs.source0.buf = dctx->fntDataBuf;
        cmdVtxSrcs.source0.stride = 20;
        _DpuBindVertexSources(dpu, &cmdVtxSrcs);

        _sglCmdDraw cmdDraw = { 0 };
        cmdDraw.instCnt = numchar;
        cmdDraw.vtxCnt = 4;
        _DpuDraw(dpu, &cmdDraw);

        _DpuFinishSynthesis(dpu, NIL);
    }
    return err;
}

_SGL void* _AfxDqueRequestArenaSpace(afxDrawBridge ddge, afxNat queIdx, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxDrawQueue* dque = &ddge->base.queues[queIdx];

    AfxEnterSlockExclusive(&dque->workArenaSlock);

    void *block = AfxRequestArenaUnit(&dque->workArena, siz);

    if (!block)
        AfxThrowError();

    AfxExitSlockExclusive(&dque->workArenaSlock);

    return block;
}

_SGL void _AfxDqueRecycleArenaSpace(afxDrawBridge ddge, afxNat queIdx, void *block, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxDrawQueue* dque = &ddge->base.queues[queIdx];

    AfxEnterSlockExclusive(&dque->workArenaSlock);

    AfxAssert(block);

    AfxRecycleArenaUnit(&dque->workArena, block, siz);

    AfxExitSlockExclusive(&dque->workArenaSlock);
}

_SGL afxError _SglDdevProcPortUnlocked(sglDpu *dpu, afxDrawBridge ddge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    afxBool ctxEntered = FALSE;
    afxNat portIdx = ddge->base.portIdx;
    afxDrawContext dctx = AfxGetDrawBridgeContext(ddge);

    if (!ctxEntered)
    {
        HGLRC rc = dpu->wgl.GetCurrentContext();

        if (rc == dpu->glrc) ctxEntered = TRUE;
        else
        {
            if (!(ctxEntered = !!dpu->wgl.MakeCurrent(dpu->dc, dpu->glrc)))
            {
                AfxThrowError();
                AfxLogError("DPU %u failed to take active context.", portIdx);
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

        afxDrawQueue* dque = &ddge->base.queues[queIdx];

        _sglQueueing* subm;
        AfxChainForEveryLinkageB2F(&dque->workChn, _sglQueueing, chain, subm)
        {
            AfxGetTime(&subm->pullTime);

            subm->exec(dpu, ddge, queIdx, subm);

            AfxGetTime(&subm->complTime);

            AfxPopLinkage(&subm->chain);

            //AfxEnterSlockExclusive(&dctxD->ports[ddge->portIdx].recyclSubmChainSlock);

            //if (dctxD->ports[ddge->portIdx].minRecyclSubmCnt > AfxGetChainLength(&dctxD->ports[ddge->portIdx].recyclSubmChain))
            {
                //AfxPushLinkage(&subm->chain, &dctxD->ports[ddge->portIdx].recyclSubmChain);
            }
            //else
            {
                _AfxDqueRecycleArenaSpace(ddge, queIdx, subm, subm->siz);
            }
            //AfxExitSlockExclusive(&dctxD->ports[ddge->portIdx].recyclSubmChainSlock);
        }
        //AfxSignalCondition2(&dque->idleCnd);
    }
}

_SGL afxError _DdgeProcCb(afxDrawBridge ddge, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxDrawDevice ddev = AfxGetObjectProvider(ddge);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxNat portIdx = ddge->base.portIdx;
    AfxAssertRange(ddev->idd->dpuCnt, portIdx, 1);
    sglDpu *dpu = &ddev->idd->dpus[portIdx];
        
    AfxAssert(portIdx == ddge->base.portIdx);

    for (afxNat queIdx = 0; queIdx < ddge->base.queCnt; queIdx++)
    {
        afxDrawQueue* dque = &ddge->base.queues[queIdx];

        if (AfxTryLockMutex(&dque->workChnMtx))
        {
            _SglDdevProcPortUnlocked(dpu, ddge, queIdx);
            AfxUnlockMutex(&dque->workChnMtx);
            AfxSignalCondition2(&dque->idleCnd);
        }
    }
    return err;
}

_SGL afxNat _SglDgdeEnqueueExecuteCb(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    while (!queued)
    {
        for (afxNat i = 0; i < ddge->base.queCnt; i++)
        {
            afxDrawQueue* dque = &ddge->base.queues[i];

            if (AfxTryLockMutex(&dque->workChnMtx))
            {
                _sglQueueingExecute* subm;
                afxNat queuingSiz = sizeof(*subm) + (cnt * sizeof(subm->items[0]));
                subm = _AfxDqueRequestArenaSpace(ddge, i, queuingSiz);
                *subm = (_sglQueueingExecute) { 0 };
                subm->hdr.siz = queuingSiz;
                AfxGetTime(&subm->hdr.pushTime);
                subm->hdr.exec = (void*)_DpuExecute;
                subm->itemCnt = cnt;

                for (afxNat i = 0; i < cnt; i++)
                {
                    afxDrawStream diob = req[i].diob;
                    AfxAssertObjects(1, &diob, afxFcc_DIOB);

                    subm->items[i] = req[i];
                    subm->items[i].diob->base.state = afxDrawStreamState_PENDING;
                }

                AfxPushLinkage(&subm->hdr.chain, &dque->workChn);
                AfxUnlockMutex(&dque->workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        AfxYield();
    }
    return queIdx;
}

_SGL afxNat _SglDdgeEnqueueTransferCb(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    while (!queued)
    {
        for (afxNat i = 0; i < ddge->base.queCnt; i++)
        {
            afxDrawQueue* dque = &ddge->base.queues[i];

            if (AfxTryLockMutex(&dque->workChnMtx))
            {
                _sglQueueingTransfer* subm;
                afxNat queuingSiz = sizeof(*subm) + (cnt * sizeof(subm->items[0]));
                subm = _AfxDqueRequestArenaSpace(ddge, i, queuingSiz);
                *subm = (_sglQueueingTransfer) { 0 };
                subm->hdr.siz = queuingSiz;
                AfxGetTime(&subm->hdr.pushTime);
                subm->hdr.exec = (void*)_DpuTransfer;
                subm->itemCnt = cnt;

                for (afxNat i = 0; i < cnt; i++)
                {
                    subm->items[i] = req[i];
                }

                AfxPushLinkage(&subm->hdr.chain, &dque->workChn);
                AfxUnlockMutex(&dque->workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        AfxYield();
    }
    return queIdx;
}

_SGL afxNat _SglDdgeEnqueuePresentCb(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    while (!queued)
    {
        for (afxNat i = 0; i < ddge->base.queCnt; i++)
        {
            afxDrawQueue* dque = &ddge->base.queues[i];

            if (AfxTryLockMutex(&dque->workChnMtx))
            {
                _sglQueueingPresent* subm;
                afxNat queuingSiz = sizeof(*subm) + (cnt * sizeof(subm->items[0]));
                subm = _AfxDqueRequestArenaSpace(ddge, i, queuingSiz);
                *subm = (_sglQueueingPresent) { 0 };
                subm->hdr.siz = queuingSiz;
                subm->hdr.exec = (void*)_DpuPresent;
                AfxGetTime(&subm->hdr.pushTime);
                subm->itemCnt = cnt;

                for (afxNat i = 0; i < cnt; i++)
                {
                    afxDrawOutput dout = req[i].dout;
                    AfxAssertObjects(1, &dout, afxFcc_DOUT);

                    subm->items[i] = req[i];
                }

                AfxPushLinkage(&subm->hdr.chain, &dque->workChn);
                AfxUnlockMutex(&dque->workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        AfxYield();
    }
    return queIdx;
}

_SGL afxNat _SglDdgeEnqueueStampCb(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    while (!queued)
    {
        for (afxNat i = 0; i < ddge->base.queCnt; i++)
        {
            afxDrawQueue* dque = &ddge->base.queues[i];

            if (AfxTryLockMutex(&dque->workChnMtx))
            {
                _sglQueueingStamp* subm;
                afxNat queuingSiz = sizeof(*subm) + (cnt * sizeof(subm->items[0]));
                subm = _AfxDqueRequestArenaSpace(ddge, i, queuingSiz);
                *subm = (_sglQueueingStamp) { 0 };
                subm->hdr.siz = queuingSiz;
                subm->hdr.exec = (void*)_DpuStamp;
                AfxGetTime(&subm->hdr.pushTime);
                subm->itemCnt = cnt;

                AfxCopyV2d(subm->origin, origin);
                AfxMakeString4096(&subm->caption, caption);

                for (afxNat i = 0; i < cnt; i++)
                {
                    afxDrawOutput dout = req[i].dout;
                    AfxAssertObjects(1, &dout, afxFcc_DOUT);
                    subm->items[i] = req[i];
                }

                AfxPushLinkage(&subm->hdr.chain, &dque->workChn);
                AfxUnlockMutex(&dque->workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        AfxYield();
    }
    return queIdx;
}

_SGL afxNat _SglDdgeEnqueueMmapCb(afxDrawBridge ddge, afxBuffer buf, afxSize off, afxNat ran, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    while (!queued)
    {
        for (afxNat i = 0; i < ddge->base.queCnt; i++)
        {
            afxDrawQueue* dque = &ddge->base.queues[i];

            if (AfxTryLockMutex(&dque->workChnMtx))
            {
                _sglQueueingMmap* subm;
                afxNat queuingSiz = sizeof(*subm);
                subm = _AfxDqueRequestArenaSpace(ddge, i, queuingSiz);
                *subm = (_sglQueueingMmap) { 0 };
                subm->hdr.siz = queuingSiz;
                subm->hdr.exec = (void*)_DpuMmap;
                AfxGetTime(&subm->hdr.pushTime);

                subm->buf = buf;
                subm->off = off;
                subm->ran = ran;
                subm->flags = flags;

                AfxPushLinkage(&subm->hdr.chain, &dque->workChn);
                AfxUnlockMutex(&dque->workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        AfxYield();
    }
    return queIdx;
}

_SGL afxError _SglDdgeWaitCb(afxDrawBridge ddge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssertRange(ddge->base.queCnt, queIdx, 1);
    afxDrawQueue* dque = &ddge->base.queues[queIdx];

    afxDrawDevice ddev = AfxGetDrawBridgeDevice(ddge);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxNat portIdx = ddge->base.portIdx;
    sglDpu *dpu = &ddev->idd->dpus[portIdx];
    AfxAssert(portIdx == ddge->base.portIdx);
    afxThread currThr;
    AfxGetThread(&currThr);

    if (ddev->idd->dedThread == currThr)
    {
        while (dque->workChn.cnt)
        {
            if (AfxTryLockMutex(&dque->workChnMtx))
            {
                _SglDdevProcPortUnlocked(dpu, ddge, queIdx);
                AfxAssert(!dque->workChn.cnt);
                AfxUnlockMutex(&dque->workChnMtx);
                AfxSignalCondition2(&dque->idleCnd);
                break;
            }
        }
    }
    else
    {
        AfxLockMutex(&dque->idleCndMtx);
        
        while (dque->workChn.cnt)
            AfxWaitCondition(&dque->idleCnd, &dque->idleCndMtx);

        AfxUnlockMutex(&dque->idleCndMtx);
    }
    return err;
}

_SGL afxError _SglDdgeDtor(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    AfxCleanUpChainedManagers(&ddge->base.mgrChn);

    for (afxNat i = 0; i < ddge->base.queCnt; i++)
    {
        afxDrawQueue* dque = &ddge->base.queues[i];
        AfxCleanUpMutex(&dque->workChnMtx);
        AfxCleanUpSlock(&dque->reqLock);
        AfxDeallocateArena(&dque->workArena);
        AfxCleanUpSlock(&dque->workArenaSlock);
        AfxCleanUpCondition(&dque->idleCnd);
        AfxCleanUpMutex(&dque->idleCndMtx);
        AfxDeallocateQueue(&dque->recycQue);
    }
    return err;
}

_SGL afxError _SglDdgeCtor(afxDrawBridge ddge, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawContext dctx = cookie->udd[1];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat portIdx = *(afxNat const *)cookie->udd[2];
    afxDrawBridgeConfig const *spec = ((afxDrawBridgeConfig const *)cookie->udd[3]);
    //afxDrawBridge dque2;

    //if (!(dque2 = AfxGetObject(&ddev->ports[portIdx].queues, AfxGetObjectId(ddge))) || dque2 != ddge) AfxThrowError();
    //else
    {
        //AfxAssertObjects(1, &dque2, afxFcc_DDGE);
        
        ddge->base.portIdx = portIdx;
        AfxPushLinkage(&ddge->base.dctx, &dctx->base.ownedBridges);

        ddge->base.waitCb = _SglDdgeWaitCb;
        ddge->base.executeCb = _SglDgdeEnqueueExecuteCb;
        ddge->base.transferCb = _SglDdgeEnqueueTransferCb;
        ddge->base.presentCb = _SglDdgeEnqueuePresentCb;
        ddge->base.stampCb = _SglDdgeEnqueueStampCb;


        ddge->base.queCnt = AfxMax(3, spec ? spec->queueCnt : 0);

        if (!(ddge->base.queues = AfxAllocate(ddge->base.queCnt, sizeof(ddge->base.queues[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            AfxSetUpChain(&ddge->base.mgrChn, ddge);
            afxClassConfig tmpClsCfg;

            for (afxNat i = 0; i < ddge->base.queCnt; i++)
            {
                afxDrawQueue* dque = &ddge->base.queues[i];

                dque->immediate = 0;// !!spec->immedate;

                AfxSetUpSlock(&dque->workArenaSlock);
                AfxAllocateArena(NIL, &dque->workArena, NIL, AfxHere());

                dque->lockedForReq = FALSE;

                AfxSetUpChain(&dque->workChn, ddge);
                AfxSetUpMutex(&dque->workChnMtx, AFX_MTX_PLAIN);
                AfxSetUpCondition(&dque->idleCnd);
                AfxSetUpMutex(&dque->idleCndMtx, AFX_MTX_PLAIN);
                AfxSetUpSlock(&dque->reqLock);

                AfxAllocateQueue(&dque->recycQue, sizeof(afxDrawStream), 3);

                tmpClsCfg = _SglDiobMgrCfg;
                AfxEstablishManager(&dque->streams, NIL, &ddge->base.mgrChn, &tmpClsCfg);
            }
        }
    }
    return err;
}

_SGL afxClassConfig _SglDdgeMgrCfg =
{
    .fcc = afxFcc_DDGE,
    .name = "DrawBrige",
    .desc = "Draw Execution Bridge",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxDrawBridge)),
    .mmu = NIL,
    .ctor = (void*)_SglDdgeCtor,
    .dtor = (void*)_SglDdgeDtor
};
