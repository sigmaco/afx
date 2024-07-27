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

#ifndef AFX_MMU_H
#define AFX_MMU_H

#include "qwadro/base/afxDebug.h"

#ifdef _AFX_DEBUG // after afxDebug!!!
#   define VLD_FORCE_ENABLE
#endif

#if 0
#if (defined(_WIN64) || defined(_WIN32))
#   ifdef VLD_FORCE_ENABLE
#       include <vld.h>
#   endif
#endif
#endif

#include "qwadro/base/afxObject.h"
#include "qwadro/base/afxChain.h"
#include "qwadro/base/afxFcc.h"
#include "qwadro/exec/afxSlock.h"

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

#if 0
AFXINL void AfxMakeSegment(afxSegment *seg, afxSize off, afxSize siz, afxSize stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(seg); AfxAssert(siz);
    seg->off = off;
    seg->siz = siz;
    seg->stride = stride ? stride : siz;
}
#endif

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

//AFX afxError        AfxMemoryRequest(afxMmu mmu, afxAllocation const *a, );
//AFX afxError        AfxMemoryDispose(afxMmu mmu, afxNat cnt, void *mem[]);
#endif

AFX_DEFINE_STRUCT(afxContextPage)
{
    afxFcc                  fcc; // MPAG
    afxLinkage              all;
    afxSize                 siz;
    afxSize                 freeSiz;
    afxChain                slices;
};


AFX_DEFINE_HANDLE(afxMemory);

typedef enum afxMemFlags
{
    afxMemFlag_ZEROED       = AFX_BIT(0),
    afxMemFlag_RESIZABLE    = AFX_BIT(1),

    afxMemFlag_TEMPORARY    = AFX_BIT(8), // to be used just inside "this" function.
    afxMemFlag_TRANSIENT    = AFX_BIT(9), // to be used across functions, as example signaling objects about an event occurance.
    afxMemFlag_PERMANENT    = AFX_BIT(10), // to be used across the entire system and or subsystem, 
    afxMemFlag_DURATION_MASK= afxMemFlag_TEMPORARY | afxMemFlag_TRANSIENT | afxMemFlag_PERMANENT,

    AFX_MEM_PROP_DEVICE_LOCAL, // specifies that memory allocated with this type is the most efficient for device access. This property will be set if and only if the memory type belongs to a heap with the VK_MEMORY_HEAP_DEVICE_LOCAL_BIT set.
    AFX_MEM_PROP_HOST_VISIBLE, // specifies that memory allocated with this type can be mapped for host access using vkMapMemory.
    AFX_MEM_PROP_HOST_COHERENT, // specifies that the host cache management commands vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges are not needed to flush host writes to the device or make device writes visible to the host, respectively.
    AFX_MEM_PROP_HOST_CACHED, // specifies that memory allocated with this type is cached on the host.Host memory accesses to uncached memory are slower than to cached memory, however uncached memory is always host coherent.
    AFX_MEM_PROP_LAZILY_ALLOCATED // specifies that the memory type only allows device access to the memory.
} afxMemFlags;

typedef void*(*afxAllocCallback)(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxHere const hint);
typedef void*(*afxReallocCallback)(afxMmu mmu, void* p, afxSize cnt, afxSize siz, afxSize align, afxHere const hint);
typedef void(*afxDeallocCallback)(afxMmu mmu, void* p);
typedef void(*afxNotifyAllocCallback)(afxMmu mmu, afxSize cnt, afxSize siz, afxHere const hint);
typedef void(*afxNotifyDeallocCallback)(afxMmu mmu, afxSize cnt, afxSize siz, afxHere const hint);

typedef afxMemory(*afxAllocCallback2)(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxMemFlags flags, afxHere const hint);
typedef afxError(*afxResizeCallback2)(afxMemory mem, afxSize cnt, afxSize siz, afxHere const hint);
typedef afxError(*afxDeallocCallback2)(afxMemory mem, afxHere const hint);
typedef void(*afxNotifyAllocCallback2)(afxMmu mmu, afxMemory mem, afxSize cnt, afxSize siz, afxHere const hint);
typedef void(*afxNotifyDeallocCallback2)(afxMmu mmu, afxMemory mem, afxSize cnt, afxSize siz, afxHere const hint);

AFX_DEFINE_STRUCT(afxAllocator)
{
    afxAllocCallback            alloc;
    afxReallocCallback          realloc;
    afxDeallocCallback          dealloc;
    afxNotifyAllocCallback      allocNot;
    afxNotifyDeallocCallback    deallocNot;
};

typedef void*(*ator)(afxMmu mmu, void* old, afxSize siz, afxSize align, afxHere const hint); // if siz is zero, memory is freed

AFX_OBJECT(afxMemory)
{
    afxLinkage      mmu;
    afxSize         siz;
    afxSize         cnt;
    afxNat          align;
    afxMask         flags;
    afxByte*        start;
};

AFX afxError    AfxMapMemory(afxMemory mem, afxSize off, afxSize siz, void **ptr); // Map a memory object into application address space
AFX void        AfxUnmapMemory(afxMemory mem); // Unmap a previously mapped memory object

AFX afxMemory   AfxAllocateMemory(afxMmu mmu, afxSize siz, afxSize cnt, afxNat align, afxMemFlags flags, afxHere const hint); // Allocate memory
AFX void        AfxDeallocateMemory(afxMemory mem, afxHere const hint); // Free memory

AFX void        AfxZeroMemory(afxMemory mem, afxSize offset, afxSize range);
AFX void        AfxFillMemory(afxMemory mem, afxSize offset, afxSize range, afxNat unitSiz, void const* value);
AFX void        AfxCopyMemory(afxMemory mem, afxSize base, afxMemory in, afxSize offset, afxSize range, afxNat unitSiz);

AFX afxError    AfxResizeMemory(afxMemory mem, afxSize siz, afxSize cnt, afxHere const hint);

AFX void        AfxUpdateMemory(afxMemory mem, afxSize offset, afxSize range, afxNat unitSiz, void const* src);
AFX void        AfxDumpMemory(afxMemory mem, afxSize offset, afxSize range, afxNat unitSiz, void* dst);

AFX afxMemory   AfxOpenMemory(afxMmu mmu, afxSize siz, void const* start);

#ifdef _AFX_MMU_C
AFX_OBJECT(afxMmu)
{
    afxAllocCallback            allocCb;
    afxReallocCallback          reallocCb;
    afxDeallocCallback          deallocCb;
    afxNotifyAllocCallback      allocNotCb;
    afxNotifyDeallocCallback    deallocNotCb;

    afxAllocCallback2           allocCb2;
    afxResizeCallback2          reallocCb2;
    afxDeallocCallback2         deallocCb2;
    afxNotifyAllocCallback2     allocNotCb2;
    afxNotifyDeallocCallback2   deallocNotCb2;

    afxSlock                    memSlock;
    afxChain                    memChain;
    afxSize                     defAlign;

// debug
    afxNat                      dbgLevel; // 0 - nothing, 1 - mechanism activity, 2 - block activity, 3 - portion (de)allocations, 4 - [implementation-dependent]
    afxChar const*              func;
    afxChar const*              fname;
    afxSize                     fline;
};
#endif//_AFX_MMU_C

AFX afxError                AfxMemoryEnableDebugging(afxMmu mmu, afxNat level);
AFX afxSize                 AfxMemoryGetDefaultAlignment(afxMmu mmu);

AFX void*                   AfxAllocate(afxSize cnt, afxSize siz, afxNat align, afxHere const hint);
AFX void*                   AfxCoallocate(afxSize siz, afxSize cnt, afxNat align, afxHere const hint);
AFX void*                   AfxReallocate(void *p, afxSize siz, afxSize cnt, afxNat align, afxHere const hint);
AFX void                    AfxDeallocate(void *p);

#define                     AfxStream(cnt_,srcStride_,dstStride_,src_,dst_) AfxStream2(cnt_,src_,srcStride_,dst_,dstStride_)
AFX void                    AfxStream2(afxNat cnt, void const* src, afxSize srcStride, void* dst, afxNat dstStride);

AFX void                    AfxCopy(void* dst, void const* src, afxSize siz);
AFX void                    AfxCopy2(afxSize cnt, afxSize siz, void const *src, void *dst);

AFX void                    AfxFill(void* p, void const* value, afxSize siz, afxSize cnt);
AFX void                    AfxFill2(afxSize cnt, afxSize siz, void const* value, void *p);

AFX void                    AfxZero(void* p, afxSize siz);
AFX void                    AfxZero2(afxSize cnt, afxSize siz, void *p);

#define AFX_ALIGNED_SIZEOF(operand_,alignment_) ((operand_ + (alignment_ - 1)) & ~(alignment_ - 1))

//#define AFX_ZERO(chnk_) AfxZero2((chnk_), sizeof(*(chnk_)))
//#define AFX_FILL(chnk_, val_) AfxFill2((chnk_), sizeof(*(chnk_)), (val_))
//#define AFX_COPY(chnkDst_, chnkSrc_) AfxCopy2((chnkDst_), (chnkSrc_), sizeof(*(chnkDst_)))

//AFX afxBool             _AfxGetMemD(afxMmu mmu, struct _afxCtxD **memD, struct _afxSysD* sysD);

AFX afxError                AfxAcquireMmus(afxNat cnt, afxHere const hint, afxAllocationStrategy const strategy[], afxMmu mmus[]);


AFX void*                   AfxMemset(void* dst, afxInt val, afxSize siz);
AFX void*                   AfxMemcpy(void* dst, void const* src, afxSize siz);
AFX afxInt                  AfxMemcmp(void const* buf1, void const* buf2, afxSize siz);
AFX void const*             AfxMemchr(void const* buf, afxInt val, afxNat cap);


AFX void                    AfxFree(void* block);
AFX void*                   AfxMalloc(afxSize siz);
AFX void*                   AfxCalloc(afxSize cnt, afxSize siz);
AFX void*                   AfxRealloc(void* block, afxSize siz);

AFX void                    AfxFreeAligned(void* block);
AFX void*                   AfxMallocAligned(afxSize siz, afxSize align);
AFX void*                   AfxReallocAligned(void* block, afxSize siz, afxSize align);
AFX void*                   AfxRecallocAligned(void* block, afxSize cnt, afxSize siz, afxSize align);


AFX afxError _AfxInitMmu(afxThread thr);
AFX afxError _AfxDeinitMmu(afxThread thr);

#endif//AFX_MMU_H
