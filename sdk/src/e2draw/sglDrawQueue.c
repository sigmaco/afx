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

#define _AFX_SURFACE_C
#define _AFX_TEXTURE_C
#define _AFX_CANVAS_C
#define _AFX_LEGO_C
#define _AFX_PIPELINE_C
#define _AFX_DRAW_QUEUE_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#include "sgl.h"

#include "afx/core/afxSystem.h"
#include "../e2coree/draw/_classified/afxDrawClassified.h"

// RENDERING SCOPE

_SGL void _SglDqueEndDrawPass(afxDrawQueue dque, _afxDscrCmd const *cmd, glVmt const* gl)
{
    (void)cmd;
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;

    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();

    _SglDqueBindAndSyncCanv(dque, NIL, GL_DRAW_FRAMEBUFFER, gl);
    gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();


    if (!idd->state.renderPass.canv && idd->state.renderPass.rasters[0].surf)
    {
        AfxAssert(idd->tmpFbo[idd->activeTmpFboIdx]);
    }

    for (afxNat i = 0; i < idd->state.renderPass.rasterCnt; i++)
    {
        afxDrawTarget const *target = &idd->state.renderPass.rasters[i];
        afxSurface surf = target->surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW));
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            ((sglTexIdd*)surf->tex.idd)->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;

            // AFX_DTS_STORE_OP_STORE
#if 0
            gl->BindTexture(surf->tex.glTarget, surf->tex.gpuTexHandle);
            gl->GetTexImage(surf->tex.glTarget, 0, surf->tex.glFmt, surf->tex.glType, (void*)AfxTextureGetData(&surf->tex, NIL, 0));
            gl->BindTexture(surf->tex.glTarget, 0);
#endif
        }
    }

    if (idd->state.renderPass.depthRt.surf)
    {
        afxSurface surf = idd->state.renderPass.depthRt.surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW));
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            ((sglTexIdd*)surf->tex.idd)->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }

    if (idd->state.renderPass.stencilRt.surf)
    {
        afxSurface surf = idd->state.renderPass.stencilRt.surf;

        if (surf)
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW));
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            ((sglTexIdd*)surf->tex.idd)->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }
};

_SGL void _SglDqueBeginDrawPassRasterTargets(afxDrawQueue dque, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;

    afxCanvas const canv = idd->state.renderPass.canv;
    afxNat rasterCnt = idd->state.renderPass.rasterCnt;
    afxNat colorAttchEnabledCnt = 0;
    GLenum drawBuffersIndices[_SGL_MAX_SURF_PER_CANV];    
    
    for (afxNat i = 0; i < rasterCnt; i++)
    {
        afxDrawTarget const *target = &idd->state.renderPass.rasters[i];
        afxSurface surf;

        if ((surf = target->surf))
        {
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(AfxTestTexture(&surf->tex, AFX_TEX_USAGE_DRAW));
            AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            surf->state = AFX_SURF_STATE_PAINTING;

            if (!canv) // will draw directly to surface (with no canvas)?
            {
                _SglDqueBindAndSyncTex(dque, 0, &surf->tex, gl);
                _SglDqueBindAndSyncTex(dque, 0, NIL, gl);
                _SglDqueSurfSync(dque, surf, gl);
                _SglDqueSurfSync(dque, NIL, gl);
                AfxAssert(surf->idd);
                AfxAssert(surf->tex.idd);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, ((sglSurfIdd*)surf->idd)->glHandle); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, ((sglTexIdd*)surf->tex.idd)->glHandle, 0); _SglThrowErrorOccuried();
            }
        }
        else
        {
            AfxThrowError();
        }

        if (target->loadOp == AFX_DTS_LOAD_OP_CLEAR)
        {
            afxReal const *color = target->clearValue.color;
            //gl->DrawBuffer(GL_COLOR_ATTACHMENT0 + i); _SglThrowErrorOccuried();
            gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
            //gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();

            if (!canv && !idd->state.renderPass.rasters[0].surf) // will draw directly to default fb?
            {
                gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else
            {
                gl->ClearBufferfv(GL_COLOR, i, color); _SglThrowErrorOccuried();
            }
        }

        if (target->storeOp == AFX_DTS_STORE_OP_STORE)
        {
            drawBuffersIndices[colorAttchEnabledCnt] = GL_COLOR_ATTACHMENT0 + i;
            colorAttchEnabledCnt++;
        }
    }

    gl->DrawBuffers(colorAttchEnabledCnt, drawBuffersIndices); _SglThrowErrorOccuried();
}

_SGL void _SglDqueBeginDrawPassDepthTargets(afxDrawQueue dque, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;

    afxCanvas const canv = idd->state.renderPass.canv;
    afxBool combinedDs = FALSE;

    afxDrawTarget const *targetD = &idd->state.renderPass.depthRt;
    afxDrawTarget const *targetS = &idd->state.renderPass.stencilRt;
    afxSurface surfD = targetD->surf;
    afxSurface surfS = targetS->surf;
    combinedDs = surfD == surfS;

    if (surfD)
    {
        AfxAssertObject(surfD, AFX_FCC_SURF);
        AfxAssert(AfxTestTexture(&surfD->tex, AFX_TEX_USAGE_DRAW));
        AfxAssert(surfD->state != AFX_SURF_STATE_PAINTING);
        AfxAssert(surfD->state != AFX_SURF_STATE_PENDING);
        surfD->state = AFX_SURF_STATE_PAINTING;
    }

    if (!combinedDs && surfS)
    {
        AfxAssertObject(surfS, AFX_FCC_SURF);
        AfxAssert(AfxTestTexture(&surfS->tex, AFX_TEX_USAGE_DRAW));
        AfxAssert(surfS->state != AFX_SURF_STATE_PAINTING);
        AfxAssert(surfS->state != AFX_SURF_STATE_PENDING);
        surfS->state = AFX_SURF_STATE_PAINTING;
    }

    if (!canv)
    {
        if (combinedDs)
        {
            if (surfD)
            {
                _SglDqueBindAndSyncTex(dque, 0, &surfD->tex, gl);
                _SglDqueBindAndSyncTex(dque, 0, NIL, gl);
                _SglDqueSurfSync(dque, surfD, gl);
                _SglDqueSurfSync(dque, NIL, gl);
                AfxAssert(surfD->idd);
                AfxAssert(surfD->tex.idd);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ((sglSurfIdd*)surfD->idd)->glHandle); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, ((sglTexIdd*)surfD->tex.idd)->glHandle, 0); _SglThrowErrorOccuried();
            }
        }
        else
        {
            if (surfD)
            {
                _SglDqueBindAndSyncTex(dque, 0, &surfD->tex, gl);
                _SglDqueBindAndSyncTex(dque, 0, NIL, gl);
                _SglDqueSurfSync(dque, surfD, gl);
                _SglDqueSurfSync(dque, NIL, gl);
                AfxAssert(surfD->idd);
                AfxAssert(surfD->tex.idd);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ((sglSurfIdd*)surfD->idd)->glHandle); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ((sglTexIdd*)surfD->tex.idd)->glHandle, 0); _SglThrowErrorOccuried();
            }

            if (surfS)
            {
                _SglDqueBindAndSyncTex(dque, 0, &surfS->tex, gl);
                _SglDqueBindAndSyncTex(dque, 0, NIL, gl);
                _SglDqueSurfSync(dque, surfS, gl);
                _SglDqueSurfSync(dque, NIL, gl);
                AfxAssert(surfS->idd);
                AfxAssert(surfS->tex.idd);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ((sglSurfIdd*)surfS->idd)->glHandle); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, ((sglTexIdd*)surfS->tex.idd)->glHandle, 0); _SglThrowErrorOccuried();
            }
        }
    }

    if (combinedDs)
    {
        if (targetD->loadOp == AFX_DTS_LOAD_OP_CLEAR)
        {
            //gl->DrawBuffer(GL_COLOR_ATTACHMENT0 + i); _SglThrowErrorOccuried();
            //gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();

            if (!canv && !idd->state.renderPass.rasters[0].surf) // will draw directly to default fb?
            {
                gl->ClearDepth(targetD->clearValue.depth); _SglThrowErrorOccuried();
                gl->ClearStencil(targetS->clearValue.stencil); _SglThrowErrorOccuried();
                gl->Clear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else
            {
                gl->ClearBufferfi(GL_DEPTH_STENCIL, 0, targetD->clearValue.depth, targetS->clearValue.stencil); _SglThrowErrorOccuried();
            }
        }
    }
    else
    {
        if (targetD->loadOp == AFX_DTS_LOAD_OP_CLEAR)
        {
            if (!canv && !idd->state.renderPass.rasters[0].surf) // will draw directly to default fb?
            {
                gl->ClearDepth(targetD->clearValue.depth); _SglThrowErrorOccuried();
                gl->Clear(GL_DEPTH_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else
            {
                gl->ClearBufferfv(GL_DEPTH_STENCIL, 0, &targetD->clearValue.depth); _SglThrowErrorOccuried();
            }
        }

        if (targetS->loadOp == AFX_DTS_LOAD_OP_CLEAR)
        {
            if (!canv && !idd->state.renderPass.rasters[0].surf) // will draw directly to default fb?
            {
                gl->ClearStencil(targetS->clearValue.stencil); _SglThrowErrorOccuried();
                gl->Clear(GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else
            {
                gl->ClearBufferiv(GL_DEPTH_STENCIL, 0, targetS->clearValue.stencil); _SglThrowErrorOccuried();
            }
        }
    }

}

_SGL void _SglDqueBeginDrawPass(afxDrawQueue dque, _afxDscrCmdBeginDrawPass const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    // renderArea.width = min(renderArea.width, frameBuffer.width - renderArea.x);

    // renderArea.height = min(renderArea.height, frameBuffer.height - renderArea.y);

    afxCanvas const canv = idd->state.renderPass.canv = cmd->canv;
    idd->state.renderPass.activeSubpass = 0;
    idd->state.renderPass.area = cmd->area;
    idd->state.renderPass.layerCnt = cmd->layerCnt;
    idd->state.renderPass.rasterCnt = canv ? AfxGetAnnexedRasterSurfaceCount(canv) : cmd->rasterCnt;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= cmd->rasterCnt);

    for (afxNat i = 0; i < cmd->rasterCnt; i++)
    {       
        idd->state.renderPass.rasters[i] = cmd->rasters[i];

        if (canv)
            AfxGetAnnexedRasterSurface(canv, i, &idd->state.renderPass.rasters[i].surf);            
    }

    idd->state.renderPass.depthRt = cmd->depth;
    idd->state.renderPass.stencilRt = cmd->stencil;

    _SglDqueBindAndSyncCanv(dque, canv, GL_DRAW_FRAMEBUFFER, gl);

    if (canv)
    {
        AfxGetAnnexedDepthSurface(canv, &idd->state.renderPass.depthRt.surf);
        AfxGetAnnexedStencilSurface(canv, &idd->state.renderPass.stencilRt.surf);            
    }
    else
    {
        if (idd->state.renderPass.rasters[0].surf) // will draw directly to surface (with no canvas)?
        {
            idd->activeTmpFboIdx = (idd->activeTmpFboIdx + 1) % _SGL_MAX_SWAPCHAIN_CAP;

            if (idd->tmpFbo[idd->activeTmpFboIdx])
            {
                gl->DeleteFramebuffers(1, &(idd->tmpFbo[idd->activeTmpFboIdx])); _SglThrowErrorOccuried();
            }
            gl->GenFramebuffers(1, &(idd->tmpFbo[idd->activeTmpFboIdx])); _SglThrowErrorOccuried();
            gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, idd->tmpFbo[idd->activeTmpFboIdx]); _SglThrowErrorOccuried();
            AfxAssert(gl->IsFramebuffer(idd->tmpFbo[idd->activeTmpFboIdx]));
        }
    }

    _SglDqueBeginDrawPassRasterTargets(dque, gl);
    _SglDqueBeginDrawPassDepthTargets(dque, gl);

    if (GL_FRAMEBUFFER_COMPLETE != gl->CheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
        AfxThrowError();
}

_SGL void _SglDqueNextPass(afxDrawQueue dque, _afxDscrCmd const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    (void)cmd;
    ++(idd->state.renderPass.activeSubpass);
}

// STATE SETTING

_SGL void _SglDqueSetScissors(afxDrawQueue dque, _afxDscrCmdSetScissor const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->cnt);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET > cmd->first);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->first + cmd->cnt);
    AfxAssert(!cmd->cnt || (cmd->cnt));

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        AfxRectCopy(&(idd->state.scissorArea[cmd->first + i]), &cmd->rect[i]);
    }

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

_SGL void _SglDqueSetViewports(afxDrawQueue dque, _afxDscrCmdSetVp const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->cnt);
    AfxAssert(_SGL_MAX_VP_PER_SET > cmd->first);
    AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->first + cmd->cnt);
    AfxAssert(!cmd->cnt || (cmd->cnt));

    for (afxNat i = 0; i < cmd->cnt; i++)
        idd->state.vp[cmd->first + i] = cmd->vp[i];

    if (cmd->cnt)
    {
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

_SGL void _SglDqueSetRasterizerState(afxDrawQueue dque, _afxDscrCmdSetRasterizerState const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    idd->state.raster = cmd->state;

    if (idd->state.raster.depthClampEnable)
    {
        gl->Enable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
    }

    if (idd->state.raster.rasterizerDiscardEnable)
    {
        gl->Enable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
    }

    gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlPolygonMode(idd->state.raster.fillMode)); _SglThrowErrorOccuried();

    if (idd->state.raster.cullMode)
    {
        gl->Enable(GL_CULL_FACE); _SglThrowErrorOccuried();
        gl->CullFace(AfxToGlCullMode(idd->state.raster.cullMode)); _SglThrowErrorOccuried();
        gl->FrontFace(AfxToGlFrontFace(idd->state.raster.frontFace)); _SglThrowErrorOccuried();
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

    if (idd->state.raster.depthBiasEnable)
    {
        gl->Enable(QwadroToGlPolygonModeBasedOffset[idd->state.raster.fillMode]); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(QwadroToGlPolygonModeBasedOffset[idd->state.raster.fillMode]); _SglThrowErrorOccuried();
    }

    gl->PolygonOffset(idd->state.raster.depthBiasSlopeFactor, idd->state.raster.depthBiasConstantFactor); _SglThrowErrorOccuried();

    gl->LineWidth(idd->state.raster.lineWidth); _SglThrowErrorOccuried();
}

_SGL void _SglDqueSetColorBlendState(afxDrawQueue dque, afxPipelineColorBlendState const *state, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    AfxAssert(state);
    idd->state.colorBlend = *state;

    AfxAssert(NIL);
}

_SGL void _SglDqueSetMultisampleState(afxDrawQueue dque, afxPipelineMultisampleState const *state, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    AfxAssert(state);
    idd->state.multisample = *state;

    if (idd->state.multisample.sampleShadingEnable)
    {
        gl->Enable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
    }

    if (idd->state.multisample.pSampleMask)
    {
        gl->Enable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
    }

    gl->SampleMaski(0, idd->state.multisample.pSampleMask[0]); _SglThrowErrorOccuried();

    if (idd->state.multisample.minSampleShading)
    {
        gl->Enable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
    }

    if (gl->MinSampleShading) // gl v4.1
    {
        gl->MinSampleShading(idd->state.multisample.minSampleShading); _SglThrowErrorOccuried();
    }

    if (idd->state.multisample.alphaToOneEnable)
    {
        gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
    }

    if (idd->state.multisample.alphaToCoverageEnable)
    {
        gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetDepthState(afxDrawQueue dque, _afxDscrCmdSetDepthState const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    idd->state.depth = cmd->state;

    if (idd->state.depth.depthTestEnable)
    {
        gl->Enable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        gl->DepthFunc(SglToGlCompareOp(idd->state.depth.depthCompareOp)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
    }

    if (idd->state.depth.depthWriteEnable)
    {
        gl->DepthMask(GL_TRUE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DepthMask(GL_FALSE); _SglThrowErrorOccuried();
    }

    if (idd->state.depth.stencilTestEnable)
    {
        gl->Enable(GL_STENCIL_TEST); _SglThrowErrorOccuried();

        gl->StencilFuncSeparate(GL_FRONT, SglToGlCompareOp(idd->state.depth.stencilOpFront.compareOp), 0, 1); _SglThrowErrorOccuried();
        gl->StencilOpSeparate(GL_FRONT, AfxToGlStencilOp(idd->state.depth.stencilOpFront.failOp), AfxToGlStencilOp(idd->state.depth.stencilOpFront.depthFailOp), AfxToGlStencilOp(idd->state.depth.stencilOpFront.passOp)); _SglThrowErrorOccuried();
        gl->StencilFuncSeparate(GL_BACK, SglToGlCompareOp(idd->state.depth.stencilOpBack.compareOp), 0, 1); _SglThrowErrorOccuried();
        gl->StencilOpSeparate(GL_BACK, AfxToGlStencilOp(idd->state.depth.stencilOpBack.failOp), AfxToGlStencilOp(idd->state.depth.stencilOpBack.depthFailOp), AfxToGlStencilOp(idd->state.depth.stencilOpBack.passOp)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetInputAssemblyState(afxDrawQueue dque, _afxDscrCmdSetInputAssemblyState const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    idd->state.ia = cmd->state;

    if (idd->state.ia.primRestartEnable)
    {
        gl->Enable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetInputStreams(afxDrawQueue dque, afxNat cnt, afxPipelineInputStream const streams[], glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    AfxAssert(cnt);
    AfxAssert(_SGL_MAX_INSTREAM_PER_SET >= cnt);

    idd->activeVaoIdx = (idd->activeVaoIdx + 1) % _SGL_MAX_VAO_PER_TIME;

#if !0
    if (idd->vao[idd->activeVaoIdx])
    {
        gl->DeleteVertexArrays(1, &(idd->vao[idd->activeVaoIdx])); _SglThrowErrorOccuried();
    }
    gl->GenVertexArrays(1, &(idd->vao[idd->activeVaoIdx])); _SglThrowErrorOccuried();
    gl->BindVertexArray(idd->vao[idd->activeVaoIdx]); _SglThrowErrorOccuried();
#endif

    for (afxNat i = 0; i < cnt; i++)
    {
        idd->state.inStreams[i] = streams[i];
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
        //gl->VertexAttribDivisor(streams[i].location, 0); _SglThrowErrorOccuried();
        //gl->VertexAttribPointer(streams[i].location, glsiz, gltype, FALSE, glStride, streams[i].offset);
    }
    //gl->BindVertexArray(0);
}

// RESOURCE BINDING

_SGL void _SglDqueBindPipeline(afxDrawQueue dque, _afxDscrCmdBindPip const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;

    idd->state.pip = cmd->pip;

    //glVmt const* gl = &thdIdd->wglVmt;
#if 0
    afxNat inStreamCnt = 0;
    afxPipelineInputStream inStreams[8];

    for (afxNat i = 0; i < AfxPipelineGetShaderCount(idd->state.pip); i++)
    {
        afxShader shd;
        AfxPipelineGetShaders(idd->state.pip, i, 1, &shd);

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
    _SglDqueSetInputStreams(dque, idd->state.pip->inCnt , idd->state.pip->ins, gl);

    if (idd->state.pip->hasVtxInAssembling)
    {
        _afxDscrCmdSetInputAssemblyState cmd;
        cmd.state = idd->state.pip->vtxInAssembling;
        _SglDqueSetInputAssemblyState(dque, &cmd, gl);
    }

    if (idd->state.pip->hasRasterization)
    {
        _afxDscrCmdSetRasterizerState cmd;
        cmd.state = idd->state.pip->rasterization;
        _SglDqueSetRasterizerState(dque, &cmd, gl);
    }

    if (idd->state.pip->hasMultisampling)
    {
        _SglDqueSetMultisampleState(dque, &idd->state.pip->multisampling, gl);
    }

    if (idd->state.pip->hasDepthHandling)
    {
        _afxDscrCmdSetDepthState cmd;
        cmd.state = idd->state.pip->depthHandling;
        _SglDqueSetDepthState(dque, &cmd, gl);
    }

    if (idd->state.pip->hasColorBlending)
    {
        _SglDqueSetColorBlendState(dque, &idd->state.pip->colorBlending, gl);
    }

    if (idd->state.pip->vpCnt)
    {
        _afxDscrCmdSetVp cmd;
        cmd.first = 0;
        cmd.cnt = idd->state.pip->vpCnt;

        for (afxNat i = 0; i < cmd.cnt; i++)
            cmd.vp[i] = idd->state.pip->vps[i];

        _SglDqueSetViewports(dque, &cmd, gl);
    }

    if (idd->state.pip->scissorCnt)
    {
        _afxDscrCmdSetScissor cmd;
        cmd.first = 0;
        cmd.cnt = idd->state.pip->scissorCnt;

        for (afxNat i = 0; i < cmd.cnt; i++)
            cmd.rect[i] = idd->state.pip->scissors[i];

        _SglDqueSetScissors(dque, &cmd, gl);
    }

    _SglDqueBindAndSyncPip(dque, idd->state.pip, gl);
}

_SGL void _SglDqueBindTextures(afxDrawQueue dque, _afxDscrCmdBindTextures const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > first);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= first + cmd->cnt);

    //glVmt const *gl = &thdIdd->wglVmt;

    AfxAssert(idd->state.pip);
    afxNat setCnt = idd->state.pip->wiringCnt;
    AfxAssert(cmd->set < setCnt);

    afxNat bindingCnt = 0;

    for (afxNat i = 0; i < setCnt; i++)
        if (idd->state.pip->wiring[i].set == cmd->set)
            bindingCnt = idd->state.pip->wiring[i].legt->entryCnt;

    AfxAssert(bindingCnt);
    AfxAssert(cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= _SGL_MAX_ENTRY_PER_LEGO);

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxNat binding = (cmd->set * _SGL_MAX_ENTRY_PER_LEGO) + first + i;

        if ((idd->state.boundRes[cmd->set][first + i].tex = cmd->tex[i]))
        {
            AfxAssertObject(cmd->tex[i], AFX_FCC_TEX);
            
            if (_SglDqueBindAndSyncTex(dque, binding, cmd->tex[i], gl))
                AfxThrowError();
        }

        if ((idd->state.boundRes[cmd->set][first + i].smp = cmd->smp[i]))
        {
            AfxAssertObject(cmd->smp[i], AFX_FCC_SAMP);
            
            if (_SglDqueBindAndSyncSmp(dque, binding, cmd->smp[i], gl))
                AfxThrowError();
        }
    }
}

_SGL void _SglDqueBindBuffers(afxDrawQueue dque, _afxDscrCmdBindBuffers const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > first);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= first + cmd->cnt);

    //glVmt const *gl = &thdIdd->wglVmt;
#if 0
    AfxAssert(idd->state.pip);
    afxPipelineRig pipr = AfxPipelineGetRig(idd->state.pip);
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
        
        idd->state.boundRes[cmd->set][first + i].buf = cmd->buf[i];
        
        {
            AfxAssertObject(cmd->buf[i], AFX_FCC_BUF);
            
            if (_SglDqueBindAndSyncBuf(dque, binding, cmd->buf[i], cmd->offset[i], cmd->range[i], GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, gl))
                AfxThrowError();
        }

        idd->state.boundRes[cmd->set][first + i].offset = cmd->offset[i];
        idd->state.boundRes[cmd->set][first + i].range = cmd->range[i];
    }
}

_SGL void _SglDqueBindIndexBuffer(afxDrawQueue dque, _afxDscrCmdBindIbuf const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;

    idd->state.indexBinding.buf = cmd->buf;
    idd->state.indexBinding.offset = cmd->offset;
    idd->state.indexBinding.idxSiz = cmd->idxSiz;

    // WARNING: element array objects binding doesn't allow offset or ranges at bind time.

    if (_SglDqueBindAndSyncBuf(dque, 0, idd->state.indexBinding.buf, 0, 0, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, gl))
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

_SGL void _SglDqueBindVertexBuffers(afxDrawQueue dque, _afxDscrCmdBindVbuf const *cmd, glVmt const* gl)
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
    sglDqueIdd *idd = dque->idd;
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_VBO_PER_BIND > first);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= first + cmd->cnt);

    //glVmt const *gl = &thdIdd->wglVmt;

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        idd->state.vertexBindings[first + i].buf = cmd->buf[i];
        idd->state.vertexBindings[first + i].offset = cmd->offset[i];
        //idd->state.vertexBindings[first + i].siz = cmd->vtxArrSiz[i];
        idd->state.vertexBindings[first + i].stride = cmd->vtxStride[i];

        //afxNat32 stride = cmd->vtxStride[i] ? cmd->vtxStride[i] : idd->state.inStreams[i].offset;
        //AfxAssert(stride);
        afxNat32 stride = idd->state.vertexBindings[first + i].stride;
        
        if (_SglDqueBindAndSyncBuf(dque, first + i, idd->state.vertexBindings[first + i].buf, idd->state.vertexBindings[first + i].offset, stride, GL_ARRAY_BUFFER, GL_STATIC_DRAW, gl))
            AfxThrowError();
    }
    idd->state.vertexBindingCnt = cmd->cnt;

    //_SglDquePushNextVao(dque);
}

#if 0
_SGL void _SglDquePushNextVao(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;

    _sglDthrIdd*thdIdd = dque->activeExecUnit;
    glVmt const* gl = &thdIdd->wglVmt;

    {
        if (_SGL_MAX_VAO_PER_TIME > idd->activeVaoIdx)
            ++idd->activeVaoIdx;
        else
            idd->activeVaoIdx = 0;

        if (idd->vao[idd->activeVaoIdx])
        {
            gl->DeleteVertexArrays(1, &(idd->vao[idd->activeVaoIdx])); _SglThrowErrorOccuried();
        }
        gl->GenVertexArrays(1, &(idd->vao[idd->activeVaoIdx])); _SglThrowErrorOccuried();
        gl->BindVertexArray(idd->vao[idd->activeVaoIdx]); _SglThrowErrorOccuried();
        AfxAssert(gl->IsVertexArray(idd->vao[idd->activeVaoIdx]));

        afxPipeline pip = idd->state.pip;
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

                if (FALSE == idd->state.vertexBindings[binding].wild)
                {
                    afxVertexBuffer vbuf = idd->state.vertexBindings[binding].vbuf;
                    afxNat baseVtx = idd->state.vertexBindings[binding].baseVtx;
                    afxNat vtxArr = idd->state.vertexBindings[binding].arr;
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
                    afxBuffer buf = idd->state.vertexBindings[binding].buf;
                    offset = idd->state.vertexBindings[binding].offset;
                    stride = idd->state.vertexBindings[binding].stride;

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

_SGL void _SglDqueDraw(afxDrawQueue dque, _afxDscrCmdDraw const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *idd = dque->idd;
    
    //_SglDqueUpdateNextStateObjects(dctx, queueIdx);
    //_SglDqueFlushNextState(dctx, queueIdx);

    AfxAssert(idd->state.pip);
    //AfxAssert(idd->state.vertexBindingCnt);

    AfxAssert(cmd->vtxCnt);
    AfxAssert(cmd->instCnt);
    GLenum top = AfxToGlTopology(idd->state.ia.topology);

    if (cmd->instCnt > 1)
    {
        gl->DrawArraysInstanced(top, cmd->firstVtx, cmd->vtxCnt, cmd->instCnt); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawArrays(top, cmd->firstVtx, cmd->vtxCnt); _SglThrowErrorOccuried();
    }
    idd->numOfFedVertices += cmd->vtxCnt;
    idd->numOfFedInstances += cmd->instCnt;

    //AfxEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _SglDqueDrawIndexed(afxDrawQueue dque, _afxDscrCmdDrawIndexed const* cmd, glVmt const* gl)
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
    sglDqueIdd *idd = dque->idd;
    
    //_SglDqueUpdateNextStateObjects(dctx, queueIdx);
    //_SglDqueFlushNextState(dctx, queueIdx);

    AfxAssert(idd->state.pip);
    AfxAssertObject(idd->state.indexBinding.buf, AFX_FCC_BUF);
    AfxAssert(idd->state.vertexBindingCnt);

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

    afxSize idxBaseOff = idd->state.indexBinding.offset;
    afxSize idxSiz = idd->state.indexBinding.idxSiz;

    GLint vtxOff2 = cmd->vtxOff;

    GLenum top = AfxToGlTopology(idd->state.ia.topology);

    if (cmd->instCnt > 1)
    {
        gl->DrawElementsInstancedBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), cmd->instCnt, vtxOff2); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawElementsBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff + (idxSiz * cmd->firstIdx)), vtxOff2); _SglThrowErrorOccuried();
    }

    idd->numOfFedIndices += cmd->idxCnt;
    idd->numOfFedInstances += cmd->instCnt;
    //AfxEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);

}

void (*_SglDqueCmdVmt[AFX_DCMD_TOTAL])(afxDrawQueue dque, _afxDscrCmd const *cmd, glVmt const* gl) =
{
    NIL,

    (void*)_SglDqueBeginDrawPass,
    (void*)_SglDqueNextPass,
    (void*)_SglDqueEndDrawPass,

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

_SGL afxError _SglDqueExecuteDscr(afxDrawQueue dque, afxDrawScript dscr, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    //glVmt const* gl = &thdIdd->wglVmt;
    sglDscrIdd *dscrIdd = dscr->idd;

    AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);

    _afxDscrCmd *cmdHdr;
    AfxChainForEveryLinkageB2F(&dscrIdd->commands, _afxDscrCmd, script, cmdHdr)
    {
        if (cmdHdr->id == AFX_DCMD_END)
            break;

        if (dscr->state != AFX_DSCR_STATE_PENDING)
        {
            AfxThrowError();
            break;
        }

        _SglDqueCmdVmt[cmdHdr->id](dque, cmdHdr, gl);
    }

    return err;
}

_SGL afxError _SglDquePresentSurf(afxDrawQueue dque, afxDrawOutput dout, afxNat outBufIdx, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObject(dout, AFX_FCC_DOUT);
    sglDqueIdd *idd = dque->idd;
    afxDrawContext dctx = dque->owner;
    sglDctxIdd*dctxIdd = dctx->idd;
    

    AfxAssert(!dout->swapping);

    {
        dout->swapping = TRUE;
        sglDoutIdd* doutIdd = dout->idd;

        afxDrawDriver ddrv = AfxGetDrawQueueDriver(dque);
        AfxAssertObject(ddrv, AFX_FCC_DDRV);
        _sglDdrvIdd *ddrvIdd = ddrv->idd;
        afxNat unitIdx = dque->portIdx;
        sglDpuIdd *dpu = &ddrvIdd->dpus[unitIdx];
        AfxAssert(unitIdx == 0);
        wglVmt const*wgl = &dpu->wgl;

        afxNat32 procUnitIdx;
        AfxGetThreadingUnit(&procUnitIdx);
        AfxAssert(unitIdx == procUnitIdx);

        HDC dc = wgl->GetCurrentDC();
        HGLRC rc = wgl->GetCurrentContext();

        if (rc != dpu->glrc || dc != doutIdd->dc)
        {
            if (!wgl->MakeCurrent(doutIdd->dc, dpu->glrc))
            {
                AfxThrowError();
                AfxError("DPU %u failed to set to draw output device %p.", unitIdx, dout);
                wgl->MakeCurrent(dpu->dc, dpu->glrc);
            }
        }
        afxSurface surf = dout->buffers[outBufIdx].surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        //AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
#if 0
        if (!idd->presentFboGpuHandle)
        {
            gl->GenFramebuffers(1, &(idd->presentFboGpuHandle)); _SglThrowErrorOccuried();
        }
        gl->BindFramebuffer(GL_FRAMEBUFFER, idd->presentFboGpuHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(idd->presentFboGpuHandle));
        gl->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, surf->tex.glTarget, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();

        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->BlitFramebuffer(0, 0, surf->tex.whd[0], surf->tex.whd[1], 0, 0, surf->tex.whd[0], surf->tex.whd[1], GL_COLOR_BUFFER_BIT, GL_NEAREST); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Flush();
        //gl->Finish();
        //gl->DeleteFramebuffers(1, &(gpuHandle)); _SglThrowErrorOccuried();
#else
        //if (dout->presentMode == AFX_PRESENT_MODE_FIFO)
            //surf = AfxContainerOf(AfxGetFirstLinkage(&dout->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else //if (dout->presentMode == AFX_PRESENT_MODE_LIFO)
            //surf = AfxContainerOf(AfxGetLastLinkage(&dout->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else AfxError("Not implemented yet.");

        //_SglDqueBindAndSyncPip(idd->presentPip, dque);
        //_SglDqueBindAndSyncBuf(&(idd->presentVbuf->buf), 0, 0, 0, GL_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
        //_SglDqueBindAndSyncLego(idd->presentLego, 0, dque);

#if !0
        
        //_AfxStdCanvUpdate(cmd->beginRenderPass.canv, dque);

        afxWhd extent;
        AfxGetTextureExtent(&surf->tex, 0, extent);
        
        _afxDscrCmdBeginDrawPass cmdBeginOp = { 0 };
        cmdBeginOp.canv = NIL;
        cmdBeginOp.area = (afxRect const){ { { 0, 0 } }, { { extent[0], extent[1] } } };
        cmdBeginOp.rasterCnt = 1;
        cmdBeginOp.rasters[0] = (afxDrawTarget const){ NIL, NIL, AFX_DTS_LOAD_OP_DONT_CARE, AFX_DTS_STORE_OP_STORE, { .color = { 0.3, 0.0, 0.3, 1 } } };

        _SglDqueBeginDrawPass(dque, &cmdBeginOp, gl);

        _afxDscrCmdBindPip cmdBindPip = { 0 };
        cmdBindPip.pip = dctxIdd->presentPip;
        _SglDqueBindPipeline(dque, &cmdBindPip, gl);

#else
        afxDrawTarget const rasterRt = { NIL, AFX_DTS_LOAD_OP_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _SglDqueBeginCombination(dque, NIL, 1, 0, &rasterRt, NIL, NIL);

        afxPipeline pip = AfxDrawOperationGetPipeline(idd->presentDop, 0, 0);
        AfxAssertObject(pip, AFX_FCC_PIP);
        _SglDqueBindPipeline(dque, pip);
#endif   
        //_SglDqueEmployTechnique(dque, 0);

        _afxDscrCmdSetVp cmdSetVp = { 0 };
        cmdSetVp.cnt = 1;
        cmdSetVp.vp[0].extent[0] = extent[0];
        cmdSetVp.vp[0].extent[1] = extent[1];
        cmdSetVp.vp[0].depth[1] = 1;

        _SglDqueSetViewports(dque, &cmdSetVp, gl);

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
        afxVertexBuffer vbufs[] = { idd->rgbRectVbo, idd->rgbRectVbo };
        _SglDqueBindVertexBuffers(dque, 0, 2, vbufs, baseVtxs, vtxArrs);
#endif

        _afxDscrCmdBindVbuf cmdBindVbuf = { 0 };
        cmdBindVbuf.cnt = 1;
        cmdBindVbuf.buf[0] = &dctxIdd->presentVbuf->buf;
        //cmdBindVbuf.vtxArrSiz[0] = 0;
        cmdBindVbuf.vtxStride[0] = AfxVertexBufferGetStride(dctxIdd->presentVbuf, 0);
    
        _SglDqueBindVertexBuffers(dque, &cmdBindVbuf, gl);

        //_SglDqueBindLegos(dque, 0, 1, &idd->presentLego);
        _afxDscrCmdBindTextures cmdBindTex = { 0 };
        cmdBindTex.cnt = 1;
        cmdBindTex.smp[0] = dctxIdd->presentSmp;
        cmdBindTex.tex[0] = &surf->tex;

        _SglDqueBindTextures(dque, &cmdBindTex, gl);

        _afxDscrCmdDraw cmdDraw = { 0 };
        cmdDraw.vtxCnt = 4;
        cmdDraw.instCnt = 1;
        _SglDqueDraw(dque, &cmdDraw, gl);

#if !0
        _SglDqueEndDrawPass(dque, NIL, gl);
#else
        _SglDqueEndCombination(dque, NIL);
#endif
#endif
        //AfxPopLinkage(&surf->swapchain);
        
        if (surf->state == AFX_SURF_STATE_PENDING)
        {
            surf->state = AFX_SURF_STATE_IDLE;
            //((sglTexIdd*)surf->tex.idd)->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
        // deadlocks all
        SglSwapBuffers(doutIdd->dc, wgl);
        
        afxReal64 currTime, deltaTime;
        AfxGetExecutionTime(&currTime, &deltaTime);
        afxNat currIter, lastFreq;
        AfxGetExecutionCounter(&currIter,&lastFreq);

        //if (0 == AfxGetProcessorIterationCount())
        {
            AfxFormatString(dout->caption, "Delta %0f, IPS %u/%u --- OpenGL/Vulkan Continuous Integration over Qwadro (c) 2017 SIGMA --- Public Test Build", deltaTime, lastFreq, currIter);
            SetWindowTextA(doutIdd->wnd, AfxGetStringDataConst(dout->caption, 0));
        }
        dout->swapping = FALSE;
        (void)gl;
    }
    return err;
}

_SGL void* _AfxDqueRequestArenaSpace(afxDrawQueue dque, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxNat unitIdx = dque->portIdx;

    AfxEnterSlockExclusive(&dque->arenaSlock);

    void *block = AfxRequestArenaSpace(&dque->cmdArena, siz);

    if (!block)
        AfxThrowError();

    AfxExitSlockExclusive(&dque->arenaSlock);

    return block;
}

_SGL void _AfxDqueRecycleArenaSpace(afxDrawQueue dque, void *block, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxNat unitIdx = dque->portIdx;

    AfxEnterSlockExclusive(&dque->arenaSlock);

    AfxAssert(block);

    AfxRecycleArenaSpace(&dque->cmdArena, block, siz);

    AfxExitSlockExclusive(&dque->arenaSlock);
}

_SGL afxError _SglDqueVmtProcCb(afxDrawQueue dque, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    sglDqueIdd *idd = dque->idd;

    afxDrawContext dctx = dthr->dctx;
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = dthr->ddrv;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *ddrvIdd = ddrv->idd;
    afxNat unitIdx = dthr->portIdx;
    sglDpuIdd *dpu = &ddrvIdd->dpus[unitIdx];
    wglVmt const*wgl = &dpu->wgl;
    
    AfxAssert(unitIdx == dque->portIdx);
    AfxAssert(dctx == dque->owner);
    
    HGLRC rc = wgl->GetCurrentContext();

    if (rc != dpu->glrc)
    {
        if (!wgl->MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxError("DPU %u failed to be set.", unitIdx);
            wgl->MakeCurrent(dpu->dc, dpu->glrc);
        }
    }

    glVmt const *gl = &dpu->gl;

    idd->numOfFedVertices = 0;
    idd->numOfFedIndices = 0;
    idd->numOfFedInstances = 0;

    if (AfxTryEnterSlockExclusive(&dque->pendingChainSlock))
    {
        _afxDqueSubm* subm;
        AfxChainForEveryLinkageB2F(&dque->pendingChain, _afxDqueSubm, chain, subm)
        {
            AfxGetTime(&subm->pullTime);

            //if (subm->isScript)
            {
                for (afxNat i = 0; i < subm->scriptCnt; i++)
                {
                    afxDrawScript dscr = subm->scripts[i];
                    AfxAssertObject(dscr, AFX_FCC_DSCR);
                    AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);

                    if (dscr->state == AFX_DSCR_STATE_PENDING)
                    {
                        if (_SglDqueExecuteDscr(dque, dscr, gl))
                        {
                            AfxThrowError();
                        }
                        else
                        {
                            dscr->state = AFX_DSCR_STATE_EXECUTABLE;
                        }

                        if (err || dscr->disposable)
                        {
                            AfxAssert(dscr->portIdx == unitIdx);
                            dscr->state = AFX_DSCR_STATE_INVALID;
                        }
                    }
                }
            }
            //else
            {
                for (afxNat i = 0; i < subm->outputCnt; i++)
                {
                    afxDrawOutput dout = subm->outputs[i];
                    AfxAssertObject(dout, AFX_FCC_DOUT);

                    afxSurface surf;
                    AfxGetDrawOutputBuffer(dout, subm->outBufIdx[i], &surf);
                    AfxAssertObject(surf, AFX_FCC_SURF);
                    //AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
                    
                    //if (surf->state == AFX_SURF_STATE_PENDING)
                        _SglDquePresentSurf(dque, dout, subm->outBufIdx[i], gl);
                }
            }

            AfxGetTime(&subm->complTime);

            AfxPopLinkage(&subm->chain);

            //AfxEnterSlockExclusive(&dctx->ports[dque->portIdx].recyclSubmChainSlock);

            //if (dctx->ports[dque->portIdx].minRecyclSubmCnt > AfxGetChainLength(&dctx->ports[dque->portIdx].recyclSubmChain))
            {
                //AfxPushLinkage(&subm->chain, &dctx->ports[dque->portIdx].recyclSubmChain);
            }
            //else
            {
                _AfxDqueRecycleArenaSpace(dque, subm, sizeof(*subm));
            }
            //AfxExitSlockExclusive(&dctx->ports[dque->portIdx].recyclSubmChainSlock);
        }

        AfxExitSlockExclusive(&dque->pendingChainSlock);
    }
    return err;
}

_SGL afxError _SglDqueVmtSubmitCb(afxDrawQueue dque, afxDrawSubmissionSpecification const *spec, afxNat *submNo) // called by din to enqueue submission
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    _afxDqueSubm* subm = _AfxDqueRequestArenaSpace(dque, sizeof(*subm));

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
        subm->scripts[i]->state = AFX_DSCR_STATE_PENDING;
    }

    subm->outputCnt = AfxMin(spec->outputCnt, 4);
    
    for (afxNat i = 0; i < subm->outputCnt; i++)
    {
        subm->outputs[i] = spec->outputs[i];
        subm->outBufIdx[i] = spec->outBufIdx[i];
        //subm->outputs[i]->buffers[subm->outBufIdx[i]]->state = AFX_SURF_STATE_PENDING;
    }
    subm->data[0] = spec->data;


    AfxEnterSlockExclusive(&dque->pendingChainSlock);

    AfxPushLinkage(&subm->chain, &dque->pendingChain);

    AfxExitSlockExclusive(&dque->pendingChainSlock);

    return err;
}

_SGL afxError _SglDqueVmtWaitCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    return err;
}

_SGL afxError _SglDqueVmtDtorCb(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxMemory mem = AfxGetDrawMemory();

    sglDqueIdd *idd = dque->idd;

    idd->submissionSuspended = TRUE;
#if 0
    AfxReleaseObject(&(idd->rgbRectVbo->buf.obj));
    AfxReleaseObject(&(idd->rgbRectPip->res.obj));
    AfxReleaseObject(&(idd->rgbRectLego->obj));
    AfxReleaseObject(&(idd->rgbRectSmp->obj));
#endif

    AfxDeallocate(mem, idd);
    dque->idd = NIL;

    return err;
}

_SGL _afxDqueVmt _SglDqueVmt =
{
    _SglDqueVmtDtorCb,
    _SglDqueVmtProcCb,
    _SglDqueVmtWaitCb,
    _SglDqueVmtSubmitCb
};

_SGL afxError _SglDdrvVmtDqueCb(afxDrawQueue dque, afxDrawQueueSpecification const *spec)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxMemory mem = AfxGetDrawMemory();

    afxDrawDriver ddrv = AfxGetDrawQueueDriver(dque);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    dque->vmt = &_SglDqueVmt;
    
    sglDqueIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());
    dque->idd = idd;

    idd->numOfFedIndices = 0;
    idd->numOfFedVertices = 0;
    idd->numOfFedInstances = 0;

    idd->deltaTime = 0.f;
    idd->frameTime = 0;
    idd->swapTime = 0;
    idd->frameNum = 0;
    idd->fps = 0;

    idd->submissionSuspended = FALSE;
    
    idd->activeTmpFboIdx = 0;
    idd->activeVaoIdx = 0;
    
    AfxZero(idd->vao, sizeof(idd->vao));
    AfxZero(idd->tmpFbo, sizeof(idd->tmpFbo));
    
    AFX_ZERO(&idd->state);

    idd->instanced = TRUE;

    idd->running = TRUE;

    return err;
}
