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

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>


#define _AFX_CORE_C
#define _AFX_MMU_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxSystem.h"

typedef struct _afxArbitraryChunk
{
#ifndef _AFX_DONT_DEBUG_MEM_LEAK
    afxNat16            line;
    afxChar const       *file;
#endif
    _AFX_DBG_FCC;
    afxSimd(uintptr_t)  data[];
} _afxArbitraryChunk;

_AFX void* AfxMemset(void* dst, afxInt val, afxSize siz)
{
    return memset(dst, val, siz);
}

_AFX void* AfxMemcpy(void* dst, void const* src, afxSize siz)
{
    return memcpy(dst, src, siz);
}

_AFX afxInt AfxMemcmp(void const* buf1, void const* buf2, afxSize siz)
{
    return memcmp(buf1, buf2, siz);
}

_AFX void const* AfxMemchr(void const* buf, afxInt val, afxNat cap)
{
    return memchr(buf, val, cap);
}

_AFX void* AfxMalloc(afxSize siz)
{
    return malloc(siz);
}

_AFX void AfxFree(void* block)
{
    free(block);
}

_AFX void* AfxCalloc(afxSize cnt, afxSize siz)
{
    return calloc(cnt, siz);
}

_AFX void* AfxRealloc(void* block, afxSize siz)
{
    return realloc(block, siz);
}

_AFX void AfxFreeAligned(void* block)
{
    _aligned_free(block);
}

_AFX void* AfxMallocAligned(afxSize siz, afxSize align)
{
    return _aligned_malloc(siz, align);
}

_AFX void* AfxReallocAligned(void* block, afxSize siz, afxSize align)
{
    return _aligned_realloc(block, siz, align);
}

_AFX void* AfxRecallocAligned(void* block, afxSize cnt, afxSize siz, afxSize align)
{
    return _aligned_recalloc(block, cnt, siz, align);
}

_AFX afxError _AfxDeallocCallback2(afxMemory mem, afxHint const hint)
{
    afxError err = AFX_ERR_NONE;
    AfxPopLinkage(&mem->mmu);
    AfxAssert(mem->start);

    if (mem->flags & afxMemFlag_RESIZABLE)
    {
        AfxFreeAligned(mem->start);
        AfxFree(mem);
    }
    else
    {
        AfxFreeAligned(mem);
    }
    return err;
}

_AFX afxError _AfxResizeCallback2(afxMemory mem, afxSize cnt, afxSize siz, afxHint const hint)
{
    afxError err = AFX_ERR_NONE;

    if (!(mem->flags & afxMemFlag_RESIZABLE)) AfxThrowError();
    else
    {
        afxNat zeroSiz = cnt * siz > mem->siz ? cnt * siz - mem->siz : 0;
        afxNat zeroOff = cnt * siz > mem->siz ? mem->siz : 0;

        void* out;

        if (!(out = AfxReallocAligned(mem->start, cnt * siz, mem->align ? mem->align : AFX_SIMD_ALIGN))) AfxThrowError();
        else
        {
            mem->siz = siz;
            mem->cnt = cnt;
            mem->start = out;
        }

        if (!err && (mem->flags & afxMemFlag_ZEROED) && zeroSiz)
            AfxZeroMemory(mem, zeroOff, zeroSiz);
    }
    return err;
}

_AFX afxMemory _AfxAllocCallback2(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxMemFlags flags, afxHint const hint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    afxMemory mem = NIL;

    if (flags & afxMemFlag_RESIZABLE)
    {
        if (!(mem = AfxMalloc(sizeof(*(afxMemory)0)))) AfxThrowError();
        else
        {
            void* p = AfxMallocAligned(cnt * siz, align ? align : AFX_SIMD_ALIGN);

            if (!(p)) AfxThrowError();
            else
            {
                mem->start = p;
                mem->align = align ? align : AFX_SIMD_ALIGN;
                mem->siz = siz;
                mem->cnt = cnt;
                mem->flags = AfxGetBitOffset(31) | flags;
                AfxPushLinkage(&mem->mmu, &mmu->memChain);
            }

            if (err)
            {
                AfxFree(mem);
                mem = NIL;
            }
        }
    }
    else
    {
        void* p = AfxMallocAligned(cnt * siz + sizeof(*(afxMemory)0), align ? align : AFX_SIMD_ALIGN);

        if (!(p)) AfxThrowError();
        else
        {
            mem = p;
            mem->start = p;
            mem->align = align ? align : AFX_SIMD_ALIGN;
            mem->siz = siz;
            mem->cnt = cnt;
            mem->flags = AfxGetBitOffset(31) | flags;
            AfxPushLinkage(&mem->mmu, &mmu->memChain);
        }
    }

    if (!err && (flags & afxMemFlag_ZEROED))
        AfxZeroMemory(mem, 0, mem->siz);

    return mem;
}

_AFX void _AfxMemAllocNotStd(afxMmu mmu, afxSize cnt, afxSize siz, afxHint const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    //AfxAssert(p);
    AfxAssert(siz);
    AfxAssert(cnt);
    AfxAssert(hint);
}

_AFX void _AfxMemDeallocNotStd(afxMmu mmu, afxSize cnt, afxSize siz, afxHint const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    //AfxAssert(p);
    //AfxAssert(siz);
    AfxAssert(hint);
}

_AFX void _AfxMemDeallocStd(afxMmu mmu, void *p)
{
    //AfxEntry("p=%p", p);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssert(p);

    AfxFreeAligned(p);

    if (mmu->deallocNotCb)
        mmu->deallocNotCb(mmu, 0, 0, AfxHint());
}

_AFX void* _AfxMemReallocStd(afxMmu mmu, void* p, afxSize cnt, afxSize siz, afxSize align, afxHint const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    //AfxAssert(p);
    AfxAssert(siz);
    void *neo = NIL;

    // TODO Allow rememocate to an or another memocator.

    if (!(neo = AfxReallocAligned(p, cnt * siz, align ? align : AFX_SIMD_ALIGN)))
        AfxThrowError();

    return neo;
}

_AFX void* _AfxMemAllocStd(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxHint const hint)
{
    //AfxEntry("ctx=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, siz, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    AfxAssert(siz);
    AfxAssert(hint);
    void *p = NIL;

    if (!(p = AfxMallocAligned(cnt * siz, align ? align : AFX_SIMD_ALIGN)))
        AfxThrowError();

    return p;
}

// ALL API

_AFX afxSize AfxMemoryGetDefaultAlignment(afxMmu mmu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu->defAlign;
}

_AFX afxError AfxMemoryEnableDebugging(afxMmu mmu, afxNat level)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    mmu->dbgLevel = level;
    return err;
}

_AFX afxError _AfxMmuCtor(afxMmu mmu, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxAllocationStrategy const *as = cookie->udd[0] ? ((afxAllocationStrategy const*)cookie->udd[0]) + cookie->no : NIL;
    afxSize const *hint = ((afxSize const*)cookie->udd[1]) + cookie->no;
    //AfxAssert(as);

    //AfxAssert(!as->align || as->align >= AFX_SIMD_ALIGN);
    mmu->defAlign = AFX_SIMD_ALIGN;

    mmu->dbgLevel = 3;

    mmu->fname = (char const *const)hint[0];
    mmu->fline = (int)hint[1];
    mmu->func = (char const *const)hint[2];

    if (as)
    {
        if (as->align)
            mmu->defAlign = as->align;
    }

    AfxTakeSlock(&mmu->memSlock);
    AfxSetUpChain(&mmu->memChain, mmu);
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
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    mmu->dbgLevel = 3;

    //afxResult rslt = AfxMemoryExhaust(ctx);

    //if (rslt)
//        AfxAdvertise("Recovered. %i bytes orphaned @ (ctx)%p", rslt, ctx);

    afxMemory mem;
    AfxChainForEveryLinkage(&mmu->memChain, struct afxMemory_T, mmu, mem)
    {
        AfxDeallocateMemory(mem, AfxHint());
    }

    AfxReleaseSlock(&mmu->memSlock);

    return err;
}

_AFX afxClassConfig const _AfxMmuClsConfig =
{
    .fcc = afxFcc_MMU,
    .name = "Memory Management Unit",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxMmu)),
    .mmu = NIL,
    .ctor = (void*)_AfxMmuCtor,
    .dtor = (void*)_AfxMmuDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireMmus(afxNat cnt, afxHint const hint, afxAllocationStrategy const strategy[], afxMmu mmus[])
{
    afxError err = AFX_ERR_NONE;

    afxManager* cls = AfxGetMmuClass();
    AfxAssertClass(cls, afxFcc_MMU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)mmus, (void const*[]) { strategy, hint }))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxInvokeMmus(afxNat first, afxNat cnt, afxBool(*f)(afxMmu, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetMmuClass();
    AfxAssertClass(cls, afxFcc_MMU);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateMmus(afxNat first, afxNat cnt, afxMmu mmus[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(mmus);
    afxManager* cls = AfxGetMmuClass();
    AfxAssertClass(cls, afxFcc_MMU);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)mmus);
}

_AFX afxNat AfxCountMmus(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetMmuClass();
    AfxAssertClass(cls, afxFcc_MMU);
    return AfxCountObjects(cls);
}

// MEMORY API

_AFX void AfxStream2(afxNat cnt, void const* src, afxSize srcStride, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(dst);
    AfxAssert(src != dst);
    AfxAssert(cnt);
    //AfxAssert(srcStride);
    AfxAssert(dstStride);

    afxByte* dst2 = dst;
    afxByte const* src2 = src;
    afxSize unitSiz = AfxMinorNonZero(dstStride, srcStride);

    if ((dstStride == srcStride) && dstStride)
        AfxCopy(cnt, unitSiz, src2, dst2);
    else for (afxNat i = 0; i < cnt; i++)
        AfxCopy(1, unitSiz, &src2[i * srcStride], &dst2[i * dstStride]);
}

_AFX void AfxCopy(afxSize cnt, afxSize siz, void const *src, void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(dst);
    AfxAssert(src != dst);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxCatchError(err);
#if !0
    if (cnt * siz)
        AfxMemcpy(dst, src, cnt * siz);
#else
    register afxChar* dst2 = dst;
    register afxChar const* src2 = src;
    register afxSize t = (afxOffset)src2;

    if ((afxSize)dst2 < (afxSize)src2)
    {
        if ((t | (afxOffset)dst2) & (sizeof(afxSize) - 1))
        {
            t = ((t ^ (afxOffset)dst2) & (sizeof(afxSize) - 1) || range < sizeof(afxSize)) ? range : (sizeof(afxSize) - (t & (sizeof(afxSize) - 1)));
            range -= t;
            do { (*(dst2)++ = *(src2)++); } while (--(t));
        }

        if ((t = range / sizeof(afxSize)))
            do { *(afxSize*)dst2 = *(afxSize*)src2; src2 += sizeof(afxSize); dst2 += sizeof(afxSize); } while (--(t));

        if ((t = range & (sizeof(afxSize) - 1)))
            do { *(dst2)++ = *(src2)++; } while (--(t));
    }
    else
    {
        src2 += range;
        dst2 += range;

        if ((t | (afxOffset)dst2) & (sizeof(afxSize) - 1))
        {
            if ((t ^ (afxOffset)dst2) & (sizeof(afxSize) - 1) || range <= sizeof(afxSize)) t = range;
            else t &= (sizeof(afxSize) - 1);

            range -= t;
            do { *(--(dst2)) = *(--(src2)); } while (--(t));
        }

        if ((t = range / sizeof(afxSize)))
            do { src2 -= sizeof(afxSize); dst2 -= sizeof(afxSize); *(afxSize*)dst2 = *(afxSize*)src2; } while (--(t));

        if ((t = range & (sizeof(afxSize) - 1)))
            do { *(--(dst2)) = *(--(src2)); } while (--(t));
    }
#endif
}

_AFX void AfxFill(afxSize cnt, afxSize siz, void const* value, void *p)
{
    // This function fills the first 'cnt' sets of 'unitSiz' bytes of memory area pointed by 'p' with the constant 'value'.

    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(cnt);
    //AfxAssert(value);
    AfxAssert(siz);

    if (value)
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            afxByte* ptr = (afxByte*)p;
            AfxCopy(1, siz, value, &ptr[i * siz]);
        }
    }
    else
    {
        if (cnt)
            AfxMemset(p, 0, cnt * siz);
    }
}

_AFX void AfxZero(afxSize cnt, afxSize siz, void *p)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxMemset(p, 0, cnt * siz);
}

_AFX void AfxDeallocate(void *p)
{
    afxError err = AFX_ERR_NONE;

    if (p)
    {
        AfxAssert(p);
        afxSize freedSiz = 0;

        afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

        if (mmu)
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            mmu->deallocCb(mmu, p);
        }
        else
        {
            AfxFreeAligned(p);
        }
    }
}

_AFX void* AfxReallocate(void *p, afxSize siz, afxSize cnt, afxNat align, afxHint const hint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(siz);
    AfxAssert(cnt);
    AfxAssert(hint);
    //AfxEntry("p=%p,siz=%u,hint=\"%s:%i!%s\"", p, siz, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    void *out = NIL;

    afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

    if (p)
    {
        if (!mmu)
        {

            if (!(out = AfxReallocAligned(p, cnt * siz, align ? align : AFX_SIMD_ALIGN)))
                AfxThrowError();

            return out;
        }
        else
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            if (!(out = mmu->reallocCb(mmu, p, cnt, siz, align, hint)))
                AfxThrowError();

            return out;
        }
    }
    else
    {
        if (mmu)
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            if (!(out = mmu->reallocCb(mmu, p, cnt, siz, align, hint)))
                AfxThrowError();

            return out;
        }
        else
        {
            if (!(out = AfxReallocAligned(NIL, cnt * siz, align ? align : AFX_SIMD_ALIGN)))
                AfxThrowError();

            return out;
        }
    }
    return out;
}

_AFX void* AfxCoallocate(afxSize cnt, afxSize siz, afxNat align, afxHint const hint)
{
    //AfxEntry("ctx=%p,cnt=%u,siz=%u,hint=\"%s:%i!%s\"", ctx, cnt, siz, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(hint);
    void *p = NIL;

    afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

    if (mmu)
    {
        AfxAssertObjects(1, &mmu, afxFcc_MMU);

        if (!(p = mmu->allocCb(mmu, cnt, siz, align, hint)))
            AfxThrowError();
    }
    else
    {
        if (!(p = AfxRecallocAligned(NIL, cnt, siz, align ? align : AFX_SIMD_ALIGN)))
            AfxThrowError();
    }
    return p;
}

_AFX void* AfxAllocate(afxSize cnt, afxSize siz, afxNat align, afxHint const hint)
{
    //AfxEntry("ctx=%p,siz=%u,cnt=%u,hint=\"%s:%i!%s\"", ctx, siz, cnt, AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssert(hint);
    AfxAssert(siz);
    AfxAssert(cnt);
    void *p = NIL;

    if (cnt * siz)
    {
        afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

        if (mmu)
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            if (!(p = mmu->allocCb(mmu, cnt, siz, align, hint)))
                AfxThrowError();

            return p;
        }
        else
        {
            if (!(p = AfxMallocAligned(cnt * siz, align ? align : AFX_SIMD_ALIGN)))
                AfxThrowError();

            return p;
        }
    }
    return p;
}













_AFX afxError AfxMapMemory(afxMemory mem, afxSize off, afxSize siz, void **ptr) // Map a memory object into application address space
{
    afxError err = NIL;
    AfxAssert(mem);
    *ptr = &mem->start[off];
    return err;
}

_AFX void AfxUnmapMemory(afxMemory mem) // Unmap a previously mapped memory object
{
    afxError err = NIL;
    AfxAssert(mem);
}

_AFX afxMemory AfxAllocateMemory(afxMmu mmu, afxSize siz, afxSize cnt, afxNat align, afxMemFlags flags, afxHint const hint) // Allocate memory
{
    afxError err = NIL;
    afxMemory mem = NIL;

    if (mmu)
    {
        AfxAssertObjects(1, &mmu, afxFcc_MMU);

        if (!(mem = mmu->allocCb2(mmu, cnt, siz, align, flags, hint)))
            AfxThrowError();
    }
    else
    {
        if (flags & afxMemFlag_RESIZABLE)
        {
            if (!(mem = AfxMalloc(sizeof(*(afxMemory)0)))) AfxThrowError();
            else
            {
                void* p = AfxMallocAligned(cnt * siz, align ? align : AFX_SIMD_ALIGN);

                if (!(p)) AfxThrowError();
                else
                {
                    mem->start = p;
                    mem->align = align ? align : AFX_SIMD_ALIGN;
                    mem->siz = siz;
                    mem->cnt = cnt;
                    mem->flags = AfxGetBitOffset(31) | flags;
                    AfxPushLinkage(&mem->mmu, &mmu->memChain);
                }

                if (err)
                {
                    AfxFreeAligned(mem);
                    mem = NIL;
                }
            }
        }
        else
        {
            void* p = AfxMallocAligned(cnt * siz + sizeof(*(afxMemory)0), align ? align : AFX_SIMD_ALIGN);

            if (!(p)) AfxThrowError();
            else
            {
                mem = p;
                mem->start = p;
                mem->align = align ? align : AFX_SIMD_ALIGN;
                mem->siz = siz;
                mem->cnt = cnt;
                mem->flags = AfxGetBitOffset(31) | flags;
                AfxPushLinkage(&mem->mmu, &mmu->memChain);
            }
        }

        if (!err && (flags & afxMemFlag_ZEROED))
            AfxZeroMemory(mem, 0, mem->siz);
    }

    if (err) AfxAssert(!mem);
    else
    {
        AfxAssert(AfxGetLinker(&mem->mmu) == mmu);
        AfxAssert(mem->siz >= siz);
        AfxAssert(mem->cnt == cnt);
        AfxAssert(mem->align >= align);
        AfxAssert(mem->start);
    }
    return mem;
}

_AFX afxError AfxResizeMemory(afxMemory mem, afxSize siz, afxSize cnt, afxHint const hint)
{
    afxError err = NIL;
    AfxAssert(mem);
    AfxAssert(siz);
    AfxAssert(cnt);
    afxMmu mmu = AfxGetObjectProvider(mem);
    afxNat align = mem->align;

    if (!mmu)
    {
        if (!(mem->flags & afxMemFlag_RESIZABLE)) AfxThrowError();
        else
        {
            afxNat zeroSiz = cnt * siz > mem->siz ? cnt * siz - mem->siz : 0;
            afxNat zeroOff = cnt * siz > mem->siz ? mem->siz : 0;

            void* out;

            if (!(out = AfxReallocAligned(mem->start, cnt * siz, align ? align : AFX_SIMD_ALIGN))) AfxThrowError();
            else
            {
                mem->cnt = cnt;
                mem->siz = siz;
                mem->start = out;
            }

            if (!err && (mem->flags & afxMemFlag_ZEROED) && zeroSiz)
                AfxZeroMemory(mem, zeroOff, zeroSiz);
        }
    }
    else
    {
        AfxAssertObjects(1, &mmu, afxFcc_MMU);

        if (mmu->reallocCb2(mem, cnt, siz, hint))
            AfxThrowError();
    }

    if (err)
    {
        AfxAssert(mem->siz);
        AfxAssert(mem->align == align);
        AfxAssert(mem->start);
    }
    else
    {
        AfxAssert(mem->siz >= siz);
        AfxAssert(mem->cnt == cnt);
        AfxAssert(mem->align >= align);
        AfxAssert(mem->start);
    }
    return err;
}

_AFX void AfxDeallocateMemory(afxMemory mem, afxHint const hint) // Free memory
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mem, afxFcc_MEM);
    afxMmu mmu = AfxGetLinker(&mem->mmu);

    if (mmu)
    {
        AfxAssertObjects(1, &mmu, afxFcc_MMU);
        
        if (mmu->deallocCb2(mem, hint))
            AfxThrowError();
    }
    else
    {
        AfxPopLinkage(&mem->mmu);
        AfxAssert(mem->start);

        if (mem->flags & afxMemFlag_RESIZABLE)
        {
            AfxFreeAligned(mem->start);
            AfxFree(mem);
        }
        else
        {
            AfxFreeAligned(mem);
        }
    }
}

_AFX void AfxUpdateMemory(afxMemory mem, afxSize offset, afxSize range, afxNat unitSiz, void const* src)
{
    afxError err = NIL;
    AfxAssert(mem);
    AfxAssertRange(mem->siz, offset, range);
    AfxAssert(!(range % unitSiz));
    AfxAssert(src);
    AfxAssert(mem->start);
    AfxCopy(1, AfxAbs(range - mem->siz), src, mem->start);
}

_AFX void AfxDumpMemory(afxMemory mem, afxSize offset, afxSize range, afxNat unitSiz, void* dst)
{
    afxError err = NIL;
    AfxAssert(mem);
    AfxAssertRange(mem->siz, offset, range);
    AfxAssert(!(range % unitSiz));
    AfxAssert(dst);
    AfxAssert(mem->start);
    AfxCopy(1, AfxAbs(range - mem->siz), mem->start, dst);
}

_AFX void AfxCopyMemory(afxMemory mem, afxSize base, afxMemory in, afxSize offset, afxSize range, afxNat unitSiz)
{
    afxError err = NIL;
    AfxAssert(mem);
    AfxAssertRange(mem->siz, base, range);
    AfxAssert(in);
    AfxAssertRange(in->siz, offset, range);
    AfxAssert(!(range % unitSiz));
    void *dst, *src;
    afxError errSiz;

    if ((errSiz = AfxMapMemory(mem, base, range, &dst))) AfxThrowError();
    else
    {
        if ((errSiz = AfxMapMemory((void*)in, offset, range, &src))) AfxThrowError();
        else
        {
            AfxCopy(1, AfxAbs(range - errSiz), src, dst);
            AfxUnmapMemory((void*)in);
        }
        AfxUnmapMemory(mem);
    }
}

_AFX void AfxFillMemory(afxMemory mem, afxSize offset, afxSize range, afxNat unitSiz, void const* value) // returns non-copied range in error occurence.
{
    afxError err = NIL;
    AfxAssert(mem);
    AfxAssertRange(mem->siz, offset, range);
    AfxAssert(!(range % unitSiz));
    void *dst, *src;
    afxError errSiz;

    if ((errSiz = AfxMapMemory(mem, 0, range, &dst))) AfxThrowError();
    else
    {
        AfxFill(AfxAbs((range / unitSiz) - errSiz), unitSiz, value, dst);
        AfxUnmapMemory(mem);
    }
}

_AFX void AfxZeroMemory(afxMemory mem, afxSize offset, afxSize range) // returns non-copied range in error occurence.
{
    afxError err = NIL;
    AfxAssert(mem);
    AfxAssertRange(mem->siz, offset, range);
    afxError errSiz;
    void *ptr;

    if ((errSiz = AfxMapMemory(mem, offset, range, &ptr))) AfxThrowError();
    else
    {
        AfxZero(1, mem->siz, ptr);
        AfxUnmapMemory(mem);
    }
}
