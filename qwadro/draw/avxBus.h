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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DRAWING DEVICE EXECUTION BUS                                      //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_BUS_H
#define AVX_BUS_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/base/afxUri.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/draw/avxFormat.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/draw/avxRasterization.h"

typedef enum avxBus
// Enumerant specifing a pipelining bus.
// In AVX, a pipelining bus is a distinct set of conductors carrying data and control signals within a graphics device, 
// to which pieces of pipeline state may be connected in parallel.
{
    // Specifies the rasterization/graphics bus.
    avxBus_GFX,
    // Specifies the compute bus.
    avxBus_PCX,
    // Specifies the ray tracing bus.
    avxBus_RTX,

    avxBus_TOTAL
} avxBus;

typedef enum avxBusStage
// Bitmask-based value specifying pipeline stages.
{
    // NIL,
    // Pseudo-stage indicating execution on the host of reads/writes of device memory. 
    // This stage is not invoked by any commands recorded in a draw context.
    avxBusStage_HOST        = AFX_BITMASK(0),

    // All operations performed by all commands supported on the queue it is used with.
    // This is a general "catch-all" stage that includes all commands in the pipeline.
    // Often used for full synchronization when you're not sure what stages are involved.
    avxBusStage_COMMANDS    = AFX_BITMASK(1),

    // First synchronization scope: Source of the synchronization (where an operation happens-before).
    // Second synchronization scope: Destination of the synchronization (where the synchronized operation is allowed to begin).
    // This is handled via pipeline stage masks and access masks, and the new synchronization APIs.
    // In the first scope (srcStageMask):
    //  - Represents "before any commands in the pipeline".
    //  - No commands are considered to be in-flight yet.
    //  - Effectively means "nothing"; so it’s equivalent to avxBusStage == NIL.
    //  - Used when no previous stages are relevant for the sync.
    // In the second scope (dstStageMask):
    //  - Represents "before any commands start".
    //  - Since synchronization ensures something happens before the dstStageMask, this means the command being synchronized to can start only after all previous commands complete.
    //  - With accessMask = 0, it becomes a full memory barrier, effectively like avxBusStage_COMMANDS (but with some nuance; see below).
    //  - So in this context, it acts like a full barrier when paired with accessMask = 0.
    avxBusStage_TOP         = AFX_BITMASK(2),
    
    // First synchronization scope (srcStageMask): Source of the sync; the stages you're waiting on.
    // Second synchronization scope (dstStageMask): Destination of the sync; the stages that must wait.
    // In the first scope (srcStageMask):
    //  - Represents after all commands have completed.
    //  - So, it's a way to say "wait for everything to be done".
    //  - With accessMask = 0, this is effectively equivalent to avxBusStage_COMMANDS with no accessMask.
    //  - Acts as a full barrier (waits for all previous commands, but doesn't make any memory visible unless access mask is used).
    // In the second scope(dstStageMask):
    //  - avxBusStage_BOTTOM is the last possible stage in the pipeline.
    //  - So there's nothing that can execute after it.
    //  - Specifying this as the destination stage means : "synchronize to... nothing".
    //  - Thus, it is equivalent to avxBusStage == NIL.
    //  - No actual synchronization occurs. Nothing is delayed.
    avxBusStage_BOTTOM      = AFX_BITMASK(3),
    
    // The execution of all copy commands.
    avxBusStage_COPY        = AFX_BITMASK(4),
    
    // The execution of all resolve commands.
    avxBusStage_RESOLVE     = AFX_BITMASK(5),

    // The execution of all blit commands.
    avxBusStage_BLIT        = AFX_BITMASK(6),
    
    // The execution of clear/fill commands, with the exception of AmxCmdClearCanvas.
    avxBusStage_CLEAR       = AFX_BITMASK(7),
    
    // The following commands: copies, blits, resolves, clears.
    avxBusStage_TRANSFER    = (avxBusStage_COPY | avxBusStage_RESOLVE | avxBusStage_BLIT | avxBusStage_CLEAR),

    // The execution of a compute shader.
    avxBusStage_COMPUTE     = AFX_BITMASK(8),

    // Where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    avxBusStage_DRAW_IND    = AFX_BITMASK(9),
    
    // The stage of the pipeline where index buffers are consumed.
    avxBusStage_VTX_IDX     = AFX_BITMASK(10),

    // The stage of the pipeline where vertex buffers are consumed.
    avxBusStage_VTX_ATTR    = AFX_BITMASK(11),

    // The stage of the pipeline where index buffers are consumed.
    avxBusStage_VTX_IN      = (avxBusStage_VTX_IDX | avxBusStage_VTX_ATTR),

    // The vertex shader stage.
    avxBusStage_VERTEX      = AFX_BITMASK(12),

    // The tessellation control shader stage.
    avxBusStage_DOMAIN      = AFX_BITMASK(13),

    // The tessellation evaluation shader stage.
    avxBusStage_HULL        = AFX_BITMASK(14),

    // The primitive (aka geometry) shader stage.
    avxBusStage_PRIM        = AFX_BITMASK(15),

    // All supported pre-rasterization stages, including vertex, tesselation, primitive and mesh shaders.
    avxBusStage_PRERASTER   = (avxBusStage_VERTEX | avxBusStage_DOMAIN | avxBusStage_HULL | avxBusStage_PRIM),

    // where early fragment tests (depth and stencil tests before fragment shading) are performed. 
    // This stage also includes draw scope load operations for canvas' attached buffers with a depth/stencil format.
    avxBusStage_EARLY_TESTS = AFX_BITMASK(16),

    // The fragment shader stage.
    avxBusStage_FRAGMENT    = AFX_BITMASK(17),

    // Where late fragment tests (depth and stencil tests after fragment shading) are performed. 
    // This stage also includes draw scope store operations for canvas' attached buffers with a depth/stencil format.
    avxBusStage_LATE_TESTS  = AFX_BITMASK(18),

    // After blending, where the final color values are output from the pipeline. This stage includes blending, 
    // logic operations, draw scope load and store operations for color buffers, draw scope multisample resolve operations, 
    // and AvxCmdClearCanvas.
    avxBusStage_COLOR_OUT   = AFX_BITMASK(19),
        
    // The execution of all graphics pipeline stages.
    avxBusStage_GRAPHICS    = ( avxBusStage_DRAW_IND | 
                                avxBusStage_VTX_IN |
                                avxBusStage_VERTEX |
                                avxBusStage_DOMAIN |
                                avxBusStage_HULL |
                                avxBusStage_PRIM |
                                avxBusStage_FRAGMENT |
                                avxBusStage_EARLY_TESTS |
                                avxBusStage_LATE_TESTS |
                                avxBusStage_COLOR_OUT),

    avxBusStage_VIDDEC      = AFX_BITMASK(20),

    avxBusStage_VIDENC      = AFX_BITMASK(21),
} avxBusStage;

#endif//AVX_BUS_H
