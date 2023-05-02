#ifndef AFX_QUEUE_H
#define AFX_QUEUE_H

#include "afx/core/afxCoreDefs.h"
#include "afx/core/afxFcc.h"

AFX_DEFINE_STRUCT(afxQueue)
{
    afxFcc      fcc;
    afxNat      stride;
    afxNat      cap;
    afxNat      head;
    afxNat      tail;
    afxByte     *bytemap;
};

AFXINL afxError AfxQueueDeploy(afxQueue *que, afxNat stride, afxNat cap);
AFXINL afxError AfxQueueDrop(afxQueue *que);

AFXINL afxError AfxQueuePush(afxQueue *que, void const *data);
AFXINL void*    AfxQueuePull(afxQueue const *que);
AFXINL afxError AfxQueuePop(afxQueue *que);

AFXINL afxBool  AfxQueueIsEmpty(afxQueue const *que);

#endif//AFX_QUEUE_H