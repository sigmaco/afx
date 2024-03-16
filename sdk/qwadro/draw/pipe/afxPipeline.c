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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _AFX_PIPELINE_C
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/io/afxXsh.h"

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
        AfxCopy(1, sizeof(streams[0]), &ins[first + i], &streams[i]);
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

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(pipelines);
    AfxAssert(cnt);
    afxClass* cls = AfxGetPipelineClass(dctx);
    AfxAssertClass(cls, afxFcc_PIP);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)pipelines);
}

_AVX afxError AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineConfig const config[], afxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetPipelineClass(dctx);
    AfxAssertClass(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dctx, (void*)config }))
        AfxThrowError();

    return err;
}

_AVX afxPipeline AfxAssemblePipelineFromXsh(afxDrawContext dctx, afxVertexInput vin, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxPipeline pip = NIL;

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));

    AfxEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str.str));

    afxUri fext;
    AfxExcerptUriExtension(uri, FALSE, &fext);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxExcerptUriPath(uri, &fpath);

        if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);
                afxBool isQwadroXml = AfxTestXmlRoot(&xml, &AfxStaticString("Qwadro"));
                AfxAssert(isQwadroXml);

                afxString query;
                AfxExcerptUriQueryToString(uri, TRUE, &query);

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

                                AfxReleaseObjects(1, (void*[]) { config.razr });
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
            AfxError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
            AfxThrowError();
        }
    }

    return pip;
}
