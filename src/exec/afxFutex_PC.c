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

// Linux futexes can provide a more efficient, low-level synchronization primitive compared to pthread_rwlock_t or mutexes in some scenarios.
#define PREFER_LINUX_FUTEX TRUE

#ifdef _WIN32
#   define _CRT_SECURE_NO_WARNINGS 1
#   define WIN32_LEAN_AND_MEAN 1
#   include <sys/stat.h>
#   include <stdio.h>
#   include <Windows.h>
#   include <Shlwapi.h>
#   include <combaseapi.h>
#else
#   include <unistd.h>
#   ifdef PREFER_LINUX_FUTEX
#       include <stdatomic.h> // compiles on Clang. Yes, even if marked as non-existing in VS.
/*
    Lock State Representation
    We will use an atomic integer (lock) to represent the lock state. The value of this integer can represent:

    0: Unlocked (available)
    1: Locked for shared access (read lock)
    2: Locked exclusively (write lock)

    The futex() system call will allow us to perform atomic operations on this lock variable.
*/
    // Define the futex system call number for Linux
#       define futex(addr, op, val, timeout, addr2, val3) syscall(SYS_futex, addr, op, val, timeout, addr2, val3)
#   endif
#endif

#include "../impl/afxExecImplKit.h"

AFX_DEFINE_STRUCT_ALIGNED(AFX_PTR_ALIGNMENT, _afxFutex)
{
#ifdef _WIN32
    SRWLOCK srwl;
    afxUnit32 tidEx;
#else
#   ifdef PREFER_LINUX_FUTEX
    atomic_int lock; // Used to store the lock state
    pthread_t tidEx; // Thread ID of the thread holding the exclusive lock
#   else
    // POSIX
    pthread_rwlock_t rwlock; // Read-Write Lock
    pthread_t tidEx; // ID of the thread holding the exclusive (write) lock
#   endif
#endif
};

AFX_STATIC_ASSERT(sizeof(afxFutex) >= sizeof(_afxFutex), "");

_AFX afxBool AfxTryLockFutexShared(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);
#ifdef _WIN32
    afxUnit tid = AfxGetTid();
    //AfxReportMessage("%p try rdlocked by %u", ftx, tid);
    return TryAcquireSRWLockShared((PSRWLOCK)&ftx2->srwl);
#else
#   ifdef PREFER_LINUX_FUTEX
    // Check if the lock is available for a shared lock (i.e., no exclusive lock is held)
    int expected = 0;  // Looking for an unlocked state
    if (atomic_compare_exchange_strong(&ftx2->lock, &expected, 1)) {
        // If lock was available, it is now locked for shared access
        return 1;
}
    return 0;  // Failed to acquire the shared lock
#   else
    // Try to acquire the read lock (shared lock)
    return pthread_rwlock_tryrdlock(&ftx2->rwlock) == 0;
#   endif
#endif
}

_AFX afxBool AfxTryLockFutex(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);

#ifdef _WIN32
    afxBool rslt = TryAcquireSRWLockExclusive((PSRWLOCK)&ftx2->srwl);
    if (rslt)
    {
        ftx2->tidEx = AfxGetTid();
        //AfxReportMessage("%p try wdlocked by %u", ftx, ftx2->tidEx);
    }
    return rslt;
#else
#   ifdef PREFER_LINUX_FUTEX
    // Try to acquire an exclusive lock
    int expected = 0;
    if (atomic_compare_exchange_strong(&ftx2->lock, &expected, 2)) {
        ftx2->tidEx = pthread_self();  // Store the thread ID holding the exclusive lock
        return 1;
    }
    return 0;  // Failed to acquire the exclusive lock
#   else
    // Try to acquire the write lock (exclusive lock)
    if (pthread_rwlock_trywrlock(&ftx2->rwlock) == 0)
    {
        ftx2->tidEx = pthread_self();  // Store the thread ID that acquired the exclusive lock
        return 1;  // Lock acquired successfully
    }
    return 0;  // Failed to acquire the lock
#   endif
#endif
}

_AFX void AfxLockFutexShared(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);
#ifdef _WIN32
    AFX_ASSERT(AfxGetTid() != ftx2->tidEx);
    //AfxReportMessage("%p rdlocked by %u", ftx, AfxGetTid());
    AcquireSRWLockShared((PSRWLOCK)&ftx2->srwl);
#else
#   ifdef PREFER_LINUX_FUTEX
    while (1)
    {
        int expected = 0;
        if (atomic_compare_exchange_strong(&ftx2->lock, &expected, 1))
        {
            // Lock acquired for shared access
            return;
        }
        // If the lock is not available, wait on the futex to be notified
        futex(&ftx2->lock, FUTEX_WAIT, 1, NULL, NULL, 0);
    }
#   else
    // Acquire the read lock (shared lock)
    pthread_rwlock_rdlock(&ftx2->rwlock);
#   endif
#endif
}

_AFX void AfxLockFutex(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);
#ifdef _WIN32
    //AfxReportMessage("%p wdlocked by %u", ftx, AfxGetTid());
    AcquireSRWLockExclusive((PSRWLOCK)&ftx2->srwl);
    ftx2->tidEx = AfxGetTid();
#else
#   ifdef PREFER_LINUX_FUTEX
    while (1)
    {
        int expected = 0;
        if (atomic_compare_exchange_strong(&ftx2->lock, &expected, 2))
        {
            // Lock acquired for exclusive access
            ftx2->tidEx = pthread_self();  // Store the thread ID holding the exclusive lock
            return;
        }
        // If the lock is not available, wait on the futex to be notified
        futex(&ftx2->lock, FUTEX_WAIT, 0, NULL, NULL, 0);
    }
#   else
    // Acquire the write lock (exclusive lock)
    pthread_rwlock_wrlock(&ftx2->rwlock);
    ftx2->tidEx = pthread_self();  // Store the thread ID that acquired the exclusive lock
#   endif
#endif
}

_AFX void AfxUnlockFutexShared(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);
#ifdef _WIN32
    AFX_ASSERT(AfxGetTid() != ftx2->tidEx);
    //AfxReportMessage("%p rdunlocked by %u", ftx, AfxGetTid());
    ReleaseSRWLockShared((PSRWLOCK)&ftx2->srwl);
#else
#   ifdef PREFER_LINUX_FUTEX
    atomic_store(&ftx2->lock, 0);  // Unlock the shared lock

    // Wake up threads waiting for the lock
    futex(&ftx2->lock, FUTEX_WAKE, 1, NULL, NULL, 0);
#   else
    // Release the read lock (shared lock)
    pthread_rwlock_unlock(&ftx2->rwlock);
#   endif
#endif
}

_AFX void AfxUnlockFutex(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);
#ifdef _WIN32
    AFX_ASSERT(AfxGetTid() == ftx2->tidEx);
    //AfxReportMessage("%p wdunlocked by %u", ftx, AfxGetTid());
    ReleaseSRWLockExclusive((PSRWLOCK)&ftx2->srwl);
    ftx2->tidEx = 0;
#else
#   ifdef PREFER_LINUX_FUTEX
    assert(pthread_equal(pthread_self(), ftx2->tidEx));  // Ensure this thread holds the exclusive lock

    atomic_store(&ftx2->lock, 0);  // Unlock the exclusive lock

    // Wake up threads waiting for the lock
    futex(&ftx2->lock, FUTEX_WAKE, 1, NULL, NULL, 0);
    ftx2->tidEx = NULL;  // Reset the thread ID
#   else
    // Ensure that the current thread holds the exclusive lock
    AFX_ASSERT(pthread_equal(pthread_self(), ftx2->tidEx));
    // Release the write lock (exclusive lock)
    pthread_rwlock_unlock(&ftx2->rwlock);
    // Reset the thread ID holding the exclusive lock
    ftx2->tidEx = NIL;
#   endif
#endif
}

_AFX afxError AfxDismantleFutex(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    _afxFutex* ftx2 = (_afxFutex*)ftx;
    //AfxAssertType(ftx, afxFcc_SLCK);
#ifdef _WIN32
    //AfxAssignTypeFcc(ftx, 0);
    ftx2->srwl.Ptr = 0;
    ftx2->tidEx = 0;
#else
#   ifdef PREFER_LINUX_FUTEX
    atomic_store(&ftx2->lock, 0);  // Reset the lock state
    ftx2->tidEx = NULL;  // Reset the thread ID
    return AFX_ERR_NONE;
#   else
    // Destroy the futex by initializing the rwlock
    pthread_rwlock_destroy(&ftx2->rwlock);
    ftx2->tidEx = NIL;
#   endif
#endif
    return err;
}

_AFX afxError AfxDeployFutex(afxFutex* ftx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ftx);
    _afxFutex* ftx2 = (_afxFutex*)ftx;
#ifdef _WIN32
    //AfxAssignTypeFcc(ftx, afxFcc_SLCK);
    InitializeSRWLock((PSRWLOCK)&ftx2->srwl);
    ftx2->tidEx = 0;
    AfxLockFutexShared(ftx);
    AfxUnlockFutexShared(ftx);
#else
#   ifdef PREFER_LINUX_FUTEX
    atomic_store(&ftx2->lock, 0);  // Initialize the lock to unlocked state
    ftx2->tidEx = NULL;
    // Try acquiring and releasing a shared lock as a test
    AfxLockFutexShared(ftx);
    AfxUnlockFutexShared(ftx);
#   else
    // Initialize the read-write lock
    pthread_rwlock_init(&ftx2->rwlock, NIL);
    ftx2->tidEx = NIL;
    // Try acquiring and releasing the shared lock as a test
    AfxLockFutexShared(ftx);
    AfxUnlockFutexShared(ftx);
#   endif
#endif
    return err;
}
