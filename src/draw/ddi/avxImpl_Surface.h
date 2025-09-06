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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_IMPL___OUTPUT_H
#define AVX_IMPL___OUTPUT_H

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/mem/afxInterlockedQueue.h"
#include "qwadro/mem/afxSlabAllocator.h"

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
    afxString const* tag;
    afxError(*ioctlCb)(afxSurface dout, afxUnit code, va_list ap);
    afxError(*adjustCb)(afxSurface dout, afxRect const* rc, afxBool fse);
    afxError(*regenCb)(afxSurface dout, afxBool build);
    afxError(*presentCb)(afxDrawQueue dque, avxPresentation* pres);
    afxError(*lockCb)(afxSurface dout, afxUnit64 timeout, afxMask exuMask, afxUnit *bufIdx, avxFence signal);
    afxError(*unlockCb)(afxSurface dout, afxUnit bufIdx);
    afxError(*modeSetCb)(afxSurface dout, avxModeSetting const* mode);
    afxError(*presOnDpuCb)(void* dpu, avxPresentation* pres);
};
#endif

#ifdef _AVX_SURFACE_C

AFX_DEFINE_STRUCT(afxVideoEndpoint)
{
    afxUri32            urn;
    afxSurfaceCaps   caps;    
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxBool             resizable;
    afxReal             refreshRate;
    afxWarp              res; // Screen resolution. Absolute extent available.
    union
    {
        afxObject       obj;
        afxWindow       wnd;
    };
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    avxVideoAlpha       presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxVideoTransform   presentTransform; // NIL leaves it as it is.
    avxPresentFlags     presentMode; // FIFO
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).
};

AFX_DEFINE_STRUCT(_avxSurfaceBuffer)
{
    afxBool     locked;
    avxCanvas   canv;
    avxRaster   ras;
    afxUnit     rcCnt;
    afxRect     rects[4];
};

#ifdef _AVX_SURFACE_IMPL
AFX_OBJECT(_avxSurface)
#else
AFX_OBJECT(afxSurface)
#endif
{
    _avxDoutDdi const*  pimpl;
    afxString           tag;
    void*               udd[4]; // user-defined data    
    // ENDPOINT
    afxVideoEndpoint    endp;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    struct _afxDoutIdd* idd; // alloc'ed by the driver

    avxRange            resolution; // Screen resolution. Absolute extent available.
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal64           wpOverHp; // physical w/h
    afxReal             refreshRate;

    // CANVAS/SURFACE
    // raster color space. sRGB is the default.    
    avxColorSpace       colorSpc;
    // format for color, depth and stencil surfaces, respectively.
    // raster usage for color, depth and stencil, respectively.
    // raster flags for color, depth and stencil, respectively.
    avxCanvasConfig     ccfg;
    // The area origin is used when the output context is not a child or confined; 
    // for example, drawing directly to a common shared display. Think it like the blit source.
    // If the output is contained in (for example) an window, it usually is zero or a offset from the frame,
    // if both shares the same implementation-defined buffer.
    afxRect             area;
    afxReal64           wwOverHw; // window w/h
    afxBool             resizable;
    afxBool             resizing;

    // SWAPCHAIN
    afxUnit             bufCnt; // usually 2 or 3; double or triple buffered.
    _avxSurfaceBuffer*     buffers;
    afxInterlockedQueue freeBuffers;
    afxAtom32           lastPresentedBufIdx;
    afxRect             dstArea;
    afxBool             persistBlit;
    afxBool             fse;
    avxPresentFlags     presentMode; // FIFO
    avxVideoAlpha       presentAlpha; // consider transparency for external composing (usually on windowing system).
    avxVideoTransform   presentTransform; // NIL leaves it as it is.
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).    
    
    afxUnit             suspendCnt;
    afxFutex            suspendSlock;
    afxAtom32           submCnt;
    afxCondition        idleCnd;
    afxMutex            idleCndMtx;

    afxClock            startClock;
    afxClock            lastClock;
    afxUnit             outNo;
    afxUnit             outRate; // outputs per second
    afxClock            outCntResetClock;

    afxClock            prevBufReqTime;
    afxInt              bufReqPerSec;
};
#endif//_AVX_SURFACE_C

AVX afxClassConfig const _AVX_DOUT_CLASS_CONFIG;
AVX _avxDoutDdi const _AVX_DOUT_DDI;

AVX afxError _AvxDoutImplIoctlCb(afxSurface dout, afxUnit code, va_list ap);
AVX afxError _AvxDoutImplAdjustCb(afxSurface dout, afxRect const* area, afxBool fse);
AVX afxError _AvxDoutImplRegenBuffers(afxSurface dout, afxBool build);
AVX afxError _AvxDquePresentBuffers(afxDrawQueue dque, afxUnit cnt, avxPresentation presentations[]);


#endif//AVX_IMPL___OUTPUT_H
