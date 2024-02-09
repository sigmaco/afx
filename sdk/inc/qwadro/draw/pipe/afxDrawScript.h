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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA GL/2.

// Command buffers are objects used to record commands which canv be subsequently
// submitted to a device queue for execution. There are two levels of command
// buffers - primary command buffers, which canv execute secondary command
// buffers, and which are submitted to queues, and secondary command buffers,
// which canv be executed by primary command buffers, and which are not directly
// submitted to queues.

/*
When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the command buffer state.
Pipeline object state that is specified as dynamic is not applied to the command buffer state at this time.
Instead, dynamic state canv be modified at any time and persists for the lifetime of the command buffer, or until modified by another dynamic state setting command, or made invalid by another pipeline bind with that state specified as static.
*/

#ifndef AFX_DRAW_SCRIPT_H
#define AFX_DRAW_SCRIPT_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum afxDrawScriptUsage
{
    /// specifies that each recording of the command buffer will only be submitted once, 
    /// and the command buffer will be reset and recorded again between each submission.
    afxDrawScriptUsage_ONCE,

    /// specifies that a secondary command buffer is considered to be entirely inside a render canvas.
    /// If this is a primary command buffer, then this bit is ignored.
    afxDrawScriptUsage_RESUME,

    /// specifies that a command buffer can be resubmitted to any queue of the same queue family while it is in the pending state, 
    /// and recorded into multiple primary command buffers.
    afxDrawScriptUsage_SIMULTANEOUS,
} afxDrawScriptUsage;

typedef enum afxDrawScriptState
/// Each command buffer is always in one of the following states
{
    /// When a command buffer is allocated, it is in the initial state.
    /// Some commands are able to reset a command buffer (or a set of command buffers) back to this state from any of the executable, recording or invalid state.
    /// Command buffers in the initial state canv only be moved to the recording state, or freed.
    afxDrawScriptState_INITIAL = 1,

    /// BeginDrawScript() changes the state of a command buffer from the initial state to the recording state.
    /// Once a command buffer is in the recording state, AfxCmd* commands canv be used to record to the command buffer.
    afxDrawScriptState_RECORDING,

    /// AfxCompileDrawScript() ends the recording of a command buffer, and moves it from the recording state to the executable state.
    /// Executable command buffers canv be submitted, reset, or recorded to another command buffer.
    afxDrawScriptState_EXECUTABLE,

    /// Queue submission of a command buffer changes the state of a command buffer from the executable state to the pending state.
    /// Whilst in the pending state, applications must not attempt to modify the command buffer in any way - as the device may be processing the commands recorded to it.
    /// Once execution of a command buffer completes, the command buffer either reverts back to the executable state, or if it was recorded with ONCE flag, it moves to the invalid state.
    /// A synchronization command should be used to detect when this occurs.
    afxDrawScriptState_PENDING,

    /// Some operations, such as modifying or deleting a resource that was used in a command recorded to a command buffer, will transition the state of that command buffer into the invalid state.
    /// Command buffers in the invalid state canv only be reset or freed.
    afxDrawScriptState_INVALID,

    afxDrawScriptState_INTERNAL_EXECUTING,
} afxDrawScriptState;

AFX_DECLARE_STRUCT(_afxDcmdVmt);

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_SCRIPT_C
#ifndef _AFX_DRAW_SCRIPT_IMPL
AFX_OBJECT(afxDrawScript)
#else
struct afxBaseDrawScript
#endif
{
    afxError            (*beginCb)(afxDrawScript, afxBool permanent);
    afxError            (*endCb)(afxDrawScript);
    afxError            (*resetCb)(afxDrawScript);
    afxDrawScriptState  state;
    afxNat              submRefCnt;

    afxNat              portIdx;
    afxArena            cmdArena; // owned by dctx data for specific port
    afxBool             disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    afxDrawInput        din;
    afxCmd const*       stdCmds;
};
#endif
#endif

AFX afxError            AfxAcquireDrawScripts(afxDrawInput din, afxNat portIdx, afxNat cnt, afxDrawScript dscr[]);

AFX afxDrawInput        AfxGetDrawScriptInput(afxDrawScript dscr);
AFX afxDrawScriptState  AfxGetDrawScriptState(afxDrawScript dscr);
AFX afxNat              AfxGetDrawScriptPort(afxDrawScript dscr);

/// Start recording a command buffer.

AFX afxError            AfxRecordDrawScript
(
    afxDrawScript       dscr, /// is the handle of the command buffer which is to be put in the recording state.
    afxDrawScriptUsage  usage /// Bitmask specifying usage behavior for command buffer.
);

/// Finish recording a command buffer.
/// The command buffer must have been in the recording state, and, if successful, is moved to the executable state.

/// If there was an error during recording, the application will be notified by an unsuccessful return code returned by AfxEndCommandBuffer, and the command buffer will be moved to the invalid state.

AFX afxError            AfxCompileDrawScript
(
    afxDrawScript       dscr  /// is the command buffer to complete recording.
);

/// Reset a command buffer to the initial state.
/// Any primary command buffer that is in the recording or executable state and has @dscr recorded into it, becomes invalid.

AFX afxError            AfxRecycleDrawScript
(
    afxDrawScript       dscr, /// is the command buffer to reset. The command buffer can be in any state other than pending, and is moved into the initial state.
    afxBool             freeRes /// specifies that most or all memory resources currently owned by the command buffer should be returned to the parent command pool. If this flag is not set, then the command buffer may hold onto memory resources and reuse them when recording commands. @dscr is moved to the initial state.
);

#endif//AFX_DRAW_SCRIPT_H
