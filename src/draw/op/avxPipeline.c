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

#define _AVX_DRAW_C
#define _AVX_PIPELINE_C
#define _AVX_RASTERIZER_C
#include "../impl/avxImplementation.h"

_AVX afxColor const AVX_BLEND_CONSTANTS_DEFAULT = { 0, 0, 0, 0 };

_AVX avxStencilInfo const AVX_STENCIL_INFO_DEFAULT =
{
    .compareOp = avxCompareOp_ALWAYS,
    .reference = 0,
    .compareMask = 1,
    .writeMask = 1,
    .failOp = avxStencilOp_KEEP,
    .depthFailOp = avxStencilOp_KEEP,
    .passOp = avxStencilOp_KEEP,
};

_AVX avxColorBlend const AVX_COLOR_BLEND_DEFAULT =
{
    .rgbBlendOp = avxBlendOp_ADD,
    .rgbSrcFactor = avxBlendFactor_ONE,
    .rgbDstFactor = avxBlendFactor_ZERO,
    .aBlendOp = avxBlendOp_ADD,
    .aSrcFactor = avxBlendFactor_ONE,
    .aDstFactor = avxBlendFactor_ZERO
};

_AVX avxColorOutput const AVX_COLOR_OUTPUT_DEFAULT =
{
    .fmt = avxFormat_UNDEFINED,
    .blendEnabled = FALSE,
    .blendConfig = AVX_COLOR_BLEND_DEFAULT, // ignore the IntelliDumb
    .discardMask = 0x00000000
};

_AVX avxPipelineBlueprint const AVX_PIPELINE_BLUEPRINT_DEFAULT =
{
    .stageCnt = 1,
    .transformationDisabled = FALSE,
    .vin = NIL,
    .primTop = avxTopology_TRI_LIST,
    .primRestartEnabled = FALSE,
    .tesselationEnabled = FALSE,
    .patchCtrlPoints = 0,
    .primShaderSupported = FALSE,
    
    .vpCnt = 1,
    .cullMode = avxCullMode_BACK,
    .frontFacingInverted = FALSE,
    .depthClampEnabled = FALSE,

    .rasterizationDisabled = FALSE,
    .fillMode = avxFillMode_SOLID,
    .lineWidth = 1.f,

    .depthBiasEnabled = FALSE,
    .depthBiasSlopeScale = 0.f,
    .depthBiasConstFactor = 0.f,
    .depthBiasClamp = 0.f,

    .msEnabled = FALSE,
    .sampleLvl = 0,
    .sampleMasks =
    {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    },
    
    .alphaToOneEnabled = FALSE,
    .alphaToCoverageEnabled = FALSE,
    .sampleShadingEnabled = FALSE,
    .minSampleShadingValue = 0.f,

    .stencilTestEnabled = FALSE,
    .stencilFront = AVX_STENCIL_INFO_DEFAULT,
    .stencilBack = AVX_STENCIL_INFO_DEFAULT,

    .depthTestEnabled = FALSE,
    .depthCompareOp = avxCompareOp_LESS,
    .depthWriteDisabled = FALSE,
    .dsFmt = avxFormat_UNDEFINED,
    
    .depthBoundsTestEnabled = FALSE,
    .depthBounds = { 0, 1 },

    // color bending, logical op and color writing
    .colorOutCnt = 1,
    .colorOuts =
    {
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT, 
        AVX_COLOR_OUTPUT_DEFAULT 
    },
    .blendConstants = { 0, 0, 0, 0 },
    .pixelLogicOpEnabled = FALSE,
    .pixelLogicOp = avxLogicOp_COPY
};

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
                //config.primFlags |= avxPipelineFlag_TOPOLOGY;
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
                //config.primFlags |= avxPipelineFlag_CULL_MODE;
            }
            break;
        }
        case 2: // frontFaceInverted
        {
            config.frontFacingInverted = TRUE;
            //config.primFlags |= avxPipelineFlag_FRONT_FACE_INV;
            break;
        }
        case 3: // depthClampEnabled
        {
            config.depthClampEnabled = TRUE;
            //config.primFlags |= avxPipelineFlag_DEPTH_CLAMP;
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
            //config.rasFlags |= avxRasterizationFlag_FILL_MODE;
            break;
        }
        case 6: // depthBiasEnabled
        {
            //config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
            config.depthBiasEnabled = TRUE;
            break;
        }
        case 7: // depthBiasConstant
        {
            afxReal depthBiasConstFactor;

            if (AfxScanString(&acontent, "%f", &depthBiasConstFactor))
            {
                config.depthBiasConstFactor = depthBiasConstFactor;
                //config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
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
                //config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
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
                //config.rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
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
                //config.rasFlags |= avxRasterizationFlag_LINE_WIDTH;
            }
            break;
        }
        case 11: // depthTestEnabled
        {
            config.depthTestEnabled = TRUE;
            //config.dsFlags |= avxDepthStencilFlag_TEST;
            break;
        }
        case 12: // depthWriteDisabled
        {
            config.depthWriteDisabled = TRUE;
            //config.dsFlags |= avxDepthStencilFlag_DONT_WRITE;
            break;
        }
        case 13: // depthCompareOp
        {
            if ((config.depthCompareOp = AfxFindCompareOp(&acontent)))
            {
                //config.dsFlags |= avxDepthStencilFlag_COMPARE;
            }
            break;
        }
        case 14: // depthBoundsTest
        {
            //config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            break;
        }
        case 15: // depthBoundsTestMin
        {
            afxReal mini;

            if (AfxScanString(&acontent, "%f", &mini))
            {
                config.depthBounds[0] = mini;
                //config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            }
            break;
        }
        case 16: // depthBoundsTestMax
        {
            afxReal maxi;

            if (AfxScanString(&acontent, "%f", &maxi))
            {
                config.depthBounds[1] = maxi;
                //config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            }
            break;
        }
        case 17: // stencilTestEnabled
        {
            //config.dsFlags |= avxDepthStencilFlag_STENCIL_TEST;
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

////////////////////////////////////////////////////////////////////////////////

_AVX afxBool AfxGetPipelineLigature(avxPipeline pip, avxLigature* ligature)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxLigature liga = pip->liga;

    if (!liga)
    {
        // rebuild the pipeline ligature
        {
            afxUnit listedShaderCnt = 0;
            avxShader listedShaders[16];

            for (afxUnit j = 0; j < pip->stageCnt; j++)
                listedShaders[j] = pip->stages[j].shd;

            listedShaderCnt = pip->stageCnt;

            avxLigatureConfig ligc = { 0 };
            ligc.shaderCnt = listedShaderCnt;
            ligc.shaders = listedShaders;

            afxDrawSystem dsys = AfxGetProvider(pip);
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
            if (AfxDeclareLigatures(dsys, 1, &ligc, &liga)) AfxThrowError();
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
                pip->liga = liga;
            }
        }
    }
    AFX_TRY_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    *ligature = liga;
    return !!liga;
}

_AVX afxBool AfxGetPipelineVertexInput(avxPipeline pip, avxVertexDecl* input)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxVertexDecl vin = pip->vin;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    *input = vin;
    return !!vin;
}

_AVX void AfxDescribePipeline(avxPipeline pip, avxPipelineInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    info->stageCnt = pip->stageCnt;
    info->primTopology = pip->primTop;
    info->primRestartEnabled = pip->primRestartEnabled;
    info->depthClampEnabled = pip->depthClampEnabled;
    info->cullMode = pip->cullMode;
    info->frontFacingInverted = pip->frontFacingInverted;
    info->liga = pip->liga;
    info->vin = pip->vin;
    
    // rasterization

    //info->dsFlags = pip->dsFlags;
    //info->msFlags = pip->msFlags;
    //info->rasFlags = pip->rasFlags;
    //info->pixelFlags = pip->pixelFlags;

    info->fillMode = pip->fillMode;
    info->lineWidth = pip->lineWidth;

    info->depthBiasEnabled = pip->depthBiasEnabled;
    info->depthBiasSlopeScale = pip->depthBiasSlopeScale;
    info->depthBiasConstFactor = pip->depthBiasConstFactor;
    info->depthBiasClamp = pip->depthBiasClamp;

    info->msEnabled = pip->msEnabled;

    if ((info->sampleLvl = pip->sampleLvl))
        AfxCopy2(info->sampleMasks, pip->sampleMasks, sizeof(pip->sampleMasks[0]), pip->sampleLvl);

    info->alphaToOneEnabled = pip->alphaToOneEnabled;
    info->alphaToCoverageEnabled = pip->alphaToCoverageEnabled;
    info->sampleShadingEnabled = pip->sampleShadingEnabled;
    info->minSampleShadingValue = pip->minSampleShadingValue;

    if ((info->stencilTestEnabled = pip->stencilTestEnabled))
    {
        info->stencilFront = pip->stencilFront;
        info->stencilBack = pip->stencilBack;
    }

    info->depthTestEnabled = pip->depthTestEnabled;
    info->depthCompareOp = pip->depthCompareOp;
    info->depthWriteDisabled = pip->depthWriteDisabled;
    info->dsFmt = pip->dsFmt;

    if ((info->depthBoundsTestEnabled = pip->depthBoundsTestEnabled))
        AfxV2dCopy(info->depthBounds, pip->depthBounds);

    if ((info->colorOutCnt = pip->outCnt))
        AfxCopy2(info->colorOuts, pip->outs, sizeof(pip->outs[0]), pip->outCnt);

    AfxColorCopy(info->blendConstants, pip->blendConstants);
    info->pixelLogicOpEnabled = pip->logicOpEnabled;
    info->pixelLogicOp = pip->logicOp;
}

_AVX afxUnit AfxGetColorOutputChannels(avxPipeline pip, afxUnit first, afxUnit cnt, avxColorOutput channels[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
    AFX_ASSERT_RANGE(pip->outCnt, first, cnt);
    AFX_ASSERT(channels);
    afxUnit i = 0;

    for (; i < cnt; i++)
        channels[i] = pip->outs[first + i];

    return i;
}

_AVX afxUnit AfxGetMultisamplingMasks(avxPipeline pip, afxUnit first, afxUnit cnt, afxMask sampleMask[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
    AFX_ASSERT_RANGE(pip->sampleLvl, first, cnt);
    AFX_ASSERT(sampleMask);
    afxUnit i = 0;

    for (afxUnit i = 0; i < cnt; i++)
        sampleMask[i] = pip->sampleMasks[first + i];

    return i;
}

_AVX afxBool AfxGetPipelineShader(avxPipeline pip, avxShaderStage stage, avxShader* shader)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);
    afxBool found = FALSE;

    for (afxUnit slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
    {
        avxShaderSlot* slot = &pip->stages[slotIdx];

        if (slot->stage != stage)
            continue;

        found = TRUE;
        AFX_ASSERT(shader);
        *shader = slot->shd;
    }
    return found;
}

_AVX afxUnit AfxGetPipelineShaders(avxPipeline pip, afxIndex first, afxUnit cnt, avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(pip->stageCnt, first, cnt);
    AFX_ASSERT(cnt);
    AFX_ASSERT(shaders);
    afxUnit hitCnt = 0;
    cnt = AfxMin(pip->stageCnt, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        shaders[i] = pip->stages[i].shd;
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxError AfxRelinkPipelineFunction(avxPipeline pip, avxShaderStage stage, avxShader shd, afxString const* fn, afxUnit const specIds[], void const* specValues[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);
    (void)specIds;
    (void)specValues;

    afxBool found = FALSE;
    avxShaderSlot* slot;
    afxUnit slotIdx;

    for (slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
    {
        slot = &pip->stages[slotIdx];

        if (slot->stage == stage)
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
    {
        for (slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
        {
            slot = &pip->stages[slotIdx];
            
            if (slot->stage == NIL) // if is unused
            {
                found = TRUE;
                slot->stage = stage;
                break;
            }
        }
    }

    if (found && (slot->shd != shd))
    {
        slot = &pip->stages[slotIdx];

        if (shd)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
            AfxReacquireObjects(1, &shd);
        }

        if (slot->shd)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &slot->shd);
            AfxDisposeObjects(1, &slot->shd);
        }

        slot->shd = shd;
        AfxMakeString8(&slot->fn, fn);

        // rebuild the pipeline ligature
        {
            afxUnit listedShaderCnt = 0;
            avxShader listedShaders[16];

            for (afxUnit j = 0; j < pip->stageCnt; j++)
                listedShaders[j] = pip->stages[j].shd;

            listedShaderCnt = pip->stageCnt;

            avxLigatureConfig ligc = { 0 };
            ligc.shaderCnt = listedShaderCnt;
            ligc.shaders = listedShaders;

            avxLigature liga;
            afxDrawSystem dsys = AfxGetProvider(pip);
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
            if (AfxDeclareLigatures(dsys, 1, &ligc, &liga)) AfxThrowError();
            else
            {
                if (pip->liga)
                    AfxDisposeObjects(1, &pip->liga);

                pip->liga = liga;
            }
        }
    }
    return err;
}

_AVX afxError AfxUplinkPipelineFunction(avxPipeline pip, avxShaderStage stage, afxUri const* uri, afxString const* fn, afxUnit const specIds[], void const* specValues[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);

    if (AfxIsUriBlank(uri))
    {
        AfxThrowError();
        return err;
    }

    avxShader shd;
    afxDrawSystem dsys = AfxGetProvider(pip);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (AfxAcquireShaders(dsys, 1, uri, NIL, &shd))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    if (AfxRelinkPipelineFunction(pip, stage, shd, fn, specIds, specValues))
        AfxThrowError();

    AfxDisposeObjects(1, &shd);

    return err;
}

_AVX afxError AfxRecompilePipelineFunction(avxPipeline pip, avxShaderStage stage, afxString const* code, afxString const* fn, afxUnit const specIds[], void const* specValues[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);
    AFX_ASSERT(code);

    if (AfxIsStringEmpty(code))
    {
        AfxThrowError();
        return err;
    }

    avxShader shd;
        
    afxDrawSystem dsys = AfxGetProvider(pip);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (AfxAcquireShaders(dsys, 1, NIL, code, &shd))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    if (AfxRelinkPipelineFunction(pip, stage, shd, fn, specIds, specValues))
        AfxThrowError();

    AfxDisposeObjects(1, &shd);

    return err;
}

_AVX afxError _AvxPipDtorCb(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    for (afxUnit i = pip->stageCnt; i-- > 0;)
    {
        avxShaderSlot* slot = &pip->stages[i];
        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &slot->shd);
        AfxDisposeObjects(1, &slot->shd);
    }

    if (pip->liga)
    {
        AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &pip->liga);
        AfxDisposeObjects(1, &pip->liga);
    }

    if (pip->vin)
    {
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &pip->vin);
        AfxDisposeObjects(1, &pip->vin);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = pip->stageCnt,
            .siz = sizeof(pip->stages[0]),
            .var = (void*)&pip->stages
        },
        {
            .cnt = pip->sampleLvl,
            .siz = sizeof(pip->sampleMasks[0]),
            .var = (void*)&pip->sampleMasks
        },
        {
            .cnt = pip->outCnt,
            .siz = sizeof(pip->outs[0]),
            .var = (void*)&pip->outs
        }
    };

    if (AfxDeallocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxPipCtorCb(avxPipeline pip, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    //afxDrawSystem dsys = args[0];
    avxPipelineBlueprint const *pipb = ((avxPipelineBlueprint const*)args[1]) + invokeNo;
    avxPipelineBlueprint const *razb = ((avxPipelineBlueprint const*)args[2]) + invokeNo;
    
    // GRAPHICS STATE SETTING

    afxUnit sampleMaskCnt = AfxMin(razb->sampleLvl, AVX_MAX_SAMPLE_MASKS);
    afxUnit colorOutCnt = AfxMin(AfxMax(1, razb->colorOutCnt), AVX_MAX_COLOR_OUTPUTS);

    pip->liga = NIL;

    pip->transformationDisabled = !!pipb->transformationDisabled;
    pip->tesselationEnabled = !pip->transformationDisabled && !!pipb->tesselationEnabled;
    pip->rasterizationDisabled = !pip->transformationDisabled && !!pipb->rasterizationDisabled;

    pip->primTop = pipb->primTop;
    pip->patchControlPoints = pipb->patchCtrlPoints;
    pip->cullMode = pipb->cullMode;
    pip->vpCnt = AfxClamp(pipb->vpCnt, 1, 8);

    //avxRasterizationFlags rasFlags = razb->rasFlags;
    //avxMultisamplingFlags msFlags = razb->msFlags;
    //avxDepthStencilFlags dsFlags = razb->dsFlags;
    //avxColorOutputFlags pixelFlags = razb->pixelFlags;
    //pip->rasFlags = NIL;
    //pip->msFlags = NIL;
    //pip->dsFlags = NIL;
    //pip->pixelFlags = NIL;

    pip->fillMode = razb->fillMode;
    pip->lineWidth = razb->lineWidth;

    pip->depthBiasEnabled = !!razb->depthBiasEnabled;
    pip->depthBiasSlopeScale = razb->depthBiasSlopeScale;
    pip->depthBiasConstFactor = razb->depthBiasConstFactor;
    pip->depthBiasClamp = razb->depthBiasClamp;

    pip->msEnabled = !!razb->msEnabled;
    pip->minSampleShadingValue = razb->minSampleShadingValue;

    pip->depthTestEnabled = !!razb->depthTestEnabled;
    pip->depthCompareOp = razb->depthCompareOp;
    pip->depthWriteDisabled = !!razb->depthWriteDisabled;

    pip->stencilTestEnabled = !!razb->stencilTestEnabled;
    pip->stencilFront = razb->stencilFront;
    pip->stencilBack = razb->stencilBack;

    pip->depthBoundsTestEnabled = !!razb->depthBoundsTestEnabled;
    AfxV2dCopy(pip->depthBounds, razb->depthBounds);

    pip->dsFmt = razb->dsFmt; // ?

    // deveria ser só o blend/write, já que só podemos determinar as saídas quando assembleado com fragment shaders enquanto pipeline completo.

    AfxV4dCopy(pip->blendConstants, razb->blendConstants);

    pip->logicOpEnabled = !!razb->pixelLogicOpEnabled;
    pip->logicOp = razb->pixelLogicOp;

    afxUnit stageCnt = 0;// pipb->stageCnt;
    //AFX_ASSERT(stageCnt);

    if (!pip->transformationDisabled)
    {
        ++stageCnt;

        if (pip->tesselationEnabled)
            ++stageCnt, ++stageCnt; // twice

        if (pipb->primShaderSupported)
            ++stageCnt;

        if (!pip->rasterizationDisabled)
            ++stageCnt;
    }
    else stageCnt = 1; // compute shader

    afxObjectStash const stashes[] =
    {
        {
            .cnt = stageCnt,
            .siz = sizeof(pip->stages[0]),
            .var = (void*)&pip->stages
        },
        {
            .cnt = sampleMaskCnt,
            .siz = sizeof(pip->sampleMasks[0]),
            .var = (void*)&pip->sampleMasks
        },
        {
            .cnt = colorOutCnt,
            .siz = sizeof(pip->outs[0]),
            .var = (void*)&pip->outs
        }
    };

    if (AfxAllocateInstanceData(pip, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        pip->outCnt = colorOutCnt;
        for (afxUnit i = 0; i < colorOutCnt; i++)
            pip->outs[i] = i < razb->colorOutCnt ? razb->colorOuts[i] : (avxColorOutput) { 0 };

        pip->sampleLvl = sampleMaskCnt;
        for (afxUnit i = 0; i < sampleMaskCnt; i++)
            pip->sampleMasks[i] = i < razb->sampleLvl ? razb->sampleMasks[i] : (afxMask) { 0 };

        pip->stageCnt = stageCnt;
        for (afxUnit i = 0; i < pip->stageCnt; i++)
        {
            avxShaderSlot* slot = &pip->stages[i];
            AfxZero(slot, sizeof(*slot));
        }

        if (pip->transformationDisabled) pip->stages[0].stage = avxShaderStage_COMPUTE;
        else
        {
            afxUnit nextStageIdx = 0;
            pip->stages[nextStageIdx++].stage = avxShaderStage_VERTEX;

            if (pip->tesselationEnabled)
            {
                pip->stages[nextStageIdx++].stage = avxShaderStage_TESS_CTRL;
                pip->stages[nextStageIdx++].stage = avxShaderStage_TESS_EVAL;
            }

            if (pipb->primShaderSupported)
            {
                pip->stages[nextStageIdx++].stage = avxShaderStage_PRIMITIVE;
            }

            if (!pip->rasterizationDisabled)
            {
                pip->stages[nextStageIdx++].stage = avxShaderStage_FRAGMENT;
            }
        }

        if ((pip->vin = pipb->vin))
        {
            pip->vin = pipb->vin;
            AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &pip->vin);
            AfxReacquireObjects(1, &pip->vin);
        }

        if (err && AfxDeallocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    return err;
}

_AVX afxClassConfig const _AVX_PIP_CLASS_CONFIG =
{
    .fcc = afxFcc_PIP,
    .name = "Pipeline",
    .desc = "Draw Pipeline State",
    .fixedSiz = sizeof(AFX_OBJECT(avxPipeline)),
    .ctor = (void*)_AvxPipCtorCb,
    .dtor = (void*)_AvxPipDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAssembleComputePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const blueprints[], avxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->pipCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dsys, (void*)blueprints, (void*)blueprints }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxAssemblePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const cfg[], avxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(pipelines);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->pipCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dsys, (void*)cfg, (void*)cfg }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxLoadPipeline(afxDrawSystem dsys, avxVertexDecl vin, afxUri const* uri, avxPipeline* pipeline)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    AFX_ASSERT(uri && !AfxIsUriBlank(uri));

    AfxLogEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str));

    afxXml xml;
    afxString query;
    afxUri fpath, fext;
    AfxGetUriQueryString(uri, TRUE, &query);
    AfxClipUriExtension(&fext, uri, FALSE);
    AfxClipUriPath(&fpath, uri);

    if (AfxIsUriBlank(&fext)) AfxThrowError();
    else if (0 != AfxCompareString(AfxGetUriString(&fext), 0, ".xml", 4, TRUE))
    {
        AfxLogError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
        AfxThrowError();
    }
    else if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
    else
    {
        afxUnit xmlElemIdx;
        if (!AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Pipeline"), &AfxStaticString("id"), 1, &query, &xmlElemIdx)) AfxThrowError();
        {
            AFX_ASSERT(xmlElemIdx != AFX_INVALID_INDEX);

            avxPipelineBlueprint defConfig = { 0 };
            defConfig.depthCompareOp = avxCompareOp_LESS;
            defConfig.fillMode = avxFillMode_SOLID;
            avxPipelineBlueprint info = defConfig;
            afxUri shdUris[16];
            avxShaderStage shdStages[16];
            afxString shdFns[16];
            
            if (_AvxParseXmlPipelineBlueprint(&xml, xmlElemIdx, 0, &info, shdStages, shdUris, shdFns)) AfxThrowError();
            else
            {
                info.vin = vin;

                avxPipeline pip = NIL;

                if (AfxAssemblePipelines(dsys, 1, &info, &pip)) AfxThrowError();
                else
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
                    AFX_ASSERT(pipeline);
                    *pipeline = pip;

                    for (afxUnit i = 0; i < info.stageCnt; i++)
                    {
                        AfxUplinkPipelineFunction(pip, shdStages[i], &shdUris[i], &shdFns[i], NIL, NIL);
                    }
                }
            }
        }
        AfxCleanUpXml(&xml);
    }
    return err;
}
