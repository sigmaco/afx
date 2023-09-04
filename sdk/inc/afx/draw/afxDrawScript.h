/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
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
#include "afx/draw/pipelining/afxPipelineRig.h"
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
    afxDrawScriptState_INITIAL = 1, // new
    afxDrawScriptState_RECORDING, // open
    afxDrawScriptState_EXECUTABLE, // closed
    afxDrawScriptState_PENDING, // submitted
    afxDrawScriptState_INVALID,
    afxDrawScriptState_INTERNAL_EXECUTING,
} afxDrawScriptState;

typedef enum afxSurfaceStoreOp // An enumerated value indicating the load operation to perform on target surface prior to executing the render pass
{
    afxSurfaceStoreOp_STORE, // Stores the resulting value of the render pass for this attachment.
    afxSurfaceStoreOp_DISCARD, // Discards the resulting value of the render pass for this attachment.
    afxSurfaceStoreOp_DONT_CARE,
} afxSurfaceStoreOp;

typedef enum afxSurfaceLoadOp // An enumerated value indicating the store operation to perform on target surface after executing the render pass
{
    afxSurfaceLoadOp_CLEAR, // Loads the clearValue for this attachment into the render pass.
    afxSurfaceLoadOp_LOAD, // Loads the existing value for this attachment into the render pass.
    afxSurfaceLoadOp_DONT_CARE
} afxSurfaceLoadOp;

AFX_DEFINE_STRUCT(afxVertexInputStream)
{
    afxBuffer           buf;
    afxNat32            offset; // the start of buffer.
    afxNat32            range; // the size in bytes of vertex data bound from buffer.
    afxNat32            stride; // the byte stride between consecutive elements within the buffer.
    afxBool8            instance; // addressing is a function of the instance index, else case, it is of the vertex index.
    afxNat32            instDivisor; // the number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
};

AFX_DEFINE_STRUCT(afxVertexInputPoint) // vertex attribute input stream
{
    afxNat8             location; // is the shader input location number for this attribute.
    afxNat8             stream; // is the binding number which this attribute takes its data from.
    afxVertexFormat     fmt; // is the size and type of the vertex attribute data.
    afxNat32            offset; // is a byte offset of this attribute relative to the start of an element in the vertex input binding.
    afxVertexUsage      usage; // special flags used to optimization
};

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
    afxTexture          tex; // the texture subresource that will be output to for this color attachment.
    afxTexture          resolve; // the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxSurfaceLoadOp    loadOp; // Indicates the load operation to perform on view prior to executing the render pass.
    afxSurfaceStoreOp   storeOp; // The store operation to perform on view after executing the render pass.
    afxClearValue       clearValue; // Indicates the value to clear view to prior to executing the render pass.
};

AFX_DEFINE_STRUCT(afxCanvasConfig)
{
    afxRect             area;
    afxNat              layerCnt;
    afxNat              rasterCnt;
    afxDrawTarget const*rasters;
    afxDrawTarget const*depth;
    afxDrawTarget const*stencil;
};

AFX_DECLARE_STRUCT(_afxDscrVmt);
AFX_DECLARE_STRUCT(_afxDcmdVmt);

#ifdef _AFX_DRAW_SCRIPT_C
AFX_OBJECT(afxDrawScript)
#else
struct afxBaseDrawScript
#endif
{
    _afxDscrVmt const*  vmt;
    _afxDcmdVmt const*  cmd;
    afxDrawScriptState  state;
    afxNat              submRefCnt;

    afxNat              portIdx;
    afxArena            cmdArena; // owned by dctx data for specific port
    afxBool             disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    afxDrawInput        din;
};

/*
When a pipeline object is bound, any pipeline object state that is not specified as dynamic is applied to the command buffer state. 
Pipeline object state that is specified as dynamic is not applied to the command buffer state at this time. 
Instead, dynamic state canv be modified at any time and persists for the lifetime of the command buffer, or until modified by another dynamic state setting command, or made invalid by another pipeline bind with that state specified as static.
*/

AFX afxDrawInput        AfxGetDrawScriptInput(afxDrawScript dscr);
AFX afxDrawScriptState  AfxGetDrawScriptState(afxDrawScript dscr);
AFX afxNat              AfxGetDrawScriptPort(afxDrawScript dscr);

AFX afxError            AfxBeginDrawScript(afxDrawScript dscr, afxBool permanent);
AFX afxError            AfxEndDrawScript(afxDrawScript dscr);
AFX afxError            AfxResetDrawScript(afxDrawScript dscr);

AFX void                AfxCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst);
AFX void                AfxCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst);

AFX void                AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AFX void                AfxCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[]);

AFX void                AfxCmdSetVertexInputLayout(afxDrawScript dscr, afxNat cnt, afxVertexInputPoint const spec[]);
AFX void                AfxCmdBindVertexStreams(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexInputStream const spec[]);
AFX void                AfxCmdBindManagedVertexStreams(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[], afxBool inst, afxNat divisor);
AFX void                AfxCmdBindIndexStream(afxDrawScript dscr, afxBuffer buf, afxNat offset, afxNat idxSiz); // offset is the starting offset in bytes within buffer used in index buffer address calculations.
AFX void                AfxCmdBindManagedIndexStream(afxDrawScript dscr, afxIndexBuffer buf, afxNat rgnIdx); // offset is the starting offset in bytes within buffer used in index buffer address calculations.

AFX void                AfxCmdBindPipeline(afxDrawScript dscr, afxPipeline pip);

AFX void                AfxCmdCopyTexture(afxDrawScript dscr, afxTexture dst, afxTexture src, afxNat rgnCnt, afxTextureRegion const rgn[]);

AFX void                AfxCmdSetScissors(afxDrawScript dscr, afxNat first, afxNat cnt, afxRect const rect[]);
AFX void                AfxCmdSetViewports(afxDrawScript dscr, afxNat first, afxNat cnt, afxViewport const vp[]);

AFX void                AfxCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts);

AFX void                AfxCmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state);
AFX void                AfxCmdEndCanvas(afxDrawScript dscr);

AFX void                AfxCmdSetRasterizerState(afxDrawScript dscr, afxPipelineRasterizerState const *state);
AFX void                AfxCmdSetDepthState(afxDrawScript dscr, afxPipelineDepthState const *state);
AFX void                AfxCmdSetInputAssemblyState(afxDrawScript dscr, afxPipelineInputAssemblyState const *state);

AFX void                AfxCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript subsets[]);

#endif//AFX_DRAW_SCRIPT_H