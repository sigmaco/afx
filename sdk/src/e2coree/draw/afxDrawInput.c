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

#define _AFX_DRAW_INPUT_C
//#define _AFX_SURFACE_C
#define _AFX_DRAW_SCRIPT_C
#include "afx/core/afxSystem.h"
#include "_classified/afxDrawClassified.h"

_AFX afxError _AfxDinFreeAllBuffers(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);

    //AfxEnterSlockExclusive(&din->scriptChainMutex);

    for (afxNat i = 0; i < din->scripts.cnt; i++)
    {
        afxDrawScript dscr = *(afxDrawScript*)AfxGetArrayUnit(&din->scripts, i);
        AfxAssertObject(dscr, AFX_FCC_DSCR);
        while (0 < AfxReleaseObject(&dscr->obj));
    }

    //AfxExitSlockExclusive(&din->scriptChainMutex);
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetConnectedDrawInputContext(afxDrawInput din, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);

    afxBool rslt;
    afxDrawContext dctx2;
    if ((rslt = !!(dctx2 = din->dctx)))
    {
        AfxAssertObject(dctx2, AFX_FCC_DCTX);

        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawInputIsConnected(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);

    return !!(AfxGetConnectedDrawInputContext(din, NIL));
}

_AFX afxError AfxDisconnectDrawInput(afxDrawInput din, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);

    _AfxDinFreeAllBuffers(din);
    
    if (din->vmt->cctx(din, din->dctx, NIL, slotIdx))
    {
        AfxThrowError();
    }
    else
    {
        din->dctx = NIL;
    }

    AfxAssert(!din->dctx);

    return err;
}

_AFX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxDrawDriver ddrv = AfxGetDrawInputDriver(din);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawDriver ddrv2 = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv2, AFX_FCC_DDRV);

    if (ddrv != ddrv2) AfxThrowError();
    else
    {
        _AfxDinFreeAllBuffers(din);

        if (din->vmt->cctx(din, din->dctx, dctx, slotIdx))
        {
            AfxThrowError();
        }
        else
        {
            din->dctx = dctx;
        }
    }
    AfxAssert(din->dctx == dctx);
    return !err;
}

////////////////////////////////////////////////////////////////////////////////
// PREFETCHING                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFX void* AfxGetDrawInputUdd(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    return din->udd;
}

_AFX afxError AfxEnableDrawInputPrefetching(afxDrawInput din, afxBool enabled)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);

    din->prefetchEnabled = !!enabled;

    return err;
}

_AFX afxError AfxSubmitDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssert(cnt);
    AfxAssert(scripts);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state. 
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state. 
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxDrawContext dctx;
    if (!(AfxGetConnectedDrawInputContext(din, &dctx))) AfxThrowError();
    else
    {
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
        AfxAssertObject(ddrv, AFX_FCC_DDRV);
        //afxNat portCnt = AfxGetDrawDriverPortCount(ddrv);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxDrawScript dscr = scripts[i];
            AfxAssertObject(dscr, AFX_FCC_DSCR);

            afxNat portIdx = dscr->portIdx;
            afxNat queIdx;

            if (!AfxRequestNextDrawQueue(dctx, portIdx, 0, &queIdx))
            {
                afxDrawQueue dque = AfxGetDrawQueue(dctx, portIdx, queIdx);
                AfxAssertObject(dque, AFX_FCC_DQUE);

                afxDrawSubmissionSpecification spec = { 0 };
                spec.scriptCnt = 1;
                spec.scripts = &dscr;

                afxNat submNo;

                if (dque->vmt->submit(dque, &spec, &submNo))
                {

                }
            }
        }
    }
    return err;
}

_AFX afxError AfxSubmitPresentations(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outputBufIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    AfxAssert(cnt);
    AfxAssert(outputs);
    AfxAssert(outputBufIdx);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo, 
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine. 
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine. 
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    afxDrawContext dctx;

    if (!(AfxGetConnectedDrawInputContext(din, &dctx))) AfxThrowError();
    else
    {
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
        AfxAssertObject(ddrv, AFX_FCC_DDRV);
        //afxNat portCnt = AfxGetDrawDriverPortCount(ddrv);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxDrawOutput dout = outputs[i];
            AfxAssertObject(dout, AFX_FCC_DOUT);

            afxNat portIdx = 0;
            afxNat queIdx;

            if (!AfxRequestNextDrawQueue(dctx, portIdx, 0, &queIdx))
            {
                afxDrawQueue dque = AfxGetDrawQueue(dctx, portIdx, queIdx);
                AfxAssertObject(dque, AFX_FCC_DQUE);

                afxDrawSubmissionSpecification spec = { 0 };
                spec.outputCnt = 1;
                spec.outputs = outputs;
                spec.outBufIdx = outputBufIdx;
                
                afxNat submNo;

                if (dque->vmt->submit(dque, &spec, &submNo))
                {

                }
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxDrawDriver AfxGetDrawInputDriver(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    afxDrawDriver ddrv = AfxObjectGetProvider(&din->obj);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError AfxAcquireDrawInputs(afxDrawInputSpecification const *spec, afxNat cnt, afxDrawInput din[])
{
    AfxEntry("spec=%p,cnt=%u", spec, cnt);
    afxError err = AFX_ERR_NONE;
    
    afxDrawDriver ddrv;

    if (!AfxGetDrawDriver(spec ? spec->drvIdx : 0, &ddrv)) AfxThrowError();
    else
    {
        AfxAssertObject(ddrv, AFX_FCC_DDRV);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxClassAcquireObjects(AfxGetDrawInputClass(ddrv), NIL, 1, spec, (afxObject**)&din[i], AfxSpawnHint()))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxAssertObject(din[j], AFX_FCC_DIN);
                    AfxReleaseObject(&din[j]->obj);
                    din[j] = NIL;
                }
                break;
            }
            else
            {
                AfxAssertObject(din[i], AFX_FCC_DIN);
            }
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            din[i] = NIL;

    return err;
}

_AFX afxBool _AfxDinEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (void*)obj;
    AfxAssertObject(din, AFX_FCC_DIN);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDinEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (void*)obj;
    AfxAssertObject(din, AFX_FCC_DIN);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDinDtor(afxDrawInput din)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    
    AfxDisconnectDrawInput(din, NIL);

    // avoid draw thread entrance

    AfxEnableDrawInputPrefetching(din, FALSE);
    //AfxDiscardAllDrawInputSubmissions(din);
    //AfxYieldThreading();
    //while (!AfxTryEnterSlockExclusive(&din->prefetchSlock)) AfxYieldThreading();

    if (din->vmt->dtor && din->vmt->dtor(din))
        AfxThrowError();

    AfxAssert(!din->idd);

    //AfxReleaseSlock(&din->prefetchSlock);

    return err;
}

_AFX afxError _AfxDinCtor(void *cache, afxNat idx, afxDrawInput din, afxDrawInputSpecification const *specs)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;
    (void)cache;

    afxDrawInputSpecification const *spec = &specs[idx];
    AfxAssert(spec);

    afxDrawDriver ddrv = AfxGetDrawInputDriver(din);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    din->dctx = NIL;
    afxMemory mem = AfxGetDrawMemory();
    din->mem = mem;

    din->prefetchEnabled = FALSE; // must be explicitally enabled to avoid unready fetches.

    din->userPrefetchProc = spec ? spec->prefetch : NIL;

    AfxAcquireArray(mem, &din->scripts, sizeof(afxDrawScript), 32, AfxSpawnHint());
    din->minScriptReserve = 2;

    din->vmt = NIL;
    din->idd = NIL;
            
    if (spec && spec->udd)
    {
        din->udd = spec->udd;
    }

    if (ddrv->vmt->din && ddrv->vmt->din(din, spec->endpoint, spec)) AfxThrowError();
    else
    {
        AfxAssert(din->vmt);
    }

    return err;
}

_AFX afxClassSpecification const _AfxDinClassSpec;

afxClassSpecification const _AfxDinClassSpec =
{
    AFX_FCC_DIN,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawInput)),
    NIL,
    (void*)_AfxDinCtor,
    (void*)_AfxDinDtor,
    .event = _AfxDinEventHandler,
    .eventFilter = _AfxDinEventFilter,
    "afxDrawInput",
    NIL
};
