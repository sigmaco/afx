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

#define _AFX_DRAW_C
#define _AFX_PIPELINE_C
#define _AFX_SHADER_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/core/afxXml.h"

#include "afx/draw/afxPipeline.h"
#include "afxDrawClassified.h"
#include "afx/core/afxUri.h"
#include "afx/core/afxSystem.h"
#include "afxDrawParadigms.h"
#include "afx/draw/afxPipeline.h"
#include "afx/draw/afxXmlBackedDrawableResources.h"

// OpenGL/Vulkan Continuous Integration


_AFX void AfxGetPrimitiveAssembling(afxPipeline pip, afxPrimTopology* top, afxBool* restartEnabled)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (top)
        *top = pip->primTop;

    if (restartEnabled)
        *restartEnabled = pip->primRestartEnabled;
}

_AFX afxBool AfxGetDepthTest(afxPipeline pip, afxCompareOp* op, afxBool* clampEnabled, afxBool* writeEnabled) // return TRUE if depth test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (op)
        *op = pip->depthCompareOp;

    if (clampEnabled)
        *clampEnabled = pip->depthClampEnabled;

    if (writeEnabled)
        *writeEnabled = pip->depthWriteEnabled;

    return pip->depthTestEnabled;
}

_AFX afxBool AfxGetDepthBias(afxPipeline pip, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp) // return TRUE if depth bias is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (constFactor)
        *constFactor = pip->depthBiasConstFactor;

    if (clamp)
        *clamp = pip->depthBiasClamp;

    if (slopeScale)
        *slopeScale = pip->depthBiasSlopeScale;

    return pip->depthBiasEnabled;
}

_AFX afxBool AfxGetDepthBounds(afxPipeline pip, afxReal bounds[2]) // return TRUE if depth bounds is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (bounds)
        AfxCopyV2d(bounds, pip->depthBounds);

    return pip->depthBoundsTestEnabled;
}

_AFX afxBool AfxGetStencilConfig(afxPipeline pip, afxStencilConfig* front, afxStencilConfig* back) // return TRUE if stencil test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (front)
        *front = pip->stencilFront;

    if (back)
        *back = pip->stencilBack;

    return pip->stencilTestEnabled;
}

_AFX afxBool AfxGetLogicalPixelOp(afxPipeline pip, afxLogicOp* op) // return TRUE if logical pixel operation is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (op)
        *op = pip->logicOp;

    return pip->logicOpEnabled;
}

_AFX void AfxGetColorBlendConstants(afxPipeline pip, afxReal rgba[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssert(rgba);
    AfxCopyV4d(rgba, pip->blendConstants);
}

_AFX afxNat AfxCountColorOutputChannels(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->outCnt;
}

_AFX afxNat AfxGetColorOutputChannels(afxPipeline pip, afxNat first, afxNat cnt, afxColorOutputChannel ch[]) // return the number of channels obtained.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssertRange(pip->outCnt, first, cnt);
    AfxAssert(ch);

    afxNat i = 0;

    for (; i < cnt; i++)
        ch[i] = pip->outs[first + i];

    return i;
}

_AFX afxBool AfxGetRasterizationMultisampling(afxPipeline pip, afxNat* sampleCnt, afxBitmask sampleMask[32]) // return TRUE if multisampling rasterization is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    
    if (sampleCnt)
        *sampleCnt = pip->sampleCnt;

    if (sampleMask)
        for (afxNat i = 0; i < pip->sampleCnt; i++)
            sampleMask[i] = pip->sampleBitmasks[i];

    return pip->msEnabled;
}

_AFX afxBool AfxGetSampleShading(afxPipeline pip, afxReal* minSampleShadingValue) // return TRUE if multisampling rasterization is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (minSampleShadingValue)
        *minSampleShadingValue = pip->minSampleShadingValue;

    return pip->sampleShadingEnabled;
}

_AFX afxCullMode AfxGetPrimitiveCulling(afxPipeline pip, afxBool* cwFrontFacing) // return the culling mode set.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (cwFrontFacing)
        *cwFrontFacing = pip->cwFrontFacing;

    return pip->cullMode;
}

_AFX afxBool AfxGetRasterization(afxPipeline pip, afxFillMode* mode, afxReal* lineWidth) // return TRUE if rasterization enabled.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    if (mode)
        *mode = pip->fillMode;

    if (lineWidth)
        *lineWidth = pip->lineWidth;

    return !pip->rasterizationDisabled;
}

_AFX afxNat AfxGetPipelineScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
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
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->scissorCnt;
}

_AFX afxNat AfxGetPipelineViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    AfxAssert(cnt);
    AfxAssert(vp);
    AfxAssertRange(pip->vpCnt, first, cnt);
    afxNat hitCnt = 0;

    for (afxNat i = 0; i < AfxMini(pip->vpCnt, cnt); i++)
    {
        afxViewport const *vp2 = &(pip->vps[first + i]);
        AfxCopyV2d(vp[i].offset, vp2->offset);
        AfxCopyV2d(vp[i].extent, vp2->extent);
        AfxCopyV2d(vp[i].depth, vp2->depth);

        hitCnt++;
    }
    return hitCnt;
}

_AFX afxNat AfxCountPipelineViewports(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->vpCnt;
}

_AFX afxNat AfxGetPipelineInputs(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputLocation streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
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

_AFX afxNat AfxCountPipelineInputs(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->inCnt;
}

_AFX afxNat AfxGetPipelineShaders(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
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
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->shaderCnt;
}

AFX afxNat AfxCountPipelineWirings(afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip->wiringCnt;
}

AFX afxError AfxGetPipelineWiring(afxPipeline pip, afxNat idx, afxNat *set, afxPipelineRig *legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    //afxPipelineRig pipr = AfxGetLinker(&pip->pipr);
    //AfxAssertObject(pipr, afxFcc_PIPR);

    *set = pip->wiring[idx].set;
    *legt = pip->wiring[idx].legt;
    return err;
}

_AFX afxError AfxBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipeline pip[], afxPipelineConfig const blueprint[])
{
    afxError err = AFX_ERR_NONE;

    if (AfxAcquireObjects(&dctx->pipelines, cnt, (afxHandle*)pip, (void*[]) { dctx, (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxPipeline AfxAssemblePipelineFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxPipeline pip = NIL;

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    AfxEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str));

    afxUri fext;
    AfxExcerptUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath, query;
        AfxExcerptUriPath(&fpath, uri);
        AfxExcerptUriQuery(&query, uri, TRUE);

        if (0 == AfxCompareStringLiteralCi(AfxUriGetStringConst(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxXmlNode const *node = AfxGetXmlRoot(&xml);
                afxString const *name = AfxGetXmlNodeName(node);
                AfxAssert(0 == AfxCompareString(name, &g_str_Qwadro));
                afxString const *queryStr = AfxUriGetStringConst(&query);
                afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                node = AfxXmlNodeFindChild(node, &g_str_Pipeline, hasQuery ? &g_str_name : NIL, hasQuery ? queryStr : NIL);

                if (node)
                {
                    afxPipelineConfig blueprint = { 0 };

                    if (AfxParseXmlBackedPipelineBlueprint(node, &blueprint, dctx)) AfxThrowError();
                    else
                    {
#if 0
                        afxString128 tmp;
                        AfxString128(&tmp);
                        AfxCopyString(&tmp.str, AfxUriGetStringConst(&fpath));

                        if (!AfxUriIsBlank(&blueprint.uri.uri))
                        {
                            AfxAppendStringLiteral(&tmp.str, "?", 1);
                            AfxAppendString(&tmp.str, AfxUriGetStringConst(&blueprint.uri.uri));
                        }

                        afxUri tmpUri;
                        AfxReflectUriString(&tmpUri, &tmp.str);
                        AfxCopyUri(&blueprint.uri.uri, &tmpUri);
#endif
                        if (AfxBuildPipelines(dctx, 1, &pip, &blueprint)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &pip, afxFcc_PIP);
                        }

                        for (afxNat i = 0; i < blueprint.shdCnt; i++)
                        {
                            AfxReleaseObjects(1, (void*[]) { blueprint.sh[i] });
                        }
                    }
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

    return pip;
}
