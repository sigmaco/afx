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
 // VARIABLE-SIZE MEMORY PREALLOCATION ARENA                                 //
//////////////////////////////////////////////////////////////////////////////

/*
    In computer science, region-based memory management is a type of memory management in which 
    each allocated object is assigned to a region.
    A region, also called a zone, arena, area, or memory context, is a collection of allocated 
    objects that can be efficiently reallocated or deallocated all at once.
    Like stack allocation, regions facilitate allocation and deallocation of memory with low 
    overhead; but they are more flexible, allowing objects to live longer than the stack frame 
    in which they were allocated.
    In typical implementations, all objects in a region are allocated in a single contiguous 
    range of memory addresses, similarly to how stack frames are typically allocated.

    Chunk Allocation. Reuses a single large block (chunkSiz) for fast allocations.
    Large Object Handling. Anything over largeItemSiz gets its own allocation (afxArenaLargeItem).
    Memory Recycling. Small objects can be reclaimed and reused (linked-list buckets by size).
    Cleanup Callbacks. AfxPushArenaCleanup() supports callbacks for teardown on arena destruction.
    Deferred Allocation. Lazily initializes the initial chunk only when needed.
    Debug & Logging. AfxDumpArenaStats() and AfxLogArenaStats() print detailed diagnostics.
*/

#ifndef AFX_ARENA_H
#define AFX_ARENA_H

#include "qwadro/mem/afxMemory.h"
#include "qwadro/io/afxStream.h"

#if (defined _AFX_DEBUG) && !(defined(_AFX_ARENA_VALIDATION_ENABLED))
#   define _AFX_ARENA_VALIDATION_ENABLED TRUE
#endif

AFX_DEFINE_STRUCT(afxArenaInfo)
{
    afxUnit     chunkSiz;
    afxUnit     largeItemSiz;
    afxUnit     initialCleanupSiz;
    afxBool     recycle;
};

AFX_DEFINE_STRUCT(afxArena)
// A chunk-based, allocator-backed memory arena
// It may track memory for diagnostics, recycle previously freed objects and delay freeing via cleanup routines.
{
#ifdef _AFX_ARENA_VALIDATION_ENABLED
    afxFcc      fcc; // afxFcc_AREN;
#endif
    afxMmu      mmu;
    afxHere     hint;
    afxUnit     totalAllocated;
    afxUnit     smallItems;
    afxUnit     largeItems;
    afxUnit     chunkCnt;
    afxUnit     unusedSpace; /* Unused space due to alignment, etc. */

    afxUnit     allocated;
    char*       initialData;
    char*       data;

    afxUnit     maxCleanupCnt;
    afxUnit     cleanupCnt;
    void*       cleanups;
    void*       largeList;

    afxUnit     chunkSiz;
    afxUnit     largeItemSiz;
    afxBool     recycleEnabled;
    void**      recycleBin;
    afxUnit     recycleSiz;
};

/*
    The AfxMakeArena() function initializes the arena with parameters in spec (includes chunk size, capacity, recycling flags, etc.).
*/

AFX afxError        AfxMakeArena
(
    afxArena*       aren, 
    afxArenaInfo const*spec, 
    // A debug/location info.
    afxHere const   hint
);

/*
    The AfxDismantleArena() function destroys the arena: runs cleanup handlers, frees chunks, and resets all internal state.
*/

AFX void            AfxDismantleArena
(
    afxArena*       aren
);

/*
    The AfxExhaustArena() function purges or marks the arena as exhausted, 
    freeing temporary or recyclable memory but not completely dismantling it.
    Could be useful for reusing an arena between frames or logic scopes.
*/

AFX void            AfxExhaustArena
(
    afxArena*       aren
);

/*
    The AfxAssumeArena() function transfer all allocations from a source arena into a destination one,
    while ensuring that resources aren't leaked or corrupted.
*/

AFX afxError        AfxAssumeArena
(
    afxArena*       dst, 
    // The arena to be transfer to @dst.
    afxArena*       src
);

/*
    The AfxMergeArena() function merges all reusable allocations from a source arena into a destination one,
    while ensuring that resources aren't leaked or corrupted.

    Zero-allocation design: Reuses existing cleanup arrays when possible.
    Safe to call just before AfxDismantleArena(src) (since AfxMergeArena() takes over all ownership).
    Does not free or reuse chunks directly — but their cleanups now live in dst, so they’ll be freed when dst is exhausted.
*/

AFX afxError        AfxMergeArena
(
    afxArena*       dst, 
    // The arena is no longer active.
    afxArena*       src
);

/*
    The AfxRequestFromArena() function allocates and optionally initializes portions of a given size, from the arena.    
    If @src is non-null, it copies from @src, possibly with a @stride.
    This supports bulk allocation with optional init. Great for tightly packed data like structs or strings.
*/

AFX void*           AfxRequestFromArena
(
    // Arena to allocate from.
    afxArena*       aren, 
    // Size of each unit.
    afxSize         size, 
    // Number of units.
    afxUnit         cnt, 
    // Optional data to copy into the new allocation.
    void const*     src, 
    // Stride between source units (0 = tightly packed).
    afxUnit         stride
);

/*
    The AfxReclaimToArena() function returns memory to the arena if recycling is enabled.
    Adds the pointer to the recycleBin for reuse.
*/

AFX void            AfxReclaimToArena
(
    afxArena*       aren, 
    void*           p, 
    afxSize         size
);

AFX afxSize     AfxPushArenaCleanup(afxArena* aren, void(*action)(void *data, void*extra), void *data, void*extra);
AFX afxBool     AfxPopArenaCleanup(afxArena* aren, void(*action)(void *, void*), void *data);

/*
    The AfxArenaDuplicateString() function allocates and copies a string into arena memory.
    Super handy for avoiding individual heap allocations for strings.
*/

AFX char*       AfxArenaDuplicateString(afxArena* aren, const char *string);

/*
    The AfxDumpArenaStats() function dumps stats (allocated size, 
    unused space, recycling status) to a stream (file, logger, etc.).
*/

AFX void        AfxDumpArenaStats(afxArena* aren, afxStream out);

/*
    The AfxGetArenaInfo() function queries internal stats.
    Useful for monitoring memory usage or during testing.
*/

AFX void        AfxGetArenaInfo(afxArena* aren, afxUnit* totalAllocated, afxUnit* unusedSpace, afxUnit* recycleSiz);

/*
    The AfxLogArenaStats() function logs current state of the arena.
*/

AFX void        AfxLogArenaStats(afxArena* aren);

#endif//AFX_ARENA_H
