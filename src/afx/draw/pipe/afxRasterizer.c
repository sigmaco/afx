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
#define _AVX_RASTERIZER_C
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/io/afxXsh.h"

 // OpenGL/Vulkan Continuous Integration

_AVX void AfxDescribeRasterizerConfiguration(afxRasterizer razr, afxRasterizationConfig* config)
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
        AfxCopy2(razr->sampleCnt, sizeof(razr->sampleMasks[0]), razr->sampleMasks, config->sampleMasks);

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
        AfxCopy2(razr->outCnt, sizeof(razr->outs[0]), razr->outs, config->colorOuts);

    AfxCopyColor(config->blendConstants, razr->blendConstants);
    config->pixelLogicOpEnabled = razr->logicOpEnabled;
    config->pixelLogicOp = razr->logicOp;

    AfxReplicateUri(&config->fragShd, &razr->fragShd.uri);
    AfxReflectString(&razr->fragFn.str.str, &config->fragFn);
}

_AVX afxBool AfxGetDepthComparator(afxRasterizer razr, afxCompareOp* op) // return TRUE if depth test is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->depthTestEnabled;

    if (enabled && op)
        *op = razr->depthCompareOp;

    return enabled;
}

_AVX afxBool AfxDepthWriteIsDisabled(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr->depthWriteDisabled;
}

_AVX afxBool AfxGetDepthBiasInfo(afxRasterizer razr, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp) // return TRUE if depth bias is enabled
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

_AVX afxBool AfxGetDepthBoundsInfo(afxRasterizer razr, afxReal bounds[2]) // return TRUE if depth bounds is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->depthBoundsTestEnabled;

    if (enabled && bounds)
        AfxCopyV2d(bounds, razr->depthBounds);

    return enabled;
}

_AVX void AfxGetFragmentShader(afxRasterizer razr, afxUri* uri, afxString* fn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    if (uri)
        AfxReplicateUri(uri, &razr->fragShd.uri);

    if (fn)
        AfxReflectString(&razr->fragFn.str.str, fn);
}

_AVX afxBool AfxGetStencilConfig(afxRasterizer razr, afxStencilConfig* front, afxStencilConfig* back) // return TRUE if stencil test is enabled
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

_AVX afxBool AfxGetLogicalPixelOperation(afxRasterizer razr, afxLogicOp* op) // return TRUE if logical pixel operation is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->logicOpEnabled;

    if (enabled && op)
        *op = razr->logicOp;

    return enabled;
}

_AVX void AfxGetColorBlendConstants(afxRasterizer razr, afxReal rgba[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    AfxCopyV4d(rgba, razr->blendConstants);
}

_AVX afxNat AfxCountColorOutputChannels(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr->outCnt;
}

_AVX afxNat AfxGetColorOutputChannels(afxRasterizer razr, afxNat first, afxNat cnt, afxColorOutputChannel ch[]) // return the number of channels obtained.
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

_AVX afxBool AfxGetMultisamplingInfo(afxRasterizer razr, afxNat* sampleCnt, afxMask sampleMask[32]) // return TRUE if multisampling rasterization is enabled
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

_AVX afxBool AfxGetMinimumSampleShadingValue(afxRasterizer razr, afxReal* minSampleShadingValue) // return TRUE if sample shading is enabled
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    afxBool enabled = razr->sampleShadingEnabled;

    if (enabled && minSampleShadingValue)
        *minSampleShadingValue = razr->minSampleShadingValue;

    return enabled;
}

_AVX afxFillMode AfxGetRasterizationMode(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return razr->fillMode;
}

_AVX afxBool AfxGetLineRasterizationWidth(afxRasterizer razr, afxReal* lineWidth) // return TRUE if rasterization enabled.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    
    if (lineWidth)
        *lineWidth = razr->lineWidth;

    return razr->fillMode == afxFillMode_EDGE;
}

_AVX afxError _AvxRazrStdDtor(afxRasterizer razr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    afxDrawContext dctx = AfxGetObjectProvider(razr);

    if (razr->sampleMasks)
        AfxDeallocate(razr->sampleMasks);

    if (razr->outs)
        AfxDeallocate(razr->outs);

    return err;
}

_AVX afxError _AvxRazrStdCtor(afxRasterizer razr, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &razr, afxFcc_RAZR);

    afxDrawContext dctx = cookie->udd[0];
    afxRasterizationConfig const *rasc = ((afxRasterizationConfig const*)cookie->udd[1]) + cookie->no;
    //AfxAssertType(rasc, afxFcc_RASB);

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxRasterizationFlags rasFlags = rasc->rasFlags;
    razr->rasFlags = NIL;

    razr->fillMode = rasc->fillMode;
    razr->lineWidth = rasc->lineWidth;

    razr->depthBiasEnabled = !!rasc->depthBiasEnabled;
    razr->depthBiasSlopeScale = rasc->depthBiasSlopeScale;
    razr->depthBiasConstFactor = rasc->depthBiasConstFactor;
    razr->depthBiasClamp = rasc->depthBiasClamp;

    afxMultisamplingFlags msFlags = rasc->msFlags;
    razr->msFlags = NIL;

    razr->msEnabled = !!rasc->msEnabled;
    razr->minSampleShadingValue = rasc->minSampleShadingValue;

    razr->sampleCnt = rasc->sampleCnt;
    razr->sampleMasks = NIL;

    if (razr->sampleCnt && !(razr->sampleMasks = AfxAllocate(razr->sampleCnt, sizeof(razr->sampleMasks[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < razr->sampleCnt; i++)
            razr->sampleMasks[i] = rasc->sampleMasks[i];
    
        afxDepthStencilFlags dsFlags = rasc->dsFlags;
        razr->dsFlags = NIL;

        razr->depthTestEnabled = !!rasc->depthTestEnabled;
        razr->depthCompareOp = rasc->depthCompareOp;
        razr->depthWriteDisabled = !!rasc->depthWriteDisabled;

        razr->stencilTestEnabled = !!rasc->stencilTestEnabled;
        razr->stencilFront = rasc->stencilFront;
        razr->stencilBack = rasc->stencilBack;

        razr->depthBoundsTestEnabled = !!rasc->depthBoundsTestEnabled;
        AfxCopyV2d(razr->depthBounds, rasc->depthBounds);

        razr->dsFmt = rasc->dsFmt; // ?

        afxColorOutputFlags pixelFlags = rasc->pixelFlags;
        razr->pixelFlags = NIL;

        razr->outCnt = rasc->colorOutCnt;
        razr->outs = NIL;

        if (razr->outCnt && !(razr->outs = AfxAllocate(razr->outCnt, sizeof(razr->outs[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < razr->outCnt; i++)
                razr->outs[i] = rasc->colorOuts[i];

            // deveria ser s� o blend/write, j� que s� podemos determinar as sa�das quando assembleado com fragment shaders enquanto pipeline completo.
        
            AfxCopyV4d(razr->blendConstants, rasc->blendConstants);

            razr->logicOpEnabled = !!rasc->pixelLogicOpEnabled;
            razr->logicOp = rasc->pixelLogicOp;

            AfxMakeUri128(&razr->fragShd, &rasc->fragShd);
            AfxMakeString8(&razr->fragFn, &rasc->fragFn);

            if (err && razr->outs)
                AfxDeallocate(razr->outs);
        }

        if (err && razr->sampleMasks)
            AfxDeallocate(razr->sampleMasks);
    }

    AfxAssertObjects(1, &razr, afxFcc_RAZR);
    return err;
}

_AVX afxClassConfig const _AvxRazrStdImplementation =
{
    .fcc = afxFcc_RAZR,
    .name = "Rasterizer",
    .desc = "Pipelined Rasterization Module",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxRasterizer)),
    .ctor = (void*)_AvxRazrStdCtor,
    .dtor = (void*)_AvxRazrStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireRasterizers(afxDrawContext dctx, afxNat cnt, afxRasterizationConfig const config[], afxRasterizer razr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(config);
    AfxAssert(razr);
    AfxAssert(cnt);

    afxManager* cls = AfxGetRasterizerClass(dctx);
    AfxAssertClass(cls, afxFcc_RAZR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)razr, (void const*[]) { dctx, (void*)config }))
        AfxThrowError();

    return err;
}

_AVX afxRasterizer AfxLoadRasterizerFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;

    afxRasterizer razr = NIL;

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
                        if (AfxAcquireRasterizers(dctx, 1, &config, &razr)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &razr, afxFcc_RAZR);
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

    return razr;
}