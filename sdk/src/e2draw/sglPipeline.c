
#define _AFX_IOS_IMPL
#define _AFX_RES_IMPL
#define _AFX_PIP_IMPL
#include "afx/core/io/afxUrd.h"
#include "afx/draw/io/afxUniformPipelineDescriptor.h"
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
    sglVmt* gl = &(((afxDrawSystem)AfxObjectGetProvider(&pip->obj))->vmt);
#if 0
    afxShaderStage                      stages;
    afxPipelineAccessType              type;
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
        res.resType = AFX_SUPPLY_TYPE_CONSTANT_BUFFER;
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
                res.resType = AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_2D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_3D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_CUBE:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER;
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

_SGL afxError _SglPipSyncAndBind(afxPipeline pip, afxDrawEngine deng)
{
    //AfxEntry("pip=%p", pip);
    afxError err = NIL;
    sglVmt const* gl = &deng->wglVmt;

    if (pip)
    {
        AfxAssertObject(pip, AFX_FCC_PIP);

        GLuint glHandle = pip->glHandle[deng->queueIdx];

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

                    for (afxNat i = 0; i < pip->stageCnt; i++)
                    {
                        afxConnection *objc = &pip->stages[i].shader;
                        afxPipelineModule pipm = AfxConnectionGetObject(objc);

                        if (pipm)
                        {
                            _SglPipmSync(pipm, pip->stages[i].type, deng);
                            AfxAssert(gl->IsShader(pipm->glHandle));
                            gl->AttachShader(glHandle, pipm->glHandle); _SglThrowErrorOccuried();
                        }
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
                        AfxEcho("Draw pipeline %p reinstanced.", pip);
                        pip->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
                    }
                }
                pip->glHandle[deng->queueIdx] = glHandle;
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

                    afxNat loc = gl->GetUniformLocation(glHandle, (void const *)AfxStringGetDataConst(&entry->name.str)); _SglThrowErrorOccuried();

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

                        gl->UniformBlockBinding(glHandle, gl->GetUniformBlockIndex(glHandle, (void const *)AfxStringGetDataConst(&entry->name.str)), binding);

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
    }
    else
    {
        gl->UseProgram(0); _SglThrowErrorOccuried();
    }
    return err;
}

_SGL afxResult _AfxPipelineForEachColorBlendAnnex(afxPipeline pip, afxResult(*f)(afxColorBlendAnnex const*, void*), void *data)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(f);
    afxResult cnt = 0;

    for (afxNat i = 0; i < pip->colorBlending.annexCnt; i++)
    {
        afxColorBlendAnnex const *annex = &pip->colorBlending.annexes[i];
        cnt++;

        if (!f(annex, data))
            break;
    }
    return cnt;
}

_SGL afxBool _AfxPipelineHasColorBlending(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasColorBlending;
}

_SGL afxBool _AfxPipelineHasMultisampling(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasMultisampling;
}

_SGL afxBool _AfxPipelineHasDepthHandling(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasDepthHandling;
}

_SGL afxBool _AfxPipelineHasRasterization(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasRasterization;
}

_SGL afxPipelineRig _AfxPipelineGetRig(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);

    afxPipelineRig pipr = AfxLinkageGetOwner(&pip->rig);
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    return pipr;
}

_SGL afxResult _AfxPipelineForEachScissor(afxPipeline pip, afxNat first, afxNat cnt, afxResult(*f)(afxRect const*, void*), void *data)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(cnt <= pip->scissorCnt - first);
    AfxAssert(f);
    afxResult cnt2 = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxRect const *rect = &pip->scissors[first + i];
        cnt2++;

        if (!f(rect, data))
            break;
    }
    return cnt2;
}

_SGL afxResult _AfxPipelineGetScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[])
{
    afxError err = NIL;
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
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->scissorCnt;
}

_SGL afxResult _AfxPipelineForEachViewport(afxPipeline pip, afxNat first, afxNat cnt, afxResult(*f)(afxViewport const*, void*), void *data)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(cnt <= pip->vpCnt - first);
    AfxAssert(f);
    afxResult cnt2 = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxViewport const *vp = &pip->vps[first + i];
        cnt2++;

        if (!f(vp, data))
            break;
    }
    return cnt2;
}

_SGL afxResult _AfxPipelineGetViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[])
{
    afxError err = NIL;
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
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->vpCnt;
}

_SGL afxBool _AfxPipelineHasInputAssembling(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasVtxInAssembling;
}

_SGL afxResult _AfxPipelineForEachInputStream(afxPipeline pip, afxNat first, afxNat cnt, afxResult(*f)(afxPipelineInputStream const*, void*), void *data)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(cnt <= pip->inStreamCnt - first);
    AfxAssert(f);
    afxResult cnt2 = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxPipelineInputStream const *layout = &pip->inStreams[first + i];
        cnt2++;

        if (!f(layout, data))
            break;
    }
    return cnt2;
}

_SGL afxResult _AfxPipelineGetInputStreams(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputStream streams[])
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(streams);
    AfxAssert(cnt <= pip->inStreamCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxCopy(&streams[i], &pip->inStreams[first + i], sizeof(streams[0]));
        hitCnt++;
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineGetInputStreamCount(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->inStreamCnt;
}

_SGL afxResult _AfxPipelineForEachStage(afxPipeline pip, afxNat first, afxNat cnt, afxResult(*f)(afxPipelineModule pipm, void *data), void *data)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(cnt <= pip->stageCnt - first);
    AfxAssert(f);
    afxResult cnt2 = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxConnection *objc = &pip->stages[first + i].shader;
        AfxAssertConnection(objc);
        afxPipelineModule pipm = AfxConnectionGetObject(objc);

        if (pipm)
        {
            AfxAssertObject(pipm, AFX_FCC_PIPM);
            ++cnt2;

            if (!f(pipm, data))
                break;
        }
    }
    return cnt2;
}

_SGL afxResult _AfxPipelineGetModules(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineModule pipm[])
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(pipm);
    AfxAssert(cnt <= pip->stageCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxConnection *objc = &pip->stages[first + i].shader;
        AfxAssertConnection(objc);
        pipm[i] = AfxConnectionGetObject(objc);
        hitCnt++;
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineGetStageCount(afxPipeline pip)
{
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->stageCnt;
}

_SGL void _AfxPipShdrHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = NIL;
    afxPipeline pip = (void*)obj;
    AfxAssertObject(pip, AFX_FCC_PIP);
    AfxAssertObject(ev->obj, AFX_FCC_PIPM);
    //AfxAssertConnection(&dout->din);
    //AfxAssert(AfxConnectionGetObject(&dout->din) == ev->obj);

    if (ev->type == AFX_EVENT_OBJ_DESTROYED)
    {
        AfxError("Shader %p attached to draw pipeline %p was destroyed.", ev->obj, obj);
        while (0 < AfxObjectRelease(&pip->res.obj));
    }
}

_SGL afxError _AfxPipDtor(afxPipeline pip)
{
    AfxEntry("pip=%p", pip);
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);

    afxDrawContext dctx = AfxPipelineGetContext(pip);

    for (afxNat i = 0; i < dctx->queueCnt; i++)
    {
        if (pip->glHandle[i])
        {
            _SglEnqueueGlResourceDeletion(dctx, i, 5, pip->glHandle[i]);
            pip->glHandle[i] = 0;
        }
    }

    for (afxNat i = 0; i < pip->stageCnt; i++)
    {
        afxConnection *objc = &pip->stages[i].shader;
        AfxAssertConnection(objc);
        afxPipelineModule pipm = AfxConnectionGetObject(objc);

        if (pipm)
        {
            AfxAssertObject(pipm, AFX_FCC_PIPM);
            AfxConnectionDrop(objc);
        }
    }

    afxPipelineRig pipr = AfxLinkageGetOwner(&pip->rig);
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    AfxLinkageDrop(&pip->rig);
    AfxObjectRelease(&pipr->obj);

    return err;
}

_SGL afxPipImpl const _AfxStdPipImpl;

afxPipImpl const _AfxStdPipImpl =
{
    _AfxPipelineForEachColorBlendAnnex,
    _AfxPipelineForEachScissor,
    _AfxPipelineForEachStage,
    _AfxPipelineForEachInputStream,
    _AfxPipelineForEachViewport,
    _AfxPipelineGetScissorCount,
    _AfxPipelineGetScissors,
    _AfxPipelineGetInputStreamCount,
    _AfxPipelineGetInputStreams,
    _AfxPipelineGetModules,
    _AfxPipelineGetRig,
    _AfxPipelineGetStageCount,
    _AfxPipelineGetViewportCount,
    _AfxPipelineGetViewports,
    _AfxPipelineHasColorBlending,
    _AfxPipelineHasDepthHandling,
    _AfxPipelineHasMultisampling,
    _AfxPipelineHasRasterization,
    _AfxPipelineHasInputAssembling








};

_SGL afxError _AfxPipCtor(afxPipeline pip, afxPipelineParadigm const *paradigm)
{
    AfxEntry("pip=%p", pip);
    afxError err = NIL;
    AfxAssertObject(pip, AFX_FCC_PIP);
    afxPipelineBlueprint const *pipb = paradigm->blueprint;
    AfxAssertType(pipb, AFX_FCC_PIPB);

    AfxAssert(pipb->stageCnt);
    pip->stageCnt = 0;

    for (afxNat i = 0; i < pipb->stageCnt; i++)
    {
        afxConnection *objc = &pip->stages[i].shader;
        AfxConnectionReset(objc);
        AfxAssertConnection(objc);

        afxPipelineStageBlueprint const *sb = &pipb->stages[i];
        AfxAssert(sb->stage);

        pip->stages[i].type = sb->stage;
        afxPipelineModule pipm = NIL;

        if (sb->srcType == 0)
            pipm = sb->pipm;
        else if (sb->srcType == 1)
            pipm = AfxDrawContextFetchPipelineModule(paradigm->dctx, &sb->uri.uri);
        else if (sb->srcType == 2)
            pipm = AfxDrawContextBuildPipelineModule(paradigm->dctx, sb->code.str._bytemap, sb->code.str._range);
        else AfxThrowError();

        if (!pipm) AfxThrowError();
        else
        {
            AfxAssertObject(pipm, AFX_FCC_PIPM);

            if (1 == AfxObjectGetRefCount(&pipm->obj))
            {
                if (!(AfxObjectConnect(&(pipm->obj), &pip->res.obj, _AfxPipShdrHandler, NIL, objc)))
                {
                    AfxThrowError();
                    AfxObjectRelease(&pipm->obj);
                    break;
                }
            }
            else
            {
                if (!(AfxObjectReacquire(&(pipm->obj), &pip->res.obj, _AfxPipShdrHandler, NIL, objc)))
                {
                    AfxThrowError();
                    break;
                }
            }

            if (!err)
                pip->stageCnt++;
        }

        if (err)
            break;
    }

    if (!err)
    {
        AfxAssert(pip->stageCnt == pipb->stageCnt);

        pip->inStreamCnt = 0;

        for (afxNat i = 0; i < pipb->inStreamCnt; i++)
        {
            pip->inStreams[i].location = pipb->inStreams[i].location;
            pip->inStreams[i].binding = pipb->inStreams[i].binding;
            pip->inStreams[i].format = pipb->inStreams[i].format;
            pip->inStreams[i].offset = pipb->inStreams[i].offset;
            pip->inStreamCnt++;
        }

        AfxAssert(pip->inStreamCnt == pipb->inStreamCnt);

        if (!(pip->hasVtxInAssembling = pipb->hasAssembling))
            AfxZero(&pip->vtxInAssembling, sizeof(pip->vtxInAssembling));
        else
        {
            pip->vtxInAssembling.topology = pipb->inAssembling.topology;
            pip->vtxInAssembling.primRestartEnable = pipb->inAssembling.primRestartEnable;
        }

        if (!(pip->hasRasterization = pipb->hasRasterization))
            AfxZero(&pip->rasterization, sizeof(pip->rasterization));
        else
        {
            pip->rasterization.depthClampEnable = pipb->rasterization.depthClampEnable;
            pip->rasterization.rasterizerDiscardEnable = pipb->rasterization.rasterizerDiscardEnable;
            pip->rasterization.fillMode = pipb->rasterization.fillMode;
            pip->rasterization.cullMode = pipb->rasterization.cullMode;
            pip->rasterization.frontFace = pipb->rasterization.frontFace;
            pip->rasterization.depthBiasEnable = pipb->rasterization.depthBiasEnable;
            pip->rasterization.depthBiasConstantFactor = pipb->rasterization.depthBiasConstantFactor;
            pip->rasterization.depthBiasClamp = pipb->rasterization.depthBiasClamp;
            pip->rasterization.depthBiasSlopeFactor = pipb->rasterization.depthBiasSlopeFactor;
            pip->rasterization.lineWidth = pipb->rasterization.lineWidth;
        }

        if (!(pip->hasMultisampling = pipb->hasMultisampling))
            AfxZero(&pip->multisampling, sizeof(pip->multisampling));
        else
        {
            pip->multisampling.rasterSamples = pipb->multisampling.rasterSamples;
            pip->multisampling.sampleShadingEnable = pipb->multisampling.sampleShadingEnable;
            pip->multisampling.minSampleShading = pipb->multisampling.minSampleShading;
            pip->multisampling.pSampleMask = pipb->multisampling.pSampleMask;
            pip->multisampling.alphaToCoverageEnable = pipb->multisampling.alphaToCoverageEnable;
            pip->multisampling.alphaToOneEnable = pipb->multisampling.alphaToOneEnable;
        }

        if (!(pip->hasDepthHandling = pipb->hasDepthHandling))
            AfxZero(&pip->depthHandling, sizeof(pip->depthHandling));
        else
        {
            pip->depthHandling.depthTestEnable = pipb->depthHandling.depthTestEnable;
            pip->depthHandling.depthWriteEnable = pipb->depthHandling.depthWriteEnable;
            pip->depthHandling.depthCompareOp = pipb->depthHandling.depthCompareOp;
            pip->depthHandling.depthBoundsTestEnable = pipb->depthHandling.depthBoundsTestEnable;
            pip->depthHandling.stencilTestEnable = pipb->depthHandling.stencilTestEnable;
            pip->depthHandling.stencilOpFront.failOp = pipb->depthHandling.stencilOpFront.failOp;
            pip->depthHandling.stencilOpFront.passOp = pipb->depthHandling.stencilOpFront.passOp;
            pip->depthHandling.stencilOpFront.depthFailOp = pipb->depthHandling.stencilOpFront.depthFailOp;
            pip->depthHandling.stencilOpFront.compareOp = pipb->depthHandling.stencilOpFront.compareOp;
            pip->depthHandling.stencilOpBack.failOp = pipb->depthHandling.stencilOpBack.failOp;
            pip->depthHandling.stencilOpBack.passOp = pipb->depthHandling.stencilOpBack.passOp;
            pip->depthHandling.stencilOpBack.depthFailOp = pipb->depthHandling.stencilOpBack.depthFailOp;
            pip->depthHandling.stencilOpBack.compareOp = pipb->depthHandling.stencilOpBack.compareOp;
            pip->depthHandling.minDepthBounds = pipb->depthHandling.minDepthBounds;
            pip->depthHandling.maxDepthBounds = pipb->depthHandling.maxDepthBounds;
        }

        pip->scissorCnt = 0;

        for (afxNat i = 0; i < pipb->scissorCnt; i++)
        {
            pip->scissors[i] = pipb->scissors[i];
            ++pip->scissorCnt;
        }

        pip->vpCnt = 0;

        for (afxNat i = 0; i < pipb->vpCnt; i++)
        {
            pip->vps[i] = pipb->vps[i];
            ++pip->vpCnt;
        }

        if (!(pip->hasColorBlending = pipb->hasColorBlending))
            AfxZero(&pip->depthHandling, sizeof(pip->depthHandling));
        else
        {
            pip->colorBlending.logicOpEnable = pipb->colorBlending.logicOpEnable;
            pip->colorBlending.logicOp = pipb->colorBlending.logicOp;
            pip->colorBlending.annexCnt = pipb->colorBlending.annexCnt;

            for (afxNat i = 0; i < pipb->colorBlending.annexCnt; i++)
                AfxCopy(&pip->colorBlending.annexes[i], &pip->colorBlending.annexes[i], sizeof(pip->colorBlending.annexes[0]));

            AfxV4dCopy(pip->colorBlending.blendConstants, pipb->colorBlending.blendConstants);
        }

        afxPipelineRig pipr;

        if ((pipr = pipb->rig))
        {
            AfxAssertObject(pipr, AFX_FCC_PIPR);

            if (!AfxObjectReacquire(&pipr->obj, &pip->res.obj, NIL, NIL, NIL))
                AfxThrowError();
        }
        else
        {
            if (!(pipr = AfxDrawContextBuildPipelineRig(paradigm->dctx, pipb->socketCnt, pipb->sockets))) AfxThrowError();
            else
            {
                AfxAssertObject(pipr, AFX_FCC_PIPR);
            }
        }

        if (!err)
        {
            AfxLinkageDeploy(&pip->rig, &(pipr->pipelines));

            AfxZero(pip->glHandle, sizeof(pip->glHandle));
            pip->updFlags = SGL_UPD_FLAG_DEVICE_INST;
        }

        if (err)
        {
            pipr = AfxLinkageGetOwner(&pip->rig);
            AfxAssertObject(pipr, AFX_FCC_PIPR);
            AfxLinkageDrop(&pip->rig);
            AfxObjectRelease(&pipr->obj);
        }
    }

    if (err)
    {
        for (afxNat i = 0; i < pip->stageCnt; i++)
        {
            afxConnection *objc = &pip->stages[i].shader;
            AfxAssertConnection(objc);
            afxPipelineModule pipm = AfxConnectionGetObject(objc);
            AfxTryAssertObject(pipm, AFX_FCC_PIPM);
            AfxConnectionDrop(objc);
        }
    }
    return err;
}

_SGL afxPipeline _AfxDrawContextBuildPipeline(afxDrawContext dctx, afxPipelineBlueprint const *pipb)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxPipeline pip = NIL;

    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DCTX);

    afxPipelineParadigm paradigm =
    {
        .res = { AfxObjectGetProvider(&dsys->obj), NIL },
        dctx,
        pipb
    };

    if (!(pip = AfxObjectAcquire(AfxDrawContextGetPipelineClass(dctx), &paradigm, AfxSpawnHint())))
        AfxThrowError();

    return pip;
}

_SGL afxPipeline _AfxDrawContextUploadPipeline(afxDrawContext dctx, afxUri const *uri)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxUri fext;
    AfxUriForkExtension(&fext, uri);

    if (AfxUriIsBlank(uri)) AfxThrowError();
    else
    {
        afxPipeline pip = NIL;

        if (0 != AfxStringCompareRawCi(AfxUriGetStringConst(&fext), 0, 4, ".urd"))
        {
            AfxAdvertise("Uniform Resource Dictionary (.urd) is the only supported format.");
            AfxThrowError();
        }
        else
        {
            if (!(pip = _AfxDrawContextUploadPipelineUrd(dctx, uri))) AfxThrowError();
            else
            {
                return pip;
            }
        }
    }
    return NIL;
}

_SGL void _AfxIteratorCompareResourceNameCiPip(afxIterator *iter)
{
    struct { afxUri const *name; afxObject *obj; } *data = iter->udd;
    afxResource res = iter->voidItem;

    afxUri name;
    AfxResourceExcerptName(res, &name);

    if (!AfxUriIsBlank(&name))
    {
        if (0 == AfxStringCompareCi(AfxUriGetStringConst(data->name), AfxUriGetStringConst(&name)))
        {
            data->obj = &res->obj;
            iter->abort = TRUE;
        }
    }
}

_SGL afxPipeline _AfxDrawContextFindPipeline(afxDrawContext dctx, afxUri const *name)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(!AfxUriIsBlank(name));

    struct { afxUri const *name; afxObject *obj; } data = { name, NIL };
    AfxClassForEveryInstance(&dctx->pipClass, FALSE, _AfxIteratorCompareResourceNameCiPip, &data);

    if (data.obj)
        return (afxPipeline)data.obj;

    return NIL;
}

_SGL afxPipeline _AfxDrawContextFetchPipeline(afxDrawContext dctx, afxUri const *uri)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxUri name;
    AfxUriForkName(&name, uri);
    afxPipeline pip = AfxDrawContextFindPipeline(dctx, &name);

    if (pip) AfxObjectReacquire(&pip->res.obj, NIL, NIL, NIL, NIL);
    else
    {
        if (!(pip = AfxDrawContextUploadPipeline(dctx, uri)))
            AfxThrowError();
    }
    return pip;
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
    "Pipeline",
    &_AfxStdPipImpl
};
