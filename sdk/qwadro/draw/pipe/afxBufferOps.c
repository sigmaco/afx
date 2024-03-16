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

#define _AFX_DRAW_C
#define _AFX_DRAW_SCRIPT_C
#include "qwadro/mem/afxArena.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/pipe/afxDrawScript.h"

// buffer ops

_AVX afxCmdId AfxCmdCopyBuffer(afxDrawScript dscr, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(src, afxBufferAccess_R));
    AfxAssert(AfxTestBufferAccess(dst, afxBufferAccess_W));
    return dscr->stdCmds->buf.cpy(dscr, src, dst, opCnt, ops);
}

_AVX afxCmdId AfxCmdCopyBufferRegion(afxDrawScript dscr, afxBuffer src, afxNat srcOffset, afxBuffer dst, afxNat dstOffset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(src, afxBufferAccess_R));
    AfxAssert(AfxTestBufferAccess(dst, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(src), srcOffset, range);
    AfxAssertRange(AfxGetBufferCapacity(dst), dstOffset, range);
    AfxAssert(srcOffset % sizeof(afxNat32) == 0);
    AfxAssert(dstOffset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    afxBufferCopyOp op;
    op.srcOffset = srcOffset;
    op.dstOffset = dstOffset;
    op.range = range;
    return dscr->stdCmds->buf.cpy(dscr, src, dst, 1, &op);
}

_AVX afxCmdId AfxCmdFillBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat range, afxNat value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    return dscr->stdCmds->buf.set(dscr, buf, offset, range, value);
}

_AVX afxCmdId AfxCmdClearBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    return dscr->stdCmds->buf.set(dscr, buf, offset, range, 0);
}

_AVX afxCmdId AfxCmdUpdateBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat range, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    AfxAssert(src);
    return dscr->stdCmds->buf.rw(dscr, buf, offset, range, FALSE, (void*)src);
}

_AVX afxCmdId AfxCmdDumpBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat range, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    AfxAssert(dst);
    return dscr->stdCmds->buf.rw(dscr, buf, offset, range, TRUE, dst);
}

_AVX afxCmdId AfxCmdStreamUpBuffer(afxDrawScript dscr, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    return dscr->stdCmds->buf.io(dscr, buf, opCnt, ops, FALSE, in);
}

_AVX afxCmdId AfxCmdStreamDownBuffer(afxDrawScript dscr, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));
    return dscr->stdCmds->buf.io(dscr, buf, opCnt, ops, TRUE, out);
}

_AVX afxCmdId AfxCmdUploadBuffer(afxDrawScript dscr, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    return dscr->stdCmds->buf.io(dscr, buf, opCnt, ops, FALSE, in);
}

_AVX afxCmdId AfxCmdDownloadBuffer(afxDrawScript dscr, afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));
    return dscr->stdCmds->buf.io(dscr, buf, opCnt, ops, TRUE, out);
}
