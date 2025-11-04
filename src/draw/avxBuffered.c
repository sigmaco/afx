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
#include "ddi/avxImplementation.h"

_AVXINL void* _AvxGetClientBufferData(avxBuffer buf, afxSize from)
{
    return &buf->storage[0].host.bytemap[buf->storageOffset + from];
}

_AVX void _AvxFillBuffer(avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data)
{
    afxError err = AFX_ERR_NONE;
    afxByte* dstp = _AvxGetClientBufferData(buf, 0);
    AfxFillPattern(&dstp[offset], range, &data, sizeof(data));
}

_AVX void _AvxCopyBuffer(avxBuffer buf, afxUnit opCnt, avxBufferCopy const ops[], avxBuffer src)
{
    afxError err = AFX_ERR_NONE;

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

_AVX void _AvxUpdateBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxByte const* src)
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

_AVX void _AvxDownloadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream out)
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

_AVX void _AvxUploadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;

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

_AVX void AvxMakeBufferedRing(avxBufferedRing* rng, afxUnit rounds, afxUnit blockSiz, afxUnit blockAlign, avxBuffer buf, afxSize bufCap, void* mapped)
{
    // Small buffered ring utility.
    // Reuse memory across frames (no new buffer creation).
    // Efficient for dynamic scenes and many objects.
    // Prevents CPU-GPU sync stalls and fragmentation.

    afxError err;
    AFX_ASSERT(rng);
    *rng = (avxBufferedRing) { 0 };

    // Use triple buffering (rounds = 3) to rotate through buffer regions.
    rng->rounds = AFX_MAX(1, rounds);
    rng->blockAlign = AFX_ALIGN_SIZE(AFX_MIN(AVX_BUFFER_ALIGNMENT, blockAlign), AVX_BUFFER_ALIGNMENT);
    rng->blockSiz = AFX_MAX(rng->blockAlign, AVX_ALIGN_BUFFERED(blockSiz));

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    rng->buf = buf;
    rng->maxSiz = bufCap;
    rng->blockCnt = rng->maxSiz / rng->blockSiz;
    rng->currOffset = 0;

    // With persistent and coherent mapping, you don't need glFenceSync() unless extremely fine-grained sync is required.
    rng->basePtr = NIL;
    if (mapped) rng->basePtr = mapped;
    else if (AvxMapBuffer(buf, 0, bufCap, NIL, (void**)&rng->basePtr))
        AfxThrowError();
}

_AVX afxSize AvxCycleBufferedRing(avxBufferedRing* rng)
{
    afxError err;
    AFX_ASSERT(rng);
    rng->currOffset = (rng->currOffset + rng->blockSiz * rng->blockCnt / rng->rounds) % rng->maxSiz;
    return rng->currOffset;
}

_AVX void* AvxAdvanceBufferedRing(avxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange)
{
    afxError err;
    AFX_ASSERT(rng);

    reqSiz = AFX_ALIGN_SIZE(reqSiz, rng->blockSiz);

    if (rng->currOffset + reqSiz > rng->maxSiz)
    {
        // Wrap around
        rng->currOffset = 0;
    }

    void* blockPtr = &rng->basePtr[rng->currOffset];

    // Bind for shader access
    // You now write directly into blockPtr and the shader sees the bound block via layout(std140, binding = %).
    if (pOffset) *pOffset = rng->currOffset;
    if (pRange) *pRange = reqSiz;

    rng->currOffset += reqSiz;

    return blockPtr;
}

