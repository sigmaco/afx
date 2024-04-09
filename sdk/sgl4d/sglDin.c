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
#include "qwadro/draw/dev/afxDrawInput.h"


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
        afxDrawBridge ddge = dctx->base.openPorts[portIdx].queues[idx];

        if (ddge)
        {
            AfxAssertObjects(1, &ddge, afxFcc_DDGE);

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

_SGL afxError _SglDdevRelinkDinCb(afxDrawDevice ddev, afxDrawContext dctx, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(inputs);
    AfxAssert(cnt);

    AfxLockMutex(&ddev->idd->ioConMtx);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawInput din = inputs[i];
        AfxAssertObjects(1, &din, afxFcc_DIN);
        afxDrawContext from;

        if (AfxGetDrawInputContext(din, &from))
        {
            AfxAssertObjects(1, &from, afxFcc_DCTX);
            AfxAssert(ddev == AfxGetDrawContextDevice(from));

            afxDrawInput din2;
            AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din2, &from->base.inputs, dctx)
            {
                AfxAssertObjects(1, &din2, afxFcc_DIN);

                if (din2 == din)
                {
                    AfxPopLinkage(&din->dctx);
                    _SglDinFreeAllBuffers(din);
                    AfxReleaseObjects(1, (void*[]) { from });
                    break;
                }
            }
        }

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            AfxAssert(ddev == AfxGetDrawContextDevice(dctx));

            AfxReacquireObjects(1, (void*[]) { dctx });
            AfxPushLinkage(&din->dctx, &dctx->base.inputs);


            din->cachedClipCfg = dctx->base.clipCfg;
        }
    }

    AfxUnlockMutex(&ddev->idd->ioConMtx);

    return err;
}
