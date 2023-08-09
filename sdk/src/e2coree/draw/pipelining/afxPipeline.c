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

_AFXINL void AfxFormulatePipelineBlueprint(afxPipelineBlueprint* blueprint, afxDrawContext dctx)
{
    afxError err = NIL;
    AfxAssert(blueprint);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssignFcc(blueprint, AFX_FCC_PIPB);
    blueprint->dctx = dctx;
    AfxUri128(&blueprint->uri);
    AfxAcquireArray(&blueprint->shaders, sizeof(afxUri*), 2, AfxSpawnHint());
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
        afxUri *reqShdUri = *(afxUri **)AfxGetArrayUnit(&blueprint->shaders, i);

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
        afxUri *reqShdUri = *(afxUri **)AfxGetArrayUnit(&blueprint->shaders, i);

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
        afxUri **reqShdUri;

        if (!(reqShdUri = AfxInsertArrayUnit(&blueprint->shaders, &foundIdx))) AfxThrowError();
        else
        {
            *reqShdUri = AfxCloneUri(&uri[i]);
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

_AFX afxBool AfxPipelineHasColorBlending(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasColorBlending;
}

_AFX afxBool AfxPipelineHasMultisampling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasMultisampling;
}

_AFX afxBool AfxPipelineHasDepthHandling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasDepthHandling;
}

_AFX afxBool AfxPipelineHasRasterization(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasRasterization;
}

_AFX afxResult AfxPipelineGetScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[])
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

_AFX afxNat AfxPipelineGetScissorCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->scissorCnt;
}

_AFX afxResult AfxPipelineGetViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[])
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

_AFX afxNat AfxPipelineGetViewportCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->vpCnt;
}

_AFX afxBool AfxPipelineHasInputAssembling(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->hasVtxInAssembling;
}

_AFX afxResult AfxPipelineGetInputStreams(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputStream streams[])
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

_AFX afxNat AfxPipelineGetInputStreamCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->inCnt;
}

_AFX afxResult AfxPipelineGetShaders(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[])
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

_AFX afxNat AfxPipelineGetShaderCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->shaderCnt;
}

AFX afxNat AfxPipelineGetWiringCount(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    return pip->wiringCnt;
}

AFX afxError AfxPipelineGetWiring(afxPipeline pip, afxNat idx, afxNat *set, afxLego *legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    //afxPipelineRig pipr = AfxGetLinker(&pip->pipr);
    //AfxAssertObject(pipr, AFX_FCC_PIPR);

    *set = pip->wiring[idx].set;
    *legt = pip->wiring[idx].legt;
    return err;
}

_AFX afxDrawContext AfxGetPipelineContext(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    afxDrawContext dctx = AfxObjectGetProvider(&pip->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    return dctx;
}

_AFX afxError AfxUploadPipelines(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxPipeline pip[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(pip);
    afxResult rslt = 0;

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

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
                        AfxFormulatePipelineBlueprint(&blueprint, dctx);

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

                            if (AfxBuildPipelines(dctx, 1, &blueprint, &pip[i])) AfxThrowError();
                            else
                            {
                                AfxAssertObject(pip[i], AFX_FCC_PIP);
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
                AfxReleaseObject(&pip[j]->obj);
                pip[j] = NIL;
            }
            break;
        }
    }
    return err;
}

_AFX afxError AfxBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipelineBlueprint const blueprint[], afxPipeline pip[])
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssert(blueprint);

    if (AfxClassAcquireObjects(AfxGetPipelineClass(dctx), NIL, cnt, blueprint, (afxInstance**)pip, AfxSpawnHint()))
        AfxThrowError();

    return err;
}

_AFX afxBool _AfxPipEventHandler(afxInstance *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxPipeline pip = (void*)obj;
    AfxAssertObject(pip, AFX_FCC_PIP);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxPipEventFilter(afxInstance *obj, afxInstance *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxPipeline pip = (void*)obj;
    AfxAssertObject(pip, AFX_FCC_PIP);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxPipDtor(afxPipeline pip)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);

    afxDrawContext dctx = AfxGetPipelineContext(pip);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    if (err)
    {
        for (afxNat i = 0; i < pip->shaderCnt; i++)
        {
            AfxObjectRemoveEventFilter(&(pip->shaders[i]->obj), &pip->obj);
            AfxReleaseObject(&(pip->shaders[i]->obj));
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
            AfxReleaseObject(&(pip->wiring[i].legt->obj));

        AfxDeallocate(mem, pip->wiring);
    }

    return err;
}

_AFX afxError _AfxPipCtor(void *cache, afxNat idx, afxPipeline pip, afxPipelineBlueprint const *blueprints)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(pip, AFX_FCC_PIP);
    (void)cache;

    afxPipelineBlueprint const *pipb = &blueprints[idx];

    AfxAssertType(pipb, AFX_FCC_PIPB);

    afxDrawContext dctx = AfxGetPipelineContext(pip);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    pip->shaders = NIL;
    pip->vps = NIL;
    pip->scissors = NIL;

    afxArray const* shaders = &pipb->shaders;
    afxNat shaderCnt = AfxGetArrayPop(shaders);
    AfxAssert(shaderCnt);
    pip->shaderCnt = 0;

    if (!(pip->shaders = AfxAllocate(mem, sizeof(pip->shaders[0]) * shaderCnt, 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < shaderCnt; i++)
        {
            afxUri const *uri = *(afxUri**)AfxGetArrayUnit(shaders, i);
            afxShader shd;

            if (AfxAcquireShaders(dctx, 1, uri, &shd)) AfxThrowError();
            else
            {
                if (AfxObjectInstallEventFilter(&shd->obj, &pip->obj))
                {
                    AfxReleaseObject(&shd->obj);
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
            afxLegoBlueprint legb[/*_SGL_MAX_LEGO_PER_BIND*/4];

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
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

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                setCnt += AfxGetArrayPop(&legb[i].bindings) ? 1 : 0;

            pip->wiringCnt = 0;

            if (setCnt && !(pip->wiring = AfxAllocate(mem, setCnt * sizeof(pip->wiring[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                {
                    if (AfxGetArrayPop(&legb[i].bindings))
                    {
                        if (AfxBuildLegos(dctx, 1, &legb[i], &(pip->wiring[pip->wiringCnt].legt)))
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

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
            {
                AfxLegoBlueprintEnd(&legb[i], NIL);
            }

            if (!err)
            {
                pip->inCnt = 0;

                for (afxNat i = 0; i < shaderCnt; i++)
                {
                    afxShader shd;
                    AfxPipelineGetShaders(pip, i, 1, &shd);

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
                afxNat vpCnt = AfxGetArrayPop(viewports);
                pip->vpCnt = 0;
                pip->vps = NIL;

                if (vpCnt && !(pip->vps = AfxAllocate(mem, sizeof(pip->vps[0]) * vpCnt, 0, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < vpCnt; i++)
                    {
                        pip->vps[pip->vpCnt] = *(afxViewport const*)AfxGetArrayUnit(viewports, i);
                        pip->vpCnt++;
                    }
                }

                if (!err)
                {
                    afxArray const* scissors = &pipb->scissors;
                    afxNat scissorCnt = AfxGetArrayPop(scissors);
                    pip->scissorCnt = 0;
                    pip->scissors = NIL;

                    if (scissorCnt && !(pip->scissors = AfxAllocate(mem, sizeof(pip->scissors[0]) * scissorCnt, 0, AfxSpawnHint()))) AfxThrowError();
                    else
                    {
                        for (afxNat i = 0; i < scissorCnt; i++)
                        {
                            pip->scissors[pip->scissorCnt] = *(afxRect const*)AfxGetArrayUnit(scissors, i);
                            pip->scissorCnt++;
                        }
                    }
                }

                pip->idd = NIL;

                if (dctxD->vmt->pip && dctxD->vmt->pip(pip)) AfxThrowError();
                else
                {
                    AfxAssert(pip->vmt);
                }
            }

            if (err && pip->wiring)
            {
                for (afxNat i = 0; i < pip->wiringCnt; i++)
                    AfxReleaseObject(&(pip->wiring[i].legt->obj));

                AfxDeallocate(mem, pip->wiring);
            }
        }

        if (err)
        {
            for (afxNat i = 0; i < pip->shaderCnt; i++)
            {
                AfxObjectRemoveEventFilter(&(pip->shaders[i]->obj), &pip->obj);
                AfxReleaseObject(&(pip->shaders[i]->obj));
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

_AFX afxClassSpecification const _AfxPipClassSpec;

afxClassSpecification const _AfxPipClassSpec =
{
    AFX_FCC_PIP,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxPipeline)),
    NIL,
    (void*)_AfxPipCtor,
    (void*)_AfxPipDtor,
    .event = _AfxPipEventHandler,
    .eventFilter = _AfxPipEventFilter,
    "afxPipeline",
    NIL
};
