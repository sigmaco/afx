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

/**
    Slab allocation is a memory management mechanism intended for the efficient memory allocation of objects.
    In comparison with earlier mechanisms, it reduces fragmentation caused by allocations and deallocations.
    This technique is used for retaining allocated memory containing a data object of a certain type for reuse upon subsequent allocations of objects of the same type.
    It is analogous to an object pool, but only applies to memory, not other resources.

    With slab allocation, a cache for a certain type or size of data object has a number of pre-allocated "slabs" of memory; 
    within each slab there are memory chunks of fixed size suitable for the objects. 
    The slab allocator keeps track of these chunks, so that when it receives a request to allocate memory for a data object of a certain type, 
    usually it can satisfy the request with a free slot (chunk) from an existing slab.
    When the allocator is asked to free the object's memory, it just adds the slot to the containing slab's list of free(unused) slots. 
    The next call to create an object of the same type (or allocate memory of the same size) will return that memory slot (or some other free slot) and remove it from the list of free slots. 
    This process eliminates the need to search for suitable memory space and greatly alleviates memory fragmentation.
    In this context, a slab is one or more contiguous pages in the memory containing pre-allocated memory chunks.
*/

#ifndef AFX_SLAB_ALLOCATOR_H
#define AFX_SLAB_ALLOCATOR_H

#include "qwadro/inc/mem/afxMemory.h"

AFX_DEFINE_STRUCT(afxMemorySlabSlot)
{
    afxMemorySlabSlot*  next;
    afxByte AFX_ADDR    data[];
};

AFX_DEFINE_STRUCT(afxMemorySlab)
{
    afxMemorySlab*      next;
    afxMemorySlab*      prev;
    afxMemorySlabSlot*  firstFree;
    afxUnit              unitCnt;
    afxByte AFX_ADDR    units[1];
};

AFX_DEFINE_STRUCT(afxSlabAllocator)
{
    afxUnit              unitSiz;
    afxUnit              unitsPerSlab;
    afxMemorySlab       anchor;
};

AFX afxError    AfxSetUpSlabAllocator(afxSlabAllocator* mgr, afxUnit unitSiz, afxUnit unitsPerSlab);

AFX afxError    AfxCleanUpSlabAllocator(afxSlabAllocator* mgr);

AFX afxError    AfxDeallocateSlab(afxSlabAllocator* mgr, void* p);

AFX void*       AfxAllocateSlab(afxSlabAllocator* mgr);

#endif//AFX_SLAB_ALLOCATOR_H