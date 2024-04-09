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

#include "qwadro/core/afxSystem.h"

_AFXINL afxError AfxSetUpFifo(afxFifo* fifo, afxNat unitSiz, afxNat cap)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fifo);
    AfxAssert(unitSiz);
    AfxAssert(cap); /// The number of entries. Must be a power of two.

    /// Initializa a file afxFifo e aloca memória para o número especificado de entradas.
    /// O número de entradas não é mutável após inicialização.

    AfxAssert((cap & (cap - 1)) == 0);

    fifo->queIdxMask = cap - 1;
    fifo->writePosn = 0;
    fifo->readPosn = 0;
    fifo->unitSiz = unitSiz;

    if (!(fifo->entries = AfxAllocate(cap, sizeof(fifo->entries[0]) + unitSiz, 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxZero(fifo->entries, cap * (sizeof(fifo->entries[0]) + unitSiz));

        for (afxNat i = 0; i < cap; ++i)
        {
            //fifo->entries[i].value = TDEFAULT;
            AfxStoreAtom64(&fifo->entries[i].seqIdx, i);
        }
    }
    return err;
}

_AFXINL afxError AfxCleanUpFifo(afxFifo* fifo)
{
    afxError err = AFX_ERR_NONE;

    /// Libera a memória reservado para a fila e redefine o estado interno.
    /// A fila deve estar vazia quando esta função for chamada.

    if (fifo->entries)
    {
        AfxAssert(fifo->readPosn == fifo->writePosn);
        AfxDeallocate(fifo->entries);
        fifo->entries = NIL;
    }
    fifo->readPosn = 0;
    fifo->writePosn = 0;
    return err;
}

_AFXINL afxBool AfxEnqueueFifo(afxFifo* fifo, void* src)
{
    afxNat64 const queIdxMask = fifo->queIdxMask;
    afxInt64 writePosn = AfxLoadAtom64(&fifo->writePosn);
    afxInt64 seqDelta;

    do
    {
        if (0 != (seqDelta = AfxLoadAtom64(&fifo->entries[writePosn & queIdxMask].seqIdx) - writePosn)) // see where we are in the sequence, relative to where we can write data
        {
            if (seqDelta < 0) return FALSE; // we would have over-enqueued if we tried to write the position in. Return false; the user needs to decide how to handle things
            else writePosn = AfxLoadAtom64(&fifo->writePosn); // if it didn't work, reload writePos: someone else must have written to the sequence and we need to get caught up
        }
        else if (AfxCasAtom64(&fifo->writePosn, writePosn + 1, writePosn))
            break; // if we're in the right spot, and we can successfully write an updated write position, break out and write the handle into the queue
    } while (1);

    // advance the sequence by one so that it can be dequeued
    AfxCopy(fifo->entries[writePosn & queIdxMask].value, src, fifo->unitSiz);
    AfxStoreAtom64(&fifo->entries[writePosn & queIdxMask].seqIdx, writePosn + 1);
    return TRUE;
}

_AFXINL afxBool AfxDequeueFifo(afxFifo* fifo, void* dst)
{
    afxInt64 const queIdxMask = fifo->queIdxMask;
    afxInt64 readPosn = AfxLoadAtom64(&fifo->readPosn);
    afxInt64 seqDelta;

    do
    {
        if (0 != (seqDelta = AfxLoadAtom64(&fifo->entries[readPosn & queIdxMask].seqIdx) - (readPosn + 1))) // see where we are in the sequence relative to where we can write data
        {
            if (seqDelta < 0) return FALSE; // if an entry has yet to be written, bail out                
            else readPosn = AfxLoadAtom64(&fifo->readPosn); // if it didn't work, reload readPos
        }
        else if (AfxCasAtom64(&fifo->readPosn, readPosn + 1, readPosn)) 
            break; // if we're in the right spot, and we can successfully write an updated read position, break out and read the entry
    } while (1);

    // update the acceptable sequence value for this entry
    AfxCopy(dst, fifo->entries[readPosn & queIdxMask].value, fifo->unitSiz);
    AfxStoreAtom64(&fifo->entries[readPosn & queIdxMask].seqIdx, readPosn + fifo->queIdxMask + 1);
    return TRUE;
}

_AFXINL afxBool AfxFifoIsEmpty(afxFifo* fifo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fifo);
    afxInt64 readPosn = AfxLoadAtom64(&readPosn);
    afxInt64 seqDelta = AfxLoadAtom64(&fifo->entries[readPosn & fifo->queIdxMask].seqIdx) - (readPosn + 1);
    return seqDelta < 0;
}
