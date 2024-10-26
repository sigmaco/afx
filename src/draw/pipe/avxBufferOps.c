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
#include "../../dev/AvxImplKit.h"

// buffer ops

_AVX afxCmdId AvxCmdCopyBuffer(avxCmdb cmdb, afxBuffer src, afxBuffer dst, afxUnit opCnt, afxBufferCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// opCnt must be greater than 0.
    AFX_ASSERT(opCnt);

    /// src must be a valid afxBuffer handle.
    AfxAssertObjects(1, &src, afxFcc_BUF);
    /// dst must be a valid afxBuffer handle.
    AfxAssertObjects(1, &dst, afxFcc_BUF);

    /// src must have been created with afxBufferUsage_SRC usage flag.
    AFX_ASSERT(AfxTestBufferAccess(src, afxBufferFlag_R));
    /// dst must have been created with afxBufferUsage_DST usage flag.
    AFX_ASSERT(AfxTestBufferAccess(dst, afxBufferFlag_W));

    return cmdb->stdCmds->buf.cpy(cmdb, src, dst, opCnt, ops);
}

_AVX afxCmdId AvxCmdFillBuffer(avxCmdb cmdb, afxBuffer buf, afxUnit offset, afxUnit range, afxUnit value)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_DST usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));
    
    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    return cmdb->stdCmds->buf.set(cmdb, buf, offset, range, value);
}

_AVX afxCmdId AvxCmdClearBuffer(avxCmdb cmdb, afxBuffer buf, afxUnit offset, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_DST usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));

    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of dst minus offset.
    AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    return cmdb->stdCmds->buf.set(cmdb, buf, offset, range, 0);
}

_AVX afxCmdId AvxCmdUpdateBuffer(avxCmdb cmdb, afxBuffer buf, afxUnit offset, afxUnit range, void const* src)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    //AFX_ASSERT(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_DST usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));

    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of buf minus offset.
    AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    /// the src data for the buffer update, and must be at least range bytes in size.
    AFX_ASSERT(src);
    return cmdb->stdCmds->buf.rw(cmdb, buf, offset, range, FALSE, (void*)src);
}

_AVX afxCmdId AvxCmdDumpBuffer(avxCmdb cmdb, afxBuffer buf, afxUnit offset, afxUnit range, void* dst)
{
    afxError err = AFX_ERR_NONE;
    /// cmdb must be a valid avxCmdb handle.
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    /// cmdb must be in the recording state.
    AFX_ASSERT(cmdb->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a render pass instance.
    AFX_ASSERT(!cmdb->inRenderPass);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!cmdb->inVideoCoding);

    /// buf must be a valid afxBuffer handle.
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    /// buf must have been created with afxBufferUsage_SRC usage flag.
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_R));

    /// offset must be less than the size of buf.
    /// If range is not equal to zero, range must be less than or equal to the size of buf minus offset.
    AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), offset, range);

    /// offset must be a multiple of 4.
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    /// If range is not equal to zero, range must be a multiple of 4.
    AFX_ASSERT(range % sizeof(afxUnit32) == 0);
    AFX_ASSERT(range);

    /// the dst for the buffer dump, and must be at least range bytes in capacity.
    AFX_ASSERT(dst);
    return cmdb->stdCmds->buf.rw(cmdb, buf, offset, range, TRUE, dst);
}
