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

#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#include "sgl.h"

#include "afx/core/afxSystem.h"
#include "../e2coree/draw/_classified/afxDrawClassified.h"

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

// RENDERING SCOPE

_SGL void _SglDpuEndCanvas(sglDpuIdd* dpu, _afxDscrCmd const *cmd, glVmt const* gl)
{
    (void)cmd;
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;

    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();

    //_SglDpuBindAndSyncCanv(dpu, NIL, GL_DRAW_FRAMEBUFFER, gl);
    gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();


    //if (!dpu->state.renderPass.canv && dpu->state.renderPass.rasters[0].tex)
    {
        //AfxAssert(dpu->tmpFbo[dpu->activeTmpFboIdx]);
    }

    for (afxNat i = 0; i < dpu->state.renderPass.rasterCnt; i++)
    {
        afxDrawTarget const *target = &dpu->state.renderPass.rasters[i];
        afxTexture tex = target->tex;

        if (tex)
        {
            AfxAssertObjects(1, &tex, AFX_FCC_TEX);
            AfxAssert(AfxTestTexture(tex, AFX_TEX_USAGE_DRAW));
            tex->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;

            // AFX_DTS_STORE_OP_STORE
#if 0
            gl->BindTexture(tex->glTarget, tex->glHandle);
            gl->GetTexImage(tex->glTarget, 0, tex->glFmt, tex->glType, tex->base.maps);
            gl->BindTexture(tex->glTarget, 0);
#endif
        }
    }

    if (dpu->state.renderPass.depthRt.tex)
    {
        afxTexture tex = dpu->state.renderPass.depthRt.tex;

        if (tex)
        {
            AfxAssertObjects(1, &tex, AFX_FCC_TEX);
            //afxTexture tex = AfxGetSurfaceTexture(surf);
            AfxAssert(AfxTestTexture(tex, AFX_TEX_USAGE_DRAW));
            //AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            //surf->state = AFX_SURF_STATE_PRESENTABLE;
            tex->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }

    if (dpu->state.renderPass.stencilRt.tex)
    {
        afxTexture tex = dpu->state.renderPass.stencilRt.tex;

        if (tex)
        {
            AfxAssertObjects(1, &tex, AFX_FCC_TEX);
            //afxTexture tex = AfxGetSurfaceTexture(surf);
            AfxAssert(AfxTestTexture(tex, AFX_TEX_USAGE_DRAW));
            //AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            //surf->state = AFX_SURF_STATE_PRESENTABLE;
            tex->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
    }
};

#if 0
_SGL void _SglDpuBeginDrawPassRasterTargets(sglDpuIdd* dpu, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;

    afxCanvas canv = dpu->state.renderPass.canv;
    afxNat rasterCnt = dpu->state.renderPass.rasterCnt;
    afxNat colorAttchEnabledCnt = 0;
    GLenum drawBuffersIndices[_SGL_MAX_SURF_PER_CANV];    
    
    for (afxNat i = 0; i < rasterCnt; i++)
    {
        afxDrawTarget const *target = &dpu->state.renderPass.rasters[i];
        afxTexture tex;

        if ((tex = target->tex))
        {
            AfxAssertObjects(1, &tex, AFX_FCC_TEX);
            //afxTexture tex = AfxGetSurfaceTexture(surf);
            AfxAssert(AfxTestTexture(tex, AFX_TEX_USAGE_DRAW));
            //AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            //AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            //surf->state = AFX_SURF_STATE_PAINTING;

            if (!canv) // will draw directly to surface (with no canvas)?
            {
                _SglDpuBindAndSyncTex(dpu, 0, tex, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
                
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, tex->glHandleAux); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->glHandle, 0); _SglThrowErrorOccuried();
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

            if (!canv && !dpu->state.renderPass.rasters[0].tex) // will draw directly to default fb?
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

_SGL void _SglDpuBeginDrawPassDepthTargets(sglDpuIdd* dpu, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;

    afxCanvas canv = dpu->state.renderPass.canv;
    afxBool combinedDs = FALSE;

    afxDrawTarget const *targetD = &dpu->state.renderPass.depthRt;
    afxDrawTarget const *targetS = &dpu->state.renderPass.stencilRt;
    afxTexture surf = targetD->tex;
    afxTexture surfS = targetS->tex;
    combinedDs = surf == surfS && surf;

    if (surf)
    {
        AfxAssertObjects(1, &surf, AFX_FCC_TEX);
        //afxTexture tex = AfxGetSurfaceTexture(surf);
        AfxAssert(AfxTestTexture(surf, AFX_TEX_USAGE_DRAW));
        //AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
        //AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
        //surf->state = AFX_SURF_STATE_PAINTING;
    }

    if (!combinedDs && surfS)
    {
        AfxAssertObjects(1, &surfS, AFX_FCC_TEX);
        //afxTexture tex = AfxGetSurfaceTexture(surf);
        AfxAssert(AfxTestTexture(surfS, AFX_TEX_USAGE_DRAW));
        //AfxAssert(surfS->state != AFX_SURF_STATE_PAINTING);
        //AfxAssert(surfS->state != AFX_SURF_STATE_PENDING);
        //surfS->state = AFX_SURF_STATE_PAINTING;
    }

    if (!canv)
    {
        if (combinedDs)
        {
            if (surf)
            {
                _SglDpuBindAndSyncTex(dpu, 0, surf, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surf->glHandleAux); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, surf->glHandle, 0); _SglThrowErrorOccuried();
            }
        }
        else
        {
            if (surf)
            {
                _SglDpuBindAndSyncTex(dpu, 0, surf, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, surf->glHandleAux); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, surf->glHandle, 0); _SglThrowErrorOccuried();
            }

            if (surfS)
            {
                _SglDpuBindAndSyncTex(dpu, 0, surfS, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surfS->glHandleAux); _SglThrowErrorOccuried();
                gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, surfS->glHandle, 0); _SglThrowErrorOccuried();
            }
        }
    }

    if (combinedDs)
    {
        if (targetD->loadOp == AFX_DTS_LOAD_OP_CLEAR)
        {
            //gl->DrawBuffer(GL_COLOR_ATTACHMENT0 + i); _SglThrowErrorOccuried();
            //gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();

            if (!canv && !dpu->state.renderPass.depthRt.tex) // will draw directly to default fb?
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
            if (!canv && !dpu->state.renderPass.depthRt.tex) // will draw directly to default fb?
            {
                gl->ClearDepth(targetD->clearValue.depth); _SglThrowErrorOccuried();
                gl->Clear(GL_DEPTH_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else
            {
                gl->ClearBufferfv(GL_DEPTH, 0, &targetD->clearValue.depth); _SglThrowErrorOccuried();
            }
        }

        if (targetS->loadOp == AFX_DTS_LOAD_OP_CLEAR)
        {
            if (!canv && !dpu->state.renderPass.stencilRt.tex) // will draw directly to default fb?
            {
                gl->ClearStencil(targetS->clearValue.stencil); _SglThrowErrorOccuried();
                gl->Clear(GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
            }
            else
            {
                GLint sCv = targetS->clearValue.stencil;
                gl->ClearBufferiv(GL_STENCIL, 0, &sCv); _SglThrowErrorOccuried();
            }
        }
    }

}
#endif

_SGL void _SglBindFboAttachment(GLenum target, GLenum glAttachment, GLenum texTarget, GLuint texHandle, afxNat level, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;

    switch (texTarget)
    {
    case GL_TEXTURE_CUBE_MAP:
    {
        gl->FramebufferTexture(target, glAttachment, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->FramebufferTexture3D(target, glAttachment, texTarget, texHandle, level, 0); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->FramebufferTexture3D(target, glAttachment, texTarget, texHandle, level, 0); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D:
    {
        gl->FramebufferTexture2D(target, glAttachment, texTarget, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->FramebufferTexture2D(target, glAttachment, texTarget, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->FramebufferTexture1D(target, glAttachment, texTarget, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
    default:
        gl->FramebufferTexture(target, glAttachment, texHandle, level); _SglThrowErrorOccuried();
        break;
    };
}

_SGL void _SglDpuBeginCanvas(sglDpuIdd* dpu, _afxDscrCmdBeginCanvas /*const*/ *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;

    _SglThrowErrorOccuried();
    gl->Flush(); _SglThrowErrorOccuried();

    //sglDqueIdd *dpu = dque->dpu;
    // renderArea.width = min(renderArea.width, frameBuffer.width - renderArea.x);

    // renderArea.height = min(renderArea.height, frameBuffer.height - renderArea.y);

    //afxCanvas canv = dpu->state.renderPass.canv = cmd->canv;
    dpu->state.renderPass.activeSubpass = 0;
    dpu->state.renderPass.area = cmd->area;
    dpu->state.renderPass.layerCnt = cmd->layerCnt;
    afxNat rasterCnt = (dpu->state.renderPass.rasterCnt = cmd->rasterCnt);
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= rasterCnt);

    for (afxNat i = 0; i < cmd->rasterCnt; i++)
        dpu->state.renderPass.rasters[i] = cmd->rasters[i];

    dpu->state.renderPass.depthRt = cmd->depth;
    dpu->state.renderPass.stencilRt = cmd->stencil;

    afxDrawTarget const* rdt;
    afxDrawTarget const* ddt = &dpu->state.renderPass.depthRt;
    afxDrawTarget const* sdt = &dpu->state.renderPass.stencilRt;
    afxTexture tex, tex2;
    afxBool combinedDs;

    //_SglDpuBindAndSyncCanv
    //_SglDpuBeginDrawPassRasterTargets(dpu, gl);
    //_SglDpuBeginDrawPassDepthTargets(dpu, gl);

    if (!cmd->fboHandle) // instantiate it
    {
        GLuint fbo;
        afxBool buildFbo = !!rasterCnt && dpu->state.renderPass.rasters[0].tex;

        if (buildFbo)
        {
            gl->GenFramebuffers(1, &(fbo)); _SglThrowErrorOccuried();
            gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); _SglThrowErrorOccuried();
            AfxAssert(gl->IsFramebuffer(fbo));
            cmd->fboHandle = fbo;
        }

        // instantiate rasters

        afxNat colorAttchEnabledCnt = 0;
        GLenum drawBuffersIndices[_SGL_MAX_SURF_PER_CANV];

        for (afxNat i = 0; i < rasterCnt; i++)
        {
            rdt = &dpu->state.renderPass.rasters[i];

            if ((tex = rdt->tex))
            {
                AfxAssertObjects(1, &tex, AFX_FCC_TEX);
                AfxAssert(AfxTestTexture(tex, AFX_TEX_USAGE_DRAW));
                //AfxAssert(AfxPixelFormatIsRaster(AfxGetTextureFormat(tex)));

                _SglDpuBindAndSyncTex(dpu, 0, tex, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);

                AfxAssert(buildFbo);

                if (tex->glHandleAux)
                {
                    gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, tex->glHandleAux); _SglThrowErrorOccuried();
                }
                //gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->glHandle, 0); _SglThrowErrorOccuried();
                _SglBindFboAttachment(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->glTarget, tex->glHandle, 0, gl);
            }

            if (rdt->storeOp == AFX_DTS_STORE_OP_STORE)
            {
                drawBuffersIndices[colorAttchEnabledCnt] = GL_COLOR_ATTACHMENT0 + i;
                colorAttchEnabledCnt++;
            }
        }

        if (buildFbo)
        {
            _SglThrowErrorOccuried();
            gl->DrawBuffers(colorAttchEnabledCnt, drawBuffersIndices); _SglThrowErrorOccuried();

            if (err)
            {
                int a = 0;
            }
        }

        // instantiate depth/stencil

        combinedDs = ((tex = ddt->tex) == (tex2 = sdt->tex)) && tex;

        if (tex)
        {
            if (combinedDs)
            {
                _SglDpuBindAndSyncTex(dpu, 0, tex, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
                AfxAssert(AfxPixelFormatIsCombinedDepthStencil(AfxGetTextureFormat(tex)));

                AfxAssert(buildFbo);

                if (tex->glHandleAux)
                {
                    gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, tex->glHandleAux); _SglThrowErrorOccuried();
                }
                //gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, tex->glHandle, 0); _SglThrowErrorOccuried();
                _SglBindFboAttachment(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, tex->glTarget, tex->glHandle, 0, gl);
            }
            else
            {
                _SglDpuBindAndSyncTex(dpu, 0, tex, gl);
                _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
                AfxAssert(AfxPixelFormatIsDepth(AfxGetTextureFormat(tex)));

                AfxAssert(buildFbo);

                if (tex->glHandleAux)
                {
                    gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tex->glHandleAux); _SglThrowErrorOccuried();
                }
                //gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->glHandle, 0); _SglThrowErrorOccuried();
                _SglBindFboAttachment(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->glTarget, tex->glHandle, 0, gl);
            }
        }

        if ((!combinedDs) && tex2)
        {
            _SglDpuBindAndSyncTex(dpu, 0, tex2, gl);
            _SglDpuBindAndSyncTex(dpu, 0, NIL, gl);
            AfxAssert(AfxPixelFormatIsStencil(AfxGetTextureFormat(tex2)));

            AfxAssert(buildFbo);

            if (tex2->glHandleAux)
            {
                gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, tex2->glHandleAux); _SglThrowErrorOccuried();
            }
            //gl->FramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, tex2->glHandle, 0); _SglThrowErrorOccuried();
            _SglBindFboAttachment(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, tex2->glTarget, tex2->glHandle, 0, gl);
        }

        if (buildFbo)
        {
            switch (gl->CheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
            {
            case GL_FRAMEBUFFER_COMPLETE:
                //AfxEcho("afxCanvas %p hardware-side data instanced.", fbo);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                AfxError("Not all framebuffer attachment points are framebuffer attachment complete.");
                _SglThrowErrorOccuried();
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                AfxError("No images are attached to the framebuffer.");
                _SglThrowErrorOccuried();
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                AfxError("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
                _SglThrowErrorOccuried();
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                AfxError("Incomplete draw frame buffer.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                AfxError("Incomplete read frame buffer.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                AfxError("Incomplete multisample.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                AfxError("Incomplete layer targets.");
                break;
            case GL_FRAMEBUFFER_UNDEFINED:
                AfxError("Default framebuffer does not exist");
                break;
            default:
                AfxError("UNKNOWN");
                _SglThrowErrorOccuried();
                break;
            }

            if (GL_FRAMEBUFFER_COMPLETE != gl->CheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
                AfxThrowError();
        }
    }
    else
    {
        AfxAssert(gl->IsFramebuffer(cmd->fboHandle));
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, cmd->fboHandle); _SglThrowErrorOccuried();
    }

    // use

    {
        // use rasters

        for (afxNat i = 0; i < rasterCnt; i++)
        {
            rdt = &dpu->state.renderPass.rasters[i];
            tex = rdt->tex;

            if (rdt->loadOp == AFX_DTS_LOAD_OP_CLEAR)
            {
                afxReal const *color = rdt->clearValue.color;
                //gl->DrawBuffer(GL_COLOR_ATTACHMENT0 + i); _SglThrowErrorOccuried();
                //gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();

                if (!tex && i == 0)
                {
                    gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
                    gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->ClearBufferfv(GL_COLOR, i, color); _SglThrowErrorOccuried();
                }
            }
        }
        
        // use depth/stencil

        combinedDs = ((tex = ddt->tex) == (tex2 = sdt->tex)) && tex;
            
        if (combinedDs)
        {
            if (ddt->loadOp == AFX_DTS_LOAD_OP_CLEAR)
            {
                if (!tex)
                {
                    gl->ClearDepth(ddt->clearValue.depth); _SglThrowErrorOccuried();
                    gl->ClearStencil(sdt->clearValue.stencil); _SglThrowErrorOccuried();
                    gl->Clear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->ClearBufferfi(GL_DEPTH_STENCIL, 0, ddt->clearValue.depth, sdt->clearValue.stencil); _SglThrowErrorOccuried();
                }
            }
        }
        else
        {
            if (ddt->loadOp == AFX_DTS_LOAD_OP_CLEAR)
            {
                if (!tex)
                {
                    gl->ClearDepth(ddt->clearValue.depth); _SglThrowErrorOccuried();
                    gl->Clear(GL_DEPTH_BUFFER_BIT); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->ClearBufferfv(GL_DEPTH, 0, &ddt->clearValue.depth); _SglThrowErrorOccuried();
                }
            }

            if (sdt->loadOp == AFX_DTS_LOAD_OP_CLEAR)
            {
                if (!tex2)
                {
                    gl->ClearStencil(sdt->clearValue.stencil); _SglThrowErrorOccuried();
                    gl->Clear(GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
                }
                else
                {
                    GLint sCv = sdt->clearValue.stencil;
                    gl->ClearBufferiv(GL_STENCIL, 0, &sCv); _SglThrowErrorOccuried();
                }
            }
        }
    }
}

_SGL void _SglDpuNextPass(sglDpuIdd* dpu, _afxDscrCmd const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    (void)cmd;
    ++(dpu->state.renderPass.activeSubpass);
}

// STATE SETTING

_SGL void _SglDpuSetScissors(sglDpuIdd* dpu, _afxDscrCmdSetScissor const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->cnt);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET > cmd->first);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cmd->first + cmd->cnt);
    AfxAssert(!cmd->cnt || (cmd->cnt));

    for (afxNat i = 0; i < cmd->cnt; i++)
        AfxRectCopy(&(dpu->state.scissorArea[cmd->first + i]), &cmd->rect[i]);

    dpu->flushSs = TRUE;

    if (dpu->flushSsBase > cmd->first)
        dpu->flushSsBase = cmd->first;

    if (dpu->flushSsCnt < cmd->first + cmd->cnt)
        dpu->flushSsCnt = cmd->first + cmd->cnt;
}

_SGL void _SglDpuSetViewports(sglDpuIdd* dpu, _afxDscrCmdSetVp const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->cnt);
    AfxAssert(_SGL_MAX_VP_PER_SET > cmd->first);
    AfxAssert(_SGL_MAX_VP_PER_SET >= cmd->first + cmd->cnt);
    AfxAssert(!cmd->cnt || (cmd->cnt));

    for (afxNat i = 0; i < cmd->cnt; i++)
        dpu->state.vp[cmd->first + i] = cmd->vp[i];

    if (dpu->flushVsBase > cmd->first)
        dpu->flushVsBase = cmd->first;

    if (dpu->flushVsCnt < cmd->first + cmd->cnt)
        dpu->flushVsCnt = cmd->first + cmd->cnt;
}

_SGL void _SglDpuSetRasterizerState(sglDpuIdd* dpu, _afxDscrCmdSetRasterizerState const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    dpu->state.raster = cmd->state;
    dpu->flushRs = TRUE;
}

_SGL void _SglDpuSetColorBlendState(sglDpuIdd* dpu, afxPipelineColorBlendState const *state, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    AfxAssert(state);
    dpu->state.colorBlend = *state;
    dpu->flushCbs = TRUE;
}

_SGL void _SglDpuSetMultisampleState(sglDpuIdd* dpu, afxPipelineMultisampleState const *state, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    AfxAssert(state);
    dpu->state.multisample = *state;
    dpu->flushMs = TRUE;
}

_SGL void _SglDpuSetDepthState(sglDpuIdd* dpu, _afxDscrCmdSetDepthState const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    dpu->state.depth = cmd->state;
    dpu->flushDs = TRUE;
}

_SGL void _SglDpuSetInputAssemblyState(sglDpuIdd* dpu, _afxDscrCmdSetInputAssemblyState const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    dpu->state.ia = cmd->state;
    dpu->flushIa = TRUE;
}

#if 0
_SGL void _SglDpuPushNextVao(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;

    _sglDthrIdd*thdIdd = dque->activeExecUnit;
    glVmt const* gl = &thdIdd->wglVmt;

    {
        if (_SGL_MAX_VAO_PER_TIME > dpu->activeVaoIdx)
            ++dpu->activeVaoIdx;
        else
            dpu->activeVaoIdx = 0;

        if (dpu->vao[dpu->activeVaoIdx])
        {
            gl->DeleteVertexArrays(1, &(dpu->vao[dpu->activeVaoIdx])); _SglThrowErrorOccuried();
        }
        gl->GenVertexArrays(1, &(dpu->vao[dpu->activeVaoIdx])); _SglThrowErrorOccuried();
        gl->BindVertexArray(dpu->vao[dpu->activeVaoIdx]); _SglThrowErrorOccuried();
        AfxAssert(gl->IsVertexArray(dpu->vao[dpu->activeVaoIdx]));

        afxPipeline pip = dpu->state.pip;
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
                case AFX_VTX_FMT_QUAT:
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

                if (FALSE == dpu->state.vertexBindings[binding].wild)
                {
                    afxVertexBuffer vbuf = dpu->state.vertexBindings[binding].vbuf;
                    afxNat baseVtx = dpu->state.vertexBindings[binding].baseVtx;
                    afxNat vtxArr = dpu->state.vertexBindings[binding].arr;
                    offset = AfxGetVertexStreamOffset(vbuf, baseVtx, vtxArr);
                    stride = glStride;

                    if (vbuf->buf.gpuHandle != lastBuf)
                    {
                        gl->BindBuffer(GL_ARRAY_BUFFER, vbuf->buf.gpuHandle); _SglThrowErrorOccuried();
                    }
                    lastBuf = vbuf->buf.gpuHandle;
                }
                else
                {
                    afxBuffer buf = dpu->state.vertexBindings[binding].buf;
                    offset = dpu->state.vertexBindings[binding].offset;
                    stride = dpu->state.vertexBindings[binding].stride;

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

#if 0
_SGL void _SglDpuSetInputStreams(sglDpuIdd* dpu, afxNat cnt, afxPipelineInputStream const streams[], glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    AfxAssert(cnt);
    AfxAssert(_SGL_MAX_INSTREAM_PER_SET >= cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        dpu->state.inStreams[i].format = streams[i].format;
        dpu->state.inStreams[i].location = streams[i].location;
        
        //dpu->state.inStreams[i].binding = ;
        //dpu->state.inStreams[i].offset = ;

        AfxAssert(16 > streams[i].location);  // max vertex attrib
        //AfxAssert(streams[i].location == streams[i].binding);
        //AfxAssert(streams[i].offset == 0);
    }
    dpu->state.inStreamCnt = cnt;
    dpu->flushIns = TRUE;
}
#endif

// RESOURCE BINDING

_SGL void _SglDpuBindPipeline(sglDpuIdd* dpu, _afxDscrCmdBindPip const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;

    afxPipeline pip = (dpu->state.pip = cmd->pip);
    dpu->flushPip = TRUE;

    //glVmt const* gl = &thdIdd->wglVmt;
#if 0
    afxNat inStreamCnt = 0;
    afxPipelineInputStream inStreams[8];

    for (afxNat i = 0; i < AfxCountPipelineShaders(dpu->state.pip); i++)
    {
        afxShader shd;
        AfxGetPipelineShaders(dpu->state.pip, i, 1, &shd);

        if (AFX_SHADER_STAGE_VERTEX == AfxGetShaderStage(shd))
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

    afxNat cnt;

    if ((cnt = AfxCountPipelineInputStreams(pip)))
    {
        afxPipelineInputStream ins[_SGL_MAX_INSTREAM_PER_SET];
        afxNat inCnt = AfxGetPipelineInputStreams(pip, 0, cnt, ins);
        //_SglDpuSetInputStreams(dpu, inCnt, ins, gl);
    }

    _afxDscrCmdSetInputAssemblyState iasCmd;    
    if (AfxGetPipelineInputAssembling(pip, &iasCmd.state))
        _SglDpuSetInputAssemblyState(dpu, &iasCmd, gl);

    _afxDscrCmdSetRasterizerState rasCmd;
    if (AfxGetPipelineRasterization(pip, &rasCmd.state))
        _SglDpuSetRasterizerState(dpu, &rasCmd, gl);

    afxPipelineMultisampleState ms;
    if (AfxGetPipelineMultisampling(pip, &ms))
        _SglDpuSetMultisampleState(dpu, &ms, gl);

    _afxDscrCmdSetDepthState dCmd;
    if (AfxGetPipelineDepthHandling(pip, &dCmd.state))
        _SglDpuSetDepthState(dpu, &dCmd, gl);

    afxPipelineColorBlendState cb;
    if (AfxGetPipelineColorBlending(pip, &cb))
        _SglDpuSetColorBlendState(dpu, &cb, gl);

    if ((cnt = AfxCountPipelineViewports(pip)))
    {
        _afxDscrCmdSetVp vpCmd;
        vpCmd.first = 0;
        if ((vpCmd.cnt = AfxGetPipelineViewports(pip, 0, cnt, vpCmd.vp)))
            _SglDpuSetViewports(dpu, &vpCmd, gl);
    }

    if ((cnt = AfxCountPipelineScissors(pip)))
    {
        _afxDscrCmdSetScissor sciCmd;
        sciCmd.first = 0;
        if ((sciCmd.cnt = AfxGetPipelineScissors(pip, 0, cnt, sciCmd.rect)))
            _SglDpuSetScissors(dpu, &sciCmd, gl);
    }
}

_SGL void _SglDpuBindTextures(sglDpuIdd* dpu, _afxDscrCmdBindTextures const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    afxNat first = cmd->first;
    AfxAssertRange(_SGL_MAX_LEGO_PER_BIND, first, cmd->cnt);

    //glVmt const *gl = &thdIdd->wglVmt;
#if 0
    AfxAssert(dpu->state.pip);
    afxNat setCnt = dpu->state.pip->wiringCnt;
    AfxAssert(cmd->set < setCnt);

    afxNat bindingCnt = 0;

    for (afxNat i = 0; i < setCnt; i++)
        if (dpu->state.pip->wiring[i].set == cmd->set)
            bindingCnt = dpu->state.pip->wiring[i].legtD->entryCnt;

    AfxAssert(bindingCnt);
    AfxAssert(cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= bindingCnt);
    AfxAssert(first + cmd->cnt <= _SGL_MAX_ENTRY_PER_LEGO);
#endif

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxNat binding = (cmd->set * _SGL_MAX_ENTRY_PER_LEGO) + first + i;

        if ((dpu->state.boundRes[cmd->set][first + i].tex = cmd->tex[i]))
        {
            AfxAssertObjects(1, &cmd->tex[i], AFX_FCC_TEX);
            
            //if (_SglDpuBindAndSyncTex(dpu, binding, cmd->tex[i], gl))
                //AfxThrowError();
        }

        if ((dpu->state.boundRes[cmd->set][first + i].smp = cmd->smp[i]))
        {
            AfxAssertObjects(1, &cmd->smp[i], AFX_FCC_SAMP);
            
            //if (_SglDpuBindAndSyncSamp(dpu, binding, cmd->smp[i], gl))
                //AfxThrowError();
        }
    }

    if (dpu->flushRbBase[cmd->set] > first)
        dpu->flushRbBase[cmd->set] = first;

    if (dpu->flushRbCnt[cmd->set] < cmd->cnt + first)
        dpu->flushRbCnt[cmd->set] = cmd->cnt + first;

    dpu->flushRb = TRUE;
}

_SGL void _SglDpuBindBuffers(sglDpuIdd* dpu, _afxDscrCmdBindBuffers const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    ////sglDqueIdd *dpu = dque->dpu;
    afxNat first = cmd->first;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > first);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cmd->cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= first + cmd->cnt);

    //glVmt const *gl = &thdIdd->wglVmt;
#if 0
    AfxAssert(dpu->state.pip);
    afxPipelineRig pipr = AfxPipelineGetRig(dpu->state.pip);
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
        
        afxBuffer buf;
        dpu->state.boundRes[cmd->set][first + i].buf = (buf = cmd->buf[i]);
        dpu->state.boundRes[cmd->set][first + i].offset = cmd->offset[i];
        
        if (buf)
        {
            AfxAssertObjects(1, &buf, AFX_FCC_BUF);
            
            //if (_SglDpuBindAndSyncBuf(dpu, binding, cmd->buf[i], cmd->offset[i], cmd->range[i], GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, gl))
                //AfxThrowError();
        }

        dpu->state.boundRes[cmd->set][first + i].range = cmd->range[i] ? cmd->range[i] : buf ? AfxGetBufferSize(buf) - cmd->offset[i] : cmd->range[i];
    }

    if (dpu->flushRbBase[cmd->set] > first)
        dpu->flushRbBase[cmd->set] = first;

    if (dpu->flushRbCnt[cmd->set] < cmd->cnt + first)
        dpu->flushRbCnt[cmd->set] = cmd->cnt + first;

    dpu->flushRb = TRUE;
}

_SGL void _SglDpuBindIndexStream(sglDpuIdd* dpu, _afxDscrCmdBindIbuf const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;

    dpu->state.indexBinding.buf = cmd->buf;
    dpu->state.indexBinding.offset = cmd->offset;
    dpu->state.indexBinding.idxSiz = cmd->idxSiz;
    
    dpu->flushIbb = TRUE;
}

_SGL void _SglDpuSetVertexInputLayout(sglDpuIdd* dpu, _afxDscrCmdSetVtxInLayout const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cmd);
    
    afxNat cnt = cmd->cnt;
    AfxAssertRange(_SGL_MAX_VBO_PER_BIND, 0, cnt);

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxVertexInputPoint const* spec = &cmd->spec[i];

        dpu->state.ins[i].stream = spec->stream;
        dpu->state.ins[i].location = spec->location;
        dpu->state.ins[i].fmt = spec->fmt;
        dpu->state.ins[i].offset = spec->offset;
        dpu->state.ins[i].usage = spec->usage;
    }
    dpu->state.inCnt = cnt;
    dpu->flushIns = TRUE;
}

_SGL void _SglDpuBindVertexStreams(sglDpuIdd* dpu, _afxDscrCmdBindVbuf const *cmd, glVmt const* gl)
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
    afxNat first = cmd->first, cnt = cmd->cnt;
    AfxAssertRange(_SGL_MAX_VBO_PER_BIND, first, cnt);

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxVertexInputStream const* spec = &cmd->spec[i];

        dpu->state.vertexBindings[first + i].buf = spec->buf;
        dpu->state.vertexBindings[first + i].offset = spec->offset;
        dpu->state.vertexBindings[first + i].range = spec->range;
        dpu->state.vertexBindings[first + i].stride = spec->stride;
        dpu->state.vertexBindings[first + i].inst = spec->instance;
        dpu->state.vertexBindings[first + i].instDivisor = spec->instDivisor;
    }

    dpu->state.vertexBindingCnt = cmd->cnt;
    dpu->flushVbb = TRUE;

    if (dpu->flushVbBase > first)
        dpu->flushVbBase = first;

    if (dpu->flushVbCnt < cnt + first)
        dpu->flushVbCnt = cnt + first;
}

// DO WORK

_SGL void _SglFlushStateChanges(sglDpuIdd* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    // BEGIN RENDER PASS

    // BIND PIPELINE SHADERS

    if (dpu->flushPip)
    {
        dpu->flushPip = FALSE;
        afxPipeline pip = dpu->state.pip;

        if (pip)
        {
            AfxAssertObjects(1, &pip, AFX_FCC_PIP);
            _SglDpuBindAndSyncPip(dpu, pip, gl);
            gl->UseProgram(pip->glHandle); _SglThrowErrorOccuried();
        }
        else
        {
            gl->UseProgram(0); _SglThrowErrorOccuried();
        }
    }

    if (dpu->flushVsCnt)
    {
        if (gl->ViewportArrayv)
        {
            GLfloat v[_SGL_MAX_VP_PER_SET][4];
            AfxAssert(_SGL_MAX_VP_PER_SET >= dpu->flushVsCnt);

            for (afxNat i = 0; i < dpu->flushVsCnt; i++)
                v[i][0] = dpu->state.vp[i].offset[0],
                v[i][1] = dpu->state.vp[i].offset[1],
                v[i][2] = dpu->state.vp[i].extent[0],
                v[i][3] = dpu->state.vp[i].extent[1];

            gl->ViewportArrayv(dpu->flushVsBase, dpu->flushVsCnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLint x = (GLint)(dpu->state.vp[0].offset[0]);
            GLint y = (GLint)(dpu->state.vp[0].offset[1]);
            GLsizei w = (GLsizei)(dpu->state.vp[0].extent[0]);
            GLsizei h = (GLsizei)(dpu->state.vp[0].extent[1]);

            gl->Viewport(x, y, w, h); _SglThrowErrorOccuried();
        }

        if (gl->DepthRangeArrayv)
        {
            GLdouble v[_SGL_MAX_VP_PER_SET][2];
            AfxAssert(_SGL_MAX_VP_PER_SET >= dpu->flushVsCnt);

            for (afxNat i = 0; i < dpu->flushVsCnt; i++)
                v[i][0] = dpu->state.vp[i].depth[0],
                v[i][1] = dpu->state.vp[i].depth[1];

            gl->DepthRangeArrayv(dpu->flushVsBase, dpu->flushVsCnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLdouble n = dpu->state.vp[0].depth[0];
            GLdouble f = dpu->state.vp[0].depth[1];

            gl->DepthRange(n, f); _SglThrowErrorOccuried();
        }

        dpu->flushVsBase = 0;
        dpu->flushVsCnt = 0;
    }

    // APPLY SCISSOR STATE

    if (dpu->flushSs)
    {
        dpu->flushSs = FALSE;

        if (dpu->flushSsCnt)
        {
            gl->Enable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();

            if (gl->ScissorArrayv)
            {
                GLint v[_SGL_MAX_SCISSOR_PER_SET][4];
                AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= dpu->flushSsCnt);

                for (afxNat i = 0; i < dpu->flushSsCnt; i++)
                    v[i][0] = dpu->state.scissorArea[i].offset[0],
                    v[i][1] = dpu->state.scissorArea[i].offset[1],
                    v[i][2] = dpu->state.scissorArea[i].extent[0],
                    v[i][3] = dpu->state.scissorArea[i].extent[1];

                gl->ScissorArrayv(dpu->flushSsBase, dpu->flushSsCnt, &v[0][0]); _SglThrowErrorOccuried();
            }
            else
            {
                GLint x = (GLint)(dpu->state.scissorArea[0].offset[0]);
                GLint y = (GLint)(dpu->state.scissorArea[0].offset[1]);
                GLsizei w = (GLsizei)(dpu->state.scissorArea[0].extent[0]);
                GLsizei h = (GLsizei)(dpu->state.scissorArea[0].extent[1]);

                AfxAssert(dpu->flushSsCnt == 1);
                gl->Scissor(x, y, w, h); _SglThrowErrorOccuried();
            }

            dpu->flushSsBase = 0;
            dpu->flushSsCnt = 0;
        }
        else
        {
            gl->Disable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
        }
    }

    // APPLY RASTERIZER STATE

    if (dpu->flushRs)
    {
        dpu->flushRs = FALSE;

        if (dpu->state.raster.depthClampEnable)
        {
            gl->Enable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
        }

        if (dpu->state.raster.rasterizerDiscardEnable)
        {
            gl->Enable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
        }

        gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlPolygonMode(dpu->state.raster.fillMode)); _SglThrowErrorOccuried();

        if (dpu->state.raster.cullMode)
        {
            gl->Enable(GL_CULL_FACE); _SglThrowErrorOccuried();
            gl->CullFace(AfxToGlCullMode(dpu->state.raster.cullMode)); _SglThrowErrorOccuried();
            gl->FrontFace(AfxToGlFrontFace(dpu->state.raster.frontFace)); _SglThrowErrorOccuried();
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

        if (dpu->state.raster.depthBiasEnable)
        {
            gl->Enable(QwadroToGlPolygonModeBasedOffset[dpu->state.raster.fillMode]); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(QwadroToGlPolygonModeBasedOffset[dpu->state.raster.fillMode]); _SglThrowErrorOccuried();
        }

        gl->PolygonOffset(dpu->state.raster.depthBiasSlopeFactor, dpu->state.raster.depthBiasConstantFactor); _SglThrowErrorOccuried();
        gl->LineWidth(dpu->state.raster.lineWidth); _SglThrowErrorOccuried();
    }

    // APPLY DEPTH/STENCIL STATE

    if (dpu->flushDs)
    {
        dpu->flushDs = FALSE;

        if (dpu->state.depth.depthTestEnable)
        {
            gl->Enable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
            gl->DepthFunc(SglToGlCompareOp(dpu->state.depth.depthCompareOp)); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        }

        if (dpu->state.depth.depthWriteEnable)
        {
            gl->DepthMask(GL_TRUE); _SglThrowErrorOccuried();
        }
        else
        {
            gl->DepthMask(GL_FALSE); _SglThrowErrorOccuried();
        }

        if (dpu->state.depth.stencilTestEnable)
        {
            gl->Enable(GL_STENCIL_TEST); _SglThrowErrorOccuried();

            gl->StencilFuncSeparate(GL_FRONT, SglToGlCompareOp(dpu->state.depth.stencilOpFront.compareOp), 0, 1); _SglThrowErrorOccuried();
            gl->StencilOpSeparate(GL_FRONT, AfxToGlStencilOp(dpu->state.depth.stencilOpFront.failOp), AfxToGlStencilOp(dpu->state.depth.stencilOpFront.depthFailOp), AfxToGlStencilOp(dpu->state.depth.stencilOpFront.passOp)); _SglThrowErrorOccuried();
            gl->StencilFuncSeparate(GL_BACK, SglToGlCompareOp(dpu->state.depth.stencilOpBack.compareOp), 0, 1); _SglThrowErrorOccuried();
            gl->StencilOpSeparate(GL_BACK, AfxToGlStencilOp(dpu->state.depth.stencilOpBack.failOp), AfxToGlStencilOp(dpu->state.depth.stencilOpBack.depthFailOp), AfxToGlStencilOp(dpu->state.depth.stencilOpBack.passOp)); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
        }
    }

    // APPLY MULTISAMPLING STATE

    if (dpu->flushMs)
    {
        dpu->flushMs = FALSE;

        if (dpu->state.multisample.sampleShadingEnable)
        {
            gl->Enable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
        }

        if (dpu->state.multisample.pSampleMask)
        {
            gl->Enable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
        }

        gl->SampleMaski(0, dpu->state.multisample.pSampleMask[0]); _SglThrowErrorOccuried();

        if (dpu->state.multisample.minSampleShading)
        {
            gl->Enable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
        }

        if (gl->MinSampleShading) // gl v4.1
        {
            gl->MinSampleShading(dpu->state.multisample.minSampleShading); _SglThrowErrorOccuried();
        }

        if (dpu->state.multisample.alphaToOneEnable)
        {
            gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
        }

        if (dpu->state.multisample.alphaToCoverageEnable)
        {
            gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
        }
    }

    // APPLY INPUT-ASSEMBLER STATE

    if (dpu->flushIa)
    {
        dpu->flushIa = FALSE;

        if (dpu->state.ia.primRestartEnable)
        {
            gl->Enable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
        }
    }

    // BIND RESOURCES (TEXTURES, SAMPLERS AND BUFFERS)

    if (dpu->flushRb)
    {
        dpu->flushRb = FALSE;

        for (afxNat i = 0; i < dpu->state.pip->base.wiringCnt; i++)
        {
            afxNat set = dpu->state.pip->base.wiring->set;
            afxPipelineRig lego = dpu->state.pip->base.wiring->legt;
            AfxAssert(dpu->state.pip->base.wiring->resolved);
            AfxAssert(lego);
            AfxAssertObjects(1, &lego, AFX_FCC_LEGO);

            for (afxNat j = AfxMini(dpu->flushRbBase[set], lego->base.entryCnt - 1); j < AfxMini(dpu->flushRbCnt[set], lego->base.entryCnt); j++)
            {
                afxPipelineRigEntry const *entry = &lego->base.entries[j];
                AfxAssert(entry->type);
                afxNat binding = (set*10) + entry->binding;
                afxNat loc;
                
                afxSampler samp;
                afxTexture tex;
                afxBuffer buf;

                switch (entry->type)
                {
                case AFX_SHD_RES_TYPE_SAMPLER:
                {
                    if ((samp = dpu->state.boundRes[set][j].smp))
                    {
                        AfxAssertObjects(1, &samp, AFX_FCC_SAMP);
                        _SglDpuBindAndSyncSamp(dpu, binding, samp, gl);
                        gl->BindSampler(binding, samp->glHandle); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        gl->BindSampler(binding, 0); _SglThrowErrorOccuried();
                    }
                    break;
                }
                case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
                {
                    if ((tex = dpu->state.boundRes[set][j].tex))
                    {
                        AfxAssertObjects(1, &tex, AFX_FCC_TEX);
                        _SglDpuBindAndSyncTex(dpu, binding, tex, gl);

                        if (gl->BindTextureUnit)
                        {
                            gl->BindTextureUnit(binding, tex->glHandle); _SglThrowErrorOccuried();
                        }
                        else
                        {
                            gl->ActiveTexture(GL_TEXTURE0 + binding); _SglThrowErrorOccuried();
                            gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();
                        }
                    }
                    else
                    {
                        if (gl->BindTextureUnit)
                        {
                            gl->BindTextureUnit(binding, 0); _SglThrowErrorOccuried();
                        }
                        else
                        {
                            gl->ActiveTexture(GL_TEXTURE0 + binding); _SglThrowErrorOccuried();
                            gl->BindTexture(GL_TEXTURE_2D, 0); _SglThrowErrorOccuried();
                        }
                    }
                    break;
                }
                case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
                {
                    if ((tex = dpu->state.boundRes[set][j].tex))
                    {
                        AfxAssertObjects(1, &tex, AFX_FCC_TEX);
                        _SglDpuBindAndSyncTex(dpu, binding, tex, gl);

                        if (gl->BindTextureUnit)
                        {
                            gl->BindTextureUnit(binding, tex->glHandle); _SglThrowErrorOccuried();
                        }
                        else
                        {
                            gl->ActiveTexture(GL_TEXTURE0 + binding); _SglThrowErrorOccuried();
                            gl->BindTexture(tex->glTarget, tex->glHandle); _SglThrowErrorOccuried();
                        }
                    }
                    else
                    {
                        if (gl->BindTextureUnit)
                        {
                            gl->BindTextureUnit(binding, 0); _SglThrowErrorOccuried();
                        }
                        else
                        {
                            gl->ActiveTexture(GL_TEXTURE0 + binding); _SglThrowErrorOccuried();
                            gl->BindTexture(GL_TEXTURE_2D, 0); _SglThrowErrorOccuried();
                        }
                    }

                    if ((samp = dpu->state.boundRes[set][j].smp))
                    {
                        AfxAssertObjects(1, &samp, AFX_FCC_SAMP);
                        _SglDpuBindAndSyncSamp(dpu, binding, samp, gl);
                        gl->BindSampler(binding, samp->glHandle); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        gl->BindSampler(binding, 0); _SglThrowErrorOccuried();
                    }
                    break;
                }
                case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
                {
                    afxNat offset = dpu->state.boundRes[set][j].offset;
                    afxNat range = dpu->state.boundRes[set][j].range;

                    if ((buf = dpu->state.boundRes[set][j].buf))
                    {
                        AfxAssertObjects(1, &buf, AFX_FCC_BUF);
                        _SglDpuBindAndSyncBuf(dpu, binding, buf, offset, range, GL_UNIFORM_BUFFER, buf->glUsage ? buf->glUsage : GL_DYNAMIC_DRAW, gl);
                        gl->BindBufferRange(GL_UNIFORM_BUFFER, binding, buf->glHandle, offset, range); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        gl->BindBufferRange(GL_UNIFORM_BUFFER, binding, 0, offset, range); _SglThrowErrorOccuried();
                    }
                    break;
                }
                default:
                {
                    AfxError("");
                }
                }
            }

            dpu->flushRbBase[set] = 0;
            dpu->flushRbCnt[set] = 0;
        }
    }

    // APPLY INPUT-STREAMING LAYOUT

    if (dpu->flushIns)
    {
        dpu->flushIns = FALSE;

        dpu->activeVaoIdx = (dpu->activeVaoIdx + 1) % _SGL_MAX_VAO_PER_TIME;

#if !0
        if (dpu->vao[dpu->activeVaoIdx])
        {
            gl->DeleteVertexArrays(1, &(dpu->vao[dpu->activeVaoIdx])); _SglThrowErrorOccuried();
        }
        gl->GenVertexArrays(1, &(dpu->vao[dpu->activeVaoIdx])); _SglThrowErrorOccuried();
        gl->BindVertexArray(dpu->vao[dpu->activeVaoIdx]); _SglThrowErrorOccuried();
#endif
        afxNat cnt = dpu->state.inCnt;

        for (afxNat i = 0; i < cnt; i++)
        {
            GLint glsiz;
            GLenum gltype;
            GLuint glStride;
            afxVertexFormat fmt = dpu->state.ins[i].fmt;

            switch (fmt)
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

            afxNat loc = dpu->state.ins[i].location;
            afxNat stream = dpu->state.ins[i].stream;
            afxSize inOff = dpu->state.ins[i].offset;
            AfxAssert(16 > loc);  // max vertex attrib
            //AfxAssert(loc == bind);

            //afxBuffer buf = dpu->state.vertexBindings[i].buf;
            //afxNat bufOff = dpu->state.vertexBindings[i].offset;
            //afxNat bufRange = dpu->state.vertexBindings[i].range;
            //afxNat bufStride = dpu->state.vertexBindings[i].stride;
            //AfxAssert(bufStride == glStride);

            AfxAssert(gl->BindVertexBuffer);

            gl->EnableVertexAttribArray(loc); _SglThrowErrorOccuried();
            gl->VertexAttribFormat(loc, glsiz, gltype, FALSE, inOff); _SglThrowErrorOccuried();
            gl->VertexAttribBinding(loc, stream); _SglThrowErrorOccuried();

            {
                //gl->BindVertexBuffer(bind, buf ? buf->glHandle : 0, bufOff, bufStride); _SglThrowErrorOccuried();
                //gl->VertexAttribPointer(loc, glsiz, gltype, FALSE, glStride, streams[i].offset); _SglThrowErrorOccuried();
            }

#if 0
            else
            {
                //AfxAssert(bufOff == 0);
                //AfxAssert(bufStride == 0);
                //AfxAssert(bind == 0);

                //_SglDpuBindAndSyncBuf(dpu, i, buf, bufOff, glStride, GL_ARRAY_BUFFER, GL_STATIC_DRAW, gl);
                gl->BindBuffer(GL_ARRAY_BUFFER, buf ? buf->glHandle : 0); _SglThrowErrorOccuried();
                gl->VertexAttribPointer(loc, glsiz, gltype, FALSE, glStride, (const void*)inOff); _SglThrowErrorOccuried();
            }
#endif
            //AfxAssert(inOff == 0);
        }
    }

    // BIND VERTEX BUFFER

    if (dpu->flushVbb)
    {
        dpu->flushVbb = FALSE;

        afxNat first = dpu->flushVbBase;
        afxNat cnt = dpu->flushVbCnt;
        afxBuffer buf;

        for (afxNat i = 0; i < cnt; i++)
        {
            afxNat offset = dpu->state.vertexBindings[first + i].offset;
            afxNat stride = dpu->state.vertexBindings[first + i].stride;
            afxNat divisor = dpu->state.vertexBindings[first + i].instDivisor;

            if ((buf = dpu->state.vertexBindings[first + i].buf))
            {
                AfxAssertObjects(1, &buf, AFX_FCC_BUF);
                _SglDpuBindAndSyncBuf(dpu, first + i, buf, offset, stride, GL_ARRAY_BUFFER, buf->glUsage ? buf->glUsage : GL_STATIC_DRAW, gl);
            }

            gl->BindVertexBuffer(first + i, buf ? buf->glHandle : 0, offset, stride); _SglThrowErrorOccuried();
            int a = 0;
            //gl->VertexBindingDivisor(first + i, divisor); _SglThrowErrorOccuried();
        }

        dpu->flushVbBase = 0;
        dpu->flushVbCnt = 0;
    }

    // BIND INDEX BUFFER

    if (dpu->flushIbb)
    {
        dpu->flushIbb = FALSE;

        afxBuffer buf = dpu->state.indexBinding.buf;
        afxNat off = dpu->state.indexBinding.offset;
        afxNat stride = dpu->state.indexBinding.idxSiz;

        // WARNING: element array objects binding doesn't allow offset or ranges at bind time. So we do it at draw call time.

        if (buf)
        {
            AfxAssertObjects(1, &buf, AFX_FCC_BUF);
            _SglDpuBindAndSyncBuf(dpu, 0, buf, off, stride, GL_ELEMENT_ARRAY_BUFFER, buf->glUsage ? buf->glUsage : GL_STATIC_DRAW, gl);
        }

        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf ? buf->glHandle : 0); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDpuDraw(sglDpuIdd* dpu, _afxDscrCmdDraw const *cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //sglDqueIdd *dpu = dque->dpu;
    
    _SglFlushStateChanges(dpu);

    //_SglDpuUpdateNextStateObjects(dctx, queueIdx);
    //_SglDpuFlushNextState(dctx, queueIdx);

    AfxAssert(dpu->state.pip);
    //AfxAssert(dpu->state.vertexBindingCnt);

    AfxAssert(cmd->vtxCnt);
    AfxAssert(cmd->instCnt);
    GLenum top = AfxToGlTopology(dpu->state.ia.topology);

    gl->DrawArraysInstanced(top, cmd->firstVtx, cmd->vtxCnt, cmd->instCnt); _SglThrowErrorOccuried();
    
    dpu->numOfFedVertices += cmd->vtxCnt;
    dpu->numOfFedInstances += cmd->instCnt;

    //AfxEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _SglDpuDrawIndexed(sglDpuIdd* dpu, _afxDscrCmdDrawIndexed const* cmd, glVmt const* gl)
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
    //sglDqueIdd *dpu = dque->dpu;
    _SglFlushStateChanges(dpu);
    //_SglDpuUpdateNextStateObjects(dctx, queueIdx);
    //_SglDpuFlushNextState(dctx, queueIdx);

    AfxAssert(dpu->state.pip);
    AfxAssertObjects(1, &dpu->state.indexBinding.buf, AFX_FCC_BUF);
    AfxAssert(dpu->state.vertexBindingCnt);

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

    afxSize idxSiz = dpu->state.indexBinding.idxSiz;
    afxSize idxBaseOff = dpu->state.indexBinding.offset + (idxSiz * cmd->firstIdx);
    
    GLint vtxOff2 = cmd->vtxOff;

    GLenum top = AfxToGlTopology(dpu->state.ia.topology);

    gl->DrawElementsInstancedBaseVertex(top, cmd->idxCnt, idxSizGl[idxSiz], (void const*)(idxBaseOff), cmd->instCnt, vtxOff2); _SglThrowErrorOccuried();
    
    dpu->numOfFedIndices += cmd->idxCnt;
    dpu->numOfFedInstances += cmd->instCnt;
    //AfxEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_SGL void _SglDpuExecuteCommands(sglDpuIdd* dpu, _afxDscrCmdExecCmds const* cmd, glVmt const* gl);

void (*_SglDpuCmdVmt[AFX_DCMD_TOTAL])(sglDpuIdd* dpu, _afxDscrCmd const *cmd, glVmt const* gl) =
{
    NIL,
    (void*)_SglDpuExecuteCommands,

    (void*)_SglDpuBeginCanvas,
    (void*)_SglDpuNextPass,
    (void*)_SglDpuEndCanvas,

    (void*)_SglDpuBindPipeline,

    (void*)_SglDpuSetInputAssemblyState,
    (void*)_SglDpuSetViewports,
    (void*)_SglDpuSetRasterizerState,
    (void*)_SglDpuSetDepthState,
    (void*)_SglDpuSetScissors,

    (void*)_SglDpuBindBuffers,
    (void*)_SglDpuBindVertexStreams,
    (void*)_SglDpuSetVertexInputLayout,
    (void*)_SglDpuBindIndexStream,
    (void*)_SglDpuBindTextures,

    (void*)_SglDpuDraw,
    (void*)_SglDpuDrawIndexed
};

_SGL void _SglDpuExecuteCommands(sglDpuIdd* dpu, _afxDscrCmdExecCmds const* cmd, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cmd->cnt; i++)
    {
        afxDrawScript dscr = cmd->subsets[i];
        AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
        AfxAssert(dscr->base.state == AFX_DSCR_STATE_PENDING);

        if (dscr->base.state == AFX_DSCR_STATE_PENDING)
        {
            _afxDscrCmd *cmdHdr;
            AfxChainForEveryLinkageB2F(&dscr->commands, _afxDscrCmd, script, cmdHdr)
            {
                if (cmdHdr->id == AFX_DCMD_END)
                    break;

                if (dscr->base.state != AFX_DSCR_STATE_PENDING)
                {
                    AfxThrowError();
                    break;
                }

                _SglDpuCmdVmt[cmdHdr->id](dpu, cmdHdr, gl);
            }

            if (!err)
            {
                dscr->base.state = AFX_DSCR_STATE_EXECUTABLE;
            }

            if (err || dscr->base.disposable)
            {
                AfxAssert(dscr->base.portIdx == dpu->portIdx);
                dscr->base.state = AFX_DSCR_STATE_INVALID;
            }
        }
    }
}

// QUEUE STUFF

_SGL afxError _SglDpuExecuteDscr(sglDpuIdd* dpu, afxDrawScript dscr, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(dque, AFX_FCC_DQUE);
    //glVmt const* gl = &thdIdd->wglVmt;

    AfxAssertObjects(1, &dpu->activeDctx, AFX_FCC_DCTX);
    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
    
    AfxAssert(dscr->base.state == AFX_DSCR_STATE_PENDING);

    if (dscr->base.state == AFX_DSCR_STATE_PENDING)
    {
        _afxDscrCmd *cmdHdr;
        AfxChainForEveryLinkageB2F(&dscr->commands, _afxDscrCmd, script, cmdHdr)
        {
            if (cmdHdr->id == AFX_DCMD_END)
                break;

            if (dscr->base.state != AFX_DSCR_STATE_PENDING)
            {
                AfxThrowError();
                break;
            }

            _SglDpuCmdVmt[cmdHdr->id](dpu, cmdHdr, gl);
        }

        if (!err)
        {
            dscr->base.state = AFX_DSCR_STATE_EXECUTABLE;
        }

        if (err || dscr->base.disposable)
        {
            AfxAssert(dscr->base.portIdx == dpu->portIdx);
            dscr->base.state = AFX_DSCR_STATE_INVALID;
        }
    }
    return err;
}

_SGL afxError _SglDpuPresentSurf(sglDpuIdd* dpu, afxDrawOutput dout, afxNat outBufIdx, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

    afxDrawContext dctx = dpu->activeDctx;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    
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
        afxTexture surf = dout->base.buffers[outBufIdx].tex;
        AfxAssertObjects(1, &surf, AFX_FCC_TEX);
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
        //if (doutD->presentMode == AFX_PRESENT_MODE_FIFO)
            //surf = AFX_REBASE(AfxGetFirstLinkage(&doutD->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else //if (doutD->presentMode == AFX_PRESENT_MODE_LIFO)
            //surf = AFX_REBASE(AfxGetLastLinkage(&doutD->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else AfxError("Not implemented yet.");

        //_SglDpuBindAndSyncPip(idd->presentPip, dque);
        //_SglDpuBindAndSyncBuf(&(idd->presentVbufD->buf), 0, 0, 0, GL_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
        //_SglDpuBindAndSyncLego(idd->presentLego, 0, dque);

#if !0
        
        //_AfxStdCanvUpdate(cmd->beginRenderPass.canv, dque);

        afxWhd extent;
        AfxGetTextureExtent(surf, 0, extent);
        
        _afxDscrCmdBeginCanvas cmdBeginOp = { 0 };
        //cmdBeginOp.canv = NIL;
        cmdBeginOp.area = (afxRect const){ { { 0, 0 } }, { { extent[0], extent[1] } } };
        cmdBeginOp.rasterCnt = 1;
        cmdBeginOp.rasters[0] = (afxDrawTarget const){ NIL, NIL, AFX_DTS_LOAD_OP_DONT_CARE, AFX_DTS_STORE_OP_STORE, { .color = { 0.3, 0.0, 0.3, 1 } } };

        _SglDpuBeginCanvas(dpu, &cmdBeginOp, gl);

        _afxDscrCmdBindPip cmdBindPip = { 0 };
        cmdBindPip.pip = dctx->presentPip;
        _SglDpuBindPipeline(dpu, &cmdBindPip, gl);

#else
        afxDrawTarget const rasterRt = { NIL, AFX_DTS_LOAD_OP_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _SglDpuBeginCombination(dpu, NIL, 1, 0, &rasterRt, NIL, NIL);

        afxPipeline pip = AfxDrawOperationGetPipeline(idd->presentDop, 0, 0);
        AfxAssertObjects(1, &pip, AFX_FCC_PIP);
        _SglDpuBindPipeline(dpu, pip);
#endif   
        //_SglDpuEmployTechnique(dque, 0);

        _afxDscrCmdSetVp cmdSetVp = { 0 };
        cmdSetVp.cnt = 1;
        cmdSetVp.vp[0].extent[0] = extent[0];
        cmdSetVp.vp[0].extent[1] = extent[1];
        cmdSetVp.vp[0].depth[1] = 1;

        _SglDpuSetViewports(dpu, &cmdSetVp, gl);

#if 0 // already set by pipeline
        afxPipelineRasterizerState const ras = { FALSE, FALSE, AFX_FILL_MODE_SOLID, AFX_CULL_MODE_BACK, AFX_FRONT_FACE_CCW, FALSE, 0, 0, 0, 1 };
        _SglDpuSetRasterizerState(dque, &ras);

        afxPipelineDepthState const depth = { 0 };
        _SglDpuSetDepthState(dque, &depth);
#endif

#if 0 // already set by pipeline
        afxPipelineInputAssemblyState ia;
        ia.topology = AFX_PRIM_TOPOLOGY_TRI_STRIP;
        ia.primRestartEnable = FALSE;
        _SglDpuSetInputAssemblyState(dque, &ia);
#endif
#if 0
        afxNat const baseVtxs[] = { 0, 0 };
        afxNat const vtxArrs[] = { 0, 1 };
        afxVertexBuffer vbufs[] = { idd->rgbRectVbo, idd->rgbRectVbo };
        _SglDpuBindVertexBuffers(dque, 0, 2, vbufs, baseVtxs, vtxArrs);
#endif

        afxNat visIdx;
        _afxDscrCmdSetVtxInLayout cmdSetVtxIn = { 0 };
        cmdSetVtxIn.cnt = 1;
        cmdSetVtxIn.spec[0].location = 0;
        AfxDescribeVertexAttribute(dctx->presentVbuf, 0, &visIdx, &cmdSetVtxIn.spec[0].fmt, &cmdSetVtxIn.spec[0].offset, &cmdSetVtxIn.spec[0].usage);
        cmdSetVtxIn.spec[0].stream = visIdx;
        _SglDpuSetVertexInputLayout(dpu, &cmdSetVtxIn, gl);
        
        _afxDscrCmdBindVbuf cmdBindVbuf = { 0 };
        cmdBindVbuf.cnt = 1;
        cmdBindVbuf.spec[0].buf = dctx->presentVbuf->base.buf;
        AfxDescribeVertexSection(dctx->presentVbuf, visIdx, &cmdBindVbuf.spec[0].offset, &cmdBindVbuf.spec[0].range, &cmdBindVbuf.spec[0].stride);
        _SglDpuBindVertexStreams(dpu, &cmdBindVbuf, gl);

        //_SglDpuBindLegos(dque, 0, 1, &idd->presentLego);
        _afxDscrCmdBindTextures cmdBindTex = { 0 };
        cmdBindTex.cnt = 1;
        cmdBindTex.smp[0] = dctx->presentSmp;
        cmdBindTex.tex[0] = surf;

        _SglDpuBindTextures(dpu, &cmdBindTex, gl);

        _afxDscrCmdDraw cmdDraw = { 0 };
        cmdDraw.vtxCnt = 4;
        cmdDraw.instCnt = 1;
        _SglDpuDraw(dpu, &cmdDraw, gl);

#if !0
        _SglDpuEndCanvas(dpu, NIL, gl);
#else
        _SglDpuEndCombination(dpu, NIL);
#endif
#endif
        //AfxPopLinkage(&surf->swapchain);
        
        //if (surf->state == AFX_SURF_STATE_PENDING)
        {
            //surf->state = AFX_SURF_STATE_IDLE;
            //((sglTexIdd*)surf->tex.idd)->updFlags |= SGL_UPD_FLAG_HOST_FLUSH;
        }
        // deadlocks all
        SglSwapBuffers(dout->dc, dpu);
        
        afxReal64 currTime, deltaTime;
        AfxGetExecutionTime(&currTime, &deltaTime);
        afxNat currIter, lastFreq;
        AfxGetExecutionCounter(&currIter,&lastFreq);

        //if (0 == AfxGetProcessorIterationCount())
        {
            AfxFormatString(&dout->base.caption, "Delta %0f, IPS %u/%u --- OpenGL/Vulkan Continuous Integration (c) 2017 SIGMA Technology Group --- Public Test Build", deltaTime, lastFreq, currIter);
            SetWindowTextA(dout->wnd, AfxGetStringDataConst(&dout->base.caption, 0));
        }
        dout->base.swapping = FALSE;
        dout->base.buffers[outBufIdx].booked = FALSE;
        (void)gl;
    }
    return err;
}

_SGL void* _AfxDqueRequestArenaSpace(afxDrawContext dctx, afxDrawQueue dque, afxNat siz)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);

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

    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);


    afxNat unitIdx = dque->base.portIdx;

    AfxEnterSlockExclusive(&dque->base.arenaSlock);

    AfxAssert(block);

    AfxRecycleArenaUnit(&dque->base.cmdArena, block, siz);

    AfxExitSlockExclusive(&dque->base.arenaSlock);
}

_SGL afxError _SglDqueVmtProcCb(afxDrawThread dthr, afxDrawContext dctx, afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);

    if (AfxTryEnterSlockExclusive(&dque->base.pendingChainSlock))
    {
        afxDrawDevice ddev = AfxGetObjectProvider(dctx);
        AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
   
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

        glVmt const *gl = &dpu->gl;

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
                    AfxAssertObjects(1, &dscr, AFX_FCC_DSCR);
                    
                    if (_SglDpuExecuteDscr(dpu, dscr, gl))
                        AfxThrowError();
                }
            }
            //else
            {
                for (afxNat i = 0; i < subm->outputCnt; i++)
                {
                    afxDrawOutput dout = subm->outputs[i];

                    if (_SglDpuPresentSurf(dpu, dout, subm->outBufIdx[i], gl))
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
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);

    _afxDqueSubm* subm = _AfxDqueRequestArenaSpace(dctx, dque, sizeof(*subm));

    *subm = (_afxDqueSubm) { 0 };

    subm->submNo = 0;
    subm->exec = NIL;
    subm->reqSubmNo = 0;
    AfxGetTime(&subm->pushTime);
    subm->pullTime = 0;
    subm->complTime = 0;

    subm->scriptCnt = AfxMini(spec->scriptCnt, 4);

    for (afxNat i = 0; i < subm->scriptCnt; i++)
    {
        subm->scripts[i] = spec->scripts[i];
        subm->scripts[i]->base.state = AFX_DSCR_STATE_PENDING;
    }

    subm->outputCnt = AfxMini(spec->outputCnt, 4);
    
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

_SGL afxError _SglDqueVmtWaitCb(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);
    return err;
}

_SGL afxError _SglDqueDtor(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);

    //if (dque->vmt->dtor && dque->vmt->dtor(dque))
    //    AfxThrowError();

    //AfxAssert(!dque->idd);

    AfxReleaseSlock(&dque->base.pendingChainSlock);
    AfxReleaseSlock(&dque->base.reqLock);
    AfxReleaseArena(&dque->base.cmdArena);
    AfxReleaseSlock(&dque->base.arenaSlock);

    return err;
}

_SGL afxError _SglDqueCtor(afxDrawQueue dque, afxCookie const* cookie)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dque, AFX_FCC_DQUE);

    afxDrawQueueSpecification const *spec = ((afxDrawQueueSpecification const *)cookie->udd[0]);

    AfxAssert(spec);

    afxDrawContext dctx = AfxGetObjectProvider(dque);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    afxContext ctx = AfxGetDrawContextMemory(dctx);

    dque->base.portIdx = spec->portIdx;
    
    dque->base.immediate = 0;// !!spec->immedate;

    AfxAcquireSlock(&dque->base.arenaSlock);
    AfxAcquireArena(ctx, &dque->base.cmdArena, NIL, AfxSpawnHint());

    dque->base.locked = FALSE;

    AfxAcquireChain(&dque->base.pendingChain, dque);
    AfxAcquireSlock(&dque->base.pendingChainSlock);
    AfxAcquireSlock(&dque->base.reqLock);

    dque->base.vmt = NIL;

    //if (ddev->vmt->dque && ddev->vmt->dque(dque, spec)) AfxThrowError();
    //else
    {
        //AfxAssert(dque->vmt);
    }

    if (err)
    {
        AfxReleaseSlock(&dque->base.pendingChainSlock);
        AfxReleaseSlock(&dque->base.reqLock);

        AfxReleaseArena(&dque->base.cmdArena);
    }

    return err;
}

_SGL afxClassConfig _SglDqueClsConfig =
{
    .fcc = AFX_FCC_DQUE,
    .name = "Draw Queue",
    .unitsPerPage = 10,
    .size = sizeof(AFX_OBJECT(afxDrawQueue)),
    .ctx = NIL,
    .ctor = (void*)_SglDqueCtor,
    .dtor = (void*)_SglDqueDtor
};
