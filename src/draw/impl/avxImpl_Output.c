/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_OUTPUT_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVX afxError _AvxDoutImplPresentCb(afxDrawQueue dque, avxPresentation* ctrl, avxFence wait, afxDrawOutput dout, afxUnit bufIdx, avxFence signal)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
    dout->presentingBufIdx = bufIdx;

    if (wait)
    {
        while (AvxWaitForFences(AfxGetProvider(dout), FALSE, AFX_TIME_INFINITE, 1, &wait))
        {
            AfxYield();
        }
    }

    if (dout->presentCb && dout->presentCb(dque, ctrl, dout, bufIdx))
        AfxThrowError();

    afxClock currClock;
    AfxGetClock(&currClock);
    ++dout->outNo;

    if ((1.0 <= AfxGetClockSecondsElapsed(&dout->outCntResetClock, &currClock)))
    {
        dout->outCntResetClock = currClock;
        dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
        dout->outNo = 0;

        afxReal64 ct = AfxGetClockSecondsElapsed(&dout->startClock, &currClock);
        afxReal64 dt = AfxGetClockSecondsElapsed(&dout->lastClock, &currClock);
        dout->lastClock = currClock;

        if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
        {
            //AfxFormatWindowTitle(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
        }

        if (dout->endpointNotifyFn)
        {
            dout->endpointNotifyFn(dout->endpointNotifyObj, bufIdx);
        }
    }

    dout->presentingBufIdx = (afxAtom32)AFX_INVALID_INDEX;
    --dout->buffers[bufIdx].locked;
    AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { bufIdx });
    //AfxDecAtom32(&dout->m.submCnt);

    return err;
}

_AVX afxError _AvxDoutImplAdjustCb(afxDrawOutput dout, avxRange whd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    dout->extent = AvxMaxRange(AVX_RANGE(1, 1, 1), whd);
    dout->wwOverHw = (afxReal64)dout->extent.w / (afxReal64)dout->extent.h;

    //afxEvent ev;
    //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->extent);
    //AfxNotifyObject(&dout->obj, &ev);

    AvxRevalidateDrawOutputBuffers(dout);
    return err;
}

_AVX afxError _AvxDoutImplIoctlCb(afxDrawOutput dout, afxUnit code, afxUnit inSiz, void* in, afxUnit outCap, void* out, afxUnit32* outSiz, avxFence signal)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    return err;
}

_AVX _avxDoutDdi const _AVX_DOUT_DDI =
{
    .adjust = _AvxDoutImplAdjustCb,
    .present = _AvxDoutImplPresentCb,
    .reqBuf = NIL,
    .recycBuf = NIL,
};
