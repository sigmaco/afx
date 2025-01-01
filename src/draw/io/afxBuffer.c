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
#define _AVX_BUFFER_C
#include "../impl/avxImplementation.h"

_AVX afxDrawSystem AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    afxDrawSystem dsys = AfxGetProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX void* AfxGetBufferUdd(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->udd;
}

_AVX afxUnit AfxGetBufferCapacity(afxBuffer buf, afxUnit from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return AfxMin(buf->cap, buf->cap - from);
}

_AVX afxBufferUsage AfxGetBufferUsage(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->usage;
}

_AVX afxBufferUsage AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->usage & usage;
}

_AVX afxBufferFlags AfxGetBufferAccess(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->flags & afxBufferFlag_ACCESS_MASK;
}

_AVX afxBufferFlags AfxTestBufferAccess(afxBuffer buf, afxBufferFlags access)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return (buf->flags & afxBufferFlag_ACCESS_MASK) & access;
}

_AVXINL void _AvxSanitizeBufferCopy(afxBuffer src, afxBuffer dst, afxBufferCopy const* raw, afxBufferCopy* san)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &dst);

    san->srcOffset = AfxMin(raw->srcOffset, src->cap - 1);
    san->dstOffset = AfxMin(raw->dstOffset, dst->cap - 1);
    san->range = AfxClamp(raw->range, 1, AfxMin(src->cap - san->srcOffset, dst->cap - san->dstOffset));
}

_AVXINL void _AvxSanitizeBufferIo(afxBuffer src, afxBuffer dst, afxBufferIo const* raw, afxBufferIo* san)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &dst);

    san->srcOffset = AfxMin(raw->srcOffset, src->cap - 1);
    san->dstOffset = AfxMin(raw->dstOffset, dst->cap - 1);
    san->srcStride = AfxMin(raw->srcStride, src->cap - san->srcOffset);
    san->dstStride = AfxMin(raw->dstStride, dst->cap - san->dstOffset);
    san->rowCnt = AfxClamp(raw->rowCnt, 1, AfxMin(raw->rowCnt - src->cap / san->srcStride, raw->rowCnt - dst->cap / san->dstStride));
}

////////////////////////////////////////////////////////////////////////////////

_AVX void AfxUnmapBuffer(afxBuffer buf, afxBool wait)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    
    if (buf->remap)
    {
        if (buf->remap(buf, 0, 0, !!wait, NIL))
            AfxThrowError();
    }
    else
    {
        while (AfxLoadAtom32(&buf->pendingRemap))
            AfxYield();
        AfxIncAtom32(&buf->pendingRemap);

        if (buf->bytemap)
        {
            buf->bytemap = NIL;
            buf->mappedRange = 0;
            buf->mappedOffset = 0;
            buf->mappedFlags = NIL;
        }
        AfxDecAtom32(&buf->pendingRemap);
    }
}

_AVX afxError AfxMapBuffer(afxBuffer buf, afxUnit offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->cap, offset, range);
    AFX_ASSERT(AFX_IS_ALIGNED(offset, AFX_BUF_ALIGNMENT));

    offset = AFX_ALIGNED_SIZE(offset, AFX_BUF_ALIGNMENT);
    offset = AfxClamp(offset, 0, ((buf->cap / AFX_BUF_ALIGNMENT) - 1) * AFX_BUF_ALIGNMENT);

    if (!range)
        range = AfxMin(buf->cap, buf->cap - offset);

    range = AFX_ALIGNED_SIZE(range, AFX_BUF_ALIGNMENT);
    range = AfxClamp(range, AFX_BUF_ALIGNMENT, buf->cap - offset);

    AfxAssert2((offset % AFX_BUF_ALIGNMENT == 0), (range % AFX_BUF_ALIGNMENT == 0));

    while (AfxLoadAtom32(&buf->pendingRemap)) AfxYield();
    AfxIncAtom32(&buf->pendingRemap);

    void* ptr = NIL;

    if (!buf->bytemap)
    {
        if (buf->remap)
        {
            if (buf->remap(buf, offset, range, flags, &ptr))
            {
                AfxThrowError();
                buf->bytemap = NIL;
                buf->mappedOffset = 0;
                buf->mappedRange = 0;
                buf->mappedFlags = NIL;
            }
        }
        else
        {
            if (buf->data)
            {
                buf->mappedOffset = offset;
                buf->mappedRange = range;
                buf->mappedFlags = flags;

                buf->bytemap = buf->data;
                buf->bytemap += offset;

                ptr = buf->bytemap;
            }
        }

        AFX_ASSERT(buf->bytemap);
        AFX_ASSERT(buf->mappedRange);
        AFX_ASSERT(buf->mappedOffset + buf->mappedRange >= offset + range);
        AFX_ASSERT(buf->mappedOffset >= offset);
    }
    else if ((offset < buf->mappedOffset) || (offset + range > buf->mappedOffset + buf->mappedRange)) AfxThrowError();
    else
    {
        AFX_ASSERT(buf->mappedOffset + buf->mappedRange >= offset + range);
        AFX_ASSERT(buf->mappedOffset >= offset);
        ptr = &buf->bytemap[offset - buf->mappedOffset];
    }

    if (placeholder)
        *placeholder = ptr;

    AfxDecAtom32(&buf->pendingRemap);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AfxAssertAlign(op->srcOffset, AFX_BUF_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->srcOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
    }
#endif

    avxTransference transfer = { 0 };
    transfer.exuIdx = exuIdx;
    transfer.srcFcc = afxFcc_BUF;
    transfer.src.buf = buf;
    transfer.dst.dst = dst;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForDrawQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AfxAssertAlign(op->dstOffset, AFX_BUF_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->dstOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
    }
#endif

    avxTransference transfer = { 0 };
    transfer.exuIdx = exuIdx;
    transfer.dstFcc = afxFcc_BUF;
    transfer.dst.buf = buf;
    transfer.src.src = src;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForDrawQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AfxAssertAlign(op->dstOffset, AFX_BUF_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->dstOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
    }
#endif

    avxTransference transfer = { 0 };
    transfer.exuIdx = exuIdx;
    transfer.dstFcc = afxFcc_BUF;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.buf = buf;
    transfer.src.iob = in;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForDrawQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

_AVX afxError AfxDownloadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxUnit exuIdx, afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_R));

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AfxAssertAlign(op->srcOffset, AFX_BUF_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->srcOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
    }
#endif

    avxTransference transfer = { 0 };
    transfer.exuIdx = exuIdx;
    transfer.srcFcc = afxFcc_BUF;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.buf = buf;
    transfer.dst.iob = out;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxTransferVideoMemory(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForDrawQueue(dsys, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxBufStdDtorCb(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (buf->data)
    {
        AfxUnmapBuffer(buf, TRUE);
        AFX_ASSERT(!buf->data);
    }

    if (buf->src)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf->src);
        AfxDisposeObjects(1, &buf->src);
    }
    return err;
}

_AVX afxError _AvxBufStdCtorCb(afxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxBufferInfo const *bufi = ((afxBufferInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(bufi && bufi->cap && bufi->usage);

    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    buf->data = NIL;
    afxBuffer src = buf->src;

    if (src)
    {
        if (!AfxTestObjectFcc(src, afxFcc_BUF))
        {
            AfxThrowError();
            return err;
        }

        afxUnit srcCap = AfxGetBufferCapacity(src, 0);
        AFX_ASSERT_RANGE(srcCap, bufi->start, bufi->cap);

        if (bufi->start >= srcCap)
        {
            AfxThrowError();
            return err;
        }

        AFX_ASSERT(bufi->start == AFX_ALIGNED_SIZE(bufi->start, AFX_BUF_ALIGNMENT));

        if (!AFX_IS_ALIGNED(bufi->start, AFX_BUF_ALIGNMENT))
        {
            AfxThrowError();
            return err;
        }

        // capacity must be a multiply of pointer size.
        afxUnit bufCap = AFX_ALIGNED_SIZE(bufi->cap ? bufi->cap : srcCap - bufi->start, sizeof(void*));
        
        if ((bufi->cap > srcCap - bufi->start))
        {
            AfxThrowError();
            return err;
        }

        AfxReacquireObjects(1, &src);
        buf->src = src;
        buf->start = bufi->start;
        buf->cap = bufCap;
    }
    else
    {
        if (!bufi->cap)
        {
            AfxThrowError();
            return err;
        }

        buf->src = NIL;
        buf->start = 0;
        buf->cap = AFX_ALIGNED_SIZE(bufi->cap, sizeof(void*));
    }

    if (!err)
    {
        buf->usage = bufi->usage;
        buf->flags = bufi->flags ? bufi->flags : afxBufferFlag_W;
        buf->fmt = bufi->fmt;

        afxUnit exuCnt = 16; // TODO Get it from DSYS
        buf->sharingMask = NIL;

        for (afxUnit i = 0; i < exuCnt; i++)
        {
            buf->sharingMask |= bufi->sharingMask & AFX_BIT(i);
        }

        buf->udd = bufi->udd;

        buf->data = NIL;
        buf->bytemap = NIL;
        buf->mappedOffset = 0;
        buf->mappedRange = 0;
        buf->mappedFlags = NIL;

        buf->pendingRemap = 0;
        buf->remap = NIL;
    }
    return err;
}

_AVX afxClassConfig const _AVX_BUF_CLASS_CONFIG =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Unformatted Video Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxBuffer)),
    .ctor = (void*)_AvxBufStdCtorCb,
    .dtor = (void*)_AvxBufStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireBuffers(afxDrawSystem dsys, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!cnt || !infos || !buffers) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxBufferInfo const* info = &infos[i];
            
            if (!info->cap) AfxThrowError();
            else if (!info->usage)
                AfxThrowError();
        }

        if (!err)
        {
            afxClass* cls = (afxClass*)AvxGetBufferClass(dsys);
            AFX_ASSERT_CLASS(cls, afxFcc_BUF);

            if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dsys, (void*)infos }))
                AfxThrowError();
        }
    }
    return err;
}
