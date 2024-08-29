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

#include "../../dev/AvxImplKit.h"
#include "qwadro/draw/io/afxXsh.h"

AVX afxChar const *shdResTypeNames[];

_AVX afxError AfxLoadRasterizationConfigFromXml(avxRasterizationConfig* config, avxRasterizationConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    AfxAssert(identity);
    //AfxAssertType(xml, afxFcc_XML);
    
    afxBool hasUnhandledNodes = FALSE;
    afxNat childCnt = AfxCountXmlChilds(xml, elemIdx);
    afxString name, content;

    for (afxNat i = 0; i < childCnt; i++)
    {
        afxNat childIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childIdx, &name, &content);
        afxNat childTagCnt = AfxCountXmlTags(xml, childIdx);

        if (0 == AfxCompareString(&name, &AfxStaticString("RasterizerDiscardEnable")))
        {
            afxBool rasterizationDisabled;

            if ((rasterizationDisabled = (0 == AfxCompareString(&content, &AfxStaticString("true")))))
            {
                AfxAssertBool(rasterizationDisabled);
                config->rasFlags |= NIL;

                // retornar sem a chance de buscar por mais entradas, uma vez que não faria sentido as ter se não para usar.
                break;
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("FillMode")))
        {
            avxFillMode fillMode = AfxFindFillMode(&content);
            AfxAssertRange(avxFillMode_TOTAL, fillMode, 1);
            config->fillMode = fillMode;
            config->rasFlags |= avxRasterizationFlag_FILL_MODE;
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DepthBias")))
        {
            afxBool enabledSet = FALSE, constantSet = FALSE, clampSet = FALSE, slopeSet = FALSE;

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxCompareString(&name, &AfxStaticString("constant")))
                {
                    afxReal depthBiasConstFactor;
                    
                    if (AfxScanString(&content, "%f", &depthBiasConstFactor))
                    {
                        config->depthBiasConstFactor = depthBiasConstFactor;
                        constantSet = TRUE;
                        enabledSet = TRUE;
                    }
                }
                else if (0 == AfxCompareString(&name, &AfxStaticString("clamp")))
                {
                    afxReal depthBiasClamp;

                    if (AfxScanString(&content, "%f", &depthBiasClamp))
                    {
                        config->depthBiasClamp = depthBiasClamp;
                        clampSet = TRUE;
                        enabledSet = TRUE;
                    }
                }
                else if (0 == AfxCompareString(&name, &AfxStaticString("slope")))
                {
                    afxReal depthBiasSlopeScale;
                    
                    if (AfxScanString(&content, "%f", &depthBiasSlopeScale))
                    {
                        config->depthBiasSlopeScale = depthBiasSlopeScale;
                        slopeSet = TRUE;
                        enabledSet = TRUE;
                    }
                }
                else
                {
                    AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            if (enabledSet)
            {
                config->rasFlags |= avxRasterizationFlag_DEPTH_BIAS;

                if (!clampSet)
                {
                    config->depthBiasClamp = identity->depthBiasClamp;
                    AfxLogY("");
                }

                if (!constantSet)
                {
                    config->depthBiasConstFactor = identity->depthBiasConstFactor;
                    AfxLogY("");
                }

                if (!slopeSet)
                {
                    config->depthBiasSlopeScale = identity->depthBiasSlopeScale;
                    AfxLogY("");
                }
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("LineWidth")))
        {
            afxReal lineWidth;

            if (AfxScanString(&content, "%f", &lineWidth))
            {
                config->lineWidth = lineWidth;
                config->rasFlags |= avxRasterizationFlag_LINE_WIDTH;
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DepthTestEnabled")))
        {
            config->depthTestEnabled = TRUE;
            config->dsFlags |= avxDepthStencilFlag_TEST;
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DepthWriteDisabled")))
        {
            config->depthWriteDisabled = TRUE;
            config->dsFlags |= avxDepthStencilFlag_DONT_WRITE;
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DepthCompareOp")))
        {
            if ((config->depthCompareOp = AfxFindCompareOp(&content)))
            {
                config->dsFlags |= avxDepthStencilFlag_COMPARE;
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DepthBoundsTest")))
        {
            afxBool boundsSet = FALSE, boundsMinSet = FALSE, boundsMaxSet = FALSE;

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxCompareString(&name, &AfxStaticString("min")))
                {
                    afxReal mini;

                    if (AfxScanString(&content, "%f", &mini))
                    {
                        config->depthBounds[0] = mini;
                        boundsMinSet = TRUE;
                        boundsSet = TRUE;
                    }
                }
                else if (0 == AfxCompareString(&name, &AfxStaticString("max")))
                {
                    afxReal maxi;

                    if (AfxScanString(&content, "%f", &maxi))
                    {
                        config->depthBounds[1] = maxi;
                        boundsMinSet = TRUE;
                        boundsSet = TRUE;
                    }
                }
                else
                {
                    AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            if (boundsSet)
            {
                config->dsFlags = avxDepthStencilFlag_BOUNDS_TEST;

                if (!boundsMinSet)
                {
                    config->depthBounds[0] = identity->depthBounds[0];
                    AfxLogY("");
                }

                if (!boundsMaxSet)
                {
                    config->depthBounds[1] = identity->depthBounds[1];
                    AfxLogY("");
                }
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("StencilTest")))
        {
            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxCompareString(&name, &AfxStaticString("enable")))
                {
                    if ((config->stencilTestEnabled = (0 == AfxCompareString(&content, &AfxStaticString("true")))))
                    {
                        config->dsFlags |= avxDepthStencilFlag_STENCIL_TEST;
#if !0
                        afxNat childCnt2 = AfxCountXmlChilds(xml, childIdx);

                        for (afxNat k = 0; k < childCnt2; k++)
                        {
                            afxNat childIdx2 = AfxGetXmlChild(xml, childIdx, k);
                            AfxQueryXmlElement(xml, childIdx2, &name, &content);

                            if (0 == AfxCompareString(&name, &AfxStaticString("Front")))
                            {
                                //_AfxParseXmlBackedPipelineDepthStateStencilFace(childChild, elemIdx, specIdx, TRUE, state, state, foundMask); // 7 - 8 - 9 - 10

                            }
                            else if (0 == AfxCompareString(&name, &AfxStaticString("Back")))
                            {
                                //_AfxParseXmlBackedPipelineDepthStateStencilFace(childChild, elemIdx, specIdx, FALSE, state, state, foundMask); // 11 - 12 - 13 - 14
                            }
                            else
                            {
                                AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
                            }
                        }
#endif
                    }
                }
                else
                {
                    AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("Transformer")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxWrapUriString(&tempUri, &content);
                AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->xfmrPipUri);
                AfxParsePipelineFromXsh(&config->xfmrPipb, &tempUri);
            }
            else
            {
                AfxResetUri(&config->xfmrPipUri);
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("FragmentShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxWrapUriString(&tempUri, &content);
                AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));
                
                AfxResetUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt]);
                AfxReplicateUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt], &tempUri);
                config->xfmrPipb.shdStage[config->xfmrPipb.shdCnt] = avxShaderStage_FRAGMENT;
                ++config->xfmrPipb.shdCnt;
            }
            else
            {
                // ?
            }
        }
        else
        {
            AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
            hasUnhandledNodes = TRUE;
        }
    }

    return err;
}

#if 0
_AVX afxError _AfxParseXmlBackedPipelineDepthStateStencilFace(afxXmlNode const *node, afxNat elemIdx, afxNat specIdx, afxBool front, avxPipelineBlueprint const* identity, avxPipelineBlueprint *ds, avxPipelineDepthFlags *foundMask)
{
    afxError err = AFX_ERR_NONE;

    //*foundMask = NIL;

    afxNat node2AttrCnt = AfxCountXmlAttributes(node);

    for (afxNat l = 0; l < node2AttrCnt; l++)
    {
        afxString const*name = AfxXmlNodeGetAttributeName(node, l);
        afxString const *content = AfxXmlNodeGetAttributeContent(node, l);

        if (0 == AfxCompareString(name, &g_str_fail))
        {
            if (front)
            {
                ds->stencilFront.failOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT(7);
            }
            else
            {
                ds->stencilBack.failOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT(11);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_pass))
        {
            if (front)
            {
                ds->stencilFront.passOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT(8);
            }
            else
            {
                ds->stencilBack.passOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT(12);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_depthFail))
        {
            if (front)
            {
                ds->stencilFront.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT(9);
            }
            else
            {
                ds->stencilBack.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT(13);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_compare))
        {
            if (front)
            {
                ds->stencilFront.compareOp = AfxFindCompareOp(content);
                *foundMask |= AFX_BIT(10);
            }
            else
            {
                ds->stencilBack.compareOp = AfxFindCompareOp(content);
                *foundMask |= AFX_BIT(14);
            }
        }
        else
        {
            AfxLogAdvertence("Attribute '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}
#endif

_AVX afxError AfxLoadPipelineConfigFromXml(avxPipelineBlueprint* config, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    //AfxAssertType(xml, afxFcc_XML);
    //AfxAssertType(blueprint, afxFcc_PIPB);
    
    afxString name;
    afxString content;

    afxNat baseShd = config->shdCnt;
    afxNat foundShdCnt = 0;
    afxNat childCnt = AfxCountXmlChilds(xml, elemIdx);

    for (afxNat i = 0; i < childCnt; i++)
    {
        afxNat childIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childIdx, &name, &content);
        
        if (0 == AfxCompareString(&name, &AfxStaticString("PrimitiveTopology")))
        {
            avxTopology primTop;

            if ((primTop = AfxFindPrimitiveTopology(&content)))
            {
                AfxAssertRange(avxTopology_TOTAL, primTop, 1);
                config->primTop = primTop;
                config->primFlags |= avxPipelineFlag_TOPOLOGY;
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("CullMode")))
        {
            avxCullMode cullMode;

            if ((cullMode = AfxFindCullMode(&content)))
            {
                AfxAssertRange(avxCullMode_TOTAL, cullMode, 1);
                config->cullMode = cullMode;
                config->primFlags |= avxPipelineFlag_CULL_MODE;
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("FrontFace")))
        {
            afxBool invertFrontFacing;

            if ((invertFrontFacing = (0 == AfxCompareString(&content, &AfxStaticString("CW")))))
            {
                AfxAssert(!invertFrontFacing && (0 == AfxCompareString(&content, &AfxStaticString("CCW"))));
                AfxAssertBool(invertFrontFacing);
                config->invertFrontFacing = invertFrontFacing;
                config->primFlags |= avxPipelineFlag_FRONT_FACE_INV;
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DepthClampEnabled")))
        {
            config->depthClampEnabled = TRUE;
            config->primFlags |= avxPipelineFlag_DEPTH_CLAMP;
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("VertexShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxWrapUriString(&tempUri, &content);
                AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[baseShd + foundShdCnt]);
                AfxReplicateUri(&config->shdUri[baseShd + foundShdCnt], &tempUri);
                config->shdStage[baseShd + foundShdCnt] = avxShaderStage_VERTEX;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("VertexInputStream")))
        {
            afxNat instanceRate = 0;
            afxNat visSlot = 0;

            if (!AfxStringIsEmpty(&content))
            {
                if (AfxScanString(&content, "%u", &visSlot))
                {

                }
            }
            else
            {
                // ?
            }

            afxNat childTagCnt = AfxCountXmlTags(xml, childIdx);

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxCompareString(&name, &AfxStaticString("instanceRate")))
                {
                    if (AfxScanString(&content, "%u", &instanceRate))
                    {

                    }
                }
                else
                {
                    AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            //config->vis[config->visCnt].instanceRate = instanceRate;
            //config->vis[config->visCnt].slot = visSlot;
            //config->visCnt++;
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("HullShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxWrapUriString(&tempUri, &content);
                AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[baseShd + foundShdCnt]);
                AfxReplicateUri(&config->shdUri[baseShd + foundShdCnt], &tempUri);
                config->shdStage[baseShd + foundShdCnt] = avxShaderStage_TESS_EVAL;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("DomainShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxWrapUriString(&tempUri, &content);
                AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[baseShd + foundShdCnt]);
                AfxReplicateUri(&config->shdUri[baseShd + foundShdCnt], &tempUri);
                config->shdStage[baseShd + foundShdCnt] = avxShaderStage_TESS_CTRL;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxCompareString(&name, &AfxStaticString("PrimitiveShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxWrapUriString(&tempUri, &content);
                AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[baseShd + foundShdCnt]);
                AfxReplicateUri(&config->shdUri[baseShd + foundShdCnt], &tempUri);
                config->shdStage[baseShd + foundShdCnt] = avxShaderStage_PRIMITIVE;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else
        {
        AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
        }
    }

    config->shdCnt += foundShdCnt;
    return err;
}

_AVX afxError AfxParsePipelineFromXsh(avxPipelineBlueprint* pipb, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(pipb);
    AfxAssert(uri);

    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));

    AfxLogEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str));

    afxUri fext;
    AfxClipUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxClipUriPath(&fpath, uri);

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
                AfxGetUriQueryString(uri, TRUE, &query);

                afxNat xmlElemIdx = 0;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Pipeline"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (foundCnt)
                {
                    if (AfxLoadPipelineConfigFromXml(pipb, 0, &xml, xmlElemIdx)) AfxThrowError();
                    else
                    {
#if 0
                        afxString128 tmp;
                        AfxMakeString128(&tmp);
                        AfxCopyString(&tmp.str, AfxGetUriString(&fpath));

                        if (!AfxUriIsBlank(&blueprint.uri.uri))
                        {
                            AfxCatenateStringL(&tmp.str, "?", 1);
                            AfxCatenateString(&tmp.str, AfxGetUriString(&blueprint.uri.uri));
                        }

                        afxUri tmpUri;
                        AfxWrapUriString(&tmpUri, &tmp.str);
                        AfxCopyUri(&blueprint.uri.uri, &tmpUri);
#endif

                        //if (AfxCompileShadersFromXsh(dctx, config.shdCnt, config.shdUri, config.shd)) AfxThrowError();
                        //else
                        {
                            //AfxAssertObjects(config.shdCnt, &config.shd, afxFcc_SHD);

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
    return err;
}

#if 0
_AVX afxError AfxParseXmlBackedDrawOperationBlueprint(afxXmlNode const *node, afxDrawOperationBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssertType(blueprint, afxFcc_DOPB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxGetXmlNodeName(node0);
    afxString const *content;
    afxNat attrCnt = AfxCountXmlAttributes(node0);

    if (AfxCompareString(name, &g_str_DrawOperation)) AfxThrowError();
    else
    {
        AfxDrawOperationBlueprintErase(blueprint);

        afxString const *dopName = NIL;

        for (afxNat i = 0; i < attrCnt; i++)
        {
            name = AfxXmlNodeGetAttributeName(node0, i);
            content = AfxXmlNodeGetAttributeContent(node0, i);

            if (0 == AfxCompareString(name, &g_str_id))
            {
                afxUri tmpUri;
                AfxWrapUriString(&tmpUri, content);
                AfxDrawOperationBlueprintRename(blueprint, &tmpUri);
            }
            else
            {
                //AfxLogAdvertence("Attribute '%.*s' not handled yet.", AfxPushString(name));
            }
        }

        afxNat childCnt0 = AfxCountXmlChildNodes(node0);

        for (afxNat i = 0; i < childCnt0; i++)
        {
            afxXmlNode const *node1 = AfxGetXmlChildNode(node0, i);
            name = AfxGetXmlNodeName(node1);
            attrCnt = AfxCountXmlAttributes(node1);

            if (0 == AfxCompareString(name, &g_str_Technique))
            {
                afxNat tecIdx;

                if (AfxDrawOperationBlueprintAddTechnique(blueprint, &tecIdx, NIL)) AfxThrowError();
                else
                {
                    for (afxNat j = 0; j < attrCnt; j++)
                    {
                        name = AfxXmlNodeGetAttributeName(node1, j);
                        content = AfxXmlNodeGetAttributeContent(node1, j);

                        if (0 == AfxCompareString(name, &g_str_id))
                        {
                            AfxDrawOperationBlueprintRenameTechnique(blueprint, tecIdx, content);
                        }
                        else
                        {
                            //AfxLogAdvertence("Attribute '%.*s' not handled yet.", AfxPushString(name));
                        }
                    }

                    afxNat childCnt1 = AfxCountXmlChildNodes(node1);

                    for (afxNat j = 0; j < childCnt1; j++)
                    {
                        afxXmlNode const *node2 = AfxGetXmlChildNode(node1, j);
                        name = AfxGetXmlNodeName(node2);
                        attrCnt = AfxCountXmlAttributes(node2);

                        if (0 == AfxCompareString(name, &g_str_Pass))
                        {
                            afxNat passIdx;

                            if (AfxDrawOperationBlueprintAddPass(blueprint, tecIdx, &passIdx, NIL)) AfxThrowError();
                            else
                            {
                                for (afxNat k = 0; k < attrCnt; k++)
                                {
                                    name = AfxXmlNodeGetAttributeName(node2, k);
                                    content = AfxXmlNodeGetAttributeContent(node2, k);

                                    if (0 == AfxCompareString(name, &g_str_id))
                                    {
                                        AfxDrawOperationBlueprintRenamePass(blueprint, tecIdx, passIdx, content);
                                    }
                                    else
                                    {
                                        //AfxLogAdvertence("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                    }
                                }

                                afxNat childCnt2 = AfxCountXmlChildNodes(node2);

                                for (afxNat k = 0; k < childCnt2; k++)
                                {
                                    afxXmlNode const *node3 = AfxGetXmlChildNode(node2, k);
                                    name = AfxGetXmlNodeName(node3);
                                    attrCnt = AfxCountXmlAttributes(node3);

                                    if (0 == AfxCompareString(name, &g_str_Shader))
                                    {
                                        avxShaderStage stage = NIL;
                                        afxString const *shaderName = NIL;

                                        for (afxNat l = 0; l < attrCnt; l++)
                                        {
                                            name = AfxXmlNodeGetAttributeName(node3, l);
                                            content = AfxXmlNodeGetAttributeContent(node3, l);

                                            if (0 == AfxCompareString(name, &g_str_stage))
                                            {
                                                stage = AfxFindShaderStage(content);
                                            }
#if 0
                                            else if (0 == AfxCompareString(name, &g_str_uri))
                                            {
                                                afxUri tempUri;
                                                AfxWrapUriString(&tempUri, content);

                                                if (AfxDrawOperationBlueprintAddShaders(blueprint, tecIdx, passIdx, 1, &tempUri))
                                                    AfxThrowError();
                                            }
#endif
                                            else
                                            {
                                                //AfxLogAdvertence("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                            }
                                        }

                                        content = AfxGetXmlNodeContent(node3);

                                        if (content && !AfxStringIsEmpty(content))
                                        {
                                            afxUri tempUri;
                                            AfxWrapUriString(&tempUri, content);

                                            if (AfxDrawOperationBlueprintAddShaders(blueprint, tecIdx, passIdx, 1, &tempUri))
                                                AfxThrowError();
                                        }
                                    }
                                    else if (0 == AfxCompareString(name, &g_str_Input))
                                    {
                                        avxPipelinePrimitiveState ias = { avxTopology_TRI_LIST, FALSE };
                                        afxNat foundMask = NIL;

                                        if (AfxParseXmlBackedPipelinePrimitiveState(node3, &ias, &foundMask))
                                            AfxThrowError();

                                        if (foundMask)
                                            AfxDrawOperationBlueprintConfigInputAssemblyState(blueprint, tecIdx, passIdx, &ias);
                                    }
                                }

                                {
                                    avxPipelineRasterizerState rs = { FALSE, FALSE, avxFillMode_SOLID, avxCullMode_BACK, avxFrontFace_CCW, FALSE, 0, 0, 0, 1 };
                                    afxNat foundMask = NIL;

                                    if (AfxParseXmlBackedPipelineRasterizerState(node2, &rs, &foundMask))
                                        AfxThrowError();

                                    if (foundMask)
                                        AfxDrawOperationBlueprintConfigRasterizerState(blueprint, tecIdx, passIdx, &rs);
                                }

                                {
                                    avxPipelineDepthState ds = { FALSE, FALSE, avxCompareOp_ALWAYS, FALSE, FALSE, { NIL, NIL, NIL, NIL }, { NIL, NIL, NIL, NIL }, 0, 1 };
                                    afxNat foundMask = NIL;

                                    if (AfxParseXmlBackedPipelineDepthState(node2, &ds, &foundMask))
                                        AfxThrowError();

                                    if (foundMask)
                                        AfxDrawOperationBlueprintConfigDepthState(blueprint, tecIdx, passIdx, &ds);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return err;
}
#endif
#if 0
_AVX afxResult AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("dctx=%p", dctx);
    AfxAssertType(dctxD, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(dop);
    afxResult rslt = 0;

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxIoSystem fsys = AfxGetDrawStorage(dsys);
    AfxAssertObject(fsys, afxFcc_FSYS);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxXml xml;

        if (!(xml = AfxStorageLoadXml(fsys, &uri[i]))) AfxThrowError();
        else
        {
            AfxAssertObject(xml, afxFcc_XML);

            afxXmlNode const *node0 = AfxGetXmlRoot(xml);
            afxString const *name = AfxGetXmlNodeName(node0);

            if (0 == AfxCompareString(name, &g_str_Qwadro))
            {
                afxNat childCnt0 = AfxCountXmlChildNodes(node0);

                for (afxNat j = 0; j < childCnt0; j++)
                {
                    afxXmlNode const *node1 = AfxGetXmlChildNode(node0, j);
                    name = AfxGetXmlNodeName(node1);

                    if (0 == AfxCompareString(name, &g_str_DrawOperation))
                    {
                        afxDrawOperationBlueprint blueprint;

                        if (AfxParseXmlBackedDrawOperationBlueprint(node1, &blueprint)) AfxThrowError();
                        else
                        {
                            for (afxNat k = 0; k < blueprint.reqShaderCnt; k++)
                            {
                                if (AfxMeasureUriDirectory(&blueprint.reqShaders[k])) // has directory, probably is defined externaly in same XML.
                                {
                                    avxShader shd;
                                    AfxAcquireShaders(dctx, 1, &blueprint.reqShaders[k], &shd);
                                    AfxAssertObjects(1, &shd, afxFcc_SHD);
                                }
                                else
                                {
                                    afxNat childCnt10 = AfxCountXmlChildNodes(node0);

                                    for (afxNat l = 0; l < childCnt10; l++)
                                    {
                                        afxXmlNode const *node11 = AfxGetXmlChildNode(node0, l);
                                        name = AfxGetXmlNodeName(node11);

                                        if (0 == AfxCompareString(name, &g_str_Shader))
                                        {
                                            afxString const *shdName = NIL;
                                            avxShaderStage shdStage = NIL;

                                            afxNat attrCnt = AfxCountXmlAttributes(node11);

                                            for (afxNat m = 0; m < attrCnt; m++)
                                            {
                                                name = AfxXmlNodeGetAttributeName(node11, m);
                                                afxString const *content = AfxXmlNodeGetAttributeContent(node11, m);

                                                if (0 == AfxCompareString(name, &g_str_stage))
                                                {
                                                    shdStage = AfxFindShaderStage(content);
                                                }
                                                else if (0 == AfxCompareString(name, &g_str_id))
                                                {
                                                    shdName = content;
                                                }
                                                else
                                                {
                                                    AfxLogAdvertence("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                                }
                                            }

                                            if (shdName)
                                            {
                                                if (0 == AfxCompareString(shdName, AfxGetUriString(&blueprint.reqShaders[k])))
                                                {
                                                    avxShaderBlueprint shdb;
                                                    AfxParseXmlBackedShaderBlueprint(&shdb, node11);

                                                    avxShader shd;
                                                    AfxCompileShaders(dctx, 1, &shdb, &shd);
                                                    AfxAssertObjects(1, &shd, afxFcc_SHD);
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if (AfxBuildDrawOperations(dctx, 1, &blueprint, &dop[i])) AfxThrowError();
                            else
                            {
                                AfxAssertObject(dop[i], afxFcc_DOP);
                                ++rslt;
                            }
                        }
                    }
                }
            }

            AfxReleaseObject(&xml->obj);
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObject(&dop[j]->obj);
                dop[j] = NIL;
            }
            break;
        }
    }
    return rslt;
}
#endif
