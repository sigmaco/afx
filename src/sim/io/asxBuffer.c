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

#define _ASX_C
#define _ASX_BUFFER_C
#include "../impl/asxImplementation.h"

_ASX afxSimulation AsxGetBufferContext(asxBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);
    afxSimulation sim = AfxGetProvider(sbuf);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_ASX void* AsxGetBufferUdd(asxBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);
    return sbuf->udd;
}

_ASX afxUnit AsxGetBufferCapacity(asxBuffer sbuf, afxUnit from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);
    return AFX_MIN(sbuf->reqSiz, sbuf->reqSiz - from);
}

_ASX asxBufferUsage AsxGetBufferUsage(asxBuffer sbuf, asxBufferUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);
    return usage ? (sbuf->usage & usage) : sbuf->usage;
}

_ASX asxBufferFlags AsxGetBufferAccess(asxBuffer sbuf, asxBufferFlags access)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);
    return access ? ((sbuf->flags & asxBufferFlag_ACCESS) & access) : access;
}

_ASXINL void _AsxSanitizeBufferCopy(asxBuffer src, asxBuffer dst, asxBufferCopy const* raw, asxBufferCopy* san)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &dst);

    san->srcOffset = AFX_MIN(raw->srcOffset, src->reqSiz - 1);
    san->dstOffset = AFX_MIN(raw->dstOffset, dst->reqSiz - 1);
    san->range = AFX_CLAMP(raw->range, 1, AFX_MIN(src->reqSiz - san->srcOffset, dst->reqSiz - san->dstOffset));
}

_ASXINL void _AsxSanitizeBufferIo(asxBuffer src, asxBuffer dst, asxBufferIo const* raw, asxBufferIo* san)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &dst);

    san->srcOffset = AFX_MIN(raw->srcOffset, src->reqSiz - 1);
    san->dstOffset = AFX_MIN(raw->dstOffset, dst->reqSiz - 1);
    san->srcStride = AFX_MIN(raw->srcStride, src->reqSiz - san->srcOffset);
    san->dstStride = AFX_MIN(raw->dstStride, dst->reqSiz - san->dstOffset);
    san->rowCnt = AFX_CLAMP(raw->rowCnt, 1, AFX_MIN(raw->rowCnt - src->reqSiz / san->srcStride, raw->rowCnt - dst->reqSiz / san->dstStride));
}

_ASX void* AsxGetBufferData(asxBuffer sbuf, afxSize from)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);
    return &sbuf->mem.hostedAlloc.bytemap[from];
}

////////////////////////////////////////////////////////////////////////////////

_ASX afxError _AsxSbufDtorCb(asxBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

    while (sbuf->mem.mapPtr)
    {
        //AsxUnmapBuffer(sbuf, TRUE);
        AfxYield();
        //AFX_ASSERT(!buf->bytemap);
    }

    if (sbuf->mem.hostedAlloc.bytemap)
    {
        AfxDeallocate((void**)&sbuf->mem.hostedAlloc.bytemap, AfxHere());
    }

    if (sbuf->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf->base);
        AfxDisposeObjects(1, &sbuf->base);
    }

    return err;
}

_ASX afxError _AsxSbufCtorCb(asxBuffer sbuf, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    asxBufferInfo const* bufi = ((asxBufferInfo const*)args[1]) + invokeNo;
    AFX_ASSERT(bufi);

    AFX_ASSERT(bufi && bufi->size && bufi->usage);

    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    sbuf->tag = bufi->tag;
    sbuf->udd = bufi->udd;

    asxBuffer base = bufi->base;
    afxSize from = bufi->from;
    afxUnit size = bufi->size;

    if (!AFX_IS_ALIGNED(from, AVX_BUFFER_ALIGNMENT))
    {
        AFX_ASSERT_ALIGNMENT(from, AVX_BUFFER_ALIGNMENT);
        AfxThrowError();
        err = afxError_OUT_OF_RANGE;
        return err;
    }

    if (!base)
    {
        if (!size)
        {
            // If there is not a @base buffer, we need a size here.
            AfxThrowError();
            return err;
        }
        // If there is not a @base buffer, @from is useless and should be zero.
        AFX_ASSERT(from == 0);
        from = 0;
        // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
        // All buffer mapping requires ranges aligned to AFX_SIMD_ALIGNMENT. This alignment is ensured at AFX level.
        size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &base);

        if (base->base)
        {
            // Base buffer must be a storage buffer, not a buffer view.
            AFX_ASSERT(!base->base);
            AfxThrowError();
            return err;
        }

        afxUnit srcCap = AsxGetBufferCapacity(base, 0);

        // If a capacity is not specified, the new buffer inherits the full capacity of the base buffer, 
        // excluding the portion displaced by @from.
        if (size == 0)
            size = srcCap - from;
        else
            size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);

        // As every buffer capacity is a power of AFX_SIMD_ALIGNMENT, it should already be aligned here.
        AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);

        AFX_ASSERT_RANGE(srcCap, from, size);

        if ((from >= srcCap) ||
            (size > srcCap) ||
            (size > srcCap - from))
        {
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }
        AfxReacquireObjects(1, &base);
    }

    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);
    sbuf->reqSiz = size;
    sbuf->from = from;
    sbuf->base = base;

    if (err) return err;
#ifdef AVX_VALIDATION_ENABLED
    if (!sbuf->base)
    {
        AFX_ASSERT(sbuf->from == 0);
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &base);
        AFX_ASSERT_ALIGNMENT(sbuf->from, AVX_BUFFER_ALIGNMENT);
    }
#endif

    sbuf->usage = bufi->usage;
    sbuf->flags = bufi->flags;
    sbuf->fmt = bufi->fmt;

    afxUnit exuCnt = 16; // TODO Get it from DSYS
    sbuf->sharingMask = NIL;
    for (afxUnit i = 0; i < exuCnt; i++)
    {
        sbuf->sharingMask |= bufi->sharingMask & AFX_BITMASK(i);
    }

    // STORAGE
    sbuf->reqMemType = NIL;
    sbuf->reqAlign = AFX_SIMD_ALIGNMENT;
    sbuf->memBase = 0;

    _asxMemory* smem = &sbuf->mem;
    smem->size = sbuf->reqSiz;
    smem->alignment = AFX_SIMD_ALIGNMENT;

    // binding
    smem->mmu = 0;
    //sbuf->storage[0].offset = 0;
    smem->hostedAlloc.addr = NIL;

    smem->mapPtr = NIL;
    smem->mapOffset = 0;
    smem->mapRange = 0;
    smem->mapFlags = NIL;

    smem->pendingRemap = 0;
    smem->permanentlyMapped = !!bufi->mapped;

    AfxAllocate(smem->size, smem->alignment, AfxHere(), (void**)&smem->hostedAlloc.bytemap);

    smem->mapPtr = smem->hostedAlloc.bytemap;
    smem->mapRange = smem->size;
    smem->permanentlyMapped = TRUE;
    

    return err;
}

_ASX afxClassConfig const _ASX_SBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_SBUF,
    .name = "SynergicBuffer",
    .desc = "Synergic Memory Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(asxBuffer)),
    .ctor = (void*)_AsxSbufCtorCb,
    .dtor = (void*)_AsxSbufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireBuffers(afxSimulation sim, afxUnit cnt, asxBufferInfo const info[], asxBuffer buffers[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxGetBufferClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_SBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { sim, info }))
    {
        AfxThrowError();
    }
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, cnt, buffers);

    return err;
}
