/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "../exec/afxSystemDDK.h"

//#define _AFX_SLAB_LINK_HEADER TRUE
//#define AFX_SLAB_DOUBLY_LINKED TRUE

_AFXINL afxSlab* _AfxFindSlabForPtr(afxSlabAllocator* mgr, void* p)
{
    afxError err = { 0 };
    AFX_ASSERT(mgr);
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    AFX_ASSERT(mgr->fcc == afxFcc_SLAB);
    AFX_ASSERT(mgr->anchor.fcc == afxFcc_SLAB);
#endif

    afxUnit range = mgr->unitSiz * mgr->unitsPerSlab;
    afxSlab const* anchor = &mgr->anchor;
    afxSlab* slab = anchor->next;
    afxByte const* p2 = p;

    while (slab != anchor)
    {
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
        AFX_ASSERT(slab->fcc == afxFcc_SLAB);
#endif

#ifdef _AFX_SLAB_LINK_HEADER
        afxByte const* base = slab->units;
#else
        afxByte const* base = AFX_CAST(afxByte const*, slab) - mgr->cachedSlabSiz - sizeof(*slab);
#endif

        if (p2 >= base && p2 < &base[range])
            return slab;

        slab = slab->next;
    }
    return NIL;
}

_AFX void* AfxPushSlabUnit(afxSlabAllocator* mgr)
{
    afxError err = { 0 };
    AFX_ASSERT(mgr);
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    AFX_ASSERT(mgr->fcc == afxFcc_SLAB);
    AFX_ASSERT(mgr->anchor.fcc == afxFcc_SLAB);
#endif
    afxSlab const* anchor = &mgr->anchor;
    afxSlab* slab = anchor->next;
    afxUnit unitsPerSlab = mgr->unitsPerSlab;
    afxBool slabFound = FALSE;

    // What if anchor as an UDD buffer?
    while (slab != anchor)
    {
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
        AFX_ASSERT(slab->fcc == afxFcc_SLAB);
#endif
        if (slab->firstFree || (unitsPerSlab > slab->unitCnt))
        {
            slabFound = TRUE;
            break;
        }
        slab = slab->next;
    }

    if (!slabFound && AfxAllocate((unitsPerSlab * (mgr->unitSiz + sizeof(afxSlabLink))) + sizeof(*slab), mgr->unitAlign, AfxHere(), (void**)&slab))
    {
        AfxThrowError();
        return 0;
    }
    else
    {
        slab->next = &mgr->anchor;
        slab->prev = mgr->anchor.prev;
        slab->next->prev = slab;
        slab->prev->next = slab;
        slab->firstFree = NIL;
        slab->unitCnt = 0;
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
        slab->fcc = afxFcc_SLAB;
#endif
    }

    if (slab)
    {
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
        AFX_ASSERT(slab->fcc == afxFcc_SLAB);
#endif
        afxByte* p = NIL;

#ifdef _AFX_SLAB_LINK_HEADER
        afxSlabLink* slot = slab->firstFree;

        if (slot)
            slab->firstFree = slot->next;
        else
            slot = (afxSlabLink*)(&slab->units[slab->unitCnt * (mgr->unitSiz + sizeof(afxSlabLink))]);

        p = slot->data;
#else
        afxSlabLink* slot = slab->firstFree;

        if (slot)
        {
            // skip content to get the base
            p = AFX_CAST(afxByte*, slot) - mgr->unitSiz;
            slab->firstFree = slot->next;
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
            AFX_ASSERT(slot->fcc == afxFcc_SLAB);
#endif
        }
        else
        {
            // skip occupied slots up to the next free one.
            p = &slab->units[slab->unitCnt * (mgr->unitSiz + sizeof(afxSlabLink))];
            // skip content to get footer
            slot = (afxSlabLink*)(&p[mgr->unitSiz]);
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
            slot->fcc = afxFcc_SLAB;
#endif
        }
#endif

        ++slab->unitCnt;
        AFX_ASSERT(unitsPerSlab >= slab->unitCnt);
        return p;
    }
    return 0;
}

_AFX afxError AfxPopSlabUnit(afxSlabAllocator* mgr, void* p)
{
    afxError err = { 0 };
    AFX_ASSERT(mgr);
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    AFX_ASSERT(mgr->fcc == afxFcc_SLAB);
    AFX_ASSERT(mgr->anchor.fcc == afxFcc_SLAB);
#endif
    AFX_ASSERT(p);
    afxUnit range = mgr->unitSiz * mgr->unitsPerSlab;
    afxSlab const* anchor = &mgr->anchor;
    afxSlab* slab = anchor->next;
    afxByte const* p2 = p;

    // What if anchor as an UDD buffer?
    while (slab != anchor)
    {
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
        AFX_ASSERT(slab->fcc == afxFcc_SLAB);
#endif

#ifdef _AFX_SLAB_LINK_HEADER
        afxByte const* base = slab->units;
#else
        afxByte const* base = AFX_CAST(afxByte const*, slab) - mgr->cachedSlabSiz - sizeof(*slab);
#endif
        if (p2 >= base && p2 < &base[range])
        {
#ifdef _AFX_SLAB_LINK_HEADER
            afxSlabLink* slot = AFX_REBASE(p, afxSlabLink, data);
#else
            afxSlabLink* slot = (afxSlabLink*)(AFX_CAST(afxByte const*, p) + mgr->unitSiz);
#endif

#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
            AFX_ASSERT(slot->fcc == afxFcc_SLAB);
#endif

            if (slab->firstFree)
            {
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
                AFX_ASSERT(slab->firstFree->fcc == afxFcc_SLAB);
#endif
                slot->next = slab->firstFree;
            }
            slab->firstFree = slot;
            break;
        }
        slab = slab->next;
    }
    return err;
}

// Discommisions a slab allocation mechanism.

_AFX afxError AfxDismantleSlabAllocator(afxSlabAllocator* mgr)
{
    afxError err = { 0 };
    AFX_ASSERT(mgr);
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    AFX_ASSERT(mgr->fcc == afxFcc_SLAB);
    AFX_ASSERT(mgr->anchor.fcc == afxFcc_SLAB);
#endif
    afxSlab const* anchor = &mgr->anchor;
    afxSlab* slab = anchor->next;

    if (slab && (slab != anchor))
    {
        do
        {
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
            AFX_ASSERT(slab->fcc == afxFcc_SLAB);
#endif
            slab->next->prev = slab->prev;
            slab->prev->next = slab->next;
            AfxDeallocate((void**)&slab, AfxHere());
            slab = anchor->next;
        } while (slab != anchor);
    }
    return err;
}

// Deploys a slab allocation mechanism.

_AFX afxError AfxDeploySlabAllocator(afxSlabAllocator* mgr, afxUnit unitSiz, afxUnit unitsPerSlab)
{
    afxError err = { 0 };
    AFX_ASSERT(mgr);
#ifdef _AFX_SLAB_ALLOC_VALIDATION_ENABLED
    mgr->fcc = afxFcc_SLAB;
    mgr->anchor.fcc = afxFcc_SLAB;
#endif
    mgr->anchor.next = &mgr->anchor;
    mgr->anchor.prev = &mgr->anchor;
    mgr->anchor.firstFree = NIL;
    mgr->anchor.unitCnt = 0;
    AFX_ASSERT(unitSiz);
    mgr->unitSiz = unitSiz;
    mgr->unitAlign = AFX_SIMD_ALIGNMENT;
    mgr->unitsPerSlab = AFX_MAX(1, unitsPerSlab);
    mgr->cachedSlabSiz = (mgr->unitsPerSlab * (mgr->unitSiz + sizeof(afxSlabLink))) + sizeof(afxSlab);
    return err;
}
