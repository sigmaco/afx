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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

/// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

/// Swapchains are a list of images, accessible by the operating system for display to the screen.
/// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

/// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

#ifndef AFX_DRAW_OUTPUT_H
#define AFX_DRAW_OUTPUT_H

#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/core/afxSlock.h"

typedef enum afxEventDout
{
    AFX_EVENT_DOUT_RECON = afxFcc_DOUT,
    AFX_EVENT_DOUT_EXTENT,
    AFX_EVENT_DOUT_SWAPBUF,
    AFX_EVENT_DOUT_REBUF,
} afxEventDout;

typedef enum afxColorSpace
{
    afxColorSpace_LINEAR    = AfxGetBitOffset(0),
    afxColorSpace_SRGB      = AfxGetBitOffset(1)
} afxColorSpace;

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
    afxRasterFlags      bufUsage; // RASTER; used as (color) rasterization surface.
    afxNat              bufCnt; // 2 or 3; double or triple-buffered.
    afxBool             dontManageCanvases;

    afxPresentAlpha     presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    afxPresentTransform presentTransform; // NIL; don't do any transform.
    afxPresentMode      presentMode; // FIFO; respect the sequence.
    afxBool             clipped; // TRUE; don't do off-screen draw.

    afxPixelFormat      auxDsFmt[2];
    void*               udd;
};

AFX_DECLARE_STRUCT(_afxDoutVmt);

//typedef afxObject afxDrawOutput;

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_OUTPUT_C
AFX_OBJECT(afxDrawOutput)
#else
struct afxBaseDrawOutput
#endif
{
    afxError            (*reqCb)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
    afxError            (*flushCb)(afxDrawOutput, afxTime timeout);
    afxError            (*procCb)(afxDrawOutput, afxNat thrUnitIdx);
    afxLinkage          dctx; // bound context
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
    
    afxRasterFlags      rasUsage; // what evil things we will do with it? Usually AFX_TEX_FLAG_SURFACE_RASTER
    afxNat              bufCnt; // usually 2 or 3; double or triple buffered.
    struct
    {
        //afxRaster       ras; // afxCanvas // should have 1 fb for each swapchain raster.
        afxCanvas       canv;
        afxBool         booked;
    }*                  buffers;
    afxPixelFormat      auxDsFmt[2];
    afxSlock            buffersLock;
    afxBool             bufferLockCnt;
    afxNat              lastReqBufIdx;
    afxBool             swapping;
    afxChain            swapchain; // display order
    
    afxPresentAlpha     presentAlpha; // consider transparency for external composing (usually on windowing system).
    afxPresentTransform presentTransform; // NIL leaves it as it is.
    afxPresentMode      presentMode; // FIFO
    afxBool             clipped; // usually true to don't do off-screen draw.

    afxV2d              absCursorPos,
                        absCursorMove,
                        ndcCursorPos,
                        ndcCursorMove;

    afxBufferedString   caption;

    void*               udd; // user-defined data
};
#endif

AFX afxDrawDevice       AfxGetDrawOutputDevice(afxDrawOutput dout);

// Connection

AFX afxBool             AfxDrawOutputIsConnected(afxDrawOutput dout);
AFX afxDrawContext      AfxGetDrawOutputContext(afxDrawOutput dout);
AFX afxBool             AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx);
AFX afxError            AfxDisconnectDrawOutput(afxDrawOutput dout);

// Extent

AFX afxError            AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const whd);
AFX afxError            AfxReadjustDrawOutputNormalized(afxDrawOutput dout, afxV3d const whd);
AFX void                AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd); // d is always 1; it is here just for compatibility.
AFX void                AfxGetDrawOutputNormalizedExtent(afxDrawOutput dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
AFX void                AfxReadjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution);

// Buffer

AFX afxNat              AfxGetDrawOutputSurface(afxDrawOutput dout, afxNat baseBufIdx, afxNat bufCnt, afxRaster rasters[]);
AFX afxNat              AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat baseBufIdx, afxNat bufCnt, afxCanvas canvases[]);
AFX afxNat              AfxGetDrawOutputCapacity(afxDrawOutput dout);
AFX afxNat              AfxEnumerateDrawOutputSurfaces(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[]);
AFX afxError            AfxRequestDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx);
AFX afxError            AfxRegenerateDrawOutputBuffers(afxDrawOutput dout);

// Utility

//AFX afxError            AfxResetDrawOutputAuxiliarBuffers(afxDrawOutput dout, afxPixelFormat depth, afxPixelFormat stencil);
//AFX afxError            AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxNat auxSurfCnt, afxSurfaceConfig const auxSurfs[], afxCanvas canv[]);

#endif//AFX_DRAW_OUTPUT_H
