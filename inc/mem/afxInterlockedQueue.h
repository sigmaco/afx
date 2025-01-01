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

/**
    afxInterlockedQueue is a lock-less, thread-safe, multi-producer-multi-consumer queue data structure. 
    It is designed to hold copyable values.
*/

#ifndef AFX_INTERLOCKED_QUEUE_H
#define AFX_INTERLOCKED_QUEUE_H

#include "qwadro/inc/base/afxCoreDefs.h"
#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/exec/afxAtomic.h"

AFX_DEFINE_STRUCT(afxInterlockedQueue)
{
    afxAtom32*      entrySeqIdx;
    afxByte*        entryValue;
#if 0
    struct
    {
        afxAtom32   seqIdx; /// Global index of the queue entry in the sequence, to detect when we are at a valid read or write pos
        afxByte     value[]; /// Value actually contained in the queue
    }*              entries; // Buffer of entries.    
#endif
    afxUnit         unitSiz;
    afxInt32        queIdxMask; /// Mask to apply to the read/write position to clamp it to array bounds
    afxAtom32       readPosn; /// readIdx of where we are in the sequence
    afxAtom32       writePosn; /// writeIdx of where we are in the sequence
};

/// Inicializa a fila afxInterlockedQueue e aloca memória para o número especificado de entradas.
/// O número de entradas não é mutável após inicialização.
AFX afxError    AfxDeployInterlockedQueue(afxInterlockedQueue* ique, afxUnit unitSiz, afxUnit cap);

/// Libera a memória reservado para a fila e redefine o estado interno.
/// A fila deve estar vazia quando esta função for chamada.
AFX afxError    AfxDismantleInterlockedQueue(afxInterlockedQueue* ique);

/// Checa se há um valor disponível para ser pipocado
AFX afxBool     AfxIsInterlockedQueueEmpty(afxInterlockedQueue* ique);

/// Enfileira o valor provido. O valor será copiado para o buffer interno da fila.
/// Retorna TRUE se o enfileiramente foi desempenhado sucesivamente.
/// Retorna FALSE se o enfileiramento não pode ser realizado.
/// Isto pode acontecer se a fila estiver cheia, e algumas operações de desenfileiramento há de ocorrer.
AFX afxBool     AfxPushInterlockedQueue(afxInterlockedQueue* ique, void* src);

/// Desenfilera um valor da file especificada, copiando-o para para a variável argumentada.
/// Retorna TRUE se um valor foi sucessivamente desenfileirado, FALSE caso contrário (se FALSE, data não será escrito.
AFX afxBool     AfxPopInterlockedQueue(afxInterlockedQueue* ique, void* value);

#endif//AFX_INTERLOCKED_QUEUE_H
