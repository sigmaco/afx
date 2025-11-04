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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DRAWING DEVICE EXECUTION PIPELINE                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_PIPELINE_H
#define AVX_PIPELINE_H

/**
    Desde que surgiu as mesh shaders, somado as experiências com rasterização com CUDA, aparentemente o processo de transformação geométrica das pipelines tende a mudar.
    Assim sendo, a SIGMA decideu quebrar o objeto de estado da pipeline em módulos, e assembleá-los, para evitar lidar com alocação de espaço para coisas pouco utilizadas.
    Esta abordagem é, de longe, a mais arbitrária, nadando contra o recúo de maré do Vulkan, Direct3D 12 e Metal.
*/

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

// Shaders are "potentially" relinkable to absorb capabilities of using separate shaders in other APIs.
// At first, it is though that instead of using it in the command buffer, we relink shaders into a pipeline.
// This is really not defined yet. There is a thinking about avxVertexInput too.

// Front facing and cull mode should not have effect no point and lines.

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/draw/avxFormat.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/draw/avxRasterization.h"

#define AVX_MAX_SHADER_SPECIALIZATIONS (16)
#define AVX_MAX_PROGRAMMABLE_PIPELINE_STAGES (8)

typedef enum avxBus
// Enumerant specifing a pipelining bus.
// In AVX, a pipelining bus is a distinct set of conductors carrying data and control signals within a graphics device, 
// to which pieces of pipeline state may be connected in parallel.
{
    // Specifies the rasterization/graphics bus.
    avxBus_GFX,
    // Specifies the compute bus.
    avxBus_PCX,
    // Specifies the ray tracing bus.
    avxBus_RTX,

    avxBus_TOTAL
} avxBus;

AFX_DEFINE_STRUCT(avxPipelineConfig)
{
    // Bus which this pipeline will be executed.
    // Probably will be removed by specialized acquision functions.
    avxBus              bus;
    // The avxCodebase to be used to assemble programmable functions.
    avxCodebase         codb;
    // A optional handle to a avxLigature object.
    // If NIL, one new avxLigature object will be generated for the pipeline.
    avxLigature         liga;
    // A debugging string assigned to the pipeline.
    afxString           tag;
    // An user-defined data assigned to the pipeline.
    void*               udd;

    afxUnit             stageCnt;
    afxUnit             specializedWorkGrpSiz[3];

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
    avxFillMode         fillMode; // avxFillMode_FACE
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

    // fragment & pixel output operations
    avxMultisampling    ms;

    // stencil test
    afxBool             stencilTestEnabled;
    // The configuration values controlling the corresponding parameters of the stencil test.
    avxStencilConfig    stencilFront;
    // The configuration controlling the corresponding parameters of the stencil test.
    avxStencilConfig    stencilBack;

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

AVX afxError            AvxAssemblePcxPipelines
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    avxPipelineConfig const blueprints[], 
    avxPipeline pipelines[]
);

AVX afxError            AvxAssembleGfxPipelines
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    avxPipelineConfig const cfg[], 
    avxPipeline razr[]
);

AVX afxError            AvxLoadPipeline
(
    afxDrawSystem dsys, 
    avxVertexInput vin, 
    afxUri const* uri, 
    avxPipeline* pipeline
);

////////////////////////////////////////////////////////////////////////////////

typedef enum avxBusStage
// Bitmask-based value specifying pipeline stages.
{
    // NIL,
    // Pseudo-stage indicating execution on the host of reads/writes of device memory. 
    // This stage is not invoked by any commands recorded in a draw context.
    avxBusStage_HOST        = AFX_BITMASK(0),

    // All operations performed by all commands supported on the queue it is used with.
    // This is a general "catch-all" stage that includes all commands in the pipeline.
    // Often used for full synchronization when you're not sure what stages are involved.
    avxBusStage_COMMANDS    = AFX_BITMASK(1),

    // First synchronization scope: Source of the synchronization (where an operation happens-before).
    // Second synchronization scope: Destination of the synchronization (where the synchronized operation is allowed to begin).
    // This is handled via pipeline stage masks and access masks, and the new synchronization APIs.
    // In the first scope (srcStageMask):
    //  - Represents "before any commands in the pipeline".
    //  - No commands are considered to be in-flight yet.
    //  - Effectively means "nothing"; so it’s equivalent to avxBusStage == NIL.
    //  - Used when no previous stages are relevant for the sync.
    // In the second scope (dstStageMask):
    //  - Represents "before any commands start".
    //  - Since synchronization ensures something happens before the dstStageMask, this means the command being synchronized to can start only after all previous commands complete.
    //  - With accessMask = 0, it becomes a full memory barrier, effectively like avxBusStage_COMMANDS (but with some nuance; see below).
    //  - So in this context, it acts like a full barrier when paired with accessMask = 0.
    avxBusStage_TOP         = AFX_BITMASK(2),
    
    // First synchronization scope (srcStageMask): Source of the sync; the stages you're waiting on.
    // Second synchronization scope (dstStageMask): Destination of the sync; the stages that must wait.
    // In the first scope (srcStageMask):
    //  - Represents after all commands have completed.
    //  - So, it's a way to say "wait for everything to be done".
    //  - With accessMask = 0, this is effectively equivalent to avxBusStage_COMMANDS with no accessMask.
    //  - Acts as a full barrier (waits for all previous commands, but doesn't make any memory visible unless access mask is used).
    // In the second scope(dstStageMask):
    //  - avxBusStage_BOTTOM is the last possible stage in the pipeline.
    //  - So there's nothing that can execute after it.
    //  - Specifying this as the destination stage means : "synchronize to... nothing".
    //  - Thus, it is equivalent to avxBusStage == NIL.
    //  - No actual synchronization occurs. Nothing is delayed.
    avxBusStage_BOTTOM      = AFX_BITMASK(3),
    
    // The execution of all copy commands.
    avxBusStage_COPY        = AFX_BITMASK(4),
    
    // The execution of all resolve commands.
    avxBusStage_RESOLVE     = AFX_BITMASK(5),

    // The execution of all blit commands.
    avxBusStage_BLIT        = AFX_BITMASK(6),
    
    // The execution of clear/fill commands, with the exception of AmxCmdClearCanvas.
    avxBusStage_CLEAR       = AFX_BITMASK(7),
    
    // The following commands: copies, blits, resolves, clears.
    avxBusStage_TRANSFER    = (avxBusStage_COPY | avxBusStage_RESOLVE | avxBusStage_BLIT | avxBusStage_CLEAR),

    // The execution of a compute shader.
    avxBusStage_COMPUTE     = AFX_BITMASK(8),

    // Where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    avxBusStage_DRAW_IND    = AFX_BITMASK(9),
    
    // The stage of the pipeline where index buffers are consumed.
    avxBusStage_VTX_IDX     = AFX_BITMASK(10),

    // The stage of the pipeline where vertex buffers are consumed.
    avxBusStage_VTX_ATTR    = AFX_BITMASK(11),

    // The stage of the pipeline where index buffers are consumed.
    avxBusStage_VTX_IN      = (avxBusStage_VTX_IDX | avxBusStage_VTX_ATTR),

    // The vertex shader stage.
    avxBusStage_VERTEX      = AFX_BITMASK(12),

    // The tessellation control shader stage.
    avxBusStage_DOMAIN      = AFX_BITMASK(13),

    // The tessellation evaluation shader stage.
    avxBusStage_HULL        = AFX_BITMASK(14),

    // The primitive (aka geometry) shader stage.
    avxBusStage_PRIM        = AFX_BITMASK(15),

    // All supported pre-rasterization stages, including vertex, tesselation, primitive and mesh shaders.
    avxBusStage_PRERASTER   = (avxBusStage_VERTEX | avxBusStage_DOMAIN | avxBusStage_HULL | avxBusStage_PRIM),

    // where early fragment tests (depth and stencil tests before fragment shading) are performed. 
    // This stage also includes draw scope load operations for canvas' attached buffers with a depth/stencil format.
    avxBusStage_EARLY_TESTS = AFX_BITMASK(16),

    // The fragment shader stage.
    avxBusStage_FRAGMENT    = AFX_BITMASK(17),

    // Where late fragment tests (depth and stencil tests after fragment shading) are performed. 
    // This stage also includes draw scope store operations for canvas' attached buffers with a depth/stencil format.
    avxBusStage_LATE_TESTS  = AFX_BITMASK(18),

    // After blending, where the final color values are output from the pipeline. This stage includes blending, 
    // logic operations, draw scope load and store operations for color buffers, draw scope multisample resolve operations, 
    // and AvxCmdClearCanvas.
    avxBusStage_COLOR_OUT   = AFX_BITMASK(19),
        
    // The execution of all graphics pipeline stages.
    avxBusStage_GRAPHICS    = ( avxBusStage_DRAW_IND | 
                                avxBusStage_VTX_IN |
                                avxBusStage_VERTEX |
                                avxBusStage_DOMAIN |
                                avxBusStage_HULL |
                                avxBusStage_PRIM |
                                avxBusStage_FRAGMENT |
                                avxBusStage_EARLY_TESTS |
                                avxBusStage_LATE_TESTS |
                                avxBusStage_COLOR_OUT),

    avxBusStage_VIDDEC      = AFX_BITMASK(20),

    avxBusStage_VIDENC      = AFX_BITMASK(21),
} avxBusStage;

typedef enum avxPipeDep
{
    avxPipeDep_VIEWPORT = AFX_BITMASK(0), // always required in Qwadro
    avxPipeDep_SCISSOR = AFX_BITMASK(1), // always required in Qwadro
    avxPipeDep_LINE_WIDTH = AFX_BITMASK(2),
    avxPipeDep_DEPTH_BIAS = AFX_BITMASK(3),
    avxPipeDep_BLEND_CONSTANTS = AFX_BITMASK(4),
    avxPipeDep_DEPTH_BOUNDS = AFX_BITMASK(5),
    avxPipeDep_STENCIL_COMPARE_MASK = AFX_BITMASK(6),
    avxPipeDep_STENCIL_WRITE_MASK = AFX_BITMASK(7),
    avxPipeDep_STENCIL_REF = AFX_BITMASK(8),
    avxPipeDep_CULL_MODE = AFX_BITMASK(9),
    avxPipeDep_FRONT_FACE = AFX_BITMASK(10),
    avxPipeDep_PRIM_TOPOLOGY = AFX_BITMASK(11),
    avxPipeDep_VIEWPORT_RESET = AFX_BITMASK(12),
    avxPipeDep_SCISSOR_RESET = AFX_BITMASK(13),
    avxPipeDep_VTX_INPUT_BINDING_STRIDE = AFX_BITMASK(14),
    avxPipeDep_DEPTH_TEST_ENABLE = AFX_BITMASK(15),
    avxPipeDep_DEPTH_WRITE_ENABLE = AFX_BITMASK(16),
    avxPipeDep_DEPTH_COMPARE_OP = AFX_BITMASK(17),
    avxPipeDep_DEPTH_BOUNDS_TEST_ENABLE = AFX_BITMASK(18),
    avxPipeDep_STENCIL_TEST_ENABLE = AFX_BITMASK(19),
    avxPipeDep_STENCIL_OP = AFX_BITMASK(20),
    avxPipeDep_RASTERIZER_DISCARD_ENABLE = AFX_BITMASK(21),
    avxPipeDep_DEPTH_BIAS_ENABLE = AFX_BITMASK(22),
    avxPipeDep_PRIM_RESTART_ENABLE = AFX_BITMASK(23),

    avxPipeDep_TOTAL
} avxPipeDeps;

#if 0
typedef enum avxPipelineFlag
{
    avxPipelineFlag_TOPOLOGY = AFX_BITMASK(0),
    avxPipelineFlag_RESTART = AFX_BITMASK(1),
    avxPipelineFlag_VIEWPORT = AFX_BITMASK(2),
    avxPipelineFlag_DEPTH_CLAMP = AFX_BITMASK(3),
    avxPipelineFlag_CULL_MODE = AFX_BITMASK(4),
    avxPipelineFlag_FRONT_FACE_INV = AFX_BITMASK(5),
    avxPipelineFlag_CTRL_POINTS = AFX_BITMASK(6)
} avxPipelineFlags;

typedef enum avxDepthStencilFlag
{
    avxDepthStencilFlag_TEST = AFX_BITMASK(0),
    avxDepthStencilFlag_COMPARE = AFX_BITMASK(1),
    avxDepthStencilFlag_DONT_WRITE = AFX_BITMASK(2),

    avxDepthStencilFlag_STENCIL_OP_F = AFX_BITMASK(4),
    avxDepthStencilFlag_STENCIL_TEST_F = AFX_BITMASK(5),
    avxDepthStencilFlag_STENCIL_WRITE_F = AFX_BITMASK(6),
    avxDepthStencilFlag_STENCIL_REF_F = AFX_BITMASK(7),
    avxDepthStencilFlag_STENCIL_FAIL_F = AFX_BITMASK(8),
    avxDepthStencilFlag_STENCIL_DFAIL_F = AFX_BITMASK(9),
    avxDepthStencilFlag_STENCIL_PASS_F = AFX_BITMASK(10),

    avxDepthStencilFlag_STENCIL_OP_B = AFX_BITMASK(11),
    avxDepthStencilFlag_STENCIL_TEST_B = AFX_BITMASK(12),
    avxDepthStencilFlag_STENCIL_WRITE_B = AFX_BITMASK(13),
    avxDepthStencilFlag_STENCIL_REF_B = AFX_BITMASK(14),
    avxDepthStencilFlag_STENCIL_FAIL_B = AFX_BITMASK(15),
    avxDepthStencilFlag_STENCIL_DFAIL_B = AFX_BITMASK(16),
    avxDepthStencilFlag_STENCIL_PASS_B = AFX_BITMASK(17),

    avxDepthStencilFlag_STENCIL_TEST = avxDepthStencilFlag_STENCIL_TEST_F | avxDepthStencilFlag_STENCIL_TEST_B,

    avxDepthStencilFlag_BOUNDS_TEST = AFX_BITMASK(31) // it is here because it only works with a depth buffer
} avxDepthStencilFlags;

typedef enum avxMultisamplingFlag
{
    avxMultisamplingFlag_SHADING = AFX_BITMASK(0),
    avxMultisamplingFlag_ALPHA_TO_COVERAGE = AFX_BITMASK(1),
    avxMultisamplingFlag_ALPHA_TO_ONE = AFX_BITMASK(2),
} avxMultisamplingFlags;

typedef enum avxRasterizationFlag
{
    avxRasterizationFlag_FILL_MODE = AFX_BITMASK(0),
    avxRasterizationFlag_LINE_WIDTH = AFX_BITMASK(1),
    avxRasterizationFlag_DEPTH_BIAS = AFX_BITMASK(2)
} avxRasterizationFlags;

typedef enum avxColorOutputFlag
{
    avxColorOutputFlag_SCISSOR = AFX_BITMASK(0),
    avxColorOutputFlag_BLEND_CONSTANTS = AFX_BITMASK(1),
    avxColorOutputFlag_LOGIC_OP = AFX_BITMASK(2)
} avxColorOutputFlags;
#endif

AFX_DEFINE_STRUCT(avxPipelineInfo)
{
    avxBus          bus;
    afxUnit         stageCnt;
    avxLigature     liga;
    avxVertexInput  vin;
    void*           udd;
    afxString       tag;
};


AFX_DEFINE_STRUCT(avxPipelineBlueprint2)
{
    afxString           tag;
    afxUnit             stageCnt;
    afxUnit             specializedWorkGrpSiz[3];
};

#if 0
AFX_DECLARE_STRUCT(avxRasterizer)
{
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
    avxClipping         clipping;
    avxRasterization    rasterization;
};
#endif

AVX avxStencilConfig const AVX_STENCIL_INFO_DEFAULT;
AVX avxPipelineConfig const AVX_PIPELINE_BLUEPRINT_DEFAULT;

AVX afxDrawSystem   AvxGetPipelineHost
(
    avxPipeline pip
);

AVX avxBus          AvxGetPipelineBus
(
    avxPipeline pip
);

AVX afxFlags            AvxGetPipelineFlags
(
    avxPipeline pip, 
    afxFlags mask
);

AVX void                AvxDescribePipeline
(
    avxPipeline pip, 
    avxPipelineInfo* info
);

AVX afxUnit             AvxGetColorOutputs
(
    avxPipeline pip, 
    afxUnit first, 
    afxUnit cnt, 
    avxColorOutput ch[]
);

AVX afxUnit             AvxGetMultisamplingMasks
(
    avxPipeline pip, 
    afxUnit first, 
    afxUnit cnt, 
    afxMask sampleMask[]
);

AVX afxBool             AvxGetPipelineCodebase
(
    avxPipeline pip,
    avxCodebase* codebase
);

AVX afxBool             AvxGetPipelineShader
(
    avxPipeline pip, 
    avxShaderType stage, 
    afxUnit* prog,
    afxString* func
);

AVX afxUnit             AvxGetPipelinePrograms
(
    avxPipeline pip, 
    afxIndex first, 
    afxUnit cnt, 
    afxUnit progs[],
    afxString funcs[]
);

AVX afxBool             AvxGetPipelineLigature
(
    avxPipeline pip, 
    avxLigature* ligature
);

AVX afxBool             AvxGetPipelineVertexInput
(
    avxPipeline pip, 
    avxVertexInput* input
);

AFX_DEFINE_STRUCT(avxShaderSpecialization)
// Structure specifying specialized shader linking into a pipeline.
{
    avxShaderType   stage;
    afxString       prog;
    afxString       fn;
    afxString       constants[AVX_MAX_SHADER_SPECIALIZATIONS];
    union
    {
        afxReal     dataf;
        afxUnit     datau;
        afxInt      datai;
    }               constantValues[AVX_MAX_SHADER_SPECIALIZATIONS];
};

AVX afxError            AvxReprogramPipeline
(
    avxPipeline pip, 
    afxUnit cnt, 
    avxShaderSpecialization const specs[]
);

// TODO: Attach multiple shaders for same stage, then use AvxCmdBindShaders(dctx, cnt, avxShaderType stage, afxUnit shdIdx) to select one at execution time.

#endif//AVX_PIPELINE_H
