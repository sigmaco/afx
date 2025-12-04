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
    amxMetabufferInfo const* subi = args[2] ? ((amxMetabufferInfo const *)args[2]) + invokeNo : NIL;
    
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
