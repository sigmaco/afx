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

#if (defined _AFX_DEBUG) && !(defined(_AFX_SLAB_ALLOC_VALIDATION_ENABLED))
#   define _AFX_SLAB_ALLOC_VALIDATION_ENABLED TRUE
#endif

AFX_DEFINE_STRUCT(afxSlabLink)
{
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    afxFcc              fcc; // afxFcc_SLAB;
#endif
    afxSlabLink*        next;
    afxByte AFX_ADDR    data[];
};

AFX_DEFINE_STRUCT(afxSlab)
{
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    afxFcc              fcc; // afxFcc_SLAB;
#endif
    afxSlab*            next;
    afxSlab*            prev;
    afxSlabLink*        firstFree;
    afxUnit             unitCnt;
    afxByte AFX_ADDR    units[1];
};

AFX_DEFINE_STRUCT(afxSlabAllocator)
{
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    afxFcc              fcc; // afxFcc_SLAB;
#endif
    afxUnit             unitSiz;
    afxUnit             unitAlign;
    afxUnit             unitsPerSlab;
    afxUnit             cachedSlabSiz; // to avoid recalc it every call and to ease footer-based linking.
    afxSlab             anchor;
};

AFX afxError    AfxDeploySlabAllocator(afxSlabAllocator* mgr, afxUnit unitSiz, afxUnit unitsPerSlab);

AFX afxError    AfxDismantleSlabAllocator(afxSlabAllocator* mgr);

AFX afxError    AfxPopSlabUnit(afxSlabAllocator* mgr, void* p);

AFX void*       AfxPushSlabUnit(afxSlabAllocator* mgr);

#endif//AFX_SLAB_ALLOCATOR_H