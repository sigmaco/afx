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

_AFX void AfxDescribeRasterizerConfiguration(afxRasterizer rast, afxRasterizationConfig* config)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);

    config->dsFlags = rast->dsFlags;
    config->msFlags = rast->msFlags;
    config->rasFlags = rast->rasFlags;
    config->pixelFlags = rast->pixelFlags;

    config->fillMode = rast->fillMode;
    config->lineWidth = rast->lineWidth;
    
    config->depthBiasEnabled = rast->depthBiasEnabled;
    config->depthBiasSlopeScale = rast->depthBiasSlopeScale;
    config->depthBiasConstFactor = rast->depthBiasConstFactor;
    config->depthBiasClamp = rast->depthBiasClamp;
    
    config->msEnabled = rast->msEnabled;
    
    if ((config->sampleCnt = rast->sampleCnt))
        AfxCopy(rast->sampleCnt, sizeof(rast->sampleMasks[0]), rast->sampleMasks, config->sampleMasks);

    config->alphaToOneEnabled = rast->alphaToOneEnabled;
    config->alphaToCoverageEnabled = rast->alphaToCoverageEnabled;
    config->sampleShadingEnabled = rast->sampleShadingEnabled;
    config->minSampleShadingValue = rast->minSampleShadingValue;

    if ((config->stencilTestEnabled = rast->stencilTestEnabled))
    {
        config->stencilFront = rast->stencilFront;
        config->stencilBack = rast->stencilBack;
    }

    config->depthTestEnabled = rast->depthTestEnabled;
    config->depthCompareOp = rast->depthCompareOp;
    config->depthWriteEnabled = rast->depthWriteEnabled;
    config->dsFmt = rast->dsFmt;
    
    if ((config->depthBoundsTestEnabled = rast->depthBoundsTestEnabled))
        AfxCopyV2d(config->depthBounds, rast->depthBounds);

    if ((config->colorOutCnt = rast->outCnt))
        AfxCopy(rast->outCnt, sizeof(rast->outs[0]), rast->outs, config->colorOuts);

    AfxCopyColor(config->blendConstants, rast->blendConstants);
    config->pixelLogicOpEnabled = rast->logicOpEnabled;
    config->pixelLogicOp = rast->logicOp;
}

_AFX afxBool AfxGetDepthTestInfo(afxRasterizer rast, afxCompareOp* op, afxBool* writeEnabled) // return TRUE if depth test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->depthTestEnabled;

    if (op && enabled)
        *op = rast->depthCompareOp;

    if (writeEnabled && enabled)
        *writeEnabled = rast->depthWriteEnabled;

    return enabled;
}

_AFX afxBool AfxGetDepthBiasInfo(afxRasterizer rast, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp) // return TRUE if depth bias is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->depthBiasEnabled;

    if (constFactor && enabled)
        *constFactor = rast->depthBiasConstFactor;

    if (clamp && enabled)
        *clamp = rast->depthBiasClamp;

    if (slopeScale && enabled)
        *slopeScale = rast->depthBiasSlopeScale;

    return enabled;
}

_AFX afxBool AfxGetDepthBoundsInfo(afxRasterizer rast, afxReal bounds[2]) // return TRUE if depth bounds is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->depthBoundsTestEnabled;

    if (bounds && enabled)
        AfxCopyV2d(bounds, rast->depthBounds);

    return enabled;
}

_AFX afxBool AfxGetStencilConfig(afxRasterizer rast, afxStencilConfig* front, afxStencilConfig* back) // return TRUE if stencil test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->stencilTestEnabled;

    if (front && enabled)
        *front = rast->stencilFront;

    if (back && enabled)
        *back = rast->stencilBack;

    return enabled;
}

_AFX afxBool AfxGetLogicalPixelOperation(afxRasterizer rast, afxLogicOp* op) // return TRUE if logical pixel operation is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->logicOpEnabled;

    if (op && enabled)
        *op = rast->logicOp;

    return enabled;
}

_AFX void AfxGetColorBlendConstants(afxRasterizer rast, afxReal rgba[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    AfxCopyV4d(rgba, rast->blendConstants);
}

_AFX afxNat AfxCountColorOutputChannels(afxRasterizer rast)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    return rast->outCnt;
}

_AFX afxNat AfxGetColorOutputChannels(afxRasterizer rast, afxNat first, afxNat cnt, afxColorOutputChannel ch[]) // return the number of channels obtained.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    AfxAssertRange(rast->outCnt, first, cnt);
    AfxAssert(ch);

    afxNat i = 0;

    for (; i < cnt; i++)
        ch[i] = rast->outs[first + i];

    return i;
}

_AFX afxBool AfxGetMultisamplingInfo(afxRasterizer rast, afxNat* sampleCnt, afxMask sampleMask[32]) // return TRUE if multisampling rasterization is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->msEnabled;

    if (sampleCnt && enabled)
        *sampleCnt = rast->sampleCnt;

    if (sampleMask && enabled)
        for (afxNat i = 0; i < rast->sampleCnt; i++)
            sampleMask[i] = rast->sampleMasks[i];

    return enabled;
}

_AFX afxBool AfxGetSampleShadingInfo(afxRasterizer rast, afxReal* minSampleShadingValue) // return TRUE if sample shading is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    afxBool enabled = rast->sampleShadingEnabled;

    if (minSampleShadingValue && enabled)
        *minSampleShadingValue = rast->minSampleShadingValue;

    return enabled;
}

_AFX afxFillMode AfxGetRasterizationMode(afxRasterizer rast)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    return rast->fillMode;
}

_AFX afxBool AfxGetLineRasterizationInfo(afxRasterizer rast, afxReal* lineWidth) // return TRUE if rasterization enabled.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rast, afxFcc_PIPR);
    
    if (lineWidth)
        *lineWidth = rast->lineWidth;

    return 1;
}

_AFX afxError AfxAcquireRasterizers(afxDrawContext dctx, afxNat cnt, afxRasterizationConfig const config[], afxRasterizer rast[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(config);
    AfxAssert(rast);
    AfxAssert(cnt);

    if (AfxAcquireObjects(&dctx->rasterizers, cnt, (afxHandle*)rast, (void*[]) { dctx, (void*)config }))
        AfxThrowError();

    return err;
}

_AFX afxRasterizer AfxLoadRasterizerFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxRasterizer rast = NIL;

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
        afxUri fpath;
        AfxGetUriPath(&fpath, uri);

        if (0 == AfxCompareStringLiteralCi(AfxGetUriString(&fext), 0, ".xml", 4))
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
                        if (AfxAcquireRasterizers(dctx, 1, &config, &rast)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &rast, afxFcc_PIPR);
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

    return rast;
}
