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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO FAST USER-SPACE MUTEX                                             //
//////////////////////////////////////////////////////////////////////////////

/*
    The Qwadro Futex API is a series of functions used for working with a futex-like mechanism that seems to be based on slim locks. 
    The functions provide locking and unlocking functionalities with support for both shared and exclusive locks.

    A futex (short for "fast user-space mutex") is a synchronization mechanism that allows user-space programs to efficiently wait 
    for conditions to change, without involving the kernel unless necessary. When threads are contending for the lock, the futex 
    system call can put them to sleep in the kernel and wake them up when it's appropriate, making it very efficient for certain 
    types of workloads.

    Slim locks enable the threads of a single process to access shared resources;
    they are optimized for speed and occupy very little memory.
    Slim locks cannot be shared across processes.

    Reader threads read data from a shared resource whereas writer threads write data to a shared resource.
    When multiple threads are reading and writing using a shared resource,
    exclusive locks such as a critical section or mutex can become a bottleneck
    if the reader threads run continuously but write operations are rare.
*/

#ifndef AFX_FUTEX_H
#define AFX_FUTEX_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/exec/afxMutex.h"
#include "qwadro/inc/base/afxFcc.h"

AFX_DEFINE_STRUCT_ALIGNED(AFX_PTR_ALIGNMENT, afxFutex)
{
    afxByte data[16];
};

/*
    The AfxDeployFutex() initializes a futex for subsequent use.
*/

AFX afxError    AfxDeployFutex(afxFutex* ftx);

/*
    The AfxDismantleFutex() function cleans up the the futex by resetting its internal state.
*/

AFX afxError    AfxDismantleFutex(afxFutex* ftx);

/*
    The AfxTryLockFutexShared() attempts to acquire a shared lock (read lock) on the futex.
    It returns TRUE if it successfully acquires the lock, otherwise it returns FALSE.
*/

AFX afxBool     AfxTryLockFutexShared(afxFutex* ftx);

/*
    The AfxTryLockFutex() function attempts to acquire an exclusive (write) lock on the futex. 
    If successful, it returns TRUE; otherwise, it returns FALSE.
*/

AFX afxBool     AfxTryLockFutex(afxFutex* ftx);

/*
    The AfxLockFutexShared() function waits until it successfully acquires a shared (read) lock on the futex.
*/

AFX void        AfxLockFutexShared(afxFutex* ftx);

/*
    The AfxLockFutex() function waits until it successfully acquires an exclusive (write) lock on the futex.
*/

AFX void        AfxLockFutex(afxFutex* ftx);

/*
    The AfxUnlockFutexShared() function releases the shared (read) lock held by the current thread.
*/

AFX void        AfxUnlockFutexShared(afxFutex* ftx);

/*
    The AfxUnlockFutex() function releases an exclusive (write) lock on the futex.
*/

AFX void        AfxUnlockFutex(afxFutex* ftx);



#endif//AFX_FUTEX_H
