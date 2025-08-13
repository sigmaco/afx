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

  //////////////////////////////////////////////////////////////////////////////
 // FIXED-SIZE PAGED MEMORY PREALLOCATION POOL                               //
//////////////////////////////////////////////////////////////////////////////

/*
    This is a paged memory pool, optimized for fixed-size allocation (like GPU resource descriptors, entity IDs, or UI elements).
    The bitmask in usage allows efficient allocation checks.
    With 32-bit usage mask, the max unitsPerPage is fixed to 32.
    It is aware of alignment-sensitive use cases (SIMD, hardware buffers, etc.).

    Validation via afxFcc is a way to debug memory corruption or ensure structure integrity.

    TODO:
    If _AFX_POOL_VALIDATION_ENABLED is active, consider adding guard bytes or poison values on free for debugging.
*/

#ifndef AFX_POOL_H
#define AFX_POOL_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/mem/afxMemory.h"

// Qwadro allocation policy enforce it to be at least AFX_SIMD_ALIGNMENT.
#define AFX_POOL_ALIGNMENT AFX_SIMD_ALIGNMENT
// The 32-bit usage bitmask per page enforce it to be at most 32.
#define AFX_MAX_UNITS_PER_POOL_PAGE (32)


#if (defined(_AFX_DEBUG)) && \
    !(defined(_AFX_POOL_VALIDATION_ENABLED))
#   define _AFX_POOL_VALIDATION_ENABLED TRUE
#endif

AFX_DEFINE_STRUCT(afxPoolPage)
// Structure specifying a page for a memory pool.
{
#ifdef _AFX_POOL_VALIDATION_ENABLED
    afxFcc          fcc; // afxFcc_POOL;
#endif
    afxLink         link;
    // Number of units currently used in this page.
    afxUnit         usedCnt;
    // Bitfield tracking unit allocation status (e.g., 1 = used, 0 = free).
    // This assumes up to 32 units per page, also enforcing this limitation.
    afxMask         usage;
    // Pointer to the actual memory block for this page.
    afxByte*        base;
};

AFX_DEFINE_STRUCT(afxPool)
// Structure specifying a paged memory pool.
{
#ifdef _AFX_POOL_VALIDATION_ENABLED
    afxFcc          fcc; // afxFcc_POOL;
#endif
    // Size (in bytes) of each unit being pooled.
    afxUnit         unitSiz;
    // Memory alignment required for each unit.
    // Qwadro allocation policy require it to be at least AFX_SIMD_ALIGNMENT.
    afxUnit         memAlign;
    // Number of units each page holds.
    // The 32-bit usage bitmask per page enforce it to be at most 32.
    afxUnit         unitsPerPage;
    afxMask         freeMaskPattern;
    // Total allocated units across all pages.
    afxUnit         totalUsedCnt;
    // The maximum number of pages allowed.
    afxUnit         maxPagCnt;
    // Number of allocated pages.
    afxUnit         pageCnt;
    // Pointer to array of pages.
    afxPoolPage**   pages;
    afxChain        freePags;
    // Memory manager or allocator context.
    afxMmu          mem;
};

/*
    The AfxDeployPool() function configures a memory pool but does not allocate memory.
    Actual allocations happen on-demand via requisition and reclamation operations.
    That approach avoids overcommitting memory and lets application scale dynamically based on real usage.
*/

AFX afxError    AfxDeployPool
(
    afxPool*    pool, 
    // Size of each allocation unit.
    afxUnit     unitSiz, 
    // Number of units per memory page.
    afxUnit     unitsPerPage, 
    // Memory alignment for each unit.
    afxUnit     memAlign
);

/*
    The AfxExhaustPool() function resets or tears down the pool depending on the @keepPages flag.
    The application must call this function to discomission the pool. 
    It only releases allocations, the pool continues ready to any subsequent requisition operation.

    Using @keepPages is a great optimization.
    It quickly resets the pool for reuse (e.g., frame allocator style) also avoids reallocation overhead in the next cycle.
    It is useful for high-performance, transient workloads.
*/

AFX afxError    AfxExhaustPool
(
    afxPool*    pool, 
    // If TRUE, fully free all allocated pages and reset pool metadata.
    // If FALSE, reset usage tracking, but keep page memory allocated.
    afxBool     keepPages
);

/*
    The AfxRequestPoolUnits() function allocates on-demand multiples units from the pool.

    If the function cannot allocate all requested units successfully, then it must free any units it did allocate during that call,
    leaving the pool in its original state.

    This function MUST:
        Find available (free) units across pages.
        If a page is full, allocate a new page.
        For each allocated unit:
        Assign a global unit ID (e.g., pageIndex * unitsPerPage + unitIndex)
        Store its unit IDs into ids[].
        Store its address in units[].
        Update usage bitfields and counts.
*/

AFX afxError    AfxRequestPoolUnits
(
    afxPool*    pool, 
    afxUnit     cnt, 
    afxUnit     ids[], 
    void*       units[]
);

/*
    The AfxReclaimPoolUnits() function returns previously allocated units back to the pool.
    All units MUST came from the specified pool.

    This function MUST:
        Locate which page it belongs to;
        Calculate the unit's index within the page;
        Clear the corresponding usage bit;
        Decrement usage counters; and
        Update the free page list, if you're tracking one.
*/

AFX afxError    AfxReclaimPoolUnits
(
    afxPool*    pool,
    afxUnit     cnt, 
    void*       units[]
);

AFX afxBool     AfxIsAnValidPoolUnit(afxPool* pool, afxSize idx);

AFX afxBool     AfxGetPoolUnit(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnits(afxPool const* pool, afxUnit cnt, afxSize const idx[], void *ptr[]);
AFX afxBool     AfxGetLinearPoolUnits(afxPool const* pool, afxUnit first, afxUnit cnt, void *ptr[]);

AFX afxBool     AfxFindPoolUnit(afxPool* pool, void* unit, afxUnit* idx, afxUnit* localIdx);
AFX afxError    AfxTakePoolUnit(afxPool* pool, afxSize idx, void *val);
AFX afxError    AfxFreePoolUnit(afxPool* pool, afxSize idx);

AFX afxUnit     AfxEnumeratePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, void *items[]);
AFX afxUnit     AfxInvokePoolItems(afxPool const* pool, afxUnit first, afxUnit cnt, afxBool(*f)(void* item, void* udd), void *udd);

AFX afxError AfxRequestPoolUnitsAt(afxPool* pool, afxUnit cnt, afxUnit const ids[], void* units[]);

// pool: afxPool*
// pageIdx: user-defined variable (int or afxUnit)
// slotIdx: user-defined variable (int or afxUnit)

#define AFX_FOREACH_USED_SLOT_IN_POOL(pool, pageIdx, slotIdx) \
    for (pageIdx = 0; pageIdx < (pool)->pageCnt; ++pageIdx) \
        for (afxPoolPage* _page = (pool)->pages[pageIdx], _pageRef = _page; \
             _page != NULL && _pageRef == _page; _page = NULL) \
            for (afxMask _mask = _pageRef->usage, _u = _mask, slotIdx = 0; _u; ++slotIdx, _u >>= 1) \
                if (_u & 1)

#define AFX_FOREACH_USED_PTR_IN_POOL(pool, pageIdx, slotIdx, ptr) \
    for (pageIdx = 0; pageIdx < (pool)->pageCnt; ++pageIdx) \
        for (afxPoolPage* _page = (pool)->pages[pageIdx], _pageRef = _page; \
             _page != NULL && _pageRef == _page; _page = NULL) \
            for (afxMask _mask = _pageRef->usage, _u = _mask, slotIdx = 0; _u; ++slotIdx, _u >>= 1) \
                if (_u & 1) \
                    for (void* ptr = _pageRef->base + (slotIdx * (pool)->unitSiz); ptr; ptr = NULL)

// pool: pointer to afxPool
// type: the actual type stored in the pool
// obj: name of the typed variable inside the loop
// pageIdx, slotIdx: loop counters (you define them outside)

#define AFX_FOREACH_USED_OBJ_IN_POOL(pool, type, obj, pageIdx, slotIdx) \
    for (pageIdx = 0; pageIdx < (pool)->pageCnt; ++pageIdx) \
        for (afxPoolPage* _page = (pool)->pages[pageIdx], _pageRef = _page; \
             _page != NULL && _pageRef == _page; _page = NULL) \
            for (afxMask _mask = _pageRef->usage, _u = _mask, slotIdx = 0; _u; ++slotIdx, _u >>= 1) \
                if (_u & 1) \
                    for (type* obj = (type*)(_pageRef->base + (slotIdx * (pool)->unitSiz)); obj; obj = NULL)

// Free slot iterator with typed object.
// Same structure as before, but it only visits free (unused) slots in the pool.
// pool: pointer to afxPool
// type: the actual type stored in the pool
// obj: the loop variable of that type
// pageIdx, slotIdx: loop counters

#define AFX_FOREACH_FREE_OBJ_IN_POOL(pool, type, obj, pageIdx, slotIdx) \
    for (pageIdx = 0; pageIdx < (pool)->pageCnt; ++pageIdx) \
        for (afxPoolPage* _page = (pool)->pages[pageIdx], \
                       _pageRef = _page; \
             _page != NULL && _pageRef == _page; _page = NULL) \
            for (afxMask _mask = _pageRef->usage, _u = ~_mask, slotIdx = 0; _u; ++slotIdx, _u >>= 1) \
                if (_u & 1) \
                    for (type* obj = (type*)(_pageRef->base + (slotIdx * (pool)->unitSiz)); obj; obj = NULL)


// Early-breaking or removable loop variant.
// This variant allows removal inside the loop by editing the bitmask and tracking objects carefully.
// Use with caution, especially if you mutate the bitmask during iteration.
// This one doesn't cache the original bitmask --- so if you modify _page->usage inside the loop (e.g., mark as free), 
// it reflects immediately.

#define AFX_FOREACH_USED_OBJ_IN_POOL_MUTABLE(pool, type, obj, pageIdx, slotIdx) \
    for (pageIdx = 0; pageIdx < (pool)->pageCnt; ++pageIdx) \
        for (afxPoolPage* _page = (pool)->pages[pageIdx]; _page; ) \
            for (afxMask _u = _page->usage, slotIdx = 0; slotIdx < (pool)->unitsPerPage; ++slotIdx) \
                if (_u & (1u << slotIdx)) \
                    for (type* obj = (type*)(_page->base + (slotIdx * (pool)->unitSiz)); obj; obj = NULL)



#endif//AFX_POOL_H
