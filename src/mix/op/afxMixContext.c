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
#include "../impl/amxImplementation.h"

_AMX afxError _AmxMixPushCmd(afxMixContext mix, afxUnit id, afxUnit siz, afxCmdId* cmdId, _amxCmd** ppCmd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(siz >= sizeof(_amxCmdHdr));

    _amxCmd* cmd = AfxRequestFromArena(&mix->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &mix->commands);
    AFX_ASSERT(ppCmd);
    *ppCmd = cmd;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxCmdId AmxCmdCommenceMixScope(afxMixContext mix, amxMixScope const* scope)
{
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(CommenceMixScope), sizeof(cmd->CommenceMixScope) + (scope->chanCnt * sizeof(cmd->CommenceMixScope.chans[0])), &cmdId, &cmd);
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

_AMX afxCmdId AmxCmdConcludeMixScope(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(ConcludeMixScope), sizeof(cmd->ConcludeMixScope), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    return cmdId;
}

_AMX afxCmdId AmxCmdReverb(afxMixContext mix, afxUnit voice, afxReal wetMix, afxReal roomSiz, afxReal width, afxReal damp, afxReal dryMix)
{
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Reverb), sizeof(cmd->Reverb), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Phaser), sizeof(cmd->Phaser), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Gainer), sizeof(cmd->Gainer), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->Gainer.voice = voice;
    cmd->Gainer.gain = gain;
    cmd->Gainer.pan = pan;
    cmd->Gainer.invChan = invChan;
    return cmdId;
}

_AMX afxCmdId AmxCmdFlanger(afxMixContext mix, afxUnit voice, afxReal amount, afxReal rate, afxReal amplitude, afxReal feedback, afxReal delay, afxReal phase, afxUnit flt)
{
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Flanger), sizeof(cmd->Flanger), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Analog), sizeof(cmd->Analog), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Compressor), sizeof(cmd->Compressor), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(LineIn), sizeof(cmd->LineIn), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(RemoteIn), sizeof(cmd->RemoteIn), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(Send), sizeof(cmd->Send), &cmdId, &cmd);
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
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

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
    _AmxMixPushCmd(mix, _AMX_CMD_ID(ResampleAudio), sizeof(cmd->ResampleAudio), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->ResampleAudio.src = src;
    cmd->ResampleAudio.dst = dst;

    _AmxSanitizeAudioPeriod(src, srcp, &cmd->ResampleAudio.srcp);
    _AmxSanitizeAudioPeriod(dst, dstp, &cmd->ResampleAudio.dstp);

    return cmdId;
}

_AMX afxCmdId AmxCmdFetchAudition(afxMixContext mix, afxUnit voice, afxUnit headIdx)
{
    afxError err = AFX_ERR_NONE;
    // mix must be a valid afxMixContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    // mix must be in the recording state.
    AFX_ASSERT(mix->state == amxMixState_RECORDING);

    afxCmdId cmdId;
    _amxCmd* cmd;
    _AmxMixPushCmd(mix, _AMX_CMD_ID(FetchAudition), sizeof(cmd->FetchAudition), &cmdId, &cmd);
    AFX_ASSERT(cmd);
    cmd->FetchAudition.voice = voice;
    cmd->FetchAudition.headIdx = headIdx;
    return cmdId;
}

////////////////////////////////////////////////////////////////////////////////

_AMX amxMixState AfxGetMixerState(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->state;
}

_AMX afxUnit AfxGetMixerPort(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->portId;
}

_AMX afxUnit AfxGetMixerPool(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return mix->poolIdx;
}

_AMX afxError AmxRollMixContext(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    // mctx must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxMixSystem msys = AfxGetProvider(mix);

    afxMixBridge mexu;
    AFX_ASSERT_RANGE(msys->bridgeCnt, /*ctrl->exuIdx*/0, 1);
    if (!AmxGetMixBridges(msys, /*ctrl->exuIdx*/0, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    if (_AmxMexuRollMixers(mexu, 0, 1, 1, 1, &mix))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxMixDtorCb(afxMixContext mix)
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

_AMX afxError _AmxMixCtorCb(afxMixContext mix, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixConfig const* cfg = AFX_CAST(afxMixConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    //afxUnit chanCnt = AFX_MAX(1, cfg->chanCnt);
    //afxUnit freq = AFX_MAX(1, cfg->freq);
    //mix->bps = cfg->bps ? cfg->bps : 32;
    //mix->latency = AFX_MAX(cfg->latency, 4096);
    //mix->freq = freq;
    //mix->chanCnt = chanCnt;

    mix->disabled = FALSE;

    mix->muted = FALSE;
    mix->routing = NIL;
    //mix->chanCnt = mix->chanCnt;
    //mix->latency = mix->latency;
    //mix->freq = mix->freq;

    mix->routing = NIL;

    AfxMakeArena(&mix->cmdArena, NIL, AfxHere());
    AfxDeployChain(&mix->commands, mix);

    mix->resetCb = _AmxMixResetCb;
    mix->endCb = _AmxMixEndCb;
    mix->state = amxMixState_RECORDING;

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

_AMX afxClassConfig const _AMX_MIX_CLASS_CONFIG =
{
    .fcc = afxFcc_MIX,
    .name = "MixContext",
    .desc = "Tracked Mixing Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixContext)),
    .ctor = (void*)_AmxMixCtorCb,
    .dtor = (void*)_AmxMixDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireMixContext(afxMixSystem msys, afxMixConfig const* cfg, afxMixContext* mixer)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(mixer);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AmxGetMixContextClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MIX);
    
    afxMixContext mix;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&mix, (void const*[]) { msys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    AFX_ASSERT(mixer);
    *mixer = mix;
    return err;
}
