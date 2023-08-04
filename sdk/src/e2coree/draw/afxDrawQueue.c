/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_QUEUE_C
#define _AFX_SURFACE_C
#define _AFX_TEXTURE_C
#define _AFX_DRAW_SCRIPT_C
#include "_classified/afxDrawClassified.h"

#if 0

_AFX afxError _AfxDqueDiscardSubm(afxDrawQueue dque, afxDrawSubmission *subm)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    for (afxNat i = 0; i < subm->scriptCnt; ++i)
    {
        AfxAssertObject(subm->scripts[i], AFX_FCC_DSCR);

        if (subm->scripts[i]->state == AFX_DSCR_STATE_PENDING) // preserve state if modified by draw queue.
            subm->scripts[i]->state = AFX_DSCR_STATE_EXECUTABLE;

        AfxReleaseObject(&subm->scripts[i]->obj);
    }

    for (afxNat i = 0; i < subm->outputCnt; ++i)
    {
        AfxAssertObject(subm->outputs[i], AFX_FCC_DOUT);
        AfxAssertRange(AfxGetDrawOutputCapacity(subm->outputs[i]), subm->outputBufIdx[i], 1);
        afxSurface surf = AfxGetDrawOutputBuffer(subm->outputs[i], subm->outputBufIdx[i]);

        if (surf->state == AFX_DSCR_STATE_PENDING) // preserve state if modified by draw queue.
            surf->state = AFX_SURF_STATE_PRESENTABLE;

        AfxReleaseObject(&surf->tex.obj);
        AfxReleaseObject(&subm->outputs[i]->obj);
    }

    return err;
}

_AFX afxDrawSubmission* AfxGetDrawQueueSubmission(afxDrawQueue dque, afxNat submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(dque), submNo, 1);
    afxLinkage *lnk = AfxFindFirstLinkage(&dque->ordinalSubmChn, submNo);
    AfxAssert(lnk);
    return AfxContainerOf(lnk, afxDrawSubmission, ordinalSubmLnk);
}

_AFX afxNat AfxGetDrawQueueSubmissionCount(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    return AfxGetChainLength(&dque->ordinalSubmChn);
}

_AFX afxResult AfxEnumerateDrawQueueSubmissions(afxDrawQueue dque, afxNat first, afxNat cnt, afxDrawSubmission *subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(dque), first, cnt);
    AfxAssert(subm);
    afxResult hitcnt = 0;

    //AfxEnterSlockExclusive(&dque->submSlock);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxLinkage *lnk = AfxFindFirstLinkage(&dque->ordinalSubmChn, first + i);
        AfxAssert(lnk);
        subm[i] = AfxContainerOf(lnk, afxDrawSubmission, ordinalSubmLnk);
        ++hitcnt;
    }

    //AfxExitSlockExclusive(&dque->submSlock);

    return hitcnt;
}

_AFX afxError AfxRequestDrawQueueSubmission(afxDrawQueue dque, afxDrawQueue dque, afxDrawSubmissionType type, afxTime timeout, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    AfxAssertObject(dque, AFX_FCC_DQUE);
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
    AfxAssertObject(dque, AFX_FCC_DQUE);

    //AfxEnterSlockExclusive(&dque->submSlock);

    if (AfxDiscardDrawQueueSubmissions(dque, 0, AfxGetDrawQueueSubmissionCount(dque)))
        AfxThrowError();

    //AfxExitSlockExclusive(&dque->submSlock);

    return err;
}

_AFX afxError _AfxSubmitDrawQueueWorkloads(afxDrawQueue dque, afxDrawSubmissionSpecification const *spec, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
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
            AfxAssertObject(spec->scripts[i], AFX_FCC_DSCR);

            if (AfxGetDrawScriptState(spec->scripts[i]) != AFX_DSCR_STATE_EXECUTABLE)
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; ++j)
                {
                    spec->scripts[j]->state = AFX_DSCR_STATE_EXECUTABLE;
                }
                break;
            }

            spec->scripts[i]->state = AFX_DSCR_STATE_PENDQUEG;
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
            AfxAssertObject(spec->outputs[i], AFX_FCC_DOUT);
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

_AFX afxError AfxWaitForDrawQueueIdle(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    return dque->vmt->wait(dque);
};

_AFX afxDrawDriver AfxGetDrawQueueDriver(afxDrawQueue dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    afxDrawDriver ddrv = AfxObjectGetProvider(&dque->obj);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError AfxAcquireDrawQueues(afxDrawDriver ddrv, afxDrawQueueSpecification const *spec, afxNat cnt, afxDrawQueue dque[])
{
    AfxEntry("ddrv=%p,spec=%p,cnt=%u", ddrv, spec, cnt);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    for (afxNat i = 0; i < cnt; i++)
    {
        if (AfxClassAcquireObjects(AfxGetDrawQueueClass(ddrv), NIL, 1, spec, (afxObject**)&dque[i], AfxSpawnHint()))
        {
            AfxThrowError();

            for (afxNat j = 0; j < i; j++)
            {
                AfxAssertObject(dque[j], AFX_FCC_DQUE);
                AfxReleaseObject(&dque[j]->obj);
                dque[j] = NIL;
            }
            break;
        }
        else
        {
            AfxAssertObject(dque[i], AFX_FCC_DQUE);
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dque[i] = NIL;

    return err;
};

_AFX afxBool _AfxDqueEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawQueue dque = (void*)obj;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDqueEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawQueue dque = (void*)obj;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDqueDtor(afxDrawQueue dque)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dque, AFX_FCC_DQUE);

    if (dque->vmt->dtor && dque->vmt->dtor(dque))
        AfxThrowError();

    AfxAssert(!dque->idd);

    AfxReleaseSlock(&dque->pendingChainSlock);
    AfxReleaseSlock(&dque->reqLock);
    AfxReleaseArena(&dque->cmdArena);
    AfxReleaseSlock(&dque->arenaSlock);
    
    return err;
}

_AFX afxError _AfxDqueCtor(void *cache, afxNat idx, afxDrawQueue dque, afxDrawQueueSpecification const *specs)
{
    AfxEntry("dque=%p", dque);
    afxError err = AFX_ERR_NONE;
    (void)cache;
    AfxAssertObject(dque, AFX_FCC_DQUE);
    
    afxDrawQueueSpecification const *spec = &specs[idx];
    AfxAssert(spec);

    afxDrawDriver ddrv = AfxGetDrawQueueDriver(dque);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    dque->portIdx = spec->portIdx;
    dque->owner = spec->owner;
    AfxAssertObject(dque->owner, AFX_FCC_DCTX);
    afxMemory mem = AfxGetDrawContextMemory(dque->owner);
    
    dque->immediate = !!spec->immedate;

    AfxAcquireSlock(&dque->arenaSlock);
    AfxAcquireArena(mem, &dque->cmdArena, NIL, AfxSpawnHint());

    dque->locked = FALSE;

    AfxAcquireChain(&dque->pendingChain, dque);
    AfxAcquireSlock(&dque->pendingChainSlock);
    AfxAcquireSlock(&dque->reqLock);

    dque->vmt = NIL;
    dque->idd = NIL;

    if (ddrv->vmt->dque && ddrv->vmt->dque(dque, spec)) AfxThrowError();
    else
    {
        AfxAssert(dque->vmt);
    }

    if (err)
    {
        AfxReleaseSlock(&dque->pendingChainSlock);
        AfxReleaseSlock(&dque->reqLock);

        AfxReleaseArena(&dque->cmdArena);
    }

    return err;
}

_AFX afxClassSpecification const _AfxDqueClassSpec;

afxClassSpecification const _AfxDqueClassSpec =
{
    AFX_FCC_DQUE,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawQueue)),
    NIL,
    (void*)_AfxDqueCtor,
    (void*)_AfxDqueDtor,
    .event = _AfxDqueEventHandler,
    .eventFilter = _AfxDqueEventFilter,
    "afxDrawQueue",
    NIL
};
