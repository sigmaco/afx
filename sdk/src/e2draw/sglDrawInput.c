/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "sgl.h"
#include "afx/afxQwadro.h"
#include "afx/draw/afxDrawInput.h"

_SGL afxError _SglDinFreeAllBuffers(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    //AfxEnterSlockExclusive(&din->scriptChainMutex);

    for (afxNat i = 0; i < din->base.scripts.cnt; i++)
    {
        afxDrawScript dscr = *(afxDrawScript*)AfxGetArrayUnit(&din->base.scripts, i);
        AfxAssertObjects(1, &dscr, afxFcc_DSCR);
        while (!AfxReleaseObjects(1, (void*[]) { dscr }));
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

_SGL afxError _SglDinSubmit(afxDrawInput din, afxNat cnt, afxDrawScript scripts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(scripts);

    afxDrawContext dctx;
    AfxGetDrawInputConnection(din, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawScript dscr = scripts[i];
        AfxAssertObjects(1, &dscr, afxFcc_DSCR);

        afxNat j = 0;
        afxDrawQueue dque;
        afxNat portIdx = dscr->base.portIdx;
        while (AfxEnumerateInstances(&dctx->base.openPorts[portIdx].queues, j, 1, (afxHandle*)&dque))
        {
            afxDrawSubmissionSpecification spec = { 0 };
            spec.scriptCnt = 1;
            spec.scripts = &dscr;
            afxNat submNo;

            if (_SglDqueVmtSubmitCb(dctx, dque, &spec, &submNo))
                break;

            ++j;
        }
    }
    return err;
}

_SGL afxError _SglDinPresent(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outputBufIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(outputs);
    AfxAssert(outputBufIdx);

    afxDrawContext dctx;
    AfxGetDrawInputConnection(din, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawOutput dout = outputs[i];
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        afxNat j = 0;
        afxDrawQueue dque;
        afxNat portIdx = 0;
        while (AfxEnumerateInstances(&dctx->base.openPorts[portIdx].queues, j, 1, (afxHandle*)&dque))
        {
            afxDrawSubmissionSpecification spec = { 0 };
            spec.outputCnt = 1;
            spec.outputs = outputs;
            spec.outBufIdx = outputBufIdx;
            afxNat submNo;

            if (_SglDqueVmtSubmitCb(dctx, dque, &spec, &submNo))
                break;

            ++j;
        }
    }
    return err;
}

_SGL afxError _SglDinProcCb(afxDrawInput din, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    if (din->base.prefetchEnabled)
    {
        //if (AfxTryEnterSlockExclusive(&dinD->prefetchSlock))
        {
            if (din->base.userPrefetchProc)
                din->base.userPrefetchProc(din, dthr);

            //AfxExitSlockExclusive(&dinD->prefetchSlock);
        }
    }
    return err;
}

_SGL _afxDinVmt const _SglDinVmt =
{
    NIL,
    _SglDinSubmit,
    _SglDinPresent
};

_SGL afxError _SglDinDtor(afxDrawInput din)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxDisconnectDrawInput(din);

    // avoid draw thread entrance

    AfxEnableDrawInputPrefetching(din, FALSE);
    //AfxDiscardAllDrawInputSubmissions(din);
    //AfxYieldThreading();
    //while (!AfxTryEnterSlockExclusive(&din->prefetchSlock)) AfxYieldThreading();

    //AfxReleaseSlock(&din->prefetchSlock);

    return err;
}

_SGL afxError _SglDinCtor(afxDrawInput din, afxCookie const *cookie)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxUriGetStringConst(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxNat devId = *(afxNat const *)(cookie->udd[0]);
    afxDrawInputConfig const *config = ((afxDrawInputConfig const *)cookie->udd[1]) + cookie->no;

    afxDrawDevice ddev = AfxGetObjectProvider(din);
    afxDrawIcd ddrv = AfxGetObjectProvider(ddev);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    
    AfxPushLinkage(&din->base.dctx, NIL);

    afxContext mem = AfxGetDrawSystemMemory(dsys);
    din->base.mem = mem;

    din->base.prefetchEnabled = FALSE; // must be explicitally enabled to avoid unready fetches.

    din->base.userPrefetchProc = config ? config->prefetch : NIL;

    AfxAcquireArray(&din->base.scripts, sizeof(afxDrawScript), 32, AfxSpawnHint());
    din->base.minScriptReserve = 2;

    din->base.vmt = NIL;
    din->base.procCb = NIL;

    if (config && config->udd)
    {
        din->base.udd = config->udd;
    }

    din->base.vmt = &_SglDinVmt;
    din->base.procCb = _SglDinProcCb;

    return err;
}

_SGL afxClassConfig _SglDinClsConfig =
{
    .fcc = afxFcc_DIN,
    .name = "Draw Input",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawInput)),
    .ctx = NIL,
    .ctor = (void*)_SglDinCtor,
    .dtor = (void*)_SglDinDtor
};
