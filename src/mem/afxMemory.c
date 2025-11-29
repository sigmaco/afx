/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

//#define RPMALLOC_ALLOC 1

//#undef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#ifdef RPMALLOC_ALLOC
#include "../../dep/rpmalloc/rpmalloc.h"
#endif
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>

#define _AFX_CORE_C
#define _AFX_MMU_C
#include "../exec/afxSystemDDK.h"

#define _AFX_VALIDATE_MEMSEG
#define _AFX_VALIDATE_MEMSEG_EXTREMES TRUE
#if 0
#define _AFX_REPORT_ALLOC (TRUE)
#define _AFX_REPORT_COALLOC (TRUE)
#define _AFX_REPORT_REALLOC (TRUE)
#define _AFX_REPORT_DEALLOC (TRUE)
#endif

static afxSize gMemoryBudget = 1024 * 1024 * 1024; // 1024³ = 1 GB
static afxSize gTotalAllocatedInSystem = 0; // Track total memory allocated

typedef struct _afxArbitraryChunk
{
#ifndef _AFX_DONT_DEBUG_MEM_LEAK
    afxSize             siz;
    afxUnit16            line;
    afxChar const       *file;
#endif
    _AFX_DBG_FCC;
    afxByte AFX_SIMD gap[16];
    afxByte AFX_SIMD data[];
} _afxArbitraryChunk;

// A helper to align memory to the specified alignment
#define AFX_ALIGNED_PTR(ptr_, alignment_) \
    (void*)((((uintptr_t)ptr_) + alignment_ - 1) & ~(alignment_ - 1));


_AFX afxError _AfxInitMmu(afxThread thr)
{
    afxError err = { 0 };
#ifdef RPMALLOC_ALLOC
    if (thr)
    {
        rpmalloc_thread_initialize();
    }
    else
    {
        rpmalloc_initialize(0);
    }
#endif
    return err;
}

_AFX afxError _AfxDeinitMmu(afxThread thr)
{
    afxError err = { 0 };
#ifdef RPMALLOC_ALLOC
    if (thr)
    {
        rpmalloc_thread_finalize();
    }
    else
    {
        rpmalloc_finalize();
    }
#endif
    return err;
}

_AFX void* AfxMallocCb(afxSize siz, afxHere const hint)
{
    void* p;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    p = _malloc_dbg(siz, _NORMAL_BLOCK, (const char*)hint[0], hint[1]);
#else
    p = malloc(siz);
#endif
#else
    p = rpmalloc(siz);
#endif
    return p;
}

_AFX void AfxFreeCb(void* block)
{
    void* p = block;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    _free_dbg(p, _NORMAL_BLOCK);
#else
    free(p);
#endif
#else
    rpfree(p);
#endif
}

_AFX void* AfxCallocCb(afxSize cnt, afxSize siz, afxHere const hint)
{
    void* p;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    p = _calloc_dbg(cnt, siz, _NORMAL_BLOCK, (const char*)hint[0], hint[1]);
#else
    p = calloc(cnt, siz);
#endif
#else
    p = rpcalloc(cnt, siz);
#endif
    return p;
}

_AFX void* AfxReallocCb(void* block, afxSize siz, afxHere const hint)
{
    void* p = block;

    void* p2;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    p2 = _realloc_dbg(p, siz, _NORMAL_BLOCK, (const char*)hint[0], hint[1]);
#else
    p2 = realloc(p, siz);
#endif
#else
    p2 = rprealloc(p, siz);
#endif
    return p2;
}

_AFX void AfxFreeAlignedCb(void* block)
{
    void* p = block;

#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    _aligned_free_dbg(p);
#else
    _aligned_free(p);
#endif    
#else
    return rpfree(p);
#endif
}

_AFX void* AfxMallocAlignedCb(afxSize siz, afxSize align, afxHere const hint)
{
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    void* p;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    p = _aligned_malloc_dbg(siz, align, (const char*)hint[0], hint[1]);
#else
    p = _aligned_malloc(siz, align);
#endif
#else
    p = rpaligned_alloc(align, siz);
#endif
    return p;
}

_AFX void* AfxReallocAlignedCb(void* block, afxSize siz, afxSize align, afxHere const hint)
{
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    void* p = block;
    void* p2;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    p2 = _aligned_realloc_dbg(p, siz, align, (const char*)hint[0], hint[1]);
#else
    p2 = _aligned_realloc(p, siz, align);
#endif
#else
    p2 = rpaligned_realloc(p, align, siz, 0, 0);
#endif
    return p2;
}

_AFX void* AfxRecallocAlignedCb(void* block, afxSize cnt, afxSize siz, afxSize align, afxHere const hint)
{
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    void* p = block;
    void* p2;
#ifndef RPMALLOC_ALLOC
#ifdef _DEBUG
    p2 = _aligned_recalloc_dbg(p, cnt, siz, align, (const char*)hint[0], hint[1]);
#else
    p2 = _aligned_recalloc(p, cnt, siz, align);
#endif
#else
    p2 = rpaligned_realloc(p, align, siz * cnt, 0, 0);
#endif
    return p2;
}


_AFX void* AfxMalloc(afxSize siz)
{
    return AfxMallocCb(siz, AfxHere());
}

_AFX void AfxFree(void** pp)
{
    afxError err = { 0 };
    AFX_ASSERT(pp);
    void* p = *pp;

    if (p)
    {
        AfxFreeCb(p);
        *pp = NIL;
    }
}

_AFX void* AfxCalloc(afxSize cnt, afxSize siz)
{
    return AfxCallocCb(cnt, siz, AfxHere());
}

_AFX void* AfxRealloc(void* block, afxSize siz)
{
    return AfxReallocCb(block, siz, AfxHere());
}

_AFX void AfxFreeAligned(void** pp)
{
    afxError err = { 0 };
    AFX_ASSERT(pp);
    void* p = *pp;

    if (p)
    {
        AfxFreeAlignedCb(p);
        *pp = NIL;
    }
}

_AFX void* AfxMallocAligned(afxSize siz, afxSize align)
{
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    return AfxMallocAlignedCb(siz, align, AfxHere());
}

_AFX void* AfxReallocAligned(void* block, afxSize siz, afxSize align)
{
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    return AfxReallocAlignedCb(block, siz, align, AfxHere());
}

_AFX void* AfxRecallocAligned(void* block, afxSize cnt, afxSize siz, afxSize align)
{
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    return AfxRecallocAlignedCb(block, cnt, siz, align, AfxHere());
}

_AFX void _AfxMemAllocNotStd(afxMmu mmu, afxSize siz, afxHere const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);
    //AFX_ASSERT(p);
    AFX_ASSERT(siz);
    AFX_ASSERT(hint);
}

_AFX void _AfxMemDeallocNotStd(afxMmu mmu, afxSize siz, afxHere const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);
    //AFX_ASSERT(p);
    //AFX_ASSERT(siz);
    AFX_ASSERT(hint);
}

_AFX void _AfxMemDeallocStd(afxMmu mmu, void *p)
{
    //AfxEntry("p=%p", p);
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

    AFX_ASSERT(p);

    AfxFreeAligned(p);

    if (mmu->deallocNotCb)
        mmu->deallocNotCb(mmu, 0, AfxHere());
}

_AFX void* _AfxMemReallocStd(afxMmu mmu, void* p, afxSize siz, afxSize align, afxHere const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);
    //AFX_ASSERT(p);
    AFX_ASSERT(siz);
    void *neo = NIL;

    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    if (!(neo = AfxReallocAligned(p, siz, align)))
        AfxThrowError();

    return neo;
}

_AFX void* _AfxMemAllocStd(afxMmu mmu, afxSize siz, afxSize align, afxHere const hint)
{
    //AfxEntry("ctx=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);
    AFX_ASSERT(siz);
    AFX_ASSERT(hint);
    void *p = NIL;

    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align); // every allocation in Qwadro is 16-byte aligned.

    if (!(p = AfxMallocAligned(siz, align)))
        AfxThrowError();

    return p;
}

// ALL API

_AFX afxSize AfxMemoryGetDefaultAlignment(afxMmu mmu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);
    return mmu->defAlign;
}

_AFX afxError AfxMemoryEnableDebugging(afxMmu mmu, afxUnit level)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);
    mmu->dbgLevel = level;
    return err;
}

_AFX afxError AfxMapMemory(afxMemory mem, afxSize offset, afxUnit range, afxFlags flags, void** var)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mem);

    return err;
}

_AFX afxError AfxUnmapMemory(afxMemory mem, afxSize offset, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mem);

    return err;
}

_AFX afxError _AfxMmuCtor(afxMmu mmu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

    afxAllocationStrategy const *as = args[0] ? ((afxAllocationStrategy const*)args[0]) + invokeNo : NIL;
    afxSize const *hint = ((afxSize const*)args[1]) + invokeNo;
    //AFX_ASSERT(as);

    //AFX_ASSERT(!as->align || as->align >= AFX_SIMD_ALIGNMENT);
    mmu->defAlign = AFX_SIMD_ALIGNMENT;

    mmu->dbgLevel = 3;

    mmu->fname = (char const *const)hint[0];
    mmu->fline = (int)hint[1];
    mmu->func = (char const *const)hint[2];

    if (as)
    {
        if (as->align)
            mmu->defAlign = as->align;
    }

    AfxSetUpFutex(&mmu->memSlock);
    AfxMakeChain(&mmu->memChain, mmu);
    // Choose which memocation mechanism to be used. Actumemy there's just two: standard (arbitrary) and arena.

    //if (mmu->cap != (afxSize)0) _AfxArenaAllCtor(ctx, paradigm);
    //else
    {
        mmu->allocCb = _AfxMemAllocStd;
        mmu->reallocCb = _AfxMemReallocStd;
        mmu->deallocCb = _AfxMemDeallocStd;
        mmu->allocNotCb = _AfxMemAllocNotStd;
        mmu->deallocNotCb = _AfxMemDeallocNotStd;

        //_AfxMemCtorArena(ctx);
    }
    return err;
}

_AFX afxError _AfxMmuDtor(afxMmu mmu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

    mmu->dbgLevel = 3;

    //afxResult rslt = AfxMemoryExhaust(ctx);

    //if (rslt)
//        AfxReportWarn("Recovered. %i bytes orphaned @ (ctx)%p", rslt, ctx);

    AfxCleanUpFutex(&mmu->memSlock);

    return err;
}

_AFX afxClassConfig const _AFX_MMU_CLASS_CONFIG =
{
    .fcc = afxFcc_MMU,
    .name = "Memory Management Unit",
    .fixedSiz = sizeof(AFX_OBJECT(afxMmu)),
    .ctor = (void*)_AfxMmuCtor,
    .dtor = (void*)_AfxMmuDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireMemory(afxMemoryFlags flags, afxSize siz, afxUri const* uri, afxHere const hint, afxMemory* memory)
{
    afxError err = { 0 };

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass* cls = (afxClass*)_AfxSysGetMmuClass(sys);
    AFX_ASSERT_CLASS(cls, afxFcc_MMU);
    afxMemory mem = NIL;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&mem, (void const*[]) { NIL, &siz, hint }))
        AfxThrowError();

    return err;
}

_AFX afxError AfxOpenMemory(afxMemoryFlags flags, afxUri const* uri, afxHere const hint, afxMemory* memory)
{
    afxError err = { 0 };

    return err;
}

















// MEMORY API

_AFX void AfxStream2(afxUnit cnt, void const* src, afxSize srcStride, void* dst, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    AFX_ASSERT(src != dst);
    AFX_ASSERT(cnt);
    //AFX_ASSERT(srcStride);
    AFX_ASSERT(dstStride);

    if ((srcStride == dstStride) && dstStride)
    {
        AfxCopy(dst, src, dstStride * cnt);
    }
    else
    {
        afxByte* dst2 = dst;
        afxByte const *src2 = src;
        afxUnit unitSiz = AFX_MAX(1, AFX_MIN(dstStride, srcStride)); // minor non-zero
        AFX_ASSERT(unitSiz);
        //afxUnit cnt = len / dstStride;
        //afxUnit cnt2 = len % dstStride;
        afxUnit majStride = AFX_MAX(1, AFX_MAX(dstStride, srcStride));
        //cnt = len / majStride;
        //AFX_ASSERT(!(len % majStride));
        //AFX_ASSERT(!cnt2);

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxCopy(&(dst2[i * dstStride]), &(src2[i * srcStride]), unitSiz);
        }
    }
#if 0
    afxByte* dst2 = dst;
    afxByte const* src2 = src;
    afxSize unitSiz = AFX_MIN_OR(dstStride, srcStride);

    if ((dstStride == srcStride) && dstStride)
        AfxCopy2(len, unitSiz, src2, dst2);
    else for (afxUnit i = 0; i < len; i++)
        AfxCopy2(1, unitSiz, &src2[i * srcStride], &dst2[i * dstStride]);
#endif
}

_AFX void AfxStream3(afxUnit cnt, void const* src, afxUnit srcOffset, afxSize srcStride, void* dst, afxUnit dstOffset, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    AFX_ASSERT(src != dst);
    AFX_ASSERT(cnt);
    //AFX_ASSERT(srcStride);
    AFX_ASSERT(dstStride);

    if ((srcStride == dstStride) && dstStride)
    {
        if ((srcOffset == dstOffset) && dstOffset)
            AfxStream2(cnt, src, srcStride, dst, dstStride);
        else
            AfxCopy(dst, src, dstStride * cnt);
    }
    else
    {
        afxByte* dst2 = dst;
        afxByte const *src2 = src;
        afxUnit unitSiz = AFX_MAX(1, AFX_MIN(dstStride, srcStride)); // minor non-zero
        AFX_ASSERT(unitSiz);
        //afxUnit cnt = len / dstStride;
        //afxUnit cnt2 = len % dstStride;
        afxUnit majStride = AFX_MAX(1, AFX_MAX(dstStride, srcStride));
        //cnt = len / unitSiz;
        //AFX_ASSERT(!(len % majStride));
        //AFX_ASSERT(!cnt2);

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxCopy(&(dst2[(i * dstStride) + dstOffset]), &(src2[(i * srcStride) + srcOffset]), unitSiz);
        }
    }
}

_AFX afxError AfxDeallocate(void** pp, afxHere const hint)
{
    afxError err = { 0 };
    void* p = *pp;

    if (p)
    {
        AFX_ASSERT(p);
        // Every and any allocation in Qwadro is 16-byte aligned.
        AFX_ASSERT_ALIGNMENT(p, AFX_SIMD_ALIGNMENT);

        afxSize freedSiz = 0;

        afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;
#ifdef _AFX_REPORT_DEALLOC
        AfxReportf(0, AfxHere(), "dealloc(%p)   %s:%i!%s", p, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
#endif
        if (mmu)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

            mmu->deallocCb(mmu, p);
        }
        else
        {
            AfxFreeAlignedCb(p);
        }
        *pp = NIL;
    }
    return err;
}

_AFX afxError AfxReallocate(afxSize siz, afxUnit align, afxHere const hint, void** pp)
{
    afxError err = { 0 };
    AFX_ASSERT(siz);
    AFX_ASSERT(hint);
    void *out = NIL;

    // Every and any allocation in Qwadro is 16-byte aligned.
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align);

    if (gMemoryBudget < gTotalAllocatedInSystem + AFX_ALIGN_SIZE(siz, align))
    {
        AfxThrowError();
    }

    afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;
    void* p = *pp;

    if (p)
    {
        // Every and any allocation in Qwadro is 16-byte aligned.
        AFX_ASSERT_ALIGNMENT(p, AFX_SIMD_ALIGNMENT);

        if (!mmu)
        {
            if (!(out = AfxReallocAlignedCb(p, siz, align, hint)))
                AfxThrowError();
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

            if (!(out = mmu->reallocCb(mmu, p, siz, align, hint)))
                AfxThrowError();
        }
    }
    else
    {
        if (mmu)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

            if (!(out = mmu->reallocCb(mmu, p, siz, align, hint)))
            {
                AfxThrowError();
            }
        }
        else
        {
            if (!(out = AfxReallocAlignedCb(NIL, siz, align, hint)))
            {
                AfxThrowError();
            }
        }
    }

#ifdef _AFX_REPORT_REALLOC
    AfxReportf(0, AfxHere(), "realloc(%u, %u, %p) => %p   %s:%i!%s", siz, align, p, out, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
#endif

    // Every and any allocation in Qwadro is 16-byte aligned.
    AFX_ASSERT_ALIGNMENT(out, AFX_SIMD_ALIGNMENT);
    *pp = out;
    return err;
}

_AFX afxError AfxCoallocate(afxSize cnt, afxSize siz, afxUnit align, afxHere const hint, void** pp)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(siz);
    AFX_ASSERT(hint);
    void *p = NIL;

    // Every and any allocation in Qwadro is 16-byte aligned.
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align);

    if (gMemoryBudget < gTotalAllocatedInSystem + AFX_ALIGN_SIZE(siz, align))
    {
        AfxThrowError();
    }

    afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

    if (mmu)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

        if (!(p = mmu->allocCb(mmu, cnt * siz, align, hint)))
        {
            AfxThrowError();
        }
    }
    else
    {
        if (!(p = AfxRecallocAlignedCb(NIL, cnt, siz, align, hint)))
        {
            AfxThrowError();
        }
    }

#ifdef _AFX_REPORT_COALLOC
    AfxReportf(0, AfxHere(), "coalloc(%u, %u, %u) => %p   %s:%i!%s", cnt, siz, align, p, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
#endif
    // Every and any allocation in Qwadro is 16-byte aligned.
    AFX_ASSERT_ALIGNMENT(p, AFX_SIMD_ALIGNMENT);
    *pp = p;
    return err;
}

_AFX afxError AfxAllocate(afxSize siz, afxUnit align, afxHere const hint, void** pp)
{
    afxError err = { 0 };
    AFX_ASSERT(hint);
    AFX_ASSERT(siz);
    void *p = NIL;

    // Every and any allocation in Qwadro is 16-byte aligned.
    align = AFX_MAX(AFX_SIMD_ALIGNMENT, align);

    if (siz)
    {
        if (gMemoryBudget < gTotalAllocatedInSystem + AFX_ALIGN_SIZE(siz, align))
        {
            AfxThrowError();
        }

        afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

        if (mmu)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

            if (!(p = mmu->allocCb(mmu, siz, align, hint)))
            {
                AfxThrowError();
            }
        }
        else
        {
            if (!(p = AfxMallocAlignedCb(siz, align, hint)))
            {
                AfxThrowError();
            }
        }
    }
#ifdef _AFX_REPORT_ALLOC
    AfxReportf(0, AfxHere(), "alloc(%u, %u) => %p   %s:%i!%s", siz, align, p, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
#endif
    // Every and any allocation in Qwadro is 16-byte aligned.
    AFX_ASSERT_ALIGNMENT(p, AFX_SIMD_ALIGNMENT);
    *pp = p;
    return err;
}




