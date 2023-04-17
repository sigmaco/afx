/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_ALLOCATOR_H
#define AFX_ALLOCATOR_H

#include "../base/afxObject.h"
#include "../base/afxLinkedList.h"
#include "../async/afxCondition.h"
#include "afxMemory.h"

AFX_DEFINE_HANDLE(afxAllocator);

#define AFX_ALL_EXTRA_DATA 128

AFX_DEFINE_STRUCT(afxAllocationStrategy)
{
    afxSize                 size;    // size of an fixed-size allocation or size of afxByte for arbitrary allocations.
    afxSize                 align;  // useful when element requires alignment, such as 128-bit alignment for SIMD instructions. If zero, Qwadro will align to 16 bytes.
    afxSize                 cap;    // useful to restrict space to a certain capacity or, when elemSiz is equal to afxByte it will be the max totality of bytes for arbitrary allocations.
    afxSize                 stock;  // useful to preallocate space for fixed-size allocations.
    afxAllocationDuration   duration;   // the expected duration of allocations.
    afxBool                 resizable;  // flag to indicate if allocations may be required to be resized.
};

#if 0
AFX_DEFINE_STRUCT(afxAllocation)
{
    afxSize                 amount;    // count of an fixed-size allocation or size of afxByte sequence for arbitrary allocations.
    afxSize                 align;  // useful when element requires alignment, such as 128-bit alignment for SIMD instructions. If zero, Qwadro will align to 16 bytes.
    afxAllocationDuration   duration;   // the expected duration of allocations.
    afxBool                 resizable;  // flag to indicate if allocations may be required to be resized.
    afxHint const           *hint;
};

//AFX afxError        AfxAllocatorRequest(afxAllocator all, afxAllocation const *a, );
//AFX afxError        AfxAllocatorDispose(afxAllocator all, afxNat cnt, void *mem[]);
#endif

AFX_DEFINE_STRUCT(afxMemoryPage)
{
    afxFcc          fcc; // MPAG
    afxLinkage      all;
    afxSize         siz;
    afxSize         freeSiz;
    afxChain        slices;
};

AFX afxError        AfxAllocatorEnableDebugging(afxAllocator all, afxNat level);
AFX afxError        AfxAllocatorExhaust(afxAllocator all, afxBool disposeSpace, afxBool disposeStock); // free all allocations
AFX afxResult       AfxAllocatorEnumeratePages(afxAllocator all, afxBool reverse, afxNat base, afxNat cnt, afxMemoryPage *mpag[]);
AFX afxResult       AfxAllocatorEnumerateChunks(afxAllocator all, afxBool reverse, afxNat base, afxNat cnt, void *chunks[]);
AFX afxSize         AfxAllocatorGetAlignment(afxAllocator all);
AFX afxSize         AfxAllocatorGetCapacity(afxAllocator all);
AFX afxSize         AfxAllocatorGetPop(afxAllocator all);
AFX afxSize         AfxAllocatorGetStock(afxAllocator all); // useful to get restant capacity of a preallocated memory pool.
AFX afxError        AfxAllocatorOptimize(afxAllocator all); // try to defragment, reorder allocations and/or free unused reserved space.

AFX_OBJECT(afxAllocator)
{
    afxObject               obj;

    afxChain                pages; // slabs, pages, arenas, etc
    afxChain                unpagedChunks;

    afxSize                 elemSiz;        // for fixed-size allocators
    afxSize                 align;
    afxSize                 cap;        // when the max growth of memory pool is limited, or (afxSize)-1 for "infinity".
    afxSize                 heapSiz;        // when preallocate pages
    afxAllocationDuration   duration;   // the expected duration of allocations.
    afxBool                 resizable;  // flag to indicate if allocations may be required to be resized.

// debug
    afxNat                  dbgLevel;       // 0 - nothing, 1 - mechanism activity, 2 - block activity, 3 - portion (de)allocations, 4 - [implementation-dependent]
    afxChar const           *func;
    afxChar const           *fname;
    afxSize                 fline;

    void*                   (*alloc)(afxAllocator all, afxSize rawsiz, afxHint const hint);
    void*                   (*coalloc)(afxAllocator all, afxSize cnt, afxSize rawsiz, afxHint const hint);
    void*                   (*realloc)(afxAllocator all, void* mem, afxSize rawsiz, afxHint const hint);
    afxResult               (*dealloc)(afxAllocator all, void* mem);

    afxError                (*dismantle)(afxAllocator all);
    afxError                (*exhaust)(afxAllocator all, afxBool disposeSpace, afxBool disposeStock);
    afxError                (*optimize)(afxAllocator all);

    afxFcc                  extFourCc;
    afxByte                 extData[AFX_ALL_EXTRA_DATA];
};

#endif//AFX_ALLOCATOR_H