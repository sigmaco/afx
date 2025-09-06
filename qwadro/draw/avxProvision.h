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

/// Execute a secondary draw context from a primary draw context.
/// If any element of @aux was not recorded with the simultaneous use flag, and it was recorded into any other primary draw context which is currently in the executable or recording state, that primary draw context becomes invalid.

/// If the nested draw context feature is enabled it is valid usage for AvxCmdExecuteCommands to also be recorded to a secondary draw context.

AVX afxCmdId        AvxCmdExecuteCommands
(
    afxDrawContext  dctx,
    afxUnit         cnt, // is the length of the @aux array.
    afxDrawContext  aux[], // is a pointer to an array of @cnt secondary draw context handles, which are recorded to execute in the primary draw context in the order they are listed in the array.
    afxUnit const   batches[]
);

/// Bind a pipeline object to a draw context.
/// Once bound, a pipeline binding affects subsequent commands that interact with the given pipeline type in the draw context until a different pipeline of the same type is bound to the bind point, or until the pipeline bind point is disturbed by binding a shader object.
/// Commands that do not interact with the given pipeline type must not be affected by the pipeline state.

AVX afxCmdId        AvxCmdBindPipeline
(
    // The draw context that the pipeline will be bound to. 
    afxDrawContext  dctx,
    // A value specifying to which bus the pipeline will be bound.
    // Binding one bus does not disturb the other buses.
    avxBus      bus,
    // The pipeline to be bound.
    avxPipeline     pip,
    // The vertex input to be bound when the pipeline is a rasterizer.
    avxVertexInput  vin,
    // Flags specifying expected dynamic states.
    afxFlags        dynamics
);

AVX afxCmdId        AvxCmdBindShadersEXT
(
    afxDrawContext  dctx,
    // The number of the stages to change.
    afxUnit         cnt,
    // An array of values specifying the stage for each shader.
    avxShaderType const stages[],
    // An array of avxShader handles describing the shader object to be bound.
    avxShader       shaders[]
);

AVX afxCmdId AvxCmdUseLigature
(
    afxDrawContext  dctx, 
    // A value specifying to which bus the ligature will be bound.
    avxBus      bus,
    // The ligature to be bound.
    avxLigature     liga,
    // Reserved flags.
    afxFlags        flags
);

AVX afxCmdId        AvxCmdBindBuffers
(
    afxDrawContext dctx,
    // A value specifying to which bus the ligature will be bound.
    avxBus      bus,
    // The destination descriptor set to update.
    afxUnit         set,
    // The descriptor binding within that set.
    afxUnit         pin,
    // The number of descriptors to update. 
    afxUnit         cnt,
    // An nullable array of avxBufferedMap structures to be used to update the set.
    avxBufferedMap const maps[]
);

AVX afxCmdId        AvxCmdBindRasters
(
    afxDrawContext  dctx,
    // A value specifying to which bus the ligature will be bound.
    avxBus      bus,
    // The destination descriptor set to update.
    afxUnit         set,
    // The descriptor binding within that set.
    afxUnit         pin,
    // The number of descriptors to update. 
    afxUnit         cnt,
    // An nullable array of avxRaster handles to be used to update the set.
    avxRaster const rasters[]
);

AVX afxCmdId        AvxCmdBindSamplers
(
    afxDrawContext  dctx,
    // A value specifying to which bus the ligature will be bound.
    avxBus      bus,
    // The destination descriptor set to update.
    afxUnit         set, 
    // The descriptor binding within that set.
    afxUnit         pin,
    // The number of descriptors to update. 
    afxUnit         cnt, 
    // An nullable array of avxSampler handles to be used to update the set.
    avxSampler const samplers[]
);

AVX afxCmdId        AvxCmdBindFontSIG(afxDrawContext dctx, afxUnit first, afxUnit cnt, /*afxTypography*/void* typ[], avxPipeline pip[], avxSampler smp[], avxRaster ras[]);
AVX afxError        AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption);

AVX afxCmdId        AvxCmdPushConstants(afxDrawContext dctx, afxUnit offset, afxUnit siz, void const* data);

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

AVX afxCmdId        AvxCmdBindArgumentBuffersSIGMA
(
    afxDrawContext  dctx,
    // the first buffer slot.
    afxUnit         bufIdx,
    // the number of buffers.
    afxUnit         cnt,
    // an array of avxBuffer objects.
    avxBufferedMap  buffers[]
);

AVX afxCmdId        AvxCmdPushUniformsSIGMA
(
    afxDrawContext  dctx,
    avxBus      bus,
    afxUnit         set,
    afxUnit         binding,
    void const*     data,
    afxUnit         dataSiz
);

#endif//AVX_PROVISION_H
