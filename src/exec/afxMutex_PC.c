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
#include "../impl/afxExecImplKit.h"

AFX_STATIC_ASSERT(sizeof(mtx_t) <= sizeof(afxMutex), "");

_AFX void AfxDismantleMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    mtx_destroy((mtx_t*)mtx);
}

_AFX afxError AfxDeployMutex(afxMutex* mtx, afxMutexType type)
{
    afxError err = AFX_ERR_NONE;

    if (thrd_success != mtx_init((mtx_t*)mtx, (int)type))
        AfxThrowError();

    AfxLockMutex(mtx);
    AfxUnlockMutex(mtx);
    return err;
}

_AFX afxError AfxLockMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;

    if (thrd_success != mtx_lock((mtx_t*)mtx))
        AfxThrowError();

    return err;
}

_AFX afxError AfxLockMutexTimed(afxMutex* mtx, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;

    if (thrd_success != mtx_timedlock((mtx_t*)mtx, (struct timespec const*)ts))
        AfxThrowError();

    return err;
}

_AFX afxBool AfxTryLockMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    return (thrd_success == mtx_trylock((mtx_t*)mtx));
}

_AFX afxError AfxUnlockMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;

    if (thrd_success != mtx_unlock((mtx_t*)mtx))
        AfxThrowError();

    return err;
}
