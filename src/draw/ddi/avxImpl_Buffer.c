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

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVXINL void* _AvxGetClientBufferData(avxBuffer buf, afxSize from)
{
    return &buf->storage[0].host.bytemap[buf->storageOffset + from];
}

_AVX void _AvxDpuCopyBuffer(avxDpu* dpu, avxBuffer src, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[])
{
    afxError err = AFX_ERR_NONE;

    afxByte* dstp = _AvxGetClientBufferData(dst, 0);
    afxByte const* srcp = _AvxGetClientBufferData(src, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferCopy const* op = &ops[i];
        AfxCopy(&dstp[op->dstOffset], &srcp[op->srcOffset], op->range);
    }
}

_AVX void _AvxDpuFillBuffer(avxDpu* dpu, avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data)
{
    afxError err = AFX_ERR_NONE;
    afxByte* dstp = _AvxGetClientBufferData(buf, 0);
    AfxFillPattern(&dstp[offset], range, &data, sizeof(data));
}

_AVX void _AvxDpuDumpBuffer(avxDpu* dpu, avxBuffer buf, afxByte* dst, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;

    afxByte const* srcp = _AvxGetClientBufferData(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->srcStride, AvxGetBufferCapacity(buf, op->srcOffset));

        if (op->srcStride != op->dstStride)
        {
            // strided
            AfxStream2(op->rowCnt, &srcp[op->srcOffset], op->srcStride, &dst[op->dstOffset], op->dstStride);
        }
        else
        {
            // sequential
            AfxCopy(&dst[op->dstOffset], &srcp[op->srcOffset], bufRange);
        }
    }
}

_AVX void _AvxDpuUpdateBuffer(avxDpu* dpu, avxBuffer buf, afxByte const* src, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;

    afxByte* dstp = _AvxGetClientBufferData(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->dstStride, AvxGetBufferCapacity(buf, op->dstOffset));

        if (op->srcStride == op->dstStride)
        {
            // sequential
            AfxCopy(&dstp[op->dstOffset], &src[op->srcOffset], bufRange);
        }
        else
        {
            // strided
            AfxStream2(op->rowCnt, &src[op->srcOffset], op->srcStride, &dstp[op->dstOffset], op->dstStride);
        }
    }
}

_AVX void _AvxDpuDownloadBuffer(avxDpu* dpu, avxBuffer buf, afxStream out, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;

    afxByte const* srcp = _AvxGetClientBufferData(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->srcStride, AvxGetBufferCapacity(buf, op->srcOffset));

        afxByte const* srcAt = &srcp[op->srcOffset];

        if (op->dstStride != op->srcStride)
            AfxWriteStreamAt2(out, op->dstOffset, bufRange, op->dstStride, srcAt, op->srcStride);
        else
            AfxWriteStreamAt(out, op->dstOffset, bufRange, 0, srcAt);
    }
}

_AVX void _AvxDpuUploadBuffer(avxDpu* dpu, avxBuffer buf, afxStream in, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = AFX_ERR_NONE;

    afxByte* dstp = _AvxGetClientBufferData(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->dstStride, AvxGetBufferCapacity(buf, op->dstOffset));

        afxByte const* dstAt = &dstp[op->dstOffset];

        if (op->dstStride != op->srcStride)
            AfxReadStreamAt2(in, op->srcOffset, bufRange, op->srcStride, dstAt, op->dstStride);
        else
            AfxReadStreamAt(in, op->srcOffset, bufRange, 0, dstAt);
    }
}
