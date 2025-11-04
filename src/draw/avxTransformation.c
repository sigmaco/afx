/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "ddi/avxImplementation.h"

_AVX afxCmdId AvxCmdUseVertexInput(afxDrawContext dctx, avxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    
    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UseVertexInput), sizeof(cmd->UseVertexInput), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UseVertexInput.vin = vin;

    dctx->vertices.vin = vin;
    dctx->vertices.vinBindCmdId = cmdId;

    return cmdId;
}

_AVX afxCmdId AvxCmdBindVertexBuffers(afxDrawContext dctx, afxUnit basePin, afxUnit cnt, avxBufferedStream const streams[])
{
    afxError err = AFX_ERR_NONE;

#if AVX_VALIDATION_ENABLED
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(streams);

#if AVX_LIMIT_VALIDATION_ENABLED
    /*
        @baseSlotIdx must be less than device limit 'maxVtxInSrcs'.

        The sum of @baseSlotIdx and @slotCnt must be less than or equal to the device limit 'maxVtxInSrcs'.
    */
    AFX_ASSERT_RANGE(dctx->devLimits->maxVtxInSrcs, basePin, cnt);
#endif
#endif

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindVertexBuffers), sizeof(cmd->BindVertexBuffers) + (cnt * sizeof(cmd->BindVertexBuffers.src[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindVertexBuffers.basePin = basePin;
    cmd->BindVertexBuffers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedStream const* stream = &streams[i];

        avxBuffer buf = stream->buf;
        afxSize offset = stream->offset;
        afxUnit range = stream->range;
        afxUnit stride = stream->stride;

#if AVX_VALIDATION_ENABLED
        if (buf)
        {
            // If @buffers is not NIL, @buf must be a valid avxBuffer handle.
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            // All elements of @buffers must have been created with the avxBufferUsage_VERTEX flag.
            AFX_ASSERT(AvxGetBufferUsage(buf, avxBufferUsage_VERTEX));

            // If @ranges is not NIL, all elements of @offsets must be less than the size of the corresponding element in @buffers.
            // If @ranges is not NIL, all elements of @offsets plus @ranges, where sizes is not zero, must be less than or equal to the size of the corresponding element in @buffers.
            afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufCap, offset, range);
            offset = AFX_MIN(offset, bufCap - 1);
            range = AFX_MIN(range ? range : bufCap, bufCap - offset);
            AFX_ASSERT(range);
        }
        else
        {
            // If an element of @buffers is NIL, then the corresponding element of @offsets must be zero.
            AFX_ASSERT(range == 0);
            AFX_ASSERT(offset == 0);
            //AFX_ASSERT(stride);
        }

#if AVX_LIMIT_VALIDATION_ENABLED
        // If @strides is not NIL each element of @strides must be less than or equal to the device limit 'maxVertexInputBindingStride'.
        AFX_ASSERT_RANGE(dctx->devLimits->maxVtxInSrcStride, 0, stride);
#endif
#endif
        cmd->BindVertexBuffers.src[i].buf = buf;
        cmd->BindVertexBuffers.src[i].offset = offset;
        cmd->BindVertexBuffers.src[i].range = range;
        cmd->BindVertexBuffers.src[i].stride = stride;

        dctx->vertices.streams[basePin + i].buf = buf;
        dctx->vertices.streams[basePin + i].offset = offset;
        dctx->vertices.streams[basePin + i].range = range;
        dctx->vertices.streams[basePin + i].stride = stride;
        dctx->vertices.vboBindCmdId[basePin + i] = cmdId;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindIndexBuffer(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, afxUnit32 range, afxUnit32 idxSiz)
{
    afxError err = AFX_ERR_NONE;

    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // idxSiz must not be zero.
    AFX_ASSERT(idxSiz);
    if (idxSiz == 1)
    {
        // If idxSiz is 1 byte longe, the indexTypeUint8 feature must be enabled.
#if AVX_FEATURE_VALIDATION_ENABLED
        //AFX_ASSERT(dctx->enabledFeatures->idxSizUint8);
#endif
    }

    if (!buf)
    {
        AFX_ASSERT(!offset);
        AFX_ASSERT(!range);
        // If @buf is NIL, @offset must be zero.
        offset = 0;
        range = 0;
    }
    else
    {
        // If @buf is not NIL, buffer must be a valid avxBuffer handle.
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
        // @buf must have been acquired with the avxBufferUsage_INDEX flag.
        AFX_ASSERT(AvxGetBufferUsage(buf, avxBufferUsage_INDEX));

        // @offset must be less than the size of buffer.
        afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
        // The sum of @offset and the base address of the @range of buffer, must be a multiple of the size of the type indicated by @idxSiz.
        // If @range is not WHOLE, @range must be a multiple of the size of the type indicated by @idxSiz.
        // If @range is not WHOLE, the sum of @offset and @range must be less than or equal to the size of buffer.
        AFX_ASSERT(offset % idxSiz == 0);
        AFX_ASSERT(range % idxSiz == 0);
        // @offset must be less than the size of buffer.
        AFX_ASSERT_RANGE(bufCap, offset, range);
        AFX_ASSERT(range);

        offset = AFX_MIN(offset, bufCap - 1);
        range = AFX_MIN(range ? range : bufCap, bufCap - offset);
    }

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindIndexBuffer), sizeof(cmd->BindIndexBuffer), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindIndexBuffer.buf = buf;
    cmd->BindIndexBuffer.offset = offset;
    cmd->BindIndexBuffer.range = range;
    cmd->BindIndexBuffer.idxSiz = idxSiz;

    dctx->vertices.ibo = buf;
    dctx->vertices.iboOffset = offset;
    dctx->vertices.iboRange = range;
    dctx->vertices.iboStride = idxSiz;
    dctx->vertices.iboBindCmdId = cmdId;

    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchFrontFace(afxDrawContext dctx, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    AFX_ASSERT_RANGE(cw, FALSE, TRUE);
    cw = AFX_CLAMP(cw, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SwitchFrontFace), sizeof(cmd->SwitchFrontFace), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchFrontFace.cw = cw;
    return cmdId;
}

_AVX afxCmdId AvxCmdChangeCullMode(afxDrawContext dctx, avxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // mode must be a valid combination of avxCullMode values.
    AFX_ASSERT_BOUNDS(mode, avxCullMode_NONE, avxCullMode_BOTH);
    mode = AFX_CLAMP(mode, avxCullMode_NONE, avxCullMode_BOTH);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetCullMode), sizeof(cmd->SetCullMode), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetCullMode.mode = mode;
    return cmdId;
}

_AVX afxCmdId AvxCmdAdjustViewports(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, avxViewport const viewports[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // cnt must be greater than 0.
    AFX_ASSERT(cnt);
    // viewports must be a valid pointer to an array of cnt valid avxViewport structures.
    AFX_ASSERT(viewports);

#if AVX_VALIDATION_ENABLED
#if AVX_LIMIT_VALIDATION_ENABLED
    // The sum of @baseIdx and @cnt must be between 1 and the device limit 'maxVpCnt", inclusive.
    AFX_ASSERT_RANGE(dctx->devLimits->maxVpCnt, baseIdx, cnt);
    AFX_ASSERT_RANGE(AVX_MAX_VIEWPORTS, baseIdx, cnt);
#endif

#if AVX_FEATURE_VALIDATION_ENABLED
    if (!dctx->enabledFeatures->multiViewport)
    {
        /*
            If the multiViewport feature is not enabled, 
                firstViewport must be 0.
            If the multiViewport feature is not enabled, 
                viewportCount must be 1.
        */
        AFX_ASSERT(baseIdx == 0);
        AFX_ASSERT(cnt == 1);
    }
#endif
#endif

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(AdjustViewports), sizeof(cmd->AdjustViewports) + (cnt * sizeof(cmd->AdjustViewports.viewports[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->AdjustViewports.baseIdx = baseIdx;
    cmd->AdjustViewports.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxViewport const* vp = &viewports[i];
#if AVX_VALIDATION_ENABLED
#if AVX_LIMIT_VALIDATION_ENABLED
        AFX_ASSERT_RANGE(dctx->devLimits->maxVpDimensions[0], vp->origin[0], vp->extent[0]);
        AFX_ASSERT_RANGE(dctx->devLimits->maxVpDimensions[1], vp->origin[1], vp->extent[1]);
        AFX_ASSERT(dctx->devLimits->vpBoundsRange[0] <= vp->minDepth);
        AFX_ASSERT(dctx->devLimits->vpBoundsRange[1] >= vp->maxDepth);
#endif
#endif
        cmd->AdjustViewports.viewports[i] = *vp;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdSetPrimitiveTopology(afxDrawContext dctx, avxTopology topology)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // topology must be a valid avxTopology value.
    AFX_ASSERT(topology < avxTopology_TOTAL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetPrimitiveTopology), sizeof(cmd->SetPrimitiveTopology), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetPrimitiveTopology.topology = topology;
    return cmdId;
}

_AVX afxCmdId AvxCmdChangeFillModeEXT(afxDrawContext dctx, avxFillMode mode)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(mode < avxFillMode_TOTAL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetFillModeEXT), sizeof(cmd->SetFillModeEXT), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetFillModeEXT.mode = mode;
    return cmdId;
}
