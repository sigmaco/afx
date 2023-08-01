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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

/*

In computer science, region-based memory management is a type of memory management in which each allocated object is assigned to a region.
A region, also called a zone, arena, area, or memory context, is a collection of allocated objects that can be efficiently reallocated or deallocated all at once.
Like stack allocation, regions facilitate allocation and deallocation of memory with low overhead; but they are more flexible, allowing objects to live longer than the stack frame in which they were allocated.
In typical implementations, all objects in a region are allocated in a single contiguous range of memory addresses, similarly to how stack frames are typically allocated.

*/

#ifndef AFX_ARENA_H
#define AFX_ARENA_H

#include "afx/core/mem/afxMemory.h"
#include "afx/core/io/afxStream.h"

AFX_DEFINE_STRUCT(afxArenaSpecification)
{
    afxNat      chunkSiz;
    afxNat      largeItemSiz;
    afxNat      initialCleanupSiz;
    afxBool     recycle;
};

AFX_DEFINE_STRUCT(afxArena)
{
    _AFX_DBG_FCC; // AREN
    afxMemory   mem;
    afxHint     hint;
    afxNat      totalAllocated;
    afxNat      smallItems;
    afxNat      largeItems;
    afxNat      chunkCnt;
    afxNat      unusedSpace; /* Unused space due to alignment, etc. */

    afxNat      allocated;
    char*       initialData;
    char*       data;

    afxNat      maxCleanupCnt;
    afxNat      cleanupCnt;
    void*       cleanups;
    void*       largeList;

    afxNat      chunkSiz;
    afxNat      largeItemSiz;
    void**      recycleBin;
    afxNat      recycleSiz;
};

AFX afxError    AfxAcquireArena(afxMemory mem, afxArena* aren, afxArenaSpecification const *spec, afxHint const hint);
AFX void        AfxReleaseArena(afxArena* aren);


AFX afxSize     AfxAddArenaCleanup(afxArena* aren, void(*action)(void *data, void*extra), void *data, void*extra);
AFX void        AfxRemoveArenaCleanup(afxArena* aren, void(*action)(void *, void*), void *data);

AFX void*       AfxRequestArenaSpace(afxArena* aren, afxSize size);
AFX void*       AfxArenaCoallocate(afxArena* aren, afxSize num, afxSize size);

AFX void*       AfxRequestArenaStorage(afxArena* aren, const void *init, afxSize size);
AFX void*       AfxRequestArenaStorages(afxArena* aren, const void *init, afxSize num, afxSize size);

AFX void*       AfxRequestArenaZeroedSpace(afxArena* aren, afxSize size);
AFX void*       AfxRequestArenaZeroedSpaces(afxArena* aren, afxSize num, afxSize size);

AFX void        AfxExhaustArena(afxArena* aren);

AFX char*       AfxArenaDuplicateString(afxArena* aren, const char *string);

AFX void        AfxRecycleArenaSpace(afxArena* aren, void *block, afxSize size);

AFX void        AfxDumpArenaStats(afxArena* aren, afxStream *out);

AFX afxSize     AfxGetArenaRecycleSize(afxArena* aren);
AFX afxSize     AfxGetArenaTotalAllocated(afxArena* aren);
AFX afxSize     AfxGetArenaUnusedSpace(afxArena* aren);

AFX void        AfxLogArenaStats(afxArena* aren);

#endif//AFX_ARENA_H