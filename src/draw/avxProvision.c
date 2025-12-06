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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

#ifdef _AFX_DEBUG
#   define _AVX_DEBUG_BINDING_COMMANDS TRUE
#endif

_AVX afxCmdId AvxCmdCommenceDebugScope(afxDrawContext dctx, afxString const* name, avxColor const color)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // Bump up the label.
    ++dctx->dbgUtilOpenLabelCnt;

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CommenceDebugScope), sizeof(cmd->CommenceDebugScope), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->CommenceDebugScope.label, name);

    if (color)
        AvxCopyColor(cmd->CommenceDebugScope.color, color);
    else
        AvxResetColor(cmd->CommenceDebugScope.color);

    return cmdId;
}

_AVX afxCmdId AvxCmdConcludeDebugScope(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // There must be an open debug scope.
    AFX_ASSERT(dctx->dbgUtilOpenLabelCnt > 0);
    --dctx->dbgUtilOpenLabelCnt;

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ConcludeDebugScope), sizeof(cmd->ConcludeDebugScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ConcludeDebugScope.nothing = NIL;
    return cmdId;
}

_AVX afxCmdId AvxCmdMarkDebugMilestone(afxDrawContext dctx, afxString const* name, avxColor const color)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(MarkDebugMilestone), sizeof(cmd->MarkDebugMilestone), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->MarkDebugMilestone.label, name);

    if (color)
        AvxCopyColor(cmd->MarkDebugMilestone.color, color);
    else
        AvxResetColor(cmd->MarkDebugMilestone.color);

    return cmdId;
}

_AVX afxError AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(at);
    AFX_ASSERT(v);
    AFX_ASSERT(caption);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(StampDebug), sizeof(cmd->StampDebug) + AFX_ALIGN_SIZE(caption->len, 16), &cmdId);
    AFX_ASSERT(cmd);
    AfxM4dCopyAtm(cmd->StampDebug.v, v);
    AfxV2dCopy(cmd->StampDebug.at, at);
    AfxCopy(cmd->StampDebug.data, caption->start, caption->len);

    return err;
}

_AVX afxCmdId AvxCmdExecuteCommands(afxDrawContext dctx, afxUnit cnt, afxDrawContext auxs[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(cnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ExecuteCommands), sizeof(cmd->ExecuteCommands) + (cnt * sizeof(cmd->ExecuteCommands.contexts[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ExecuteCommands.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->ExecuteCommands.contexts[i].dctx = auxs[i];
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindPipeline(afxDrawContext dctx, avxPipeline pip, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    // This command must not be recorded when transform feedback is active.
    AFX_ASSERT(!dctx->xfbActive);

    // pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    avxBus bus = AvxGetPipelineBus(pip);
    AFX_ASSERT(bus < avxBus_TOTAL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindPipeline), sizeof(cmd->BindPipeline), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindPipeline.pip = pip;
    cmd->BindPipeline.bus = bus;
    cmd->BindPipeline.vin = vin;
    cmd->BindPipeline.dynamics = dynamics;
    cmd->BindPipeline.flags = NIL;

    dctx->pipelines[bus].pip = pip;
    dctx->pipelines[bus].useSepShaders = FALSE;
    dctx->pipelines[bus].vin = vin;
    dctx->pipelines[bus].dynFlags = dynamics;

    avxLigature liga = NIL;
    AvxGetPipelineLigature(pip, &liga);
    if (dctx->ligatures[bus].liga != liga)
    {
        dctx->ligatures[bus].liga = liga;
        dctx->ligatures[bus].ligaBindCmdId = cmdId;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdUseLigature(afxDrawContext dctx, avxBus bus, avxLigature liga, afxFlags flags)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // liga must be a valid avxLigature handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

#if 0
    if (dctx->ligatures[bus].liga == liga)
        return dctx->ligatures[bus].bindCmdId;
#endif

    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UseLigature), sizeof(cmd->UseLigature), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UseLigature.liga = liga;
    cmd->UseLigature.bus = bus;
    cmd->UseLigature.flags = flags;

    if (dctx->ligatures[bus].liga != liga)
    {
        dctx->ligatures[bus].liga = liga;
        dctx->ligatures[bus].ligaBindCmdId = cmdId;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindShadersEXT(afxDrawContext dctx, afxUnit cnt, avxShaderType const stages[], avxCodebase shaders[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // pip must be a valid avxPipeline handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SHD, cnt, shaders);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindShadersEXT), sizeof(cmd->BindShadersEXT) + (cnt * sizeof(cmd->BindShadersEXT.stages[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindShadersEXT.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->BindShadersEXT.stages[i].shd = shaders[i];
        cmd->BindShadersEXT.stages[i].stage = stages[i];
    }

    return cmdId;
}

_AVX afxCmdId AvxCmdBindBuffers(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit pin, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindBuffers), sizeof(cmd->BindBuffers) + (cnt * sizeof(cmd->BindBuffers.maps[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindBuffers.bus = bus;
    cmd->BindBuffers.set = set;
    cmd->BindBuffers.pin = pin;
    cmd->BindBuffers.cnt = cnt;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
    avxLigature liga = dctx->ligatures[bus].liga;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
#endif//_AVX_DEBUG_BINDING_COMMANDS

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedMap const* map = maps ? &maps[i] : &(avxBufferedMap const) { 0 };        
        afxSize offset = map->offset;
        afxUnit range = map->range;
        avxBuffer buf = map->buf;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
        avxBufferUsage usage;
        afxSize bufSiz;
        if (buf)
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            bufSiz = AvxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufSiz, offset, range);
            usage = AvxGetBufferUsage(buf, NIL);
        }

        avxLigament desc;
        if (AvxDescribeLigament(liga, set, pin + i, 1, &desc))
        {
            switch (desc.type)
            {
            case avxShaderParam_UNIFORM:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_UNIFORM);
                }
                break;
            }
            case avxShaderParam_STORAGE:
                //case avxShaderParam_BUFFER:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_STORAGE);
                }
                break;
            }
            case avxShaderParam_FETCH:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_FETCH);
                }
                break;
            }
            case avxShaderParam_TSBO:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_TENSOR);
                }
                break;
            }
            default:
            {
                AFX_ASSERT((usage & avxBufferUsage_UNIFORM) ||
                    (usage & avxBufferUsage_STORAGE) ||
                    (usage & avxBufferUsage_FETCH) ||
                    (usage & avxBufferUsage_TENSOR));
                AfxThrowError();
                break;
            }
            }
        }
#endif//_AVX_DEBUG_BINDING_COMMANDS

        cmd->BindBuffers.maps[i].buf = buf;
        cmd->BindBuffers.maps[i].offset = offset;
        cmd->BindBuffers.maps[i].range = range;

        dctx->ligatures[bus].bindings[set][pin].buf.buf = buf;
        dctx->ligatures[bus].bindings[set][pin].buf.offset = offset;
        dctx->ligatures[bus].bindings[set][pin].buf.range = range;
        dctx->ligatures[bus].bindings[set][pin].buf.bufBindCmdId = cmdId;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindRasters(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit pin, afxUnit cnt, avxRaster const rasters[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindRasters), sizeof(cmd->BindRasters) + (cnt * sizeof(cmd->BindRasters.rasters[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindRasters.bus = bus;
    cmd->BindRasters.set = set;
    cmd->BindRasters.pin = pin;
    cmd->BindRasters.cnt = cnt;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
    avxLigature liga = dctx->ligatures[bus].liga;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
#endif//_AVX_DEBUG_BINDING_COMMANDS

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRaster ras = rasters ? rasters[i] : NIL;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
        }

        avxLigament desc;
        if (AvxDescribeLigament(liga, set, pin + i, 1, &desc))
        {
            switch (desc.type)
            {
            case avxShaderParam_TEXTURE:
            {
                break;
            }
            case avxShaderParam_RASTER:
            {
                break;
            }
            case avxShaderParam_IMAGE:
            {
                break;
            }
            default:
            {
                AFX_ASSERT((desc.type == avxShaderParam_TEXTURE) ||
                    (desc.type == avxShaderParam_RASTER) ||
                    (desc.type == avxShaderParam_IMAGE));
                AfxThrowError();
                break;
            }
            }
        }
#endif//_AVX_DEBUG_BINDING_COMMANDS

        cmd->BindRasters.rasters[i] = ras;

        dctx->ligatures[bus].bindings[set][pin].img.ras = ras;
        dctx->ligatures[bus].bindings[set][pin].img.rasBindCmdId = cmdId;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdBindSamplers(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit pin, afxUnit cnt, avxSampler const samplers[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindSamplers), sizeof(cmd->BindSamplers) + (cnt * sizeof(cmd->BindSamplers.samplers[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindSamplers.bus = bus;
    cmd->BindSamplers.set = set;
    cmd->BindSamplers.pin = pin;
    cmd->BindSamplers.cnt = cnt;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
    avxLigature liga = dctx->ligatures[bus].liga;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
#endif//_AVX_DEBUG_BINDING_COMMANDS

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxSampler samp = samplers ? samplers[i] : NIL;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
        if (samp)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
        }

        avxLigament desc;
        if (AvxDescribeLigament(liga, set, pin, 1, &desc))
        {
            switch (desc.type)
            {
            case avxShaderParam_TEXTURE:
            {
                break;
            }
            case avxShaderParam_SAMPLER:
            {
                break;
            }
            default:
            {
                AFX_ASSERT((desc.type == avxShaderParam_TEXTURE) ||
                    (desc.type == avxShaderParam_SAMPLER));
                AfxThrowError();
                break;
            }
            }
        }
#endif//_AVX_DEBUG_BINDING_COMMANDS

        cmd->BindSamplers.samplers[i] = samp;

        dctx->ligatures[bus].bindings[set][pin].img.samp = samp;
        dctx->ligatures[bus].bindings[set][pin].img.sampBindCmdId = cmdId;
    }
    return cmdId;
}

_AVX afxCmdId AvxCmdPushConstants(afxDrawContext dctx, afxUnit offset, afxUnit siz, void const* value)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
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
                bufis[bufGenCnt].size = AFX_ALIGN_SIZE(buffers[i].range, AVX_BUFFER_ALIGNMENT);
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
        if (AvxAcquireBuffers(AfxGetHost(AfxGetHost(AfxGetHost(dctx))), bufGenCnt, bufis, bufGens))
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

        if (AvxMapBuffers(AfxGetHost(bufGens[0]), bufGenCnt, bufGenRemaps, bufPtr))
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

_AVX afxCmdId AvxCmdPushUniformsSIGMA(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit binding, void const* data, afxUnit dataSiz)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxUnit siz = AFX_ALIGN_SIZE(dataSiz, AFX_SIMD_ALIGNMENT);

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
    AvxCmdBindBuffers(dctx, bus, set, binding, 1, &map);

    AfxCopy(&(dctx->argBufs[set].bytemap[dctx->argBufs[set].nextOffset]), data, dataSiz);

    dctx->argBufs[set].nextOffset += siz;
    dctx->argBufs[set].remainRoom -= siz;

    return err;
}
