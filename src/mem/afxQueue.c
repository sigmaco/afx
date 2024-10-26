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

#include "../dev/afxExecImplKit.h"

_AFXINL afxError AfxWrapQueue(afxQueue* que, afxUnit unitSiz, afxUnit cap, afxByte* bytemap)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(que);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(cap);
    //AfxAssignTypeFcc(que, afxFcc_QUE);
    que->unitSiz = unitSiz;
    que->cap = cap;
    que->head = (que->tail = 0);
    que->bytemap = bytemap;
    return err;
}

_AFXINL afxError AfxSetUpQueue(afxQueue* que, afxUnit unitSiz, afxUnit cap)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(que);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(cap);

    if (AfxWrapQueue(que, unitSiz, cap, NIL))
        AfxThrowError();

    return err;
}

_AFXINL afxError AfxCleanUpQueue(afxQueue* que)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(que, afxFcc_QUE);

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

_AFXINL afxError AfxPushQueue(afxQueue *que, void const* data)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(que, afxFcc_QUE);

    if (!que->bytemap && !(que->bytemap = AfxAllocate(que->cap, que->unitSiz, 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxCopy(&que->bytemap[que->unitSiz * que->tail], data, que->unitSiz);
        que->tail = (que->tail + 1) % que->cap;
    }
    return err;
}

_AFXINL void* AfxPeekQueue(afxQueue const* que)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(que, afxFcc_QUE);
    return (que->head == que->tail ? NIL : &que->bytemap[que->head * que->unitSiz]);
}

_AFXINL afxBool AfxIsQueueEmpty(afxQueue const* que)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(que, afxFcc_QUE);
    return (que->head == que->tail);
}

_AFXINL afxError AfxPopQueue(afxQueue *que)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(que, afxFcc_QUE);
    AFX_ASSERT(que->head != que->tail);
    que->head = (que->head + 1) % que->cap;
    return err;
}
