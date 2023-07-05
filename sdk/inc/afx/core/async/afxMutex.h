/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MUTEX_H
#define AFX_MUTEX_H

#include "afx/core/time/afxTime.h"

// The afxMutex class provides access serialization between threads.

// The purpose of a QMutex is to protect an object, data structure or section of code so that only one thread can access it at a time (this is similar to the Java synchronized keyword).
// It is usually best to use a mutex with a QMutexLocker since this makes it easy to ensure that locking and unlocking are performed consistently.

// The QRecursiveMutex class is a mutex, like QMutex, with which it is API-compatible. It differs from QMutex by accepting lock() calls from the same thread any number of times. QMutex would deadlock in this situation.
// QRecursiveMutex is much more expensive to construct and operate on, so use a plain QMutex whenever you can.Sometimes, one public function, however, calls another public function, and they both need to lock the same mutex.In this case, you have two options :
// - Factor the code that needs mutex protection into private functions, which assume that the mutex is held when they are called, and lock a plain QMutex in the public functions before you call the private implementation ones.
// - Or use a recursive mutex, so it doesn't matter that the first public function has already locked the mutex when the second one wishes to do so.

AFX_DEFINE_STRUCT(afxMutex)
{
    afxByte         data[64];
};

typedef enum afxMutexType
{
    AFX_MTX_PLAIN,
    AFX_MTX_TIMED,
    AFX_MTX_RECURSIVE
} afxMutexType;

AFX afxResult       AfxMutexDeploy(afxMutex *mtx, afxMutexType type);
AFX void            AfxMutexDrop(afxMutex *mtx);

AFX afxResult       AfxMutexLock(afxMutex *mtx);
AFX afxResult       AfxMutexLockTimed(afxMutex *mtx, afxTimeSpec const* ts);
AFX afxResult       AfxMutexTryLock(afxMutex *mtx);
AFX afxResult       AfxMutexUnlock(afxMutex *mtx);

#endif//AFX_MUTEX_H