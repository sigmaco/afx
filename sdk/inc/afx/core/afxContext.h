/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MEMORY_H
#define AFX_MEMORY_H

#ifdef _DEBUG
//#   define VLD_FORCE_ENABLE
#endif

#include <stdlib.h>
#if (defined(_WIN64) || defined(_WIN32))
#   ifdef VLD_FORCE_ENABLE
#       include <vld.h>
#   endif
#endif

#include "afx/core/afxInstance.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxFcc.h"
#include "afx/core/afxDebug.h"

//#define _AFX_TRY_ASSERT_ALLOCATION_AREA 1

// MEMORY LAYOUT
// size is the raw lenght of a type in bytes.
// alignment is an additional size used to pad elements in memory.
// stride is the sum of size with alignment.

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

//AFX afxError        AfxMemoryRequest(afxContext mem, afxAllocation const *a, );
//AFX afxError        AfxMemoryDispose(afxContext mem, afxNat cnt, void *mem[]);
#endif

AFX_DEFINE_STRUCT(afxAllocationBlock)
{
    afxNat              usedUnitCnt;
    afxByte             *base;
    afxNat              firstIdx;
    afxAllocationBlock  *prev;
};

AFX_DEFINE_STRUCT(afxContextPage)
{
    afxFcc                  fcc; // MPAG
    afxLinkage              all;
    afxSize                 siz;
    afxSize                 freeSiz;
    afxChain                slices;
};


typedef enum afxMemFlags
{
    AFX_MEM_PROP_DEVICE_LOCAL, // specifies that memory allocated with this type is the most efficient for device access. This property will be set if and only if the memory type belongs to a heap with the VK_MEMORY_HEAP_DEVICE_LOCAL_BIT set.
    AFX_MEM_PROP_HOST_VISIBLE, // specifies that memory allocated with this type can be mapped for host access using vkMapMemory.
    AFX_MEM_PROP_HOST_COHERENT, // specifies that the host cache management commands vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges are not needed to flush host writes to the device or make device writes visible to the host, respectively.
    AFX_MEM_PROP_HOST_CACHED, // specifies that memory allocated with this type is cached on the host.Host memory accesses to uncached memory are slower than to cached memory, however uncached memory is always host coherent.
    AFX_MEM_PROP_LAZILY_ALLOCATED // specifies that the memory type only allows device access to the memory.
} afxMemFlags;

typedef void*(*afxAllocCallback)(afxContext mem, afxSize siz, afxSize align, afxHint const hint);
typedef void*(*afxReallocCallback)(afxContext mem, void* p, afxSize siz, afxSize align, afxHint const hint);
typedef void(*afxDeallocCallback)(afxContext mem, void* p);
typedef void(*afxNotifyAllocCallback)(afxContext mem, afxSize siz, afxHint const hint);
typedef void(*afxNotifyDeallocCallback)(afxContext mem, afxSize siz, afxHint const hint);

AFX_DEFINE_STRUCT(afxAllocator)
{
    afxAllocCallback            alloc;
    afxReallocCallback          realloc;
    afxDeallocCallback          dealloc;
    afxNotifyAllocCallback      allocNot;
    afxNotifyDeallocCallback    deallocNot;
};

typedef void*(*ator)(afxContext mem, void* old, afxSize siz, afxSize align, afxHint const hint); // if siz is zero, memory is freed

AFX_DEFINE_STRUCT(afxMem)
{
    _AFX_DBG_FCC
    afxAllocator    ator;
    afxSize         siz;
    struct
    {
        afxNat      base; // when forked
        union
        {
            afxByte*start; // when wrapped or externally buffered.
            afxMem* parent; // when forked
        };
    }               src;
};

AFX_DEFINE_STRUCT(afxMappedMemoryRange)
{
    afxMem*     mem;
    afxNat      off;
    afxNat      siz;
};

AFX afxError    AfxMapMemory(afxMem *mem, afxSize off, afxNat siz, void **ptr); // Map a memory object into application address space
AFX void        AfxUnmapMemory(afxMem *mem); // Unmap a previously mapped memory object

AFX afxError    AfxAllocateMemory(afxMem *mem, afxMem *parent, afxNat siz, afxNat align); // Allocate memory
AFX afxError    AfxReallocateMemory(afxMem *mem, afxNat siz, afxNat align);
AFX void        AfxDeallocateMemory(afxMem *mem); // Free memory

AFX afxError    AfxCopyMemory(afxMem *mem, afxMem const* in); // returns non-copied range in error occurence.
AFX afxError    AfxCopyMemoryRange(afxMem *mem, afxMem const* in, afxNat offset, afxNat range); // returns non-copied range in error occurence.

AFX void        AfxZeroMemory(afxMem *mem);
AFX afxError    AfxZeroMemoryRange(afxMem *mem, afxNat offset, afxNat range); // returns non-zeroed range in error occurence.

#ifdef _AFX_CONTEXT_C
AFX_OBJECT(afxContext)
{
    afxAllocCallback            allocCb;
    afxReallocCallback          reallocCb;
    afxDeallocCallback          deallocCb;
    afxNotifyAllocCallback      allocNotCb;
    afxNotifyDeallocCallback    deallocNotCb;

    afxSize                     defAlign;

// debug
    afxNat                      dbgLevel; // 0 - nothing, 1 - mechanism activity, 2 - block activity, 3 - portion (de)allocations, 4 - [implementation-dependent]
    afxChar const*              func;
    afxChar const*              fname;
    afxSize                     fline;
};
#endif

AFX afxNat                  AfxEnumerateContexts(afxNat first, afxNat cnt, afxContext mem[]);

AFX afxError                AfxAcquireContexts(afxNat cnt, afxContext ctx[], afxAllocationStrategy const strategy[], afxHint const hint);

AFX afxError                AfxMemoryEnableDebugging(afxContext mem, afxNat level);
AFX afxSize                 AfxMemoryGetDefaultAlignment(afxContext mem);

AFX void*                   AfxAllocate(afxContext mem, afxSize siz, afxNat align, afxHint const hint);
AFX void*                   AfxCoallocate(afxContext mem, afxSize cnt, afxSize siz, afxNat align, afxHint const hint);
AFX void*                   AfxReallocate(afxContext mem, void *p, afxSize siz, afxNat align, afxHint const hint);
AFX void                    AfxDeallocate(afxContext mem, void *p);

AFX void                    AfxStream(afxNat cnt, afxSize srcStride, afxNat dstStride, void const* src, void* dst);
AFX void                    AfxCopy(void *dst, void const *src, afxSize range);
AFX void                    AfxFill(void *p, afxNat cnt, afxNat unitSiz, void const* value);
AFX void                    AfxZero(void *p, afxSize range);

#define AFX_ALIGN(operand_,alignment_) ((operand_ + (alignment_ - 1)) & ~(alignment_ - 1))

//#define AFX_ZERO(chnk_) AfxZero((chnk_), sizeof(*(chnk_)))
//#define AFX_FILL(chnk_, val_) AfxFill((chnk_), sizeof(*(chnk_)), (val_))
//#define AFX_COPY(chnkDst_, chnkSrc_) AfxCopy((chnkDst_), (chnkSrc_), sizeof(*(chnkDst_)))

//AFX afxBool             _AfxGetMemD(afxContext mem, struct _afxCtxD **memD, struct _afxSysD* sysD);

#endif//AFX_MEMORY_H
