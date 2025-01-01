/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "../impl/avxImplementation.h"

AVX afxChar const *shdResTypeNames[];

_AVX afxError _AvxParseXmlPipelineBlueprint(afxXml const* xml, afxUnit elemIdx, afxUnit specId, avxPipelineBlueprint* pipb, avxShaderStage shaderStages[], afxUri shaderUris[], afxString shaderFns[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pipb);

    avxPipelineBlueprint config = { 0 };
    config = *pipb;

    afxString const pipAttrNames[] =
    {
        AfxStaticString("topology"),
        AfxStaticString("cullMode"),
        AfxStaticString("frontFaceInverted"),
        AfxStaticString("depthClampEnabled"),
        // RASTERIZATION DATA AHEAD
        AfxStaticString("rasterizationDisabled"),
        AfxStaticString("fillMode"),
        AfxStaticString("depthBiasEnabled"),
        AfxStaticString("depthBiasConstant"),
        AfxStaticString("depthBiasClamp"),
        AfxStaticString("depthBiasSlope"),
        AfxStaticString("lineWidth"),
        AfxStaticString("depthTestEnabled"),
        AfxStaticString("depthWriteDisabled"),
        AfxStaticString("depthCompareOp"),
        AfxStaticString("depthBoundsTest"),
        AfxStaticString("depthBoundsTestMin"),
        AfxStaticString("depthBoundsTestMax"),
        AfxStaticString("stencilTestEnabled")
    };

    afxString name, content, acontent;
    afxUnit tagCnt = AfxCountXmlTags(xml, elemIdx);

    for (afxUnit i = 0; i < tagCnt; i++)
    {
        AfxQueryXmlTag(xml, elemIdx, i, &name, &acontent);

        switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(pipAttrNames), pipAttrNames))
        {
        case 0: // topology
        {
            avxTopology primTop;

            if ((primTop = AfxFindPrimitiveTopology(&acontent)))
            {
                AFX_ASSERT_RANGE(avxTopology_TOTAL, primTop, 1);
                config.primTop = primTop;
                config.primFlags |= avxPipelineFlag_TOPOLOGY;
            }
            break;
        }
        case 1: // cullMode
        {
            avxCullMode cullMode;

            if ((cullMode = AfxFindCullMode(&acontent)))
            {
                AFX_ASSERT_RANGE(avxCullMode_TOTAL, cullMode, 1);
                config.cullMode = cullMode;
                config.primFlags |= avxPipelineFlag_CULL_MODE;
            }
            break;
        }
        case 2: // frontFaceInverted
        {
            config.frontFacingInverted = TRUE;
            config.primFlags |= avxPipelineFlag_FRONT_FACE_INV;
            break;
        }
        case 3: // depthClampEnabled
        {
            config.depthClampEnabled = TRUE;
            config.primFlags |= avxPipelineFlag_DEPTH_CLAMP;
            break;
        }
        case 4: // rasterizationDisabled
        {
            config.rasterizationDisabled = TRUE;
            break;
        }
        case 5: // fillMode
        {
            avxFillMode fillMode = AfxFindFillMode(&acontent);
            AFX_ASSERT_RANGE(avxFillMode_TOTAL, fillMode, 1);
            config.fillMode = fillMode;
            config.rasFlags |= avxRasterizationFlag_FILL_MODE;
            break;
        }
        case 6: // depthBiasEnabled
        {
            config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
            config.depthBiasEnabled = TRUE;
            break;
        }
        case 7: // depthBiasConstant
        {
            afxReal depthBiasConstFactor;

            if (AfxScanString(&acontent, "%f", &depthBiasConstFactor))
            {
                config.depthBiasConstFactor = depthBiasConstFactor;
                config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
                config.depthBiasEnabled = TRUE;
            }
            break;
        }
        case 8: // depthBiasClamp
        {
            afxReal depthBiasClamp;

            if (AfxScanString(&acontent, "%f", &depthBiasClamp))
            {
                config.depthBiasClamp = depthBiasClamp;
                config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
                config.depthBiasEnabled = TRUE;
            }
            break;
        }
        case 9: // depthBiasSlope
        {
            afxReal depthBiasSlopeScale;

            if (AfxScanString(&acontent, "%f", &depthBiasSlopeScale))
            {
                config.depthBiasSlopeScale = depthBiasSlopeScale;
                config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
                config.depthBiasEnabled = TRUE;
            }
            break;
        }
        case 10: // lineWidth
        {
            afxReal lineWidth;

            if (AfxScanString(&acontent, "%f", &lineWidth))
            {
                config.lineWidth = lineWidth;
                config.rasFlags |= avxRasterizationFlag_LINE_WIDTH;
            }
            break;
        }
        case 11: // depthTestEnabled
        {
            config.depthTestEnabled = TRUE;
            config.dsFlags |= avxDepthStencilFlag_TEST;
            break;
        }
        case 12: // depthWriteDisabled
        {
            config.depthWriteDisabled = TRUE;
            config.dsFlags |= avxDepthStencilFlag_DONT_WRITE;
            break;
        }
        case 13: // depthCompareOp
        {
            if ((config.depthCompareOp = AfxFindCompareOp(&acontent)))
            {
                config.dsFlags |= avxDepthStencilFlag_COMPARE;
            }
            break;
        }
        case 14: // depthBoundsTest
        {
            config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            break;
        }
        case 15: // depthBoundsTestMin
        {
            afxReal mini;

            if (AfxScanString(&acontent, "%f", &mini))
            {
                config.depthBounds[0] = mini;
                config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            }
            break;
        }
        case 16: // depthBoundsTestMax
        {
            afxReal maxi;

            if (AfxScanString(&acontent, "%f", &maxi))
            {
                config.depthBounds[1] = maxi;
                config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            }
            break;
        }
        case 17: // stencilTestEnabled
        {
            config.dsFlags |= avxDepthStencilFlag_STENCIL_TEST;
            break;
        }
        default:
        {
            AfxLogY("Tag '%.*s' not handled.", AfxPushString(&name));
            break;
        }
        }
    }

    afxString const stageNames[] =
    {
        AFX_STR_EMPTY,
        AfxStaticString("VERTEX"),
        AfxStaticString("FRAGMENT"),
        AfxStaticString("PRIMITIVE"),
        AfxStaticString("DOMAIN"),
        AfxStaticString("HULL"),
        AfxStaticString("COMPUTE"),
    };

    afxUnit childCnt = AfxCountXmlChilds(xml, elemIdx);

    for (afxUnit i = 0; i < childCnt; i++)
    {
        afxUnit childElemIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childElemIdx, &name, &content);
        afxUnit childTagCnt = AfxCountXmlTags(xml, childElemIdx);

        if (0 == AfxCompareStrings(&name, 0, TRUE, 1, &AfxStaticString("Shader")))
        {
            avxShaderStage shdStage = NIL;
            afxString shdFn = { 0 };
            afxUri shdUri = { 0 };

            afxString const shdAttrNames[] =
            {
                AfxStaticString("stage"),
                AfxStaticString("uri"),
                AfxStaticString("fn")
            };

            for (afxUnit j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childElemIdx, j, &name, &acontent);

                switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(shdAttrNames), shdAttrNames))
                {
                case 0: // stage
                {
                    switch (AfxCompareStrings(&acontent, 0, TRUE, ARRAY_SIZE(stageNames), stageNames))
                    {
                    case avxShaderStage_VERTEX: shdStage = avxShaderStage_VERTEX; break;
                    case avxShaderStage_FRAGMENT: shdStage = avxShaderStage_FRAGMENT; break;
                    case avxShaderStage_PRIMITIVE: shdStage = avxShaderStage_PRIMITIVE; break;
                    case avxShaderStage_TESS_CTRL: shdStage = avxShaderStage_TESS_CTRL; break;
                    case avxShaderStage_TESS_EVAL: shdStage = avxShaderStage_TESS_EVAL; break;
                    case avxShaderStage_COMPUTE: shdStage = avxShaderStage_COMPUTE; break;
                    default:
                    {
                        AfxLogY("Shader stage '%.*s' not handled.", AfxPushString(&acontent));
                        break;
                    }
                    }
                    break;
                }
                case 1: // uri
                {
                    if (!AfxIsStringEmpty(&acontent))
                    {
                        afxUri tempUri;
                        AfxWrapUriString(&tempUri, &acontent);
                        AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                        AfxReplicateUri(&shdUri, &tempUri);
                    }
                    break;
                }
                case 2: // fn
                {
                    if (!AfxIsStringEmpty(&acontent))
                    {
                        shdFn = acontent;
                    }
                    break;
                }
                default:
                {
                    AfxLogY("Tag '%.*s' not handled.", AfxPushString(&name));
                    break;
                }
                }
            }

            if (!AfxIsStringEmpty(&content))
            {
                //AfxAcquireShaders(dsys, 1, );
            }

            if (shaderStages)
                shaderStages[config.stageCnt] = shdStage;

            if (shaderUris)
                shaderUris[config.stageCnt] = shdUri;

            if (shaderFns)
                shaderFns[config.stageCnt] = shdFn;

            ++config.stageCnt;
        }
        else if (0 == AfxCompareStrings(&name, 0, TRUE, 1, &AfxStaticString("Stencil")))
        {
            avxStencilInfo si = { 0 };

            afxString const shdAttrNames[] =
            {
                AfxStaticString("compareOp"),
                AfxStaticString("reference"),
                AfxStaticString("compareMask"),
                AfxStaticString("writeMask"),
                AfxStaticString("failOp"),
                AfxStaticString("depthFailOp"),
                AfxStaticString("passOp"),
                AfxStaticString("front"),
                AfxStaticString("back"),
            };

            afxBool front = FALSE;
            afxBool back = FALSE;

            for (afxUnit j = 0; j < childTagCnt; j++)
            {
                AfxQueryXmlTag(xml, childElemIdx, j, &name, &acontent);

                switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(shdAttrNames), shdAttrNames))
                {
                case 0: // compareOp
                {
                    si.compareOp = AfxFindCompareOp(&acontent);
                    break;
                }
                case 1: // reference
                {
                    afxUnit32 reference = 0;

                    if (AfxScanString(&acontent, "%d", &reference))
                    {
                        si.reference = reference;
                    }
                    break;
                }
                case 2: // compareMask
                {
                    afxUnit32 compareMask = 1;

                    if (AfxScanString(&acontent, "%d", &compareMask))
                    {
                        si.compareMask = compareMask;
                    }
                    break;
                }
                case 3: // writeMask
                {
                    afxUnit32 writeMask = 1;

                    if (AfxScanString(&acontent, "%d", &writeMask))
                    {
                        si.writeMask = writeMask;
                    }
                    break;
                }
                case 4: // failOp
                {
                    si.failOp = AfxFindStencilOp(&acontent);
                    break;
                }
                case 5: // depthFailOp
                {
                    si.depthFailOp = AfxFindStencilOp(&acontent);
                    break;
                }
                case 6: // passOp
                {
                    si.passOp = AfxFindStencilOp(&acontent);
                    break;
                }
                case 7: // front
                {
                    front = TRUE;
                    break;
                }
                case 8: // back
                {
                    back = TRUE;
                    break;
                }
                default:
                {
                    AfxLogY("Tag '%.*s' not handled.", AfxPushString(&name));
                    break;
                }
                }
            }

            if (front)
                config.stencilFront = si;

            if (back)
                config.stencilBack = si;
        }
        else
        {
            AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
        }
    }

    *pipb = config;

    return err;
}
