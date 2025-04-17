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

/**
    No Qwadro, avxCanvas é um recurso que combina operações de framebuffer e draw scope em outras APIs.
    An surface is a memory location that can act as a buffer for the canvas. Think of it as an image or renderbuffer.
*/

#ifndef AVX_CANVAS_H
#define AVX_CANVAS_H

#include "qwadro/inc/draw/io/avxRaster.h"

#define AVX_MAX_AUX_BUFFERS (2)
#define AVX_MAX_COLOR_BUFFERS (8)
#define AVX_MAX_CANVAS_BUFFERS (AVX_MAX_COLOR_BUFFERS + AVX_MAX_AUX_BUFFERS)

typedef enum afxCanvasFlag
{
    afxCanvasFlag_DEPTH         = AFX_BIT(0),
    afxCanvasFlag_STENCIL       = AFX_BIT(1),
    afxCanvasFlag_COMBINED_DS   = AFX_BIT(2)
} afxCanvasFlags;

AFX_DEFINE_STRUCT(avxDrawSurface)
{
    // a avxRaster handle which will be used as the buffer.
    avxRaster       ras;
    
    // format used to create an image used with this canvas. Ignored if @ras is present.
    avxFormat       fmt; // layout
    // usage used to create an image used with this canvas. Ignored if @ras is present.
    avxRasterUsage  rasUsage;
    // flags used to create an image that will be used with this canvas. Ignored if @ras is present.
    avxRasterFlags  rasFlags;

    afxUnit         sampleCnt; // layout. I am still in doubt if I let this here or in pipeline.
};

AFX_DEFINE_STRUCT(avxCanvasConfig)
{
    afxFlags        flags;
    // the dimensions of the canvas.
    avxRange        whd;
    // the number of attachments.
    afxUnit         surCnt;
    avxDrawSurface  surfs[AVX_MAX_CANVAS_BUFFERS];
    afxString       tag;
};

/*
    The AvxGetCanvasFlags() function testS specific flags of a specified avxCanvas. 
    It uses a bitmask to check whether certain flags are set for the canvas.
*/

AVX afxCanvasFlags  AvxGetCanvasFlags
(
    // The canvas object that you want to test.
    avxCanvas       canv, 

    // A bitmask that specifies which flags to test for. 
    // Each bit represents a specific condition of the canvas (e.g., whether a specific feature is enabled).
    afxCanvasFlags  bitmask
);

/*
    The AvxGetCanvasArea() function retrieves the dimensions (width and height) of a given canvas. 
    This information is essential for rendering, scaling, and layout management in graphical applications, 
    particularly when dealing with dynamic or resizable drawing surfaces.
*/

AVX avxRange        AvxGetCanvasArea
(
    // A canvas object, which represents a framebuffer in the system.
    avxCanvas       canv,
    avxOrigin       origin
);

/*
    The AvxQueryDrawBufferSlots() function populates the provided pointers (colorSlotCnt, dSlotIdx, and sSlotIdx) 
    with relevant data and returns the total number of attachment slots that can be used.
*/

AVX afxUnit         AvxQueryDrawBufferSlots
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

    It returns a boolean value indicating whether the retrieval was successful or not.
*/

AVX afxBool         AvxGetColorBuffers
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
    The AvxGetDepthBuffers() function retrieves the depth buffer and stencil buffer associated with a given canvas. 
    It populates the provided pointers for the depth and stencil buffers, which can be used for further processing or rendering.

    It returns a boolean value indicating whether the retrieval was successful or not.
*/

AVX afxBool         AvxGetDepthBuffers
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

AVX afxError        AvxPrintDrawBuffer
(
    // The canvas from which the draw buffer is to be printed.
    avxCanvas       canv, 

    // The surface index. 
    // This parameter specifies which surface or buffer on the canvas to print. 
    // If the canvas has multiple surfaces or buffers (e.g., for double or triple buffering), 
    // this parameter determines which one to print.
    afxUnit         surIdx, 

    // A structure specifying the I/O operation on the raster attached to the canvas.
    avxRasterIo const*op, 

    // The execution unit index. 
    // The specific unit that will be used to handle the printing process.
    afxUnit         exuIdx, 

    // The URI where the buffer should be printed or exported to. 
    // This could be a file path or a network location.
    afxUri const*   uri
);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AvxCoacquireCanvas
// Acquire a new buffered drawing canvas object.
(
    // the draw system that provides the canvas.
    afxDrawSystem   dsys,
    // the specification of a newly created canvas
    avxCanvasConfig const*cfg,
    // the number of canvas to be created.
    afxUnit         cnt,
    // an array of avxCanvas handles in which the resulting objects are returned.
    avxCanvas       canvases[]
);

#endif//AVX_CANVAS_H
