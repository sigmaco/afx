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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

/// Swapchains are a list of images, accessible by the operating system for display to the screen.
/// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

/// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

/*
    Draw output refers to the result of a drawing operation in computing or graphics, where data is visually represented on a screen or other display medium. 
    It is the visual representation produced by drawing commands in a graphical application, software, or system.

    In graphics programming, draw output refers to the image or graphical content rendered by a drawing function or command.
    If a program draws a circle, the draw output would be the circle as it appears on the screen.

    In video games and interactive media, draw output is the rendered frames or scenes displayed to the player.
    The graphical representation of game environments, characters, and actions.
    The continuous output of images that create the visual experience in real-time.
*/

/*
    A video endpoint refers to a device or system component that acts as the final destination or interface for video signals or streams in a video communication or distribution setup. 
    It can be part of a video conferencing system, media streaming setup, or any system where video is transmitted and received.

    In media streaming, a video endpoint is the device or software that receives and displays video content from a streaming source.
    In networking and video surveillance systems, a video endpoint is a device that either produces or consumes video data within the network.
    In the context of video communication protocols, a video endpoint is a device or software that interfaces with the video network to send or receive video data.
    In television and display systems, a video endpoint is the device that receives video input and displays it on a screen.
    Managing video endpoints involves configuring and monitoring these devices to ensure proper functioning and integration within a video system.
*/

#ifndef AVX_DRAW_OUTPUT_H
#define AVX_DRAW_OUTPUT_H

#include "qwadro/inc/draw/afxDrawBridge.h"

typedef enum avxPresentScaling
/// mask specifying presentation scaling methods.
{
    /// Do not scale.
    /// Pixels are mapped to one and only one pixel in the surface.
    avxPresentScaling_ONE_TO_ONE    = AFX_BIT(0),

    /// Minify or magnify such that at least one of the resulting width or height is equal to the corresponding surface dimension, 
    /// and the other resulting dimension is less than or equal to the corresponding surface dimension, 
    /// with the aspect ratio of the resulting image being identical to that of the original swapchain image.
    avxPresentScaling_ASPECT_RATIO  = AFX_BIT(1),

    /// minify or magnify such that the resulting dimensions are equal to those of the surface.
    avxPresentScaling_STRETCH       = AFX_BIT(2)
} avxPresentScaling;

typedef enum avxPresentTransform
{
    /// Invert pixel grid vertically.
    avxPresentTransform_FLIP_V  = AFX_BIT(0),
    
    /// Invert pixel grid horizontally.
    avxPresentTransform_FLIP_H  = AFX_BIT(1)

    /// If no option is chosen, the Qwadro treats it as identity.
} avxPresentTransform;

typedef enum avxPresentAlpha
{
    /// Ignore alpha component in the compositing process. 
    /// The raster is treated as if it has a constant alpha of 1.0.
    avxPresentAlpha_OPAQUE      = AFX_BIT(0),

    /// The compositor will respect the alpha component.
    avxPresentAlpha_PREMUL      = AFX_BIT(1),

    /// The compositor will multiply the color components of the raster by the alpha component during compositing.
    avxPresentAlpha_POSTMUL     = AFX_BIT(2)

    /// If no option is chosen, the way in which the presentation engine treats the alpha component in the images is unknown to the SIGMA GL/2 API. 
    /// Instead, the application is responsible for setting the composite alpha blending mode using native window system commands. 
    /// If the application does not set the blending mode using native window system commands, then a platform-specific default will be used.
} avxPresentAlpha;

typedef enum avxPresentMode
{
    /// Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    /// Tearing cannot be observed. An internal single-entry queue is used to hold pending presentation requests. 
    /// If the queue is full when a new presentation request is received, the new request replaces the existing entry, and any images associated with the prior entry become available for re-use by the application. 
    /// One request is removed from the queue and processed during each vertical blanking period in which the queue is non-empty.
    avxPresentMode_LIFO, // like triple-buffered mode
    
    /// Specifies that the presentation engine waits for the next vertical blanking period to update the current image. 
    /// Tearing cannot be observed. An internal queue is used to hold pending presentation requests. 
    /// New requests are appended to the end of the queue, and one request is removed from the beginning of the queue and processed during each vertical blanking period in which the queue is non-empty. 
    /// This is the only value of presentMode that is required to be supported.
    avxPresentMode_FIFO, // like V-sync'ed double-buffered mode
    
    /// Specifies that the presentation engine does not wait for a vertical blanking period to update the current image, meaning this mode may result in visible tearing. 
    /// No internal queuing of presentation requests is needed, as the requests are applied immediately.
    avxPresentMode_IMMEDIATE,
    
} avxPresentMode;

typedef afxBool(*avxPresentNotifier)(afxObject receiver, afxNat);

AFX_DEFINE_STRUCT(afxDrawOutputCaps)
{
    afxNat              minBufCnt;
    afxNat              maxBufCnt;
    afxNat              currExtent[2];
    afxWhd              minWhd; // D is layer
    afxWhd              maxWhd; // D is layer
    avxPresentTransform supportedTransforms;
    avxPresentTransform currTransform;
    avxPresentAlpha     supportedCompositeAlpha;
    afxRasterFlags      supportedUsageFlags;
    avxPresentScaling   supportedScaling;
    afxNat              supportedModeCnt;
    avxPresentMode      supportedModes;
};

AFX_DEFINE_STRUCT(afxDrawOutputConfig)
{
    // canvas
    afxWhd              whd;
    avxFormat           pixelFmt; // RGBA8; pixel format of raster surfaces. Pass zero to let driver choose the optimal format.
    afxRasterUsage      bufUsage; // DRAW; used as (color) target surface for rasterization.
    afxRasterFlags      bufFlags;
    avxFormat           pixelFmtDs[2]; // D24/S8/D24S8; pixel format of raster surfaces. Pass zero to disable depth and/or stencil.
    afxRasterUsage      bufUsageDs[2];
    afxRasterFlags      bufFlagsDs[2];
    avxColorSpace       colorSpc; // avxColorSpace_SRGB; if sRGB isn't present, fall down to LINEAR.

    // swapchain
    afxNat              minBufCnt; // usually 2 or 3; double or triple-buffered.

    // endpoint
    avxPresentNotifier  endpointNotifyFn;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentTransform presentTransform; // NIL; don't do any transform.
    avxPresentAlpha     presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    avxPresentMode      presentMode; // FIFO; respect the sequence.
    afxBool             doNotClip; // FALSE; don't do off-screen draw.
    void*               udd[4];
    
    union // WSI
    {
        struct
        {
            afxWindow   wnd;
        } wsi;
#ifdef AFX_OS_WIN
        struct
        {
            void*       hInst; // HINSTANCE
            void*       hWnd; // HWND
            void*       hDc; // HDC
        } w32;
#endif
    };
};

AVX afxDrawDevice   AfxGetDrawOutputDevice(afxDrawOutput dout);

AVX void**          AfxGetDrawOutputUdd(afxDrawOutput dout);
AVX afxError        AfxGetDrawOutputIdd(afxDrawOutput dout, afxNat code, void* dst);

AVX void            AfxGetDrawOutputFrequency(afxDrawOutput dout, afxNat* rate);

// Connection

AVX afxBool         AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx);
AVX afxBool         AfxGetDrawOutputContext(afxDrawOutput dout, afxDrawContext* context);
AVX afxError        AfxDisconnectDrawOutput(afxDrawOutput dout);

// Extent

AVX void            AfxGetDrawOutputResolution(afxDrawOutput dout, afxWhd res, afxReal* refreshRate);
AVX void            AfxGetDrawOutputAspectRatio(afxDrawOutput dout, afxReal64* wpOverHp, afxReal64* wrOverHr, afxReal64* wwOverHw);
AVX afxBool         AfxResetDrawOutputResolution(afxDrawOutput dout, afxWhd const res, afxReal refreshRate, afxReal64 physAspectRatio);

AVX afxError        AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd);
AVX afxError        AfxAdjustDrawOutputFromNdc(afxDrawOutput dout, afxV3d const whd);

AVX void            AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd); // d is always 1; it is here just for compatibility.
AVX void            AfxGetDrawOutputExtentAsNdc(afxDrawOutput dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.

// Buffer

AVX afxBool         AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat bufIdx, avxCanvas* canvas);
AVX afxNat          AfxGetDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, avxCanvas canvases[]);

AVX afxBool         AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxRaster* raster);
AVX afxNat          AfxGetDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[]);

AVX afxError        AfxRevalidateDrawOutputBuffers(afxDrawOutput dout);
AVX afxNat          AfxCountDrawOutputBuffers(afxDrawOutput dout);

// Reserves a buffer for future use. You must dispose it at some time to avoid starvation.
AVX afxError        AfxLockDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx);
// Disposes a previously acquired draw buffer without present it.
AVX afxError        AfxDisposeDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx);

AVX afxError        AfxWaitForDrawOutput(afxDrawOutput dout);

// Se não há garantia de presentação no tempo de retorno da função nem controle sobre a fila, não seria melhor submeter direto pelo afxDrawOutput e esperar pelo semáforo?

AVX afxError        AfxPrintDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxUri const* uri);

#endif//AVX_DRAW_OUTPUT_H
