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

#ifndef AVX_DRAW_CONTEXT_H
#define AVX_DRAW_CONTEXT_H

/**
    Command buffers are objects used to record commands which can be subsequently submitted to a device queue for execution. 
    There are two levels of command buffers - primary command buffers, which can execute secondary command buffers, and which are submitted to queues, 
    and secondary command buffers, which can be executed by primary command buffers, and which are not directly submitted to queues.
*/
/*
    When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the draw context state.
    Pipeline object state that is specified as dynamic is not applied to the draw context state at this time.
    Instead, dynamic state can be modified at any time and persists for the lifetime of the draw context, 
    or until modified by another dynamic state setting command, 
    or made invalid by another pipeline bind with that state specified as static.
*/

// COMMAND BATCH --- a group of records processed as a single unit, usually without input from a user.

#include "qwadro/inc/draw/op/avxDrawing.h"
#include "qwadro/inc/draw/op/avxProvision.h"
#include "qwadro/inc/draw/afxDrawBridge.h"

typedef enum afxDrawContextFlag
{
    // specifies that each recording of the draw context will only be submitted once, 
    // and the draw context will be reset and recorded again between each submission.
    afxDrawContextFlag_ONCE,

    // specifies that a secondary draw context is considered to be entirely inside a render canvas.
    // If this is a primary draw context, then this bit is ignored.
    afxDrawContextFlag_RESUME,

    // specifies that a draw context can be resubmitted to any queue of the same queue family while it is in the pending state, 
    // and recorded into multiple primary command buffers.
    afxDrawContextFlag_SIMULTANEOUS,
} afxDrawContextFlags;

AVX afxUnit     AvxGetCommandPort(afxDrawContext dctx);

AVX afxUnit     AvxGetCommandPool(afxDrawContext dctx);

/// Finish recording a draw context.

AVX afxError    AvxCompileDrawCommands(afxDrawContext dctx);

/// Reset a draw context to the initial state.
/// Any primary draw context that is in the recording or executable state and has @dctx recorded into it, becomes invalid.

AVX afxError    AfxExhaustDrawContext(afxDrawContext dctx, afxBool freeMem);

////////////////////////////////////////////////////////////////////////////////


/*
    The AvxAcquireDrawContexts() function allocates multiple draw contexts (which could be command buffers 
    or command contexts) for recording drawing commands for a specified draw system.
    
    This function allocates @cnt draw contexts (command buffers) and returns them in the @contexts[] array.
    If the allocation of any of the draw contexts fails, all successfully allocated draw contexts are freed and set to NIL.
    The function returns an error code if any allocation fails.
*/
// NOTE 1: In Mantle, unlike Vulkan, we specify the queue capabilites instead of exuIdx.
// NOTE 2: In Mantle and Vulkan, we specify optimization hints (like ONE_TIME) when we starts recording a context (with a Begin() function);

AVX afxError        AvxAcquireDrawContexts
(
    // The draw system to provide the drawing contexts.
    afxDrawSystem   dsys, 
    // The execution unit index (in Mantle, this refers to a specific GPU hardware unit or execution engine).
    // NOTE 1 mentions that, in Mantle, unlike Vulkan, you specify queue capabilities rather than the execution unit index.
    afxUnit         exuIdx, 
    // The queue index in the graphics API (e.g., rendering or compute queue), indicating the command queue to use for these draw contexts.
    afxUnit         queIdx, 
    // The count of the draw contexts wanted to be allocated.
    afxUnit         cnt, 
    // An array of draw contexts to be allocated.
    afxDrawContext  contexts[]
);

/*
    The AvxRecycleDrawContexts() function recycles draw contexts, releasing resources or allowing for reuse.
    This function is used to opportunistically try to recycle draw contexts (potentially preserving allocations) or dispose it.
    
    A flag can be used to indicate whether all or most of the resources owned by the context should be reclaimed by the system.
    If this flag is not set, then the draw context may hold onto memory resources and reuse them when recording commands.
*/

AVX afxError        AvxRecycleDrawContexts
(
    // The draw system providing the draw contexts.
    afxDrawSystem   dsys,
    // A flag that indicates whether all or most of the resources owned by the context should be reclaimed by the system.
    afxBool         exhaust,
    // The count of draw contexts to be recycled.
    afxUnit         cnt,
    // An array of draw contexts that need to be recycled.
    afxDrawContext  contexts[]
);

/*
    The AvxExecuteDrawCommands() function submits multiple draw commands to a drawing system for execution.
    It allows for synchronized execution using a fence object, ensuring that commands are executed in the proper order.
    This function is useful in graphics systems where multiple draw contexts (command buffers) need to be processed,
    and synchronization between CPU and GPU tasks is required. By managing the submission through a control structure
    and using a fence for synchronization, the function ensures that drawing commands are executed efficiently and correctly.
*/

AVX afxError        AvxExecuteDrawCommands
(
    // The drawing system where the draw commands will be executed.
    afxDrawSystem   dsys,

    // A control structure that might define various parameters related to how the draw commands are submitted, 
    // such as synchronization or execution order. This encapsulate details like submission flags, 
    // command queues, or context properties that manage how and when the draw commands are executed.
    avxSubmission*  ctrl,

    // The number of draw contexts to be executed.
    // This tells the function how many contexts in the @contexts array should be processed.
    afxUnit         cnt,

    // An array of draw contexts that represent the individual drawing commands or operations to be executed. 
    // Each afxDrawContext could contain specific information about a set of drawing commands, such as commands 
    // for rendering objects, setting up shaders, and managing resources.
    afxDrawContext  contexts[],

    // A fence object to be signaled, used for synchronization.
    avxFence        fence
);

#endif//AVX_DRAW_CONTEXT_H
