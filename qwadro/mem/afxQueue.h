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
    A queue is a linear structure that follows a particular order in which the operations are performed. 
    The order is First In First Out (FIFO). 
    A good example of a queue is any queue of consumers for a resource where the consumer that came first is served first. 
    The difference between stacks and queues is in removing. In a stack we remove the item the most recently added; in a queue, 
    we remove the item the least recently added.

    Thread safe se apenas um thread insere e apenas um outro remove.
*/

#ifndef AFX_QUEUE_H
#define AFX_QUEUE_H

#include "qwadro/base/afxCoreDefs.h"
#include "qwadro/exec/afxDebug.h"

#if (defined _AFX_DEBUG) && !(defined(_AFX_QUEUE_VALIDATION_ENABLED))
#   define _AFX_QUEUE_VALIDATION_ENABLED TRUE
#endif

AFX_DEFINE_STRUCT(afxQueue)
{
#ifdef _AFX_QUEUE_VALIDATION_ENABLED
    afxFcc      fcc; // afxFcc_QUE;
#endif
    afxUnit     head;
    afxUnit     tail;
    afxUnit     cap;
    afxUnit     unitSiz;
    afxByte*    bytemap;
    afxBool     alloced;
};

AFXINL afxError AfxDismantleQueue(afxQueue* que);
AFXINL afxError AfxMakeQueue(afxQueue* que, afxUnit unitSiz, afxUnit cap, void* buf, afxUnit pop);

/// Remove, also called deque or pop, this removes the oldest element from the queue.
AFXINL afxError AfxPopQueue(afxQueue* que);

/// Add, also called queue or push, this adds elements to the queue.
AFXINL afxError AfxPushQueue(afxQueue* que, void const *data);

/// Show the next element in the queue scheduled for removal.
AFXINL void*    AfxPeekQueue(afxQueue const* que);

AFXINL afxBool  AfxIsQueueEmpty(afxQueue const* que);

#endif//AFX_QUEUE_H
