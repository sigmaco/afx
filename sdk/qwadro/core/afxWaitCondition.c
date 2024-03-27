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

#include "../dep/tinycthread/tinycthread.h"
#include "qwadro/core/afxWaitCondition.h"
#include "qwadro/core/afxDebug.h"

#if defined(_TTHREAD_WIN32_)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif

_AFX void AfxCleanUpWaitCondition(afxWaitCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(cnd_t) <= sizeof(afxWaitCondition));
    cnd_destroy((cnd_t*)cond);
}

_AFX afxError AfxSetUpWaitCondition(afxWaitCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(cnd_t) <= sizeof(afxWaitCondition));

    if (thrd_success != cnd_init((cnd_t*)cond))
        AfxThrowError();

    return err;
}

_AFX afxError AfxSignalSingleCondition(afxWaitCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(cnd_t) <= sizeof(afxWaitCondition));
    
    if (thrd_success != cnd_signal((cnd_t*)cond))
        AfxThrowError();

    return err;
}

_AFX afxError AfxSignalMultipleCondition(afxWaitCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(cnd_t) <= sizeof(afxWaitCondition));
    
    if (thrd_success != cnd_broadcast((cnd_t*)cond))
        AfxThrowError();

    return err;
}

_AFX afxError AfxWaitCondition(afxWaitCondition* cond, afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(cnd_t) <= sizeof(afxWaitCondition));
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));

    if (thrd_success != cnd_wait((cnd_t*)cond, (mtx_t*)mtx))
        AfxThrowError();

    return err;
}

_AFX afxError AfxWaitTimedCondition(afxWaitCondition* cond, afxMutex* mtx, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(cnd_t) <= sizeof(afxWaitCondition));
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));

    if (thrd_success != cnd_timedwait((cnd_t*)cond, (mtx_t*)mtx, (struct timespec const*)ts))
        AfxThrowError();

    return err;
}
