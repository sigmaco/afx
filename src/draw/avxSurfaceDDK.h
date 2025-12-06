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

#ifndef AFX_SURFACE_DDK_H
#define AFX_SURFACE_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

/*
    Blit: Transfers or copies pixel data from one area to another within or between buffers. It’s used for operations like rendering images or updating parts of the screen.
    Flip: Swaps the front and back buffers in double buffering to update the display with a new frame, reducing flickering and improving performance.
    Swap: Refers to the exchange of buffers in multiple buffering strategies (e.g., double or triple buffering) to update the screen, often used to manage buffer visibility and performance.
    Present: Displays the contents of the current frame buffer on the screen, making the rendered image visible to the user.
*/

AFX_DECLARE_STRUCT(_avxIddDout);

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDdiDout);
#else
AFX_DEFINE_STRUCT(_avxDdiDout)
{
    afxString const* tag;
    afxError(*ioctlCb)(afxSurface dout, afxUnit code, va_list ap);
    afxError(*adjustCb)(afxSurface dout, afxRect const* rc, afxBool fse);
    afxError(*regenCb)(afxSurface dout, afxBool build);
    afxError(*presentCb)(afxDrawQueue dque, avxPresentation* pres);
    afxError(*lockCb)(afxSurface dout, afxMask exuMask, avxFence signal, afxUnit64 timeout, afxUnit *bufIdx);
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

AFX_DEFINE_STRUCT(_avxSurfaceSwap)
{
    afxBool         locked;
    afxMask         exuMask;
    avxFence        fenc;
    afxUnit64       fencNextValue;
    // Locked canvas is an attempt to avoid crashes and busy-waits while readjustments.
    //avxCanvas       lockedCanv;
    avxCanvas       canv;
    avxRaster       ras;
    afxLayeredRect  bounds;
    afxUnit         updRcCnt;
    afxRect         updRcs[4];
};

#ifdef _AVX_SURFACE_IMPL
AFX_OBJECT(_avxSurface)
#else
AFX_OBJECT(afxSurface)
#endif
{
    _avxDdiDout const*  ddi;
    _avxIddDout*        idd; // alloc'ed by the driver
    // User-defined data.
    void*               udd[4];
    // Debugging tag.
    afxString           tag;

    // ENDPOINT
    afxVideoEndpoint    endp;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;

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
    afxUnit             swapCnt; // usually 2 or 3; double or triple buffered.
    _avxSurfaceSwap*    swaps;
    afxInterlockedQueue freeBuffers;
    afxAtom32           lastPresentedBufIdx;
    // Destination area is different because it can be used to place presentation to non-desktop display.
    // If the surface is a window's surface (aka client area), thus is confined, .dstArea is equal to .area.
    afxRect             dstArea;
    // Source area is different because it can be used to crop on presentation from greater canvas.
    // Usually .srcArea is equal to .area. It is up to the user dealing with it.
    afxRect             srcArea;
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

AVX afxClassConfig const _AVX_CLASS_CONFIG_DOUT;
AVX _avxDdiDout const _AVX_DDI_DOUT;

AVX afxError _AvxDquePresentBuffers(afxDrawQueue dque, afxUnit cnt, avxPresentation presentations[]);

AVX afxError _AvxDoutImplIoctlCb(afxSurface dout, afxUnit code, va_list ap);
AVX afxError _AvxDoutImplAdjustCb(afxSurface dout, afxRect const* area, afxBool fse);
AVX afxError _AvxDoutImplRegenBuffers(afxSurface dout, afxBool build);
AVX afxError _AvxDoutImplLockBufferCb(afxSurface dout, afxMask exuMask, avxFence signal, afxUnit64 timeout, afxUnit* bufIdx);
AVX afxError _AvxDoutImplUnlockBufferCb(afxSurface dout, afxUnit bufIdx);

AVX afxError _AvxDoutAdjustNormalized(afxSurface dout, afxV3d const whd);
AVX void _AvxDoutGetExtentNormalized(afxSurface dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.


#endif//AFX_SURFACE_DDK_H
