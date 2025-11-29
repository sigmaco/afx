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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_RING_C
#include "../exec/afxSystemDDK.h"

_AFX afxError AfxDeployRing(afxRing* ring, afxUnit unitSiz, afxUnit32 cap)
{
    afxError err = { 0 };
    ring->cap = cap;
    ring->readIdx = 0;
    ring->writeIdx = 0;
    ring->readableItemCnt = 0;
    ring->unitSiz = unitSiz;

    if (AfxAllocate(cap * unitSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&ring->data))
        AfxThrowError();

    return err;
}

_AFX void AfxDismantleRing(afxRing* ring)
{
    ring->cap = 0;
    ring->readIdx = 0;
    ring->writeIdx = 0;
    ring->readableItemCnt = 0;

    if (ring->data)
    {
        AfxDeallocate((void**)&ring->data, AfxHere());
        ring->data = NIL;
    }
}

// ---- Producer ---- //

_AFX afxUnit32 AfxGetRingWriteIndex(afxRing* ring)
{
    return ring->writeIdx;
}

_AFX void* AfxGetRingWritePtr(afxRing* ring)
{
    return &ring->data[ring->writeIdx * ring->unitSiz];
}

_AFX afxUnit32 AfxGetRingNbWritableItems(afxRing* ring)
{
    return ring->cap - (afxUnit32)ring->readableItemCnt;
}

_AFX void AfxIncrementRingWriteIndex(afxRing* ring, afxUnit32 itemCnt)
{
    afxError err = { 0 };
    AFX_ASSERT(AfxGetRingNbWritableItems(ring) >= itemCnt);
    ring->writeIdx = (ring->writeIdx + itemCnt) % ring->cap;
    AfxAddAtom32(&ring->readableItemCnt, itemCnt);
}

// ---- Consumer ----

_AFX afxUnit32 AfxGetRingReadIndex(afxRing* ring)
{
    return ring->readIdx;
}

_AFX void const* AfxGetRingReadPtr(afxRing* ring)
{
    return &ring->data[ring->readIdx * ring->unitSiz];
}

// Peek at any item between the read and write pointer without advancing the read pointer.
_AFX void const* AfxPeekRing(afxRing* ring, afxUnit32 offset)
{
    afxError err = { 0 };
    AFX_ASSERT((afxUnit32)ring->readableItemCnt > offset);
    afxUnit32 readIdx = (ring->readIdx + offset) % ring->cap;
    return &ring->data[readIdx * ring->unitSiz];
}

_AFX void AfxIncrementRingReadIndex(afxRing* ring, afxUnit32 itemCnt)
{
    afxError err = { 0 };
    AFX_ASSERT((afxUnit32)ring->readableItemCnt >= ring->cap);
    ring->readIdx = (ring->readIdx + itemCnt) % ring->cap;
    AfxSubAtom32(&ring->readableItemCnt, itemCnt);
}

_AFX afxUnit32 AfxGetRingNbReadableItems(afxRing* ring)
{
    return ring->readableItemCnt;
}

_AFX afxUnit32 AfxGetRingSize(afxRing* ring)
{
    return ring->cap;
}

// Warning: requires external locking to prevent concurrent Grow+Read in a multi-threaded scenario. 
// Like the rest of the class, assumes a single writing thread.
_AFX afxBool AfxGrowRing(afxRing* ring, afxUnit32 growBy)
{
    afxUnit32 newItemCnt = ring->cap + growBy;
    afxByte* newData;
    afxUnit unitSiz = ring->unitSiz;

    if (!AfxAllocate(newItemCnt * unitSiz, 0, AfxHere(), (void**)&newData))
    {
        if (ring->readableItemCnt)
        {
            if (ring->readIdx >= ring->writeIdx)
            {
                // insert new free space in the middle of the buffer.

                if (ring->writeIdx)
                    AfxCopy(newData, ring->data, unitSiz * ring->writeIdx);

                AfxCopy(&newData[unitSiz * (ring->readIdx + growBy)], &ring->data[unitSiz * ring->readIdx], unitSiz * (ring->cap - ring->readIdx));
                ring->readIdx += growBy;
            }
            else
            {
                // insert new free space at the end of the buffer.
                AfxCopy(&newData[unitSiz * ring->readIdx], &ring->data[unitSiz * ring->readIdx], unitSiz * (afxUnit32)ring->readableItemCnt);
            }
        }

        AfxDeallocate((void**)&ring->data, AfxHere());
        ring->data = newData;
        ring->cap = newItemCnt;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
