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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_VERTEX_BUFFER_C
#define _AFX_INDEX_BUFFER_C
#include "qwadro/draw/io/afxVertexStream.h"
#include "qwadro/draw/afxDrawContext.h"

_AFX afxVertexInput AfxGetVertexBufferLayout(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    return vbuf->layout;
}

_AFX afxNat AfxGetVertexBufferUsage(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    return vbuf->bookedSiz;
}

_AFX afxNat AfxGetVertexBufferCapacity(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    return vbuf->bufSiz;
}

_AFX afxBuffer AfxGetVertexBufferStorage(afxVertexBuffer vbuf)
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

_AFX void AfxDiscardVertexBufferization2(afxVertexBuffer vbuf, afxNat page, afxNat32 base, afxNat32 range)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    AfxAssertRange(vbuf->pageCnt, page, 1);
    AfxAssert(range);
    AfxAssert(AfxIsAligned(base, 64));
    AfxAssert(AfxIsAligned(range, 64));

    if (vbuf->pageCnt > page)
    {
        if (vbuf->pages && vbuf->pages[page])
        {
            AfxAssertRange(vbuf->pages[page]->bufSiz, base, range);

            afxVertexBufferRoom* room;
            AfxChainForEveryLinkage(&vbuf->pages[page]->rooms, afxVertexBufferRoom, vbuf, room)
            {
                if (range < AfxAbs((afxInt64)(room->base + room->range) - (afxInt64)(base + range)))
                {
                    AfxThrowError();
                }
            }

            if (!err)
            {
                afxBool freePage = (vbuf->pages[page]->freeSiz + range == vbuf->pages[page]->bufSiz);

                if (freePage)
                {
                    afxVertexBufferRoom* room;
                    AfxChainForEveryLinkage(&vbuf->pages[page]->rooms, afxVertexBufferRoom, vbuf, room)
                    {
                        AfxPopLinkage(&room->vbuf);
                        AfxDeallocate(NIL, room);
                    }

                    AfxReleaseObjects(1, (void*[]) { vbuf->pages[page]->buf });
                    AfxReleaseObjects(1, (void*[]) { vbuf->pages[page]->layout });
                    AfxDeallocate(NIL, vbuf->pages[page]);
                    vbuf->pages[page] = NIL;
                }
                else
                {
                    // TODO try to merge to next/prev room

                    if (!(room = AfxAllocate(NIL, sizeof(*room), 1, 0, AfxHint()))) AfxThrowError();
                    else
                    {
                        room->base = base;
                        room->range = range;
                        AfxPushLinkage(&room->vbuf, &vbuf->rooms);

                        vbuf->pages[page]->freeSiz += range;
                    }
                }
            }
        }
    }
}

_AFX void AfxDiscardVertexBufferization(afxVertexBuffer vbuf, afxNat page, afxNat32 base, afxNat32 range)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    AfxAssertRange(vbuf->pageCnt, page, 1);
    AfxAssert(range);
    AfxAssert(AfxIsAligned(base, 64));
    AfxAssert(AfxIsAligned(range, 64));

    if (vbuf->pageCnt > page)
    {
        if (vbuf->pages && vbuf->pages[page])
        {
            AfxAssertRange(vbuf->pages[page]->bufSiz, base, range);

            afxVertexBufferRoom* room;
            AfxChainForEveryLinkage(&vbuf->pages[page]->rooms, afxVertexBufferRoom, vbuf, room)
            {
                if (range < AfxAbs((afxInt64)(room->base + room->range) - (afxInt64)(base + range)))
                {
                    AfxThrowError();
                }
            }

            if (!err)
            {
                afxBool freePage = (vbuf->pages[page]->freeSiz + range == vbuf->pages[page]->bufSiz);

                if (freePage)
                {
                    afxVertexBufferRoom* room;
                    AfxChainForEveryLinkage(&vbuf->pages[page]->rooms, afxVertexBufferRoom, vbuf, room)
                    {
                        AfxPopLinkage(&room->vbuf);
                        AfxDeallocate(NIL, room);
                    }

                    AfxReleaseObjects(1, (void*[]) { vbuf->pages[page]->buf });
                    AfxReleaseObjects(1, (void*[]) { vbuf->pages[page]->layout });
                    AfxDeallocate(NIL, vbuf->pages[page]);
                    vbuf->pages[page] = NIL;
                }
                else
                {
                    // TODO try to merge to next/prev room

                    if (!(room = AfxAllocate(NIL, sizeof(*room), 1, 0, AfxHint()))) AfxThrowError();
                    else
                    {
                        room->base = base;
                        room->range = range;
                        AfxPushLinkage(&room->vbuf, &vbuf->rooms);

                        vbuf->pages[page]->freeSiz += range;
                    }
                }
            }
        }
    }
}

_AFX afxError AfxRequestVertexBufferization(afxVertexBuffer vbuf, afxVertexInput layout, afxNat baseStream, afxNat streamCnt, afxNat vtxCnt, afxBufferAccess access, afxNat *page, afxNat32* base, afxNat32* range)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    
    AfxAssertObjects(1, &layout, afxFcc_VIN);
    AfxAssertRange(AfxCountVertexInputStreams(layout), baseStream, streamCnt);
    afxNat stride = 0;
    
    for (afxNat i = 0; i < streamCnt; i++)
        stride += AfxMeasureVertexInputStream(layout, baseStream + i);
    
    AfxAssert(vtxCnt);
    afxSize neededSiz = AFX_ALIGN(stride * vtxCnt, 64);
    AfxAssertRange(AfxGetBufferSize(vbuf->buf), 0, neededSiz);
    AfxAssert(AfxIsAligned(neededSiz, 64));

    if (!vbuf->pages) // create the directory if it doesnt exist
    {
        AfxAssert(!vbuf->pageCnt);

        if (!(vbuf->pages = AfxAllocate(NIL, sizeof(vbuf->pages[0]), 4, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < 4; i++)
                vbuf->pages[vbuf->pageCnt + i] = NIL;

            vbuf->pageCnt += 4;
        }
    }

    afxBool found = FALSE;

    for (afxNat pageIdx = 0; pageIdx < vbuf->pageCnt; pageIdx++)
    {
        if (vbuf->pages[pageIdx] && (vbuf->pages[pageIdx]->layout == layout) && (vbuf->pages[pageIdx]->baseStream == baseStream) && (vbuf->pages[pageIdx]->streamCnt == streamCnt))
        {
            if (neededSiz <= vbuf->pages[pageIdx]->freeSiz)
            {
                afxSize padding = 0;
                afxSize alignedBase = 0;
                afxSize alignedSiz = 0;

                afxVertexBufferRoom* room, *room2 = NIL;
                AfxChainForEveryLinkage(&vbuf->rooms, afxVertexBufferRoom, vbuf, room)
                {
                    if (!(neededSiz > room->range))
                    {
                        alignedBase = AFX_ALIGN(room->base, 64);
                        padding = AfxAbs(alignedBase - room->base);
                        alignedSiz = padding + neededSiz;

                        if (!(alignedSiz > room->range))
                        {
                            room2 = room;
                            break;
                        }
                    }
                }

                AfxAssert(AfxIsAligned(alignedBase, 64));
                AfxAssert(AfxIsAligned(alignedSiz, 64));

                if (!room2) AfxThrowError();
                else
                {
                    if (room2->range > alignedSiz)
                    {
                        room2->range = room2->range - alignedSiz;
                        room2->base = alignedBase + alignedSiz;
                    }
                    else
                    {
                        AfxPopLinkage(&room2->vbuf);
                        AfxDeallocate(NIL, room2);
                    }

                    vbuf->pages[pageIdx]->freeSiz -= alignedSiz;
                    //seg->range = alignedSiz;
                    //seg->base = offset;
                    //seg->vbuf = vbuf;

                    AfxAssert(page);
                    *page = pageIdx;
                    AfxAssert(base);
                    *base = alignedBase;
                    AfxAssert(range);
                    *range = alignedSiz;

                    found = TRUE;

                    return 0;
                    //if (IsHostVisible())
                    {
                        //seg->data = vbuf->buf->data + offset;
                    }
                }
            }
        }
    }

    if (!found && !err) // require new page
    {
        for (afxNat i = 0; i < vbuf->pageCnt; i++)
        {
            afxNat pageIdx = vbuf->pageCnt + i;

            if (!vbuf->pages[pageIdx])
            {
                if (!(vbuf->pages[pageIdx] = AfxAllocate(NIL, sizeof(vbuf->pages[0][0]), 1, NIL, AfxHint()))) AfxThrowError();
                else
                {
                    vbuf->pages[pageIdx]->baseStream = baseStream;
                    vbuf->pages[pageIdx]->streamCnt = streamCnt;
                    vbuf->pages[pageIdx]->bufAccess = access;
                    vbuf->pages[pageIdx]->bufSiz = neededSiz;
                    vbuf->pages[pageIdx]->freeSiz = 0;
                    AfxTakeChain(&vbuf->pages[pageIdx]->rooms, vbuf);
                    vbuf->pages[pageIdx]->stride = stride;
                    vbuf->pages[pageIdx]->vtxSiz = vtxSiz;
                    vbuf->pages[pageIdx]->vtxSizAligned = vtxSizAligned;

                    afxDrawContext dctx = AfxGetObjectProvider(vbuf);

                    afxBufferSpecification bufSpec = { 0 };
                    bufSpec.access = vbuf->pages[pageIdx]->bufAccess;
                    bufSpec.siz = vbuf->pages[pageIdx]->bufSiz;
                    bufSpec.src = NIL;
                    bufSpec.usage = afxBufferUsage_VERTEX;

                    if (AfxAcquireBuffers(dctx, 1, &bufSpec, &vbuf->pages[pageIdx]->buf)) AfxThrowError();
                    else
                    {

                    }
                }
                break;
            }
        }

        afxNat pageIdx = vbuf->pageCnt;

        if (!vbuf->pages && !(vbuf->pages = AfxAllocate(NIL, sizeof(vbuf->pages[0]), 4, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < 4; i++)
                vbuf->pages[vbuf->pageCnt + i] = NIL;

            vbuf->pageCnt = 4;

            if (0 == vbuf->pageCnt || neo)
            {
                void **pgs;

                if (!(pgs = AfxReallocate(NIL, vbuf->pages, sizeof(vbuf->pages[0]), vbuf->pageCnt + 1, 0, AfxHint()))) AfxThrowError();
                else
                {
                    vbuf->pages = pgs;

                    vbuf->pages[pageIdx]->data = NIL;
                    vbuf->pages[pageIdx]->usage = NIL;
                    vbuf->pages[pageIdx]->baseStream = baseStream;
                    vbuf->pages[pageIdx]->streamCnt = streamCnt;
                    vbuf->pages[pageIdx]->bufAccess = access;
                    vbuf->pages[pageIdx]->bufSiz = neededSiz;
                    vbuf->pages[pageIdx]->freeSiz = 0;
                    AfxTakeChain(&vbuf->pages[pageIdx]->rooms, vbuf);
                    vbuf->pages[pageIdx]->stride = stride;
                    vbuf->pages[pageIdx]->vtxSiz = vtxSiz;
                    vbuf->pages[pageIdx]->vtxSizAligned = vtxSizAligned;

                    vbuf->pageCnt += 1;
                }
            }
        }
    }

    afxNat vtxCap = vbuf->vtxCap;
    AfxAssertRange(vtxCap, 0, vtxCnt);
    afxNat bookedVtxCnt = vbuf->bookedVtxCnt;
    afxNat freeCnt = vtxCap - bookedVtxCnt;
    afxNat alignedCnt = 0;

    if (freeCnt < vtxCnt) AfxThrowError();
    else
    {
        afxSize padding = 0;
        afxSize alignedBaseVtx = 0;

        afxVertexBufferRoom* room, *room2 = NIL;
        AfxChainForEveryLinkage(&vbuf->rooms, afxVertexBufferRoom, vbuf, room)
        {
            if (!(vtxCnt > room->vtxCnt))
            {
                alignedBaseVtx = AFX_ALIGN(room->baseVtx, AfxAbs(64 / stride));
                padding = AfxAbs(alignedBaseVtx - room->baseVtx);
                alignedCnt = padding + size;

                if (!(alignedCnt > room->vtxCnt))
                {
                    room2 = (alignedCnt + bookedVtxCnt >= vtxCap) ? NIL : room;
                    break;
                }
            }
        }

        if (!room2) AfxThrowError();
        else
        {
            if (room2->vtxCnt > alignedCnt)
            {
                room2->vtxCnt = room2->vtxCnt - alignedCnt;
                room2->baseVtx = alignedBaseVtx + alignedCnt;
            }
            else
            {
                AfxPopLinkage(&room2->vbuf);
                AfxDeallocate(NIL, room2);
            }

            vbuf->bookedVtxCnt += alignedCnt;
            //seg->range = alignedSiz;
            //seg->base = offset;
            //seg->vbuf = vbuf;

            AfxAssert(baseVtx);
            *baseVtx = alignedBaseVtx;

            //if (IsHostVisible())
            {
                //seg->data = vbuf->buf->data + offset;
            }
        }
    }

    if (err)
        alignedCnt = 0;

    return alignedCnt;
}

_AFX afxNat AfxRequestVertexBufferSegment(afxVertexBuffer vbuf, afxNat vtxCnt, afxNat32* base)
{
    afxError err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    AfxAssert(vtxCnt);
    
    afxVertexInput layout = vbuf->layout;
    AfxAssertObjects(1, &layout, afxFcc_VIN);
    afxNat stride = AfxMeasureVertexInputStream(layout, vbuf->streamIdx);
    afxSize size = stride * vtxCnt;
    AfxAssertRange(AfxGetBufferSize(vbuf->buf), 0, size);

    afxNat vtxCap = vbuf->vtxCap;
    AfxAssertRange(vtxCap, 0, vtxCnt);
    afxNat bookedVtxCnt = vbuf->bookedVtxCnt;
    afxNat freeCnt = vtxCap - bookedVtxCnt;
    afxNat alignedCnt = 0;

    if (freeCnt < vtxCnt) AfxThrowError();
    else
    {
        afxSize padding = 0;
        afxSize alignedBaseVtx = 0;

        afxVertexBufferRoom* room, *room2 = NIL;
        AfxChainForEveryLinkage(&vbuf->rooms, afxVertexBufferRoom, vbuf, room)
        {
            if (!(vtxCnt > room->vtxCnt))
            {
                alignedBaseVtx = AFX_ALIGN(room->baseVtx, AfxAbs(64/ stride));
                padding = AfxAbs(alignedBaseVtx - room->baseVtx);
                alignedCnt = padding + size;

                if (!(alignedCnt > room->vtxCnt))
                {
                    room2 = (alignedCnt + bookedVtxCnt >= vtxCap) ? NIL : room;
                    break;
                }
            }
        }

        if (!room2) AfxThrowError();
        else
        {
            if (room2->vtxCnt > alignedCnt)
            {
                room2->vtxCnt = room2->vtxCnt - alignedCnt;
                room2->baseVtx = alignedBaseVtx + alignedCnt;
            }
            else
            {
                AfxPopLinkage(&room2->vbuf);
                AfxDeallocate(NIL, room2);
            }

            vbuf->bookedVtxCnt += alignedCnt;
            //seg->range = alignedSiz;
            //seg->base = offset;
            //seg->vbuf = vbuf;

            AfxAssert(baseVtx);
            *baseVtx = alignedBaseVtx;

            //if (IsHostVisible())
            {
                //seg->data = vbuf->buf->data + offset;
            }
        }
    }

    if (err)
        alignedCnt = 0;

    return alignedCnt;
}

_AFX afxError _AfxVbufDtor(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    afxDrawContext dctx = AfxGetObjectProvider(vbuf);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxVertexInput layout;
    
    if ((layout = vbuf->layout))
    {
        AfxAssertObjects(1, &layout, afxFcc_VIN);
        AfxReleaseObjects(1, (void*[]) { layout });
    }

    afxBuffer buf;

    if ((buf = vbuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        AfxReleaseObjects(1, (void*[]) { buf });
    }
    return err;
}

_AFX afxError _AfxVbufCtor(afxVertexBuffer vbuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    afxDrawContext dctx = cookie->udd[0];
    afxVertexBufferSpecification const *spec = ((afxVertexBufferSpecification const *)cookie->udd[1]) + cookie->no;
    afxNat baseStream = spec->baseStream;
    afxNat streamCnt = spec->streamCnt;

    afxVertexInput layout = vbuf->layout;
    AfxAssertObjects(1, &layout, afxFcc_VIN);
    AfxAssertRange(AfxCountVertexInputStreams(layout), baseStream, streamCnt);
    afxNat32 stride = AfxMeasureVertexInputStream(layout, baseStream);

    afxSize bufSiz = stride * spec->vtxCap;

    afxBuffer buf;
    afxBufferSpecification const bufSpec =
    {
        .siz = bufSiz,
        .access = spec->access,
        .usage = afxBufferUsage_VERTEX,
        .src = NIL
    };
    
    if (AfxAcquireBuffers(dctx, 1, &bufSpec, &buf)) AfxThrowError();
    else
    {
        vbuf->buf = buf;
        vbuf->access = spec->access;
        vbuf->vtxCap = spec->vtxCap;
        vbuf->bookedVtxCnt = 0;

        AfxTakeChain(&vbuf->rooms, vbuf);

        afxVertexInput layout = spec->layout;
        AfxAssertObjects(1, &layout, afxFcc_VIN);
        AfxReacquireObjects(1, (void*[]) { layout });
        vbuf->layout = layout;
        vbuf->streamIdx = streamIdx;
    }
    return err;
}

_AFX afxClassConfig const _AfxVbufClsConfig =
{
    .fcc = afxFcc_VBUF,
    .name = "Vertex Buffer",
    .unitsPerPage = 4,
    .size = sizeof(AFX_OBJECT(afxVertexBuffer)),
    .mmu = NIL,
    .ctor = (void*)_AfxVbufCtor,
    .dtor = (void*)_AfxVbufDtor
};

_AFX afxError AfxAcquireVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBufferSpecification const spec[], afxVertexBuffer vbuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(vbuf);

    afxClass* cls = AfxGetVertexBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_VBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)vbuf, (void const*[]) { dctx, spec }))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat32 AfxGetIndexBufferStride(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    return ibuf->idxSiz;
}

_AFX afxNat AfxGetIndexBufferUsage(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    return ibuf->bookedIdxCnt;
}

_AFX afxNat AfxGetIndexBufferCapacity(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    return ibuf->idxCap;
}

_AFX afxBuffer AfxGetIndexBuffer(afxIndexBuffer ibuf)
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

_AFX void AfxDisposeIndexBufferSegment(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt)
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
        if (!(room = AfxAllocate(NIL, sizeof(*room), 1, 0, AfxHint()))) AfxThrowError();
        else
        {
            room->baseIdx = baseIdx;
            room->idxCnt = idxCnt;
            AfxPushLinkage(&room->ibuf, &ibuf->rooms);
        }
    }
}

_AFX afxNat AfxReserveIndexBufferSegment(afxIndexBuffer ibuf, afxNat idxCnt, afxNat* baseIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_VBUF);
    AfxAssertRange(ibuf->idxCap, 0, idxCnt);
    AfxAssert(idxCnt);

    afxNat stride = ibuf->idxSiz;
    afxSize size = stride * idxCnt;
    AfxAssertRange(AfxGetBufferSize(ibuf->buf), 0, size);

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

_AFX afxError _AfxIbufDtor(afxIndexBuffer ibuf)
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

_AFX afxError _AfxIbufCtor(afxIndexBuffer ibuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxDrawContext dctx = cookie->udd[0];
    afxIndexBufferSpecification const *spec = ((afxIndexBufferSpecification const *)cookie->udd[1]) + cookie->no;

    AfxTakeChain(&ibuf->rooms, ibuf);

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

_AFX afxClassConfig const _AfxIbufClsConfig =
{
    .fcc = afxFcc_IBUF,
    .name = "Index Buffer",
    .unitsPerPage = 4,
    .size = sizeof(AFX_OBJECT(afxIndexBuffer)),
    .mmu = NIL,
    .ctor = (void*)_AfxIbufCtor,
    .dtor = (void*)_AfxIbufDtor
};

_AFX afxError AfxAcquireIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferSpecification const spec[], afxIndexBuffer ibuf[])
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
