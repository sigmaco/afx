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

#ifndef AVX_DRAW_OPS_H
#define AVX_DRAW_OPS_H

#include "qwadro/math/afxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/pipe/afxRasterOps.h"
#include "qwadro/draw/pipe/afxBufferOps.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/io/afxTypography.h"
#include "qwadro/draw/pipe/afxQueryOps.h"
#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/pipe/afxTransformationOps.h"
#include "qwadro/draw/pipe/afxRasterizationOps.h"

typedef enum afxDrawPrefab
{
    afxDrawPrefab_QUAD,

    afxDrawPrefab_TOTAL
} afxDrawPrefab;

AFX_DEFINE_STRUCT(afxDrawIndirectCmd)
/// Structure specifying a indirect drawing command
{
    afxNat32            vtxCnt; /// is the number of vertices to draw.
    afxNat32            instCnt; /// is the number of instances to draw.
    afxNat32            firstVtx; /// is the index of the first vertex to draw.
    afxNat32            firstInst; /// is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(afxDrawIndexedIndirectCmd)
/// Structure specifying a indexed indirect drawing command
{
    afxNat32            idxCnt; /// is the number of vertices to draw.
    afxNat32            instCnt; /// is the number of instances to draw.
    afxNat32            firstIdx;/// is the base index within the index buffer.
    afxInt32            vtxOff; /// is the value added to the vertex index before indexing into the vertex buffer.
    afxNat32            firstInst; /// is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(afxCmdBuf)
{
    afxCmdId(*cpy)(avxCmdb cmdb, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[]);
    afxCmdId(*set)(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, afxNat data);
    afxCmdId(*rw)(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* data);
};

AFX_DEFINE_STRUCT(afxCmdRas)
{
    afxCmdId(*pak)(avxCmdb cmdb, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIo const ops[], afxBool unpack);
    afxCmdId(*cpy)(avxCmdb cmdb, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopy const ops[]);
    afxCmdId(*mip)(avxCmdb cmdb, afxRaster ras, afxNat baseLod, afxNat lodCnt);
    afxCmdId(*xform)(avxCmdb cmdb, afxRaster ras, afxReal const m[4][4], afxNat rgnCnt, afxRasterRegion const rgn[]);
    afxCmdId(*swizzle)(avxCmdb cmdb, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[]);
};

AFX_DEFINE_STRUCT(afxCmdQuery)
{
    afxCmdId(*Begin)(avxCmdb cmdb, afxQueryPool pool, afxNat queryIdx, afxBool precise);
    afxCmdId(*End)(avxCmdb cmdb, afxQueryPool pool, afxNat queryIdx);
    afxCmdId(*CopyResults)(avxCmdb cmdb, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags);
    afxCmdId(*Reset)(avxCmdb cmdb, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt);
    afxCmdId(*WriteTimestamp)(avxCmdb cmdb, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage);
};

AFX_DEFINE_STRUCT(afxCmdRasterization)
{
    afxCmdId(*DisableRasterization)(avxCmdb cmdb, afxBool disable);

    afxCmdId(*EnableDepthBias)(avxCmdb cmdb, afxBool enable);
    afxCmdId(*SetDepthBias)(avxCmdb cmdb, afxReal constFactor, afxReal clamp, afxReal slopeFactor);

    afxCmdId(*SetLineWidth)(avxCmdb cmdb, afxReal lineWidth);

    afxCmdId(*AdjustScissors)(avxCmdb cmdb, afxNat baseIdx, afxNat cnt, afxRect const rects[]);
    afxCmdId(*AdjustCurtains)(avxCmdb cmdb, afxNat baseIdx, afxNat cnt, afxRect const rects[]);

    afxCmdId(*EnableDepthBoundsTest)(avxCmdb cmdb, afxBool enable);
    afxCmdId(*SetDepthBounds)(avxCmdb cmdb, afxReal const bounds[2]);

    afxCmdId(*EnableStencilTest)(avxCmdb cmdb, afxBool enable);
    afxCmdId(*SetStencilCompareMask)(avxCmdb cmdb, afxMask faceMask, afxNat32 compareMask);
    afxCmdId(*SetStencilWriteMask)(avxCmdb cmdb, afxMask faceMask, afxNat32 writeMask);
    afxCmdId(*SetStencilReference)(avxCmdb cmdb, afxMask faceMask, afxNat32 reference);

    afxCmdId(*EnableDepthTest)(avxCmdb cmdb, afxBool enable);
    afxCmdId(*SetDepthCompareOp)(avxCmdb cmdb, afxCompareOp op);
    afxCmdId(*DisableDepthWrite)(avxCmdb cmdb, afxBool enable);

    afxCmdId(*SetBlendConstants)(avxCmdb cmdb, afxReal const blendConstants[4]);

    afxCmdId(*BeginSynthesis)(avxCmdb cmdb, afxSynthesisConfig const*state);
    afxCmdId(*FinishSynthesis)(avxCmdb cmdb);
    afxCmdId(*NextPass)(avxCmdb cmdb, afxBool useAuxStreams);
};

AFX_DEFINE_STRUCT(afxCmdTransformation)
{
    afxCmdId(*BindVertexInput)(avxCmdb cmdb, afxVertexInput vin);

    afxCmdId(*BindVertexSources)(avxCmdb cmdb, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[]);
    afxCmdId(*BindIndexSource)(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz);
    afxCmdId(*SetPrimitiveTopology)(avxCmdb cmdb, afxPrimTopology topology);

    afxCmdId(*AdjustViewports)(avxCmdb cmdb, afxNat baseIdx, afxNat cnt, afxViewport const vp[]);

    afxCmdId(*SwitchFrontFace)(avxCmdb cmdb, afxBool cw);
    afxCmdId(*SetCullMode)(avxCmdb cmdb, afxCullMode mode);
};

AFX_DEFINE_STRUCT(afxCmd)
{
    void*EndOfScript;

    afxCmdId(*ExecuteCommands)(avxCmdb cmdb, afxNat cnt, avxCmdb aux[]);

    afxCmdId(*BindPipeline)(avxCmdb cmdb, afxNat segment, afxPipeline pip, afxFlags dynFlags);
    afxCmdId(*BindRasterizer)(avxCmdb cmdb, afxRasterizer razr, afxFlags dynFlags);

    afxCmdId(*BindBuffers)(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
    afxCmdId(*BindRasters)(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster ras[]);

    // draw
    afxCmdId(*Draw)(avxCmdb cmdb, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx);
    afxCmdId(*DrawIndirect)(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndirectCount)(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexed)(avxCmdb cmdb, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx);
    afxCmdId(*DrawIndexedIndirect)(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexedIndirectCount)(avxCmdb cmdb, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    
    afxCmdRas               ras;
    afxCmdBuf               buf;
    afxCmdQuery             Query;
    afxCmdRasterization     Rasterization;
    afxCmdTransformation    Transformation;

    void* Total;
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Bind a pipeline object to a command buffer.
/// Once bound, a pipeline binding affects subsequent commands that interact with the given pipeline type in the command buffer until a different pipeline of the same type is bound to the bind point, or until the pipeline bind point is disturbed by binding a shader object.
/// Commands that do not interact with the given pipeline type must not be affected by the pipeline state.

AVX afxCmdId                AvxCmdBindPipeline
(
    avxCmdb                 cmdb, /// is the command buffer that the pipeline will be bound to. 
    afxNat                  segment, /// is a value specifying to which level the pipeline is bound. Binding one does not disturb the others.
    afxPipeline             pip, /// is the pipeline to be bound.
    afxFlags                dynamics
);

/// Bind a rasterizer object to a command buffer.
/// Once bound, a rasterizer binding affects subsequent commands that interact with the given rasterization pipeline in the command buffer until a different pipeline of the same type is bound to the bind point, or until the pipeline bind point is disturbed by binding a shader object.
/// Commands that do not interact with the given pipeline type must not be affected by the pipeline state.

AVX afxCmdId                AvxCmdBindRasterizer
(
    avxCmdb                 cmdb, /// is the command buffer that the pipeline will be bound to. 
    afxRasterizer           razr, /// is the rasterizer to be bound.
    afxFlags                dynamics
);

/// Execute a secondary command buffer from a primary command buffer.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary command buffer which is currently in the executable or recording state, that primary command buffer becomes invalid.

/// If the nested command buffer feature is enabled it is valid usage for AvxCmdExecuteCommands to also be recorded to a secondary command buffer.

AVX afxCmdId                AvxCmdExecuteCommands
(
    avxCmdb            cmdb,
    afxNat                  cnt, /// is the length of the @aux array.
    avxCmdb            aux[] /// is a pointer to an array of @cnt secondary command buffer handles, which are recorded to execute in the primary command buffer in the order they are listed in the array.
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AvxCmdDraw
(
    avxCmdb            cmdb,
    afxNat                  baseInstIdx, /// is the instance ID of the first instance to draw.
    afxNat                  instCnt, /// is the number of instances to draw.
    afxNat                  baseVtxIdx, /// is the index of the first vertex to draw.
    afxNat                  vtxCnt /// is the number of vertices to draw.
);

/// Draw primitives with indirect parameters.
/// AvxCmdDrawIndirect behaves similarly to AvxCmdDraw except that the parameters are read by the device from a buffer during execution.
/// @drawCount draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndirectCmd structures.
/// If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId                AvxCmdDrawIndirect
(
    avxCmdb            cmdb,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into @buf where parameters begin.
    afxNat32                drawCnt, /// is the number of draws to execute, and can be zero.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indirect parameters and draw count.
/// AvxCmdDrawIndirectCount behaves similarly to AvxCmdDrawIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId                AvxCmdDrawIndirectCount
(
    avxCmdb            cmdb,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into buffer where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxNat32                cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32                maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indexed vertices. 
/// When the command is executed, primitives are assembled using the current primitive topology and @idxCnt vertices whose indices are retrieved from the index buffer.
/// The index buffer is treated as an array of tightly packed unsigned integers of size defined by the @idxSiz parameter (of AvxCmdBindIndexSource) with which the buffer was bound.

/// The first vertex index is at an offset of @firstIdx × @idxSiz + @offset within the bound index buffer, where @offset is the offset specified by AvxCmdBindIndexSource, and @idxSize is the byte size of the type specified by @idxSiz.
/// Subsequent index values are retrieved from consecutive locations in the index buffer.
/// Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the @idxSiz is 1 or 2) and have @vtxOff added to them, before being supplied as the @vtxIdx value.

/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AvxCmdDrawIndexed
(
    avxCmdb            cmdb,
    afxNat                  vtxOff, /// is the value added to the vertex index before indexing into the vertex buffer.
    afxNat                  baseInstIdx, /// is the instance ID of the first instance to draw.
    afxNat                  instCnt, /// is the number of instances to draw.
    afxNat                  baseIdx, /// is the base index within the index buffer.
    afxNat                  idxCnt /// is the number of vertices to draw.
);

/// Draw primitives with indirect parameters and indexed vertices.
/// AvxCmdDrawIndexedIndirect behaves similarly to AvxCmdDrawIndexed except that the parameters are read by the device from a buffer during execution.
/// @drawCnt draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndexedIndirectCmd structures. If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId                AvxCmdDrawIndexedIndirect
(
    avxCmdb            cmdb,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into buffer where parameters begin.
    afxNat32                drawCnt, /// is the number of draws to execute, and can be zero.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw parameters with indirect parameters, indexed vertices, and draw count.
/// AvxCmdDrawIndexedIndirectCount behaves similarly to AvxCmdDrawIndexedIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId                AvxCmdDrawIndexedIndirectCount
(
    avxCmdb            cmdb,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into @buf where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxNat32                cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32                maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

AVX afxCmdId            AvxCmdBindBuffers(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AVX afxCmdId            AvxCmdBindRasters(avxCmdb cmdb, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[]);

AVX afxCmdId            AvxCmdBindFontSIG(avxCmdb cmdb, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[]);

#endif//AVX_DRAW_OPS_H
