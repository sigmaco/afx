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

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "../impl/avxImplementation.h"

_AVX afxCmdId AvxCmdBindVertexInput(afxDrawContext dctx, avxVertexDecl vin)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    
    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindVertexInput), sizeof(cmd->BindVertexInput), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindVertexInput.vin = vin;
    return cmdId;
}

_AVX afxCmdId AvxCmdSwitchFrontFace(afxDrawContext dctx, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    AFX_ASSERT_RANGE(cw, FALSE, TRUE);
    cw = AfxClamp(cw, FALSE, TRUE);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SwitchFrontFace), sizeof(cmd->SwitchFrontFace), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchFrontFace.cw = cw;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetCullMode(afxDrawContext dctx, avxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// mode must be a valid combination of avxCullMode values.
    AFX_ASSERT_BOUNDS(mode, avxCullMode_NONE, avxCullMode_BOTH);
    mode = AfxClamp(mode, avxCullMode_NONE, avxCullMode_BOTH);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetCullMode), sizeof(cmd->SetCullMode), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetCullMode.mode = mode;
    return cmdId;
}

_AVX afxCmdId AvxCmdAdjustViewports(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, afxViewport const viewports[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// cnt must be greater than 0.
    AFX_ASSERT(cnt);

    /// viewports must be a valid pointer to an array of cnt valid afxViewport structures.
    AFX_ASSERT(viewports);    
    AFX_ASSERT_RANGE(8, baseIdx, cnt);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(AdjustViewports), sizeof(cmd->AdjustViewports) + (cnt * sizeof(cmd->AdjustViewports.viewports[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->AdjustViewports.baseIdx = baseIdx;
    cmd->AdjustViewports.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxViewport const* vp = &viewports[i];
        AFX_ASSERT(vp->extent[0]);
        AFX_ASSERT(vp->extent[1]);
        AFX_ASSERT_BOUNDS(vp->minDepth, 0.0, 1.0);
        AFX_ASSERT_BOUNDS(vp->maxDepth, 0.0, 1.0);

        cmd->AdjustViewports.viewports[i] = *vp;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindVertexSources(afxDrawContext dctx, afxUnit baseSlotIdx, afxUnit slotCnt, afxBuffer buffers[], afxUnit32 const offsets[], afxUnit32 const ranges[], afxUnit32 const strides[])
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(8, baseSlotIdx, slotCnt);
    AFX_ASSERT(strides);
    
    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindVertexSources), sizeof(cmd->BindVertexSources) + (slotCnt * sizeof(cmd->BindVertexSources.items[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindVertexSources.baseSlotIdx = baseSlotIdx;
    cmd->BindVertexSources.slotCnt = slotCnt;

    for (afxUnit i = 0; i < slotCnt; i++)
    {
        if (buffers)
        {
            afxBuffer buf = buffers[i];

            if (buf)
            {
                /// If buf is not NIL, buffer must be a valid afxBuffer handle.
                AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

                /// All elements of buffers must have been created with the afxBufferUsage_VERTEX flag.
                AFX_ASSERT(AfxTestBufferUsage(buf, afxBufferUsage_VERTEX));

                afxUnit bufCap = AfxGetBufferCapacity(buf, 0);

                /// If sizes is not NIL, all elements of offsets must be less than the size of the corresponding element in buffers.
                /// If sizes is not NIL, all elements of offsets plus pSizes , where sizes is not zero, must be less than or equal to the size of the corresponding element in buffers.
                afxUnit offset = offsets ? offsets[i] : 0;
                afxUnit range = ranges ? ranges[i] : bufCap - offset;
                AFX_ASSERT_RANGE(bufCap, offset, range);
                AFX_ASSERT(range);

                AFX_ASSERT(strides && strides[i]);
            }
        }
        else
        {
            /// If an element of buffers is NIL, then the corresponding element of offsets must be zero.
            AFX_ASSERT(!ranges || !ranges[i]);
            AFX_ASSERT(!offsets || !offsets[i]);
            AFX_ASSERT(!strides || !strides[i]);
        }
        cmd->BindVertexSources.items[i].buf = buffers ? buffers[i] : NIL;
        cmd->BindVertexSources.items[i].offset = offsets ? offsets[i] : 0;
        cmd->BindVertexSources.items[i].range = ranges ? ranges[i] : 0;
        cmd->BindVertexSources.items[i].stride = strides ? strides[i] : 0;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindIndexSource(afxDrawContext dctx, afxBuffer buf, afxUnit32 offset, afxUnit32 range, afxUnit32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    /// idxSiz must not be zero.
    AFX_ASSERT(idxSiz);

    if (!buf)
    {
        AFX_ASSERT(!offset);
        AFX_ASSERT(!range);
        offset = 0;
        range = 0;
    }
    else
    {
        /// If buf is not NIL, buffer must be a valid afxBuffer handle.
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        /// buffer must have been created with the afxBufferUsage_INDEX flag.
        AFX_ASSERT(AfxTestBufferUsage(buf, afxBufferUsage_INDEX));

        /// offset must be less than the size of buffer.
        afxUnit bufCap = AfxGetBufferCapacity(buf, 0);
        AFX_ASSERT_RANGE(bufCap, offset, range);
        AFX_ASSERT(range);

        offset = AfxClamp(offset, 0, bufCap - 1);
        range = AfxClamp(range, 0, bufCap - offset);
    }

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(BindIndexSource), sizeof(cmd->BindIndexSource), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindIndexSource.buf = buf;
    cmd->BindIndexSource.offset = offset;
    cmd->BindIndexSource.range = range;
    cmd->BindIndexSource.idxSiz = idxSiz;
    return cmdId;
}

_AVX afxCmdId AvxCmdSetPrimitiveTopology(afxDrawContext dctx, avxTopology topology)
{
    afxError err = AFX_ERR_NONE;
    /// dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    /// dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxCmdbState_RECORDING);
    /// This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /// topology must be a valid avxTopology value.
    AFX_ASSERT(topology < avxTopology_TOTAL);

    afxCmdId cmdId;
    avxCmd* cmd = _AvxDctxPushCmd(dctx, AVX_GET_STD_CMD_ID(SetPrimitiveTopology), sizeof(cmd->SetPrimitiveTopology), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetPrimitiveTopology.topology = topology;
    return cmdId;
}
