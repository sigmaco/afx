/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// Command buffers are objects used to record commands which canv be subsequently
// submitted to a device queue for execution. There are two levels of command
// buffers - primary command buffers, which canv execute secondary command
// buffers, and which are submitted to queues, and secondary command buffers,
// which canv be executed by primary command buffers, and which are not directly
// submitted to queues.


#ifndef AFX_DRAW_SCRIPT_H
#define AFX_DRAW_SCRIPT_H

#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/afxDrawOperation.h"
#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/pipelining/afxSampler.h"
#include "afx/draw/res/afxCanvas.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afx/draw/res/afxBuffer.h"

/*
Each command buffer is always in one of the following states:

Initial - When a command buffer is allocated, it is in the initial state. Some commands are able to reset a command buffer (or a set of command buffers) back to this state from any of the executable, recording or invalid state. Command buffers in the initial state canv only be moved to the recording state, or freed.
Recording - vkBeginCommandBuffer changes the state of a command buffer from the initial state to the recording state. Once a command buffer is in the recording state, vkCmd* commands canv be used to record to the command buffer.
Executable - vkEndCommandBuffer ends the recording of a command buffer, and moves it from the recording state to the executable state. Executable command buffers canv be submitted, reset, or recorded to another command buffer.
Pending - Queue submission of a command buffer changes the state of a command buffer from the executable state to the pending state. Whilst in the pending state, applications must not attempt to modify the command buffer in any way - as the device may be processing the commands recorded to it. Once execution of a command buffer completes, the command buffer either reverts back to the executable state, or if it was recorded with VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, it moves to the invalid state. A synchronization command should be used to detect when this occurs.
Invalid - Some operations, such as modifying or deleting a resource that was used in a command recorded to a command buffer, will transition the state of that command buffer into the invalid state. Command buffers in the invalid state canv only be reset or freed.
*/

typedef enum afxDrawScriptState
{
    AFX_DSCR_STATE_INITIAL = 1, // new
    AFX_DSCR_STATE_RECORDING, // open
    AFX_DSCR_STATE_EXECUTABLE, // closed
    AFX_DSCR_STATE_PENDING, // submitted
    AFX_DSCR_STATE_INVALID,
    AFX_DSCR_STATE_INTERNAL_EXECUTING,
} afxDrawScriptState;

typedef enum afxSurfaceStoreOp
{
    // NIL // none
    AFX_SURFACE_STORE_OP_STORE = 1,
    AFX_SURFACE_STORE_OP_DONT_CARE,
} afxSurfaceStoreOp;

typedef enum afxSurfaceLoadOp
{
    AFX_SURFACE_LOAD_OP_LOAD,
    AFX_SURFACE_LOAD_OP_CLEAR,
    AFX_SURFACE_LOAD_OP_DONT_CARE
} afxSurfaceLoadOp;

AFX_DEFINE_UNION(afxClearValue)
{
    union
    {
        afxV4d          color;
        afxInt          colori[4];
        afxNat          colorn[4];
    };
    struct
    {
        afxReal         depth; // default is 1.f
        afxNat          stencil; // default is 0
    };
};

AFX_DEFINE_STRUCT(afxClearRect)
{
    afxRect             rect;
    afxNat32            baseLayer;
    afxNat32            layerCnt;
};

AFX_DEFINE_STRUCT(afxDrawTarget)
{
    afxSurface          surf;
    afxSurfaceLoadOp    loadOp;
    afxSurfaceStoreOp   storeOp;
    afxClearValue       clearValue;
};

AFX_DEFINE_STRUCT(afxRenderPassAnnex)
{
    afxBool             clearOnLoad, ignoreOnStore;
    afxClearValue       clearValue;
};

AFX_DEFINE_HANDLE(afxDrawScript);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxDrawScript)
{
    afxObject           obj;
};

#endif

/*
When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the command buffer state. 
Pipeline object state that is specified as dynamic is not applied to the command buffer state at this time. 
Instead, dynamic state canv be modified at any time and persists for the lifetime of the command buffer, or until modified by another dynamic state setting command, or made invalid by another pipeline bind with that state specified as static.
*/

AFX void*               AfxDrawScriptGetContext(afxDrawScript dscr);
AFX void*               AfxDrawScriptGetDriver(afxDrawScript dscr);
AFX void*               AfxDrawScriptGetDrawSystem(afxDrawScript dscr);

AFX afxDrawScriptState  AfxDrawScriptGetState(afxDrawScript dscr);

AFX afxError            AfxDrawScriptBegin(afxDrawScript dscr, afxNat flags);
AFX afxError            AfxDrawScriptEnd(afxDrawScript dscr);
AFX afxError            AfxDrawScriptReset(afxDrawScript dscr);

AFX void                AfxDrawScriptCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst);
AFX void                AfxDrawScriptCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst);

AFX void                AfxDrawScriptCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AFX void                AfxDrawScriptCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[]);

AFX void                AfxDrawScriptCmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[]);
AFX void                AfxDrawScriptCmdBindVertexBuffers2(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const size[], afxSize const stride[]);
AFX void                AfxDrawScriptCmdBindManagedVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[]);
AFX void                AfxDrawScriptCmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat idxSiz); // offset is the starting offset in bytes within buffer used in index buffer address calculations.
AFX void                AfxDrawScriptCmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer buf, afxNat rgnIdx); // offset is the starting offset in bytes within buffer used in index buffer address calculations.

AFX void                AfxDrawScriptCmdBindPipeline(afxDrawScript dscr, afxPipeline pip);

AFX void                AfxDrawScriptCmdCopyTexture(afxDrawScript dscr, afxTexture dst, afxTexture src, afxNat rgnCnt, afxTextureRegion const rgn[]);

AFX void                AfxDrawScriptCmdSetScissors(afxDrawScript dscr, afxNat first, afxNat cnt, afxRect const rect[]);
AFX void                AfxDrawScriptCmdSetViewports(afxDrawScript dscr, afxNat first, afxNat cnt, afxViewport const vp[]);

AFX void                AfxDrawScriptCmdBeginOperation(afxDrawScript dscr, afxDrawOperation dop, afxNat tecIdx, afxCanvas canv, afxRect const *area, afxNat annexCnt, afxRenderPassAnnex const annexes[]);
AFX void                AfxDrawScriptCmdEndOperation(afxDrawScript dscr);
AFX void                AfxDrawScriptCmdEmployTechnique(afxDrawScript dscr, afxNat tecIdx);
AFX void                AfxDrawScriptCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts);
AFX void                AfxDrawScriptCmdBeginCombination(afxDrawScript dscr, afxRect const *area, afxNat layerCnt, afxNat rasterCnt, afxDrawTarget const rasters[], afxDrawTarget const *depth, afxDrawTarget const *stencil);
AFX void                AfxDrawScriptCmdEndCombination(afxDrawScript dscr);

AFX void                AfxDrawScriptCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state);
AFX void                AfxDrawScriptCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state);
AFX void                AfxDrawScriptCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state);

#endif//AFX_DRAW_SCRIPT_H