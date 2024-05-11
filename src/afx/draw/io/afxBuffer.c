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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxDrawContext AfxGetBufferContext(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxDrawContext dctx = AfxGetObjectProvider(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxNat AfxGetBufferCapacity(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    return buf->siz;
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

_AVX void AfxUnmapBuffer(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (buf->mappedRange)
    {
        if (buf->unmap(buf))
            AfxThrowError();

        AfxAssert(!buf->mappedRange);
    }
}

_AVX void* AfxMapBuffer(afxBuffer buf, afxNat offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    //AfxAssertAlign(offset, 64);
    AfxAssertRange(buf->siz, offset, range);
    void* ptr = NIL;

    if (!buf->mappedRange)
    {
        if (buf->remap(buf, offset, range, flags)) AfxThrowError();
        else
        {
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
    return ptr;
}

_AVX afxError AfxDumpBuffer2(afxBuffer buf, afxNat offset, afxNat stride, afxNat cnt, void *dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertAlign(offset, 64);

    afxNat siz = AfxGetBufferCapacity(buf);
    AfxAssertRange(siz, offset, cnt * stride);
    AfxAssert(dst);
    AfxAssert(dstStride);

    afxByte *map = AfxMapBuffer(buf, offset, cnt * stride, afxBufferAccess_R);

    if (!map) AfxThrowError();
    else
    {
        if (!dstStride || dstStride == stride)
        {
            AfxCopy2(cnt, stride, map, dst);
        }
        else
        {
            afxByte *dst2 = dst;

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxCopy2(1, stride, &(map[i * stride]), &(dst2[i * dstStride]));
            }
        }

        AfxUnmapBuffer(buf);
    }
    return err;
}

_AVX afxError AfxDumpBuffer(afxBuffer buf, afxNat offset, afxNat range, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertAlign(offset, 64);

    afxNat siz = AfxGetBufferCapacity(buf);
    AfxAssertRange(siz, offset, range);
    range = range ? range : siz - offset;
    AfxAssertRange(siz, offset, range);
    AfxAssert(dst);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.data.buf = buf;
    req.data.offset = offset;
    req.data.range = range;
    req.srcFcc = afxFcc_BUF;
    req.data.dst = dst;
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxUpdateBufferRegion(afxBuffer buf, afxBufferRegion const* rgn, void const* src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(rgn);
    AfxAssertAlign(rgn->base, 64);
    

    afxNat bufSiz = AfxGetBufferCapacity(buf);
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

        AfxUnmapBuffer(buf);
    }

    return err;
}

_AVX afxError AfxUpdateBuffer2(afxBuffer buf, afxNat offset, afxNat range, afxNat stride, void const *src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertAlign(offset, 64);

    afxNat bufSiz = AfxGetBufferCapacity(buf);
    AfxAssertRange(bufSiz, offset, range);
    AfxAssert(src);
    AfxAssert(srcStride);

    afxByte *map = AfxMapBuffer(buf, offset, range, afxBufferAccess_W);

    if (!map) AfxThrowError();
    else
    {
        if ((srcStride == stride) && stride)
        {
            AfxCopy2(1, range, src, map);
        }
        else
        {
            afxByte const *src2 = src;
            afxNat unitSiz = AfxMinorNonZero(stride, srcStride); // minor non-zero
            afxNat cnt = range / unitSiz;
            afxNat cnt2 = range % unitSiz;
            AfxAssert(!cnt2);

            for (afxNat i = 0; i < cnt; i++)
            {
                AfxAssert(stride != 1 || (stride == 1 && AFX_N8_MAX >= (afxNat8)(src2[i * srcStride])));
                AfxAssert(stride != 2 || (stride == 2 && AFX_N16_MAX >= (afxNat16)(src2[i * srcStride])));
                AfxAssert(stride != 4 || (stride == 4 && AFX_N32_MAX >= (afxNat32)(src2[i * srcStride])));

                AfxCopy2(1, unitSiz, &(src2[i * srcStride]), &(map[i * stride]));
            }
        }

        AfxUnmapBuffer(buf);
    }
    return err;
}

_AVX afxError AfxUpdateBuffer(afxBuffer buf, afxNat offset, afxNat range, void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    //AfxAssertAlign(offset, 64);

    afxNat siz = AfxGetBufferCapacity(buf);
    AfxAssertRange(siz, offset, range);
    range = range ? range : siz - offset;
    AfxAssertRange(siz, offset, range);
    AfxAssert(src);

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    afxTransferRequest req = { 0 };
    req.data.buf = buf;
    req.data.offset = offset;
    req.data.range = range;
    req.dstFcc = afxFcc_BUF;
    req.data.src = src;
    afxNat queIdx = AFX_INVALID_INDEX;

    if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
    else
    {
        AfxAssert(queIdx != AFX_INVALID_INDEX);

        if (AfxWaitForIdleDrawQueue(ddge, queIdx))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxUploadBuffer(afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_W));

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxTransferRequest req = { 0 };
        req.data.buf = buf;
        req.data.offset = ops[i].dstOffset;
        req.data.range = ops[i].range;
        req.dstFcc = afxFcc_BUF;
        req.data.iob = in;
        req.data.dataOff = ops[i].srcOffset;
        req.srcFcc = afxFcc_IOB;
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);

            if (AfxWaitForIdleDrawQueue(ddge, queIdx))
                AfxThrowError();
        }
    }
    return err;
}

_AVX afxError AfxDownloadBuffer(afxBuffer buf, afxNat opCnt, afxBufferIoOp const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(AfxTestBufferAccess(buf, afxBufferAccess_R));

    afxDrawContext dctx = AfxGetBufferContext(buf);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxTransferRequest req = { 0 };
        req.data.buf = buf;
        req.data.offset = ops[i].srcOffset;
        req.data.range = ops[i].range;
        req.srcFcc = afxFcc_BUF;
        req.data.iob = out;
        req.data.dataOff = ops[i].dstOffset;
        req.dstFcc = afxFcc_IOB;
        afxNat queIdx = AFX_INVALID_INDEX;

        if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueTransferRequest(ddge, NIL, 1, &req))) AfxThrowError();
        else
        {
            AfxAssert(queIdx != AFX_INVALID_INDEX);

            if (AfxWaitForIdleDrawQueue(ddge, queIdx))
                AfxThrowError();
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
        AfxUnmapBuffer(buf);
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
        buf->siz = spec->siz;
        buf->usage = spec->usage;
        buf->access = spec->access ? spec->access : afxBufferAccess_W;

        buf->bytemap = NIL;
        buf->mappedOffset = 0;
        buf->mappedRange = 0;
        buf->mappedFlags = NIL;

        buf->remap = NIL;
        buf->unmap = NIL;

        if (spec->src)
        {
            if (AfxUpdateBuffer(buf, 0, buf->siz, spec->src))
                AfxThrowError();
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxBufStdImplementation =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxBuffer)),
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

    afxManager* cls = AfxGetBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { dctx, (void*)spec }))
        AfxThrowError();

    return err;
}
