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
    avxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxPresentTransform presentTransform; // NIL leaves it as it is.
    avxPresentMode      presentMode; // FIFO
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).
};

#ifdef _AVX_DRAW_OUTPUT_IMPL
AFX_OBJECT(_avxDrawOutput)
#else
AFX_OBJECT(afxDrawOutput)
#endif
{
    void*               udd[4]; // user-defined data    
    // endpoint
    afxVideoEndpoint    endp;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    afxError            (*lockCb)(afxDrawOutput, afxTime timeout, afxUnit*bufIdx);
    afxError            (*unlockCb)(afxDrawOutput, afxUnit cnt, afxUnit const bufIdx[]);
    afxError            (*presentCb)(afxDrawQueue dque, avxPresentation* ctrl, afxDrawOutput,afxUnit bufIdx);
    afxError            (*adjustCb)(afxDrawOutput,afxWhd const);
    afxBool             (*iddCb)(afxDrawOutput,afxUnit,void*);
    struct _afxDoutIdd* idd; // alloc'ed by the driver

    afxWhd              resolution; // Screen resolution. Absolute extent available.
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal64           wpOverHp; // physical w/h
    afxReal             refreshRate;
    afxBool             exclusive;
    afxBool             resizable;

    // canvas
    avxColorSpace       colorSpc; // raster color space. sRGB is the default.    
    avxFormat           pixelFmt; // pixel format of raster surfaces.
    avxFormat           pixelFmtDs[2]; // pixel format for depth/stencil. D24/S8/D24S8
    afxRasterUsage      bufUsage; // raster usage
    afxRasterUsage      bufUsageDs[2]; // raster usage for depth/stencil
    afxRasterFlags      bufFlags; // raster flags. What evil things we will do with it?
    afxRasterFlags      bufFlagsDs[2]; // raster flags for depth/stencil
    afxWhd              extent;
    afxReal64           wwOverHw; // window w/h
    afxBool             resizing;

    // swapchain
    afxUnit             bufCnt; // usually 2 or 3; double or triple buffered.
    avxCanvas*          canvases;
    afxInterlockedQueue freeBuffers;
    afxAtom32           presentingBufIdx;
    avxPresentMode      presentMode; // FIFO
    avxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxPresentTransform presentTransform; // NIL leaves it as it is.
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

AVX afxError _AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd whd);
AVX afxError _AvxPresentDrawOutput(afxDrawQueue dque, avxPresentation* ctrl, afxDrawOutput dout, afxUnit bufIdx);

#endif//AVX_IMPL___OUTPUT_H
