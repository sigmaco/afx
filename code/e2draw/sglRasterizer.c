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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "qwadro/core/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/afxPipeline.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/core/afxUri.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxXsh.h"
// OpenGL/Vulkan Continuous Integration

_SGL afxError _SglDpuBindAndSyncRazr(sglDpuIdd* dpu, afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxBool rasterizationDisabled = !razr;

    AfxAssert(razr);

    afxRasterizationConfig config;
    AfxDescribeRasterizerConfiguration(razr, &config);

    if (!razr)
    {

    }
    else
    {
        AfxAssertObjects(1, &razr, afxFcc_RAZR);
        GLuint glHandleFragShdProg = razr->glHandleFragShdProg;
        sglUpdateFlags devUpdReq = (razr->updFlags & SGL_UPD_FLAG_DEVICE);

        if ((!glHandleFragShdProg) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandleFragShdProg)
            {
                gl->DeleteProgram(glHandleFragShdProg); _SglThrowErrorOccuried();
                glHandleFragShdProg = NIL;
            }

            //afxArray code;
            //AfxAllocateArray(&code, 2048, sizeof(afxChar), NIL);
            //AfxLoadGlScript(&code, &razr->base.fragShd.uri);
            afxShaderBlueprint shdb;
            _SglLoadShaderBlueprint(&shdb, &razr->base.fragShd.uri);

            if (gl->CreateShaderProgramv)
            {
                //if (!(glHandleFragShdProg = gl->CreateShaderProgramv(GL_FRAGMENT_SHADER, 1, (GLchar const*const[]) { (void*)code.bytemap })))
                if (!(glHandleFragShdProg = gl->CreateShaderProgramv(GL_FRAGMENT_SHADER, 1, (GLchar const*const[]) { (void*)shdb.codes.bytemap })))
                {
                    _SglThrowErrorOccuried();
                }
            }
            else
            {
                GLuint shader;

                if (!(shader = gl->CreateShader(GL_FRAGMENT_SHADER)))
                {
                    _SglThrowErrorOccuried();
                }
                else
                {
                    afxChar errStr[1024];
                    GLint compiled = 0;
                    //gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _SglThrowErrorOccuried();
                    gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)shdb.codes.bytemap }, (GLint const[]) { shdb.codes.cnt }); _SglThrowErrorOccuried();
                    gl->CompileShader(shader); _SglThrowErrorOccuried();
                    gl->GetShaderiv(shader, GL_COMPILE_STATUS, &compiled); _SglThrowErrorOccuried();

                    if (compiled == GL_FALSE)
                    {
                        AfxThrowError();
                        gl->GetShaderInfoLog(shader, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                        AfxError(errStr);
                    }
                    else
                    {
                        GLuint program;

                        if (!(program = gl->CreateProgram()))
                        {
                            _SglThrowErrorOccuried();
                        }
                        else
                        {
                            gl->ProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE); _SglThrowErrorOccuried();
                            afxBool linked;
                            gl->AttachShader(program, shader); _SglThrowErrorOccuried();
                            gl->LinkProgram(program); _SglThrowErrorOccuried();
                            gl->GetProgramiv(program, GL_LINK_STATUS, &linked); _SglThrowErrorOccuried();

                            if (linked == GL_FALSE)
                            {
                                AfxThrowError();
                                gl->GetProgramInfoLog(program, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                                AfxError(errStr);
                            }
                            gl->DetachShader(program, shader); _SglThrowErrorOccuried();
                        }
                        glHandleFragShdProg = program;
                    }
                    gl->DeleteShader(shader); _SglThrowErrorOccuried();
                }
            }

#if 0
            afxShader fsh;
            AfxFindLinkedShader(pip, afxShaderStage_FRAGMENT, &fsh);
            afxNat outCnt = AfxCountColorOutputChannels(pip->base.razr);
            //vtxShd->base.ioDecls.
            for (afxNat i = 0; i < outCnt; i++)
            {
                afxColorOutputChannel out;
                AfxGetColorOutputChannels(pip->base.razr, i, 1, &out);
                //gl->BindFragDataLocationIndexed(glHandle, i, fsh->base.ioDecls[i].location, AfxGetBufferedStringData(&(fsh->base.ioDecls[i].semantic), 0)); _SglThrowErrorOccuried();
            }
#endif


            //AfxEcho("%.*s", code.cnt, code.bytemap);
            AfxEcho("%.*s", shdb.codes.cnt, shdb.codes.bytemap);
            //AfxDeallocateArray(&code);
            AfxShaderBlueprintEnd(&shdb, 0, NIL);

            AfxAssert(gl->IsProgram(glHandleFragShdProg));
            
            razr->glHandleFragShdProg = glHandleFragShdProg;
            razr->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
        }
    }
    return err;
}

_SGL afxError _SglRazrDtor(afxRasterizer razr)
{
    AfxEntry("razr=%p", razr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    afxDrawContext dctx = AfxGetObjectProvider(razr);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (razr->base.sampleMasks)
        AfxDeallocate(mmu, razr->base.sampleMasks);

    if (razr->base.outs)
        AfxDeallocate(mmu, razr->base.outs);

    return err;
}

_SGL afxError _SglRazrCtor(afxRasterizer razr, afxCookie const* cookie)
{
    AfxEntry("razr=%p", razr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    afxDrawContext dctx = cookie->udd[0];
    afxRasterizationConfig const *rasc = ((afxRasterizationConfig const*)cookie->udd[1]) + cookie->no;
    //AfxAssertType(rasc, afxFcc_RASB);

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxRasterizationFlags rasFlags = rasc->rasFlags;
    razr->base.rasFlags = NIL;

    razr->base.fillMode = rasc->fillMode;
    razr->base.lineWidth = rasc->lineWidth;

    razr->base.depthBiasEnabled = !!rasc->depthBiasEnabled;
    razr->base.depthBiasSlopeScale = rasc->depthBiasSlopeScale;
    razr->base.depthBiasConstFactor = rasc->depthBiasConstFactor;
    razr->base.depthBiasClamp = rasc->depthBiasClamp;

    afxMultisamplingFlags msFlags = rasc->msFlags;
    razr->base.msFlags = NIL;

    razr->base.msEnabled = !!rasc->msEnabled;
    razr->base.minSampleShadingValue = rasc->minSampleShadingValue;

    razr->base.sampleCnt = rasc->sampleCnt;
    razr->base.sampleMasks = NIL;

    if (razr->base.sampleCnt && !(razr->base.sampleMasks = AfxAllocate(mmu, sizeof(razr->base.sampleMasks[0]), razr->base.sampleCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < razr->base.sampleCnt; i++)
            razr->base.sampleMasks[i] = rasc->sampleMasks[i];
    }

    if (!err)
    {
        afxDepthStencilFlags dsFlags = rasc->dsFlags;
        razr->base.dsFlags = NIL;

        razr->base.depthTestEnabled = !!rasc->depthTestEnabled;
        razr->base.depthCompareOp = rasc->depthCompareOp;
        razr->base.depthWriteEnabled = !!rasc->depthWriteEnabled;

        razr->base.stencilTestEnabled = !!rasc->stencilTestEnabled;
        razr->base.stencilFront = rasc->stencilFront;
        razr->base.stencilBack = rasc->stencilBack;
        
        razr->base.depthBoundsTestEnabled = !!rasc->depthBoundsTestEnabled;
        AfxCopyV2d(razr->base.depthBounds, rasc->depthBounds);

        razr->base.dsFmt = rasc->dsFmt; // ?

        afxColorOutputFlags pixelFlags = rasc->pixelFlags;
        razr->base.pixelFlags = NIL;

        razr->base.outCnt = rasc->colorOutCnt;
        razr->base.outs = NIL;

        if (razr->base.outCnt && !(razr->base.outs = AfxAllocate(mmu, sizeof(razr->base.outs[0]), razr->base.outCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < razr->base.outCnt; i++)
                razr->base.outs[i] = rasc->colorOuts[i];

            // deveria ser só o blend/write, já que só podemos determinar as saídas quando assembleado com fragment shaders enquanto pipeline completo.
        }

        if (!err)
        {
            AfxCopyV4d(razr->base.blendConstants, rasc->blendConstants);

            razr->base.logicOpEnabled = !!rasc->pixelLogicOpEnabled;
            razr->base.logicOp = rasc->pixelLogicOp;

            AfxMakeFixedUri128(&razr->base.fragShd, &rasc->fragShd);
            AfxMakeFixedString8(&razr->base.fragFn, &rasc->fragFn);

            razr->updFlags = SGL_UPD_FLAG_DEVICE_INST;
            
            if (err)
            {
                if (razr->base.outs)
                    AfxDeallocate(mmu, razr->base.outs);
            }
        }

        if (err)
        {
            if (razr->base.sampleMasks)
                AfxDeallocate(mmu, razr->base.sampleMasks);
        }
    }
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return err;
}

_SGL afxClassConfig _SglRazrClsConfig =
{
    .fcc = afxFcc_RAZR,
    .name = "Rasterizer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxRasterizer)),
    .mmu = NIL,
    .ctor = (void*)_SglRazrCtor,
    .dtor = (void*)_SglRazrDtor
};
