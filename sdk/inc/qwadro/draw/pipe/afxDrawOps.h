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

#ifndef AFX_DRAW_OPS_H
#define AFX_DRAW_OPS_H

#include "qwadro/math/afxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/pipe/afxRasterOps.h"
#include "qwadro/draw/pipe/afxBufferOps.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/font/afxTypography.h"
#include "qwadro/draw/pipe/afxQueryOps.h"
#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/pipe/afxTransformOps.h"
#include "qwadro/draw/pipe/afxImagingOps.h"

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

AFX_DEFINE_STRUCT(afxCmdBuffer)
{
    afxCmdId(*cpy)(afxDrawStream dscr, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[]);
    afxCmdId(*set)(afxDrawStream dscr, afxBuffer buf, afxNat offset, afxNat range, afxNat data);
    afxCmdId(*rw)(afxDrawStream dscr, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* data);
    afxCmdId(*io)(afxDrawStream dscr, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxBool toHost, afxStream io);
};

AFX_DEFINE_STRUCT(afxCmdRaster)
{
    afxCmdId(*rw)(afxDrawStream dscr, afxRaster ras, afxStream ios, afxNat opCnt, afxRasterIoOp const ops[], afxBool down, afxCodec cdc);
    afxCmdId(*pak)(afxDrawStream dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[], afxBool unpack);
    afxCmdId(*cpy)(afxDrawStream dscr, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[]);
    afxCmdId(*mip)(afxDrawStream dscr, afxRaster ras, afxNat baseLod, afxNat lodCnt);
    afxCmdId(*xform)(afxDrawStream dscr, afxRaster ras, afxReal const m[4][4], afxNat rgnCnt, afxRasterRegion const rgn[]);
    afxCmdId(*swizzle)(afxDrawStream dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[]);
};

AFX_DEFINE_STRUCT(afxCmdQuery)
{
    afxCmdId(*Begin)(afxDrawStream dscr, afxQueryPool pool, afxNat queryIdx, afxBool precise);
    afxCmdId(*End)(afxDrawStream dscr, afxQueryPool pool, afxNat queryIdx);
    afxCmdId(*CopyResults)(afxDrawStream dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags);
    afxCmdId(*Reset)(afxDrawStream dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt);
    afxCmdId(*WriteTimestamp)(afxDrawStream dscr, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage);
};

AFX_DEFINE_STRUCT(afxCmdRasterization)
{
    afxCmdId(*DisableRasterization)(afxDrawStream dscr, afxBool disable);

    afxCmdId(*EnableDepthBias)(afxDrawStream dscr, afxBool enable);
    afxCmdId(*SetDepthBias)(afxDrawStream dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor);

    afxCmdId(*SetLineWidth)(afxDrawStream dscr, afxReal lineWidth);

    afxCmdId(*ResetScissors)(afxDrawStream dscr, afxNat cnt, afxRect const rc[]);
    afxCmdId(*ReadjustScissors)(afxDrawStream dscr, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    afxCmdId(*ResetAreas)(afxDrawStream dscr, afxBool exclusive, afxNat cnt, afxRect const rc[]);
    afxCmdId(*ReadjustAreas)(afxDrawStream dscr, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    afxCmdId(*EnableDepthBoundsTest)(afxDrawStream dscr, afxBool enable);
    afxCmdId(*SetDepthBounds)(afxDrawStream dscr, afxReal const bounds[2]);

    afxCmdId(*EnableStencilTest)(afxDrawStream dscr, afxBool enable);
    afxCmdId(*SetStencilCompareMask)(afxDrawStream dscr, afxMask faceMask, afxNat32 compareMask);
    afxCmdId(*SetStencilWriteMask)(afxDrawStream dscr, afxMask faceMask, afxNat32 writeMask);
    afxCmdId(*SetStencilReference)(afxDrawStream dscr, afxMask faceMask, afxNat32 reference);

    afxCmdId(*EnableDepthTest)(afxDrawStream dscr, afxBool enable);
    afxCmdId(*SetDepthCompareOp)(afxDrawStream dscr, afxCompareOp op);
    afxCmdId(*DisableDepthWrite)(afxDrawStream dscr, afxBool enable);

    afxCmdId(*SetBlendConstants)(afxDrawStream dscr, afxReal const blendConstants[4]);

    afxCmdId(*BeginSynthesis)(afxDrawStream dscr, afxSynthesisConfig const*state);
    afxCmdId(*FinishSynthesis)(afxDrawStream dscr);
    afxCmdId(*NextPass)(afxDrawStream dscr, afxBool useAuxScripts);
};

AFX_DEFINE_STRUCT(afxCmdTransformation)
{
    afxCmdId(*BindVertexInput)(afxDrawStream dscr, afxVertexInput vin);

    afxCmdId(*BindVertexSources)(afxDrawStream dscr, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[]);
    //afxCmdId(*ResetVertexStreams)(afxDrawStream dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[]);
    //afxCmdId(*ResetVertexAttributes)(afxDrawStream dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[]);
    afxCmdId(*BindIndexSource)(afxDrawStream dscr, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz);
    afxCmdId(*SetPrimitiveTopology)(afxDrawStream dscr, afxPrimTopology topology);

    afxCmdId(*ResetViewports)(afxDrawStream dscr, afxNat cnt, afxViewport const vp[]);
    afxCmdId(*ReadjustViewports)(afxDrawStream dscr, afxNat baseIdx, afxNat cnt, afxViewport const vp[]);

    afxCmdId(*SwitchFrontFace)(afxDrawStream dscr, afxBool cw);
    afxCmdId(*SetCullMode)(afxDrawStream dscr, afxCullMode mode);
};

AFX_DEFINE_STRUCT(afxCmd)
{
    void*EndOfScript;

    afxCmdId(*ExecuteCommands)(afxDrawStream dscr, afxNat cnt, afxDrawStream aux[]);

    afxCmdId(*BindPipeline)(afxDrawStream dscr, afxNat segment, afxPipeline pip);
    afxCmdId(*BindBuffers)(afxDrawStream dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
    afxCmdId(*BindRasters)(afxDrawStream dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster ras[]);

    // draw
    afxCmdId(*Draw)(afxDrawStream dscr, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx);
    afxCmdId(*DrawIndirect)(afxDrawStream dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndirectCount)(afxDrawStream dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexed)(afxDrawStream dscr, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx);
    afxCmdId(*DrawIndexedIndirect)(afxDrawStream dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexedIndirectCount)(afxDrawStream dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    
    afxCmdRaster            ras;
    afxCmdBuffer            buf;
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

AVX afxCmdId                AfxCmdBindPipeline
(
    afxDrawStream           dscr, /// is the command buffer that the pipeline will be bound to. 
    afxNat                  segment, /// is a value specifying to which level the pipeline is bound. Binding one does not disturb the others.
    afxPipeline             pip /// is the pipeline to be bound.
);

/// Execute a secondary command buffer from a primary command buffer.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary command buffer which is currently in the executable or recording state, that primary command buffer becomes invalid.

/// If the nested command buffer feature is enabled it is valid usage for AfxCmdExecuteCommands to also be recorded to a secondary command buffer.

AVX afxCmdId                AfxCmdExecuteCommands
(
    afxDrawStream           dscr,
    afxNat                  cnt, /// is the length of the @aux array.
    afxDrawStream           aux[] /// is a pointer to an array of @cnt secondary command buffer handles, which are recorded to execute in the primary command buffer in the order they are listed in the array.
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AfxCmdDraw
(
    afxDrawStream           dscr,
    afxNat                  baseInstIdx, /// is the instance ID of the first instance to draw.
    afxNat                  instCnt, /// is the number of instances to draw.
    afxNat                  baseVtxIdx, /// is the index of the first vertex to draw.
    afxNat                  vtxCnt /// is the number of vertices to draw.
);

/// Draw primitives with indirect parameters.
/// AfxCmdDrawIndirect behaves similarly to AfxCmdDraw except that the parameters are read by the device from a buffer during execution.
/// @drawCount draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndirectCmd structures.
/// If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId                AfxCmdDrawIndirect
(
    afxDrawStream           dscr,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into @buf where parameters begin.
    afxNat32                drawCnt, /// is the number of draws to execute, and can be zero.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indirect parameters and draw count.
/// AfxCmdDrawIndirectCount behaves similarly to AfxCmdDrawIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId                AfxCmdDrawIndirectCount
(
    afxDrawStream           dscr,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into buffer where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxNat32                cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32                maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indexed vertices. 
/// When the command is executed, primitives are assembled using the current primitive topology and @idxCnt vertices whose indices are retrieved from the index buffer.
/// The index buffer is treated as an array of tightly packed unsigned integers of size defined by the @idxSiz parameter (of AfxCmdBindIndexSource) with which the buffer was bound.

/// The first vertex index is at an offset of @firstIdx × @idxSiz + @offset within the bound index buffer, where @offset is the offset specified by AfxCmdBindIndexSource, and @idxSize is the byte size of the type specified by @idxSiz.
/// Subsequent index values are retrieved from consecutive locations in the index buffer.
/// Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the @idxSiz is 1 or 2) and have @vtxOff added to them, before being supplied as the @vtxIdx value.

/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AfxCmdDrawIndexed
(
    afxDrawStream           dscr,
    afxNat                  vtxOff, /// is the value added to the vertex index before indexing into the vertex buffer.
    afxNat                  baseInstIdx, /// is the instance ID of the first instance to draw.
    afxNat                  instCnt, /// is the number of instances to draw.
    afxNat                  baseIdx, /// is the base index within the index buffer.
    afxNat                  idxCnt /// is the number of vertices to draw.
);

/// Draw primitives with indirect parameters and indexed vertices.
/// AfxCmdDrawIndexedIndirect behaves similarly to AfxCmdDrawIndexed except that the parameters are read by the device from a buffer during execution.
/// @drawCnt draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndexedIndirectCmd structures. If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId                AfxCmdDrawIndexedIndirect
(
    afxDrawStream           dscr,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into buffer where parameters begin.
    afxNat32                drawCnt, /// is the number of draws to execute, and can be zero.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw parameters with indirect parameters, indexed vertices, and draw count.
/// AfxCmdDrawIndexedIndirectCount behaves similarly to AfxCmdDrawIndexedIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId                AfxCmdDrawIndexedIndirectCount
(
    afxDrawStream           dscr,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into @buf where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxNat32                cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32                maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

AVX afxCmdId            AfxCmdBindBuffers(afxDrawStream dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AVX afxCmdId            AfxCmdBindRasters(afxDrawStream dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[]);

AVX afxCmdId            AfxCmdBindFontSIG(afxDrawStream dscr, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[]);

#endif//AFX_DRAW_OPS_H
