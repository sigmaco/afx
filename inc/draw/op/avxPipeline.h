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

/**
    Desde que surgiu as mesh shaders, somado as experiências com rasterização com CUDA, aparentemente o processo de transformação geométrica das pipelines tende a mudar.
    Assim sendo, a SIGMA decideu quebrar o objeto de estado da pipeline em módulos, e assembleá-los, para evitar lidar com alocação de espaço para coisas pouco utilizadas.
    Esta abordagem é, de longe, a mais arbitrária, nadando contra o recúo de maré do Vulkan, Direct3D 12 e Metal.
*/

#ifndef AVX_PIPELINE_H
#define AVX_PIPELINE_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/draw/io/avxFormat.h"
#include "qwadro/inc/draw/math/avxColor.h"
#include "qwadro/inc/base/afxFixedString.h"

/*
    The first stage of the graphics pipeline (Input Assembler) assembles vertices to form geometric primitives such as points, lines, and triangles, based on a requested primitive topology.
    In the next stage (Vertex Shader) vertices can be transformed, computing positions and attributes for each vertex.
    If tessellation and/or geometry shaders are supported, they can then generate multiple primitives from a single input primitive, possibly changing the primitive topology or generating additional attribute data in the process.
    The final resulting primitives are clipped to a clip volume in preparation for the next stage, Rasterization.
    The rasterizer produces a series of fragments associated with a region of the framebuffer, from a two-dimensional description of a point, line segment, or triangle.
    These fragments are processed by fragment operations to determine whether generated values will be written to the framebuffer.
    Fragment shading determines the values to be written to the framebuffer attachments.
    Framebuffer operations then read and write the color and depth/stencil attachments of the framebuffer for a given subpass of a draw scope instance.
    The attachments can be used as input attachments in the fragment shader in a later subpass of the same draw scope.
*/

// Front facing and cull mode should not have effect no point and lines.

#define AVX_MAX_SAMPLE_MASKS (32)
#define AVX_MAX_COLOR_OUTPUTS (8)

typedef enum avxPipelineType
{
    avxPipelineType_COMPUTER,
    avxPipelineType_TRANSFORMER,
    avxPipelineType_RASTERIZER
} avxPipelineType;

typedef enum avxPipelineStage
{
    // NIL,
    avxPipelineStage_TOP            = AFX_BIT(0),
    avxPipelineStage_DRAW_INDIRECT  = AFX_BIT(1), // where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    avxPipelineStage_VERTEX_INPUT   = AFX_BIT(2), // where vertex and index buffers are consumed.
    avxPipelineStage_VERTEX         = AFX_BIT(3), // the vertex shader stage.
    avxPipelineStage_DOMAIN         = AFX_BIT(4), // the tessellation control shader stage.
    avxPipelineStage_HULL           = AFX_BIT(5), // the tessellation evaluation shader stage.
    avxPipelineStage_PRIM           = AFX_BIT(6), // the primitive (aka geometry) shader stage.
    avxPipelineStage_FRAGMENT       = AFX_BIT(7), // the fragment shader stage.
    avxPipelineStage_EARLY_TESTS    = AFX_BIT(8), // where early fragment tests (depth and stencil tests before fragment shading) are performed. This stage also includes draw scope load operations for framebuffer attachments with a depth/stencil format.
    avxPipelineStage_LATE_TESTS     = AFX_BIT(9), // where late fragment tests (depth and stencil tests after fragment shading) are performed. This stage also includes draw scope store operations for framebuffer attachments with a depth/stencil format.
    avxPipelineStage_COLOR_OUTPUT   = AFX_BIT(10), // where the final color values are output from the pipeline. This stage includes blending, logic operations, draw scope load and store operations for color attachments, draw scope multisample resolve operations, and AvxCmdClearAttachments.
    avxPipelineStage_COMPUTE        = AFX_BIT(11), // the execution of a compute shader.
    avxPipelineStage_TRANSFER       = AFX_BIT(12), // the following commands: copies, blits, resolves, clears.
    avxPipelineStage_BOTTOM         = AFX_BIT(13), 
    avxPipelineStage_HOST           = AFX_BIT(14), // pseudo-stage indicating execution on the host of reads/writes of device memory. This stage is not invoked by any commands recorded in a draw context.
    avxPipelineStage_GRAPHICS       = AFX_BIT(15), // the execution of all graphics pipeline stages.
    avxPipelineStage_COMMANDS       = AFX_BIT(16), // all operations performed by all commands supported on the queue it is used with.
    avxPipelineStage_COPY           = AFX_BIT(17),
    avxPipelineStage_RESOLVE        = AFX_BIT(18),
    avxPipelineStage_BLIT           = AFX_BIT(19),
    avxPipelineStage_CLEAR          = AFX_BIT(20),
    avxPipelineStage_VERTEX_INDEX   = AFX_BIT(21),
    avxPipelineStage_VERTEX_ATTR    = AFX_BIT(22),
    avxPipelineStage_PRERASTER      = AFX_BIT(23),
    avxPipelineStage_VIDDEC         = AFX_BIT(24),
    avxPipelineStage_VIDENC         = AFX_BIT(25),
} avxPipelineStage;

typedef enum avxPipelineDep
{
    avxPipelineDep_VIEWPORT, // always required in Qwadro
    avxPipelineDep_SCISSOR, // always required in Qwadro
    avxPipelineDep_LINE_WIDTH,
    avxPipelineDep_DEPTH_BIAS,
    avxPipelineDep_BLEND_CONSTANTS,
    avxPipelineDep_DEPTH_BOUNDS,
    avxPipelineDep_STENCIL_COMPARE_MASK,
    avxPipelineDep_STENCIL_WRITE_MASK,
    avxPipelineDep_STENCIL_REF,
    avxPipelineDep_CULL_MODE,
    avxPipelineDep_FRONT_FACE,
    avxPipelineDep_PRIM_TOPOLOGY,
    avxPipelineDep_VIEWPORT_RESET,
    avxPipelineDep_SCISSOR_RESET,
    avxPipelineDep_VTX_INPUT_BINDING_STRIDE,
    avxPipelineDep_DEPTH_TEST_ENABLE,
    avxPipelineDep_DEPTH_WRITE_ENABLE,
    avxPipelineDep_DEPTH_COMPARE_OP,
    avxPipelineDep_DEPTH_BOUNDS_TEST_ENABLE,
    avxPipelineDep_STENCIL_TEST_ENABLE,
    avxPipelineDep_STENCIL_OP,
    avxPipelineDep_RASTERIZER_DISCARD_ENABLE,
    avxPipelineDep_DEPTH_BIAS_ENABLE,
    avxPipelineDep_PRIM_RESTART_ENABLE,

    avxPipelineDep_TOTAL
} avxPipelineDeps;

#if 0
typedef enum avxPipelineFlag
{
    avxPipelineFlag_TOPOLOGY = AFX_BIT(0),
    avxPipelineFlag_RESTART = AFX_BIT(1),
    avxPipelineFlag_VIEWPORT = AFX_BIT(2),
    avxPipelineFlag_DEPTH_CLAMP = AFX_BIT(3),
    avxPipelineFlag_CULL_MODE = AFX_BIT(4),
    avxPipelineFlag_FRONT_FACE_INV = AFX_BIT(5),
    avxPipelineFlag_CTRL_POINTS = AFX_BIT(6)
} avxPipelineFlags;

typedef enum avxDepthStencilFlag
{
    avxDepthStencilFlag_TEST = AFX_BIT(0),
    avxDepthStencilFlag_COMPARE = AFX_BIT(1),
    avxDepthStencilFlag_DONT_WRITE = AFX_BIT(2),

    avxDepthStencilFlag_STENCIL_OP_F = AFX_BIT(4),
    avxDepthStencilFlag_STENCIL_TEST_F = AFX_BIT(5),
    avxDepthStencilFlag_STENCIL_WRITE_F = AFX_BIT(6),
    avxDepthStencilFlag_STENCIL_REF_F = AFX_BIT(7),
    avxDepthStencilFlag_STENCIL_FAIL_F = AFX_BIT(8),
    avxDepthStencilFlag_STENCIL_DFAIL_F = AFX_BIT(9),
    avxDepthStencilFlag_STENCIL_PASS_F = AFX_BIT(10),

    avxDepthStencilFlag_STENCIL_OP_B = AFX_BIT(11),
    avxDepthStencilFlag_STENCIL_TEST_B = AFX_BIT(12),
    avxDepthStencilFlag_STENCIL_WRITE_B = AFX_BIT(13),
    avxDepthStencilFlag_STENCIL_REF_B = AFX_BIT(14),
    avxDepthStencilFlag_STENCIL_FAIL_B = AFX_BIT(15),
    avxDepthStencilFlag_STENCIL_DFAIL_B = AFX_BIT(16),
    avxDepthStencilFlag_STENCIL_PASS_B = AFX_BIT(17),

    avxDepthStencilFlag_STENCIL_TEST = avxDepthStencilFlag_STENCIL_TEST_F | avxDepthStencilFlag_STENCIL_TEST_B,

    avxDepthStencilFlag_BOUNDS_TEST = AFX_BIT(31) // it is here because it only works with a depth buffer
} avxDepthStencilFlags;

typedef enum avxMultisamplingFlag
{
    avxMultisamplingFlag_SHADING = AFX_BIT(0),
    avxMultisamplingFlag_ALPHA_TO_COVERAGE = AFX_BIT(1),
    avxMultisamplingFlag_ALPHA_TO_ONE = AFX_BIT(2),
} avxMultisamplingFlags;

typedef enum avxRasterizationFlag
{
    avxRasterizationFlag_FILL_MODE = AFX_BIT(0),
    avxRasterizationFlag_LINE_WIDTH = AFX_BIT(1),
    avxRasterizationFlag_DEPTH_BIAS = AFX_BIT(2)
} avxRasterizationFlags;

typedef enum avxColorOutputFlag
{
    avxColorOutputFlag_SCISSOR = AFX_BIT(0),
    avxColorOutputFlag_BLEND_CONSTANTS = AFX_BIT(1),
    avxColorOutputFlag_LOGIC_OP = AFX_BIT(2)
} avxColorOutputFlags;
#endif

typedef enum avxColorMask
// Bitmask controlling which components are written to the framebuffer.
// The color write mask operation is applied regardless of whether blending is enabled.
// The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
{
    // The R value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_R = AFX_BIT(0),
    
    // The G value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_G = AFX_BIT(1),
    
    // The B value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_B = AFX_BIT(2),
    
    // The A value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxColorMask_A = AFX_BIT(3),

    avxColorMask_RG = avxColorMask_R | avxColorMask_G,
    avxColorMask_RGB = avxColorMask_RG | avxColorMask_B,
    avxColorMask_RGBA = avxColorMask_RGB | avxColorMask_A
} avxColorMask;

AFX_DEFINE_STRUCT(avxStencilInfo)
// Structure specifying stencil operation state.
{
    // The comparison operator used in the stencil test.
    avxCompareOp        compareOp; // avxCompareOp_ALWAYS

    // An integer value that is used as stencil reference in the unsigned stencil comparison. 
    afxUnit32           reference; // 0

    // The bits of the unsigned integer stencil values participating in the stencil test.
    afxUnit32           compareMask; // 1

    // The bits of the unsigned integer stencil values updated by the stencil test in the stencil framebuffer attachment.
    afxUnit32           writeMask; // 1

    // A value specifying the action performed on samples that fail the stencil test.
    avxStencilOp        failOp; // avxStencilOp_KEEP

    // A value specifying the action performed on samples that pass the stencil test and fail the depth test.
    avxStencilOp        depthFailOp; // avxStencilOp_KEEP

    // A value specifying the action performed on samples that pass both the depth and stencil tests.
    avxStencilOp        passOp; // avxStencilOp_KEEP
};

AFX_DEFINE_STRUCT(avxColorBlend)
// Describes how the color and/or alpha components of a fragment are blended.
{
    // Blend operation is used to calculate the RGB values to write to the color attachment. 
    avxBlendOp          rgbBlendOp; // ADD

    // Blend factor is used to determine the source factors (Sr,Sg,Sb). 
    avxBlendFactor      rgbSrcFactor; // ONE

    // Blend factor is used to determine the destination factors (Dr,Dg,Db). 
    avxBlendFactor      rgbDstFactor; // ZERO

    // Blend operation is used to calculate the alpha values to write to the color attachment.
    avxBlendOp          aBlendOp; // ADD

    // Blend factor is used to determine the source factor Sa.
    avxBlendFactor      aSrcFactor; // ONE

    // Blend factor is used to determine the destination factor Da.
    avxBlendFactor      aDstFactor; // ZERO
};

AFX_DEFINE_STRUCT(avxColorOutput)
{
    // Surface format compatible with pixel output.
    avxFormat           fmt;

    // controls whether blending is enabled for the corresponding color attachment. 
    // If blending is not enabled, the source fragment’s color for that attachment is passed through unmodified.
    afxBool             blendEnabled; // FALSE

    // Describes how the color or alpha components of a fragment are blended.
    avxColorBlend       blendConfig;

    // A bitmask to allow discarding of some or all color components.
    avxColorMask        discardMask; // NIL = write all
};

AFX_DEFINE_STRUCT(avxPipelineInfo)
{
    afxString           tag;
    afxUnit             stageCnt;
    avxLigature         liga;

    avxVertexInput       vin;
    avxTopology         primTopology;
    afxBool             primRestartEnabled;
    avxCullMode         cullMode;
    afxBool             frontFacingInverted;
    afxBool             depthClampEnabled;
    // controls whether primitives are discarded immediately before the rasterization stage. 
    afxBool             rasterizationDisabled; // FALSE
#if 0
    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;
#endif
    // rasterization
    // is the triangle rendering mode. 
    avxFillMode         fillMode; // avxFillMode_SOLID
    // is the width of rasterized line segments. 
    afxReal             lineWidth; // 1.f
    // depth bias computation
    // controls whether to bias fragment depth values. 
    afxBool             depthBiasEnabled; // FALSE
    // is a scalar factor applied to a fragment's slope in depth bias calculations. 
    afxReal             depthBiasSlopeScale; // 0.f
    // is a scalar factor controlling the constant depth value added to each fragment. 
    afxReal             depthBiasConstFactor; // 0.f
    // is the maximum (or minimum) depth bias of a fragment. 
    afxReal             depthBiasClamp; // 0.f
    // multisampling rasterization
    afxBool             msEnabled;
    // is a value specifying the number of samples used in rasterization. 
    afxUnit             sampleLvl; // 0
    // an array of sample mask values used in the sample mask test. 
    afxMask             sampleMasks[AVX_MAX_SAMPLE_MASKS]; // [ 1, ]

    // fragment & pixel output operations

    // scissor test

    // controls whether the alpha component of the fragment's first color output is replaced with one.
    afxBool             alphaToOneEnabled; // FALSE
    // controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. 
    afxBool             alphaToCoverageEnabled; // FALSE
     // used to enable Sample Shading.
    afxBool             sampleShadingEnabled; // FALSE
    // specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE.
    afxReal             minSampleShadingValue; // 0.f

    // stencil test
    afxBool             stencilTestEnabled;
    // is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilFront;
    // is the configuration controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilBack;

    // depth test
    // controls whether depth testing is enabled.
    afxBool             depthTestEnabled; // FALSE
    // is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. 
    avxCompareOp        depthCompareOp; // avxCompareOp_LESS
    // controls whether depth writes are enabled when depthTestEnabled is TRUE. 
    // Depth writes are always disabled when depthTestEnabled is FALSE.
    afxBool             depthWriteDisabled; // FALSE
    // is the format of depth/stencil surface this pipeline will be compatible with.
    avxFormat           dsFmt;
    // depth bounds test
    // controls whether depth bounds testing is enabled.
    afxBool             depthBoundsTestEnabled;
    // is the minimum depth bound used in the depth bounds test.
    afxV2d              depthBounds; // [ min, max ]

    // color bending, logical op and color writing
    afxUnit             colorOutCnt;
    avxColorOutput      colorOuts[AVX_MAX_COLOR_OUTPUTS];
    avxColor            blendConstants;
    afxBool             pixelLogicOpEnabled;
    avxLogicOp          pixelLogicOp;
};

AFX_DEFINE_STRUCT(avxPipelineBlueprint)
{
    afxString           tag;
    afxUnit             stageCnt;
    
    afxBool             transformationDisabled;
    // NOTE: the following members are ignored if transformationDisabled is TRUE.

    avxVertexInput      vin;
    // The primitive topology.
    avxTopology         primTop; // avxTopology_TRI_LIST
    // Treat a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) as restarting the assembly of primitives.
    afxBool             primRestartEnabled; // FALSE
    
    // Enable the programmable primitive tesselation stages in pipeline.
    afxBool             tesselationEnabled; // FALSE
    // The number of control points per patch.
    afxUnit             patchCtrlPoints;

    // Enable the programmable primitive processing stage in pipeline.
    afxBool             primShaderSupported; // FALSE
    // The number of viewports used by the pipeline.
    afxUnit             vpCnt; // At least 1.
    // The triangle facing direction used for primitive culling.
    avxCullMode         cullMode; // avxCullMode_BACK
    // When front facing is inverted, a triangle will be considered front-facing if its vertices are clockwise.
    afxBool             frontFacingInverted; // FALSE (CCW)
    // When depth clamp is enabled, the fragment's depth values will be clamped.
    afxBool             depthClampEnabled; // FALSE    
    
    // When rasterization is disabled, primitives are discarded immediately before the rasterization stage.
    afxBool             rasterizationDisabled; // FALSE
    // NOTE: the following members are ignored if rasterizationDisabled is TRUE.
#if 0
    avxPipelineFlags    primFlags;
    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;
#endif
    // rasterization
    // The triangle rendering mode.
    avxFillMode         fillMode; // avxFillMode_SOLID
    // The width of rasterized line segments.
    afxReal             lineWidth; // 1.f
    
    // --- Depth Bias Computation
    // When depth bias is enabled, the fragment depth values will be biased according to a specified configuration.
    afxBool             depthBiasEnabled; // FALSE
    // A scalar factor applied to a fragment's slope in depth bias calculations.
    afxReal             depthBiasSlopeScale; // 0.f
    // A scalar factor controlling the constant depth value added to each fragment.
    afxReal             depthBiasConstFactor; // 0.f
    // The maximum (or minimum) depth bias of a fragment.
    afxReal             depthBiasClamp; // 0.f

    // multisampling rasterization
    afxBool             msEnabled;
    // is a value specifying the number of samples used in rasterization. 
    afxUnit             sampleLvl; // 0
    // an array of sample mask values used in the sample mask test. 
    afxMask             sampleMasks[AVX_MAX_SAMPLE_MASKS]; // [ 1, ]

    // fragment & pixel output operations

    // scissor test

    // The alpha component of the fragment's first color output is replaced with one.
    afxBool             alphaToOneEnabled; // FALSE
    // A temporary coverage value is generated based on the alpha component of the fragment's first color output.
    afxBool             alphaToCoverageEnabled; // FALSE
    
    // Enable Sample Shading.
    afxBool             sampleShadingEnabled; // FALSE
    // A minimum fraction of sample shading if sampleShadingEnable is set to TRUE.
    afxReal             minSampleShadingValue; // 0.f

    // stencil test
    afxBool             stencilTestEnabled;
    // The configuration values controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilFront;
    // The configuration controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilBack;

    // depth test
    // Enable depth testing.
    afxBool             depthTestEnabled; // FALSE
    // is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. 
    avxCompareOp        depthCompareOp; // avxCompareOp_LESS
    // controls whether depth writes are enabled when depthTestEnabled is TRUE. 
    // Depth writes are always disabled when depthTestEnabled is FALSE. 
    afxBool             depthWriteDisabled; // FALSE
    // is the format of depth/stencil surface this pipeline will be compatible with.
    avxFormat           dsFmt;
    // depth bounds test
    // Controls whether depth bounds testing is enabled.
    afxBool             depthBoundsTestEnabled;
    // The minimum depth bound used in the depth bounds test.
    afxV2d              depthBounds; // [ min, max ]

    // color bending, logical op and color writing
    afxUnit             colorOutCnt;
    avxColorOutput      colorOuts[AVX_MAX_COLOR_OUTPUTS];
    avxColor            blendConstants;
    afxBool             pixelLogicOpEnabled;
    avxLogicOp          pixelLogicOp;
};

AVX avxColor const AVX_BLEND_CONSTANTS_DEFAULT;
AVX avxStencilInfo const AVX_STENCIL_INFO_DEFAULT;
AVX avxColorBlend const AVX_COLOR_BLEND_DEFAULT;
AVX avxColorOutput const AVX_COLOR_OUTPUT_DEFAULT;
AVX avxPipelineBlueprint const AVX_PIPELINE_BLUEPRINT_DEFAULT;

AVX void                AvxDescribePipeline(avxPipeline pip, avxPipelineInfo* info);

AVX afxUnit             AvxGetColorOutputs(avxPipeline pip, afxUnit first, afxUnit cnt, avxColorOutput ch[]);
AVX afxUnit             AvxGetMultisamplingMasks(avxPipeline pip, afxUnit first, afxUnit cnt, afxMask sampleMask[]);

AVX afxBool             AvxGetPipelineShader(avxPipeline pip, avxShaderStage stage, avxShader* shader);
AVX afxUnit             AvxGetPipelineShaders(avxPipeline pip, afxIndex first, afxUnit cnt, avxShader shaders[]);

AVX afxBool             AvxGetPipelineLigature(avxPipeline pip, avxLigature* ligature);
AVX afxBool             AvxGetPipelineVertexInput(avxPipeline pip, avxVertexInput* input);

AVX afxError            AvxRelinkPipelineFunction(avxPipeline pip, avxShaderStage stage, avxShader shd, afxString const* fn, afxUnit const specIds[], void const* specValues[]);
AVX afxError            AvxUplinkPipelineFunction(avxPipeline pip, avxShaderStage stage, afxUri const* uri, afxString const* fn, afxUnit const specIds[], void const* specValues[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AvxAssembleComputePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const blueprints[], avxPipeline pipelines[]);

AVX afxError            AvxAssemblePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const cfg[], avxPipeline razr[]);

AVX afxError            AvxLoadPipeline(afxDrawSystem dsys, avxVertexInput vin, afxUri const* uri, avxPipeline* pipeline);

#endif//AVX_PIPELINE_H
