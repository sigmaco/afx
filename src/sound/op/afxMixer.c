/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ASX_SOUND_C
#define _ASX_SOUND_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_SYSTEM_C
#define _ASX_SINK_C
#define _ASX_MIXER_C
#define _ASX_TRACK_C
#define _ASX_AUDIO_C
#define _ASX_WAVEFORM_C
#include "../impl/asxImplementation.h"

_ASX asxCmd* _AsxMixPushCmd(afxMixer mix, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    asxCmd* cmd = AfxRequestArenaUnit(&mix->cmdArena, siz);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &mix->commands);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_ASX afxCmdId AsxCmdCommenceMixScope(afxMixer mix, asxMixScope const* scope)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(CommenceMixScope), sizeof(cmd->CommenceMixScope) + (scope->chanCnt * sizeof(cmd->CommenceMixScope.chans[0])), &cmdId);
    AFX_ASSERT(cmd);
    
    AFX_ASSERT(scope);
    cmd->CommenceMixScope.scope = *scope;

    if (!scope->freq)
    {
        if (scope->sink)
            cmd->CommenceMixScope.scope.freq = scope->sink->freq;
        else
            cmd->CommenceMixScope.scope.freq = 48000;
    }

    AFX_ASSERT(scope->chans);

    for (afxUnit i = 0; i < scope->chanCnt; i++)
    {
        cmd->CommenceMixScope.chans[i] = scope->chans[i];
    }
    return cmdId;
}

_ASX afxCmdId AsxCmdConcludeMixScope(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(ConcludeMixScope), sizeof(cmd->ConcludeMixScope), &cmdId);
    AFX_ASSERT(cmd);
    return cmdId;
}

_ASX afxCmdId AsxCmdReverb(afxMixer mix, afxReal wetMix, afxReal roomSiz, afxReal width, afxReal damp, afxReal dryMix)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Reverb), sizeof(cmd->Reverb), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Reverb.wetMix = wetMix;
    cmd->Reverb.roomSiz = roomSiz;
    cmd->Reverb.width = width;
    cmd->Reverb.damp = damp;
    cmd->Reverb.dryMix = dryMix;
    return cmdId;
}

_ASX afxCmdId AsxCmdPhaser(afxMixer mix, afxReal floor, afxReal ceil, afxReal rate, afxReal feedback, afxReal depth, afxReal phase, afxUnit stageCnt)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Phaser), sizeof(cmd->Phaser), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Phaser.floor = floor;
    cmd->Phaser.ceil = ceil;
    cmd->Phaser.rate = rate;
    cmd->Phaser.feedback = feedback;
    cmd->Phaser.depth = depth;
    cmd->Phaser.phase = phase;
    cmd->Phaser.stageCnt = stageCnt;
    return cmdId;
}

_ASX afxCmdId AsxCmdGainer(afxMixer mix, afxReal gain, afxReal pan, afxMask invChan)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Gainer), sizeof(cmd->Gainer), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Gainer.gain = gain;
    cmd->Gainer.pan = pan;
    cmd->Gainer.invChan = invChan;
    return cmdId;
}

_ASX afxCmdId AsxCmdFlanger(afxMixer mix, afxReal amount, afxReal rate, afxReal amplitude, afxReal feedback, afxReal delay, afxReal phase, afxUnit flt)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Flanger), sizeof(cmd->Flanger), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Flanger.amount = amount;
    cmd->Flanger.rate = rate;
    cmd->Flanger.amplitude = amplitude;
    cmd->Flanger.feedback = feedback;
    cmd->Flanger.delay = delay;
    cmd->Flanger.phase = phase;
    cmd->Flanger.flt = flt;
    return cmdId;
}

_ASX afxCmdId AsxCmdAnalog(afxMixer mix, afxUnit type, afxUnit flt, afxReal cutoff, afxReal resonance, afxReal inertia, afxReal drive, afxUnit oversamp, afxReal start, afxReal end)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Analog), sizeof(cmd->Analog), &cmdId);
    AFX_ASSERT(cmd);
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

_ASX afxCmdId AsxCmdCompressor(afxMixer mix, afxReal threshold, afxReal ratio, afxReal attack, afxReal release, afxReal makeup)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Compressor), sizeof(cmd->Compressor), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Compressor.threshold = threshold;
    cmd->Compressor.ratio = ratio;
    cmd->Compressor.attack = attack;
    cmd->Compressor.release = release;
    cmd->Compressor.makeup = makeup;
    return cmdId;
}

_ASX afxCmdId AsxCmdLineIn(afxMixer mix, afxUnit line, afxMask chanMask, afxUnit latency, afxReal pan, afxReal vol)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(LineIn), sizeof(cmd->LineIn), &cmdId);
    AFX_ASSERT(cmd);
    cmd->LineIn.line = line;
    cmd->LineIn.chanMask = chanMask;
    cmd->LineIn.latency = latency;
    cmd->LineIn.pan = pan;
    cmd->LineIn.vol = vol;
    return cmdId;
}

_ASX afxCmdId AsxCmdRemoteIn(afxMixer mix, afxUnit src, afxMask chanMask, afxReal pan, afxReal vol)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(RemoteIn), sizeof(cmd->RemoteIn), &cmdId);
    AFX_ASSERT(cmd);
    cmd->RemoteIn.src = src;
    cmd->RemoteIn.chanMask = chanMask;
    cmd->RemoteIn.pan = pan;
    cmd->RemoteIn.vol = vol;
    return cmdId;
}

_ASX afxCmdId AsxCmdSend(afxMixer mix, afxBool muteSrc, afxReal amount, afxReal pan, afxUnit receiverSubmix)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(Send), sizeof(cmd->Send), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Send.muteSrc = muteSrc;
    cmd->Send.amount = amount;
    cmd->Send.pan = pan;
    cmd->Send.receiverSubmix = receiverSubmix;
    return cmdId;
}

_ASX afxCmdId AsxCmdResampleAudio(afxMixer mix, afxAudio src, afxAudio dst, afxWaveInterval const* srci, afxWaveInterval const* dsti)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxAudioInfo srcd, dstd;
    AfxDescribeAudio(src, &srcd);
    AfxDescribeAudio(src, &dstd);

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)(dst->freq / src->freq);
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);

    AFX_ASSERT(dstd.sampCnt >= outputLen);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(ResampleAudio), sizeof(cmd->ResampleAudio), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResampleAudio.src = src;
    cmd->ResampleAudio.dst = dst;

    _AsxSanitizeWaveInterval(src, srci, &cmd->ResampleAudio.srci);
    _AsxSanitizeWaveInterval(dst, dsti, &cmd->ResampleAudio.dsti);

    return cmdId;
}

_ASX afxCmdId AsxCmdFetchAudition(afxMixer mix, afxUnit headIdx)
{
    afxError err = AFX_ERR_NONE;
    /// mix must be a valid afxMixer handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    /// mix must be in the recording state.
    AFX_ASSERT(mix->state == asxMixState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxMixPushCmd(mix, ASX_GET_STD_CMD_ID(FetchAudition), sizeof(cmd->FetchAudition), &cmdId);
    AFX_ASSERT(cmd);
    cmd->FetchAudition.headIdx = headIdx;
    return cmdId;
}

////////////////////////////////////////////////////////////////////////////////

_ASX asxMixState AfxGetMixerState(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->state;
}

_ASX afxUnit AfxGetMixerPort(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->portId;
}

_ASX afxUnit AfxGetMixerPool(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->poolIdx;
}

_ASX afxError _AsxMixDtorCb(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    while (AfxLoadAtom32(&mix->submCnt))
    {
        AfxYield();
    }

    AfxDismantleArena(&mix->cmdArena);

    return err;
}

_ASX afxError _AsxMixCtorCb(afxMixer mix, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxSoundSystem ssys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    //afxMixConfig const* cfg = ((afxMixConfig const *)args[1]) + invokeNo;
    //AFX_ASSERT(cfg);

    //afxUnit chanCnt = AfxMax(1, cfg->chanCnt);
    //afxUnit freq = AfxMax(1, cfg->freq);
    //mix->bps = cfg->bps ? cfg->bps : 32;
    //mix->latency = AfxMax(cfg->latency, 4096);
    //mix->freq = freq;
    //mix->chanCnt = chanCnt;

    mix->disabled = FALSE;

    mix->muted = FALSE;
    mix->routing = NIL;
    //mix->chanCnt = mix->chanCnt;
    //mix->latency = mix->latency;
    //mix->freq = mix->freq;

    mix->routing = NIL;

    AfxDeployArena(&mix->cmdArena, NIL, AfxHere());
    AfxDeployChain(&mix->commands, mix);

    mix->resetCb = _AsxMixResetCb;
    mix->endCb = _AsxMixEndCb;
    mix->state = asxMixState_RECORDING;

    {
        mix->motor.flags = 1;// afxCapstanFlag_ACTIVE;
        mix->motor.timing.currClock = 0;// cfg->currClock; --- set by roll
        mix->motor.dtLocalClockPending = 0.0;
        mix->motor.localClock = 0.0;
        mix->motor.speed = 1.0;
        mix->motor.localDur = 0;// cfg->localDur; --- set by roll
        mix->motor.currIterIdx = 0;
        mix->motor.iterCnt = 0;// cfg->iterCnt; --- set by roll
        mix->motor.timing.killClock = 0;
        mix->motor.currWeight = 1;
        mix->motor.timing.easeInStartClock = 0;
        mix->motor.timing.easeInEndClock = 0;
        mix->motor.easeInValues = (afxUnit)-65536;
        mix->motor.timing.easeOutStartClock = 0;
        mix->motor.timing.easeOutEndClock = 0;
        mix->motor.easeOutValues = (afxUnit)0xFFFF;
    }

    return err;
}

_ASX afxClassConfig const _ASX_MIX_CLASS_CONFIG =
{
    .fcc = afxFcc_MIX,
    .name = "Mixer",
    .desc = "Tracked Audio Mixer",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixer)),
    .ctor = (void*)_AsxMixCtorCb,
    .dtor = (void*)_AsxMixDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireMixer(afxSoundSystem ssys, afxUnit exuIdx, afxUnit poolIdx, afxMixer* mixer)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(mixer);

    afxClass* cls = (afxClass*)AsxGetMixerClass(ssys);
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    
    afxMixer mix;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&mix, (void const*[]) { ssys, &exuIdx, &poolIdx }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(mixer);
    *mixer = mix;
    return err;
}
