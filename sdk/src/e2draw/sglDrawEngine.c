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

_SGL void _AfxDengDraw(afxDrawEngine deng, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst)
{
    afxError err = NIL;
    sglVmt const*gl = &deng->wglVmt;

    //_AfxDengUpdateNextStateObjects(dctx, queueIdx);
    //_AfxDengFlushNextState(dctx, queueIdx);

    AfxAssert(deng->state.pip);
    //AfxAssert(deng->state.vertexBindingCnt);

    AfxAssert(vtxCnt);
    AfxAssert(instCnt);
    GLenum top = AfxToGlTopology(deng->state.ia.topology);

    if (instCnt > 1)
    {
        gl->DrawArraysInstanced(top, firstVtx, vtxCnt, instCnt); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawArrays(top, firstVtx, vtxCnt); _SglThrowErrorOccuried();
    }
    deng->numOfFedVertices += vtxCnt;
    deng->numOfFedInstances += instCnt;

    //AfxEcho("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_SGL void _AfxDengDrawIndexed(afxDrawEngine deng, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst)
{
    afxError err = NIL;
    sglVmt const*gl = &deng->wglVmt;

    //_AfxDengUpdateNextStateObjects(dctx, queueIdx);
    //_AfxDengFlushNextState(dctx, queueIdx);

    AfxAssert(deng->state.pip);
    AfxAssert(deng->state.ibuf);
    AfxAssert(deng->state.vertexBindingCnt);

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

    afxSize idxSiz = AfxIndexBufferGetStride(deng->state.ibuf);

    GLint vtxOff2 = vtxOff;

    GLenum top = AfxToGlTopology(deng->state.ia.topology);

    if (instCnt > 1)
    {
        gl->DrawElementsInstancedBaseVertex(top, idxCnt, idxSizGl[idxSiz], (void const*)(idxSiz * firstIdx), instCnt, vtxOff2); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DrawElementsBaseVertex(top, idxCnt, idxSizGl[idxSiz], (void const*)(idxSiz * firstIdx), vtxOff2); _SglThrowErrorOccuried();
    }

    deng->numOfFedIndices += idxCnt;
    deng->numOfFedInstances += instCnt;
    //AfxEcho("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);

}

_SGL void _AfxDengSetInputStreams(afxDrawEngine deng, afxNat cnt, afxPipelineInputStream const streams[])
{
    afxError err = NIL;
    AfxAssert(cnt);
    AfxAssert(_SGL_MAX_INSTREAM_PER_SET >= cnt);

    sglVmt const*gl = &deng->wglVmt;

    deng->activeVaoIdx = (deng->activeVaoIdx + 1) % _SGL_MAX_VAO_PER_TIME;
#if !0
    if (!deng->vao[deng->activeVaoIdx])
    {
        gl->GenVertexArrays(1, &(deng->vao[deng->activeVaoIdx])); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DeleteVertexArrays(1, &(deng->vao[deng->activeVaoIdx])); _SglThrowErrorOccuried();
        gl->GenVertexArrays(1, &(deng->vao[deng->activeVaoIdx])); _SglThrowErrorOccuried();
    }
    gl->BindVertexArray(deng->vao[deng->activeVaoIdx]); _SglThrowErrorOccuried();
#endif
    for (afxNat i = 0; i < cnt; i++)
    {
        deng->state.inStreams[i] = streams[i];
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

        gl->EnableVertexAttribArray(streams[i].location); _SglThrowErrorOccuried();
        gl->VertexAttribFormat(streams[i].location, glsiz, gltype, FALSE, streams[i].offset); _SglThrowErrorOccuried();
        gl->VertexAttribBinding(streams[i].location, streams[i].binding); _SglThrowErrorOccuried();
    }
}

_SGL void _AfxDengSetScissor(afxDrawEngine deng, afxNat first, afxNat cnt, afxRect const rect[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= cnt);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET > first);
    AfxAssert(_SGL_MAX_SCISSOR_PER_SET >= first + cnt);
    AfxAssert(!cnt || (cnt && rect));

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxRectCopy(&(deng->state.scissorArea[first + i]), &rect[i]);
    }

    sglVmt const*gl = &deng->wglVmt;

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

            gl->ScissorArrayv(first, cnt, v); _SglThrowErrorOccuried();
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

_SGL void _AfxDengSetViewport(afxDrawEngine deng, afxNat first, afxNat cnt, afxViewport const vp[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_VP_PER_SET >= cnt);
    AfxAssert(_SGL_MAX_VP_PER_SET > first);
    AfxAssert(_SGL_MAX_VP_PER_SET >= first + cnt);
    AfxAssert(!cnt || (cnt && vp));

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxV2dCopy(deng->state.vp[first + i].offset, vp[i].offset);
        AfxV2dCopy(deng->state.vp[first + i].extent, vp[i].extent);
        AfxV2dCopy(deng->state.vp[first + i].depth,  vp[i].depth);
    }

    if (cnt)
    {
        sglVmt const*gl = &deng->wglVmt;

        if (gl->ViewportArrayv)
        {
            GLfloat v[_SGL_MAX_VP_PER_SET][4];
            AfxAssert(_SGL_MAX_VP_PER_SET >= cnt);

            for (afxNat i = 0; i < cnt; i++)
                v[i][0] = vp[i].offset[0],
                v[i][1] = vp[i].offset[1],
                v[i][2] = vp[i].extent[0], 
                v[i][3] = vp[i].extent[1];

            gl->ViewportArrayv(first, cnt, v); _SglThrowErrorOccuried();
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

            gl->DepthRangeArrayv(first, cnt, v); _SglThrowErrorOccuried();
        }
        else
        {
            GLdouble n = vp[0].depth[0];
            GLdouble f = vp[0].depth[1];

            gl->DepthRange(n, f); _SglThrowErrorOccuried();
        }
    }
}

_SGL void _AfxDengSetRasterizerState(afxDrawEngine deng, afxPipelineRasterizerState const *state)
{
    afxError err = NIL;
    AfxAssert(state);
    deng->state.raster = *state;

    sglVmt const *gl = &deng->wglVmt;

    if (deng->state.raster.depthClampEnable)
    {
        gl->Enable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
    }

    if (deng->state.raster.rasterizerDiscardEnable)
    {
        gl->Enable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_RASTERIZER_DISCARD); _SglThrowErrorOccuried();
    }

    gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlPolygonMode(deng->state.raster.fillMode)); _SglThrowErrorOccuried();

    if (deng->state.raster.cullMode)
    {
        gl->Enable(GL_CULL_FACE); _SglThrowErrorOccuried();
        gl->CullFace(AfxToGlCullMode(deng->state.raster.cullMode)); _SglThrowErrorOccuried();
        gl->FrontFace(AfxToGlFrontFace(deng->state.raster.frontFace)); _SglThrowErrorOccuried();
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

    if (deng->state.raster.depthBiasEnable)
    {
        gl->Enable(QwadroToGlPolygonModeBasedOffset[deng->state.raster.fillMode]); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(QwadroToGlPolygonModeBasedOffset[deng->state.raster.fillMode]); _SglThrowErrorOccuried();
    }

    gl->PolygonOffset(deng->state.raster.depthBiasSlopeFactor, deng->state.raster.depthBiasConstantFactor); _SglThrowErrorOccuried();

    gl->LineWidth(deng->state.raster.lineWidth); _SglThrowErrorOccuried();
}

_SGL void _AfxDengSetColorBlendState(afxDrawEngine deng, afxPipelineColorBlendState const *state)
{
    afxError err = NIL;
    AfxAssert(state);
    deng->state.colorBlend = *state;
    AfxAssert(NIL);
}

_SGL void _AfxDengSetMultisampleState(afxDrawEngine deng, afxPipelineMultisampleState const *state)
{
    afxError err = NIL;
    AfxAssert(state);
    deng->state.multisample = *state;

    sglVmt const *gl = &deng->wglVmt;

    if (deng->state.multisample.sampleShadingEnable)
    {
        gl->Enable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_MULTISAMPLE); _SglThrowErrorOccuried();
    }

    if (deng->state.multisample.pSampleMask)
    {
        gl->Enable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_MASK); _SglThrowErrorOccuried();
    }

    gl->SampleMaski(0, deng->state.multisample.pSampleMask[0]); _SglThrowErrorOccuried();
    
    if (deng->state.multisample.minSampleShading)
    {
        gl->Enable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_SHADING); _SglThrowErrorOccuried();
    }

    // gl v4.1?
    //gl->MinSampleShading(dctx->queue[queueIdx].data.nextAccumState.draw.multisample.minSampleShading); _SglThrowErrorOccuried();

    if (deng->state.multisample.alphaToOneEnable)
    {
        gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _SglThrowErrorOccuried();
    }

    if (deng->state.multisample.alphaToCoverageEnable)
    {
        gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _SglThrowErrorOccuried();
    }
}

_SGL void _AfxDengSetDepthState(afxDrawEngine deng, afxPipelineDepthState const *state)
{
    afxError err = NIL;
    AfxAssert(state);
    deng->state.depth = *state;

    sglVmt const *gl = &deng->wglVmt;

    if (deng->state.depth.depthTestEnable)
    {
        gl->Enable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
        gl->DepthFunc(SglToGlCompareOp(deng->state.depth.depthCompareOp)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_DEPTH_TEST); _SglThrowErrorOccuried();
    }

    if (deng->state.depth.depthWriteEnable)
    {
        gl->DepthMask(GL_TRUE); _SglThrowErrorOccuried();
    }
    else
    {
        gl->DepthMask(GL_FALSE); _SglThrowErrorOccuried();
    }

    if (deng->state.depth.stencilTestEnable)
    {
        gl->Enable(GL_STENCIL_TEST); _SglThrowErrorOccuried();

        gl->StencilFuncSeparate(GL_FRONT, SglToGlCompareOp(deng->state.depth.stencilOpFront.compareOp), 0, 1); _SglThrowErrorOccuried();
        gl->StencilOpSeparate(GL_FRONT, AfxToGlStencilOp(deng->state.depth.stencilOpFront.failOp), AfxToGlStencilOp(deng->state.depth.stencilOpFront.depthFailOp), AfxToGlStencilOp(deng->state.depth.stencilOpFront.passOp)); _SglThrowErrorOccuried();
        gl->StencilFuncSeparate(GL_BACK, SglToGlCompareOp(deng->state.depth.stencilOpBack.compareOp), 0, 1); _SglThrowErrorOccuried();
        gl->StencilOpSeparate(GL_BACK, AfxToGlStencilOp(deng->state.depth.stencilOpBack.failOp), AfxToGlStencilOp(deng->state.depth.stencilOpBack.depthFailOp), AfxToGlStencilOp(deng->state.depth.stencilOpBack.passOp)); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_STENCIL_TEST); _SglThrowErrorOccuried();
    }
}

_SGL void _AfxDengSetInputAssemblyState(afxDrawEngine deng, afxPipelineInputAssemblyState const *state)
{
    afxError err = NIL;
    AfxAssert(state);
    deng->state.ia = *state;

    sglVmt const *gl = &deng->wglVmt;

    if (deng->state.ia.primRestartEnable)
    {
        gl->Enable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
    }
    else
    {
        gl->Disable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
    }
}

_SGL void _AfxDengBindIndexBuffer(afxDrawEngine deng, afxIndexBuffer ibuf, afxNat baseIdx)
{
    //_gfxRendererContextState *state = &dctx->state;
    //sglVmt *gl = dctx->activeOglVmt;

    deng->state.ibuf = ibuf;
    deng->state.baseIdx = baseIdx;

    sglVmt const *gl = &deng->wglVmt;

    gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, deng->state.ibuf->buf.gpuHandle); _SglThrowErrorOccuried();
}

_SGL void _AfxDengBindLegos(afxDrawEngine deng, afxNat base, afxNat cnt, afxLego lego[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_LEGO_PER_BIND > base);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= cnt);
    AfxAssert(_SGL_MAX_LEGO_PER_BIND >= base + cnt);

    sglVmt const *gl = &deng->wglVmt;

    AfxAssert(deng->state.pip);
    afxPipelineRig pipr = AfxPipelineGetRig(deng->state.pip);
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    afxNat socketCnt = pipr->socketCnt;
    AfxAssert(base < socketCnt);
    AfxAssert(cnt <= socketCnt);
    AfxAssert(base + cnt <= socketCnt);
    AfxAssert(base + cnt <= _SGL_MAX_LEGO_PER_BIND);

    for (afxNat i = 0; i < cnt; i++)
    {
        deng->state.lego[base + i] = lego[i];

        afxLegoSchema const *schema = &(deng->state.lego[base + i]->schema);
        afxLegoSchema const *schema2 = &(pipr->sockets[i]);
        AfxAssert(schema->entryCnt == schema2->entryCnt);

        for (afxNat j = 0; j < schema->entryCnt; j++)
        {
            afxLegoSchemaEntry const *entry = &schema->entries[j];
            afxLegoSchemaEntry const *entry2 = &schema2->entries[j];
            AfxAssert(entry->binding == entry2->binding);
            AfxAssert(entry->visibility == entry2->visibility);
            AfxAssert(entry->type == entry2->type);

            afxLegoPoint const *point = &deng->state.lego[base + i]->points[j];
            
            afxNat setId = (base * _SGL_MAX_ENTRY_PER_LEGO) + (i * _SGL_MAX_ENTRY_PER_LEGO);
            afxNat binding = setId + entry->binding;

            switch (entry->type)
            {
            case AFX_SUPPLY_TYPE_SAMPLER:
            {
                AfxAssertObject(point->resource.img.smp, AFX_FCC_SMP);
                gl->BindSampler(binding, point->resource.img.smp->gpuHandle); _SglThrowErrorOccuried();
                break;
            }
            case AFX_SUPPLY_TYPE_SAMPLED_IMAGE:
            {
                if (gl->BindTextureUnit)
                {
                    AfxAssertObject(point->resource.img.tex, AFX_FCC_TEX);
                    gl->BindTextureUnit(binding, point->resource.img.tex->gpuTexHandle); _SglThrowErrorOccuried();
                }
                else
                {
                    AfxAssertObject(point->resource.img.tex, AFX_FCC_TEX);
                    gl->ActiveTexture(GL_TEXTURE0 + binding); _SglThrowErrorOccuried();
                    gl->BindTexture(point->resource.img.tex->glTarget, point->resource.img.tex->gpuTexHandle); _SglThrowErrorOccuried();
                }
                break;
            }
            case AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER:
            {
                if (gl->BindTextureUnit)
                {
                    AfxAssertObject(point->resource.img.tex, AFX_FCC_TEX);
                    gl->BindTextureUnit(binding, point->resource.img.tex->gpuTexHandle); _SglThrowErrorOccuried();
                }
                else
                {
                    AfxAssertObject(point->resource.img.tex, AFX_FCC_TEX);
                    gl->ActiveTexture(GL_TEXTURE0 + binding); _SglThrowErrorOccuried();
                    gl->BindTexture(point->resource.img.tex->glTarget, point->resource.img.tex->gpuTexHandle); _SglThrowErrorOccuried();
                }
                
                AfxAssertObject(point->resource.img.smp, AFX_FCC_SMP);
                gl->BindSampler(binding, point->resource.img.smp->gpuHandle); _SglThrowErrorOccuried();
#if 0
                afxUri128 uri;
                AfxUri128(&uri, NIL);
                AfxTextureDownload(point->resource.img.tex, AfxUriFormat(&uri.uri, "system/tex-%u-%u.tga", i, entry->binding));
#endif
                break;
            }
            case AFX_SUPPLY_TYPE_CONSTANT_BUFFER:
            {
                AfxAssertObject(point->resource.data.buf, AFX_FCC_BUF);

                // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

                //loc = gl->GetUniformBlockIndex(deng->state.pip->gpuHandle[deng->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
                //gl->UniformBlockBinding(deng->state.pip->gpuHandle[deng->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));
                gl->BindBufferRange(GL_UNIFORM_BUFFER, binding, point->resource.data.buf->gpuHandle, point->resource.data.base, point->resource.data.range); _SglThrowErrorOccuried();

                break;
            }
            default:
            {
                AfxError("");
            }
            }
        }
    }
}

_SGL void _AfxDengPushNextVao(afxDrawEngine deng)
{
    afxError err = NIL;

    sglVmt const* gl = &deng->wglVmt;

    {
        if (_SGL_MAX_VAO_PER_TIME > deng->activeVaoIdx)
            ++deng->activeVaoIdx;
        else
            deng->activeVaoIdx = 0;

        if (deng->vao[deng->activeVaoIdx])
        {
            gl->DeleteVertexArrays(1, &(deng->vao[deng->activeVaoIdx])); _SglThrowErrorOccuried();
        }
        gl->GenVertexArrays(1, &(deng->vao[deng->activeVaoIdx])); _SglThrowErrorOccuried();
        gl->BindVertexArray(deng->vao[deng->activeVaoIdx]); _SglThrowErrorOccuried();
        AfxAssert(gl->IsVertexArray(deng->vao[deng->activeVaoIdx]));

        afxPipeline pip = deng->state.pip;
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

                if (FALSE == deng->state.vertexBindings[binding].wild)
                {
                    afxVertexBuffer vbuf = deng->state.vertexBindings[binding].vbuf;
                    afxNat baseVtx = deng->state.vertexBindings[binding].baseVtx;
                    afxNat vtxArr = deng->state.vertexBindings[binding].arr;
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
                    afxBuffer buf = deng->state.vertexBindings[binding].buf;
                    offset = deng->state.vertexBindings[binding].offset;
                    stride = deng->state.vertexBindings[binding].stride;

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

_SGL void _AfxDengBindVertexBuffers(afxDrawEngine deng, afxNat base, afxNat cnt, afxVertexBuffer const vbuf[], afxNat const baseVtx[], afxNat const vtxArr[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_VBO_PER_BIND > base);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= cnt);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= base + cnt);

    sglVmt const *gl = &deng->wglVmt;

    for (afxNat i = 0; i < cnt; i++)
    {
        deng->state.vertexBindings[base + i].wild = FALSE;
        deng->state.vertexBindings[base + i].vbuf = vbuf[i];
        deng->state.vertexBindings[base + i].baseVtx = baseVtx[i];
        deng->state.vertexBindings[base + i].arr = vtxArr[i];
        deng->state.vertexBindings[base + i].siz = 0;
        deng->state.vertexBindings[base + i].stride = 0;
    
        if (gl->VertexAttribFormat)
        {
            afxNat offset, stride;
            GLuint glHandle;

            if (vbuf[i])
            {
                offset = AfxVertexBufferGetOffset(vbuf[i], baseVtx[i], vtxArr[i]);
                stride = AfxVertexBufferGetStride(vbuf[i], vtxArr[i]);
                glHandle = vbuf[i]->buf.gpuHandle;
            }
            else
            {
                offset = 0;
                stride = 0;
                glHandle = 0;
            }

            AfxAssert(stride);
            gl->BindVertexBuffer(base + i, glHandle, offset, stride); _SglThrowErrorOccuried();
            // gl->VertexAttribBinding(base + i, );
            //gl->VertexAttribFormat();
        }
        else
        {
            AfxThrowError();
        }
    }
    deng->state.vertexBindingCnt = cnt;

    //_AfxDengPushNextVao(deng);
}

_SGL void _AfxDengBindWildVertexBuffers(afxDrawEngine deng, afxNat base, afxNat cnt, afxBuffer const buf[], afxSize const offset[], afxSize const size[], afxSize const stride[])
{
    afxError err = NIL;
    AfxAssert(_SGL_MAX_VBO_PER_BIND > base);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= cnt);
    AfxAssert(_SGL_MAX_VBO_PER_BIND >= base + cnt);

    sglVmt const *gl = &deng->wglVmt;
    
    for (afxNat i = 0; i < cnt; i++)
    {
        deng->state.vertexBindings[base + i].wild = TRUE;
        deng->state.vertexBindings[base + i].buf = buf[i];
        deng->state.vertexBindings[base + i].offset = offset[i];
        deng->state.vertexBindings[base + i].arr = 0;
        deng->state.vertexBindings[base + i].siz = size[i];
        deng->state.vertexBindings[base + i].stride = stride[i];

        if (gl->VertexAttribFormat)
        {
            GLuint glHandle = buf[i] ? buf[i]->gpuHandle : 0;

            AfxAssert(stride[i]);
            gl->BindVertexBuffer(base + i, glHandle, offset[i], stride[i]); _SglThrowErrorOccuried();
            // gl->VertexAttribBinding(base + i, );
            //gl->VertexAttribFormat();
        }
        else
        {
            AfxThrowError();
        }
    }
    deng->state.vertexBindingCnt = cnt;

    //_AfxDengPushNextVao(deng);
}

_SGL void _AfxDengEndRendering(afxDrawEngine deng)
{
    afxError err = NIL;

    sglVmt const *gl = &deng->wglVmt;
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();
    
    AfxAssert(deng->tmpFbo[deng->activeFboIdx]);

    for (afxNat i = 0; i < deng->state.renderPass.rasterCnt; i++)
    {
        afxRenderTarget const *raster = &deng->state.renderPass.rasters[i];
        afxSurface surf = raster->surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER));
        AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
        surf->state = AFX_SURF_STATE_PRESENTABLE;
        surf->tex.lastDevUpdTime = deng->lastSubmisBeginTime;
        
        // AFX_SURFACE_STORE_OP_STORE
#if 0
        gl->BindTexture(surf->tex.glTarget, surf->tex.gpuTexHandle);
        gl->GetTexImage(surf->tex.glTarget, 0, surf->tex.glFmt, surf->tex.glType, (void*)AfxTextureGetData(&surf->tex, NIL, 0));
        gl->BindTexture(surf->tex.glTarget, 0);
#endif
    }

    if (deng->state.renderPass.depthRt.surf)
    {
        afxSurface surf = deng->state.renderPass.depthRt.surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
        AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
        surf->state = AFX_SURF_STATE_PRESENTABLE;
        surf->tex.lastDevUpdTime = deng->lastSubmisBeginTime;
    }

    if (deng->state.renderPass.stencilRt.surf)
    {
        afxSurface surf = deng->state.renderPass.stencilRt.surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
        AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
        surf->state = AFX_SURF_STATE_PRESENTABLE;
        surf->tex.lastDevUpdTime = deng->lastSubmisBeginTime;
    }
};

_SGL void _AfxDengBeginRendering(afxDrawEngine deng, afxRect const *area, afxNat layerCnt, afxNat rasterCnt, afxRenderTarget const rasters[], afxRenderTarget const *depth, afxRenderTarget const *stencil)
{
    afxError err = NIL;
    //_gfxRendererContextState *state = &dctx->state;

    deng->state.renderPass.annexCnt = 0;
    deng->state.renderPass.canv = NIL;
    
    if (area)
        AfxRectCopy(&deng->state.renderPass.area, area);

    deng->state.renderPass.layerCnt = layerCnt;
    deng->state.renderPass.rasterCnt = rasterCnt;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= rasterCnt);
    AfxCopy(deng->state.renderPass.rasters, rasters, sizeof(rasters[0]) * rasterCnt);
    
    if (depth)
        deng->state.renderPass.depthRt = *depth;

    if (stencil)
        deng->state.renderPass.stencilRt = *stencil;

    sglVmt const *gl = &deng->wglVmt;

    if (_SGL_MAX_SWAPCHAIN_CAP > deng->activeFboIdx)
        ++deng->activeFboIdx;
    else
        deng->activeFboIdx = 0;

    if (deng->tmpFbo[deng->activeFboIdx])
    {
        gl->DeleteFramebuffers(1, &(deng->tmpFbo[deng->activeFboIdx])); _SglThrowErrorOccuried();
    }
    gl->GenFramebuffers(1, &(deng->tmpFbo[deng->activeFboIdx])); _SglThrowErrorOccuried();
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, deng->tmpFbo[deng->activeFboIdx]); _SglThrowErrorOccuried();
    AfxAssert(gl->IsFramebuffer(deng->tmpFbo[deng->activeFboIdx]));

    afxNat colorAttchBaseIdx = 0;

    for (afxNat i = 0; i < deng->state.renderPass.rasterCnt; i++)
    {
        afxRenderTarget const *raster = &deng->state.renderPass.rasters[i];
        afxSurface surf = raster->surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER));
        AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
        AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
        surf->state = AFX_SURF_STATE_PAINTING;
        
        gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx, GL_RENDERBUFFER, surf->gpuRboHandle);
        gl->FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx, surf->tex.glTarget, surf->tex.gpuTexHandle, 0);

        if (raster->loadOp == AFX_SURFACE_LOAD_OP_CLEAR)
        {
            afxReal const *color = raster->clearValue.color;
            
            gl->DrawBuffer(GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx); _SglThrowErrorOccuried();
            colorAttchBaseIdx++;
            gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
            gl->Clear(GL_COLOR_BUFFER_BIT); _SglThrowErrorOccuried();
        }
    }

    if (depth && depth->surf)
    {
        afxSurface surf = deng->state.renderPass.depthRt.surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
        AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
        AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
        surf->state = AFX_SURF_STATE_PAINTING;

        gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, surf->gpuRboHandle);
        gl->FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, surf->tex.glTarget, surf->tex.gpuTexHandle, 0);

        gl->ClearDepth(deng->state.renderPass.depthRt.clearValue.depth); _SglThrowErrorOccuried();
        gl->Clear(GL_DEPTH_BUFFER_BIT); _SglThrowErrorOccuried();
    }

    if (stencil && stencil->surf)
    {
        afxSurface surf = deng->state.renderPass.stencilRt.surf;
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH));
        AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
        AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
        surf->state = AFX_SURF_STATE_PAINTING;

        gl->FramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, surf->gpuRboHandle);
        gl->FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, surf->tex.glTarget, surf->tex.gpuTexHandle, 0);

        gl->ClearStencil(deng->state.renderPass.stencilRt.clearValue.stencil); _SglThrowErrorOccuried();
        gl->Clear(GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
    }

    if (GL_FRAMEBUFFER_COMPLETE != gl->CheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
        AfxThrowError();
}

_SGL void _AfxDengBeginRenderPass(afxDrawEngine deng, afxCanvas canv, afxRect const *area, afxNat annexCnt, afxRenderPassAnnex const annexes[])
{
    afxError err = NIL;
    //_gfxRendererContextState *state = &dctx->state;
    sglVmt const *gl = &deng->wglVmt;

    deng->state.renderPass.canv = canv;
    AfxAssert(_SGL_MAX_SURF_PER_CANV >= annexCnt);
    deng->state.renderPass.annexCnt = annexCnt;

    if (deng->state.renderPass.annexCnt)
    {
        AfxAssert(annexes);
        AfxCopy(deng->state.renderPass.annexes, annexes, sizeof(annexes[0]) * annexCnt);
    }

    deng->state.renderPass.layerCnt = 0;
    deng->state.renderPass.depthRt.surf = NIL;
    deng->state.renderPass.stencilRt.surf = NIL;
    deng->state.renderPass.rasters[0].surf = NIL;
    deng->state.renderPass.rasterCnt = 0;

    if (!deng->state.renderPass.canv)
    {
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        
        if (deng->state.renderPass.annexCnt)
        {
            afxReal *color = deng->state.renderPass.annexes[0].clearValue.color;
            gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
            gl->ClearDepth(1); _SglThrowErrorOccuried();
            gl->ClearStencil(0); _SglThrowErrorOccuried();
            gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, deng->state.renderPass.canv->gpuHandle[deng->queueIdx]); _SglThrowErrorOccuried();
        AfxAssert(AfxCanvasGetSurfaceCount(deng->state.renderPass.canv) == deng->state.renderPass.annexCnt);
        afxNat colorAttchBaseIdx = 0;

        for (afxNat i = 0; i < deng->state.renderPass.annexCnt; i++)
        {
            afxSurface surf = AfxCanvasGetSurface(deng->state.renderPass.canv, i);
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state != AFX_SURF_STATE_PAINTING);
            AfxAssert(surf->state != AFX_SURF_STATE_PENDING);
            surf->state = AFX_SURF_STATE_PAINTING;

            if (deng->state.renderPass.annexes[i].clearOnLoad)
            {
                GLbitfield bufferBits = NIL;

                if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER))
                {
                    afxReal *color = deng->state.renderPass.annexes[0].clearValue.color;
                    bufferBits |= GL_COLOR_BUFFER_BIT;
                    gl->DrawBuffer(GL_COLOR_ATTACHMENT0 + colorAttchBaseIdx); _SglThrowErrorOccuried();
                    colorAttchBaseIdx++;
                    gl->ClearColor(color[0], color[1], color[2], color[3]); _SglThrowErrorOccuried();
                }

                if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH))
                {
                    bufferBits |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
                    gl->ClearDepth(deng->state.renderPass.annexes[i].clearValue.depth); _SglThrowErrorOccuried();
                    gl->ClearStencil(deng->state.renderPass.annexes[i].clearValue.stencil); _SglThrowErrorOccuried();
                }

                gl->Clear(bufferBits); _SglThrowErrorOccuried();
            }
        }
    }
}

_SGL void _AfxDengEndRenderPass(afxDrawEngine deng)
{
    afxError err = NIL;

    sglVmt const*gl = &deng->wglVmt;

    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
    //gl->Flush(); _SglThrowErrorOccuried();
    //gl->Finish(); _SglThrowErrorOccuried();

    if (deng->state.renderPass.canv)
    {
        for (afxNat i = 0; i < deng->state.renderPass.annexCnt; i++)
        {
            afxSurface surf = AfxCanvasGetSurface(deng->state.renderPass.canv, i);
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.lastDevUpdTime = deng->lastSubmisBeginTime;
        }
    }
    else
    {
        for (afxNat i = 0; i < deng->state.renderPass.rasterCnt; i++)
        {
            afxRenderTarget const *raster = &deng->state.renderPass.rasters[i];
            afxSurface surf = raster->surf;
            AfxAssertObject(surf, AFX_FCC_SURF);
            AfxAssert(surf->state == AFX_SURF_STATE_PAINTING);
            surf->state = AFX_SURF_STATE_PRESENTABLE;
            surf->tex.lastDevUpdTime = deng->lastSubmisBeginTime;
        }
    }
}

_SGL void _AfxDengCopyTexture(afxDrawEngine deng, afxTexture dst, afxTexture src, afxNat rgnCnt, afxTextureRegion const rgn[])
{
    afxError err = NIL;
    sglVmt const* gl = &deng->wglVmt;
    
    // como fazer isso sem modificar o framebuffer ativo?
}

_SGL void _AfxDengBindPipeline(afxDrawEngine deng, afxPipeline pip)
{
    afxError err = NIL;

    deng->state.pip = pip;

    sglVmt const* gl = &deng->wglVmt;
#if 0
    if (pip->inBindingCnt)
    {
        _AfxDengSetInputSlots(deng, pip->inBindingCnt, pip->inBindings);
    }
#endif
    if (deng->state.pip->inStreamCnt)
    {
        _AfxDengSetInputStreams(deng, deng->state.pip->inStreamCnt, deng->state.pip->inStreams);
    }

    if (deng->state.pip->hasVtxInAssembling)
    {
        _AfxDengSetInputAssemblyState(deng, &deng->state.pip->vtxInAssembling);
    }

    if (deng->state.pip->hasRasterization)
    {
        _AfxDengSetRasterizerState(deng, &deng->state.pip->rasterization);
    }

    if (deng->state.pip->hasMultisampling)
    {
        _AfxDengSetMultisampleState(deng, &deng->state.pip->multisampling);
    }

    if (deng->state.pip->hasDepthHandling)
    {
        _AfxDengSetDepthState(deng, &deng->state.pip->depthHandling);
    }

    if (deng->state.pip->hasColorBlending)
    {
        _AfxDengSetColorBlendState(deng, &deng->state.pip->colorBlending);
    }

    if (deng->state.pip->vpCnt)
    {
        _AfxDengSetViewport(deng, 0, deng->state.pip->vpCnt, deng->state.pip->vps);
    }

    if (deng->state.pip->scissorCnt)
    {
        _AfxDengSetScissor(deng, 0, deng->state.pip->scissorCnt, deng->state.pip->scissors);
    }

    gl->UseProgram(deng->state.pip->gpuHandle[deng->queueIdx]); _SglThrowErrorOccuried();

    afxPipelineRig pipr = AfxPipelineGetRig(deng->state.pip);
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    afxNat setCnt = pipr->socketCnt;
    
    for (afxNat i = 0; i < setCnt; i++)
    {
        afxLegoSchema *schema = &pipr->sockets[i];

        for (afxNat j = 0; j < schema->entryCnt; j++)
        {
            afxLegoSchemaEntry const *entry = &schema->entries[j];
            AfxAssert(!AfxStringIsEmpty(&entry->name.str));
            AfxAssert(entry->type);
            //AfxAssert(entry->visibility);
            //AfxAssert(entry->cnt);

            afxNat setId = (i * _SGL_MAX_ENTRY_PER_LEGO);
            afxNat binding = setId + entry->binding;

            afxNat loc = gl->GetUniformLocation(deng->state.pip->gpuHandle[deng->queueIdx], (void const *)AfxStringGetDataConst(&entry->name.str)); _SglThrowErrorOccuried();

            switch (entry->type)
            {
            case AFX_SUPPLY_TYPE_SAMPLER:
            {
                gl->Uniform1i(loc, binding); _SglThrowErrorOccuried();
                break;
            }
            case AFX_SUPPLY_TYPE_SAMPLED_IMAGE:
            {
                gl->Uniform1i(loc, binding); _SglThrowErrorOccuried();
                break;
            }
            case AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER:
            {
                gl->Uniform1i(loc, binding); _SglThrowErrorOccuried();
                break;
            }
            case AFX_SUPPLY_TYPE_CONSTANT_BUFFER:
            {
                // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

                //loc = gl->GetUniformBlockIndex(deng->state.pip->gpuHandle[deng->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
                //gl->UniformBlockBinding(deng->state.pip->gpuHandle[deng->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));

                gl->UniformBlockBinding(deng->state.pip->gpuHandle[deng->queueIdx], gl->GetUniformBlockIndex(deng->state.pip->gpuHandle[deng->queueIdx], (void const *)AfxStringGetDataConst(&entry->name.str)), binding);

                break;
            }
            default:
            {
                AfxError("");
            }
            }
        }
    }
}

_SGL afxError _AfxStdLegoUpdate(afxLego lego, afxDrawEngine deng)
{
    //AfxEntry("pip=%p", pip);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    sglVmt const* gl = &deng->wglVmt;

    for (afxNat i = 0; i < lego->schema.entryCnt; i++)
    {
        afxLegoSchemaEntry *entry = &lego->schema.entries[i];

        switch (entry->type)
        {
        case AFX_SUPPLY_TYPE_SAMPLER:
        {
            _AfxStdSmpUpdate(lego->points[i].resource.img.smp, deng);
            break;
        }
        case AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            if (lego->points[i].resource.img.smp)
                _AfxStdSmpUpdate(lego->points[i].resource.img.smp, deng);

            if (lego->points[i].resource.img.tex)
                _SglTexUpdate(lego->points[i].resource.img.tex, deng);
            break;
        }
        case AFX_SUPPLY_TYPE_SAMPLED_IMAGE:
        {
            _SglTexUpdate(lego->points[i].resource.img.tex, deng);
            break;
        }
        case AFX_SUPPLY_TYPE_CONSTANT_BUFFER:
        {
            _AfxStdBufUpdate(lego->points[i].resource.data.buf, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, deng);
            break;
        }
        default:
            AfxError("");
        }
    }
    return err;
}

_SGL afxError _AfxDengPresentSurface(afxDrawEngine deng, afxDrawOutput dout, afxNat outBufIdx)
{
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);
    AfxAssertObject(dout, AFX_FCC_DOUT);
    sglVmt const* gl = &deng->wglVmt;

    AfxAssert(!dout->swapping);

    {
        dout->swapping = TRUE;
        
        afxSurface surf = dout->buffers[outBufIdx];
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(surf->state == AFX_SURF_STATE_PENDING);
#if 0
        if (!deng->presentFboGpuHandle)
        {
            gl->GenFramebuffers(1, &(deng->presentFboGpuHandle)); _SglThrowErrorOccuried();
        }
        gl->BindFramebuffer(GL_FRAMEBUFFER, deng->presentFboGpuHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(deng->presentFboGpuHandle));
        gl->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, surf->tex.glTarget, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();

        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->BlitFramebuffer(0, 0, surf->tex.whd[0], surf->tex.whd[1], 0, 0, surf->tex.whd[0], surf->tex.whd[1], GL_COLOR_BUFFER_BIT, GL_NEAREST); _SglThrowErrorOccuried();
        gl->BindFramebuffer(GL_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
        gl->Flush();
        //gl->Finish();
        //gl->DeleteFramebuffers(1, &(gpuHandle)); _SglThrowErrorOccuried();
#else
        //if (dout->presentMode == AFX_PRESENT_MODE_FIFO)
            //surf = AfxContainerOf(AfxChainGetLastLinkage(&dout->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else //if (dout->presentMode == AFX_PRESENT_MODE_MAILBOX)
            //surf = AfxContainerOf(AfxChainGetFirstLinkage(&dout->swapchain), AFX_OBJECT(afxSurface), swapchain);
        //else AfxError("Not implemented yet.");

        afxLegoPoint point = { 0 };
        point.resource.img.tex = &surf->tex;
        point.resource.img.smp = deng->presentSmp;
        AfxLegoUpdate(deng->presentLego, 1, &point);
        _AfxStdPipUpdate(deng->presentPip, deng);
        _AfxStdBufUpdate(&(deng->presentVbuf->buf), GL_ARRAY_BUFFER, GL_STATIC_DRAW, deng);
        _AfxStdLegoUpdate(deng->presentLego, deng);

#if !0
        afxRenderPassAnnex fbufAnnexes[2] =
        {
            { TRUE, FALSE, {.color = { AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), 1 } } },
            { TRUE, FALSE, {.depth = 1.f,.stencil = 0 } },
        };
        //_AfxStdCanvUpdate(cmd->beginRenderPass.canv, deng);
        _AfxDengBeginRenderPass(deng, NIL, 0, 0, fbufAnnexes);
#else
        afxRenderTarget const rasterRt = { NIL, AFX_SURFACE_LOAD_OP_CLEAR, NIL, { 0.3, 0.1, 0.3, 1.0 } };
        _AfxDengBeginRendering(deng, NIL, 1, 0, &rasterRt, NIL, NIL);
#endif   
        _AfxDengBindPipeline(deng, deng->presentPip);

        afxWhd extent;
        AfxTextureGetExtent(&surf->tex, 0, extent);
        afxViewport const vp = { { 0, 0 }, { extent[0], extent[1] }, { 0, 1 } };
        _AfxDengSetViewport(deng, 0, 1, &vp);
#if 0 // already set by pipeline
        afxPipelineRasterizerState const ras = { FALSE, FALSE, AFX_FILL_MODE_SOLID, AFX_CULL_MODE_BACK, AFX_FRONT_FACE_CCW, FALSE, 0, 0, 0, 1 };
        _AfxDengSetRasterizerState(deng, &ras);

        afxPipelineDepthState const depth = { 0 };
        _AfxDengSetDepthState(deng, &depth);
#endif

#if 0 // already set by pipeline
        afxPipelineInputAssemblyState ia;
        ia.topology = AFX_PRIM_TOPOLOGY_TRI_STRIP;
        ia.primRestartEnable = FALSE;
        _AfxDengSetInputAssemblyState(deng, &ia);
#endif
#if 0
        afxNat const baseVtxs[] = { 0, 0 };
        afxNat const vtxArrs[] = { 0, 1 };
        afxVertexBuffer vbufs[] = { deng->rgbRectVbo, deng->rgbRectVbo };
        _AfxDengBindVertexBuffers(deng, 0, 2, vbufs, baseVtxs, vtxArrs);
#endif
        afxNat baseVtx[] = { 0 }, vtxArr[] = { 0 };
        _AfxDengBindVertexBuffers(deng, 0, 1, &deng->presentVbuf, baseVtx, vtxArr);

        _AfxDengBindLegos(deng, 0, 1, &deng->presentLego);

        _AfxDengDraw(deng, 4, 1, 0, 0);

#if !0
        _AfxDengEndRenderPass(deng);
#else
        _AfxDengEndRendering(deng);
#endif
#endif
        //AfxLinkageDrop(&surf->swapchain);
        surf->state = AFX_SURF_STATE_IDLE;
        surf->tex.lastDevUpdTime = deng->lastPresentBeginTime;

        // deadlocks all
        _SglSwapBuffersNow(dout);

        dout->swapping = FALSE;
    }
    return err;
}

_SGL afxError _AfxDengExecuteDscr(afxDrawEngine deng, afxDrawScript dscr)
{
    //AfxEntry("din/%.4s=%p,dctx=%p,dout/%.4s=%p", (afxChar*)&din->impl->fcc, din, dctx, (afxChar*)&dout->impl->fcc, dout);
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);
    AfxAssertObject(dscr, AFX_FCC_DSCR);
    sglVmt const* gl = &deng->wglVmt;

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
            _AfxStdBufUpdate(&(cmd->bindIbuf.ibuf->buf), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, deng);
            break;
        }
        case AFX_DSC_BIND_WILD_VERTEX_BUFFERS:
        {
            for (i = 0; i < cmd->bindVbuf.cnt; i++)
            {
                AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
                _AfxStdBufUpdate(&(cmd->bindVbuf.vbuf[i]->buf), GL_ARRAY_BUFFER, GL_STATIC_DRAW, deng);
            }
            break;
        }
        case AFX_DSC_BIND_VERTEX_BUFFERS:
        {
            for (i = 0; i < cmd->bindVbuf.cnt; i++)
            {
                AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
                _AfxStdBufUpdate(&(cmd->bindVbuf.vbuf[i]->buf), GL_ARRAY_BUFFER, GL_STATIC_DRAW, deng);
            }
            break;
        }
        case AFX_DSC_BIND_PIPELINE_PLUGS:
        {
            for (i = 0; i < cmd->bindLego.cnt; i++)
            {
                AfxAssertObject(cmd->bindLego.lego[i], AFX_FCC_LEGO);
                _AfxStdLegoUpdate(cmd->bindLego.lego[i], deng);
            }
            break;
        }
        case AFX_DSC_BIND_PIPELINE:
        {
            AfxAssertObject(cmd->bindPip.pip, AFX_FCC_PIP);
            _AfxStdPipUpdate(cmd->bindPip.pip, deng);
            break;
        }
        case AFX_DSC_BEGIN_RENDER_PASS:
        {
            AfxAssertObject(cmd->beginRenderPass.canv, AFX_FCC_CANV);
            _AfxStdCanvUpdate(cmd->beginRenderPass.canv, deng);
            break;
        }
        case AFX_DSC_BEGIN_RENDERING:
        {
            for (i = 0; i < cmd->beginRendering.rasterCnt; i++)
                _SglTexUpdate(&(cmd->beginRendering.rasters[i].surf->tex), deng);

            if (cmd->beginRendering.depth.surf)
                _SglTexUpdate(&(cmd->beginRendering.depth.surf->tex), deng);

            if (cmd->beginRendering.stencil.surf)
                _SglTexUpdate(&(cmd->beginRendering.stencil.surf->tex), deng);

            break;
        }
        case AFX_DSC_COPY_TEXTURE:
        {
            AfxAssertObject(cmd->copyTex.dst, AFX_FCC_TEX);
            AfxAssertObject(cmd->copyTex.src, AFX_FCC_TEX);
            _SglTexUpdate(cmd->copyTex.dst, deng);
            _SglTexUpdate(cmd->copyTex.src, deng);
        }
        default: break;
        }
    }

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
            _AfxDengBindIndexBuffer(deng, cmd->bindIbuf.ibuf, cmd->bindIbuf.baseIdx);
            break;
        }
        case AFX_DSC_BIND_WILD_VERTEX_BUFFERS:
        {
            //AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
            _AfxDengBindWildVertexBuffers(deng, cmd->bindVbuf.first, cmd->bindVbuf.cnt, cmd->bindVbuf.buf, cmd->bindVbuf.offset, cmd->bindVbuf.vtxArrSiz, cmd->bindVbuf.vtxStride);
            break;
        }
        case AFX_DSC_BIND_VERTEX_BUFFERS:
        {
            //AfxAssertObject(cmd->bindVbuf.vbuf[i], AFX_FCC_VBUF);
            _AfxDengBindVertexBuffers(deng, cmd->bindVbuf.first, cmd->bindVbuf.cnt, cmd->bindVbuf.vbuf, cmd->bindVbuf.baseVtx, cmd->bindVbuf.vtxArr);
            break;
        }
        case AFX_DSC_BIND_PIPELINE_PLUGS:
        {
            //AfxAssertObject(cmd->bindLego.lego[i], AFX_FCC_LEGO);
            _AfxDengBindLegos(deng, cmd->bindLego.first, cmd->bindLego.cnt, cmd->bindLego.lego);
            break;
        }
        case AFX_DSC_SET_INPUT_ASSEMBLY_STATE:
        {
            _AfxDengSetInputAssemblyState(deng, &cmd->ia.state);
            break;
        }
        case AFX_DSC_SET_RASTERIZATION_STATE:
        {
            _AfxDengSetRasterizerState(deng, &cmd->ras.state);
            break;
        }
        case AFX_DSC_SET_DEPTH_STENCIL_STATE:
        {
            _AfxDengSetDepthState(deng, &cmd->depth.state);
            break;
        }
        case AFX_DSC_BIND_PIPELINE:
        {
            //AfxAssertObject(cmd->bindPip.pip, AFX_FCC_PIP);
            _AfxDengBindPipeline(deng, cmd->bindPip.pip);
            break;
        }
        case AFX_DSC_BEGIN_RENDER_PASS:
        {
            //AfxAssertObject(cmd->beginRenderPass.canv, AFX_FCC_CANV);
            _AfxDengBeginRenderPass(deng, cmd->beginRenderPass.canv, &cmd->beginRenderPass.area, cmd->beginRenderPass.annexCnt, cmd->beginRenderPass.annexes);
            break;
        }
        case AFX_DSC_BEGIN_RENDERING:
        {
            //AfxAssertObject(cmd->beginRenderPass.canv, AFX_FCC_CANV);
            _AfxDengBeginRendering(deng, &cmd->beginRendering.area, cmd->beginRendering.layerCnt, cmd->beginRendering.rasterCnt, cmd->beginRendering.rasters, &cmd->beginRendering.depth, &cmd->beginRendering.stencil);
            break;
        }
        case AFX_DSC_END_RENDERING:
        {
            _AfxDengEndRendering(deng);
            break;
        }
        case AFX_DSC_END_RENDER_PASS:
        {
            _AfxDengEndRenderPass(deng);
            break;
        }
        case AFX_DSC_SET_VIEWPORT:
        {
            _AfxDengSetViewport(deng, cmd->vp.first, cmd->vp.cnt, cmd->vp.vp);
            break;
        }
        case AFX_DSC_SET_SCISSOR:
        {
            _AfxDengSetScissor(deng, cmd->scissor.first, cmd->scissor.cnt, cmd->scissor.rect);
            break;
        }
        case AFX_DSC_DRAW:
        {
            _AfxDengDraw(deng, cmd->draw.vtxCnt, cmd->draw.instCnt, cmd->draw.firstVtx, cmd->draw.firstInst);
            break;
        }
        case AFX_DSC_DRAW_INDEXED:
        {
            _AfxDengDrawIndexed(deng, cmd->drawIndexed.idxCnt, cmd->drawIndexed.instCnt, cmd->drawIndexed.firstIdx, cmd->drawIndexed.vtxOff, cmd->drawIndexed.firstInst);
            break;
        }
        case AFX_DSC_COPY_TEXTURE:
        {
            _AfxDengCopyTexture(deng, cmd->copyTex.dst, cmd->copyTex.src, cmd->copyTex.rgnCnt, cmd->copyTex.rgn);
            break;
        }
        default:
            AfxError("Command %i not handled", cmd->cmd.id);
            break;
        }
    }

    dscr->state = AFX_DSCR_STATE_EXECUTABLE;

#if 0
    // do stuff
    {
        if (din->impl->fcc == AFX_FCC_WIMP)
        {

            afxRect surfaceArea = { .offset = { 0, 0 },.extent = { din->surfaceArea.extent[0] * AfxScalar(dout->width), din->surfaceArea.extent[1] * AfxScalar(dout->height) } };
            gl->ClearColor(0.3, 0.3, 0.3, 1.0);
            gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            afxApplication wimp;

            if ((wimp = AfxObjectGetUdd(&din->obj)))
            {
                AfxApplicationStepWimp(wimp, dctx->deltaTime);

                nvgSave(din->wimp.ctx);
                nvgBeginFrame(din->wimp.ctx, surfaceArea.w, surfaceArea.h, surfaceArea.w / surfaceArea.w);

                struct { afxDrawContext dctx; afxApplication app; } data = { dctx, wimp };
                AfxApplicationForEachWidget(wimp, (void*)_SglDrawWidget, &data);

                nvgEndFrame(din->wimp.ctx);
                nvgRestore(din->wimp.ctx);
            }
        }
    }
#endif
    return err;
}

_SGL afxResult _AfxDengImplDrawWorkStreaming(afxDrawEngine deng, afxDrawWork const *dwrk)
{
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);
    AfxAssert(dwrk);

    AfxGetTime(&deng->lastStreamBeginTime);

    afxDrawContext dctx = AfxObjectGetProvider(&deng->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    for (afxNat j = 0; j < dwrk->streaming.resCnt; j++)
    {
        afxObject *obj = dwrk->streaming.resources[j];
        //AfxAssertObject(dscr, AFX_FCC_DSCR);
        //AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);
        //_AfxDengExecuteDscr(deng, dscr);

        HDC dc = wsglGetCurrentDC();
        HGLRC rc = wsglGetCurrentContext();
        AfxAssert(rc == deng->wglGlrc);

        if (dc != deng->wglDc)
        {
            if (!wsglMakeCurrent(deng->wglDc, deng->wglGlrc))
            {
                AfxThrowError();
                AfxError("deng %p failed to be set.", deng);
                wsglMakeCurrent(deng->wglDc, deng->wglGlrc);
            }
        }
    }

    AfxGetTime(&deng->lastStreamEndTime);

    return err;
}

_SGL afxResult _AfxDengImplDrawWorkSubmission(afxDrawEngine deng, afxDrawWork const *dwrk)
{
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);
    AfxAssert(dwrk);

    AfxGetTime(&deng->lastSubmisBeginTime);

    //afxDrawContext dctx = AfxObjectGetProvider(&deng->obj);
    //AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxSize frameTime = AfxGetTimer();

    if (1000 <= (frameTime - deng->swapTime))
    {
        deng->swapTime = frameTime;
        deng->fps = deng->frameNum;
        deng->frameNum = 0;
    }

    // deveriam ser movidos para afxDrawInput para separação e exibição correta para cada saída?

    ++deng->frameNum;
    deng->frameTime = frameTime;
    deng->numOfFedVertices = 0;
    deng->numOfFedIndices = 0;
    deng->numOfFedInstances = 0;

    for (afxNat j = 0; j < dwrk->submission.scriptCnt; j++)
    {
        afxDrawScript dscr = dwrk->submission.scripts[j];
        AfxAssertObject(dscr, AFX_FCC_DSCR);
        AfxAssert(dscr->state == AFX_DSCR_STATE_PENDING);

        HDC dc = wsglGetCurrentDC();
        HGLRC rc = wsglGetCurrentContext();
        AfxAssert(rc == deng->wglGlrc);

        if (dc != deng->wglDc)
        {
            if (!wsglMakeCurrent(deng->wglDc, deng->wglGlrc))
            {
                AfxThrowError();
                AfxError("deng %p failed to be set.", deng);
                wsglMakeCurrent(deng->wglDc, deng->wglGlrc);
            }
        }

        _AfxDengExecuteDscr(deng, dscr);
    }

    AfxGetTime(&deng->lastSubmisEndTime);

    return err;
}

_SGL afxResult _AfxDengImplDrawWorkPresentation(afxDrawEngine deng, afxDrawWork const *dwrk)
{
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);
    AfxAssert(dwrk);

    AfxGetTime(&deng->lastPresentBeginTime);

    afxDrawContext dctx = AfxObjectGetProvider(&deng->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    for (afxNat j = 0; j < dwrk->presentation.outputCnt; j++)
    {
        afxDrawOutput dout = dwrk->presentation.outputs[j];
        AfxAssertObject(dout, AFX_FCC_DOUT);

        afxSurface surf = AfxDrawOutputGetBuffer(dout, dwrk->presentation.outputBufIdx[j]);
        AfxAssertObject(surf, AFX_FCC_SURF);
        AfxAssert(surf->state == AFX_SURF_STATE_PRESENTABLE);
        surf->state = AFX_SURF_STATE_PENDING;

        AfxStringFormat(&(dout->title.str), "%i FPS, %d/%d --- QWADRO Draw System (SIGGL %u.%u.%u) --- (c) 2017 SIGMA Technology Group", deng->fps, deng->numOfFedVertices, deng->numOfFedIndices,  deng->wglGlrcVerMajor, deng->wglGlrcVerMinor, deng->wglGlrcVerPatch);

        HDC dc = wsglGetCurrentDC();
        HGLRC rc = wsglGetCurrentContext();
        AfxAssert(rc == deng->wglGlrc);

        if (dc != dout->wglDc)
        {
            if (!wsglMakeCurrent(dout->wglDc, deng->wglGlrc))
            {
                AfxThrowError();
                AfxError("deng %p failed to set to draw output device %p.", deng, dout);
                wsglMakeCurrent(deng->wglDc, deng->wglGlrc);
            }
        }

        _AfxDengPresentSurface(deng, dout, dwrk->presentation.outputBufIdx[j]);
    }
    AfxGetTime(&deng->lastPresntEndTime);

    return err;
}

#if 0
_SGL afxError AfxDrawEngineWaitForIdle(afxDrawEngine deng)
{

}
#endif

_SGL void _AfxStdDqueProcess(afxDrawEngine deng)
{
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);
    afxNat queueIdx = deng->queueIdx;
    afxDrawContext dctx = AfxObjectGetProvider(&(deng->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    sglVmt const* gl = &deng->wglVmt;
    _sglDeleteGlRes *delRes;

    if (AfxTransistorTryEnterExclusive(&deng->deletionLock))
    {
        if ((delRes = AfxQueuePull(&deng->deletionQueue)))
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
            AfxQueuePop(&deng->deletionQueue);
        }
        AfxTransistorExitExclusive(&deng->deletionLock);
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

                    _AfxDrawInputProcess(din, deng->queueIdx); // should just do input to script, using just CPU resources.

                    if (din->streamingThreadEnabled[queueIdx])
                    {
                        if (AfxTransistorTryEnterExclusive(&din->streamingLock))
                        {
                            if ((dwrk = AfxQueuePull(&din->streamingQueue)))
                            {
                                _AfxDengImplDrawWorkStreaming(deng, dwrk);
                                AfxQueuePop(&din->streamingQueue);
                            }
                            AfxTransistorExitExclusive(&din->streamingLock);
                        }
                    }

                    if (!deng->submissionSuspended)
                    {
                        if (din->submissionThreadEnabled[queueIdx])
                        {
                            if (AfxTransistorTryEnterExclusive(&din->submissionLock))
                            {
                                if ((dwrk = AfxQueuePull(&din->submissionQueue)))
                                {
                                    _AfxDengImplDrawWorkSubmission(deng, dwrk);
                                    AfxQueuePop(&din->submissionQueue);
                                }
                                AfxTransistorExitExclusive(&din->submissionLock);
                            }
                        }
                    }

                    if (!deng->presentationSuspended)
                    {
                        if (din->presentationThreadEnabled[queueIdx])
                        {
                            if (AfxTransistorTryEnterExclusive(&din->presentationLock))
                            {
                                if ((dwrk = AfxQueuePull(&din->presentationQueue)))
                                {
                                    _AfxDengImplDrawWorkPresentation(deng, dwrk);
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

_SGL void _AfxStdDqueWorkerThreadExec(afxThread thr)
{
    afxError err = NIL;
    AfxAssertObject(thr, AFX_FCC_THR);
    afxDrawEngine deng = thr->udd;
    _AfxStdDqueProcess(deng);
}

_SGL void _SglEnqueueGlResourceDeletion(afxDrawContext dctx, afxNat queueIdx, afxNat type, GLuint gpuHandle)
{
    afxDrawEngine deng = dctx->queues[queueIdx];
    AfxTransistorEnterExclusive(&deng->deletionLock);
    _sglDeleteGlRes delRes;
    delRes.gpuHandle = gpuHandle;
    delRes.type = type;
    AfxQueuePush(&deng->deletionQueue, &delRes);
    AfxTransistorExitExclusive(&deng->deletionLock);
};

_SGL void _AfxStdDqueWorkerThreadStart(afxThread thr, afxDrawEngine deng)
{
    afxError err = NIL;
    AfxAssertObject(thr, AFX_FCC_THR);
    AfxAssertObject(deng, AFX_FCC_DENG);

    afxDrawContext dctx = AfxObjectGetProvider(&(deng->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawDriver ddrv = AfxDrawContextGetDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDriverIdd *ddrvIdd = ddrv->idd;

    if (_SglGlCtxW32Create(&ddrvIdd->oglWndClss, ddrvIdd->wglPrimeGlrc, &(deng->wglWnd), &(deng->wglDc), &(deng->wglGlrc))) AfxThrowError();
    else
    {
        deng->wglDcPxlFmt = wsglGetPixelFormat(deng->wglDc);

        if (!(wsglMakeCurrent(deng->wglDc, deng->wglGlrc))) AfxThrowError();
        else
        {
            sglVmt* gl = (sglVmt*)&deng->wglVmt;
            _SglLoadVmt(gl, 1, 0);

            afxString ver;
            AfxStringMapConst(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
            AfxStringScan(&ver, "%u.%u.%u", &deng->wglGlrcVerMajor, &deng->wglGlrcVerMinor, &deng->wglGlrcVerPatch);
            gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(deng->wglGlrcVerMajor));
            gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(deng->wglGlrcVerMinor));

            _SglLoadVmt(gl, 4, 6);

            AfxStringUpdate(&deng->subsysName.str, 0, 0, (afxChar const*)gl->GetString(GL_RENDERER));
            //AfxComment("4D Draw System (OpenGL/Vulkan Continuous Integration (%.*s v%u.%u.%u))", AfxPushString(&deng->subsysName.str), deng->wglGlrcVerMajor, deng->wglGlrcVerMinor, deng->wglGlrcVerPatch);
            //SetWindowTextA(deng->wglWnd, AfxStringGetData(&ver));

            //gl->Enable(GL_FRAMEBUFFER_SRGB);
            gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }

        if (err)
        {
            wsglMakeCurrent(NIL, NIL);
            wsglDeleteContext(deng->wglGlrc);
            ReleaseDC(deng->wglWnd, deng->wglDc);
            DestroyWindow(deng->wglWnd);
        }
    }

}

_SGL afxDrawEngine _AfxDrawContextAcquireEngine(afxDrawContext dctx, afxNat idx, afxBool autonomous)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxEntry("dctx=%p", dctx);
    afxDrawEngine deng = NIL;

    _afxDqueCtorArgs args =
    {
        dctx,
        idx,
        autonomous
    };

    if (!(deng = AfxObjectAcquire(AfxDrawContextGetQueueClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return deng;
};

_SGL afxError _AfxDengDtor(afxDrawEngine deng)
{
    AfxEntry("deng=%p", deng);
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);

    deng->submissionSuspended = TRUE;
    deng->presentationSuspended = TRUE;
#if 0
    AfxObjectRelease(&(deng->rgbRectVbo->buf.obj));
    AfxObjectRelease(&(deng->rgbRectPip->res.obj));
    AfxObjectRelease(&(deng->rgbRectLego->obj));
    AfxObjectRelease(&(deng->rgbRectSmp->obj));
#endif

    if (deng->thread)
    {
        AfxObjectRelease(&deng->thread->obj);
    }
    
    AfxQueueDrop(&deng->deletionQueue);
    AfxTransistorDrop(&deng->deletionLock);

    if (deng->thread)
    {
        wsglMakeCurrent(NIL, NIL);
        wsglDeleteContext(deng->wglGlrc);
        ReleaseDC(deng->wglWnd, deng->wglDc);
        DestroyWindow(deng->wglWnd);
    }
    return err;
}

_SGL afxError _AfxDengCtor(afxDrawEngine deng, _afxDqueCtorArgs *args)
{
    AfxEntry("deng=%p", deng);
    afxError err = NIL;
    AfxAssertObject(deng, AFX_FCC_DENG);

    afxDrawContext dctx = AfxObjectGetProvider(&(deng->obj));
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxDrawEngineGetDriver(deng);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    AfxTransistorDeploy(&deng->deletionLock);
    AfxQueueDeploy(&deng->deletionQueue, sizeof(_sglDeleteGlRes), 32);
    AfxChainDeploy(&deng->instantiationList, deng);

    deng->queueIdx = args->idx;

    AfxString128(&deng->subsysName, NIL);

    deng->numOfFedIndices = 0;
    deng->numOfFedVertices = 0;
    deng->numOfFedInstances = 0;

    deng->deltaTime = 0.f;
    deng->frameTime = 0;
    deng->swapTime = 0;
    deng->frameNum = 0;
    deng->fps = 0;

    deng->submissionSuspended = FALSE;
    deng->presentationSuspended = FALSE;
    
    deng->activeFboIdx = 0;
    deng->activeVaoIdx = 0;
    deng->presentFboGpuHandle = 0;
    
    AfxZero(deng->vao, sizeof(deng->vao));
    AfxZero(deng->tmpFbo, sizeof(deng->tmpFbo));
    
    AFX_ZERO(&deng->state);


    afxUri uri;
    AfxUriMapConstData(&uri, "data/pipeline/rgbaToRgbaYFlipped.pip.urd", 0);
    //AfxUriMapConstData(&uri, "data/pipeline/rgbaToRgbaYFlippedBrokenLens.pip.urd", 0);
    deng->presentPip = AfxDrawContextFetchPipeline(dctx, &uri);
    AfxAssertObject(deng->presentPip, AFX_FCC_PIP);

    afxSamplerSpecification smpSpec = { 0 };
    smpSpec.magFilter = AFX_TEXEL_FLT_NEAREST;
    smpSpec.minFilter = AFX_TEXEL_FLT_NEAREST;
    smpSpec.mipmapFilter = AFX_TEXEL_FLT_NEAREST;
    smpSpec.uvw[0] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[1] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit
    smpSpec.uvw[2] = AFX_TEXEL_ADDR_REPEAT; // EDGE fucks this shit

    deng->presentSmp = AfxDrawContextAcquireSampler(dctx, &smpSpec);
    AfxAssertObject(deng->presentSmp, AFX_FCC_SMP);

    afxPipelineRig rig = AfxPipelineGetRig(deng->presentPip);
    AfxAssertObject(rig, AFX_FCC_PIPR);
    afxLegoPoint point;
    point.resource.img.smp = deng->presentSmp;
    point.resource.img.tex = NIL;
    deng->presentLego = AfxDrawContextAcquireLego(dctx, &rig->sockets[0], &point);
    AfxAssertObject(deng->presentLego, AFX_FCC_LEGO);

    afxString tmpStr;
    const afxV2d vertex[] =
    {
        { -1.0,  1.0 },
        { -1.0, -1.0 },
        {  1.0,  1.0 },
        {  1.0, -1.0 }
    };
    afxVertexDataSpecification presentVbufSpec[1];
    presentVbufSpec[0].fmt = AFX_VTX_FMT_XY32;
    presentVbufSpec[0].semantic = AfxStringMapConst(&tmpStr, "a_pos", 5);
    presentVbufSpec[0].src = vertex;
    presentVbufSpec[0].srcFmt = AFX_VTX_FMT_XY32;
    presentVbufSpec[0].usage = AFX_VTX_USAGE_POS;
    deng->presentVbuf = AfxDrawContextAcquireVertexBuffer(dctx, 4, 1, presentVbufSpec);
    AfxAssertObject(deng->presentVbuf, AFX_FCC_VBUF);

    if (args->autonomous)
    {
        deng->thread = AfxSystemAcquireThread(AfxSystemGet(), (void*)_AfxStdDqueWorkerThreadStart, deng, _AfxStdDqueWorkerThreadExec, AfxSpawnHint());
    }
    else
    {
        deng->thread = NIL;

        _sglDriverIdd *idd = ddrv->idd;

        sglVmt* gl = (sglVmt*)&deng->wglVmt;
        *gl = idd->wglPrimeVmt;
        deng->wglWnd = idd->wglPrimeWnd;
        deng->wglDc = idd->wglPrimeDc;
        deng->wglDcPxlFmt = idd->wglPrimeDcPxlFmt;
        deng->wglGlrc = idd->wglPrimeGlrc;
        deng->wglGlrcVerMajor = idd->wglPrimeGlrcVerMajor;
        deng->wglGlrcVerMinor = idd->wglPrimeGlrcVerMinor;
        deng->wglGlrcVerPatch = idd->wglPrimeGlrcVerPatch;
        AfxStringCopy(&deng->subsysName.str, &idd->subsysName.str);



        gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }


    deng->instanced = TRUE;


    return err;
}

_SGL afxClassSpecification const _AfxDengClassSpec;

afxClassSpecification const _AfxDengClassSpec =
{
    AFX_FCC_DENG,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawEngine)),
    NIL,
    (void*)_AfxDengCtor,
    (void*)_AfxDengDtor,
    "Draw Engine",
    NIL
};
