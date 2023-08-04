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
 *                               (c) 2017 Federação SIGMA
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
#include "afx/draw/pipelining/afxDrawOperation.h"
#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/res/afxSampler.h"
#include "afx/draw/pipelining/afxCanvas.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afx/draw/res/afxBuffer.h"
#include "afx/core/mem/afxArena.h"

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
    AFX_DTS_STORE_OP_STORE,
    AFX_DTS_STORE_OP_DISCARD,
    AFX_DTS_STORE_OP_DONT_CARE,
} afxSurfaceStoreOp;

typedef enum afxSurfaceLoadOp
{
    AFX_DTS_LOAD_OP_CLEAR,
    AFX_DTS_LOAD_OP_LOAD,
    AFX_DTS_LOAD_OP_DONT_CARE
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
    afxSurface          surf; // the texture subresource that will be output to for this color attachment.
    afxSurface          resolve; // the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxSurfaceLoadOp    loadOp; // Indicates the load operation to perform on view prior to executing the render pass.
    afxSurfaceStoreOp   storeOp; // The store operation to perform on view after executing the render pass.
    afxClearValue       clearValue; // Indicates the value to clear view to prior to executing the render pass.
};

AFX_DEFINE_STRUCT(afxDrawPassState)
{
    afxCanvas           canv;
    afxRect             area;
    afxNat              layerCnt;
    afxNat              rasterCnt;
    afxDrawTarget const*rasters;
    afxDrawTarget const*depth;
    afxDrawTarget const*stencil;
};

AFX_DECLARE_STRUCT(_afxDscrVmt);
AFX_DECLARE_STRUCT(_afxDcmdVmt);

AFX_OBJECT(afxDrawScript)
{
    afxObject           obj;
    _afxDscrVmt const*  vmt;
    void*               idd;
#ifdef _AFX_DRAW_SCRIPT_C
    _afxDcmdVmt const*  cmd;
    afxDrawScriptState  state;
    afxNat              submRefCnt;

    afxNat              portIdx;
    afxArena            cmdArena; // owned by dctx data for specific port
    afxBool             disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    afxDrawInput        din;

    // move to instance data, together with command chain
    afxChain            drawCalls;
    afxChain            stateChanges;
    afxChain            pipBinds;
    afxChain            vbufBinds;
    afxChain            ibufBinds;
    afxChain            bufBinds;
    afxChain            texBinds;
    
    afxNat              totalCmdCnt;
    afxNat              lastDrawCmdIdx;
    afxNat              drawCmdCnt;
    afxNat              lastPipBindCmdIdx;
    afxNat              pipBindCmdCnt;
    afxNat              lastVbufBindCmdIdx;
    afxNat              vbufBindCmdCnt;
    afxNat              lastIbufBindCmdIdx;
    afxNat              ibufBindCmdCnt;
    afxNat              lastBufBindCmdIdx;
    afxNat              bufBindCmdCnt;
    afxNat              lastTexBindCmdIdx;
    afxNat              texBindCmdCnt;
    afxNat              lastRasUpdCmdIdx;
    afxNat              rasUpdCmdCnt;
    afxNat              lastDepthUpdCmdIdx;
    afxNat              depthUpdCmdCnt;
    afxNat              lastVpCmdIdx; // viewport and/or scissor changes
    afxNat              vpUpdCmdCnt;
    afxNat              lastScisCmdIdx; // viewport and/or scissor changes
    afxNat              scisUpdCmdCnt;
#endif
};

/*
When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the command buffer state. 
Pipeline object state that is specified as dynamic is not applied to the command buffer state at this time. 
Instead, dynamic state canv be modified at any time and persists for the lifetime of the command buffer, or until modified by another dynamic state setting command, or made invalid by another pipeline bind with that state specified as static.
*/

AFX afxDrawDriver       AfxGetDrawScriptDriver(afxDrawScript dscr);
AFX afxDrawInput        AfxGetDrawScriptInput(afxDrawScript dscr);
AFX afxDrawScriptState  AfxGetDrawScriptState(afxDrawScript dscr);
AFX afxNat              AfxGetDrawScriptPort(afxDrawScript dscr);

AFX afxError            AfxBeginDrawScript(afxDrawScript dscr, afxBool permanent);
AFX afxError            AfxEndDrawScript(afxDrawScript dscr);
AFX afxError            AfxResetDrawScript(afxDrawScript dscr);

AFX void                AfxDrawCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst);
AFX void                AfxDrawCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst);

AFX void                AfxDrawCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AFX void                AfxDrawCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[]);

AFX void                AfxDrawCmdBindVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[]);
AFX void                AfxDrawCmdBindVertexBuffers2(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const stride[]);
AFX void                AfxDrawCmdBindManagedVertexBuffers(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[]);
AFX void                AfxDrawCmdBindIndexBuffer(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat idxSiz); // offset is the starting offset in bytes within buffer used in index buffer address calculations.
AFX void                AfxDrawCmdBindManagedIndexBuffer(afxDrawScript dscr, afxIndexBuffer buf, afxNat rgnIdx); // offset is the starting offset in bytes within buffer used in index buffer address calculations.

AFX void                AfxDrawCmdBindPipeline(afxDrawScript dscr, afxPipeline pip);

AFX void                AfxDrawCmdCopyTexture(afxDrawScript dscr, afxTexture dst, afxTexture src, afxNat rgnCnt, afxTextureRegion const rgn[]);

AFX void                AfxDrawCmdSetScissors(afxDrawScript dscr, afxNat first, afxNat cnt, afxRect const rect[]);
AFX void                AfxDrawCmdSetViewports(afxDrawScript dscr, afxNat first, afxNat cnt, afxViewport const vp[]);

AFX void                AfxDrawCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts);

AFX void                AfxDrawCmdBeginDrawPass(afxDrawScript dscr, afxDrawPassState const *state);
AFX void                AfxDrawCmdEndDrawPass(afxDrawScript dscr);

AFX void                AfxDrawCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state);
AFX void                AfxDrawCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state);
AFX void                AfxDrawCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state);

#endif//AFX_DRAW_SCRIPT_H