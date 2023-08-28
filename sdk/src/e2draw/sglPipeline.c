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

#include "afx/core/io/afxXml.h"
#include "sgl.h"

#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/pipelining/afxPipelineBlueprint.h"

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
    glVmt* gl = &(((afxDrawSystem)AfxGetObjectProvider(&pip->obj))->vmt);
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

_SGL afxError _SglDpuBindAndSyncPip(sglDpuIdd* dpu, afxPipeline pip, glVmt const* gl)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;

    if (pip)
    {
        AfxAssertObjects(1, &pip, AFX_FCC_PIP);

        GLuint glHandle = pip->glHandle;

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

                    for (afxNat i = 0; i < pip->base.shaderCnt; i++)
                    {
                        afxShader shd = pip->base.shaders[i];
                        AfxAssertObjects(1, &shd, AFX_FCC_SHD);
                        afxShaderStage stage = AfxGetShaderStage(shd);

                        _SglDpuSyncShd(dpu, shd, stage, gl);
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
                pip->glHandle = glHandle;
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
            for (afxNat i = 0; i < pip->base.wiringCnt; i++)
            {
                if (!pip->base.wiring[i].resolved)
                {
                    pip->base.wiring[i].resolved = !(_SglDpuBindAndResolveLego(dpu, pip->base.wiring[i].set, pip->base.wiring[i].legt, gl));
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

_SGL afxError _SglPipDtor(afxPipeline pip)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    afxDrawContext dctx = AfxGetObjectProvider(pip);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_CTX);

    if (err)
    {
        for (afxNat i = 0; i < pip->base.shaderCnt; i++)
        {
            AfxReleaseObjects(1, (void*[]) { pip->base.shaders[i] });
        }
    }

    AfxAssert(pip->base.shaders);
    AfxDeallocate(mem, pip->base.shaders);

    if (pip->base.vps)
        AfxDeallocate(mem, pip->base.vps);

    if (pip->base.scissors)
        AfxDeallocate(mem, pip->base.scissors);

    if (pip->base.wiring)
    {
        for (afxNat i = 0; i < pip->base.wiringCnt; i++)
            AfxReleaseObjects(1, (void*[]) { pip->base.wiring[i].legt });

        AfxDeallocate(mem, pip->base.wiring);
    }

    if (pip->glHandle)
    {
        _SglDeleteGlRes(dctx, 5, pip->glHandle);
        pip->glHandle = 0;
    }
    return err;
}

_SGL afxError _SglPipCtor(afxPipeline pip, afxCookie const* cookie)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    afxPipelineBlueprint const *pipb = ((afxPipelineBlueprint const*)cookie->udd[0]) + cookie->no;
    AfxAssertType(pipb, AFX_FCC_PIPB);

    afxDrawContext dctx = AfxGetObjectProvider(pip);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_CTX);

    pip->base.shaders = NIL;
    pip->base.vps = NIL;
    pip->base.scissors = NIL;

    afxArray const* shaders = &pipb->shaders;
    afxNat shaderCnt = AfxGetArrayPop(shaders);
    AfxAssert(shaderCnt);
    pip->base.shaderCnt = 0;

    if (!(pip->base.shaders = AfxAllocate(mem, sizeof(pip->base.shaders[0]) * shaderCnt, 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < shaderCnt; i++)
        {
            afxUri const *uri = AfxGetArrayUnit(shaders, i);
            afxShader shd;

            if (AfxAcquireShaders(dctx, 1, &shd, uri)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &shd, AFX_FCC_SHD);
                pip->base.shaders[pip->base.shaderCnt] = shd;
                pip->base.shaderCnt++;
            }

            if (err)
                break;
        }

        if (!err)
        {
            AfxAssert(pip->base.shaderCnt == shaderCnt);
            afxLegoBlueprint legb[/*_SGL_MAX_LEGO_PER_BIND*/4];

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
            {
                AfxLegoBlueprintBegin(&legb[i], 1);

                if (AfxLegoBlueprintAddShaderContributions(&legb[i], i, shaderCnt, pip->base.shaders))
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

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                setCnt += AfxGetArrayPop(&legb[i].bindings) ? 1 : 0;

            pip->base.wiringCnt = 0;

            if (setCnt && !(pip->base.wiring = AfxAllocate(mem, setCnt * sizeof(pip->base.wiring[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                {
                    if (AfxGetArrayPop(&legb[i].bindings))
                    {
                        if (AfxBuildLegos(dctx, 1, &(pip->base.wiring[pip->base.wiringCnt].legt), &legb[i]))
                        {
                            AfxThrowError();
                        }
                        else
                        {
                            AfxAssertObjects(1, &(pip->base.wiring[pip->base.wiringCnt].legt), AFX_FCC_LEGO);
                            pip->base.wiring[pip->base.wiringCnt].resolved = FALSE;
                            pip->base.wiring[pip->base.wiringCnt].set = i;
                            ++pip->base.wiringCnt;
                        }
                    }

                    if (i >= setCnt)
                        break;
                }
            }

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
            {
                AfxLegoBlueprintEnd(&legb[i], NIL);
            }

            if (!err)
            {
                pip->base.inCnt = 0;

                for (afxNat i = 0; i < shaderCnt; i++)
                {
                    afxShader shd;
                    AfxGetPipelineShaders(pip, i, 1, &shd);
                    AfxAssertObjects(1, &shd, AFX_FCC_SHD);

                    if (AFX_SHADER_STAGE_VERTEX == AfxGetShaderStage(shd))
                    {
                        for (afxNat j = 0; j < shd->base.ioDeclCnt; j++)
                        {
                            pip->base.ins[pip->base.inCnt].location = shd->base.ioDecls[j].location;
                            pip->base.ins[pip->base.inCnt].binding = pip->base.ins[pip->base.inCnt].location;
                            pip->base.ins[pip->base.inCnt].format = shd->base.ioDecls[j].fmt;
                            pip->base.ins[pip->base.inCnt].offset = 0;
                            pip->base.inCnt++;
                        }
                        break;
                    }
                }

                pip->base.vtxInAssembling = (pip->base.hasVtxInAssembling = pipb->hasAssembling) ? pipb->inAssembling : (afxPipelineInputAssemblyState) { 0 };
                pip->base.rasterization = (pip->base.hasRasterization = pipb->hasRasterization) ? pipb->rasterization : (afxPipelineRasterizerState) { 0 };
                pip->base.depthHandling = (pip->base.hasDepthHandling = pipb->hasDepthHandling) ? pipb->depthHandling : (afxPipelineDepthState) { 0 };
                pip->base.multisampling = (pip->base.hasMultisampling = pipb->hasMultisampling) ? pipb->multisampling : (afxPipelineMultisampleState) { 0 };
                pip->base.colorBlending = (pip->base.hasColorBlending = pipb->hasColorBlending) ? pipb->colorBlending : (afxPipelineColorBlendState) { 0 };

                afxArray const* viewports = &pipb->viewports;
                afxNat vpCnt = AfxGetArrayPop(viewports);
                pip->base.vpCnt = 0;
                pip->base.vps = NIL;

                if (vpCnt && !(pip->base.vps = AfxAllocate(mem, sizeof(pip->base.vps[0]) * vpCnt, 0, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < vpCnt; i++)
                    {
                        pip->base.vps[pip->base.vpCnt] = *(afxViewport const*)AfxGetArrayUnit(viewports, i);
                        pip->base.vpCnt++;
                    }
                }

                if (!err)
                {
                    afxArray const* scissors = &pipb->scissors;
                    afxNat scissorCnt = AfxGetArrayPop(scissors);
                    pip->base.scissorCnt = 0;
                    pip->base.scissors = NIL;

                    if (scissorCnt && !(pip->base.scissors = AfxAllocate(mem, sizeof(pip->base.scissors[0]) * scissorCnt, 0, AfxSpawnHint()))) AfxThrowError();
                    else
                    {
                        for (afxNat i = 0; i < scissorCnt; i++)
                        {
                            pip->base.scissors[pip->base.scissorCnt] = *(afxRect const*)AfxGetArrayUnit(scissors, i);
                            pip->base.scissorCnt++;
                        }
                    }

                    pip->glHandle = 0;
                    pip->updFlags = SGL_UPD_FLAG_DEVICE_INST;

                }
            }

            if (err && pip->base.wiring)
            {
                for (afxNat i = 0; i < pip->base.wiringCnt; i++)
                    AfxReleaseObjects(1, (void*[]) { pip->base.wiring[i].legt });

                AfxDeallocate(mem, pip->base.wiring);
            }
        }

        if (err)
        {
            for (afxNat i = 0; i < pip->base.shaderCnt; i++)
            {
                AfxReleaseObjects(1, (void*[]) { pip->base.shaders[i] });
            }

            AfxAssert(pip->base.shaders);
            AfxDeallocate(mem, pip->base.shaders);

            if (pip->base.vps)
                AfxDeallocate(mem, pip->base.vps);

            if (pip->base.scissors)
                AfxDeallocate(mem, pip->base.scissors);
        }
    }
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return err;
}

_SGL afxClassConfig _SglPipClsConfig =
{
    .fcc = AFX_FCC_PIP,
    .name = "Pipeline",
    .unitsPerPage = 4,
    .size = sizeof(AFX_OBJECT(afxPipeline)),
    .ctx = NIL,
    .ctor = (void*)_SglPipCtor,
    .dtor = (void*)_SglPipDtor
};
