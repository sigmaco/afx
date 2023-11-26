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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_QUEUE_C
#define _AFX_SURFACE_C
#define _AFX_RASTER_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/draw/afxDrawContext.h"

#if 0

_AFX afxError _AfxDqueDiscardSubm(afxDrawQueue dque, afxDrawSubmission *subm)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);

    for (afxNat i = 0; i < subm->scriptCnt; ++i)
    {
        AfxAssertObject(subm->scripts[i], afxFcc_DSCR);

        if (subm->scripts[i]->state == afxDrawScriptState_PENDING) // preserve state if modified by draw queue.
            subm->scripts[i]->state = afxDrawScriptState_EXECUTABLE;

        AfxReleaseObject(&subm->scripts[i]->obj);
    }

    for (afxNat i = 0; i < subm->outputCnt; ++i)
    {
        AfxAssertObject(subm->outputs[i], afxFcc_DOUT);
        AfxAssertRange(AfxGetDrawOutputCapacity(subm->outputs[i]), subm->outputBufIdx[i], 1);
        afxSurface surf = AfxGetDrawOutputBuffer(subm->outputs[i], subm->outputBufIdx[i]);

        if (surf->state == afxDrawScriptState_PENDING) // preserve state if modified by draw queue.
            surf->state = AFX_SURF_STATE_PRESENTABLE;

        AfxReleaseObject(&surf->tex.obj);
        AfxReleaseObject(&subm->outputs[i]->obj);
    }

    return err;
}

_AFX afxDrawSubmission* AfxGetDrawQueueSubmission(afxDrawQueue dque, afxNat submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(dque), submNo, 1);
    afxLinkage *lnk = AfxFindFirstLinkage(&dque->ordinalSubmChn, submNo);
    AfxAssert(lnk);
    return AFX_REBASE(lnk, afxDrawSubmission, ordinalSubmLnk);
}

_AFX afxNat AfxGetDrawQueueSubmissionCount(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);
    return AfxGetChainLength(&dque->ordinalSubmChn);
}

_AFX afxResult AfxEnumerateDrawQueueSubmissions(afxDrawQueue dque, afxNat first, afxNat cnt, afxDrawSubmission *subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(dque), first, cnt);
    AfxAssert(subm);
    afxResult hitcnt = 0;

    //AfxEnterSlockExclusive(&dque->submSlock);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxLinkage *lnk = AfxFindFirstLinkage(&dque->ordinalSubmChn, first + i);
        AfxAssert(lnk);
        subm[i] = AFX_REBASE(lnk, afxDrawSubmission, ordinalSubmLnk);
        ++hitcnt;
    }

    //AfxExitSlockExclusive(&dque->submSlock);

    return hitcnt;
}

_AFX afxError AfxRequestDrawQueueSubmission(afxDrawQueue dque, afxDrawQueue dque, afxDrawSubmissionType type, afxTime timeout, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);
    AfxAssertObject(dque, afxFcc_DQUE);
    AfxAssertRange(AFX_DRAW_SUBMISSION_TOTAL, type, 1);
    AfxAssert(submNo);
    (void)timeout;

    if (1)
    {
        //AfxEnterSlockExclusive(&dque->pullSlock);

        afxNat pullReqCnt = AfxGetChainLength(&dque->ordinalPullChn);

        if (!pullReqCnt)
        {
            //AfxEnterSlockExclusive(&dque->submSlock);

            AfxSwapChains(&dque->ordinalPullChn, &dque->ordinalSubmChn);

            for (afxNat i = 0; i < AFX_DRAW_SUBMISSION_TOTAL; i++)
                AfxSwapChains(&dque->extraordinalPullChn[i], &dque->extraordinalSubmChn[i]);

            //AfxExitSlockExclusive(&dque->submSlock);
        }

        afxNat i = 0;
        afxDrawSubmission *subm;
        AfxChainForEveryLinkageB2F(&dque->ordinalPullChn, afxDrawSubmission, ordinalSubmLnk, subm)
        {
            if (subm->type == type && !subm->dque)
            {
                AfxAssert(subm->submNo == i);
                *submNo = subm->submNo;
                subm->dque = dque;
                AfxGetTime(&subm->fetchTime);

                if (dque->vmt->pull(dque, subm))
                    AfxThrowError();

                AfxGetTime(&subm->complTime);

                if (_AfxDqueDiscardSubm(dque, subm))
                    AfxThrowError();
            }
            i++;
        }

        //AfxExitSlockExclusive(&dque->pullSlock);
    }
    return err;
}

_AFX afxError AfxDiscardDrawQueueSubmissions(afxDrawQueue dque, afxNat first, afxNat cnt)
{
    afxError err = AFX_ERR_NONE;

    //AfxEnterSlockExclusive(&dque->submSlock);

    afxNat i = 0;
    afxDrawSubmission *subm;
    AfxChainForEveryLinkageB2F(&dque->ordinalSubmChn, afxDrawSubmission, ordinalSubmLnk, subm)
    {
        if (subm->submNo == first + i)
        {
            _AfxDqueDiscardSubm(dque, subm);
        }

        i++;

        if (i >= cnt)
            break;
    }

    //AfxExitSlockExclusive(&dque->submSlock);

    return err;
}

_AFX afxError AfxDiscardAllDrawQueueSubmissions(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);

    //AfxEnterSlockExclusive(&dque->submSlock);

    if (AfxDiscardDrawQueueSubmissions(dque, 0, AfxGetDrawQueueSubmissionCount(dque)))
        AfxThrowError();

    //AfxExitSlockExclusive(&dque->submSlock);

    return err;
}

_AFX afxError _AfxSubmitDrawQueueWorkloads(afxDrawQueue dque, afxDrawSubmissionSpecification const *spec, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);
    AfxAssert(spec);

    //AfxEnterSlockExclusive(&dque->submSlock);

    afxDrawSubmission *dwrkScript;
    afxDrawSubmission *dwrkPresent;

    if (!spec->scriptCnt) dwrkScript = NIL;
    else if (!(dwrkScript = _AfxDqueRequestArenaSpace(dque, sizeof(*dwrkScript))))
        AfxThrowError();

    if (!spec->outputCnt) dwrkPresent = NIL;
    else if (!(dwrkPresent = _AfxDqueRequestArenaSpace(dque, sizeof(*dwrkPresent))))
        AfxThrowError();

    if (spec->scriptCnt)
    {
        dwrkScript->type = AFX_DRAW_SUBMISSION_SCRIPT;
        dwrkScript->scriptCnt = 0;
        dwrkScript->reqSubmNo = spec->reqSubmNo;
        dwrkScript->submNo = AfxGetChainLength(&dque->ordinalSubmChn);
        *submNo = dwrkScript->submNo;
        AfxPushLinkage(&dwrkScript->ordinalSubmLnk, &dque->ordinalSubmChn);
        AfxPushLinkage(&dwrkScript->extraordinalSubmLnk, &dque->extraordinalSubmChn[dwrkScript->type]);
        dwrkScript->dque = NIL;
        AfxGetTime(&dwrkScript->submTime);
        dwrkScript->complTime = 0;
        dwrkScript->fetchTime = 0;

        for (afxNat i = 0; i < spec->scriptCnt; ++i)
        {
            AfxAssertObject(spec->scripts[i], afxFcc_DSCR);

            if (AfxGetDrawScriptState(spec->scripts[i]) != afxDrawScriptState_EXECUTABLE)
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; ++j)
                {
                    spec->scripts[j]->state = afxDrawScriptState_EXECUTABLE;
                }
                break;
            }

            spec->scripts[i]->state = afxDrawScriptState_PENDQUEG;
            AfxObjectReacquire(&spec->scripts[i]->obj, &dque->obj, NIL, 0, NIL);
            dwrkScript->scripts[i] = spec->scripts[i];
            ++dwrkScript->scriptCnt;
        }
    }

    if (spec->outputCnt)
    {
        dwrkPresent->type = AFX_DRAW_SUBMISSION_PRESENT;
        dwrkPresent->outputCnt = 0;
        dwrkPresent->reqSubmNo = spec->scriptCnt ? dwrkScript->submNo : spec->reqSubmNo; // if has scripts, scripts requires spec passed reqSubmNo.
        dwrkPresent->submNo = AfxGetChainLength(&dque->ordinalSubmChn);

        if (0 == spec->scriptCnt)
            *submNo = dwrkPresent->submNo;

        AfxPushLinkage(&dwrkPresent->ordinalSubmLnk, &dque->ordinalSubmChn);
        AfxPushLinkage(&dwrkPresent->extraordinalSubmLnk, &dque->extraordinalSubmChn[dwrkPresent->type]);
        dwrkPresent->dque = NIL;
        AfxGetTime(&dwrkPresent->submTime);
        dwrkPresent->complTime = 0;
        dwrkPresent->fetchTime = 0;

        for (afxNat i = 0; i < spec->outputCnt; ++i)
        {
            AfxAssertObject(spec->outputs[i], afxFcc_DOUT);
            AfxAssertRange(AfxGetDrawOutputCapacity(spec->outputs[i]), spec->outputBufIdx[i], 1);
            afxSurface surf = AfxGetDrawOutputBuffer(spec->outputs[i], spec->outputBufIdx[i]);

            if (AfxGetSurfaceState(surf) > AFX_SURF_STATE_PRESENTABLE)
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; ++j)
                {
                    surf = AfxGetDrawOutputBuffer(spec->outputs[j], spec->outputBufIdx[j]);
                    surf->state = AFX_SURF_STATE_PRESENTABLE;
                }
                break;
            }

            surf->state = AFX_SURF_STATE_PENDQUEG;
            AfxObjectReacquire(&spec->outputs[i]->obj, &dque->obj, NIL, 0, NIL);
            AfxObjectReacquire(&surf->tex.obj, &dque->obj, NIL, 0, NIL);
            dwrkPresent->outputs[i] = spec->outputs[i];
            dwrkPresent->outputBufIdx[i] = spec->outputBufIdx[i];
            ++dwrkPresent->outputCnt;
        }
    }

    //AfxExitSlockExclusive(&dque->submSlock);
    return err;
}

#endif

#if 0
_AFX afxError AfxWaitForDrawQueueIdle(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;

    struct _afxDqueD *dque;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    AfxAssertType(&TheDrawSystem, afxFcc_DSYS);
    AfxExposeResidentObjects(&TheDrawSystem.queues, 1, &dque, (void**)&dque);
    AfxAssertType(dque, afxFcc_DQUE);

    return dque->vmt->wait(dque);
}


_AFX afxDrawDevice AfxGetDrawQueueDriver(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, afxFcc_DQUE);
    //afxDrawDevice ddev = AfxGetObjectProvider(&dque->obj);
    //AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return 0;// ddev;
}
#endif

#if 0
_AFX void _AfxObjDeallocDque(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;

    struct _afxDqueD *dque;
    AfxAssertObjects(1, &dque, afxFcc_DQUE);
    AfxAssertType(&TheDrawSystem, afxFcc_DSYS);
    AfxExposeResidentObjects(&TheDrawSystem.queues, 1, &dque, (void**)&dque);
    AfxAssertType(dque, afxFcc_DQUE);

    _AfxDqueDtor(dque, dque);
    AfxDeallocateResidentObjects(&TheDrawSystem.queues, 1, &dque);
}

_AFX void AfxReleaseDrawQueues(afxDrawContext dctx, afxNat cnt, afxDrawQueue dque[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, dque, afxFcc_DQUE);
    AfxReleaseObjects(cnt, dque, _AfxObjDeallocDque);
}
#endif

#if 0
_AFX afxError AfxAcquireDrawQueues(afxDrawContext dctx, afxNat cnt, afxDrawQueue dque[], afxDrawQueueSpecification const *spec[])
{
    AfxEntry("cnt=%u,dque=%p,spec=%p,", cnt, dque, spec);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (AfxAcquireObjects(&dctx->queues, cnt, (afxHandle*)dque, (void*[]) { (void*)spec }))
        AfxThrowError();

    return err;
}
#endif
