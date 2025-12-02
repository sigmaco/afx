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
#include "avxIcd.h"

#define _AVX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED TRUE

_AVX afxDrawSystem AvxGetBufferHost(avxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    afxDrawSystem dsys = AfxGetHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxSize AvxGetBufferAddress(avxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->storage[0].host.addr + buf->storageOffset;
}

_AVX afxSize AvxGetBufferCapacity(avxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return AFX_MIN(buf->reqSiz, buf->reqSiz - from);
}

_AVX avxBufferUsage AvxGetBufferUsage(avxBuffer buf, avxBufferUsage mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return (!mask) ? buf->usage : (buf->usage & mask);
}

_AVX avxBufferFlags AvxGetBufferFlags(avxBuffer buf, avxBufferFlags mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return (!mask) ? buf->flags : (buf->flags & mask);
}

_AVXINL void _AvxSanitizeBufferCopy(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferCopy const raw[], avxBufferCopy san[])
{
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferFlags(buf, avxBufferFlag_RW));

    afxDrawSystem dsys = AvxGetBufferHost(buf);
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
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferFlags(buf, avxBufferFlag_RW));
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AVX_BUFFER_ALIGNMENT));

    avxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxDrawSystem dsys = AvxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    void* holder;
    if (AvxMapBuffers(dsys, 1, &map, (void**[]) { &holder }))
        AfxThrowError();

    // TODO(?): If no placeholder is specified, do not wait for completion.

    AFX_ASSERT(placeholder);
    *placeholder = holder;

    return err;
}

_AVX afxError AvxCohereMappedBuffer(avxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, afxBool invalidate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AVX_BUFFER_ALIGNMENT));

    avxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxDrawSystem dsys = AvxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (AvxCohereMappedBuffers(dsys, invalidate, 1, &map))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxBufDtorCb(avxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AvxGetBufferHost(buf);
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

    //afxDrawSystem dsys = AvxGetBufferHost(buf);
    if (_AvxDsysGetDdi(dsys)->deallocBufCb(dsys, 1, &buf))
    {
        AfxThrowError();
    }

    return err;
}

_AVX afxError _AvxBufCtorCb(avxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AvxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxBufferInfo const *bufi = args[1] ? ((avxBufferInfo const *)args[1]) + invokeNo : NIL;
    AFX_ASSERT(bufi && bufi->size && bufi->usage);
    avxMetabufferInfo const* sub = args[2] ? ((avxMetabufferInfo const *)args[2]) + invokeNo : NIL;

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
    buf->storage[0].mapRefCnt = 0;

    buf->storage[0].pendingRemap = 0;
    buf->storage[0].permanentlyMapped = !!bufi->mapped;

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_BUF =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Video Buffer", // AVX Buffer
    .fixedSiz = sizeof(AFX_OBJECT(avxBuffer)),
    .ctor = (void*)_AvxBufCtorCb,
    .dtor = (void*)_AvxBufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[])
{
    afxError err = { 0 };
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

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->bufCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dsys, (void*)infos, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);

    if (_AvxDsysGetDdi(dsys)->allocBufCb(dsys, cnt, infos, buffers))
    {
        AfxDisposeObjects(cnt, buffers);
        AfxThrowError();
        return err;
    }

    if (err) return err;

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
                iop.srcStride = 1;
                iop.dstStride = 1;
                iop.rowCnt = bufi->dataSiz;
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

_AVX afxError AvxReacquireBuffers(afxDrawSystem dsys, afxUnit cnt, avxMetabufferInfo const infos[], avxBuffer buffers[])
{
    afxError err = { 0 };
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
        avxMetabufferInfo const* info = &infos[i];

        if (!info->buf)
        {
            AFX_ASSERT(info->buf);
            AfxThrowError();
        }
    }

    if (err) return err;

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->bufCls(dsys);
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
        avxMetabufferInfo const* bufi = &infos[i];

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
