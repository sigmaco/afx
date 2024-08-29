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

/*

In computer science, region-based memory management is a type of memory management in which each allocated object is assigned to a region.
A region, also called a zone, arena, area, or memory context, is a collection of allocated objects that can be efficiently reallocated or deallocated all at once.
Like stack allocation, regions facilitate allocation and deallocation of memory with low overhead; but they are more flexible, allowing objects to live longer than the stack frame in which they were allocated.
In typical implementations, all objects in a region are allocated in a single contiguous range of memory addresses, similarly to how stack frames are typically allocated.

*/

#ifndef AFX_ARENA_H
#define AFX_ARENA_H

#include "qwadro/inc/mem/afxMemory.h"
#include "qwadro/inc/io/afxStream.h"

AFX_DEFINE_STRUCT(afxArenaSpecification)
{
    afxNat      chunkSiz;
    afxNat      largeItemSiz;
    afxNat      initialCleanupSiz;
    afxBool     recycle;
};

AFX_DEFINE_STRUCT(afxArena)
{
    afxMmu      mmu;
    afxHere     hint;
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
    afxBool     recycleEnabled;
    void**      recycleBin;
    afxNat      recycleSiz;
};

AFX afxError    AfxDeployArena(afxArena* aren, afxArenaSpecification const *spec, afxHere const hint);
AFX void        AfxDismantleArena(afxArena* aren);

AFX afxSize     AfxPushArenaCleanup(afxArena* aren, void(*action)(void *data, void*extra), void *data, void*extra);
AFX afxBool     AfxPopArenaCleanup(afxArena* aren, void(*action)(void *, void*), void *data);

AFX void*       AfxAllocateArena(afxArena* aren, afxSize size);
AFX void*       AfxCoallocateArena(afxArena* aren, afxSize num, afxSize size);

AFX void*       AfxAllocateArenaCopied(afxArena* aren, afxSize size, const void* init);
AFX void*       AfxCoallocateArenaCopied(afxArena* aren, afxSize num, afxSize size, const void* init);

AFX void*       AfxAllocateArenaZeroed(afxArena* aren, afxSize size);
AFX void*       AfxCoallocateArenaZeroed(afxArena* aren, afxSize num, afxSize size);

AFX void        AfxExhaustArena(afxArena* aren);

AFX char*       AfxArenaDuplicateString(afxArena* aren, const char *string);

AFX void        AfxRecycleArena(afxArena* aren, void* p, afxSize size);

AFX void        AfxDumpArenaStats(afxArena* aren, afxStream out);

AFX afxSize     AfxGetArenaRecycleSize(afxArena* aren);
AFX afxSize     AfxGetArenaTotalAllocated(afxArena* aren);
AFX afxSize     AfxGetArenaUnusedSpace(afxArena* aren);

AFX void        AfxLogArenaStats(afxArena* aren);

#endif//AFX_ARENA_H
