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
#define _AFX_DRAW_CONTEXT_C
#include "afx/core/afxSystem.h"
#include "_classified/afxDrawClassified.h"

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetConnectedDrawInputContext(afxDrawInput din, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxBool rslt;
    afxDrawContext dctx2;
    if ((rslt = !!(dctx2 = din->dctx)))
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
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    if (din->vmt->link(din, din->dctx, NIL, slotIdx))
        AfxThrowError();

    AfxAssert(!din->dctx);
    return err;
}

_AFX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawDevice ddev = AfxGetObjectProvider(din);
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);

    if (dctx)
    {
        afxDrawDevice ddrv2 = AfxGetObjectProvider(dctx);
        AfxAssertObjects(1, &ddrv2, AFX_FCC_DDEV);

        if (ddev != ddrv2)
            AfxThrowError();
    }

    if (!err)
    {
        if (din->vmt->link(din, din->dctx, dctx, slotIdx))
            AfxThrowError();
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
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    return din->udd;
}

_AFX afxError AfxEnableDrawInputPrefetching(afxDrawInput din, afxBool enabled)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    din->prefetchEnabled = !!enabled;
    return err;
}

_AFX afxError AfxSubmitDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
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
        AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

        if (din->vmt->subm(din,1,scripts))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxSubmitPresentations(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outputBufIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
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
        AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

        if (din->vmt->pres(din, 1, outputs, outputBufIdx))
            AfxThrowError();
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireDrawInputs(afxDrawSystem dsys, afxNat cnt, afxDrawInput din[], afxDrawInputConfig const config[])
{
    AfxEntry("cnt=%u,config=%p,", cnt, config);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;

    if (!(AfxGetInstance(&dsys->devices, config ? config->devId : 0, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);

        if (AfxAcquireObjects(AfxGetDrawInputClass(ddev), cnt, (afxHandle*)din, (void*[]) { (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, din, AFX_FCC_DIN);
    }
    return err;
}
