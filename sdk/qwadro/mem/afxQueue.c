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

AFXINL afxError AfxAllocateQueue(afxQueue* que, afxNat unitSiz, afxNat cap)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(que);
    AfxAssert(unitSiz);
    AfxAssert(cap);

    if (!(que->bytemap = AfxAllocate(cap, unitSiz, 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxAssignTypeFcc(que, afxFcc_QUE);
        que->unitSiz = unitSiz;
        que->cap = cap;
        que->head = (que->tail = 0);
    }
    return err;
}

AFXINL afxError AfxDeallocateQueue(afxQueue* que)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(que, afxFcc_QUE);

    que->head = (que->tail = 0);
    que->cap = 0;
    que->unitSiz = 0;

    if (que->bytemap)
    {
        AfxDeallocate(que->bytemap);
        que->bytemap = NIL;
    }
    return err;
}

AFXINL afxError AfxPushQueueUnit(afxQueue *que, void const *data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(que, afxFcc_QUE);
    AfxCopy(&que->bytemap[que->unitSiz * que->tail], data, que->unitSiz);
    que->tail = (que->tail + 1) % que->cap;
    return err;
}

AFXINL void* AfxPullNextQueueUnit(afxQueue const* que)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(que, afxFcc_QUE);
    return (que->head == que->tail ? NIL : &que->bytemap[que->head * que->unitSiz]);
}

AFXINL afxBool AfxQueueIsEmpty(afxQueue const* que)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(que, afxFcc_QUE);
    return (que->head == que->tail);
}

AFXINL afxError AfxPopNextQueue(afxQueue *que)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(que, afxFcc_QUE);
    AfxAssert(que->head != que->tail);
    que->head = (que->head + 1) % que->cap;
    return err;
}
