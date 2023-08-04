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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

/// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

/// Swapchains are a list of images, accessible by the operating system for display to the screen.
/// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

/// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

#ifndef AFX_DRAW_OUTPUT_H
#define AFX_DRAW_OUTPUT_H

#include "afx/draw/res/afxSurface.h"
#include "afx/core/async/afxSlock.h"

typedef enum afxEventDout
{
    AFX_EVENT_DOUT_RECON = AFX_FCC_DOUT,
    AFX_EVENT_DOUT_EXTENT,
    AFX_EVENT_DOUT_SWAPBUF,
    AFX_EVENT_DOUT_REBUF,
} afxEventDout;

typedef enum afxColorSpace
{
    AFX_COLOR_SPACE_LINEAR,
    AFX_COLOR_SPACE_SRGB
} afxColorSpace;

typedef enum afxPresentTransform
{
    //NIL // Identity
    AFX_PRESENT_TRANSFORM_FLIP_V = AFX_FLAG(0), // invert pixel grid vertically.
    AFX_PRESENT_TRANSFORM_FLIP_H = AFX_FLAG(1) // invert pixel grid horizontally.
} afxPresentTransform;

typedef enum afxPresentAlpha
{
    // NIL // Ignore. Let desktop window manager do whatever it want to do about transparency.
    AFX_PRESENT_ALPHA_OPAQUE = 1,
    AFX_PRESENT_ALPHA_PREMUL,
    AFX_PRESENT_ALPHA_POSTMUL
} afxPresentAlpha;

typedef enum afxPresentMode
{
    AFX_PRESENT_MODE_LIFO, // triple-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal single-entry queue is used to hold pending presentation requests. 
    // If the queue is full when a new presentation request is received, the new request replaces the existing entry, and any images associated with the prior entry become available for re-use by the application. 
    // One request is removed from the queue and processed during each vertical blanking period in which the queue is non-empty.

    AFX_PRESENT_MODE_FIFO, // double-buffered mode
    // Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    // Tearing cannot be observed. An internal queue is used to hold pending presentation requests. 
    // New requests are appended to the end of the queue, and one request is removed from the beginning of the queue and processed during each vertical blanking period in which the queue is non-empty. 
    // This is the only value of presentMode that is required to be supported.

    AFX_PRESENT_MODE_IMMEDIATE,
    // Specifies that the presentation engine does not wait for a vertical blanking period to update the current image, meaning this mode may result in visible tearing. 
    // No internal queuing of presentation requests is needed, as the requests are applied immediately.

} afxPresentMode;

AFX_DEFINE_STRUCT(afxDrawOutputSpecification)
{
    afxNat              drvIdx; // registered on draw system.
    afxUri const*       endpoint; // window, desktop, etc
    afxWhd              whd;
    afxPixelFormat      pixelFmt; // RGBA8; pixel format of raster surfaces. Pass zero to let driver choose the optimal format.
    afxColorSpace       colorSpc; // AFX_COLOR_SPACE_SRGB; if sRGB isn't present, fall down to LINEAR.
    afxTextureFlags     bufUsage; // RASTER; used as (color) rasterization surface.
    afxNat              bufCnt; // 2 or 3; double or triple-buffered.

    afxPresentAlpha     presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    afxPresentTransform presentTransform; // NIL; don't do any transform.
    afxPresentMode      presentMode; // FIFO; respect the sequence.
    afxBool             clipped; // TRUE; don't do off-screen draw.

    afxPixelFormat      auxDepthFmt;
    afxPixelFormat      auxStencilFmt;
    void*               udd;
};

AFX_DECLARE_STRUCT(_afxDoutVmt);

AFX_OBJECT(afxDrawOutput)
{
    afxObject               obj;
    _afxDoutVmt const*      vmt;
    void*                   idd; // implementation-defined data
#ifdef _AFX_DRAW_OUTPUT_C
    afxDrawContext          dctx; // bound context
    afxNat                  suspendCnt;
    afxSlock                suspendSlock;

    afxWhd                  extent;
    afxWhd                  resolution; // Screen resolution. Absolute extent available.
    afxReal                 wpOverHp; // physical w/h
    afxReal                 wrOverHr; // (usually screen) resolution w/h
    afxReal                 wwOverHw; // window w/h
    afxNat                  refreshRate;
    afxBool                 focused;

    afxBool                 resizable;
    afxBool                 resizing;

    // swapchain-related data
    afxFlags                flags;
    afxPixelFormat          pixelFmt; // pixel format of raster surfaces.
    afxColorSpace           colorSpc; // color space of raster surfaces. sRGB is the default.
    
    afxTextureFlags         bufUsage; // what evil things we will do with it? Usually AFX_TEX_FLAG_SURFACE_RASTER
    afxNat                  bufCnt; // usually 2 or 3; double or triple buffered.
    struct
    {
        afxSurface          surf; // afxCanvas // should have 1 fb for each swapchain raster.
        afxCanvas           canv;
    }*                      buffers;
    afxPixelFormat          auxDsFmt[2];
    afxSlock                buffersLock;
    afxBool                 bufferLockCnt;
    afxNat                  lastReqBufIdx;
    afxBool                 swapping;
    afxChain                swapchain; // display order
    
    afxPresentAlpha         presentAlpha; // consider transparency for external composing (usually on windowing system).
    afxPresentTransform     presentTransform; // NIL leaves it as it is.
    afxPresentMode          presentMode; // FIFO
    afxBool                 clipped; // usually true to don't do off-screen draw.

    afxV2d                  absCursorPos,
                            absCursorMove,
                            ndcCursorPos,
                            ndcCursorMove;

    afxString*              caption;

    void*                   udd; // user-defined data
#endif
};

AFX afxDrawDriver       AfxGetDrawOutputDriver(afxDrawOutput dout);

// Connection

AFX afxBool             AfxGetConnectedDrawOutputContext(afxDrawOutput dout, afxDrawContext *dctx);
AFX afxBool             AfxDrawOutputIsConnected(afxDrawOutput dout);
AFX afxBool             AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx, afxNat *slotIdx);
AFX afxError            AfxDisconnectDrawOutput(afxDrawOutput dout, afxNat *slotIdx);

// Extent

AFX afxError            AfxReadjustDrawOutput(afxDrawOutput dout, afxWhd const extent);
AFX afxError            AfxReadjustDrawOutputNdc(afxDrawOutput dout, afxV3d const extent);
AFX void                AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd extent); // d is always 1; it is here just for compatibility.
AFX void                AfxGetDrawOutputExtentNdc(afxDrawOutput dout, afxV3d extent); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
AFX void                AfxReadjustDrawOutputProportion(afxDrawOutput dout, afxReal physicalAspectRatio, afxWhd const resolution);

// Buffer

AFX afxBool             AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat idx, afxSurface *surf);
AFX afxBool             AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat idx, afxCanvas *canv);
AFX afxNat              AfxGetDrawOutputCapacity(afxDrawOutput dout);
AFX afxResult           AfxEnumerateDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxSurface surf[]);
AFX afxError            AfxRequestNextDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx);
AFX afxError            AfxRegenerateDrawOutputBuffers(afxDrawOutput dout);

// Utility

//AFX afxError            AfxResetDrawOutputAuxiliarBuffers(afxDrawOutput dout, afxPixelFormat depth, afxPixelFormat stencil);
//AFX afxError            AfxBuildDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, afxPixelFormat depth, afxPixelFormat stencil, afxCanvas canv[]);

#endif//AFX_DRAW_OUTPUT_H