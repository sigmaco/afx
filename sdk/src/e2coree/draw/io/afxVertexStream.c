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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _AFX_VERTEX_BUFFER_C
#define _AFX_INDEX_BUFFER_C
#include "qwadro/core/afxClass.h"
#include "qwadro/draw/io/afxVertexStream.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"

_AVX afxNat AfxGetVertexBufferUsage(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    return vbuf->bufSiz - vbuf->freeSiz;
}

_AVX afxNat AfxGetVertexBufferCapacity(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    return vbuf->bufSiz;
}

_AVX afxBuffer AfxGetVertexBufferStorage(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    afxBuffer buf;

    if (!(buf = vbuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
    }
    return buf;
}

_AVX afxError _AfxVbufDtor(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    afxDrawContext dctx = AfxGetObjectProvider(vbuf);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxBuffer buf;

    if ((buf = vbuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        AfxReleaseObjects(1, (void*[]) { buf });
    }
    return err;
}

_AVX afxError _AfxVbufCtor(afxVertexBuffer vbuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    afxDrawInput din = cookie->udd[0];
    afxVertexBufferSpecification const *spec = ((afxVertexBufferSpecification const *)cookie->udd[1]) + cookie->no;

    afxDrawContext dctx = AfxGetDrawInputContext(din);

    afxSize bufSiz = spec->bufCap;

    afxBuffer buf;
    afxBufferSpecification const bufSpec =
    {
        .siz = AFX_ALIGN(bufSiz, 64),
        .access = spec->access,
        .usage = afxBufferUsage_VERTEX,
        .src = NIL
    };
    
    if (AfxAcquireBuffers(dctx, 1, &bufSpec, &buf)) AfxThrowError();
    else
    {
        vbuf->buf = buf;
        vbuf->access = spec->access;
        vbuf->bufSiz = spec->bufCap;
        vbuf->freeSiz = vbuf->bufSiz;

        AfxSetUpChain(&vbuf->rooms, vbuf);
    }
    return err;
}

_AVX afxClassConfig const _AfxVbufClsConfig =
{
    .fcc = afxFcc_VBUF,
    .name = "Vertex Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxVertexBuffer)),
    .mmu = NIL,
    .ctor = (void*)_AfxVbufCtor,
    .dtor = (void*)_AfxVbufDtor
};

_AVX afxError AfxAcquireVertexBuffers(afxDrawInput din, afxNat cnt, afxVertexBufferSpecification const spec[], afxVertexBuffer vbuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(vbuf);

    afxClass* cls = AfxGetVertexBufferClass(din);
    AfxAssertClass(cls, afxFcc_VBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)vbuf, (void const*[]) { din, spec }))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxNat32 AfxGetIndexBufferStride(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    return ibuf->idxSiz;
}

_AVX afxNat AfxGetIndexBufferUsage(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    return ibuf->bookedIdxCnt;
}

_AVX afxNat AfxGetIndexBufferCapacity(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    return ibuf->idxCap;
}

_AVX afxBuffer AfxGetIndexBuffer(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    afxBuffer buf;

    if (!(buf = ibuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
    }
    return buf;
}

_AVX void AfxDisposeIndexBufferSegment(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_VBUF);
    AfxAssertRange(ibuf->idxCap, baseIdx, idxCnt);
    AfxAssert(idxCnt);

    afxIndexBufferRoom* room;
    AfxChainForEveryLinkage(&ibuf->rooms, afxIndexBufferRoom, ibuf, room)
    {
        if (idxCnt < AfxAbs((afxInt64)(room->baseIdx + room->idxCnt) - (afxInt64)(baseIdx + idxCnt)))
        {
            AfxThrowError();
        }
    }

    if (!err)
    {
        if (!(room = AfxAllocate(NIL, 1, sizeof(*room), 0, AfxHint()))) AfxThrowError();
        else
        {
            room->baseIdx = baseIdx;
            room->idxCnt = idxCnt;
            AfxPushLinkage(&room->ibuf, &ibuf->rooms);
        }
    }
}

_AVX afxNat AfxReserveIndexBufferSegment(afxIndexBuffer ibuf, afxNat idxCnt, afxNat* baseIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_VBUF);
    AfxAssertRange(ibuf->idxCap, 0, idxCnt);
    AfxAssert(idxCnt);

    afxNat stride = ibuf->idxSiz;
    afxSize size = stride * idxCnt;
    AfxAssertRange(AfxGetBufferCapacity(ibuf->buf), 0, size);

    afxNat idxCap = ibuf->idxCap;
    AfxAssertRange(idxCap, 0, idxCnt);
    afxNat bookedIdxCnt = ibuf->bookedIdxCnt;
    afxNat freeCnt = idxCap - bookedIdxCnt;
    afxNat alignedCnt = 0;

    if (freeCnt < idxCnt) AfxThrowError();
    else
    {
        afxSize padding = 0;
        afxSize alignedBaseIdx = 0;

        afxIndexBufferRoom* room, *room2 = NIL;
        AfxChainForEveryLinkage(&ibuf->rooms, afxIndexBufferRoom, ibuf, room)
        {
            if (!(idxCnt > room->idxCnt))
            {
                alignedBaseIdx = AFX_ALIGN(room->baseIdx, AfxAbs(64 / stride));
                padding = AfxAbs(alignedBaseIdx - room->baseIdx);
                alignedCnt = padding + size;

                if (!(alignedCnt > room->idxCnt))
                {
                    room2 = (alignedCnt + bookedIdxCnt >= idxCap) ? NIL : room;
                    break;
                }
            }
        }

        if (!room2) AfxThrowError();
        else
        {
            if (room2->idxCnt > alignedCnt)
            {
                room2->idxCnt = room2->idxCnt - alignedCnt;
                room2->baseIdx = alignedBaseIdx + alignedCnt;
            }
            else
            {
                AfxPopLinkage(&room2->ibuf);
                AfxDeallocate(NIL, room2);
            }

            ibuf->bookedIdxCnt += alignedCnt;
            //seg->range = alignedSiz;
            //seg->base = offset;
            //seg->ibuf = ibuf;

            AfxAssert(baseIdx);
            *baseIdx = alignedBaseIdx;

            //if (IsHostVisible())
            {
                //seg->data = ibuf->buf->data + offset;
            }
        }
    }

    if (err)
        alignedCnt = 0;

    return alignedCnt;
}

_AVX afxError _AfxIbufDtor(afxIndexBuffer ibuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxDrawContext dctx = AfxGetObjectProvider(ibuf);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxBuffer buf;

    if ((buf = ibuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        AfxReleaseObjects(1, (void*[]) { buf });
    }
    return err;
}

_AVX afxError _AfxIbufCtor(afxIndexBuffer ibuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxDrawContext dctx = cookie->udd[0];
    afxIndexBufferSpecification const *spec = ((afxIndexBufferSpecification const *)cookie->udd[1]) + cookie->no;

    AfxSetUpChain(&ibuf->rooms, ibuf);

    afxSize bufSiz = spec->idxSiz * spec->idxCap;

    afxBuffer buf;
    afxBufferSpecification const bufSpec =
    {
        .siz = bufSiz,
        .access = spec->access,
        .usage = afxBufferUsage_INDEX,
        .src = NIL
    };

    if (AfxAcquireBuffers(dctx, 1, &bufSpec, &buf)) AfxThrowError();
    else
    {
        ibuf->buf = buf;
        ibuf->access = spec->access;
        ibuf->idxCap = spec->idxCap;
        ibuf->idxSiz = spec->idxSiz;
        ibuf->bookedIdxCnt = 0;
    }
    return err;
}

_AVX afxClassConfig const _AfxIbufClsConfig =
{
    .fcc = afxFcc_IBUF,
    .name = "Index Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxIndexBuffer)),
    .mmu = NIL,
    .ctor = (void*)_AfxIbufCtor,
    .dtor = (void*)_AfxIbufDtor
};

_AVX afxError AfxAcquireIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferSpecification const spec[], afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(ibuf);

    afxClass* cls = AfxGetIndexBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_IBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ibuf, (void const*[]) { dctx, spec }))
        AfxThrowError();

    return err;
}
