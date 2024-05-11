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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_DRAW_STREAM_C
#include "qwadro/draw/afxDrawSystem.h"

// buffer ops

_AVX afxCmdId AfxCmdCopyBuffer(afxDrawStream diob, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &src, afxFcc_BUF);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(src, afxBufferAccess_R));
    AfxAssert(AfxTestBufferAccess(dst, afxBufferAccess_W));
    return diob->stdCmds->buf.cpy(diob, src, dst, opCnt, ops);
}

_AVX afxCmdId AfxCmdCopyBufferRegion(afxDrawStream diob, afxBuffer src, afxNat srcOffset, afxBuffer dst, afxNat dstOffset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
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
    return diob->stdCmds->buf.cpy(diob, src, dst, 1, &op);
}

_AVX afxCmdId AfxCmdFillBuffer(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, afxNat value)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    return diob->stdCmds->buf.set(diob, buf, offset, range, value);
}

_AVX afxCmdId AfxCmdClearBuffer(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    return diob->stdCmds->buf.set(diob, buf, offset, range, 0);
}

_AVX afxCmdId AfxCmdUpdateBuffer(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    AfxAssert(src);
    return diob->stdCmds->buf.rw(diob, buf, offset, range, FALSE, (void*)src);
}

_AVX afxCmdId AfxCmdDumpBuffer(afxDrawStream diob, afxBuffer buf, afxNat offset, afxNat range, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, range);
    AfxAssert(offset % sizeof(afxNat32) == 0);
    AfxAssert(range % sizeof(afxNat32) == 0);
    AfxAssert(range);
    AfxAssert(dst);
    return diob->stdCmds->buf.rw(diob, buf, offset, range, TRUE, dst);
}
