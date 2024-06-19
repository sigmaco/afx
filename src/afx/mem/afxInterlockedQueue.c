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

#include "qwadro/exec/afxSystem.h"

_AFXINL afxError AfxSetUpInterlockedQueue(afxInterlockedQueue* ique, afxNat unitSiz, afxNat cap)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ique);
    AfxAssert(unitSiz);
    AfxAssert(cap); /// The number of entries. Must be a power of two.

    /// Initializa a file afxInterlockedQueue e aloca memória para o número especificado de entradas.
    /// O número de entradas não é mutável após inicialização.

    AfxAssert((cap & (cap - 1)) == 0);

    ique->queIdxMask = cap - 1;
    ique->writePosn = 0;
    ique->readPosn = 0;
    ique->unitSiz = unitSiz;

    if (!(ique->entries = AfxAllocate(cap, sizeof(ique->entries[0]) + unitSiz, 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxZero(ique->entries, cap * (sizeof(ique->entries[0]) + unitSiz));

        for (afxNat i = 0; i < cap; ++i)
        {
            //ique->entries[i].value = TDEFAULT;
            AfxStoreAtom64(&ique->entries[i].seqIdx, i);
        }
    }
    return err;
}

_AFXINL afxError AfxCleanUpInterlockedQueue(afxInterlockedQueue* ique)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ique);

    /// Libera a memória reservado para a fila e redefine o estado interno.
    /// A fila deve estar vazia quando esta função for chamada.

    if (ique->entries)
    {
        AfxAssert(ique->readPosn == ique->writePosn);
        AfxDeallocate(ique->entries);
        ique->entries = NIL;
    }
    ique->readPosn = 0;
    ique->writePosn = 0;
    return err;
}

_AFXINL afxBool AfxPushInterlockedQueue(afxInterlockedQueue* ique, void* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ique);
    AfxAssert(src);

    afxNat64 const queIdxMask = ique->queIdxMask;
    afxInt64 writePosn = AfxLoadAtom64(&ique->writePosn);
    afxInt64 seqDelta;

    do
    {
        if (0 != (seqDelta = AfxLoadAtom64(&ique->entries[writePosn & queIdxMask].seqIdx) - writePosn)) // see where we are in the sequence, relative to where we can write data
        {
            if (seqDelta < 0) return FALSE; // we would have over-enqueued if we tried to write the position in. Return false; the user needs to decide how to handle things
            else writePosn = AfxLoadAtom64(&ique->writePosn); // if it didn't work, reload writePos: someone else must have written to the sequence and we need to get caught up
        }
        else if (AfxCasAtom64(&ique->writePosn, &writePosn, writePosn + 1))
            break; // if we're in the right spot, and we can successfully write an updated write position, break out and write the handle into the queue
    } while (1);

    // advance the sequence by one so that it can be dequeued
    AfxCopy(ique->entries[writePosn & queIdxMask].value, src, ique->unitSiz);
    AfxStoreAtom64(&ique->entries[writePosn & queIdxMask].seqIdx, writePosn + 1);
    return TRUE;
}

_AFXINL afxBool AfxPopInterlockedQueue(afxInterlockedQueue* ique, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ique);
    AfxAssert(dst);

    afxInt64 const queIdxMask = ique->queIdxMask;
    afxInt64 readPosn = AfxLoadAtom64(&ique->readPosn);
    afxInt64 seqDelta;

    do
    {
        if (0 != (seqDelta = AfxLoadAtom64(&ique->entries[readPosn & queIdxMask].seqIdx) - (readPosn + 1))) // see where we are in the sequence relative to where we can write data
        {
            if (seqDelta < 0) return FALSE; // if an entry has yet to be written, bail out                
            else readPosn = AfxLoadAtom64(&ique->readPosn); // if it didn't work, reload readPos
        }
        else if (AfxCasAtom64(&ique->readPosn, &readPosn, readPosn + 1)) 
            break; // if we're in the right spot, and we can successfully write an updated read position, break out and read the entry
    } while (1);

    // update the acceptable sequence value for this entry
    AfxCopy(dst, ique->entries[readPosn & queIdxMask].value, ique->unitSiz);
    AfxStoreAtom64(&ique->entries[readPosn & queIdxMask].seqIdx, readPosn + ique->queIdxMask + 1);
    return TRUE;
}

_AFXINL afxBool AfxIsInterlockedQueueEmpty(afxInterlockedQueue* ique)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ique);
    afxInt64 readPosn = AfxLoadAtom64(&readPosn);
    afxInt64 seqDelta = AfxLoadAtom64(&ique->entries[readPosn & ique->queIdxMask].seqIdx) - (readPosn + 1);
    return seqDelta < 0;
}
