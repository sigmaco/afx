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

#include "../dev/afxExecImplKit.h"

_AFXINL afxMemorySlab* _AfxFindSlabForPtr(afxSlabAllocator* mgr, void* p)
{
    afxError err = NIL;
    AFX_ASSERT(mgr);

    afxUnit range = mgr->unitSiz * mgr->unitsPerSlab;
    afxMemorySlab const* anchor = &mgr->anchor;
    afxMemorySlab* slab = anchor->next;
    afxByte const* p2 = p;

    while (slab != anchor)
    {
        afxByte const* base = slab->units;

        if (p2 >= base && p2 < &base[range])
            return slab;

        slab = slab->next;
    }
    return NIL;
}

_AFX void* AfxAllocateSlab(afxSlabAllocator* mgr)
{
    afxError err = NIL;
    AFX_ASSERT(mgr);
    afxMemorySlab const* anchor = &mgr->anchor;
    afxMemorySlab* slab = anchor->next;
    afxUnit unitsPerSlab = mgr->unitsPerSlab;
    afxBool slabFound = FALSE;

    while (slab != anchor)
    {
        if (slab->firstFree || unitsPerSlab > slab->unitCnt)
        {
            slabFound = TRUE;
            break;
        }
        slab = slab->next;
    }

    if (!slabFound && !(slab = AfxAllocate(1, sizeof(*slab) + (unitsPerSlab * mgr->unitSiz), 0, AfxHere()))) AfxThrowError();
    else
    {
        slab->next = &mgr->anchor;
        slab->prev = mgr->anchor.prev;
        slab->next->prev = slab;
        slab->prev->next = slab;
        slab->firstFree = NIL;
        slab->unitCnt = 0;
    }
    
    if (slab)
    {
        afxMemorySlabSlot* slot = slab->firstFree;

        if (slot)
            slab->firstFree = slot->next;
        else
            slot = (afxMemorySlabSlot*)(&slab->units[slab->unitCnt * mgr->unitSiz]);

        ++slab->unitCnt;
        AFX_ASSERT(unitsPerSlab >= slab->unitCnt);

        return slot->data;
    }
    return 0;
}

_AFX afxError AfxDeallocateSlab(afxSlabAllocator* mgr, void* p)
{
    afxError err = NIL;
    AFX_ASSERT(mgr);
    AFX_ASSERT(p);
    afxUnit range = mgr->unitSiz * mgr->unitsPerSlab;
    afxMemorySlab const* anchor = &mgr->anchor;
    afxMemorySlab* slab = anchor->next;
    afxByte const* p2 = p;

    while (slab != anchor)
    {
        afxByte const* base = slab->units;

        if (p2 >= base && p2 < &base[range])
        {
            afxMemorySlabSlot* slot = AFX_REBASE(p, afxMemorySlabSlot, data);
                    
            if (slab->firstFree)
                slab->firstFree = slab->firstFree->next;
                    
            slab->firstFree = slot;
            break;
        }
        slab = slab->next;
    }
    return err;
}

_AFX afxError AfxCleanUpSlabAllocator(afxSlabAllocator* mgr)
{
    afxError err = NIL;
    AFX_ASSERT(mgr);
    afxMemorySlab const* anchor = &mgr->anchor;
    afxMemorySlab* slab = anchor->next;

    if (slab && slab != anchor)
    {
        do
        {
            slab->next->prev = slab->prev;
            slab->prev->next = slab->next;
            AfxDeallocate(slab);
            slab = anchor->next;
        } while (slab != anchor);
    }
    return err;
}

_AFX afxError AfxSetUpSlabAllocator(afxSlabAllocator* mgr, afxUnit unitSiz, afxUnit unitsPerSlab)
{
    afxError err = NIL;
    AFX_ASSERT(mgr);
    AFX_ASSERT(unitSiz);
    mgr->unitsPerSlab = unitsPerSlab;
    mgr->unitSiz = unitSiz;
    mgr->anchor.next = &mgr->anchor;
    mgr->anchor.prev = &mgr->anchor;
    mgr->anchor.firstFree = NIL;
    mgr->anchor.unitCnt = 0;
    return err;
}
