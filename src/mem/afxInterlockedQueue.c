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

_AFXINL afxError AfxMakeInterlockedQueue(afxInterlockedQueue* ique, afxUnit unitSiz, afxUnit cap)
{
    afxError err = { 0 };
    AFX_ASSERT(ique);

#ifdef _AFX_INTERLOCKED_QUEUE_VALIDATION_ENABLED
    ique->fcc = afxFcc_FIFO;
#endif
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(cap); // The number of entries. Must be a power of two.

    // Initializa a file afxInterlockedQueue e aloca memória para o número especificado de entradas.
    // O número de entradas não é mutável após inicialização.

    cap = AfxPowerOfTwo32(cap);
    AFX_ASSERT((cap & (cap - 1)) == 0); // test for power of 2
    AFX_ASSERT_P2(cap);
    
    ique->queIdxMask = cap - 1;
    ique->writePosn = 0;
    ique->readPosn = 0;
    ique->unitSiz = unitSiz;

    if (AfxAllocate(cap * sizeof(ique->entrySeqIdx[0]), 0, AfxHere(), (void**)&ique->entrySeqIdx))
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cap; ++i)
    {
        //ique->entries[i].value = TDEFAULT;
        AfxStoreAtom32(&ique->entrySeqIdx[i], i);
    }

    if (AfxAllocate(cap * unitSiz, 0, AfxHere(), (void**)&ique->entryValue)) AfxThrowError();
    else
    {
        AfxZero(ique->entryValue, cap * ique->unitSiz);
    }

    if (err && ique->entrySeqIdx)
        AfxDeallocate((void**)&ique->entrySeqIdx, AfxHere());

    return err;
}

_AFXINL afxError AfxExhaustInterlockedQueue(afxInterlockedQueue* ique)
{
    afxError err = { 0 };
    AFX_ASSERT(ique);

    // Libera a memória reservado para a fila e redefine o estado interno.
    // A fila deve estar vazia quando esta função for chamada.

    if (ique->entrySeqIdx)
    {
        AFX_ASSERT(ique->readPosn == ique->writePosn);
        AfxDeallocate((void**)&ique->entrySeqIdx, AfxHere());
        ique->entrySeqIdx = NIL;
    }

    if (ique->entryValue)
    {
        AFX_ASSERT(ique->readPosn == ique->writePosn);
        AfxDeallocate((void**)&ique->entryValue, AfxHere());
        ique->entryValue = NIL;
    }

    ique->readPosn = 0;
    ique->writePosn = 0;
    return err;
}

_AFXINL afxBool AfxPushInterlockedQueue(afxInterlockedQueue* ique, void* src)
{
    afxError err = { 0 };
    AFX_ASSERT(ique);
    AFX_ASSERT(src);

    afxUnit32 const queIdxMask = ique->queIdxMask;
    afxInt32 writePosn = AfxLoadAtom32(&ique->writePosn);
    afxInt32 seqDelta;

    while (1)
    {
        // Checks if the current write slot is available (seqDelta == 0).
        // See where we are in the sequence, relative to where we can write data.
        seqDelta = AfxLoadAtom32(&ique->entrySeqIdx[writePosn & queIdxMask]) - writePosn;
        
        if (0 != seqDelta) 
        {
            if (seqDelta < 0)
                return FALSE; // FULL; we would have over-enqueued if we tried to write the position in. Return false.
            
            writePosn = AfxLoadAtom32(&ique->writePosn); // RETRY: if it didn't work, reload writePos: someone else must have written to the sequence and we need to get caught up
        }
        // CAS ensures only one producer advances the writePosn.
        else if (AfxCasAtom32(&ique->writePosn, &writePosn, writePosn + 1))
            break; // if we're in the right spot, and we can successfully write an updated write position, break out and write the handle into the queue
    }

    // Writes to the data and advances the entrySeqIdx to signal it's ready.
    // Memory is safely written only after writePosn claim is made.
    // Slot is marked with entrySeqIdx = writePosn + 1.
    // Advance the sequence by one so that it can be dequeued
    AfxCopy(&ique->entryValue[(writePosn & queIdxMask) * ique->unitSiz], src, ique->unitSiz);
    AfxStoreAtom32(&ique->entrySeqIdx[writePosn & queIdxMask], writePosn + 1);
    return TRUE;
}

_AFXINL afxBool AfxPopInterlockedQueue(afxInterlockedQueue* ique, void* dst)
{
    afxError err = { 0 };
    AFX_ASSERT(ique);
    AFX_ASSERT(dst);

    afxInt32 const queIdxMask = ique->queIdxMask;
    afxInt32 readPosn = AfxLoadAtom32(&ique->readPosn);
    afxInt32 seqDelta;

    while (1)
    {
        // Waits for the slot to be ready for reading (seqDelta == 0).
        // See where we are in the sequence relative to where we can write data.
        seqDelta = AfxLoadAtom32(&ique->entrySeqIdx[readPosn & queIdxMask]) - (readPosn + 1);

        if (0 != seqDelta)
        {
            if (seqDelta < 0)
                return FALSE; // EMPTY: if an entry has yet to be written, bail out.
            
            readPosn = AfxLoadAtom32(&ique->readPosn); // RETRY: if it didn't work, reload readPos
        }
        else if (AfxCasAtom32(&ique->readPosn, &readPosn, readPosn + 1)) 
            break; // if we're in the right spot, and we can successfully write an updated read position, break out and read the entry
    }

    // Reads the value.
    // Marks the slot as reusable by setting entrySeqIdx = readPosn + queIdxMask + 1.
    // Update the acceptable sequence value for this entry
    AfxCopy(dst, &ique->entryValue[(readPosn & queIdxMask) * ique->unitSiz], ique->unitSiz);
    AfxStoreAtom32(&ique->entrySeqIdx[readPosn & queIdxMask], readPosn + ique->queIdxMask + 1);
    return TRUE;
}

_AFXINL afxBool AfxIsInterlockedQueueEmpty(afxInterlockedQueue* ique)
{
    afxError err = { 0 };
    AFX_ASSERT(ique);
    afxInt32 readPosn = AfxLoadAtom32(&ique->readPosn);
    afxInt32 seqDelta = AfxLoadAtom32(&ique->entrySeqIdx[readPosn & ique->queIdxMask]) - (readPosn + 1);
    // Returns TRUE if the next read slot is not yet ready.
    return seqDelta < 0;
}
