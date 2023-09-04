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

#include "afx/draw/io/afxXmlBackedDrawableResources.h"
#include "afx/draw/afxDrawSystem.h"

_AFX afxError AfxParseXmlBackedShaderBlueprint(afxXmlNode const *node, afxShaderBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssertType(blueprint, AFX_FCC_SHDB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxXmlNodeGetName(node0);
    afxString const *content;
    afxNat attrCnt0 = AfxXmlNodeGetAttributeCount(node0);

    if (AfxCompareString(name, &g_str_Shader)) AfxThrowError();
    else
    {
        AfxShaderBlueprintErase(blueprint);

        for (afxNat j = 0; j < attrCnt0; j++)
        {
            name = AfxXmlNodeGetAttributeName(node0, j);
            content = AfxXmlNodeGetAttributeContent(node0, j);

            if (0 == AfxCompareString(name, &g_str_stage))
            {
                afxShaderStage stage = NIL;

                if (!(stage = AfxFindShaderStage(content)))
                    AfxThrowError();

                AfxShaderBlueprintSetStage(blueprint, stage);
            }
            else if (0 == AfxCompareString(name, &g_str_name))
            {
                afxUri tmpUri;
                AfxReflectUriString(&tmpUri, content);

                AfxShaderBlueprintRename(blueprint, &tmpUri);
            }
            else if (0 == AfxCompareString(name, &g_str_entry))
            {
                AfxShaderBlueprintChooseEntryPoint(blueprint, content);
            }
            else
            {
                AfxAdvertise("Attribute '%.*s' not handled.", AfxPushString(name));
            }
        }

        afxNat childCnt0 = AfxXmlNodeGetChildrenCount(node0);

        for (afxNat j = 0; j < childCnt0; j++)
        {
            afxXmlNode const *node1 = AfxXmlNodeGetChild(node0, j);
            name = AfxXmlNodeGetName(node1);
            afxNat attrCnt1 = AfxXmlNodeGetAttributeCount(node1);

            if (0 == AfxCompareString(name, &g_str_Include))
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
                content = AfxXmlNodeGetContent(node1);

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
                afxString const *resName = NIL;

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
                        
                        if (resType == AFX_SHD_RES_TYPE_OUTPUT)
                        {
                            int a = 0;
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

                if (AfxShaderBlueprintDeclareResource(blueprint, resSet, resBinding, resType, resCnt, AfxXmlNodeGetContent(node1)))
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

                if (AfxShaderBlueprintDeclareInOut(blueprint, ioLocation, ioFormat, AfxXmlNodeGetContent(node1)))
                    AfxThrowError();
            }
        }

        content = AfxXmlNodeGetContent(node0);

        if (content && !AfxStringIsEmpty(content))
        {
            if (AfxShaderBlueprintAddCode(blueprint, AfxGetStringDataConst(content, 0), AfxGetStringSize(content)))
                AfxThrowError();
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelineInputAssemblyState(afxXmlNode const *node, afxPipelineInputAssemblyState *state, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(state);
    AfxAssert(node);
    //*state = (afxPipelineInputAssemblyState) { 0 };
    *foundMask = 0;

    afxNat nodeAttrCnt = AfxXmlNodeGetAttributeCount(node);
    afxString const *name = AfxXmlNodeGetName(node);

    if (0 == AfxCompareString(name, &g_str_Input))
    {
        for (afxNat j = 0; j < nodeAttrCnt; j++)
        {
            name = AfxXmlNodeGetAttributeName(node, j);
            afxString const *content = AfxXmlNodeGetAttributeContent(node, j);

            if (0 == AfxCompareString(name, &g_str_topology))
            {
                state->topology = AfxFindPrimitiveTopology(content);
                *foundMask |= AFX_FLAG(0);
            }
            else if (0 == AfxCompareString(name, &g_str_restart))
            {
                state->primRestartEnable = (0 == AfxCompareString(content, &g_str_true));
                *foundMask |= AFX_FLAG(1);
            }
            else
            {
                //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
            }
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelineRasterizerState(afxXmlNode const *node, afxPipelineRasterizerState *state, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(state);
    AfxAssert(node);
    //*state = (afxPipelineRasterizerState) { 0 };
    *foundMask = NIL;
    afxNat baseChildCnt = AfxXmlNodeGetChildrenCount(node);

    for (afxNat i = 0; i < baseChildCnt; i++)
    {
        afxXmlNode const *node1 = AfxXmlNodeGetChild(node, i);
        afxNat node1AttrCnt = AfxXmlNodeGetAttributeCount(node1);
        afxString const *name = AfxXmlNodeGetName(node1);
        afxString const *content = AfxXmlNodeGetContent(node1);

        if (0 == AfxCompareString(name, &g_str_DepthClampEnable))
        {
            state->depthClampEnable = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_FLAG(0);
        }
        else if (0 == AfxCompareString(name, &g_str_RasterizerDiscardEnable))
        {
            state->rasterizerDiscardEnable = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_FLAG(1);
        }
        else if (0 == AfxCompareString(name, &g_str_FillMode))
        {
            state->fillMode = AfxFindFillMode(content);
            *foundMask |= AFX_FLAG(2);
        }
        else if (0 == AfxCompareString(name, &g_str_CullMode))
        {
            state->cullMode = AfxFindCullMode(content);
            *foundMask |= AFX_FLAG(3);
        }
        else if (0 == AfxCompareString(name, &g_str_FrontFace))
        {
            state->frontFace = AfxFindFrontFace(content);
            *foundMask |= AFX_FLAG(4);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthBias))
        {
            for (afxNat j = 0; j < node1AttrCnt; j++)
            {
                name = AfxXmlNodeGetAttributeName(node1, j);
                content = AfxXmlNodeGetAttributeContent(node1, j);

                if (0 == AfxCompareString(name, &g_str_enable))
                {
                    state->depthBiasEnable = (0 == AfxCompareString(content, &g_str_true));
                    *foundMask |= AFX_FLAG(5);
                }
                else if (0 == AfxCompareString(name, &g_str_constant))
                {
                    AfxScanString(content, "%f", &state->depthBiasConstantFactor);
                    *foundMask |= AFX_FLAG(6);
                }
                else if (0 == AfxCompareString(name, &g_str_clamp))
                {
                    AfxScanString(content, "%f", &state->depthBiasClamp);
                    *foundMask |= AFX_FLAG(7);
                }
                else if (0 == AfxCompareString(name, &g_str_slope))
                {
                    AfxScanString(content, "%f", &state->depthBiasSlopeFactor);
                    *foundMask |= AFX_FLAG(8);
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
            *foundMask |= AFX_FLAG(9);
        }
        else
        {
            //AfxError("Node '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}

_AFX afxError _AfxParseXmlBackedPipelineDepthStateStencilFace(afxXmlNode const *node, afxBool front, afxPipelineDepthState *ds, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;

    //*foundMask = NIL;

    afxNat node2AttrCnt = AfxXmlNodeGetAttributeCount(node);

    for (afxNat l = 0; l < node2AttrCnt; l++)
    {
        afxString const*name = AfxXmlNodeGetAttributeName(node, l);
        afxString const *content = AfxXmlNodeGetAttributeContent(node, l);

        if (0 == AfxCompareString(name, &g_str_fail))
        {
            if (front)
            {
                ds->stencilOpFront.failOp = AfxFindStencilOp(content);
                *foundMask |= AFX_FLAG(7);
            }
            else
            {
                ds->stencilOpBack.failOp = AfxFindStencilOp(content);
                *foundMask |= AFX_FLAG(11);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_pass))
        {
            if (front)
            {
                ds->stencilOpFront.passOp = AfxFindStencilOp(content);
                *foundMask |= AFX_FLAG(8);
            }
            else
            {
                ds->stencilOpBack.passOp = AfxFindStencilOp(content);
                *foundMask |= AFX_FLAG(12);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_depthFail))
        {
            if (front)
            {
                ds->stencilOpFront.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AFX_FLAG(9);
            }
            else
            {
                ds->stencilOpBack.depthFailOp = AfxFindStencilOp(content);
                *foundMask |= AFX_FLAG(13);
            }
        }
        else if (0 == AfxCompareString(name, &g_str_compare))
        {
            if (front)
            {
                ds->stencilOpFront.compareOp = AfxFindCompareOp(content);
                *foundMask |= AFX_FLAG(10);
            }
            else
            {
                ds->stencilOpBack.compareOp = AfxFindCompareOp(content);
                *foundMask |= AFX_FLAG(14);
            }
        }
        else
        {
            //AfxError("Attribute '%.*s' not handled.", AfxPushString(name));
        }
    }
    return err;
}

_AFX afxError AfxParseXmlBackedPipelineDepthState(afxXmlNode const *node, afxPipelineDepthState *state, afxNat *foundMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(state);
    AfxAssert(node);

    *foundMask = NIL;
    //*state = (afxPipelineDepthState) { 0 };

    afxNat baseChildCnt = AfxXmlNodeGetChildrenCount(node);

    for (afxNat i = 0; i < baseChildCnt; i++)
    {
        afxXmlNode const *node1 = AfxXmlNodeGetChild(node, i);
        afxNat node1ChildCnt = AfxXmlNodeGetChildrenCount(node1);
        afxNat node1AttrCnt = AfxXmlNodeGetAttributeCount(node1);
        afxString const *name = AfxXmlNodeGetName(node1);
        afxString const *content = AfxXmlNodeGetContent(node1);

        if (0 == AfxCompareString(name, &g_str_DepthTestEnable))
        {
            state->depthTestEnable = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_FLAG(0);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthWriteEnable))
        {
            state->depthWriteEnable = (0 == AfxCompareString(content, &g_str_true));
            *foundMask |= AFX_FLAG(1);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthCompareOp))
        {
            state->depthCompareOp = AfxFindCompareOp(content);
            *foundMask |= AFX_FLAG(2);
        }
        else if (0 == AfxCompareString(name, &g_str_DepthBoundsTest))
        {
            for (afxNat j = 0; j < node1AttrCnt; j++)
            {
                name = AfxXmlNodeGetAttributeName(node1, j);
                content = AfxXmlNodeGetAttributeContent(node1, j);

                if (0 == AfxCompareString(name, &g_str_enable))
                {
                    state->depthBoundsTestEnable = (0 == AfxCompareString(content, &g_str_true));
                    *foundMask |= AFX_FLAG(3);
                }
                else if (0 == AfxCompareString(name, &g_str_min))
                {
                    AfxScanString(content, "%f", &state->minDepthBounds);
                    *foundMask |= AFX_FLAG(4);
                }
                else if (0 == AfxCompareString(name, &g_str_max))
                {
                    AfxScanString(content, "%f", &state->maxDepthBounds);
                    *foundMask |= AFX_FLAG(5);
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
                    if ((state->stencilTestEnable = (0 == AfxCompareString(content, &g_str_true))))
                    {
                        *foundMask |= AFX_FLAG(6);

                        for (afxNat k = 0; k < node1ChildCnt; k++)
                        {
                            afxXmlNode const *node2 = AfxXmlNodeGetChild(node1, k);
                            name = AfxXmlNodeGetName(node2);

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

_AFX afxError AfxParseXmlBackedPipelineBlueprint(afxXmlNode const *node, afxPipelineBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssertType(blueprint, AFX_FCC_PIPB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxXmlNodeGetName(node0);
    afxString const *content;
    afxNat attrCnt = AfxXmlNodeGetAttributeCount(node0);

    if (AfxCompareString(name, &g_str_Pipeline)) AfxThrowError();
    else
    {
        AfxPipelineBlueprintErase(blueprint);

        for (afxNat i = 0; i < attrCnt; i++)
        {
            name = AfxXmlNodeGetAttributeName(node0, i);
            content = AfxXmlNodeGetAttributeContent(node0, i);

            if (0 == AfxCompareString(name, &g_str_name))
            {
                afxUri tmpUri;
                AfxReflectUriString(&tmpUri, content);
                AfxCopyUri(&blueprint->uri.uri , &tmpUri);
            }
            else
            {
                //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
            }
        }

        afxNat childCnt2 = AfxXmlNodeGetChildrenCount(node0);

        for (afxNat k = 0; k < childCnt2; k++)
        {
            afxXmlNode const *node3 = AfxXmlNodeGetChild(node0, k);
            name = AfxXmlNodeGetName(node3);
            attrCnt = AfxXmlNodeGetAttributeCount(node3);

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

                content = AfxXmlNodeGetContent(node3);

                if (content && !AfxStringIsEmpty(content))
                {
                    afxUri tempUri;
                    AfxReflectUriString(&tempUri, content);
                    AfxEcho("%.*s", AfxPushString(AfxUriGetStringConst(&tempUri)));

                    if (AfxPipelineBlueprintAddShaders(blueprint, 1, &tempUri))
                        AfxThrowError();
                }
            }
            else if (0 == AfxCompareString(name, &g_str_Input))
            {
                afxPipelineInputAssemblyState ias = { afxPrimTopology_TRI_LIST, FALSE };
                afxNat foundMask = NIL;

                if (AfxParseXmlBackedPipelineInputAssemblyState(node3, &ias, &foundMask))
                    AfxThrowError();

                if (foundMask)
                    AfxPipelineBlueprintConfigInputAssemblyState(blueprint, &ias);
            }
        }

        {
            afxPipelineRasterizerState rs = { FALSE, FALSE, afxFillMode_SOLID, afxCullMode_BACK, afxFrontFace_CCW, FALSE, 0, 0, 0, 1 };
            afxNat foundMask = NIL;

            if (AfxParseXmlBackedPipelineRasterizerState(node0, &rs, &foundMask))
                AfxThrowError();

            if (foundMask)
                AfxPipelineBlueprintConfigRasterizerState(blueprint, &rs);
        }

        {
            afxPipelineDepthState ds = { FALSE, FALSE, afxCompareOp_ALWAYS, FALSE, FALSE, { NIL, NIL, NIL, NIL }, { NIL, NIL, NIL, NIL }, 0, 1 };
            afxNat foundMask = NIL;

            if (AfxParseXmlBackedPipelineDepthState(node0, &ds, &foundMask))
                AfxThrowError();

            if (foundMask)
                AfxPipelineBlueprintConfigDepthState(blueprint, &ds);
        }
    }
    return err;
}

#if 0
_AFX afxError AfxParseXmlBackedDrawOperationBlueprint(afxXmlNode const *node, afxDrawOperationBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(node);
    AfxAssertType(blueprint, AFX_FCC_DOPB);
    afxXmlNode const *node0 = node;
    afxString const *name = AfxXmlNodeGetName(node0);
    afxString const *content;
    afxNat attrCnt = AfxXmlNodeGetAttributeCount(node0);

    if (AfxCompareString(name, &g_str_DrawOperation)) AfxThrowError();
    else
    {
        AfxDrawOperationBlueprintErase(blueprint);

        afxString const *dopName = NIL;

        for (afxNat i = 0; i < attrCnt; i++)
        {
            name = AfxXmlNodeGetAttributeName(node0, i);
            content = AfxXmlNodeGetAttributeContent(node0, i);

            if (0 == AfxCompareString(name, &g_str_name))
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

        afxNat childCnt0 = AfxXmlNodeGetChildrenCount(node0);

        for (afxNat i = 0; i < childCnt0; i++)
        {
            afxXmlNode const *node1 = AfxXmlNodeGetChild(node0, i);
            name = AfxXmlNodeGetName(node1);
            attrCnt = AfxXmlNodeGetAttributeCount(node1);

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

                        if (0 == AfxCompareString(name, &g_str_name))
                        {
                            AfxDrawOperationBlueprintRenameTechnique(blueprint, tecIdx, content);
                        }
                        else
                        {
                            //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                        }
                    }

                    afxNat childCnt1 = AfxXmlNodeGetChildrenCount(node1);

                    for (afxNat j = 0; j < childCnt1; j++)
                    {
                        afxXmlNode const *node2 = AfxXmlNodeGetChild(node1, j);
                        name = AfxXmlNodeGetName(node2);
                        attrCnt = AfxXmlNodeGetAttributeCount(node2);

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

                                    if (0 == AfxCompareString(name, &g_str_name))
                                    {
                                        AfxDrawOperationBlueprintRenamePass(blueprint, tecIdx, passIdx, content);
                                    }
                                    else
                                    {
                                        //AfxAdvertise("Attribute '%.*s' not handled yet.", AfxPushString(name));
                                    }
                                }

                                afxNat childCnt2 = AfxXmlNodeGetChildrenCount(node2);

                                for (afxNat k = 0; k < childCnt2; k++)
                                {
                                    afxXmlNode const *node3 = AfxXmlNodeGetChild(node2, k);
                                    name = AfxXmlNodeGetName(node3);
                                    attrCnt = AfxXmlNodeGetAttributeCount(node3);

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

                                        content = AfxXmlNodeGetContent(node3);

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
                                        afxPipelineInputAssemblyState ias = { afxPrimTopology_TRI_LIST, FALSE };
                                        afxNat foundMask = NIL;

                                        if (AfxParseXmlBackedPipelineInputAssemblyState(node3, &ias, &foundMask))
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
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(dop);
    afxResult rslt = 0;

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_CTX);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertType(dsys, AFX_FCC_DSYS);
    afxIoSystem fsys = AfxGetDrawFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertType(&uri[i], AFX_FCC_URI);
        afxXml xml;

        if (!(xml = AfxFileSystemLoadXml(fsys, &uri[i]))) AfxThrowError();
        else
        {
            AfxAssertObject(xml, AFX_FCC_XML);

            afxXmlNode const *node0 = AfxXmlGetRootNode(xml);
            afxString const *name = AfxXmlNodeGetName(node0);

            if (0 == AfxCompareString(name, &g_str_Qwadro))
            {
                afxNat childCnt0 = AfxXmlNodeGetChildrenCount(node0);

                for (afxNat j = 0; j < childCnt0; j++)
                {
                    afxXmlNode const *node1 = AfxXmlNodeGetChild(node0, j);
                    name = AfxXmlNodeGetName(node1);

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
                                    AfxAssertObjects(1, &shd, AFX_FCC_SHD);
                                }
                                else
                                {
                                    afxNat childCnt10 = AfxXmlNodeGetChildrenCount(node0);

                                    for (afxNat l = 0; l < childCnt10; l++)
                                    {
                                        afxXmlNode const *node11 = AfxXmlNodeGetChild(node0, l);
                                        name = AfxXmlNodeGetName(node11);

                                        if (0 == AfxCompareString(name, &g_str_Shader))
                                        {
                                            afxString const *shdName = NIL;
                                            afxShaderStage shdStage = NIL;

                                            afxNat attrCnt = AfxXmlNodeGetAttributeCount(node11);

                                            for (afxNat m = 0; m < attrCnt; m++)
                                            {
                                                name = AfxXmlNodeGetAttributeName(node11, m);
                                                afxString const *content = AfxXmlNodeGetAttributeContent(node11, m);

                                                if (0 == AfxCompareString(name, &g_str_stage))
                                                {
                                                    shdStage = AfxFindShaderStage(content);
                                                }
                                                else if (0 == AfxCompareString(name, &g_str_name))
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
                                                    AfxAssertObjects(1, &shd, AFX_FCC_SHD);
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if (AfxBuildDrawOperations(dctx, 1, &blueprint, &dop[i])) AfxThrowError();
                            else
                            {
                                AfxAssertObject(dop[i], AFX_FCC_DOP);
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
