#include "sgl.h"
#include "afx/afxQwadro.h"

_SGL void _AfxDinDscrHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = NIL;
    afxDrawInput din = (void*)obj;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssertObject(ev->obj, AFX_FCC_DSCR);
    //AfxAssertConnection(&dout->din);
    //AfxAssert(AfxConnectionGetObject(&dout->din) == ev->obj);

    if (ev->type == AFX_EVENT_OBJ_DESTROYED)
    {
        AfxError("Draw script %p attached to draw input device %p was destroyed.", ev->obj, obj);
        while (0 < AfxObjectRelease(&din->obj));
    }
}

_SGL afxError _AfxDrawInputProcess(afxDrawInput din, afxNat queueIdx)
{
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);

    if (din->prefetchThreadEnabled[queueIdx])
    {
        if (AfxTransistorTryEnterExclusive(&din->prefetchLock))
        {
            if (din->prefetchProc)
                din->prefetchProc(din, queueIdx, din->udd);

            AfxTransistorExitExclusive(&din->prefetchLock);
        }
    }
    return err;
}

_SGL afxError _AfxDrawInputAffinePrefetchThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[])
{
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
        din->prefetchThreadEnabled[base + i] = enabled[i];

    return err;
}

_SGL afxError _AfxDrawInputAffineStreamingThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[])
{
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
        din->streamingThreadEnabled[base + i] = enabled[i];

    return err;
}

_SGL afxError _AfxDrawInputAffineSubmissionThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[])
{
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
        din->submissionThreadEnabled[base + i] = enabled[i];

    return err;
}

_SGL afxError _AfxDrawInputAffinePresentationThreads(afxDrawInput din, afxNat base, afxNat cnt, afxNat const enabled[])
{
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
        din->presentationThreadEnabled[base + i] = enabled[i];

    return err;
}

_SGL afxError _AfxDrawInputEnqueueSubmission(afxDrawInput din, afxNat scriptCnt, afxDrawScript scripts[])
{
    afxError err = NIL;

    afxDrawWork dwrk;
    AfxGetTime(&dwrk.timestamp);
    dwrk.type = AFX_DRAW_WORK_TYPE_SUBMISSION;
    dwrk.submission.scriptCnt = 0;

    for (afxNat i = 0; i < scriptCnt; ++i)
    {
        AfxAssertObject(scripts[i], AFX_FCC_DSCR);

        if (scripts[i]->state != AFX_DSCR_STATE_EXECUTABLE)
        {
            AfxThrowError();
            break;
        }

        scripts[i]->state = AFX_DSCR_STATE_PENDING;
        //AfxObjectReacquire(&scripts[i]->obj, &din->obj, NIL, 0, NIL);
        dwrk.submission.scripts[i] = scripts[i];
        ++dwrk.submission.scriptCnt;
    }

    AfxTransistorEnterExclusive(&din->submissionLock);

    if (AfxQueuePush(&din->submissionQueue, &dwrk))
        AfxThrowError();

    AfxTransistorExitExclusive(&din->submissionLock);

    return err;
}

_SGL afxError _AfxDrawInputEnqueuePresentation(afxDrawInput din, afxNat outputCnt, afxDrawOutput outputs[], afxNat outputBufIdx[])
{
    afxError err = NIL;

    afxDrawWork dwrk;
    AfxGetTime(&dwrk.timestamp);
    dwrk.type = AFX_DRAW_WORK_TYPE_PRESENTATION;
    dwrk.presentation.outputCnt = 0;

    for (afxNat i = 0; i < outputCnt; ++i)
    {
        AfxAssertObject(outputs[i], AFX_FCC_DOUT);

        if (outputs[i]->buffers[outputBufIdx[i]]->state == AFX_SURF_STATE_PENDING)
        {
            AfxThrowError();
            break;
        }
        //AfxObjectReacquire(&outputs[i]->obj, &din->obj, NIL, 0, NIL);
        dwrk.presentation.outputs[i] = outputs[i];
        dwrk.presentation.outputBufIdx[i] = outputBufIdx[i];
        ++dwrk.presentation.outputCnt;
    }

    AfxTransistorEnterExclusive(&din->presentationLock);

    if (AfxQueuePush(&din->presentationQueue, &dwrk))
        AfxThrowError();

    AfxTransistorExitExclusive(&din->presentationLock);
    
    return err;
}

_SGL afxError _AfxDrawInputEnqueueStreaming(afxDrawInput din, afxNat resCnt, afxObject *resources[])
{
    afxError err = NIL;

    afxDrawWork dwrk;
    AfxGetTime(&dwrk.timestamp);
    dwrk.type = AFX_DRAW_WORK_TYPE_STREAMING;
    dwrk.streaming.resCnt = 0;

    for (afxNat i = 0; i < resCnt; ++i)
    {
        //AfxAssertObject(resources[i], AFX_FCC_DOUT);

        //if (outputs[i]->buffers[outputBufIdx[i]]->state == AFX_SURF_STATE_PENDING)
        {
            //AfxThrowError();
            //break;
        }

        //AfxObjectReacquire(resources[i], &din->obj, NIL, 0, NIL);
        dwrk.streaming.resources[i] = resources[i];
        ++dwrk.streaming.resCnt;
    }

    AfxTransistorEnterExclusive(&din->streamingLock);

    if (AfxQueuePush(&din->streamingQueue, &dwrk))
        AfxThrowError();

    AfxTransistorExitExclusive(&din->streamingLock);
    
    return err;
}

_SGL afxDinImpl _AfxStdDinImpl =
{
    _AfxDrawInputAcquireScript,
    _AfxDrawInputEnqueueStreaming,
    _AfxDrawInputEnqueueSubmission,
    _AfxDrawInputEnqueuePresentation,
    _AfxDrawInputAffineStreamingThreads,
    _AfxDrawInputAffineSubmissionThreads,
    _AfxDrawInputAffinePresentationThreads,
    _AfxDrawInputAffinePrefetchThreads
};

_SGL afxDrawInput _AfxDrawContextAcquireInput(afxDrawContext dctx, afxDrawInputSpecification const *spec)
{
    AfxEntry("dctx=%p,spec=%p", dctx, spec);
    afxError err = NIL;
    afxDrawInput din = NIL;

    if (!(din = AfxObjectAcquire(AfxDrawContextGetInputClass(dctx), spec, AfxSpawnHint())))
        AfxThrowError();

    return din;
}

_SGL afxError _AfxDinDtor(afxDrawInput din)
{
    AfxEntry("din=%p", din);
    afxError err = NIL;
    AfxAssertObject(din, AFX_FCC_DIN);
    
    afxNat falses[] = { FALSE, FALSE, FALSE, FALSE };

    // avoid draw thread entrance
    AfxDrawInputAffineStreamingThreads(din, 0, 4, falses);
    AfxDrawInputAffineSubmissionThreads(din, 0, 4, falses);
    AfxDrawInputAffinePresentationThreads(din, 0, 4, falses);
    AfxDrawInputAffinePrefetchThreads(din, 0, 4, falses);
    AfxYield();
#if 0
    AfxTransistorEnterExclusive(&din->presentationLock);
    AfxTransistorEnterExclusive(&din->submissionLock);
    AfxTransistorEnterExclusive(&din->streamingLock);
    AfxTransistorEnterExclusive(&din->fetchLock);
#else
    while (!AfxTransistorTryEnterExclusive(&din->prefetchLock)) AfxYield();
    while (!AfxTransistorTryEnterExclusive(&din->streamingLock)) AfxYield();
    while (!AfxTransistorTryEnterExclusive(&din->submissionLock)) AfxYield();
    while (!AfxTransistorTryEnterExclusive(&din->presentationLock)) AfxYield();
#endif
    AfxQueueDrop(&din->presentationQueue);
    AfxQueueDrop(&din->submissionQueue);
    AfxQueueDrop(&din->streamingQueue);

    AfxTransistorDrop(&din->presentationLock);
    AfxTransistorDrop(&din->submissionLock);
    AfxTransistorDrop(&din->streamingLock);
    AfxTransistorDrop(&din->prefetchLock);

    return err;
}

_SGL afxError _AfxDinCtor(afxDrawInput din, afxDrawInputSpecification const *spec)
{
    AfxEntry("din=%p", din);
    afxError err = NIL;

    AfxAssert(spec);
    AfxAssert(spec->estimatedSubmissionCnt);
    afxNat estimatedSubmissionCnt = spec->estimatedSubmissionCnt;

    afxDrawContext dctx = AfxDrawInputGetContext(din);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);

    afxSystem sys = AfxObjectGetProvider(&(dsys->obj));
    AfxAssertObject(sys, AFX_FCC_SYS);

    afxAllocationStrategy const as =
    {
        sizeof(afxByte),
        AFX_SIMD_ALIGN,
        spec->cmdPoolMemStock,
        spec->cmdPoolMemStock,
        AFX_ALL_DUR_TRANSIENT,
        FALSE
    };

    for (afxNat i = 0; i < dctx->queueCnt; i++)
    {
        din->prefetchThreadEnabled[i] = FALSE; // must be explicitally enabled to avoid unready fetches.

        din->streamingThreadEnabled[i] = spec->enabledStreamingThreads ? spec->enabledStreamingThreads[i] : TRUE;
        din->submissionThreadEnabled[i] = spec->enabledSubmissionThreads ? spec->enabledSubmissionThreads[i] : TRUE;
        din->presentationThreadEnabled[i] = spec->enabledPresentationThreads ? spec->enabledPresentationThreads[i] : TRUE;
    }

    if (!(din->cmdAll = AfxSystemAcquireAllocator(sys, &as, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (AfxTransistorDeploy(&din->streamingLock)) AfxThrowError();
        else
        {
            if (AfxQueueDeploy(&din->streamingQueue, sizeof(afxDrawWork), 32)) AfxThrowError();
            else
            {
                if (AfxTransistorDeploy(&din->submissionLock)) AfxThrowError();
                else
                {
                    if (AfxQueueDeploy(&din->submissionQueue, sizeof(afxDrawWork), 32)) AfxThrowError();
                    else
                    {
                        if (AfxTransistorDeploy(&din->presentationLock)) AfxThrowError();
                        else
                        {
                            if (AfxQueueDeploy(&din->presentationQueue, sizeof(afxDrawWork), 32)) AfxThrowError();
                            else
                            {
                                if (AfxTransistorDeploy(&din->prefetchLock)) AfxThrowError();
                                else
                                {
                                    din->prefetchProc = spec->prefetch;
                                    din->udd = spec->udd;

                                    if (err)
                                        AfxTransistorDrop(&din->prefetchLock);
                                }

                                if (err)
                                    AfxQueueDrop(&din->presentationQueue);
                            }

                            if (err)
                                AfxTransistorDrop(&din->presentationLock);
                        }

                        if (err)
                            AfxQueueDrop(&din->submissionQueue);
                    }
                    
                    if (err)
                        AfxTransistorDrop(&din->submissionLock);
                }

                if (err)
                    AfxQueueDrop(&din->streamingQueue);
            }
            
            if (err)
                AfxTransistorDrop(&din->streamingLock);
        }

        if (err)
            AfxObjectRelease(&din->cmdAll->obj);
    }

    return err;
}

_SGL afxClassSpecification const _AfxDinClassSpec;

afxClassSpecification const _AfxDinClassSpec =
{
    AFX_FCC_DIN,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawInput)),
    NIL,
    (void*)_AfxDinCtor,
    (void*)_AfxDinDtor,
    "Draw Input",
    &_AfxStdDinImpl
};
