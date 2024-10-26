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

#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(afxCmd);

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
    avxCmdbState        state;
    afxAtom32           submCnt; /// number of submissions
    afxMask64           submQueMask; /// one for each queue where this cmdb was submitted into.

    afxUnit              portId;
    afxUnit              poolIdx;
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
    avxFormat       fmt; /// the format of the image that will be used for the attachment.
    afxRasterUsage  usage; /// additional properties of the attachment.
    afxRasterFlags  flags;
    afxUnit          sampleCnt; /// the number of samples of the image.
};
#ifdef _AVX_CANVAS_IMPL
AFX_OBJECT(_avxCanvas)
#else
AFX_OBJECT(avxCanvas)
#endif
{
    afxWhd          extent; // D is layered    
    afxWhd          whdMin; // when a user-provided raster are attached
    afxWhd          whdMax; // when a user-provided raster are attached    
    afxUnit          linkedCnt;
    afxUnit          slotCnt; // immutable
    afxRasterSlot*  slots;
    afxFlags        ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
    afxUnit          colorCnt;
    afxUnit          dsSlotIdx[2];
    afxCanvasFlags  flags;
    afxError        (*readjust)(avxCanvas, afxWhd const);
    afxError        (*relink)(avxCanvas,afxBool,afxUnit,afxUnit,afxRaster[]);
    void*           udd[2];
};
#endif//_AVX_CANVAS_C

#ifdef _AVX_LIGATURE_C
AFX_DEFINE_STRUCT(avxLigatureSet)
{
    afxUnit          set;
    afxUnit          baseEntry;
    afxUnit          entryCnt;
    afxUnit32        crc32;
};
#ifdef _AVX_BIND_SCHEMA_IMPL
AFX_OBJECT(_avxLigature)
#else
AFX_OBJECT(avxLigature)
#endif
{
    afxUnit              totalEntryCnt;
    avxLigatureEntry*   totalEntries; // The map of binding indices pointing to the avxLigatureEntry, which this avxLigature describes.

    afxUnit              setCnt;
    avxLigatureSet*     sets;

    afxBool             pushables;
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
    avxPipelineType     type;
    afxUnit              stageCnt;
    avxShaderSlot*      stages;    
    avxLigature         liga;
    
    ///////////////////////////////////
    // TRANSFORMATION PIPELINE BLOCK //
    ///////////////////////////////////
    afxBool             transformationDisabled;

    avxVertexInput      vin;
    avxTopology         primTop; /// is a option defining the primitive topology. /// avxTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    
    afxBool             tesselationEnabled;
    afxUnit              patchControlPoints; /// is the number of control points per patch.
    
    afxUnit              vpCnt; /// the number of viewports used by the pipeline. At least 1.
    avxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// avxCullMode_BACK
    afxBool             frontFacingInverted; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)
    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE

    //////////////////////////////////
    // RASTERIZATION PIPELINE BLOCK //
    //////////////////////////////////
    afxBool             rasterizationDisabled;

    avxDepthStencilFlags    dsFlags;
    avxMultisamplingFlags   msFlags;
    avxRasterizationFlags   rasFlags;
    avxColorOutputFlags     pixelFlags;

    // rasterization
    avxFillMode         fillMode; /// is the triangle rendering mode. /// avxFillMode_SOLID
    afxReal             lineWidth; /// is the width of rasterized line segments. /// 1.f    
    // depth bias computation
    afxBool             depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal             depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal             depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal             depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f

    /// Multisampling is a process for reducing aliasing at the edges of rasterized primitives.
    /// Structure specifying parameters of a pipeline multisample state
    /// Each bit in the sample mask is associated with a unique sample index as defined for the coverage mask.
    /// Each bit b for mask word w in the sample mask corresponds to sample index i, where i = 32 × w + b.
    /// sampleBitmasks has a length equal to sampleCnt / 32 words.
    /// If sampleBitmasks is NULL, it is treated as if the mask has all bits set to 1.

    afxBool             msEnabled; /// If enabld, multisample rasterization will be used. FALSE
    afxUnit              sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask*            sampleMasks; /// an array of sample mask values used in the sample mask test. /// [ 1, ]

    // pixel output operations

    // scissor test

    //
    afxBool             alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE
    afxBool             alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool             sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal             minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f

    // stencil test
    afxBool             stencilTestEnabled; /// FALSE
    avxStencilInfo      stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilInfo      stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.

    // depth test
    afxBool             depthTestEnabled; /// controls whether depth testing is enabled. /// FALSE
    avxCompareOp        depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. /// avxCompareOp_LESS
    afxBool             depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
    avxFormat           dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.
    // depth bounds test
    afxBool             depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled. /// FALSE
    afxV2d              depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]

    // color blending, logical op and writing
    afxUnit              outCnt;
    avxColorOutput*     outs;
    afxColor            blendConstants; /// [ 0, 0, 0, 1 ]
    afxBool             logicOpEnabled; /// FALSE
    avxLogicOp          logicOp; /// avxLogicOp_NOP
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
    afxUnit          cap;
};
#endif

#ifdef _AVX_SAMPLER_C
#ifdef _AVX_SAMPLER_IMPL
AFX_OBJECT(_avxSampler)
#else
AFX_OBJECT(avxSampler)
#endif
{
    afxUnit32                crc;
    avxYuvSamplerInfo     cfg;
    afxBool                 yuv;
};
#endif

#ifdef _AVX_VERTEX_INPUT_C
#ifdef _AVX_VERTEX_INPUT_IMPL
AFX_OBJECT(_avxVertexInput)
#else
AFX_OBJECT(avxVertexInput)
#endif
{
    afxUnit                  attrCnt;
    avxVertexInputAttr*     attrs;
    afxUnit                  streamCnt;
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
    afxUnit          bufCap;
    afxBufferUsage  usage;
    afxBufferFlags  flags;
    avxFormat       fmt; // for FETCH buffer
    void*           udd;

    afxByte*        bytemap;
    afxBool         sysmemBuffered;
    afxSize         mappedOffset;
    afxUnit          mappedRange;
    afxFlags        mappedFlags;
    afxAtom32       pendingRemap;
    afxError        (*remap)(afxBuffer, afxUnit, afxUnit, afxFlags, void**);
};
#endif//_AVX_BUFFER_C

#ifdef _AVX_RASTER_C
AFX_DEFINE_STRUCT(afxSubraster)
{
    afxUnit              baseLod;
    afxUnit              lodCnt;
    afxUnit              baseLayer;
    afxUnit              layerCnt;
    avxFormat           fmt;
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
    avxFormat           fmt;
    afxWhd              extent; // extent of image
    afxUnit              lodCnt; // mip level cnt
    afxUnit              sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.

    afxUnit              subCnt;
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

    afxUnit                  verMajor;
    afxUnit                  verMinor;
    afxBool                 extended;
    afxByte*                code;
    afxUnit16                codeLen;
    afxString               entry;

    afxString32             pushConstName;
    afxUnit8                 resDeclCnt;
    avxShaderResource*      resDecls;

    afxUnit8                 ioDeclCnt;
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


AFX_DEFINE_STRUCT(afxCmdQuery)
{
    afxCmdId(*Begin)(avxCmdb cmdb, avxQueryPool pool, afxUnit queryIdx, afxBool precise);
    afxCmdId(*End)(avxCmdb cmdb, avxQueryPool pool, afxUnit queryIdx);
    afxCmdId(*CopyResults)(avxCmdb cmdb, avxQueryPool pool, afxUnit baseQuery, afxUnit queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags);
    afxCmdId(*Reset)(avxCmdb cmdb, avxQueryPool pool, afxUnit baseQuery, afxUnit queryCnt);
    afxCmdId(*WriteTimestamp)(avxCmdb cmdb, avxQueryPool pool, afxUnit queryIdx, avxPipelineStage stage);
};

AFX_DEFINE_STRUCT(afxCmd)
{
    void*EndOfScript;

    afxCmdId(*ExecuteCommands)(avxCmdb cmdb, afxUnit cnt, avxCmdb aux[]);

    afxCmdId(*BindPipeline)(avxCmdb cmdb, afxUnit segment, avxPipeline pip, avxVertexInput vin, afxFlags dynFlags);
    
    afxCmdId(*BindBuffers)(avxCmdb cmdb, afxUnit set, afxUnit baseIdx, afxUnit cnt, afxBuffer buffers[], afxUnit offset[], afxUnit range[]);
    afxCmdId(*BindRasters)(avxCmdb cmdb, afxUnit set, afxUnit baseIdx, afxUnit cnt, afxRaster rasters[], afxUnit const subIdx[]);
    afxCmdId(*BindSamplers)(avxCmdb cmdb, afxUnit set, afxUnit baseIdx, afxUnit cnt, avxSampler samplers[]);

    // draw
    afxCmdId(*Draw)(avxCmdb cmdb, afxUnit32 vtxCnt, afxUnit32 instCnt, afxUnit32 firstVtx, afxUnit32 firstInst);
    afxCmdId(*DrawIndirect)(avxCmdb cmdb, afxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride);
    afxCmdId(*DrawIndirectCount)(avxCmdb cmdb, afxBuffer buf, afxUnit32 offset, afxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride);
    afxCmdId(*DrawIndexed)(avxCmdb cmdb, afxUnit32 idxCnt, afxUnit32 instCnt, afxUnit32 firstIdx, afxUnit32 vtxOff, afxUnit32 firstInst);
    afxCmdId(*DrawIndexedIndirect)(avxCmdb cmdb, afxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride);
    afxCmdId(*DrawIndexedIndirectCount)(avxCmdb cmdb, afxBuffer buf, afxUnit32 offset, afxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride);

    afxCmdId(*Dispatch)(avxCmdb cmdb, afxUnit32 grpCntX, afxUnit32 grpCntY, afxUnit32 grpCntZ);
    afxCmdId(*DispatchIndirect)(avxCmdb cmdb, afxBuffer buf, afxUnit32 offset);

    struct // raster ops
    {
        afxCmdId(*pak)(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxUnit opCnt, afxRasterIo const ops[], afxBool unpack);
        afxCmdId(*cpy)(avxCmdb cmdb, afxRaster src, afxRaster dst, afxUnit opCnt, afxRasterCopy const ops[]);
        afxCmdId(*mip)(avxCmdb cmdb, afxRaster ras, afxUnit baseLod, afxUnit lodCnt);
        afxCmdId(*xform)(avxCmdb cmdb, afxRaster ras, afxReal const m[4][4], afxUnit rgnCnt, afxRasterRegion const rgn[]);
        afxCmdId(*swizzle)(avxCmdb cmdb, afxRaster ras, avxColorSwizzle a, avxColorSwizzle b, afxUnit rgnCnt, afxRasterRegion const rgn[]);
    } ras;

    struct // buffer ops
    {
        afxCmdId(*cpy)(avxCmdb cmdb, afxBuffer src, afxBuffer dst, afxUnit opCnt, afxBufferCopy const ops[]);
        afxCmdId(*set)(avxCmdb cmdb, afxBuffer buf, afxUnit offset, afxUnit range, afxUnit data);
        afxCmdId(*rw)(avxCmdb cmdb, afxBuffer buf, afxUnit offset, afxUnit range, afxBool toHost, void* data);
    } buf;

    afxCmdId(*PushConstants)(avxCmdb cmdb, afxUnit offset, afxUnit siz, void const* value);

    afxCmdQuery             Query;

    struct
    {
        afxCmdId(*DisableRasterization)(avxCmdb cmdb, afxBool disable);

        afxCmdId(*EnableDepthBias)(avxCmdb cmdb, afxBool enable);
        afxCmdId(*SetDepthBias)(avxCmdb cmdb, afxReal constFactor, afxReal clamp, afxReal slopeFactor);

        afxCmdId(*SetLineWidth)(avxCmdb cmdb, afxReal lineWidth);

        afxCmdId(*AdjustScissors)(avxCmdb cmdb, afxUnit baseIdx, afxUnit cnt, afxRect const rects[]);
        afxCmdId(*AdjustCurtains)(avxCmdb cmdb, afxUnit baseIdx, afxUnit cnt, afxRect const rects[]);

        afxCmdId(*EnableDepthBoundsTest)(avxCmdb cmdb, afxBool enable);
        afxCmdId(*SetDepthBounds)(avxCmdb cmdb, afxReal const bounds[2]);

        afxCmdId(*EnableStencilTest)(avxCmdb cmdb, afxBool enable);
        afxCmdId(*SetStencilCompareMask)(avxCmdb cmdb, afxMask faceMask, afxUnit32 compareMask);
        afxCmdId(*SetStencilWriteMask)(avxCmdb cmdb, afxMask faceMask, afxUnit32 writeMask);
        afxCmdId(*SetStencilReference)(avxCmdb cmdb, afxMask faceMask, afxUnit32 reference);

        afxCmdId(*EnableDepthTest)(avxCmdb cmdb, afxBool enable);
        afxCmdId(*SetDepthCompareOp)(avxCmdb cmdb, avxCompareOp op);
        afxCmdId(*DisableDepthWrite)(avxCmdb cmdb, afxBool enable);

        afxCmdId(*SetBlendConstants)(avxCmdb cmdb, afxReal const blendConstants[4]);

        afxCmdId(*CommenceDrawScope)(avxCmdb cmdb, avxDrawScope const*state);
        afxCmdId(*ConcludeDrawScope)(avxCmdb cmdb);
        afxCmdId(*NextPass)(avxCmdb cmdb, afxBool useAuxStreams);
    } razr;

    struct // vertex transformation ops
    {
        afxCmdId(*BindVertexInput)(avxCmdb cmdb, avxVertexInput vin);

        afxCmdId(*BindVertexSources)(avxCmdb cmdb, afxUnit baseSlotIdx, afxUnit slotCnt, afxBuffer buf[], afxUnit32 const offset[], afxUnit32 const range[], afxUnit32 const stride[]);
        afxCmdId(*BindIndexSource)(avxCmdb cmdb, afxBuffer buf, afxUnit32 offset, afxUnit32 range, afxUnit32 idxSiz);
        afxCmdId(*SetPrimitiveTopology)(avxCmdb cmdb, avxTopology topology);

        afxCmdId(*AdjustViewports)(avxCmdb cmdb, afxUnit baseIdx, afxUnit cnt, afxViewport const vp[]);

        afxCmdId(*SwitchFrontFace)(avxCmdb cmdb, afxBool cw);
        afxCmdId(*SetCullMode)(avxCmdb cmdb, avxCullMode mode);
    } xfrm;

    afxCmdId(*PopDebugScope)(avxCmdb cmdb);
    afxCmdId(*PushDebugScope)(avxCmdb cmdb, afxString const* name, afxColor const color);
    afxCmdId(*InsertDebugLabel)(avxCmdb cmdb, afxString const* name, afxColor const color);

    void* Total;
};

AVX afxError    _AvxLoadGlScript(afxStream file, afxArray* fCode);

#endif//AVX_CONTEXT_IMPL_KIT_H
