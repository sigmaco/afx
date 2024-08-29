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

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/io/afxUri.h"
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

AFX_DEFINE_STRUCT(avxPipelineBlueprint)
{
    avxPipelineFlags    primFlags;
    avxTopology         primTop; /// is a option defining the primitive topology. /// avxTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat              patchControlPoints; /// is the number of control points per patch.
    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE    
    afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
    avxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// avxCullMode_BACK
    afxBool             invertFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxNat              vpCnt; /// the number of viewports used by the pipeline. At least 1.
    avxVertexInput      vin;

    afxNat              shdCnt;
    avxShader           shd[5];
    avxShaderStage      shdStage[5];
    afxUri              shdUri[5];
    afxString           shdFn[5];
    //avxRasterizer       razr;
    //afxUri              razrUri;
};

AVX avxTopology         AfxGetPrimitiveTopology(avxPipeline pip);
AVX afxBool             AfxPrimitiveRestartIsEnabled(avxPipeline pip);
AVX afxBool             AfxDepthClampIsEnabled(avxPipeline pip);

AVX avxCullMode         AfxGetPrimitiveCullingMode(avxPipeline pip, afxBool* frontFacingInverted); // return the culling mode set if any.

AVX afxBool             AfxGetPipelineLigature(avxPipeline pip, avxLigature* ligature);

AVX afxBool             AfxGetPipelineVertexInput(avxPipeline pip, avxVertexInput* input);

AVX afxNat              AfxCountShaderSlots(avxPipeline pip);
AVX afxNat              AfxGetLinkedShaders(avxPipeline pip, afxIndex first, afxNat cnt, avxShader shaders[]);
AVX afxBool             AfxGetPipelineStage(avxPipeline pip, avxShaderStage stage, avxShader* shader);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, avxPipelineBlueprint const blueprints[], avxPipeline pipelines[]);

AVX avxPipeline         AfxLoadPipelineFromXsh(afxDrawContext dctx, avxVertexInput vin, afxUri const* uri);

#endif//AVX_PIPELINE_H
