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

#include "../dep/tinycthread.h"
#include "../dev/afxExecImplKit.h"

#if defined(_TTHREAD_WIN32_)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif

AFX_STATIC_ASSERT(sizeof(cnd_t) <= sizeof(afxCondition), "");

_AFX void AfxCleanUpCondition(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    cnd_destroy((cnd_t*)cond);
}

_AFX afxError AfxSetUpCondition(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;

    if (thrd_success != cnd_init((cnd_t*)cond))
        AfxThrowError();

    return err;
}

_AFX afxError AfxSignalCondition(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    
    if (thrd_success != cnd_signal((cnd_t*)cond))
        AfxThrowError();

    return err;
}

_AFX afxError AfxSignalCondition2(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    
    if (thrd_success != cnd_broadcast((cnd_t*)cond))
        AfxThrowError();

    return err;
}

_AFX afxError AfxWaitCondition(afxCondition* cond, afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;

    if (thrd_success != cnd_wait((cnd_t*)cond, (mtx_t*)mtx))
        AfxThrowError();

    return err;
}

_AFX afxError AfxWaitTimedCondition(afxCondition* cond, afxMutex* mtx, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;

    err = (thrd_success != cnd_timedwait((cnd_t*)cond, (mtx_t*)mtx, (struct timespec const*)ts));
        //AfxThrowError();

    return err;
}
