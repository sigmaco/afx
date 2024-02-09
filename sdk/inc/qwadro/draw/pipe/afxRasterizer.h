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

#ifndef AFX_RASTERIZER_H
#define AFX_RASTERIZER_H

#include "qwadro/io/afxUri.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/afxColor.h"
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

typedef enum afxRgbaMask
/// Bitmask controlling which components are written to the framebuffer.
/// The color write mask operation is applied regardless of whether blending is enabled.
/// The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
{
    afxRgbaMask_R           = AfxGetBitOffset(0), /// specifies that the R value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaMask_G           = AfxGetBitOffset(1), /// specifies that the G value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaMask_B           = AfxGetBitOffset(2), /// specifies that the B value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaMask_A           = AfxGetBitOffset(3), /// specifies that the A value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    afxRgbaMask_RG          = afxRgbaMask_R | afxRgbaMask_G,
    afxRgbaMask_RGB         = afxRgbaMask_RG | afxRgbaMask_B,
    afxRgbaMask_RGBA        = afxRgbaMask_RGB | afxRgbaMask_A
} afxRgbaMask;

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
    afxRgbaMask             discardMask; /// NIL = write all
};

typedef enum afxDepthStencilFlag
{
    afxDepthStencilFlag_TEST            = AfxGetBitOffset(0),
    afxDepthStencilFlag_COMPARE         = AfxGetBitOffset(1),
    afxDepthStencilFlag_DONT_WRITE      = AfxGetBitOffset(2),
    
    afxDepthStencilFlag_STENCIL_OP_F    = AfxGetBitOffset(4),
    afxDepthStencilFlag_STENCIL_TEST_F  = AfxGetBitOffset(5),
    afxDepthStencilFlag_STENCIL_WRITE_F = AfxGetBitOffset(6),
    afxDepthStencilFlag_STENCIL_REF_F   = AfxGetBitOffset(7),
    afxDepthStencilFlag_STENCIL_FAIL_F  = AfxGetBitOffset(8),
    afxDepthStencilFlag_STENCIL_DFAIL_F = AfxGetBitOffset(9),
    afxDepthStencilFlag_STENCIL_PASS_F  = AfxGetBitOffset(10),

    afxDepthStencilFlag_STENCIL_OP_B    = AfxGetBitOffset(11),
    afxDepthStencilFlag_STENCIL_TEST_B  = AfxGetBitOffset(12),
    afxDepthStencilFlag_STENCIL_WRITE_B = AfxGetBitOffset(13),
    afxDepthStencilFlag_STENCIL_REF_B   = AfxGetBitOffset(14),
    afxDepthStencilFlag_STENCIL_FAIL_B  = AfxGetBitOffset(15),
    afxDepthStencilFlag_STENCIL_DFAIL_B = AfxGetBitOffset(16),
    afxDepthStencilFlag_STENCIL_PASS_B  = AfxGetBitOffset(17),

    afxDepthStencilFlag_STENCIL_TEST    = afxDepthStencilFlag_STENCIL_TEST_F | afxDepthStencilFlag_STENCIL_TEST_B,

    afxDepthStencilFlag_BOUNDS_TEST     = AfxGetBitOffset(31) // it is here because it only works with a depth buffer
} afxDepthStencilFlags;

typedef enum afxMultisamplingFlag
{
    afxMultisamplingFlag_SHADING        = AfxGetBitOffset(0),
    afxMultisamplingFlag_ALPHA_TO_COVERAGE  = AfxGetBitOffset(1),
    afxMultisamplingFlag_ALPHA_TO_ONE   = AfxGetBitOffset(2),
} afxMultisamplingFlags;

typedef enum afxRasterizationFlag
{
    afxRasterizationFlag_FILL_MODE      = AfxGetBitOffset(0),
    afxRasterizationFlag_LINE_WIDTH     = AfxGetBitOffset(1),
    afxRasterizationFlag_DEPTH_BIAS     = AfxGetBitOffset(2)
} afxRasterizationFlags;

typedef enum afxColorOutputFlag
{
    afxColorOutputFlag_SCISSOR          = AfxGetBitOffset(0),
    afxColorOutputFlag_BLEND_CONSTANTS  = AfxGetBitOffset(1),
    afxColorOutputFlag_LOGIC_OP         = AfxGetBitOffset(2)
} afxColorOutputFlags;

AFX_DEFINE_STRUCT(afxRasterizationConfig)
{
    afxDepthStencilFlags        dsFlags;
    afxMultisamplingFlags       msFlags;
    afxRasterizationFlags       rasFlags;
    afxColorOutputFlags         pixelFlags;

    // rasterization
    afxFillMode                 fillMode; /// is the triangle rendering mode. /// afxFillMode_SOLID
    afxReal                     lineWidth; /// is the width of rasterized line segments. /// 1.f
    // depth bias computation
    afxBool                     depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal                     depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal                     depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal                     depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f
    // multisampling rasterization
    afxBool                     msEnabled;
    afxNat                      sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask                     sampleMasks[32]; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // fragment & pixel output operations
    afxUri                      fragShd;
    afxString                   fragFn; // entry point
    //                          fragConstants;
    //                          fragSpecId;

    // scissor test

    //
    afxBool                     alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool                     alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool                     sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal                     minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool                     stencilTestEnabled;
    afxStencilConfig            stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    afxStencilConfig            stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool                     depthTestEnabled; /// controls whether depth testing is enabled. FALSE
    afxCompareOp                depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. afxCompareOp_LESS
    afxBool                     depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. FALSE
    afxPixelFormat              dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool                     depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled.
    afxV2d                      depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color bending, logical op and color writing
    afxNat                      colorOutCnt;
    afxColorOutputChannel       colorOuts[8];
    afxColor                    blendConstants;
    afxBool                     pixelLogicOpEnabled;
    afxLogicOp                  pixelLogicOp;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_RASTERIZER_C
#ifndef _AFX_RASTERIZER_IMPL
AFX_OBJECT(afxRasterizer)
#else
struct afxBaseRasterizer
#endif
{
    afxDepthStencilFlags        dsFlags;
    afxMultisamplingFlags       msFlags;
    afxRasterizationFlags       rasFlags;
    afxColorOutputFlags         pixelFlags;

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
    afxMask*                sampleMasks; /// an array of sample mask values used in the sample mask test. /// [ 1, ]
    
    // fragment & pixel output operations
    afxFixedUri128          fragShd;
    afxFixedString8         fragFn;
    //                      fragConstants;
    //                      fragSpecId;

    // scissor test

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
    afxBool                 depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
    afxPixelFormat          dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool                 depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled. /// FALSE
    afxV2d                  depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color blending, logical op and writing
    afxNat                  outCnt;
    afxColorOutputChannel*  outs;
    afxColor                blendConstants; /// [ 0, 0, 0, 1 ]
    afxBool                 logicOpEnabled; /// FALSE
    afxLogicOp              logicOp; /// afxLogicOp_NOP
};
#endif
#endif

AFX afxError            AfxAcquireRasterizers(afxDrawContext dctx, afxNat cnt, afxRasterizationConfig const config[], afxRasterizer razr[]);
AFX afxRasterizer       AfxLoadRasterizerFromXsh(afxDrawContext dctx, afxUri const* uri);

AFX afxBool             AfxGetDepthComparator(afxRasterizer razr, afxCompareOp* op); // return TRUE if depth test is enabled
AFX afxBool             AfxDepthWriteIsDisabled(afxRasterizer razr);
AFX afxBool             AfxGetDepthBiasInfo(afxRasterizer razr, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp); // return TRUE if depth bias is enabled
AFX afxBool             AfxGetDepthBoundsInfo(afxRasterizer razr, afxReal bounds[2]); // return TRUE if depth bounds is enabled
AFX afxBool             AfxGetStencilConfig(afxRasterizer razr, afxStencilConfig* front, afxStencilConfig* back); // return TRUE if stencil test is enabled
AFX afxBool             AfxGetLogicalPixelOperation(afxRasterizer razr, afxLogicOp* op); // return TRUE if logical pixel operation is enabled
AFX void                AfxGetColorBlendConstants(afxRasterizer razr, afxReal rgba[4]);
AFX afxNat              AfxGetColorOutputChannels(afxRasterizer razr, afxNat first, afxNat cnt, afxColorOutputChannel ch[]);
AFX afxNat              AfxCountColorOutputChannels(afxRasterizer razr);
AFX afxBool             AfxGetMinimumSampleShadingValue(afxRasterizer razr, afxReal* minSampleShadingValue); // return TRUE if sample shading is enabled
AFX afxFillMode         AfxGetRasterizationMode(afxRasterizer razr);
AFX afxBool             AfxGetLineRasterizationWidth(afxRasterizer razr, afxReal* lineWidth);
AFX afxBool             AfxGetMultisamplingInfo(afxRasterizer razr, afxNat* sampleCnt, afxMask sampleMask[32]);

AFX void                AfxGetFragmentShader(afxRasterizer razr, afxUri* uri, afxString* fn);

AFX void                AfxDescribeRasterizerConfiguration(afxRasterizer razr, afxRasterizationConfig* config);

#endif//AFX_RASTERIZER_H
