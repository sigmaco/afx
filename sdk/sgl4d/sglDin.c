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

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/afxDrawInput.h"


_SGL afxError _SglDinFreeAllBuffers(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    //AfxEnterSlockExclusive(&din->scriptChainMutex);

    for (afxNat i = 0; i < din->scripts.cnt; i++)
    {
        afxDrawStream* pdscr = AfxGetArrayUnit(&din->scripts, i);

        if (*pdscr)
        {
            AfxAssertObjects(1, pdscr, afxFcc_DIOB);
            AfxReleaseObjects(1, (void**)pdscr);
            *pdscr = NIL;
        }
    }

    //AfxExitSlockExclusive(&din->scriptChainMutex);
    return err;
}

#if 0
_SGL afxError AfxRequestNextDrawQueue(afxDrawContext dctx, afxNat portIdx, afxTime timeout, afxNat *queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertRange(dctx->base.openPortCnt, portIdx, 1);
    AfxAssert(queIdx);
    //AfxAssertRange(dctx->ports[portIdx].queueCnt, queIdx, 1);

    (void)timeout;
    *queIdx = AFX_INVALID_INDEX;

    afxNat idx = dctx->base.openPorts[portIdx].lastReqQueIdx;
    afxTime time, t2;
    AfxGetTime(&time);

    for (;;)
    {
        idx = (idx + 1) % dctx->base.openPorts[portIdx].queueCnt;
        afxDrawBridge dxge = dctx->base.openPorts[portIdx].queues[idx];

        if (dxge)
        {
            AfxAssertObjects(1, &dxge, afxFcc_DXGE);

            //AfxAssert(!surf->swapchain.chain);
            *queIdx = idx;
            break;
        }

        if (timeout > AfxGetTime(&t2) - time)
        {
            AfxThrowError();
            *queIdx = AFX_INVALID_INDEX;
            break;
        }
    }

    if (!err)
        dctx->base.openPorts[portIdx].lastReqQueIdx = idx;

    return err;
}
#endif
