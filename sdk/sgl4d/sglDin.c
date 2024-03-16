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

AFX afxClassConfig const _AfxCamClsConfig;
AFX afxClassConfig const _AfxVbufClsConfig;
//AFX afxClassConfig const _AfxIbufClsConfig;

_SGL afxError _SglDinFreeAllBuffers(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    //AfxEnterSlockExclusive(&din->scriptChainMutex);

    for (afxNat i = 0; i < din->base.scripts.cnt; i++)
    {
        afxDrawScript* pdscr = AfxGetArrayUnit(&din->base.scripts, i);

        if (*pdscr)
        {
            AfxAssertObjects(1, pdscr, afxFcc_DSCR);
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
        afxDrawQueue dque = dctx->base.openPorts[portIdx].queues[idx];

        if (dque)
        {
            AfxAssertObjects(1, &dque, afxFcc_DQUE);

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

_SGL afxError _SglDinDtor(afxDrawInput din)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxDisconnectDrawInput(din);

    // avoid draw thread entrance

    //AfxDiscardAllDrawInputSubmissions(din);
    //AfxYieldThreading();
    //while (!AfxTryEnterSlockExclusive(&din->prefetchSlock)) AfxYieldThreading();

    //AfxReleaseSlock(&din->prefetchSlock);

    _AfxUninstallChainedClasses(&din->base.classes);

    return err;
}

_SGL afxError _SglDinCtor(afxDrawInput din, afxCookie const *cookie)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawDevice ddev = cookie->udd[0];
    afxDrawInputConfig const *config = ((afxDrawInputConfig const *)cookie->udd[1]) + cookie->no;

    din->base.dctx = NIL;

    afxMmu mmu = AfxGetDrawSystemMmu();
    din->base.mmu = mmu;

    din->base.scripts;
    AfxAllocateArray(&din->base.scripts, 32, sizeof(afxDrawScript), NIL);
    din->base.minScriptReserve = 2;

    afxNat defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    din->base.minIdxBufSiz = defStreamSiz / 4;
    din->base.maxIdxBufSiz = 0;
    din->base.minVtxBufSiz = defStreamSiz - din->base.minIdxBufSiz;
    din->base.maxVtxBufSiz = 0;

    if (config && config->udd)
        din->base.udd = config->udd;

    din->base.procCb = config ? config->proc : NIL;
    
    afxChain *classes = &din->base.classes;
    AfxSetUpChain(classes, (void*)din);

    afxClassConfig tmpClsConf;

    tmpClsConf = _AfxVbufClsConfig;
    tmpClsConf.mmu = mmu;
    AfxMountClass(&din->base.vbuffers, NIL, classes, &tmpClsConf);

    tmpClsConf = _AfxCamClsConfig;
    tmpClsConf.mmu = mmu;
    AfxMountClass(&din->base.cameras, NIL, classes, &tmpClsConf);

    din->base.cachedClipCfg = ddev->clipCfg;

    return err;
}

_SGL afxClassConfig _SglDinClsConfig =
{
    .fcc = afxFcc_DIN,
    .name = "Draw Input",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawInput)),
    .mmu = NIL,
    .ctor = (void*)_SglDinCtor,
    .dtor = (void*)_SglDinDtor
};
