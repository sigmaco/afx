#ifndef AFX_FIFO_H
#define AFX_FIFO_H

#include "qwadro/core/afxCoreDefs.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxAtomic.h"

/// afxFifo is a lock-less, thread-safe, multi-producer-multi-consumer queue data structure. It is designed to hold copyable values.

AFX_DEFINE_STRUCT(afxFifo)
{
    struct
    {
        afxAtom64   seqIdx; /// Global index of the queue entry in the sequence, to detect when we are at a valid read or write pos
        afxByte     value[]; /// Value actually contained in the queue
    }*              entries; // Buffer of entries.    
    afxNat          unitSiz;
    afxInt64        queIdxMask; /// Mask to apply to the read/write position to clamp it to array bounds
    afxAtom64       readPosn; /// readIndex of where we are in the sequence
    afxAtom64       writePosn; /// writeIndex of where we are in the sequence
};

/// Initializa a file afxFifo e aloca memória para o número especificado de entradas.
/// O número de entradas não é mutável após inicialização.
AFXINL afxError     AfxSetUpFifo(afxFifo* fifo, afxNat unitSiz, afxNat cap);

/// Libera a memória reservado para a fila e redefine o estado interno.
/// A fila deve estar vazia quando esta função for chamada.
AFXINL afxError     AfxCleanUpFifo(afxFifo* fifo);

/// Enfileira o valor provido. O valor será copiado para o buffer interno da fila.
/// Retorna TRUE se o enfileiramente foi desempenhado sucesivamente.
/// Retorna FALSE se o enfileiramento não pode ser realizado.
/// Isto pode acontecer se a fila estiver cheia, e algumas operações de desenfileiramento há de ocorrer.
AFXINL afxBool      AfxEnqueueFifo(afxFifo* fifo, void* src);

/// Desenfilera um valor da file especificada, copiando-o para para a variável argumentada.
/// Retorna TRUE se um valor foi sucessivamente desenfileirado, FALSE caso contrário (se FALSE, data não será escrito.
AFXINL afxBool      AfxDequeueFifo(afxFifo* fifo, void* value);

/// Checa se há um valor disponível para ser pipocado
AFXINL afxBool      AfxFifoIsEmpty(afxFifo* fifo);

#endif//AFX_FIFO_H
