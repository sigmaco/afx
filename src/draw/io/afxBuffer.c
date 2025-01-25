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

AVX void* _AvxGetMapBuffersCallback(afxDrawSystem dsys);
AVX void* _AvxGetUnmapBuffersCallback(afxDrawSystem dsys);
AVX void* _AvxGetSyncMapsCallback(afxDrawSystem dsys);
AVX void* _AvxGetFlushMapsCallback(afxDrawSystem dsys);

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
    return buf->flags & afxBufferFlag_ACCESS;
}

_AVX afxBufferFlags AfxTestBufferAccess(afxBuffer buf, afxBufferFlags access)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return (buf->flags & afxBufferFlag_ACCESS) & access;
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

_AVX afxError AfxUnmapBuffer(afxBuffer buf, afxBool wait)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (AfxUnmapBuffers(dsys, 1, &buf))
        AfxThrowError();

    return err;
}

_AVX afxError AfxMapBuffer(afxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->cap, offset, range);
    AFX_ASSERT(AFX_IS_ALIGNED(offset, AFX_BUF_ALIGNMENT));

    afxBufferRemap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;
    map.placeholder = placeholder;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (AfxMapBuffers(dsys, 1, &map))
        AfxThrowError();

    return err;
}

_AVX afxError AfxFlushBuffer(afxBuffer buf, afxSize offset, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->cap, offset, range);
    AFX_ASSERT(AFX_IS_ALIGNED(offset, AFX_BUF_ALIGNMENT));

    afxBufferMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (AfxFlushBufferMaps(dsys, 1, &map))
        AfxThrowError();

    return err;
}

_AVX afxError AfxSyncBuffer(afxBuffer buf, afxSize offset, afxUnit range)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->cap, offset, range);
    AFX_ASSERT(AFX_IS_ALIGNED(offset, AFX_BUF_ALIGNMENT));

    afxBufferMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (AfxSyncBufferMaps(dsys, 1, &map))
        AfxThrowError();

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

_AVX afxError _AvxBufDtorCb(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxDrawSystem dsys = AfxGetBufferContext(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    while (buf->bytemap)
    {
        AfxUnmapBuffer(buf, TRUE);
        AfxYield();
        //AFX_ASSERT(!buf->bytemap);
    }

    if (buf->src)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf->src);
        AfxDisposeObjects(1, &buf->src);
    }
    return err;
}

_AVX afxError _AvxBufCtorCb(afxBuffer buf, void** args, afxUnit invokeNo)
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
    .ctor = (void*)_AvxBufCtorCb,
    .dtor = (void*)_AvxBufDtorCb
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
            afxClass* cls = (afxClass*)_AvxGetBufferClass(dsys);
            AFX_ASSERT_CLASS(cls, afxFcc_BUF);

            if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dsys, (void*)infos }))
                AfxThrowError();
        }
    }
    return err;
}

/*
    AfxFlushBufferMaps guarantees that host writes to the memory ranges described by maps are made available to the host memory domain,
    such that they can be made available to the device memory domain via memory domain operations using the WRITE access type.
*/

_AVX afxError AfxFlushBufferMaps(afxDrawSystem dsys, afxUnit cnt, afxBufferMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferMap const* map = &maps[i];
        afxBuffer buf = map->buf;

        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        afxSize bufCap = AfxGetBufferCapacity(buf, 0);
        if (bufCap < map->offset + map->range)
        {
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        afxDrawSystem dsys2 = AfxGetBufferContext(buf);
        if (dsys2 != dsys)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);
            AFX_ASSERT(dsys2 == dsys);
            AfxThrowError();
            break;
        }
    }

    if (err)
        return err;

    afxError(*flushMapsCb)(afxDrawSystem, afxUnit, afxBufferMap const[]) = _AvxGetFlushMapsCallback(dsys);

    if (!flushMapsCb)
    {
        afxDrawQueue dque;
        afxDrawBridge dexu;
        afxUnit portId = 0;
        AfxGetDrawBridges(dsys, portId, 1, &dexu);
        AfxGetDrawQueues(dexu, 0, 1, &dque);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (_AvxSubmitSyncMaps(dque, cnt, maps, 0, NIL))
        {
            AfxThrowError();
        }
    }
    else if (flushMapsCb(dsys, cnt, maps))
        AfxThrowError();

    return err;
}

/*
    AfxSyncBufferMaps guarantees that device writes to the memory ranges described by maps,
    which have been made available to the host memory domain using the WRITE and READ access types,
    are made visible to the host. If a range of non-coherent memory is written by the host and then
    invalidated without first being flushed, its contents are undefined.
*/

_AVX afxError AfxSyncBufferMaps(afxDrawSystem dsys, afxUnit cnt, afxBufferMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferMap const* map = &maps[i];
        afxBuffer buf = map->buf;

        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        afxSize bufCap = AfxGetBufferCapacity(buf, 0);
        if (bufCap < map->offset + map->range)
        {
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        afxDrawSystem dsys2 = AfxGetBufferContext(buf);
        if (dsys2 != dsys)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);
            AFX_ASSERT(dsys2 == dsys);
            AfxThrowError();
            break;
        }
    }

    if (err)
        return err;

    afxError(*syncMapsCb)(afxDrawSystem, afxUnit, afxBufferMap const[]) = _AvxGetSyncMapsCallback(dsys);

    if (!syncMapsCb)
    {
        afxDrawQueue dque;
        afxDrawBridge dexu;
        afxUnit portId = 0;
        AfxGetDrawBridges(dsys, portId, 1, &dexu);
        AfxGetDrawQueues(dexu, 0, 1, &dque);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (_AvxSubmitSyncMaps(dque, 0, NIL, cnt, maps))
        {
            AfxThrowError();
        }
    }
    else if (syncMapsCb(dsys, cnt, maps))
        AfxThrowError();

    return err;
}

_AVX afxError AfxMapBuffers(afxDrawSystem dsys, afxUnit cnt, afxBufferRemap maps[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AFX_ASSERT(cnt <= 32);
    afxBufferRemap maps2[32];
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferRemap const* map = &maps[i];
        afxBuffer buf = map->buf;

        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        afxSize bufCap = AfxGetBufferCapacity(buf, 0);
        if (bufCap < map->offset + map->range)
        {
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        afxDrawSystem dsys2 = AfxGetBufferContext(buf);
        if (dsys2 != dsys)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);
            AFX_ASSERT(dsys2 == dsys);
            AfxThrowError();
            break;
        }

        afxSize offset = map->offset;
        afxUnit range = map->range;
        AFX_ASSERT_RANGE(buf->cap, offset, range);
        AFX_ASSERT(AFX_IS_ALIGNED(offset, AFX_BUF_ALIGNMENT));

        offset = AFX_ALIGNED_SIZE(offset, AFX_BUF_ALIGNMENT);
        offset = AfxClamp(offset, 0, ((buf->cap / AFX_BUF_ALIGNMENT) - 1) * AFX_BUF_ALIGNMENT);

        if (!range)
            range = AfxMin(buf->cap, buf->cap - offset);

        //range = AFX_ALIGNED_SIZE(range, AFX_BUF_ALIGNMENT);
        //range = AfxClamp(range, AFX_BUF_ALIGNMENT, buf->cap - offset);

        AFX_ASSERT(offset % AFX_BUF_ALIGNMENT == 0);

        // wait for any submitted mapping operation.
        while (AfxLoadAtom32(&buf->pendingRemap)) AfxYield();
        // signal a ongoing mapping operation.
        AfxIncAtom32(&buf->pendingRemap);

        // try to avoid a operation submission if we have already mapped the request buffer region.
        if (buf->bytemap)
        {
            AFX_ASSERT_RANGE(buf->cap, buf->mappedOffset, buf->mappedRange);

            if ((offset < buf->mappedOffset) || (offset + range > buf->mappedOffset + buf->mappedRange))
            {
                AfxThrowError();
                break;
            }
            else
            {
                AFX_ASSERT(buf->mappedOffset + buf->mappedRange >= offset + range);
                AFX_ASSERT(buf->mappedOffset >= offset);
                AFX_ASSERT(map->placeholder);
                *map->placeholder = &buf->bytemap[offset - buf->mappedOffset];
            }

            // remove pendency as we don't inc opCnt
            AfxDecAtom32(&buf->pendingRemap);
        }
        else if (buf->data) // if buffer is host-side allocated, just do it here.
        {
            buf->mappedOffset = offset;
            buf->mappedRange = range;
            buf->mappedFlags = map->flags;
            buf->bytemap = ((afxByte*)buf->data) + offset;

            AFX_ASSERT(map->placeholder);
            *map->placeholder = buf->bytemap;

            // remove pendency as we don't inc opCnt
            AfxDecAtom32(&buf->pendingRemap);
        }
        else
        {
            maps2[opCnt].buf = buf;
            maps2[opCnt].offset = offset;
            maps2[opCnt].range = range;
            maps2[opCnt].flags = map->flags;
            maps2[opCnt].placeholder = map->placeholder;
            AFX_ASSERT(maps2[opCnt].placeholder);
            opCnt++;

            //AfxDecAtom32(&buf->pendingRemap);
        }
    }

    if (err || !opCnt)
        return err;

    afxError(*mapBufsCb)(afxDrawSystem, afxUnit, afxBufferRemap const[]) = _AvxGetMapBuffersCallback(dsys);

    if (!mapBufsCb)
    {
        afxDrawQueue dque;
        afxDrawBridge dexu;
        afxUnit portId = 0;
        AfxGetDrawBridges(dsys, portId, 1, &dexu);
        AfxGetDrawQueues(dexu, 0, 1, &dque);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (_AvxSubmitRemapping(dque, opCnt, maps2, 0, NIL))
        {
            AfxThrowError();
        }
        else
        {
            if (AfxWaitForEmptyDrawQueue(dque, AFX_TIME_INFINITE))
                AfxThrowError();
        }
    }
    else if (mapBufsCb(dsys, opCnt, maps2))
        AfxThrowError();

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBuffer buf = maps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // some assertions
        AFX_ASSERT(buf->bytemap);
        AFX_ASSERT(buf->mappedRange);
        AFX_ASSERT(buf->mappedOffset + buf->mappedRange >= maps2[i].offset + maps2[i].range);
        AFX_ASSERT(buf->mappedOffset >= maps2[i].offset);
        AFX_ASSERT_RANGE(buf->cap, buf->mappedOffset, buf->mappedRange);

        // remove pendency
        AfxDecAtom32(&buf->pendingRemap);
    }

    return err;
}

_AVX afxError AfxUnmapBuffers(afxDrawSystem dsys, afxUnit cnt, afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    /// dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    /*
        This function does not accept flags. Instead, any behavior wanted in unmapping must be specified in mapping operation.
    */

    afxBuffer unmaps2[32];
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBuffer buf = buffers[i];
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        afxDrawSystem dsys2 = AfxGetBufferContext(buf);
        if (dsys2 != dsys)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys2);
            AFX_ASSERT(dsys2 == dsys);
            AfxThrowError();
            break;
        }

        // wait for any submitted mapping operation.
        while (AfxLoadAtom32(&buf->pendingRemap)) AfxYield();
        // signal a ongoing mapping operation.
        AfxIncAtom32(&buf->pendingRemap);

        if (buf->bytemap)
        {
            // if buffer is host-side allocated, do it here.
            if (buf->data)
            {
                buf->mappedOffset = 0;
                buf->mappedRange = 0;
                buf->mappedFlags = NIL;
                buf->bytemap = NIL;

                AfxDecAtom32(&buf->pendingRemap);
            }
            else
            {
                unmaps2[opCnt] = buf;
                opCnt++;
            }
        }
    }

    if (err || !opCnt)
        return err;

    afxError(*unmapBufsCb)(afxDrawSystem,afxUnit,afxBuffer const[]) = _AvxGetUnmapBuffersCallback(dsys);

    if (!unmapBufsCb)
    {
        afxDrawQueue dque;
        afxDrawBridge dexu;
        afxUnit portId = 0;
        AfxGetDrawBridges(dsys, portId, 1, &dexu);
        AfxGetDrawQueues(dexu, 0, 1, &dque);
        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

        if (_AvxSubmitRemapping(dque, 0, NIL, cnt, unmaps2))
        {
            AfxThrowError();
        }
    }
    else if (unmapBufsCb(dsys, cnt, unmaps2))
        AfxThrowError();

    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBuffer buf = unmaps2[i];
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
#if 0 // if no waited, these assertions will always fail.
        // some assertions
        AFX_ASSERT(!buf->bytemap);
        AFX_ASSERT(!buf->mappedRange);
        AFX_ASSERT(!buf->mappedOffset);
#endif
        // remove pendency
        AfxDecAtom32(&buf->pendingRemap);
    }
    return err;
}
