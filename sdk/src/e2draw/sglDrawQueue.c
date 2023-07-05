/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "sgl.h"

#include "afx/core/afxSystem.h"
#include "../e2coree/draw/_classified/afxDrawClassified.h"

// RENDERING SCOPE

_SGL void _SglDqueEndCombination(afxDrawQueue dque, _afxDscrCmd const *cmd)
{
    (void)cmd;
    afxError err = AFX_ERR_NONE;

    sglVmt const *gl = &dque->wglVmt;
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();

    AfxAssert(dque->tmpFbo[dque->activeTmpFboIdx]);

    for (afxNat i = 0; i < dque->state.renderPass.rasterCnt; i++)
    {
        afxDrawTarget const *target = &dque->state.renderPass.rasters[i];
        afxSurface surf = target->surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER));
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;

            // AFX_SURFACE_STORE_OP_STORE
#if 0
            gl->BindTexture(surf->tex.glTarget, surf->tex.gpuTexHandle);
            gl->GetTexImage(surf->tex.glTarget, 0, surf->tex.glFmt, surf->tex.glType, (void*)AfxTextureGetData(&surf->tex, NIL, 0));
            gl->BindTexture(surf->tex.glTarget, 0);
#endif
        }
    }

    if (dque->state.renderPass.depthRt.surf)
    {
        afxSurface surf = dque->state.renderPass.depthRt.surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }

    if (dque->state.renderPass.stencilRt.surf)
    {
        afxSurface surf = dque->state.renderPass.stencilRt.surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }
};

_SGL void _SglDqueBeginCombination(afxDrawQueue dque, _afxDscrCmdBeginComb const *cmd)
{
    afxError err = AFX_ERR_NONE;
    //_gfxRendererContextState *state = &dctx->state;

    dque->state.renderPass.annexCnt = 0;
    dque->state.renderPass.canv = NIL;
    dque->state.renderPass.area = cmd->area;
    dque->state.renderPass.layerCnt = cmd->layerCnt;
    dque->state.renderPass.rasterCnt = cmd->rasterCnt;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= cmd->rasterCnt);

    for (afxNat i = 0; i < cmd->rasterCnt; i++)
        dque->state.renderPass.rasters[i] = cmd->rasters[i];

    dque->state.renderPass.depthRt = cmd->depth;
    dque->state.renderPass.stencilRt = cmd->stencil;

    sglVmt const *gl = &dque->wglVmt;

    dque->activeTmpFboIdx = (dque->activeTmpFboIdx + 1) % _SGL_MAX_SWAPCHAIN_CAP;

    if (dque->tmpFbo[dque->activeTmpFboIdx])
    {
        gl->DeleteFramebuffers(1, &(dque->tmpFbo[dque->activeTmpFboIdx])); _SglThrowErrorOccuried();
    }
    gl->GenFramebuffers(1, &(dque->tmpFbo[dque->activeTmpFboIdx])); _SglThrowErrorOccuried();
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, dque->tmpFbo[dque->activeTmpFboIdx]); _SglThrowErrorOccuried();
    AfxAssert(gl->IsFramebuffer(dque->tmpFbo[dque->activeTmpFboIdx]));

    afxNat colorAttchBaseIdx = 0;
    GLenum drawBuffersIndices[_SGL_MAX_SURF_PER_CANV];
    afxDrawTarget const *target;
    afxSurface surf;

    for (afxNat i = 0; i < dque->state.renderPass.rasterCnt; i++)
    {
        target = &dque->state.renderPass.rasters[i];
        surf = target->surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER));
            AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            surf->state = AFX_SURF_STATE_PAINTING;

            _SglDqueBindAndSyncTex(dque, 0, &surf->tex);
            _SglDqueBindAndSyncTex(dque, 0, NIL);
            _SglDqueSurfSync(dque, surf);
            _SglDqueSurfSync(dque, NIL);

            gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx, GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
            gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
        }

        drawBuffersIndices[colorAttchBaseIdx] = GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx;

        if (target->loadOp == AFX_SURFACE_LOAD_OP_CLEAR)
        {
            afxReal const *color = target->clearValue.color;            
            gl->DrawBuffers(1, &drawBuffersIndices[colorAttchBaseIdx]); _SglThrowErrorOccuried();
            gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
            gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();
        }

        colorAttchBaseIdx++;
    }

    gl->DrawBuffers(colorAttchBaseIdx, drawBuffersIndices); _SglThrowErrorOccuried();

    //if (cmd->depth)
    {
        target = &dque->state.renderPass.depthRt;
        surf = target->surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
            AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            surf->state = AFX_SURF_STATE_PAINTING;

            _SglDqueBindAndSyncTex(dque, 0, &surf->tex);
            _SglDqueBindAndSyncTex(dque, 0, NIL);
            _SglDqueSurfSync(dque, surf);
            _SglDqueSurfSync(dque, NIL);

            gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
            gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
        }

        if (target->loadOp == AFX_SURFACE_LOAD_OP_CLEAR)
        {
            gl->ClearDepth(target->clearValue.depth); _SglThrowErrorOccuried();
            gl->Clear(GL_DEPTH_BUFFER_BIT); _SglThrowErrorOccuried();
        }
    }

    //if (cmd->stencil)
    {
        target = &dque->state.renderPass.stencilRt;
        surf = target->surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
            AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            surf->state = AFX_SURF_STATE_PAINTING;

            _SglDqueBindAndSyncTex(dque, 0, &surf->tex);
            _SglDqueBindAndSyncTex(dque, 0, NIL);
            _SglDqueSurfSync(dque, surf);
            _SglDqueSurfSync(dque, NIL);

            gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
            gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
        }

        if (target->loadOp == AFX_SURFACE_LOAD_OP_CLEAR)
        {
            gl->ClearStencil(target->clearValue.stencil); _SglThrowErrorOccuried();
            gl->Clear(GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
        }
    }

    if (GL_FRAMEBUFFER_COMPLETE != gl->CheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
        AfxThrowError();
}

_SGL void _SglDqueEndOperation(afxDrawQueue dque, _afxDscrCmd const *cmd)
{
    afxError err = AFX_ERR_NONE;
    (void)cmd;
    sglVmt const*gl = &dque->wglVmt;

    _SglDqueBindAndSyncCanv(dque, NIL, GL_DRAW_FRAMEBUFFER);
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();
    afxSurface surf;

    if (dque->state.renderPass.canv)
    {
        for (afxNat i = 0; i < dque->state.renderPass.annexCnt; i++)
        {
            afxNat surfCnt = AfxCanvasGetSurfaceCount(dque->state.renderPass.canv);
            surf = AfxCanvasGetSurface(dque->state.renderPass.canv, i);
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }
    else
    {
        for (afxNat i = 0; i < dque->state.renderPass.rasterCnt; i++)
        {
            afxDrawTarget const *target = &dque->state.renderPass.rasters[i];
            surf = target->surf;

            if (surf)
            {
                AfxAssertObject(surf, AFX_FCC_SURF);
                AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
                surf->state = AFX_SURF_STATE_PRESENTABLE;
                surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
            }
        }

        if ((surf = dque->state.renderPass.depthRt.surf))
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }

        if ((surf = dque->state.renderPass.stencilRt.surf))
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }
}

_SGL void _SglDqueBindPipeline(afxDrawQueue dque, _afxDscrCmdBindPip const *cmd);

_SGL void _SglDqueEmployTechnique(afxDrawQueue dque, _afxDscrCmdEmployTec const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmd->tecIdx < AfxDrawOperationGetTechniqueCount(dque->state.renderPass.dop));
    dque->state.renderPass.activeTec = cmd->tecIdx;
    dque->state.renderPass.activePass = 0;
    afxPipeline pip = AfxDrawOperationGetPipeline(dque->state.renderPass.dop, dque->state.renderPass.activeTec, dque->state.renderPass.activePass);
    AfxAssertObject(pip, AFX_FCC_PIP);
    _afxDscrCmdBindPip cmdBindPip;
    cmdBindPip.pip = pip;
    _SglDqueBindPipeline(dque, &cmdBindPip);
}

_SGL void _SglDqueNextPass(afxDrawQueue dque, _afxDscrCmd const *cmd)
{
    afxError err = AFX_ERR_NONE;
    (void)cmd;
    ++(dque->state.renderPass.activePass);
    AfxAssert(dque->state.renderPass.activePass < AfxDrawOperationGetPassCount(dque->state.renderPass.dop, dque->state.renderPass.activeTec));
    afxPipeline pip = AfxDrawOperationGetPipeline(dque->state.renderPass.dop, dque->state.renderPass.activeTec, dque->state.renderPass.activePass);
    AfxAssertObject(pip, AFX_FCC_PIP);
    _afxDscrCmdBindPip cmdBindPip;
    cmdBindPip.pip = pip;
    _SglDqueBindPipeline(dque, &cmdBindPip);
}

_SGL void _SglDqueBeginOperation(afxDrawQueue dque, _afxDscrCmdBeginOp const *cmd)
{
    afxError err = AFX_ERR_NONE;
    //_gfxRendererContextState *state = &dctx->state;
    sglVmt const *gl = &dque->wglVmt;

    //dque->state.renderPass.dop = dop;
    dque->state.renderPass.activeTec = 0;
    dque->state.renderPass.activePass = 0;
    dque->state.renderPass.canv = cmd->canv;
    dque->state.renderPass.area = cmd->area;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= cmd->annexCnt);
    dque->state.renderPass.annexCnt = cmd->annexCnt;

    for (afxNat i = 0; i < cmd->annexCnt; i++)
        dque->state.renderPass.annexes[i] = cmd->annexes[i];

    dque->state.renderPass.layerCnt = 0;
    dque->state.renderPass.depthRt = (afxDrawTarget) { 0 };
    dque->state.renderPass.stencilRt = (afxDrawTarget) { 0 };
    dque->state.renderPass.rasters[0] = (afxDrawTarget) { 0 };
    dque->state.renderPass.rasterCnt = 0;

    _SglDqueBindAndSyncCanv(dque, cmd->canv, GL_DRAW_FRAMEBUFFER);

    if (!cmd->canv)
    {
        if (dque->state.renderPass.annexCnt)
        {
            GLenum colorAtt[] = { GL_COLOR_ATTACHMENT0 };
            //gl->DrawBuffers(1, colorAtt); _SglThrowErrorOccuried();
            afxReal *color = dque->state.renderPass.annexes[0].clearValue.color;
            gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
            gl->ClearDepth(1); _SglThrowErrorOccuried();
            gl->ClearStencil(0); _SglThrowErrorOccuried();
            gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
        }
    }
    else
    {
        AfxAssert(AfxCanvasGetSurfaceCount(dque->state.renderPass.canv) >= dque->state.renderPass.annexCnt);
        afxNat colorAttchCnt = 0;
        GLenum drawBuffersIndices[_SGL_MAX_SURF_PER_CANV];

        for (afxNat i = 0; i < dque->state.renderPass.annexCnt; i++)
        {
            afxSurface surf = AfxCanvasGetSurface(dque->state.renderPass.canv, i);
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            surf->state = AFX_SURF_STATE_PAINTING;

            if (dque->state.renderPass.annexes[i].clearOnLoad)
            {
                GLbitfield bufferBits = NIL;

                if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER))
                {
                    afxReal *color = dque->state.renderPass.annexes[0].clearValue.color;
                    bufferBits |= GL_COLOR_BUFFER_BIT;
                    drawBuffersIndices[colorAttchCnt] = GL_COLOR_ATTACHMENT0 + colorAttchCnt;
                    gl->DrawBuffers(1, &drawBuffersIndices[colorAttchCnt]); _SglThrowErrorOccuried();
                    colorAttchCnt++;
                    gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
                }
                else if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH))
                {
                    if (cmd->canv->depthIdx == i)
                    {
                        bufferBits |= GL_DEPTH_BUFFER_BIT;
                        gl->ClearDepth(dque->state.renderPass.annexes[i].clearValue.depth); _SglThrowErrorOccuried();
                    }

                    if (cmd->canv->stencilIdx == i)
                    {
                        bufferBits |= GL_STENCIL_BUFFER_BIT;
                        gl->ClearStencil(dque->state.renderPass.annexes[i].clearValue.stencil); _SglThrowErrorOccuried();
                    }
                }

                gl->Clear(bufferBits); _SglThrowErrorOccuried();
            }
        }
        gl->DrawBuffers(colorAttchCnt, drawBuffersIndices); _SglThrowErrorOccuried();
    }
    dque->state.renderPass.dop = cmd->dop;
    _afxDscrCmdEmployTec cmdEmployTec = { 0 };
    cmdEmployTec.tecIdx = cmd->tecIdx;
    _SglDqueEmployTechnique(dque, &cmdEmployTec);
}

// STATE SETTING

_SGL void _SglDqueSetScissors(afxDrawQueue dque, _afxDscrCmdSetScissor const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->cnt);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET > cmd->first);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->first + cmd->cnt);
    AfxAssert(!cmd->cnt || (cmd->cnt));

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        AfxRectCopy(&(dque->state.scissorArea[cmd->first + i]), &cmd->rect[i]);
    }

    sglVmt const*gl = &dque->wglVmt;

    if (cmd->cnt)
    {
        gl->Enable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();

        if (gl->ScissorArrayv)
        {
            GLint v[_SGL_MAX_SCISSOR_PER_SET][4];
            AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->cnt);

            for (afxNat i = 0; i < cmd->cnt; i++)
                v[i][0] = cmd->rect[i].offset[0],
                v[i][1] = cmd->rect[i].offset[1],
                v[i][2] = cmd->rect[i].extent[0],
                v[i][3] = cmd->rect[i].extent[1];

            gl->ScissorArrayv(cmd->first, cmd->cnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLint x = (GLint)(cmd->rect[0].offset[0]);
            GLint y = (GLint)(cmd->rect[0].offset[1]);
            GLsizei w = (GLsizei)(cmd->rect[0].extent[0]);
            GLsizei h = (GLsizei)(cmd->rect[0].extent[1]);

            gl->Scissor(x, y, w, h); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->Disable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetViewports(afxDrawQueue dque, _afxDscrCmdSetVp const *cmd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->cnt);
    AfxAssert(_SGL_MAX_VP_PER_SET > cmd->first);
    AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->first + cmd->cnt);
    AfxAssert(!cmd->cnt || (cmd->cnt));

    for (afxNat i = 0; i < cmd->cnt; i++)
        dque->state.vp[cmd->first + i] = cmd->vp[i];

    if (cmd->cnt)
    {
        sglVmt const*gl = &dque->wglVmt;

        if (gl->ViewportArrayv)
        {
            GLfloat v[_SGL_MAX_VP_PER_SET][4];
            AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->cnt);

            for (afxNat i = 0; i < cmd->cnt; i++)
                v[i][0] = cmd->vp[i].offset[0],
                v[i][1] = cmd->vp[i].offset[1],
                v[i][2] = cmd->vp[i].extent[0],
                v[i][3] = cmd->vp[i].extent[1];

            gl->ViewportArrayv(cmd->first, cmd->cnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLint x = (GLint)(cmd->vp[0].offset[0]);
            GLint y = (GLint)(cmd->vp[0].offset[1]);
            GLsizei w = (GLsizei)(cmd->vp[0].extent[0]);
            GLsizei h = (GLsizei)(cmd->vp[0].extent[1]);

            gl->Viewport(x, y, w, h); _SglThrowErrorOccuried();
        }

        if (gl->DepthRangeArrayv)
        {
            GLdouble v[_SGL_MAX_VP_PER_SET][2];
            AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->cnt);

            for (afxNat i = 0; i < cmd->cnt; i++)
                v[i][0] = cmd->vp[i].depth[0],
                v[i][1] = cmd->vp[i].depth[1];

            gl->DepthRangeArrayv(cmd->first, cmd->cnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLdouble n = cmd->vp[0].depth[0];
            GLdouble f = cmd->vp[0].depth[1];

            gl->DepthRange(n, f); _SglThrowErrorOccuried();
        }
    }
}

_SGL void _SglDqueSetRasterizerState(afxDrawQueue dque, _afxDscrCmdSetRasterizerState const *cmd)
{
    afxError err = AFX_ERR_NONE;

    dque->state.raster = cmd->state;

    sglVmt const *gl = &dque->wglVmt;

    if (dque->state.raster.depthClampEnable)
    {
        gl->Enable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
    }

    if (dque->state.raster.rasterizerDiscardEnable)
    {
        gl->Enable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
    }

    gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlPolygonMode(dque->state.raster.fillMode)); _SglThrowErrorOccuried();

    if (dque->state.raster.cullMode)
    {
        gl->Enable(GL_CULL_FACE); _SglThrowErrorOccuried();
        gl->CullFace(AfxToGlCullMode(dque->state.raster.cullMode)); _SglThrowErrorOccuried();
        gl->FrontFace(AfxToGlFrontFace(dque->state.raster.frontFace)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_CULL_FACE); _SglThrowErrorOccuried();
    }

    static int const QwadroToGlPolygonModeBasedOffset[] =
    {
        GL_POLYGON_OFFSET_FILL,
        GL_POLYGON_OFFSET_LINE,
        GL_POLYGON_OFFSET_POINT
    };

    if (dque->state.raster.depthBiasEnable)
    {
        gl->Enable(QwadroToGlPolygonModeBasedOffset[dque->state.raster.fillMode]); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(QwadroToGlPolygonModeBasedOffset[dque->state.raster.fillMode]); _SglThrowErrorOccuried();
    }

    gl->PolygonOffset(dque->state.raster.depthBiasSlopeFactor, dque->state.raster.depthBiasConstantFactor); _SglThrowErrorOccuried();

    gl->LineWidth(dque->state.raster.lineWidth); _SglThrowErrorOccuried();
}

_SGL void _SglDqueSetColorBlendState(afxDrawQueue dque, afxPipelineColorBlendState const *state)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(state);
    dque->state.colorBlend = *state;

    AfxAssert(NIL);
}

_SGL void _SglDqueSetMultisampleState(afxDrawQueue dque, afxPipelineMultisampleState const *state)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(state);
    dque->state.multisample = *state;

    sglVmt const *gl = &dque->wglVmt;

    if (dque->state.multisample.sampleShadingEnable)
    {
        gl->Enable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
    }

    if (dque->state.multisample.pSampleMask)
    {
        gl->Enable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
    }

    gl->SampleMaski(0, dque->state.multisample.pSampleMask[0]); _SglThrowErrorOccuried();

    if (dque->state.multisample.minSampleShading)
    {
        gl->Enable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
    }

    // gl v4.1?
    //gl->MinSampleShading(dctx->queue[queueIdx].data.nextAccumState.draw.multisample.minSampleShading); _SglThrowErrorOccuried();

    if (dque->state.multisample.alphaToOneEnable)
    {
        gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
    }

    if (dque->state.multisample.alphaToCoverageEnable)
    {
        gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetDepthState(afxDrawQueue dque, _afxDscrCmdSetDepthState const *cmd)
{
    afxError err = AFX_ERR_NONE;

    dque->state.depth = cmd->state;

    sglVmt const *gl = &dque->wglVmt;

    if (dque->state.depth.depthTestEnable)
    {
        gl->Enable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        gl->DepthFunc(SglToGlCompareOp(dque->state.depth.depthCompareOp)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
    }

    if (dque->state.depth.depthWriteEnable)
    {
        gl->DepthMask(GL_TRUE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DepthMask(GL_FALSE); _SglThrowErrorOccuried();
    }

    if (dque->state.depth.stencilTestEnable)
    {
        gl->Enable(GL_STENCIL_TEST); _SglThrowErrorOccuried();

        gl->StencilFuncSeparate(GL_FRONT, SglToGlCompareOp(dque->state.depth.stencilOpFront.compareOp), 0, 1); _SglThrowErrorOccuried();
        gl->StencilOpSeparate(GL_FRONT, AfxToGlStencilOp(dque->state.depth.stencilOpFront.failOp), AfxToGlStencilOp(dque->state.depth.stencilOpFront.depthFailOp), AfxToGlStencilOp(dque->state.depth.stencilOpFront.passOp)); _SglThrowErrorOccuried();
        gl->StencilFuncSeparate(GL_BACK, SglToGlCompareOp(dque->state.depth.stencilOpBack.compareOp), 0, 1); _SglThrowErrorOccuried();
        gl->StencilOpSeparate(GL_BACK, AfxToGlStencilOp(dque->state.depth.stencilOpBack.failOp), AfxToGlStencilOp(dque->state.depth.stencilOpBack.depthFailOp), AfxToGlStencilOp(dque->state.depth.stencilOpBack.passOp)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetInputAssemblyState(afxDrawQueue dque, _afxDscrCmdSetInputAssemblyState const *cmd)
{
    afxError err = AFX_ERR_NONE;

    dque->state.ia = cmd->state;

    sglVmt const *gl = &dque->wglVmt;

    if (dque->state.ia.primRestartEnable)
    {
        gl->Enable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetInputStreams(afxDrawQueue dque, afxNat cnt, afxPipelineInputStream const streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(_SGL_MAX_INSTREAM_PER_SET >= cnt);

    sglVmt const*gl = &dque->wglVmt;

    dque->activeVaoIdx = (dque->activeVaoIdx + 1) % _SGL_MAX_VAO_PER_TIME;

#if !0
    if (dque->vao[dque->activeVaoIdx])
    {
        gl->DeleteVertexArrays(1, &(dque->vao[dque->activeVaoIdx])); _SglThrowErrorOccuried();
    }
    gl->GenVertexArrays(1, &(dque->vao[dque->activeVaoIdx])); _SglThrowErrorOccuried();
    gl->BindVertexArray(dque->vao[dque->activeVaoIdx]); _SglThrowErrorOccuried();
#endif

    for (afxNat i = 0; i < cnt; i++)
    {
        dque->state.inStreams[i] = streams[i];
        AfxAssert(16 > streams[i].location);  // max vertex attrib

        GLint glsiz;
        GLenum gltype;
        GLuint glStride;

        switch (streams[i].format)
        {
        case AFX_VTX_FMT_REAL:
            glsiz = 1;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        case AFX_VTX_FMT_V2D:
            glsiz = 2;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        case AFX_VTX_FMT_V3D:
            glsiz = 3;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        case AFX_VTX_FMT_V4D:
            glsiz = 4;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        default:
            glsiz = 0;
            gltype = NIL;
            glStride = 0;
            AfxThrowError();
            break;
        }

        AfxAssert(streams[i].location == streams[i].binding);
        gl->VertexAttribBinding(streams[i].location, streams[i].binding); _SglThrowErrorOccuried();
        gl->VertexAttribFormat(streams[i].location, glsiz, gltype, FALSE, streams[i].offset); _SglThrowErrorOccuried();
        AfxAssert(streams[i].offset == 0);
        gl->EnableVertexAttribArray(streams[i].location); _SglThrowErrorOccuried();
        //gl->VertexAttribPointer(streams[i].location, glsiz, gltype, FALSE, glStride, streams[i].offset);
    }
    //gl->BindVertexArray(0);
}

// RESOURCE BINDING

_SGL void _SglDqueBindPipeline(afxDrawQueue dque, _afxDscrCmdBindPip const *cmd)
{
    afxError err = AFX_ERR_NONE;

    dque->state.pip = cmd->pip;

    //sglVmt const* gl = &dque->wglVmt;
#if 0
    afxNat inStreamCnt = 0;
    afxPipelineInputStream inStreams[8];

    for (afxNat i = 0; i < AfxPipelineGetStageCount(dque->state.pip); i++)
    {
        afxShader shd;
        AfxPipelineGetModules(dque->state.pip, i, 1, &shd);

        if (AFX_SHADER_STAGE_VERTEX == AfxShaderGetStage(shd))
        {
            for (afxNat j = 0; j < shd->ioDeclCnt; j++)
            {
                inStreams[inStreamCnt].location = shd->ioDecls[j].location;
                inStreams[inStreamCnt].format = shd->ioDecls[j].fmt;
                inStreams[inStreamCnt].binding = shd->ioDecls[j].location;
                inStreams[inStreamCnt].offset = 0;
                inStreamCnt++;
            }
            break;
        }
    }
#endif
    _SglDqueSetInputStreams(dque, dque->state.pip->inCnt , dque->state.pip->ins);

    if (dque->state.pip->hasVtxInAssembling)
    {
        _afxDscrCmdSetInputAssemblyState cmd;
        cmd.state = dque->state.pip->vtxInAssembling;
        _SglDqueSetInputAssemblyState(dque, &cmd);
    }

    if (dque->state.pip->hasRasterization)
    {
        _afxDscrCmdSetRasterizerState cmd;
        cmd.state = dque->state.pip->rasterization;
        _SglDqueSetRasterizerState(dque, &cmd);
    }

    if (dque->state.pip->hasMultisampling)
    {
        _SglDqueSetMultisampleState(dque, &dque->state.pip->multisampling);
    }

    if (dque->state.pip->hasDepthHandling)
    {
        _afxDscrCmdSetDepthState cmd;
        cmd.state = dque->state.pip->depthHandling;
        _SglDqueSetDepthState(dque, &cmd);
    }

    if (dque->state.pip->hasColorBlending)
    {
        _SglDqueSetColorBlendState(dque, &dque->state.pip->colorBlending);
    }

    if (dque->state.pip->vpCnt)
    {
        _afxDscrCmdSetVp cmd;
        cmd.first = 0;
        cmd.cnt = dque->state.pip->vpCnt;

        for (afxNat i = 0; i < cmd.cnt; i++)
            cmd.vp[i] = dque->state.pip->vps[i];

        _SglDqueSetViewports(dque, &cmd);
    }

    if (dque->state.pip->scissorCnt)
    {
        _afxDscrCmdSetScissor cmd;
        cmd.first = 0;
        cmd.cnt = dque->state.pip->scissorCnt;

        for (afxNat i = 0; i < cmd.cnt; i++)
            cmd.rect[i] = dque->state.pip->scissors[i];

        _SglDqueSetScissors(dque, &cmd);
    }

    _SglDqueBindAndSyncPip(dque, dque->state.pip);
}

_SGL void _SglDqueBindTextures(afxDrawQueue dque, _afxDscrCmdBindTextures const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > first);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= first + cmd->cnt);

    //sglVmt const *gl = &dque->wglVmt;

    AfxAssert(dque->state.pip);
    afxNat setCnt = dque->state.pip->wiringCnt;
    AfxAssert(cmd->set < setCnt);

    afxNat bindingCnt = 0;

    for (afxNat i = 0; i < setCnt; i++)
        if (dque->state.pip->wiring[i].set == cmd->set)
            bindingCnt = dque->state.pip->wiring[i].legt->entryCnt;

    AfxAssert(bindingCnt);
    AfxAssert(cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= _SGL_MAX_ENTRY_PER_LEGO);

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxNat binding = (cmd->set * _SGL_MAX_ENTRY_PER_LEGO) + first + i;

        if ((dque->state.boundRes[cmd->set][first + i].tex = cmd->tex[i]))
        {
            AfxAssertObject(cmd->tex[i], AFX_FCC_TEX);
            
            if (_SglDqueBindAndSyncTex(dque, binding, cmd->tex[i]))
                AfxThrowError();
        }

        if ((dque->state.boundRes[cmd->set][first + i].smp = cmd->smp[i]))
        {
            AfxAssertObject(cmd->smp[i], AFX_FCC_SMP);
            
            if (_SglDqueBindAndSyncSmp(dque, binding, cmd->smp[i]))
                AfxThrowError();
        }
    }
}

_SGL void _SglDqueBindBuffers(afxDrawQueue dque, _afxDscrCmdBindBuffers const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > first);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= first + cmd->cnt);

    //sglVmt const *gl = &dque->wglVmt;
#if 0
    AfxAssert(dque->state.pip);
    afxPipelineRig pipr = AfxPipelineGetRig(dque->state.pip);
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    afxNat setCnt = pipr->legtCnt;
    AfxAssert(cmd->set < setCnt);
    afxNat bindingCnt = pipr->legt[cmd->set]->entryCnt;
    AfxAssert(cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= _SGL_MAX_ENTRY_PER_LEGO);
#endif

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxNat binding = (cmd->set * _SGL_MAX_ENTRY_PER_LEGO) + first + i;
        
        dque->state.boundRes[cmd->set][first + i].buf = cmd->buf[i];
        
        {
            AfxAssertObject(cmd->buf[i], AFX_FCC_BUF);
            
            if (_SglDqueBindAndSyncBuf(dque, binding, cmd->buf[i], cmd->offset[i], cmd->range[i], GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW))
                AfxThrowError();
        }

        dque->state.boundRes[cmd->set][first + i].offset = cmd->offset[i];
        dque->state.boundRes[cmd->set][first + i].range = cmd->range[i];
    }
}

_SGL void _SglDqueBindIndexBuffer(afxDrawQueue dque, _afxDscrCmdBindIbuf const *cmd)
{
    afxError err = AFX_ERR_NONE;
    //_gfxRendererContextState *state = &dctx->state;
    //sglVmt *gl = dctx->activeOglVmt;

    dque->state.indexBinding.buf = cmd->buf;
    dque->state.indexBinding.offset = cmd->offset;
    dque->state.indexBinding.idxSiz = cmd->idxSiz;

    // WARNING: element array objects binding doesn't allow offset or ranges at bind time.

    if (_SglDqueBindAndSyncBuf(dque, 0, dque->state.indexBinding.buf, 0, 0, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW))
        AfxThrowError();
}

/*
The values taken from elements i of pBuffers and pOffsets replace the current state for the vertex input binding firstBinding + i, for i in [0, bindingCount).
The vertex input binding is updated to start at the offset indicated by pOffsets[i] from the start of the buffer pBuffers[i].
If pSizes is not NULL then pSizes[i] specifies the bound size of the vertex buffer starting from the corresponding elements of pBuffers[i] plus pOffsets[i].
All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
If the nullDescriptor feature is enabled, elements of pBuffers can be VK_NULL_HANDLE, and can be used by the vertex shader.
If a vertex input attribute is bound to a vertex input binding that is VK_NULL_HANDLE, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with (0,0,1).

This command also dynamically sets the byte strides between consecutive elements within buffer pBuffers[i] to the corresponding pStrides[i] value when the graphics pipeline is created with VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE set in VkPipelineDynamicStateCreateInfo::pDynamicStates.
Otherwise, strides are specified by the VkVertexInputBindingDescription::stride values used to create the currently active pipeline.

If the bound pipeline state object was also created with the VK_DYNAMIC_STATE_VERTEX_INPUT_EXT dynamic state enabled then vkCmdSetVertexInputEXT can be used instead of vkCmdBindVertexBuffers2 to set the stride.
*/

_SGL void _SglDqueBindVertexBuffers(afxDrawQueue dque, _afxDscrCmdBindVbuf const *cmd)
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
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_VBO_PER_BIND > first);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= first + cmd->cnt);

    //sglVmt const *gl = &dque->wglVmt;

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        dque->state.vertexBindings[first + i].buf = cmd->buf[i];
        dque->state.vertexBindings[first + i].offset = cmd->offset[i];
        dque->state.vertexBindings[first + i].siz = cmd->vtxArrSiz[i];
        dque->state.vertexBindings[first + i].stride = cmd->vtxStride[i];

        //afxNat32 stride = cmd->vtxStride[i] ? cmd->vtxStride[i] : dque->state.inStreams[i].offset;
        //AfxAssert(stride);
        afxNat32 stride = dque->state.vertexBindings[first + i].stride;
        
        if (_SglDqueBindAndSyncBuf(dque, first + i, dque->state.vertexBindings[first + i].buf, dque->state.vertexBindings[first + i].offset, stride, GL_ARRAY_BUFFER, GL_STATIC_DRAW))
            AfxThrowError();
    }
    dque->state.vertexBindingCnt = cmd->cnt;

    //_SglDquePushNextVao(dque);
}

#if 0
_SGL void _SglDquePushNextVao(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;

    sglVmt const* gl = &dque->wglVmt;

    {
        if (_SGL_MAX_VAO_PER_TIME > dque->activeVaoIdx)
            ++dque->activeVaoIdx;
        else
            dque->activeVaoIdx = 0;

        if (dque->vao[dque->activeVaoIdx])
        {
            gl->DeleteVertexArrays(1, &(dque->vao[dque->activeVaoIdx])); _SglThrowErrorOccuried();
        }
        gl->GenVertexArrays(1, &(dque->vao[dque->activeVaoIdx])); _SglThrowErrorOccuried();
        gl->BindVertexArray(dque->vao[dque->activeVaoIdx]); _SglThrowErrorOccuried();
        AfxAssert(gl->IsVertexArray(dque->vao[dque->activeVaoIdx]));

        afxPipeline pip = dque->state.pip;
        afxNat inStreamCnt = pip->inStreamCnt;

        if (!inStreamCnt)
        {
            //gl->BindVertexArray(0); _SglThrowErrorOccuried();
            gl->BindBuffer(GL_ARRAY_BUFFER, 0); _SglThrowErrorOccuried();
            gl->EnableVertexAttribArray(0); _SglThrowErrorOccuried();
            //gl->VertexAttribPointer(0, 0, 0, FALSE, 0, (const void*)0); _SglThrowErrorOccuried();

            //gl->BindVertexBuffer(0, 0, 0, 12);
        }
        else
        {
            GLint glsiz;
            GLenum gltype;
            GLuint glStride;

            GLuint lastBuf = NIL;

            for (afxNat i = 0; i < inStreamCnt; i++)
            {
                afxPipelineInputStream const *layout = &(pip->inStreams[i]);

                switch (layout->format)
                {
                case AFX_VTX_FMT_REAL:
                    glsiz = 1;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                case AFX_VTX_FMT_V2D:
                    glsiz = 2;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                case AFX_VTX_FMT_V3D:
                    glsiz = 3;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                case AFX_VTX_FMT_V4D:
                case AFX_VTX_FMT_XYZW32Q:
                    glsiz = 4;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                default:
                    glsiz = 0;
                    gltype = NIL;
                    glStride = 0;
                    AfxThrowError();
                    break;
                }

                afxNat binding = layout->binding;
                afxSize stride;
                afxSize offset;

                if (FALSE == dque->state.vertexBindings[binding].wild)
                {
                    afxVertexBuffer vbuf = dque->state.vertexBindings[binding].vbuf;
                    afxNat baseVtx = dque->state.vertexBindings[binding].baseVtx;
                    afxNat vtxArr = dque->state.vertexBindings[binding].arr;
                    offset = AfxVertexBufferGetOffset(vbuf, baseVtx, vtxArr);
                    stride = glStride;

                    if (vbuf->buf.gpuHandle != lastBuf)
                    {
                        gl->BindBuffer(GL_ARRAY_BUFFER, vbuf->buf.gpuHandle); _SglThrowErrorOccuried();
                    }
                    lastBuf = vbuf->buf.gpuHandle;
                }
                else
                {
                    afxBuffer buf = dque->state.vertexBindings[binding].buf;
                    offset = dque->state.vertexBindings[binding].offset;
                    stride = dque->state.vertexBindings[binding].stride;

                    if (buf->gpuHandle != lastBuf)
                    {
                        gl->BindBuffer(GL_ARRAY_BUFFER, buf->gpuHandle); _SglThrowErrorOccuried();
                    }
                    lastBuf = buf->gpuHandle;
                }

                gl->EnableVertexAttribArray(layout->location); _SglThrowErrorOccuried();
                gl->VertexAttribPointer(layout->location, glsiz, gltype, FALSE, stride, (const void*)offset); _SglThrowErrorOccuried();
                int a = 0;
            }
        }
    }
}
#endif

// DO WORK

_SGL void _SglFlushVertexStateChanges(afxDrawQueue dque)
{
    
}

_SGL void _SglDqueDraw(afxDrawQueue dque, _afxDscrCmdDraw const *cmd)
{
    afxError err = AFX_ERR_NONE;
    sglVmt const*gl = &dque->wglVmt;
    
    //_SglDqueUpdateNextStateObjects(dctx, queueIdx);
    //_SglDqueFlushNextState(dctx, queueIdx);

    AfxAssert(dque->state.pip);
    //AfxAssert(dque->state.vertexBindingCnt);

    AfxAssert(cmd->vtxCnt);
    AfxAssert(cmd->instCnt);
    GLenum top = AfxToGlTopology(dque->state.ia.topology);

    if (cmd->instCnt > 1)
    {
        gl->DrawArraysInstanced(top, cmd->firstVtx, cmd->vtxCnt, cmd->instCnt); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawArrays(top, cmd->firstVtx, cmd->vtxCnt); _SglThrowErrorOccuried();
    }
    dque->numOfFedVertices += cmd->vtxCnt;
    dque->numOfFedInstances += cmd->instCnt;

    //AfxEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _SglDqueDrawIndexed(afxDrawQueue dque, _afxDscrCmdDrawIndexed const* cmd)
{
    /*
        When the command is executed, primitives are assembled using the current primitive topology and indexCount vertices whose indices are retrieved from the index buffer. 
        The index buffer is treated as an array of tightly packed unsigned integers of size defined by the vkCmdBindIndexBuffer::indexType parameter with which the buffer was bound.

        The first vertex index is at an offset of firstIndex × indexSize + offset within the bound index buffer, where offset is the offset specified by CmdBindIndexBuffer and indexSize is the byte size of the type specified by indexType. 
        Subsequent index values are retrieved from consecutive locations in the index buffer. 
        Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the indexType is VK_INDEX_TYPE_UINT8_EXT or VK_INDEX_TYPE_UINT16) and have vertexOffset added to them, before being supplied as the vertexIndex value.

        The primitives are drawn instanceCount times with instanceIndex starting with firstInstance and increasing sequentially for each instance. 
        The assembled primitives execute the bound graphics pipeline.
    */

    afxError err = AFX_ERR_NONE;
    sglVmt const*gl = &dque->wglVmt;

    //_SglDqueUpdateNextStateObjects(dctx, queueIdx);
    //_SglDqueFlushNextState(dctx, queueIdx);

    AfxAssert(dque->state.pip);
    AfxAssertObject(dque->state.indexBinding.buf, AFX_FCC_BUF);
    AfxAssert(dque->state.vertexBindingCnt);

    AfxAssert(cmd->idxCnt);
    AfxAssert(cmd->instCnt);

    static const GLenum idxSizGl[] =
    {
        0,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_SHORT,
        0,
        GL_UNSIGNED_INT
    };

    afxSize idxBaseOff = dque->state.indexBinding.offset;
    afxSize idxSiz = dque->state.indexBinding.idxSiz;

    GLint vtxOff2 = cmd->vtxOff;

    GLenum top = AfxToGlTopology(dque->state.ia.topology);

    if (cmd->instCnt > 1)
    {
        gl->DrawElementsInstancedBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), cmd->instCnt, vtxOff2); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawElementsBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), vtxOff2); _SglThrowErrorOccuried();
    }

    dque->numOfFedIndices += cmd->idxCnt;
    dque->numOfFedInstances += cmd->instCnt;
    //AfxEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);

}

void (*_SglDqueVmt[AFX_DSC_TOTAL])(afxDrawQueue dque, _afxDscrCmd const *cmd) =
{
    NIL,

    (void*)_SglDqueBeginCombination,
    (void*)_SglDqueBeginOperation,
    (void*)_SglDqueEmployTechnique,
    (void*)_SglDqueNextPass,
    (void*)_SglDqueEndOperation,
    (void*)_SglDqueEndCombination,

    (void*)_SglDqueBindPipeline,

    (void*)_SglDqueSetInputAssemblyState,
    (void*)_SglDqueSetViewports,
    (void*)_SglDqueSetRasterizerState,
    (void*)_SglDqueSetDepthState,
    (void*)_SglDqueSetScissors,

    (void*)_SglDqueBindBuffers,
    (void*)_SglDqueBindVertexBuffers,
    (void*)_SglDqueBindIndexBuffer,
    (void*)_SglDqueBindTextures,

    (void*)_SglDqueDraw,
    (void*)_SglDqueDrawIndexed
};










// QUEUE STUFF

_SGL afxResult _SglDqueImplDrawWorkStreaming(afxDrawQueue dque, afxDrawWorkload const *dwrk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssert(dwrk);

    AfxGetTime(&dque->lastStreamBeginTime);

    afxDrawContext dctx = AfxObjectGetProvider(&dque->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    for (afxNat j = 0; j < dwrk->streaming.resCnt; j++)
    {
        afxObject *obj = dwrk->streaming.resources[j];
        //AfxAssertObject(dscr, AFX_FCC_DSCR);
        //AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);
        //_SglDqueExecuteDscr(dque, dscr);

        HDC dc = _wglGetCurrentDC();
        HGLRC rc = _wglGetCurrentContext();
        AfxAssert(rc == dque->wglGlrc);

        if (dc != dque->wglDc)
        {
            if (!_wglMakeCurrent(dque->wglDc, dque->wglGlrc))
            {
                AfxThrowError();
                AfxError("dque %p failed to be set.", dque);
                _wglMakeCurrent(dque->wglDc, dque->wglGlrc);
            }
        }
    }

    AfxGetTime(&dque->lastStreamEndTime);
    
    return err;
}

_SGL afxError _SglDqueExecuteDscr(afxDrawQueue dque, afxDrawScript dscr)
{
    //AfxEntry("din/%.4s=%p,dctx=%p,dout/%.4s=%p", (afxChar*)&din->impl->fcc, din, dctx, (afxChar*)&dout->impl->fcc, dout);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    //sglVmt const* gl = &dque->wglVmt;

    AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);

    _afxDscrCmd *cmdHdr;
    AfxChainForEveryLinkageB2F(&dscr->commands, _afxDscrCmd, script, cmdHdr)
    {
        if (cmdHdr->id == AFX_DSC_END)
            break;

        _SglDqueVmt[cmdHdr->id](dque, cmdHdr);
    }

    dscr->state = AFX_DSCR_STATE_EXECUTABLE;
    return err;
}

_SGL afxResult _SglDqueImplDrawWorkSubmission(afxDrawQueue dque, afxDrawWorkload const *dwrk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssert(dwrk);

    AfxGetTime(&dque->lastSubmisBeginTime);

    //afxDrawContext dctx = AfxObjectGetProvider(&dque->obj);
    //AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxSize frameTime = AfxGetTimer();

    if (1000 <= (frameTime - dque->swapTime))
    {
        dque->swapTime = frameTime;
        dque->fps = dque->frameNum;
        dque->frameNum = 0;
    }

    // deveriam ser movidos para afxDrawInput para separação e exibição correta para cada saída?

    ++dque->frameNum;
    dque->frameTime = frameTime;
    dque->numOfFedVertices = 0;
    dque->numOfFedIndices = 0;
    dque->numOfFedInstances = 0;

    for (afxNat j = 0; j < dwrk->submission.scriptCnt; j++)
    {
        afxDrawScript dscr = dwrk->submission.scripts[j];
        AfxAssertObject(dscr, AFX_FCC_DSCR);
        AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);

        HDC dc = _wglGetCurrentDC();
        HGLRC rc = _wglGetCurrentContext();
        AfxAssert(rc == dque->wglGlrc);

        if (dc != dque->wglDc)
        {
            if (!_wglMakeCurrent(dque->wglDc, dque->wglGlrc))
            {
                AfxThrowError();
                AfxError("dque %p failed to be set.", dque);
                //_wglMakeCurrent(dque->wglDc, dque->wglGlrc);
            }
        }

        _SglDqueExecuteDscr(dque, dscr);
    }

    AfxGetTime(&dque->lastSubmisEndTime);

    return err;
}

_SGL afxError _SglDquePresentSurface(afxDrawQueue dque, afxDrawOutput dout, afxNat outBufIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObject(dout, AFX_FCC_DOUT);
    sglVmt const* gl = &dque->wglVmt;

    AfxAssert(!dout->swapping);

    {
        dout->swapping = TRUE;

        afxSurface surf = dout->buffers[outBufIdx];
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
#if 0
        if (!dque->presentFboGpuHandle)
        {
            gl->GenFramebuffers(1, &(dque->presentFboGpuHandle)); _SglThrowErrorOccuried();
        }
        gl->BindFramebuffer(GL_FRAMEBUFFER, dque->presentFboGpuHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(dque->presentFboGpuHandle));
        gl->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, surf->tex.glTarget, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();

        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->BlitFramebuffer(0, 0, surf->tex.whd[0], surf->tex.whd[1], 0, 0, surf->tex.whd[0], surf->tex.whd[1], GL_COLOR_BUFFER_BIT, GL_NEAREST); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Flush();
        //gl->Finish();
        //gl->DeleteFramebuffers(1, &(gpuHandle)); _SglThrowErrorOccuried();
#else
        //if (dout->presentMode == AFX_PRESENT_MODE_FIFO)
            //surf = AfxContainerOf(AfxChainGetEnd(&dout->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else //if (dout->presentMode == AFX_PRESENT_MODE_LIFO)
            //surf = AfxContainerOf(AfxChainGetBegin(&dout->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else AfxError("Not implemented yet.");

        //_SglDqueBindAndSyncPip(dque->presentPip, dque);
        //_SglDqueBindAndSyncBuf(&(dque->presentVbuf->buf), 0, 0, 0, GL_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
        //_SglDqueBindAndSyncLego(dque->presentLego, 0, dque);

#if !0
        
        //_AfxStdCanvUpdate(cmd->beginRenderPass.canv, dque);

        afxWhd extent;
        AfxTextureGetExtent(&surf->tex, 0, extent);
        
        _afxDscrCmdBeginOp cmdBeginOp;
        cmdBeginOp.dop = dque->presentDop;
        cmdBeginOp.tecIdx = 1;
        cmdBeginOp.canv = NIL;
        cmdBeginOp.area = (afxRect const){ { { 0, 0 } }, { { extent[0], extent[1] } } };
        cmdBeginOp.annexCnt = 1;
        cmdBeginOp.annexes[0] = (afxRenderPassAnnex const){ TRUE, FALSE, { .color = { 0.3, 0.0, 0.3, 1 } } };

        _SglDqueBeginOperation(dque, &cmdBeginOp);

#else
        afxDrawTarget const rasterRt = { NIL, AFX_SURFACE_LOAD_OP_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _SglDqueBeginCombination(dque, NIL, 1, 0, &rasterRt, NIL, NIL);

        afxPipeline pip = AfxDrawOperationGetPipeline(dque->presentDop, 0, 0);
        AfxAssertObject(pip, AFX_FCC_PIP);
        _SglDqueBindPipeline(dque, pip);
#endif   
        //_SglDqueEmployTechnique(dque, 0);

        _afxDscrCmdSetVp cmdSetVp;
        cmdSetVp.first = 0;
        cmdSetVp.cnt = 1;
        cmdSetVp.vp[0] = (afxViewport const){ { 0, 0 }, { extent[0], extent[1] }, { 0, 1 } };

        _SglDqueSetViewports(dque, &cmdSetVp);

#if 0 // already set by pipeline
        afxPipelineRasterizerState const ras = { FALSE, FALSE, AFX_FILL_MODE_SOLID, AFX_CULL_MODE_BACK, AFX_FRONT_FACE_CCW, FALSE, 0, 0, 0, 1 };
        _SglDqueSetRasterizerState(dque, &ras);

        afxPipelineDepthState const depth = { 0 };
        _SglDqueSetDepthState(dque, &depth);
#endif

#if 0 // already set by pipeline
        afxPipelineInputAssemblyState ia;
        ia.topology = AFX_PRIM_TOPOLOGY_TRI_STRIP;
        ia.primRestartEnable = FALSE;
        _SglDqueSetInputAssemblyState(dque, &ia);
#endif
#if 0
        afxNat const baseVtxs[] = { 0, 0 };
        afxNat const vtxArrs[] = { 0, 1 };
        afxVertexBuffer vbufs[] = { dque->rgbRectVbo, dque->rgbRectVbo };
        _SglDqueBindVertexBuffers(dque, 0, 2, vbufs, baseVtxs, vtxArrs);
#endif

        _afxDscrCmdBindVbuf cmdBindVbuf;
        cmdBindVbuf.first = 0;
        cmdBindVbuf.cnt = 1;
        cmdBindVbuf.buf[0] = &dque->presentVbuf->buf;
        cmdBindVbuf.offset[0] = 0;
        cmdBindVbuf.vtxArrSiz[0] = 0;
        cmdBindVbuf.vtxStride[0] = AfxVertexBufferGetStride(dque->presentVbuf, 0);
    
        _SglDqueBindVertexBuffers(dque, &cmdBindVbuf);

        //_SglDqueBindLegos(dque, 0, 1, &dque->presentLego);
        _afxDscrCmdBindTextures cmdBindTex;
        cmdBindTex.set = 0;
        cmdBindTex.first = 0;
        cmdBindTex.cnt = 1;
        cmdBindTex.smp[0] = dque->presentSmp;
        cmdBindTex.tex[0] = &surf->tex;

        _SglDqueBindTextures(dque, &cmdBindTex);

        _afxDscrCmdDraw cmdDraw;
        cmdDraw.vtxCnt = 4;
        cmdDraw.instCnt = 1;
        cmdDraw.firstVtx = 0;
        cmdDraw.firstInst = 0;
        _SglDqueDraw(dque, &cmdDraw);

#if !0
        _SglDqueEndOperation(dque, NIL);
#else
        _SglDqueEndCombination(dque, NIL);
#endif
#endif
        //AfxLinkageDrop(&surf->swapchain);
        surf->state = AFX_SURF_STATE_IDLE;
        surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;

        // deadlocks all
        _SglSwapBuffers(dout->wglDc);

        dout->swapping = FALSE;
        (void)gl;
    }
    return err;
}

_SGL afxResult _SglDqueImplDrawWorkPresentation(afxDrawQueue dque, afxDrawWorkload const *dwrk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssert(dwrk);

    AfxGetTime(&dque->lastPresentBeginTime);

    afxDrawContext dctx = AfxObjectGetProvider(&dque->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    for (afxNat j = 0; j < dwrk->presentation.outputCnt; j++)
    {
        afxDrawOutput dout = dwrk->presentation.outputs[j];
        AfxAssertObject(dout, AFX_FCC_DOUT);

        afxSurface surf = AfxDrawOutputGetBuffer(dout, dwrk->presentation.outputBufIdx[j]);
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(surf->state == AFX_SURF_STATE_PRESENTABLE);
        surf->state = AFX_SURF_STATE_PENDING;

        AfxStringFormat(dout->title, "%i FPS, %d/%d/%d --- 4D Draw System (SIGGL %u.%u.%u) --- Public Test Build --- (c) 2017 SIGMA Technology Group", dque->fps, dque->numOfFedVertices, dque->numOfFedIndices, dque->numOfFedInstances,  dque->wglGlrcVerMajor, dque->wglGlrcVerMinor, dque->wglGlrcVerPatch);

        HDC dc = _wglGetCurrentDC();
        HGLRC rc = _wglGetCurrentContext();
        AfxAssert(rc == dque->wglGlrc);

        if (dc != dout->wglDc)
        {
            if (!_wglMakeCurrent(dout->wglDc, dque->wglGlrc))
            {
                AfxThrowError();
                AfxError("dque %p failed to set to draw output device %p.", dque, dout);
                _wglMakeCurrent(dque->wglDc, dque->wglGlrc);
            }
        }

        _SglDquePresentSurface(dque, dout, dwrk->presentation.outputBufIdx[j]);
    }
    AfxGetTime(&dque->lastPresntEndTime);

    return err;
}

#if 0
_SGL afxError AfxDrawQueueWaitForIdle(afxDrawQueue dque)
{

}
#endif

_SGL void _SglDqueProcess(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    afxNat queueIdx = dque->queueIdx;
    afxDrawContext dctx = AfxObjectGetProvider(&(dque->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    sglVmt const* gl = &dque->wglVmt;
    _sglDeleteGlRes *delRes;

    if (AfxTransistorTryEnterExclusive(&dque->deletionLock))
    {
        if ((delRes = AfxQueuePull(&dque->deletionQueue)))
        {
            switch (delRes->type)
            {
            case 0: // buf
                gl->DeleteBuffers(1, &delRes->gpuHandle);
                break;
            case 1: // tex
                gl->DeleteTextures(1, &delRes->gpuHandle);
                break;
            case 2: // surf
                gl->DeleteRenderbuffers(1, &delRes->gpuHandle);
                break;
            case 3: // canv
                gl->DeleteFramebuffers(1, &delRes->gpuHandle);
                break;
            case 4: // smp
                gl->DeleteSamplers(1, &delRes->gpuHandle);
                break;
            case 5: // pip
                gl->DeleteProgram(delRes->gpuHandle);
                break;
            case 6: // shd
                gl->DeleteShader(delRes->gpuHandle);
                break;
            default:
                AfxError("");
                break;
            }
            AfxQueuePop(&dque->deletionQueue);
        }
        AfxTransistorExitExclusive(&dque->deletionLock);
    }

    if (dctx->open)
    {
        if (AfxTransistorTryEnterShared(&dctx->dinClass.transitor))
        {
            afxDrawInput din;
            AfxChainForEveryLinkageB2F(&dctx->dinClass.instances, AFX_OBJECT(afxDrawInput), obj.cls, din)
            {
                AfxAssertObject(din, AFX_FCC_DIN);

                if (din->prefetchThreadEnabled[queueIdx])
                {
                    afxDrawWorkload const *dwrk;

                    _AfxDrawInputProcess(din, dque->queueIdx); // should just do input to script, using just CPU resources.

                    if (din->streamingThreadEnabled[queueIdx])
                    {
                        if (AfxTransistorTryEnterExclusive(&din->streamingLock))
                        {
                            if ((dwrk = AfxQueuePull(&din->streamingQueue)))
                            {
                                _SglDqueImplDrawWorkStreaming(dque, dwrk);
                                AfxQueuePop(&din->streamingQueue);
                            }
                            AfxTransistorExitExclusive(&din->streamingLock);
                        }
                    }

                    if (!dque->submissionSuspended)
                    {
                        if (din->submissionThreadEnabled[queueIdx])
                        {
                            if (AfxTransistorTryEnterExclusive(&din->submissionLock))
                            {
                                if ((dwrk = AfxQueuePull(&din->submissionQueue)))
                                {
                                    _SglDqueImplDrawWorkSubmission(dque, dwrk);
                                    AfxQueuePop(&din->submissionQueue);
                                }
                                AfxTransistorExitExclusive(&din->submissionLock);
                            }
                        }
                    }

                    if (!dque->presentationSuspended)
                    {
                        if (din->presentationThreadEnabled[queueIdx])
                        {
                            if (AfxTransistorTryEnterExclusive(&din->presentationLock))
                            {
                                if ((dwrk = AfxQueuePull(&din->presentationQueue)))
                                {
                                    _SglDqueImplDrawWorkPresentation(dque, dwrk);
                                    AfxQueuePop(&din->presentationQueue);
                                }
                                AfxTransistorExitExclusive(&din->presentationLock);
                            }
                        }
                    }
                }
            }
            AfxTransistorExitShared(&dctx->dinClass.transitor);
        }
    }
}

_SGL void _SglDqueWorkerThreadExec(afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(thr, AFX_FCC_THR);
    afxDrawQueue dque = AfxThreadGetUdd(thr);
    _SglDqueProcess(dque);
}

_SGL void _SglEnqueueGlResourceDeletion(afxDrawContext dctx, afxNat queueIdx, afxNat type, GLuint gpuHandle)
{
    afxDrawQueue dque = dctx->queues[queueIdx];
    AfxTransistorEnterExclusive(&dque->deletionLock);
    _sglDeleteGlRes delRes;
    delRes.gpuHandle = gpuHandle;
    delRes.type = type;
    AfxQueuePush(&dque->deletionQueue, &delRes);
    AfxTransistorExitExclusive(&dque->deletionLock);
};

_SGL void _SglDqueWorkerThreadStart(afxThread thr, afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(thr, AFX_FCC_THR);
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxDrawContext dctx = AfxObjectGetProvider(&(dque->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawDriver ddrv = AfxDrawContextGetDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDriverIdd *ddrvIdd = AfxDrawDriverGetIdd(ddrv);

    if (_SglCreateCombinedDeviceContext(&ddrvIdd->oglWndClss, ddrvIdd->wglPrimeGlrc, &(dque->wglWnd), &(dque->wglDc), &(dque->wglGlrc))) AfxThrowError();
    else
    {
        dque->wglDcPxlFmt = _wglGetPixelFormat(dque->wglDc);

        if (!(_wglMakeCurrent(dque->wglDc, dque->wglGlrc))) AfxThrowError();
        else
        {
            sglVmt* gl = (sglVmt*)&dque->wglVmt;
            _SglLoadVmt(gl, 1, 0);

            afxString ver;
            AfxStringWrapLiteral(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
            AfxStringScan(&ver, "%u.%u.%u", &dque->wglGlrcVerMajor, &dque->wglGlrcVerMinor, &dque->wglGlrcVerPatch);
            gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(dque->wglGlrcVerMajor));
            gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(dque->wglGlrcVerMinor));

            _SglLoadVmt(gl, 4, 6);

            AfxStringWrapLiteral(&ver, (afxChar const*)gl->GetString(GL_RENDERER), 0);
            AfxStringCopy(dque->subsysName, &ver);
            //AfxComment("4D Draw System (OpenGL/Vulkan Continuous Integration (%.*s v%u.%u.%u))", AfxPushString(&dque->subsysName.str), dque->wglGlrcVerMajor, dque->wglGlrcVerMinor, dque->wglGlrcVerPatch);
            //SetWindowTextA(dque->wglWnd, AfxStringGetData(&ver));

            //gl->Enable(GL_FRAMEBUFFER_SRGB);
            gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }

        if (err)
        {
            _wglMakeCurrent(NIL, NIL);
            _wglDeleteContext(dque->wglGlrc);
            ReleaseDC(dque->wglWnd, dque->wglDc);
            DestroyWindow(dque->wglWnd);
        }
    }

}

_SGL afxBool _SglDqueEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawQueue dque = (void*)obj;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglDqueEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawQueue dque = (void*)obj;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxDqueDtor(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    dque->submissionSuspended = TRUE;
    dque->presentationSuspended = TRUE;
#if 0
    AfxObjectRelease(&(dque->rgbRectVbo->buf.obj));
    AfxObjectRelease(&(dque->rgbRectPip->res.obj));
    AfxObjectRelease(&(dque->rgbRectLego->obj));
    AfxObjectRelease(&(dque->rgbRectSmp->obj));
#endif

    if (dque->thread)
    {
        AfxObjectRelease(&dque->thread->obj);
    }
    
    AfxQueueDrop(&dque->deletionQueue);
    AfxTransistorDrop(&dque->deletionLock);

    if (dque->thread)
    {
        _wglMakeCurrent(NIL, NIL);
        _wglDeleteContext(dque->wglGlrc);
        ReleaseDC(dque->wglWnd, dque->wglDc);
        DestroyWindow(dque->wglWnd);
    }

    if (dque->subsysName)
        AfxStringDeallocate(dque->subsysName);

    return err;
}

_SGL afxError _AfxDqueCtor(void *cache, afxNat idx, afxDrawQueue dque, afxDrawQueueSpecification const *specs)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxDrawQueueSpecification const *spec = &specs[idx];

    afxDrawContext dctx = AfxObjectGetProvider(&(dque->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxDrawQueueGetDriver(dque);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    AfxTransistorDeploy(&dque->deletionLock);
    AfxQueueDeploy(&dque->deletionQueue, sizeof(_sglDeleteGlRes), 32);
    AfxChainDeploy(&dque->instantiationList, dque);

    dque->queueIdx = spec->idx;

    dque->subsysName = NIL;

    dque->numOfFedIndices = 0;
    dque->numOfFedVertices = 0;
    dque->numOfFedInstances = 0;

    dque->deltaTime = 0.f;
    dque->frameTime = 0;
    dque->swapTime = 0;
    dque->frameNum = 0;
    dque->fps = 0;

    dque->submissionSuspended = FALSE;
    dque->presentationSuspended = FALSE;
    
    dque->activeTmpFboIdx = 0;
    dque->activeVaoIdx = 0;
    dque->presentFboGpuHandle = 0;
    
    AfxZero(dque->vao, sizeof(dque->vao));
    AfxZero(dque->tmpFbo, sizeof(dque->tmpFbo));
    
    AFX_ZERO(&dque->state);


    afxUri uri;
    AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgba.xml", 0);
    //AfxUriWrapLiteral(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.xml", 0);
    //dque->presentPip = AfxDrawContextFetchPipeline(dctx, &uri);

    AfxDrawContextAcquireOperations(dctx, 1, &uri, &dque->presentDop);

    AfxAssertObject(dque->presentDop, AFX_FCC_DOP);

    afxSamplerSpecification smpSpec = { 0 };
    smpSpec.magFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.minFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.mipmapFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.uvw[0] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[1] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[2] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit

    AfxDrawContextAcquireSamplers(dctx, 1, &smpSpec, &dque->presentSmp);
    AfxAssertObject(dque->presentSmp, AFX_FCC_SMP);

    afxString tmpStr;
    AfxStringWrapLiteral(&tmpStr, "a_xy", 0);
    const afxV2d vtxPos[] =
    {
        { -1.0,  1.0 },
        { -1.0, -1.0 },
        {  1.0,  1.0 },
        {  1.0, -1.0 },
    };

    afxVertexBufferBlueprint vbub;
    AfxVertexBufferBlueprintReset(&vbub, 4);
    AfxVertexBufferBlueprintAddRow(&vbub, &tmpStr, AFX_VTX_FMT_V2D, AFX_VTX_USAGE_POS, vtxPos, AFX_VTX_FMT_V2D);
    AfxDrawContextBuildVertexBuffers(dctx, 1, &vbub, &dque->presentVbuf);
    AfxAssertObject(dque->presentVbuf, AFX_FCC_VBUF);

    if (spec->autonomous)
    {
        dque->thread = AfxSystemAcquireThread(AfxSystemGet(), (void*)_SglDqueWorkerThreadStart, dque, _SglDqueWorkerThreadExec, AfxSpawnHint());
    }
    else
    {
        dque->thread = NIL;

        _sglDriverIdd *idd = AfxDrawDriverGetIdd(ddrv);

        sglVmt* gl = (sglVmt*)&dque->wglVmt;
        *gl = idd->wglPrimeVmt;
        dque->wglWnd = idd->wglPrimeWnd;
        dque->wglDc = idd->wglPrimeDc;
        dque->wglDcPxlFmt = idd->wglPrimeDcPxlFmt;
        dque->wglGlrc = idd->wglPrimeGlrc;
        dque->wglGlrcVerMajor = idd->wglPrimeGlrcVerMajor;
        dque->wglGlrcVerMinor = idd->wglPrimeGlrcVerMinor;
        dque->wglGlrcVerPatch = idd->wglPrimeGlrcVerPatch;
        dque->subsysName = idd->subsysName && !AfxStringIsEmpty(idd->subsysName) ? AfxStringClone(idd->subsysName) : NIL;

        gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }


    dque->instanced = TRUE;


    return err;
}

_SGL afxClassSpecification const _AfxDqueClassSpec;

afxClassSpecification const _AfxDqueClassSpec =
{
    AFX_FCC_DQUE,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawQueue)),
    NIL,
    (void*)_AfxDqueCtor,
    (void*)_AfxDqueDtor,
    .event = _SglDqueEventHandler,
    .eventFilter = _SglDqueEventFilter,
    "afxDrawQueue",
    NIL
};
