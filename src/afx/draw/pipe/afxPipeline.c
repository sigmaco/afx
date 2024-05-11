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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_PIPELINE_C
#define _AVX_RASTERIZER_C
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/io/afxXsh.h"
#include "qwadro/draw/io/afxShaderBlueprint.h"

 // OpenGL/Vulkan Continuous Integration

_AVX afxPrimTopology AfxGetPrimitiveTopology(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->primTop;
}

_AVX afxBool AfxPrimitiveRestartIsEnabled(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->primRestartEnabled;
}

_AVX afxBool AfxDepthClampIsEnabled(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->depthClampEnabled;
}

_AVX afxCullMode AfxGetPrimitiveCullingMode(afxPipeline pip, afxBool* frontFacingInverted) // return the culling mode set.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    afxCullMode cullMode = pip->cullMode;

    if (frontFacingInverted && cullMode)
        *frontFacingInverted = pip->frontFacingInverted;

    return cullMode;
}

_AVX afxNat AfxGetPipelineInputs(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputLocation streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssertRange(pip->inCnt, first, cnt);
    AfxAssert(streams);
    AfxAssert(cnt);
    afxNat hitCnt = 0;

    afxPipelineInputLocation const* ins = pip->ins;
    afxNat cnt2 = AfxMin(pip->inCnt, cnt);

    for (afxNat i = 0; i < cnt2; i++)
    {
        AfxCopy2(1, sizeof(streams[0]), &ins[first + i], &streams[i]);
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxVertexInput AfxGetPipelineVertexInput(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    afxVertexInput vin = pip->vin;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin;
}

_AVX afxNat AfxCountPipelineInputs(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->inCnt;
}

_AVX afxBool AfxFindLinkedShader(afxPipeline pip, afxShaderStage stage, afxUri* shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssertRange(afxShaderStage_TOTAL, stage, 1);
    AfxAssert(shd);    

    for (afxNat i = 0; i < pip->stageCnt; i++)
    {
        if (pip->stages[i].stage == stage)
        {
            if (shd)
            {
                AfxReplicateUri(shd, &pip->stages[i].shd.uri);
            }
            return TRUE;
        }
    }
    return FALSE;
}

_AVX afxNat AfxGetLinkedShaders(afxPipeline pip, afxNat first, afxNat cnt, afxUri shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssert(cnt);
    AfxAssert(shd);
    AfxAssertRange(pip->stageCnt, first, cnt);
    afxNat hitCnt = 0;

    for (afxNat i = 0; i < AfxMin(pip->stageCnt, cnt); i++)
    {
        AfxReplicateUri(&shd[i], &pip->stages[first + i].shd.uri);
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxNat AfxCountLinkedShaders(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->stageCnt;
}

AVX afxNat AfxCountPipelineBindSchemas(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->wiringCnt;
}

AVX afxError AfxGetPipelineBindSchemas(afxPipeline pip, afxNat first, afxNat cnt, afxNat setIdx[], afxBindSchema bsch[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssertRange(pip->wiringCnt, first, cnt);
    AfxAssert(setIdx);
    AfxAssert(bsch);
    //afxBindSchema pipr = AfxGetLinker(&pip->pipr);
    //AfxAssertObject(pipr, afxFcc_RAZR);

    for (afxNat i = 0; i < AfxMin(pip->wiringCnt, cnt); i++)
    {
        afxNat schIdx = first + i;
        setIdx[0] = pip->wiring[schIdx].set;
        bsch[i] = pip->wiring[schIdx].legt;
    }
    return err;
}

_AVX afxRasterizer AfxGetLinkedRasterizer(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    afxRasterizer razr = pip->razr;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr;
}

_AVX afxError _AvxPipStdDtor(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = AfxGetObjectProvider(pip);

    AfxAssert(pip->stages);
    AfxDeallocate(pip->stages);

    if (pip->wiring)
    {
        for (afxNat i = 0; i < pip->wiringCnt; i++)
            AfxReleaseObjects(1, &pip->wiring[i].legt);

        AfxDeallocate(pip->wiring);
    }

    if (pip->razr)
    {
        AfxReleaseObjects(1, &pip->razr);
    }

    if (pip->vin)
    {
        AfxReleaseObjects(1, &pip->vin);
    }

    return err;
}

_AVX afxError _AvxPipStdCtor(afxPipeline pip, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = cookie->udd[0];
    afxPipelineConfig const *pipb = ((afxPipelineConfig const*)cookie->udd[1]) + cookie->no;
    //AfxAssertType(pipb, afxFcc_PIPB);

    // GRAPHICS STATE SETTING

    afxPipelinePrimitiveFlags primFlags = pipb->primFlags;
    pip->primFlags = NIL;

    //if (primFlags)
    {
        if (primFlags & afxPipelinePrimitiveFlag_TOPOLOGY)
            pip->primTop = pipb->primTop;

        if (primFlags & afxPipelinePrimitiveFlag_CTRL_POINTS)
            pip->patchControlPoints = pipb->patchControlPoints;

        if (primFlags & afxPipelinePrimitiveFlag_CULL_MODE)
            pip->cullMode = pipb->cullMode;
    }

    if ((pip->razr = pipb->razr))
        AfxReacquireObjects(1, &pip->razr);

    // XFORM STATE SETTING

    pip->stages = NIL;
    afxNat stageCnt = pipb->shdCnt;
    AfxAssert(stageCnt);
    pip->stageCnt = 0;

    if (!(pip->stages = AfxAllocate(stageCnt, sizeof(pip->stages[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxNat shaderCnt = 0;
        afxShaderBlueprint shdb[5];

        for (afxNat i = 0; i < stageCnt; i++)
        {
            AfxMakeUri128(&pip->stages[pip->stageCnt].shd, &pipb->shdUri[i]);
            AfxMakeString8(&pip->stages[pip->stageCnt].fn, NIL/*&pipb->shdFn[i]*/);
            pip->stages[pip->stageCnt].stage = pipb->shdStage[i];

            AfxShaderBlueprintBegin(&shdb[shaderCnt], afxShaderStage_VERTEX, NIL, NIL, 0, 0, 0);
            AfxLoadGlScript(&shdb[shaderCnt], &pip->stages[pip->stageCnt].shd.uri);

            shaderCnt++;
            pip->stageCnt++;
        }

        if (!err)
        {
            AfxAssert(pip->stageCnt == stageCnt);

            //afxShaderBlueprint test;
            AfxShaderBlueprintBegin(&shdb[shaderCnt], afxShaderStage_FRAGMENT, NIL, NIL, 0, 0, 0);
            AfxLoadGlScript(&shdb[shaderCnt], &pip->razr->fragShd.uri);

            AfxLogEcho("%s", &shdb[shaderCnt].codes.bytemap);

            shaderCnt++;

            afxPipelineRigBlueprint legb[/*_SGL_MAX_LEGO_PER_BIND*/4];

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
            {
                AfxLegoBlueprintBegin(&legb[i], 1);

                if (AfxLegoBlueprintAddShaderContributions(&legb[i], i, shaderCnt, shdb))
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
                setCnt += AfxCountArrayElements(&legb[i].bindings) ? 1 : 0;

            pip->wiringCnt = 0;

            if (setCnt && !(pip->wiring = AfxAllocate(setCnt, sizeof(pip->wiring[0]), 0, AfxHere()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                {
                    if (AfxCountArrayElements(&legb[i].bindings))
                    {
                        if (AfxAcquireBindSchemas(dctx, 1, &(pip->wiring[pip->wiringCnt].legt), &legb[i]))
                        {
                            AfxThrowError();
                        }
                        else
                        {
                            AfxAssertObjects(1, &(pip->wiring[pip->wiringCnt].legt), afxFcc_BSCH);
                            pip->wiring[pip->wiringCnt].set = i;
                            ++pip->wiringCnt;
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
                pip->inCnt = 0;

                for (afxNat i = 0; i < stageCnt; i++)
                {
                    if (afxShaderStage_VERTEX == shdb->stage)
                    {
                        for (afxNat j = 0; j < shdb->inOuts.cnt; j++)
                        {
                            afxShaderBlueprintInOut shdio = *(afxShaderBlueprintInOut*)AfxGetArrayUnit(&shdb->inOuts, j);

                            pip->ins[pip->inCnt].location = shdio.location;
                            //pip->base.ins[pip->base.inCnt].binding = pip->base.ins[pip->base.inCnt].location;
                            pip->ins[pip->inCnt].fmt = shdio.fmt;
                            //pip->base.ins[pip->base.inCnt].offset = 0;
                            pip->ins[pip->inCnt].stream = shdio.stream;


                            pip->ins[pip->inCnt].stream = shdio.location > 2 ? 1 : 0; // DEBUG only

                            pip->inCnt++;
                        }
                        break;
                    }
                }

                if ((pip->vin = pipb->vin))
                    AfxReacquireObjects(1, &pip->vin);
            }

            for (afxNat i = stageCnt; i-- > 0;)
            {
                AfxShaderBlueprintEnd(&shdb[i], 0, NIL);
            }

            if (err && pip->wiring)
            {
                for (afxNat i = 0; i < pip->wiringCnt; i++)
                    AfxReleaseObjects(1, &pip->wiring[i].legt);

                AfxDeallocate(pip->wiring);
            }
        }

        if (err)
        {
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
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxPipeline)),
    .mmu = NIL,
    .ctor = (void*)_AvxPipStdCtor,
    .dtor = (void*)_AvxPipStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineConfig const config[], afxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;

    afxManager* cls = AfxGetPipelineClass(dctx);
    AfxAssertClass(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dctx, (void*)config }))
        AfxThrowError();

    return err;
}

_AVX afxPipeline AfxAssemblePipelineFromXsh(afxDrawContext dctx, afxVertexInput vin, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxPipeline pip = NIL;

    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));

    AfxLogEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str.str));

    afxUri fext;
    AfxPickUriExtension(uri, FALSE, &fext);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxPickUriPath(uri, &fpath);

        if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                //AfxAssertType(&xml, afxFcc_XML);
                afxBool isQwadroXml = AfxTestXmlRoot(&xml, &AfxStaticString("Qwadro"));
                AfxAssert(isQwadroXml);

                afxString query;
                AfxPickUriQueryToString(uri, TRUE, &query);

                afxNat xmlElemIdx = 0;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Pipeline"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (foundCnt)
                {
                    afxPipelineConfig defConfig = { 0 };
                    defConfig.cullMode = afxCullMode_BACK;
                    defConfig.primTop = afxPrimTopology_TRI_LIST;
                    afxPipelineConfig config = defConfig;

                    if (AfxLoadPipelineConfigFromXml(&config, &defConfig, 0, &xml, xmlElemIdx)) AfxThrowError();
                    else
                    {
#if 0
                        afxString128 tmp;
                        AfxMakeString128(&tmp);
                        AfxCopyString(&tmp.str, AfxGetUriString(&fpath));

                        if (!AfxUriIsBlank(&blueprint.uri.uri))
                        {
                            AfxConcatenateStringL(&tmp.str, "?", 1);
                            AfxConcatenateString(&tmp.str, AfxGetUriString(&blueprint.uri.uri));
                        }

                        afxUri tmpUri;
                        AfxUriFromString(&tmpUri, &tmp.str);
                        AfxCopyUri(&blueprint.uri.uri, &tmpUri);
#endif
                        
                        //if (AfxCompileShadersFromXsh(dctx, config.shdCnt, config.shdUri, config.shd)) AfxThrowError();
                        //else
                        {
                            //AfxAssertObjects(config.shdCnt, &config.shd, afxFcc_SHD);

                            if (!(config.razr = AfxLoadRasterizerFromXsh(dctx, &config.razrUri))) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &config.razr, afxFcc_RAZR);

                                config.vin = vin;

                                if (AfxAssemblePipelines(dctx, 1, &config, &pip)) AfxThrowError();
                                else
                                {
                                    AfxAssertObjects(1, &pip, afxFcc_PIP);
                                }

                                AfxReleaseObjects(1, &config.razr);
                            }
                            //AfxReleaseObjects(config.shdCnt, (void**)config.shd );
                        }
                    }
                }

                AfxCleanUpXml(&xml);
            }
        }
        else
        {
            AfxLogError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
            AfxThrowError();
        }
    }

    return pip;
}
