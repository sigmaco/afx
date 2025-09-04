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

#ifndef AVX_IMPL___PIPELINE_H
#define AVX_IMPL___PIPELINE_H

#include "qwadro/inc/draw/afxDrawSystem.h"

#ifdef _AVX_LIGATURE_C

AFX_DEFINE_STRUCT(_avxPushRange)
{
    afxUnit16       size;
    afxUnit16       offset;
    afxMask         visibility;
};

AFX_DEFINE_STRUCT(_avxLigament)
{
    afxUnit32       binding; // A unique identifier for a resource binding within the avxLigature, corresponding to a avxLigatureEntry.binding and a @binding attribute in the avxShader.
    //afxUnit         cachedSet;
    afxMask         visibility; // A bitset of the members of avxShaderType. Each set bit indicates that a avxLigatureEntry's resource will be accessible from the associated shader stage.
    avxShaderParam  type;
    afxUnit         cnt;
    afxString16     name;

    union
    {
        struct
        {
            afxBool hasDynOffsets; // if exist it is a push constant block?
            afxUnit minSizeBound;
            afxBool writeable; // if true it is no uniform buffer?
        }           buf;
        struct
        {
            int a;
        }           img;
    };
    afxFlags        flags;
    void*           idd;
};

AFX_DEFINE_STRUCT(_avxLigatureSet)
{
    afxUnit         set;
    afxUnit         baseEntry;
    afxUnit         entryCnt;
    afxFlags        flags;
    afxUnit32       crc32;
    void*           idd;
};
#ifdef _AVX_BIND_SCHEMA_IMPL
AFX_OBJECT(_avxLigature)
#else
AFX_OBJECT(avxLigature)
#endif
{
    afxString       tag;
    void*           udd;
    afxFlags        flags;
    afxUnit         pushCnt;
    _avxPushRange*  pushes;
    afxUnit         totalEntryCnt;
    _avxLigament*   totalEntries; // The map of binding indices pointing to the avxLigatureEntry, which this avxLigature describes.
    afxUnit         setCnt;
    _avxLigatureSet*sets;
};
#endif//_AVX_LIGATURE_C

AFX_DEFINE_STRUCT(avxShaderSlot)
{
    avxShaderType      stage;
    avxShader           shd;
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
    afxString           tag;
    void*               udd;
    avxPipelineType     type;
    afxUnit             stageCnt;
    avxShaderSlot*      stages;    
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
    avxStencilInfo      stencilFront; // is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilBack; // is the configuration controlling the corresponding parameters of the stencil test.

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

#ifdef _AVX_VERTEX_INPUT_C
#ifdef _AVX_VERTEX_DECL_IMPL
AFX_OBJECT(_avxVertexInput)
#else
AFX_OBJECT(avxVertexInput)
#endif
{
    afxFlags        flags;
    afxUnit         totalAttrCnt;
    avxVertexAttr*  attrs;
    afxUnit         srcCnt;
    avxVertexFetch* srcs;
    afxString       tag;
    void*           udd;
};
#endif

#ifdef _AVX_SHADER_C
#ifdef _AVX_SHADER_IMPL
AFX_OBJECT(_avxShader)
#else
AFX_OBJECT(avxShader)
#endif
{
    afxString               tag;
    void*                   udd;
    afxUri128               uri;
    avxShaderType           stage;

    afxUnit                 verMajor;
    afxUnit                 verMinor;
    afxBool                 extended;
    afxByte*                code;
    afxUnit16               codeLen;
    afxString               entry;

    afxString32             pushConstName;
    afxUnit8                resDeclCnt;
    avxShaderResource*      resDecls;

    afxUnit8                ioDeclCnt;
    avxShaderIoChannel*     ioDecls;
    avxTopology             topology;
};
#endif//_AVX_SHADER_C

#ifdef _AVX_QUERY_POOL_C
#ifdef _AVX_QUERY_POOL_IMPL
AFX_OBJECT(_avxQueryPool)
#else
AFX_OBJECT(avxQueryPool)
#endif
{
    afxString       tag;
    void*           udd;
    avxQueryType    type;
    afxUnit         cap;
};
#endif

#ifdef _AVX_CANVAS_C
AFX_DEFINE_STRUCT(_avxCanvasUnit)
{
    avxRaster       ras; // the texture subresource that will be output to for this color attachment.
    avxRaster       resolve; // the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxBool         managed;
    avxFormat       fmt; // the format of the image that will be used for the attachment.
    avxRasterUsage  usage; // additional properties of the attachment.
    avxRasterFlags  flags;
    afxUnit         sampleCnt; // the number of samples of the image.
};
#ifdef _AVX_CANVAS_IMPL
AFX_OBJECT(_avxCanvas)
#else
AFX_OBJECT(avxCanvas)
#endif
{
    afxString       tag;
    void*           udd;
    avxCanvasFlags  flags;
    avxRange        whdMin; // when a user-provided raster are attached
    avxRange        whdMax; // when a user-provided raster are attached
    avxRange        whd; // D is layered
    afxUnit         lodCnt;
    afxUnit         linkedCnt;
    afxUnit         slotCnt; // immutable
    _avxCanvasUnit* slots;
    afxFlags        ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
    afxUnit         colorCnt;
    afxUnit         dsSlotIdx[2];
    afxError        (*readjust)(avxCanvas, afxWarp const);
    afxError        (*relink)(avxCanvas, afxBool, afxUnit, afxUnit, avxRaster[]);
    void*           idd[2];
};
#endif//_AVX_CANVAS_C

#ifdef _AVX_SAMPLER_C
#ifdef _AVX_SAMPLER_IMPL
AFX_OBJECT(_avxSampler)
#else
AFX_OBJECT(avxSampler)
#endif
{
    afxString       tag;
    void*           udd;
    afxUnit32       crc;
    avxSamplerInfo  cfg;
};
#endif

AVX afxClassConfig const _AVX_CANV_CLASS_CONFIG;
AVX afxClassConfig const _AVX_SAMP_CLASS_CONFIG;
AVX afxClassConfig const _AVX_PIP_CLASS_CONFIG;
AVX afxClassConfig const _AVX_SHD_CLASS_CONFIG;
AVX afxClassConfig const _AVX_LIGA_CLASS_CONFIG;
AVX afxClassConfig const _AVX_VIN_CLASS_CONFIG;
AVX afxClassConfig const _AVX_QRYP_CLASS_CONFIG;

AVX afxError    _AvxLoadGlScript(afxStream file, afxArray* fCode);

#endif//AVX_IMPL___PIPELINE_H
