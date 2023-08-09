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

#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_INPUT_C
//#define _AFX_SURFACE_C
#define _AFX_DRAW_SCRIPT_C
#include "afx/core/afxSystem.h"
#include "_classified/afxDrawClassified.h"

_AFX afxBool _AfxGetDinD(afxDrawInput din, struct _afxDinD **dinD, struct _afxDsysD* dsysD)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxGetObjectsResidency(1, &din, (void**)dinD, &dsysD->inputs);
}

_AFX afxError _AfxDinFreeAllBuffers(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);

    //AfxEnterSlockExclusive(&dinD->scriptChainMutex);

    for (afxNat i = 0; i < dinD->scripts.cnt; i++)
    {
        afxDrawScript dscr = *(afxDrawScript*)AfxGetArrayUnit(&dinD->scripts, i);
        AfxAssertObject(dscr, AFX_FCC_DSCR);
        while (0 < AfxReleaseObject(&dscr->obj));
    }

    //AfxExitSlockExclusive(&dinD->scriptChainMutex);
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetConnectedDrawInputContext(afxDrawInput din, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);

    afxBool rslt;
    afxDrawContext dctx2;
    if ((rslt = !!(dctx2 = dinD->dctx)))
    {
        AfxAssertObjects(1, &dctx2, AFX_FCC_DCTX);

        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawInputIsConnected(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    return !!(AfxGetConnectedDrawInputContext(din, NIL));
}

_AFX afxError AfxDisconnectDrawInput(afxDrawInput din, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);

    _AfxDinFreeAllBuffers(din);
    
    if (dinD->vmt->cctx(din, dinD->dctx, NIL, slotIdx))
    {
        AfxThrowError();
    }
    else
    {
        dinD->dctx = NIL;
    }

    AfxAssert(!dinD->dctx);

    return err;
}

_AFX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);

    afxDrawDriver ddrv = AfxGetDrawInputDriver(din);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawDriver ddrv2 = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv2, AFX_FCC_DDRV);

    if (ddrv != ddrv2) AfxThrowError();
    else
    {
        _AfxDinFreeAllBuffers(din);

        if (dinD->vmt->cctx(din, dinD->dctx, dctx, slotIdx))
        {
            AfxThrowError();
        }
        else
        {
            dinD->dctx = dctx;
        }
    }
    AfxAssert(dinD->dctx == dctx);
    return !err;
}

////////////////////////////////////////////////////////////////////////////////
// PREFETCHING                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFX void* AfxGetDrawInputUdd(afxDrawInput din)
{ 
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
    return dinD->udd;
}

_AFX afxError AfxEnableDrawInputPrefetching(afxDrawInput din, afxBool enabled)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);

    dinD->prefetchEnabled = !!enabled;

    return err;
}

_AFX afxError AfxSubmitDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
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
        //AfxAssertType(dctxD, AFX_FCC_DCTX);
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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
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
        //AfxAssertType(dctxD, AFX_FCC_DCTX);
        afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
        AfxAssertObject(ddrv, AFX_FCC_DDRV);
        //afxNat portCnt = AfxGetDrawDriverPortCount(ddrv);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxDrawOutput dout = outputs[i];
            //AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

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
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
    afxDrawDriver ddrv = dinD->ddrv;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError _AfxDinDtor(afxDrawInput din, struct _afxDinD *dinD)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    AfxAssertType(dinD, AFX_FCC_DIN);

    AfxDisconnectDrawInput(din, NIL);

    // avoid draw thread entrance

    AfxEnableDrawInputPrefetching(din, FALSE);
    //AfxDiscardAllDrawInputSubmissions(din);
    //AfxYieldThreading();
    //while (!AfxTryEnterSlockExclusive(&dinD->prefetchSlock)) AfxYieldThreading();

    if (dinD->vmt->dtor && dinD->vmt->dtor(din))
        AfxThrowError();

    AfxAssert(!dinD->idd);

    //AfxReleaseSlock(&dinD->prefetchSlock);

    return err;
}

_AFX void _AfxObjDeallocDin(afxDrawInput din)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
    _AfxDinDtor(din, dinD);
    AfxDeallocateObjects(1, &din, &dsysD->inputs);
}

_AFX void AfxReleaseDrawInputs(afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, din, AFX_FCC_DIN);
    AfxReleaseObjects(cnt, din, _AfxObjDeallocDin);
}

_AFX afxError _AfxDinCtor(afxDrawInput din, struct _afxDinD *dinD, afxDrawInputConfig const *config)
{
    AfxEntry("din=%p", din);
    afxError err = AFX_ERR_NONE;


    AfxAssignFcc(dinD, AFX_FCC_DIN);

    afxDrawDriver ddrv;
    AfxGetDrawDriver(config ? config->drvIdx : 0, &ddrv);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    dinD->ddrv = ddrv;
    dinD->dctx = NIL;
    afxContext mem = AfxGetDrawMemory();
    dinD->mem = mem;

    dinD->prefetchEnabled = FALSE; // must be explicitally enabled to avoid unready fetches.

    dinD->userPrefetchProc = config ? config->prefetch : NIL;

    AfxAcquireArray(&dinD->scripts, sizeof(afxDrawScript), 32, AfxSpawnHint());
    dinD->minScriptReserve = 2;

    dinD->vmt = NIL;
    dinD->idd = NIL;
            
    if (config && config->udd)
    {
        dinD->udd = config->udd;
    }

    if (ddrv->vmt->din && ddrv->vmt->din(din, config->endpoint, config)) AfxThrowError();
    else
    {
        AfxAssert(dinD->vmt);
    }

    return err;
}

_AFX afxError AfxAcquireDrawInputs(afxNat cnt, afxDrawInput din[], afxDrawInputConfig const config[])
{
    AfxEntry("cnt=%u,config=%p,", cnt, config);
    afxError err = AFX_ERR_NONE;

    afxDrawDriver ddrv;

    if (!AfxGetDrawDriver(config ? config->drvIdx : 0, &ddrv)) AfxThrowError();
    else
    {
        AfxAssertObject(ddrv, AFX_FCC_DDRV);

        afxDrawSystem dsys;
        AfxGetDrawSystem(&dsys);
        AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
        struct _afxDsysD* dsysD;
        _AfxGetDsysD(dsys, &dsysD);
        AfxAssertType(dsysD, AFX_FCC_DSYS);

        if (AfxAcquireNamedObjects(cnt, din, AFX_FCC_DIN, "Draw Input")) AfxThrowError();
        else
        {
            if (AfxAllocateObjects(cnt, din, &dsysD->inputs)) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < cnt; i++)
                {
                    struct _afxDinD *dinD;
                    _AfxGetDinD(din[i], &dinD, dsysD);

                    if (_AfxDinCtor(din[i], dinD, &config[i]))
                    {
                        AfxThrowError();
                        afxNat j = i;

                        for (afxNat k = 0; k < i; k++)
                            if (_AfxDinDtor(din[j], dinD))
                                AfxThrowError();
                    }
                }
            }

            if (err)
                AfxReleaseObjects(cnt, din, NIL);
        }
    }

    if (err)
    {
        for (afxNat i = 0; i < cnt; i++)
            din[i] = NIL;
    }
    else
    {
        AfxAssertObjects(cnt, din, AFX_FCC_DIN);
    }
    return err;
}
