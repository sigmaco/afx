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

#ifndef AFX_MUTEX_H
#define AFX_MUTEX_H

#include "qwadro/exec/afxAtomic.h"
#include "qwadro/exec/afxTime.h"

/**
    The afxMutex object provides access serialization between threads.

    The purpose of a afxMutex is to protect an object, data structure or 
    section of code so that only one thread can access it at a time (this is similar to the Java synchronized keyword).
    
    Recursive mutexes accepts lock() calls from the same thread any number of times. A plain mutex would deadlock in this situation.
    A recursive mutex is much more expensive to construct and operate on, so use a plain mutex whenever you can.
    Sometimes, one public function, however, calls another public function, and they both need to lock the same mutex.
    In this case, you have two options:
     - Factor the code that needs mutex protection into private functions, which assume that the mutex is held when they are called, 
       and lock a plain afxMutex in the public functions before you call the private implementation ones.
     - Or use a recursive mutex, so it doesn't matter that the first public function has already locked the mutex when the second one wishes to do so.

*/

AFX_DEFINE_STRUCT(afxMutex)
{
#ifdef AFX_ON_WINDOWS
#   ifdef AFX_ON_X86_64
    // must at least 56 bytes
    afxByte     data[AFX_ALIGN_SIZE(64, AFX_ATOMIC_ALIGNMENT)]; // 64 bytes
#else
    // must at least 36 bytes
    afxByte     data[AFX_ALIGN_SIZE(48, AFX_ATOMIC_ALIGNMENT)]; // 48 bytes
#   endif
#else
    afxByte     data[AFX_ALIGN_SIZE(48, AFX_ATOMIC_ALIGNMENT)]; // 48 bytes
#endif
};

typedef enum afxMutexType
{
    AFX_MTX_PLAIN,
    AFX_MTX_TIMED,
    AFX_MTX_RECURSIVE
} afxMutexType;

AFX afxError        AfxDeployMutex(afxMutex* mtx, afxMutexType type);
AFX void            AfxDismantleMutex(afxMutex* mtx);

AFX afxError        AfxLockMutex(afxMutex* mtx);
AFX afxError        AfxLockMutexTimed(afxMutex* mtx, afxTimeSpec const* ts);
AFX afxBool         AfxTryLockMutex(afxMutex* mtx);
AFX afxError        AfxUnlockMutex(afxMutex* mtx);

#endif//AFX_MUTEX_H
