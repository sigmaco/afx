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
#include "qwadro/inc/draw/io/afxPixel.h"
#include "qwadro/inc/math/afxColor.h"
#include "qwadro/inc/base/afxFixedString.h"

/*
    The first stage of the graphics pipeline (Input Assembler) assembles vertices to form geometric primitives such as points, lines, and triangles, based on a requested primitive topology.
    In the next stage (Vertex Shader) vertices can be transformed, computing positions and attributes for each vertex.
    If tessellation and/or geometry shaders are supported, they can then generate multiple primitives from a single input primitive, possibly changing the primitive topology or generating additional attribute data in the process.
    The final resulting primitives are clipped to a clip volume in preparation for the next stage, Rasterization.
    The rasterizer produces a series of fragments associated with a region of the framebuffer, from a two-dimensional description of a point, line segment, or triangle.
    These fragments are processed by fragment operations to determine whether generated values will be written to the framebuffer.
    Fragment shading determines the values to be written to the framebuffer attachments.
    Framebuffer operations then read and write the color and depth/stencil attachments of the framebuffer for a given subpass of a render pass instance.
    The attachments can be used as input attachments in the fragment shader in a later subpass of the same render pass.
*/

// Front facing and cull mode should not have effect no point and lines.

#define AVX_MAX_SAMPLE_MASKS 32
#define AVX_MAX_COLOR_OUTPUTS 8

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
    avxPipelineStage_DRAW_INDIRECT  = AFX_BIT(1), /// where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    avxPipelineStage_VERTEX_INPUT   = AFX_BIT(2), /// where vertex and index buffers are consumed.
    avxPipelineStage_VERTEX         = AFX_BIT(3), /// the vertex shader stage.
    avxPipelineStage_DOMAIN         = AFX_BIT(4), /// the tessellation control shader stage.
    avxPipelineStage_HULL           = AFX_BIT(5), /// the tessellation evaluation shader stage.
    avxPipelineStage_PRIM           = AFX_BIT(6), /// the primitive (aka geometry) shader stage.
    avxPipelineStage_FRAGMENT       = AFX_BIT(7), /// the fragment shader stage.
    avxPipelineStage_EARLY_TESTS    = AFX_BIT(8), /// where early fragment tests (depth and stencil tests before fragment shading) are performed. This stage also includes render pass load operations for framebuffer attachments with a depth/stencil format.
    avxPipelineStage_LATE_TESTS     = AFX_BIT(9), /// where late fragment tests (depth and stencil tests after fragment shading) are performed. This stage also includes render pass store operations for framebuffer attachments with a depth/stencil format.
    avxPipelineStage_COLOR_OUTPUT   = AFX_BIT(10), /// where the final color values are output from the pipeline. This stage includes blending, logic operations, render pass load and store operations for color attachments, render pass multisample resolve operations, and AvxCmdClearAttachments.
    avxPipelineStage_COMPUTE        = AFX_BIT(11), /// the execution of a compute shader.
    avxPipelineStage_TRANSFER       = AFX_BIT(12), /// the following commands: copies, blits, resolves, clears.
    avxPipelineStage_BOTTOM         = AFX_BIT(13), 
    avxPipelineStage_HOST           = AFX_BIT(14), /// pseudo-stage indicating execution on the host of reads/writes of device memory. This stage is not invoked by any commands recorded in a command buffer.
    avxPipelineStage_GRAPHICS       = AFX_BIT(15), /// the execution of all graphics pipeline stages.
    avxPipelineStage_COMMANDS       = AFX_BIT(16), /// all operations performed by all commands supported on the queue it is used with.
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

typedef enum avxPipelineFlag
{
    avxPipelineFlag_TOPOLOGY        = AFX_BIT(0),
    avxPipelineFlag_RESTART         = AFX_BIT(1),
    avxPipelineFlag_VIEWPORT        = AFX_BIT(2),
    avxPipelineFlag_DEPTH_CLAMP     = AFX_BIT(3),
    avxPipelineFlag_CULL_MODE       = AFX_BIT(4),
    avxPipelineFlag_FRONT_FACE_INV  = AFX_BIT(5),
    avxPipelineFlag_CTRL_POINTS     = AFX_BIT(6)
} avxPipelineFlags;

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

typedef enum avxRgbaMask
/// Bitmask controlling which components are written to the framebuffer.
/// The color write mask operation is applied regardless of whether blending is enabled.
/// The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
{
    avxRgbaMask_R = AFX_BIT(0), /// specifies that the R value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_G = AFX_BIT(1), /// specifies that the G value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_B = AFX_BIT(2), /// specifies that the B value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_A = AFX_BIT(3), /// specifies that the A value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_RG = avxRgbaMask_R | avxRgbaMask_G,
    avxRgbaMask_RGB = avxRgbaMask_RG | avxRgbaMask_B,
    avxRgbaMask_RGBA = avxRgbaMask_RGB | avxRgbaMask_A
} avxRgbaMask;

AFX_DEFINE_STRUCT(avxStencilInfo)
/// Structure specifying stencil operation state
{
    avxCompareOp        compareOp; /// is a value specifying the comparison operator used in the stencil test. /// avxCompareOp_ALWAYS
    afxUnit32           reference; /// is an integer stencil reference value that is used in the unsigned stencil comparison. /// 0
    afxUnit32           compareMask; /// selects the bits of the unsigned integer stencil values participating in the stencil test. // 1
    afxUnit32           writeMask; /// selects the bits of the unsigned integer stencil values updated by the stencil test in the stencil framebuffer attachment. /// 1
    avxStencilOp        failOp; /// is a value specifying the action performed on samples that fail the stencil test. /// avxStencilOp_KEEP
    avxStencilOp        depthFailOp; /// is a value specifying the action performed on samples that pass the stencil test and fail the depth test. /// avxStencilOp_KEEP
    avxStencilOp        passOp; /// is a value specifying the action performed on samples that pass both the depth and stencil tests. /// avxStencilOp_KEEP    
};

AFX_DEFINE_STRUCT(avxColorBlend)
/// Describes how the color or alpha components of a fragment are blended
{
    avxBlendOp          rgbBlendOp; /// selects which blend operation is used to calculate the RGB values to write to the color attachment. // ADD
    avxBlendFactor      rgbSrcFactor; /// selects which blend factor is used to determine the source factors (Sr,Sg,Sb). // ONE
    avxBlendFactor      rgbDstFactor; /// selects which blend factor is used to determine the destination factors (Dr,Dg,Db). // ZERO
    avxBlendOp          aBlendOp; /// selects which blend operation is used to calculate the alpha values to write to the color attachment. // ADD
    avxBlendFactor      aSrcFactor; /// selects which blend factor is used to determine the source factor Sa. // ONE
    avxBlendFactor      aDstFactor; /// selects which blend factor is used to determine the destination factor Da. // ZERO
};

AFX_DEFINE_STRUCT(avxColorOutput)
{
    avxFormat           fmt; /// Surface format compatible with pixel output.
    afxBool             blendEnabled; /// controls whether blending is enabled for the corresponding color attachment. If blending is not enabled, the source fragment’s color for that attachment is passed through unmodified. FALSE
    avxColorBlend       blendConfig; /// Describes how the color or alpha components of a fragment are blended.
    avxRgbaMask         discardMask; /// NIL = write all
};

AFX_DEFINE_STRUCT(avxPipelineInfo)
{
    afxUnit             stageCnt;
    avxLigature         liga;

    avxVertexDecl      vin;
    avxTopology         primTopology;
    afxBool             primRestartEnabled;
    avxCullMode         cullMode;
    afxBool             frontFacingInverted;
    afxBool             depthClampEnabled;
    afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE

    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;

    // rasterization
    avxFillMode         fillMode; /// is the triangle rendering mode. /// avxFillMode_SOLID
    afxReal             lineWidth; /// is the width of rasterized line segments. /// 1.f
    // depth bias computation
    afxBool             depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal             depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal             depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal             depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f
    // multisampling rasterization
    afxBool             msEnabled;
    afxUnit             sampleLvl; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask             sampleMasks[AVX_MAX_SAMPLE_MASKS]; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // fragment & pixel output operations

    // scissor test

    //
    afxBool             alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool             alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool             sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal             minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool             stencilTestEnabled;
    avxStencilInfo      stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool             depthTestEnabled; /// controls whether depth testing is enabled. FALSE
    avxCompareOp        depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. avxCompareOp_LESS
    afxBool             depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. FALSE
    avxFormat           dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool             depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled.
    afxV2d              depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color bending, logical op and color writing
    afxUnit             colorOutCnt;
    avxColorOutput      colorOuts[AVX_MAX_COLOR_OUTPUTS];
    afxColor            blendConstants;
    afxBool             pixelLogicOpEnabled;
    avxLogicOp          pixelLogicOp;
};

AFX_DEFINE_STRUCT(avxPipelineBlueprint)
{
    afxUnit             stageCnt;
    
    afxBool             transformationDisabled;
    // NOTE: the following members are ignored if transformationDisabled is TRUE.

    avxVertexDecl       vin;
    avxTopology         primTop; /// is a option defining the primitive topology. /// avxTopology_TRI_LIST
    avxPipelineFlags    primFlags;
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    
    afxBool             tesselationEnabled;
    afxUnit             patchCtrlPoints; /// is the number of control points per patch.

    afxBool             primShaderSupported;
    afxUnit             vpCnt; /// the number of viewports used by the pipeline. At least 1.
    avxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// avxCullMode_BACK
    afxBool             frontFacingInverted; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE    
    
    afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
    // NOTE: the following members are ignored if rasterizationDisabled is TRUE.

    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;

    // rasterization
    avxFillMode         fillMode; /// is the triangle rendering mode. /// avxFillMode_SOLID
    afxReal             lineWidth; /// is the width of rasterized line segments. /// 1.f
    // depth bias computation
    afxBool             depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal             depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal             depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal             depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f
    // multisampling rasterization
    afxBool             msEnabled;
    afxUnit             sampleLvl; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask             sampleMasks[AVX_MAX_SAMPLE_MASKS]; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // fragment & pixel output operations

    // scissor test

    //
    afxBool             alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool             alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool             sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal             minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool             stencilTestEnabled;
    avxStencilInfo      stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool             depthTestEnabled; /// controls whether depth testing is enabled. FALSE
    avxCompareOp        depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. avxCompareOp_LESS
    afxBool             depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. FALSE
    avxFormat           dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool             depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled.
    afxV2d              depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color bending, logical op and color writing
    afxUnit             colorOutCnt;
    avxColorOutput      colorOuts[AVX_MAX_COLOR_OUTPUTS];
    afxColor            blendConstants;
    afxBool             pixelLogicOpEnabled;
    avxLogicOp          pixelLogicOp;
};

AVX afxDrawSystem       AfxGetPipelineContext(avxPipeline pip);

AVX void                AfxDescribePipeline(avxPipeline pip, avxPipelineInfo* info);

AVX afxUnit             AfxGetColorOutputChannels(avxPipeline pip, afxUnit first, afxUnit cnt, avxColorOutput ch[]);
AVX afxUnit             AfxGetMultisamplingMasks(avxPipeline pip, afxUnit first, afxUnit cnt, afxMask sampleMask[]);

AVX afxBool             AfxGetPipelineShader(avxPipeline pip, avxShaderStage stage, avxShader* shader);
AVX afxUnit             AfxGetPipelineShaders(avxPipeline pip, afxIndex first, afxUnit cnt, avxShader shaders[]);

AVX afxBool             AfxGetPipelineLigature(avxPipeline pip, avxLigature* ligature);
AVX afxBool             AfxGetPipelineVertexInput(avxPipeline pip, avxVertexDecl* input);

AVX afxError            AfxRelinkPipelineFunction(avxPipeline pip, avxShaderStage stage, avxShader shd, afxString const* fn, afxUnit const specIds[], void const* specValues[]);
AVX afxError            AfxUplinkPipelineFunction(avxPipeline pip, avxShaderStage stage, afxUri const* uri, afxString const* fn, afxUnit const specIds[], void const* specValues[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAssembleComputePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const blueprints[], avxPipeline pipelines[]);

AVX afxError            AfxAssemblePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const cfg[], avxPipeline razr[]);

AVX afxError            AfxLoadPipeline(afxDrawSystem dsys, avxVertexDecl vin, afxUri const* uri, avxPipeline* pipeline);

#endif//AVX_PIPELINE_H
