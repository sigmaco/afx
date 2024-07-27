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
#include "dev/AvxDevKit.h"

_AVX afxDrawContext AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxDrawContext dctx = AfxGetObjectProvider(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
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

_AVX void AfxUnmapBuffer(afxBuffer buf, afxBool wait)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    
    while (AfxLoadAtom32(&buf->pendingRemap))
        AfxYield();

    AfxIncAtom32(&buf->pendingRemap);

    if (buf->mappedRange)
    {
        afxNat portIdx, dqueIdx;
        
        if (buf->unmap(buf, &portIdx, &dqueIdx)) AfxThrowError();
        else if (wait)
        {
            afxDrawContext dctx = AfxGetBufferContext(buf);
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            AfxAssert(portIdx != AFX_INVALID_INDEX);
            AfxAssert(dqueIdx != AFX_INVALID_INDEX);
            AfxAssertRange(AfxCountDrawBridges(dctx), portIdx, 1);
            AfxAssertRange(AfxCountDrawQueues(dctx, portIdx), dqueIdx, 1);

            if (AfxWaitForIdleDrawQueue(dctx, portIdx, dqueIdx))
                AfxThrowError();

            AfxAssert(!buf->mappedRange);
        }
    }
    AfxDecAtom32(&buf->pendingRemap);
}

_AVX void* AfxMapBuffer(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags)
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
        afxNat portIdx, dqueIdx;

        if (buf->remap(buf, offset, range, flags, &portIdx, &dqueIdx)) AfxThrowError();
        else
        {
            afxDrawContext dctx = AfxGetBufferContext(buf);
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            AfxAssert(portIdx != AFX_INVALID_INDEX);
            AfxAssert(dqueIdx != AFX_INVALID_INDEX);
            AfxAssertRange(AfxCountDrawBridges(dctx), portIdx, 1);
            AfxAssertRange(AfxCountDrawQueues(dctx, portIdx), dqueIdx, 1);

            if (AfxWaitForIdleDrawQueue(dctx, portIdx, dqueIdx))
                AfxThrowError();

            AfxAssert(buf->mappedRange);

            AfxAssert(buf->mappedOffset + buf->mappedRange >= offset + range);
            AfxAssert(buf->mappedOffset >= offset);
            ptr = &buf->bytemap[buf->mappedOffset - offset];
        }
    }
    else if ((offset < buf->mappedOffset) || (range > buf->mappedRange)) AfxThrowError();
    else
    {
        AfxAssert(buf->mappedOffset + buf->mappedRange >= offset + range);
        AfxAssert(buf->mappedOffset >= offset);
        ptr = &buf->bytemap[buf->mappedOffset - offset];
    }
    AfxDecAtom32(&buf->pendingRemap);
    return ptr;
}

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxNat portIdx, afxNat opCnt, afxBufferIo const ops[], void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(dst);

    afxDrawBridge ddge;
    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    if (!AfxGetDrawBridge(dctx, portIdx, &ddge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, 64);
            afxNat siz = AfxGetBufferCapacity(buf, 0);
            AfxAssertRange(siz, op->srcOffset, op->range);

            afxTransferRequest req = { 0 };
            req.srcFcc = afxFcc_BUF;
            req.data.buf = buf;
            req.data.dst = dst;
            req.data.op = *op;

            afxNat queIdx = AFX_INVALID_INDEX;

            if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
            else
            {
                AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
                if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
                    AfxThrowError();
#endif
            }
        }
    }
    return err;
}

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxNat portIdx, afxNat opCnt, afxBufferIo const ops[], void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(src);

    afxDrawBridge ddge;
    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (!AfxGetDrawBridge(dctx, portIdx, &ddge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, 64);
            afxNat siz = AfxGetBufferCapacity(buf, 0);
            AfxAssertRange(siz, op->srcOffset, op->range);

            afxTransferRequest req = { 0 };
            req.dstFcc = afxFcc_BUF;
            req.data.buf = buf;
            req.data.src = src;
            req.data.op = *op;

            afxNat queIdx = AFX_INVALID_INDEX;

            if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
            else
            {
                AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
                if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
                    AfxThrowError();
#endif
            }
        }
    }
    return err;
}

_AVX afxError AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));
    AfxAssert(rgn);
    AfxAssertAlign(rgn->base, 64);
    
    afxNat bufSiz = AfxGetBufferCapacity(buf, 0);
    AfxAssertRange(bufSiz, rgn->base, rgn->range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBuffer(buf, rgn->base, rgn->range, afxBufferAccess_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == rgn->stride) && rgn->stride)
        {
            AfxCopy2(rgn->range, 1, src, map);
        }
        else
        {
            afxByte const *src2 = src;
            afxNat unitSiz = AfxMinorNonZero(rgn->stride, srcStride); // minor non-zero
            AfxAssert(unitSiz == rgn->unitSiz);
            afxNat cnt = rgn->range / rgn->stride;
            afxNat cnt2 = rgn->range % rgn->stride;
            //AfxAssert(!cnt2);

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssertRange(bufSiz, (i * rgn->stride) + rgn->offset, rgn->unitSiz);
                AfxCopy2(1, rgn->unitSiz, &(src2[i * srcStride]), &(map[(i * rgn->stride) + rgn->offset]));
            }
        }

        AfxUnmapBuffer(buf, FALSE);
    }

    return err;
}

_AVX afxError AfxUploadBuffer(afxBuffer buf, afxNat portIdx, afxNat opCnt, afxBufferIo const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));

    afxDrawBridge ddge;
    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (!AfxGetDrawBridge(dctx, portIdx, &ddge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, 64);
            afxNat siz = AfxGetBufferCapacity(buf, 0);
            AfxAssertRange(siz, op->srcOffset, op->range);

            afxTransferRequest req = { 0 };
            req.dstFcc = afxFcc_BUF;
            req.srcFcc = afxFcc_IOB;
            req.data.buf = buf;
            req.data.iob = in;
            req.data.op = *op;

            afxNat queIdx = AFX_INVALID_INDEX;

            if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
            else
            {
                AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
                if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
                    AfxThrowError();
#endif
            }
        }
    }
    return err;
}

_AVX afxError AfxDownloadBuffer(afxBuffer buf, afxNat portIdx, afxNat opCnt, afxBufferIo const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));

    afxDrawBridge ddge;
    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    if (!AfxGetDrawBridge(dctx, portIdx, &ddge)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        for (afxNat i = 0; i < opCnt; i++)
        {
            afxBufferIo const* op = &ops[i];

            AfxAssertAlign(op->srcOffset, 64);
            afxNat siz = AfxGetBufferCapacity(buf, 0);
            AfxAssertRange(siz, op->srcOffset, op->range);

            afxTransferRequest req = { 0 };
            req.srcFcc = afxFcc_BUF;
            req.dstFcc = afxFcc_IOB;
            req.data.buf = buf;
            req.data.iob = out;
            req.data.op = *op;

            afxNat queIdx = AFX_INVALID_INDEX;

            if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
            else
            {
                AfxAssert(queIdx != AFX_INVALID_INDEX);
#if 0
                if (AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx))
                    AfxThrowError();
#endif
            }
        }
    }
    return err;
}

_AVX afxError _AvxBufStdDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetBufferContext(buf);

    if (buf->mappedRange)
    {
        AfxUnmapBuffer(buf, TRUE);
        AfxAssert(!buf->mappedRange);
    }
    return err;
}

_AVX afxError _AvxBufStdCtor(afxBuffer buf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    afxBufferSpecification const *spec = ((afxBufferSpecification const *)cookie->udd[1]) + cookie->no;
    AfxAssert(spec);

    if (!spec->siz) AfxThrowError();
    else if (!spec->usage) AfxThrowError();
    else
    {
        buf->cap = AFX_ALIGNED_SIZEOF(spec->siz, AFX_SIMD_ALIGN);
        buf->usage = spec->usage;
        buf->access = spec->access ? spec->access : afxBufferAccess_W;

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
