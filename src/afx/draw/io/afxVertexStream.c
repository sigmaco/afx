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
#define _AVX_VERTEX_BUFFER_C
#define _AVX_INDEX_BUFFER_C
#include "../dev/AvxDevKit.h"

typedef struct _rwD3D9freeVBlistEntry RxD3D9freeVBlistEntry;
struct _rwD3D9freeVBlistEntry
{
    afxNat32                offset;
    afxNat32                size;
    void                    *vbo;
    RxD3D9freeVBlistEntry  *next;
    RxD3D9freeVBlistEntry  *prev;
};

typedef struct _rwD3D9createdVBlistEntry RxD3D9createdVBlistEntry;
struct _rwD3D9createdVBlistEntry
{
    void        *vertexBuffer;
    RxD3D9createdVBlistEntry  *next;
};

typedef struct _rwD3D9StrideEntry RxD3D9StrideEntry;
struct _rwD3D9StrideEntry
{
    afxNat32                    stride;
    RxD3D9freeVBlistEntry       *freelist;
    RxD3D9createdVBlistEntry    *vblist;
    RxD3D9StrideEntry           *next;
};

static afxNat32         DefaultVBSize = 128 * 1024;

static RxD3D9StrideEntry    *StrideList = NULL;

static afxSlabAllocator *StrideFreeList = NULL;
static afxSlabAllocator *FreeVBFreeList = NULL;
static afxSlabAllocator *CreatedVBFreeList = NULL;

afxBool RwD3D9CreateVertexBuffer(afxNat32 stride, afxNat32 size, void **vertexBuffer, afxNat32 *offset)
{
    RxD3D9StrideEntry           *strideList;
    RxD3D9freeVBlistEntry       *freelist;
    RxD3D9createdVBlistEntry    *vblist;
    afxNat32                    nextOffset;
    afxError err;
    AfxAssert(stride <= size);
    AfxAssert((stride & 0x3) == 0);
    AfxAssert(vertexBuffer);
    AfxAssert(offset);
    strideList = StrideList;

    afxDrawContext dctx;

    while (strideList) // Search for an available stride
    {
        RxD3D9StrideEntry  *next = strideList->next;

        if (stride == strideList->stride)
            break;

        strideList = next;
    }

    if (!strideList) // create one if there isn't any
    {
        strideList = AfxAllocateSlab(&StrideFreeList);
        strideList->stride = stride;
        strideList->freelist = NULL;
        strideList->vblist = NULL;

        strideList->next = StrideList; // Add it to the begining
        StrideList = strideList;
    }

    freelist = strideList->freelist;

    while (freelist) // Search for an available free space
    {
        RxD3D9freeVBlistEntry   *nextFreelist = freelist->next;

        if (freelist->size >= size)
        {
            break;
        }

        freelist = nextFreelist;
    }

    if (!freelist) // If there is not free space, create a new VB
    {
        freelist = AfxAllocateSlab(&FreeVBFreeList);
        freelist->offset = 0;
        freelist->size = (((DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
            freelist->size = size;

        afxBufferSpecification spec = { 0 };
        spec.siz = freelist->size;
        spec.access = afxBufferAccess_W;
        spec.usage = afxBufferUsage_VERTEX;

        if (AfxAcquireBuffers(dctx, 1, &spec, &freelist->vbo))
        {
            AfxDeallocateSlab(&FreeVBFreeList, freelist);            
            return FALSE;
        }

        freelist->next = strideList->freelist; // Add it to the begining
        freelist->prev = NULL;

        if (strideList->freelist)
            strideList->freelist->prev = freelist;

        strideList->freelist = freelist;

        // Add an entry to the vb list
        vblist = AfxAllocateSlab(&CreatedVBFreeList);
        vblist->vertexBuffer = freelist->vbo;
        vblist->next = strideList->vblist;
        strideList->vblist = vblist;

#if defined(RWDEBUG)
        VBCreated++;
#endif
    }

    *vertexBuffer = freelist->vbo;
    *offset = freelist->offset;
    nextOffset = (*offset) + size;

    AfxAssert(freelist->size >= (nextOffset - freelist->offset));
    freelist->size -= nextOffset - freelist->offset;
    freelist->offset = nextOffset;

    if (freelist->size <= 0)
    {
        if (freelist->prev)
            freelist->prev->next = freelist->next;

        if (freelist->next)
            freelist->next->prev = freelist->prev;

        if (strideList->freelist == freelist)
            strideList->freelist = freelist->next;

        AfxDeallocateSlab(&FreeVBFreeList, freelist);
    }

#if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved += size;
#endif

    return TRUE;
}

void RwD3D9DestroyVertexBuffer(afxNat32 stride, afxNat32 size, void *vertexBuffer, afxNat32 offset)
{
    RxD3D9StrideEntry* strideList;
    RxD3D9freeVBlistEntry* freelist;
    afxError err;
    AfxAssert(stride <= size);
    AfxAssert((stride & 0x3) == 0);
    AfxAssert(vertexBuffer != NULL);

    // Search for the stride
    strideList = StrideList;

    while (strideList )
    {
        RxD3D9StrideEntry* next = strideList->next;

        if (stride == strideList->stride)
            break;

        strideList = next;
    }

    if (strideList)
    {
        // Search for an available free space to add
        freelist = strideList->freelist;

        while (freelist)
        {
            RxD3D9freeVBlistEntry* nextFreelist = freelist->next;

            if (freelist->vbo == vertexBuffer)
            {
                // Could be added to the end of the free space?

                if ((freelist->offset + freelist->size) == offset) break;
                else
                {
                    // Could be added to the begin of the free space?

                    if (offset + size == freelist->offset)
                        break;
                }
            }
            freelist = nextFreelist;
        }

        if (freelist)
        {
            afxBool canCompactMore = FALSE;
            afxNat32 newoffset = offset;
            afxNat32 newSize = size;
            RxD3D9freeVBlistEntry* oldFreelist = freelist;

            do
            {
                // Could be added to the end of the free space?

                if ((freelist->offset + freelist->size) == newoffset) freelist->size += newSize;
                else
                {
                    // Could be added to the begin of the free space?

                    if (newoffset + newSize == freelist->offset)
                    {
                        freelist->offset = newoffset;
                        freelist->size += newSize;
                    }
                }

                // Could be compact more?
                newoffset = freelist->offset;
                newSize = freelist->size;
                oldFreelist = freelist;
                canCompactMore = FALSE;
                freelist = strideList->freelist;

                while (freelist)
                {
                    RxD3D9freeVBlistEntry* nextFreelist = freelist->next;

                    if (freelist->vbo == vertexBuffer)
                    {
                        // Could be added to the end of the free space?

                        if ((freelist->offset + freelist->size) == newoffset)
                        {
                            canCompactMore = TRUE;
                            break;
                        }
                        else
                        {
                            // Could be added to the begin of the free space?

                            if (newoffset + newSize == freelist->offset)
                            {
                                canCompactMore = TRUE;
                                break;
                            }
                        }
                    }
                    freelist = nextFreelist;
                }

                // Remove old list

                if (canCompactMore)
                {
                    if (oldFreelist->prev)
                        oldFreelist->prev->next = oldFreelist->next;

                    if (oldFreelist->next)
                        oldFreelist->next->prev = oldFreelist->prev;

                    if (strideList->freelist == oldFreelist)
                        strideList->freelist = oldFreelist->next;

                    AfxDeallocateSlab(&FreeVBFreeList, oldFreelist);
                }
            } while (canCompactMore);
        }
        else
        {
            freelist = AfxAllocateSlab(&FreeVBFreeList);
            freelist->offset = offset;
            freelist->size = size;
            freelist->vbo = vertexBuffer;

            // Add it to the begining
            freelist->next = strideList->freelist;
            freelist->prev = NULL;

            if (strideList->freelist)
                strideList->freelist->prev = freelist;

            strideList->freelist = freelist;
        }

#if defined(RWDEBUG)
        BlocksCreated--;
        BytesReserved -= size;
#endif
    }
}



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

_AVX afxError _AvxVbufDtor(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    //afxDrawContext dctx = AfxGetObjectProvider(vbuf);
    //afxMmu mmu = AfxGetDrawContextMmu(dctx);
    //AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxBuffer buf;

    if ((buf = vbuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        AfxReleaseObjects(1, &buf);
    }
    return err;
}

_AVX afxError _AvxVbufCtor(afxVertexBuffer vbuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    afxDrawInput din = cookie->udd[0];
    afxVertexBufferSpecification const *spec = ((afxVertexBufferSpecification const *)cookie->udd[1]) + cookie->no;

    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);

    afxSize bufSiz = spec->bufCap;

    afxBuffer buf;
    afxBufferSpecification const bufSpec =
    {
        .siz = AFX_ALIGNED_SIZEOF(bufSiz, 64),
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

        AfxDeployChain(&vbuf->rooms, vbuf);
    }
    return err;
}

_AVX afxClassConfig const _AvxVbufMgrCfg =
{
    .fcc = afxFcc_VBUF,
    .name = "Vertex Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxVertexBuffer)),
    .ctor = (void*)_AvxVbufCtor,
    .dtor = (void*)_AvxVbufDtor
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
        if (!(room = AfxAllocate(1, sizeof(*room), 0, AfxHere()))) AfxThrowError();
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
    AfxAssertRange(AfxGetBufferCapacity(ibuf->buf, 0), 0, size);

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
                alignedBaseIdx = AFX_ALIGNED_SIZEOF(room->baseIdx, (64 / stride));
                padding = (alignedBaseIdx - room->baseIdx);
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
                AfxDeallocate(room2);
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

_AVX afxError _AvxIbufDtor(afxIndexBuffer ibuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxDrawContext dctx = AfxGetObjectProvider(ibuf);

    afxBuffer buf;

    if ((buf = ibuf->buf))
    {
        AfxAssertObjects(1, &buf, afxFcc_BUF);
        AfxReleaseObjects(1, &buf);
    }
    return err;
}

_AVX afxError _AvxIbufCtor(afxIndexBuffer ibuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxDrawContext dctx = cookie->udd[0];
    afxIndexBufferSpecification const *spec = ((afxIndexBufferSpecification const *)cookie->udd[1]) + cookie->no;

    AfxDeployChain(&ibuf->rooms, ibuf);

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

_AVX afxClassConfig const _AvxIbufMgrCfg =
{
    .fcc = afxFcc_IBUF,
    .name = "Index Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(afxIndexBuffer)),
    .ctor = (void*)_AvxIbufCtor,
    .dtor = (void*)_AvxIbufDtor
};

_AVX afxError AfxAcquireIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferSpecification const spec[], afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(ibuf);

    afxClass* cls = 0;// AfxGetIndexBufferClass(dctx);
    AfxAssertClass(cls, afxFcc_IBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ibuf, (void const*[]) { dctx, spec }))
        AfxThrowError();

    return err;
}
