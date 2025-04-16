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
*/

#ifndef AFX_ARENA_H
#define AFX_ARENA_H

#include "qwadro/inc/mem/afxMemory.h"
#include "qwadro/inc/io/afxStream.h"

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

AFX afxError    AfxMakeArena(afxArena* aren, afxArenaInfo const *spec, afxHere const hint);
AFX void        AfxDismantleArena(afxArena* aren);

AFX void        AfxExhaustArena(afxArena* aren);

/*
    The AfxRequestArenaUnit() function allocates and optionally initializes portions of a given size, from the arena.
*/

AFX void*       AfxRequestArenaUnit
(
    // Arena to allocate from.
    afxArena*   aren, 
    // Size of each unit.
    afxSize     size, 
    // Number of units.
    afxUnit     cnt, 
    // Optional data to copy into the new allocation.
    void const* src, 
    // Stride between source units (0 = tightly packed).
    afxUnit     stride
);

AFX void        AfxRecycleArenaUnit(afxArena* aren, void* p, afxSize size);

AFX afxSize     AfxPushArenaCleanup(afxArena* aren, void(*action)(void *data, void*extra), void *data, void*extra);
AFX afxBool     AfxPopArenaCleanup(afxArena* aren, void(*action)(void *, void*), void *data);

AFX char*       AfxArenaDuplicateString(afxArena* aren, const char *string);

AFX void        AfxDumpArenaStats(afxArena* aren, afxStream out);

AFX void        AfxGetArenaInfo(afxArena* aren, afxUnit* totalAllocated, afxUnit* unusedSpace, afxUnit* recycleSiz);

AFX void        AfxLogArenaStats(afxArena* aren);

#endif//AFX_ARENA_H
