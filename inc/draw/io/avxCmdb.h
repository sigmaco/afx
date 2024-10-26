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

#ifndef AVX_CMDB_H
#define AVX_CMDB_H

#include "qwadro/inc/draw/pipe/avxDrawOps.h"

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

typedef enum avxCmdbState
/// Each command buffer is always in one of the following states
{
    /// When a command buffer is allocated, it is in the initial state.
    /// Some commands are able to reset a command buffer (or a set of command buffers) back to this state from any of the executable, recording or invalid state.
    /// Command buffers in the initial state canv only be moved to the recording state, or freed.
    avxCmdbState_INITIAL = 1,

    /// BeginCmdBuffer() changes the state of a command buffer from the initial state to the recording state.
    /// Once a command buffer is in the recording state, AvxCmd* commands canv be used to record to the command buffer.
    avxCmdbState_RECORDING,

    /// AfxCompileCmdBuffer() ends the recording of a command buffer, and moves it from the recording state to the executable state.
    /// Executable command buffers canv be submitted, reset, or recorded to another command buffer.
    avxCmdbState_EXECUTABLE,

    /// Queue submission of a command buffer changes the state of a command buffer from the executable state to the pending state.
    /// Whilst in the pending state, applications must not attempt to modify the command buffer in any way - as the device may be processing the commands recorded to it.
    /// Once execution of a command buffer completes, the command buffer either reverts back to the executable state, or if it was recorded with ONCE flag, it moves to the invalid state.
    /// A synchronization command should be used to detect when this occurs.
    avxCmdbState_PENDING,

    /// Some operations, such as modifying or deleting a resource that was used in a command recorded to a command buffer, will transition the state of that command buffer into the invalid state.
    /// Command buffers in the invalid state canv only be reset or freed.
    avxCmdbState_INVALID,

    avxCmdbState_INTERNAL_EXECUTING,
} avxCmdbState;

AVX avxCmdbState    AfxGetCmdBufferState(avxCmdb cmdb);

AVX afxUnit          AfxGetCmdBufferPort(avxCmdb cmdb);

AVX afxUnit          AfxGetCmdBufferPool(avxCmdb cmdb);

AVX afxCmdId        AvxCmdPushDebugScope
/// Open a command buffer debug label region.
(
    avxCmdb         cmdb,
    afxString const*name, /// the name of the label.
    afxColor const  color /// is an optional color associated with the label.
);

AVX afxCmdId        AvxCmdPopDebugScope
/// Close a command buffer label region.
(
    avxCmdb         cmdb
);

AVX afxCmdId        AvxCmdInsertDebugLabel
/// Insert a label into a command buffer.
(
    avxCmdb         cmdb,
    afxString const*name, /// the name of the label.
    afxColor const  color /// is an optional color associated with the label.
);

////////////////////////////////////////////////////////////////////////////////


AVX afxError        AfxRecordCmdBuffers_
(
    avxCmdbUsage    usage, /// Bitmask specifying usage behavior for command buffer.
    afxUnit          cnt,
    avxCmdb         buffers[] /// is the handle of the command buffer which is to be put in the recording state.
);

/// Reset a command buffer to the initial state.
/// Any primary command buffer that is in the recording or executable state and has @cmdb recorded into it, becomes invalid.

////////////////////////////////////////////////////////////////////////////////

/// AfxOpenDrawBatches can be used to allocate multiple command buffers.
/// If the allocation of any of those command buffers fails, 
/// the implementation must free all successfully allocated command buffer objects from this command, 
/// set all entries of the buffers[] array to NIL and return the error.

AVX afxError    AfxOpenDrawBatches(afxDrawContext dctx, afxUnit queIdx, afxUnit cnt, avxCmdb batches[]);

/// Start recording a command buffer.

AVX afxError    AfxOpenCmdBuffers(afxDrawInput din, afxUnit poolIdx, avxCmdbUsage usage, afxUnit cnt, avxCmdb batches[]);

/// Finish recording a command buffer.

AVX afxError    AfxCompileDrawBatches(afxUnit cnt, avxCmdb buffers[]);

/// 'exhaust' specifies that most or all memory resources currently owned by the command buffer should be returned to the parent command pool. If this flag is not set, then the command buffer may hold onto memory resources and reuse them when recording commands. @cmdb is moved to the initial state.

AVX afxError    AfxRecycleCmdBuffers(afxBool exhaust, afxUnit cnt, avxCmdb buffers[]);

#endif//AVX_CMDB_H
