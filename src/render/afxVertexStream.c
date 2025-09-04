/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ARX_VERTEX_BUFFER_C
#define _ARX_INDEX_BUFFER_C
#define _ARX_DRAW_INPUT_C
#include "ddi/arxImpl_Input.h"
#include "qwadro/inc/math/afxReal16.h"

void* aligned_alloc_buffer(_arxFrameBufferizerChunk* chunk, afxSize size, afxUnit align, afxSize* out_offset) {
    afxSize aligned_offset = (chunk->used + align - 1) & ~(align - 1);
    if (aligned_offset + size > chunk->capacity) return NULL;

    *out_offset = aligned_offset;
    chunk->used = aligned_offset + size;
    return (char*)chunk->mapped_ptr + aligned_offset;
}

_arxFrameBufferizerChunk create_new_chunk(arxBufferedPump* alloc, afxSize size)
{
    _arxFrameBufferizerChunk chunk = { 0 };
    chunk.capacity = size;
    chunk.used = 0;
    chunk.frame_in_use = -1;

    avxBuffer buf;
    avxBufferInfo bufi = { 0 };
    bufi.usage = alloc->bufUsage;
    bufi.flags = alloc->bufFlags;
    bufi.size = chunk.capacity;
    AvxAcquireBuffers(alloc->dsys, 1, &bufi, &buf);
    chunk.buffer = buf;

    AvxMapBuffer(buf, 0, bufi.size, NIL, &chunk.mapped_ptr);

    if (!chunk.mapped_ptr)
    {
        afxError err;
        AfxThrowError();
    }
    return chunk;
}

void ArxDeployBufferedPump(arxBufferedPump* alloc)
{
    alloc->capacity_chunks = 4;
    alloc->chunks = (_arxFrameBufferizerChunk*)malloc(sizeof(_arxFrameBufferizerChunk) * alloc->capacity_chunks);
    alloc->num_chunks = 0;
    alloc->current_frame = 0;
    alloc->rounds = 3; // triple buffered
    alloc->bufUsage = avxBufferUsage_UNIFORM;
    alloc->bufFlags = avxBufferFlag_WX;
    alloc->minChunkSiz = (2 * 1024 * 1024); // 2MB;
    alloc->blockAlign = 256; // 256 = uniform alignment
}

_arxFrameBufferizerChunk* find_available_chunk(arxBufferedPump* alloc, afxSize size_needed)
{
    if (alloc->last)
    {
        _arxFrameBufferizerChunk* chunk = alloc->last;
        if (chunk->frame_in_use < alloc->current_frame - alloc->rounds)
        {
            chunk->used = 0;
            chunk->frame_in_use = alloc->current_frame;
        }
        afxSize offset;
        if (aligned_alloc_buffer(chunk, size_needed, alloc->blockAlign, &offset))
        {
            return chunk;
        }
    }

    for (afxSize i = 0; i < alloc->num_chunks; ++i)
    {
        _arxFrameBufferizerChunk* chunk = &alloc->chunks[i];
        if (chunk->frame_in_use < alloc->current_frame - alloc->rounds)
        {
            chunk->used = 0;
            chunk->frame_in_use = alloc->current_frame;
        }
        afxSize offset;
        if (aligned_alloc_buffer(chunk, size_needed, alloc->blockAlign, &offset))
        {
            return chunk;
        }
    }

    // Allocate new chunk
    if (alloc->num_chunks >= alloc->capacity_chunks)
    {
        alloc->capacity_chunks *= 2;
        alloc->chunks = (_arxFrameBufferizerChunk*)realloc(alloc->chunks, sizeof(_arxFrameBufferizerChunk) * alloc->capacity_chunks);
    }

    _arxFrameBufferizerChunk new_chunk = create_new_chunk(alloc, alloc->minChunkSiz > size_needed ? alloc->minChunkSiz : size_needed);
    new_chunk.frame_in_use = alloc->current_frame;
    alloc->chunks[alloc->num_chunks++] = new_chunk;
    return &alloc->chunks[alloc->num_chunks - 1];
}

void* ArxRequestBufferedPump(arxBufferedPump* alloc, afxSize size, avxBuffer* out_buffer, afxSize* out_offset)
{
    _arxFrameBufferizerChunk* chunk = find_available_chunk(alloc, size);
    alloc->last = chunk;
    void* ptr = aligned_alloc_buffer(chunk, size, alloc->blockAlign, out_offset);
    *out_buffer = chunk->buffer;
    return ptr;
}

void ArxAdvanceBufferedPump(arxBufferedPump* alloc)
{
    alloc->current_frame += 1;
}

void ArxDismantleBufferedPump(arxBufferedPump* alloc)
{
    for (afxSize i = 0; i < alloc->num_chunks; ++i)
    {
        AvxUnmapBuffer(alloc->chunks[i].buffer, FALSE);
        AfxDisposeObjects(1, &alloc->chunks[i].buffer);
    }
    free(alloc->chunks);
}

AFX_DEFINE_STRUCT(freeVBlistEntry)
{
    freeVBlistEntry*        next;
    freeVBlistEntry*        prev;
    avxBuffer               vbo;
    afxUnit32               offset;
    afxUnit32               size;
};

AFX_DEFINE_STRUCT(_arxSharedBuffer)
{
    //createdVBlistEntry* next;
    avxBuffer           buf;
    avxBufferUsage      reqUsage;
    avxBufferFlags      reqFlags;
    afxUnit             usedCnt;
};

AFX_DEFINE_STRUCT(_arxVertexCache)
{
    afxUnit sharedBufId;
    afxSize offset;
    afxUnit range;
    afxUnit stride;
};

AFX_DEFINE_STRUCT(createdVBlistEntry)
{
    createdVBlistEntry*     next;
    avxBuffer               vbo;
};

AFX_DEFINE_STRUCT(StrideEntry)
{
    StrideEntry*            next;
    afxUnit32               stride;
    freeVBlistEntry*        freelist;
    createdVBlistEntry*     vblist;
};

AFX_DEFINE_STRUCT(DynamicVertexBuffer)
{
    DynamicVertexBuffer*    next;
    avxBuffer               vbo;
    afxUnit32               size;
    afxUnit32               used;
    avxBuffer*              variableAddress;
};

#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER       4
#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE (256*1024)

AFX_OBJECT(arxBufferizer)
{
    arxRenderware        din;
    afxUnit32           DefaultVBSize;

    StrideEntry*        StrideList;

    afxSlabAllocator    StrideFreeList;
    afxSlabAllocator    FreeVBFreeList;
    afxSlabAllocator    CreatedVBFreeList;

    DynamicVertexBuffer*DynamicVertexBufferList;
    afxSlabAllocator    DynamicVertexBufferFreeList;

    afxUnit32           CurrentDynamicVertexBufferManager;
    afxUnit32           OffSetDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];
    afxUnit32           SizeDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];
    avxBuffer           DynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];

};

#define GET_ADDR(base_, stride_, idx_) (void*)((afxByte*)base_ + stride_ * idx_)
#define GET_CONST_ADDR(base_, stride_, idx_) (void const*)((afxByte const*)base_ + stride_ * idx_)

#if 0
_ARX afxError AfxConvert2
(
    afxUnit cnt,
    void const* src,
    afxVertexFormat srcFmt,
    afxUnit srcStride,
    void* dst,
    afxVertexFormat dstFmt,
    afxUnit dstStride,
    afxBool normalized
)
{
    afxError err = AFX_ERR_NONE;

    afxUnit srcCompCnt = 0;
    afxUnit dstCompCnt = 0;

    switch (srcFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R16:
    case afxVertexFormat_U32:
    case afxVertexFormat_I32:
    case afxVertexFormat_U16:
    case afxVertexFormat_I16:
    case afxVertexFormat_U8:
    case afxVertexFormat_I8:
        srcCompCnt = 1; break;
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_U32x2:
    case afxVertexFormat_I32x2:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_I16x2:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_I8x2:
        srcCompCnt = 2; break;
    case afxVertexFormat_R32x3:
        //case afxVertexFormat_U32x3:
        //case afxVertexFormat_I32x3:
        srcCompCnt = 3; break;
    case afxVertexFormat_R32x4:
    case afxVertexFormat_R16x4:
    case afxVertexFormat_U32x4:
    case afxVertexFormat_I32x4:
    case afxVertexFormat_U16x4:
    case afxVertexFormat_I16x4:
    case afxVertexFormat_U8x4:
    case afxVertexFormat_I8x4:
        srcCompCnt = 4; break;
    default:
        break;
    }

    switch (dstFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R16:
    case afxVertexFormat_U32:
    case afxVertexFormat_I32:
    case afxVertexFormat_U16:
    case afxVertexFormat_I16:
    case afxVertexFormat_U8:
    case afxVertexFormat_I8:
        dstCompCnt = 1; break;
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_U32x2:
    case afxVertexFormat_I32x2:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_I16x2:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_I8x2:
        dstCompCnt = 2; break;
    case afxVertexFormat_R32x3:
        //case afxVertexFormat_U32x3:
        //case afxVertexFormat_I32x3:
        dstCompCnt = 3; break;
    case afxVertexFormat_R32x4:
    case afxVertexFormat_R16x4:
    case afxVertexFormat_U32x4:
    case afxVertexFormat_I32x4:
    case afxVertexFormat_U16x4:
    case afxVertexFormat_I16x4:
    case afxVertexFormat_U8x4:
    case afxVertexFormat_I8x4:
        dstCompCnt = 4; break;
    default:
        break;
    }

    switch (dstFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R32x3:
    case afxVertexFormat_R32x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32:
        case afxVertexFormat_R32x2:
        case afxVertexFormat_R32x3:
        case afxVertexFormat_R32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_R16:
        case afxVertexFormat_R16x2:
        case afxVertexFormat_R16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxUnpackReal16(src4[j]);
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxVertexFormat_R16:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_R16x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32:
        case afxVertexFormat_R32x2:
        case afxVertexFormat_R32x3:
        case afxVertexFormat_R32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16(src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_R16:
        case afxVertexFormat_R16x2:
        case afxVertexFormat_R16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxVertexFormat_U32:
    case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
    case afxVertexFormat_U32x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat32
        case afxVertexFormat_R32x2: // real32 to nat32
        case afxVertexFormat_R32x3: // real32 to nat32
        case afxVertexFormat_R32x4: // real32 to nat32
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    case afxVertexFormat_U16:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_U16x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x2: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x3: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x4: // real32 to nat8 (normalized or raw)
        {
            if (normalized)
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j] * 65535.f;
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            else
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j];
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    case afxVertexFormat_U8:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_U8x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x2: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x3: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x4: // real32 to nat8 (normalized or raw)
        {
            if (normalized)
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j] * 255.f;
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            else
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j];
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    default: AfxThrowError();  break;
    }

    return err;
}
#endif



void BufferizeTriangles(arxBufferizer vbMgr, afxUnit idxCnt, afxUnit vtxCnt)
{

}


afxBool CreateVertexBuffer(arxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, avxBuffer* vbo, afxUnit32* offset)
{
    // based on RwD3D9CreateVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo);
    AFX_ASSERT(offset);
    StrideEntry* strideList = vbMgr->StrideList;

    while (strideList) // Search for an available stride
    {
        if (stride == strideList->stride)
            break;

        strideList = strideList->next;
    }

    if (!strideList)
    {
        // create one if there isn't any
        strideList = AfxPushSlabUnit(&vbMgr->StrideFreeList);
        AFX_ASSERT(strideList);
        strideList->stride = stride;
        strideList->freelist = NULL;
        strideList->vblist = NULL;

        // Add it to the begining
        strideList->next = vbMgr->StrideList;
        vbMgr->StrideList = strideList;
    }

    freeVBlistEntry* freelist = strideList->freelist;

    while (freelist) // Search for an available free space
    {
        if (freelist->size >= size)
            break;

        freelist = freelist->next;
    }

    if (!freelist)
    {
        // If there is not free space, create a new VB
        freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
        AFX_ASSERT(freelist);
        freelist->offset = 0;
        freelist->size = (((vbMgr->DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
            freelist->size = size;
        
        afxDrawSystem dsys = ArxGetDrawInputContext(vbMgr->din);

        avxBuffer vbo;
        avxBufferInfo spec = { 0 };
        spec.size = freelist->size;
        spec.flags = avxBufferFlag_W;
        spec.usage = avxBufferUsage_VERTEX;

        if (AvxAcquireBuffers(dsys, 1, &spec, &vbo))
        {
            AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);            
            return FALSE;
        }
        
        freelist->vbo = vbo;
        freelist->next = strideList->freelist; // Add it to the begining
        freelist->prev = NULL;

        if (strideList->freelist)
            strideList->freelist->prev = freelist;

        strideList->freelist = freelist;

        // Add an entry to the vb list
        createdVBlistEntry* vblist = AfxPushSlabUnit(&vbMgr->CreatedVBFreeList);
        vblist->vbo = vbo;
        vblist->next = strideList->vblist;
        strideList->vblist = vblist;

#if defined(RWDEBUG)
        VBCreated++;
#endif
    }

    *vbo = freelist->vbo;
    *offset = freelist->offset;
    afxUnit32 nextOffset = (*offset) + size;

    AFX_ASSERT(freelist->size >= (nextOffset - freelist->offset));
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

        AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);
    }

#if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved += size;
#endif

    return TRUE;
}

void DestroyVertexBuffer(arxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, avxBuffer vbo, afxUnit32 offset)
{
    // based on RwD3D9DestroyVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo != NULL);

    // Search for the stride
    StrideEntry* strideList = vbMgr->StrideList;

    while (strideList )
    {
        StrideEntry* next = strideList->next;

        if (stride == strideList->stride)
            break;

        strideList = next;
    }

    if (strideList)
    {
        // Search for an available free space to add
        freeVBlistEntry* freelist = strideList->freelist;

        while (freelist)
        {
            freeVBlistEntry* nextFreelist = freelist->next;

            if (freelist->vbo == vbo)
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
            afxUnit32 newoffset = offset;
            afxUnit32 newSize = size;
            freeVBlistEntry* oldFreelist = freelist;

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
                    freeVBlistEntry* nextFreelist = freelist->next;

                    if (freelist->vbo == vbo)
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

                    AfxPopSlabUnit(&vbMgr->FreeVBFreeList, oldFreelist);
                }
            } while (canCompactMore);
        }
        else
        {
            freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
            freelist->offset = offset;
            freelist->size = size;
            freelist->vbo = vbo;

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

afxBool DynamicVertexBufferCreate(arxBufferizer vbMgr, afxUnit size, avxBuffer *vertexBuffer)
{
    afxError err = NIL;
    DynamicVertexBuffer *freeVertexBuffer = NULL;
    DynamicVertexBuffer *currentVertexBuffer = vbMgr->DynamicVertexBufferList;

    while (currentVertexBuffer)
    {
        DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (!currentVertexBuffer->used)
        {
            if (currentVertexBuffer->size == size)
            {
                break;
            }
            else if (freeVertexBuffer == NULL || freeVertexBuffer->size < currentVertexBuffer->size)
            {
                freeVertexBuffer = currentVertexBuffer;
            }
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = TRUE;

        *vertexBuffer = currentVertexBuffer->vbo;

        currentVertexBuffer->variableAddress = vertexBuffer;
    }
    else
    {
        /* Destroy a free vertex buffer to make room for the new one */
        if (freeVertexBuffer)
        {
            AfxDisposeObjects(1, &freeVertexBuffer->vbo);
            freeVertexBuffer->vbo = NULL;

            currentVertexBuffer = freeVertexBuffer;

#if defined(RWDEBUG)
            NumDynamicVertexBuffer--;
#endif /* defined(RWDEBUG) */
        }
        else
        {
            /* Create a new item */
            currentVertexBuffer = AfxPushSlabUnit(&vbMgr->DynamicVertexBufferFreeList);

            /* Add it to the beginnig of the list */
            currentVertexBuffer->next = vbMgr->DynamicVertexBufferList;

            vbMgr->DynamicVertexBufferList = currentVertexBuffer;
        }

        afxDrawSystem dsys = ArxGetDrawInputContext(vbMgr->din);

        avxBufferInfo spec = { 0 };
        spec.size = size;
        spec.usage = avxBufferUsage_VERTEX;
        spec.flags = avxBufferFlag_W;
        
        if (AvxAcquireBuffers(dsys, 1, &spec, vertexBuffer))
        {
            AfxThrowError();
            return FALSE;
        }

        /* Fill item info */
        currentVertexBuffer->size = size;
        currentVertexBuffer->used = TRUE;
        currentVertexBuffer->vbo = (*vertexBuffer);
        currentVertexBuffer->variableAddress = (vertexBuffer);

#if defined(RWDEBUG)
        NumDynamicVertexBuffer++;
#endif /* defined(RWDEBUG) */
    }

    return TRUE;
}

void DynamicVertexBufferDestroy(arxBufferizer vbMgr, void *vertexBuffer)
{
    DynamicVertexBuffer *currentVertexBuffer = vbMgr->DynamicVertexBufferList;

    while (currentVertexBuffer)
    {
        DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (currentVertexBuffer->vbo == (vertexBuffer))
        {
            break;
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = FALSE;
        currentVertexBuffer->variableAddress = NULL;
    }
}

afxBool _VertexBufferManagerOpen(arxBufferizer vbMgr)
{
    afxError err = NIL;
    //arxVertexBufferizer* vbMgr = AfxAllocate(1, sizeof(vbMgr[0]), 0, AfxHere());
    //*vbMgrPtr = vbMgr;

    vbMgr->DefaultVBSize = 128 * 1024;
    AfxDeploySlabAllocator(&vbMgr->StrideFreeList, sizeof(StrideEntry), 16);
    AfxDeploySlabAllocator(&vbMgr->FreeVBFreeList, sizeof(freeVBlistEntry), 100);
    AfxDeploySlabAllocator(&vbMgr->CreatedVBFreeList, sizeof(createdVBlistEntry), 100);

    AfxDeploySlabAllocator(&vbMgr->DynamicVertexBufferFreeList, sizeof(DynamicVertexBuffer), 4);

    afxBool created;
    afxUnit32 n;

    vbMgr->CurrentDynamicVertexBufferManager = 0;

    for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        vbMgr->OffSetDynamicVertexBufferManager[n] = 0;

        vbMgr->SizeDynamicVertexBufferManager[n] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

        created = DynamicVertexBufferCreate(vbMgr, MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE, &(vbMgr->DynamicVertexBufferManager[n]));

        if (!created)
        {
            AfxThrowError();
        }
    }

    return TRUE;
}

void _VertexBufferManagerClose(arxBufferizer vbMgr)
{
    //arxVertexBufferizer* vbMgr = *vbMgrPtr;
#if defined(RWDEBUG)
    AFX_ASSERT(BlocksCreated == 0);
    AFX_ASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    // D3D9DynamicVertexBufferManagerDestroy();

    vbMgr->CurrentDynamicVertexBufferManager = 0;

    for (afxUnit n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        vbMgr->OffSetDynamicVertexBufferManager[n] = 0;

        vbMgr->SizeDynamicVertexBufferManager[n] = 0;

        if (vbMgr->DynamicVertexBufferManager[n])
        {
            DynamicVertexBufferDestroy(vbMgr, vbMgr->DynamicVertexBufferManager[n]);

            vbMgr->DynamicVertexBufferManager[n] = NULL;
        }
    }

    // D3D9DynamicVertexBufferListDestroy();

    while (vbMgr->DynamicVertexBufferList)
    {
        DynamicVertexBuffer *next = vbMgr->DynamicVertexBufferList->next;

        if (vbMgr->DynamicVertexBufferList->vbo)
        {
            AfxDisposeObjects(1, &vbMgr->DynamicVertexBufferList->vbo);

            vbMgr->DynamicVertexBufferList->vbo = NULL;

            if (vbMgr->DynamicVertexBufferList->variableAddress)
            {
                *vbMgr->DynamicVertexBufferList->variableAddress = NULL;
            }
        }

        AfxPopSlabUnit(&vbMgr->DynamicVertexBufferFreeList, vbMgr->DynamicVertexBufferList);

        vbMgr->DynamicVertexBufferList = next;
    }

    AfxDismantleSlabAllocator(&vbMgr->DynamicVertexBufferFreeList);

#if defined(RWDEBUG)
    NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

    // ---

    while (vbMgr->StrideList != NULL)
    {
        StrideEntry  *next = vbMgr->StrideList->next;

        /*  Destroy the free list */
        while (vbMgr->StrideList->freelist != NULL)
        {
            freeVBlistEntry   *nextFreelist = vbMgr->StrideList->freelist->next;

            AfxPopSlabUnit(&vbMgr->FreeVBFreeList, vbMgr->StrideList->freelist);

            vbMgr->StrideList->freelist = nextFreelist;
        }

        /*  Destroy the VB list */
        while (vbMgr->StrideList->vblist != NULL)
        {
            createdVBlistEntry   *nextvblist = vbMgr->StrideList->vblist->next;

            if (vbMgr->StrideList->vblist->vbo != NULL)
            {
                AfxDisposeObjects(1, &(vbMgr->StrideList->vblist->vbo));
            }

            AfxPopSlabUnit(&vbMgr->CreatedVBFreeList, vbMgr->StrideList->vblist);

            vbMgr->StrideList->vblist = nextvblist;
        }

        AfxPopSlabUnit(&vbMgr->StrideFreeList, vbMgr->StrideList);

        vbMgr->StrideList = next;
    }

    AfxDismantleSlabAllocator(&vbMgr->CreatedVBFreeList);
    AfxDismantleSlabAllocator(&vbMgr->FreeVBFreeList);
    AfxDismantleSlabAllocator(&vbMgr->StrideFreeList);

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */
}

_ARX afxError _ArxBufzDtor(arxBufferizer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VBUF, 1, &vbuf);
    
    _VertexBufferManagerClose(vbuf);
    
    return err;
}

_ARX afxError _ArxBufzCtor(arxBufferizer vbuf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VBUF, 1, &vbuf);

    arxRenderware din = args[0];
    arxBufferizerInfo const* info = ((arxBufferizerInfo const *)args[1]) + invokeNo;

    _VertexBufferManagerOpen(vbuf);

    return err;
}

_ARX afxClassConfig const _ARX_VBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_VBUF,
    .name = "Bufferizer",
    .desc = "Bufferizer",
    .fixedSiz = sizeof(AFX_OBJECT(arxBufferizer)),
    .ctor = (void*)_ArxBufzCtor,
    .dtor = (void*)_ArxBufzDtor
};

_ARX afxError AfxAcquireBufferizer(arxRenderware din, arxBufferizerInfo const* info, arxBufferizer bufferizer[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(bufferizer);
    AFX_ASSERT(info);

    afxClass* cls = ArxGetVertexBufferClass(din);
    AFX_ASSERT_CLASS(cls, afxFcc_VBUF);

    if (AfxAcquireObjects(cls, 1, (afxObject*)bufferizer, (void const*[]) { din, info }))
        AfxThrowError();

    return err;
}
