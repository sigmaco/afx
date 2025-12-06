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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_DRAW_OPS_H
#define AVX_DRAW_OPS_H

#include "qwadro/draw/avxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxTransference.h"
#include "qwadro/draw/avxTransference.h"
#include "qwadro/draw/avxSampler.h"
#include "qwadro/draw/avxSynchronization.h"
#include "qwadro/draw/avxPipeline.h"
#include "qwadro/draw/avxTransformation.h"
#include "qwadro/draw/avxRasterization.h"

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(avxDrawIndirect)
/// Structure specifying a indirect drawing command
{
    afxUnit32   vtxCnt; // is the number of vertices to draw.
    afxUnit32   instCnt; // is the number of instances to draw.
    afxUnit32   baseVtx; // is the index of the first vertex to draw.
    afxUnit32   baseInst; // is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(avxDrawIndexedIndirect)
/// Structure specifying a indexed indirect drawing command
{
    afxUnit32   idxCnt; // is the number of vertices to draw.
    afxUnit32   instCnt; // is the number of instances to draw.
    afxUnit32   baseIdx;/// is the base index within the index buffer.
    afxInt32    vtxOffset; // is the value added to the vertex index before indexing into the vertex buffer.
    afxUnit32   baseInst; // is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(avxDispatchIndirect)
/// Structure specifying a indirect dispatching command.
{
    afxUnit32   w, h, d; // the number of warps (local workgroups) to dispatch in the X, Y and Z dimensions.
};

#pragma pack(pop)

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/*
    The AvxCmdCommenceDrawScope() operation commences a new drawing scope.
    After beginning a drawing scope, the drawing context is ready to record draw commands.

    If flags includes resuming then this draw scope is resumed from a drawing scope that has been suspended earlier in submission order.
*/

AVX afxCmdId AvxCmdCommenceDrawScope
(
    afxDrawContext dctx,
    // Structure specifying details of the draw scope instance to begin.
    avxDrawScope const* cfg
);

/*
    The AvxCmdNextPass() operation transitions to the next pass of a drawing scope.

   The pass index for a render canvas begins at zero when AvxCmdCommenceDrawScope is recorded, and increments each time AvxCmdNextPass is recorded.

   After transitioning to the next pass, the application can record the commands for that pass.
*/

AVX afxCmdId AvxCmdNextPass
(
    afxDrawContext dctx,
    // specifies how the commands in the next subpass will be provided, in the same fashion as the corresponding parameter of vkCmdBeginRenderPass.
    afxBool useAuxScripts
);

/*
    The AvxCmdConcludeDrawScope() operation concludes the current drawing scope.

    If the value of flags used to begin this draw scope instance included suspending,
    then this render canvas is suspended and will be resumed later in submission order.
*/

AVX afxCmdId AvxCmdConcludeDrawScope
(
    afxDrawContext dctx
);

/*
    The AvxCmdClearCanvas() operations clears a number of areas in every canvas' annex listed.
*/

AVX afxCmdId AvxCmdClearCanvas
(
    afxDrawContext dctx,
    // The number of annexes to clear.
    afxUnit bufCnt,
    // An array of indices to each annex to be cleared.
    // If NIL, it will index sequentially up to @annexCnt.
    afxUnit const bins[],
    // An array of clear values, one for each annex.
    avxClearValue const values[],
    // The number of areas to be cleared in every annex.
    afxUnit areaCnt,
    // An array of areas to be cleared in every annex.
    afxLayeredRect const areas[]
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId AvxCmdDraw
(
    afxDrawContext dctx,
    afxUnit vtxCnt, // is the number of vertices to draw.
    afxUnit instCnt, // is the number of instances to draw.
    afxUnit baseVtx, // is the index of the first vertex to draw.
    afxUnit baseInst // is the instance ID of the first instance to draw.
);

/// Draw primitives with indirect parameters.
/// AvxCmdDrawIndirect behaves similarly to AvxCmdDraw except that the parameters are read by the device from a buffer during execution.
/// @drawCount draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndirectCmd structures.
/// If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId AvxCmdDrawIndirect
(
    afxDrawContext dctx,
    avxBuffer buf, // is the buffer containing draw parameters.
    afxUnit32 offset, // is the byte offset into @buf where parameters begin.
    afxUnit32 drawCnt, // is the number of draws to execute, and can be zero.
    afxUnit32 stride // is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indirect parameters and draw count.
/// AvxCmdDrawIndirect2 behaves similarly to AvxCmdDrawIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId AvxCmdDrawIndirect2
(
    afxDrawContext dctx,
    avxBuffer buf, // is the buffer containing draw parameters.
    afxUnit32 offset, // is the byte offset into buffer where parameters begin.
    avxBuffer cntBuf, // is the buffer containing the draw count.
    afxUnit32 cntBufOff, // is the byte offset into @cntBuf where the draw count begins.
    afxUnit32 maxDrawCnt, // specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit32 stride // is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indexed vertices. 
/// When the command is executed, primitives are assembled using the current primitive topology and @idxCnt vertices whose indices are retrieved from the index buffer.
/// The index buffer is treated as an array of tightly packed unsigned integers of size defined by the @idxSiz parameter (of AvxCmdBindIndexBuffer) with which the buffer was bound.

/// The first vertex index is at an offset of @firstIdx × @idxSiz + @offset within the bound index buffer, where @offset is the offset specified by AvxCmdBindIndexBuffer, and @idxSize is the byte size of the type specified by @idxSiz.
/// Subsequent index values are retrieved from consecutive locations in the index buffer.
/// Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the @idxSiz is 1 or 2) and have @vtxOff added to them, before being supplied as the @vtxIdx value.

/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxCmdId AvxCmdDrawIndexed
(
    afxDrawContext dctx,
    afxUnit idxCnt, // is the number of vertices to draw.
    afxUnit instCnt, // is the number of instances to draw.
    afxUnit baseIdx, // is the base index within the index buffer.
    afxUnit vtxOffset, // is the value added to the vertex index before indexing into the vertex buffer.
    afxUnit baseInst // is the instance ID of the first instance to draw.
);

/// Draw primitives with indirect parameters and indexed vertices.
/// AvxCmdDrawIndexedIndirect behaves similarly to AvxCmdDrawIndexed except that the parameters are read by the device from a buffer during execution.
/// @drawCnt draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndexedIndirectCmd structures. If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxCmdId AvxCmdDrawIndexedIndirect
(
    afxDrawContext dctx,
    avxBuffer buf, // is the buffer containing draw parameters.
    afxUnit32 offset, // is the byte offset into buffer where parameters begin.
    afxUnit32 drawCnt, // is the number of draws to execute, and can be zero.
    afxUnit32 stride // is the byte stride between successive sets of draw parameters.
);

/// Draw parameters with indirect parameters, indexed vertices, and draw count.
/// AvxCmdDrawIndexedIndirect2 behaves similarly to AvxCmdDrawIndexedIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxCmdId AvxCmdDrawIndexedIndirect2
(
    afxDrawContext dctx,
    avxBuffer buf, // is the buffer containing draw parameters.
    afxUnit32 offset, // is the byte offset into @buf where parameters begin.
    avxBuffer cntBuf, // is the buffer containing the draw count.
    afxUnit32 cntBufOff, // is the byte offset into @cntBuf where the draw count begins.
    afxUnit32 maxDrawCnt, // specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit32 stride // is the byte stride between successive sets of draw parameters.
);

AVX afxCmdId AvxCmdDispatch
/// Dispatch compute work items.
(
    afxDrawContext dctx,
    afxUnit w, // the number of local workgroups to dispatch in the X dimension.
    afxUnit h, // the number of local workgroups to dispatch in the Y dimension.
    afxUnit d // the number of local workgroups to dispatch in the Z dimension.
);

AVX afxCmdId AvxCmdDispatchIndirect
/// Dispatch compute work items with indirect parameters.
(
    afxDrawContext dctx,
    avxBuffer buf, // the buffer containing dispatch parameters.
    afxUnit32 offset // the byte offset into buffer where parameters begin.
);

#endif//AVX_DRAW_OPS_H
