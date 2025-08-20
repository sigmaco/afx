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
#include "../ddi/avxImplementation.h"

#define _AVX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED TRUE

_AVX afxDrawSystem AvxGetBufferProvider(avxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    afxDrawSystem dsys = AfxGetProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX void* AvxGetBufferUdd(avxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->udd;
}

_AVX afxSize AvxGetBufferCapacity(avxBuffer buf, afxSize from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return AFX_MIN(buf->reqSiz, buf->reqSiz - from);
}

_AVX avxBufferUsage AvxGetBufferUsage(avxBuffer buf, avxBufferUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return usage ? (buf->usage & usage) : buf->usage;
}

_AVX avxBufferFlags AvxGetBufferAccess(avxBuffer buf, avxBufferFlags access)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return access ? ((buf->flags & avxBufferFlag_ACCESS) & access) : access;
}

_AVXINL void _AvxSanitizeBufferCopy(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferCopy const raw[], avxBufferCopy san[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxSize bufCap = buf->reqSiz;
    afxSize srcBufCap = src->reqSiz;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferCopy* s = &san[i];
        avxBufferCopy const* r = &raw[i];
        s->srcOffset = AFX_MIN(r->srcOffset, srcBufCap - 1);
        s->dstOffset = AFX_MIN(r->dstOffset, bufCap - 1);
        s->range = AFX_CLAMP(r->range, 1, AFX_MIN(srcBufCap - s->srcOffset, bufCap - s->dstOffset));
    }
}

_AVXINL void _AvxSanitizeBufferIo(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferIo const raw[], avxBufferIo san[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxSize bufCap = buf->reqSiz;
    afxSize srcBufCap = src->reqSiz;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferIo* s = &san[i];
        avxBufferIo const* r = &raw[i];
        s->srcOffset = AFX_MIN(r->srcOffset, srcBufCap - 1);
        s->dstOffset = AFX_MIN(r->dstOffset, bufCap - 1);
        s->srcStride = AFX_MIN(r->srcStride, srcBufCap - s->srcOffset);
        s->dstStride = AFX_MIN(r->dstStride, bufCap - s->dstOffset);
        s->rowCnt = AFX_CLAMP(r->rowCnt, 1, AFX_MIN(r->rowCnt - srcBufCap / s->srcStride, r->rowCnt - bufCap / s->dstStride));
    }
}

////////////////////////////////////////////////////////////////////////////////

_AVX void* AvxGetBufferMap(avxBuffer buf, afxSize offset, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxSize mappedOffset = buf->storage[0].mapOffset;
    afxSize mappedRange = buf->storage[0].mapRange;

    // Validate if the requested region is within the currently mapped region
    if ((offset < mappedOffset) || ((offset + range) > (mappedOffset + mappedRange)))
        return NIL;

    // If the region is valid, set the placeholder to the corresponding address
    AFX_ASSERT(mappedOffset + mappedRange >= offset + range);
    AFX_ASSERT(mappedOffset <= offset);    
    afxSize offDiff = offset - mappedOffset;

    AFX_ASSERT(buf->storage[0].mapPtr);
    return &buf->storage[0].mapPtr[offDiff];
}

_AVX afxError AvxUnmapBuffer(avxBuffer buf, afxBool wait)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_RW));

    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    avxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = buf->storage[0].mapOffset;
    map.range = buf->storage[0].mapRange;
    map.flags = buf->storage[0].mapFlags;

    if (AvxUnmapBuffers(dsys, 1, &map))
        AfxThrowError();

    return err;
}

_AVX afxError AvxMapBuffer(avxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_RW));
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AVX_BUFFER_ALIGNMENT));

    avxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    void* holder;
    if (AvxMapBuffers(dsys, 1, &map, (void**[]) { &holder }))
        AfxThrowError();

    // TODO(?): If no placeholder is specified, do not wait for completion.

    AFX_ASSERT(placeholder);
    *placeholder = holder;

    return err;
}

_AVX afxError AvxCohereMappedBuffer(avxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, afxBool discard)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AVX_BUFFER_ALIGNMENT));

    avxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (AvxCohereMappedBuffers(dsys, discard, 1, &map))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxDumpBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], void* dst, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if AVX_VALIDATION_ENABLED
    // Validate the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AVX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), op->srcOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AVX_VALIDATION_ENABLED

    // Prepare the transfer structure for the dump operation
    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_BUF;
    transfer.src.buf = buf;
    transfer.dst.dst = dst;

    // Get the draw system context associated with the buffer
    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Perform the transfer (dump) of data
    if (_AvxDsysGetImpl(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        // If the transfer fails, throw an error
        AfxThrowError();
        return err;
    }

    // Ensure that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the draw queue to finish processing the dump
    if (AvxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

_AVX afxError AvxUpdateBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], void const* src, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_W));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

#if AVX_VALIDATION_ENABLED
    // Verify that all operations are valid (this block only runs if validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AVX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), op->dstOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AVX_VALIDATION_ENABLED

    // Prepare the transfer structure
    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_BUF;
    transfer.dst.buf = buf;
    transfer.src.src = src;

    // Get the draw system associated with the buffer
    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Perform the data transfer (buffer update)
    if (_AvxDsysGetImpl(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        // If transfer fails, throw an error
        AfxThrowError();
        return err;
    }

    // Ensure the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the draw queue to finish processing (commented out)
    if (AvxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxUploadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream in, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_W));

#if AVX_VALIDATION_ENABLED
    // Validation of the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AVX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), op->dstOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AVX_VALIDATION_ENABLED

    // Prepare a transfer structure for the operation
    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_BUF;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.buf = buf;
    transfer.src.iob = in;

    // Get the draw system (context) associated with the buffer
    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Perform the actual transfer of data
    if (_AvxDsysGetImpl(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    // Assert that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the draw queue to finish processing
    if (AvxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

_AVX afxError AvxDownloadBuffer(avxBuffer buf, afxUnit opCnt, avxBufferIo const ops[], afxStream out, afxMask exuMask)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_R));

#if AVX_VALIDATION_ENABLED
    // Validate the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AVX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), op->srcOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AVX_VALIDATION_ENABLED

    // Prepare the transfer structure for the download operation
    avxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_BUF;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.buf = buf;
    transfer.dst.iob = out;

    // Get the draw system context associated with the buffer
    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Perform the transfer (download) of data
    if (_AvxDsysGetImpl(dsys)->transferCb(dsys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    // Ensure that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AvxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxBufDtorCb(avxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    while (buf->storage[0].mapPtr)
    {
        AvxUnmapBuffer(buf, TRUE);
        AfxYield();
        //AFX_ASSERT(!buf->bytemap);
    }

    if (buf->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf->base);
        AfxDisposeObjects(1, &buf->base);
    }

    //afxDrawSystem dsys = AfxGetProvider(buf);
    if (_AvxDsysGetImpl(dsys)->deallocBufCb(dsys, 1, &buf))
    {
        AfxThrowError();
    }

    return err;
}

_AVX afxError _AvxBufCtorCb(avxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AvxGetBufferProvider(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxBufferInfo const *bufi = args[1] ? ((avxBufferInfo const *)args[1]) + invokeNo : NIL;
    AFX_ASSERT(bufi && bufi->size && bufi->usage);
    avxSubbufferInfo const* sub = args[2] ? ((avxSubbufferInfo const *)args[2]) + invokeNo : NIL;

    buf->tag = bufi->tag;
    buf->udd = bufi->udd;

    if (sub)
    {
        avxBuffer base = sub->buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &base);

        if (base->base && (base->base != base))
        {
            // Base buffer must be a storage buffer, not a buffer view.
            AFX_ASSERT(!base->base);
            AfxThrowError();
            return err;
        }

        afxSize from = sub->from;
        afxSize range = sub->range;
        afxSize srcCap = AvxGetBufferCapacity(base, 0);
        AFX_ASSERT_RANGE(srcCap, from, range);

        if (!AFX_TEST_ALIGNMENT(from, AVX_BUFFER_ALIGNMENT))
        {
            AFX_ASSERT_ALIGNMENT(from, AVX_BUFFER_ALIGNMENT);
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        // If a capacity is not specified, the new buffer inherits the full capacity of the base buffer, 
        // excluding the portion displaced by @from.
        if (range == 0)
            range = srcCap - from;
        else
            range = AFX_ALIGN_SIZE(range, AFX_SIMD_ALIGNMENT);

        // As every buffer capacity is a power of AFX_SIMD_ALIGNMENT, it should already be aligned here.
        AFX_ASSERT_ALIGNMENT(range, AFX_SIMD_ALIGNMENT);

        if ((from >= srcCap) ||
            (range > srcCap) ||
            (range > srcCap - from))
        {
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }
        AfxReacquireObjects(1, &base);

        buf->reqSiz = range;
        buf->fmt = sub->fmt;

        buf->usage = base->usage;
        buf->flags = base->flags;
        buf->sharingMask = base->sharingMask;

        buf->storage[0] = base->storage[0];
        buf->storageOffset = base->storageOffset;

        return err;
    }

    buf->base = NIL;
    buf->from = 0;
    buf->fmt = bufi->fmt;

    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    buf->usage = bufi->usage;
    buf->flags = bufi->flags;

    afxSize size = bufi->size;

    if (!size)
    {
        // If there is not a @base buffer, we need a size here.
        AfxThrowError();
        return err;
    }
    // If there is not a @base buffer, @from is useless and should be zero.
    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    // All buffer mapping requires ranges aligned to AFX_SIMD_ALIGNMENT. This alignment is ensured at AFX level.
    size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);
    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);
    buf->reqSiz = size;

    if (err) return err;

    afxUnit exuCnt = 16; // TODO Get it from DSYS
    buf->sharingMask = NIL;
    for (afxUnit i = 0; i < exuCnt; i++)
    {
        buf->sharingMask |= bufi->sharingMask & AFX_BITMASK(i);
    }

    // STORAGE
    buf->reqMemType = NIL;
    buf->reqAlign = AVX_BUFFER_ALIGNMENT;

    // binding
    buf->storage[0].mmu = 0;
    buf->storageOffset = 0;
    buf->storage[0].host.addr = NIL;

    buf->storage[0].mapPtr = NIL;
    buf->storage[0].mapOffset = 0;
    buf->storage[0].mapRange = 0;
    buf->storage[0].mapFlags = NIL;
    buf->storage[0].mapRefCnt = 1;

    buf->storage[0].pendingRemap = 0;
    buf->storage[0].permanentlyMapped = !!bufi->mapped;

    return err;
}

_AVX afxClassConfig const _AVX_BUF_CLASS_CONFIG =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Video Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(avxBuffer)),
    .ctor = (void*)_AvxBufCtorCb,
    .dtor = (void*)_AvxBufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AFX_ASSERT(cnt);
    AFX_ASSERT(infos);
    AFX_ASSERT(buffers);
    if (!cnt || !infos || !buffers)
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferInfo const* info = &infos[i];

        if (!info->usage)
        {
            AFX_ASSERT(info->usage);
            AfxThrowError();
        }

        if (!info->size)
        {
            AFX_ASSERT(!info->size);
            AfxThrowError();
        }
    }

    if (err) return err;

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->bufCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dsys, (void*)infos, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);

    if (_AvxDsysGetImpl(dsys)->allocBufCb(dsys, cnt, infos, buffers))
    {
        AfxDisposeObjects(cnt, buffers);
        AfxThrowError();
        return err;
    }

#ifdef AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];
        avxBufferInfo const* bufi = &infos[i];

        AFX_ASSERT(buf->reqSiz >= bufi->size);
        AFX_ASSERT((buf->flags & bufi->flags) == bufi->flags);
        AFX_ASSERT(buf->sharingMask == bufi->sharingMask);
        AFX_ASSERT(buf->udd == bufi->udd);
        AFX_ASSERT((buf->usage & bufi->usage) == bufi->usage);
        AFX_ASSERT(buf->tag.start == bufi->tag.start);
    }
#endif

    // Proceed to permanently map and/or upload initial data.
    // We can't do it in constructor callback because ICD callbacks are called after the AVX one,
    // so it would be never ready to be mapped at device side.

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];
        avxBufferInfo const* bufi = &infos[i];

        if (bufi->mapped)
        {
            buf->storage[0].permanentlyMapped = TRUE;

            void* ptr;
            if (AvxMapBuffer(buf, 0, buf->reqSiz, NIL, &ptr))
            {
                AfxThrowError();
                break;
            }
            AFX_ASSERT(ptr == buf->storage[0].mapPtr);

            if (bufi->dataSiz)
            {
                AFX_ASSERT(bufi->data);
                AFX_ASSERT(ptr == buf->storage[0].mapPtr);
                AfxCopy(ptr, bufi->data, bufi->dataSiz);

                // Unmapping should do nothing in a persistently mapped buffer.
                // But we will keep it here for good didactic reasons.
                AvxUnmapBuffer(buf, 0);
            }
        }
        else
        {
            if (bufi->dataSiz)
            {
                AFX_ASSERT(bufi->data);
                avxBufferIo iop = { 0 };
                iop.srcStride = bufi->dataSiz;
                iop.rowCnt = 1;
                if (AvxUpdateBuffer(buf, 1, &iop, bufi->data, 0))
                {
                    AfxThrowError();
                    break;
                }
            }
        }
    }

    if (err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);
        AfxDisposeObjects(cnt, buffers);
    }

    return err;
}

_AVX afxError AvxReacquireBuffers(afxDrawSystem dsys, afxUnit cnt, avxSubbufferInfo const infos[], avxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AFX_ASSERT(cnt);
    AFX_ASSERT(infos);
    AFX_ASSERT(buffers);
    if (!cnt || !infos || !buffers)
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxSubbufferInfo const* info = &infos[i];

        if (!info->buf)
        {
            AFX_ASSERT(info->buf);
            AfxThrowError();
        }
    }

    if (err) return err;

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->bufCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dsys, NIL, (void*)infos }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);

#ifdef AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];
        avxSubbufferInfo const* bufi = &infos[i];

        AFX_ASSERT(buf->base == bufi->buf);
        AFX_ASSERT(buf->reqSiz >= bufi->range);
        AFX_ASSERT((buf->flags & bufi->flags) == bufi->flags);
        AFX_ASSERT(buf->fmt == bufi->fmt);
        AFX_ASSERT(buf->from == bufi->from);
        AFX_ASSERT(buf->udd == bufi->udd);
        AFX_ASSERT(buf->tag.start == bufi->tag.start);
    }
#endif

    return err;
}

_AVX afxError AvxMapBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferedMap maps[], void** placeholders[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Ensure that the operation count doesn't exceed 32
    AFX_ASSERT(cnt <= AVX_MAX_VERTEX_SOURCES);
    // Temporary array for mapping operations
    _avxBufferRemapping remaps2[AVX_MAX_VERTEX_SOURCES];
    // Count of operations to be processed
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedMap const* map = &maps[i];
        avxBuffer buf = map->buf;
        
        // Skip if the buffer is NULL
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // Get the buffer's capacity
        afxSize bufCap = AvxGetBufferCapacity(buf, 0);

        afxUnit range = map->range;
        // range always is SIMD aligned.
        range = AFX_ALIGN_SIZE(range, AFX_SIMD_ALIGNMENT);
        AFX_ASSERT_ALIGNMENT(range, AFX_SIMD_ALIGNMENT);
        
        afxSize offset = map->offset;
        AFX_ASSERT_ALIGNMENT(offset, AVX_BUFFER_ALIGNMENT);
        // offset must be aligned to AVX_BUFFER_ALIGNMENT.

        // Perform validation if enabled
#ifdef AVX_VALIDATION_ENABLED
        AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_RW));

        if (bufCap < map->offset + map->range)
        {
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }
        
        // Check that the buffer context matches the draw system.
        afxDrawSystem dsys2 = AvxGetBufferProvider(buf);
        if (dsys2 != dsys)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);
            AFX_ASSERT(dsys2 == dsys);
            AfxThrowError();
            break;
        }

        // Validate offset and range
        AFX_ASSERT_RANGE(bufCap, offset, range);

#endif//AVX_VALIDATION_ENABLED

        // If range is 0, calculate it based on the buffer's capacity
        if (!range) range = AFX_MIN(bufCap, bufCap - offset);

        // Wait for any pending mapping operation on the buffer
        while (AfxLoadAtom32(&buf->storage[0].pendingRemap)) AfxYield();
        // Signal an ongoing mapping operation
        AfxIncAtom32(&buf->storage[0].pendingRemap);

        // If the buffer has already been mapped, check if the region is valid
        if (buf->storage[0].mapPtr)
        {
            AFX_ASSERT_RANGE(buf->reqSiz, buf->storage[0].mapOffset, buf->storage[0].mapRange);

            // Validate if the requested region is within the currently mapped region
            if ((offset < buf->storage[0].mapOffset) || ((offset + range) > (buf->storage[0].mapOffset + buf->storage[0].mapRange)))
            {
                AfxThrowError();
                AfxDecAtom32(&buf->storage[0].pendingRemap);
                break;
            }
            else
            {
                // If the region is valid, set the placeholder to the corresponding address
                AFX_ASSERT(buf->storage[0].mapOffset + buf->storage[0].mapRange >= offset + range);
                AFX_ASSERT(buf->storage[0].mapOffset <= offset);
                ++buf->storage[0].mapRefCnt;
                afxUnit offDiff = offset - buf->storage[0].mapOffset;
                AFX_ASSERT(placeholders);
                *placeholders[i] = &buf->storage[0].mapPtr[offDiff];
            }

            // Decrement the pending remap count as no new operation was added
            AfxDecAtom32(&buf->storage[0].pendingRemap);
        }
        // If buffer is host-side allocated, map directly (just do it here)
        else if (buf->storage[0].host.addr)
        {
#ifndef _AVX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED
            buf->storage[0].mapOffset = offset;
            buf->storage[0].mapRange = range;
            buf->storage[0].mapFlags = map->flags;
            buf->storage[0].mapPtr = &buf->storage[0].hostedAlloc.bytemap[offset];

            AFX_ASSERT(placeholders);
            *placeholders[i] = buf->storage[0].mapPtr;
#else
            // Always map the whole buffer when host-side allocated.
            buf->storage[0].mapRefCnt = 1;
            buf->storage[0].mapOffset = 0;
            buf->storage[0].mapRange = buf->reqSiz;
            buf->storage[0].mapFlags = map->flags;
            buf->storage[0].mapPtr = buf->storage[0].host.bytemap;

            AFX_ASSERT(placeholders);
            *placeholders[i] = &buf->storage[0].mapPtr[map->offset];
#endif
            // Decrement the pending remap count as no new operation was added
            AfxDecAtom32(&buf->storage[0].pendingRemap);
        }
        else
        {
            // Buffer needs to be mapped through a draw system operation
            remaps2[opCnt].buf = buf;
            remaps2[opCnt].offset = offset;
            remaps2[opCnt].range = range;
            remaps2[opCnt].flags = map->flags;
            remaps2[opCnt].unmap = FALSE;
            remaps2[opCnt].placeholder = placeholders[i];
            AFX_ASSERT(remaps2[opCnt].placeholder);
            opCnt++;

            //AfxDecAtom32(&buf->pendingRemap);
        }
    }

    // If an error occurred or no operations are left to process, return the error
    if (err || !opCnt)
        return err;

    if (_AvxDsysGetImpl(dsys)->remapCb(dsys, FALSE, opCnt, remaps2))
    {
        AfxThrowError();
    }

    // Final validation and cleaning up pending remaps for each buffer.
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBuffer buf = remaps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

#if AVX_VALIDATION_ENABLED
        // Perform final validation on the buffer after remapping.
        AFX_ASSERT(buf->storage[0].mapPtr);
        AFX_ASSERT(buf->storage[0].mapRange >= remaps2[i].range);
        AFX_ASSERT(buf->storage[0].mapOffset >= remaps2[i].offset);
        AFX_ASSERT((buf->storage[0].mapOffset + buf->storage[0].mapRange) >= (remaps2[i].offset + remaps2[i].range));
        AFX_ASSERT_RANGE(buf->reqSiz, buf->storage[0].mapOffset, buf->storage[0].mapRange);
#endif//AVX_VALIDATION_ENABLED
        
        // Decrement the pending remap counter for each buffer.
        AfxDecAtom32(&buf->storage[0].pendingRemap);
    }

    // Return the error code (still AFX_ERR_NONE if no error occurred).
    return err;
}

_AVX afxError AvxUnmapBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferedMap maps[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    /*
        This function does not accept flags. Instead, any behavior wanted in unmapping must be specified in mapping operation.
    */

    // Ensure that the operation count doesn't exceed 32
    AFX_ASSERT(cnt <= AVX_MAX_VERTEX_SOURCES);
    _avxBufferRemapping unmaps2[AVX_MAX_VERTEX_SOURCES];
    // Count of buffers to unmap.
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedMap const* map = &maps[i];
        avxBuffer buf = map->buf;

        // Skip if the buffer is NULL
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // Can not be unmapped.
        if (buf->storage[0].permanentlyMapped) continue;

#if AVX_VALIDATION_ENABLED
        AFX_ASSERT(AvxGetBufferAccess(buf, avxBufferFlag_RW));

        // Validate the draw system context of the buffer
        // Ensure the buffer is associated with the current draw system
        afxDrawSystem dsys2 = AvxGetBufferProvider(buf);
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);

        if (dsys2 != dsys)
        {
            AFX_ASSERT(dsys2 == dsys);
            // If the buffer is not associated with the correct draw system, throw an error
            AfxThrowError();
            break;
        }
#endif//AVX_VALIDATION_ENABLED

        // Wait for any ongoing mapping operation to complete before unmapping.
        // Yield if there are pending remaps.
        while (AfxLoadAtom32(&buf->storage[0].pendingRemap)) AfxYield();
        // Indicate that an unmap operation is ongoing.
        AfxIncAtom32(&buf->storage[0].pendingRemap);

        if (buf->storage[0].mapPtr)
        {
            AFX_ASSERT(buf->storage[0].mapRefCnt);

            if (1 == buf->storage[0].mapRefCnt)
            {
                --buf->storage[0].mapRefCnt;
                AFX_ASSERT(0 == buf->storage[0].mapRefCnt);

                // If the buffer is host-side allocated (i.e., directly accessible in CPU memory)
                if (buf->storage[0].host.addr)
                {
                    // Clear the mapped state for host-side buffers
                    buf->storage[0].mapOffset = 0;
                    buf->storage[0].mapRange = 0;
                    buf->storage[0].mapFlags = NIL;
                    buf->storage[0].mapPtr = NIL;
                    // Decrement the pending remap counter
                    AfxDecAtom32(&buf->storage[0].pendingRemap);
                }
                else
                {
                    // If the buffer is not host-side allocated, prepare it for an unmap operation
                    unmaps2[opCnt].buf = buf;
                    unmaps2[opCnt].offset = 0;
                    unmaps2[opCnt].range = 0;
                    unmaps2[opCnt].flags = NIL;
                    unmaps2[opCnt].unmap = TRUE;
                    unmaps2[opCnt].placeholder = NIL;
                    opCnt++;
                }
            }
        }
    }

    // If no buffers need to be unmapped or if an error occurred, return the error code
    if (err || !opCnt)
        return err;

    if (_AvxDsysGetImpl(dsys)->remapCb(dsys, TRUE, cnt, unmaps2))
    {
        AfxThrowError();
    }

    // Final cleanup for unmapped buffers
    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBuffer buf = unmaps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

#if AVX_VALIDATION_ENABLED
        // Some final validation checks (if validation is enabled)

#if 0 // if no waited, these assertions will always fail.
        // some assertions
        AFX_ASSERT(!buf->bytemap);
        AFX_ASSERT(!buf->mappedRange);
        AFX_ASSERT(!buf->mappedOffset);
#endif

#endif//AVX_VALIDATION_ENABLED

        // Remove the pending remap state for the unmapped buffer
        AfxDecAtom32(&buf->storage[0].pendingRemap);
    }

    // Return the error code (AFX_ERR_NONE if successful)
    return err;
}

_AVX afxError AvxCohereMappedBuffers(afxDrawSystem dsys, afxBool discard, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

#if AVX_VALIDATION_ENABLED
    // Perform validation for each buffer in the maps
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedMap const* map = &maps[i];
        avxBuffer buf = map->buf;

        // Skip if the buffer is invalid
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // Ensure the map's range is within the buffer's capacity
        afxSize bufCap = AvxGetBufferCapacity(buf, 0);
        if (bufCap < map->offset + map->range)
        {
            // If the range is out of bounds, throw an error
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        // Ensure the buffer belongs to the same draw system as the provided one
        afxDrawSystem dsys2 = AvxGetBufferProvider(buf);
        if (dsys2 != dsys)
        {
            // If the buffer belongs to a different system, throw an error
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);
            AFX_ASSERT(dsys2 == dsys);
            AfxThrowError();
            break;
        }
    }

    // If any error has occurred during validation, return the error code
    if (err)
        return err;
#endif//AVX_VALIDATION_ENABLED

    // If no ICD callback is provided, use a draw queue to perform the flush/invalidate operation.
    if (_AvxDsysGetImpl(dsys)->cohereCb(dsys, discard, cnt, maps))
    {
        // If flushing fails, throw an error
        AfxThrowError();
    }
    // Return the error code (AFX_ERR_NONE if successful)
    return err;
}
