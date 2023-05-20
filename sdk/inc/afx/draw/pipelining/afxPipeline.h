/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// Graphics pipelines consist of multiple shader stages, multiple fixed-function
// pipeline stages, and a pipeline layout.


#ifndef AFX_PIPELINE_H
#define AFX_PIPELINE_H

#include "afx/draw/pipelining/afxShader.h"
#include "afx/draw/pipelining/afxPipelineRig.h"
#include "afx/draw/pipelining/afxSampler.h"
#include "afx/core/io/afxResource.h"
#include "afx/draw/res/afxVertexBuffer.h"

// sgl_frame
// sgl_pass
// sgl_mtl
// sgl_obj

// The descriptor set number 0 will be used for engine-global resources, and bound once per frame.
// The descriptor set number 1 will be used for per-pass resources, and bound once per pass.
// The descriptor set number 2 will be used for material resources, and the number 3 will be used for per-object resources.
// This way, the inner render loops will only be binding descriptor sets 2 and 3, and performance will be high.


/*
The first stage of the graphics pipeline (Input Assembler) assembles vertices to form geometric primitives such as points, lines, and triangles, based on a requested primitive topology.
In the next stage (Vertex Shader) vertices can be transformed, computing positions and attributes for each vertex.
If tessellation and/or geometry shaders are supported, they can then generate multiple primitives from a single input primitive, possibly changing the primitive topology or generating additional attribute data in the process.
The final resulting primitives are clipped to a clip volume in preparation for the next stage, Rasterization. The rasterizer produces a series of fragments associated with a region of the framebuffer, from a two-dimensional description of a point, line segment, or triangle.
These fragments are processed by fragment operations to determine whether generated values will be written to the framebuffer. Fragment shading determines the values to be written to the framebuffer attachments. Framebuffer operations then read and write the color and depth/stencil attachments of the framebuffer for a given subpass of a render pass instance.
The attachments can be used as input attachments in the fragment shader in a later subpass of the same render pass.
*/

typedef enum
{
    _AFX_PIP_STATE_TYPE_ASSEMBLER,
    _AFX_PIP_STATE_TYPE_DEPTH,
    _AFX_PIP_STATE_TYPE_RASTERIZER,
    _AFX_PIP_STATE_TYPE_MULTISAMPLE,
    _AFX_PIP_STATE_TYPE_COLORBLEND,
} afxPipelineStateType;

AFX_DEFINE_STRUCT(afxPipelineInputStream) // vertex attribute input stream
{
    afxNat32            location; // is the shader input location number for this attribute.
    afxNat32            binding; // is the binding number which this attribute takes its data from.
    afxVertexFormat     format; // is the size and type of the vertex attribute data.
    afxNat32            offset; // is a byte offset of this attribute relative to the start of an element in the vertex input binding.
};

AFX_DEFINE_STRUCT(afxPipelineInputAssemblyState)
{
    afxPrimTopology     topology; // AFX_PRIM_TOPOLOGY_TRIANGLE // defines the primitive topology, as described below.
    afxBool             primRestartEnable; // FALSE // controls whether a special vertex index value is treated as restarting the assembly of primitives.
};

AFX_DEFINE_STRUCT(afxViewport)
{
    afxV2d              offset; // x, y
    afxV2d              extent; // w, h
    afxV2d              depth;  // n, f --- min, max range // []{ 0, 1 }
};

AFX_DEFINE_STRUCT(afxPipelineDepthState)
{
    afxBool             depthTestEnable; // FALSE
    afxBool             depthWriteEnable; // FALSE
    afxCompareOp        depthCompareOp; // LESS
    afxBool             depthBoundsTestEnable;
    afxBool             stencilTestEnable;
    struct
    {
        afxStencilOp    failOp;
        afxStencilOp    passOp;
        afxStencilOp    depthFailOp;
        afxCompareOp    compareOp;
    }                   stencilOpFront, stencilOpBack;
    afxReal             minDepthBounds, maxDepthBounds;
};

AFX_DEFINE_STRUCT(afxPipelineRasterizerState)
{
    afxBool             depthClampEnable; // FALSE
    afxBool             rasterizerDiscardEnable; // FALSE
    afxFillMode         fillMode; // AFX_FILL_MODE_SOLID
    afxCullMode         cullMode; // AFX_CULL_MODE_BACK
    afxFrontFace        frontFace; // AFX_FRONT_FACE_CCW
    afxBool             depthBiasEnable; // FALSE
    afxReal             depthBiasConstantFactor; // 0.f
    afxReal             depthBiasClamp; // 0.f
    afxReal             depthBiasSlopeFactor; // 0.f
    afxReal             lineWidth; // 1.f
};

AFX_DEFINE_STRUCT(afxPipelineMultisampleState)
{
    afxNat32            rasterSamples;
    afxBool             sampleShadingEnable;
    afxReal             minSampleShading;
    afxNat32 const      *pSampleMask;
    afxBool             alphaToCoverageEnable;
    afxBool             alphaToOneEnable;
};

AFX_DEFINE_STRUCT(afxColorBlendAnnex)
{
    afxBool             blendEnable;
    afxBlendFactor      srcColorBlendFactor;
    afxBlendFactor      dstColorBlendFactor;
    afxBlendOp          colorBlendOp;
    afxBlendFactor      srcAlphaBlendFactor;
    afxBlendFactor      dstAlphaBlendFactor;
    afxBlendOp          alphaBlendOp;
    afxFlags            colorWriteMask;
};

AFX_DEFINE_STRUCT(afxPipelineColorBlendState)
{
    afxBool             logicOpEnable;
    afxLogicOp          logicOp;
    afxNat              annexCnt;
    afxColorBlendAnnex  annexes[8];
    afxReal             blendConstants[4];
};

AFX_DEFINE_HANDLE(afxPipeline);

AFX void*               AfxPipelineGetContext(afxPipeline pip);
AFX void*               AfxPipelineGetDriver(afxPipeline pip);
AFX void*               AfxPipelineGetDrawSystem(afxPipeline pip);
AFX afxPipelineRig      AfxPipelineGetRig(afxPipeline pip);

AFX afxResult           AfxPipelineForEachColorBlendAnnex(afxPipeline pip, afxResult (*f)(afxColorBlendAnnex const*, void*), void *data);
AFX afxResult           AfxPipelineForEachScissor(afxPipeline pip, afxNat first, afxNat cnt, afxResult (*f)(afxRect const*, void*), void *data);

AFX afxResult           AfxPipelineForEachStage(afxPipeline pip, afxNat first, afxNat cnt, afxResult (*f)(afxShader shd, void *data), void *data);
AFX afxResult           AfxPipelineForEachInputStream(afxPipeline pip, afxNat first, afxNat cnt, afxResult (*f)(afxPipelineInputStream const*, void*), void *data);
AFX afxResult           AfxPipelineForEachViewport(afxPipeline pip, afxNat first, afxNat cnt, afxResult (*f)(afxViewport const*, void*), void *data);

AFX afxNat              AfxPipelineGetScissorCount(afxPipeline pip);
AFX afxResult           AfxPipelineGetScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[]);

AFX afxNat              AfxPipelineGetInputStreamCount(afxPipeline pip);
AFX afxResult           AfxPipelineGetInputStreams(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputStream streams[]);

AFX afxResult           AfxPipelineGetModules(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[]);
AFX afxNat              AfxPipelineGetStageCount(afxPipeline pip);

AFX afxNat              AfxPipelineGetViewportCount(afxPipeline pip);
AFX afxResult           AfxPipelineGetViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[]);

AFX afxBool             AfxPipelineHasColorBlending(afxPipeline pip);
AFX afxBool             AfxPipelineHasDepthHandling(afxPipeline pip);
AFX afxBool             AfxPipelineHasMultisampling(afxPipeline pip);
AFX afxBool             AfxPipelineHasRasterization(afxPipeline pip);
AFX afxBool             AfxPipelineHasInputAssembling(afxPipeline pip);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxPipeline)
{
    AFX_OBJECT(afxResource) res;
};

#endif

#endif//AFX_PIPELINE_H