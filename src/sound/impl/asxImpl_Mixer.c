/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                     Q W A D R O   S O U N D   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _ASX_SOUND_C
//#define _ASX_SOUND_BRIDGE_C
//#define _ASX_SOUND_SYSTEM_C
//#define _ASX_SOUND_QUEUE_C
#define _ASX_BUFFER_C
#define _ASX_WAVEFORM_C
#define _ASX_VOICE_C
#define _ASX_MIXER_C
#define _ASX_SINK_C
#include "asxImplementation.h"

_ASX afxError _AsxSpuLoadAudio(asxSpu* spu, afxAudio src, afxAudio dst, afxUnit chIdx, afxUnit baseSamp, afxUnit sampCnt, asxMixFactor fac, afxReal constant)
{
    afxError err = NIL;
    afxReal32* out = &dst->samples32f[chIdx * dst->sampCnt + baseSamp];
    afxReal32 const* in = &src->samples32f[chIdx * src->sampCnt + baseSamp];

    switch (fac)
    {
    case asxMixFactor_ZERO:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 0.f;
        }
        break;
    }
    // 1
    case asxMixFactor_ONE:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f;
        }
        break;
    }
    // As0
    case asxMixFactor_SRC:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = in[i];
        }
        break;
    }
    // 1-As0
    case asxMixFactor_ONE_MINUS_SRC:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f - in[i];
        }
        break;
    }
    // Ad
    case asxMixFactor_DST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = out[i];
        }
        break;
    }
    // 1-Ad
    case asxMixFactor_ONE_MINUS_DST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f - out[i];
        }
        break;
    }
    // Ac
    case asxMixFactor_CONST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = constant;
        }
        break;
    }
    // 1-Ac
    case asxMixFactor_ONE_MINUS_CONST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f - constant;
        }
        break;
    }
    // 1
    case asxMixFactor_SRC_SATURATE:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f;
        }
        break;
    }
    // As1
    case asxMixFactor_SRC1:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = in[i + 1];
        }
        break;
    }
    // 1-As1
    case asxMixFactor_ONE_MINUS_SRC1:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1 - in[i + 1];
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ASX afxError _AsxSpu_StoreAudio(asxSpu* spu, afxAudio src, afxAudio dst, afxUnit chIdx, afxUnit baseSamp, afxUnit sampCnt, asxMixOp op)
{
    afxError err = AFX_ERR_NONE;
    afxReal32* out = &dst->samples32f[chIdx * dst->sampCnt + baseSamp];
    afxReal32 const* in = &src->samples32f[chIdx * src->sampCnt + baseSamp];

    switch (op)
    {
    // A = As0 × Sf + Ad × Df
    case asxMixOp_ADD:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] += in[i];
        }
        break;
    }
    // A = As0 × Sf - Ad × Df
    case asxMixOp_SUB:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] -= in[i];
        }
        break;
    }
    // A = Ad × Df - As0 × Sf
    case asxMixOp_REV_SUB:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] += in[i];
        }
        break;
    }
    // A = min(As0, Ad)
    case asxMixOp_MIN:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = AfxMin(out[i], in[i]);
        }
        break;
    }
    // A = max(As0, Ad)
    case asxMixOp_MAX:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = AfxMax(out[i], in[i]);
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ASX afxError _AsxSpuCmd_CommenceMixScope(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    afxUnit sinkChanCnt = cmd->CommenceMixScope.scope.chanCnt;

    AFX_ASSERT(ARRAY_SIZE(spu->sinkChans) >= sinkChanCnt);
    sinkChanCnt = AfxMin(sinkChanCnt, ARRAY_SIZE(spu->sinkChans));
    
    spu->mixFreq = cmd->CommenceMixScope.scope.freq;
    spu->sinkChanCnt = sinkChanCnt;
    spu->sinkBaseChan = cmd->CommenceMixScope.scope.baseChan;
    spu->sinkBaseSamp = cmd->CommenceMixScope.scope.baseSamp;
    spu->sinkSampCnt = cmd->CommenceMixScope.scope.sampCnt;
    spu->sinkBuf = cmd->CommenceMixScope.scope.sink;

    for (afxUnit i = 0; i < sinkChanCnt; i++)
    {
        spu->sinkChans[i] = cmd->CommenceMixScope.chans[i];
    }
    return err;
}

_ASX afxError _AsxSpuCmd_ConcludeMixScope(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    if (AfxRequestSinkBuffer(spu->sinkBuf, 0, &spu->sinkBufIdx))
        return err;

    afxAudio out = spu->sinkBuf->buffers[spu->sinkBufIdx];
    afxUnit sinkSampCnt = out->sampCnt;
    afxUnit sinkBaseSamp = 0;

    // Calculate the number of frames to process
    afxInt frames = (sinkSampCnt + spu->samplesPerFrame - 1) / spu->samplesPerFrame; // Round up

    for (afxInt frame = 0; frame < frames; frame++)
    {
        int frameStart = frame * spu->samplesPerFrame;
        int frameEnd = (frame + 1) * spu->samplesPerFrame;

        if (frameEnd > sinkSampCnt)
            frameEnd = sinkSampCnt;  // Handle the last chunk that may not be full

        int frameSiz = frameEnd - frameStart;

        for (afxUnit chIdx = 0; chIdx < spu->sinkChanCnt; chIdx++)
        {
            asxMixSink const* msc = &spu->sinkChans[chIdx];

            switch (msc->loadOp)
            {
            case asxLoadOp_CLEAR:
            {
                //_AsxGenerateSquareWave(spu->a, chIdx, 1, 440.0, 60);
                afxWaveInterval op = { 0 };
                op.baseChan = spu->sinkBaseChan + chIdx;
                op.chanCnt = 1;
                op.sampCnt = frameSiz;
                _AsxFillWave(spu->a, &op, msc->clearAmpl, spu->mixFreq, 1);
                break;
            }
            case asxLoadOp_LOAD:
            {
                afxWaveResample op = { 0 };
                op.dstFreq = spu->mixFreq;
                op.dst.baseChan = chIdx;
                op.dst.chanCnt = 1;
                op.dst.sampCnt = frameSiz;
                op.srcFreq = spu->sinkBuf->freq;
                op.src.baseChan = spu->sinkBaseChan + chIdx;
                op.src.baseSamp = sinkBaseSamp;
                op.src.sampCnt = sinkSampCnt;
                op.src.chanCnt = 1;
                _AsxSpu_ResampleWave(spu, out, spu->a, &op);
                break;
            }
            case asxLoadOp_DONT_CARE:
            {
                break;
            }
            default: AfxThrowError(); break;
            }

            switch (msc->storeOp)
            {
            case asxStoreOp_STORE:
            {
                afxWaveResample op = { 0 };
                op.dstFreq = spu->sinkBuf->freq;
                op.src.chanCnt = 1;
                op.src.baseChan = chIdx;
                op.src.sampCnt = frameSiz;
                op.srcFreq = spu->mixFreq;
                op.dst.baseChan = spu->sinkBaseChan + chIdx;
                op.dst.baseSamp = sinkBaseSamp;
                op.dst.sampCnt = sinkSampCnt;
                op.dst.chanCnt = 1;
                _AsxSpu_ResampleWave(spu, spu->a, out, &op);
                break;
            }
            case asxStoreOp_MERGE:
            {

                break;
            }
            case asxStoreOp_DISCARD:
            {
                break;
            }
            case asxStoreOp_DONT_CARE:
            {
                break;
            }
            default: AfxThrowError(); break;
            }
        }
    }

    AfxPushInterlockedQueue(&spu->sinkBuf->readyBuffers, &spu->sinkBufIdx);

    spu->sinkBuf = NIL;
    spu->sinkBufIdx = AFX_INVALID_INDEX;
    spu->sinkChanCnt = 0;
    spu->sinkSampCnt = 0;

    return err;
}

_ASX afxError _AsxSpuCmd_UpdateAudio(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;
    
    if (_AsxUpdateAudio(cmd->UpdateAudio.buf, &cmd->UpdateAudio.op, cmd->UpdateAudio.src))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxSpuCmd_ResampleWave(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    //if (_AsxSpu_ResampleWave(spu, cmd->ResampleAudio.src, cmd->ResampleAudio.dst, &cmd->ResampleAudio.srci, &cmd->ResampleAudio.dsti))
        AfxThrowError();

    return err;
}

_ASX afxError _AsxSpuCmd_TransposeAudio(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    if (_AsxTransposeAudio(cmd->TransposeAudio.src, cmd->TransposeAudio.dst, &cmd->TransposeAudio.op))
        AfxThrowError();

    return err;
}

_ASX void _AsxSpuCmd_FetchAudition(asxSpu* spu, asxCmd const* cmd)
{
    afxError err = NIL;
    //cmd->Audit();
    afxSoundSystem ssys = spu->ssys;
    afxMixer mix = spu->mix;
    
    afxSound snd;
    afxUnit i = 0;
    while (AfxEnumerateObjects(AsxGetSoundClass(ssys), i++, 1, (afxObject*)&snd))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

        
        //AfxResampleAudio(snd->audio, mix->latency, mix->chanCnt, mix->freq, sizeof(spu->bufA), spu->bufA);

    }
}

_ASX void _AsxSpuCmd_Reverb(asxSpu* spu, asxCmd const* cmd)
{
    //_SpuReverb(spu, cmd->Reverb.wetMix, cmd->Reverb.roomSiz, cmd->Reverb.width, cmd->Reverb.damp, cmd->Reverb.dryMix);
}

_ASX asxCmdList _ASX_SPU_CMD_VMT =
{
    .Reverb = (void*)_AsxSpuCmd_Reverb,
    .UpdateAudio = (void*)_AsxSpuCmd_UpdateAudio,
    .FetchAudition = (void*)_AsxSpuCmd_FetchAudition,
    .TransposeAudio = (void*)_AsxSpuCmd_TransposeAudio,
    .CommenceMixScope = (void*)_AsxSpuCmd_CommenceMixScope,
    .ConcludeMixScope = (void*)_AsxSpuCmd_ConcludeMixScope,
};

_ASXINL void _MixModulusLocalClock(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    afxReal t = mix->motor.localClock / mix->motor.localDur;
    afxInt currIterIdx = mix->motor.currIterIdx;
    afxInt i = currIterIdx + (afxInt)t;

    if (mix->motor.localClock < 0.0)
        --i;

    afxInt iterCnt = mix->motor.iterCnt;

    if (iterCnt)
    {
        if (i >= 0)
        {
            if (i >= iterCnt)
                i = iterCnt - 1;
        }
        else i = 0;
    }

    afxInt j = i - currIterIdx;

    if (j)
    {
        mix->motor.currIterIdx = j + currIterIdx;
        mix->motor.localClock = mix->motor.localClock - (afxReal64)j * mix->motor.localDur;
    }
}

_ASXINL afxReal _MixLocalClock(afxMixer mix, afxReal* clamped)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    afxInt currIterIdx = mix->motor.currIterIdx;
    afxInt iterCnt = mix->motor.iterCnt;

    if (mix->motor.dtLocalClockPending != 0.0)
    {
        // process pending local clock

        while (1)
        {
            afxReal t = mix->motor.speed * mix->motor.dtLocalClockPending + mix->motor.localClock;
            mix->motor.localClock = t;

            if (1) // [!]
            {
                if (iterCnt && (currIterIdx <= 0))
                    break; // avoid modulus
            }
            else
            {
                if (t < mix->motor.localDur)
                    break; // avoid modulus

                if (iterCnt)
                {
                    if (currIterIdx >= (iterCnt - 1))
                        break; // avoid modulus
                }
            }
            _MixModulusLocalClock(mix);
            break;
        }
        mix->motor.dtLocalClockPending = 0.0; // reset pending local clock
    }

    afxReal localClock = mix->motor.localClock;

    if (clamped)
    {
        if (localClock < 0.0)
            *clamped = 0.0;
        else if (localClock > mix->motor.localDur)
            *clamped = mix->motor.localDur;
        else
            *clamped = localClock;
    }
    return localClock;
}

_ASXINL void AfxQueryMixState(afxMixer mix, afxCapstanState* state)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxReal localClockRaw, localClockClamped;
    localClockRaw = _MixLocalClock(mix, &localClockClamped);
    afxReal speed = mix->motor.speed;
    afxInt iterCnt = state->iterCnt;
    afxInt currIterIdx = mix->motor.currIterIdx;
    afxReal localDur = mix->motor.localDur;

    AFX_ASSERT(state);
    state->active = 1;// (mix->motor.flags & afxCapstanFlag_ACTIVE) == afxCapstanFlag_ACTIVE;
    state->speed = speed;
    state->iterCnt = iterCnt;
    state->currIterIdx = currIterIdx;
    state->currWeight = mix->motor.currWeight;
    state->effectiveWeight = state->active ? /*curve ease multiplier*/1.f * mix->motor.currWeight : 0.f;
    state->localClockRaw = localClockRaw;
    state->localClockClamped = localClockClamped;
    state->localDur = localDur;

    // determine iteration state
#if 0
    if (mix->motor.flags & afxCapstanFlag_FORCE_CLAMPLED_LOOPS)
    {
        state->overflow = FALSE;
        state->underflow = FALSE;
    }
    else
#endif
    if (iterCnt)
    {
        state->underflow = currIterIdx > 0;
        state->overflow = currIterIdx < iterCnt - 1;
    }
    else
    {
        state->overflow = TRUE;
        state->underflow = TRUE;
    }

    // determine duration

    state->dur = 1.9140881e38;
    state->durLeft = 1.9140881e38;

    if (iterCnt)
    {
        if (speed > 0.0)
        {
            state->dur = (localDur / speed) * iterCnt;

            if (currIterIdx < iterCnt)
                state->durLeft = (afxReal)(iterCnt - currIterIdx - 1) * (1.0 / speed * localDur) + (localDur - localClockRaw) * (1.0 / speed);
            else
                state->durLeft = 0.0;
        }
        else if (speed < 0.0)
        {
            state->dur = -(localDur / speed) * iterCnt;

            if (mix->motor.currIterIdx < 0)
                state->durLeft = 0.0;
            else
                state->durLeft = -(1.0 / speed * localDur) * (afxReal)currIterIdx - 1.0 / speed * localClockRaw;
        }
    }
}

void _SetUpSampleContext(afxSampleContext* ctx, afxMixer mix)
{
    afxCapstanState ms;
    AfxQueryMixState(mix, &ms);
    ctx->localClock = ms.localClockClamped;
    ctx->localDur = ms.localDur;
    ctx->frameIdx = 0;//Binding->ca.Binding->ID.Animation->dur / ctx->LocalClock;
    ctx->underflowLoop = ms.underflow;
    ctx->overflowLoop = ms.overflow;
}

_ASX afxError _AsxSpuRollMixers(asxSpu* spu, afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    if (mix->state == asxMixState_PENDING)
    {
        AFX_ASSERT(mix->state == asxMixState_PENDING);

        // step mixer
        afxReal64 curTime, lastTime;
        AfxGetThreadTime(&curTime, &lastTime);
        afxReal dt = curTime - mix->motor.localClock;
        mix->motor.localClock = curTime;
        mix->motor.dtLocalClockPending += dt;

        afxSampleContext sampCtx;
        _SetUpSampleContext(&sampCtx, mix);
        spu->sampCtx = sampCtx;

        asxCmd* cmdHdr;
        AfxChainForEveryLinkageB2F(&mix->commands, asxCmd, hdr.script, cmdHdr)
        {
            if (cmdHdr->hdr.id == NIL)
                break;

            if (mix->state != asxMixState_PENDING)
            {
                AfxThrowError();
                break;
            }
            _ASX_SPU_CMD_VMT.f[cmdHdr->hdr.id](spu, cmdHdr);
        }

#if 0
        if (!err)
        {
            mix->state = asxMixState_EXECUTABLE;
        }

        if (err || mix->m.disposable)
        {
            AFX_ASSERT(mix->m.portId == spu->portId);
            mix->m.state = avxCmdbState_INVALID;
            afxSoundQueue sque = AfxGetProvider(mix);
            AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

            afxUnit poolIdx = mix->m.poolIdx;

            AfxEnterSlockExclusive(&sque->m.cmdbReqLock);

            if (AfxPushQueue(&sque->m.cmdbRecycQue, &mix))
            {
                AfxDisposeObjects(1, (void**)&mix);
            }

            AfxExitSlockExclusive(&sque->m.cmdbReqLock);
        }
#endif
    }
    //AfxDecAtom32(&mix->submCnt);
    //AfxDisposeObjects(1, &mix);
    return err;
}

_ASX afxError _AsxMixResetCb(afxMixer mix, afxBool freeMem, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(mix->state != asxMixState_PENDING);
    mix->disposable = !permanent;

    AfxDeployChain(&mix->commands, mix);
    AfxExhaustArena(&mix->cmdArena);

    return err;
}

_ASX afxError _AsxMixEndCb(afxMixer mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return err;
}
