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
#define _AFX_FENCE_C
#define _AFX_DRAW_CONTEXT_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxBool AfxFenceIsSignaled(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    return fenc->signaled;
}

_AVX afxDrawContext AfxGetFenceContext(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    afxDrawContext dctx = AfxGetLinker(&fenc->dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxError AfxWaitForFences(afxBool waitAll, afxNat64 timeout, afxNat cnt, afxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, fences, afxFcc_FENC);
    afxFence first = fences[0];
    AfxAssertObjects(1, &first, afxFcc_FENC);
    afxDrawContext dctx = AfxGetFenceContext(first);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (dctx->waitFenc(waitAll, timeout, cnt, fences))
        AfxThrowError();

    return err;
}

_AVX afxError AfxResetFences(afxNat cnt, afxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, fences, afxFcc_FENC);
    afxFence first = fences[0];
    AfxAssertObjects(1, &first, afxFcc_FENC);
    afxDrawContext dctx = AfxGetFenceContext(first);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    
    if (dctx->resetFenc(cnt, fences))
        AfxThrowError();

    return err;
}

_AVX afxError AfxAcquireFences(afxDrawContext dctx, afxBool signaled, afxNat cnt, afxFence fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(fences);

    afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager* cls = AfxGetFenceClass(ddev);
    AfxAssertClass(cls, afxFcc_FENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fences, (void const*[]) { ddev, dctx, &signaled }))
        AfxThrowError();

    return err;
}
