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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_CONTEXT_IMPL_KIT_H
#define AVX_CONTEXT_IMPL_KIT_H

#include "../dev/afxDevCoreBase.h"
#include "qwadro/draw/dev/afxDrawSystem.h"

#ifdef _AVX_CMD_BUFFER_C
#ifdef _AVX_CMD_BUFFER_IMPL
AFX_OBJECT(_avxCmdb)
#else
AFX_OBJECT(avxCmdb)
#endif
{
    afxError            (*beginCb)(avxCmdb, afxBool permanent);
    afxError            (*endCb)(avxCmdb);
    afxError            (*resetCb)(avxCmdb);
    avxCmdbState   state;
    afxAtom32           submCnt; /// number of submissions
    afxMask64           submQueMask; /// one for each queue where this cmdb was submitted into.

    afxNat              portIdx;
    afxNat              poolIdx;
    afxArena            cmdArena; /// owned by dctx data for specific port
    afxBool             disposable; /// if true, at execution end, it is moved to invalid state and considered in recycle chain.

    afxBool             inRenderPass;
    afxBool             inVideoCoding;

    afxCmd const*       stdCmds;
};
#endif//_AVX_CMD_BUFFER_C

#ifdef _AVX_CANVAS_C
AFX_DEFINE_STRUCT(afxRasterSlot)
{
    afxRaster       ras; /// the texture subresource that will be output to for this color attachment.
    afxRaster       resolve; /// the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxBool         managed;
    afxPixelFormat  fmt; /// the format of the image that will be used for the attachment.
    afxRasterUsage  usage; /// additional properties of the attachment.
    afxRasterFlags  flags;
    afxNat          sampleCnt; /// the number of samples of the image.
};
#ifdef _AVX_CANVAS_IMPL
AFX_OBJECT(_avxCanvas)
#else
AFX_OBJECT(avxCanvas)
#endif
{
    afxWhd          whd; // D is layered    
    afxWhd          whdMin; // when a user-provided raster are attached
    afxWhd          whdMax; // when a user-provided raster are attached    
    afxNat          linkedCnt;
    afxNat          slotCnt; // immutable
    afxRasterSlot*  slots;
    afxFlags        ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
    afxNat          colorCnt;
    afxNat          dsSlotIdx[2];
    afxCanvasFlags  flags;
    afxError        (*readjust)(avxCanvas, afxWhd const);
    afxError        (*relink)(avxCanvas,afxBool,afxNat,afxNat,afxRaster[]);
    void*           udd[2];
};
#endif//_AVX_CANVAS_C

#ifdef _AVX_LIGATURE_C
AFX_DEFINE_STRUCT(avxLigatureSet)
{
    afxNat          set;
    afxNat          baseEntry;
    afxNat          entryCnt;
    afxNat32        crc32;
};
#ifdef _AVX_BIND_SCHEMA_IMPL
AFX_OBJECT(_avxLigature)
#else
AFX_OBJECT(avxLigature)
#endif
{
    afxNat              totalEntryCnt;
    avxLigatureEntry*   totalEntries; // The map of binding indices pointing to the avxLigatureEntry, which this avxLigature describes.

    afxNat              setCnt;
    avxLigatureSet*     sets;
};
#endif//_AVX_LIGATURE_C

AFX_DEFINE_STRUCT(avxShaderSlot)
{
    avxShaderStage      stage;
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
    afxNat              stageCnt;
    avxShaderSlot*      stages;
    afxNat              vtxShdIdx;
    afxNat              hulShdIdx;
    afxNat              domShdIdx;
    afxNat              geoShdIdx;
    afxNat              fshdIdx;

    avxTopology         primTop; /// is a option defining the primitive topology. /// avxTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat              patchControlPoints; /// is the number of control points per patch.
    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE
    avxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// avxCullMode_BACK
    afxBool             frontFacingInverted; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxNat              vpCnt; /// the number of viewports used by the pipeline. At least 1.

    //avxRasterizer       razr;
    avxLigature         liga;
    avxVertexInput      vin;
};
#endif//_AVX_PIPELINE_C

#ifdef _AVX_QUERY_POOL_C
#ifdef _AVX_QUERY_POOL_IMPL
AFX_OBJECT(_avxQueryPool)
#else
AFX_OBJECT(avxQueryPool)
#endif
{
    afxQueryType    type;
    afxNat          cap;
};
#endif

#ifdef _AVX_SAMPLER_C
#ifdef _AVX_SAMPLER_IMPL
AFX_OBJECT(_avxSampler)
#else
AFX_OBJECT(avxSampler)
#endif
{
    afxNat32                crc;
    avxYuvSamplerConfig     cfg;
    afxBool                 yuv;
};
#endif

#ifdef _AVX_RASTERIZER_C
#ifdef _AVX_RASTERIZER_IMPL
AFX_OBJECT(_avxRasterizer)
#else
AFX_OBJECT(avxRasterizer)
#endif
{
    avxPipeline             pip;

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

    /// Multisampling is a process for reducing aliasing at the edges of rasterized primitives.
    /// Structure specifying parameters of a pipeline multisample state
    /// Each bit in the sample mask is associated with a unique sample index as defined for the coverage mask.
    /// Each bit b for mask word w in the sample mask corresponds to sample index i, where i = 32 × w + b.
    /// sampleBitmasks has a length equal to sampleCnt / 32 words.
    /// If sampleBitmasks is NULL, it is treated as if the mask has all bits set to 1.

    afxBool                 msEnabled; /// If enabld, multisample rasterization will be used. FALSE
    afxNat                  sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask*                sampleMasks; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // pixel output operations

    // scissor test

    //
    afxBool                 alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool                 alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool                 sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal                 minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool                 stencilTestEnabled; /// FALSE
    avxStencilConfig        stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilConfig        stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool                 depthTestEnabled; /// controls whether depth testing is enabled. /// FALSE
    avxCompareOp            depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. /// avxCompareOp_LESS
    afxBool                 depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
    afxPixelFormat          dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool                 depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled. /// FALSE
    afxV2d                  depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color blending, logical op and writing
    afxNat                  outCnt;
    avxColorOutputChannel*  outs;
    afxColor                blendConstants; /// [ 0, 0, 0, 1 ]
    afxBool                 logicOpEnabled; /// FALSE
    avxLogicOp              logicOp; /// avxLogicOp_NOP
};
#endif//_AVX_RASTERIZER_C

#ifdef _AVX_VERTEX_INPUT_C
#ifdef _AVX_VERTEX_INPUT_IMPL
AFX_OBJECT(_avxVertexInput)
#else
AFX_OBJECT(avxVertexInput)
#endif
{
    afxNat                  attrCnt;
    avxVertexInputAttr*     attrs;
    afxNat                  streamCnt;
    avxVertexInputStream*   streams;
};
#endif

#ifdef _AVX_BUFFER_C
#ifdef _AVX_BUFFER_IMPL
AFX_OBJECT(_avxBuffer)
#else
AFX_OBJECT(afxBuffer)
#endif
{
    afxNat          cap;
    afxBufferUsage  usage;
    afxBufferAccess access;
    afxByte*        bytemap;
    void*           udd;

    afxSize         mappedOffset;
    afxNat          mappedRange;
    afxFlags        mappedFlags;
    afxAtom32       pendingRemap;
    afxError        (*remap)(afxBuffer, afxNat, afxNat, afxFlags, void**);
    afxError        (*unmap)(afxBuffer, afxBool);
};
#endif//_AVX_BUFFER_C

#ifdef _AVX_RASTER_C
AFX_DEFINE_STRUCT(afxSubraster)
{
    afxNat              baseLod;
    afxNat              lodCnt;
    afxNat              baseLayer;
    afxNat              layerCnt;
    afxPixelFormat      fmt;
    avxColorSwizzling   swizzling;
    afxRasterFlags      flags;
};
#ifdef _AVX_RASTER_IMPL
AFX_OBJECT(_avxRaster)
#else
AFX_OBJECT(afxRaster)
#endif
{
    afxRasterFlags      flags;
    afxRasterUsage      usage;
    void*               udd;
    afxPixelFormat      fmt;
    afxWhd              whd; // extent of image
    afxNat              lodCnt; // mip level cnt
    afxNat              sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.

    afxNat              subCnt;
    union
    {
        afxSubraster    sub0;
        afxSubraster*   subs;
    };
};
#endif

#ifdef _AVX_SHADER_C
#ifdef _AVX_SHADER_IMPL
AFX_OBJECT(_avxShader)
#else
AFX_OBJECT(avxShader)
#endif
{
    afxUri128               uri;
    avxShaderStage          stage;

    afxNat                  verMajor;
    afxNat                  verMinor;
    afxBool                 extended;
    afxByte*                code;
    afxNat16                codeLen;
    afxString               entry;

    afxNat8                 resDeclCnt;
    avxShaderResource*      resDecls;

    afxNat8                 ioDeclCnt;
    avxShaderIoChannel*     ioDecls;
    avxTopology             topology;
};
#endif//_AVX_SHADER_C

AVX afxClassConfig const _AvxCmdbStdImplementation;
AVX afxClassConfig const _AvxCanvStdImplementation;
AVX afxClassConfig const _AvxPipStdImplementation;
AVX afxClassConfig const _AvxSampStdImplementation;
AVX afxClassConfig const _AvxShdStdImplementation;
AVX afxClassConfig const _AvxRasStdImplementation;
AVX afxClassConfig const _AvxBufStdImplementation;
AVX afxClassConfig const _AvxRazrStdImplementation;
AVX afxClassConfig const _AvxLigaStdImplementation;
AVX afxClassConfig const _AvxVinStdImplementation;
AVX afxClassConfig const _AvxQrypStdImplementation;

#endif//AVX_CONTEXT_IMPL_KIT_H
