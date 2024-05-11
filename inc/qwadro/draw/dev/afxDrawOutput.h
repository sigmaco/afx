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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

/// Swapchains are a list of images, accessible by the operating system for display to the screen.
/// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

/// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

#ifndef AVX_DRAW_OUTPUT_H
#define AVX_DRAW_OUTPUT_H

#include "qwadro/draw/dev/afxDrawBridge.h"

typedef enum afxEventDout
{
    AFX_EVENT_DOUT_RECON = afxFcc_DOUT,
    AFX_EVENT_DOUT_EXTENT,
    AFX_EVENT_DOUT_SWAPBUF,
    AFX_EVENT_DOUT_REBUF,
} afxEventDout;

typedef enum afxPresentScaling
/// tmask specifying presentation scaling methods
{
    /// specifies that no scaling occurs, and pixels in the swapchain image are mapped to one and only one pixel in the surface. The mapping between pixels is defined by the chosen presentation gravity.
    afxPresentScaling_ONE_TO_ONE    = AFX_BIT_OFFSET(0),

    /// specifies that the swapchain image will be minified or magnified such that at least one of the resulting width or height is equal to the corresponding surface dimension, and the other resulting dimension is less than or equal to the corresponding surface dimension, with the aspect ratio of the resulting image being identical to that of the original swapchain image.
    afxPresentScaling_ASPECT_RATIO  = AFX_BIT_OFFSET(1),

    /// specifies that the swapchain image will be minified or magnified such that the resulting image dimensions are equal to those of the surface.
    afxPresentScaling_STRETCH       = AFX_BIT_OFFSET(2)
} afxPresentScaling;

typedef enum afxPresentTransform
{
    //NIL // Identity
    afxPresentTransform_FLIP_V  = AFX_BIT_OFFSET(0), // invert pixel grid vertically.
    afxPresentTransform_FLIP_H  = AFX_BIT_OFFSET(1) // invert pixel grid horizontally.
} afxPresentTransform;

typedef enum afxPresentAlpha
{
    // NIL The way in which the presentation engine treats the alpha component in the images is unknown to the SIGMA GL/2 API. Instead, the application is responsible for setting the composite alpha blending mode using native window system commands. If the application does not set the blending mode using native window system commands, then a platform-specific default will be used.
    afxPresentAlpha_OPAQUE      = AFX_BIT_OFFSET(0), // The alpha component, if it exists, of the images is ignored in the compositing process. Instead, the image is treated as if it has a constant alpha of 1.0.
    afxPresentAlpha_PREMUL      = AFX_BIT_OFFSET(1), // The alpha component, if it exists, of the images is respected in the compositing process. The non-alpha components of the image are expected to already be multiplied by the alpha component by the application.
    afxPresentAlpha_POSTMUL     = AFX_BIT_OFFSET(2) // The alpha component, if it exists, of the images is respected in the compositing process. The non-alpha components of the image are not expected to already be multiplied by the alpha component by the application; instead, the compositor will multiply the non-alpha components of the image by the alpha component during compositing.
} afxPresentAlpha;

typedef enum afxPresentMode
{
    afxPresentMode_LIFO, // aka triple-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal single-entry queue is used to hold pending presentation requests. 
    // If the queue is full when a new presentation request is received, the new request replaces the existing entry, and any images associated with the prior entry become available for re-use by the application. 
    // One request is removed from the queue and processed during each vertical blanking period in which the queue is non-empty.

    afxPresentMode_FIFO, // aka V-sync'ed double-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal queue is used to hold pending presentation requests. 
    // New requests are appended to the end of the queue, and one request is removed from the beginning of the queue and processed during each vertical blanking period in which the queue is non-empty. 
    // This is the only value of presentMode that is required to be supported.

    afxPresentMode_IMMEDIATE,
    // Specifies that the presentation engine does not wait for a vertical blanking period to update the current image, meaning this mode may result in visible tearing. 
    // No internal queuing of presentation requests is needed, as the requests are applied immediately.

} afxPresentMode;

typedef afxBool(*afxPresentNotifier)(afxObject receiver, afxNat);

AFX_DEFINE_STRUCT(afxDrawOutputConfig)
{
    // canvas
    afxWhd              whd;
    afxColorSpace       colorSpc; // afxColorSpace_SRGB; if sRGB isn't present, fall down to LINEAR.
    afxPixelFormat      pixelFmt; // RGBA8; pixel format of raster surfaces. Pass zero to let driver choose the optimal format.
    afxPixelFormat      pixelFmtDs[2]; // D24/S8/D24S8; pixel format of raster surfaces. Pass zero to disable depth and/or stencil.
    afxRasterUsage      bufUsage; // DRAW; used as (color) target surface for rasterization.
    afxRasterUsage      bufUsageDs[2];
    afxRasterFlags      bufFlags;
    afxRasterFlags      bufFlagsDs[2];

    // swapchain
    afxNat              bufCnt; // 2 or 3; double or triple-buffered.
    afxBool             dontManageCanvases;

    // endpoint
    afxPresentAlpha     presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    afxPresentNotifier  endpointNotifyFn;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    afxPresentTransform presentTransform; // NIL; don't do any transform.
    afxPresentMode      presentMode; // FIFO; respect the sequence.
    afxBool             doNotClip; // FALSE; don't do off-screen draw.
    void*               udd;

#ifdef AFX_OS_WIN
    struct
    {
        void*           hwnd;
        void*           hinstance;
    }                   w32;
#endif
};

AFX_DEFINE_STRUCT(afxDrawOutputCaps)
{
    afxNat              minBufCnt;
    afxNat              maxBufCnt;
    afxNat              currExtent[2];
    afxWhd              minWhd;
    afxWhd              maxWhd;
    afxNat              maxLayerCnt;
    afxPresentTransform supportedTransforms;
    afxPresentTransform currTransform;
    afxPresentAlpha     supportedCompositeAlpha;
    afxRasterFlags      supportedUsageFlags;
    afxPresentScaling   supportedScaling;
    afxNat              supportedModeCnt;
    afxPresentMode      supportedModes;
};

AFX_DEFINE_STRUCT(afxDrawOutputEndpoint)
{
    afxLinkage          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
    afxDrawOutputCaps   caps;
};

#ifdef _AVX_DRAW_C
#ifdef _AVX_DRAW_OUTPUT_C

AFX_DEFINE_STRUCT(afxDrawOutputLink)
{
    afxChain a;
};

AFX_OBJECT(afxDrawOutput)
{
    afxLinkage          ddev;
    afxLinkage          dctx; // bound context

    // canvas
    afxWhd              whd;
    afxColorSpace       colorSpc; // raster color space. sRGB is the default.    
    afxPixelFormat      pixelFmt; // pixel format of raster surfaces.
    afxPixelFormat      pixelFmtDs[2]; // pixel format for depth/stencil. D24/S8/D24S8
    afxRasterUsage      bufUsage; // raster usage
    afxRasterUsage      bufUsageDs[2]; // raster usage for depth/stencil
    afxRasterFlags      bufFlags; // raster flags. What evil things we will do with it?
    afxRasterFlags      bufFlagsDs[2]; // raster flags for depth/stencil

    // swapchain
    afxSlock            buffersLock;
    afxAtom32           lockedBufCnt;
    afxNat              bufCnt; // usually 2 or 3; double or triple buffered.
    afxAtom32           lastLockedBufIdx;
    afxAtom32           lastUnlockedBufIdx;
    afxAtom32           presentingBufIdx;
    afxChain            swapchain; // display order
    struct
    {
        //afxRaster       ras; // afxCanvas // should have 1 fb for each swapchain raster.
        afxCanvas       canv;
        afxSemaphore    readySem;
        afxAtom32       booked;
        afxSlock        lck;
    }*buffers;
    afxError            (*lockCb)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
    afxError            (*unlockCb)(afxDrawOutput, afxNat cnt, afxNat const bufIdx[]);
    
    // endpoint
    afxReal64           wpOverHp; // physical w/h
    afxReal64           wrOverHr; // (usually screen) resolution w/h
    afxReal64           wwOverHw; // window w/h
    afxBool             resizing;
    afxBool             resizable;
    afxNat              refreshRate;
    afxWhd              res; // Screen resolution. Absolute extent available.
    afxObject           endpointNotifyObj; // must ensure life of draw output
    afxPresentNotifier  endpointNotifyFn;
    afxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    afxPresentTransform presentTransform; // NIL leaves it as it is.
    afxPresentMode      presentMode; // FIFO
    afxBool             doNotClip; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).
    void*               udd; // user-defined data

    afxNat              suspendCnt;
    afxSlock            suspendSlock;


    afxAtom32           reconnecting;

    afxClock            startClock;
    afxClock            lastClock;
    afxNat              outNo;
    afxNat              outRate; // outputs per second
    afxClock            outCntResetClock;

    afxBool             (*getIddCb)(afxDrawOutput,afxNat,void*);
    struct _afxDoutIdd* idd;

#ifdef _AVX_DRAW_OUTPUT_IMPL
#ifdef AFX_OS_WIN
    struct
    {
        HINSTANCE       hInst;
        HWND            hWnd;
        HDC             hDc;
        int             dcPxlFmt;
        int             dcPxlFmtBkp;
        afxBool         isWpp;
    }                   w32;
#endif//AFX_OS_WIN
#endif//_AVX_DRAW_OUTPUT_IMPL
};
#endif//_AVX_DRAW_OUTPUT_C
#endif//_AVX_DRAW_C


AVX afxDrawDevice   AfxGetDrawOutputDevice(afxDrawOutput dout);

AVX afxError        AfxGetDrawOutputIdd(afxDrawOutput dout, afxNat code, void* dst);

// Connection

AVX afxBool         AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx);
AVX afxError        AfxDisconnectDrawOutput(afxDrawOutput dout);
AVX afxBool         AfxGetDrawOutputContext(afxDrawOutput dout, afxDrawContext* context);

// Extent

AVX void            AfxGetDrawOutputResolution(afxDrawOutput dout, afxWhd res, afxNat* refreshRate);
AVX void            AfxGetDrawOutputAspectRatio(afxDrawOutput dout, afxReal64* wpOverHp, afxReal64* wrOverHr, afxReal64* wwOverHw);
AVX void            AfxAdaptDrawOutputResolution(afxDrawOutput dout, afxWhd const res, afxNat refreshRate, afxReal64 physAspectRatio);

AVX afxError        AfxAdjustDrawOutputNdc(afxDrawOutput dout, afxV3d const whd);
AVX afxError        AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd);

AVX void            AfxGetDrawOutputExtentNdc(afxDrawOutput dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
AVX void            AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd); // d is always 1; it is here just for compatibility.

// Buffer

AVX afxNat          AfxEnumerateDrawOutputCanvases(afxDrawOutput dout, afxNat baseBuf, afxNat bufCnt, afxCanvas canvases[]);
AVX afxNat          AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat baseBuf, afxNat bufCnt, afxRaster rasters[]);

AVX afxError        AfxRedoDrawOutputBuffers(afxDrawOutput dout);
AVX afxNat          AfxCountDrawOutputBuffers(afxDrawOutput dout);

AVX afxError        AfxLockDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx);


// Se não há garantia de presentação no tempo de retorno da função nem controle sobre a fila, não seria melhor submeter direto pelo afxDrawOutput e esperar pelo semáforo?

AVX afxNat          AfxPresentDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxSemaphore wait);
AVX afxNat          AfxStampDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxSemaphore wait, afxV2d const origin, afxString const* caption);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireDrawOutput(afxNat ddevId, afxDrawOutputConfig const* cfg, afxDrawOutput* output);

AVX afxError        AfxOpenDrawOutput(afxNat ddevId, afxUri const* endpoint, afxDrawOutputConfig const* cfg, afxDrawOutput* output); // file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.

#endif//AVX_DRAW_OUTPUT_H
