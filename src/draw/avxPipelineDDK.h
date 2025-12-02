/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_PIPELINE_DDK_H
#define AVX_PIPELINE_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddPip);
AFX_DECLARE_STRUCT(_avxDdiPip);

AFX_DEFINE_STRUCT(_avxProgrammableStage)
{
    avxShaderType       stage;
    // Unique identifier for code unit into the linked avxCodebase.
    afxUnit             progId;
    afxString8          fn;
    //constants;
    //                  specId;
};

#ifdef _AVX_PIPELINE_C
#ifdef _AVX_PIPELINE_IMPL
AFX_OBJECT(_avxPipeline)
#else
AFX_OBJECT(avxPipeline)
#endif
{
    _avxDdiPip const*   ddi;
    _avxIddPip*         idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxFlags            flags;
    avxBus              bus;
    avxCodebase         codb;
    afxUnit             stageCnt;
    _avxProgrammableStage*stages;    
    afxBool             isUserLiga;
    avxLigature         liga;
    afxUnit             specializedWorkGrpSiz[3];

    ///////////////////////////////////
    // TRANSFORMATION PIPELINE BLOCK //
    ///////////////////////////////////
    afxBool             transformationDisabled;

    avxVertexInput      vin;
    avxTopology         primTop; // is a option defining the primitive topology. // avxTopology_TRI_LIST
    afxBool             primRestartEnabled; // controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. // FALSE
    
    afxBool             tesselationEnabled;
    afxUnit             patchControlPoints; // is the number of control points per patch.
    
    afxUnit             vpCnt; // the number of viewports used by the pipeline. At least 1.
    avxCullMode         cullMode; // is the triangle facing direction used for primitive culling. // avxCullMode_BACK
    afxBool             frontFacingInverted; // If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. // FALSE (CCW)
    afxBool             depthClampEnabled; // controls whether to clamp the fragment's depth values as described in Depth Test. // FALSE

    //////////////////////////////////
    // RASTERIZATION PIPELINE BLOCK //
    //////////////////////////////////
    afxBool             rasterizationDisabled;

#if 0
    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;
#endif

    // rasterization
    avxFillMode         fillMode; // is the triangle rendering mode. // avxFillMode_FACE
    afxReal             lineWidth; // is the width of rasterized line segments. // 1.f    
    // depth bias computation
    afxBool             depthBiasEnabled; // controls whether to bias fragment depth values. // FALSE
    afxReal             depthBiasSlopeScale; // is a scalar factor applied to a fragment's slope in depth bias calculations. // 0.f
    afxReal             depthBiasConstFactor; // is a scalar factor controlling the constant depth value added to each fragment. // 0.f
    afxReal             depthBiasClamp; // is the maximum (or minimum) depth bias of a fragment. // 0.f

    // Multisampling is a process for reducing aliasing at the edges of rasterized primitives.
    // Structure specifying parameters of a pipeline multisample state
    // Each bit in the sample mask is associated with a unique sample index as defined for the coverage mask.
    // Each bit b for mask word w in the sample mask corresponds to sample index i, where i = 32 × w + b.
    // sampleBitmasks has a length equal to sampleCnt / 32 words.
    // If sampleBitmasks is NULL, it is treated as if the mask has all bits set to 1.

    afxBool             msEnabled; // If enabld, multisample rasterization will be used. FALSE
    afxUnit             sampleLvl; // is a value specifying the number of samples used in rasterization. // 0
    afxMask*            sampleMasks; // an array of sample mask values used in the sample mask test. // [ 1, ]

    // pixel output operations

    // scissor test

    //
    afxBool             alphaToOneEnabled; // controls whether the alpha component of the fragment's first color output is replaced with one. // FALSE
    afxBool             alphaToCoverageEnabled; // controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. // FALSE
    afxBool             sampleShadingEnabled; // used to enable Sample Shading. // FALSE
    afxReal             minSampleShadingValue; // specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. // 0.f

    // stencil test
    afxBool             stencilTestEnabled; // FALSE
    avxStencilConfig      stencilFront; // is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilConfig      stencilBack; // is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool             depthTestEnabled; // controls whether depth testing is enabled. // FALSE
    avxCompareOp        depthCompareOp; // is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. // avxCompareOp_LESS
    afxBool             depthWriteDisabled; // controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. // FALSE
    avxFormat           dsFmt; // is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool             depthBoundsTestEnabled; // controls whether depth bounds testing is enabled. // FALSE
    afxV2d              depthBounds; // is the minimum depth bound used in the depth bounds test. // [ min, max ]

    // color blending, logical op and writing
    afxUnit             outCnt;
    avxColorOutput*     outs;
    avxColor            blendConstants; // [ 0, 0, 0, 1 ]
    afxBool             logicOpEnabled; // FALSE
    avxLogicOp          logicOp; // avxLogicOp_NOP
};
#endif//_AVX_PIPELINE_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_PIP;

#endif//AVX_PIPELINE_DDK_H
