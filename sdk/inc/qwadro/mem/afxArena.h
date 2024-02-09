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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

/*

In computer science, region-based memory management is a type of memory management in which each allocated object is assigned to a region.
A region, also called a zone, arena, area, or memory context, is a collection of allocated objects that can be efficiently reallocated or deallocated all at once.
Like stack allocation, regions facilitate allocation and deallocation of memory with low overhead; but they are more flexible, allowing objects to live longer than the stack frame in which they were allocated.
In typical implementations, all objects in a region are allocated in a single contiguous range of memory addresses, similarly to how stack frames are typically allocated.

*/

#ifndef AFX_ARENA_H
#define AFX_ARENA_H

#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxStream.h"

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
    afxMmu      mmu;
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

AFX afxError    AfxAllocateArena(afxMmu mmu, afxArena* aren, afxArenaSpecification const *spec, afxHint const hint);
AFX void        AfxDeallocateArena(afxArena* aren);


AFX afxSize     AfxAddArenaCleanup(afxArena* aren, void(*action)(void *data, void*extra), void *data, void*extra);
AFX void        AfxRemoveArenaCleanup(afxArena* aren, void(*action)(void *, void*), void *data);

AFX void*       AfxRequestArenaUnit(afxArena* aren, afxSize size);
AFX void*       AfxRequestArenaUnits(afxArena* aren, afxSize num, afxSize size);

AFX void*       AfxRequestArenaStorage(afxArena* aren, const void *init, afxSize size);
AFX void*       AfxRequestArenaStorages(afxArena* aren, const void *init, afxSize num, afxSize size);

AFX void*       AfxRequestZeroedArenaUnit(afxArena* aren, afxSize size);
AFX void*       AfxRequestZeroedArenaUnits(afxArena* aren, afxSize num, afxSize size);

AFX void        AfxExhaustArena(afxArena* aren);

AFX char*       AfxArenaDuplicateString(afxArena* aren, const char *string);

AFX void        AfxRecycleArenaUnit(afxArena* aren, void *block, afxSize size);

AFX void        AfxDumpArenaStats(afxArena* aren, afxStream out);

AFX afxSize     AfxGetArenaRecycleSize(afxArena* aren);
AFX afxSize     AfxGetArenaTotalAllocated(afxArena* aren);
AFX afxSize     AfxGetArenaUnusedSpace(afxArena* aren);

AFX void        AfxLogArenaStats(afxArena* aren);

#endif//AFX_ARENA_H
