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

#define _AFX_PIPELINE_C
#define _AFX_SHADER_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/core/io/afxXml.h"

#include "afx/draw/pipelining/afxPipeline.h"
#include "../_classified/afxDrawClassified.h"
#include "afx/core/io/afxUri.h"
#include "afx/core/afxSystem.h"
#include "../afxDrawParadigms.h"
#include "afx/draw/pipelining/afxPipelineBlueprint.h"
#include "afx/draw/io/afxXmlBackedDrawableResources.h"

// OpenGL/Vulkan Continuous Integration

////////////////////////////////////////////////////////////////////////////////
// PIPELINE BLUEPRINT                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxFormulatePipelineBlueprint(afxPipelineBlueprint* blueprint)
{
    afxError err = NIL;
    AfxAssert(blueprint);
    AfxAssignFcc(blueprint, AFX_FCC_PIPB);
    blueprint->dctx = NIL;
    AfxUri128(&blueprint->uri);
    AfxAcquireArray(&blueprint->shaders, sizeof(afxUri), 2, AfxSpawnHint());
    AfxAcquireArray(&blueprint->viewports, sizeof(afxViewport), 0, AfxSpawnHint());
    AfxAcquireArray(&blueprint->scissors, sizeof(afxRect), 0, AfxSpawnHint());
    blueprint->hasAssembling = FALSE;
    blueprint->hasRasterization = FALSE;
    blueprint->hasDepthHandling = FALSE;
    blueprint->hasMultisampling = FALSE;
    blueprint->hasColorBlending = FALSE;
    blueprint->inAssembling = (afxPipelineInputAssemblyState) { 0 };
    blueprint->rasterization = (afxPipelineRasterizerState) { 0 };
    blueprint->depthHandling = (afxPipelineDepthState) { 0 };
    blueprint->multisampling = (afxPipelineMultisampleState) { 0 };
    blueprint->colorBlending = (afxPipelineColorBlendState) { 0 };
}

_AFXINL void AfxPipelineBlueprintDiscard(afxPipelineBlueprint *blueprint)
{
    afxError err = NIL;
    AfxAssertType(blueprint, AFX_FCC_PIPB);

    for (afxNat i = 0; i < AfxGetArrayPop(&blueprint->shaders); i++)
    {
        afxUri *reqShdUri = AfxGetArrayUnit(&blueprint->shaders, i);

        if (reqShdUri)
            AfxUriDeallocate(reqShdUri);
    }
    AfxReleaseArray(&blueprint->shaders);

    AfxReleaseArray(&blueprint->viewports);
    AfxReleaseArray(&blueprint->scissors);
    blueprint->fcc = NIL;
}

_AFXINL void AfxPipelineBlueprintErase(afxPipelineBlueprint *blueprint)
{
    afxError err = NIL;
    AfxAssertType(blueprint, AFX_FCC_PIPB);

    for (afxNat i = 0; i < AfxGetArrayPop(&blueprint->shaders); i++)
    {
        afxUri *reqShdUri = AfxGetArrayUnit(&blueprint->shaders, i);

        if (reqShdUri)
            AfxUriDeallocate(reqShdUri);
    }

    AfxEmptyArray(&blueprint->shaders);
    AfxEmptyArray(&blueprint->viewports);
    AfxEmptyArray(&blueprint->scissors);
    blueprint->hasAssembling = FALSE;
    blueprint->hasRasterization = FALSE;
    blueprint->hasDepthHandling = FALSE;
    blueprint->hasMultisampling = FALSE;
    blueprint->hasColorBlending = FALSE;
    blueprint->inAssembling = (afxPipelineInputAssemblyState) { 0 };
    blueprint->rasterization = (afxPipelineRasterizerState) { 0 };
    blueprint->depthHandling = (afxPipelineDepthState) { 0 };
    blueprint->multisampling = (afxPipelineMultisampleState) { 0 };
    blueprint->colorBlending = (afxPipelineColorBlendState) { 0 };
}

_AFXINL afxError AfxPipelineBlueprintAddViewports(afxPipelineBlueprint *blueprint, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);

    //AfxReserveArraySpace(&blueprint->viewports, AfxGetArrayPop(&blueprint->viewports) + cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat idx;
        afxViewport *pVp;

        if (!(pVp = AfxInsertArrayUnit(&blueprint->viewports, &idx)))
            AfxThrowError();
        else
            *pVp = vp[i];
        
        if (err)
            break;
    }
    return err;
}

_AFXINL afxError AfxPipelineBlueprintAddScissors(afxPipelineBlueprint *blueprint, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);

    //AfxReserveArraySpace(&blueprint->scissors, AfxGetArrayPop(&blueprint->scissors) + cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat idx;
        afxRect *pRc;

        if (!(pRc = AfxInsertArrayUnit(&blueprint->scissors, &idx)))
            AfxThrowError();
        else
            *pRc = rc[i];

        if (err)
            break;
    }
    return err;
}

_AFXINL afxError AfxPipelineBlueprintAddShaders(afxPipelineBlueprint *blueprint, afxNat cnt, afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);

    //AfxReserveArraySpace(&blueprint->shaders, AfxGetArrayPop(&blueprint->shaders) + cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat foundIdx = AFX_INVALID_INDEX;
        afxUri *reqShdUri;

        if (!(reqShdUri = AfxInsertArrayUnit(&blueprint->shaders, &foundIdx))) AfxThrowError();
        else
        {
             AfxCloneUri(reqShdUri, &uri[i]);
             int a = 0;
        }
    }
    return err;
}

_AFXINL void AfxPipelineBlueprintConfigInputAssemblyState(afxPipelineBlueprint *blueprint, afxPipelineInputAssemblyState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);
    blueprint->inAssembling = state ? *state : (afxPipelineInputAssemblyState) { 0 };
    blueprint->hasAssembling = !!state;
}

_AFXINL void AfxPipelineBlueprintConfigRasterizerState(afxPipelineBlueprint *blueprint, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);
    blueprint->rasterization = state ? *state : (afxPipelineRasterizerState) { 0 };
    blueprint->hasRasterization = !!state;
}

_AFXINL void AfxPipelineBlueprintConfigDepthState(afxPipelineBlueprint *blueprint, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);
    blueprint->depthHandling = state ? *state : (afxPipelineDepthState) { 0 };
    blueprint->hasDepthHandling = !!state;
}

_AFXINL void AfxPipelineBlueprintConfigMultisampleState(afxPipelineBlueprint *blueprint, afxPipelineMultisampleState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);
    blueprint->multisampling = state ? *state : (afxPipelineMultisampleState) { 0 };
    blueprint->hasMultisampling = !!state;
}

_AFXINL void AfxPipelineBlueprintConfigColorBlendState(afxPipelineBlueprint *blueprint, afxPipelineColorBlendState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_PIPB);
    blueprint->colorBlending = state ? *state : (afxPipelineColorBlendState) { 0 };
    blueprint->hasColorBlending = !!state;
}

////////////////////////////////////////////////////////////////////////////////
// PIPELINE                                                                   //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetPipelineColorBlending(afxPipeline pip, afxPipelineColorBlendState* state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    if (state)
        *state = pip->colorBlending;

    return pip->hasColorBlending;
}

_AFX afxBool AfxGetPipelineDepthHandling(afxPipeline pip, afxPipelineDepthState* state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    if (state)
        *state = pip->depthHandling;

    return pip->hasDepthHandling;
}

_AFX afxBool AfxGetPipelineMultisampling(afxPipeline pip, afxPipelineMultisampleState* state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    if (state)
        *state = pip->multisampling;

    return pip->hasMultisampling;
}

_AFX afxBool AfxGetPipelineRasterization(afxPipeline pip, afxPipelineRasterizerState* state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    if (state)
        *state = pip->rasterization;

    return pip->hasRasterization;
}

_AFX afxBool AfxGetPipelineInputAssembling(afxPipeline pip, afxPipelineInputAssemblyState* state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    if (state)
        *state = pip->vtxInAssembling;

    return pip->hasVtxInAssembling;
}

_AFX afxBool AfxPipelineHasColorBlending(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->hasColorBlending;
}

_AFX afxBool AfxPipelineHasMultisampling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->hasMultisampling;
}

_AFX afxBool AfxPipelineHasDepthHandling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->hasDepthHandling;
}

_AFX afxBool AfxPipelineHasRasterization(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->hasRasterization;
}

_AFX afxNat AfxGetPipelineScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(rect);
    AfxAssertRange(pip->scissorCnt, first, cnt);
    afxNat hitCnt = 0;

    for (afxNat i = 0; i < AfxMini(pip->scissorCnt, cnt); i++)
    {
        afxRect const *rect2 = &pip->scissors[first + i];
        AfxRectCopy(&rect[i], rect2);
        hitCnt++;
    }
    return hitCnt;
}

_AFX afxNat AfxCountPipelineScissors(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->scissorCnt;
}

_AFX afxNat AfxGetPipelineViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(vp);
    AfxAssertRange(pip->vpCnt, first, cnt);
    afxNat hitCnt = 0;

    for (afxNat i = 0; i < AfxMini(pip->vpCnt, cnt); i++)
    {
        afxViewport const *vp2 = &(pip->vps[first + i]);
        AfxV2dCopy(vp[i].offset, vp2->offset);
        AfxV2dCopy(vp[i].extent, vp2->extent);
        AfxV2dCopy(vp[i].depth, vp2->depth);

        hitCnt++;
    }
    return hitCnt;
}

_AFX afxNat AfxCountPipelineViewports(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->vpCnt;
}

_AFX afxBool AfxPipelineHasInputAssembling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->hasVtxInAssembling;
}

_AFX afxNat AfxGetPipelineInputStreams(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(streams);
    AfxAssertRange(pip->inCnt, first, cnt);
    afxNat hitCnt = 0;

    for (afxNat i = 0; i < AfxMini(pip->inCnt, cnt); i++)
    {
        AfxCopy(&streams[i], &pip->ins[first + i], sizeof(streams[0]));
        hitCnt++;
    }
    return hitCnt;
}

_AFX afxNat AfxCountPipelineInputStreams(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->inCnt;
}

_AFX afxNat AfxGetPipelineShaders(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    AfxAssert(cnt);
    AfxAssert(shd);
    AfxAssertRange(pip->shaderCnt, first, cnt);
    afxNat hitCnt = 0;

    for (afxNat i = 0; i < AfxMini(pip->shaderCnt, cnt); i++)
    {
        shd[i] = pip->shaders[first + i];
        hitCnt++;
    }
    return hitCnt;
}

_AFX afxNat AfxCountPipelineShaders(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->shaderCnt;
}

AFX afxNat AfxCountPipelineWirings(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);
    return pip->wiringCnt;
}

AFX afxError AfxGetPipelineWiring(afxPipeline pip, afxNat idx, afxNat *set, afxLego *legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, AFX_FCC_PIP);

    //afxPipelineRig pipr = AfxGetLinker(&pip->pipr);
    //AfxAssertObject(pipr, AFX_FCC_PIPR);

    *set = pip->wiring[idx].set;
    *legt = pip->wiring[idx].legt;
    return err;
}

_AFX afxError AfxBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipeline pip[], afxPipelineBlueprint const blueprint[])
{
    afxError err = AFX_ERR_NONE;

    if (AfxAcquireObjects(&dctx->pipelines, cnt, (afxHandle*)pip, (void*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxError AfxUploadPipelines(afxDrawContext dctx, afxNat cnt, afxPipeline pip[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(pip);
    afxResult rslt = 0;

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_CTX);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertType(&uri[i], AFX_FCC_URI);
        AfxAssert(!AfxUriIsBlank(&uri[i]));

        AfxEcho("Uploading pipeline '%.*s'", AfxPushString(&uri[i].str));

        afxUri fext;
        AfxExcerptUriExtension(&fext, &uri[i], FALSE);

        if (AfxUriIsBlank(&fext)) AfxThrowError();
        else
        {
            afxUri fpath, query;
            AfxExcerptUriPath(&fpath, &uri[i]);
            AfxExcerptUriQuery(&query, &uri[i], TRUE);

            if (0 == AfxCompareStringLiteralCi(AfxUriGetStringConst(&fext), 0, ".xml", 4))
            {
                afxXml xml;

                if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
                else
                {
                    AfxAssertType(&xml, AFX_FCC_XML);

                    afxXmlNode const *node = AfxXmlGetRootNode(&xml);
                    afxString const *name = AfxXmlNodeGetName(node);
                    AfxAssert(0 == AfxCompareString(name, &g_str_Qwadro));
                    afxString const *queryStr = AfxUriGetStringConst(&query);
                    afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                    node = AfxXmlNodeFindChild(node, &g_str_Pipeline, hasQuery ? &g_str_name : NIL, hasQuery ? queryStr : NIL);

                    if (node)
                    {
                        afxPipelineBlueprint blueprint;
                        AfxFormulatePipelineBlueprint(&blueprint);

                        if (AfxParseXmlBackedPipelineBlueprint(node, &blueprint)) AfxThrowError();
                        else
                        {
                            afxString128 tmp;
                            AfxString128(&tmp);
                            AfxCopyString(&tmp.str, AfxUriGetStringConst(&fpath));

                            if (!AfxUriIsBlank(&blueprint.uri.uri))
                            {
                                AfxAppendStringLiteral(&tmp.str, "?", 1);
                                AfxAppendString(&tmp.str, AfxUriGetStringConst(&blueprint.uri.uri));
                            }

                            afxUri tmpUri;
                            AfxUriReflectString(&tmpUri, &tmp.str);
                            AfxCopyUri(&blueprint.uri.uri, &tmpUri);

                            if (AfxBuildPipelines(dctx, 1, &pip[i], &blueprint)) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &pip[i], AFX_FCC_PIP);
                                ++rslt;
                            }
                        }
                        AfxPipelineBlueprintDiscard(&blueprint);
                    }

                    AfxReleaseXml(&xml);
                }
            }
            else
            {
                AfxError("Extension (%.*s) not supported.", AfxPushString(AfxUriGetStringConst(&fext)));
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObjects(1, (void*[]) { pip[j] });
                pip[j] = NIL;
            }
            break;
        }
    }
    return err;
}

_AFX afxNat AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(pip);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return AfxEnumerateInstances(&dctx->pipelines, first, cnt, (afxHandle*)pip);
}
