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

  ////////////////////////////////////////////////////////////////////////////////
 // QWADRO BUFFERED DRAWING CANVAS                                             //
////////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_CANVAS_H
#define AVX_CANVAS_H

/**
    No Qwadro, avxCanvas é um recurso que combina operações de framebuffer e draw scope em outras APIs.
    An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.

    The Qwadro API has evolved its terminology for clarity, which is common as graphics systems grow more abstract or modular.
    The old term 'Framebuffer' has historically been misleading because it implies the object is some type of buffer in memory.
    This term also historically referred to both the drawing target and the memory where it's stored (VRAM).

    The new term 'Canvas' represents only the frame or render target --- the place where rendering commands are directed.
    It does not itself hold the pixel data buffer. Canvas is only the "frame" of a framebuffer in a sense of arrangement.
    A 'Raster' is the actual VRAM-buffered image --- the memory/storage side. Canvas binds one or more Rasters as attachments.
*/

#include "qwadro/inc/draw/io/avxRaster.h"

#define AVX_MAX_AUX_BUFFERS (2)
#define AVX_MAX_COLOR_BUFFERS (8)
#define AVX_MAX_CANVAS_BUFFERS (AVX_MAX_COLOR_BUFFERS + AVX_MAX_AUX_BUFFERS)

typedef enum avxCanvasFlag
{
    // No annexes.
    // Each buffer must be at least as large as the canvas dimensions.
    avxCanvasFlag_VOID          = AFX_BITMASK(0),
    // Has color annex.
    avxCanvasFlag_COLOR         = AFX_BITMASK(1),
    // Has depth annex.
    avxCanvasFlag_DEPTH         = AFX_BITMASK(2),
    // Has stencil annex.
    avxCanvasFlag_STENCIL       = AFX_BITMASK(3),
    // Has a combined depth+stencil annex.
    avxCanvasFlag_COMBINED_DS   = AFX_BITMASK(4),
    // Draw buffers can be rebinded after acquisition of canvas.
    // If not flagged so, managed allocation takes place for any missing buffer.
    avxCanvasFlag_REBINDABLE    = AFX_BITMASK(5),
} avxCanvasFlags;

AFX_DEFINE_STRUCT(avxCanvasBuffer)
{
    // a avxRaster handle which will be used as the buffer.
    avxRaster       ras;
    // format used to create an image used with this canvas.
    // Ignored if @ras is present.
    avxFormat       fmt; // layout
    // usage used to create an image used with this canvas.
    // Ignored if @ras is present.
    avxRasterUsage  usage;
    // flags used to create an image that will be used with this canvas. 
    // Ignored if @ras is present.
    avxRasterFlags  flags;
    // layout. I am still in doubt if I let this here or in pipeline.
    afxUnit         sampleCnt; 
};

/*
    The AvxGetCanvasFlags() function testS specific flags of a specified avxCanvas. 
    It uses a bitmask to check whether certain flags are set for the canvas.
*/

AVX avxCanvasFlags  AvxGetCanvasFlags
(
    // The canvas object that you want to test.
    avxCanvas       canv, 

    // A bitmask that specifies which flags to test for. 
    // Each bit represents a specific condition of the canvas (e.g., whether a specific feature is enabled).
    avxCanvasFlags  bitmask
);

/*
    The AvxGetCanvasArea() function retrieves the dimensions (width and height) of a given canvas. 
    This information is essential for rendering, scaling, and layout management in graphical applications, 
    particularly when dealing with dynamic or resizable drawing surfaces.
    It also provides the area's magnitude (width × height in pixels) as a return value.
*/

AVX afxUnit         AvxGetCanvasArea
(
    // A canvas object that represents a framebuffer, render target, or drawable surface.
    avxCanvas       canv,
    // Represents the origin or coordinate space used to define the area.
    avxOrigin       origin,
    // A pointer to an afxRect structure that will be filled with the canvas's area.
    afxRect*        area
);

/*
    The AvxQueryCanvasSlots() function populates the provided pointers (colorSlotCnt, dSlotIdx, and sSlotIdx) 
    with relevant data and returns the total number of attachment slots that can be used.
*/

AVX afxUnit         AvxQueryCanvasSlots
// Returns the total of buffer slots in canvas.
(
    // The canvas in which the slots are being managed.
    avxCanvas       canv, 

    // A recipient to hold how many slots are dedicated to color storage or processing.
    afxUnit*        colorSlotCnt, 

    // A recipient to hold the index of the slot where a depth buffer is attached.
    afxUnit*        dSlotIdx, 

    // A recipient to hold the index of the slot where a stencil buffer is attached.
    afxUnit*        sSlotIdx
);

/*
    The AvxGetDrawBuffers() function retrieves a subset of drawing buffers attached to the specified canvas. 
    It allows you to specify where to start the query, how many buffers to retrieve, 
    and an recipient array to hold the retrieved buffer handles.

    Returns the number of valid slots inserted in the recipient array.
*/

AVX afxUnit         AvxGetDrawBuffers
(
    // The canvas for which the draw buffers are requested.
    avxCanvas       canv, 

    // The base index for the buffer slots to retrieve. 
    // If there are multiple drawing buffers, the baseSlotIdx indicates where to start the query for buffers. 
    // This is useful when you need a subset of available buffers, not just the first one.
    afxUnit         baseSlotIdx, 

    // The number of buffers to retrieve. 
    // This specifies how many buffers you wish to get starting from the @baseSlotIdx. 
    // The function will return this number of buffers into the @rasters array.
    afxUnit         cnt, 

    // An array of rasters that will be populated with the retrieved buffers. 
    // Each element in the array corresponds to a drawing buffer associated with the canvas.
    // Each item in this array represents a buffer that holds rendered data for further processing or display.
    avxRaster       rasters[]
);

/*
    The AvxGetColorBuffers() function retrieves only color buffers from the canvas. 
    This function will allow you to specify a starting index, how many color buffers to retrieve, 
    and provide an recipient array to hold the retrieved color buffers.

    It returns a amount of retrivied color buffers.
*/

AVX afxUnit         AvxGetColorBuffers
(
    // The canvas object from which the color buffers are being retrieved.
    avxCanvas       canv, 

    // The base index of the first color buffer to start retrieving. 
    // This is useful when you want a subset of the available color buffers, not necessarily starting from the first.
    afxUnit         baseSlotIdx, 

    // The number of color buffers to retrieve. 
    // This specifies how many color buffers you want to get starting from the @baseSlotIdx.
    afxUnit         cnt, 

    // An recipient array that will be populated with the retrieved color buffers. 
    // Each element in this array will hold one of the color buffers.
    avxRaster       rasters[]
);

/*
    The AvxGetAuxBuffers() function retrieves the depth buffer and stencil buffer associated with a given canvas. 
    It populates the provided pointers for the depth and stencil buffers, which can be used for further processing or rendering.

    It returns the amount of retrieved auxiliar buffers.
*/

AVX afxUnit         AvxGetAuxBuffers
(
    // The canvas from which the depth and stencil buffers are being retrieved.
    avxCanvas       canv, 
    // A recipient to store the depth buffer associated with the canvas.
    avxRaster*      depth, 

    // A recipient to store the stencil buffer associated with the canvas.
    avxRaster*      stencil
);

/*
    The AvxPrintDrawBuffer() function prints (exports) drawing buffers from a canvas to a specified URI. 
    It provides control over the printing operation, including options for the output format, execution unit, 
    and destination location. This function can be used for saving graphical content to a file, debugging 
    rendered frames, or exporting buffers to remote destinations.
*/

AVX afxError            AvxPrintDrawBuffer
(
    // The canvas from which the draw buffer is to be printed.
    avxCanvas           canv, 

    // The surface index. 
    // This parameter specifies which surface or buffer on the canvas to print. 
    // If the canvas has multiple surfaces or buffers (e.g., for double or triple buffering), 
    // this parameter determines which one to print.
    afxUnit             surIdx, 

    // A structure specifying the I/O operation on the raster attached to the canvas.
    avxRasterIo const*  op,

    // The URI where the buffer should be printed or exported to. 
    // This could be a file path or a network location.
    afxUri const*       uri, 

    // The mask specifying the execution unit.
    // The specific unit that will be used to handle the printing process.
    afxMask             exuMask
);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(avxCanvasConfig)
// A structure specifying a canvas configuration.
{
    // Flags.
    avxCanvasFlags      flags;
    // The dimensions of the canvas.
    avxRange            whd;
    // Multisampling order (2^lodCnt); 1 = 1x, 2 = 2x, 3 = 4x, 4 = 8x, ...
    afxUnit             lodCnt;
    // the number of attachments.
    afxUnit             slotCnt;
    avxCanvasBuffer     slots[AVX_MAX_CANVAS_BUFFERS];
    afxString           tag;
    void*               udd;
};

/*
    The AvxConfigureCanvas() function configures a canvas configuration that will be later used to acquire canvas instances.

    Set up the config to describe dimensions, attachment types (color, depth, etc.), formats, sample counts, etc.
    Not creating the framebuffer yet, just building the recipe.
*/

AVX afxError            AvxConfigureCanvas
(
    // the draw system that provides the canvas.
    afxDrawSystem       dsys, 
    // the specification for a new canvas
    avxCanvasConfig*    cfg
);

/*
    The AvxCoacquireCanvas() acquires one or more actual canvas objects based on the configuration (cfg). 
    This is where real GPU resources are created; framebuffers, images, views, etc.

    Example:
        avxCanvas myCanvases[3];
        AvxCoacquireCanvas(dsys, &cfg, 3, myCanvases); // Create triple-buffered canvas set
*/

AVX afxError                AvxCoacquireCanvas
// Acquire a new buffered drawing canvas object.
(
    // the draw system that provides the canvas.
    afxDrawSystem           dsys,
    // the specification of a newly created canvas
    avxCanvasConfig const*  cfg,
    // the number of canvas to be created.
    afxUnit                 cnt,
    // an array of avxCanvas handles in which the resulting objects are returned.
    avxCanvas               canvases[]
);

#endif//AVX_CANVAS_H
