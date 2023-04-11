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

AFX_DEFINE_HANDLE(afxMemContext);

#define AFX_ALL_EXTRA_DATA 128

typedef enum afxAllocationDuration
{
    AFX_ALL_DUR_TEMPORARY = (1 << 0), // to be used just inside "this" function.
    AFX_ALL_DUR_TRANSIENT = (1 << 1), // to be used across functions, as example signaling objects about an event occurance.
    AFX_ALL_DUR_PERMANENT = (1 << 2), // to be used across the entire system and or subsystem, 
}
afxAllocationDuration;

AFX_DEFINE_STRUCT(afxAllocationStrategy)
{
    afxSize                 size;    // size of an fixed-size allocation or size of afxByte for arbitrary allocations.
    afxSize                 align;  // useful when element requires alignment, such as 128-bit alignment for SIMD instructions. If zero, Qwadro will align to 16 bytes.
    afxSize                 cap;    // useful to restrict space to a certain capacity or, when elemSiz is equal to afxByte it will be the max totality of bytes for arbitrary allocations.
    afxSize                 stock;  // useful to preallocate space for fixed-size allocations.
    afxAllocationDuration   duration;   // the expected duration of allocations.
    afxBool                 resizable;  // flag to indicate if allocations may be required to be resized.
};

AFX_DEFINE_STRUCT(afxAllocation)
{
    afxSize                 amount;    // count of an fixed-size allocation or size of afxByte sequence for arbitrary allocations.
    afxSize                 align;  // useful when element requires alignment, such as 128-bit alignment for SIMD instructions. If zero, Qwadro will align to 16 bytes.
    afxAllocationDuration   duration;   // the expected duration of allocations.
    afxBool                 resizable;  // flag to indicate if allocations may be required to be resized.
    afxHint const           *hint;
};

//AFX afxError        AfxMemContextRequest(afxMemContext mctx, afxAllocation const *a, );
//AFX afxError        AfxMemContextDispose(afxMemContext mctx, afxNat cnt, void *mem[]);

AFX void*           AfxMemContextAllocate(afxMemContext mctx, afxSize siz, afxHint const hint); // 'siz' is not used if is a fixed-size allocator.
AFX afxResult       AfxMemContextCoallocate(afxMemContext mctx, afxNat cnt, afxSize siz, afxHint const hint, void *chunks[]); // 'siz' is not used if is a fixed-size allocator.
AFX afxResult       AfxMemContextDeallocate(afxMemContext mctx, afxNat cnt, void *chunks[]);
AFX afxError        AfxMemContextEnableDebugging(afxMemContext mctx, afxNat level);
AFX afxError        AfxMemContextExhaust(afxMemContext mctx); // free mctx allocations
AFX afxResult       AfxMemContextForEachPage(afxMemContext mctx, afxResult(*f)(afxMemContext mctx, void *page, void* data), void *data);
AFX afxResult       AfxMemContextForEachChunk(afxMemContext mctx, afxResult(*f)(afxMemContext mctx, void *chunk, void* data), void *data);
AFX afxSize         AfxMemContextGetAlignment(afxMemContext mctx);
AFX afxSize         AfxMemContextGetCapacity(afxMemContext mctx);
AFX afxSize         AfxMemContextGetPop(afxMemContext mctx);
AFX afxSize         AfxMemContextGetStock(afxMemContext mctx); // useful to get restant capacity of a preallocated memory pool.
AFX afxError        AfxMemContextOptimize(afxMemContext mctx); // try to defragment, reorder allocations and/or free unused reserved space.
AFX afxResult       AfxMemContextReallocate(afxMemContext mctx, afxNat cnt, afxSize siz, void *chunksIn[], void *chunksOut[], afxHint const hint); // unsupported for fixed-size allocators.

AFX_DEFINE_STRUCT(afxMemPage)
{
    afxFcc          fcc; // MPAG
    afxSize         siz;
    afxSize         freeSiz;
    afxChain        chunks;
};

AFX_OBJECT(afxMemContext)
{
    afxObject               obj;

    afxChain                pages; // slabs, pages, arenas, etc
    afxChain                chunks;

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

    void*                   (*alloc)(afxMemContext mctx, afxSize rawsiz, afxHint const hint);
    void*                   (*coalloc)(afxMemContext mctx, afxSize cnt, afxSize rawsiz, afxHint const hint);
    void*                   (*realloc)(afxMemContext mctx, void* mem, afxSize rawsiz, afxHint const hint);
    afxResult               (*dealloc)(afxMemContext mctx, void* mem);

    afxError                (*dismantle)(afxMemContext mctx);
    afxError                (*exhaust)(afxMemContext mctx);
    afxError                (*optimize)(afxMemContext mctx);
    afxResult               (*forEachPage)(afxMemContext mctx, afxResult(AFXCALL *callback)(afxMemContext mctx, void *page, void* data), void* data);
    afxResult               (*forEachChunk)(afxMemContext mctx, afxResult(AFXCALL *callback)(afxMemContext mctx, void *chunk, void* data), void* data);

    afxFcc                  extFourCc;
    afxByte                 extData[AFX_ALL_EXTRA_DATA];
};

#endif//AFX_ALLOCATOR_H