/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#include "../../dev/AvxImplKit.h"

_AVX afxDrawContext AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxDrawContext dctx = AfxGetProvider(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX void* AfxGetBufferUdd(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->udd;
}

_AVX afxUnit AfxGetBufferCapacity(afxBuffer buf, afxUnit from)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->bufCap - from;
}

_AVX afxBufferUsage AfxGetBufferUsage(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->usage;
}

_AVX afxBufferUsage AfxTestBufferUsage(afxBuffer buf, afxBufferUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->usage & usage;
}

_AVX afxBufferFlags AfxGetBufferAccess(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->flags & afxBufferFlag_ACCESS_MASK;
}

_AVX afxBufferFlags AfxTestBufferAccess(afxBuffer buf, afxBufferFlags access)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return (buf->flags & afxBufferFlag_ACCESS_MASK) & access;
}

////////////////////////////////////////////////////////////////////////////////

_AVX void AfxUnmapBuffer(afxBuffer buf, afxBool wait)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    
    while (AfxLoadAtom32(&buf->pendingRemap))
        AfxYield();
    AfxIncAtom32(&buf->pendingRemap);

    if (buf->bytemap)
    {
        if (!buf->remap)
        {
            AFX_ASSERT(buf->sysmemBuffered);
            AfxDeallocate(buf->bytemap);
            buf->sysmemBuffered = FALSE;
        }
        else
        {
            if (buf->remap(buf, 0, 0, !!wait, NIL))
                AfxThrowError();
        }
        
        buf->bytemap = NIL;
        buf->mappedRange = 0;
        buf->mappedOffset = 0;
        buf->mappedFlags = NIL;
    }
    AfxDecAtom32(&buf->pendingRemap);
}

_AVX afxError AfxMapBuffer(afxBuffer buf, afxUnit offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT_RANGE(buf->bufCap, offset, range);

    offset = AFX_ALIGNED_SIZE(offset, AFX_BUF_ALIGNMENT);
    offset = AfxClamp(offset, 0, ((buf->bufCap / AFX_BUF_ALIGNMENT) - 1) * AFX_BUF_ALIGNMENT);

    if (!range)
        range = AfxMin(buf->bufCap, buf->bufCap - offset);

    range = AFX_ALIGNED_SIZE(range, AFX_BUF_ALIGNMENT);
    range = AfxClamp(range, AFX_BUF_ALIGNMENT, buf->bufCap - offset);

    AfxAssert2((offset % AFX_BUF_ALIGNMENT == 0), (range % AFX_BUF_ALIGNMENT == 0));
    void* ptr = NIL;

    while (AfxLoadAtom32(&buf->pendingRemap))
        AfxYield();
    AfxIncAtom32(&buf->pendingRemap);

    if (!buf->bytemap)
    {
        if (!buf->remap)
        {
            ptr = AfxAllocate(buf->bufCap - offset, sizeof(afxByte), AFX_BUF_ALIGNMENT, AfxHere());

            if (!ptr) AfxThrowError();
            else
            {
                buf->sysmemBuffered = TRUE;
                buf->bytemap = ptr;
                buf->mappedOffset = offset;
                buf->mappedRange = range;
                buf->mappedFlags = flags;
            }
        }
        else
        {
            buf->mappedOffset = offset;
            buf->mappedRange = range;
            buf->mappedFlags = flags;

            if (buf->remap(buf, offset, range, flags, &ptr))
            {
                AfxThrowError();
                buf->bytemap = NIL;
                buf->mappedOffset = 0;
                buf->mappedRange = 0;
                buf->mappedFlags = NIL;
            }
            ptr = buf->bytemap;
        }

        AFX_ASSERT(buf->bytemap);
        AFX_ASSERT(buf->mappedRange);
        AFX_ASSERT(buf->mappedOffset + buf->mappedRange >= offset + range);
        AFX_ASSERT(buf->mappedOffset >= offset);
    }
    else if ((offset < buf->mappedOffset) || (range > buf->mappedRange)) AfxThrowError();
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

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], void* dst, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;
    
    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        avxTransference transfer = { 0 };

        transfer.srcFcc = afxFcc_BUF;
        transfer.data.buf = buf;
        transfer.data.dst = dst;
        
#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, AFX_BUF_ALIGNMENT);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->srcOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], void const* src, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        avxTransference transfer = { 0 };

        transfer.dstFcc = afxFcc_BUF;
        transfer.data.buf = buf;
        transfer.data.src = src;

#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->dstOffset, AFX_BUF_ALIGNMENT);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->dstOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxStream in, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_W));

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        avxTransference transfer = { 0 };

        transfer.dstFcc = afxFcc_BUF;
        transfer.srcFcc = afxFcc_IOB;
        transfer.data.buf = buf;
        transfer.data.iob = in;

#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->dstOffset, AFX_BUF_ALIGNMENT);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->dstOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxDownloadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxStream out, afxUnit portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AFX_ASSERT(AfxTestBufferAccess(buf, afxBufferFlag_R));

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;
    
    if (!AfxGetDrawBridges(dctx, portId, 1, &dexu)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dexu, afxFcc_DEXU);
        avxTransference transfer = { 0 };

        transfer.srcFcc = afxFcc_BUF;
        transfer.dstFcc = afxFcc_IOB;
        transfer.data.buf = buf;
        transfer.data.iob = out;

#if _AFX_DEBUG
        for (afxUnit i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, AFX_BUF_ALIGNMENT);
            AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->srcOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxUnit queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AFX_ASSERT(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxBufStdDtorCb(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (buf->mappedRange)
    {
        AfxUnmapBuffer(buf, TRUE);
        AFX_ASSERT(!buf->mappedRange);
    }
    return err;
}

_AVX afxError _AvxBufStdCtorCb(afxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxBufferInfo const *bufi = ((afxBufferInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(bufi && bufi->bufCap && bufi->usage);

    if (!bufi) AfxThrowError();
    else if (!bufi->bufCap) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();
    
    if (!err)
    {
        buf->bufCap = AFX_ALIGNED_SIZE(bufi->bufCap, AFX_BUF_ALIGNMENT);
        buf->usage = bufi->usage;
        buf->flags = bufi->flags ? bufi->flags : afxBufferFlag_W;
        buf->fmt = bufi->fmt;

        buf->udd = bufi->udd;

        buf->bytemap = NIL;
        buf->mappedOffset = 0;
        buf->mappedRange = 0;
        buf->mappedFlags = NIL;

        buf->pendingRemap = 0;
        buf->remap = NIL;
    }
    return err;
}

_AVX afxClassConfig const _AvxBufStdImplementation =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Device Memory Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxBuffer)),
    .ctor = (void*)_AvxBufStdCtorCb,
    .dtor = (void*)_AvxBufStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireBuffers(afxDrawContext dctx, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(buffers);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!cnt || !infos || !buffers) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxBufferInfo const* info = &infos[i];
            
            if (!info->bufCap) AfxThrowError();
            else if (!info->usage)
                AfxThrowError();
        }

        if (!err)
        {
            afxClass* cls = (afxClass*)AfxGetBufferClass(dctx);
            AfxAssertClass(cls, afxFcc_BUF);

            if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dctx, (void*)infos }))
                AfxThrowError();
        }
    }
    return err;
}
