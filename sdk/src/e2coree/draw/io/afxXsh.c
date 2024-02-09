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

#include "qwadro/draw/io/afxXsh.h"
#include "qwadro/draw/pipe/afxShaderBlueprint.h"
#include "qwadro/io/afxFile.h"

AFX afxChar const *shdResTypeNames[];

_AFX afxError AfxLoadGlScript(afxArray* code, afxUri const* path)
{
    afxError err = NIL;

    afxString keyword;
    AfxMakeString(&keyword, "#include ", 0);
    afxFile file;

    if (AfxOpenFiles(afxFileFlag_R, 1, path, &file)) AfxThrowError();
    else
    {
        afxNat baseChar;
        afxFixedString4096 src;
        AfxMakeFixedString4096(&src, NIL);
        afxFixedString2048 line;
        AfxMakeFixedString2048(&line, NIL);

        while (!AfxReadFileString(file, &line.str))
        {
            afxString excerpt;

            if (AfxExtractExcerpt(&line.str.str, &keyword, &excerpt))
            {
                afxUri inc;
                afxChar uriBuf[2048];
                AfxScanString(&excerpt, "#include %[\"%s\" ^\n]", uriBuf);
                AfxMakeBufferedUri(&inc, AFX_COUNTOF(uriBuf), uriBuf, 0);

                void* room = AfxInsertArrayUnits(code, line.str.str.len + 3, &baseChar, NIL);
                AfxDumpString(&AfxString("// "), 0, 3, room);
                AfxDumpString(&line.str.str, 0, line.str.str.len, room);

                if (AfxLoadGlScript(code, &inc))
                    AfxThrowError();

                continue;
            }

            void* room = AfxInsertArrayUnits(code, line.str.str.len, &baseChar, NIL);
            AfxDumpString(&line.str.str, 0, line.str.str.len, room);
        }
        AfxReleaseObjects(1, (void*[]) { file });
    }
    return err;
}

_AFX afxError AfxParseXmlBackedShaderBlueprint(afxShaderBlueprint *blueprint, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(xml, afxFcc_XML);
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxShaderBlueprintErase(blueprint);

    afxNat childTagCnt;
    afxString name;
    afxString content;
    afxNat childCnt = AfxCountXmlChilds(xml, elemIdx);

    for (afxNat i = 0; i < childCnt; i++)
    {
        afxNat childIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childIdx, &name, &content);

        if (0 == AfxTestStringEquality(&name, &AfxStaticString("Stage")))
        {
            afxShaderStage stage = NIL;

            if (!(stage = AfxFindShaderStage(&content)))
                AfxThrowError();

            AfxShaderBlueprintSetStage(blueprint, stage);
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("Entry")))
        {
            AfxShaderBlueprintChooseEntryPoint(blueprint, &content);
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("Include")))
        {
#if 0
            for (afxNat k = 0; k < attrCnt1; k++)
            {
                name = AfxXmlNodeGetAttributeName(node1, k);
                content = AfxXmlNodeGetAttributeContent(node1, k);

                if (0 == AfxTestStringEquality(name, &AfxStaticString("uri")))
                {
                    afxUri uri;
                    AfxUriFromString(&uri, content);

                    if (AfxShaderBlueprintAddCodeFromResource(blueprint, &uri))
                        AfxThrowError();
                }
                else
                {
                    AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
                }
            }
#endif
            if (!AfxStringIsEmpty(&content))
            {
                afxUri uri;
                AfxUriFromString(&uri, &content);

                if (AfxShaderBlueprintAddCodeFromResource(blueprint, &uri))
                    AfxThrowError();
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("Resource")))
        {
            afxNat resSet = 0;
            afxNat resBinding = 0;
            afxNat resCnt = 1;
            afxShaderResourceType resType = 0;
            afxString resName;
            AfxReplicateString(&resName, &content);

            childTagCnt = AfxCountXmlTags(xml, childIdx);

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxTestStringEquality(&name, &AfxStaticString("set")))
                {
                    AfxScanString(&content, "%u", &resSet);
                }
                else if (0 == AfxTestStringEquality(&name, &AfxStaticString("binding")))
                {
                    AfxScanString(&content, "%u", &resBinding);
                }
                else if (0 == AfxTestStringEquality(&name, &AfxStaticString("count")))
                {
                    AfxScanString(&content, "%u", &resCnt);
                }
                else if (0 == AfxTestStringEquality(&name, &AfxStaticString("type")))
                {
                    for (afxNat l = 0; l < AFX_SHD_RES_TYPE_TOTAL; l++)
                    {
                        if (0 == AfxTestStringEqualityLiteral(&content, 0, shdResTypeNames[l], 0))
                        {
                            resType = (afxShaderResourceType)l;
                            break;
                        }
                    }                        
                }
#if 0
                else if (0 == AfxTestStringEquality(name, &AfxStaticString("name")))
                {
                    resName = content;
                }
#endif
                else
                {
                    AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            if (AfxShaderBlueprintDeclareResource(blueprint, resSet, resBinding, resType, resCnt, &resName))
                AfxThrowError();
        }
        else if ((0 == AfxTestStringEquality(&name, &g_str_In)) || (0 == AfxTestStringEquality(&name, &g_str_Out)))
        {
            afxNat ioLocation = 0;
            afxVertexFormat ioFormat = NIL;
            afxNat inStream = 0;
            afxFixedString32 fmtName, ioName;
            fmtName.buf[0] = '\0';
            ioName.buf[0] = '\0';
            AfxScanString(&content, "%8s %32[a-z,0-9,_]", fmtName.buf, ioName.buf);
            AfxMakeString(&fmtName.str.str, fmtName.buf, 0);
            AfxMakeString(&ioName.str.str, ioName.buf, 0);
            ioFormat = AfxFindVertexFormat(&fmtName.str.str);
            AfxAssertRange(afxVertexFormat_TOTAL, ioFormat, 1);
            
            childTagCnt = AfxCountXmlTags(xml, childIdx);

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);
                
                if (0 == AfxTestStringEquality(&name, &AfxStaticString("location")))
                {
                    AfxScanString(&content, "%u", &ioLocation);
                }
                else
                {
                    AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            AfxCatchError(err);

            if (AfxShaderBlueprintDeclareInOut(blueprint, ioLocation, ioFormat, inStream, &ioName.str.str))
                AfxThrowError();
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("Flag")))
        {
            AfxAdvertise("%.*s : flag = %.*s", AfxPushString(&name), AfxPushString(&content));
        }
    }

    AfxQueryXmlElement(xml, elemIdx, &name, &content);

    if (!AfxStringIsEmpty(&content))
    {
        if (AfxShaderBlueprintAddCode(blueprint, AfxGetStringData(&content, 0), AfxGetStringLength(&content)))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxLoadRasterizationConfigFromXml(afxRasterizationConfig* config, afxRasterizationConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    AfxAssert(identity);
    AfxAssertType(xml, afxFcc_XML);
    
    afxBool hasUnhandledNodes = FALSE;
    afxNat childCnt = AfxCountXmlChilds(xml, elemIdx);
    afxString name, content;

    for (afxNat i = 0; i < childCnt; i++)
    {
        afxNat childIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childIdx, &name, &content);
        afxNat childTagCnt = AfxCountXmlTags(xml, childIdx);

        if (0 == AfxTestStringEquality(&name, &AfxStaticString("RasterizerDiscardEnable")))
        {
            afxBool rasterizationDisabled;

            if ((rasterizationDisabled = (0 == AfxTestStringEquality(&content, &AfxStaticString("true")))))
            {
                AfxAssertBool(rasterizationDisabled);
                config->rasFlags |= NIL;

                // retornar sem a chance de buscar por mais entradas, uma vez que não faria sentido as ter se não para usar.
                break;
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("FillMode")))
        {
            afxFillMode fillMode = AfxFindFillMode(&content);
            AfxAssertRange(afxFillMode_TOTAL, fillMode, 1);
            config->fillMode = fillMode;
            config->rasFlags |= afxRasterizationFlag_FILL_MODE;
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DepthBias")))
        {
            afxBool enabledSet = FALSE, constantSet = FALSE, clampSet = FALSE, slopeSet = FALSE;

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxTestStringEquality(&name, &AfxStaticString("constant")))
                {
                    afxBool depthBiasConstFactor;
                    
                    if (AfxScanString(&content, "%f", &depthBiasConstFactor))
                    {
                        config->depthBiasConstFactor = depthBiasConstFactor;
                        constantSet = TRUE;
                        enabledSet = TRUE;
                    }
                }
                else if (0 == AfxTestStringEquality(&name, &AfxStaticString("clamp")))
                {
                    afxBool depthBiasClamp;

                    if (AfxScanString(&content, "%f", &depthBiasClamp))
                    {
                        config->depthBiasClamp = depthBiasClamp;
                        clampSet = TRUE;
                        enabledSet = TRUE;
                    }
                }
                else if (0 == AfxTestStringEquality(&name, &AfxStaticString("slope")))
                {
                    afxBool depthBiasSlopeScale;
                    
                    if (AfxScanString(&content, "%f", &depthBiasSlopeScale))
                    {
                        config->depthBiasSlopeScale = depthBiasSlopeScale;
                        slopeSet = TRUE;
                        enabledSet = TRUE;
                    }
                }
                else
                {
                    AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            if (enabledSet)
            {
                config->rasFlags |= afxRasterizationFlag_DEPTH_BIAS;

                if (!clampSet)
                {
                    config->depthBiasClamp = identity->depthBiasClamp;
                    AfxAdvertise("");
                }

                if (!constantSet)
                {
                    config->depthBiasConstFactor = identity->depthBiasConstFactor;
                    AfxAdvertise("");
                }

                if (!slopeSet)
                {
                    config->depthBiasSlopeScale = identity->depthBiasSlopeScale;
                    AfxAdvertise("");
                }
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("LineWidth")))
        {
            afxReal lineWidth;

            if (AfxScanString(&content, "%f", &lineWidth))
            {
                config->lineWidth = lineWidth;
                config->rasFlags |= afxRasterizationFlag_LINE_WIDTH;
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DepthTestEnabled")))
        {
            config->depthTestEnabled = TRUE;
            config->dsFlags |= afxDepthStencilFlag_TEST;
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DepthWriteDisabled")))
        {
            config->depthWriteDisabled = TRUE;
            config->dsFlags |= afxDepthStencilFlag_DONT_WRITE;
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DepthCompareOp")))
        {
            if ((config->depthCompareOp = AfxFindCompareOp(&content)))
            {
                config->dsFlags |= afxDepthStencilFlag_COMPARE;
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DepthBoundsTest")))
        {
            afxBool boundsSet = FALSE, boundsMinSet = FALSE, boundsMaxSet = FALSE;

            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxTestStringEquality(&name, &AfxStaticString("min")))
                {
                    afxReal mini;

                    if (AfxScanString(&content, "%f", &mini))
                    {
                        config->depthBounds[0] = mini;
                        boundsMinSet = TRUE;
                        boundsSet = TRUE;
                    }
                }
                else if (0 == AfxTestStringEquality(&name, &AfxStaticString("max")))
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
                    AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }

            if (boundsSet)
            {
                config->dsFlags = afxDepthStencilFlag_BOUNDS_TEST;

                if (!boundsMinSet)
                {
                    config->depthBounds[0] = identity->depthBounds[0];
                    AfxAdvertise("");
                }

                if (!boundsMaxSet)
                {
                    config->depthBounds[1] = identity->depthBounds[1];
                    AfxAdvertise("");
                }
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("StencilTest")))
        {
            for (afxNat j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childIdx, j, &name, &content);

                if (0 == AfxTestStringEquality(&name, &AfxStaticString("enable")))
                {
                    if ((config->stencilTestEnabled = (0 == AfxTestStringEquality(&content, &AfxStaticString("true")))))
                    {
                        config->dsFlags |= afxDepthStencilFlag_STENCIL_TEST;
#if !0
                        afxNat childCnt2 = AfxCountXmlChilds(xml, childIdx);

                        for (afxNat k = 0; k < childCnt2; k++)
                        {
                            afxNat childIdx2 = AfxGetXmlChild(xml, childIdx, k);
                            AfxQueryXmlElement(xml, childIdx2, &name, &content);

                            if (0 == AfxTestStringEquality(&name, &AfxStaticString("Front")))
                            {
                                //_AfxParseXmlBackedPipelineDepthStateStencilFace(childChild, elemIdx, specIdx, TRUE, state, state, foundMask); // 7 - 8 - 9 - 10

                            }
                            else if (0 == AfxTestStringEquality(&name, &AfxStaticString("Back")))
                            {
                                //_AfxParseXmlBackedPipelineDepthStateStencilFace(childChild, elemIdx, specIdx, FALSE, state, state, foundMask); // 11 - 12 - 13 - 14
                            }
                            else
                            {
                                AfxAdvertise("Node '%.*s' not handled.", AfxPushString(&name));
                            }
                        }
#endif
                    }
                }
                else
                {
                    AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(&name));
                }
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("FragmentShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxUriFromString(&tempUri, &content);
                AfxEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->fragShd);
                AfxReplicateUri(&config->fragShd, &tempUri);
                AfxResetString(&config->fragFn);
            }
            else
            {
                // ?
            }
        }
        else
        {
            AfxAdvertise("Node '%.*s' not handled.", AfxPushString(&name));
            hasUnhandledNodes = TRUE;
        }
    }

    return err;
}

#if 0
_AFX afxError _AfxParseXmlBackedPipelineDepthStateStencilFace(afxXmlNode const *node, afxNat elemIdx, afxNat specIdx, afxBool front, afxPipelineConfig const* identity, afxPipelineConfig *ds, afxPipelineDepthFlags *foundMask)
{
    afxError err = AFX_ERR_NONE;

    //*foundMask = NIL;

    afxNat node2AttrCnt = AfxCountXmlAttributes(node);

    for (afxNat l = 0; l < node2AttrCnt; l++)
    {
        afxString const*name = AfxXmlNodeGetAttributeName(node, l);
        afxString const *content = AfxXmlNodeGetAttributeContent(node, l);

        if (0 == AfxTestStringEquality(name, &g_str_fail))
        {
            if (front)
            {
                ds->stencilFront.failOp = AfxFindStencilOp(content);
                *foundMask |= AfxGetBitOffset(7);
            }
            else
            {
                ds->stencilBack.failOp = AfxFindStencilOp(content);
                *foundMask |= AfxGetBitOffset(11);
            }
        }
        else if (0 == AfxTestStringEquality(name, &g_str_pass))
        {
            if (front)
            {
                ds->stencilFront.passOp = AfxFindStencilOp(content);
                *foundMask |= AfxGetBitOffset(8);
            }
            else
            {
                ds->stencilBack.passOp = AfxFindStencilOp(content);
                *foundMask |= AfxGetBitOffset(12);
            }
        }
        else if (0 == AfxTestStringEquality(name, &g_str_depthFail))
        {
            if (front)
            {
                ds->stencilFront.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AfxGetBitOffset(9);
            }
            else
            {
                ds->stencilBack.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AfxGetBitOffset(13);
            }
        }
        else if (0 == AfxTestStringEquality(name, &g_str_compare))
        {
            if (front)
            {
                ds->stencilFront.compareOp = AfxFindCompareOp(content);
                *foundMask |= AfxGetBitOffset(10);
            }
            else
            {
                ds->stencilBack.compareOp = AfxFindCompareOp(content);
                *foundMask |= AfxGetBitOffset(14);
            }
        }
        else
        {
            AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}
#endif

_AFX afxError AfxLoadPipelineConfigFromXml(afxPipelineConfig* config, afxPipelineConfig const* identity, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    AfxAssert(identity);
    AfxAssertType(xml, afxFcc_XML);
    //AfxAssertType(blueprint, afxFcc_PIPB);
    
    afxString name;
    afxString content;

    afxNat foundShdCnt = 0;
    afxNat childCnt = AfxCountXmlChilds(xml, elemIdx);

    for (afxNat i = 0; i < childCnt; i++)
    {
        afxNat childIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childIdx, &name, &content);
        
        if (0 == AfxTestStringEquality(&name, &AfxStaticString("PrimitiveTopology")))
        {
            afxPrimTopology primTop;

            if ((primTop = AfxFindPrimitiveTopology(&content)))
            {
                AfxAssertRange(afxPrimTopology_TOTAL, primTop, 1);
                config->primTop = primTop;
                config->primFlags |= afxPipelinePrimitiveFlag_TOPOLOGY;
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("CullMode")))
        {
            afxCullMode cullMode;

            if ((cullMode = AfxFindCullMode(&content)))
            {
                AfxAssertRange(afxCullMode_TOTAL, cullMode, 1);
                config->cullMode = cullMode;
                config->primFlags |= afxPipelinePrimitiveFlag_CULL_MODE;
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("FrontFace")))
        {
            afxBool invertFrontFacing;

            if ((invertFrontFacing = (0 == AfxTestStringEquality(&content, &AfxStaticString("CW")))))
            {
                AfxAssert(!invertFrontFacing && (0 == AfxTestStringEquality(&content, &AfxStaticString("CCW"))));
                AfxAssertBool(invertFrontFacing);
                config->invertFrontFacing = invertFrontFacing;
                config->primFlags |= afxPipelinePrimitiveFlag_FRONT_FACE_INV;
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DepthClampEnabled")))
        {
            config->depthClampEnabled = TRUE;
            config->primFlags |= afxPipelinePrimitiveFlag_DEPTH_CLAMP;
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("VertexShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxUriFromString(&tempUri, &content);
                AfxEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[foundShdCnt]);
                AfxReplicateUri(&config->shdUri[foundShdCnt], &tempUri);
                config->shdStage[foundShdCnt] = afxShaderStage_VERTEX;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("HullShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxUriFromString(&tempUri, &content);
                AfxEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[foundShdCnt]);
                AfxReplicateUri(&config->shdUri[foundShdCnt], &tempUri);
                config->shdStage[foundShdCnt] = afxShaderStage_TESS_EVAL;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("DomainShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxUriFromString(&tempUri, &content);
                AfxEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[foundShdCnt]);
                AfxReplicateUri(&config->shdUri[foundShdCnt], &tempUri);
                config->shdStage[foundShdCnt] = afxShaderStage_TESS_CTRL;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("PrimitiveShader")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxUriFromString(&tempUri, &content);
                AfxEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                AfxResetUri(&config->shdUri[foundShdCnt]);
                AfxReplicateUri(&config->shdUri[foundShdCnt], &tempUri);
                config->shdStage[foundShdCnt] = afxShaderStage_PRIMITIVE;
                ++foundShdCnt;
            }
            else
            {
                // ?
            }
        }
        else if (0 == AfxTestStringEquality(&name, &AfxStaticString("Rasterizer")))
        {
            if (!AfxStringIsEmpty(&content))
            {
                afxUri tempUri;
                AfxUriFromString(&tempUri, &content);
                AfxEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                config->razr = NIL;
                AfxResetUri(&config->razrUri);
                AfxReplicateUri(&config->razrUri, &tempUri);
            }
            else
            {
                config->razr = NIL;
                AfxResetUri(&config->razrUri);
            }
        }
        else
        {
            AfxAdvertise("Node '%.*s' not handled.", AfxPushString(&name));
        }
    }

    config->shdCnt = foundShdCnt;
    return err;
}

#if 0
_AFX afxError AfxParseXmlBackedDrawOperationBlueprint(afxXmlNode const *node, afxDrawOperationBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssertType(blueprint, afxFcc_DOPB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxGetXmlNodeName(node0);
    afxString const *content;
    afxNat attrCnt = AfxCountXmlAttributes(node0);

    if (AfxTestStringEquality(name, &g_str_DrawOperation)) AfxThrowError();
    else
    {
        AfxDrawOperationBlueprintErase(blueprint);

        afxString const *dopName = NIL;

        for (afxNat i = 0; i < attrCnt; i++)
        {
            name = AfxXmlNodeGetAttributeName(node0, i);
            content = AfxXmlNodeGetAttributeContent(node0, i);

            if (0 == AfxTestStringEquality(name, &g_str_id))
            {
                afxUri tmpUri;
                AfxUriFromString(&tmpUri, content);
                AfxDrawOperationBlueprintRename(blueprint, &tmpUri);
            }
            else
            {
                //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
            }
        }

        afxNat childCnt0 = AfxCountXmlChildNodes(node0);

        for (afxNat i = 0; i < childCnt0; i++)
        {
            afxXmlNode const *node1 = AfxGetXmlChildNode(node0, i);
            name = AfxGetXmlNodeName(node1);
            attrCnt = AfxCountXmlAttributes(node1);

            if (0 == AfxTestStringEquality(name, &g_str_Technique))
            {
                afxNat tecIdx;

                if (AfxDrawOperationBlueprintAddTechnique(blueprint, &tecIdx, NIL)) AfxThrowError();
                else
                {
                    for (afxNat j = 0; j < attrCnt; j++)
                    {
                        name = AfxXmlNodeGetAttributeName(node1, j);
                        content = AfxXmlNodeGetAttributeContent(node1, j);

                        if (0 == AfxTestStringEquality(name, &g_str_id))
                        {
                            AfxDrawOperationBlueprintRenameTechnique(blueprint, tecIdx, content);
                        }
                        else
                        {
                            //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                        }
                    }

                    afxNat childCnt1 = AfxCountXmlChildNodes(node1);

                    for (afxNat j = 0; j < childCnt1; j++)
                    {
                        afxXmlNode const *node2 = AfxGetXmlChildNode(node1, j);
                        name = AfxGetXmlNodeName(node2);
                        attrCnt = AfxCountXmlAttributes(node2);

                        if (0 == AfxTestStringEquality(name, &g_str_Pass))
                        {
                            afxNat passIdx;

                            if (AfxDrawOperationBlueprintAddPass(blueprint, tecIdx, &passIdx, NIL)) AfxThrowError();
                            else
                            {
                                for (afxNat k = 0; k < attrCnt; k++)
                                {
                                    name = AfxXmlNodeGetAttributeName(node2, k);
                                    content = AfxXmlNodeGetAttributeContent(node2, k);

                                    if (0 == AfxTestStringEquality(name, &g_str_id))
                                    {
                                        AfxDrawOperationBlueprintRenamePass(blueprint, tecIdx, passIdx, content);
                                    }
                                    else
                                    {
                                        //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                    }
                                }

                                afxNat childCnt2 = AfxCountXmlChildNodes(node2);

                                for (afxNat k = 0; k < childCnt2; k++)
                                {
                                    afxXmlNode const *node3 = AfxGetXmlChildNode(node2, k);
                                    name = AfxGetXmlNodeName(node3);
                                    attrCnt = AfxCountXmlAttributes(node3);

                                    if (0 == AfxTestStringEquality(name, &g_str_Shader))
                                    {
                                        afxShaderStage stage = NIL;
                                        afxString const *shaderName = NIL;

                                        for (afxNat l = 0; l < attrCnt; l++)
                                        {
                                            name = AfxXmlNodeGetAttributeName(node3, l);
                                            content = AfxXmlNodeGetAttributeContent(node3, l);

                                            if (0 == AfxTestStringEquality(name, &g_str_stage))
                                            {
                                                stage = AfxFindShaderStage(content);
                                            }
#if 0
                                            else if (0 == AfxTestStringEquality(name, &g_str_uri))
                                            {
                                                afxUri tempUri;
                                                AfxUriFromString(&tempUri, content);

                                                if (AfxDrawOperationBlueprintAddShaders(blueprint, tecIdx, passIdx, 1, &tempUri))
                                                    AfxThrowError();
                                            }
#endif
                                            else
                                            {
                                                //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                            }
                                        }

                                        content = AfxGetXmlNodeContent(node3);

                                        if (content && !AfxStringIsEmpty(content))
                                        {
                                            afxUri tempUri;
                                            AfxUriFromString(&tempUri, content);

                                            if (AfxDrawOperationBlueprintAddShaders(blueprint, tecIdx, passIdx, 1, &tempUri))
                                                AfxThrowError();
                                        }
                                    }
                                    else if (0 == AfxTestStringEquality(name, &g_str_Input))
                                    {
                                        afxPipelinePrimitiveState ias = { afxPrimTopology_TRI_LIST, FALSE };
                                        afxNat foundMask = NIL;

                                        if (AfxParseXmlBackedPipelinePrimitiveState(node3, &ias, &foundMask))
                                            AfxThrowError();

                                        if (foundMask)
                                            AfxDrawOperationBlueprintConfigInputAssemblyState(blueprint, tecIdx, passIdx, &ias);
                                    }
                                }

                                {
                                    afxPipelineRasterizerState rs = { FALSE, FALSE, afxFillMode_SOLID, afxCullMode_BACK, afxFrontFace_CCW, FALSE, 0, 0, 0, 1 };
                                    afxNat foundMask = NIL;

                                    if (AfxParseXmlBackedPipelineRasterizerState(node2, &rs, &foundMask))
                                        AfxThrowError();

                                    if (foundMask)
                                        AfxDrawOperationBlueprintConfigRasterizerState(blueprint, tecIdx, passIdx, &rs);
                                }

                                {
                                    afxPipelineDepthState ds = { FALSE, FALSE, afxCompareOp_ALWAYS, FALSE, FALSE, { NIL, NIL, NIL, NIL }, { NIL, NIL, NIL, NIL }, 0, 1 };
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
_AFX afxResult AfxUploadXmlBackedDrawOperations(afxNat cnt, afxUri const uri[], afxDrawContext dctx, afxDrawOperation dop[])
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
    AfxAssertType(dsys, afxFcc_DSYS);
    afxIoSystem fsys = AfxGetDrawFileSystem(dsys);
    AfxAssertObject(fsys, afxFcc_FSYS);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertType(&uri[i], afxFcc_URI);
        afxXml xml;

        if (!(xml = AfxFileSystemLoadXml(fsys, &uri[i]))) AfxThrowError();
        else
        {
            AfxAssertObject(xml, afxFcc_XML);

            afxXmlNode const *node0 = AfxGetXmlRoot(xml);
            afxString const *name = AfxGetXmlNodeName(node0);

            if (0 == AfxTestStringEquality(name, &g_str_Qwadro))
            {
                afxNat childCnt0 = AfxCountXmlChildNodes(node0);

                for (afxNat j = 0; j < childCnt0; j++)
                {
                    afxXmlNode const *node1 = AfxGetXmlChildNode(node0, j);
                    name = AfxGetXmlNodeName(node1);

                    if (0 == AfxTestStringEquality(name, &g_str_DrawOperation))
                    {
                        afxDrawOperationBlueprint blueprint;

                        if (AfxParseXmlBackedDrawOperationBlueprint(node1, &blueprint)) AfxThrowError();
                        else
                        {
                            for (afxNat k = 0; k < blueprint.reqShaderCnt; k++)
                            {
                                if (AfxGetUriDirectoryLength(&blueprint.reqShaders[k])) // has directory, probably is defined externaly in same XML.
                                {
                                    afxShader shd;
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

                                        if (0 == AfxTestStringEquality(name, &g_str_Shader))
                                        {
                                            afxString const *shdName = NIL;
                                            afxShaderStage shdStage = NIL;

                                            afxNat attrCnt = AfxCountXmlAttributes(node11);

                                            for (afxNat m = 0; m < attrCnt; m++)
                                            {
                                                name = AfxXmlNodeGetAttributeName(node11, m);
                                                afxString const *content = AfxXmlNodeGetAttributeContent(node11, m);

                                                if (0 == AfxTestStringEquality(name, &g_str_stage))
                                                {
                                                    shdStage = AfxFindShaderStage(content);
                                                }
                                                else if (0 == AfxTestStringEquality(name, &g_str_id))
                                                {
                                                    shdName = content;
                                                }
                                                else
                                                {
                                                    AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                                }
                                            }

                                            if (shdName)
                                            {
                                                if (0 == AfxTestStringEquality(shdName, AfxGetBufferedUriString(&blueprint.reqShaders[k])))
                                                {
                                                    afxShaderBlueprint shdb;
                                                    AfxParseXmlBackedShaderBlueprint(&shdb, node11);

                                                    afxShader shd;
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
