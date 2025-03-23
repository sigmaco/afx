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

//#include "../../dep/tinycthread.h"
//#include "../../dep/cthreads.h"
#include "../../dep/c11threads/c11threads.h"
#include "../impl/afxExecImplKit.h"

#if defined(_TTHREAD_WIN32_)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif

AFX_STATIC_ASSERT(sizeof(cnd_t) <= sizeof(afxCondition), "");

_AFX void AfxDismantleCondition(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    cnd_destroy((cnd_t*)cond);
}

_AFX afxError AfxDeployCondition(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;

    switch (cnd_init((cnd_t*)cond))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: err = afxError_INVALID_HANDLE; break;
    case thrd_nomem: err = afxError_OUT_OF_MEMORY; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxSignalCondition(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    
    switch (cnd_signal((cnd_t*)cond))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: err = afxError_INVALID_HANDLE; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxSignalCondition2(afxCondition* cond)
{
    afxError err = AFX_ERR_NONE;
    
    switch (cnd_broadcast((cnd_t*)cond))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: err = afxError_INVALID_HANDLE; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxWaitCondition(afxCondition* cond, afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;

    switch (cnd_wait((cnd_t*)cond, (mtx_t*)mtx))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: err = afxError_INVALID_HANDLE; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxWaitTimedCondition(afxCondition* cond, afxMutex* mtx, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;

    switch (cnd_timedwait((cnd_t*)cond, (mtx_t*)mtx, (struct timespec const*)ts))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_timedout: err = afxError_TIMEOUT; break;
    case thrd_busy: err = afxError_BUSY; break;
    case thrd_error: err = afxError_INVALID_HANDLE; break;
    case thrd_nomem: err = afxError_OUT_OF_MEMORY; break;
    default: AfxThrowError(); break;
    }

    return err;
}
