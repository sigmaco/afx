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
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxDrawContext AfxGetSemaphoreContext(afxSemaphore sem)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);
    afxDrawContext dctx = AfxGetLinker(&sem->dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxError AfxAcquireSemaphores(afxDrawContext dctx, afxNat cnt, afxSemaphore semaphores[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(semaphores);

    afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager* cls = AfxGetSemaphoreClass(ddev);
    AfxAssertClass(cls, afxFcc_SEM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)semaphores, (void const*[]) { ddev, dctx }))
        AfxThrowError();

    return err;
}
