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

#define _AFX_DRAW_C
#define _AFX_SEMAPHORE_C
#define _AFX_DRAW_CONTEXT_C
#include "qwadro/draw/pipe/afxSemaphore.h"
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxDrawContext AfxGetSemaphoreContext(afxSemaphore sem)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);
    afxDrawContext dctx = AfxGetObjectProvider(sem);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxError AfxAcquireSemaphores(afxDrawContext dctx, afxNat cnt, afxSemaphore semaphores[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(semaphores);

    afxManager* cls = AfxGetSemaphoreClass(dctx);
    AfxAssertClass(cls, afxFcc_SEM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)semaphores, (void const*[]) { dctx }))
        AfxThrowError();

    return err;
}
