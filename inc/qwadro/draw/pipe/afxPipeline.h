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

// Desde que surgiu as mesh shaders, somado as experiências com rasterização com CUDA, aparentemente o processo de transformação geométrica das pipelines tende a mudar.
// Assim sendo, a SIGMA decideu quebrar o objeto de estado da pipeline em módulos, e assembleá-los, para evitar lidar com alocação de espaço para coisas pouco utilizadas.
// Esta abordagem é, de longe, a mais arbitrária, nadando contra o recúo de maré do Vulkan, Direct3D 12 e Metal.

#ifndef AVX_PIPELINE_H
#define AVX_PIPELINE_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxFixedString.h"

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

typedef enum afxPipelineStage
{
    // NIL,
    afxPipelineStage_TOP            = AFX_BIT(0),
    afxPipelineStage_DRAW_INDIRECT  = AFX_BIT(1), /// where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    afxPipelineStage_VERTEX_INPUT   = AFX_BIT(2), /// where vertex and index buffers are consumed.
    afxPipelineStage_VERTEX         = AFX_BIT(3), /// the vertex shader stage.
    afxPipelineStage_DOMAIN         = AFX_BIT(4), /// the tessellation control shader stage.
    afxPipelineStage_HULL           = AFX_BIT(5), /// the tessellation evaluation shader stage.
    afxPipelineStage_PRIM           = AFX_BIT(6), /// the primitive (aka geometry) shader stage.
    afxPipelineStage_FRAGMENT       = AFX_BIT(7), /// the fragment shader stage.
    afxPipelineStage_EARLY_TESTS    = AFX_BIT(8), /// where early fragment tests (depth and stencil tests before fragment shading) are performed. This stage also includes render pass load operations for framebuffer attachments with a depth/stencil format.
    afxPipelineStage_LATE_TESTS     = AFX_BIT(9), /// where late fragment tests (depth and stencil tests after fragment shading) are performed. This stage also includes render pass store operations for framebuffer attachments with a depth/stencil format.
    afxPipelineStage_COLOR_OUTPUT   = AFX_BIT(10), /// where the final color values are output from the pipeline. This stage includes blending, logic operations, render pass load and store operations for color attachments, render pass multisample resolve operations, and AvxCmdClearAttachments.
    afxPipelineStage_COMPUTE        = AFX_BIT(11), /// the execution of a compute shader.
    afxPipelineStage_TRANSFER       = AFX_BIT(12), /// the following commands: copies, blits, resolves, clears.
    afxPipelineStage_BOTTOM         = AFX_BIT(13), 
    afxPipelineStage_HOST           = AFX_BIT(14), /// pseudo-stage indicating execution on the host of reads/writes of device memory. This stage is not invoked by any commands recorded in a command buffer.
    afxPipelineStage_GRAPHICS       = AFX_BIT(15), /// the execution of all graphics pipeline stages.
    afxPipelineStage_COMMANDS       = AFX_BIT(16), /// all operations performed by all commands supported on the queue it is used with.
} afxPipelineStage;

typedef enum afxPipelineFlag
{
    afxPipelineFlag_TOPOLOGY        = AFX_BIT(0),
    afxPipelineFlag_RESTART         = AFX_BIT(1),
    afxPipelineFlag_VIEWPORT        = AFX_BIT(2),
    afxPipelineFlag_DEPTH_CLAMP     = AFX_BIT(3),
    afxPipelineFlag_CULL_MODE       = AFX_BIT(4),
    afxPipelineFlag_FRONT_FACE_INV  = AFX_BIT(5),
    afxPipelineFlag_CTRL_POINTS     = AFX_BIT(6)
} afxPipelineFlags;

AFX_DEFINE_STRUCT(afxPipelineBlueprint)
{
    afxPipelineFlags    primFlags;
    afxPrimTopology     primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat              patchControlPoints; /// is the number of control points per patch.
    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE    
    afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
    afxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool             invertFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxNat              vpCnt; /// the number of viewports used by the pipeline. At least 1.
    afxVertexInput      vin;

    afxNat              shdCnt;
    afxShader           shd[5];
    afxShaderStage      shdStage[5];
    afxUri              shdUri[5];
    afxString           shdFn[5];
    //afxRasterizer       razr;
    //afxUri              razrUri;
};

#ifdef _AVX_DRAW_C
AFX_DEFINE_STRUCT(afxShaderSlot)
{
    afxShaderStage      stage;
    afxShader           shd;
    afxString8          fn;
    //constants;
    //                  specId;
};
#ifdef _AVX_PIPELINE_C
#ifndef _AVX_PIPELINE_IMPL
AFX_OBJECT(afxPipeline)
#else
struct afxBasePipeline
#endif
{
    afxNat              stageCnt;
    afxShaderSlot*      stages;
    afxNat              vtxShdIdx;
    afxNat              hulShdIdx;
    afxNat              domShdIdx;
    afxNat              geoShdIdx;
    afxNat              fshdIdx;

    afxPrimTopology     primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat              patchControlPoints; /// is the number of control points per patch.
    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE
    afxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool             frontFacingInverted; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxNat              vpCnt; /// the number of viewports used by the pipeline. At least 1.

    //afxRasterizer       razr;
    afxLigature         liga;
    afxVertexInput      vin;
};
#endif//_AVX_PIPELINE_C
#endif//_AVX_DRAW_C

AVX afxPrimTopology     AfxGetPrimitiveTopology(afxPipeline pip);
AVX afxBool             AfxPrimitiveRestartIsEnabled(afxPipeline pip);
AVX afxBool             AfxDepthClampIsEnabled(afxPipeline pip);

AVX afxCullMode         AfxGetPrimitiveCullingMode(afxPipeline pip, afxBool* frontFacingInverted); // return the culling mode set if any.

AVX afxBool             AfxGetPipelineLigature(afxPipeline pip, afxLigature* ligature);

AVX afxBool             AfxGetPipelineVertexInput(afxPipeline pip, afxVertexInput* input);

AVX afxNat              AfxCountShaderSlots(afxPipeline pip);
AVX afxNat              AfxGetLinkedShaders(afxPipeline pip, afxIndex first, afxNat cnt, afxShader shaders[]);
AVX afxBool             AfxGetPipelineStage(afxPipeline pip, afxShaderStage stage, afxShader* shader);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineBlueprint const blueprints[], afxPipeline pipelines[]);

AVX afxPipeline         AfxLoadPipelineFromXsh(afxDrawContext dctx, afxVertexInput vin, afxUri const* uri);

#endif//AVX_PIPELINE_H
