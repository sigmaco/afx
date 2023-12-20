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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

// Desde que surgiu as mesh shaders, somado as experiências com rasterização com CUDA, aparentemente o processo de transformação geométrica das pipelines tende a mudar.
// Assim sendo, a SIGMA decideu quebrar o objeto de estado da pipeline em módulos, e assembleá-los, para evitar lidar com alocação de espaço para coisas pouco utilizadas.
// Esta abordagem é, de longe, a mais arbitrária, nadando contra o recúo de maré do Vulkan, Direct3D 12 e Metal.

#ifndef AFX_PIPELINE_H
#define AFX_PIPELINE_H

#include "qwadro/draw/pipe/afxShader.h"
#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/core/afxResource.h"
#include "qwadro/math/afxViewport.h"
#include "qwadro/draw/pipe/afxRasterizer.h"

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

AFX_DEFINE_STRUCT(afxPipelineAssembler)
{
    int a;
};

//AFX afxError            AfxAssemblePrimitiveRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AfxAssemblePrimitiveShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AwxAssembleMeshShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AwxAssembleMeshRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);

AFX afxError            AfxAssemblePipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
AFX afxPipeline         AfxAssemblePipelineFromXml(afxDrawContext dctx, afxXmlNode const* node);

AFX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineConfig const config[], afxPipeline pip[]);
AFX afxPipeline         AfxLoadPipelineFromXsh(afxDrawContext dctx, afxUri const* uri);

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

#endif//AFX_PIPELINE_H
