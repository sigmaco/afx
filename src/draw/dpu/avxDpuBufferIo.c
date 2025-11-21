/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#include "../avxIcd.h"

_AVXINL void* _AvxGetClientBufferData(avxBuffer buf, afxSize from)
{
    return &buf->storage[0].host.bytemap[buf->storageOffset + from];
}

_AVX void _AvxFillBuffer(avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data)
{
    afxError err = { 0 };
    afxByte* dstp = _AvxGetClientBufferData(buf, 0);
    AfxFillPattern(&dstp[offset], range, &data, sizeof(data));
}

_AVX void _AvxCopyBuffer(avxBuffer buf, afxUnit opCnt, avxBufferCopy const ops[], avxBuffer src)
{
    afxError err = { 0 };

    afxByte* dstp = _AvxGetClientBufferData(buf, 0);
    afxByte const* srcp = _AvxGetClientBufferData(src, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferCopy const* op = &ops[i];
        AfxCopy(&dstp[op->dstOffset], &srcp[op->srcOffset], op->range);
    }
}

_AVX void _AvxDumpBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxByte* dst)
{
    afxError err = { 0 };

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

_AVX void _AvxUpdateBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxByte const* src)
{
    afxError err = { 0 };

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

_AVX void _AvxDownloadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream out)
{
    afxError err = { 0 };

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

_AVX void _AvxUploadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream in)
{
    afxError err = { 0 };

    afxByte* dstp = _AvxGetClientBufferData(buf, 0);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->dstStride, AvxGetBufferCapacity(buf, op->dstOffset));

        afxByte* dstAt = &dstp[op->dstOffset];

        if (op->dstStride != op->srcStride)
            AfxReadStreamAt2(in, op->srcOffset, bufRange, op->srcStride, dstAt, op->dstStride);
        else
            AfxReadStreamAt(in, op->srcOffset, bufRange, 0, dstAt);
    }
}

_AVX void _AvxDpuCopyBuffer(avxDpu* dpu, avxBuffer src, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[])
{
    afxError err = { 0 };
    _AvxCopyBuffer(dst, opCnt, ops, src);
}

_AVX void _AvxDpuFillBuffer(avxDpu* dpu, avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data)
{
    afxError err = { 0 };
    _AvxFillBuffer(buf, offset, range, data);
}

_AVX void _AvxDpuDumpBuffer(avxDpu* dpu, avxBuffer buf, afxByte* dst, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    _AvxDumpBuffer(buf, opCnt, ops, dst);
}

_AVX void _AvxDpuUpdateBuffer(avxDpu* dpu, avxBuffer buf, afxByte const* src, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    _AvxUpdateBuffer(buf, opCnt, ops, src);
}

_AVX void _AvxDpuDownloadBuffer(avxDpu* dpu, avxBuffer buf, afxStream out, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    _AvxDownloadBuffer(buf, opCnt, ops, out);
}

_AVX void _AvxDpuUploadBuffer(avxDpu* dpu, avxBuffer buf, afxStream in, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    _AvxUploadBuffer(buf, opCnt, ops, in);
}
