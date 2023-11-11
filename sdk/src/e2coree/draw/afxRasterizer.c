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

#define _AFX_DRAW_C
#define _AFX_RASTERIZER_C
#define _AFX_SHADER_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/draw/afxRasterizer.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/draw/afxXsh.h"

 // OpenGL/Vulkan Continuous Integration

_AFX void AfxDescribeRasterizerConfiguration(afxRasterizer ras, afxRasterizationConfig* config)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    config->dsFlags = ras->dsFlags;
    config->msFlags = ras->msFlags;
    config->rasFlags = ras->rasFlags;
    config->pixelFlags = ras->pixelFlags;

    config->fillMode = ras->fillMode;
    config->lineWidth = ras->lineWidth;
    
    config->depthBiasEnabled = ras->depthBiasEnabled;
    config->depthBiasSlopeScale = ras->depthBiasSlopeScale;
    config->depthBiasConstFactor = ras->depthBiasConstFactor;
    config->depthBiasClamp = ras->depthBiasClamp;
    
    config->msEnabled = ras->msEnabled;
    
    if ((config->sampleCnt = ras->sampleCnt))
        AfxCopy(config->sampleMasks, ras->sampleMasks, ras->sampleCnt * sizeof(ras->sampleMasks[0]));

    config->alphaToOneEnabled = ras->alphaToOneEnabled;
    config->alphaToCoverageEnabled = ras->alphaToCoverageEnabled;
    config->sampleShadingEnabled = ras->sampleShadingEnabled;
    config->minSampleShadingValue = ras->minSampleShadingValue;

    if ((config->stencilTestEnabled = ras->stencilTestEnabled))
    {
        config->stencilFront = ras->stencilFront;
        config->stencilBack = ras->stencilBack;
    }

    config->depthTestEnabled = ras->depthTestEnabled;
    config->depthCompareOp = ras->depthCompareOp;
    config->depthWriteEnabled = ras->depthWriteEnabled;
    config->dsFmt = ras->dsFmt;
    
    if ((config->depthBoundsTestEnabled = ras->depthBoundsTestEnabled))
        AfxCopyV2d(config->depthBounds, ras->depthBounds);

    if ((config->colorOutCnt = ras->outCnt))
        AfxCopy(config->colorOuts, ras->outs, ras->outCnt * sizeof(ras->outs[0]));

    AfxCopyColor(config->blendConstants, ras->blendConstants);
    config->pixelLogicOpEnabled = ras->logicOpEnabled;
    config->pixelLogicOp = ras->logicOp;
}

_AFX afxBool AfxGetDepthTestInfo(afxRasterizer ras, afxCompareOp* op, afxBool* writeEnabled) // return TRUE if depth test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->depthTestEnabled;

    if (op && enabled)
        *op = ras->depthCompareOp;

    if (writeEnabled && enabled)
        *writeEnabled = ras->depthWriteEnabled;

    return enabled;
}

_AFX afxBool AfxGetDepthBiasInfo(afxRasterizer ras, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp) // return TRUE if depth bias is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->depthBiasEnabled;

    if (constFactor && enabled)
        *constFactor = ras->depthBiasConstFactor;

    if (clamp && enabled)
        *clamp = ras->depthBiasClamp;

    if (slopeScale && enabled)
        *slopeScale = ras->depthBiasSlopeScale;

    return enabled;
}

_AFX afxBool AfxGetDepthBoundsInfo(afxRasterizer ras, afxReal bounds[2]) // return TRUE if depth bounds is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->depthBoundsTestEnabled;

    if (bounds && enabled)
        AfxCopyV2d(bounds, ras->depthBounds);

    return enabled;
}

_AFX afxBool AfxGetStencilConfig(afxRasterizer ras, afxStencilConfig* front, afxStencilConfig* back) // return TRUE if stencil test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->stencilTestEnabled;

    if (front && enabled)
        *front = ras->stencilFront;

    if (back && enabled)
        *back = ras->stencilBack;

    return enabled;
}

_AFX afxBool AfxGetLogicalPixelOperation(afxRasterizer ras, afxLogicOp* op) // return TRUE if logical pixel operation is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->logicOpEnabled;

    if (op && enabled)
        *op = ras->logicOp;

    return enabled;
}

_AFX void AfxGetColorBlendConstants(afxRasterizer ras, afxReal rgba[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxCopyV4d(rgba, ras->blendConstants);
}

_AFX afxNat AfxCountColorOutputChannels(afxRasterizer ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->outCnt;
}

_AFX afxNat AfxGetColorOutputChannels(afxRasterizer ras, afxNat first, afxNat cnt, afxColorOutputChannel ch[]) // return the number of channels obtained.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->outCnt, first, cnt);
    AfxAssert(ch);

    afxNat i = 0;

    for (; i < cnt; i++)
        ch[i] = ras->outs[first + i];

    return i;
}

_AFX afxBool AfxGetMultisamplingInfo(afxRasterizer ras, afxNat* sampleCnt, afxMask sampleMask[32]) // return TRUE if multisampling rasterization is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->msEnabled;

    if (sampleCnt && enabled)
        *sampleCnt = ras->sampleCnt;

    if (sampleMask && enabled)
        for (afxNat i = 0; i < ras->sampleCnt; i++)
            sampleMask[i] = ras->sampleMasks[i];

    return enabled;
}

_AFX afxBool AfxGetSampleShadingInfo(afxRasterizer ras, afxReal* minSampleShadingValue) // return TRUE if sample shading is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxBool enabled = ras->sampleShadingEnabled;

    if (minSampleShadingValue && enabled)
        *minSampleShadingValue = ras->minSampleShadingValue;

    return enabled;
}

_AFX afxFillMode AfxGetRasterizationMode(afxRasterizer ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->fillMode;
}

_AFX afxBool AfxGetLineRasterizationInfo(afxRasterizer ras, afxReal* lineWidth) // return TRUE if rasterization enabled.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    
    if (lineWidth)
        *lineWidth = ras->lineWidth;

    return 1;
}

_AFX afxError AfxAcquireRasterizers(afxDrawContext dctx, afxNat cnt, afxRasterizationConfig const config[], afxRasterizer ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(config);
    AfxAssert(ras);
    AfxAssert(cnt);

    if (AfxAcquireObjects(&dctx->rasterizers, cnt, (afxHandle*)ras, (void*[]) { dctx, (void*)config }))
        AfxThrowError();

    return err;
}

_AFX afxRasterizer AfxLoadRasterizerFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxRasterizer ras = NIL;

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    AfxEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str));

    afxUri fext;
    AfxGetUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath, query;
        AfxGetUriPath(&fpath, uri);
        AfxGetUriQuery(&query, uri, TRUE);

        if (0 == AfxCompareStringLiteralCi(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxXmlNode const *node = AfxGetXmlRoot(&xml);
                afxString const *name = AfxGetXmlNodeName(node);
                AfxAssert(0 == AfxCompareString(name, &AfxStaticString("Qwadro")));
                afxString const *queryStr = AfxGetUriString(&query);
                afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                node = AfxXmlNodeFindChild(node, &AfxStaticString("Rasterizer"), hasQuery ? &AfxStaticString("id") : NIL, hasQuery ? queryStr : NIL);

                afxNat xmlElemIdx;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Rasterizer"), &AfxStaticString("id"), 1, queryStr, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (node)
                {
                    afxRasterizationConfig defConfig = { 0 };
                    defConfig.depthCompareOp = afxCompareOp_LESS;
                    defConfig.fillMode = afxFillMode_SOLID;
                    afxRasterizationConfig config = defConfig;

                    if (AfxLoadRasterizationConfigFromXml(&config, &defConfig, 0, node, &xml, xmlElemIdx)) AfxThrowError();
                    else
                    {
#if 0
                        afxString128 tmp;
                        AfxString128(&tmp);
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
                        if (AfxAcquireRasterizers(dctx, 1, &config, &ras)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &ras, afxFcc_RAS);
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

    return ras;
}
