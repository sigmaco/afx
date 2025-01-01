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

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "../impl/avxImplementation.h"

// buffer ops

_AVX afxCmdId AvxCmdCopyBuffer(afxDrawContext dctx, afxBuffer src, afxBuffer dst, afxUnit opCnt, afxBufferCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);

    /// src must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    /// dst must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &dst);

    /// src must have been created with afxBufferUsage_UPLOAD usage flag.
    AFX_ASSERT(AfxTestBufferAccess(src, afxBufferFlag_R));
    /// dst must have been created with afxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AfxTestBufferAccess(dst, afxBufferFlag_W));

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(CopyBuffer), sizeof(cmd->CopyBuffer) + (opCnt * sizeof(cmd->CopyBuffer.ops[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyBuffer.src = src;
    cmd->CopyBuffer.dst = dst;
    cmd->CopyBuffer.opCnt = opCnt;

    for (afxUnit i = 0; i < opCnt; i++)
        _AvxSanitizeBufferCopy(src, dst, &ops[i], &cmd->CopyBuffer.ops[i]);

    return cmdId;
}

_AVX afxCmdId AvxCmdFillBuffer(afxDrawContext dctx, afxBuffer buf, afxUnit offset, afxUnit range, afxUnit value)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    /// buf must have been created with afxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));
    
    afxUnit bufCap = AfxGetBufferCapacity(buf, 0);
    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AFX_ASSERT_RANGE(bufCap, offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(FillBuffer), sizeof(cmd->FillBuffer), &cmdId);
    AFX_ASSERT(cmd);
    cmd->FillBuffer.buf = buf;
    cmd->FillBuffer.offset = AfxMin(offset, bufCap - 1);
    cmd->FillBuffer.range = AfxMin(range, bufCap - cmd->FillBuffer.offset);
    cmd->FillBuffer.value = value;
    return cmdId;
}

_AVX afxCmdId AvxCmdClearBuffer(afxDrawContext dctx, afxBuffer buf, afxUnit offset, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    /// buf must have been created with afxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));

    afxUnit bufCap = AfxGetBufferCapacity(buf, 0);
    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AFX_ASSERT_RANGE(bufCap, offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(ClearBuffer), sizeof(cmd->ClearBuffer), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ClearBuffer.buf = buf;
    cmd->ClearBuffer.offset = AfxMin(offset, bufCap - 1);
    cmd->ClearBuffer.range = AfxMin(range, bufCap - cmd->ClearBuffer.offset);
    return cmdId;
}

_AVX afxCmdId AvxCmdUpdateBuffer(afxDrawContext dctx, afxBuffer buf, afxUnit offset, afxUnit range, void const* src)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    //AFX_ASSERT(!dctx->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    /// buf must have been created with afxBufferUsage_DOWNLOAD usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));

    afxUnit bufCap = AfxGetBufferCapacity(buf, 0);
    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of buf minus offset.
    AFX_ASSERT_RANGE(bufCap, offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    /// the src data for the buffer update, and must be at least range bytes in size.
    AFX_ASSERT(src);

    AFX_ASSERT(64000 >= range);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(UpdateBuffer), sizeof(cmd->UpdateBuffer) + range, &cmdId);
    AFX_ASSERT(cmd);
    cmd->UpdateBuffer.buf = buf;
    cmd->UpdateBuffer.offset = AfxMin(offset, bufCap - 1);
    cmd->UpdateBuffer.range = AfxMin(range, bufCap - cmd->UpdateBuffer.offset);
    AfxCopy(cmd->UpdateBuffer.data, src, cmd->UpdateBuffer.range);

    return cmdId;
}
