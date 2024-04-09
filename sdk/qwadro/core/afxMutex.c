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

#include "../dep/tinycthread/tinycthread.h"
#include "qwadro/core/afxSystem.h"

_AFX void AfxCleanUpMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));
    mtx_destroy((mtx_t*)mtx);
}

_AFX afxError AfxSetUpMutex(afxMutex* mtx, afxMutexType type)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));

    if (thrd_success != mtx_init((mtx_t*)mtx, (int)type))
        AfxThrowError();

    AfxLockMutex(mtx);
    AfxUnlockMutex(mtx);
    return err;
}

_AFX afxError AfxLockMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));

    if (thrd_success != mtx_lock((mtx_t*)mtx))
        AfxThrowError();

    return err;
}

_AFX afxError AfxLockMutexTimed(afxMutex* mtx, afxTimeSpec const* ts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));

    if (thrd_success != mtx_timedlock((mtx_t*)mtx, (struct timespec const*)ts))
        AfxThrowError();

    return err;
}

_AFX afxBool AfxTryLockMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));
    return (thrd_success == mtx_trylock((mtx_t*)mtx));
}

_AFX afxError AfxUnlockMutex(afxMutex* mtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(sizeof(mtx_t) <= sizeof(afxMutex));

    if (thrd_success != mtx_unlock((mtx_t*)mtx))
        AfxThrowError();

    return err;
}
