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
#define _AFX_RASTERIZER_C
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/pipe/afxRasterizer.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/io/afxXsh.h"

 // OpenGL/Vulkan Continuous Integration

_AFX void AfxDescribeRasterizerConfiguration(afxRasterizer razr, afxRasterizationConfig* config)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    config->dsFlags = razr->dsFlags;
    config->msFlags = razr->msFlags;
    config->rasFlags = razr->rasFlags;
    config->pixelFlags = razr->pixelFlags;

    config->fillMode = razr->fillMode;
    config->lineWidth = razr->lineWidth;
    
    config->depthBiasEnabled = razr->depthBiasEnabled;
    config->depthBiasSlopeScale = razr->depthBiasSlopeScale;
    config->depthBiasConstFactor = razr->depthBiasConstFactor;
    config->depthBiasClamp = razr->depthBiasClamp;
    
    config->msEnabled = razr->msEnabled;
    
    if ((config->sampleCnt = razr->sampleCnt))
        AfxCopy(razr->sampleCnt, sizeof(razr->sampleMasks[0]), razr->sampleMasks, config->sampleMasks);

    config->alphaToOneEnabled = razr->alphaToOneEnabled;
    config->alphaToCoverageEnabled = razr->alphaToCoverageEnabled;
    config->sampleShadingEnabled = razr->sampleShadingEnabled;
    config->minSampleShadingValue = razr->minSampleShadingValue;

    if ((config->stencilTestEnabled = razr->stencilTestEnabled))
    {
        config->stencilFront = razr->stencilFront;
        config->stencilBack = razr->stencilBack;
    }

    config->depthTestEnabled = razr->depthTestEnabled;
    config->depthCompareOp = razr->depthCompareOp;
    config->depthWriteDisabled = razr->depthWriteDisabled;
    config->dsFmt = razr->dsFmt;
    
    if ((config->depthBoundsTestEnabled = razr->depthBoundsTestEnabled))
        AfxCopyV2d(config->depthBounds, razr->depthBounds);

    if ((config->colorOutCnt = razr->outCnt))
        AfxCopy(razr->outCnt, sizeof(razr->outs[0]), razr->outs, config->colorOuts);

    AfxCopyColor(config->blendConstants, razr->blendConstants);
    config->pixelLogicOpEnabled = razr->logicOpEnabled;
    config->pixelLogicOp = razr->logicOp;

    AfxReplicateUri(&config->fragShd, &razr->fragShd.uri);
    AfxReplicateString(&config->fragFn, &razr->fragFn.str.str);
}

_AFX afxBool AfxGetDepthComparator(afxRasterizer razr, afxCompareOp* op) // return TRUE if depth test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->depthTestEnabled;

    if (enabled && op)
        *op = razr->depthCompareOp;

    return enabled;
}

_AFX afxBool AfxDepthWriteIsDisabled(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr->depthWriteDisabled;
}

_AFX afxBool AfxGetDepthBiasInfo(afxRasterizer razr, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp) // return TRUE if depth bias is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->depthBiasEnabled;

    if (enabled && constFactor)
        *constFactor = razr->depthBiasConstFactor;

    if (enabled && clamp)
        *clamp = razr->depthBiasClamp;

    if (enabled && slopeScale)
        *slopeScale = razr->depthBiasSlopeScale;

    return enabled;
}

_AFX afxBool AfxGetDepthBoundsInfo(afxRasterizer razr, afxReal bounds[2]) // return TRUE if depth bounds is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->depthBoundsTestEnabled;

    if (enabled && bounds)
        AfxCopyV2d(bounds, razr->depthBounds);

    return enabled;
}

_AFX void AfxGetFragmentShader(afxRasterizer razr, afxUri* uri, afxString* fn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    if (uri)
        AfxReplicateUri(uri, &razr->fragShd.uri);

    if (fn)
        AfxReplicateString(fn, &razr->fragFn.str.str);
}

_AFX afxBool AfxGetStencilConfig(afxRasterizer razr, afxStencilConfig* front, afxStencilConfig* back) // return TRUE if stencil test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->stencilTestEnabled;

    if (enabled && front)
        *front = razr->stencilFront;

    if (enabled && back)
        *back = razr->stencilBack;

    return enabled;
}

_AFX afxBool AfxGetLogicalPixelOperation(afxRasterizer razr, afxLogicOp* op) // return TRUE if logical pixel operation is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->logicOpEnabled;

    if (enabled && op)
        *op = razr->logicOp;

    return enabled;
}

_AFX void AfxGetColorBlendConstants(afxRasterizer razr, afxReal rgba[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    AfxCopyV4d(rgba, razr->blendConstants);
}

_AFX afxNat AfxCountColorOutputChannels(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr->outCnt;
}

_AFX afxNat AfxGetColorOutputChannels(afxRasterizer razr, afxNat first, afxNat cnt, afxColorOutputChannel ch[]) // return the number of channels obtained.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    AfxAssertRange(razr->outCnt, first, cnt);
    AfxAssert(ch);

    afxNat i = 0;

    for (; i < cnt; i++)
        ch[i] = razr->outs[first + i];

    return i;
}

_AFX afxBool AfxGetMultisamplingInfo(afxRasterizer razr, afxNat* sampleCnt, afxMask sampleMask[32]) // return TRUE if multisampling rasterization is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->msEnabled;

    if (enabled && sampleCnt)
        *sampleCnt = razr->sampleCnt;

    if (enabled && sampleMask)
        for (afxNat i = 0; i < razr->sampleCnt; i++)
            sampleMask[i] = razr->sampleMasks[i];

    return enabled;
}

_AFX afxBool AfxGetMinimumSampleShadingValue(afxRasterizer razr, afxReal* minSampleShadingValue) // return TRUE if sample shading is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->sampleShadingEnabled;

    if (enabled && minSampleShadingValue)
        *minSampleShadingValue = razr->minSampleShadingValue;

    return enabled;
}

_AFX afxFillMode AfxGetRasterizationMode(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr->fillMode;
}

_AFX afxBool AfxGetLineRasterizationWidth(afxRasterizer razr, afxReal* lineWidth) // return TRUE if rasterization enabled.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    
    if (lineWidth)
        *lineWidth = razr->lineWidth;

    return razr->fillMode == afxFillMode_EDGE;
}

_AFX afxError AfxAcquireRasterizers(afxDrawContext dctx, afxNat cnt, afxRasterizationConfig const config[], afxRasterizer razr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(config);
    AfxAssert(razr);
    AfxAssert(cnt);

    afxClass* cls = AfxGetRasterizerClass(dctx);
    AfxAssertClass(cls, afxFcc_RAZR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)razr, (void const*[]) { dctx, (void*)config }))
        AfxThrowError();

    return err;
}

_AFX afxRasterizer AfxLoadRasterizerFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxRasterizer razr = NIL;

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    AfxEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str.str));

    afxUri fext;
    AfxGetUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxGetUriPath(&fpath, uri);

        if (0 == AfxTestStringEquivalenceLiteral(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);
                afxBool isQwadroXml = AfxTestXmlRoot(&xml, &AfxStaticString("Qwadro"));
                AfxAssert(isQwadroXml);

                afxString query;
                AfxGetUriQueryString(uri, TRUE, &query);

                afxNat xmlElemIdx;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Rasterizer"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (foundCnt)
                {
                    afxRasterizationConfig defConfig = { 0 };
                    defConfig.depthCompareOp = afxCompareOp_LESS;
                    defConfig.fillMode = afxFillMode_SOLID;
                    afxRasterizationConfig config = defConfig;

                    if (AfxLoadRasterizationConfigFromXml(&config, &defConfig, 0, &xml, xmlElemIdx)) AfxThrowError();
                    else
                    {
#if 0
                        afxFixedString128 tmp;
                        AfxMakeFixedString128(&tmp);
                        AfxCopyString(&tmp.str, AfxGetUriString(&fpath));

                        if (!AfxUriIsBlank(&blueprint.uri.uri))
                        {
                            AfxAppendStringLiteral(&tmp.str, "?", 1);
                            AfxAppendString(&tmp.str, AfxGetUriString(&blueprint.uri.uri));
                        }

                        afxUri tmpUri;
                        AfxUriFromString(&tmpUri, &tmp.str);
                        AfxCopyUri(&blueprint.uri.uri, &tmpUri);
#endif
                        if (AfxAcquireRasterizers(dctx, 1, &config, &razr)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &razr, afxFcc_RAZR);
                        }
                    }
                }

                AfxReleaseXml(&xml);
            }
        }
        else
        {
            AfxError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
            AfxThrowError();
        }
    }

    return razr;
}
