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

_AVX afxCmdId AvxCmdExecuteCommands(afxDrawContext dctx, afxUnit cnt, afxDrawContext aux[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(cnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ExecuteCommands), sizeof(cmd->ExecuteCommands) + (cnt * sizeof(cmd->ExecuteCommands.contexts[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ExecuteCommands.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
        cmd->ExecuteCommands.contexts[i] = aux[i];

    return cmdId;
}

_AVX afxCmdId AvxCmdBindPipeline(afxDrawContext dctx, afxUnit segment, avxPipeline pip, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindPipeline), sizeof(cmd->BindPipeline), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindPipeline.pip = pip;
    cmd->BindPipeline.segment = segment;
    cmd->BindPipeline.vin = vin;
    cmd->BindPipeline.dynamics = dynamics;
    return cmdId;
}

_AVX afxCmdId AvxCmdBindBuffers(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindBuffers), sizeof(cmd->BindBuffers) + (cnt * sizeof(cmd->BindBuffers.maps[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindBuffers.set = set;
    cmd->BindBuffers.baseIdx = baseIdx;
    cmd->BindBuffers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedMap const* map = &maps[i];
        cmd->BindBuffers.maps[i].buf = map->buf;
        cmd->BindBuffers.maps[i].offset = map->offset;
        cmd->BindBuffers.maps[i].range = map->range;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindRasters(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, avxRaster const rasters[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindRasters), sizeof(cmd->BindRasters) + (cnt * sizeof(cmd->BindRasters.rasters[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindRasters.set = set;
    cmd->BindRasters.baseIdx = baseIdx;
    cmd->BindRasters.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->BindRasters.rasters[i] = rasters ? rasters[i] : NIL;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindSamplers(afxDrawContext dctx, afxUnit set, afxUnit baseIdx, afxUnit cnt, avxSampler const samplers[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindSamplers), sizeof(cmd->BindSamplers) + (cnt * sizeof(cmd->BindSamplers.samplers[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindSamplers.set = set;
    cmd->BindSamplers.baseIdx = baseIdx;
    cmd->BindSamplers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
        cmd->BindSamplers.samplers[i] = samplers[i];

    return cmdId;
}

_AVX afxCmdId AvxCmdPushConstants(afxDrawContext dctx, afxUnit offset, afxUnit siz, void const* value)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    AFX_ASSERT(siz % sizeof(afxUnit32) == 0);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PushConstants), sizeof(cmd->PushConstants) + siz, &cmdId);
    AFX_ASSERT(cmd);
    cmd->PushConstants.offset = offset;
    cmd->PushConstants.siz = siz;
    AfxCopy(cmd->PushConstants.data, value, siz);
    return cmdId;
}

_AVX afxCmdId AvxCmdBindArgumentBuffersSIGMA(afxDrawContext dctx, afxUnit bufIdx, afxUnit cnt, avxBufferedMap buffers[])
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxUnit bufGenCnt = 0;
    avxBuffer bufGens[4];
    avxBufferInfo bufis[4] = { 0 };
    afxUnit mapSlotToBufGen[4] = { 0 };
    afxUnit mapBufGenToSlot[4] = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i].buf;

        if (!buf)
        {
            if (buffers[i].range)
            {
                bufis[bufGenCnt].size = AFX_ALIGNED_SIZE(buffers[i].range, AVX_BUFFER_ALIGNMENT);
                bufis[bufGenCnt].usage = avxBufferUsage_ARGUMENT | avxBufferUsage_UNIFORM;
                bufis[bufGenCnt].flags = avxBufferFlag_WX;
                mapSlotToBufGen[bufGenCnt] = i;
                mapBufGenToSlot[i] = bufGenCnt;
                ++bufGenCnt;
            }
        }
    }

    if (bufGenCnt)
    {
        if (AvxAcquireBuffers(AfxGetProvider(AfxGetProvider(AfxGetProvider(dctx))), bufGenCnt, bufis, bufGens))
        {
            AfxThrowError();
        }
        AFX_ASSERT_OBJECTS(afxFcc_BUF, bufGenCnt, bufGens);

        afxUnit baseObsToBeDisposedUnit;
        afxObject* obsToBeDisposed = AfxPushArrayUnits(&dctx->objsToBeDisposed, bufGenCnt, &baseObsToBeDisposedUnit, NIL, 0);
        AFX_ASSERT(obsToBeDisposed);

        void** bufPtr[4] = { 0 };
        avxBufferedMap bufGenRemaps[4] = { 0 };

        for (afxUnit i = 0; i < bufGenCnt; i++)
        {
            bufGenRemaps[i].buf = bufGens[i];
            bufGenRemaps[i].range = bufis[mapBufGenToSlot[i]].size;
            bufPtr[i] = (void**)&dctx->argBufs[mapBufGenToSlot[i]].bytemap;
            
            dctx->argBufs[mapBufGenToSlot[i]].alloced = TRUE;
            obsToBeDisposed[i] = bufGens[i];
        }

        if (AvxMapBuffers(AfxGetProvider(bufGens[0]), bufGenCnt, bufGenRemaps, bufPtr))
        {
            AfxThrowError();
        }
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i].buf;

        if (!buf)
        {
            if (buffers[i].range)
            {
                buf = bufGens[i];
                AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
                dctx->argBufs[bufIdx + i].map.buf = buf;
                dctx->argBufs[bufIdx + i].map.offset = 0;
                dctx->argBufs[bufIdx + i].map.range = bufis[mapSlotToBufGen[i]].size;
                dctx->argBufs[bufIdx + i].map.flags = bufis[mapSlotToBufGen[i]].flags;
                AFX_ASSERT(dctx->argBufs[bufIdx + i].alloced);
                AFX_ASSERT(dctx->argBufs[bufIdx + i].bytemap);
                dctx->argBufs[bufIdx + i].nextOffset = 0;
                dctx->argBufs[bufIdx + i].remainRoom = dctx->argBufs[bufIdx].map.range;
            }
#if 0
            else
            {
                dctx->argBufs[bufIdx].map.buf = NIL;
                dctx->argBufs[bufIdx].map.offset = 0;
                dctx->argBufs[bufIdx].map.range = 0;
                dctx->argBufs[bufIdx].map.flags = NIL;
                dctx->argBufs[bufIdx].nextOffset = 0;
                dctx->argBufs[bufIdx].remainRoom = 0;
            }
#endif
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            avxBufferUsage usage = AvxGetBufferUsage(buf, avxBufferUsage_ARGUMENT);
            AFX_ASSERT(usage == avxBufferUsage_ARGUMENT);
            afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufCap, buffers[i].offset, buffers[i].range);

            AFX_ASSERT_ALIGNMENT(buffers[i].range, AFX_SIMD_ALIGNMENT);

            if (dctx->argBufs[bufIdx + i].map.buf)
            {
                AvxUnmapBuffer(dctx->argBufs[bufIdx + i].map.buf, FALSE);
            }

            dctx->argBufs[bufIdx + i].map.buf = buf;
            dctx->argBufs[bufIdx + i].map.offset = AFX_MIN(buffers[i].offset, bufCap - 1);
            dctx->argBufs[bufIdx + i].map.range = AFX_MIN(buffers[i].range, bufCap - dctx->argBufs[bufIdx + i].map.offset);
            dctx->argBufs[bufIdx + i].map.flags = buffers[i].flags;
            dctx->argBufs[bufIdx + i].alloced = FALSE;
            dctx->argBufs[bufIdx + i].nextOffset = 0;
            dctx->argBufs[bufIdx + i].remainRoom = dctx->argBufs[bufIdx + i].map.range;

            void* ptr = NIL;
            AvxMapBuffer(dctx->argBufs[bufIdx + i].map.buf, dctx->argBufs[bufIdx + i].map.offset, dctx->argBufs[bufIdx + i].map.range, dctx->argBufs[bufIdx + i].map.flags, &ptr);
            dctx->argBufs[bufIdx + i].bytemap = ptr;
        }
    }

    return err;
}

_AVX afxCmdId AvxCmdPushUniformsSIGMA(afxDrawContext dctx, afxUnit set, afxUnit binding, void const* data, afxUnit dataSiz)
{
    afxError err = AFX_ERR_NONE;
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxDrawContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxUnit siz = AFX_ALIGNED_SIZE(dataSiz, AFX_SIMD_ALIGNMENT);

    if (siz > dctx->argBufs[set].remainRoom)
    {
        AfxThrowError();
        return err;
    }

    avxBufferedMap map = { 0 };
    map.buf = dctx->argBufs[set].map.buf;
    map.offset = dctx->argBufs[set].nextOffset;
    map.range = siz;
    map.flags = dctx->argBufs[set].map.flags;
    AvxCmdBindBuffers(dctx, set, binding, 1, &map);

    AfxCopy(&(dctx->argBufs[set].bytemap[dctx->argBufs[set].nextOffset]), data, dataSiz);

    dctx->argBufs[set].nextOffset += siz;
    dctx->argBufs[set].remainRoom -= siz;

    return err;
}
