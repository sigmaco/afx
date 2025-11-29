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

#define USE_TINYCTHREAD

#ifdef USE_TINYCTHREAD
#include "../../dep/tinycthread/tinycthread.h"
#else
#include "../../dep/c11threads/c11threads.h"
#endif
#include "afxSystemDDK.h"

#if defined(_TTHREAD_WIN32_)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif

AFX_STATIC_ASSERT(sizeof(afxCondition) >= sizeof(cnd_t), "");

_AFX void AfxDismantleCondition(afxCondition* cond)
{
    afxError err = { 0 };
    cnd_destroy((cnd_t*)cond);
}

_AFX afxError AfxDeployCondition(afxCondition* cond)
{
    afxError err = { 0 };

    switch (cnd_init((cnd_t*)cond))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: AfxThrowError(); err = afxError_INVALID_HANDLE; break;
    case thrd_nomem: AfxThrowError(); err = afxError_OUT_OF_MEMORY; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxSignalCondition(afxCondition* cond)
{
    afxError err = { 0 };
    
    switch (cnd_signal((cnd_t*)cond))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: AfxThrowError(); err = afxError_INVALID_HANDLE; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxSignalCondition2(afxCondition* cond)
{
    afxError err = { 0 };
    
    switch (cnd_broadcast((cnd_t*)cond))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: AfxThrowError(); err = afxError_INVALID_HANDLE; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxWaitCondition(afxCondition* cond, afxMutex* mtx)
{
    afxError err = { 0 };

    switch (cnd_wait((cnd_t*)cond, (mtx_t*)mtx))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_error: AfxThrowError(); err = afxError_INVALID_HANDLE; break;
    default: AfxThrowError(); break;
    }

    return err;
}

_AFX afxError AfxWaitTimedCondition(afxCondition* cond, afxMutex* mtx, afxTimeSpec const* ts)
{
    afxError err = { 0 };

    switch (cnd_timedwait((cnd_t*)cond, (mtx_t*)mtx, (struct timespec const*)ts))
    {
    case thrd_success: err = afxError_SUCCESS; break;
    case thrd_timedout: err = afxError_TIMEOUT; break;
    case thrd_busy: err = afxError_BUSY; break;
    case thrd_error: AfxThrowError(); err = afxError_INVALID_HANDLE; break;
    case thrd_nomem: AfxThrowError(); err = afxError_OUT_OF_MEMORY; break;
    default: AfxThrowError(); break;
    }

    return err;
}
