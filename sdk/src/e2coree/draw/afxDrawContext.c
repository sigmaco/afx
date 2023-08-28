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

#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_SYSTEM_C
//#define _AFX_DRAW_DRIVER_C

#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C
#define _AFX_CANVAS_C
#define _AFX_BUFFER_C
#define _AFX_INDEX_BUFFER_C
#define _AFX_VERTEX_BUFFER_C
#define _AFX_TEXTURE_C
#define _AFX_SURFACE_C
#define _AFX_PIPELINE_C
#define _AFX_LEGO_C
#define _AFX_SHADER_C
#include "_classified/afxDrawClassified.h"

extern struct _afxDsysD dsys;

_AFX void _AfxDctxFreeAllInputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (dctx->inputs)
    {
        for (afxNat i = 0; i < dctx->inputCap; i++)
        {
            afxDrawInput din = dctx->inputs[i];

            if (din)
            {
                AfxDisconnectDrawInput(din, NIL);
                dctx->inputs[i] = NIL;
                --dctx->inputCnt;
            }
        }
        dctx->inputCap = 0;
        AfxDeallocate(dctx->ctx, dctx->inputs);
        dctx->inputs = NIL;
    }
}

_AFX void _AfxDctxFreeAllOutputSlots(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (dctx->outputs)
    {
        for (afxNat i = 0; i < dctx->outputCap; i++)
        {
            afxDrawOutput dout = dctx->outputs[i];

            if (dout)
            {
                AfxDisconnectDrawOutput(dout, NIL);
                dctx->outputs[i] = NIL;
                --dctx->outputCnt;
            }
        }
        dctx->outputCap = 0;
        AfxDeallocate(dctx->ctx, dctx->outputs);
        dctx->outputs = NIL;
    }
}

_AFX afxNat AfxCurateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCnt, first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;

    for (afxNat i = 0; i < dctx->inputCap; i++)
    {
        afxDrawInput din = dctx->inputs[i];

        if (din)
        {
            //AfxAssertType(dinD, AFX_FCC_DIN);

            ++cnt2;

            if (f(din, udd))
                break;

            if (cnt2 == cnt)
                break;
        }

    }
    return cnt2;
}

_AFX afxNat AfxCurateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCnt, first, cnt);
    AfxAssert(f);
    afxNat cnt2 = 0;

    for (afxNat i = 0; i < dctx->outputCap; i++)
    {
        afxDrawOutput dout = dctx->outputs[i];

        if (dout)
        {
            AfxAssertObjects(1, &dout, AFX_FCC_DOUT);

            ++cnt2;

            if (f(dout, udd))
                break;

            if (cnt2 == cnt)
                break;
        }

    }
    return cnt2;
}

_AFX afxNat AfxEnumerateConnectedDrawOutputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCnt, first, cnt);
    AfxAssert(dout);
    afxNat cnt2 = 0;
    afxNat total = 0;

    for (afxNat i = 0; i < dctx->outputCap; i++)
    {
        afxDrawOutput dout2 = dctx->outputs[i];

        if (dout2)
        {
            if (total >= first)
            {
                AfxAssertObjects(1, &dout2, AFX_FCC_DOUT);
                dout[i] = dout2;
                ++cnt2;

            }
            ++total;
        }

    }
    return cnt2;
}

_AFX afxNat AfxEnumerateConnectedDrawInputs(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->inputCnt, first, cnt);
    AfxAssert(din);
    afxNat cnt2 = 0;
    afxNat total = 0;

    for (afxNat i = 0; i < dctx->inputCap; i++)
    {
        afxDrawInput din2 = dctx->inputs[i];

        if (din2)
        {
            if (total >= first)
            {
                AfxAssertObjects(1, &din2, AFX_FCC_DIN);
                din[i] = din2;
                ++cnt2;

            }
            ++total;
        }

    }
    return cnt2;
}

_AFX afxError AfxDisconnectAllDrawInputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxDctxFreeAllInputSlots(dctx);
    return err;
}

_AFX afxError AfxDisconnectAllDrawOutputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    _AfxDctxFreeAllOutputSlots(dctx);
    return err;
}

_AFX afxDrawOutput AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->outputCap, slotIdx, 1);
    return dctx->outputs[slotIdx];
}

_AFX afxDrawInput AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->inputCap, slotIdx, 1);
    return dctx->inputs[slotIdx];
}

_AFX afxNat AfxCountConnectedDrawOutputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return dctx->outputCnt;
}

_AFX afxNat AfxCountConnectedDrawInputs(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return dctx->inputCnt;
}

_AFX afxDrawQueue AfxGetDrawQueue(afxDrawContext dctx, afxNat portIdx, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->openPortCnt, portIdx, 1);
    AfxAssertRange(AfxCountInstances(&dctx->openPorts[portIdx].queues), queIdx, 1);
    afxDrawQueue dque;
    
    if (!AfxEnumerateInstances(&dctx->openPorts[portIdx].queues, queIdx, 1, (afxHandle*)&dque))
        dque = NIL;

    return dque;
}

_AFX afxNat AfxGetDrawQueueCount(afxDrawContext dctx, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxAssertRange(dctx->openPortCnt, portIdx, 1);
    return dctx->openPorts[portIdx].queues.pool.totalUsedCnt;
}

_AFX afxContext AfxGetDrawContextMemory(afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    afxContext ctx = dctx->ctx;
    AfxAssertObjects(1, &ctx, AFX_FCC_CTX);
    return ctx;
}

_AFX afxError AfxAcquireDrawContexts(afxDrawSystem dsys, afxNat cnt, afxDrawContext dctx[], afxDrawContextConfig const config[])
{
    AfxEntry("cnt=%u,config=%p", cnt, config);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    
    if (!(AfxGetInstance(&dsys->devices, config ? config->devId : 0, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);

        if (AfxAcquireObjects(AfxGetDrawContextClass(ddev), cnt, (afxHandle*)dctx, (void*[]) { (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, dctx, AFX_FCC_DCTX);
    }
    return err;
}
