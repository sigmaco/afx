/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

// Swapchains are a list of images, accessible by the operating system for display to the screen.
// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double - buffer or triple - buffer rendering.

// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

#ifndef AFX_DRAW_OUTPUT_H
#define AFX_DRAW_OUTPUT_H

#include "res/afxSurface.h"

AFX_DEFINE_HANDLE(afxDrawOutput);

typedef enum afxColorSpace
{
    AFX_COLOR_SPC_SRGB,
    AFX_COLOR_SPC_LINEAR
} afxColorSpace;

typedef enum afxPresentTransform
{
    AFX_PRESENT_TRANSFORM_FLIP_V = AFX_FLAG(0), // invert pixel grid vertically.
    AFX_PRESENT_TRANSFORM_FLIP_H = AFX_FLAG(1) // invert pixel grid horizontally.
} afxPresentTransform;

typedef enum afxPresentCompositing
{
    AFX_PRESENT_IGNORE_ALPHA,
    AFX_PRESENT_PREMUL_ALPHA,
    AFX_PRESENT_POSTMUL_ALPHA
} afxPresentCompositing;

typedef enum afxPresentMode
{
    AFX_PRESENT_MODE_FIFO, // double-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal queue is used to hold pending presentation requests. 
    // New requests are appended to the end of the queue, and one request is removed from the beginning of the queue and processed during each vertical blanking period in which the queue is non-empty. 
    // This is the only value of presentMode that is required to be supported.
    
    AFX_PRESENT_MODE_IMMEDIATE,
    // Specifies that the presentation engine does not wait for a vertical blanking period to update the current image, meaning this mode may result in visible tearing. 
    // No internal queuing of presentation requests is needed, as the requests are applied immediately.
    
    AFX_PRESENT_MODE_MAILBOX, // triple-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal single-entry queue is used to hold pending presentation requests. 
    // If the queue is full when a new presentation request is received, the new request replaces the existing entry, and any images associated with the prior entry become available for re-use by the application. 
    // One request is removed from the queue and processed during each vertical blanking period in which the queue is non-empty.
} afxPresentMode;

AFX_DEFINE_STRUCT(afxDrawOutputSpecification)
{
    afxUri const            *endpoint; // window, desktop, etc
    afxPixelFormat          pixelFmt; // RGBA8; pixel format of raster surfaces.
    afxColorSpace           colorSpc; // AFX_COLOR_SPC_SRGB; if sRGB isn't present, fall down to LINEAR.
    afxTextureUsage         bufUsage; // RASTER; used as (color) rasterization surface.
    afxNat                  bufCnt; // 2 or 3; double or triple-buffered.

    afxPresentCompositing   compositeAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    afxPresentTransform     presentTransform; // NIL; don't do any transform.
    afxPresentMode          presentMode; // FIFO; respect the sequence.
    afxBool                 clipped; // TRUE; don't do off-screen draw.
};

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxDrawOutput)
{
    afxObject               obj;
};

#endif

AFX void*                   AfxDrawOutputGetContext(afxDrawOutput dout);
AFX void*                   AfxDrawOutputGetDriver(afxDrawOutput dout);
AFX void*                   AfxDrawOutputGetDrawSystem(afxDrawOutput dout);

AFX afxError                AfxDrawOutputSetExtent(afxDrawOutput dout, afxWhd const extent);
AFX afxError                AfxDrawOutputSetExtentNdc(afxDrawOutput dout, afxV3d const extent);
AFX afxNat*                 AfxDrawOutputGetExtent(afxDrawOutput dout, afxWhd extent); // d is always 1; it is here just for compatibility.
AFX afxReal*                AfxDrawOutputGetExtentNdc(afxDrawOutput dout, afxV3d extent); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.

AFX afxSurface              AfxDrawOutputGetBuffer(afxDrawOutput dout, afxNat idx);
AFX afxResult               AfxDrawOutputForEachBuffer(afxDrawOutput dout, afxResult(*f)(afxSurface, void*), void *data);
AFX afxError                AfxDrawOutputRequestBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx);

#endif//AFX_DRAW_OUTPUT_H