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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_BUFFER_C
#include "amxIcd.h"

extern unsigned char rawData[1095920];

_AMX afxMixSystem AmxGetBufferHost(amxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    afxMixSystem msys = AfxGetHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMX void* AmxGetBufferUdd(amxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->udd;
}

_AMX afxSize AmxGetBufferAddress(amxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    return buf->storage[0].hostedAlloc.addr + buf->storage[0].offset;
}

_AMX afxSize AmxGetBufferCapacity(amxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    return AFX_MIN(buf->reqSiz, buf->reqSiz - from);
}

_AMX amxBufferUsage AmxGetBufferUsage(amxBuffer buf, amxBufferUsage mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    return (!mask) ? buf->usage : (buf->usage & mask);
}

_AMX amxBufferFlags AmxGetBufferFlags(amxBuffer buf, amxBufferFlags mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    return (!mask) ? buf->flags : (buf->flags & mask);
}

////////////////////////////////////////////////////////////////////////////////

_AMX void* AmxGetBufferMap(amxBuffer mbuf, afxSize offset, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);

    afxSize mappedOffset = mbuf->storage[0].mapOffset;
    afxSize mappedRange = mbuf->storage[0].mapRange;

    // Validate if the requested region is within the currently mapped region
    if ((offset < mappedOffset) || ((offset + range) > (mappedOffset + mappedRange)))
        return NIL;

    // If the region is valid, set the placeholder to the corresponding address
    AFX_ASSERT(mappedOffset + mappedRange >= offset + range);
    AFX_ASSERT(mappedOffset <= offset);
    afxSize offDiff = offset - mappedOffset;

    AFX_ASSERT(mbuf->storage[0].mapPtr);
    return &mbuf->storage[0].mapPtr[offDiff];
}

_AMX afxError AmxUnmapBuffer(amxBuffer buf, afxBool wait)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_RW));

    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    amxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = buf->storage[0].mapOffset;
    map.range = buf->storage[0].mapRange;
    map.flags = buf->storage[0].mapFlags;

    if (AmxUnmapBuffers(msys, 1, &map))
        AfxThrowError();

    return err;
}

_AMX afxError AmxMapBuffer(amxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_RW));
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AMX_BUFFER_ALIGNMENT));

    amxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    void* holder;
    if (AmxMapBuffers(msys, 1, &map, (void**[]) { &holder }))
        AfxThrowError();

    // TODO(?): If no placeholder is specified, do not wait for completion.

    AFX_ASSERT(placeholder);
    *placeholder = holder;

    return err;
}

_AMX afxError AmxCohereMappedBuffer(amxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, afxBool discard)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AMX_BUFFER_ALIGNMENT));

    amxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (AmxCohereMappedBuffers(msys, discard, 1, &map))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxDumpBuffer(amxBuffer buf, afxUnit opCnt, amxBufferIo const ops[], void* dst, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if AMX_VALIDATION_ENABLED
    // Validate the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AMX_MBUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AmxGetBufferCapacity(buf, 0), op->srcOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AMX_VALIDATION_ENABLED

    // Prepare the transfer structure for the dump operation
    amxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_MBUF;
    transfer.src.buf = buf;
    transfer.dst.dst = dst;

    // Get the mix system context associated with the buffer
    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    // Perform the transfer (dump) of data
    if (_AmxMsysGetImpl(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        // If the transfer fails, throw an error
        AfxThrowError();
        return err;
    }

    // Ensure that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the mix queue to finish processing the dump
    if (AmxWaitForMixQueue(msys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

_AMX afxError AmxUpdateBuffer(amxBuffer buf, afxUnit opCnt, amxBufferIo const ops[], void const* src, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_W));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

#if AMX_VALIDATION_ENABLED
    // Verify that all operations are valid (this block only runs if validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AMX_MBUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AmxGetBufferCapacity(buf, 0), op->dstOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AMX_VALIDATION_ENABLED

    // Prepare the transfer structure
    amxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_MBUF;
    transfer.dst.buf = buf;
    transfer.src.src = src;

    // Get the mix system associated with the buffer
    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    // Perform the data transfer (buffer update)
    if (_AmxMsysGetImpl(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        // If transfer fails, throw an error
        AfxThrowError();
        return err;
    }

    // Ensure the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the mix queue to finish processing (commented out)
    if (AmxWaitForMixQueue(msys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxUploadBuffer(amxBuffer buf, afxUnit opCnt, amxBufferIo const ops[], afxStream in, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_W));

#if AMX_VALIDATION_ENABLED
    // Validation of the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AMX_MBUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AmxGetBufferCapacity(buf, 0), op->dstOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AMX_VALIDATION_ENABLED

    // Prepare a transfer structure for the operation
    amxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.dstFcc = afxFcc_MBUF;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.buf = buf;
    transfer.src.iob = in;

    // Get the mix system (context) associated with the buffer
    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    // Perform the actual transfer of data
    if (_AmxMsysGetImpl(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    // Assert that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the mix queue to finish processing
    if (AmxWaitForMixQueue(msys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

_AMX afxError AmxDownloadBuffer(amxBuffer buf, afxUnit opCnt, amxBufferIo const ops[], afxStream out, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);
    AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_R));

#if AMX_VALIDATION_ENABLED
    // Validate the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AMX_MBUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AmxGetBufferCapacity(buf, 0), op->srcOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AMX_VALIDATION_ENABLED

    // Prepare the transfer structure for the download operation
    amxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.srcFcc = afxFcc_MBUF;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.buf = buf;
    transfer.dst.iob = out;

    // Get the mix system context associated with the buffer
    afxMixSystem msys = AmxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    // Perform the transfer (download) of data
    if (_AmxMsysGetImpl(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    // Ensure that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AmxWaitForMixQueue(msys, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError _AmxMbufDtorCb(amxBuffer mbuf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);

    if (!(mbuf->flags & amxBufferFlag_F))
    {
        if (mbuf->storage[0].hostedAlloc.bytemap && (!mbuf->base || (mbuf->base != mbuf)))
        {
            AfxDeallocate((void**)&mbuf->storage[0].hostedAlloc.bytemap, AfxHere());
        }
    }
    return err;
}

_AMX afxError _AmxMbufCtorCb(amxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxBufferInfo const* bufi = args[1] ? ((amxBufferInfo const *)args[1]) + invokeNo : NIL;
    amxSubbufferInfo const* subi = args[2] ? ((amxSubbufferInfo const *)args[2]) + invokeNo : NIL;
    
    if (subi)
    {
        amxBuffer base = subi->base;

        if (!base)
        {
            AFX_ASSERT(!base);
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &base);

        if (base->base)
        {
            // Base buffer must be a storage buffer, not a buffer view.
            AFX_ASSERT(!base->base);
            AfxThrowError();
            return err;
        }

        afxUnit srcCap = AmxGetBufferCapacity(base, 0);
        afxUnit size = subi->range;
        afxSize from = subi->from;

        if (!AFX_TEST_ALIGNMENT(from, AMX_BUFFER_ALIGNMENT))
        {
            AFX_ASSERT_ALIGNMENT(from, AMX_BUFFER_ALIGNMENT);
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        AFX_ASSERT_RANGE(srcCap, from, size);

        if ((from >= srcCap) ||
            (size > srcCap) ||
            (size > srcCap - from))
        {
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        // If a capacity is not specified, the new buffer inherits the full capacity of the base buffer, 
        // excluding the portion displaced by @from.
        if (size == 0)
            size = srcCap - from;
        else
            size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);

        // As every buffer capacity is a power of AFX_SIMD_ALIGNMENT, it should already be aligned here.
        AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);

        buf->from = from;
        buf->base = base;
        buf->reqSiz = size;


        afxUnit bps = 0;
        switch (buf->fmt)
        {
        case amxFormat_M32f: bps = 32; break;
        case amxFormat_S32f: bps = 32; break;
        case amxFormat_M32i: bps = 32; break;
        case amxFormat_S32i: bps = 32; break;
        case amxFormat_M24i: bps = 24; break;
        case amxFormat_S24i: bps = 24; break;
        case amxFormat_M16i: bps = 16; break;
        case amxFormat_S16i: bps = 16; break;
        case amxFormat_M8i: bps = 8; break;
        case amxFormat_S8i: bps = 8; break;
        default: AfxThrowError();  break;
        }
        buf->fmt = subi->fmt;
        buf->fmtBps = bps;
        buf->fmtStride = bps / AFX_BYTE_SIZE;

        buf->usage = base->usage;
        buf->flags = base->flags | subi->flags;
        buf->sharingMask = base->sharingMask;

        buf->reqMemType = base->reqMemType;

        buf->tag = subi->tag;
        buf->udd = subi->udd;

        buf->storage[0] = base->storage[0];

        AfxReacquireObjects(1, &base);

        return err;
    }

    AFX_ASSERT(bufi && bufi->size && bufi->usage);
    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    buf->tag = bufi->tag;
    buf->udd = bufi->udd;

    afxUnit size = bufi->size;

    if (!size)
    {
        // If there is not a @base buffer, we need a size here.
        AfxThrowError();
        return err;
    }

    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    // All buffer mapping requires ranges aligned to AFX_SIMD_ALIGNMENT. This alignment is ensured at AFX level.
    size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);

    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);
    buf->reqAlign = AFX_SIMD_ALIGNMENT;
    buf->reqSiz = size;
    buf->from = 0;
    buf->base = NIL;

    if (err) return err;
#ifdef AMX_VALIDATION_ENABLED
    if (!buf->base)
    {
        AFX_ASSERT(buf->from == 0);
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &base);
        AFX_ASSERT_ALIGNMENT(buf->from, AMX_BUFFER_ALIGNMENT);
    }
#endif

    buf->usage = bufi->usage;
    buf->flags = bufi->flags;
    

    afxUnit bps = 0;
    switch (bufi->fmt)
    {
    case amxFormat_M32f: bps = 32; break;
    case amxFormat_S32f: bps = 32; break;
    case amxFormat_M32i: bps = 32; break;
    case amxFormat_S32i: bps = 32; break;
    case amxFormat_M24i: bps = 24; break;
    case amxFormat_S24i: bps = 24; break;
    case amxFormat_M16i: bps = 16; break;
    case amxFormat_S16i: bps = 16; break;
    case amxFormat_M8i: bps = 8; break;
    case amxFormat_S8i: bps = 8; break;
    default: /*AfxThrowError();*/  break;
    }
    buf->fmt = bufi->fmt;
    buf->fmtBps = bps;
    buf->fmtStride = bps / AFX_BYTE_SIZE;


    afxUnit exuCnt = 16; // TODO Get it from MSYS
    buf->sharingMask = NIL;
    for (afxUnit i = 0; i < exuCnt; i++)
    {
        buf->sharingMask |= bufi->sharingMask & AFX_BITMASK(i);
    }

    // STORAGE
    buf->reqMemType = NIL;

    // binding
    buf->storage[0].mmu = 0;
    buf->storage[0].offset = 0;
    buf->storage[0].hostedAlloc.addr = NIL;

    buf->storage[0].mapPtr = NIL;
    buf->storage[0].mapOffset = 0;
    buf->storage[0].mapRange = 0;
    buf->storage[0].mapFlags = NIL;

    buf->storage[0].pendingRemap = 0;
    buf->storage[0].permanentlyMapped = !!bufi->mapped;

    if (buf->flags & amxBufferFlag_F)
    {
        if (!bufi->data) AfxThrowError();
        else
        {
            buf->storage[0].hostedAlloc.bytemap = bufi->data;
            buf->storage[0].mapPtr = buf->storage[0].hostedAlloc.bytemap;
            buf->storage[0].mapRange = buf->reqSiz;
        }
    }
    else
    {
        if (AfxAllocate(buf->reqSiz, buf->reqAlign, AfxHere(), (void**)&buf->storage[0].hostedAlloc.bytemap))
        {
            AfxThrowError();
        }
        else
        {
            buf->storage[0].mapPtr = buf->storage[0].hostedAlloc.bytemap;
            buf->storage[0].mapRange = buf->reqSiz;

            if (!bufi->data)
                AfxCopy(buf->storage[0].mapPtr, &rawData[44], AFX_MIN(sizeof(rawData), buf->reqSiz));
            else
                AfxCopy(buf->storage[0].mapPtr, bufi->data, bufi->dataSiz);
        }
    }
    return err;
}

_AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_MBUF,
    .name = "MixBuffer",
    .desc = "Mix Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(amxBuffer)),
    .ctor = (void*)_AmxMbufCtorCb,
    .dtor = (void*)_AmxMbufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireBuffers(afxMixSystem msys, afxUnit cnt, amxBufferInfo const infos[], amxBuffer buffers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetBufClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { msys, (void*)infos, NIL }))
        AfxThrowError();

    return err;
}

_AMX afxError AmxLoadBuffers(afxMixSystem msys, afxUnit cnt, afxUri const uris[], amxBuffer buffers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(uris);

    for (afxUnit idx = 0; idx < cnt; idx++)
    {
        afxUri const* uri = &uris[idx];

        afxStream file;
        if (AfxOpenFile(uri, afxFileFlag_R, &file))
        {
            AfxThrowError();
            AfxDisposeObjects(idx, buffers);
            break;
        }

        afxSize len = AfxGetStreamLength(file);

        amxBufferInfo bufi = { 0 };
        bufi.flags = amxBufferFlag_RWXC;
        bufi.usage = amxBufferUsage_UPLOAD;
        bufi.size = len;
        if (AmxAcquireBuffers(msys, 1, &bufi, &buffers[idx]))
        {
            AfxThrowError();
            AfxDisposeObjects(idx, buffers);
            break;
        }

        AfxReadStream(file, len, 0, &buffers[idx]->storage[0].hostedAlloc.bytemap);
        // clear/fill stream with zero in padding?

        AfxDisposeObjects(1, &file);
    }
    return err;
}

_AMX afxError AmxMapBuffers(afxMixSystem msys, afxUnit cnt, amxBufferedMap maps[], void** placeholders[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    // Ensure that the operation count doesn't exceed 32
    AFX_ASSERT(cnt <= 32);
    // Temporary array for mapping operations
    _amxBufferRemapping remaps2[32];
    // Count of operations to be processed
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        amxBufferedMap const* map = &maps[i];
        amxBuffer buf = map->buf;

        // Skip if the buffer is NULL
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);

        // Get the buffer's capacity
        afxSize bufCap = AmxGetBufferCapacity(buf, 0);

        afxUnit range = map->range;
        // range always is SIMD aligned.
        range = AFX_ALIGN_SIZE(range, AFX_SIMD_ALIGNMENT);
        AFX_ASSERT_ALIGNMENT(range, AFX_SIMD_ALIGNMENT);

        afxSize offset = map->offset;
        AFX_ASSERT_ALIGNMENT(offset, AMX_BUFFER_ALIGNMENT);
        // offset must be aligned to AMX_BUFFER_ALIGNMENT.

        // Perform validation if enabled
#ifdef AMX_VALIDATION_ENABLED
        AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_RW));

        if (bufCap < map->offset + map->range)
        {
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        // Check that the buffer context matches the mix system.
        afxMixSystem msys2 = AmxGetBufferHost(buf);
        if (msys2 != msys)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys2);
            AFX_ASSERT(msys2 == msys);
            AfxThrowError();
            break;
        }

        // Validate offset and range
        AFX_ASSERT_RANGE(bufCap, offset, range);

#endif//AMX_VALIDATION_ENABLED

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
        else if (buf->storage[0].hostedAlloc.addr)
        {
#ifndef _AMX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED
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
            // Buffer needs to be mapped through a mix system operation
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

    if (_AmxMsysGetImpl(msys)->remapCb(msys, FALSE, opCnt, remaps2))
    {
        AfxThrowError();
    }

    // Final validation and cleaning up pending remaps for each buffer.
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxBuffer buf = remaps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);

#if AMX_VALIDATION_ENABLED
        // Perform final validation on the buffer after remapping.
        AFX_ASSERT(buf->storage[0].mapPtr);
        AFX_ASSERT(buf->storage[0].mapRange >= remaps2[i].range);
        AFX_ASSERT(buf->storage[0].mapOffset >= remaps2[i].offset);
        AFX_ASSERT((buf->storage[0].mapOffset + buf->storage[0].mapRange) >= (remaps2[i].offset + remaps2[i].range));
        AFX_ASSERT_RANGE(buf->reqSiz, buf->storage[0].mapOffset, buf->storage[0].mapRange);
#endif//AMX_VALIDATION_ENABLED

        // Decrement the pending remap counter for each buffer.
        AfxDecAtom32(&buf->storage[0].pendingRemap);
    }

    // Return the error code (still AFX_ERR_NONE if no error occurred).
    return err;
}

_AMX afxError AmxUnmapBuffers(afxMixSystem msys, afxUnit cnt, amxBufferedMap maps[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    /*
        This function does not accept flags. Instead, any behavior wanted in unmapping must be specified in mapping operation.
    */

    // Ensure that the operation count doesn't exceed 32
    AFX_ASSERT(cnt <= 32);
    _amxBufferRemapping unmaps2[32];
    // Count of buffers to unmap.
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        amxBufferedMap const* map = &maps[i];
        amxBuffer buf = map->buf;

        // Skip if the buffer is NULL
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);

        // Can not be unmapped.
        if (buf->storage[0].permanentlyMapped) continue;

#if AMX_VALIDATION_ENABLED
        AFX_ASSERT(AmxGetBufferFlags(buf, amxBufferFlag_RW));

        // Validate the mix system context of the buffer
        // Ensure the buffer is associated with the current mix system
        afxMixSystem msys2 = AmxGetBufferHost(buf);
        AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys2);

        if (msys2 != msys)
        {
            AFX_ASSERT(msys2 == msys);
            // If the buffer is not associated with the correct mix system, throw an error
            AfxThrowError();
            break;
        }
#endif//AMX_VALIDATION_ENABLED

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
                if (buf->storage[0].hostedAlloc.addr)
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

    if (_AmxMsysGetImpl(msys)->remapCb(msys, TRUE, cnt, unmaps2))
    {
        AfxThrowError();
    }

    // Final cleanup for unmapped buffers
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxBuffer buf = unmaps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);

#if AMX_VALIDATION_ENABLED
        // Some final validation checks (if validation is enabled)

#if 0 // if no waited, these assertions will always fail.
        // some assertions
        AFX_ASSERT(!buf->bytemap);
        AFX_ASSERT(!buf->mappedRange);
        AFX_ASSERT(!buf->mappedOffset);
#endif

#endif//AMX_VALIDATION_ENABLED

        // Remove the pending remap state for the unmapped buffer
        AfxDecAtom32(&buf->storage[0].pendingRemap);
    }

    // Return the error code (AFX_ERR_NONE if successful)
    return err;
}

_AMX afxError AmxCohereMappedBuffers(afxMixSystem msys, afxBool discard, afxUnit cnt, amxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

#if AMX_VALIDATION_ENABLED
    // Perform validation for each buffer in the maps
    for (afxUnit i = 0; i < cnt; i++)
    {
        amxBufferedMap const* map = &maps[i];
        amxBuffer buf = map->buf;

        // Skip if the buffer is invalid
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &buf);

        // Ensure the map's range is within the buffer's capacity
        afxSize bufCap = AmxGetBufferCapacity(buf, 0);
        if (bufCap < map->offset + map->range)
        {
            // If the range is out of bounds, throw an error
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        // Ensure the buffer belongs to the same mix system as the provided one
        afxMixSystem msys2 = AmxGetBufferHost(buf);
        if (msys2 != msys)
        {
            // If the buffer belongs to a different system, throw an error
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys2);
            AFX_ASSERT(msys2 == msys);
            AfxThrowError();
            break;
        }
    }

    // If any error has occurred during validation, return the error code
    if (err)
        return err;
#endif//AMX_VALIDATION_ENABLED

    // If no ICD callback is provided, use a mix queue to perform the flush/invalidate operation.
    if (_AmxMsysGetImpl(msys)->cohereCb(msys, discard, cnt, maps))
    {
        // If flushing fails, throw an error
        AfxThrowError();
    }
    // Return the error code (AFX_ERR_NONE if successful)
    return err;
}
