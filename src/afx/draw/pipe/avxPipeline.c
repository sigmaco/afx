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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_PIPELINE_C
#define _AVX_RASTERIZER_C
#include "../dev/AvxDevKit.h"
#include "qwadro/draw/io/afxXsh.h"
#include "qwadro/draw/io/avxShaderBlueprint.h"

 // OpenGL/Vulkan Continuous Integration

_AVX avxTopology AfxGetPrimitiveTopology(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->primTop;
}

_AVX afxBool AfxPrimitiveRestartIsEnabled(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->primRestartEnabled;
}

_AVX afxBool AfxDepthClampIsEnabled(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->depthClampEnabled;
}

_AVX avxCullMode AfxGetPrimitiveCullingMode(avxPipeline pip, afxBool* frontFacingInverted) // return the culling mode set.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    avxCullMode cullMode = pip->cullMode;

    if (frontFacingInverted && cullMode)
        *frontFacingInverted = pip->frontFacingInverted;

    return cullMode;
}

_AVX afxNat AfxCountShaderSlots(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->stageCnt;
}

_AVX afxBool AfxGetPipelineLigature(avxPipeline pip, avxLigature* ligature)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    avxLigature liga = pip->liga;
    AfxTryAssertObjects(1, &liga, afxFcc_BSCH);
    *ligature = liga;
    return !!liga;
}

_AVX afxBool AfxGetPipelineVertexInput(avxPipeline pip, avxVertexInput* input)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    avxVertexInput vin = pip->vin;
    AfxTryAssertObjects(1, &vin, afxFcc_BSCH);
    *input = vin;
    return !!vin;
}

_AVX afxBool AfxGetPipelineStage(avxPipeline pip, avxShaderStage stage, avxShader* shader)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssertRange(avxShaderStage_TOTAL, stage, 1);
    AfxAssert(shader);
    afxBool rslt = FALSE;

    switch (stage)
    {
    case avxShaderStage_COMPUTE: *shader = pip->stageCnt ? pip->stages[0].shd : NIL; break;
    case avxShaderStage_VERTEX: *shader = (pip->vtxShdIdx != AFX_INVALID_INDEX) ? pip->stages[pip->vtxShdIdx].shd : NIL; break;
    case avxShaderStage_TESS_EVAL: *shader = (pip->hulShdIdx != AFX_INVALID_INDEX) ? pip->stages[pip->hulShdIdx].shd : NIL; break;
    case avxShaderStage_TESS_CTRL: *shader = (pip->domShdIdx != AFX_INVALID_INDEX) ? pip->stages[pip->domShdIdx].shd : NIL; break;
    case avxShaderStage_PRIMITIVE: *shader = (pip->geoShdIdx != AFX_INVALID_INDEX) ? pip->stages[pip->geoShdIdx].shd : NIL; break;
    case avxShaderStage_FRAGMENT: *shader = (pip->fshdIdx != AFX_INVALID_INDEX) ? pip->stages[pip->fshdIdx].shd : NIL; break;
    default: AfxThrowError(); break;
    }
    return rslt;
}

_AVX afxNat AfxGetLinkedShaders(avxPipeline pip, afxIndex first, afxNat cnt, avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssertRange(pip->stageCnt, first, cnt);
    AfxAssert(cnt);
    AfxAssert(shaders);
    afxNat hitCnt = 0;
    cnt = AfxMin(pip->stageCnt, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        shaders[i] = pip->stages[i].shd;
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxError _AvxPipStdDtor(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = AfxGetObjectProvider(pip);

    if (pip->stages)
    {
        for (afxNat i = pip->stageCnt; i-- > 0;)
            AfxReleaseObjects(1, &pip->stages[i].shd);

        AfxDeallocate(pip->stages);
    }

    if (pip->liga)
        AfxReleaseObjects(1, &pip->liga);

    if (pip->vin)
        AfxReleaseObjects(1, &pip->vin);

    return err;
}

_AVX afxError _AvxPipStdCtor(avxPipeline pip, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = cookie->udd[0];
    avxPipelineBlueprint const *pipb = ((avxPipelineBlueprint const*)cookie->udd[1]) + cookie->no;
    afxBool additiveStage = !!cookie->udd[2];
    //AfxAssertType(pipb, afxFcc_PIPB);

    // GRAPHICS STATE SETTING

    pip->primTop = pipb->primTop;
    pip->patchControlPoints = pipb->patchControlPoints;
    pip->cullMode = pipb->cullMode;
    pip->vpCnt = AfxClamp(pipb->vpCnt, 1, 8);

    pip->vtxShdIdx = AFX_INVALID_INDEX;
    pip->domShdIdx = AFX_INVALID_INDEX;
    pip->hulShdIdx = AFX_INVALID_INDEX;
    pip->geoShdIdx = AFX_INVALID_INDEX;
    pip->fshdIdx = AFX_INVALID_INDEX;

    afxNat listedShaderCnt = 0;
    avxShader listedShaders[10] = { 0 };

    pip->stages = NIL;
    afxNat stageCnt = pipb->shdCnt;
    AfxAssert(stageCnt);
    pip->stageCnt = 0;

    if (!(pip->stages = AfxAllocate(stageCnt, sizeof(pip->stages[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxNat stageIdx = AFX_INVALID_INDEX;
        afxNat shaderCnt = 0;

        for (afxNat i = 0; i < stageCnt; i++)
        {
            afxBool replace = FALSE;

            switch (pipb->shdStage[i])
            {
            case avxShaderStage_VERTEX:
            {
                AfxAssert(pip->vtxShdIdx == AFX_INVALID_INDEX);
                
                if (pip->vtxShdIdx == AFX_INVALID_INDEX)
                    pip->vtxShdIdx = (stageIdx = shaderCnt);
                else
                    stageIdx = pip->vtxShdIdx, replace = TRUE;

                break;
            }
            case avxShaderStage_TESS_EVAL:
            {
                AfxAssert(pip->hulShdIdx == AFX_INVALID_INDEX);

                if (pip->hulShdIdx == AFX_INVALID_INDEX)
                    pip->hulShdIdx = (stageIdx = shaderCnt);
                else
                    stageIdx = pip->hulShdIdx, replace = TRUE;
                    
                break;
            }
            case avxShaderStage_TESS_CTRL:
            {
                AfxAssert(pip->domShdIdx == AFX_INVALID_INDEX);

                if (pip->domShdIdx == AFX_INVALID_INDEX)
                    pip->domShdIdx = (stageIdx = shaderCnt);                    
                else
                    stageIdx = pip->domShdIdx, replace = TRUE;
                    
                break;
            }
            case avxShaderStage_PRIMITIVE:
            {
                AfxAssert(pip->geoShdIdx == AFX_INVALID_INDEX);

                if (pip->geoShdIdx == AFX_INVALID_INDEX)
                    pip->geoShdIdx = (stageIdx = shaderCnt);
                else
                    stageIdx = pip->geoShdIdx, replace = TRUE;

                break;
            }
            case avxShaderStage_FRAGMENT:
            {
                AfxAssert(pip->fshdIdx == AFX_INVALID_INDEX);

                if (pip->fshdIdx == AFX_INVALID_INDEX)
                    pip->fshdIdx = (stageIdx = shaderCnt);
                else
                    stageIdx = pip->fshdIdx, replace = TRUE;

                break;
            }
            default: AfxThrowError(); break;
            };

            if (!err)
            {
                if (pipb->shd[i])
                {
                    AfxAssertObjects(1, &pipb->shd[i], afxFcc_SHD);
                    AfxReacquireObjects(1, &pipb->shd[i]);

                    if (replace)
                        AfxReleaseObjects(1, &pip->stages[stageIdx].shd);

                    pip->stages[stageIdx].shd = pipb->shd[i];
                    pip->stages[stageIdx].stage = pipb->shdStage[i];
                    listedShaders[listedShaderCnt] = pip->stages[stageIdx].shd;
                    listedShaderCnt++;
                    shaderCnt++;
                    pip->stageCnt++;
                }
                else
                {
                    if (!AfxUriIsBlank(&pipb->shdUri[i]))
                    {
                        if (AfxCompileShadersFromFile(dctx, 1, &pipb->shdUri[i], &pip->stages[stageIdx].shd)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &pip->stages[stageIdx].shd, afxFcc_SHD);
                            
                            if (replace)
                                AfxReleaseObjects(1, &pip->stages[stageIdx].shd);

                            pip->stages[stageIdx].stage = pipb->shdStage[i];
                            listedShaders[listedShaderCnt] = pip->stages[stageIdx].shd;
                            listedShaderCnt++;
                            shaderCnt++;
                            pip->stageCnt++;
                        }
                    }
                }

                AfxMakeString8(&pip->stages[stageIdx].fn, NIL);

                if (!AfxStringIsEmpty(&pipb->shdFn[i]))
                {
                    AfxAssertObjects(1, &pip->stages[stageIdx].shd, afxFcc_SHD);
                    afxStringBase strb;

                    if (!AfxGetShaderStringBase(pip->stages[stageIdx].shd, &strb)) AfxThrowError();
                    else
                    {
                        if (!AfxCatalogStrings2(strb, 1, &pipb->shdFn[i], &pip->stages[stageIdx].fn.str.str))
                            AfxThrowError();
                    }
                }
            }
        }

        if (!err)
        {
            AfxAssert(pip->stageCnt == stageCnt);

            if (!err)
            {
                avxLigature liga;
                avxLigatureConfig ligc = { 0 };
                ligc.shaderCnt = listedShaderCnt;
                ligc.shaders = listedShaders;

                if (AfxBuildLigatures(dctx, 1, &ligc, &liga)) AfxThrowError();
                else
                {
                    pip->liga = liga;

                    if ((pip->vin = pipb->vin))
                        AfxReacquireObjects(1, &pip->vin);

                    if (err && liga)
                        AfxReleaseObjects(1, &liga);
                }
            }
        }

        if (err && pip->stages)
        {
            for (afxNat i = pip->stageCnt; i--> 0;)
                AfxReleaseObjects(1, &pip->stages[i].shd);

            AfxAssert(pip->stages);
            AfxDeallocate(pip->stages);
        }
    }
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return err;
}

_AVX afxClassConfig const _AvxPipStdImplementation =
{
    .fcc = afxFcc_PIP,
    .name = "Pipeline",
    .desc = "Draw Device Execution Pipeline",
    .fixedSiz = sizeof(AFX_OBJECT(avxPipeline)),
    .ctor = (void*)_AvxPipStdCtor,
    .dtor = (void*)_AvxPipStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, avxPipelineBlueprint const blueprints[], avxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetPipelineClass(dctx);
    AfxAssertClass(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dctx, (void*)blueprints }))
        AfxThrowError();

    return err;
}

_AVX avxPipeline AfxLoadPipelineFromXsh(afxDrawContext dctx, avxVertexInput vin, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    avxPipeline pip = NIL;

    avxPipelineBlueprint pipb = { 0 };
    pipb.cullMode = avxCullMode_BACK;
    pipb.primTop = avxTopology_TRI_LIST;
    pipb.vpCnt = 1;

    if (AfxParsePipelineFromXsh(&pipb, uri)) AfxThrowError();
    else
    {
        pipb.vin = vin;

        if (AfxAssemblePipelines(dctx, 1, &pipb, &pip)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &pip, afxFcc_PIP);
        }
    }
    return pip;
}
