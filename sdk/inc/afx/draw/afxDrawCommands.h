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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_COMMANDS_H
#define AFX_DRAW_COMMANDS_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/math/afxViewport.h"
#include "afx/draw/afxRaster.h"

typedef enum afxDrawPrefab
{
    afxDrawPrefab_QUAD,

    afxDrawPrefab_TOTAL
} afxDrawPrefab;

typedef enum afxSurfaceStoreOp
/// An enumerated value indicating the load operation to perform on target surface prior to executing the render pass
{
    /// Stores the resulting value of the render pass for this attachment.
    afxSurfaceStoreOp_STORE,

    /// Discards the resulting value of the render pass for this attachment.
    afxSurfaceStoreOp_DISCARD,

    afxSurfaceStoreOp_DONT_CARE,
} afxSurfaceStoreOp;

typedef enum afxSurfaceLoadOp
/// An enumerated value indicating the store operation to perform on target surface after executing the render pass
{
    /// Loads the clearValue for this attachment into the render pass.
    afxSurfaceLoadOp_CLEAR,

    /// Loads the existing value for this attachment into the render pass.
    afxSurfaceLoadOp_LOAD,

    afxSurfaceLoadOp_DONT_CARE
} afxSurfaceLoadOp;

AFX_DEFINE_STRUCT(afxVertexInputSource)
{
    afxBuffer           buf;
    afxNat32            offset; /// the start of buffer.
    afxNat32            range; /// the size in bytes of vertex data bound from buffer.
    afxNat32            stride; /// the byte stride between consecutive elements within the buffer.
    afxBool8            instance; /// addressing is a function of the instance index, else case, it is of the vertex index.
    afxNat32            instDivisor; /// the number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
};

AFX_DEFINE_STRUCT(afxVertexInputPoint) /// vertex attribute input stream
{
    afxNat8             locationIdx; /// is the shader input location number for this attribute.
    afxNat8             srcSlotIdx; /// is the binding number which this attribute takes its data from.
    afxVertexFormat     fmt; /// is the size and type of the vertex attribute data.
    afxNat32            offset; /// is a byte offset of this attribute relative to the start of an element in the vertex input binding.
    //afxVertexUsage      usage; /// special flags used to opportunistic optimization
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
        afxReal         depth; /// default is 1.f
        afxNat          stencil; /// default is 0
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
    afxRaster          tex; /// the texture subresource that will be output to for this color attachment.
    afxSurfaceLoadOp    loadOp; /// Indicates the load operation to perform on view prior to executing the render pass.
    afxSurfaceStoreOp   storeOp; /// The store operation to perform on view after executing the render pass.
    afxClearValue       clearValue; /// Indicates the value to clear view to prior to executing the render pass.
    afxRaster          resolve; /// the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
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

AFX_DEFINE_STRUCT(afxDrawIndirectCmd)
/// Structure specifying a indirect drawing command
{
    afxNat32            vtxCnt; /// is the number of vertices to draw.
    afxNat32            instCnt; /// is the number of instances to draw.
    afxNat32            firstVtx; /// is the index of the first vertex to draw.
    afxNat32            firstInst; /// is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(afxDrawIndexedIndirectCmd)
/// Structure specifying a indexed indirect drawing command
{
    afxNat32            idxCnt; /// is the number of vertices to draw.
    afxNat32            instCnt; /// is the number of instances to draw.
    afxNat32            firstIdx;/// is the base index within the index buffer.
    afxInt32            vtxOff; /// is the value added to the vertex index before indexing into the vertex buffer.
    afxNat32            firstInst; /// is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(afxCmd)
{
    void*EndOfScript;

    void(*BindPipeline)(afxDrawScript dscr, afxNat level, afxPipeline pip);
    void(*BindBuffers)(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
    void(*BindTextures)(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[]);

    // fixed-function vertex processing

    void(*BindVertexSources)(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[]);
    void(*ResetVertexStreams)(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[]);
    void(*ResetVertexAttributes)(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[]);
    void(*BindIndexSource)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz);
    void(*SetPrimitiveTopology)(afxDrawScript dscr, afxPrimTopology topology);

    // fixed-function vertex post-processing

    void(*ResetViewports)(afxDrawScript dscr, afxNat cnt, afxViewport const vp[]);
    void(*ReadjustViewports)(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxViewport const vp[]);

    // rasterization

    void(*DisableRasterization)(afxDrawScript dscr, afxBool disable);

    void(*SwitchFrontFace)(afxDrawScript dscr, afxBool cw);
    void(*SetCullMode)(afxDrawScript dscr, afxCullMode mode);

    void(*EnableDepthBias)(afxDrawScript dscr, afxBool enable);
    void(*SetDepthBias)(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor);

    void(*SetLineWidth)(afxDrawScript dscr, afxReal lineWidth);

    // fragment operations

    void(*ResetScissors)(afxDrawScript dscr, afxNat cnt, afxRect const rc[]);
    void(*ReadjustScissors)(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    void(*ResetAreas)(afxDrawScript dscr, afxBool exclusive, afxNat cnt, afxRect const rc[]);
    void(*ReadjustAreas)(afxDrawScript dscr, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[]);

    void(*EnableDepthBoundsTest)(afxDrawScript dscr, afxBool enable);
    void(*SetDepthBounds)(afxDrawScript dscr, afxReal const bounds[2]);

    void(*EnableStencilTest)(afxDrawScript dscr, afxBool enable);
    void(*SetStencilCompareMask)(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask);
    void(*SetStencilWriteMask)(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask);
    void(*SetStencilReference)(afxDrawScript dscr, afxMask faceMask, afxNat32 reference);

    void(*EnableDepthTest)(afxDrawScript dscr, afxBool enable);
    void(*SetDepthCompareOp)(afxDrawScript dscr, afxCompareOp op);
    void(*EnableDepthWrite)(afxDrawScript dscr, afxBool enable);

    // framebuffer operations

    void(*SetBlendConstants)(afxDrawScript dscr, afxReal const blendConstants[4]);

    // canvas

    void(*BeginCanvas)(afxDrawScript dscr, afxCanvasConfig const*state);
    void(*EndCanvas)(afxDrawScript dscr);
    void(*NextPass)(afxDrawScript dscr, afxBool useAuxScripts);

    void(*ExecuteCommands)(afxDrawScript dscr, afxNat cnt, afxDrawScript aux[]);

    // draw

    void(*Draw)(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat baseVtxIdx, afxNat baseInstIdx);
    void(*DrawIndirect)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    void(*DrawIndirectCount)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    void(*DrawIndexed)(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat baseIdx, afxNat vtxOff, afxNat baseInstIdx);
    void(*DrawIndexedIndirect)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
    void(*DrawIndexedIndirectCount)(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
    
    void* Total;
};

////////////////////////////////////////////////////////////////////////////////
// COMMANDS                                                                   //
////////////////////////////////////////////////////////////////////////////////

/// Bind a pipeline object to a command buffer.
/// Once bound, a pipeline binding affects subsequent commands that interact with the given pipeline type in the command buffer until a different pipeline of the same type is bound to the bind point, or until the pipeline bind point is disturbed by binding a shader object.
/// Commands that do not interact with the given pipeline type must not be affected by the pipeline state.

AFX void                AfxCmdBindPipeline
(
    afxDrawScript       dscr, /// is the command buffer that the pipeline will be bound to. 
    afxNat              level, /// is a value specifying to which level the pipeline is bound. Binding one does not disturb the others.
    afxPipeline         pip /// is the pipeline to be bound.
);


AFX void                AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
AFX void                AfxCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat baseIdx, afxNat cnt, afxSampler smp[], afxRaster tex[]);

/// Set the viewport count and viewports dynamically for a command buffer.
/// This command sets the viewport count and viewports state for subsequent drawing commands when pipeline is created without viewport set.

AFX void                AfxCmdResetViewports
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxNat              cnt, /// specifies the viewport count.
    afxViewport const   vp[] /// specifies the viewports to use for drawing.
);

/// Set the viewport dynamically for a command buffer.
/// This command sets the viewport transformation parameters state for subsequent drawing commands when the graphics pipeline is created without viewport set.

/// The viewport parameters taken from element #i of @vp replace the current state for the viewport index @baseIdx + #i, for #i in[0, @cnt).

AFX void                AfxCmdReadjustViewports
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxNat              baseIdx, /// is the index of the first viewport whose parameters are updated by the command.
    afxNat              cnt, /// is the number of viewports whose parameters are updated by the command.
    afxViewport const   vp[] /// is a pointer to an array of afxViewport structures specifying viewport parameters.
);

/// Set the scissor count and scissor rectangular bounds dynamically for a command buffer.
/// This command sets the scissor count and scissor rectangular bounds state for subsequent drawing commands when the graphics pipeline is created without scissor set.

AFX void                AfxCmdResetScissors
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxNat              cnt, /// specifies the scissor count.
    afxRect const       rc[] /// specifies the scissors to use for drawing.
);

/// Set scissor rectangles dynamically for a command buffer.
/// This command sets the scissor rectangles for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without scissor set.

/// The scissor rectangles taken from element #i of @rect replace the current state for the scissor index @baseIdx + #i, for #i in [0, @cnt).

AFX void                AfxCmdReadjustScissors
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxNat              baseIdx, /// is the index of the first scissor whose state is updated by the command.
    afxNat              cnt, /// is the number of scissors whose rectangles are updated by the command.
    afxRect const       rect[] /// is a pointer to an array of afxRect structures defining scissor rectangles.
);

/// Bind vertex buffers to a command buffer and dynamically set strides.
/// The values taken from elements #i of @buf and @offset replace the current state for the vertex input binding @baseIdx + #i, for #i in [0, @cnt].
/// The vertex input binding is updated to start at the offset indicated by @offset[#i] from the start of the buffer @buf[#i].
/// If @range is not NIL then @range[#i] specifies the bound size of the vertex buffer starting from the corresponding elements of @buf[#i] plus @offset[#i].
/// If @range[#i] is WHOLE_SIZE then the bound size is from @buf[#i] plus @offset[#i] to the end of the buffer @buf[#i].
/// All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
/// If the null descriptor feature is enabled, elements of @buf can be NIL, and can be used by the vertex shader.
/// If a vertex input attribute is bound to a vertex input binding that is NIL, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with (0,0,1).

/// This command also dynamically sets the byte strides between consecutive elements within buffer @buf[#i] to the corresponding @stride[#i] value when drawing using shader objects, or when the graphics pipeline is created without vertex input binding stride set.

AFX void                AfxCmdBindVertexSources
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxNat              baseIdx, /// is the index of the first vertex input binding whose state is updated by the command.
    afxNat              cnt, /// is the number of vertex input bindings whose state is updated by the command.
    afxBuffer           buf[], /// is an array of buffer handles.
    afxNat32 const      offset[], /// the start of buffer.
    afxNat32 const      range[] /// the size in bytes of vertex data bound from buffer.
    //afxNat32 const      stride[] /// the byte stride between consecutive elements within the buffer.
);

/// Bind an index buffer to a command buffer.

AFX void                AfxCmdBindIndexSource
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxBuffer           buf, /// is the buffer being bound.
    afxNat32            offset, /// is the starting offset in bytes within buffer used in index buffer address calculations.
    afxNat32            range, /// is the size in bytes of index data bound from buffer.
    afxNat32            idxSiz /// is a value specifying the size of the indices.
);

/// Set primitive topology state dynamically for a command buffer.
/// This command sets the primitive topology for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without primitive topology set.

AFX void                AfxCmdSetPrimitiveTopology
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxPrimTopology     topology /// specifies the primitive topology to use for drawing.
);

/// Begin a dynamic render canvas instance
/// After beginning a render canvas instance, the command buffer is ready to record draw commands.

/// If flags includes resuming then this render pass is resumed from a render canvas instance that has been suspended earlier in submission order.

AFX void                AfxCmdBeginCanvas
(
    afxDrawScript       dscr, /// is the command buffer in which to record the command.
    afxCanvasConfig const*state /// is a pointer to a afxCanvasConfig structure specifying details of the render pass instance to begin.
);

/// End a dynamic render canvas instance.
/// If the value of flags used to begin this render pass instance included suspending, then this render canvas is suspended and will be resumed later in submission order.

AFX void                AfxCmdEndCanvas
(
    afxDrawScript       dscr /// is the command buffer in which to record the command.
);

/// Transition to the next pass of a render canvas.
/// The pass index for a render canvas begins at zero when AfxCmdBeginCanvas is recorded, and increments each time AfxCmdNextPass is recorded.

/// After transitioning to the next pass, the application can record the commands for that pass.

AFX void                AfxCmdNextPass
(
    afxDrawScript       dscr, /// is the command buffer in which to record the command.
    afxBool             useAuxScripts /// specifies how the commands in the next subpass will be provided, in the same fashion as the corresponding parameter of vkCmdBeginRenderPass.
);

/// Execute a secondary command buffer from a primary command buffer.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary command buffer which is currently in the executable or recording state, that primary command buffer becomes invalid.

/// If the nested command buffer feature is enabled it is valid usage for AfxCmdExecuteCommands to also be recorded to a secondary command buffer.

AFX void                AfxCmdExecuteCommands
(
    afxDrawScript       dscr, /// is a handle to a primary command buffer that the secondary command buffers are executed in.
    afxNat              cnt, /// is the length of the @aux array.
    afxDrawScript       aux[] /// is a pointer to an array of @cnt secondary command buffer handles, which are recorded to execute in the primary command buffer in the order they are listed in the array.
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AFX void                AfxCmdDraw
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxNat              vtxCnt, /// is the number of vertices to draw.
    afxNat              instCnt, /// is the number of instances to draw.
    afxNat              baseVtxIdx, /// is the index of the first vertex to draw.
    afxNat              baseInstIdx /// is the instance ID of the first instance to draw.
);

/// Draw primitives with indirect parameters.
/// AfxCmdDrawIndirect behaves similarly to AfxCmdDraw except that the parameters are read by the device from a buffer during execution.
/// @drawCount draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndirectCmd structures.
/// If @drawCnt is less than or equal to one, @stride is ignored.

AFX void                AfxCmdDrawIndirect
(
    afxDrawScript       dscr, ///  is the command buffer into which the command is recorded.
    afxBuffer           buf, /// is the buffer containing draw parameters.
    afxNat32            offset, /// is the byte offset into @buf where parameters begin.
    afxNat32            drawCnt, /// is the number of draws to execute, and can be zero.
    afxNat32            stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indirect parameters and draw count.
/// AfxCmdDrawIndirectCount behaves similarly to AfxCmdDrawIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AFX void                AfxCmdDrawIndirectCount
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxBuffer           buf, /// is the buffer containing draw parameters.
    afxNat32            offset, /// is the byte offset into buffer where parameters begin.
    afxBuffer           cntBuf, /// is the buffer containing the draw count.
    afxNat32            cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32            maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32            stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw primitives with indexed vertices. 
/// When the command is executed, primitives are assembled using the current primitive topology and @idxCnt vertices whose indices are retrieved from the index buffer.
/// The index buffer is treated as an array of tightly packed unsigned integers of size defined by the @idxSiz parameter (of AfxCmdBindIndexSource) with which the buffer was bound.

/// The first vertex index is at an offset of @firstIdx × @idxSiz + @offset within the bound index buffer, where @offset is the offset specified by AfxCmdBindIndexSource, and @idxSize is the byte size of the type specified by @idxSiz.
/// Subsequent index values are retrieved from consecutive locations in the index buffer.
/// Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the @idxSiz is 1 or 2) and have @vtxOff added to them, before being supplied as the @vtxIdx value.

/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AFX void                AfxCmdDrawIndexed
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxNat              idxCnt, /// is the number of vertices to draw.
    afxNat              instCnt, /// is the number of instances to draw.
    afxNat              baseIdx, /// is the base index within the index buffer.
    afxNat              vtxOff, /// is the value added to the vertex index before indexing into the vertex buffer.
    afxNat              baseInstIdx /// is the instance ID of the first instance to draw.
);

/// Draw primitives with indirect parameters and indexed vertices.
/// AfxCmdDrawIndexedIndirect behaves similarly to AfxCmdDrawIndexed except that the parameters are read by the device from a buffer during execution.
/// @drawCnt draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndexedIndirectCmd structures. If @drawCnt is less than or equal to one, @stride is ignored.

AFX void                AfxCmdDrawIndexedIndirect
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxBuffer           buf, /// is the buffer containing draw parameters.
    afxNat32            offset, /// is the byte offset into buffer where parameters begin.
    afxNat32            drawCnt, /// is the number of draws to execute, and can be zero.
    afxNat32            stride /// is the byte stride between successive sets of draw parameters.
);

/// Draw parameters with indirect parameters, indexed vertices, and draw count.
/// AfxCmdDrawIndexedIndirectCount behaves similarly to AfxCmdDrawIndexedIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AFX void                AfxCmdDrawIndexedIndirectCount
(
    afxDrawScript       dscr, /// is the command buffer into which the command is recorded.
    afxBuffer           buf, /// is the buffer containing draw parameters.
    afxNat32            offset, /// is the byte offset into @buf where parameters begin.
    afxBuffer           cntBuf, /// is the buffer containing the draw count.
    afxNat32            cntBufOff, /// is the byte offset into @cntBuf where the draw count begins.
    afxNat32            maxDrawCnt, /// specifies the maximum number of draws that will be executed. The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxNat32            stride /// is the byte stride between successive sets of draw parameters.
);

AFX void                AfxCmdDisableRasterization
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             disable /// controls whether primitives are discarded immediately before the rasterization stage.
);

AFX void                AfxCmdSwitchFrontFace
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             cw /// is a value specifying the front-facing triangle orientation to be used for culling.
);

AFX void                AfxCmdSetCullMode
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxCullMode         mode /// specifies the cull mode property to use for drawing.
);

AFX void                AfxCmdEnableDepthBias
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             enable /// controls whether to bias fragment depth values.
);

AFX void                AfxCmdSetDepthBias
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxReal             constFactor, /// is a scalar factor controlling the constant depth value added to each fragment.
    afxReal             clamp, /// is the maximum (or minimum) depth bias of a fragment.
    afxReal             slopeFactor /// is a scalar factor applied to a fragment’s slope in depth bias calculations.
);

AFX void                AfxCmdSetLineWidth
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxReal             lineWidth /// is the width of rasterized line segments.
);

AFX void                AfxCmdEnableDepthTest
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             enable /// specifies if the depth test is enabled.
);

AFX void                AfxCmdSetDepthCompareOp
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxCompareOp        op /// is a afxCompareOp value specifying the comparison operator used for the Depth Comparison step of the depth test.
);

AFX void                AfxCmdEnableDepthWrite
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             enable /// specifies if depth writes are enabled.
);

AFX void                AfxCmdEnableStencilTest
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             enable /// specifies if the stencil test is enabled.
);

AFX void                AfxCmdSetStencilCompareMask
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxMask          faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the compare mask.
    afxNat32            compareMask /// is the new value to use as the stencil compare mask.
);

AFX void                AfxCmdSetStencilWriteMask
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxMask          faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to update the write mask, as described above for vkCmdSetStencilCompareMask.
    afxNat32            writeMask /// is the new value to use as the stencil write mask.
);

AFX void                AfxCmdSetStencilReference
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxMask          faceMask, /// is a bitmask of (0/FRONT, 1/BACK, 2/BOTH) bits specifying the set of stencil state for which to
    afxNat32            reference /// is the new value to use as the stencil reference value.
);

AFX void                AfxCmdEnableDepthBoundsTest
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxBool             enable /// specifies if the depth bounds test is enabled.
);

AFX void                AfxCmdSetDepthBounds
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxReal const       bounds[2] /// is the minimum and maximum depth bounds.
);

AFX void                AfxCmdSetBlendConstants
(
    afxDrawScript       dscr, /// is the command buffer into which the command will be recorded.
    afxReal const       blendConstants[4] /// is an array of four values specifying the Rc, Gc, Bc, and Ac components of the blend constant color used in blending, depending on the blend factor.
);













//                      TODO Pipeline Gateway
AFX void                AfxCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[]);
AFX void                AfxCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[]);

AFX void                AfxCmdCopyTexture(afxDrawScript dscr, afxRaster dst, afxRaster src, afxNat rgnCnt, afxRasterRegion const rgn[]);

#endif//AFX_DRAW_COMMANDS_H
