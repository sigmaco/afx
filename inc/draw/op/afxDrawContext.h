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
    When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the command buffer state.
    Pipeline object state that is specified as dynamic is not applied to the command buffer state at this time.
    Instead, dynamic state can be modified at any time and persists for the lifetime of the command buffer, 
    or until modified by another dynamic state setting command, 
    or made invalid by another pipeline bind with that state specified as static.
*/

// COMMAND BATCH --- a group of records processed as a single unit, usually without input from a user.

#include "qwadro/inc/draw/op/avxDrawOps.h"

typedef enum avxCmdbUsage
{
    /// specifies that each recording of the command buffer will only be submitted once, 
    /// and the command buffer will be reset and recorded again between each submission.
    avxCmdbUsage_ONCE,

    /// specifies that a secondary command buffer is considered to be entirely inside a render canvas.
    /// If this is a primary command buffer, then this bit is ignored.
    avxCmdbUsage_RESUME,

    /// specifies that a command buffer can be resubmitted to any queue of the same queue family while it is in the pending state, 
    /// and recorded into multiple primary command buffers.
    avxCmdbUsage_SIMULTANEOUS,
} avxCmdbUsage;

AVX afxUnit     AfxGetCommandPort(afxDrawContext dctx);

AVX afxUnit     AfxGetCommandPool(afxDrawContext dctx);

/// Finish recording a command buffer.

AVX afxError    AfxCompileDrawCommands(afxDrawContext dctx);

/// Reset a command buffer to the initial state.
/// Any primary command buffer that is in the recording or executable state and has @dctx recorded into it, becomes invalid.

AVX afxError    AfxResetDrawContext(afxDrawContext dctx, afxBool freeMem);

/// 'exhaust' specifies that most or all memory resources currently owned by the command buffer should be returned to the parent command pool. If this flag is not set, then the command buffer may hold onto memory resources and reuse them when recording commands. @dctx is moved to the initial state.

AVX afxError    AfxRecycleDrawContext(afxDrawContext dctx, afxBool exhaust);

////////////////////////////////////////////////////////////////////////////////

/// AfxAcquireDrawContexts can be used to allocate multiple command buffers.
/// If the allocation of any of those command buffers fails, 
/// the implementation must free all successfully allocated command buffer objects from this command, 
/// set all entries of the buffers[] array to NIL and return the error.

AVX afxError    AfxAcquireDrawContexts(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, afxDrawContext batches[]);

#endif//AVX_DRAW_CONTEXT_H
