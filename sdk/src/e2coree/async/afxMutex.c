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

#include "qwadro/async/afxMutex.h"
#include "qwadro/core/afxDebug.h"
#include "../dep/tinycthread.h"
#include <stdlib.h>

afxResult AfxTakeMutex(afxMutex* mtx, afxMutexType type)
{
    AfxEntry("mtx=%p,type=%p", mtx, type);
    afxError err = AFX_ERR_NONE;
    mtx_t* mtx2 = (mtx_t*)mtx;
#if defined(_TTHREAD_WIN32_)
    mtx2->mAlreadyLocked = FALSE;
    mtx2->mRecursive = type & mtx_recursive;
    mtx2->mTimed = type & mtx_timed;
    if (!mtx2->mTimed)
    {
        InitializeCriticalSection(&(mtx2->mHandle.cs));
    }
    else
    {
        mtx2->mHandle.mut = CreateMutex(NULL, FALSE, NULL);
        if (mtx2->mHandle.mut == NULL)
        {
            AfxThrowError();
            return thrd_error;
        }
    }
    AfxEcho("Created. (mtx)%p", mtx);
    return thrd_success;
#else
    int ret;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    if (type & mtx_recursive)
    {
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    }
    ret = pthread_mutex_init(mtx, &attr);
    pthread_mutexattr_destroy(&attr);
    return ret == 0 ? thrd_success : thrd_error;
#endif
}

void AfxCleanUpMutex(afxMutex* mtx)
{
    AfxEntry("mtx=%p", mtx);

    mtx_t* mtx2 = (mtx_t*)mtx;
#if defined(_TTHREAD_WIN32_)
    if (!mtx2->mTimed)
    {
        DeleteCriticalSection(&(mtx2->mHandle.cs));
    }
    else
    {
        CloseHandle(mtx2->mHandle.mut);
    }
#else
    pthread_mutex_destroy(mtx);
#endif
    AfxEcho("Destroyed. (mtx)%p", mtx);
}

afxResult AfxLockMutex(afxMutex* mtx)
{
    //AfxComment("$ lock ( mtx : %p )", mtx);

    mtx_t* mtx2 = (mtx_t*)mtx;
#if defined(_TTHREAD_WIN32_)
    if (!mtx2->mTimed)
    {
        EnterCriticalSection(&(mtx2->mHandle.cs));
    }
    else
    {
        switch (WaitForSingleObject(mtx2->mHandle.mut, INFINITE))
        {
        case WAIT_OBJECT_0:
            break;
        case WAIT_ABANDONED:
        default:
            return thrd_error;
        }
    }

    if (!mtx2->mRecursive)
    {
        while (mtx2->mAlreadyLocked) Sleep(1); /* Simulate deadlock... */
        mtx2->mAlreadyLocked = TRUE;
    }
    return thrd_success;
#else
    return pthread_mutex_lock(mtx) == 0 ? thrd_success : thrd_error;
#endif
}

afxResult AfxLockMutexTimed(afxMutex* mtx, afxTimeSpec const* ts)
{
    //AfxComment("$ lock-timed ( mtx : %p, ts : %p )", mtx, ts);

    mtx_t* mtx2 = (mtx_t*)mtx;
#if defined(_TTHREAD_WIN32_)
    afxTimeSpec current_ts;
    DWORD timeoutMs;

    if (!mtx2->mTimed)
    {
        return thrd_error;
    }

    timespec_get((struct timespec*)&current_ts, TIME_UTC);

    if ((current_ts.sec > ts->sec) || ((current_ts.sec == ts->sec) && (current_ts.nsec >= ts->nsec)))
    {
        timeoutMs = 0;
    }
    else
    {
        timeoutMs = (DWORD)(ts->sec - current_ts.sec) * (afxSize)1000;
        timeoutMs += (ts->nsec - current_ts.nsec) / (afxNat32)1000000;
        timeoutMs += 1;
    }

    /* TODO: the timeout for WaitForSingleObject doesn't include time
       while the computer is asleep. */
    switch (WaitForSingleObject(mtx2->mHandle.mut, timeoutMs))
    {
    case WAIT_OBJECT_0:
        break;
    case WAIT_TIMEOUT:
        return thrd_timedout;
    case WAIT_ABANDONED:
    default:
        return thrd_error;
    }

    if (!mtx2->mRecursive)
    {
        while (mtx2->mAlreadyLocked) Sleep(1); /* Simulate deadlock... */
        mtx2->mAlreadyLocked = TRUE;
    }

    return thrd_success;
#elif defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >= 200112L) && defined(_POSIX_THREADS) && (_POSIX_THREADS >= 200112L)
    switch (pthread_mutex_timedlock(mtx, ts)) {
    case 0:
        return thrd_success;
    case ETIMEDOUT:
        return thrd_timedout;
    default:
        return thrd_error;
    }
#else
    int rc;
    struct timespec cur, dur;

    /* Try to acquire the lock and, if we fail, sleep for 5ms. */
    while ((rc = pthread_mutex_trylock(mtx)) == EBUSY) {
        timespec_get(&cur, TIME_UTC);

        if ((cur.tv_sec > ts->tv_sec) || ((cur.tv_sec == ts->tv_sec) && (cur.tv_nsec >= ts->tv_nsec)))
        {
            break;
        }

        dur.tv_sec = ts->tv_sec - cur.tv_sec;
        dur.tv_nsec = ts->tv_nsec - cur.tv_nsec;
        if (dur.tv_nsec < 0)
        {
            dur.tv_sec--;
            dur.tv_nsec += 1000000000;
        }

        if ((dur.tv_sec != 0) || (dur.tv_nsec > 5000000))
        {
            dur.tv_sec = 0;
            dur.tv_nsec = 5000000;
        }

        nanosleep(&dur, NULL);
    }

    switch (rc) {
    case 0:
        return thrd_success;
    case ETIMEDOUT:
    case EBUSY:
        return thrd_timedout;
    default:
        return thrd_error;
    }
#endif
}

afxResult AfxTryLockMutex(afxMutex* mtx)
{
    //AfxComment("$ try-lock ( mtx : %p )", mtx);

    mtx_t* mtx2 = (mtx_t*)mtx;
#if defined(_TTHREAD_WIN32_)
    int ret;

    if (!mtx2->mTimed)
    {
        ret = TryEnterCriticalSection(&(mtx2->mHandle.cs)) ? thrd_success : thrd_busy;
    }
    else
    {
        ret = (WaitForSingleObject(mtx2->mHandle.mut, 0) == WAIT_OBJECT_0) ? thrd_success : thrd_busy;
    }

    if ((!mtx2->mRecursive) && (ret == thrd_success))
    {
        if (mtx2->mAlreadyLocked)
        {
            LeaveCriticalSection(&(mtx2->mHandle.cs));
            ret = thrd_busy;
        }
        else
        {
            mtx2->mAlreadyLocked = TRUE;
        }
    }
    return ret;
#else
    return (pthread_mutex_trylock(mtx) == 0) ? thrd_success : thrd_busy;
#endif
}

afxResult AfxUnlockMutex(afxMutex* mtx)
{
    //AfxComment("$ unlock ( mtx : %p )", mtx);

    mtx_t* mtx2 = (mtx_t*)mtx;
#if defined(_TTHREAD_WIN32_)
    mtx2->mAlreadyLocked = FALSE;
    if (!mtx2->mTimed)
    {
        LeaveCriticalSection(&(mtx2->mHandle.cs));
    }
    else
    {
        if (!ReleaseMutex(mtx2->mHandle.mut))
        {
            return thrd_error;
        }
    }
    return thrd_success;
#else
    return pthread_mutex_unlock(mtx) == 0 ? thrd_success : thrd_error;;
#endif
}
