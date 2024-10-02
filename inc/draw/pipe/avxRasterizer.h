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

#ifndef AVX_RASTERIZER_H
#define AVX_RASTERIZER_H

#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/afxPixel.h"
#include "qwadro/inc/math/afxColor.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/draw/pipe/avxPipeline.h"

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

#define AVX_MAX_SAMPLE_MASKS 32
#define AVX_MAX_COLOR_OUTPUT_CHANNELS 8

typedef enum avxDepthStencilFlag
{
    avxDepthStencilFlag_TEST = AFX_BIT(0),
    avxDepthStencilFlag_COMPARE = AFX_BIT(1),
    avxDepthStencilFlag_DONT_WRITE = AFX_BIT(2),

    avxDepthStencilFlag_STENCIL_OP_F = AFX_BIT(4),
    avxDepthStencilFlag_STENCIL_TEST_F = AFX_BIT(5),
    avxDepthStencilFlag_STENCIL_WRITE_F = AFX_BIT(6),
    avxDepthStencilFlag_STENCIL_REF_F = AFX_BIT(7),
    avxDepthStencilFlag_STENCIL_FAIL_F = AFX_BIT(8),
    avxDepthStencilFlag_STENCIL_DFAIL_F = AFX_BIT(9),
    avxDepthStencilFlag_STENCIL_PASS_F = AFX_BIT(10),

    avxDepthStencilFlag_STENCIL_OP_B = AFX_BIT(11),
    avxDepthStencilFlag_STENCIL_TEST_B = AFX_BIT(12),
    avxDepthStencilFlag_STENCIL_WRITE_B = AFX_BIT(13),
    avxDepthStencilFlag_STENCIL_REF_B = AFX_BIT(14),
    avxDepthStencilFlag_STENCIL_FAIL_B = AFX_BIT(15),
    avxDepthStencilFlag_STENCIL_DFAIL_B = AFX_BIT(16),
    avxDepthStencilFlag_STENCIL_PASS_B = AFX_BIT(17),

    avxDepthStencilFlag_STENCIL_TEST = avxDepthStencilFlag_STENCIL_TEST_F | avxDepthStencilFlag_STENCIL_TEST_B,

    avxDepthStencilFlag_BOUNDS_TEST = AFX_BIT(31) // it is here because it only works with a depth buffer
} avxDepthStencilFlags;

typedef enum avxMultisamplingFlag
{
    avxMultisamplingFlag_SHADING = AFX_BIT(0),
    avxMultisamplingFlag_ALPHA_TO_COVERAGE = AFX_BIT(1),
    avxMultisamplingFlag_ALPHA_TO_ONE = AFX_BIT(2),
} avxMultisamplingFlags;

typedef enum avxRasterizationFlag
{
    avxRasterizationFlag_FILL_MODE = AFX_BIT(0),
    avxRasterizationFlag_LINE_WIDTH = AFX_BIT(1),
    avxRasterizationFlag_DEPTH_BIAS = AFX_BIT(2)
} avxRasterizationFlags;

typedef enum avxColorOutputFlag
{
    avxColorOutputFlag_SCISSOR = AFX_BIT(0),
    avxColorOutputFlag_BLEND_CONSTANTS = AFX_BIT(1),
    avxColorOutputFlag_LOGIC_OP = AFX_BIT(2)
} avxColorOutputFlags;

typedef enum avxRgbaMask
/// Bitmask controlling which components are written to the framebuffer.
/// The color write mask operation is applied regardless of whether blending is enabled.
/// The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
{
    avxRgbaMask_R = AFX_BIT(0), /// specifies that the R value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_G = AFX_BIT(1), /// specifies that the G value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_B = AFX_BIT(2), /// specifies that the B value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_A = AFX_BIT(3), /// specifies that the A value is written to the color attachment for the appropriate sample. Otherwise, the value in memory is unmodified.
    avxRgbaMask_RG = avxRgbaMask_R | avxRgbaMask_G,
    avxRgbaMask_RGB = avxRgbaMask_RG | avxRgbaMask_B,
    avxRgbaMask_RGBA = avxRgbaMask_RGB | avxRgbaMask_A
} avxRgbaMask;

AFX_DEFINE_STRUCT(avxStencilConfig)
/// Structure specifying stencil operation state
{
    avxCompareOp            compareOp; /// is a value specifying the comparison operator used in the stencil test. /// avxCompareOp_ALWAYS
    afxNat32                reference; /// is an integer stencil reference value that is used in the unsigned stencil comparison. /// 0
    afxNat32                compareMask; /// selects the bits of the unsigned integer stencil values participating in the stencil test. // 1
    afxNat32                writeMask; /// selects the bits of the unsigned integer stencil values updated by the stencil test in the stencil framebuffer attachment. /// 1
    avxStencilOp            failOp; /// is a value specifying the action performed on samples that fail the stencil test. /// avxStencilOp_KEEP
    avxStencilOp            depthFailOp; /// is a value specifying the action performed on samples that pass the stencil test and fail the depth test. /// avxStencilOp_KEEP
    avxStencilOp            passOp; /// is a value specifying the action performed on samples that pass both the depth and stencil tests. /// avxStencilOp_KEEP    
};

AFX_DEFINE_STRUCT(avxColorBlendConfig)
/// Describes how the color or alpha components of a fragment are blended
{
    avxBlendOp              rgbBlendOp; /// selects which blend operation is used to calculate the RGB values to write to the color attachment. // ADD
    avxBlendFactor          rgbSrcFactor; /// selects which blend factor is used to determine the source factors (Sr,Sg,Sb). // ONE
    avxBlendFactor          rgbDstFactor; /// selects which blend factor is used to determine the destination factors (Dr,Dg,Db). // ZERO
    avxBlendOp              aBlendOp; /// selects which blend operation is used to calculate the alpha values to write to the color attachment. // ADD
    avxBlendFactor          aSrcFactor; /// selects which blend factor is used to determine the source factor Sa. // ONE
    avxBlendFactor          aDstFactor; /// selects which blend factor is used to determine the destination factor Da. // ZERO
};

AFX_DEFINE_STRUCT(avxColorOutputChannel)
{
    avxFormat               fmt; /// Surface format compatible with pixel output.
    afxBool                 blendEnabled; /// controls whether blending is enabled for the corresponding color attachment. If blending is not enabled, the source fragment’s color for that attachment is passed through unmodified. FALSE
    avxColorBlendConfig     blendConfig; /// Describes how the color or alpha components of a fragment are blended.
    avxRgbaMask             discardMask; /// NIL = write all
};

AFX_DEFINE_STRUCT(avxRasterizationConfig)
{
    avxPipelineBlueprint    xfmrPipb;
    afxUri                  xfmrPipUri;
    //avxShader               fsh;
    //afxUri                  fshUri;
    //afxString               fshFn; // entry point
    //                          fshConstants;
    //                          fshSpecId;

    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;

    // rasterization
    avxFillMode             fillMode; /// is the triangle rendering mode. /// avxFillMode_SOLID
    afxReal                 lineWidth; /// is the width of rasterized line segments. /// 1.f
    // depth bias computation
    afxBool                 depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal                 depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal                 depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal                 depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f
    // multisampling rasterization
    afxBool                 msEnabled;
    afxNat                  sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask                 sampleMasks[AVX_MAX_SAMPLE_MASKS]; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // fragment & pixel output operations

    // scissor test

    //
    afxBool                 alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool                 alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool                 sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal                 minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool                 stencilTestEnabled;
    avxStencilConfig        stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilConfig        stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool                 depthTestEnabled; /// controls whether depth testing is enabled. FALSE
    avxCompareOp            depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. avxCompareOp_LESS
    afxBool                 depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. FALSE
    avxFormat               dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool                 depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled.
    afxV2d                  depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color bending, logical op and color writing
    afxNat                  colorOutCnt;
    avxColorOutputChannel   colorOuts[AVX_MAX_COLOR_OUTPUT_CHANNELS];
    afxColor                blendConstants;
    afxBool                 pixelLogicOpEnabled;
    avxLogicOp              pixelLogicOp;
};

AVX afxBool             AfxGetDepthComparison(avxRasterizer razr, avxCompareOp* op); // return TRUE if depth test is enabled
AVX afxBool             AfxDepthWriteIsDisabled(avxRasterizer razr);
AVX afxBool             AfxGetDepthBiasInfo(avxRasterizer razr, afxReal* slopeScale, afxReal* constFactor, afxReal* clamp); // return TRUE if depth bias is enabled
AVX afxBool             AfxGetDepthBoundsInfo(avxRasterizer razr, afxV2d bounds); // return TRUE if depth bounds is enabled
AVX afxBool             AfxGetStencilConfig(avxRasterizer razr, avxStencilConfig* front, avxStencilConfig* back); // return TRUE if stencil test is enabled
AVX afxBool             AfxGetLogicalPixelOperation(avxRasterizer razr, avxLogicOp* op); // return TRUE if logical pixel operation is enabled
AVX void                AfxGetColorBlendConstants(avxRasterizer razr, afxV4d rgba);
AVX afxNat              AfxGetColorOutputChannels(avxRasterizer razr, afxNat first, afxNat cnt, avxColorOutputChannel ch[]);
AVX afxNat              AfxCountColorOutputChannels(avxRasterizer razr);
AVX afxBool             AfxGetSampleShadingInfo(avxRasterizer razr, afxReal* minSampleShadingValue); // return TRUE if sample shading is enabled
AVX avxFillMode         AfxGetRasterizationMode(avxRasterizer razr);
AVX afxBool             AfxGetLineRasterizationWidth(avxRasterizer razr, afxReal* lineWidth);
AVX afxBool             AfxGetMultisamplingInfo(avxRasterizer razr, afxNat* sampleCnt, afxMask sampleMask[32]);

AVX avxPipeline         AfxGetRasterizerPipeline(avxRasterizer razr);

AVX void                AfxDescribeRasterizerConfiguration(avxRasterizer razr, avxRasterizationConfig* config);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAssembleRasterizers(afxDrawContext dctx, afxNat cnt, avxRasterizationConfig const cfg[], avxRasterizer razr[]);

AVX avxRasterizer       AfxLoadRasterizerFromXsh(afxDrawContext dctx, avxVertexInput vin, afxUri const* uri);

#endif//AVX_RASTERIZER_H
