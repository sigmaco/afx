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

#ifndef AFX_MEMORY_H
#define AFX_MEMORY_H

#ifdef _DEBUG
#   define VLD_FORCE_ENABLE
#endif

#include <stdlib.h>
#if (defined(_WIN64) || defined(_WIN32))
#   ifdef VLD_FORCE_ENABLE
#       include <vld.h>
#   endif
#endif

#include "afx/core/afxObject.h"
#include "afx/core/afxLinkedList.h"
#include "afx/core/afxFcc.h"
#include "afx/core/diag/afxDebug.h"

//#define _AFX_TRY_ASSERT_ALLOCATION_AREA 1

// MEMORY LAYOUT
// size is the raw lenght of a type in bytes.
// alignment is an additional size used to pad elements in memory.
// stride is the sum of size with alignment.

AFX_DEFINE_HANDLE(afxMemory);
AFX_DECLARE_STRUCT(afxSegment);
AFX_DECLARE_STRUCT(afxAllocation);

struct afxSegment
{
    afxSize off, siz, stride;
};

#define AfxSpawnSegment(off_,siz_,stride_) (afxSegment){ .off = (afxSize)(off_), .siz = (afxSize)(siz_), .stride = (afxSize)(stride_) }

AFXINL void AfxMakeSegment(afxSegment *seg, afxSize off, afxSize siz, afxSize stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(seg); AfxAssert(siz);
    seg->off = off, seg->siz = siz;
    seg->stride = stride ? stride : siz;
};

typedef enum afxAllocationFlags
{
    AFX_ALL_FLAG_RESIZABLE  = (1 << 8),
    AFX_ALL_FLAG_ZEROED     = (1 << 9),
} afxAllocationFlags;

typedef enum afxAllocationDuration
{
    AFX_ALL_DUR_TEMPORARY, // to be used just inside "this" function.
    AFX_ALL_DUR_TRANSIENT, // to be used across functions, as example signaling objects about an event occurance.
    AFX_ALL_DUR_PERMANENT, // to be used across the entire system and or subsystem, 
} afxAllocationDuration;

#define AfxFlagsGetDuration(dur_) (dur_ & 0x00000003)

#define AFX_ALL_EXTRA_DATA 256

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
    void                    *old; // old allocation to be reallocated.
    afxSize                 size;    // count of an fixed-size allocation or size of afxByte sequence for arbitrary allocations.
    afxSize                 align;  // useful when element requires alignment, such as 128-bit alignment for SIMD instructions. If zero, Qwadro will align to 16 bytes.
    afxFlags                flags;  // flag to indicate if allocations may be required to be resized and the expected duration of allocations.
    void                    *src;
};

//AFX afxError        AfxMemoryRequest(afxMemory mem, afxAllocation const *a, );
//AFX afxError        AfxMemoryDispose(afxMemory mem, afxNat cnt, void *mem[]);
#endif

AFX_DEFINE_STRUCT(afxMemoryPage)
{
    afxFcc                  fcc; // MPAG
    afxLinkage              all;
    afxSize                 siz;
    afxSize                 freeSiz;
    afxChain                slices;
};

AFX_OBJECT(afxMemory)
{
    afxObject               obj;
#ifdef _AFX_ALLOCATOR_C
    afxChain                pages; // slabs, pages, arenas, etc
    afxChain                unpagedChunks;

    afxSize                 elemSiz; // for fixed-size allocators
    afxSize                 align;
    afxSize                 cap; // when the max growth of memory pool is limited, or (afxSize)-1 for "infinity".
    afxNat                  stock;
    afxSize                 heapSiz; // when preallocate pages
    afxAllocationDuration   duration; // the expected duration of allocations.
    afxBool                 resizable; // flag to indicate if allocations may be required to be resized.

// debug
    afxNat                  dbgLevel; // 0 - nothing, 1 - mechanism activity, 2 - block activity, 3 - portion (de)allocations, 4 - [implementation-dependent]
    afxChar const           *func;
    afxChar const           *fname;
    afxSize                 fline;

    void*                   (*alloc)(afxMemory mem, afxSize rawsiz, afxHint const hint);
    void*                   (*coalloc)(afxMemory mem, afxSize cnt, afxSize rawsiz, afxHint const hint);
    void*                   (*realloc)(afxMemory mem, void* p, afxSize rawsiz, afxHint const hint);
    afxResult               (*dealloc)(afxMemory mem, void* p);

    afxError                (*exhaust)(afxMemory mem, afxBool disposeSpace, afxBool disposeStock);
    afxError                (*optimize)(afxMemory mem);
    afxResult               (*enumPages)(afxMemory mem, afxBool reverse, afxNat base, afxNat cnt, afxMemoryPage *mpag[]);
    afxResult               (*enumChunks)(afxMemory mem, afxBool reverse, afxNat base, afxNat cnt, void *chunks[]);

    afxFcc                  extFourCc;
    afxByte                 extData[AFX_ALL_EXTRA_DATA];
#endif
};

AFX afxError                AfxMemoryEnableDebugging(afxMemory mem, afxNat level);
AFX afxError                AfxMemoryExhaust(afxMemory mem, afxBool disposeSpace, afxBool disposeStock); // free all allocations
AFX afxResult               AfxMemoryEnumeratePages(afxMemory mem, afxBool reverse, afxNat base, afxNat cnt, afxMemoryPage *mpag[]);
AFX afxResult               AfxMemoryEnumerateChunks(afxMemory mem, afxBool reverse, afxNat base, afxNat cnt, void *chunks[]);
AFX afxSize                 AfxMemoryGetAlignment(afxMemory mem);
AFX afxSize                 AfxMemoryGetCapacity(afxMemory mem);
AFX afxSize                 AfxMemoryGetPop(afxMemory mem);
AFX afxSize                 AfxMemoryGetStock(afxMemory mem); // useful to get restant capacity of a preallocated memory pool.
AFX afxError                AfxMemoryOptimize(afxMemory mem); // try to defragment, reorder allocations and/or free unused reserved space.
AFX afxError                AfxMemoryDescribeStrategy(afxMemory mem, afxAllocationStrategy *as);

AFX void*                   AfxAllocate(afxMemory mem, afxSize siz, afxHint const hint);
AFX void*                   AfxCoallocate(afxMemory mem, afxSize cnt, afxSize siz, afxHint const hint);
AFX void*                   AfxReallocate(afxMemory mem, void *p, afxSize siz, afxHint const hint);
AFX afxResult               AfxDeallocate(afxMemory mem, void *p);

AFX void                    AfxCopy(void *dst, void const *src, afxSize range);
AFX void                    AfxFill(void *p, afxSize range, afxInt value);
AFX void                    AfxZero(void *p, afxSize range);

AFX afxResult               AfxWriteSegmented(void *p, afxNat cnt, afxSegment const *seg, void const *from);

#define AFX_ALIGN(operand_,alignment_) ((operand_ + (alignment_ - 1)) & ~(alignment_ - 1))

#define AFX_ZERO(chnk_) AfxZero((chnk_), sizeof(*(chnk_)))
#define AFX_FILL(chnk_, val_) AfxFill((chnk_), sizeof(*(chnk_)), (val_))
#define AFX_COPY(chnkDst_, chnkSrc_) AfxCopy((chnkDst_), (chnkSrc_), sizeof(*(chnkDst_)))

#endif//AFX_MEMORY_H
