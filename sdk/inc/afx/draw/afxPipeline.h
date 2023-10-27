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

// This section is part of SIGMA GL.

// Graphics pipelines consist of multiple shader stages, multiple fixed-function
// pipeline stages, and a pipeline layout.


#ifndef AFX_PIPELINE_H
#define AFX_PIPELINE_H

#include "afx/draw/afxShader.h"
#include "afx/draw/afxPipelineRig.h"
#include "afx/draw/afxSampler.h"
#include "afx/core/afxResource.h"
#include "afx/draw/afxVertexBuffer.h"
#include "afx/math//afxViewport.h"

/*
The first stage of the graphics pipeline (Input Assembler) assembles vertices to form geometric primitives such as points, lines, and triangles, based on a requested primitive topology.
In the next stage (Vertex Shader) vertices can be transformed, computing positions and attributes for each vertex.
If tessellation and/or geometry shaders are supported, they can then generate multiple primitives from a single input primitive, possibly changing the primitive topology or generating additional attribute data in the process.
The final resulting primitives are clipped to a clip volume in preparation for the next stage, Rasterization. The rasterizer produces a series of fragments associated with a region of the framebuffer, from a two-dimensional description of a point, line segment, or triangle.
These fragments are processed by fragment operations to determine whether generated values will be written to the framebuffer. Fragment shading determines the values to be written to the framebuffer attachments. Framebuffer operations then read and write the color and depth/stencil attachments of the framebuffer for a given subpass of a render pass instance.
The attachments can be used as input attachments in the fragment shader in a later subpass of the same render pass.
*/

AFX_DEFINE_STRUCT(afxPipelineInputLocation)
/// vertex attribute input stream
{
    afxNat8             location; /// is the shader input location number for this attribute.
    //afxNat8             binding; /// is the binding number which this attribute takes its data from.
    afxVertexFormat     format; // is the size and type of the vertex attribute data.
    //afxNat32            offset; /// is a byte offset of this attribute relative to the start of an element in the vertex input binding.
};

// Front facing and cull mode should not have effect no point and lines.

AFX_DEFINE_STRUCT(afxStencilConfig)
/// Structure specifying stencil operation state
{
    afxCompareOp    compareOp; /// is a value specifying the comparison operator used in the stencil test. /// afxCompareOp_ALWAYS
    afxNat32        reference; /// is an integer stencil reference value that is used in the unsigned stencil comparison. /// 0
    afxNat32        compareMask; /// selects the bits of the unsigned integer stencil values participating in the stencil test. // 1
    afxNat32        writeMask; /// selects the bits of the unsigned integer stencil values updated by the stencil test in the stencil framebuffer attachment. /// 1
    afxStencilOp    failOp; /// is a value specifying the action performed on samples that fail the stencil test. /// afxStencilOp_KEEP
    afxStencilOp    depthFailOp; /// is a value specifying the action performed on samples that pass the stencil test and fail the depth test. /// afxStencilOp_KEEP
    afxStencilOp    passOp; /// is a value specifying the action performed on samples that pass both the depth and stencil tests. /// afxStencilOp_KEEP    
};

typedef enum afxRgbaBitmask
/// Bitmask controlling which components are written to the framebuffer.
/// The color write mask operation is applied regardless of whether blending is enabled.
/// The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
{
    afxRgbaBitmask_R        = AFX_BIT_OFFSET(0), /// specifies that the R value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaBitmask_G        = AFX_BIT_OFFSET(1), /// specifies that the G value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaBitmask_B        = AFX_BIT_OFFSET(2), /// specifies that the B value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaBitmask_A        = AFX_BIT_OFFSET(3), /// specifies that the A value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaBitmask_RG       = afxRgbaBitmask_R | afxRgbaBitmask_G,
    afxRgbaBitmask_RGB      = afxRgbaBitmask_RG | afxRgbaBitmask_B,
    afxRgbaBitmask_RGBA     = afxRgbaBitmask_RGB | afxRgbaBitmask_A
} afxRgbaBitmask;

AFX_DEFINE_STRUCT(afxColorBlendConfig)
/// Describes how the color or alpha components of a fragment are blended
{
    afxBlendOp              rgbBlendOp; /// selects which blend operation is used to calculate the RGB values to write to the color attachment. // ADD
    afxBlendFactor          rgbSrcFactor; /// selects which blend factor is used to determine the source factors (Sr,Sg,Sb). // ONE
    afxBlendFactor          rgbDstFactor; /// selects which blend factor is used to determine the destination factors (Dr,Dg,Db). // ZERO
    afxBlendOp              aBlendOp; /// selects which blend operation is used to calculate the alpha values to write to the color attachment. // ADD
    afxBlendFactor          aSrcFactor; /// selects which blend factor is used to determine the source factor Sa. // ONE
    afxBlendFactor          aDstFactor; /// selects which blend factor is used to determine the destination factor Da. // ZERO
};

AFX_DEFINE_STRUCT(afxColorOutputChannel)
{
    afxPixelFormat          fmt; /// Surface format compatible with pixel output.
    afxBool                 blendEnabled; /// controls whether blending is enabled for the corresponding color attachment. If blending is not enabled, the source fragment’s color for that attachment is passed through unmodified. FALSE
    afxColorBlendConfig     blendConfig; /// Describes how the color or alpha components of a fragment are blended.
    afxRgbaBitmask          discardMask; /// NIL = write all
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

AFX_DEFINE_STRUCT(afxPipelineConfig)
{
    // vertex
    afxNat                  shdCnt;
    afxShader               sh[6];

    // primitive
    afxPrimTopology         primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool                 primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    // tesselation
    afxNat                  patchControlPoints; /// is the number of control points per patch.

    // clipping
    afxBool                 depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE
    // viewport transform
    afxNat                  vpCnt;
    afxViewport             vps[8];
    // culling
    afxBool                 rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
    afxCullMode             cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool                 cwFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    // rasterization
    afxFillMode             fillMode; /// is the triangle rendering mode. /// afxFillMode_SOLID
    afxReal                 lineWidth; /// is the width of rasterized line segments. /// 1.f
    // depth bias computation
    afxBool                 depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal                 depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal                 depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal                 depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f
    // multisampling rasterization
    afxBool                 msEnabled;
    afxNat                  sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxBitmask              sampleBitmasks[32]; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // fragment & pixel output operations

    // scissor test
    afxNat                  scisCnt;
    afxRect                 scisRects[8];

    //
    afxBool                 alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool                 alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool                 sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal                 minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool                 stencilTestEnabled;
    afxStencilConfig        stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    afxStencilConfig        stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool                 depthTestEnabled; /// controls whether depth testing is enabled. FALSE
    afxCompareOp            depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. afxCompareOp_LESS
    afxBool                 depthWriteEnabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. FALSE
    afxPixelFormat          dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool                 depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled.
    afxV2d                  depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color bending, logical op and color writing
    afxNat                  colorOutCnt;
    afxColorOutputChannel   colorOuts[8];
    afxReal                 blendConstants[4];
    afxBool                 pixelLogicOpEnabled;
    afxLogicOp              pixelLogicOp;
};

#ifdef _AFX_PIPELINE_C
AFX_OBJECT(afxPipeline)
#else
struct afxBasePipeline
#endif
{
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
    afxNat                  vpCnt; /// 0
    afxViewport*            vps;
    // culling
    afxBool                 rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
    afxCullMode             cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool                 cwFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    // rasterization
    afxFillMode             fillMode; /// is the triangle rendering mode. /// afxFillMode_SOLID
    afxReal                 lineWidth; /// is the width of rasterized line segments. /// 1.f    
    // depth bias computation
    afxBool                 depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal                 depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal                 depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal                 depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f

    /// Multisampling is a process for reducing aliasing at the edges of rasterized primitives.
    /// Structure specifying parameters of a pipeline multisample state
    /// Each bit in the sample mask is associated with a unique sample index as defined for the coverage mask.
    /// Each bit b for mask word w in the sample mask corresponds to sample index i, where i = 32 × w + b.
    /// sampleBitmasks has a length equal to sampleCnt / 32 words.
    /// If sampleBitmasks is NULL, it is treated as if the mask has all bits set to 1.

    afxBool                 msEnabled; /// If enabld, multisample rasterization will be used. FALSE
    afxNat                  sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxBitmask*             sampleBitmasks; /// an array of sample mask values used in the sample mask test. /// [ 1, ]
        
    // scissor test
    afxNat                  scissorCnt; /// 0
    afxRect*                scissors;

    //
    afxBool                 alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool                 alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool                 sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal                 minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool                 stencilTestEnabled; /// FALSE
    afxStencilConfig        stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    afxStencilConfig        stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool                 depthTestEnabled; /// controls whether depth testing is enabled. /// FALSE
    afxCompareOp            depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. /// afxCompareOp_LESS
    afxBool                 depthWriteEnabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
    afxPixelFormat          dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool                 depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled. /// FALSE
    afxV2d                  depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color blending, logical op and writing
    afxNat                  outCnt;
    afxColorOutputChannel*  outs;
    afxReal                 blendConstants[4]; /// [ 0, 0, 0, 1 ]
    afxBool                 logicOpEnabled; /// FALSE
    afxLogicOp              logicOp; /// afxLogicOp_NOP
};

AFX_DEFINE_STRUCT(afxPipelineAssembler)
{

};

//AFX afxError            AfxAssemblePrimitiveShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
//AFX afxError            AfxAssembleMeshShadingPipelines2(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);

AFX afxError            AfxAssemblePipelines(afxDrawContext dctx, afxPipelineAssembler const* pipb, afxNat cnt, void* data[], afxPipeline pip[]);
AFX afxPipeline         AfxAssemblePipelineFromXml(afxDrawContext dctx, afxXmlNode const* node);

AFX afxError            AfxBuildPipelines(afxDrawContext dctx, afxNat cnt, afxPipeline pip[], afxPipelineConfig const blueprint[]);
AFX afxPipeline         AfxAssemblePipelineFromXsh(afxDrawContext dctx, afxUri const* uri);

AFX void                AfxGetPrimitiveAssembling(afxPipeline pip, afxPrimTopology* top, afxBool* restartEnabled);
AFX afxBool             AfxGetDepthTest(afxPipeline pip, afxCompareOp* op, afxBool* clampEnabled, afxBool* writeEnabled); // return TRUE if depth test is enabled
AFX afxBool             AfxGetDepthBias(afxPipeline pip, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp); // return TRUE if depth bias is enabled
AFX afxBool             AfxGetDepthBounds(afxPipeline pip, afxReal bounds[2]); // return TRUE if depth bounds is enabled
AFX afxBool             AfxGetStencilConfig(afxPipeline pip, afxStencilConfig* front, afxStencilConfig* back); // return TRUE if stencil test is enabled
AFX afxBool             AfxGetLogicalPixelOp(afxPipeline pip, afxLogicOp* op); // return TRUE if logical pixel operation is enabled
AFX void                AfxGetColorBlendConstants(afxPipeline pip, afxReal rgba[4]);
AFX afxNat              AfxGetColorOutputChannels(afxPipeline pip, afxNat first, afxNat cnt, afxColorOutputChannel ch[]); // return the number of channels obtained.
AFX afxNat              AfxCountColorOutputChannels(afxPipeline pip);
AFX afxBool             AfxGetSampleShading(afxPipeline pip, afxReal* minSampleShadingValue); // return TRUE if multisampling rasterization is enabled
AFX afxCullMode         AfxGetPrimitiveCulling(afxPipeline pip, afxBool* cwFrontFacing); // return the culling mode set if any.
AFX afxBool             AfxGetRasterization(afxPipeline pip, afxFillMode* mode, afxReal* lineWidth); // return TRUE if rasterization enabled.
AFX afxBool             AfxGetRasterizationMultisampling(afxPipeline pip, afxNat* sampleCnt, afxBitmask sampleMask[32]); // return TRUE if multisampling rasterization is enabled

AFX afxNat              AfxCountPipelineWirings(afxPipeline pip);
AFX afxError            AfxGetPipelineWiring(afxPipeline pip, afxNat idx, afxNat *set, afxPipelineRig *legt);

AFX afxNat              AfxCountPipelineScissors(afxPipeline pip);
AFX afxNat              AfxGetPipelineScissors(afxPipeline pip, afxNat first, afxNat cnt, afxRect rect[]);

AFX afxNat              AfxCountPipelineInputs(afxPipeline pip);
AFX afxNat              AfxGetPipelineInputs(afxPipeline pip, afxNat first, afxNat cnt, afxPipelineInputLocation streams[]);

AFX afxNat              AfxCountPipelineShaders(afxPipeline pip);
AFX afxNat              AfxGetPipelineShaders(afxPipeline pip, afxNat first, afxNat cnt, afxShader shd[]);

AFX afxNat              AfxCountPipelineViewports(afxPipeline pip);
AFX afxNat              AfxGetPipelineViewports(afxPipeline pip, afxNat first, afxNat cnt, afxViewport vp[]);

#endif//AFX_PIPELINE_H