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

#ifndef AFX_DRAW_COMMANDS_H
#define AFX_DRAW_COMMANDS_H

#include "qwadro/math/afxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/font/afxTypography.h"
#include "qwadro/draw/pipe/afxQueryPool.h"
#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/pipe/afxTransformation.h"
#include "qwadro/draw/pipe/afxRasterization.h"

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
    afxCmdId(*cpy)(afxDrawScript dscr, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[]);
    afxCmdId(*set)(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat range, afxNat data);
    afxCmdId(*rw)(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* data);
    afxCmdId(*io)(afxDrawScript dscr, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxBool toHost, afxStream io);
};

AFX_DEFINE_STRUCT(afxCmdRaster)
{
    afxCmdId(*rw)(afxDrawScript dscr, afxRaster ras, afxStream ios, afxNat opCnt, afxRasterIoOp const ops[], afxBool down, afxCodec cdc);
    afxCmdId(*pak)(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[], afxBool unpack);
    afxCmdId(*cpy)(afxDrawScript dscr, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[]);
    afxCmdId(*mip)(afxDrawScript dscr, afxRaster ras, afxNat baseLod, afxNat lodCnt);
    afxCmdId(*xform)(afxDrawScript dscr, afxRaster ras, afxReal const m[4][4], afxNat rgnCnt, afxRasterRegion const rgn[]);
    afxCmdId(*swizzle)(afxDrawScript dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[]);
};

AFX_DEFINE_STRUCT(afxCmdQuery)
{
    afxCmdId(*Begin)(afxDrawScript dscr, afxQueryPool pool, afxNat queryIdx, afxBool precise);
    afxCmdId(*End)(afxDrawScript dscr, afxQueryPool pool, afxNat queryIdx);
    afxCmdId(*CopyResults)(afxDrawScript dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags);
    afxCmdId(*Reset)(afxDrawScript dscr, afxQueryPool pool, afxNat baseQuery, afxNat queryCnt);
    afxCmdId(*WriteTimestamp)(afxDrawScript dscr, afxQueryPool pool, afxNat queryIdx, afxPipelineStage stage);
};

AFX_DEFINE_STRUCT(afxCmdRasterization)
{
    afxCmdId(*DisableRasterization)(afxDrawScript dscr, afxBool disable);

    afxCmdId(*EnableDepthBias)(afxDrawScript dscr, afxBool enable);
    afxCmdId(*SetDepthBias)(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor);

    afxCmdId(*SetLineWidth)(afxDrawScript dscr, afxReal lineWidth);

    afxCmdId(*ResetScissors)(afxDrawScript dscr, afxNat cnt, afxRect const rc[]);
    afxCmdId(*ReadjustScissors)(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    afxCmdId(*ResetAreas)(afxDrawScript dscr, afxBool exclusive, afxNat cnt, afxRect const rc[]);
    afxCmdId(*ReadjustAreas)(afxDrawScript dscr, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    afxCmdId(*EnableDepthBoundsTest)(afxDrawScript dscr, afxBool enable);
    afxCmdId(*SetDepthBounds)(afxDrawScript dscr, afxReal const bounds[2]);

    afxCmdId(*EnableStencilTest)(afxDrawScript dscr, afxBool enable);
    afxCmdId(*SetStencilCompareMask)(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask);
    afxCmdId(*SetStencilWriteMask)(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask);
    afxCmdId(*SetStencilReference)(afxDrawScript dscr, afxMask faceMask, afxNat32 reference);

    afxCmdId(*EnableDepthTest)(afxDrawScript dscr, afxBool enable);
    afxCmdId(*SetDepthCompareOp)(afxDrawScript dscr, afxCompareOp op);
    afxCmdId(*DisableDepthWrite)(afxDrawScript dscr, afxBool enable);

    afxCmdId(*SetBlendConstants)(afxDrawScript dscr, afxReal const blendConstants[4]);

    afxCmdId(*BeginSynthesis)(afxDrawScript dscr, afxSynthesisConfig const*state);
    afxCmdId(*FinishSynthesis)(afxDrawScript dscr);
    afxCmdId(*NextPass)(afxDrawScript dscr, afxBool useAuxScripts);
};

AFX_DEFINE_STRUCT(afxCmdTransformation)
{
    afxCmdId(*BindVertexInput)(afxDrawScript dscr, afxVertexInput vin);

    afxCmdId(*BindVertexSources)(afxDrawScript dscr, afxNat baseSlotIdx, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[]);
    //afxCmdId(*ResetVertexStreams)(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[]);
    //afxCmdId(*ResetVertexAttributes)(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[]);
    afxCmdId(*BindIndexSource)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz);
    afxCmdId(*SetPrimitiveTopology)(afxDrawScript dscr, afxPrimTopology topology);

    afxCmdId(*ResetViewports)(afxDrawScript dscr, afxNat cnt, afxViewport const vp[]);
    afxCmdId(*ReadjustViewports)(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxViewport const vp[]);

    afxCmdId(*SwitchFrontFace)(afxDrawScript dscr, afxBool cw);
    afxCmdId(*SetCullMode)(afxDrawScript dscr, afxCullMode mode);
};

AFX_DEFINE_STRUCT(afxCmd)
{
    void*EndOfScript;

    afxCmdId(*ExecuteCommands)(afxDrawScript dscr, afxNat cnt, afxDrawScript aux[]);

    afxCmdId(*BindPipeline)(afxDrawScript dscr, afxNat segment, afxPipeline pip);
    afxCmdId(*BindBuffers)(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
    afxCmdId(*BindRasters)(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster ras[]);

    // draw
    afxCmdId(*Draw)(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx);
    afxCmdId(*DrawIndirect)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndirectCount)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexed)(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx);
    afxCmdId(*DrawIndexedIndirect)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    afxCmdId(*DrawIndexedIndirectCount)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    
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

/// Execute a secondary command buffer from a primary command buffer.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary command buffer which is currently in the executable or recording state, that primary command buffer becomes invalid.

/// If the nested command buffer feature is enabled it is valid usage for AfxCmdExecuteCommands to also be recorded to a secondary command buffer.

AVX afxCmdId                AfxCmdExecuteCommands
(
    afxDrawScript           dscr,
    afxNat                  cnt, /// is the length of the @aux array.
    afxDrawScript           aux[] /// is a pointer to an array of @cnt secondary command buffer handles, which are recorded to execute in the primary command buffer in the order they are listed in the array.
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AfxCmdDraw
(
    afxDrawScript           dscr,
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
    afxDrawScript           dscr,
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
    afxDrawScript           dscr,
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
    afxDrawScript           dscr,
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
    afxDrawScript           dscr,
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
    afxDrawScript           dscr,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxNat32                offset, /// is the byte offset into @buf where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxNat32                cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32                maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32                stride /// is the byte stride between successive sets of draw parameters.
);

AVX afxCmdId            AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AVX afxCmdId            AfxCmdBindRasters(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[]);

AVX afxCmdId            AfxCmdBindFontSIG(afxDrawScript dscr, afxNat first, afxNat cnt, afxTypography typ[], afxPipeline pip[], afxSampler smp[], afxRaster ras[]);

#endif//AFX_DRAW_COMMANDS_H
