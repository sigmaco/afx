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

_AFXINL afxError AfxMakeQueue(afxQueue* que, afxUnit unitSiz, afxUnit cap, void* buf, afxUnit pop)
{
    afxError err = { 0 };
    AFX_ASSERT(que);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(cap);
    AFX_ASSERT(pop <= cap);
#ifdef _AFX_QUEUE_VALIDATION_ENABLED
    que->fcc = afxFcc_QUE;
#endif
    que->unitSiz = unitSiz;
    que->cap = cap;
    que->head = (que->tail = 0);
    que->bytemap = buf;
    que->alloced = !buf;
    return err;
}

_AFXINL afxError AfxDismantleQueue(afxQueue* que)
{
    afxError err = { 0 };
    AFX_ASSERT(que);
    que->head = (que->tail = 0);
    que->cap = 0;
    que->unitSiz = 0;
    
    if (que->bytemap && que->alloced)
    {
        AfxDeallocate((void**)&que->bytemap, AfxHere());
        que->bytemap = NIL;
        que->alloced = FALSE;
    }
    return err;
}

_AFXINL afxError AfxPushQueue(afxQueue* que, void const* data)
{
    afxError err = { 0 };
    AFX_ASSERT(que);
    AFX_ASSERT(data);

    if (!que->bytemap && AfxAllocate(que->cap * que->unitSiz, 0, AfxHere(), (void**)&que->bytemap)) AfxThrowError();
    else
    {
        AfxCopy(&que->bytemap[que->unitSiz * que->tail], data, que->unitSiz);
        que->tail = (que->tail + 1) % que->cap;
    }
    return err;
}

_AFXINL afxError AfxPopQueue(afxQueue *que)
{
    afxError err = { 0 };
    AFX_ASSERT(que);
    AFX_ASSERT(que->head != que->tail);
    que->head = (que->head + 1) % que->cap;
    return err;
}

_AFXINL void* AfxPeekQueue(afxQueue const* que)
{
    afxError err = { 0 };
    AFX_ASSERT(que);
    return (que->head == que->tail ? NIL : &que->bytemap[que->head * que->unitSiz]);
}

_AFXINL afxBool AfxIsQueueEmpty(afxQueue const* que)
{
    afxError err = { 0 };
    AFX_ASSERT(que);
    return (que->head == que->tail);
}

