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

_AVX afxNat AfxGetBufferCapacity(afxBuffer buf, afxNat from)
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

    if (buf->mappedRange)
    {
        if (!buf->unmap)
        {
            AfxAssert(buf->sysmemBuffered);
            AfxDeallocate(buf->bytemap);
            buf->bytemap = NIL;
            buf->mappedRange = 0;
            buf->mappedOffset = 0;
            buf->mappedFlags = NIL;
            buf->sysmemBuffered = FALSE;
        }
        else if (buf->unmap(buf, wait))
            AfxThrowError();

        if (wait)
        {
            AfxAssert(!buf->mappedRange);
        }
    }
    AfxDecAtom32(&buf->pendingRemap);
}

_AVX afxError AfxMapBuffer(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags, void** placeholder)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    //AfxAssertAlign(offset, 64);
    AfxAssertRange(buf->bufCap, offset, range);
    void* ptr = NIL;

    while (AfxLoadAtom32(&buf->pendingRemap))
        AfxYield();

    if (!range)
        range = AfxMin(buf->bufCap, buf->bufCap - offset);

    AfxIncAtom32(&buf->pendingRemap);

    if (!buf->mappedRange)
    {
        if (!buf->remap)
        {
            buf->bytemap = AfxAllocate(buf->bufCap, sizeof(afxByte), AFX_SIMD_ALIGN, AfxHere());

            if (!buf->bytemap) AfxThrowError();
            else
            {
                buf->sysmemBuffered = TRUE;
                buf->mappedOffset = offset;
                buf->mappedRange = range;
                buf->mappedFlags = flags;
                AfxAssert(buf->bytemap);

                ptr = &buf->bytemap[buf->mappedOffset - offset];

                if (placeholder)
                    *placeholder = ptr;
            }
        }
        else if (buf->remap(buf, offset, range, flags, &ptr))
            AfxThrowError();

        AfxAssert(buf->bytemap);
        AfxAssert(buf->mappedRange);
        AfxAssert(buf->mappedOffset + buf->mappedRange >= offset + range);
        AfxAssert(buf->mappedOffset >= offset);

        if (placeholder)
            *placeholder = ptr;
    }
    else if ((offset < buf->mappedOffset) || (range > buf->mappedRange)) AfxThrowError();
    else
    {
        AfxAssert(buf->mappedOffset + buf->mappedRange >= offset + range);
        AfxAssert(buf->mappedOffset >= offset);
        ptr = &buf->bytemap[buf->mappedOffset - offset];
        
        if (placeholder)
            *placeholder = ptr;
    }
    AfxDecAtom32(&buf->pendingRemap);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxNat opCnt, afxBufferIo const ops[], void* dst, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferFlag_R));
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(dst);

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
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, 64);
            AfxAssertRange(AfxGetBufferCapacity(buf, 0), op->srcOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxNat opCnt, afxBufferIo const ops[], void const* src, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferFlag_W));
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(src);

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
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->dstOffset, 64);
            AfxAssertRange(AfxGetBufferCapacity(buf, 0), op->dstOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxUploadBuffer(afxBuffer buf, afxNat opCnt, afxBufferIo const ops[], afxStream in, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferFlag_W));

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
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->dstOffset, 64);
            AfxAssertRange(AfxGetBufferCapacity(buf, 0), op->dstOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
            if (AfxWaitForDrawQueue(dctx, portIdx, queIdx))
                AfxThrowError();
#endif
        }
    }
    return err;
}

_AVX afxError AfxDownloadBuffer(afxBuffer buf, afxNat opCnt, afxBufferIo const ops[], afxStream out, afxNat portId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferFlag_R));

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
        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, 64);
            AfxAssertRange(AfxGetBufferCapacity(buf, 0), op->srcOffset, AfxMax(1, op->rowCnt) * AfxMax(1, op->dstStride));
        }
#endif
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = _AvxSubmitTransferences(dexu, &transfer, opCnt, ops))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);
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
        AfxAssert(!buf->mappedRange);
    }
    return err;
}

_AVX afxError _AvxBufStdCtorCb(afxBuffer buf, void** args, afxNat invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxBufferInfo const *spec = ((afxBufferInfo const *)args[1]) + invokeNo;
    AfxAssert(spec && spec->bufCap && spec->usage);

    if (!spec) AfxThrowError();
    else if (!spec->bufCap) AfxThrowError();
    else if (!spec->usage)
        AfxThrowError();
    
    if (!err)
    {
        buf->bufCap = AFX_ALIGNED_SIZEOF(spec->bufCap, AFX_SIMD_ALIGN);
        buf->usage = spec->usage;
        buf->flags = spec->flags ? spec->flags : afxBufferFlag_W;

        buf->udd = spec->udd;

        buf->bytemap = NIL;
        buf->mappedOffset = 0;
        buf->mappedRange = 0;
        buf->mappedFlags = NIL;

        buf->pendingRemap = 0;
        buf->remap = NIL;
        buf->unmap = NIL;
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

_AVX afxError AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferInfo const infos[], afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(buffers);
    AfxAssert(infos);
    AfxAssert(cnt);

    if (!cnt || !infos || !buffers) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
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
