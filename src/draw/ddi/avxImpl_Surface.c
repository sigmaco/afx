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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "avxImplementation.h"

_AVX afxError _AvxDquePresentBuffers(afxDrawQueue dque, afxUnit cnt, avxPresentation presentations[])
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxPresentation* pres = &presentations[i];

        afxSurface dout = pres->dout;
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
#if 0
        if (pres->waitOnDpu && pres->dout->pimpl->presOnDpuCb)
        {
            _AvxDqueSubmitCallback(dque, (void*)pres->dout->pimpl->presOnDpuCb, pres);
        }
        else
#endif
        {
            //AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
            //dout->presentingBufIdx = pres->bufIdx;
#if 0
            if (pres->waitOnDpu)
            {
                while (AvxWaitForFences(AfxGetHost(pres->dout), AFX_TIMEOUT_INFINITE, FALSE, 1, &pres->waitOnDpu, NIL))
                {
                    AfxYield();
                }
            }
#endif
            if (dout->pimpl->presentCb && dout->pimpl->presentCb(dque, pres))
                AfxThrowError();

            dout->lastPresentedBufIdx = (afxAtom32)pres->bufIdx;

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
                    //AfxFormatWindowTitle(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
                }

                if (dout->endpointNotifyFn)
                {
                    dout->endpointNotifyFn(dout->endpointNotifyObj, pres->bufIdx);
                }
            }

            --dout->buffers[pres->bufIdx].locked;
            AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { pres->bufIdx });
            //AfxDecAtom32(&dout->m.submCnt);
        }
    }
    return err;
}

_AVX afxError _AvxDoutImplIoctl2Cb(afxSurface dout, afxUnit code, afxUnit inSiz, void* in, afxUnit outCap, void* out, afxUnit32* outSiz, avxFence signal)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    return err;
}

_AVX afxError _AvxDoutImplIoctlCb(afxSurface dout, afxUnit code, va_list ap)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    return err;
}

_AVX _avxDoutDdi const _AVX_DOUT_DDI =
{
    .ioctlCb = _AvxDoutImplIoctlCb,
    .adjustCb = _AvxDoutImplAdjustCb,
    .presentCb = NIL,
    .lockCb = NIL,
    .unlockCb = NIL,
    .regenCb = _AvxDoutImplRegenBuffers
};
