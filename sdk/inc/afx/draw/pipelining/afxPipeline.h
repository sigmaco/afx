/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// Graphics pipelines consist of multiple shader stages, multiple fixed-function
// pipeline stages, and a pipeline layout.


#ifndef AFX_PIPELINE_H
#define AFX_PIPELINE_H

#include "afx/draw/res/afxShader.h"
#include "afx/draw/pipelining/afxPipelineRig.h"
#include "afx/draw/res/afxSampler.h"
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
    afxNat8             location; // is the shader input location number for this attribute.
    //afxNat8             binding; // is the binding number which this attribute takes its data from.
    afxVertexFormat     format; // is the size and type of the vertex attribute data.
    //afxNat32            offset; // is a byte offset of this attribute relative to the start of an element in the vertex input binding.
};

AFX_DEFINE_STRUCT(afxPipelineInputAssemblyState)
{
    afxPrimTopology     topology; // afxPrimTopology_TRIANGLE --- 0 // defines the primitive topology, as described below.
    afxBool             primRestartEnable; // FALSE --- 1 // controls whether a special vertex index value is treated as restarting the assembly of primitives.
};

AFX_DEFINE_STRUCT(afxViewport)
{
    afxV2d              offset; // x, y
    afxV2d              extent; // w, h
    afxV2d              depth;  // n, f --- min, max range // []{ 0, 1 }
};

AFX_DEFINE_STRUCT(afxPipelineDepthState)
{
    afxBool             depthTestEnable; // FALSE --- 0
    afxBool             depthWriteEnable; // FALSE --- 1
    afxCompareOp        depthCompareOp; // LESS --- 2
    afxBool             depthBoundsTestEnable; // --- 3
    afxBool             stencilTestEnable; // --- 4
    struct
    {
        afxStencilOp    failOp; // --- 7/11
        afxStencilOp    passOp; // --- 8/12
        afxStencilOp    depthFailOp; // --- 9/13
        afxCompareOp    compareOp; // --- 10/14
    }                   stencilOpFront, stencilOpBack;
    afxReal             minDepthBounds; // --- 5
    afxReal             maxDepthBounds; // --- 6
};

AFX_DEFINE_STRUCT(afxPipelineRasterizerState)
{
    afxBool             depthClampEnable; // FALSE --- 0
    afxBool             rasterizerDiscardEnable; // FALSE --- 1
    afxFillMode         fillMode; // afxFillMode_SOLID --- 2
    afxCullMode         cullMode; // afxCullMode_BACK --- 3
    afxFrontFace        frontFace; // afxFrontFace_CCW --- 4
    afxBool             depthBiasEnable; // FALSE --- 5
    afxReal             depthBiasConstantFactor; // 0.f --- 6
    afxReal             depthBiasClamp; // 0.f --- 7
    afxReal             depthBiasSlopeFactor; // 0.f --- 8
    afxReal             lineWidth; // 1.f --- 9
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

AFX_DEFINE_STRUCT(afxPipelineBlueprint)
{
    afxFcc                          fcc; // PIPB
    afxDrawContext                  dctx;

    afxUri128                       uri;

    afxArray                        shaders;
    afxArray                        viewports;
    afxArray                        scissors;

    afxBool                         hasAssembling;
    afxBool                         hasRasterization;
    afxBool                         hasDepthHandling;
    afxBool                         hasMultisampling;
    afxBool                         hasColorBlending;

    afxPipelineInputAssemblyState   inAssembling;
    afxPipelineRasterizerState      rasterization;
    afxPipelineDepthState           depthHandling;
    afxPipelineMultisampleState     multisampling;
    afxPipelineColorBlendState      colorBlending;
};

AFX_DECLARE_STRUCT(_afxPipVmt);

#ifdef _AFX_PIPELINE_C
AFX_OBJECT(afxPipeline)
#else
struct afxBasePipeline
#endif
{
    afxNat                          shaderCnt;
    afxShader*                      shaders;

    struct
    {
        afxNat                      set;
        afxPipelineRig                     legt;
        afxBool                     resolved;
    }                               *wiring;
    afxNat                          wiringCnt;

    afxNat                          inCnt;
    afxPipelineInputStream          ins[8];

    afxBool                         hasVtxInAssembling;
    afxPipelineInputAssemblyState   vtxInAssembling;

    // viewport state --- render area state
    afxNat                          vpCnt;
    afxViewport*                    vps;
    afxNat                          scissorCnt;
    afxRect*                        scissors;

    afxBool                         hasRasterization;
    afxPipelineRasterizerState      rasterization;
    afxBool                         hasMultisampling;
    afxPipelineMultisampleState     multisampling;
    afxBool                         hasDepthHandling;
    afxPipelineDepthState           depthHandling;
    afxBool                         hasColorBlending;
    afxPipelineColorBlendState      colorBlending;
};

AFX afxNat              AfxEnumeratePipelines(afxDrawContext dctx, afxNat first, afxNat cnt, afxPipeline pip[]);

AFX afxError            AfxBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipeline pip[], afxPipelineBlueprint const blueprint[]);
AFX afxError            AfxUploadPipelines(afxDrawContext dctx, afxNat cnt, afxPipeline pip[], afxUri const uri[]);

AFX afxNat              AfxCountPipelineWirings(afxPipeline pip);
AFX afxError            AfxGetPipelineWiring(afxPipeline pip, afxNat idx, afxNat *set, afxPipelineRig *legt);

AFX afxNat              AfxCountPipelineScissors(afxPipeline pip);
AFX afxNat              AfxGetPipelineScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[]);

AFX afxNat              AfxCountPipelineInputStreams(afxPipeline pip);
AFX afxNat              AfxGetPipelineInputStreams(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputStream streams[]);

AFX afxNat              AfxCountPipelineShaders(afxPipeline pip);
AFX afxNat              AfxGetPipelineShaders(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[]);

AFX afxNat              AfxCountPipelineViewports(afxPipeline pip);
AFX afxNat              AfxGetPipelineViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[]);

AFX afxBool             AfxPipelineHasColorBlending(afxPipeline pip);
AFX afxBool             AfxPipelineHasDepthHandling(afxPipeline pip);
AFX afxBool             AfxPipelineHasMultisampling(afxPipeline pip);
AFX afxBool             AfxPipelineHasRasterization(afxPipeline pip);
AFX afxBool             AfxPipelineHasInputAssembling(afxPipeline pip);

AFX afxBool             AfxGetPipelineColorBlending(afxPipeline pip, afxPipelineColorBlendState* state);
AFX afxBool             AfxGetPipelineDepthHandling(afxPipeline pip, afxPipelineDepthState* state);
AFX afxBool             AfxGetPipelineMultisampling(afxPipeline pip, afxPipelineMultisampleState* state);
AFX afxBool             AfxGetPipelineRasterization(afxPipeline pip, afxPipelineRasterizerState* state);
AFX afxBool             AfxGetPipelineInputAssembling(afxPipeline pip, afxPipelineInputAssemblyState* state);

#endif//AFX_PIPELINE_H