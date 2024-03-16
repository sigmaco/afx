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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
#   include <combaseapi.h>
#else
#   include <unistd.h>
#endif

#include "qwadro/async/afxSlock.h"
#include "qwadro/async/afxTxu.h"


AFX afxBool AfxTryEnterSlockShared(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);

    afxNat tid;
    AfxGetThreadingId(&tid);
    //AfxEcho("%p try rdlocked by %u", slck, tid);

    return TryAcquireSRWLockShared((PSRWLOCK)&slck->srwl);
}

AFX afxBool AfxTryEnterSlockExclusive(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);
    afxBool rslt = TryAcquireSRWLockExclusive((PSRWLOCK)&slck->srwl);

    if (rslt)
    {
        afxNat tid;
        AfxGetThreadingId(&tid);
        //AfxEcho("%p try wdlocked by %u", slck, tid);

        AfxGetThreadingId(&slck->tidEx);
    }
    return rslt;
}

AFX void AfxEnterSlockShared(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);
    afxNat tid;
    AfxGetThreadingId(&tid);
    //AfxEcho("%p rdlocked by %u", slck, tid);
    AcquireSRWLockShared((PSRWLOCK)&slck->srwl);
}

AFX void AfxEnterSlockExclusive(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);
    afxNat tid;
    AfxGetThreadingId(&tid);
    //AfxEcho("%p wdlocked by %u", slck, tid);
    AcquireSRWLockExclusive((PSRWLOCK)&slck->srwl);
    AfxGetThreadingId(&slck->tidEx);
}

AFX void AfxExitSlockShared(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);
    afxNat tid;
    AfxGetThreadingId(&tid);
    //AfxEcho("%p rdunlocked by %u", slck, tid);
    ReleaseSRWLockShared((PSRWLOCK)&slck->srwl);
}

AFX void AfxExitSlockExclusive(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);
    afxNat32 tid;
    AfxGetThreadingId(&tid);
    AfxAssert(slck->tidEx == tid);
    //AfxEcho("%p wdunlocked by %u", slck, tid);
    ReleaseSRWLockExclusive((PSRWLOCK)&slck->srwl);
    slck->tidEx = 0;
}

AFX afxError AfxReleaseSlock(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(slck, afxFcc_SLCK);
    AfxAssignTypeFcc(slck, 0);
    slck->srwl = 0;
    slck->tidEx = 0;
    return err;
}

AFX afxError AfxTakeSlock(afxSlock *slck)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(slck);
    AfxAssignTypeFcc(slck, afxFcc_SLCK);
    InitializeSRWLock((PSRWLOCK)&slck->srwl);
    slck->tidEx = 0;
    return err;
}
