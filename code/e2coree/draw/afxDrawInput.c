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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_INPUT_C
//#define _AFX_SURFACE_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/core/afxSystem.h"

////////////////////////////////////////////////////////////////////////////////
// CONNECTION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxGetDrawInputConnection(afxDrawInput din, afxDrawContext *dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxBool rslt;
    afxDrawContext dctx2 = AfxGetLinker(&din->dctx);
    if ((rslt = !!(dctx2)))
    {
        AfxAssertObjects(1, &dctx2, afxFcc_DCTX);

        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AFX afxBool AfxDrawInputIsConnected(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return !!(AfxGetDrawInputConnection(din, NIL));
}

_AFX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext curr = NIL;
    AfxGetDrawInputConnection(din, &curr);

    if (dctx != curr)
    {
        afxDrawDevice ddev = AfxGetDrawInputDevice(din);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, afxFcc_DDEV);

            if (ddev != ddrv2) // can not connect to context acquired of another device
                AfxThrowError();
        }

        if (!err)
        {
            if (dctx)
                AfxReacquireObjects(1, (void*[]) { dctx });

            if (ddev->relinkDin(ddev, din, dctx))
                AfxThrowError();

            if (err)
                AfxReleaseObjects(1, (void*[]) { dctx });
            else if (curr)
            {
                AfxAssertObjects(1, &curr, afxFcc_DCTX);
                AfxReleaseObjects(1, (void*[]) { curr });
            }
        }
    }
    AfxAssert(AfxGetLinker(&din->dctx) == dctx);
    return !err;
}

_AFX afxError AfxDisconnectDrawInput(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!AfxReconnectDrawInput(din, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawInputIsConnected(din));
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// PREFETCHING                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFX void* AfxGetDrawInputUdd(afxDrawInput din)
{ 
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din->udd;
}

_AFX afxError AfxEnableDrawInputPrefetching(afxDrawInput din, afxBool enabled)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    din->prefetchEnabled = !!enabled;
    return err;
}

_AFX afxError AfxSubmitDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(scripts);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state. 
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state. 
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxDrawContext dctx;

    if (!(AfxGetDrawInputConnection(din, &dctx))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        if (din->submitCb(din,1,scripts))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxPresentDrawOutputBuffers(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outputBufIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
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

    if (!(AfxGetDrawInputConnection(din, &dctx))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        if (din->presentCb(din, 1, outputs, outputBufIdx))
            AfxThrowError();
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SYSTEM                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxDrawDevice AfxGetDrawInputDevice(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = AfxGetObjectProvider(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AFX afxError AfxOpenDrawInputs(afxDrawSystem dsys, afxNat ddevId, afxNat cnt, afxDrawInputConfig const config[], afxDrawInput din[])
{
    AfxEntry("cnt=%u,config=%p,", cnt, config);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxDrawDevice ddev;
    
    if (!(AfxGetDrawDevice(dsys, ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = AfxGetDrawInputClass(ddev);
        AfxAssertClass(cls, afxFcc_DIN);

        if (AfxAcquireObjects(cls, cnt, (afxHandle*)din, (void*[]) { &ddevId, (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, din, afxFcc_DIN);
    }
    return err;
}

_AFX afxNat AfxCurateDrawInputs(afxDrawSystem dsys, afxNat ddevId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass* cls = AfxGetDrawDeviceClass(dsys);
    AfxAssertClass(cls, afxFcc_DDEV);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((ddevId != AFX_INVALID_INDEX) && !(AfxGetInstance(cls, ddevId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((ddevId == AFX_INVALID_INDEX)) cls = &dsys->inputs;
        else
        {
            AfxAssertObjects(1, &ddev, afxFcc_DDEV);
            cls = AfxGetDrawInputClass(ddev);
        }

        AfxAssertClass(cls, afxFcc_DIN);
        rslt = AfxCurateInstances(cls, first, cnt, (void*)f, udd);
    }
    return rslt;
}

_AFX afxNat AfxEnumerateDrawInputs(afxDrawSystem dsys, afxNat ddevId, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(din);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass* cls = AfxGetDrawDeviceClass(dsys);
    AfxAssertClass(cls, afxFcc_DDEV);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((ddevId != AFX_INVALID_INDEX) && !(AfxGetInstance(cls, ddevId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((ddevId == AFX_INVALID_INDEX)) cls = &dsys->inputs;
        else
        {
            AfxAssertObjects(1, &ddev, afxFcc_DDEV);
            cls = AfxGetDrawInputClass(ddev);
        }

        AfxAssertClass(cls, afxFcc_DIN);
        rslt = AfxEnumerateInstances(cls, first, cnt, (afxHandle*)din);
    }
    return rslt;
}

_AFX afxNat AfxCountDrawInputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxCountInstances(cls);
}