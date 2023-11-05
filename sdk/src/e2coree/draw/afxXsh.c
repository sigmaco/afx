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

#include "afx/draw/afxXsh.h"
#include "afx/draw/afxDrawSystem.h"

AFX afxChar const *shdResTypeNames[];

_AFX afxError AfxParseXmlBackedShaderBlueprint(afxXmlNode const *node, afxShaderBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssertType(blueprint, afxFcc_SHDB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxGetXmlNodeName(node0);
    afxString const *content;
    afxNat attrCnt0 = AfxCountXmlAttributes(node0);

    if (AfxCompareString(name, &g_str_Shader)) AfxThrowError();
    else
    {
        AfxShaderBlueprintErase(blueprint);

        for (afxNat j = 0; j < attrCnt0; j++)
        {
            name = AfxXmlNodeGetAttributeName(node0, j);
            content = AfxXmlNodeGetAttributeContent(node0, j);

            if (0 == AfxCompareString(name, &g_str_id))
            {
                afxUri tmpUri;
                AfxReflectUriString(&tmpUri, content);

                AfxShaderBlueprintRename(blueprint, &tmpUri);
            }
            else
            {
                AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
            }
        }

        afxNat childCnt0 = AfxCountXmlChildNodes(node0);

        for (afxNat j = 0; j < childCnt0; j++)
        {
            afxXmlNode const *node1 = AfxGetXmlChildNode(node0, j);
            name = AfxGetXmlNodeName(node1);
            afxNat attrCnt1 = AfxCountXmlAttributes(node1);
            content = AfxGetXmlNodeContent(node1);

            if (0 == AfxCompareString(name, &g_str_Stage))
            {
                afxShaderStage stage = NIL;

                if (!(stage = AfxFindShaderStage(content)))
                    AfxThrowError();

                AfxShaderBlueprintSetStage(blueprint, stage);
            }
            else if (0 == AfxCompareString(name, &g_str_Entry))
            {
                AfxShaderBlueprintChooseEntryPoint(blueprint, content);
            }
            else if (0 == AfxCompareString(name, &g_str_Include))
            {
#if 0
                for (afxNat k = 0; k < attrCnt1; k++)
                {
                    name = AfxXmlNodeGetAttributeName(node1, k);
                    content = AfxXmlNodeGetAttributeContent(node1, k);

                    if (0 == AfxCompareString(name, &g_str_uri))
                    {
                        afxUri uri;
                        AfxReflectUriString(&uri, content);

                        if (AfxShaderBlueprintAddCodeFromResource(blueprint, &uri))
                            AfxThrowError();
                    }
                    else
                    {
                        AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
                    }
                }
#endif
                if (content && !AfxStringIsEmpty(content))
                {
                    afxUri uri;
                    AfxReflectUriString(&uri, content);

                    if (AfxShaderBlueprintAddCodeFromResource(blueprint, &uri))
                        AfxThrowError();
                }
            }
            else if (0 == AfxCompareString(name, &g_str_Resource))
            {
                afxNat resSet = 0;
                afxNat resBinding = 0;
                afxNat resCnt = 1;
                afxShaderResourceType resType = 0;
                //afxString const *resName = NIL;

                for (afxNat k = 0; k < attrCnt1; k++)
                {
                    name = AfxXmlNodeGetAttributeName(node1, k);
                    content = AfxXmlNodeGetAttributeContent(node1, k);

                    if (0 == AfxCompareString(name, &g_str_set))
                    {
                        AfxScanString(content, "%u", &resSet);
                    }
                    else if (0 == AfxCompareString(name, &g_str_binding))
                    {
                        AfxScanString(content, "%u", &resBinding);
                    }
                    else if (0 == AfxCompareString(name, &g_str_count))
                    {
                        AfxScanString(content, "%u", &resCnt);
                    }
                    else if (0 == AfxCompareString(name, &g_str_type))
                    {
                        for (afxNat l = 0; l < AFX_SHD_RES_TYPE_TOTAL; l++)
                        {
                            if (0 == AfxCompareStringLiteral(content, 0, shdResTypeNames[l], 0))
                            {
                                resType = (afxShaderResourceType)l;
                                break;
                            }
                        }                        
                    }
#if 0
                    else if (0 == AfxCompareString(name, &g_str_name))
                    {
                        resName = content;
                    }
#endif
                    else
                    {
                        AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
                    }
                }

                if (AfxShaderBlueprintDeclareResource(blueprint, resSet, resBinding, resType, resCnt, AfxGetXmlNodeContent(node1)))
                    AfxThrowError();
            }
            else if ((0 == AfxCompareString(name, &g_str_In)) || (0 == AfxCompareString(name, &g_str_Out)))
            {
                afxNat ioLocation = 0;
                afxVertexFormat ioFormat = NIL;

                for (afxNat k = 0; k < attrCnt1; k++)
                {
                    name = AfxXmlNodeGetAttributeName(node1, k);
                    content = AfxXmlNodeGetAttributeContent(node1, k);

                    if (0 == AfxCompareString(name, &g_str_location))
                    {
                        AfxScanString(content, "%u", &ioLocation);
                    }
                    else if (0 == AfxCompareString(name, &g_str_format))
                    {
                        ioFormat = AfxFindVertexFormat(content);
                    }
                    else
                    {
                        AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
                    }
                }

                if (AfxShaderBlueprintDeclareInOut(blueprint, ioLocation, ioFormat, AfxGetXmlNodeContent(node1)))
                    AfxThrowError();
            }
            else if (0 == AfxCompareString(name, &g_str_Flag))
            {
                AfxAdvertise("%.*s : flag = %.*s", AfxPushString(name), AfxPushString(content));
            }
        }

        content = AfxGetXmlNodeContent(node0);

        if (content && !AfxStringIsEmpty(content))
        {
            if (AfxShaderBlueprintAddCode(blueprint, AfxGetStringDataConst(content, 0), AfxGetStringSize(content)))
                AfxThrowError();
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelinePrimitiveState(afxXmlNode const *node, afxPipelineConfig *state, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(state);
    AfxAssert(node);
    //*state = (afxPipelinePrimitiveState) { 0 };
    *foundMask = 0;

    afxNat nodeAttrCnt = AfxCountXmlAttributes(node);
    afxString const *name = AfxGetXmlNodeName(node);

    if (0 == AfxCompareString(name, &g_str_Input))
    {
        for (afxNat j = 0; j < nodeAttrCnt; j++)
        {
            name = AfxXmlNodeGetAttributeName(node, j);
            afxString const *content = AfxXmlNodeGetAttributeContent(node, j);

            if (0 == AfxCompareString(name, &g_str_restart))
            {
                state->primRestartEnabled = (0 == AfxCompareString(content, &g_str_true));
                *foundMask |= AFX_BIT_OFFSET(1);
            }
            else
            {
                //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
            }
        }
    }

    afxNat baseChildCnt = AfxCountXmlChildNodes(node);

    for (afxNat i = 0; i < baseChildCnt; i++)
    {
        afxXmlNode const *node1 = AfxGetXmlChildNode(node, i);
        //afxNat node1AttrCnt = AfxCountXmlAttributes(node1);
        afxString const *name = AfxGetXmlNodeName(node1);
        afxString const *content = AfxGetXmlNodeContent(node1);

        if (0 == AfxCompareString(name, &g_str_PrimitiveTopology))
        {
            state->primTop = AfxFindPrimitiveTopology(content);
            *foundMask |= AFX_BIT_OFFSET(0);
        }
        else if (0 == AfxCompareString(name, &g_str_CullMode))
        {
            state->cullMode = AfxFindCullMode(content);
            *foundMask |= AFX_BIT_OFFSET(2);
        }
        else if (0 == AfxCompareString(name, &g_str_FrontFace))
        {
            state->cwFrontFacing = (0 == AfxCompareStringLiteralCi(content, 0, "CW", 2));
            *foundMask |= AFX_BIT_OFFSET(3);
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelineRasterizerState(afxXmlNode const *node, afxPipelineConfig *state, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(state);
    AfxAssert(node);
    //*state = (afxPipelineRasterizerState) { 0 };
    *foundMask = NIL;
    afxNat baseChildCnt = AfxCountXmlChildNodes(node);

    for (afxNat i = 0; i < baseChildCnt; i++)
    {
        afxXmlNode const *node1 = AfxGetXmlChildNode(node, i);
        afxNat node1AttrCnt = AfxCountXmlAttributes(node1);
        afxString const *name = AfxGetXmlNodeName(node1);
        afxString const *content = AfxGetXmlNodeContent(node1);

        if (0 == AfxCompareString(name, &g_str_DepthClampEnable))
        {
            state->depthClampEnabled = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_BIT_OFFSET(0);
        }
        else if (0 == AfxCompareString(name, &g_str_RasterizerDiscardEnable))
        {
            state->rasterizationDisabled = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_BIT_OFFSET(1);
        }
        else if (0 == AfxCompareString(name, &g_str_FillMode))
        {
            state->fillMode = AfxFindFillMode(content);
            *foundMask |= AFX_BIT_OFFSET(2);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthBias))
        {
            for (afxNat j = 0; j < node1AttrCnt; j++)
            {
                name = AfxXmlNodeGetAttributeName(node1, j);
                content = AfxXmlNodeGetAttributeContent(node1, j);

                if (0 == AfxCompareString(name, &g_str_enable))
                {
                    state->depthBiasEnabled = (0 == AfxCompareString(content, &g_str_true));
                    *foundMask |= AFX_BIT_OFFSET(5);
                }
                else if (0 == AfxCompareString(name, &g_str_constant))
                {
                    AfxScanString(content, "%f", &state->depthBiasConstFactor);
                    *foundMask |= AFX_BIT_OFFSET(6);
                }
                else if (0 == AfxCompareString(name, &g_str_clamp))
                {
                    AfxScanString(content, "%f", &state->depthBiasClamp);
                    *foundMask |= AFX_BIT_OFFSET(7);
                }
                else if (0 == AfxCompareString(name, &g_str_slope))
                {
                    AfxScanString(content, "%f", &state->depthBiasSlopeScale);
                    *foundMask |= AFX_BIT_OFFSET(8);
                }
                else
                {
                    //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
                }
            }
        }
        else if (0 == AfxCompareString(name, &g_str_LineWidth))
        {
            AfxScanString(content, "%f", &state->lineWidth);
            *foundMask |= AFX_BIT_OFFSET(9);
        }
        else
        {
            //AfxError("Node '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}

_AFX afxError _AfxParseXmlBackedPipelineDepthStateStencilFace(afxXmlNode const *node, afxBool front, afxPipelineConfig *ds, afxNat *foundMask)
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
                *foundMask |= AFX_BIT_OFFSET(7);
            }
            else
            {
                ds->stencilBack.failOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT_OFFSET(11);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_pass))
        {
            if (front)
            {
                ds->stencilFront.passOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT_OFFSET(8);
            }
            else
            {
                ds->stencilBack.passOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT_OFFSET(12);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_depthFail))
        {
            if (front)
            {
                ds->stencilFront.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT_OFFSET(9);
            }
            else
            {
                ds->stencilBack.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AFX_BIT_OFFSET(13);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_compare))
        {
            if (front)
            {
                ds->stencilFront.compareOp = AfxFindCompareOp(content);
                *foundMask |= AFX_BIT_OFFSET(10);
            }
            else
            {
                ds->stencilBack.compareOp = AfxFindCompareOp(content);
                *foundMask |= AFX_BIT_OFFSET(14);
            }
        }
        else
        {
            //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelineDepthState(afxXmlNode const *node, afxPipelineConfig *state, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(state);
    AfxAssert(node);

    *foundMask = NIL;
    //*state = (afxPipelineDepthState) { 0 };

    afxNat baseChildCnt = AfxCountXmlChildNodes(node);

    for (afxNat i = 0; i < baseChildCnt; i++)
    {
        afxXmlNode const *node1 = AfxGetXmlChildNode(node, i);
        afxNat node1ChildCnt = AfxCountXmlChildNodes(node1);
        afxNat node1AttrCnt = AfxCountXmlAttributes(node1);
        afxString const *name = AfxGetXmlNodeName(node1);
        afxString const *content = AfxGetXmlNodeContent(node1);

        if (0 == AfxCompareString(name, &g_str_DepthTestEnable))
        {
            state->depthTestEnabled = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_BIT_OFFSET(0);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthWriteEnable))
        {
            state->depthWriteEnabled = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_BIT_OFFSET(1);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthCompareOp))
        {
            state->depthCompareOp = AfxFindCompareOp(content);
            *foundMask |= AFX_BIT_OFFSET(2);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthBoundsTest))
        {
            for (afxNat j = 0; j < node1AttrCnt; j++)
            {
                name = AfxXmlNodeGetAttributeName(node1, j);
                content = AfxXmlNodeGetAttributeContent(node1, j);

                if (0 == AfxCompareString(name, &g_str_enable))
                {
                    state->depthBoundsTestEnabled = (0 == AfxCompareString(content, &g_str_true));
                    *foundMask |= AFX_BIT_OFFSET(3);
                }
                else if (0 == AfxCompareString(name, &g_str_min))
                {
                    AfxScanString(content, "%f", &state->depthBounds[0]);
                    *foundMask |= AFX_BIT_OFFSET(4);
                }
                else if (0 == AfxCompareString(name, &g_str_max))
                {
                    AfxScanString(content, "%f", &state->depthBounds[1]);
                    *foundMask |= AFX_BIT_OFFSET(5);
                }
                else
                {
                    //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
                }
            }
        }
        else if (0 == AfxCompareString(name, &g_str_StencilTest))
        {
            for (afxNat j = 0; j < node1AttrCnt; j++)
            {
                name = AfxXmlNodeGetAttributeName(node1, j);
                content = AfxXmlNodeGetAttributeContent(node1, j);

                if (0 == AfxCompareString(name, &g_str_enable))
                {
                    if ((state->stencilTestEnabled = (0 == AfxCompareString(content, &g_str_true))))
                    {
                        *foundMask |= AFX_BIT_OFFSET(6);

                        for (afxNat k = 0; k < node1ChildCnt; k++)
                        {
                            afxXmlNode const *node2 = AfxGetXmlChildNode(node1, k);
                            name = AfxGetXmlNodeName(node2);

                            if (0 == AfxCompareString(name, &g_str_Front))
                            {
                                _AfxParseXmlBackedPipelineDepthStateStencilFace(node2, TRUE, state, foundMask); // 7 - 8 - 9 - 10

                            }
                            else if (0 == AfxCompareString(name, &g_str_Back))
                            {
                                _AfxParseXmlBackedPipelineDepthStateStencilFace(node2, FALSE, state, foundMask); // 11 - 12 - 13 - 14
                            }
                            else
                            {
                                //AfxError("Node '%.*s' not handled.", AfxPushString(name));
                            }
                        }
                    }
                }
                else
                {
                    //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
                }
            }
        }
        else
        {
            //AfxError("Node '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelineBlueprint(afxXmlNode const *node, afxPipelineConfig *blueprint, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    //AfxAssertType(blueprint, afxFcc_PIPB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxGetXmlNodeName(node0);
    afxString const *content;
    afxNat attrCnt = AfxCountXmlAttributes(node0);

    if (AfxCompareString(name, &g_str_Pipeline)) AfxThrowError();
    else
    {
        //AfxPipelineBlueprintErase(blueprint);

        for (afxNat i = 0; i < attrCnt; i++)
        {
            name = AfxXmlNodeGetAttributeName(node0, i);
            content = AfxXmlNodeGetAttributeContent(node0, i);

            if (0 == AfxCompareString(name, &g_str_id))
            {
                afxUri tmpUri;
                AfxReflectUriString(&tmpUri, content);
                //AfxCopyUri(&blueprint->uri.uri , &tmpUri);
            }
            else
            {
                //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
            }
        }

        afxNat childCnt2 = AfxCountXmlChildNodes(node0);

        for (afxNat k = 0; k < childCnt2; k++)
        {
            afxXmlNode const *node3 = AfxGetXmlChildNode(node0, k);
            name = AfxGetXmlNodeName(node3);
            attrCnt = AfxCountXmlAttributes(node3);

            if (0 == AfxCompareString(name, &g_str_Shader))
            {
                afxShaderStage stage = NIL;
                //afxString const *shaderName = NIL;

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
                        AfxReflectUriString(&tempUri, content);

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
                    AfxReflectUriString(&tempUri, content);
                    AfxEcho("%.*s", AfxPushString(AfxUriGetStringConst(&tempUri)));

                    if (!(blueprint->sh[blueprint->shdCnt] = AfxCompileShaderFromXsh(dctx, &tempUri))) AfxThrowError();
                    else
                    {
                        blueprint->shdCnt++;
                    }
                }
            }
            else 
            {
                if (0 == AfxCompareString(name, &g_str_Input))
                {
                    

                }

            }
        }

        {
            blueprint->primTop = afxPrimTopology_TRI_LIST;
            blueprint->primRestartEnabled = FALSE;
            blueprint->cullMode = afxCullMode_BACK;
            blueprint->cwFrontFacing = FALSE;
            afxNat foundMask = NIL;

            if (AfxParseXmlBackedPipelinePrimitiveState(node0, blueprint, &foundMask))
            {
                AfxThrowError();
            }
        }

        {
            blueprint->rasterizationDisabled = FALSE;
            blueprint->depthBiasClamp = FALSE;
            blueprint->fillMode = afxFillMode_SOLID;
            blueprint->depthBiasEnabled = FALSE;
            blueprint->depthBiasSlopeScale = 0;
            blueprint->depthBiasConstFactor = 0;
            blueprint->depthBiasClamp = 0;
            blueprint->lineWidth = 1;
            afxNat foundMask = NIL;

            if (AfxParseXmlBackedPipelineRasterizerState(node0, blueprint, &foundMask))
                AfxThrowError();
        }

        {
            blueprint->depthTestEnabled = FALSE;
            blueprint->depthWriteEnabled = FALSE;
            blueprint->depthCompareOp = afxCompareOp_ALWAYS;
            blueprint->stencilTestEnabled = FALSE;
            blueprint->stencilFront.compareMask = (blueprint->stencilBack.compareMask = 1);
            blueprint->stencilFront.compareOp = (blueprint->stencilBack.compareOp = afxCompareOp_ALWAYS);
            blueprint->stencilFront.depthFailOp = (blueprint->stencilBack.depthFailOp = afxStencilOp_KEEP);
            blueprint->stencilFront.failOp = (blueprint->stencilBack.failOp = afxStencilOp_KEEP);
            blueprint->stencilFront.passOp = (blueprint->stencilBack.passOp = afxStencilOp_KEEP);
            blueprint->stencilFront.reference = (blueprint->stencilBack.reference = 1);
            blueprint->stencilFront.writeMask = (blueprint->stencilBack.writeMask = 1);
            blueprint->depthBoundsTestEnabled = FALSE;
            blueprint->depthBounds[0] = 0;
            blueprint->depthBounds[1] = 1;
            afxNat foundMask = NIL;

            if (AfxParseXmlBackedPipelineDepthState(node0, blueprint, &foundMask))
                AfxThrowError();
        }
    }
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
                AfxReflectUriString(&tmpUri, content);
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
                            //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
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
                                        //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
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
                                        afxShaderStage stage = NIL;
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
                                                AfxReflectUriString(&tempUri, content);

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
                                            AfxReflectUriString(&tempUri, content);

                                            if (AfxDrawOperationBlueprintAddShaders(blueprint, tecIdx, passIdx, 1, &tempUri))
                                                AfxThrowError();
                                        }
                                    }
                                    else if (0 == AfxCompareString(name, &g_str_Input))
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

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);
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
                                if (AfxUriHasDirectory(&blueprint.reqShaders[k])) // has directory, probably is defined externaly in same XML.
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

                                        if (0 == AfxCompareString(name, &g_str_Shader))
                                        {
                                            afxString const *shdName = NIL;
                                            afxShaderStage shdStage = NIL;

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
                                                    AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                                }
                                            }

                                            if (shdName)
                                            {
                                                if (0 == AfxCompareString(shdName, AfxUriGetString(&blueprint.reqShaders[k])))
                                                {
                                                    afxShaderBlueprint shdb;
                                                    AfxParseXmlBackedShaderBlueprint(&shdb, node11);

                                                    afxShader shd;
                                                    AfxBuildShaders(dctx, 1, &shdb, &shd);
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
