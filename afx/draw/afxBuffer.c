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
#include "../dev/AvxImplKit.h"

_AVX afxDrawContext AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxDrawContext dctx = AfxGetParent(buf);
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
    return buf->cap - from;
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

_AVX afxBufferAccess AfxGetBufferAccess(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->access;
}

_AVX afxBufferAccess AfxTestBufferAccess(afxBuffer buf, afxBufferAccess access)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->access & access;
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
            AfxDeallocate(buf->bytemap);
            buf->bytemap = NIL;
            buf->mappedRange = 0;
            buf->mappedOffset = 0;
            buf->mappedFlags = NIL;
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
    AfxAssertRange(buf->cap, offset, range);
    void* ptr = NIL;

    while (AfxLoadAtom32(&buf->pendingRemap))
        AfxYield();

    if (!range)
        range = AfxMin(buf->cap, buf->cap - offset);

    AfxIncAtom32(&buf->pendingRemap);

    if (!buf->mappedRange)
    {
        if (!buf->remap)
        {
            buf->bytemap = AfxAllocate(buf->cap, sizeof(afxByte), AFX_SIMD_ALIGN, AfxHere());

            if (!buf->bytemap) AfxThrowError();
            else
            {
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

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(dst);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;
    
    if (!AfxQueryDrawBridges(dctx, portId, 0, 1, &dexu)) AfxThrowError();
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

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(src);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxQueryDrawBridges(dctx, portId, 0, 1, &dexu)) AfxThrowError();
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

_AVX afxError AfxUploadBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;

    if (!AfxQueryDrawBridges(dctx, portId, 0, 1, &dexu)) AfxThrowError();
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

_AVX afxError AfxDownloadBuffer(afxBuffer buf, afxNat portId, afxNat opCnt, afxBufferIo const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge dexu;
    
    if (!AfxQueryDrawBridges(dctx, portId, 0, 1, &dexu)) AfxThrowError();
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

_AVX afxError _AvxBufStdDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);

    if (buf->mappedRange)
    {
        AfxUnmapBuffer(buf, TRUE);
        AfxAssert(!buf->mappedRange);
    }
    return err;
}

_AVX afxError _AvxBufStdCtor(afxBuffer buf, void** args, afxNat invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    afxBufferSpecification const *spec = ((afxBufferSpecification const *)args[1]) + invokeNo;
    AfxAssert(spec);

    if (!spec->siz) AfxThrowError();
    else if (!spec->usage) AfxThrowError();
    else
    {
        buf->cap = AFX_ALIGNED_SIZEOF(spec->siz, AFX_SIMD_ALIGN);
        buf->usage = spec->usage;
        buf->access = spec->access ? spec->access : afxBufferAccess_W;
        
        buf->udd = spec->udd;

        buf->bytemap = NIL;
        buf->mappedOffset = 0;
        buf->mappedRange = 0;
        buf->mappedFlags = NIL;

        buf->pendingRemap = 0;
        buf->remap = NIL;
        buf->unmap = NIL;

        if (spec->src)
        {
            // instantiation flags must be set before using it from Qwadro.

            //if (AfxUpdateBuffer(buf, 0, buf->cap, spec->src))
                //AfxThrowError();
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxBufStdImplementation =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Device Memory Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxBuffer)),
    .ctor = (void*)_AvxBufStdCtor,
    .dtor = (void*)_AvxBufStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireBuffers(afxDrawContext dctx, afxNat cnt, afxBufferSpecification const spec[], afxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(buffers);
    AfxAssert(spec);
    AfxAssert(cnt);

    afxClass* cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dctx, (void*)spec }))
        AfxThrowError();

    return err;
}
