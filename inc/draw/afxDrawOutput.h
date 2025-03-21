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

// afxDrawOutput é um objeto que abstrai a ideia de swapchain junto a surface da plataforma.

// Swapchains are a list of images, accessible by the operating system for display to the screen.
// You canv create swapchains with more or less images, but generally you will want only 2 or 3 images to perform double-buffer or triple-buffer rendering.

// afxDrawOutput devices operates framebuffers, swapchains, windowSurface and window(?).

/*
    afxDrawOutput is a mechanism that efficiently transfers a sequence of image frames from one API to another.
    A producer and a consumer are attached to two ends of a stream object: a producer adds image frames into the stream,
    and a consumer retrieves image frames from the stream.

    The producer is the entity that posts image frames into the afxDrawOutput. 
    The producer is responsible for inserting each image frame into the afxDrawOutput at the correct time so that the consumer can display the image frame for the appropriate period of time.

    The consumer is the entity that retrieves image frames from the afxDrawOutput. 
    The consumer is responsible for noticing that an image frame is available and displaying it (or otherwise consuming it). 
    The consumer is also responsible for indicating the latency when that is possible (the latency is the time that elapses between the time it is retrieved from the afxDrawOutput until the time it is displayed).
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

#ifndef AVX_DRAW_OUTPUT_H
#define AVX_DRAW_OUTPUT_H

#include "qwadro/inc/draw/op/avxCanvas.h"
#include "qwadro/inc/draw/afxDrawBridge.h"

typedef enum avxVideoScaling
// mask specifying presentation scaling methods.
{
    // Do not scale.
    // Pixels are mapped to one and only one pixel in the surface.
    avxVideoScaling_ONE_TO_ONE    = AFX_BIT(0),

    // Minify or magnify such that at least one of the resulting width or height is equal to the corresponding surface dimension, 
    // and the other resulting dimension is less than or equal to the corresponding surface dimension, 
    // with the aspect ratio of the resulting image being identical to that of the original swapchain image.
    avxVideoScaling_ASPECT_RATIO  = AFX_BIT(1),

    // minify or magnify such that the resulting dimensions are equal to those of the surface.
    avxVideoScaling_STRETCH       = AFX_BIT(2)
} avxVideoScaling;

typedef enum avxVideoTransform
{
    // When not specified, it is treated as IDENTITY, what is expected to has not rotation nor mirroring (0 degrees).
    // If no option is chosen, the Qwadro treats it as identity.
    avxVideoTransform_IDENTITY,

    // 90 degrees rotated
    avxVideoTransform_90 = AFX_BIT(0),
    
    // 180 degrees rotated
    avxVideoTransform_180 = AFX_BIT(1),
    
    // 270 degrees rotated
    avxVideoTransform_270 = AFX_BIT(2),

    // Invert pixel grid horizontally.
    avxVideoTransform_FLIP_H = AFX_BIT(3),

    // Invert pixel grid vertically.
    avxVideoTransform_FLIP_V  = AFX_BIT(4),

    // 90 degress rotated + V mirror
    avxVideoTransform_90FV = (avxVideoTransform_90 | avxVideoTransform_FLIP_V),

    // 90 degrees rotated + H mirror
    avxVideoTransform_90FH = (avxVideoTransform_90 | avxVideoTransform_FLIP_H),    
} avxVideoTransform;

typedef enum avxVideoAlpha
{
    // Ignore alpha component in the compositing process. 
    // The raster is treated as if it has a constant alpha of 1.0.
    avxVideoAlpha_OPAQUE      = AFX_BIT(0),

    // The compositor will respect the alpha component.
    avxVideoAlpha_PREMUL      = AFX_BIT(1),

    // The compositor will multiply the color components of the raster by the alpha component during compositing.
    avxVideoAlpha_POSTMUL     = AFX_BIT(2)

    // If no option is chosen, the way in which the presentation engine treats the alpha component in the images is unknown to the SIGMA GL/2 API. 
    // Instead, the application is responsible for setting the composite alpha blending mode using native window system commands. 
    // If the application does not set the blending mode using native window system commands, then a platform-specific default will be used.
} avxVideoAlpha;

typedef enum avxPresentFlag
// Flags specifying the presentation rules.
{
    // Frame rate limited.
    avxPresentFlag_RATE_LIMITED = AFX_BIT(0),

    // Screen tearing explicitly forbidden
    avxPresentFlag_NO_TEARING   = AFX_BIT(1),

    // Frame rate limited, screen tearing forbidden. ('vsync on')
    avxPresentFlag_VSYNC        = (avxPresentFlag_RATE_LIMITED | avxPresentFlag_NO_TEARING),
} avxPresentFlags;

typedef afxBool(*avxPresentNotifier)(afxObject receiver, afxUnit);

AFX_DEFINE_STRUCT(afxDrawOutputCaps)
{
    afxUnit             minBufCnt;
    afxUnit             maxBufCnt;
    afxUnit             currExtent[2];
    afxWhd              minWhd; // D is layer
    afxWhd              maxWhd; // D is layer
    avxVideoTransform   supportedTransforms;
    avxVideoTransform   currTransform;
    avxVideoAlpha       supportedCompositeAlpha;
    avxRasterFlags      supportedUsageFlags;
    avxVideoScaling     supportedScaling;
    afxUnit             supportedModeCnt;
    avxPresentFlags     supportedModes;
};

AFX_DEFINE_STRUCT(afxDrawOutputConfig)
{
    afxReal64           refreshRate;
    avxRange            resolution; // Screen resolution. Absolute extent available.
    afxBool             exclusive; // assume full control of the endpoint

    // canvas
    avxColorSpace       colorSpc; // avxColorSpace_STANDARD; if sRGB isn't present, fall down to LINEAR.
    avxFormat           bufFmt[3]; // formats for color, depth and stencil format, respectively. Pass zero to let driver choose the optimal format. Pass zero to disable depth and/or stencil.
    avxRasterUsage      bufUsage[3]; // DRAW; used as (drawable) target surface for rasterization.
    avxRasterFlags      bufFlags[3];
    avxRange            extent;
    afxBool             resizable;

    // swapchain
    afxUnit             minBufCnt; // usually 2 or 3; double or triple-buffered.
    avxPresentFlags      presentMode; // FIFO; respect the sequence.
    avxVideoAlpha       presentAlpha; // FALSE; ignore transparency when composing endpoint background, letting it opaque.
    avxVideoTransform   presentTransform; // NIL; don't do any transform.
    afxBool             doNotClip; // FALSE; don't do off-screen draw.

    void*               udd[4];
    afxFcc              ext;

    // endpoint
    afxUri              endpoint;
    avxPresentNotifier  endpointNotifyFn;
    afxObject           endpointNotifyObj; // must ensure life of draw output

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

AVX afxDrawOutputConfig const AVX_DEFAULT_DRAW_OUTPUT_CONFIG;

/*
    The function AvxGetDrawOutputContext() is used to retrieve the draw system context associated with a specific drawing output. 
    This context provides access to the drawing system's state, settings, and configuration, which can be necessary for rendering 
    or interacting with the drawing output in a more detailed and controlled manner.
*/

AVX afxDrawSystem AvxGetDrawOutputContext(afxDrawOutput dout);

/*
    The function AvxGetDrawOutputIcd() retrieves the installable client driver (ICD) operating a specific drawing output. 
    This ICD is a lower-level handle that provides access to the physical or virtual hardware device (ex.: video display adapter) 
    used for rendering or displaying the output. The returned ICD can be used for querying device-specific properties, 
    capabilities, or performing advanced operations directly related to the hardware rendering or display system.
*/

AVX afxModule AvxGetDrawOutputIcd(afxDrawOutput dout);

/*
    The primary purpose of AvxDescribeDrawOutput() is to retrieve descriptive information about a drawing output 
    (e.g., a window, display, or framebuffer). This is useful when you need to know the capabilities or configuration of 
    the drawing output before performing rendering or display operations.
*/

AVX void AvxDescribeDrawOutput(afxDrawOutput dout, afxDrawOutputConfig* cfg);

/*
    The primary purpose of AvxGetDrawOutputUdd() is to retrieve custom, user-defined data that has been associated with a 
    specific drawing output (e.g., a render target, display, or framebuffer). This allows the application to store and retrieve 
    arbitrary data that may be needed during rendering or display operations.
*/

AVX void* AvxGetDrawOutputUdd(afxDrawOutput dout, afxUnit slotIdx);

/*
    The function AvxCallDrawOutput() is used to call implementation-specific functionality associated with a specific drawing output 
    and store results in a provided destination buffer. This function is designed to interface with the underlying hardware or rendering 
    context, extracting low-level information about the drawing output, which may include hardware-specific data, driver configurations, 
    or other internal details related to the output device.
*/

AVX afxError AvxCallDrawOutput(afxDrawOutput dout, afxUnit code, ...);

/*
    The AvxQueryDrawOutputResolution() function is used to query various display settings or attributes for a specific drawing output. 
    It provides information about the resolution, refresh rate, and possibly the scaling factors related to the physical aspect ratio 
    of the display output. You would typically call this function when you need to retrieve the current display properties of a 
    particular screen or rendering target.
*/

AVX void AvxQueryDrawOutputResolution
(
    // The drawing output for which resolution details are queried.
    afxDrawOutput dout,
    
    // The width-to-physical aspect ratio (possibly the horizontal scaling factor).
    afxReal64* wpOverHp,
    
    // Pointer to store the current refresh rate (e.g., 60Hz, 120Hz).
    afxReal* refreshRate,
    
    // Pointer to store the current resolution (width x height).
    avxRange* resolution,
    
    // The width-to-physical aspect ratio (possibly the vertical scaling factor).
    afxReal64* wrOverHr
);

/*
    The AvxResetDrawOutputResolution() function is used to change or reset the drawing resolution for a specific output device, 
    adjusting both the display resolution and potentially the refresh rate, while considering the aspect ratio and whether the 
    change is in an exclusive fullscreen mode.
*/

AVX afxError AvxResetDrawOutputResolution
(
    // The drawing output to be reset or configured.
    afxDrawOutput dout,

    // The physical aspect ratio of the display.
    afxReal64 physAspectRatio,

    // The refresh rate to be set (e.g., 60Hz, 120Hz).
    afxReal refreshRate,

    // The desired screen resolution (Width x Height).
    avxRange resolution,

    // A flag indicating if the resolution change is exclusive (possibly fullscreen).
    afxBool exclusive
);

/*
    The primary purpose of AvxAdjustDrawOutput() is to adjust the properties (such as resolution, size, or other dimensions) 
    of the specified drawing output (dout) according to the provided width, height, and depth values (whd).
*/
AVX afxError AvxAdjustDrawOutput
(
    // The drawing output whose properties are to be adjusted.
    afxDrawOutput dout, 

    // The new width, height, and depth to which the drawing output should be adjusted.
    avxRange whd
);

AVX afxError _AvxDoutAdjustNormalized(afxDrawOutput dout, afxV3d const whd);
AVX void _AvxDoutGetExtentNormalized(afxDrawOutput dout, afxV3d whd); // normalized (bethween 0 and 1 over the total available) porportions of exhibition area.


AVX void AvxGetDrawOutputRate(afxDrawOutput dout, afxUnit* rate);

/*
    The function AvxQueryDrawOutputExtent is used to query the extent (size or dimensions) of a drawing output, 
    such as a display surface, framebuffer, or render target. It provides information about the drawing output's 
    resolution and other relevant properties.
*/

AVX void AvxQueryDrawOutputExtent
(
    // The drawing output whose extent (size) is being queried.
    afxDrawOutput dout, 

    // A pointer to a structure or variable that will hold the extent (size) of the drawing output, typically including the width, height, and potentially the depth.
    avxRange* extent,

    // A pointer to a afxReal64 value that will store a ratio or value representing width over height.
    afxReal64* wwOverHw
);

/*
    The AvxGetDrawOutputCanvas() function is used to query a specific canvas (or drawing buffer) associated with a drawing output context. 
    It allows you to retrieve information about a particular rendering surface (e.g., a frame buffer or texture) that may be used 
    for drawing operations. Depending on the system, a canvas might represent an off-screen buffer (like a back buffer for double 
    buffering) or an on-screen rendering target.
*/

AVX afxBool AvxGetDrawOutputCanvas
(
    // Drawing output for which the canvas is being queried.
    afxDrawOutput dout,

    // Index of the buffer or canvas that is being queried.
    afxUnit bufIdx,

    // Pointer to the canvas that will hold the queried information.
    avxCanvas* canvas
);

/*
    The function AvxGetDrawOutputBuffer() is designed to retrieve a specific buffer associated with a drawing output. 
    This function is typically used in graphics systems where multiple rendering buffers are managed and need to be accessed 
    for rendering or presentation. The buffer in question could be a frame buffer, back buffer, or any other type of memory buffer 
    used for rendering.
*/

AVX afxBool AvxGetDrawOutputBuffer
(
    // The drawing output for which you want to retrieve the buffer.
    afxDrawOutput dout,

    // The buffer index that specifies which buffer to retrieve.
    afxUnit bufIdx, 

    // A pointer to where the function will store the reference to the retrieved buffer.
    avxRaster* buffer
);

AVX afxError AvxRevalidateDrawOutputBuffers(afxDrawOutput dout);

/*
    The AvxRequestDrawOutputBuffer() function is designed to request or reserve a buffer for future use. 
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

AVX afxError AvxRequestDrawOutputBuffer
(
    // The drawing output for which to request a buffer.
    afxDrawOutput dout,

    // The timeout duration to wait for the buffer to be available.
    afxUnit64 timeout,

    // A pointer to store the index of the reserved buffer.
    afxUnit *bufIdx
);

/*
    The function AvxRecycleDrawOutputBuffer() is used to dispose of a previously acquired drawing buffer 
    without presenting it to the screen or any output target. This function helps manage resources by 
    allowing the application to release a buffer that is no longer needed, without having to render or display its contents.

    The primary purpose of AvxRecycleDrawOutputBuffer() is to release or dispose of a buffer that was previously requested 
    but is no longer needed. This is often used in cases where the buffer will not be used for rendering or presentation, 
    but the system still needs to manage resources efficiently.
*/

AVX afxError AvxRecycleDrawOutputBuffer
(
    // The drawing output associated with the buffer.
    afxDrawOutput dout,

    // The index of the buffer to discard.
    afxUnit bufIdx
);

/*
    The primary purpose of AvxWaitForDrawOutput() is to synchronize your application with the rendering process. 
    It allows you to block or wait until the drawing output is ready for further operations (e.g., drawing to a surface, 
    reading from a surface, or performing buffer swaps).

    AvxWaitForDrawOutput() is used to block or wait until a specific drawing output (e.g., a render target, framebuffer, or window) 
    is ready for further operations, with a specified timeout to avoid blocking indefinitely.
    It can be useful for synchronizing operations between the CPU and GPU, ensuring that rendering tasks are completed before 
    proceeding with additional steps.
*/

AVX afxError AvxWaitForDrawOutput
(
    // The drawing output to wait for.
    afxDrawOutput dout,

    // The timeout duration to wait for the drawing output to be ready.
    afxTime timeout
);

/*
    The AvxPrintDrawOutput() function is designed to "print" or "export" the contents of a drawing output buffer to an external resource. 
    Depending on the library or framework, "printing" could mean saving the rendered buffer to a file, sending it to a printer, 
    or exporting the drawing in a specific format.

    This could be useful in situations where you need to save a rendered frame or an image to disk (e.g., for screenshot functionality, 
    exporting frames in a video sequence, or saving images for further processing).
*/

AVX afxError AvxPrintDrawOutput
(
    // The drawing output for which the content is to be printed or saved.
    afxDrawOutput dout, 

    // The index of the buffer or canvas (e.g., front buffer, back buffer).
    afxUnit bufIdx,

    avxRasterIo const* op,

    // The index of the execution unit to handle the I/O.
    afxUnit exuIdx, 

    // The URI (Uniform Resource Identifier) specifying the destination or file path.
    afxUri const* uri
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AvxPresentDrawOutputs() function presents rendered outputs (such as frames or images) to a display system or window.
    It handles synchronization between the drawing operations and the presentation phase, ensuring that the outputs are
    presented only after they are fully rendered. It also supports managing multiple outputs and buffer indices for
    double/triple buffering, as well as using fences to signal the completion of the presentation. This function is typically
    used at the end of a rendering cycle to display the final results to the user.
*/

AVX afxError AvxPresentDrawOutputs
(
    // The drawing system where the outputs will be presented.
    afxDrawSystem dsys,

    // A control structure for the presentation settings. 
    // This structure would contain configuration options that manage how the output is presented, such as swap chains, 
    // synchronization details, or specific rendering modes(e.g., vsync settings or buffer flipping).
    avxPresentation* ctrl,

    // A fence that the function will wait on before proceeding with the presentation. 
    // Fences are used to ensure synchronization, especially when working with GPUs. 
    // The function will wait for this fence to be signaled before beginning the presentation. 
    // This ensures that drawing commands are completed before the output is presented.
    avxFence wait,

    // The number of outputs to be presented. This indicates how many items in the @outputs array should be processed.
    afxUnit cnt,

    // An array of draw outputs to be presented. Each output could be an image that has been rendered and is now ready for display.
    afxDrawOutput outputs[],

    // An array of buffer indices specifying which buffers to present. 
    // This indicate which buffer is to be shown as the result of rendering.
    afxUnit const bufIdx[],

    // An array of fences to signal after the presentation is completed. 
    // Once the presentation is done, these fences are signaled, allowing the system to continue processing other tasks 
    // that depend on the completion of the presentation. This is useful for synchronization with other parts of the 
    // rendering pipeline or application logic.
    avxFence signal[]
);

/*
    The AvxEnumerateDrawOutputs() function enumerates the drawing outputs open for a established drawing system. 
    This can include devices like display monitors or render targets, allowing applications to discover and 
    interact with these outputs for rendering or display purposes. The function provides the ability to query 
    a specific range of outputs and retrieve information about each one for further processing.
*/

AVX afxUnit AvxEnumerateDrawOutputs
(
    // The established draw system associated with the enumeration process.
    // The function will query available drawing outputs for the given drawing system (dsys).
    afxDrawSystem dsys, 

    // The starting index for the enumeration of drawing outputs.
    // If there are many outputs available, @first allows you to specify where to start enumerating 
    // (e.g., you might skip some outputs if you want to continue from a specific position in the list).
    afxUnit first, 

    // The number of outputs to retrieve.
    // The function will retrieve up to @cnt drawing outputs, starting from the index specified by @first.
    afxUnit cnt, 

    // An array where the enumerated drawing outputs will be stored.
    afxDrawOutput outputs[]
);

/*
    The AvxInvokeDrawOutputs() function enables custom processing of drawing outputs within a drawing system. 
    By passing a user-defined function and user data, this function allows the application to query and process 
    drawing outputs in a flexible and extensible way. This could be useful for tasks like querying display properties, 
    updating output settings, or interacting with different display or render target devices.
*/

AVX afxUnit AvxInvokeDrawOutputs
(
    // The drawing system whose outputs are to be processed.
    afxDrawSystem dsys, 

    // The starting index for the outputs to be processed.
    // It specifies the position of the first drawing output to be processed. 
    // If you want to skip some outputs, you can adjust this index accordingly.
    afxUnit first, 

    // The number of outputs to process.
    // It defines how many outputs should be processed, starting from the @first index. 
    // The function will process up to @cnt drawing outputs.
    afxUnit cnt, 

    // A pointer to a user-defined function that is called for each drawing output.
    // Its return type is afxBool, which indicates whether the process should be aborted.
    afxBool(*f)(afxDrawOutput, void*), 

    // This is user-defined data that is passed to the function f for each drawing output.
    void *udd
);

/*
    The AvxEvokeDrawOutputs() function enables the application to retrieve a set of drawing outputs from a drawing system 
    and invoke a user-defined function on each output. This allows for customized processing of outputs such as displays, 
    render targets, or other output interfaces. The user-defined data provides flexibility, enabling the user to pass 
    additional context or configuration to the processing function. This function is useful when the application needs to 
    process or configure multiple outputs based on a drawing system.
*/

AVX afxUnit AvxEvokeDrawOutputs
(
    // The drawing system from which the drawing outputs will be retrieved.
    afxDrawSystem dsys, 

    // A user-defined function pointer that is invoked on each retrieved drawing output.
    // The return type is afxBool, which indicates if an draw output should be inserted in @output array.
    afxBool(*f)(afxDrawOutput, void*), 

    // This is user-defined data that is passed to the user-defined function when processing each drawing output.
    void* udd, 

    // The starting index of the drawing outputs to retrieve.
    afxUnit first, 

    // The number of drawing outputs to retrieve and process.
    afxUnit cnt, 

    // An array that will store the retrieved drawing outputs.
    afxDrawOutput outputs[]
);

AVX afxError AvxConfigureDrawOutput
(
    // The drawing system to which the drawing output belongs.
    afxDrawSystem dsys,
    
    // A pointer to the configuration settings to be applied to the drawing output.
    afxDrawOutputConfig* cfg
);

/*
    The AvxOpenDrawOutput() function opens a drawing output with specific settings provided 
    in the configuration structure. This could involve creating a window for rendering, 
    allocating a framebuffer, setting up a full-screen display, or initializing other kinds 
    of graphical outputs.

    Once successfully executed, the function returns a handle to the newly created drawing output (output), 
    which can be used to perform further rendering or interaction.
*/

AVX afxError AvxOpenDrawOutput
(
    // The drawing system where the drawing output will be created or opened.
    afxDrawSystem dsys, 

    // A pointer to the configuration settings to initialize the drawing output.
    afxDrawOutputConfig const* cfg, 
    
    // A pointer to store the created or opened drawing output.
    afxDrawOutput* output
);

#endif//AVX_DRAW_OUTPUT_H
