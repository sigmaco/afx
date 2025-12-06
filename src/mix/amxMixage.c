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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_SINK_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_TRACK_C
#define _AMX_AUDIO_C
#define _AMX_AUDIO_C
#include "amxIcd.h"

_AMX afxCmdId AmxCmdCommenceMixScope(afxMixContext mix, amxMixScope const* scope)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(CommenceMixScope), sizeof(cmd->CommenceMixScope) + (scope->chanCnt * sizeof(cmd->CommenceMixScope.chans[0])), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    
    AFX_ASSERT(scope);
    cmd->CommenceMixScope.baseChan = scope->baseChan;
    cmd->CommenceMixScope.baseSamp = scope->baseSamp;
    cmd->CommenceMixScope.baseSeg = scope->baseSeg;
    cmd->CommenceMixScope.chanCnt = scope->chanCnt;
    cmd->CommenceMixScope.sampCnt = scope->sampCnt;
    cmd->CommenceMixScope.segCnt = scope->segCnt;
    cmd->CommenceMixScope.flags = scope->flags;
    cmd->CommenceMixScope.sink = scope->sink;

    for (afxUnit i = 0; i < scope->chanCnt; i++)
    {
        cmd->CommenceMixScope.chans[i] = scope->chans[i];
    }
    return cmdId;
}

_AMX afxCmdId AmxCmdConcludeMixScope(afxMixContext mix)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(ConcludeMixScope), sizeof(cmd->ConcludeMixScope), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    return cmdId;
}

_AMX afxCmdId AmxCmdReverb(afxMixContext mix, afxUnit voice, afxReal wetMix, afxReal roomSiz, afxReal width, afxReal damp, afxReal dryMix)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Reverb), sizeof(cmd->Reverb), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Reverb.voice = voice;
    cmd->Reverb.wetMix = wetMix;
    cmd->Reverb.roomSiz = roomSiz;
    cmd->Reverb.width = width;
    cmd->Reverb.damp = damp;
    cmd->Reverb.dryMix = dryMix;
    return cmdId;
}

_AMX afxCmdId AmxCmdPhaser(afxMixContext mix, afxUnit voice, afxReal floor, afxReal ceil, afxReal rate, afxReal feedback, afxReal depth, afxReal phase, afxUnit stageCnt)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Phaser), sizeof(cmd->Phaser), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Phaser.voice = voice;
    cmd->Phaser.floor = floor;
    cmd->Phaser.ceil = ceil;
    cmd->Phaser.rate = rate;
    cmd->Phaser.feedback = feedback;
    cmd->Phaser.depth = depth;
    cmd->Phaser.phase = phase;
    cmd->Phaser.stageCnt = stageCnt;
    return cmdId;
}

_AMX afxCmdId AmxCmdGainer(afxMixContext mix, afxUnit voice, afxReal gain, afxReal pan, afxMask invChan)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Gainer), sizeof(cmd->Gainer), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Gainer.voice = voice;
    cmd->Gainer.gain = gain;
    cmd->Gainer.pan = pan;
    cmd->Gainer.invChan = invChan;
    return cmdId;
}

_AMX afxCmdId AmxCmdFlanger(afxMixContext mix, afxUnit voice, afxReal amount, afxReal rate, afxReal amplitude, afxReal feedback, afxReal delay, afxReal phase, afxUnit flt)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Flanger), sizeof(cmd->Flanger), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Flanger.voice = voice;
    cmd->Flanger.amount = amount;
    cmd->Flanger.rate = rate;
    cmd->Flanger.amplitude = amplitude;
    cmd->Flanger.feedback = feedback;
    cmd->Flanger.delay = delay;
    cmd->Flanger.phase = phase;
    cmd->Flanger.flt = flt;
    return cmdId;
}

_AMX afxCmdId AmxCmdAnalog(afxMixContext mix, afxUnit voice, afxUnit type, afxUnit flt, afxReal cutoff, afxReal resonance, afxReal inertia, afxReal drive, afxUnit oversamp, afxReal start, afxReal end)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Analog), sizeof(cmd->Analog), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Analog.voice = voice;
    cmd->Analog.type = type;
    cmd->Analog.flt = flt;
    cmd->Analog.cutoff = cutoff;
    cmd->Analog.resonance = resonance;
    cmd->Analog.inertia = inertia;
    cmd->Analog.drive = drive;
    cmd->Analog.oversamp = oversamp;
    cmd->Analog.start = start;
    cmd->Analog.end = end;
    return cmdId;
}

_AMX afxCmdId AmxCmdCompressor(afxMixContext mix, afxUnit voice, afxReal threshold, afxReal ratio, afxReal attack, afxReal release, afxReal makeup)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Compressor), sizeof(cmd->Compressor), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Compressor.voice = voice;
    cmd->Compressor.threshold = threshold;
    cmd->Compressor.ratio = ratio;
    cmd->Compressor.attack = attack;
    cmd->Compressor.release = release;
    cmd->Compressor.makeup = makeup;
    return cmdId;
}

_AMX afxCmdId AmxCmdLineIn(afxMixContext mix, afxUnit voice, afxUnit line, afxMask chanMask, afxUnit latency, afxReal pan, afxReal vol)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(LineIn), sizeof(cmd->LineIn), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->LineIn.voice = voice;
    cmd->LineIn.line = line;
    cmd->LineIn.chanMask = chanMask;
    cmd->LineIn.latency = latency;
    cmd->LineIn.pan = pan;
    cmd->LineIn.vol = vol;
    return cmdId;
}

_AMX afxCmdId AmxCmdRemoteIn(afxMixContext mix, afxUnit voice, afxUnit src, afxMask chanMask, afxReal pan, afxReal vol)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(RemoteIn), sizeof(cmd->RemoteIn), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->RemoteIn.voice = voice;
    cmd->RemoteIn.src = src;
    cmd->RemoteIn.chanMask = chanMask;
    cmd->RemoteIn.pan = pan;
    cmd->RemoteIn.vol = vol;
    return cmdId;
}

_AMX afxCmdId AmxCmdSend(afxMixContext mix, afxUnit voice, afxBool muteSrc, afxReal amount, afxReal pan, afxUnit receiverSubmix)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(Send), sizeof(cmd->Send), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Send.voice = voice;
    cmd->Send.muteSrc = muteSrc;
    cmd->Send.amount = amount;
    cmd->Send.pan = pan;
    cmd->Send.receiverSubmix = receiverSubmix;
    return cmdId;
}

_AMX afxCmdId AmxCmdResampleAudio(afxMixContext mix, amxAudio src, amxAudio dst, amxAudioPeriod const* srcp, amxAudioPeriod const* dstp)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    amxAudioInfo srcd, dstd;
    AmxDescribeAudio(src, &srcd);
    AmxDescribeAudio(src, &dstd);

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)(dst->freq / src->freq);
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);

    AFX_ASSERT(dstd.sampCnt >= outputLen);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(ResampleAudio), sizeof(cmd->ResampleAudio), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->ResampleAudio.src = src;
    cmd->ResampleAudio.dst = dst;

    _AmxSanitizeAudioPeriod(src, srcp, &cmd->ResampleAudio.srcp);
    _AmxSanitizeAudioPeriod(dst, dstp, &cmd->ResampleAudio.dstp);

    return cmdId;
}

_AMX afxCmdId AmxCmdResampleBufferedAudio(afxMixContext mix, amxBufferedAudio const* src, amxBufferedAudio const* dst, amxAudioPeriod const* srcp, amxAudioPeriod const* dstp)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(ResampleBufferedAudio), sizeof(cmd->ResampleBufferedAudio), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->ResampleBufferedAudio.src = *src;
    cmd->ResampleBufferedAudio.dst = *dst;

    cmd->ResampleBufferedAudio.srcp = *srcp;
    cmd->ResampleBufferedAudio.dstp = *dstp;

    return cmdId;
}

_AMX afxCmdId AmxCmdFetchAudition(afxMixContext mix, afxUnit voice, afxUnit headIdx)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(FetchAudition), sizeof(cmd->FetchAudition), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->FetchAudition.voice = voice;
    cmd->FetchAudition.headIdx = headIdx;
    return cmdId;
}

_AMX afxCmdId AmxCmdDeclareTimelineBarrier(afxMixContext mix, afxReal time)
{
    afxError err = { 0 };
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MCTX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxContextState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMctxPushCmd(mix, _AMX_CMD_ID(FetchAudition), sizeof(cmd->FetchAudition), &cmdId, &cmd);
    AFX_ASSERT(cmd);

    return cmdId;
}
