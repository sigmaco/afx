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

_AVX afxError _AvxPresentDrawOutput(afxDrawQueue dque, avxPresentation* ctrl, afxDrawOutput dout, afxUnit bufIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
    dout->presentingBufIdx = bufIdx;

    if (dout->presentCb && dout->presentCb(dque, ctrl, dout, bufIdx))
        AfxThrowError();

    afxClock currClock;
    AfxGetClock(&currClock);
    ++dout->outNo;

    if ((1.0 <= AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)))
    {
        dout->outCntResetClock = currClock;
        dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
        dout->outNo = 0;

        afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
        afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
        dout->lastClock = currClock;

        if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
        {
            //AfxFormatWindowCaption(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
        }

        if (dout->endpointNotifyFn)
        {
            dout->endpointNotifyFn(dout->endpointNotifyObj, bufIdx);
        }
    }

    dout->presentingBufIdx = (afxAtom32)AFX_INVALID_INDEX;
    AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { bufIdx });
    //AfxDecAtom32(&dout->m.submCnt);

    return err;
}

_AVXINL afxError _AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AfxWhdMax(&dout->extent, AfxWhd(1, 1, 1), whd);
    dout->wwOverHw = (afxReal64)dout->extent.w / (afxReal64)dout->extent.h;

    //afxEvent ev;
    //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->extent);
    //AfxNotifyObject(&dout->obj, &ev);

    AfxRevalidateDrawOutputBuffers(dout);
    return err;
}
