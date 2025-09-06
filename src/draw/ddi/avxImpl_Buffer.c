/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVX void _AvxDpuCopyBuffer(avxDpu* dpu, avxBuffer src, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    _AvxCopyBuffer(dst, opCnt, ops, src);
}

_AVX void _AvxDpuFillBuffer(avxDpu* dpu, avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data)
{
    afxError err = AFX_ERR_NONE;
    _AvxFillBuffer(buf, offset, range, data);
}

_AVX void _AvxDpuDumpBuffer(avxDpu* dpu, avxBuffer buf, afxByte* dst, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;
    _AvxDumpBuffer(buf, opCnt, ops, dst);
}

_AVX void _AvxDpuUpdateBuffer(avxDpu* dpu, avxBuffer buf, afxByte const* src, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;
    _AvxUpdateBuffer(buf, opCnt, ops, src);
}

_AVX void _AvxDpuDownloadBuffer(avxDpu* dpu, avxBuffer buf, afxStream out, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;
    _AvxDownloadBuffer(buf, opCnt, ops, out);
}

_AVX void _AvxDpuUploadBuffer(avxDpu* dpu, avxBuffer buf, afxStream in, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;
    _AvxUploadBuffer(buf, opCnt, ops, in);
}
