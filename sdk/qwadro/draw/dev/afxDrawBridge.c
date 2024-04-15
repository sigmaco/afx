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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_C
#define _AFX_DRAW_BRIDGE_C
#include "qwadro/draw/afxDrawSystem.h"

#if 0

_AVX afxError _AfxDqueDiscardSubm(afxDrawBridge ddge, afxDrawSubmission *subm)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);

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

_AVX afxDrawSubmission* AfxGetDrawQueueSubmission(afxDrawBridge ddge, afxNat submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(ddge), submNo, 1);
    afxLinkage *lnk = AfxFindFirstLinkage(&ddge->ordinalSubmChn, submNo);
    AfxAssert(lnk);
    return AFX_REBASE(lnk, afxDrawSubmission, ordinalSubmLnk);
}

_AVX afxNat AfxGetDrawQueueSubmissionCount(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);
    return AfxGetChainLength(&ddge->ordinalSubmChn);
}

_AVX afxResult AfxEnumerateDrawQueueSubmissions(afxDrawBridge ddge, afxNat first, afxNat cnt, afxDrawSubmission *subm[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);
    AfxAssertRange(AfxGetDrawQueueSubmissionCount(ddge), first, cnt);
    AfxAssert(subm);
    afxResult hitcnt = 0;

    //AfxEnterSlockExclusive(&ddge->submSlock);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxLinkage *lnk = AfxFindFirstLinkage(&ddge->ordinalSubmChn, first + i);
        AfxAssert(lnk);
        subm[i] = AFX_REBASE(lnk, afxDrawSubmission, ordinalSubmLnk);
        ++hitcnt;
    }

    //AfxExitSlockExclusive(&ddge->submSlock);

    return hitcnt;
}

_AVX afxError AfxRequestDrawQueueSubmission(afxDrawBridge ddge, afxDrawBridge ddge, afxDrawSubmissionType type, afxTime timeout, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);
    AfxAssertObject(ddge, afxFcc_DDGE);
    AfxAssertRange(AFX_DRAW_SUBMISSION_TOTAL, type, 1);
    AfxAssert(submNo);
    (void)timeout;

    if (1)
    {
        //AfxEnterSlockExclusive(&ddge->pullSlock);

        afxNat pullReqCnt = AfxGetChainLength(&ddge->ordinalPullChn);

        if (!pullReqCnt)
        {
            //AfxEnterSlockExclusive(&ddge->submSlock);

            AfxSwapChains(&ddge->ordinalPullChn, &ddge->ordinalSubmChn);

            for (afxNat i = 0; i < AFX_DRAW_SUBMISSION_TOTAL; i++)
                AfxSwapChains(&ddge->extraordinalPullChn[i], &ddge->extraordinalSubmChn[i]);

            //AfxExitSlockExclusive(&ddge->submSlock);
        }

        afxNat i = 0;
        afxDrawSubmission *subm;
        AfxChainForEveryLinkageB2F(&ddge->ordinalPullChn, afxDrawSubmission, ordinalSubmLnk, subm)
        {
            if (subm->type == type && !subm->ddge)
            {
                AfxAssert(subm->submNo == i);
                *submNo = subm->submNo;
                subm->ddge = ddge;
                AfxGetTime(&subm->fetchTime);

                if (ddge->vmt->pull(ddge, subm))
                    AfxThrowError();

                AfxGetTime(&subm->complTime);

                if (_AfxDqueDiscardSubm(ddge, subm))
                    AfxThrowError();
            }
            i++;
        }

        //AfxExitSlockExclusive(&ddge->pullSlock);
    }
    return err;
}

_AVX afxError AfxDiscardDrawQueueSubmissions(afxDrawBridge ddge, afxNat first, afxNat cnt)
{
    afxError err = AFX_ERR_NONE;

    //AfxEnterSlockExclusive(&ddge->submSlock);

    afxNat i = 0;
    afxDrawSubmission *subm;
    AfxChainForEveryLinkageB2F(&ddge->ordinalSubmChn, afxDrawSubmission, ordinalSubmLnk, subm)
    {
        if (subm->submNo == first + i)
        {
            _AfxDqueDiscardSubm(ddge, subm);
        }

        i++;

        if (i >= cnt)
            break;
    }

    //AfxExitSlockExclusive(&ddge->submSlock);

    return err;
}

_AVX afxError AfxDiscardAllDrawQueueSubmissions(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);

    //AfxEnterSlockExclusive(&ddge->submSlock);

    if (AfxDiscardDrawQueueSubmissions(ddge, 0, AfxGetDrawQueueSubmissionCount(ddge)))
        AfxThrowError();

    //AfxExitSlockExclusive(&ddge->submSlock);

    return err;
}

_AVX afxError _AfxSubmitDrawQueueWorkloads(afxDrawBridge ddge, afxDrawSubmissionSpecification const *spec, afxNat *submNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddge, afxFcc_DDGE);
    AfxAssert(spec);

    //AfxEnterSlockExclusive(&ddge->submSlock);

    afxDrawSubmission *dwrkScript;
    afxDrawSubmission *dwrkPresent;

    if (!spec->scriptCnt) dwrkScript = NIL;
    else if (!(dwrkScript = _AfxDqueRequestArenaSpace(ddge, sizeof(*dwrkScript))))
        AfxThrowError();

    if (!spec->outputCnt) dwrkPresent = NIL;
    else if (!(dwrkPresent = _AfxDqueRequestArenaSpace(ddge, sizeof(*dwrkPresent))))
        AfxThrowError();

    if (spec->scriptCnt)
    {
        dwrkScript->type = AFX_DRAW_SUBMISSION_SCRIPT;
        dwrkScript->scriptCnt = 0;
        dwrkScript->reqSubmNo = spec->reqSubmNo;
        dwrkScript->submNo = AfxGetChainLength(&ddge->ordinalSubmChn);
        *submNo = dwrkScript->submNo;
        AfxPushLinkage(&dwrkScript->ordinalSubmLnk, &ddge->ordinalSubmChn);
        AfxPushLinkage(&dwrkScript->extraordinalSubmLnk, &ddge->extraordinalSubmChn[dwrkScript->type]);
        dwrkScript->ddge = NIL;
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
            AfxObjectReacquire(&spec->scripts[i]->obj, &ddge->obj, NIL, 0, NIL);
            dwrkScript->scripts[i] = spec->scripts[i];
            ++dwrkScript->scriptCnt;
        }
    }

    if (spec->outputCnt)
    {
        dwrkPresent->type = AFX_DRAW_SUBMISSION_PRESENT;
        dwrkPresent->outputCnt = 0;
        dwrkPresent->reqSubmNo = spec->scriptCnt ? dwrkScript->submNo : spec->reqSubmNo; // if has scripts, scripts requires spec passed reqSubmNo.
        dwrkPresent->submNo = AfxGetChainLength(&ddge->ordinalSubmChn);

        if (0 == spec->scriptCnt)
            *submNo = dwrkPresent->submNo;

        AfxPushLinkage(&dwrkPresent->ordinalSubmLnk, &ddge->ordinalSubmChn);
        AfxPushLinkage(&dwrkPresent->extraordinalSubmLnk, &ddge->extraordinalSubmChn[dwrkPresent->type]);
        dwrkPresent->ddge = NIL;
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
            AfxObjectReacquire(&spec->outputs[i]->obj, &ddge->obj, NIL, 0, NIL);
            AfxObjectReacquire(&surf->tex.obj, &ddge->obj, NIL, 0, NIL);
            dwrkPresent->outputs[i] = spec->outputs[i];
            dwrkPresent->outputBufIdx[i] = spec->outputBufIdx[i];
            ++dwrkPresent->outputCnt;
        }
    }

    //AfxExitSlockExclusive(&ddge->submSlock);
    return err;
}

#endif

_AVX afxDrawDevice AfxGetDrawBridgeDevice(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxDrawDevice ddev = AfxGetObjectProvider(ddge);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxDrawContext AfxGetDrawBridgeContext(afxDrawBridge ddge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    afxDrawContext dctx = AfxGetLinker(&ddge->dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxError AfxWaitForIdleDrawQueue(afxDrawBridge ddge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    return ddge->waitCb(ddge, queIdx);
}

_AVX afxNat AfxEnqueueExecutionRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state.
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->executeCb(ddge, fenc, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AfxEnqueueTransferRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->transferCb(ddge, fenc, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AfxEnqueuePresentRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->presentCb(ddge, cnt, req)))
        AfxThrowError();

    return queIdx;
}

_AVX afxNat AfxEnqueueStampRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);
    AfxAssert(cnt);
    AfxAssert(req);

    afxNat queIdx;

    if (AFX_INVALID_INDEX == (queIdx = ddge->stampCb(ddge, cnt, req, origin, caption)))
        AfxThrowError();

    return queIdx;
}
