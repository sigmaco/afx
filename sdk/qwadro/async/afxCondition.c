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

#include "qwadro/async/afxCondition.h"
#include "qwadro/core/afxDebug.h"
#include "../dep/tinycthread.h"
#include <stdlib.h>

#if defined(_TTHREAD_WIN32_)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif

_AFX afxResult AfxTakeWaitCondition(AfxCondition* cond)
{
    cnd_t* cnd = (cnd_t*)cond;
#if defined(_TTHREAD_WIN32_)
    cnd->mWaitersCount = 0;

    /* Init critical section */
    InitializeCriticalSection(&cnd->mWaitersCountLock);

    /* Init events */
    cnd->mEvents[_CONDITION_EVENT_ONE] = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (cnd->mEvents[_CONDITION_EVENT_ONE] == NULL)
    {
        cnd->mEvents[_CONDITION_EVENT_ALL] = NULL;
        return thrd_error;
    }
    cnd->mEvents[_CONDITION_EVENT_ALL] = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (cnd->mEvents[_CONDITION_EVENT_ALL] == NULL)
    {
        CloseHandle(cnd->mEvents[_CONDITION_EVENT_ONE]);
        cnd->mEvents[_CONDITION_EVENT_ONE] = NULL;
        return thrd_error;
    }

    return thrd_success;
#else
    return pthread_cond_init(cond, NULL) == 0 ? thrd_success : thrd_error;
#endif
}

_AFX void AfxReleaseWaitCondition(AfxCondition* cond)
{
    cnd_t* cnd = (cnd_t*)cond;
#if defined(_TTHREAD_WIN32_)
    if (cnd->mEvents[_CONDITION_EVENT_ONE] != NULL)
    {
        CloseHandle(cnd->mEvents[_CONDITION_EVENT_ONE]);
    }
    if (cnd->mEvents[_CONDITION_EVENT_ALL] != NULL)
    {
        CloseHandle(cnd->mEvents[_CONDITION_EVENT_ALL]);
    }
    DeleteCriticalSection(&cnd->mWaitersCountLock);
#else
    pthread_cond_destroy(cond);
#endif
}

_AFX afxResult AfxSignalSingleCondition(AfxCondition* cond)
{
    cnd_t* cnd = (cnd_t*)cond;
#if defined(_TTHREAD_WIN32_)
    int haveWaiters;

    /* Are there any waiters? */
    EnterCriticalSection(&cnd->mWaitersCountLock);
    haveWaiters = (cnd->mWaitersCount > 0);
    LeaveCriticalSection(&cnd->mWaitersCountLock);

    /* If we have any waiting threads, send them a signal */
    if (haveWaiters)
    {
        if (SetEvent(cnd->mEvents[_CONDITION_EVENT_ONE]) == 0)
        {
            return thrd_error;
        }
    }

    return thrd_success;
#else
    return pthread_cond_signal(cond) == 0 ? thrd_success : thrd_error;
#endif
}

_AFX afxResult AfxSignalMultipleCondition(AfxCondition* cond)
{
    cnd_t* cnd = (cnd_t*)cond;
#if defined(_TTHREAD_WIN32_)
    int haveWaiters;

    /* Are there any waiters? */
    EnterCriticalSection(&cnd->mWaitersCountLock);
    haveWaiters = (cnd->mWaitersCount > 0);
    LeaveCriticalSection(&cnd->mWaitersCountLock);

    /* If we have any waiting threads, send them a signal */
    if (haveWaiters)
    {
        if (SetEvent(cnd->mEvents[_CONDITION_EVENT_ALL]) == 0)
        {
            return thrd_error;
        }
    }

    return thrd_success;
#else
    return pthread_cond_broadcast(cond) == 0 ? thrd_success : thrd_error;
#endif
}

#if defined(_TTHREAD_WIN32_)
static int _cnd_timedwait_win32(cnd_t* cond, mtx_t* mtx, DWORD timeout)
{
    DWORD result;
    int lastWaiter;

    /* Increment number of waiters */
    EnterCriticalSection(&cond->mWaitersCountLock);
    ++cond->mWaitersCount;
    LeaveCriticalSection(&cond->mWaitersCountLock);

    /* Release the mutex while waiting for the condition (will decrease
       the number of waiters when done)... */
    AfxUnlockMutex((afxMutex*)mtx);

    /* Wait for either event to become signaled due to cnd_signal() or
       cnd_broadcast() being called */
    result = WaitForMultipleObjects(2, cond->mEvents, FALSE, timeout);
    if (result == WAIT_TIMEOUT)
    {
        /* The mutex is locked again before the function returns, even if an error occurred */
        AfxLockMutex((afxMutex*)mtx);
        return thrd_timedout;
    }
    else if (result == WAIT_FAILED)
    {
        /* The mutex is locked again before the function returns, even if an error occurred */
        AfxLockMutex((afxMutex*)mtx);
        return thrd_error;
    }

    /* Check if we are the last waiter */
    EnterCriticalSection(&cond->mWaitersCountLock);
    --cond->mWaitersCount;
    lastWaiter = (result == (WAIT_OBJECT_0 + _CONDITION_EVENT_ALL)) &&
        (cond->mWaitersCount == 0);
    LeaveCriticalSection(&cond->mWaitersCountLock);

    /* If we are the last waiter to be notified to stop waiting, reset the event */
    if (lastWaiter)
    {
        if (ResetEvent(cond->mEvents[_CONDITION_EVENT_ALL]) == 0)
        {
            /* The mutex is locked again before the function returns, even if an error occurred */
            AfxLockMutex((afxMutex*)mtx);
            return thrd_error;
        }
    }

    /* Re-acquire the mutex */
    AfxLockMutex((afxMutex*)mtx);

    return thrd_success;
}
#endif

_AFX afxResult AfxWaitCondition(AfxCondition* cond, afxMutex* mtx)
{
#if defined(_TTHREAD_WIN32_)
    return _cnd_timedwait_win32((cnd_t*)cond, (mtx_t*)mtx, INFINITE);
#else
    return pthread_cond_wait(cond, mtx) == 0 ? thrd_success : thrd_error;
#endif
}

_AFX afxResult AfxWaitTimedCondition(AfxCondition* cond, afxMutex* mtx, afxTimeSpec const* ts)
{
#if defined(_TTHREAD_WIN32_)
    afxTimeSpec now;
    if (timespec_get((struct timespec*)&now, TIME_UTC) == TIME_UTC)
    {
        unsigned long long nowInMilliseconds = now.sec * 1000 + now.nsec / 1000000;
        unsigned long long tsInMilliseconds = ts->sec * 1000 + ts->nsec / 1000000;
        DWORD delta = (tsInMilliseconds > nowInMilliseconds) ?
            (DWORD)(tsInMilliseconds - nowInMilliseconds) : 0;
        return _cnd_timedwait_win32((cnd_t*)cond, (mtx_t*)mtx, delta);
    }
    else
        return thrd_error;
#else
    int ret;
    ret = pthread_cond_timedwait(cond, mtx, ts);
    if (ret == ETIMEDOUT)
    {
        return thrd_timedout;
    }
    return ret == 0 ? thrd_success : thrd_error;
#endif
}
