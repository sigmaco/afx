/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DRAW_OPS_H
#define AVX_DRAW_OPS_H

#include "qwadro/inc/draw/math/afxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/op/avxRasterOps.h"
#include "qwadro/inc/draw/op/avxBufferOps.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/op/avxQueryOps.h"
#include "qwadro/inc/draw/pipe/avxPipeline.h"
#include "qwadro/inc/draw/op/avxTransformOps.h"
#include "qwadro/inc/draw/op/avxRasterizationOps.h"

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(avxDrawIndirect)
/// Structure specifying a indirect drawing command
{
    afxUnit32   vtxCnt; /// is the number of vertices to draw.
    afxUnit32   instCnt; /// is the number of instances to draw.
    afxUnit32   baseVtx; /// is the index of the first vertex to draw.
    afxUnit32   baseInst; /// is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(avxDrawIndexedIndirect)
/// Structure specifying a indexed indirect drawing command
{
    afxUnit32   idxCnt; /// is the number of vertices to draw.
    afxUnit32   instCnt; /// is the number of instances to draw.
    afxUnit32   baseIdx;/// is the base index within the index buffer.
    afxInt32    vtxOff; /// is the value added to the vertex index before indexing into the vertex buffer.
    afxUnit32   baseInst; /// is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(avxDispatchIndirect)
/// Structure specifying a indirect dispatching command.
{
    afxUnit32   x, y, z; /// the number of warps (local workgroups) to dispatch in the X, Y and Z dimensions.
};

#pragma pack(pop)

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxCmdId        AvxCmdPushDebugScope
/// Open a command buffer debug label region.
(
    afxDrawContext  dctx,
    afxString const*name, /// the name of the label.
    afxColor const  color /// is an optional color associated with the label.
);

AVX afxCmdId        AvxCmdPopDebugScope
/// Close a command buffer label region.
(
    afxDrawContext  dctx
);

AVX afxCmdId        AvxCmdInsertDebugLabel
/// Insert a label into a command buffer.
(
    afxDrawContext  dctx,
    afxString const*name, /// the name of the label.
    afxColor const  color /// is an optional color associated with the label.
);

/// Bind a pipeline object to a command buffer.
/// Once bound, a pipeline binding affects subsequent commands that interact with the given pipeline type in the command buffer until a different pipeline of the same type is bound to the bind point, or until the pipeline bind point is disturbed by binding a shader object.
/// Commands that do not interact with the given pipeline type must not be affected by the pipeline state.

AVX afxCmdId                AvxCmdBindPipeline
(
    afxDrawContext          dctx, /// is the command buffer that the pipeline will be bound to. 
    afxUnit                 segment, /// is a value specifying to which level the pipeline is bound. Binding one does not disturb the others.
    avxPipeline             pip, /// is the pipeline to be bound.
    avxVertexDecl           vin,
    afxFlags                dynamics
);

/// Execute a secondary command buffer from a primary command buffer.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary command buffer which is currently in the executable or recording state, that primary command buffer becomes invalid.

/// If the nested command buffer feature is enabled it is valid usage for AvxCmdExecuteCommands to also be recorded to a secondary command buffer.

AVX afxCmdId                AvxCmdExecuteCommands
(
    afxDrawContext          dctx,
    afxUnit                 cnt, /// is the length of the @aux array.
    afxDrawContext          aux[] /// is a pointer to an array of @cnt secondary command buffer handles, which are recorded to execute in the primary command buffer in the order they are listed in the array.
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AvxCmdDraw
(
    afxDrawContext          dctx,
    afxUnit                 vtxCnt, /// is the number of vertices to draw.
    afxUnit                 instCnt, /// is the number of instances to draw.
    afxUnit                 baseVtx, /// is the index of the first vertex to draw.
    afxUnit                 baseInst /// is the instance ID of the first instance to draw.
);

/// Draw primitives with indirect parameters.
/// AvxCmdDrawIndirect behaves similarly to AvxCmdDraw except that the parameters are read by the device from a buffer during execution.
/// @drawCount draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndirectCmd structures.
/// If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId                AvxCmdDrawIndirect
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxUnit32               offset, /// is the byte offset into @buf where parameters begin.
    afxUnit32               drawCnt, /// is the number of draws to execute, and can be zero.
    afxUnit32               stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indirect parameters and draw count.
/// AvxCmdDrawIndirectCount behaves similarly to AvxCmdDrawIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId                AvxCmdDrawIndirectCount
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxUnit32               offset, /// is the byte offset into buffer where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxUnit32               cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxUnit32               maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit32               stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indexed vertices. 
/// When the command is executed, primitives are assembled using the current primitive topology and @idxCnt vertices whose indices are retrieved from the index buffer.
/// The index buffer is treated as an array of tightly packed unsigned integers of size defined by the @idxSiz parameter (of AvxCmdBindIndexBuffer) with which the buffer was bound.

/// The first vertex index is at an offset of @firstIdx × @idxSiz + @offset within the bound index buffer, where @offset is the offset specified by AvxCmdBindIndexBuffer, and @idxSize is the byte size of the type specified by @idxSiz.
/// Subsequent index values are retrieved from consecutive locations in the index buffer.
/// Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the @idxSiz is 1 or 2) and have @vtxOff added to them, before being supplied as the @vtxIdx value.

/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId                AvxCmdDrawIndexed
(
    afxDrawContext          dctx,
    afxUnit                 idxCnt, /// is the number of vertices to draw.
    afxUnit                 instCnt, /// is the number of instances to draw.
    afxUnit                 baseIdx, /// is the base index within the index buffer.
    afxUnit                 vtxOffset, /// is the value added to the vertex index before indexing into the vertex buffer.
    afxUnit                 baseInst /// is the instance ID of the first instance to draw.
);

/// Draw primitives with indirect parameters and indexed vertices.
/// AvxCmdDrawIndexedIndirect behaves similarly to AvxCmdDrawIndexed except that the parameters are read by the device from a buffer during execution.
/// @drawCnt draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndexedIndirectCmd structures. If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId                AvxCmdDrawIndexedIndirect
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxUnit32               offset, /// is the byte offset into buffer where parameters begin.
    afxUnit32               drawCnt, /// is the number of draws to execute, and can be zero.
    afxUnit32               stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw parameters with indirect parameters, indexed vertices, and draw count.
/// AvxCmdDrawIndexedIndirectCount behaves similarly to AvxCmdDrawIndexedIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId                AvxCmdDrawIndexedIndirectCount
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// is the buffer containing draw parameters.
    afxUnit32               offset, /// is the byte offset into @buf where parameters begin.
    afxBuffer               cntBuf, /// is the buffer containing the draw count.
    afxUnit32               cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxUnit32               maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit32               stride /// is the byte stride between successive sets of draw parameters.
);

AVX afxCmdId                AvxCmdDispatch
/// Dispatch compute work items.
(
    afxDrawContext          dctx,
    afxUnit                 grpCntX, /// the number of local workgroups to dispatch in the X dimension.
    afxUnit                 grpCntY, /// the number of local workgroups to dispatch in the Y dimension.
    afxUnit                 grpCntZ /// the number of local workgroups to dispatch in the Z dimension.
);

AVX afxCmdId                AvxCmdDispatchIndirect
/// Dispatch compute work items with indirect parameters.
(
    afxDrawContext          dctx,
    afxBuffer               buf, /// the buffer containing dispatch parameters.
    afxUnit32               offset /// the byte offset into buffer where parameters begin.
);


AVX afxCmdId            AvxCmdBindBuffers(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, afxBufferMap const maps[]);
AVX afxCmdId            AvxCmdBindRasters(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, afxRaster const rasters[]);
AVX afxCmdId            AvxCmdBindSamplers(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, avxSampler const samplers[]);

AVX afxCmdId            AvxCmdBindFontSIG(afxDrawContext dctx, afxUnit first, afxUnit cnt, afxTypography typ[], avxPipeline pip[], avxSampler smp[], afxRaster ras[]);

AVX afxCmdId            AvxCmdPushConstants(afxDrawContext dctx, afxUnit offset, afxUnit siz, void const* data);

AVX afxError            AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption);

#endif//AVX_DRAW_OPS_H
