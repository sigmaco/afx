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
#define _AFX_DRAW_BRIDGE_C
#include "qwadro/draw/afxDrawSystem.h"

#if 0

_AVX afxError _AfxDqueDiscardSubm(afxDrawBridge dxge, afxDrawSubmission *subm)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);

    for (afxNat i = 0; i < subm->scriptCnt; ++i)
    {
        AfxAssertObject(subm->scripts[i], afxFcc_DIOB);

        if (subm->scripts[i]->state == afxDrawStreamState_PENDING) // preserve state if modified by draw queue.
            subm->scripts[i]->state = afxDrawStreamState_EXECUTABLE;

        AfxReleaseObject(&subm->scripts[i]->obj);
    }

    for (afxNat i = 0; i < subm->outputCnt; ++i)
    {
        AfxAssertObject(subm->outputs[i], afxFcc_DOUT);
        AfxAssertRange(AfxCountDrawOutputBuffers(subm->outputs[i]), subm->outputBufIdx[i], 1);
        afxSurface surf = AfxEnumerateDrawOutputBuffers(subm->outputs[i], subm->outputBufIdx[i]);

        if (surf->state == afxDrawStreamState_PENDING) // preserve state if modified by draw queue.
            surf->state = AFX_SURF_STATE_PRESENTABLE;

        AfxReleaseObject(&surf->tex.obj);
        AfxReleaseObject(&subm->outputs[i]->obj);
    }

    return err;
}

_AVX afxDrawSubmission* AfxGetDrawQueueSubmission(afxDrawBridge dxge, afxNat submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(dxge), submNo, 1);
    afxLinkage *lnk = AfxFindFirstLinkage(&dxge->ordinalSubmChn, submNo);
    AfxAssert(lnk);
    return AFX_REBASE(lnk, afxDrawSubmission, ordinalSubmLnk);
}

_AVX afxNat AfxGetDrawQueueSubmissionCount(afxDrawBridge dxge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);
    return AfxGetChainLength(&dxge->ordinalSubmChn);
}

_AVX afxResult AfxEnumerateDrawQueueSubmissions(afxDrawBridge dxge, afxNat first, afxNat cnt, afxDrawSubmission *subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(dxge), first, cnt);
    AfxAssert(subm);
    afxResult hitcnt = 0;

    //AfxEnterSlockExclusive(&dxge->submSlock);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxLinkage *lnk = AfxFindFirstLinkage(&dxge->ordinalSubmChn, first + i);
        AfxAssert(lnk);
        subm[i] = AFX_REBASE(lnk, afxDrawSubmission, ordinalSubmLnk);
        ++hitcnt;
    }

    //AfxExitSlockExclusive(&dxge->submSlock);

    return hitcnt;
}

_AVX afxError AfxRequestDrawQueueSubmission(afxDrawBridge dxge, afxDrawBridge dxge, afxDrawSubmissionType type, afxTime timeout, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);
    AfxAssertObject(dxge, afxFcc_DXGE);
    AfxAssertRange(AFX_DRAW_SUBMISSION_TOTAL, type, 1);
    AfxAssert(submNo);
    (void)timeout;

    if (1)
    {
        //AfxEnterSlockExclusive(&dxge->pullSlock);

        afxNat pullReqCnt = AfxGetChainLength(&dxge->ordinalPullChn);

        if (!pullReqCnt)
        {
            //AfxEnterSlockExclusive(&dxge->submSlock);

            AfxSwapChains(&dxge->ordinalPullChn, &dxge->ordinalSubmChn);

            for (afxNat i = 0; i < AFX_DRAW_SUBMISSION_TOTAL; i++)
                AfxSwapChains(&dxge->extraordinalPullChn[i], &dxge->extraordinalSubmChn[i]);

            //AfxExitSlockExclusive(&dxge->submSlock);
        }

        afxNat i = 0;
        afxDrawSubmission *subm;
        AfxChainForEveryLinkageB2F(&dxge->ordinalPullChn, afxDrawSubmission, ordinalSubmLnk, subm)
        {
            if (subm->type == type && !subm->dxge)
            {
                AfxAssert(subm->submNo == i);
                *submNo = subm->submNo;
                subm->dxge = dxge;
                AfxGetTime(&subm->fetchTime);

                if (dxge->vmt->pull(dxge, subm))
                    AfxThrowError();

                AfxGetTime(&subm->complTime);

                if (_AfxDqueDiscardSubm(dxge, subm))
                    AfxThrowError();
            }
            i++;
        }

        //AfxExitSlockExclusive(&dxge->pullSlock);
    }
    return err;
}

_AVX afxError AfxDiscardDrawQueueSubmissions(afxDrawBridge dxge, afxNat first, afxNat cnt)
{
    afxError err = AFX_ERR_NONE;

    //AfxEnterSlockExclusive(&dxge->submSlock);

    afxNat i = 0;
    afxDrawSubmission *subm;
    AfxChainForEveryLinkageB2F(&dxge->ordinalSubmChn, afxDrawSubmission, ordinalSubmLnk, subm)
    {
        if (subm->submNo == first + i)
        {
            _AfxDqueDiscardSubm(dxge, subm);
        }

        i++;

        if (i >= cnt)
            break;
    }

    //AfxExitSlockExclusive(&dxge->submSlock);

    return err;
}

_AVX afxError AfxDiscardAllDrawQueueSubmissions(afxDrawBridge dxge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);

    //AfxEnterSlockExclusive(&dxge->submSlock);

    if (AfxDiscardDrawQueueSubmissions(dxge, 0, AfxGetDrawQueueSubmissionCount(dxge)))
        AfxThrowError();

    //AfxExitSlockExclusive(&dxge->submSlock);

    return err;
}

_AVX afxError _AfxSubmitDrawQueueWorkloads(afxDrawBridge dxge, afxDrawSubmissionSpecification const *spec, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dxge, afxFcc_DXGE);
    AfxAssert(spec);

    //AfxEnterSlockExclusive(&dxge->submSlock);

    afxDrawSubmission *dwrkScript;
    afxDrawSubmission *dwrkPresent;

    if (!spec->scriptCnt) dwrkScript = NIL;
    else if (!(dwrkScript = _AfxDqueRequestArenaSpace(dxge, sizeof(*dwrkScript))))
        AfxThrowError();

    if (!spec->outputCnt) dwrkPresent = NIL;
    else if (!(dwrkPresent = _AfxDqueRequestArenaSpace(dxge, sizeof(*dwrkPresent))))
        AfxThrowError();

    if (spec->scriptCnt)
    {
        dwrkScript->type = AFX_DRAW_SUBMISSION_SCRIPT;
        dwrkScript->scriptCnt = 0;
        dwrkScript->reqSubmNo = spec->reqSubmNo;
        dwrkScript->submNo = AfxGetChainLength(&dxge->ordinalSubmChn);
        *submNo = dwrkScript->submNo;
        AfxPushLinkage(&dwrkScript->ordinalSubmLnk, &dxge->ordinalSubmChn);
        AfxPushLinkage(&dwrkScript->extraordinalSubmLnk, &dxge->extraordinalSubmChn[dwrkScript->type]);
        dwrkScript->dxge = NIL;
        AfxGetTime(&dwrkScript->submTime);
        dwrkScript->complTime = 0;
        dwrkScript->fetchTime = 0;

        for (afxNat i = 0; i < spec->scriptCnt; ++i)
        {
            AfxAssertObject(spec->scripts[i], afxFcc_DIOB);

            if (AfxGetDrawStreamState(spec->scripts[i]) != afxDrawStreamState_EXECUTABLE)
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; ++j)
                {
                    spec->scripts[j]->state = afxDrawStreamState_EXECUTABLE;
                }
                break;
            }

            spec->scripts[i]->state = afxDrawStreamState_PENDQUEG;
            AfxObjectReacquire(&spec->scripts[i]->obj, &dxge->obj, NIL, 0, NIL);
            dwrkScript->scripts[i] = spec->scripts[i];
            ++dwrkScript->scriptCnt;
        }
    }

    if (spec->outputCnt)
    {
        dwrkPresent->type = AFX_DRAW_SUBMISSION_PRESENT;
        dwrkPresent->outputCnt = 0;
        dwrkPresent->reqSubmNo = spec->scriptCnt ? dwrkScript->submNo : spec->reqSubmNo; // if has scripts, scripts requires spec passed reqSubmNo.
        dwrkPresent->submNo = AfxGetChainLength(&dxge->ordinalSubmChn);

        if (0 == spec->scriptCnt)
            *submNo = dwrkPresent->submNo;

        AfxPushLinkage(&dwrkPresent->ordinalSubmLnk, &dxge->ordinalSubmChn);
        AfxPushLinkage(&dwrkPresent->extraordinalSubmLnk, &dxge->extraordinalSubmChn[dwrkPresent->type]);
        dwrkPresent->dxge = NIL;
        AfxGetTime(&dwrkPresent->submTime);
        dwrkPresent->complTime = 0;
        dwrkPresent->fetchTime = 0;

        for (afxNat i = 0; i < spec->outputCnt; ++i)
        {
            AfxAssertObject(spec->outputs[i], afxFcc_DOUT);
            AfxAssertRange(AfxCountDrawOutputBuffers(spec->outputs[i]), spec->outputBufIdx[i], 1);
            afxSurface surf = AfxEnumerateDrawOutputBuffers(spec->outputs[i], spec->outputBufIdx[i]);

            if (AfxGetSurfaceState(surf) > AFX_SURF_STATE_PRESENTABLE)
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; ++j)
                {
                    surf = AfxEnumerateDrawOutputBuffers(spec->outputs[j], spec->outputBufIdx[j]);
                    surf->state = AFX_SURF_STATE_PRESENTABLE;
                }
                break;
            }

            surf->state = AFX_SURF_STATE_PENDQUEG;
            AfxObjectReacquire(&spec->outputs[i]->obj, &dxge->obj, NIL, 0, NIL);
            AfxObjectReacquire(&surf->tex.obj, &dxge->obj, NIL, 0, NIL);
            dwrkPresent->outputs[i] = spec->outputs[i];
            dwrkPresent->outputBufIdx[i] = spec->outputBufIdx[i];
            ++dwrkPresent->outputCnt;
        }
    }

    //AfxExitSlockExclusive(&dxge->submSlock);
    return err;
}

#endif

_AVX afxError AfxWaitForIdleDrawQueue(afxDrawBridge dxge, afxNat queueIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dxge, afxFcc_DXGE);
    return dxge->waitCb(dxge, queueIdx);
}

_AVX afxDrawContext AfxGetDrawQueueContext(afxDrawBridge dxge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dxge, afxFcc_DXGE);
    afxDrawContext dctx = AfxGetLinker(&dxge->dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxDrawDevice AfxGetDrawQueueDevice(afxDrawBridge dxge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dxge, afxFcc_DXGE);
    afxDrawDevice ddev = AfxGetObjectProvider(dxge);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}
