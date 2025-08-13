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

_AVX afxError _AvxDquePresentBuffers(afxDrawQueue dque, afxUnit cnt, avxPresentation presentations[])
{
    afxError err = AFX_ERR_NONE;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxPresentation* pres = &presentations[i];
        
        afxSurface dout = pres->dout;
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

        AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
        dout->presentingBufIdx = pres->bufIdx;

        if (pres->wait)
        {
            while (AvxWaitForFences(AfxGetProvider(pres->dout), FALSE, AFX_TIMEOUT_INFINITE, 1, &pres->wait))
            {
                AfxYield();
            }
        }

        if (dout->pimpl->presentCb && dout->pimpl->presentCb(dque, pres))
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
                //AfxFormatWindowTitle(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
            }

            if (dout->endpointNotifyFn)
            {
                dout->endpointNotifyFn(dout->endpointNotifyObj, pres->bufIdx);
            }
        }

        dout->presentingBufIdx = (afxAtom32)AFX_INVALID_INDEX;
        --dout->buffers[pres->bufIdx].locked;
        AfxPushInterlockedQueue(&dout->freeBuffers, (afxUnit[]) { pres->bufIdx });
        //AfxDecAtom32(&dout->m.submCnt);
    }
    return err;
}

_AVX afxError _AvxDoutImplRegenBuffers(afxSurface dout, afxBool build)
{
    afxError err = AFX_ERR_NONE;
    // @dout must be a valid afxSurface handle.
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    AvxWaitForSurface(dout, 0);

    afxDrawSystem dsys = AvxGetSurfaceSystem(dout);

    for (afxUnit i = 0; i < dout->bufCnt; i++)
    {
        _avxDoutBuffer* slot = &dout->buffers[i];
        ++slot->locked;

        if (slot->canv)
        {
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &slot->canv);
            AfxDisposeObjects(1, &slot->canv);
            AFX_ASSERT(!slot->canv);
            slot->ras = NIL;
        }

        if (!build)
            continue;

        if (AvxCoacquireCanvas(dsys, &dout->ccfg, 1, &slot->canv))
        {
            AfxThrowError();

            // delete buffers?
        }
        else
        {
            avxCanvas canv = slot->canv;
            AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
            avxRaster ras;
            AvxGetDrawBuffers(canv, 0, 1, &ras);
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            slot->ras = ras;
        }

        --slot->locked;
    }
    return err;
}

_AVX afxError _AvxDoutImplAdjustCb(afxSurface dout, afxRect const* area, afxBool fse)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    dout->area.x = area->x;
    dout->area.y = area->y;
    dout->area.w = AFX_MAX(1, area->w);
    dout->area.h = AFX_MAX(1, area->h);
    dout->wwOverHw = (afxReal64)dout->area.w / (afxReal64)dout->area.h;
    dout->ccfg.whd = AvxMaxRange(AVX_RANGE(1, 1, 1), AVX_RANGE(dout->area.w, dout->area.h, 1));

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
