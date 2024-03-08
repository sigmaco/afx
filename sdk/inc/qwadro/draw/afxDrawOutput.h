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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

/// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

/// Swapchains are a list of images, accessible by the operating system for display to the screen.
/// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

/// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

#ifndef AFX_DRAW_OUTPUT_H
#define AFX_DRAW_OUTPUT_H

#include "qwadro/draw/afxDrawDefs.h"

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
    afxPresentScaling_ONE_TO_ONE    = AfxGetBitOffset(0),

    /// specifies that the swapchain image will be minified or magnified such that at least one of the resulting width or height is equal to the corresponding surface dimension, and the other resulting dimension is less than or equal to the corresponding surface dimension, with the aspect ratio of the resulting image being identical to that of the original swapchain image.
    afxPresentScaling_ASPECT_RATIO  = AfxGetBitOffset(1),

    /// specifies that the swapchain image will be minified or magnified such that the resulting image dimensions are equal to those of the surface.
    afxPresentScaling_STRETCH       = AfxGetBitOffset(2)
} afxPresentScaling;

typedef enum afxPresentTransform
{
    //NIL // Identity
    afxPresentTransform_FLIP_V  = AfxGetBitOffset(0), // invert pixel grid vertically.
    afxPresentTransform_FLIP_H  = AfxGetBitOffset(1) // invert pixel grid horizontally.
} afxPresentTransform;

typedef enum afxPresentAlpha
{
    // NIL // Ignore. Let desktop window manager do whatever it want to do about transparency.
    afxPresentAlpha_OPAQUE      = AfxGetBitOffset(0),
    afxPresentAlpha_PREMUL      = AfxGetBitOffset(1),
    afxPresentAlpha_POSTMUL     = AfxGetBitOffset(2)
} afxPresentAlpha;

typedef enum afxPresentMode
{
    afxPresentMode_LIFO         = AfxGetBitOffset(0), // triple-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal single-entry queue is used to hold pending presentation requests. 
    // If the queue is full when a new presentation request is received, the new request replaces the existing entry, and any images associated with the prior entry become available for re-use by the application. 
    // One request is removed from the queue and processed during each vertical blanking period in which the queue is non-empty.

    afxPresentMode_FIFO         = AfxGetBitOffset(1), // double-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal queue is used to hold pending presentation requests. 
    // New requests are appended to the end of the queue, and one request is removed from the beginning of the queue and processed during each vertical blanking period in which the queue is non-empty. 
    // This is the only value of presentMode that is required to be supported.

    afxPresentMode_IMMEDIATE    = AfxGetBitOffset(2),
    // Specifies that the presentation engine does not wait for a vertical blanking period to update the current image, meaning this mode may result in visible tearing. 
    // No internal queuing of presentation requests is needed, as the requests are applied immediately.

} afxPresentMode;

AFX_DEFINE_STRUCT(afxDrawOutputConfig)
{
    afxUri const*       endpoint; // window, desktop, etc
    afxWhd              whd;
    afxPixelFormat      pixelFmt; // RGBA8; pixel format of raster surfaces. Pass zero to let driver choose the optimal format.
    afxColorSpace       colorSpc; // afxColorSpace_SRGB; if sRGB isn't present, fall down to LINEAR.
    afxRasterUsage      bufUsage; // DRAW; used as (color) target surface for rasterization.
    afxRasterFlags      bufFlags;
    afxNat              bufCnt; // 2 or 3; double or triple-buffered.
    afxBool             dontManageCanvases;

    afxPresentAlpha     presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    afxPresentTransform presentTransform; // NIL; don't do any transform.
    afxPresentMode      presentMode; // FIFO; respect the sequence.
    afxBool             notClipped; // FALSE; don't do off-screen draw.

    afxPixelFormat      auxDsFmt[2]; // D24/S8/D24S8; pixel format of raster surfaces. Pass zero to disable depth and/or stencil.
    afxRasterUsage      auxDsUsage[2];
    afxRasterFlags      auxDsFlags[2];
    void*               udd;
};

AFX_DECLARE_STRUCT(_afxDoutVmt);

//typedef afxObject afxDrawOutput;

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_OUTPUT_C
#ifndef _AFX_DRAW_OUTPUT_IMPL
AFX_OBJECT(afxDrawOutput)
#else
struct afxBaseDrawOutput
#endif
{
    afxLinkage          endpoint;
    afxDrawContext      dctx; // bound context
    afxNat              suspendCnt;
    afxSlock            suspendSlock;
    
    afxWhd              whd;
    afxWhd              resolution; // Screen resolution. Absolute extent available.
    afxReal             wpOverHp; // physical w/h
    afxReal             wrOverHr; // (usually screen) resolution w/h
    afxReal             wwOverHw; // window w/h
    afxNat              refreshRate;
    afxBool             focused;

    afxBool             resizable;
    afxBool             resizing;

    // swapchain-related data
    afxFlags            flags;
    afxPixelFormat      pixelFmt; // pixel format of raster surfaces.
    afxColorSpace       colorSpc; // color space of raster surfaces. sRGB is the default.
    
    afxRasterUsage      rasUsage; // always has afxRasterUsage_DRAW
    afxRasterFlags      rasFlags; // what evil things we will do with it?
    afxNat              bufCnt; // usually 2 or 3; double or triple buffered.
    struct
    {
        //afxRaster       ras; // afxCanvas // should have 1 fb for each swapchain raster.
        afxCanvas       canv;
        afxSemaphore    readySem;
        afxBool         booked;
    }*                  buffers;
    afxPixelFormat      auxDsFmt[2]; // D24/S8/D24S8
    afxRasterUsage      auxDsUsage[2];
    afxRasterFlags      auxDsFlags[2];
    afxSlock            buffersLock;
    afxBool             bufferLockCnt;
    afxNat              lastReqBufIdx;
    afxBool             swapping;
    afxChain            swapchain; // display order
    
    afxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    afxPresentTransform presentTransform; // NIL leaves it as it is.
    afxPresentMode      presentMode; // FIFO
    afxBool             notClipped; // usually false to don't do off-screen draw on compositor-based endpoints (aka window).

    afxV2d              absCursorPos,
                        absCursorMove,
                        ndcCursorPos,
                        ndcCursorMove;

    afxRestring   caption;

    afxError            (*reqCb)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);

    void*               udd; // user-defined data
};
#endif
#endif

AVX afxDrawDevice       AfxGetDrawOutputDevice(afxDrawOutput dout);

// Connection

AVX afxBool             AfxDrawOutputIsOnline(afxDrawOutput dout);
AVX afxDrawContext      AfxGetDrawOutputContext(afxDrawOutput dout);
AVX afxError            AfxDisconnectDrawOutput(afxDrawOutput dout);
AVX afxBool             AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx);

// Extent

AVX afxError            AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd);
AVX afxError            AfxAdjustDrawOutputNormalized(afxDrawOutput dout, afxV3d const whd);
AVX void                AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd); // d is always 1; it is here just for compatibility.
AVX void                AfxGetDrawOutputNormalizedExtent(afxDrawOutput dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
AVX void                AfxAdjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution);

// Buffer

AVX afxNat              AfxGetDrawOutputCapacity(afxDrawOutput dout);
AVX afxNat              AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[]);
AVX afxNat              AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat first, afxNat cnt, afxCanvas canvases[]);
AVX afxNat              AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[]);
AVX afxError            AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx);
AVX afxError            AfxRegenerateDrawOutputBuffers(afxDrawOutput dout);

// Utility

//AVX afxError            AfxResetDrawOutputAuxiliarBuffers(afxDrawOutput dout, afxPixelFormat depth, afxPixelFormat stencil);
//AVX afxError            AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxNat auxSurfCnt, afxSurfaceConfig const auxSurfs[], afxCanvas canv[]);

#endif//AFX_DRAW_OUTPUT_H
