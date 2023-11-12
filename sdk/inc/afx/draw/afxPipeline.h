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

#include "afx/draw/afxShader.h"
#include "afx/draw/afxPipelineRig.h"
#include "afx/draw/afxSampler.h"
#include "afx/core/afxResource.h"
#include "afx/math//afxViewport.h"
#include "afx/draw/afxRasterizer.h"

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
    afxNat8             location; /// is the shader input location number for this attribute.
    //afxNat8             binding; /// is the binding number which this attribute takes its data from.
    afxVertexFormat     fmt; // is the size and type of the vertex attribute data.
    //afxNat32            offset; /// is a byte offset of this attribute relative to the start of an element in the vertex input binding.
    afxNat              stream;
};

AFX_DEFINE_STRUCT(afxVertexInputChannel)
{
    afxNat                  location;
    afxVertexFormat         fmt;
    afxNat                  streamIdx;
    afxNat32                offset;
};

AFX_DEFINE_STRUCT(afxVertexInputStream)
{
    afxNat32                stride;
    afxBool                 instance;
    afxNat                  chCnt;
    afxVertexInputChannel   ch[8];
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
    afxShader                   shd[8];
    afxUri                      shdUri[8];
    afxRasterizer               ras;
    afxUri                      rasUri;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_PIPELINE_C
AFX_OBJECT(afxPipeline)
#else
struct afxBasePipeline
#endif
{
    afxPipelinePrimitiveFlags   primFlags;

    afxNat                  shaderCnt;
    afxShader*              shaders;

    struct
    {
        afxNat              set;
        afxPipelineRig      legt;
        afxBool             resolved;
    }                       *wiring;
    afxNat                  wiringCnt;

    afxNat                  inCnt;
    afxPipelineInputLocation ins[8];

    // primitive
    afxPrimTopology         primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool                 primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    // tesselation
    afxNat                  patchControlPoints; /// is the number of control points per patch.
    // clamping
    afxBool                 depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE
    // viewport transform
    //afxNat                  vpCnt; /// 0
    //afxViewport*            vps;
    // culling

    afxCullMode             cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool                 frontFacingInverted; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxRasterizer           ras;
};
#endif

AFX_DEFINE_STRUCT(afxPipelineAssembler)
{
    int a;
};

//AFX afxError            AfxAssemblePrimitiveRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AfxAssemblePrimitiveShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AfxAssembleMeshShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AfxAssembleMeshRasterizationPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);

AFX afxError            AfxAssemblePipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
AFX afxPipeline         AfxAssemblePipelineFromXml(afxDrawContext dctx, afxXmlNode const* node);

AFX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxNat cnt, afxPipelineConfig const config[], afxPipeline pip[]);
AFX afxPipeline         AfxLoadPipelineFromXsh(afxDrawContext dctx, afxUri const* uri);

AFX afxPrimTopology     AfxGetPrimitiveTopology(afxPipeline pip);
AFX afxBool             AfxPrimitiveRestartIsEnabled(afxPipeline pip);
AFX afxBool             AfxDepthClampIsEnabled(afxPipeline pip);

AFX afxCullMode         AfxGetPrimitiveCullingMode(afxPipeline pip, afxBool* frontFacingInverted); // return the culling mode set if any.

AFX afxNat              AfxCountPipelineWirings(afxPipeline pip);
AFX afxError            AfxGetPipelineWiring(afxPipeline pip, afxNat idx, afxNat *set, afxPipelineRig *legt);

AFX afxNat              AfxCountPipelineInputs(afxPipeline pip);
AFX afxNat              AfxGetPipelineInputs(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputLocation streams[]);

AFX afxNat              AfxCountLinkedShaders(afxPipeline pip);
AFX afxNat              AfxGetLinkedShaders(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[]);
AFX afxBool             AfxFindLinkedShader(afxPipeline pip, afxShaderStage stage, afxShader* shd);

#endif//AFX_PIPELINE_H