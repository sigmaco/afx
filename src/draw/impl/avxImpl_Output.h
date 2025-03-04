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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_IMPL___OUTPUT_H
#define AVX_IMPL___OUTPUT_H

#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/mem/afxInterlockedQueue.h"
#include "qwadro/inc/mem/afxSlabAllocator.h"

/*
    Blit: Transfers or copies pixel data from one area to another within or between buffers. It’s used for operations like rendering images or updating parts of the screen.
    Flip: Swaps the front and back buffers in double buffering to update the display with a new frame, reducing flickering and improving performance.
    Swap: Refers to the exchange of buffers in multiple buffering strategies (e.g., double or triple buffering) to update the screen, often used to manage buffer visibility and performance.
    Present: Displays the contents of the current frame buffer on the screen, making the rendered image visible to the user.
*/

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDoutDdi);
#else
AFX_DEFINE_STRUCT(_avxDoutDdi)
{
    afxError(*ioctl)(afxDrawOutput, afxUnit, va_list ap);
    afxError(*adjust)(afxDrawOutput, avxRange);
    afxError(*present)(afxDrawQueue, avxPresentation*, avxFence wait, afxDrawOutput, afxUnit bufIdx, avxFence signal);
    afxError(*reqBuf)(afxDrawOutput dout, afxTime timeout, afxUnit *bufIdx);
    afxError(*recycBuf)(afxDrawOutput dout, afxUnit bufIdx);
};
#endif

#ifdef _AVX_DRAW_OUTPUT_C

AFX_DEFINE_STRUCT(afxVideoEndpoint)
{
    afxUri32            urn;
    afxDrawOutputCaps   caps;    
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxBool             resizable;
    afxReal             refreshRate;
    afxWhd              res; // Screen resolution. Absolute extent available.
    union
    {
        afxObject       obj;
        afxWindow       wnd;
    };
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    avxVideoAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxVideoTransform presentTransform; // NIL leaves it as it is.
    avxPresentMode      presentMode; // FIFO
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).
};

#ifdef _AVX_DRAW_OUTPUT_IMPL
AFX_OBJECT(_avxDrawOutput)
#else
AFX_OBJECT(afxDrawOutput)
#endif
{
    _avxDoutDdi const* pimpl;
    void*               udd[4]; // user-defined data    
    // endpoint
    afxVideoEndpoint    endp;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    afxError            (*lockCb)(afxDrawOutput, afxTime timeout, afxUnit*bufIdx);
    afxError            (*unlockCb)(afxDrawOutput, afxUnit cnt, afxUnit const bufIdx[]);
    afxError            (*presentCb)(afxDrawQueue dque, avxPresentation* ctrl, afxDrawOutput,afxUnit bufIdx);
    struct _afxDoutIdd* idd; // alloc'ed by the driver

    avxRange           resolution; // Screen resolution. Absolute extent available.
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal64           wpOverHp; // physical w/h
    afxReal             refreshRate;
    afxBool             exclusive;
    afxBool             resizable;

    // canvas
    avxColorSpace       colorSpc; // raster color space. sRGB is the default.    
    avxFormat           bufFmt[3]; // format for color, depth and stencil surfaces, respectively.
    afxRasterUsage      bufUsage[3]; // raster usage for color, depth and stencil, respectively.
    afxRasterFlags      bufFlags[3]; // raster flags for color, depth and stencil, respectively.
    avxRange           extent;
    afxReal64           wwOverHw; // window w/h
    afxBool             resizing;

    // swapchain
    afxUnit             bufCnt; // usually 2 or 3; double or triple buffered.
    avxCanvas*          canvases;
    afxInterlockedQueue freeBuffers;
    afxAtom32           presentingBufIdx;
    avxPresentMode      presentMode; // FIFO
    avxVideoAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxVideoTransform presentTransform; // NIL leaves it as it is.
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).

    afxUnit             suspendCnt;
    afxSlock            suspendSlock;
    afxAtom32           submCnt;
    afxCondition        idleCnd;
    afxMutex            idleCndMtx;

    afxClock            startClock;
    afxClock            lastClock;
    afxUnit             outNo;
    afxUnit             outRate; // outputs per second
    afxClock            outCntResetClock;
};
#endif//_AVX_DRAW_OUTPUT_C

AVX afxClassConfig const _AVX_DOUT_CLASS_CONFIG;
AVX _avxDoutDdi const _AVX_DOUT_DDI;

AVX afxError _AvxDoutImplIoctlCb(afxDrawOutput dout, afxUnit code, afxUnit inSiz, void* in, afxUnit outCap, void* out, afxUnit32* outSiz, avxFence signal);
AVX afxError _AvxDoutImplAdjustCb(afxDrawOutput dout, avxRange whd);
AVX afxError _AvxDoutImplPresentCb(afxDrawQueue dque, avxPresentation* ctrl, avxFence wait, afxDrawOutput dout, afxUnit bufIdx, avxFence signal);
AVX afxError _AvxDoutImplRequestBufferCb(afxDrawOutput dout, afxTime timeout, afxUnit *bufIdx);
AVX afxError _AvxDoutImplRecycleBufferCb(afxDrawOutput dout, afxUnit bufIdx);


#endif//AVX_IMPL___OUTPUT_H
