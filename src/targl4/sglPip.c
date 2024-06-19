/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/io/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/avxPipeline.h"
#include "qwadro/draw/io/avxShaderBlueprint.h"
#include "qwadro/draw/io/afxXsh.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/exec/afxSystem.h"
// OpenGL/Vulkan Continuous Integration

#define _AFX_DBG_IGNORE_PRIM_RESTART
#define _AFX_DBG_IGNORE_DEPTH_CLAMP

////////////////////////////////////////////////////////////////////////////////
// DRAW PIPELINE                                                              //
////////////////////////////////////////////////////////////////////////////////

#if 0
_SGL afxResult _AfxRegisterOpenGlResourcesToQwadroDrawPipeline(avxPipeline pip)
{
    glVmt* gl = &(((afxDrawSystem)AfxGetObjectProvider(&pip->obj))->vmt);
#if 0
    avxShaderStage                      stages;
    avxShaderResourceType              type;
    avxPipelineSubresType           baseType;
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
    avxPipelineSubresource const    *subresources;
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
        AfxLogEcho("%d %d %d %d", res.loc, res.binding, i, j);
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
                AfxLogError("%X is a unhandled type", type);
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
                    AfxLogEcho("%d %d %d %d", res.location, res.binding, i, j);
                }
            }
        }
    }
    return 0;
}
#endif

_SGL afxError _SglLoadShaderBlueprint(avxShaderBlueprint* shdb, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    afxUri fext;
    AfxClipUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxClipUriPath(&fpath, uri);

        if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxString query;
                AfxGetUriQueryString(uri, TRUE, &query);

                afxNat xmlElemIdx = 0;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Shader"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (foundCnt)
                {
                    AfxShaderBlueprintBegin(shdb, NIL, NIL, NIL, 0, 0, 0);

                    if (AfxParseXmlBackedShaderBlueprint(shdb, 0, &xml, xmlElemIdx)) AfxThrowError();
                    else
                    {

                    }
                    //AfxShaderBlueprintEnd(shdb, 0, NIL);
                }
                AfxCleanUpXml(&xml);
            }
        }
    }
    return err;
}

_SGL afxError _DpuBindAndSyncPip(sglDpu* dpu, afxBool bind, afxBool sync, avxPipeline pip)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (!pip)
    {
        if (bind)
        {
            //gl->BindProgramPipeline(0); _SglThrowErrorOccuried();
            gl->UseProgram(0); _SglThrowErrorOccuried();
            //gl->BindVertexArray(0); _SglThrowErrorOccuried();
        }
    }
    else
    {
        AfxAssertObjects(1, &pip, afxFcc_PIP);
        GLuint glHandle = pip->glHandle;
        sglUpdateFlags devUpdReq = (pip->updFlags & SGL_UPD_FLAG_DEVICE);
        afxBool bound = FALSE;

        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteProgram(glHandle); _SglThrowErrorOccuried();
                glHandle = NIL;
            }

            //afxArray code;
            afxChar errStr[1024];
            afxNat tmpShdGlHandleCnt = 0;
            GLuint tmpShdGlHandles[5];
            //AfxAllocateArray(&code, 2048, sizeof(afxChar), NIL);

            afxArray code;
            AfxAllocateArray(&code, 4096, sizeof(afxByte), NIL);

            for (afxNat stageIdx = 0; stageIdx < pip->m.stageCnt; stageIdx++)
            {
                AfxEmptyArray(&code);
                AfxDumpShaderCode(pip->m.stages[stageIdx].shd, &code);

                avxShaderStage stage = pip->m.stages[stageIdx].stage;

                GLuint shader;

                if (!(shader = gl->CreateShader(AfxToGlShaderStage(stage))))
                {
                    _SglThrowErrorOccuried();
                }
                else
                {
                    GLint compiled = 0;
                    //gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _SglThrowErrorOccuried();
                    AfxLogComment("%.*s", code.cnt, code.bytemap);
                    gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _SglThrowErrorOccuried();
                    gl->CompileShader(shader); _SglThrowErrorOccuried();
                    gl->GetShaderiv(shader, GL_COMPILE_STATUS, &compiled); _SglThrowErrorOccuried();

                        
                    if (compiled == GL_FALSE)
                    {
                        AfxThrowError();
                        gl->GetShaderInfoLog(shader, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                        AfxLogError(errStr);
                        gl->DeleteShader(shader); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        tmpShdGlHandles[tmpShdGlHandleCnt] = shader;
                        tmpShdGlHandleCnt++;
                    }
                }

                if (err)
                {
                    for (afxNat i = tmpShdGlHandleCnt; i-- > 0;)
                    {
                        gl->DeleteShader(tmpShdGlHandles[tmpShdGlHandleCnt]); _SglThrowErrorOccuried();
                        tmpShdGlHandles[tmpShdGlHandleCnt] = NIL;
                    }
                    break;
                }
            }

            if (!err)
            {
#if 0
                avxShader shd;
                avxRasterizer razr;

                if (AfxGetLinkedRasterizer(pip, &razr))
                {
                    if (AfxGetFragmentShader(razr, &shd, NIL))
                    {
                        GLuint shader;

                        if (!(shader = gl->CreateShader(GL_FRAGMENT_SHADER)))
                        {
                            _SglThrowErrorOccuried();
                        }
                        else
                        {
                            AfxEmptyArray(&code);
                            AfxDumpShaderCode(shd, &code);

                            GLint compiled = 0;
                            //gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _SglThrowErrorOccuried();
                            AfxLogComment("%.*s", code.cnt, code.bytemap);
                            gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _SglThrowErrorOccuried();
                            gl->CompileShader(shader); _SglThrowErrorOccuried();
                            gl->GetShaderiv(shader, GL_COMPILE_STATUS, &compiled); _SglThrowErrorOccuried();

                                
                            if (compiled == GL_FALSE)
                            {
                                AfxThrowError();
                                gl->GetShaderInfoLog(shader, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                                AfxLogError(errStr);
                                gl->DeleteShader(shader); _SglThrowErrorOccuried();
                            }
                            else
                            {
                                tmpShdGlHandles[tmpShdGlHandleCnt] = shader;
                                tmpShdGlHandleCnt++;
                            }
                        }
                    }
                }
#endif
            }

            AfxDeallocateArray(&code);

            if (!err)
            {
                if (!(glHandle = gl->CreateProgram()))
                {
                    _SglThrowErrorOccuried();
                }
                else
                {
                    afxBool linked;

                    for (afxNat i = 0; i < tmpShdGlHandleCnt; i++)
                    {
                        gl->AttachShader(glHandle, tmpShdGlHandles[i]); _SglThrowErrorOccuried();
                    }

                    gl->LinkProgram(glHandle); _SglThrowErrorOccuried();
                    gl->GetProgramiv(glHandle, GL_LINK_STATUS, &linked); _SglThrowErrorOccuried();

                    if (linked == GL_FALSE)
                    {
                        AfxThrowError();
                        gl->GetProgramInfoLog(glHandle, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                        AfxLogError(errStr);
                    }

                    // required bind due to issue with Intel Graphics Drivers no allowing retrieve of uniform locations after assembling.
                    AfxAssert(gl->IsProgram(glHandle));
                    gl->UseProgram(glHandle); _SglThrowErrorOccuried();
                    bound = TRUE;

                    if (_DpuBindAndResolveLego(dpu, pip->m.liga, glHandle))
                        AfxThrowError();

                    for (afxNat i = tmpShdGlHandleCnt; i-- > 0;)
                    {
                        gl->DetachShader(glHandle, tmpShdGlHandles[i]); _SglThrowErrorOccuried();
                    }

                    if (err)
                    {
                        gl->DeleteProgram(glHandle); _SglThrowErrorOccuried();
                        glHandle = NIL;
                    }
                }

                for (afxNat i = tmpShdGlHandleCnt; i-- > 0;)
                {
                    gl->DeleteShader(tmpShdGlHandles[i]); _SglThrowErrorOccuried();
                    tmpShdGlHandles[i] = NIL;
                }
            }

            pip->glHandle = glHandle;

            if (!err)
            {
                AfxLogEcho("avxPipeline %p hardware-side data instanced.", pip);
                pip->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
            }
        }
        else
        {
            AfxAssert(gl->IsProgram(glHandle));
            gl->UseProgram(glHandle); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _SglPipDtor(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = AfxGetObjectProvider(pip);

    if (pip->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 5, (void*)pip->glHandle);
        pip->glHandle = 0;
    }

    if (_AvxPipStdImplementation.dtor(pip))
        AfxThrowError();

    return err;
}

_SGL afxError _SglPipCtor(avxPipeline pip, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (_AvxPipStdImplementation.ctor(pip, cookie)) AfxThrowError();
    else
    {
        afxDrawContext dctx = cookie->udd[0];
        avxPipelineBlueprint const *pipb = ((avxPipelineBlueprint const*)cookie->udd[1]) + cookie->no;
        //AfxAssertType(pipb, afxFcc_PIPB);

        pip->glHandle = 0;
        pip->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (err && _AvxPipStdImplementation.dtor(pip))
            AfxThrowError();
    }
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return err;
}
