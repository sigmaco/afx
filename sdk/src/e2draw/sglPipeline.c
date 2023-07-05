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

#include "afx/core/io/afxXml.h"
#include "sgl.h"

#include "afx/draw/pipelining/afxPipeline.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/core/io/afxUri.h"
#include "afx/core/afxSystem.h"
#include "../e2coree/draw/afxDrawParadigms.h"
// OpenGL/Vulkan Continuous Integration

////////////////////////////////////////////////////////////////////////////////
// DRAW PIPELINE                                                              //
////////////////////////////////////////////////////////////////////////////////

#if 0
_SGL afxResult _AfxRegisterOpenGlResourcesToQwadroDrawPipeline(afxPipeline pip)
{
    sglVmt* gl = &(((afxDrawSystem)AfxObjectGetProvider(&pip->obj))->vmt);
#if 0
    afxShaderStage                      stages;
    afxShaderResourceType              type;
    afxPipelineSubresType           baseType;
    afxNat32                            set,
        binding,
        loc,
        inputAttachIdx,
        vecSiz,
        columns,
        arrSiz,
        off,
        siz;
    afxEmbeddedString(name, 16);
    afxPipelineSubresource const    *subresources;
#endif
    _DrawPipelineResource res, *resptr;
    GLuint gpuHandle = pip->gpuHandle;

    afxInt cnt = 0;
    gl->GetProgramiv(gpuHandle, GL_ACTIVE_UNIFORM_BLOCKS, &cnt); _SglThrowErrorOccuried();

    for (afxInt i = 0; i < cnt; i++)
    {
        AFX_ZERO(&res);

        GLint binding, dataSiz, nameLen, subresCnt, subresIndices[1], refByVsh, refByFsh, refByGsh;
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_BINDING, &binding);  _SglThrowErrorOccuried();// The current block binding, as set either within the shader or from gl->UniformBlockBinding.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSiz);  _SglThrowErrorOccuried();// The buffer object storage size needed for this block.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);  _SglThrowErrorOccuried();// The length of this block's name.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &subresCnt);  _SglThrowErrorOccuried();// The number of active uniforms within this block.
        //glGetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &subresIndices); // params​ will be filled in with the uniform indices of all uniforms that are stored in this block.It will receive GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS number of uniforms.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, &refByVsh);  _SglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, &refByFsh);  _SglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER, &refByGsh);  _SglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.


        res.binding = binding;
        res.resType = AFX_SHD_RES_TYPE_CONSTANT_BUFFER;
        res.set = 0;
        res.location = i;
        res.arraySize = 0;
        res.vecSize = 0;
        res.inputAttachmentIndex = 0;
        res.size = dataSiz;
        res.stages = 0;
        gl->GetActiveUniformBlockName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
        res.name[15] = '\0';
        //res.loc = gl->GetUniformLocation(gpuHandle, res.nameBuf); _SglThrowErrorOccuried();

        afxResult j = AfxPipeline.RegisterResource(pip, &res);
        AfxPipeline.GetResource(pip, j, &res);
        AfxAssert(i == j);
        AfxEcho("%d %d %d %d", res.loc, res.binding, i, j);
    }

    gl->GetProgramiv(gpuHandle, GL_ACTIVE_UNIFORMS, &cnt);

    for (GLuint i = 0; i < (GLuint)cnt; i++)
    {
        AFX_ZERO(&res);
        AfxDeployString(&res.name, res.nameBuf);

        afxBool valid = FALSE;

        GLint type, blockIdx;
        gl->GetActiveUniformsiv(gpuHandle, 1, &i, GL_UNIFORM_TYPE, &type); _SglThrowErrorOccuried();
        gl->GetActiveUniformsiv(gpuHandle, 1, &i, GL_UNIFORM_BLOCK_INDEX, &blockIdx); _SglThrowErrorOccuried();

        if (blockIdx == -1)
        {
            switch (type)
            {
            case GL_SAMPLER_1D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_2D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_3D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_CUBE:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            default:
                AfxError("%X is a unhandled type", type);
                valid = FALSE;
                break;
            }

            if (valid)
            {
                res.location = gl->GetUniformLocation(gpuHandle, res.name); _SglThrowErrorOccuried();
                //res.loc = i;
                afxSupplyDescriptor entry[1];
                afxResult j = AfxPipeline.FindEntries(pip, 1, &res.name, entry);
                //AfxPipeline.GetResource(pip, j, &res);

                if (j == 1)
                {
                    AfxAssert(i == j);
                    gl->BindUniform(gpuHandle, );
                    AfxEcho("%d %d %d %d", res.location, res.binding, i, j);
                }
            }
        }
    }
    return 0;
}
#endif

_SGL afxError _SglDqueBindAndSyncPip(afxDrawQueue dque, afxPipeline pip)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    sglVmt const* gl = &dque->wglVmt;

    if (pip)
    {
        AfxAssertObject(pip, AFX_FCC_PIP);

        GLuint glHandle = pip->glHandle[dque->queueIdx];

        if ((pip->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((pip->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (glHandle)
                {
                    gl->DeleteProgram(glHandle); _SglThrowErrorOccuried();
                    glHandle = NIL;
                }

                AfxAssert(NIL == glHandle);

                if (!(glHandle = gl->CreateProgram()))
                {
                    AfxThrowError();
                    _SglThrowErrorOccuried();
                }
                else
                {
                    AfxAssert(gl->IsProgram(glHandle));

                    for (afxNat i = 0; i < pip->shaderCnt; i++)
                    {
                        afxShader shd = pip->shaders[i];
                        AfxAssertObject(shd, AFX_FCC_SHD);
                        afxShaderStage stage = AfxShaderGetStage(shd);

                        _SglDqueSyncShd(dque, shd, stage);
                        AfxAssert(gl->IsShader(shd->glHandle));
                        gl->AttachShader(glHandle, shd->glHandle); _SglThrowErrorOccuried();
                    }

                    gl->LinkProgram(glHandle); _SglThrowErrorOccuried();
                    afxBool linked;
                    gl->GetProgramiv(glHandle, GL_LINK_STATUS, &linked); _SglThrowErrorOccuried();

                    if (linked == GL_FALSE)
                    {
                        AfxThrowError();
                        afxChar str[1024];
                        gl->GetProgramInfoLog(glHandle, sizeof(str), NIL, (GLchar*)str); _SglThrowErrorOccuried();
                        AfxError(str);
                        gl->DeleteProgram(glHandle); _SglThrowErrorOccuried();
                        glHandle = NIL;
                    }
                    else
                    {
                        pip->assembled = linked;

                        gl->UseProgram(glHandle); _SglThrowErrorOccuried();
                        //_AfxRegisterOpenGlResourcesToQwadroDrawPipeline(pip);
                        AfxEcho("afxPipeline %p hardware-side data instanced.", pip);
                        pip->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
                    }
                }
                pip->glHandle[dque->queueIdx] = glHandle;
            }
            else if ((pip->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(glHandle);
                AfxThrowError(); // can't be modified
            }
        }
        else
        {
            AfxAssert(glHandle);
            gl->UseProgram(glHandle); _SglThrowErrorOccuried();
        }

        if (!err)
        {
            for (afxNat i = 0; i < pip->wiringCnt; i++)
            {
                if (!pip->wiring[i].resolved)
                {
                    pip->wiring[i].resolved = !(_SglDqueBindAndResolveLego(dque, pip->wiring[i].set, pip->wiring[i].legt));
                }
            }
        }
    }
    else
    {
        gl->UseProgram(0); _SglThrowErrorOccuried();
    }
    return err;
}

_SGL afxBool _AfxPipelineHasColorBlending(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasColorBlending;
}

_SGL afxBool _AfxPipelineHasMultisampling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasMultisampling;
}

_SGL afxBool _AfxPipelineHasDepthHandling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasDepthHandling;
}

_SGL afxBool _AfxPipelineHasRasterization(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasRasterization;
}

_SGL afxResult _AfxPipelineGetScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(rect);
    AfxAssert(cnt <= pip->scissorCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxRect const *rect2 = &pip->scissors[first + i];
        AfxRectCopy(&rect[i], rect2);
        hitCnt++;
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineGetScissorCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->scissorCnt;
}

_SGL afxResult _AfxPipelineGetViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(vp);
    AfxAssert(cnt <= pip->vpCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxViewport const *vp2 = &(pip->vps[first + i]);
        AfxV2dCopy(vp[i].offset, vp2->offset);
        AfxV2dCopy(vp[i].extent, vp2->extent);
        AfxV2dCopy(vp[i].depth, vp2->depth);

        hitCnt++;
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineGetViewportCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->vpCnt;
}

_SGL afxBool _AfxPipelineHasInputAssembling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasVtxInAssembling;
}

_SGL afxResult _AfxPipelineGetInputStreams(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(streams);
    AfxAssert(cnt <= pip->inCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxCopy(&streams[i], &pip->ins[first + i], sizeof(streams[0]));
        hitCnt++;
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineGetInputStreamCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->inCnt;
}

_SGL afxResult _AfxPipelineGetModules(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(shd);
    AfxAssert(cnt <= pip->shaderCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        shd[i] = pip->shaders[first + i];
        hitCnt++;
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineGetStageCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->shaderCnt;
}

_SGL afxNat _AfxPipelineGetWiringCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->wiringCnt;
}

_SGL afxError _AfxPipelineGetWiring(afxPipeline pip, afxNat idx, afxNat *set, afxLego *legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    //afxPipelineRig pipr = AfxLinkageGetOwner(&pip->pipr);
    //AfxAssertObject(pipr, AFX_FCC_PIPR);
    
    *set = pip->wiring[idx].set;
    *legt = pip->wiring[idx].legt;
    return err;
}

_SGL void _AfxPipShdrHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxPipeline pip = (void*)obj;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssertObject(ev->obj, AFX_FCC_SHD);
    //AfxAssertConnection(&dout->din);
    //AfxAssert(AfxConnectionGetObject(&dout->din) == ev->obj);

    if (ev->type == AFX_EVENT_OBJ_DESTROYED)
    {
        AfxError("Shader %p attached to draw pipeline %p was destroyed.", ev->obj, obj);
        while (0 < AfxObjectRelease(&pip->obj));
    }
}

_SGL afxBool _SglPipEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxPipeline pip = (void*)obj;
    AfxAssertObject(pip, AFX_FCC_PIP);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglPipEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxPipeline pip = (void*)obj;
    AfxAssertObject(pip, AFX_FCC_PIP);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxPipDtor(afxPipeline pip)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);

    afxDrawContext dctx = AfxObjectGetProvider(&pip->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    for (afxNat i = 0; i < dctx->queueCnt; i++)
    {
        if (pip->glHandle[i])
        {
            _SglEnqueueGlResourceDeletion(dctx, i, 5, pip->glHandle[i]);
            pip->glHandle[i] = 0;
        }
    }

    if (err)
    {
        for (afxNat i = 0; i < pip->shaderCnt; i++)
        {
            AfxObjectRemoveEventFilter(&(pip->shaders[i]->obj), &pip->obj);
            AfxObjectRelease(&(pip->shaders[i]->obj));
        }
    }

    AfxAssert(pip->shaders);
    AfxDeallocate(mem, pip->shaders);

    if (pip->vps)
        AfxDeallocate(mem, pip->vps);

    if (pip->scissors)
        AfxDeallocate(mem, pip->scissors);

    if (pip->wiring)
    {
        for (afxNat i = 0; i < pip->wiringCnt; i++)
            AfxObjectRelease(&(pip->wiring[i].legt->obj));

        AfxDeallocate(mem, pip->wiring);
    }

    return err;
}

_SGL afxPipImpl const _AfxStdPipImpl;

afxPipImpl const _AfxStdPipImpl =
{
    _AfxPipelineGetScissorCount,
    _AfxPipelineGetScissors,
    _AfxPipelineGetInputStreamCount,
    _AfxPipelineGetInputStreams,
    _AfxPipelineGetModules,
    _AfxPipelineGetStageCount,
    _AfxPipelineGetViewportCount,
    _AfxPipelineGetViewports,
    _AfxPipelineHasColorBlending,
    _AfxPipelineHasDepthHandling,
    _AfxPipelineHasMultisampling,
    _AfxPipelineHasRasterization,
    _AfxPipelineHasInputAssembling,
    _AfxPipelineGetWiringCount,
    _AfxPipelineGetWiring,
};

_SGL afxError _AfxPipCtor(void *cache, afxNat idx, afxPipeline pip, afxPipelineBlueprint const *blueprints)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);

    afxPipelineBlueprint const *pipb = &blueprints[idx];

    AfxAssertType(pipb, AFX_FCC_PIPB);

    afxDrawContext dctx = AfxObjectGetProvider(&pip->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    pip->shaders = NIL;
    pip->vps = NIL;
    pip->scissors = NIL;

    afxArray const* shaders = &pipb->shaders;
    afxNat shaderCnt = AfxArrayGetPop(shaders);
    AfxAssert(shaderCnt);
    pip->shaderCnt = 0;

    if (!(pip->shaders = AfxAllocate(mem, sizeof(pip->shaders[0]) * shaderCnt, AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < shaderCnt; i++)
        {
            afxUri const *uri = *(afxUri**)AfxArrayGetElement(shaders, i);
            afxShader shd;

            if (AfxDrawContextAcquireShaders(dctx, 1, uri, &shd)) AfxThrowError();
            else
            {
                if (AfxObjectInstallEventFilter(&shd->obj, &pip->obj))
                {
                    AfxObjectRelease(&shd->obj);
                    AfxThrowError();
                }
                else
                {
                    pip->shaders[pip->shaderCnt] = shd;
                    pip->shaderCnt++;
                }
            }

            if (err)
                break;
        }

        if (!err)
        {
            AfxAssert(pip->shaderCnt == shaderCnt);
            afxLegoBlueprint legb[_SGL_MAX_LEGO_PER_BIND];

            for (afxNat i = 0; i < _SGL_MAX_LEGO_PER_BIND; i++)
            {
                AfxLegoBlueprintBegin(&legb[i], dctx, 1);

                if (AfxLegoBlueprintAddShaderContributions(&legb[i], i, shaderCnt, pip->shaders))
                {
                    AfxThrowError();
                }

                if (err)
                {

                    for (afxNat j = 0; j < i; j++)
                    {
                        AfxLegoBlueprintEnd(&legb[i], NIL);
                    }
                    break;
                }
            }

            afxNat setCnt = 0;

            for (afxNat i = 0; i < _SGL_MAX_LEGO_PER_BIND; i++)
                setCnt += (legb[i].bindings._pop) ? 1 : 0;

            pip->wiringCnt = 0;

            if (setCnt && !(pip->wiring = AfxAllocate(mem, setCnt * sizeof(pip->wiring[0]), AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < _SGL_MAX_LEGO_PER_BIND; i++)
                {
                    if (legb[i].bindings._pop)
                    {
                        if (AfxDrawContextBuildLegos(dctx, 1, &legb[i], &(pip->wiring[pip->wiringCnt].legt)))
                        {
                            AfxThrowError();
                        }
                        else
                        {
                            pip->wiring[pip->wiringCnt].resolved = FALSE;
                            pip->wiring[pip->wiringCnt].set = i;
                            ++pip->wiringCnt;
                        }
                    }

                    if (i >= setCnt)
                        break;
                }
            }

            for (afxNat i = 0; i < _SGL_MAX_LEGO_PER_BIND; i++)
            {
                AfxLegoBlueprintEnd(&legb[i], NIL);
            }

            if (!err)
            {
                pip->inCnt = 0;

                for (afxNat i = 0; i < shaderCnt; i++)
                {
                    afxShader shd;
                    AfxPipelineGetModules(pip, i, 1, &shd);

                    if (AFX_SHADER_STAGE_VERTEX == AfxShaderGetStage(shd))
                    {
                        for (afxNat j = 0; j < shd->ioDeclCnt; j++)
                        {
                            pip->ins[pip->inCnt].location = shd->ioDecls[j].location;
                            pip->ins[pip->inCnt].binding = pip->ins[pip->inCnt].location;
                            pip->ins[pip->inCnt].format = shd->ioDecls[j].fmt;
                            pip->ins[pip->inCnt].offset = 0;
                            pip->inCnt++;
                        }
                        break;
                    }
                }

                pip->vtxInAssembling = (pip->hasVtxInAssembling = pipb->hasAssembling) ? pipb->inAssembling : (afxPipelineInputAssemblyState) { 0 };
                pip->rasterization = (pip->hasRasterization = pipb->hasRasterization) ? pipb->rasterization : (afxPipelineRasterizerState) { 0 };
                pip->depthHandling = (pip->hasDepthHandling = pipb->hasDepthHandling) ? pipb->depthHandling : (afxPipelineDepthState) { 0 };
                pip->multisampling = (pip->hasMultisampling = pipb->hasMultisampling) ? pipb->multisampling : (afxPipelineMultisampleState) { 0 };
                pip->colorBlending = (pip->hasColorBlending = pipb->hasColorBlending) ? pipb->colorBlending : (afxPipelineColorBlendState) { 0 };

                afxArray const* viewports = &pipb->viewports;
                afxNat vpCnt = AfxArrayGetPop(viewports);
                pip->vpCnt = 0;
                pip->vps = NIL;

                if (vpCnt && !(pip->vps = AfxAllocate(mem, sizeof(pip->vps[0]) * vpCnt, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < vpCnt; i++)
                    {
                        pip->vps[pip->vpCnt] = *(afxViewport const*)AfxArrayGetElement(viewports, i);
                        pip->vpCnt++;
                    }
                }

                if (!err)
                {
                    afxArray const* scissors = &pipb->scissors;
                    afxNat scissorCnt = AfxArrayGetPop(scissors);
                    pip->scissorCnt = 0;
                    pip->scissors = NIL;

                    if (scissorCnt && !(pip->scissors = AfxAllocate(mem, sizeof(pip->scissors[0]) * scissorCnt, AfxSpawnHint()))) AfxThrowError();
                    else
                    {
                        for (afxNat i = 0; i < scissorCnt; i++)
                        {
                            pip->scissors[pip->scissorCnt] = *(afxRect const*)AfxArrayGetElement(scissors, i);
                            pip->scissorCnt++;
                        }
                    }
                }

                AfxZero(pip->glHandle, sizeof(pip->glHandle));
                pip->updFlags = SGL_UPD_FLAG_DEVICE_INST;
            }

            if (err && pip->wiring)
            {
                for (afxNat i = 0; i < pip->wiringCnt; i++)
                    AfxObjectRelease(&(pip->wiring[i].legt->obj));

                AfxDeallocate(mem, pip->wiring);
            }
        }

        if (err)
        {
            for (afxNat i = 0; i < pip->shaderCnt; i++)
            {
                AfxObjectRemoveEventFilter(&(pip->shaders[i]->obj), &pip->obj);
                AfxObjectRelease(&(pip->shaders[i]->obj));
            }

            AfxAssert(pip->shaders);
            AfxDeallocate(mem, pip->shaders);

            if (pip->vps)
                AfxDeallocate(mem, pip->vps);

            if (pip->scissors)
                AfxDeallocate(mem, pip->scissors);
        }
    }
    return err;
}

_SGL afxClassSpecification const _AfxPipClassSpec;

afxClassSpecification const _AfxPipClassSpec =
{
    AFX_FCC_PIP,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxPipeline)),
    NIL,
    (void*)_AfxPipCtor,
    (void*)_AfxPipDtor,
    .event = _SglPipEventHandler,
    .eventFilter = _SglPipEventFilter,
    "afxPipeline",
    &_AfxStdPipImpl
};
