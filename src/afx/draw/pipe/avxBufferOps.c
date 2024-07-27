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

#define _AVX_DRAW_C
#define _AVX_CMD_BUFFER_C
#include "../dev/AvxDevKit.h"

// buffer ops

_AVX afxCmdId AvxCmdCopyBufferRegions(avxCmdb cmdb, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// opCnt must be greater than 0.
    AfxAssert(opCnt);

    /// src must be a valid afxBuffer handle.
    AfxAssertObjects(1, &src, afxFcc_BUF);
    /// dst must be a valid afxBuffer handle.
    AfxAssertObjects(1, &dst, afxFcc_BUF);

    /// src must have been created with afxBufferUsage_SRC usage flag.
    AfxAssert(AfxTestBufferAccess(src, afxBufferAccess_R));
    /// dst must have been created with afxBufferUsage_DST usage flag.
    AfxAssert(AfxTestBufferAccess(dst, afxBufferAccess_W));

    return cmdb->stdCmds->buf.cpy(cmdb, src, dst, opCnt, ops);
}

_AVX afxCmdId AvxCmdCopyBuffer(avxCmdb cmdb, afxBuffer src, afxNat srcOffset, afxBuffer dst, afxNat dstOffset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// src must be a valid afxBuffer handle.
    AfxAssertObjects(1, &src, afxFcc_BUF);
    /// dst must be a valid afxBuffer handle.
    AfxAssertObjects(1, &dst, afxFcc_BUF);

    /// src must have been created with afxBufferUsage_SRC usage flag.
    AfxAssert(AfxTestBufferAccess(src, afxBufferAccess_R));
    /// dst must have been created with afxBufferUsage_DST usage flag.
    AfxAssert(AfxTestBufferAccess(dst, afxBufferAccess_W));
    

    /// The srcOffset member of each element of pRegions must be less than the size of src.
    /// The size member of each element of pRegions must be less than or equal to the size of src minus srcOffset.
    AfxAssertRange(AfxGetBufferCapacity(src, 0), srcOffset, range);
    /// The dstOffset member of each element of pRegions must be less than the size of dst.
    /// The size member of each element of pRegions must be less than or equal to the size of dst minus dstOffset.
    AfxAssertRange(AfxGetBufferCapacity(dst, 0), dstOffset, range);

    AfxAssert(srcOffset % sizeof(afxNat32) == 0);
    AfxAssert(dstOffset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    afxBufferCopyOp op;
    op.srcOffset = srcOffset;
    op.dstOffset = dstOffset;
    op.range = range;
    return cmdb->stdCmds->buf.cpy(cmdb, src, dst, 1, &op);
}

_AVX afxCmdId AvxCmdFillBuffer(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, afxNat value)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_DST usage flag.
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    
    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AfxAssertRange(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AfxAssert(offset % sizeof(afxNat32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);

    return cmdb->stdCmds->buf.set(cmdb, buf, offset, range, value);
}

_AVX afxCmdId AvxCmdClearBuffer(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_DST usage flag.
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));

    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AfxAssertRange(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AfxAssert(offset % sizeof(afxNat32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);

    return cmdb->stdCmds->buf.set(cmdb, buf, offset, range, 0);
}

_AVX afxCmdId AvxCmdUpdateBuffer(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, void const* src)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    //AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_DST usage flag.
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));

    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of buf minus offset.
    AfxAssertRange(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AfxAssert(offset % sizeof(afxNat32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);

    /// the src data for the buffer update, and must be at least range bytes in size.
    AfxAssert(src);
    return cmdb->stdCmds->buf.rw(cmdb, buf, offset, range, FALSE, (void*)src);
}

_AVX afxCmdId AvxCmdDumpBuffer(avxCmdb cmdb, afxBuffer buf, afxNat offset, afxNat range, void* dst)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AfxAssert(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AfxAssert(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AfxAssert(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_SRC usage flag.
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));

    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of buf minus offset.
    AfxAssertRange(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AfxAssert(offset % sizeof(afxNat32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);

    /// the dst for the buffer dump, and must be at least range bytes in capacity.
    AfxAssert(dst);
    return cmdb->stdCmds->buf.rw(cmdb, buf, offset, range, TRUE, dst);
}
