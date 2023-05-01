#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_CLASSIFIED_VBUF 1
#include "sgl.h"

#include "afx/core/afxSystem.h"
#include "../e2coree/draw/_classified/afxDrawClassified.h"

typedef struct
{
    afxDrawContext dctx;
    afxNat idx;
    afxBool autonomous;
} _afxDqueCtorArgs;

// RENDERING SCOPE

_SGL void _SglDqueEndRendering(afxDrawQueue dque)
{
    afxError err = NIL;

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

_SGL void _SglDqueBeginRendering(afxDrawQueue dque, afxRect const *area, afxNat layerCnt, afxNat rasterCnt, afxDrawTarget const rasters[], afxDrawTarget const *depth, afxDrawTarget const *stencil)
{
    afxError err = NIL;
    //_gfxRendererContextState *state = &dctx->state;

    dque->state.renderPass.annexCnt = 0;
    dque->state.renderPass.canv = NIL;
    dque->state.renderPass.area = *area;
    dque->state.renderPass.layerCnt = layerCnt;
    dque->state.renderPass.rasterCnt = rasterCnt;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= rasterCnt);

    for (afxNat i = 0; i < rasterCnt; i++)
        dque->state.renderPass.rasters[i] = rasters[i];

    dque->state.renderPass.depthRt = depth ? *depth : (afxDrawTarget const) {0};
    dque->state.renderPass.stencilRt = stencil ? *stencil : (afxDrawTarget const) { 0 };

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

    if (depth)
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

    if (stencil)
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

_SGL void _SglDqueEndRenderPass(afxDrawQueue dque)
{
    afxError err = NIL;

    sglVmt const*gl = &dque->wglVmt;

    _SglDqueBindAndSyncCanv(dque, NIL, GL_DRAW_FRAMEBUFFER);
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();
    afxSurface surf;

    if (dque->state.renderPass.canv)
    {
        for (afxNat i = 0; i < dque->state.renderPass.annexCnt; i++)
        {
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

_SGL void _SglDqueBeginRenderPass(afxDrawQueue dque, afxCanvas canv, afxRect const *area, afxNat annexCnt, afxRenderPassAnnex const annexes[])
{
    afxError err = NIL;
    //_gfxRendererContextState *state = &dctx->state;
    sglVmt const *gl = &dque->wglVmt;

    dque->state.renderPass.canv = canv;
    dque->state.renderPass.area = *area;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= annexCnt);
    dque->state.renderPass.annexCnt = annexCnt;

    for (afxNat i = 0; i < annexCnt; i++)
        dque->state.renderPass.annexes[i] = annexes[i];

    dque->state.renderPass.layerCnt = 0;
    dque->state.renderPass.depthRt = (afxDrawTarget) {0};
    dque->state.renderPass.stencilRt = (afxDrawTarget) { 0 };
    dque->state.renderPass.rasters[0] = (afxDrawTarget) { 0 };
    dque->state.renderPass.rasterCnt = 0;

    _SglDqueBindAndSyncCanv(dque, canv, GL_DRAW_FRAMEBUFFER);

    if (!canv)
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
        afxNat colorAttchBaseIdx = 0;
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
                    drawBuffersIndices[colorAttchBaseIdx] = GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx;
                    gl->DrawBuffers(1, &drawBuffersIndices[colorAttchBaseIdx]); _SglThrowErrorOccuried();
                    colorAttchBaseIdx++;
                    gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
                }

                if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH))
                {
                    bufferBits |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
                    gl->ClearDepth(dque->state.renderPass.annexes[i].clearValue.depth); _SglThrowErrorOccuried();
                    gl->ClearStencil(dque->state.renderPass.annexes[i].clearValue.stencil); _SglThrowErrorOccuried();
                }

                gl->Clear(bufferBits); _SglThrowErrorOccuried();
            }
        }

        gl->DrawBuffers(colorAttchBaseIdx, drawBuffersIndices); _SglThrowErrorOccuried();

    }
}

// STATE SETTING

_SGL void _SglDqueSetScissor(afxDrawQueue dque, afxNat first, afxNat cnt, afxRect const rect[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cnt);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET > first);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= first + cnt);
    AfxAssert(!cnt || (cnt && rect));

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxRectCopy(&(dque->state.scissorArea[first + i]), &rect[i]);
    }

    sglVmt const*gl = &dque->wglVmt;

    if (cnt)
    {
        gl->Enable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();

        if (gl->ScissorArrayv)
        {
            GLint v[_SGL_MAX_SCISSOR_PER_SET][4];
            AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cnt);

            for (afxNat i = 0; i < cnt; i++)
                v[i][0] = rect[i].offset[0],
                v[i][1] = rect[i].offset[1],
                v[i][2] = rect[i].extent[0],
                v[i][3] = rect[i].extent[1];

            gl->ScissorArrayv(first, cnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLint x = (GLint)(rect[0].offset[0]);
            GLint y = (GLint)(rect[0].offset[1]);
            GLsizei w = (GLsizei)(rect[0].extent[0]);
            GLsizei h = (GLsizei)(rect[0].extent[1]);

            gl->Scissor(x, y, w, h); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->Disable(GL_SCISSOR_TEST); _SglThrowErrorOccuried();
    }
}

_SGL void _SglDqueSetViewport(afxDrawQueue dque, afxNat first, afxNat cnt, afxViewport const vp[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_VP_PER_SET >= cnt);
    AfxAssert(_SGL_MAX_VP_PER_SET > first);
    AfxAssert(_SGL_MAX_VP_PER_SET >= first + cnt);
    AfxAssert(!cnt || (cnt && vp));

    for (afxNat i = 0; i < cnt; i++)
        dque->state.vp[first + i] = vp[i];

    if (cnt)
    {
        sglVmt const*gl = &dque->wglVmt;

        if (gl->ViewportArrayv)
        {
            GLfloat v[_SGL_MAX_VP_PER_SET][4];
            AfxAssert(_SGL_MAX_VP_PER_SET >= cnt);

            for (afxNat i = 0; i < cnt; i++)
                v[i][0] = vp[i].offset[0],
                v[i][1] = vp[i].offset[1],
                v[i][2] = vp[i].extent[0],
                v[i][3] = vp[i].extent[1];

            gl->ViewportArrayv(first, cnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLint x = (GLint)(vp[0].offset[0]);
            GLint y = (GLint)(vp[0].offset[1]);
            GLsizei w = (GLsizei)(vp[0].extent[0]);
            GLsizei h = (GLsizei)(vp[0].extent[1]);

            gl->Viewport(x, y, w, h); _SglThrowErrorOccuried();
        }

        if (gl->DepthRangeArrayv)
        {
            GLdouble v[_SGL_MAX_VP_PER_SET][2];
            AfxAssert(_SGL_MAX_VP_PER_SET >= cnt);

            for (afxNat i = 0; i < cnt; i++)
                v[i][0] = vp[i].depth[0],
                v[i][1] = vp[i].depth[1];

            gl->DepthRangeArrayv(first, cnt, &v[0][0]); _SglThrowErrorOccuried();
        }
        else
        {
            GLdouble n = vp[0].depth[0];
            GLdouble f = vp[0].depth[1];

            gl->DepthRange(n, f); _SglThrowErrorOccuried();
        }
    }
}

_SGL void _SglDqueSetRasterizerState(afxDrawQueue dque, afxPipelineRasterizerState const *state)
{
    afxError err = NIL;

    AfxAssert(state);
    dque->state.raster = *state;

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
    afxError err = NIL;

    AfxAssert(state);
    dque->state.colorBlend = *state;

    AfxAssert(NIL);
}

_SGL void _SglDqueSetMultisampleState(afxDrawQueue dque, afxPipelineMultisampleState const *state)
{
    afxError err = NIL;

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

_SGL void _SglDqueSetDepthState(afxDrawQueue dque, afxPipelineDepthState const *state)
{
    afxError err = NIL;

    AfxAssert(state);
    dque->state.depth = *state;

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

_SGL void _SglDqueSetInputAssemblyState(afxDrawQueue dque, afxPipelineInputAssemblyState const *state)
{
    afxError err = NIL;

    AfxAssert(state);
    dque->state.ia = *state;

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
    afxError err = NIL;
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
        case AFX_VTX_FMT_X32:
            glsiz = 1;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        case AFX_VTX_FMT_XY32:
            glsiz = 2;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        case AFX_VTX_FMT_XYZ32:
            glsiz = 3;
            gltype = GL_FLOAT;
            glStride = sizeof(afxReal) * glsiz;
            AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
            break;
        case AFX_VTX_FMT_XYZW32:
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

        gl->VertexAttribBinding(streams[i].location, streams[i].binding); _SglThrowErrorOccuried();
        gl->VertexAttribFormat(streams[i].location, glsiz, gltype, FALSE, streams[i].offset); _SglThrowErrorOccuried();
        gl->EnableVertexAttribArray(streams[i].location); _SglThrowErrorOccuried();
        //gl->VertexAttribPointer(streams[i].location, glsiz, gltype, FALSE, glStride, streams[i].offset);
    }
    //gl->BindVertexArray(0);
}

// RESOURCE BINDING

_SGL void _SglDqueBindPipeline(afxDrawQueue dque, afxPipeline pip)
{
    afxError err = NIL;

    dque->state.pip = pip;

    sglVmt const* gl = &dque->wglVmt;

    if (dque->state.pip->inStreamCnt)
        _SglDqueSetInputStreams(dque, dque->state.pip->inStreamCnt, dque->state.pip->inStreams);

    if (dque->state.pip->hasVtxInAssembling)
        _SglDqueSetInputAssemblyState(dque, &dque->state.pip->vtxInAssembling);

    if (dque->state.pip->hasRasterization)
        _SglDqueSetRasterizerState(dque, &dque->state.pip->rasterization);

    if (dque->state.pip->hasMultisampling)
        _SglDqueSetMultisampleState(dque, &dque->state.pip->multisampling);

    if (dque->state.pip->hasDepthHandling)
        _SglDqueSetDepthState(dque, &dque->state.pip->depthHandling);

    if (dque->state.pip->hasColorBlending)
        _SglDqueSetColorBlendState(dque, &dque->state.pip->colorBlending);

    if (dque->state.pip->vpCnt)
        _SglDqueSetViewport(dque, 0, dque->state.pip->vpCnt, dque->state.pip->vps);

    if (dque->state.pip->scissorCnt)
        _SglDqueSetScissor(dque, 0, dque->state.pip->scissorCnt, dque->state.pip->scissors);

    _SglDqueBindAndSyncPip(dque, dque->state.pip);
}

_SGL void _SglDqueBindLegos(afxDrawQueue dque, afxNat base, afxNat cnt, afxLego lego[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > base);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= base + cnt);

    sglVmt const *gl = &dque->wglVmt;

    AfxAssert(dque->state.pip);
    afxPipelineRig pipr = AfxPipelineGetRig(dque->state.pip);
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    afxNat socketCnt = pipr->socketCnt;
    AfxAssert(base < socketCnt);
    AfxAssert(cnt <= socketCnt);
    AfxAssert(base + cnt <= socketCnt);
    AfxAssert(base + cnt <= _SGL_MAX_LEGO_PER_BIND);

    for (afxNat i = 0; i < cnt; i++)
    {
        dque->state.lego[base + i] = lego[i];
        _SglDqueBindAndSyncLego(dque, base + i, lego[i]);
    }
}

_SGL void _SglDqueBindIndexBuffer(afxDrawQueue dque, afxIndexBuffer ibuf, afxNat baseIdx)
{
    //_gfxRendererContextState *state = &dctx->state;
    //sglVmt *gl = dctx->activeOglVmt;

    dque->state.ibuf = ibuf;
    dque->state.baseIdx = baseIdx;

    _SglDqueBindAndSyncBuf(dque, 0, &ibuf->buf, 0, 0, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
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

_SGL void _SglDqueBindVertexBuffers(afxDrawQueue dque, afxNat base, afxNat cnt, afxVertexBuffer const vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_VBO_PER_BIND > base);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= cnt);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= base + cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        dque->state.vertexBindings[base + i].wild = FALSE;
        dque->state.vertexBindings[base + i].vbuf = vbuf[i];
        dque->state.vertexBindings[base + i].baseVtx = baseVtx[i];
        dque->state.vertexBindings[base + i].arr = vtxArr[i];
        dque->state.vertexBindings[base + i].siz = 0;
        dque->state.vertexBindings[base + i].stride = 0;

        afxNat offset, stride;

        if (vbuf[i])
        {
            offset = AfxVertexBufferGetOffset(vbuf[i], baseVtx[i], vtxArr[i]);
            stride = AfxVertexBufferGetStride(vbuf[i], vtxArr[i]);
        }
        else
        {
            offset = 0;
            stride = 0;
        }

        AfxAssert(stride);
        //stride = 0;
        _SglDqueBindAndSyncBuf(dque, base + i, vbuf[i] ? &(vbuf[i]->buf) : NIL, offset, stride, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    }
    dque->state.vertexBindingCnt = cnt;

    //_SglDquePushNextVao(dque);
}

_SGL void _SglDqueBindWildVertexBuffers(afxDrawQueue dque, afxNat base, afxNat cnt, afxBuffer const buf[], afxSize const offset[], afxSize const size[], afxSize const stride[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_VBO_PER_BIND > base);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= cnt);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= base + cnt);

    sglVmt const *gl = &dque->wglVmt;

    for (afxNat i = 0; i < cnt; i++)
    {
        dque->state.vertexBindings[base + i].wild = TRUE;
        dque->state.vertexBindings[base + i].buf = buf[i];
        dque->state.vertexBindings[base + i].offset = offset[i];
        dque->state.vertexBindings[base + i].arr = 0;
        dque->state.vertexBindings[base + i].siz = size[i];
        dque->state.vertexBindings[base + i].stride = stride[i];

        AfxAssert(stride[i]);
        _SglDqueBindAndSyncBuf(dque, base + i, buf[i], offset[i], stride[i], GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    }
    dque->state.vertexBindingCnt = cnt;

    //_SglDquePushNextVao(dque);
}

#if 0
_SGL void _SglDquePushNextVao(afxDrawQueue dque)
{
    afxError err = NIL;

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
                case AFX_VTX_FMT_X32:
                    glsiz = 1;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                case AFX_VTX_FMT_XY32:
                    glsiz = 2;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                case AFX_VTX_FMT_XYZ32:
                    glsiz = 3;
                    gltype = GL_FLOAT;
                    glStride = sizeof(afxReal) * glsiz;
                    AfxAssert(sizeof(afxReal) == sizeof(GLfloat));
                    break;
                case AFX_VTX_FMT_XYZW32:
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

_SGL void _SglDqueCopyTexture(afxDrawQueue dque, afxTexture dst, afxTexture src, afxNat rgnCnt, afxTextureRegion const rgn[])
{
    afxError err = NIL;
    sglVmt const* gl = &dque->wglVmt;

    // como fazer isso sem modificar o framebuffer ativo?
}

_SGL void _SglFlushVertexStateChanges(afxDrawQueue dque)
{
    
}

_SGL void _SglDqueDraw(afxDrawQueue dque, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst)
{
    afxError err = NIL;
    sglVmt const*gl = &dque->wglVmt;
    
    //_SglDqueUpdateNextStateObjects(dctx, queueIdx);
    //_SglDqueFlushNextState(dctx, queueIdx);

    AfxAssert(dque->state.pip);
    //AfxAssert(dque->state.vertexBindingCnt);

    AfxAssert(vtxCnt);
    AfxAssert(instCnt);
    GLenum top = AfxToGlTopology(dque->state.ia.topology);

    if (instCnt > 1)
    {
        gl->DrawArraysInstanced(top, firstVtx, vtxCnt, instCnt); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawArrays(top, firstVtx, vtxCnt); _SglThrowErrorOccuried();
    }
    dque->numOfFedVertices += vtxCnt;
    dque->numOfFedInstances += instCnt;

    //AfxEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _SglDqueDrawIndexed(afxDrawQueue dque, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst)
{
    afxError err = NIL;
    sglVmt const*gl = &dque->wglVmt;

    //_SglDqueUpdateNextStateObjects(dctx, queueIdx);
    //_SglDqueFlushNextState(dctx, queueIdx);

    AfxAssert(dque->state.pip);
    AfxAssert(dque->state.ibuf);
    AfxAssert(dque->state.vertexBindingCnt);

    AfxAssert(idxCnt);
    AfxAssert(instCnt);

    static const GLenum idxSizGl[] =
    {
        0,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_SHORT,
        0,
        GL_UNSIGNED_INT
    };

    afxSize idxSiz = AfxIndexBufferGetStride(dque->state.ibuf);

    GLint vtxOff2 = vtxOff;

    GLenum top = AfxToGlTopology(dque->state.ia.topology);

    if (instCnt > 1)
    {
        gl->DrawElementsInstancedBaseVertex(top, idxCnt, idxSizGl[idxSiz], (void const*)(idxSiz * (dque->state.baseIdx + firstIdx)), instCnt, vtxOff2); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawElementsBaseVertex(top, idxCnt, idxSizGl[idxSiz], (void const*)(idxSiz * (dque->state.baseIdx + firstIdx)), vtxOff2); _SglThrowErrorOccuried();
    }

    dque->numOfFedIndices += idxCnt;
    dque->numOfFedInstances += instCnt;
    //AfxEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);

}












// QUEUE STUFF

_SGL afxResult _SglDqueImplDrawWorkStreaming(afxDrawQueue dque, afxDrawWork const *dwrk)
{
    afxError err = NIL;
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
    afxError err = NIL;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    sglVmt const* gl = &dque->wglVmt;

    AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);

    // Update resources first to avoid anomalous state changes.

    afxNat i;
    union
    {
        _afxDscrCmd                         cmd;
        _afxDscrCmdBindIbuf                 bindIbuf;
        _afxDscrCmdBindVbuf                 bindVbuf;
        _afxDscrCmdBindLego                 bindLego;
        _afxDscrCmdBindPip                  bindPip;
        _afxDscrCmdBeginRenderPass          beginRenderPass;
        _afxDscrCmdBeginRendering           beginRendering;
        _afxDscrCmdEndRenderPass            endRenderPass;
        _afxDscrCmdEndRendering             endRendering;
        _afxDscrCmdDraw                     draw;
        _afxDscrCmdDrawIndexed              drawIndexed;
        _afxDscrCmdSetScissor               scissor;
        _afxDscrCmdSetVp                    vp;
        _afxDscrCmdSetRasterizerState       ras;
        _afxDscrCmdSetDepthState            depth;
        _afxDscrCmdSetInputAssemblyState    ia;
        _afxDscrCmdCopyTex                  copyTex;
    } *cmd;

    _afxDscrCmd *cmdHdr;
#if 0
    AfxChainForEveryLinkageB2F(&dscr->commands, _afxDscrCmd, script, cmdHdr)
    {
        cmd = (void*)cmdHdr;

        if (cmd->cmd.id == AFX_DSC_BEGIN) continue;
        else if (cmd->cmd.id == AFX_DSC_END)
            break;

        switch (cmd->cmd.id)
        {
        case AFX_DSC_BIND_INDEX_BUFFER:
        {
            AfxAssertObject(cmd->bindIbuf.ibuf, AFX_FCC_IBUF);
            _AfxStdBufUpdate(&(cmd->bindIbuf.ibuf->buf), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
            break;
        }
        case AFX_DSC_BIND_WILD_VERTEX_BUFFERS:
        {
            for (i = 0; i < cmd->bindVbuf.cnt; i++)
            {
                AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
                _AfxStdBufUpdate(&(cmd->bindVbuf.vbuf[i]->buf), GL_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
            }
            break;
        }
        case AFX_DSC_BIND_VERTEX_BUFFERS:
        {
            for (i = 0; i < cmd->bindVbuf.cnt; i++)
            {
                AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
                _AfxStdBufUpdate(&(cmd->bindVbuf.vbuf[i]->buf), GL_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
            }
            break;
        }
        case AFX_DSC_BIND_PIPELINE_PLUGS:
        {
            for (i = 0; i < cmd->bindLego.cnt; i++)
            {
                AfxAssertObject(cmd->bindLego.lego[i], AFX_FCC_LEGO);
                _AfxStdLegoUpdate(cmd->bindLego.lego[i], dque);
            }
            break;
        }
        case AFX_DSC_BIND_PIPELINE:
        {
            AfxAssertObject(cmd->bindPip.pip, AFX_FCC_PIP);
            _AfxStdPipUpdate(cmd->bindPip.pip, dque);
            break;
        }
        case AFX_DSC_BEGIN_RENDER_PASS:
        {
            AfxAssertObject(cmd->beginRenderPass.canv, AFX_FCC_CANV);
            _AfxStdCanvUpdate(cmd->beginRenderPass.canv, dque);
            break;
        }
        case AFX_DSC_BEGIN_RENDERING:
        {
            for (i = 0; i < cmd->beginRendering.rasterCnt; i++)
                _SglTexUpdate(&(cmd->beginRendering.rasters[i].surf->tex), dque);

            if (cmd->beginRendering.depth.surf)
                _SglTexUpdate(&(cmd->beginRendering.depth.surf->tex), dque);

            if (cmd->beginRendering.stencil.surf)
                _SglTexUpdate(&(cmd->beginRendering.stencil.surf->tex), dque);

            break;
        }
        case AFX_DSC_COPY_TEXTURE:
        {
            AfxAssertObject(cmd->copyTex.dst, AFX_FCC_TEX);
            AfxAssertObject(cmd->copyTex.src, AFX_FCC_TEX);
            _SglTexUpdate(cmd->copyTex.dst, dque);
            _SglTexUpdate(cmd->copyTex.src, dque);
        }
        default: break;
        }
    }
#endif
    // Do work

    AfxChainForEveryLinkageB2F(&dscr->commands, _afxDscrCmd, script, cmdHdr)
    {
        cmd = (void*)cmdHdr;

        if (cmd->cmd.id == AFX_DSC_BEGIN) continue;
        else if (cmd->cmd.id == AFX_DSC_END)
            break;

        switch (cmd->cmd.id)
        {
        case AFX_DSC_BIND_INDEX_BUFFER:
        {
            //AfxAssertObject(cmd->bindIbuf.ibuf, AFX_FCC_IBUF);
            _SglDqueBindIndexBuffer(dque, cmd->bindIbuf.ibuf, cmd->bindIbuf.baseIdx);
            break;
        }
        case AFX_DSC_BIND_WILD_VERTEX_BUFFERS:
        {
            //AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
            _SglDqueBindWildVertexBuffers(dque, cmd->bindVbuf.first, cmd->bindVbuf.cnt, cmd->bindVbuf.buf, cmd->bindVbuf.offset, cmd->bindVbuf.vtxArrSiz, cmd->bindVbuf.vtxStride);
            break;
        }
        case AFX_DSC_BIND_VERTEX_BUFFERS:
        {
            //AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
            _SglDqueBindVertexBuffers(dque, cmd->bindVbuf.first, cmd->bindVbuf.cnt, cmd->bindVbuf.vbuf, cmd->bindVbuf.baseVtx, cmd->bindVbuf.vtxArr);
            break;
        }
        case AFX_DSC_BIND_PIPELINE_PLUGS:
        {
            //AfxAssertObject(cmd->bindLego.lego[i], AFX_FCC_LEGO);
            _SglDqueBindLegos(dque, cmd->bindLego.first, cmd->bindLego.cnt, cmd->bindLego.lego);
            break;
        }
        case AFX_DSC_SET_INPUT_ASSEMBLY_STATE:
        {
            _SglDqueSetInputAssemblyState(dque, &cmd->ia.state);
            break;
        }
        case AFX_DSC_SET_RASTERIZATION_STATE:
        {
            _SglDqueSetRasterizerState(dque, &cmd->ras.state);
            break;
        }
        case AFX_DSC_SET_DEPTH_STENCIL_STATE:
        {
            _SglDqueSetDepthState(dque, &cmd->depth.state);
            break;
        }
        case AFX_DSC_BIND_PIPELINE:
        {
            //AfxAssertObject(cmd->bindPip.pip, AFX_FCC_PIP);
            _SglDqueBindPipeline(dque, cmd->bindPip.pip);
            break;
        }
        case AFX_DSC_BEGIN_RENDER_PASS:
        {
            //AfxAssertObject(cmd->beginRenderPass.canv, AFX_FCC_CANV);
            _SglDqueBeginRenderPass(dque, cmd->beginRenderPass.canv, &cmd->beginRenderPass.area, cmd->beginRenderPass.annexCnt, cmd->beginRenderPass.annexes);
            break;
        }
        case AFX_DSC_BEGIN_RENDERING:
        {
            //AfxAssertObject(cmd->beginRenderPass.canv, AFX_FCC_CANV);
            _SglDqueBeginRendering(dque, &cmd->beginRendering.area, cmd->beginRendering.layerCnt, cmd->beginRendering.rasterCnt, cmd->beginRendering.rasters, &cmd->beginRendering.depth, &cmd->beginRendering.stencil);
            break;
        }
        case AFX_DSC_END_RENDERING:
        {
            _SglDqueEndRendering(dque);
            break;
        }
        case AFX_DSC_END_RENDER_PASS:
        {
            _SglDqueEndRenderPass(dque);
            break;
        }
        case AFX_DSC_SET_VIEWPORT:
        {
            _SglDqueSetViewport(dque, cmd->vp.first, cmd->vp.cnt, cmd->vp.vp);
            break;
        }
        case AFX_DSC_SET_SCISSOR:
        {
            _SglDqueSetScissor(dque, cmd->scissor.first, cmd->scissor.cnt, cmd->scissor.rect);
            break;
        }
        case AFX_DSC_DRAW:
        {
            _SglDqueDraw(dque, cmd->draw.vtxCnt, cmd->draw.instCnt, cmd->draw.firstVtx, cmd->draw.firstInst);
            break;
        }
        case AFX_DSC_DRAW_INDEXED:
        {
            _SglDqueDrawIndexed(dque, cmd->drawIndexed.idxCnt, cmd->drawIndexed.instCnt, cmd->drawIndexed.firstIdx, cmd->drawIndexed.vtxOff, cmd->drawIndexed.firstInst);
            break;
        }
        case AFX_DSC_COPY_TEXTURE:
        {
            _SglDqueCopyTexture(dque, cmd->copyTex.dst, cmd->copyTex.src, cmd->copyTex.rgnCnt, cmd->copyTex.rgn);
            break;
        }
        default:
            AfxError("Command %i not handled", cmd->cmd.id);
            break;
        }
    }

    dscr->state = AFX_DSCR_STATE_EXECUTABLE;
    return err;
}

_SGL afxResult _SglDqueImplDrawWorkSubmission(afxDrawQueue dque, afxDrawWork const *dwrk)
{
    afxError err = NIL;
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
    afxError err = NIL;
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

        afxLegoPoint point = { 0 };
        point.resource.img.tex = &surf->tex;
        point.resource.img.smp = dque->presentSmp;
        AfxLegoUpdate(dque->presentLego, 1, &point);
        //_SglDqueBindAndSyncPip(dque->presentPip, dque);
        //_SglDqueBindAndSyncBuf(&(dque->presentVbuf->buf), 0, 0, 0, GL_ARRAY_BUFFER, GL_STATIC_DRAW, dque);
        //_SglDqueBindAndSyncLego(dque->presentLego, 0, dque);

#if !0
        
        //_AfxStdCanvUpdate(cmd->beginRenderPass.canv, dque);

        afxWhd extent;
        AfxTextureGetExtent(&surf->tex, 0, extent);
        afxRect area = { { { 0, 0 } }, { { extent[0], extent[1] } } };
        afxRenderPassAnnex fbufAnnexes[2] =
        {
            { TRUE, FALSE, {.color = { 0.6, 0.0, 0.3, 1 } } },
            { TRUE, FALSE, {.depth = 1.f,.stencil = 0 } },
        };
        _SglDqueBeginRenderPass(dque, NIL, &area, 1, fbufAnnexes);
#else
        afxDrawTarget const rasterRt = { NIL, AFX_SURFACE_LOAD_OP_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _SglDqueBeginRendering(dque, NIL, 1, 0, &rasterRt, NIL, NIL);
#endif   
        _SglDqueBindPipeline(dque, dque->presentPip);

        afxViewport const vp = { { 0, 0 }, { extent[0], extent[1] }, { 0, 1 } };
        _SglDqueSetViewport(dque, 0, 1, &vp);
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
        afxVertexBuffer vbos[] = { dque->presentVbuf };
        afxNat baseVtx[] = { 0 }, vtxArr[] = { 0 };
        _SglDqueBindVertexBuffers(dque, 0, 1, vbos, baseVtx, vtxArr);

        _SglDqueBindLegos(dque, 0, 1, &dque->presentLego);

        _SglDqueDraw(dque, 4, 1, 0, 0);

#if !0
        _SglDqueEndRenderPass(dque);
#else
        _SglDqueEndRendering(dque);
#endif
#endif
        //AfxLinkageDrop(&surf->swapchain);
        surf->state = AFX_SURF_STATE_IDLE;
        surf->tex.updFlags |= SGL_UPD_FLAG_HOST_FLUSH;

        // deadlocks all
        _SglSwapBuffers(dout->wglDc);

        dout->swapping = FALSE;
    }
    return err;
}

_SGL afxResult _SglDqueImplDrawWorkPresentation(afxDrawQueue dque, afxDrawWork const *dwrk)
{
    afxError err = NIL;
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

        AfxStringFormat(&(dout->title.str), "%i FPS, %d/%d/%d --- 4D Draw System (SIGGL %u.%u.%u) --- Public Test Build --- (c) 2017 SIGMA Technology Group", dque->fps, dque->numOfFedVertices, dque->numOfFedIndices, dque->numOfFedInstances,  dque->wglGlrcVerMajor, dque->wglGlrcVerMinor, dque->wglGlrcVerPatch);

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
    afxError err = NIL;
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
            case 6: // pipm
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
                    afxDrawWork const *dwrk;

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
    afxError err = NIL;
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
    afxError err = NIL;
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
            AfxStringMapConst(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
            AfxStringScan(&ver, "%u.%u.%u", &dque->wglGlrcVerMajor, &dque->wglGlrcVerMinor, &dque->wglGlrcVerPatch);
            gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(dque->wglGlrcVerMajor));
            gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(dque->wglGlrcVerMinor));

            _SglLoadVmt(gl, 4, 6);

            AfxStringUpdate(&dque->subsysName.str, 0, 0, (afxChar const*)gl->GetString(GL_RENDERER));
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

_SGL afxDrawQueue _AfxDrawContextAcquireQueue(afxDrawContext dctx, afxNat idx, afxBool autonomous)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxEntry("dctx=%p", dctx);
    afxDrawQueue dque = NIL;

    _afxDqueCtorArgs args =
    {
        dctx,
        idx,
        autonomous
    };

    if (!(dque = AfxObjectAcquire(AfxDrawContextGetQueueClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return dque;
};

_SGL afxError _AfxDqueDtor(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = NIL;
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
    return err;
}

_SGL afxError _AfxDqueCtor(afxDrawQueue dque, _afxDqueCtorArgs *args)
{
    AfxEntry("dque=%p", dque);
    afxError err = NIL;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    afxDrawContext dctx = AfxObjectGetProvider(&(dque->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxDrawQueueGetDriver(dque);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    AfxTransistorDeploy(&dque->deletionLock);
    AfxQueueDeploy(&dque->deletionQueue, sizeof(_sglDeleteGlRes), 32);
    AfxChainDeploy(&dque->instantiationList, dque);

    dque->queueIdx = args->idx;

    AfxString128(&dque->subsysName, NIL);

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
    AfxUriMapConstData(&uri, "data/pipeline/rgbaToRgbaYFlipped.pip.urd", 0);
    //AfxUriMapConstData(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.urd", 0);
    dque->presentPip = AfxDrawContextFetchPipeline(dctx, &uri);
    AfxAssertObject(dque->presentPip, AFX_FCC_PIP);

    afxSamplerSpecification smpSpec = { 0 };
    smpSpec.magFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.minFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.mipmapFilter = AFX_TEXEL_FLT_POINT;
    smpSpec.uvw[0] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[1] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[2] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit

    dque->presentSmp = AfxDrawContextAcquireSampler(dctx, &smpSpec);
    AfxAssertObject(dque->presentSmp, AFX_FCC_SMP);

    afxPipelineRig rig = AfxPipelineGetRig(dque->presentPip);
    AfxAssertObject(rig, AFX_FCC_PIPR);
    afxLegoPoint point;
    point.resource.img.smp = dque->presentSmp;
    point.resource.img.tex = NIL;
    dque->presentLego = AfxDrawContextAcquireLego(dctx, &rig->sockets[0], &point);
    AfxAssertObject(dque->presentLego, AFX_FCC_LEGO);

    afxString tmpStr;
    const afxV2d vtxPos[] =
    {
        { -1.0,  1.0 },
        { -1.0, -1.0 },
        {  1.0,  1.0 },
        {  1.0, -1.0 },
    };
    afxVertexDataSpecification presentVbufSpec[1];
    presentVbufSpec[0].fmt = AFX_VTX_FMT_XY32;
    presentVbufSpec[0].semantic = AfxStringMapConst(&tmpStr, "a_xy", 0);
    presentVbufSpec[0].src = vtxPos;
    presentVbufSpec[0].srcFmt = AFX_VTX_FMT_XY32;
    presentVbufSpec[0].usage = AFX_VTX_USAGE_POS;
    dque->presentVbuf = AfxDrawContextBuildVertexBuffer(dctx, 4, 1, presentVbufSpec);
    AfxAssertObject(dque->presentVbuf, AFX_FCC_VBUF);

    if (args->autonomous)
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
        AfxStringCopy(&dque->subsysName.str, &idd->subsysName.str);



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
    "Draw Queue",
    NIL
};
