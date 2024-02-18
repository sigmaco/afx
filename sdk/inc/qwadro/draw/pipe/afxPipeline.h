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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

// Desde que surgiu as mesh shaders, somado as experiências com rasterização com CUDA, aparentemente o processo de transformação geométrica das pipelines tende a mudar.
// Assim sendo, a SIGMA decideu quebrar o objeto de estado da pipeline em módulos, e assembleá-los, para evitar lidar com alocação de espaço para coisas pouco utilizadas.
// Esta abordagem é, de longe, a mais arbitrária, nadando contra o recúo de maré do Vulkan, Direct3D 12 e Metal.

#ifndef AFX_PIPELINE_H
#define AFX_PIPELINE_H

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
    afxPipelineStage_TOP            = AfxGetBitOffset(0),
    afxPipelineStage_DRAW_INDIRECT  = AfxGetBitOffset(1), /// where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    afxPipelineStage_VERTEX_INPUT   = AfxGetBitOffset(2), /// where vertex and index buffers are consumed.
    afxPipelineStage_VERTEX         = AfxGetBitOffset(3), /// the vertex shader stage.
    afxPipelineStage_DOMAIN         = AfxGetBitOffset(4), /// the tessellation control shader stage.
    afxPipelineStage_HULL           = AfxGetBitOffset(5), /// the tessellation evaluation shader stage.
    afxPipelineStage_PRIM           = AfxGetBitOffset(6), /// the primitive (aka geometry) shader stage.
    afxPipelineStage_FRAGMENT       = AfxGetBitOffset(7), /// the fragment shader stage.
    afxPipelineStage_EARLY_TESTS    = AfxGetBitOffset(8), /// where early fragment tests (depth and stencil tests before fragment shading) are performed. This stage also includes render pass load operations for framebuffer attachments with a depth/stencil format.
    afxPipelineStage_LATE_TESTS     = AfxGetBitOffset(9), /// where late fragment tests (depth and stencil tests after fragment shading) are performed. This stage also includes render pass store operations for framebuffer attachments with a depth/stencil format.
    afxPipelineStage_COLOR_OUTPUT   = AfxGetBitOffset(10), /// where the final color values are output from the pipeline. This stage includes blending, logic operations, render pass load and store operations for color attachments, render pass multisample resolve operations, and AfxCmdClearAttachments.
    afxPipelineStage_COMPUTE        = AfxGetBitOffset(11), /// the execution of a compute shader.
    afxPipelineStage_TRANSFER       = AfxGetBitOffset(12), /// the following commands: copies, blits, resolves, clears.
    afxPipelineStage_BOTTOM         = AfxGetBitOffset(13), 
    afxPipelineStage_HOST           = AfxGetBitOffset(14), /// pseudo-stage indicating execution on the host of reads/writes of device memory. This stage is not invoked by any commands recorded in a command buffer.
    afxPipelineStage_GRAPHICS       = AfxGetBitOffset(15), /// the execution of all graphics pipeline stages.
    afxPipelineStage_COMMANDS       = AfxGetBitOffset(16), /// all operations performed by all commands supported on the queue it is used with.
} afxPipelineStage;

AFX_DEFINE_STRUCT(afxPipelineInputLocation)
/// vertex attribute input stream
{
    afxNat              location; /// is the shader input location number for this attribute.
    //afxNat8             binding; /// is the binding number which this attribute takes its data from.
    afxVertexFormat     fmt; // is the size and type of the vertex attribute data.
    //afxNat32            offset; /// is a byte offset of this attribute relative to the start of an element in the vertex input binding.
    afxNat              stream;
};

typedef enum afxPipelinePrimitiveFlag
{
    afxPipelinePrimitiveFlag_TOPOLOGY       = AfxGetBitOffset(0),
    afxPipelinePrimitiveFlag_RESTART        = AfxGetBitOffset(1),
    afxPipelinePrimitiveFlag_VIEWPORT       = AfxGetBitOffset(2),
    afxPipelinePrimitiveFlag_DEPTH_CLAMP    = AfxGetBitOffset(3),
    afxPipelinePrimitiveFlag_CULL_MODE      = AfxGetBitOffset(4),
    afxPipelinePrimitiveFlag_FRONT_FACE_INV = AfxGetBitOffset(5),
    afxPipelinePrimitiveFlag_CTRL_POINTS    = AfxGetBitOffset(6)
} afxPipelinePrimitiveFlags;

AFX_DEFINE_STRUCT(afxPipelineConfig)
{
    afxPipelinePrimitiveFlags   primFlags;
    afxPrimTopology             primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool                     primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat                      patchControlPoints; /// is the number of control points per patch.
    afxBool                     depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE    
    afxBool                     rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
    afxCullMode                 cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool                     invertFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    
    afxNat                      visCnt;
    struct
    {
        afxNat                  slot;
        afxNat                  instanceRate;
    }                           vis[8];

    afxNat                      shdCnt;
    afxShaderStage              shdStage[4];
    afxUri                      shdUri[4];
    afxString                   shdFn[4];
    afxRasterizer               razr;
    afxUri                      razrUri;
};

#ifdef _AFX_DRAW_C

AFX_DEFINE_STRUCT(afxPipelineModule)
{
    afxFixedUri128          shd;
    afxFixedString8         fn;
    afxShaderStage          stage;
    //constants;
    //                      specId;
};

#ifdef _AFX_PIPELINE_C
#ifndef _AFX_PIPELINE_IMPL
AFX_OBJECT(afxPipeline)
#else
struct afxBasePipeline
#endif
{
    afxPipelinePrimitiveFlags   primFlags;

    afxNat                  stageCnt;
    afxPipelineModule*      stages;

    struct
    {
        afxNat              set;
        afxBindSchema       legt;
    }                      *wiring;
    afxNat                  wiringCnt;

    afxNat                  inCnt;
    afxPipelineInputLocation ins[8];
    afxVertexInput          vin;

    afxPrimTopology         primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool                 primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat                  patchControlPoints; /// is the number of control points per patch.
    afxBool                 depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE
    afxCullMode             cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool                 frontFacingInverted; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    
    afxRasterizer           razr;
};
#endif
#endif

AFX_DEFINE_STRUCT(afxPipelineAssembler)
{
    int a;
};

//AFX afxError            AfxAssemblePrimitiveRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AfxAssemblePrimitiveShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AwxAssembleMeshShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AwxAssembleMeshRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);

//AFX afxError            AfxAssemblePipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxPipeline         AfxAssemblePipelineFromXml(afxDrawContext dctx, afxXmlNode const* node);

AFX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineConfig const config[], afxPipeline pipelines[]);
AFX afxPipeline         AfxAssemblyPipelineFromXsh(afxDrawContext dctx, afxUri const* uri);

AFX afxPrimTopology     AfxGetPrimitiveTopology(afxPipeline pip);
AFX afxBool             AfxPrimitiveRestartIsEnabled(afxPipeline pip);
AFX afxBool             AfxDepthClampIsEnabled(afxPipeline pip);

AFX afxCullMode         AfxGetPrimitiveCullingMode(afxPipeline pip, afxBool* frontFacingInverted); // return the culling mode set if any.

AFX afxNat              AfxCountPipelineBindSchemas(afxPipeline pip);
AFX afxError            AfxGetPipelineBindSchemas(afxPipeline pip, afxNat first, afxNat cnt, afxNat setIdx[], afxBindSchema bsch[]);

AFX afxNat              AfxCountPipelineInputs(afxPipeline pip);
AFX afxNat              AfxGetPipelineInputs(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputLocation streams[]);
AFX afxVertexInput      AfxGetPipelineVertexInput(afxPipeline pip);

AFX afxNat              AfxCountLinkedShaders(afxPipeline pip);
AFX afxNat              AfxGetLinkedShaders(afxPipeline pip, afxNat first, afxNat cnt, afxUri shd[]);
AFX afxBool             AfxFindLinkedShader(afxPipeline pip, afxShaderStage stage, afxUri* shd);

AFX afxRasterizer       AfxGetLinkedRasterizer(afxPipeline pip);

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Bind a pipeline object to a command buffer.
/// Once bound, a pipeline binding affects subsequent commands that interact with the given pipeline type in the command buffer until a different pipeline of the same type is bound to the bind point, or until the pipeline bind point is disturbed by binding a shader object.
/// Commands that do not interact with the given pipeline type must not be affected by the pipeline state.

AFX afxCmdId                AfxCmdBindPipeline
(
    afxDrawScript           dscr, /// is the command buffer that the pipeline will be bound to. 
    afxNat                  segment, /// is a value specifying to which level the pipeline is bound. Binding one does not disturb the others.
    afxPipeline             pip /// is the pipeline to be bound.
);

#endif//AFX_PIPELINE_H
