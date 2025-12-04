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

  //////////////////////////////////////////////////////////////////////////////
 // GEOMETRIC PRIMITIVE TRANSFORMATION PIPELINE EXTENDED OPERATIONS          //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_TRANSFORMATION_EXT_H
#define AVX_TRANSFORMATION_EXT_H

#include "qwadro/draw/avxTransformation.h"

// Specify polygon mode dynamically for a draw context.

AVX afxCmdId            AvxCmdChangeFillModeEXT
(
    afxDrawContext      dctx,
    avxFillMode         mode
);

AFX afxCmdId AvxCmdBeginTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cntBufCnt,
    avxBuffer const cntBufs[],
    afxSize const cntBufOffsets[]
);

AFX afxCmdId AvxCmdEndTransformFeedbackEXT
(
    afxDrawContext dctx,
    afxUnit firstCntBuf,
    afxUnit cntBufCnt,
    avxBuffer const cntBufs[],
    afxSize const cntBufOffsets[]
);

AFX afxCmdId AvxCmdBeginQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxBool precise,
    afxUnit index
);

AFX afxCmdId AvxCmdEndQueryIndexedEXT
(
    afxDrawContext dctx,
    avxQueryPool qryp,
    afxUnit query,
    afxUnit index
);

AFX afxCmdId AvxCmdBindTransformFeedbackBuffersEXT
(
    afxDrawContext dctx,
    afxUnit first,
    afxUnit cnt,
    avxBuffer const buffers[],
    afxSize const offsets[],
    afxSize const ranges[]
);

AFX afxCmdId AvxCmdDrawIndirectByteCountEXT
(
    afxDrawContext dctx,
    afxUnit instCnt,
    afxUnit firstInst,
    avxBuffer cntBuf,
    afxSize cntBufOffset,
    afxUnit cntOffset,
    afxUnit vtxStride
);

/*
    AvxCmdDrawMeshTasksEXT() issues mesh task work items.
*/

AVX afxCmdId AvxCmdDrawMeshTasksEXT
(
    afxDrawContext dctx,
    // The number of local workgroups to dispatch in the X dimension.
    afxUnit w,
    // The number of local workgroups to dispatch in the Y dimension.
    afxUnit h,
    // The number of local workgroups to dispatch in the Z dimension.
    afxUnit d
);

AFX_DEFINE_STRUCT(avxDrawMeshTasksIndirectEXT)
// Structure specifying a mesh tasks draw indirect command.
{
    // The number of local workgroups to dispatch in the X dimension.
    afxUnit w;
    // The number of local workgroups to dispatch in the Y dimension.
    afxUnit h;
    // The number of local workgroups to dispatch in the Z dimension.
    afxUnit d;
};

/*
    AvxCmdDrawMeshTasksIndirectEXT() is a version of AvxCmdDrawMeshTasksEXT() where the parameters are read by 
    the device from a buffer during execution. @drawCnt draws are executed by the command, with parameters taken from 
    @buf starting at @offset and increasing by stride bytes for each successive draw. The parameters of each draw are 
    encoded in an array of avxDrawMeshTasksIndirectEXT structures. If @drawCnt is less than or equal to one, 
    @stride is ignored.
*/

AVX afxCmdId AvxCmdDrawMeshTasksIndirectEXT
(
    afxDrawContext dctx,
    // The buffer containing draw parameters.
    avxBuffer buf,
    // The byte offset into buffer where parameters begin.
    afxSize offset,
    // The number of draws to execute, and can be zero.
    afxUnit drawCnt,
    // The byte stride between successive sets of draw parameters.
    afxUnit stride
);

/*
    AvxCmdDrawMeshTasksIndirectCountEXT() is a version of AvxCmdDrawMeshTasksIndirectEXT() where the draw count is read by 
    the device from a buffer during execution. The command will read an unsigned 32-bit integer from @cntBuf located at 
    @cntBufOffset and use this as the draw count.
*/

AVX afxCmdId AvxCmdDrawMeshTasksIndirectCountEXT
(
    afxDrawContext dctx,
    // The buffer containing draw parameters.
    avxBuffer buf,
    // The byte offset into buffer where parameters begin.
    afxSize offset,
    // The buffer containing the draw count.
    avxBuffer cntBuf,
    // The byte offset into countBuffer where the draw count begins.
    afxSize cntBufOffset,
    // The maximum number of draws that will be executed. 
    // The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit maxDrawCnt,
    // The byte stride between successive sets of draw parameters.
    afxUnit stride
);

#endif//AVX_TRANSFORMATION_EXT_H
