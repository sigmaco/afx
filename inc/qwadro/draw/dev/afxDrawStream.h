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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// Command buffers are objects used to record commands which can be subsequently submitted to a device queue for execution. 
/// There are two levels of command buffers - primary command buffers, which can execute secondary command buffers, and which are submitted to queues, 
/// and secondary command buffers, which can be executed by primary command buffers, and which are not directly submitted to queues.

/*
When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the command buffer state.
Pipeline object state that is specified as dynamic is not applied to the command buffer state at this time.
Instead, dynamic state can be modified at any time and persists for the lifetime of the command buffer, or until modified by another dynamic state setting command, or made invalid by another pipeline bind with that state specified as static.
*/

#ifndef AVX_DRAW_STREAM_H
#define AVX_DRAW_STREAM_H

#include "qwadro/draw/pipe/afxDrawOps.h"

typedef enum afxDrawStreamUsage
{
    /// specifies that each recording of the command buffer will only be submitted once, 
    /// and the command buffer will be reset and recorded again between each submission.
    afxDrawStreamUsage_ONCE,

    /// specifies that a secondary command buffer is considered to be entirely inside a render canvas.
    /// If this is a primary command buffer, then this bit is ignored.
    afxDrawStreamUsage_RESUME,

    /// specifies that a command buffer can be resubmitted to any queue of the same queue family while it is in the pending state, 
    /// and recorded into multiple primary command buffers.
    afxDrawStreamUsage_SIMULTANEOUS,
} afxDrawStreamUsage;

typedef enum afxDrawStreamState
/// Each command buffer is always in one of the following states
{
    /// When a command buffer is allocated, it is in the initial state.
    /// Some commands are able to reset a command buffer (or a set of command buffers) back to this state from any of the executable, recording or invalid state.
    /// Command buffers in the initial state canv only be moved to the recording state, or freed.
    afxDrawStreamState_INITIAL = 1,

    /// BeginDrawStream() changes the state of a command buffer from the initial state to the recording state.
    /// Once a command buffer is in the recording state, AfxCmd* commands canv be used to record to the command buffer.
    afxDrawStreamState_RECORDING,

    /// AfxCompileDrawStream() ends the recording of a command buffer, and moves it from the recording state to the executable state.
    /// Executable command buffers canv be submitted, reset, or recorded to another command buffer.
    afxDrawStreamState_EXECUTABLE,

    /// Queue submission of a command buffer changes the state of a command buffer from the executable state to the pending state.
    /// Whilst in the pending state, applications must not attempt to modify the command buffer in any way - as the device may be processing the commands recorded to it.
    /// Once execution of a command buffer completes, the command buffer either reverts back to the executable state, or if it was recorded with ONCE flag, it moves to the invalid state.
    /// A synchronization command should be used to detect when this occurs.
    afxDrawStreamState_PENDING,

    /// Some operations, such as modifying or deleting a resource that was used in a command recorded to a command buffer, will transition the state of that command buffer into the invalid state.
    /// Command buffers in the invalid state canv only be reset or freed.
    afxDrawStreamState_INVALID,

    afxDrawStreamState_INTERNAL_EXECUTING,
} afxDrawStreamState;

#ifdef _AVX_DRAW_C
#ifdef _AVX_DRAW_STREAM_C
#ifndef _AVX_DRAW_STREAM_IMPL
AFX_OBJECT(afxDrawStream)
#else
struct afxBaseDrawStream
#endif
{
    afxError            (*beginCb)(afxDrawStream, afxBool permanent);
    afxError            (*endCb)(afxDrawStream);
    afxError            (*resetCb)(afxDrawStream);
    afxDrawStreamState  state;
    afxNat              submRefCnt;
    afxMask64           submQueMask; // one for each queue where this diob was submitted into.

    afxNat              portIdx;
    afxNat              queIdx;
    afxArena            cmdArena; // owned by dctx data for specific port
    afxBool             disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    afxCmd const*       stdCmds;
};
#endif
#endif

AVX afxDrawStreamState  AfxGetDrawStreamState(afxDrawStream diob);
AVX afxNat              AfxGetDrawStreamPort(afxDrawStream diob);

////////////////////////////////////////////////////////////////////////////////

/// Start recording a command buffer.

AVX afxError            AfxRecordDrawStreams
(
    afxDrawStreamUsage  usage, /// Bitmask specifying usage behavior for command buffer.
    afxNat              cnt,
    afxDrawStream       streams[] /// is the handle of the command buffer which is to be put in the recording state.
);

/// Finish recording a command buffer.
/// The command buffer must have been in the recording state, and, if successful, is moved to the executable state.

/// If there was an error during recording, the application will be notified by an unsuccessful return code returned by AfxEndCommandBuffer, and the command buffer will be moved to the invalid state.

AVX afxError            AfxCompileDrawStreams
(
    afxNat              cnt,
    afxDrawStream       streams[]  /// is the command buffer to complete recording.
);

/// Reset a command buffer to the initial state.
/// Any primary command buffer that is in the recording or executable state and has @diob recorded into it, becomes invalid.

AVX afxError            AfxRecycleDrawStreams
(
    afxBool             freeRes, /// specifies that most or all memory resources currently owned by the command buffer should be returned to the parent command pool. If this flag is not set, then the command buffer may hold onto memory resources and reuse them when recording commands. @diob is moved to the initial state.
    afxNat              cnt,
    afxDrawStream       streams[] /// is the command buffer to reset. The command buffer can be in any state other than pending, and is moved into the initial state.
);

AVX afxError            AfxAcquireDrawStreams(afxDrawContext dctx, afxNat portIdx, afxNat* queIdx, afxNat cnt, afxDrawStream streams[]);

AVX afxError            AfxOpenDrawStreams(afxDrawContext dctx, afxNat portIdx, afxNat queIdx, afxNat cnt, afxDrawStream streams[]);

#endif//AVX_DRAW_STREAM_H
