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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_RING_C
#include "../dev/afxIoImplKit.h"

AFX_DEFINE_STRUCT(afxRing)
{
    afxUnit32    readIdx;
    afxUnit32    writeIdx;
    afxAtom32   readableItemCnt;
    afxUnit32    itemCnt;
    afxUnit      unitSiz;
    afxByte*    data;
};

afxError Init(afxRing* ring, afxUnit unitSiz, afxUnit32 itemCnt)
{
    afxError err = NIL;
    ring->itemCnt = itemCnt;
    ring->readIdx = 0;
    ring->writeIdx = 0;
    ring->readableItemCnt = 0;
    ring->unitSiz = unitSiz;

    if (!(ring->data = AfxAllocate(itemCnt, unitSiz, AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    return err;
}

void Term(afxRing* ring)
{
    ring->itemCnt = 0;
    ring->readIdx = 0;
    ring->writeIdx = 0;
    ring->readableItemCnt = 0;

    if (ring->data)
    {
        AfxDeallocate(ring->data);
        ring->data = NIL;
    }
}

// ---- Producer ---- //

afxUnit32 AfxGetRingWriteIndex(afxRing* ring)
{
    return ring->writeIdx;
}

void* AfxGetRingWritePtr(afxRing* ring)
{
    return &ring->data[ring->writeIdx * ring->unitSiz];
}

afxUnit32 AfxGetRingNbWritableItems(afxRing* ring)
{
    return ring->itemCnt - (afxUnit32)ring->readableItemCnt;
}

void AfxIncrementRingWriteIndex(afxRing* ring, afxUnit32 itemCnt)
{
    afxError err = NIL;
    AFX_ASSERT(AfxGetRingNbWritableItems(ring) >= itemCnt);
    ring->writeIdx = (ring->writeIdx + itemCnt) % ring->itemCnt;
    AfxAddAtom32(&ring->readableItemCnt, itemCnt);
}

// ---- Consumer ----

afxUnit32 AfxGetRingReadIndex(afxRing* ring)
{
    return ring->readIdx;
}

void const* AfxGetRingReadPtr(afxRing* ring)
{
    return &ring->data[ring->readIdx * ring->unitSiz];
}

// Peek at any item between the read and write pointer without advancing the read pointer.
void const* AfxPeekRing(afxRing* ring, afxUnit32 offset)
{
    afxError err = NIL;
    AFX_ASSERT((afxUnit32)ring->readableItemCnt > offset);
    afxUnit32 readIdx = (ring->readIdx + offset) % ring->itemCnt;
    return &ring->data[readIdx * ring->unitSiz];
}

void AfxIncrementRingReadIndex(afxRing* ring, afxUnit32 itemCnt)
{
    afxError err = NIL;
    AFX_ASSERT((afxUnit32)ring->readableItemCnt >= ring->itemCnt);
    ring->readIdx = (ring->readIdx + itemCnt) % ring->itemCnt;
    AfxSubAtom32(&ring->readableItemCnt, itemCnt);
}

afxUnit32 AfxGetRingNbReadableItems(afxRing* ring)
{
    return ring->readableItemCnt;
}

afxUnit32 AfxGetRingSize(afxRing* ring)
{
    return ring->itemCnt;
}

// Warning: requires external locking to prevent concurrent Grow+Read in a multi-threaded scenario. 
// Like the rest of the class, assumes a single writing thread.
afxBool AfxGrowRing(afxRing* ring, afxUnit32 growBy)
{
    afxUnit32 newItemCnt = ring->itemCnt + growBy;
    afxByte* newData;
    afxUnit unitSiz = ring->unitSiz;

    if ((newData = AfxAllocate(newItemCnt, unitSiz, 0, AfxHere())))
    {
        if (ring->readableItemCnt)
        {
            if (ring->readIdx >= ring->writeIdx)
            {
                // insert new free space in the middle of the buffer.

                if (ring->writeIdx)
                    AfxCopy(newData, ring->data, unitSiz * ring->writeIdx);

                AfxCopy(&newData[unitSiz * (ring->readIdx + growBy)], &ring->data[unitSiz * ring->readIdx], unitSiz * (ring->itemCnt - ring->readIdx));
                ring->readIdx += growBy;
            }
            else
            {
                // insert new free space at the end of the buffer.
                AfxCopy(&newData[unitSiz * ring->readIdx], &ring->data[unitSiz * ring->readIdx], unitSiz * (afxUnit32)ring->readableItemCnt);
            }
        }

        AfxDeallocate(ring->data);
        ring->data = newData;
        ring->itemCnt = newItemCnt;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
