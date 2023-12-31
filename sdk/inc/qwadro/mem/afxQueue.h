#ifndef AFX_QUEUE_H
#define AFX_QUEUE_H

#include "qwadro/core/afxCoreDefs.h"
#include "qwadro/core/afxDebug.h"

// A Queue is a linear structure that follows a particular order in which the operations are performed. 
// The order is First In First Out (FIFO). A good example of a queue is any queue of consumers for a resource where the consumer that came first is served first. 
// The difference between stacks and queues is in removing. In a stack we remove the item the most recently added; in a queue, we remove the item the least recently added.

// Thread safe se apenas um thread insere e apenas um outro remove.

AFX_DEFINE_STRUCT(afxQueue)
{
    _AFX_DBG_FCC;
    afxNat      unitSiz;
    afxNat      cap;
    afxNat      head;
    afxNat      tail;
    afxByte     *bytemap;
};

AFXINL afxError AfxAllocateQueue(afxQueue *que, afxNat unitSiz, afxNat cap);
AFXINL afxError AfxDeallocateQueue(afxQueue *que);

AFXINL afxError AfxPushQueueUnit(afxQueue *que, void const *data);
AFXINL void*    AfxPullNextQueueUnit(afxQueue const *que);
AFXINL afxError AfxPopNextQueue(afxQueue *que);

AFXINL afxBool  AfxQueueIsEmpty(afxQueue const *que);

#endif//AFX_QUEUE_H
