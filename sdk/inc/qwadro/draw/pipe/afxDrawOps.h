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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

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
    afxCmdId(*cpy)(afxDrawStream diob, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[]);
    afxCmdId(*set)(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, afxNat data);
    afxCmdId(*rw)(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* data);
    afxCmdId(*io)(afxDrawStream diob, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxBool toHost, afxStream io);
};

AFX_DEFINE_STRUCT(afxCmdRaster)
{
    afxCmdId(*io)(afxDrawStream diob, afxRaster ras, afxStream iob, afxNat opCnt, afxRasterIoOp const ops[], afxBool down, afxCodec cdc);
    afxCmdId(*pak)(afxDrawStream diob, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[], afxBool unpack);
    afxCmdId(*cpy)(afxDrawStream diob, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[]);
    afxCmdId(*mip)(afxDrawStream diob, afxRaster ras, afxNat baseLod, afxNat lodCnt);
    afxCmdId(*xform)(afxDrawStream diob, afxRaster ras, afxReal const m[4][4], afxNat rgnCnt, afxRasterRegion const rgn[]);
    afxCmdId(*swizzle)(afxDrawStream diob, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[]);
};

AFX_DEFINE_STRUCT(afxCmdQuery)
{
    afxCmdId(*Begin)(afxDrawStream diob, afxQueryPool pool, afxNat queryIdx, afxBool precise);
    afxCmdId(*End)(afxDrawStream diob, afxQueryPool pool, afxNat queryIdx);
    afxCmdId(*CopyResults)(afxDrawStream diob, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags);
    afxCmdId(*Reset)(afxDrawStream diob, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt);
    afxCmdId(*WriteTimestamp)(afxDrawStream diob, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage);
};

AFX_DEFINE_STRUCT(afxCmdRasterization)
{
    afxCmdId(*DisableRasterization)(afxDrawStream diob, afxBool disable);

    afxCmdId(*EnableDepthBias)(afxDrawStream diob, afxBool enable);
    afxCmdId(*SetDepthBias)(afxDrawStream diob, afxReal constFactor, afxReal clamp, afxReal slopeFactor);

    afxCmdId(*SetLineWidth)(afxDrawStream diob, afxReal lineWidth);

    afxCmdId(*ResetScissors)(afxDrawStream diob, afxNat cnt, afxRect const rc[]);
    afxCmdId(*ReadjustScissors)(afxDrawStream diob, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    afxCmdId(*ResetAreas)(afxDrawStream diob, afxBool exclusive, afxNat cnt, afxRect const rc[]);
    afxCmdId(*ReadjustAreas)(afxDrawStream diob, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    afxCmdId(*EnableDepthBoundsTest)(afxDrawStream diob, afxBool enable);
    afxCmdId(*SetDepthBounds)(afxDrawStream diob, afxReal const bounds[2]);

    afxCmdId(*EnableStencilTest)(afxDrawStream diob, afxBool enable);
    afxCmdId(*SetStencilCompareMask)(afxDrawStream diob, afxMask faceMask, afxNat32 compareMask);
    afxCmdId(*SetStencilWriteMask)(afxDrawStream diob, afxMask faceMask, afxNat32 writeMask);
    afxCmdId(*SetStencilReference)(afxDrawStream diob, afxMask faceMask, afxNat32 reference);

    afxCmdId(*EnableDepthTest)(afxDrawStream diob, afxBool enable);
    afxCmdId(*SetDepthCompareOp)(afxDrawStream diob, afxCompareOp op);
    afxCmdId(*DisableDepthWrite)(afxDrawStream diob, afxBool enable);

    afxCmdId(*SetBlendConstants)(afxDrawStream diob, afxReal const blendConstants[4]);

    afxCmdId(*BeginSynthesis)(afxDrawStream diob, afxSynthesisConfig const*state);
    afxCmdId(*FinishSynthesis)(afxDrawStream diob);
    afxCmdId(*NextPass)(afxDrawStream diob, afxBool useAuxScripts);
};

AFX_DEFINE_STRUCT(afxCmdTransformation)
{
    afxCmdId(*BindVertexInput)(afxDrawStream diob, afxVertexInput vin);

    afxCmdId(*BindVertexSources)(afxDrawStream diob, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[]);
    //afxCmdId(*ResetVertexStreams)(afxDrawStream diob, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[]);
    //afxCmdId(*ResetVertexAttributes)(afxDrawStream diob, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[]);
    afxCmdId(*BindIndexSource)(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz);
    afxCmdId(*SetPrimitiveTopology)(afxDrawStream diob, afxPrimTopology topology);

    afxCmdId(*ResetViewports)(afxDrawStream diob, afxNat cnt, afxViewport const vp[]);
    afxCmdId(*ReadjustViewports)(afxDrawStream diob, afxNat baseIdx, afxNat cnt, afxViewport const vp[]);

    afxCmdId(*SwitchFrontFace)(afxDrawStream diob, afxBool cw);
    afxCmdId(*SetCullMode)(afxDrawStream diob, afxCullMode mode);
};

AFX_DEFINE_STRUCT(afxCmd)
{
    void*EndOfScript;

    afxCmdId(*ExecuteCommands)(afxDrawStream diob, afxNat cnt, afxDrawStream aux[]);

    afxCmdId(*BindPipeline)(afxDrawStream diob, afxNat segment, afxPipeline pip);
    afxCmdId(*BindBuffers)(afxDrawStream diob, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
    afxCmdId(*BindRasters)(afxDrawStream diob, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster ras[]);

    // draw
    afxCmdId(*Draw)(afxDrawStream diob, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx);
    afxCmdId(*DrawIndirect)(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndirectCount)(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexed)(afxDrawStream diob, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx);
    afxCmdId(*DrawIndexedIndirect)(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexedIndirectCount)(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    
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
    afxDrawStream           diob, /// is the command buffer that the pipeline will be bound to. 
    afxNat                  segment, /// is a value specifying to which level the pipeline is bound. Binding one does not disturb the others.
    afxPipeline             pip /// is the pipeline to be bound.
);

/// Execute a secondary command buffer from a primary command buffer.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary command buffer which is currently in the executable or recording state, that primary command buffer becomes invalid.

/// If the nested command buffer feature is enabled it is valid usage for AfxCmdExecuteCommands to also be recorded to a secondary command buffer.

AVX afxCmdId                AfxCmdExecuteCommands
(
    afxDrawStream           diob,
    afxNat                  cnt, /// is the length of the @aux array.
    afxDrawStream           aux[] /// is a pointer to an array of @cnt secondary command buffer handles, which are recorded to execute in the primary command buffer in the order they are listed in the array.
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AfxCmdDraw
(
    afxDrawStream           diob,
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
    afxDrawStream           diob,
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
    afxDrawStream           diob,
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
    afxDrawStream           diob,
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
    afxDrawStream           diob,
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
    afxDrawStream           diob,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into @buf where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxNat32                cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32                maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

AVX afxCmdId            AfxCmdBindBuffers(afxDrawStream diob, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AVX afxCmdId            AfxCmdBindRasters(afxDrawStream diob, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[]);

AVX afxCmdId            AfxCmdBindFontSIG(afxDrawStream diob, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[]);

#endif//AFX_DRAW_OPS_H
