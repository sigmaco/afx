/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
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

#include "qwadro/inc/draw/pipe/avxCanvas.h"

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

typedef afxBool(*avxPresentNotifier)(afxObject receiver, afxUnit);

AFX_DEFINE_STRUCT(afxDrawOutputCaps)
{
    afxUnit             minBufCnt;
    afxUnit             maxBufCnt;
    afxUnit             currExtent[2];
    afxWhd              minWhd; // D is layer
    afxWhd              maxWhd; // D is layer
    avxPresentTransform supportedTransforms;
    avxPresentTransform currTransform;
    avxPresentAlpha     supportedCompositeAlpha;
    afxRasterFlags      supportedUsageFlags;
    avxPresentScaling   supportedScaling;
    afxUnit             supportedModeCnt;
    avxPresentMode      supportedModes;
};

AFX_DEFINE_STRUCT(afxDrawOutputConfig)
{
    // endpoint
    afxUri              endpoint;
    avxPresentNotifier  endpointNotifyFn;
    afxObject           endpointNotifyObj; // must ensure life of draw output
    afxReal64           refreshRate;
    afxWhd              resolution; // Screen resolution. Absolute extent available.
    afxBool             exclusive; // assume full control of the endpoint

    // canvas
    avxColorSpace       colorSpc; // avxColorSpace_STANDARD; if sRGB isn't present, fall down to LINEAR.
    avxFormat           pixelFmt; // RGBA8; pixel format of raster surfaces. Pass zero to let driver choose the optimal format.
    avxFormat           pixelFmtDs[2]; // D24/S8/D24S8; pixel format of raster surfaces. Pass zero to disable depth and/or stencil.
    afxRasterUsage      bufUsage; // DRAW; used as (color) target surface for rasterization.
    afxRasterUsage      bufUsageDs[2];
    afxRasterFlags      bufFlags;
    afxRasterFlags      bufFlagsDs[2];
    afxWhd              extent;

    // swapchain
    afxUnit             minBufCnt; // usually 2 or 3; double or triple-buffered.
    avxPresentMode      presentMode; // FIFO; respect the sequence.
    avxPresentAlpha     presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    avxPresentTransform presentTransform; // NIL; don't do any transform.
    afxBool             doNotClip; // FALSE; don't do off-screen draw.

    void*               udd[4];
    afxFcc              ext;
    
    union // WSI
    {
        struct
        {
            afxWindow   wnd;
        } wsi;
//#ifdef AFX_OS_WIN
        struct
        {
            void*       hInst; // HINSTANCE
            void*       hWnd; // HWND
            void*       hDc; // HDC
        } w32;
//#endif
    };
};

/*
    The function AfxGetDrawOutputContext() is used to retrieve the draw system context associated with a specific drawing output. 
    This context provides access to the drawing system's state, settings, and configuration, which can be necessary for rendering 
    or interacting with the drawing output in a more detailed and controlled manner.
*/

AVX afxDrawSystem   AfxGetDrawOutputContext(afxDrawOutput dout);

/*
    The function AfxGetDrawOutputIcd() retrieves the installable client driver (ICD) operation a specific drawing output. 
    This ICD is a lower-level handle that provides access to the physical or virtual hardware device (e.g., GPU, display adapter) 
    used for rendering or displaying the output. The returned ICD can be used for querying device-specific properties, 
    capabilities, or performing advanced operations directly related to the hardware rendering or display system.
*/

AVX afxModule AfxGetDrawOutputIcd(afxDrawOutput dout);

/*
    The primary purpose of AfxDescribeDrawOutput() is to retrieve descriptive information about a drawing output 
    (e.g., a window, display, or framebuffer). This is useful when you need to know the capabilities or configuration of 
    the drawing output before performing rendering or display operations.
*/

AVX void AfxDescribeDrawOutput(afxDrawOutput dout, afxDrawOutputConfig* cfg);

/*
    The primary purpose of AfxGetDrawOutputUdd() is to retrieve custom, user-defined data that has been associated with a 
    specific drawing output (e.g., a render target, display, or framebuffer). This allows the application to store and retrieve 
    arbitrary data that may be needed during rendering or display operations.
*/

AVX void* AfxGetDrawOutputUdd(afxDrawOutput dout, afxUnit slotIdx);

/*
    The function AfxGetDrawOutputIdd() is used to retrieve implementation-defined data (IDD) associated with a specific drawing output 
    and store it in a provided destination buffer. This function is likely designed to interface with the underlying hardware or rendering 
    context, extracting low-level information about the drawing output, which may include hardware-specific data, driver configurations, 
    or other internal details related to the output device.
*/

AVX afxError AfxGetDrawOutputIdd(afxDrawOutput dout, afxUnit code, void* dst);

/*
    The AfxQueryDrawOutputResolution() function is used to query various display settings or attributes for a specific drawing output. 
    It provides information about the resolution, refresh rate, and possibly the scaling factors related to the physical aspect ratio 
    of the display output. You would typically call this function when you need to retrieve the current display properties of a 
    particular screen or rendering target.
*/

AVX void AfxQueryDrawOutputResolution
(
    // The drawing output for which resolution details are queried.
    afxDrawOutput dout,
    
    // The width-to-physical aspect ratio (possibly the horizontal scaling factor).
    afxReal64* wpOverHp,
    
    // Pointer to store the current refresh rate (e.g., 60Hz, 120Hz).
    afxReal* refreshRate,
    
    // Pointer to store the current resolution (width x height).
    afxWhd* resolution,
    
    // The width-to-physical aspect ratio (possibly the vertical scaling factor).
    afxReal64* wrOverHr
);

/*
    The AfxResetDrawOutputResolution() function is used to change or reset the drawing resolution for a specific output device, 
    adjusting both the display resolution and potentially the refresh rate, while considering the aspect ratio and whether the 
    change is in an exclusive fullscreen mode.
*/

AVX afxError AfxResetDrawOutputResolution
(
    // The drawing output to be reset or configured.
    afxDrawOutput dout,

    // The physical aspect ratio of the display.
    afxReal64 physAspectRatio,

    // The refresh rate to be set (e.g., 60Hz, 120Hz).
    afxReal refreshRate,

    // The desired screen resolution (Width x Height).
    afxWhd resolution,

    // A flag indicating if the resolution change is exclusive (possibly fullscreen).
    afxBool exclusive
);

/*
    The primary purpose of AfxAdjustDrawOutput() is to adjust the properties (such as resolution, size, or other dimensions) 
    of the specified drawing output (dout) according to the provided width, height, and depth values (whd).
*/
AVX afxError        AfxAdjustDrawOutput
(
    // The drawing output whose properties are to be adjusted.
    afxDrawOutput dout, 

    // The new width, height, and depth to which the drawing output should be adjusted.
    afxWhd whd
);

AVX afxError _AvxAdjustDrawOutputNormalized(afxDrawOutput dout, afxV3d const whd);
AVX void _AvxGetDrawOutputExtentNormalized(afxDrawOutput dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.


AVX void AfxGetDrawOutputFrequency(afxDrawOutput dout, afxUnit* rate);

/*
    The function AfxQueryDrawOutputExtent is used to query the extent (size or dimensions) of a drawing output, 
    such as a display surface, framebuffer, or render target. It provides information about the drawing output's 
    resolution and other relevant properties.
*/

AVX void AfxQueryDrawOutputExtent
(
    // The drawing output whose extent (size) is being queried.
    afxDrawOutput dout, 

    // A pointer to a structure or variable that will hold the extent (size) of the drawing output, typically including the width, height, and potentially the depth.
    afxWhd* extent, 

    // A pointer to a afxReal64 value that will store a ratio or value representing width over height.
    afxReal64* wwOverHw
);

/*
    The AfxGetDrawOutputCanvas() function is used to query a specific canvas (or drawing buffer) associated with a drawing output context. 
    It allows you to retrieve information about a particular rendering surface (e.g., a frame buffer or texture) that may be used 
    for drawing operations. Depending on the system, a canvas might represent an off-screen buffer (like a back buffer for double 
    buffering) or an on-screen rendering target.
*/

AVX afxBool AfxGetDrawOutputCanvas
(
    // Drawing output for which the canvas is being queried.
    afxDrawOutput dout,

    // Index of the buffer or canvas that is being queried.
    afxUnit bufIdx,

    // Pointer to the canvas that will hold the queried information.
    avxCanvas* canvas
);

/*
    The function AfxGetDrawOutputBuffer() is designed to retrieve a specific buffer associated with a drawing output. 
    This function is typically used in graphics systems where multiple rendering buffers are managed and need to be accessed 
    for rendering or presentation. The buffer in question could be a frame buffer, back buffer, or any other type of memory buffer 
    used for rendering.
*/

AVX afxBool AfxGetDrawOutputBuffer
(
    // The drawing output for which you want to retrieve the buffer.
    afxDrawOutput dout,

    // The buffer index that specifies which buffer to retrieve.
    afxUnit bufIdx, 

    // A pointer to where the function will store the reference to the retrieved buffer.
    afxRaster* buffer
);

AVX afxError AfxRevalidateDrawOutputBuffers(afxDrawOutput dout);

/*
    The AfxRequestDrawOutputBuffer() function is designed to request or reserve a buffer for future use. 
    Buffers are commonly used in graphics and rendering systems for a variety of purposes:

        Double-Buffering: 
            To prevent flickering by rendering to one buffer while the other buffer is being displayed.

        Triple-Buffering: 
            Increases rendering performance by providing an additional buffer for smoother transitions.

        Offscreen Buffers: 
            Used for rendering images that will later be used for post-processing or as textures.
    
    This function helps in managing these buffers by allowing the application to request a buffer for drawing, 
    which might be necessary when the system is using multiple buffers for efficient rendering.

    You must present or discard it at some time to avoid starvation.
*/

AVX afxError AfxRequestDrawOutputBuffer
(
    // The drawing output for which to request a buffer.
    afxDrawOutput dout,

    // The timeout duration to wait for the buffer to be available.
    afxTime timeout,

    // A pointer to store the index of the reserved buffer.
    afxUnit *bufIdx
);

/*
    The function AfxRecycleDrawOutputBuffer() is used to dispose of a previously acquired drawing buffer 
    without presenting it to the screen or any output target. This function helps manage resources by 
    allowing the application to release a buffer that is no longer needed, without having to render or display its contents.

    The primary purpose of AfxRecycleDrawOutputBuffer() is to release or dispose of a buffer that was previously requested 
    but is no longer needed. This is often used in cases where the buffer will not be used for rendering or presentation, 
    but the system still needs to manage resources efficiently.
*/

AVX afxError AfxRecycleDrawOutputBuffer
(
    // The drawing output associated with the buffer.
    afxDrawOutput dout,

    // The index of the buffer to discard.
    afxUnit bufIdx
);

/*
    The primary purpose of AfxWaitForDrawOutput() is to synchronize your application with the rendering process. 
    It allows you to block or wait until the drawing output is ready for further operations (e.g., drawing to a surface, 
    reading from a surface, or performing buffer swaps).

    AfxWaitForDrawOutput() is used to block or wait until a specific drawing output (e.g., a render target, framebuffer, or window) 
    is ready for further operations, with a specified timeout to avoid blocking indefinitely.
    It can be useful for synchronizing operations between the CPU and GPU, ensuring that rendering tasks are completed before 
    proceeding with additional steps.
*/

AVX afxError AfxWaitForDrawOutput
(
    // The drawing output to wait for.
    afxDrawOutput dout,

    // The timeout duration to wait for the drawing output to be ready.
    afxTime timeout
);

/*
    The AfxPrintDrawOutput() function is designed to "print" or "export" the contents of a drawing output buffer to an external resource. 
    Depending on the library or framework, "printing" could mean saving the rendered buffer to a file, sending it to a printer, 
    or exporting the drawing in a specific format.

    This could be useful in situations where you need to save a rendered frame or an image to disk (e.g., for screenshot functionality, 
    exporting frames in a video sequence, or saving images for further processing).
*/

AVX afxError AfxPrintDrawOutput
(
    // The drawing output for which the content is to be printed or saved.
    afxDrawOutput dout, 

    // The index of the buffer or canvas (e.g., front buffer, back buffer).
    afxUnit bufIdx, 

    // The index of the execution unit to handle the I/O.
    afxUnit exuIdx, 

    // The URI (Uniform Resource Identifier) specifying the destination or file path.
    afxUri const* uri
);

////////////////////////////////////////////////////////////////////////////////

AVX afxError AfxConfigureDrawOutput
(
    // The drawing system to which the drawing output belongs.
    afxDrawSystem dsys,
    
    // A pointer to the configuration settings to be applied to the drawing output.
    afxDrawOutputConfig* cfg
);


/*
    The primary purpose of AfxOpenDrawOutput() is to initialize or open a drawing output with specific settings provided 
    in the configuration structure (cfg). This could involve creating a window for rendering, allocating a framebuffer, 
    setting up a full-screen display, or initializing other kinds of graphical outputs.

    Once successfully executed, the function returns a handle to the newly created drawing output (output), 
    which can be used to perform further rendering or interaction.
*/

AVX afxError AfxOpenDrawOutput
(
    // The drawing system where the drawing output will be created or opened.
    afxDrawSystem dsys, 

    // A pointer to the configuration settings to initialize the drawing output.
    afxDrawOutputConfig const* cfg, 
    
    // A pointer to store the created or opened drawing output.
    afxDrawOutput* output
);

#endif//AVX_DRAW_OUTPUT_H
