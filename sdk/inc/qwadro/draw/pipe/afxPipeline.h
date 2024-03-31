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
    afxPipelineStage_TOP            = AFX_BIT_OFFSET(0),
    afxPipelineStage_DRAW_INDIRECT  = AFX_BIT_OFFSET(1), /// where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    afxPipelineStage_VERTEX_INPUT   = AFX_BIT_OFFSET(2), /// where vertex and index buffers are consumed.
    afxPipelineStage_VERTEX         = AFX_BIT_OFFSET(3), /// the vertex shader stage.
    afxPipelineStage_DOMAIN         = AFX_BIT_OFFSET(4), /// the tessellation control shader stage.
    afxPipelineStage_HULL           = AFX_BIT_OFFSET(5), /// the tessellation evaluation shader stage.
    afxPipelineStage_PRIM           = AFX_BIT_OFFSET(6), /// the primitive (aka geometry) shader stage.
    afxPipelineStage_FRAGMENT       = AFX_BIT_OFFSET(7), /// the fragment shader stage.
    afxPipelineStage_EARLY_TESTS    = AFX_BIT_OFFSET(8), /// where early fragment tests (depth and stencil tests before fragment shading) are performed. This stage also includes render pass load operations for framebuffer attachments with a depth/stencil format.
    afxPipelineStage_LATE_TESTS     = AFX_BIT_OFFSET(9), /// where late fragment tests (depth and stencil tests after fragment shading) are performed. This stage also includes render pass store operations for framebuffer attachments with a depth/stencil format.
    afxPipelineStage_COLOR_OUTPUT   = AFX_BIT_OFFSET(10), /// where the final color values are output from the pipeline. This stage includes blending, logic operations, render pass load and store operations for color attachments, render pass multisample resolve operations, and AfxCmdClearAttachments.
    afxPipelineStage_COMPUTE        = AFX_BIT_OFFSET(11), /// the execution of a compute shader.
    afxPipelineStage_TRANSFER       = AFX_BIT_OFFSET(12), /// the following commands: copies, blits, resolves, clears.
    afxPipelineStage_BOTTOM         = AFX_BIT_OFFSET(13), 
    afxPipelineStage_HOST           = AFX_BIT_OFFSET(14), /// pseudo-stage indicating execution on the host of reads/writes of device memory. This stage is not invoked by any commands recorded in a command buffer.
    afxPipelineStage_GRAPHICS       = AFX_BIT_OFFSET(15), /// the execution of all graphics pipeline stages.
    afxPipelineStage_COMMANDS       = AFX_BIT_OFFSET(16), /// all operations performed by all commands supported on the queue it is used with.
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
    afxPipelinePrimitiveFlag_TOPOLOGY       = AFX_BIT_OFFSET(0),
    afxPipelinePrimitiveFlag_RESTART        = AFX_BIT_OFFSET(1),
    afxPipelinePrimitiveFlag_VIEWPORT       = AFX_BIT_OFFSET(2),
    afxPipelinePrimitiveFlag_DEPTH_CLAMP    = AFX_BIT_OFFSET(3),
    afxPipelinePrimitiveFlag_CULL_MODE      = AFX_BIT_OFFSET(4),
    afxPipelinePrimitiveFlag_FRONT_FACE_INV = AFX_BIT_OFFSET(5),
    afxPipelinePrimitiveFlag_CTRL_POINTS    = AFX_BIT_OFFSET(6)
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
    
    afxVertexInput              vin;
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
    afxUri128          shd;
    afxString8         fn;
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

AVX afxPrimTopology     AfxGetPrimitiveTopology(afxPipeline pip);
AVX afxBool             AfxPrimitiveRestartIsEnabled(afxPipeline pip);
AVX afxBool             AfxDepthClampIsEnabled(afxPipeline pip);

AVX afxCullMode         AfxGetPrimitiveCullingMode(afxPipeline pip, afxBool* frontFacingInverted); // return the culling mode set if any.

AVX afxNat              AfxCountPipelineBindSchemas(afxPipeline pip);
AVX afxError            AfxGetPipelineBindSchemas(afxPipeline pip, afxNat first, afxNat cnt, afxNat setIdx[], afxBindSchema bsch[]);

AVX afxNat              AfxCountPipelineInputs(afxPipeline pip);
AVX afxNat              AfxGetPipelineInputs(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputLocation streams[]);
AVX afxVertexInput      AfxGetPipelineVertexInput(afxPipeline pip);

AVX afxNat              AfxCountLinkedShaders(afxPipeline pip);
AVX afxNat              AfxGetLinkedShaders(afxPipeline pip, afxNat first, afxNat cnt, afxUri shd[]);
AVX afxBool             AfxFindLinkedShader(afxPipeline pip, afxShaderStage stage, afxUri* shd);

AVX afxRasterizer       AfxGetLinkedRasterizer(afxPipeline pip);

////////////////////////////////////////////////////////////////////////////////

//AVX afxError            AfxAssemblePrimitiveRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AVX afxError            AfxAssemblePrimitiveShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AVX afxError            AwxAssembleMeshShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AVX afxError            AwxAssembleMeshRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);

//AVX afxError            AfxAssemblePipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AVX afxPipeline         AfxAssemblePipelineFromXml(afxDrawContext dctx, afxXmlNode const* node);

AVX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineConfig const config[], afxPipeline pipelines[]);
AVX afxPipeline         AfxAssemblePipelineFromXsh(afxDrawContext dctx, afxVertexInput vin, afxUri const* uri);

#endif//AFX_PIPELINE_H
