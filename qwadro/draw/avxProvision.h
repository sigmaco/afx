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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_PROVISION_H
#define AVX_PROVISION_H

#include "qwadro/draw/avxDrawing.h"

/*
    AvxCmdCommenceDebugScope(), AvxCmdConcludeDebugScope(), and AvxCmdMarkDebugMilestone().

    These functions let you annotate a draw context (afxDrawContext) with debug scopes and milestones.
    Tools like GPU debuggers, frame analyzers, profilers, or custom logging systems can use these annotations to show:
     - hierarchical regions of GPU work
     - named events or markers
     - color-coded debug ranges

    They do not affect rendering but make GPU command streams easier to inspect.
*/

/*
    The AvxCmdCommenceDebugScope() method starts (opens) a debug scope within the draw context.
    It marks the beginning of a labeled section of GPU work.
    All commands recorded after this call will be considered part of this debug region until AvxCmdConcludeDebugScope is called.
*/

AVX afxCmdId AvxCmdCommenceDebugScope
(
    // The draw context being annotated.
    afxDrawContext dctx,
    // A string label for the region (e.g., "Shadow Pass").
    afxString const*name,
    // Optional color to visually represent this region in debugging tools.
    avxColor const color
);

/*
    The AvxCmdConcludeDebugScope() ends (closes) the most recently opened debug scope.
    It completes the region started by AvxCmdCommenceDebugScope.
    Scopes must be properly nested and balanced.

*/

AVX afxCmdId AvxCmdConcludeDebugScope
(
    // The draw context being annotated.
    afxDrawContext dctx
);

/*
    The AvxCmdMarkDebugMilestone() method inserts a single debug marker at the current point in the command stream.
    It does not create a region. It represents an instantaneous "event" or "checkpoint" in the GPU command flow.
    Shows up as a point marker in profilers or debugging tools.
*/

AVX afxCmdId AvxCmdMarkDebugMilestone
(
    // The draw context being annotated.
    afxDrawContext dctx,
    // A string that describes what the milestone represents (e.g., "Upload Complete").
    afxString const*name,
    // Optional color for the marker.
    avxColor const color
);

/*
    The AvxCmdExecuteCommands() method records a command into a primary draw context (dctx) that causes a list of secondary draw contexts (aux[]) to be executed when the primary context is submitted.
    This is essentially the same concept as secondary command buffers in Vulkan or D3D12 bundles.

    Primary draw context is the main command stream you submit to the GPU.
    Secondary draw context is composed of pre-recorded command sequences which can be reused or inserted into primaries.
    AvxCmdExecuteCommands() inserts these secondary sequences into the primary command flow.

    A secondary that does not have the "simultaneous use" capability can only be used by one primary at a time.
    If you try to use it in multiple primaries at once, the engine invalidates the others to prevent undefined behavior.
    This is nearly identical to Vulkan's VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT.
    Why this exists? A non-simultaneous-use secondary may contain internal state that cannot be safely reused while 
    it is already queued or being recorded in another context.

    If the nested draw context feature is enabled it is valid usage for AvxCmdExecuteCommands() to also be recorded to a secondary draw context.
    This means:
     - Secondary contexts can contain calls to AvxCmdExecuteCommands.
     - In other words, secondary-of-secondary nesting is allowed if enabled.
     - You can build hierarchies of reusable command sequences.
    This matches Vulkan’s inheriting and nested secondary command buffer support.
*/

AVX afxCmdId AvxCmdExecuteCommands
(
    // The draw context that will side-load other contexts.
    afxDrawContext dctx,
    // The number of auxiliary contexts to be inlined.
    afxUnit cnt,
    // An ordinal array of @cnt auxiliary draw context handles, which are recorded to execute in the main draw context.
    afxDrawContext auxs[]
);

/*
    The AvxCmdBindPipeline() method binds a pipeline object to a draw context so that subsequent draw/dispatch 
    commands use the pipeline's state until it is replaced or disrupted.
    By binding, the pipeline will typically sets the context with:
     - programmable shaders
     - fixed-function state
     - input assembly or vertex input state
     - rasterization/compute configuration
     - blending, depth/stencil, etc.
    Once bound, it defines how future graphics or compute commands behave.

    Pipeline state is sticky: it stays active until replaced.
    If you bind another pipeline with the same bus (e.g., another graphics pipeline), the previous one is overridden.
    Binding a shader object explicitly (if the API allows separate shader objects) may invalidate or override 
    part of the pipeline's state, forcing a re-bind or reconfiguration.
    This ensures that draw commands know which pipeline they belong to.

    The affected bus depends on pipeline type. So, binding a graphics pipeline does not affect compute commands.
    And, binding a compute pipeline does not affect draw commands.
    If you bind a graphics pipeline, then call a compute dispatch, it has no effect.
    If you bind a compute pipeline, then issue a draw call, it is invalid or ignored.

    The avxVertexInput argument is only meaningful for rasterizer/graphics pipelines.
    It defines: vertex buffer formats, attribute layouts, stride and offset, and per-vertex/per-instance input classification.

    The expected dynamic states tell the draw context which pipeline states will be provided dynamically instead of coming from the compiled pipeline object.

    The API may allow separate shader objects (similar to OpenGL or Vulkan's shader object extension).
    It does mean that if a shader object replaces part of the pipeline (e.g., fragment shader), 
    then the pipeline’s internal state is considered modified or invalidated.
    The next draw call may require a new pipeline or rebinding.
    This is a special case and indicates hybrid pipeline/shader binding rules.


*/

AVX afxCmdId AvxCmdBindPipeline
(
    // The draw context that the pipeline will be bound to. 
    afxDrawContext dctx,
    // The pipeline to be bound.
    avxPipeline pip,
    // The vertex input to be bound when the pipeline is a rasterizer.
    avxVertexInput vin,
    // Flags specifying expected dynamic states.
    afxFlags dynamics
);

/*
    The AvxCmdBindShadersEXT() method directly binds individual shader objects to specific shader stages in the current draw context.
    You are not binding a full pipeline here; you are binding individual shaders. Each one is separately bound.
    This provides a programmable, flexible, OpenGL-like shader binding model instead of requiring all stages to be packaged into a monolithic pipeline.

*/

AVX afxCmdId AvxCmdBindShadersEXT
(
    afxDrawContext dctx,
    // The number of the stages to change.
    afxUnit cnt,
    // An array of values specifying the stage for each shader.
    avxShaderType const stages[],
    // An array of avxCodebase handles describing the shader object to be bound.
    avxCodebase shaders[]
);

/*
    The AvxCmdUseLigature() method binds a ligature object to a specified bus within a draw context.
    A ligature in this rendering framework is a resource binding layout.
    It packages multiple shader-accessible resource interfaces together into a single bindable unit.

    The name "ligature" matches the concept: it ties ("ligates") multiple resource bindings into one object.
    The word "ligature" is the latin term for "binding". The target audience of this API is latin-based speaker.

    Like pipeline binding or shader binding, ligature usage is sticky.
    That is, all subsequent draw/dispatch commands use the resources from the currently bound ligature until a different one is bound.
*/

AVX afxCmdId AvxCmdUseLigature
(
    afxDrawContext dctx, 
    // A value specifying to which bus the ligature will be bound.
    avxBus bus,
    // The ligature to be bound.
    avxLigature liga,
    // Reserved flags.
    afxFlags flags
);

/*
    The AvxCmdBindBuffers() method updates buffer descriptors inside a ligature bound to a bus, 
    but does so directly inside the command buffer, rather than rebuilding the ligature externally.

    In other words, it updates (in-place) buffer bindings for a specific descriptor set and binding index (pin) located on a specific bus.
    This is a very flexible, runtime-friendly descriptor update mechanism.

    @maps is nullable. Doing so, it unbinds existing descriptors, or leaves them unchanged (depending on implementation),
    or resets them to a null resource.
    
    General compatibility rules:
    - Bound ligature (on bus) must have a layout matching the descriptor set and binding ranges being updated.
    - Descriptor type at set, pin must be compatible with buffer descriptors (uniform, storage, etc.).
    - cnt must not exceed the descriptor array length for that binding.
*/

AVX afxCmdId AvxCmdBindBuffers
(
    afxDrawContext dctx,
    // A value specifying to which bus the ligature will be bound.
    avxBus bus,
    // The destination descriptor set to update.
    afxUnit set,
    // The descriptor binding within that set.
    afxUnit pin,
    // The number of descriptors to update. 
    afxUnit cnt,
    // An nullable array of avxBufferedMap structures to be used to update the set.
    avxBufferedMap const maps[]
);

/*
    The AvxCmdBindRasters() method updates raster/image descriptors in a descriptor set of a ligature associated with a bus.
    In simple terms, it binds @cnt raster objects to the descriptor binding starting at (set, pin) in the ligature bound to bus.

    @rasters is nullable. Doing so, it unbinds existing images, or clears descriptors to "null texture".
    You would typically only pass NULL when intentionally invalidating a descriptor.

    Compatibility is layout-dependent. The ligature's descriptor layout determines:
     - whether this binding expects sampled images, storage images, input attachments, etc.
     - how many descriptors it supports
     - their array sizes and types

    AvxCmdBindRasters must obey that layout; exactly like Vulkan's descriptor compatibility rules.
*/

AVX afxCmdId AvxCmdBindRasters
(
    afxDrawContext dctx,
    // A value specifying to which bus the ligature will be bound.
    avxBus bus,
    // The destination descriptor set to update.
    afxUnit set,
    // The descriptor binding within that set.
    afxUnit pin,
    // The number of descriptors to update. 
    afxUnit cnt,
    // An nullable array of avxRaster handles to be used to update the set.
    avxRaster const rasters[]
);

/*
    The AvxCmdBindSamplers() method updates sampler descriptors inside a descriptor set of a ligature bound to a bus.
    In simple terms, it writes @cnt sampler handles into the descriptor binding (set, pin) of the ligature bound to @bus.

    @samplers is nullable. Doing so, you unbind (reset) the sampler descriptor(s), or set them to a null/default sampler.
    This is useful for disabling sampling or forcing a resource rebind.
    
    Samplers and rasters are paired. Ligatures have parallel binding slots for them.
    The descriptor set layout in the active ligature must define whether the binding is a sampler-binding.
    The number (cnt) must match the descriptor array size. And the shader must expect a sampler at that binding location.
    Otherwise, the descriptor update is invalid.
*/

AVX afxCmdId AvxCmdBindSamplers
(
    afxDrawContext dctx,
    // A value specifying to which bus the ligature will be bound.
    avxBus bus,
    // The destination descriptor set to update.
    afxUnit set, 
    // The descriptor binding within that set.
    afxUnit pin,
    // The number of descriptors to update. 
    afxUnit cnt, 
    // An nullable array of avxSampler handles to be used to update the set.
    avxSampler const samplers[]
);

/*
    The AvxCmdPushConstants() method copies a small block of immediate CPU-side data directly into the draw context's "push constant" memory region.
    In other words, it updates a small piece of fast, low-latency uniform data that the currently bound pipeline/shaders can read.
    These values can then be accessed by shaders during subsequent draw/dispatch commands without creating or updating buffers or descriptors.

    Push constants are ultra-fast per-command data.
    Push constants are:
     - Very small (typically a few hundred bytes total)
     - Extremely cheap to update
     - Meant for "scalar" or tiny structured data
     - Intended for per-draw / per-dispatch parameters

    They avoid the overhead of:
     - CPU-side buffer allocations
     - descriptor updates
     - memory barriers

    offset + siz specifies byte ranges in the push constant buffer
    This command gives complete byte-level control:
     - offset --- where in the push-constant region the data begins
     - siz --- number of bytes to copy
     - data --- pointer to the source memory

    This matches Vulkan, which allows partial updates of a larger push-constant block.

    They must match shader/pipeline push-constant layout
    The pipeline or shader object defines:
     - which stages can read push constants
     - the allowed size
     - the internal layout (struct layout)
    If @siz exceeds the pipeline's defined push-constant size,
    @offset + @siz goes out of bounds, or the shader expects a different layout,
    the behavior is invalid. Just like Vulkan, validation rules apply.

    Push constants persist until replaced.
    Once pushed, the data stays active until modified again or until the next pipeline/shader change resets them according to your backend rules.
    Usually, changing pipelines does not erase push constants; it only changes which subset of them is visible.

    Extremely cheap for small, high-frequency updates.
    Unlike buffer-based uniform updates:
     - No descriptor sets
     - No ligature rebinding
     - No memory allocations
     - No synchronization required
    This makes AvxCmdPushConstants ideal for rapidly changing parameters.
*/

AVX afxCmdId AvxCmdPushConstants
(
    afxDrawContext dctx, 
    // The offset where in the push-constant region the data begins.
    afxUnit offset, 
    // The number of bytes to copy.
    afxUnit siz, 
    // The pointer to the source memory.
    void const* data
);

AVX afxCmdId AvxCmdBindFontSIG(afxDrawContext dctx, afxUnit first, afxUnit cnt, /*afxTypography*/void* typ[], avxPipeline pip[], avxSampler smp[], avxRaster ras[]);
AVX afxError AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption);

/*
    void vkCmdBindDescriptorBuffersEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    bufferCount,
    const VkDescriptorBufferBindingInfoEXT*     pBindingInfos);

    vkCmdBindDescriptorBuffersEXT causes any offsets previously set by vkCmdSetDescriptorBufferOffsetsEXT that use the bindings numbered 
    [0.. bufferCount-1] to be no longer valid for subsequent bound pipeline commands. Any previously bound buffers at binding points 
    greater than or equal to bufferCount are unbound.

    void vkCmdSetDescriptorBufferOffsetsEXT(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    setCount,
    const uint32_t*                             pBufferIndices,
    const VkDeviceSize*                         pOffsets);

    The vkCmdSetDescriptorBufferOffsetsEXT() binds setCount pairs of descriptor buffers, specified by indices into the binding points bound using 
    vkCmdBindDescriptorBuffersEXT, and buffer offsets to set numbers [firstSet..firstSet+descriptorSetCount-1] for subsequent bound 
    pipeline commands set by pipelineBindPoint. Set [firstSet + i] is bound to the descriptor buffer at binding pBufferIndices[i] at an 
    offset of pOffsets[i]. Any bindings that were previously applied via these sets, or calls to vkCmdBindDescriptorSets, are no longer valid.
*/

/*
    The AvxCmdBindArgumentBuffersSIGMA() command binds argument buffers to a draw context. It causes any offsets previously set by 
    AvxCmdPushUniformsSIGMA() that use the bindings numbered [ 0, @cnt - 1 ] to be no longer valid for subsequent bound pipeline 
    commands. Any previously bound buffers at binding points greater than or equal to @cnt are unbound.
*/

AVX afxCmdId AvxCmdBindArgumentBuffersSIGMA
(
    afxDrawContext  dctx,
    // the first buffer slot.
    afxUnit bufIdx,
    // the number of buffers.
    afxUnit cnt,
    // an array of avxBuffer objects.
    avxBufferedMap buffers[]
);

AVX afxCmdId AvxCmdPushUniformsSIGMA
(
    afxDrawContext dctx,
    avxBus bus,
    afxUnit set,
    afxUnit binding,
    void const* data,
    afxUnit dataSiz
);

#endif//AVX_PROVISION_H
