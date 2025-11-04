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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_SURFACE_H
#define AVX_SURFACE_H

// afxSurface é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.
 
// Swapchains are a list of images, accessible by the operating system for display to the screen.
// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

// afxSurface devices operates framebuffers, swapchains, windowSurface and window(?).

/*
    afxSurface is a mechanism that efficiently transfers a sequence of image frames from one API to another.
    A producer and a consumer are attached to two ends of a stream object: a producer adds image frames into the stream,
    and a consumer retrieves image frames from the stream.

    The producer is the entity that posts image frames into the afxSurface. 
    The producer is responsible for inserting each image frame into the afxSurface at the correct time so that the consumer can display the image frame for the appropriate period of time.

    The consumer is the entity that retrieves image frames from the afxSurface. 
    The consumer is responsible for noticing that an image frame is available and displaying it (or otherwise consuming it). 
    The consumer is also responsible for indicating the latency when that is possible (the latency is the time that elapses between the time it is retrieved from the afxSurface until the time it is displayed).
*/

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

#include "qwadro/draw/avxCanvas.h"
#include "qwadro/draw/afxDrawBridge.h"
#include "qwadro/hid/afxDisplay.h"

typedef enum avxPresentFlag
// Flags specifying the presentation rules.
{
    // Frame rate limited.
    avxPresentFlag_RATE_LIMITED = AFX_BITMASK(0),

    // Screen tearing explicitly forbidden
    avxPresentFlag_NO_TEARING   = AFX_BITMASK(1),

    // Frame rate limited, screen tearing forbidden. ('vsync on')
    avxPresentFlag_VSYNC        = (avxPresentFlag_RATE_LIMITED | avxPresentFlag_NO_TEARING),
} avxPresentFlags;

typedef afxBool(*avxPresentNotifier)(afxObject receiver, afxUnit);

AFX_DEFINE_STRUCT(afxSurfaceCaps)
{
    // The minimum latency measured in latent buffers.
    afxUnit             minBufCnt;
    // The maximum latency measured in latent buffers.
    afxUnit             maxBufCnt;
    afxUnit             currExtent[2];
    afxWarp             minWhd; // D is layer
    afxWarp             maxWhd; // D is layer
    avxVideoTransform   supportedTransforms;
    avxVideoTransform   currTransform;
    avxVideoAlpha       supportedCompositeAlpha;
    avxRasterFlags      supportedUsageFlags;
    avxVideoScaling     supportedScaling;
    afxUnit             supportedModeCnt;
    avxPresentFlags     supportedModes;
};

AFX_DEFINE_STRUCT(afxSurfaceInterop)
{
    afxObject           endpointNotifyObj; // must ensure life of draw output
    avxPresentNotifier  endpointNotifyFn;
    afxFcc              ext;
    afxSurfaceInterop*  next;
    afxString           wsi; // w32, wgl, wgldx9, wgldx10, wgldx11, gdi
    union // WSI
    {
#ifdef AFX_ON_WINDOWS
        struct
        {
            void*       hInst; // HINSTANCE
            void*       hWnd; // HWND
        } w32;
        struct
        {
            void*       hInst; // HINSTANCE
            void*       hWnd; // HWND
            void*       hGlrc; // HGLRC
        } wgl;
        struct
        {
            void*       hDc; // HDC
        } gdi;
        struct
        {
            void*       hDev;
        } d3d9;
#endif
    };
};

AFX_DEFINE_STRUCT(afxSurfaceConfig)
{
    // The display from which the surface will be acquired.
    afxDisplay          dpy;
    // Boolean requesting full, exclusive control of the video endpoint.
    afxBool             exclusive;
    // The screen resolution to be used to mode-set the video endpoint.
    avxRange            resolution;
    // The refresh rate to be used to mode-set the video endpoint.
    afxReal64           refreshRate;
    
    // Swapchain
    // The configuration for the canvas, and its draw buffers.
    avxCanvasConfig     ccfg;
    // A value specifying the way the swapchain interprets image data.
    avxColorSpace       colorSpc;
    // Boolean requesting adjustability.
    afxBool             resizable;
    // The latency of swapchain measured in latent buffers.
    // Usually 2 or 3, for a double or triple-buffered mechanism, respectively.
    afxUnit             latency;
    // Endpoint
    // Flags specifying how the video endpoint presentation mechanism should behaves.
    avxPresentFlags     presentMode;
    // Flags specifying how the video endpoint should position the image.
    avxVideoTransform   presentTransform;
    // Flags specifying how the video endpoint compositor should consider the alpha channel (for example, using it for transparency).
    avxVideoAlpha       presentAlpha;
    // Boolean specifying when to do not do off-screen blitting.
    afxBool             doNotClip;

    // A debuggint string.
    afxString           tag;
    // A user-defined data.
    void*               udd[4];
    // Interoperability extender.
    afxSurfaceInterop   iop;
};

/*
    The AvxConfigureSurface() configures the settings for a drawing output context before it is acquired.
    This involve preparing the configuration structure (cfg) with system-specific defaults or constraints.

    One should call this function to prepopulate and/or validate a afxSurfaceConfig structure,
    and ensure that the configuration is compatible with the draw system (dsys).
*/

AVX afxError AvxConfigureSurface
(
    // The drawing system to which the drawing output context belongs.
    afxDrawSystem dsys,

    // A pointer to the configuration settings to be applied to the drawing output context.
    afxSurfaceConfig*cfg
);

/*
    The AvxAcquireSurface() function opens a drawing output context with specific settings provided
    in the configuration structure. This could involve creating a window for rendering,
    allocating a framebuffer, setting up a full-screen display, or initializing other kinds
    of graphical outputs.

    Once successfully executed, the function returns a handle to the newly created drawing output context (output),
    which can be used to perform further rendering or interaction.
*/

AVX afxError AvxAcquireSurface
(
    // The drawing system where the drawing output context will be created or opened.
    afxDrawSystem dsys,

    // A pointer to the configuration settings to initialize the drawing output context.
    afxSurfaceConfig const* cfg,

    // A pointer to store the created or opened drawing output context.
    afxSurface* output
);

/*
    The AvxEnumerateSurfaces() function enumerates the drawing output contexts open for a established drawing system.
    This can include devices like display monitors or render targets, allowing applications to discover and
    interact with these outputs for rendering or display purposes. The function provides the ability to query
    a specific range of outputs and retrieve information about each one for further processing.
*/

AVX afxUnit AvxEnumerateSurfaces
(
    // The established draw system associated with the enumeration process.
    // The function will query available drawing output contexts for the given drawing system (dsys).
    afxDrawSystem dsys,

    // The starting index for the enumeration of drawing output contexts.
    // If there are many outputs available, @first allows you to specify where to start enumerating 
    // (e.g., you might skip some outputs if you want to continue from a specific position in the list).
    afxUnit first,

    // The number of outputs to retrieve.
    // The function will retrieve up to @cnt drawing output contexts, starting from the index specified by @first.
    afxUnit cnt,

    // An array where the enumerated drawing output contexts will be stored.
    afxSurface outputs[]
);

/*
    The AvxEvokeSurfaces() function enables the application to retrieve a set of drawing output contexts from a drawing system
    and invoke a user-defined function on each output. This allows for customized processing of outputs such as displays,
    render targets, or other output interfaces. The user-defined data provides flexibility, enabling the user to pass
    additional context or configuration to the processing function. This function is useful when the application needs to
    process or configure multiple outputs based on a drawing system.
*/

AVX afxUnit AvxEvokeSurfaces
(
    // The drawing system from which the drawing output contexts will be retrieved.
    afxDrawSystem dsys,

    // A user-defined function pointer that is invoked on each retrieved drawing output context.
    // The return type is afxBool, which indicates if an draw output should be inserted in @output array.
    afxBool(*f)(afxSurface, void*),

    // This is user-defined data that is passed to the user-defined function when processing each drawing output context.
    void* udd,

    // The starting index of the drawing output contexts to retrieve.
    afxUnit first,

    // The number of drawing output contexts to retrieve and process.
    afxUnit cnt,

    // An array that will store the retrieved drawing output contexts.
    afxSurface outputs[]
);

/*
    The AvxInvokeSurfaces() function enables custom processing of drawing output contexts within a drawing system.
    By passing a user-defined function and user data, this function allows the application to query and process
    drawing output contexts in a flexible and extensible way. This could be useful for tasks like querying display properties,
    updating output settings, or interacting with different display or render target devices.
*/

AVX afxUnit AvxInvokeSurfaces
(
    // The drawing system whose outputs are to be processed.
    afxDrawSystem dsys,

    // The starting index for the outputs to be processed.
    // It specifies the position of the first drawing output context to be processed. 
    // If you want to skip some outputs, you can adjust this index accordingly.
    afxUnit first,

    // The number of outputs to process.
    // It defines how many outputs should be processed, starting from the @first index. 
    // The function will process up to @cnt drawing output contexts.
    afxUnit cnt,

    // A pointer to a user-defined function that is called for each drawing output context.
    // Its return type is afxBool, which indicates whether the process should be aborted.
    afxBool(*f)(afxSurface, void*),

    // This is user-defined data that is passed to the function f for each drawing output context.
    void* udd
);

/*
    The AvxPresentSurfaces() function presents rendered outputs (such as frames or images) to a display system or window.
    It handles synchronization between the drawing operations and the presentation phase, ensuring that the outputs are
    presented only after they are fully rendered. It also supports managing multiple outputs and buffer indices for
    double/triple buffering, as well as using fences to signal the completion of the presentation. This function is typically
    used at the end of a rendering cycle to display the final results to the user.
*/

AVX afxError AvxPresentSurfaces
(
    // The drawing system where the outputs will be presented.
    afxDrawSystem dsys,

    // The number of outputs to be presented. This indicates how many items in the @outputs array should be processed.
    afxUnit cnt,

    // A control structure for the presentation settings. 
    // This structure would contain configuration options that manage how the output is presented, such as swap chains, 
    // synchronization details, or specific rendering modes(e.g., vsync settings or buffer flipping).
    avxPresentation presentations[]
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AvxDescribeSurface() method retrieves descriptive information about a drawing output context.
    This is useful when you need to know the configuration before performing rendering or display operations.
*/

AVX void AvxDescribeSurface
(
    afxSurface dout,
    afxSurfaceConfig* cfg
);

/*
    The function AvxGetSurfaceHost() is used to retrieve the draw system context associated with a specific drawing output context.
    This context provides access to the drawing system's state, settings, and configuration, which can be necessary for rendering
    or interacting with the drawing output context in a more detailed and controlled manner.
*/

AVX afxDrawSystem AvxGetSurfaceHost
(
    afxSurface dout
);

/*
    The primary purpose of AvxGetSurfaceUdd() is to retrieve custom, user-defined data that has been associated with a
    specific drawing output context (e.g., a render target, display, or framebuffer). This allows the application to store and retrieve
    arbitrary data that may be needed during rendering or display operations.
*/

AVX void* AvxGetSurfaceUdd
(
    afxSurface dout,
    afxUnit slotIdx
);

/*
    The function AvxGetSurfaceIcd() retrieves the installable client driver (ICD) operating a specific drawing output context.
    This ICD is a lower-level handle that provides access to the physical or virtual hardware device (ex.: video display adapter)
    used for rendering or displaying the output. The returned ICD can be used for querying device-specific properties,
    capabilities, or performing advanced operations directly related to the hardware rendering or display system.
*/

AVX afxModule AvxGetSurfaceIcd
(
    afxSurface dout
);

/*
    The function AvxCallSurfaceEndpoint() is used to call implementation-specific functionality associated with a specific drawing output context
    and store results in a provided destination buffer. This function is designed to interface with the underlying hardware or rendering
    context, extracting low-level information about the drawing output context, which may include hardware-specific data, driver configurations,
    or other internal details related to the output device.
*/

AVX afxError AvxCallSurfaceEndpoint
(
    afxSurface dout,
    afxUnit code,
    ...
);

/*
    The primary purpose of AvxWaitForSurface() is to synchronize your application with the rendering process.
    It allows you to block or wait until the drawing output context is ready for further operations (e.g., drawing to a surface,
    reading from a surface, or performing buffer swaps).

    AvxWaitForSurface() is used to block or wait until a specific drawing output context (e.g., a render target, framebuffer, or window)
    is ready for further operations, with a specified timeout to avoid blocking indefinitely.
    It can be useful for synchronizing operations between the CPU and GPU, ensuring that rendering tasks are completed before
    proceeding with additional steps.
*/

AVX afxError AvxWaitForSurface
(
    // The drawing output context to wait for.
    afxSurface dout,

    // The timeout duration, in nanoseconds, to wait for the drawing output context to be ready.
    afxUnit64 timeout
);

AFX_DEFINE_STRUCT(avxModeSetting)
{
    // The physical aspect ratio of the display.
    afxReal64       wpOverHp;

    // The refresh rate (e.g., 60Hz, 120Hz).
    afxReal         refreshRate;

    // The screen resolution (Width x Height).
    avxRange        resolution;

    // The width-to-physical aspect ratio.
    afxReal64       wrOverHr;

    // A flag indicating if the resolution change is exclusive (possibly fullscreen).
    afxBool         exclusive;

    afxUnit         outRate;

    // The rectangular region of pixels to present in the canvas being presented.
    // If empty, this region will be assumed to be the entire presentable canvas.
    afxRect         srcArea;

    // The rectangular region within the visible region of the swapchain's display mode.
    // If empty, this region will be assumed to be the entire visible region of the swapchain's mode.
    // If the specified rectangle is a subset of the display mode's visible region, 
    // content from display planes below the swapchain's plane will be visible outside the rectangle. 
    // If there are no planes below the swapchain's, the area outside the specified rectangle will be black. 
    // If portions of the specified rectangle are outside of the display's visible region, pixels mapping only to those portions of the rectangle will be discarded.
    afxRect         dstArea;

    // If TRUE, the display device will enable buffered mode on displays that support it. 
    // This allows the display device to stop sending content to the display until a new image is presented. 
    // The display will instead maintain a copy of the last presented image. This allows less power to be used, 
    // but may increase presentation latency. If FALSE, persistent mode will not be used.
    afxBool         persistent;
};

/*
    The AvxQuerySurfaceSettings() function is used to query various display settings or attributes for a specific drawing output context.
    It provides information about the resolution, refresh rate, and possibly the scaling factors related to the physical aspect ratio
    of the display output. You would typically call this function when you need to retrieve the current display properties of a
    particular screen or rendering target.
*/

AVX void AvxQuerySurfaceSettings
(
    // The drawing output context for which resolution details are queried.
    afxSurface dout,

    // A pointer to a avxModeSetting struct which will be filled with mode-setting description.
    avxModeSetting* mode
);

/*
    The AvxChangeSurfaceSettings() function is used to change or reset the drawing resolution for a specific output device,
    adjusting both the display resolution and potentially the refresh rate, while considering the aspect ratio and whether the
    change is in an exclusive fullscreen mode.
*/

AVX afxError AvxChangeSurfaceSettings
(
    // The drawing output context to be reset or configured.
    afxSurface dout,

    // A pointer to a avxModeSetting struct specifying the desired mode-setting.
    avxModeSetting const* mode
);

/*
    The function AvxGetSurfaceArea is used to query the extent (size or dimensions) of a drawing output context,
    such as a display surface, framebuffer, or render target. It provides information about the drawing output context's
    resolution and other relevant properties.

    Returns the afxReal64 (wwOverHw) value that will store a ratio or value representing width over height.
*/

AVX afxReal64 AvxGetSurfaceArea
(
    // The drawing output context whose extent (size) is being queried.
    afxSurface dout,

    // A pointer to a afxRect that will hold the placement of the drawing output context, including the width, height, and potentially the origin.
    afxRect* area
);

/*
    The AvxAdjustSurface() adjusts the canvas extent of the specified drawing output context (dout).
    That includes moving/resizing the rendering surface (e.g., changing window or swapchain dimensions), 
    switching modes, such as going into fullscreen exclusive (often required for lower latency or higher performance in games),
    and updating internal states or reallocating buffers to fit the new area.

    If a draw context is contained in another positional entity, .x and .y members of area may be zero or not directly map to the screen.
*/

AVX afxError AvxAdjustSurface
(
    // The drawing output context whose properties are to be adjusted.
    afxSurface dout, 

    // The new placement which the drawing output context should be adjusted.
    afxRect const* area,

    // A boolean flag indicating whether to switch from/to fullscreen exclusive mode.
    afxBool fse
);

AVX afxError _AvxDoutAdjustNormalized(afxSurface dout, afxV3d const whd);
AVX void _AvxDoutGetExtentNormalized(afxSurface dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.

/*
    The AvxLockSurfaceBuffer() function is designed to request or reserve a buffer for future use. 
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

AVX afxError AvxLockSurfaceBuffer
(
    // The drawing output context for which to request a buffer.
    afxSurface dout,

    // The timeout duration to wait for the buffer to be available.
    afxUnit64 timeout,

    // A bitmask specifying which bridges can used the buffer when ready and signaled so.
    afxMask exuMask,

    // A fence to signal.
    avxFence signal,

    // A pointer to store the index of the reserved buffer.
    afxUnit* bufIdx
);

/*
    The function AvxUnlockSurfaceBuffer() is used to dispose of a previously acquired drawing buffer 
    without presenting it to the screen or any output target. This function helps manage resources by 
    allowing the application to release a buffer that is no longer needed, without having to render or display its contents.

    The primary purpose of AvxUnlockSurfaceBuffer() is to release or dispose of a buffer that was previously requested 
    but is no longer needed. This is often used in cases where the buffer will not be used for rendering or presentation, 
    but the system still needs to manage resources efficiently.
*/

AVX afxError AvxUnlockSurfaceBuffer
(
    // The drawing output context associated with the buffer.
    afxSurface dout,

    // The index of the buffer to discard.
    afxUnit bufIdx
);

/*
    The AvxGetSurfaceCanvas() function is used to query a specific canvas (or drawing buffer) associated with a drawing output context context.
    It allows you to retrieve information about a particular rendering surface (e.g., a frame buffer or texture) that may be used
    for drawing operations. Depending on the system, a canvas might represent an off-screen buffer (like a back buffer for double
    buffering) or an on-screen rendering target.
*/

AVX afxBool AvxGetSurfaceCanvas
(
    // Drawing output context for which the canvas is being queried.
    afxSurface dout,

    // Index of the canvas/buffer that is being queried.
    afxUnit bufIdx,

    // Pointer to an placeholder that will be populated with the avxCanvas object handle.
    avxCanvas* canvas,

    // Pointer to an afxLayeredRect structure defining the area and layers of the canvas being referenced.
    // Could be used to query or limit to a subregion of the canvas.
    afxLayeredRect* bounds
);

/*
    The function AvxGetSurfaceBuffer() is designed to retrieve a specific buffer associated with a drawing output context.
    This function is typically used in graphics systems where multiple rendering buffers are managed and need to be accessed
    for rendering or presentation. The buffer in question could be a frame buffer, back buffer, or any other type of memory buffer
    used for rendering.
*/

AVX afxBool AvxGetSurfaceBuffer
(
    // The drawing output context for which you want to retrieve the buffer.
    afxSurface dout,

    // The buffer index that specifies which buffer to retrieve.
    afxUnit bufIdx,

    // A pointer to where the function will store the reference to the retrieved buffer.
    avxRaster* buffer
);

/*
    The AvxPrintSurfaceBuffer() function is designed to "print" or "export" the contents of a drawing output context buffer to an external resource.
    Depending on the library or framework, "printing" could mean saving the rendered buffer to a file, sending it to a printer,
    or exporting the drawing in a specific format.

    This could be useful in situations where you need to save a rendered frame or an image to disk (e.g., for screenshot functionality,
    exporting frames in a video sequence, or saving images for further processing).
*/

AVX afxError AvxPrintSurfaceBuffer
(
    // The drawing output context for which the content is to be printed or saved.
    afxSurface dout,

    // The index of the buffer or canvas (e.g., front buffer, back buffer).
    afxUnit bufIdx,

    avxRasterIo const* op,

    // The URI (Uniform Resource Identifier) specifying the destination or file path.
    afxUri const* uri,

    // The mask of the execution unit to handle the I/O.
    afxMask exuMask
);

#endif//AVX_SURFACE_H
