/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_PIPELINE_C
#define _AVX_RASTERIZER_C
#include "avxIcd.h"

_AVX avxStencilConfig const AVX_STENCIL_INFO_DEFAULT =
{
    .compareOp = avxCompareOp_ALWAYS,
    .reference = 0,
    .compareMask = 1,
    .writeMask = 1,
    .failOp = avxStencilOp_KEEP,
    .depthFailOp = avxStencilOp_KEEP,
    .passOp = avxStencilOp_KEEP,
};

_AVX avxPipelineConfig const AVX_PIPELINE_BLUEPRINT_DEFAULT =
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
    .fillMode = avxFillMode_FACE,
    .lineWidth = 1.f,

    .depthBiasEnabled = FALSE,
    .depthBiasSlopeScale = 0.f,
    .depthBiasConstFactor = 0.f,
    .depthBiasClamp = 0.f,

    .ms.msEnabled = FALSE,
    .ms.sampleLvl = 0,
    .ms.sampleMasks =
    {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    },
    
    .ms.alphaToOneEnabled = FALSE,
    .ms.alphaToCoverageEnabled = FALSE,
    .ms.sampleShadingEnabled = FALSE,
    .ms.minSampleShadingValue = 0.f,

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
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT,
        AVX_DEFAULT_COLOR_OUTPUT
    },
    .blendConstants = { 0, 0, 0, 0 },
    .pixelLogicOpEnabled = FALSE,
    .pixelLogicOp = avxLogicOp_COPY
};

_AVX afxError _AvxParseXmlPipelineBlueprint(afxXmlNode const* xmle, afxUnit specId, avxPipelineConfig* pipb, avxShaderType shaderStages[], afxUri shaderUris[], afxString shaderFns[])
{
    afxError err = { 0 };
    AFX_ASSERT(pipb);

    avxPipelineConfig config = { 0 };
    config = *pipb;

    afxString const pipAttrNames[] =
    {
        AFX_STRING("topology"),
        AFX_STRING("cullMode"),
        AFX_STRING("frontFaceInverted"),
        AFX_STRING("depthClampEnabled"),
        // RASTERIZATION DATA AHEAD
        AFX_STRING("rasterizationDisabled"),
        AFX_STRING("fillMode"),
        AFX_STRING("depthBiasEnabled"),
        AFX_STRING("depthBiasConstant"),
        AFX_STRING("depthBiasClamp"),
        AFX_STRING("depthBiasSlope"),
        AFX_STRING("lineWidth"),
        AFX_STRING("depthTestEnabled"),
        AFX_STRING("depthWriteDisabled"),
        AFX_STRING("depthCompareOp"),
        AFX_STRING("depthBoundsTest"),
        AFX_STRING("depthBoundsTestMin"),
        AFX_STRING("depthBoundsTestMax"),
        AFX_STRING("stencilTestEnabled")
    };

    afxUnit sIdx;

    afxXmlAttr const* a;
    AFX_ITERATE_CHAIN_B2F(a, elem, &xmle->attributes)
    {
        if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(pipAttrNames), pipAttrNames, &sIdx))
            continue;
        
        switch (sIdx)
        {
        case 0: // topology
        {
            avxTopology primTop;

            if ((primTop = AfxFindPrimitiveTopology(&a->content)))
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

            if ((cullMode = AfxFindCullMode(&a->content)))
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
            avxFillMode fillMode = AfxFindFillMode(&a->content);
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

            if (AfxScanString(&a->content, "%f", &depthBiasConstFactor))
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

            if (AfxScanString(&a->content, "%f", &depthBiasClamp))
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

            if (AfxScanString(&a->content, "%f", &depthBiasSlopeScale))
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

            if (AfxScanString(&a->content, "%f", &lineWidth))
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
            if ((config.depthCompareOp = AfxFindCompareOp(&a->content)))
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

            if (AfxScanString(&a->content, "%f", &mini))
            {
                config.depthBounds[0] = mini;
                //config.dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
            }
            break;
        }
        case 16: // depthBoundsTestMax
        {
            afxReal maxi;

            if (AfxScanString(&a->content, "%f", &maxi))
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
            AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
            break;
        }
        }
    }

    afxString const stageNames[] =
    {
        AFX_STRING_EMPTY,
        AFX_STRING("VERTEX"),
        AFX_STRING("FRAGMENT"),
        AFX_STRING("PRIMITIVE"),
        AFX_STRING("DOMAIN"),
        AFX_STRING("HULL"),
        AFX_STRING("COMPUTE"),
    };

    afxXmlNode const* child;
    AFX_ITERATE_CHAIN_B2F(child, parent, &xmle->children)
    {
        if (AfxCompareStrings(&child->name, 0, TRUE, 1, &AFX_STRING("Shader"), NIL))
        {
            avxShaderType shdStage = NIL;
            afxString shdFn = { 0 };
            afxUri shdUri = { 0 };

            afxString const shdAttrNames[] =
            {
                AFX_STRING("stage"),
                AFX_STRING("uri"),
                AFX_STRING("fn")
            };

            AFX_ITERATE_CHAIN_B2F_B(a, elem, &child->attributes)
            {
                if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(shdAttrNames), shdAttrNames, &sIdx))
                    continue;
                
                switch (sIdx)
                {
                case 0: // stage
                {
                    if (!AfxCompareStrings(&a->content, 0, TRUE, ARRAY_SIZE(stageNames), stageNames, &sIdx))
                        continue;

                    switch (sIdx)
                    {
                    case avxShaderType_VERTEX: shdStage = avxShaderType_VERTEX; break;
                    case avxShaderType_FRAGMENT: shdStage = avxShaderType_FRAGMENT; break;
                    case avxShaderType_PRIMITIVE: shdStage = avxShaderType_PRIMITIVE; break;
                    case avxShaderType_TESS_CTRL: shdStage = avxShaderType_TESS_CTRL; break;
                    case avxShaderType_TESS_EVAL: shdStage = avxShaderType_TESS_EVAL; break;
                    case avxShaderType_COMPUTE: shdStage = avxShaderType_COMPUTE; break;
                    default:
                    {
                        AfxReportY("Shader stage '%.*s' not handled.", AfxPushString(&a->content));
                        break;
                    }
                    }
                    break;
                }
                case 1: // uri
                {
                    if (!AfxIsStringEmpty(&a->content))
                    {
                        afxUri tempUri;
                        AfxWrapUriString(&tempUri, &a->content);
                        AfxReportMessage("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                        AfxReplicateUri(&shdUri, &tempUri);
                    }
                    break;
                }
                case 2: // fn
                {
                    if (!AfxIsStringEmpty(&a->content))
                    {
                        shdFn = a->content;
                    }
                    break;
                }
                default:
                {
                    AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
                    break;
                }
                }
            }

            if (!AfxIsStringEmpty(&child->content))
            {
                //AvxAcquireCodebases(dsys, 1, );
            }

            if (shaderStages)
                shaderStages[config.stageCnt] = shdStage;

            if (shaderUris)
                shaderUris[config.stageCnt] = shdUri;

            if (shaderFns)
                shaderFns[config.stageCnt] = shdFn;

            ++config.stageCnt;
        }
        else if (AfxCompareStrings(&child->name, 0, TRUE, 1, &AFX_STRING("Stencil"), NIL))
        {
            avxStencilConfig si = { 0 };

            afxString const shdAttrNames[] =
            {
                AFX_STRING("compareOp"),
                AFX_STRING("reference"),
                AFX_STRING("compareMask"),
                AFX_STRING("writeMask"),
                AFX_STRING("failOp"),
                AFX_STRING("depthFailOp"),
                AFX_STRING("passOp"),
                AFX_STRING("front"),
                AFX_STRING("back"),
            };

            afxBool front = FALSE;
            afxBool back = FALSE;

            AFX_ITERATE_CHAIN_B2F_B(a, elem, &child->attributes)
            {
                if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(shdAttrNames), shdAttrNames, &sIdx))
                    continue;

                switch (sIdx)
                {
                case 0: // compareOp
                {
                    si.compareOp = AfxFindCompareOp(&a->content);
                    break;
                }
                case 1: // reference
                {
                    afxUnit32 reference = 0;

                    if (AfxScanString(&a->content, "%d", &reference))
                    {
                        si.reference = reference;
                    }
                    break;
                }
                case 2: // compareMask
                {
                    afxUnit32 compareMask = 1;

                    if (AfxScanString(&a->content, "%d", &compareMask))
                    {
                        si.compareMask = compareMask;
                    }
                    break;
                }
                case 3: // writeMask
                {
                    afxUnit32 writeMask = 1;

                    if (AfxScanString(&a->content, "%d", &writeMask))
                    {
                        si.writeMask = writeMask;
                    }
                    break;
                }
                case 4: // failOp
                {
                    si.failOp = AfxFindStencilOp(&a->content);
                    break;
                }
                case 5: // depthFailOp
                {
                    si.depthFailOp = AfxFindStencilOp(&a->content);
                    break;
                }
                case 6: // passOp
                {
                    si.passOp = AfxFindStencilOp(&a->content);
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
                    AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
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
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
        }
    }

    *pipb = config;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawSystem AvxGetPipelineHost(avxPipeline pip)
{
    afxError err = { 0 };
    // @pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    afxDrawSystem dsys = AfxGetHost(pip);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX avxBus AvxGetPipelineBus(avxPipeline pip)
{
    afxError err = { 0 };
    // @pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    return pip->bus;
}

_AVX afxFlags AvxGetPipelineFlags(avxPipeline pip, afxFlags mask)
{
    afxError err = { 0 };
    // @pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    return (!mask) ? pip->flags : (pip->flags & mask);
}

_AVX afxBool AvxGetPipelineLigature(avxPipeline pip, avxLigature* ligature)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxLigature liga = pip->liga;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    *ligature = liga;
    return !!liga;
}

_AVX afxBool AvxGetPipelineVertexInput(avxPipeline pip, avxVertexInput* input)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxVertexInput vin = pip->vin;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    *input = vin;
    return !!vin;
}

_AVX void AvxDescribePipeline(avxPipeline pip, avxPipelineInfo* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    info->tag = pip->tag;
    info->udd = pip->udd;
    info->bus = pip->bus;
    info->stageCnt = pip->stageCnt;
    info->liga = pip->liga;
    info->vin = pip->vin;
}

_AVX void AvxDescribePipelineTransformation(avxPipeline pip, avxTransformation* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    info->transformationDisabled = pip->transformationDisabled;
    // Vertex shading
    info->vin = pip->vin;
    // Primitive assembling
    info->primTop = pip->primTop;
    info->primRestartEnabled = pip->primRestartEnabled;
    // Tesselation
    info->tesselationEnabled = pip->tesselationEnabled;
    info->patchCtrlPoints = pip->patchControlPoints;
    // Primitive shading
    info->primShaderSupported = FALSE;

    // Clipping
    info->depthClampEnabled = pip->depthClampEnabled;
    // Culling
    info->cullMode = pip->cullMode;
    info->frontFacingInverted = pip->frontFacingInverted;
    // Viewport transform
    info->vpCnt = pip->vpCnt;
}

_AVX void AvxDescribePipelineRasterization(avxPipeline pip, avxRasterization* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    info->fillMode = pip->fillMode;
    info->lineWidth = pip->lineWidth;

    info->depthBiasEnabled = pip->depthBiasEnabled;
    info->depthBiasSlopeScale = pip->depthBiasSlopeScale;
    info->depthBiasConstFactor = pip->depthBiasConstFactor;
    info->depthBiasClamp = pip->depthBiasClamp;

    {
        // Multisampling

        info->ms.msEnabled = pip->msEnabled;

        if ((info->ms.sampleLvl = pip->sampleLvl))
            AfxCopy(info->ms.sampleMasks, pip->sampleMasks, sizeof(pip->sampleMasks[0]) * pip->sampleLvl);

        info->ms.alphaToOneEnabled = pip->alphaToOneEnabled;
        info->ms.alphaToCoverageEnabled = pip->alphaToCoverageEnabled;
        info->ms.sampleShadingEnabled = pip->sampleShadingEnabled;
        info->ms.minSampleShadingValue = pip->minSampleShadingValue;
    }

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
        AfxCopy(info->colorOuts, pip->outs, sizeof(pip->outs[0]) * pip->outCnt);

    AvxCopyColor(info->blendConstants, pip->blendConstants);
    info->pixelLogicOpEnabled = pip->logicOpEnabled;
    info->pixelLogicOp = pip->logicOp;
}

_AVX afxUnit AvxGetColorOutputs(avxPipeline pip, afxUnit first, afxUnit cnt, avxColorOutput channels[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
    AFX_ASSERT_RANGE(pip->outCnt, first, cnt);
    AFX_ASSERT(channels);
    afxUnit i = 0;

    for (; i < cnt; i++)
        channels[i] = pip->outs[first + i];

    return i;
}

_AVX afxUnit AvxGetMultisamplingMasks(avxPipeline pip, afxUnit first, afxUnit cnt, afxMask sampleMask[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
    AFX_ASSERT_RANGE(pip->sampleLvl, first, cnt);
    AFX_ASSERT(sampleMask);
    afxUnit i = 0;

    for (afxUnit i = 0; i < cnt; i++)
        sampleMask[i] = pip->sampleMasks[first + i];

    return i;
}

_AVX afxBool AvxGetPipelineCodebase(avxPipeline pip, avxCodebase* codebase)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxCodebase codb = pip->codb;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
    AFX_ASSERT(codebase);
    *codebase = codb;
    return !!codb;
}

_AVX afxBool AvxGetPipelineShader(avxPipeline pip, avxShaderType stage, afxUnit* prog, afxString* func)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderType_TOTAL, stage, 1);
    afxBool found = FALSE;

    for (afxUnit slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
    {
        _avxProgrammableStage* pstage = &pip->stages[slotIdx];

        if (pstage->stage != stage)
            continue;

        AFX_ASSERT(prog || func);
        if (prog) *prog = pstage->progId;
        if (func) *func = pstage->fn.s;

        found = TRUE;
    }
    return found;
}

_AVX afxUnit AvxGetPipelinePrograms(avxPipeline pip, afxIndex first, afxUnit cnt, afxUnit progs[], afxString funcs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(pip->stageCnt, first, cnt);
    AFX_ASSERT(progs || funcs);
    afxUnit hitCnt = 0;
    cnt = AFX_MIN(pip->stageCnt, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (progs) progs[i] = pip->stages[i].progId;
        if (funcs) funcs[i] = pip->stages[i].fn.s;

        hitCnt++;
    }
    return hitCnt;
}

_AVX afxError AvxReprogramPipeline(avxPipeline pip, afxUnit cnt, avxShaderSpecialization const specs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT(specs);

    avxCodebase codb = pip->codb;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);

    for (afxUnit opIt = 0; opIt < cnt; opIt++)
    {
        avxShaderSpecialization const* spec = &specs[opIt];
        AFX_ASSERT_RANGE(avxShaderType_TOTAL, spec->stage, 1);

        afxBool found = FALSE;
        _avxProgrammableStage* pstage;
        afxUnit slotIdx;

        for (slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
        {
            pstage = &pip->stages[slotIdx];

            if (pstage->stage == spec->stage)
            {
                found = TRUE;
                break;
            }
        }

        if (!found)
        {
            for (slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
            {
                pstage = &pip->stages[slotIdx];

                if (pstage->stage == NIL) // if is unused
                {
                    found = TRUE;
                    pstage->stage = spec->stage;
                    break;
                }
            }
        }

        if (!found)
        {
            AfxThrowError();
            continue;
        }

        afxUnit progId;
        if (!AvxFindPrograms(codb, 1, &spec->prog, &progId))
        {
            AfxThrowError();
            continue;
        }

        // Specialization can change code
        //if ((slot->progId == progId)) continue;

        pstage = &pip->stages[slotIdx];
        pstage->progId = progId;
        AfxMakeString8(&pstage->fn, &spec->fn);

        // rebuild the pipeline ligature
        if (!pip->isUserLiga)
        {
            afxUnit listedProgCnt = 0;
            afxUnit listedProgs[16];

            for (afxUnit j = 0; j < pip->stageCnt; j++)
                listedProgs[j] = pip->stages[j].progId;

            listedProgCnt = pip->stageCnt;

            afxDrawSystem dsys = AvxGetPipelineHost(pip);
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

            avxLigatureConfig ligc = { 0 };
            AvxConfigureLigature(dsys, &ligc, codb, listedProgCnt, listedProgs);

            avxLigature liga;
            if (AvxAcquireLigatures(dsys, 1, &ligc, &liga)) AfxThrowError();
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

_AVX afxError _AvxPipDtorCb(avxPipeline pip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    for (afxUnit i = pip->stageCnt; i-- > 0;)
    {
        _avxProgrammableStage* slot = &pip->stages[i];
        //AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &slot->shd);
        //AfxDisposeObjects(1, &slot->shd);
    }

    if (pip->liga)
    {
        AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &pip->liga);
        AfxDisposeObjects(1, &pip->liga);
    }

    if (pip->codb)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &pip->codb);
        AfxDisposeObjects(1, &pip->codb);
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxPipelineConfig const *pipb = ((avxPipelineConfig const*)args[1]) + invokeNo;
    avxPipelineConfig const *razb = ((avxPipelineConfig const*)args[2]) + invokeNo;
    
    avxLimits const* limits = _AvxDsysGetLimits(dsys);
    avxFeatures const* enabledFeatures = _AvxDsysGetReqFeatures(dsys);

    // GRAPHICS STATE SETTING

    afxUnit sampleMaskCnt = AFX_MIN(razb->ms.sampleLvl, AVX_MAX_SAMPLE_MASKS);
    afxUnit colorOutCnt = AFX_MIN(AFX_MAX(1, razb->colorOutCnt), AVX_MAX_COLOR_OUTPUTS);

    pip->udd = pipb->udd;
    pip->tag = pipb->tag;

    pip->liga = NIL;
    pip->vin = NIL;
    pip->codb = NIL;

    pip->specializedWorkGrpSiz[0] = pipb->specializedWorkGrpSiz[0];
    pip->specializedWorkGrpSiz[1] = pipb->specializedWorkGrpSiz[1];
    pip->specializedWorkGrpSiz[2] = pipb->specializedWorkGrpSiz[2];

    pip->transformationDisabled = !!pipb->transformationDisabled;
    pip->tesselationEnabled = !pip->transformationDisabled && !!pipb->tesselationEnabled;
    pip->rasterizationDisabled = !pip->transformationDisabled && !!pipb->rasterizationDisabled;

    pip->primTop = pipb->primTop;
    pip->patchControlPoints = pipb->patchCtrlPoints;
    pip->cullMode = pipb->cullMode;
    pip->vpCnt = AFX_CLAMP(pipb->vpCnt, 1, limits->maxVpCnt);

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

    pip->msEnabled = !!razb->ms.msEnabled;
    pip->minSampleShadingValue = razb->ms.minSampleShadingValue;

    pip->depthTestEnabled = !!razb->depthTestEnabled;
    pip->depthCompareOp = razb->depthCompareOp;
    pip->depthWriteDisabled = (!pip->depthTestEnabled) ? FALSE : (!!razb->depthWriteDisabled);

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

    if (AfxAllocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    pip->outCnt = colorOutCnt;
    for (afxUnit i = 0; i < colorOutCnt; i++)
        pip->outs[i] = i < razb->colorOutCnt ? razb->colorOuts[i] : (avxColorOutput) { 0 };

    pip->sampleLvl = sampleMaskCnt;
    for (afxUnit i = 0; i < sampleMaskCnt; i++)
        pip->sampleMasks[i] = i < razb->ms.sampleLvl ? razb->ms.sampleMasks[i] : (afxMask) { 0 };

    pip->stageCnt = stageCnt;
    for (afxUnit i = 0; i < pip->stageCnt; i++)
    {
        _avxProgrammableStage* slot = &pip->stages[i];
        AfxZero(slot, sizeof(*slot));
    }

    if (pip->transformationDisabled) pip->stages[0].stage = avxShaderType_COMPUTE;
    else
    {
        afxUnit nextStageIdx = 0;
        pip->stages[nextStageIdx++].stage = avxShaderType_VERTEX;

        if (pip->tesselationEnabled)
        {
            pip->stages[nextStageIdx++].stage = avxShaderType_TESS_CTRL;
            pip->stages[nextStageIdx++].stage = avxShaderType_TESS_EVAL;
        }

        if (pipb->primShaderSupported)
        {
            pip->stages[nextStageIdx++].stage = avxShaderType_PRIMITIVE;
        }

        if (!pip->rasterizationDisabled)
        {
            pip->stages[nextStageIdx++].stage = avxShaderType_FRAGMENT;
        }
    }

    if (!err)
    {
        avxCodebase codb = pipb->codb;
        pip->codb = codb;

        if (codb)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &pip->codb);
            AfxReacquireObjects(1, &pip->codb);
        }
        else
        {
            if (AvxAcquireCodebases(dsys, 1, NIL, &pip->codb))
            {
                AfxThrowError();
            }
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &pip->codb);
            }
        }

        if (!err)
        {
            avxLigature liga = pipb->liga;
            pip->liga = liga;
            pip->isUserLiga = !!(liga);

            if (liga)
            {
                AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &pip->liga);
                AfxReacquireObjects(1, &pip->liga);
            }

            avxVertexInput vin = pipb->vin;
            pip->vin = vin;

            if (vin)
            {
                AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &pip->vin);
                AfxReacquireObjects(1, &pip->vin);
            }
        }
    }

    if (err && AfxDeallocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_PIP =
{
    .fcc = afxFcc_PIP,
    .name = "Pipeline",
    .desc = "AVX Pipeline State Object",
    .fixedSiz = sizeof(AFX_OBJECT(avxPipeline)),
    .ctor = (void*)_AvxPipCtorCb,
    .dtor = (void*)_AvxPipDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAssemblePcxPipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineConfig const blueprints[], avxPipeline pipelines[])
{
    afxError err = { 0 };

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->pipCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dsys, (void*)blueprints, (void*)blueprints }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_PIP, cnt, pipelines);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxPipeline pip = pipelines[i];
    }
#endif

    return err;
}

_AVX afxError AvxAssembleGfxPipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineConfig const cfg[], avxPipeline pipelines[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(pipelines);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->pipCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dsys, (void*)cfg, (void*)cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_PIP, cnt, pipelines);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxPipelineConfig const* c = &cfg[i];
        avxPipeline pip = pipelines[i];

        AFX_ASSERT(pip->tag.start == c->tag.start);

        AFX_ASSERT(pip->stageCnt >= c->stageCnt);

        AFX_ASSERT(c->transformationDisabled == pip->transformationDisabled);
        AFX_ASSERT(c->vin == pip->vin);
        AFX_ASSERT(pip->primTop == c->primTop);
        AFX_ASSERT(pip->primRestartEnabled == c->primRestartEnabled);
        AFX_ASSERT(pip->patchControlPoints == c->patchCtrlPoints);
        AFX_ASSERT(pip->cullMode == c->cullMode);
        AFX_ASSERT(pip->frontFacingInverted == c->frontFacingInverted);
        AFX_ASSERT(pip->depthClampEnabled == c->depthClampEnabled);
        AFX_ASSERT(c->tesselationEnabled == pip->tesselationEnabled);
        AFX_ASSERT((c->vpCnt == pip->vpCnt) || ((c->vpCnt == 0) && (pip->vpCnt == 1)));

        {
            AFX_ASSERT(pip->rasterizationDisabled == c->rasterizationDisabled);
            AFX_ASSERT(pip->fillMode == c->fillMode);
            AFX_ASSERT(pip->lineWidth == c->lineWidth);

            AFX_ASSERT(pip->depthBiasClamp == c->depthBiasClamp);
            AFX_ASSERT(pip->depthBiasConstFactor == c->depthBiasConstFactor);
            AFX_ASSERT(pip->depthBiasEnabled == c->depthBiasEnabled);
            AFX_ASSERT(pip->depthBiasSlopeScale == c->depthBiasSlopeScale);

            {
                // Test multisampling

                AFX_ASSERT(pip->msEnabled == c->ms.msEnabled);
                AFX_ASSERT(pip->sampleLvl == c->ms.sampleLvl);
                for (afxUnit j = 0; j < c->ms.sampleLvl; j++)
                {
                    AFX_ASSERT(pip->sampleMasks[j] == c->ms.sampleMasks[j]);
                }
                AFX_ASSERT(pip->sampleShadingEnabled == c->ms.sampleShadingEnabled);
                AFX_ASSERT(pip->minSampleShadingValue == c->ms.minSampleShadingValue);
                AFX_ASSERT(c->ms.alphaToCoverageEnabled == pip->alphaToCoverageEnabled);
                AFX_ASSERT(c->ms.alphaToOneEnabled == pip->alphaToOneEnabled);
            }

            {
                // fragment tests

                AFX_ASSERT(c->stencilBack.compareMask == pip->stencilBack.compareMask);
                AFX_ASSERT(c->stencilBack.compareOp == pip->stencilBack.compareOp);
                AFX_ASSERT(c->stencilBack.depthFailOp == pip->stencilBack.depthFailOp);
                AFX_ASSERT(c->stencilBack.failOp == pip->stencilBack.failOp);
                AFX_ASSERT(c->stencilBack.passOp == pip->stencilBack.passOp);
                AFX_ASSERT(c->stencilBack.reference == pip->stencilBack.reference);
                AFX_ASSERT(c->stencilBack.writeMask == pip->stencilBack.writeMask);
                AFX_ASSERT(c->stencilFront.compareMask == pip->stencilFront.compareMask);
                AFX_ASSERT(c->stencilFront.compareOp == pip->stencilFront.compareOp);
                AFX_ASSERT(c->stencilFront.depthFailOp == pip->stencilFront.depthFailOp);
                AFX_ASSERT(c->stencilFront.failOp == pip->stencilFront.failOp);
                AFX_ASSERT(c->stencilFront.passOp == pip->stencilFront.passOp);
                AFX_ASSERT(c->stencilFront.reference == pip->stencilFront.reference);
                AFX_ASSERT(c->stencilFront.writeMask == pip->stencilFront.writeMask);
                AFX_ASSERT(c->stencilTestEnabled == pip->stencilTestEnabled);

                AFX_ASSERT(pip->depthBounds[0] == c->depthBounds[0]);
                AFX_ASSERT(pip->depthBounds[1] == c->depthBounds[1]);
                AFX_ASSERT(pip->depthBoundsTestEnabled == c->depthBoundsTestEnabled);
                AFX_ASSERT(pip->depthCompareOp == c->depthCompareOp);
                AFX_ASSERT(pip->depthTestEnabled == c->depthTestEnabled);
                AFX_ASSERT(pip->depthWriteDisabled == c->depthWriteDisabled);
                AFX_ASSERT(pip->dsFmt == c->dsFmt);
            }

            {
                AFX_ASSERT(pip->blendConstants[0] == c->blendConstants[0]);
                AFX_ASSERT(pip->blendConstants[1] == c->blendConstants[1]);
                AFX_ASSERT(pip->blendConstants[2] == c->blendConstants[2]);
                AFX_ASSERT(pip->blendConstants[3] == c->blendConstants[3]);
                AFX_ASSERT((pip->outCnt == c->colorOutCnt) || ((c->colorOutCnt == 0) && (pip->outCnt == 1)));

                for (afxUnit j = 0; j < c->colorOutCnt; j++)
                {
                    AFX_ASSERT(pip->outs[j].blendEnabled == c->colorOuts[j].blendEnabled);
                    AFX_ASSERT(pip->outs[j].blendConfig.aBlendOp == c->colorOuts[j].blendConfig.aBlendOp);
                    AFX_ASSERT(pip->outs[j].blendConfig.aDstFactor == c->colorOuts[j].blendConfig.aDstFactor);
                    AFX_ASSERT(pip->outs[j].blendConfig.aSrcFactor == c->colorOuts[j].blendConfig.aSrcFactor);
                    AFX_ASSERT(pip->outs[j].blendConfig.rgbBlendOp == c->colorOuts[j].blendConfig.rgbBlendOp);
                    AFX_ASSERT(pip->outs[j].blendConfig.rgbDstFactor == c->colorOuts[j].blendConfig.rgbDstFactor);
                    AFX_ASSERT(pip->outs[j].blendConfig.rgbSrcFactor == c->colorOuts[j].blendConfig.rgbSrcFactor);
                    AFX_ASSERT(pip->outs[j].discardMask == c->colorOuts[j].discardMask);
                }

                AFX_ASSERT(pip->logicOp == c->pixelLogicOp);
                AFX_ASSERT(pip->logicOpEnabled == c->pixelLogicOpEnabled);
            }
        }
        
        //AFX_ASSERT(pip->primShaderSupported == c->primShaderSupported);

    }
#endif

    return err;
}

_AVX afxError AvxLoadPipeline(afxDrawSystem dsys, avxVertexInput vin, afxUri const* uri, avxPipeline* pipeline)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    AFX_ASSERT(uri && !AfxIsUriBlank(uri));

    AfxReportMessage("Uploading pipeline '%.*s'", AfxPushString(&uri->s));

    afxXml xml;
    afxString query;
    afxUri fpath, fext;
    AfxGetUriQueryString(uri, TRUE, &query);
    AfxClipUriExtension(&fext, uri, FALSE);
    AfxClipUriPath(&fpath, uri);

    if (AfxIsUriBlank(&fext)) AfxThrowError();
    else if (0 != AfxCompareString(AfxGetUriString(&fext), 0, ".xml", 4, TRUE))
    {
        AfxReportError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
        AfxThrowError();
    }
    else if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
    else
    {
        afxXmlNode const* pipNode;

        if (!(pipNode = AfxFindXmlAttributedChildElement(xml.root, NIL, &AFX_STRING("Pipeline"), &AFX_STRING("id"), &query))) AfxThrowError();
        else
        {
            avxPipelineConfig defConfig = { 0 };
            defConfig.depthCompareOp = avxCompareOp_LESS;
            defConfig.fillMode = avxFillMode_FACE;
            avxPipelineConfig info = defConfig;
            afxUri shdUris[16];
            avxShaderType shdStages[16];
            afxString shdFns[16];
            
            if (_AvxParseXmlPipelineBlueprint(pipNode, 0, &info, shdStages, shdUris, shdFns)) AfxThrowError();
            else
            {
                info.vin = vin;

                avxPipeline pip = NIL;
                if (AvxAssembleGfxPipelines(dsys, 1, &info, &pip)) AfxThrowError();
                else
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
                    AFX_ASSERT(pipeline);
                    *pipeline = pip;

                    avxCodebase codb;
                    AvxGetPipelineCodebase(pip, &codb);
                    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
                    avxShaderSpecialization specs[8] = { 0 };

                    for (afxUnit i = 0; i < info.stageCnt; i++)
                    {
                        afxUri file;
                        AfxExcerptPathSegments(&shdUris[i], NIL, NIL, &file, NIL);
                        
                        if (AvxCompileShaderFromDisk(codb, &file.s, &shdUris[i]))
                            AfxThrowError();

                        specs[i].stage = shdStages[i];
                        specs[i].prog = file.s;
                        specs[i].fn = shdFns[i];
                    }

                    if (AvxReprogramPipeline(pip, info.stageCnt, specs))
                        AfxThrowError();
                }
            }
        }
        AfxCleanUpXml(&xml);
    }
    return err;
}
