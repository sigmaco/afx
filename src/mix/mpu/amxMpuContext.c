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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#define _AMX_VOICE_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_SINK_C
#include "../amxIcd.h"

void resample_stream(ResampleState* state, float* out, int out_count)
{
    int out_written = 0;

    while (out_written < out_count && state->in_pos + 1 < state->in_len)
    {
        int i = state->in_pos * 2; // stereo

        float l0 = state->input_buf[i];
        float r0 = state->input_buf[i + 1];
        float l1 = state->input_buf[i + 2];
        float r1 = state->input_buf[i + 3];

        float frac = (float)(state->frac_pos);

        float l = l0 + (l1 - l0) * frac;
        float r = r0 + (r1 - r0) * frac;

        out[out_written * 2] = l;
        out[out_written * 2 + 1] = r;

        out_written++;
        state->frac_pos += 1.0 / state->ratio;

        while (state->frac_pos >= 1.0) {
            state->in_pos++;
            state->frac_pos -= 1.0;
        }
    }
}

void fill_sink_buffer(float* out, int out_frames, ResampleState* state)
{
    int written = 0;
    while (written < out_frames)
    {
        if (state->in_pos + 1 >= state->in_len)
        {
            // No more input, must wait for next Doom buffer
            state->needs_more_input = TRUE;
            break;
        }

        int i = state->in_pos * 2;
        float l0 = state->input_buf[i];
        float r0 = state->input_buf[i + 1];
        float l1 = state->input_buf[i + 2];
        float r1 = state->input_buf[i + 3];

        float frac = (float)(state->frac_pos);

        float l = l0 + (l1 - l0) * frac;
        float r = r0 + (r1 - r0) * frac;

        out[written * 2 + 0] = l;
        out[written * 2 + 1] = r;
        written++;

        state->frac_pos += 1.0 / state->ratio;

        while (state->frac_pos >= 1.0) {
            state->in_pos++;
            state->frac_pos -= 1.0;
        }
    }

    // Pad remainder with silence if underrun
    while (written < out_frames)
    {
        out[written * 2 + 0] = 0.0f;
        out[written * 2 + 1] = 0.0f;
        written++;
    }
}

// Doom push update (on 512-sample cycle)
void doom_audio_update(ResampleState* state, int16_t const* doom_buf)
{
    //int16_t doom_buf[512 * 2];
    //doom_render(doom_buf); // get next 512 interleaved stereo samples

    // Convert to float
    afxUnit soCnt;
    //resample_44100_to_48000_interleaved2(doom_buf, SAMPLECOUNT, 512, state->input_buf, &soCnt, SAMPLERATE, 48000);
    afxError err;
    AfxThrowError();
    state->in_len = 512;
    state->in_pos = 0;
    // Don't reset frac_pos
    state->needs_more_input = FALSE;
}

_AMX afxError _AmxSpuLoadAudio(amxMpu* mpu, amxAudio src, amxAudio dst, afxUnit chIdx, afxUnit baseSamp, afxUnit sampCnt, amxMixFactor fac, afxReal constant)
{
    afxError err = { 0 };
    afxReal32* out = &dst->buf->storage[0].hostedAlloc.f32[chIdx * dst->sampCnt + baseSamp];
    afxReal32 const* in = &src->buf->storage[0].hostedAlloc.f32[chIdx * src->sampCnt + baseSamp];

    switch (fac)
    {
    case amxMixFactor_ZERO:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 0.f;
        }
        break;
    }
    // 1
    case amxMixFactor_ONE:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f;
        }
        break;
    }
    // As0
    case amxMixFactor_SRC:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = in[i];
        }
        break;
    }
    // 1-As0
    case amxMixFactor_ONE_MINUS_SRC:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f - in[i];
        }
        break;
    }
    // Ad
    case amxMixFactor_DST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = out[i];
        }
        break;
    }
    // 1-Ad
    case amxMixFactor_ONE_MINUS_DST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f - out[i];
        }
        break;
    }
    // Ac
    case amxMixFactor_CONST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = constant;
        }
        break;
    }
    // 1-Ac
    case amxMixFactor_ONE_MINUS_CONST:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f - constant;
        }
        break;
    }
    // 1
    case amxMixFactor_SRC_SATURATE:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = 1.f;
        }
        break;
    }
    // As1
    case amxMixFactor_SRC1:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = in[i + 1];
        }
        break;
    }
    // 1-As1
    case amxMixFactor_ONE_MINUS_SRC1:
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

_AMX afxError _AmxSpu_StoreAudio(amxMpu* mpu, amxAudio src, amxAudio dst, afxUnit chIdx, afxUnit baseSamp, afxUnit sampCnt, amxMixOp op)
{
    afxError err = { 0 };
    afxReal32* out = &dst->buf->storage[0].hostedAlloc.f32[chIdx * dst->sampCnt + baseSamp];
    afxReal32 const* in = &src->buf->storage[0].hostedAlloc.f32[chIdx * src->sampCnt + baseSamp];

    switch (op)
    {
    // A = As0 × Sf + Ad × Df
    case amxMixOp_ADD:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] += in[i];
        }
        break;
    }
    // A = As0 × Sf - Ad × Df
    case amxMixOp_SUB:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] -= in[i];
        }
        break;
    }
    // A = Ad × Df - As0 × Sf
    case amxMixOp_REV_SUB:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] += in[i];
        }
        break;
    }
    // A = min(As0, Ad)
    case amxMixOp_MIN:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = AFX_MIN(out[i], in[i]);
        }
        break;
    }
    // A = max(As0, Ad)
    case amxMixOp_MAX:
    {
        for (afxUnit i = 0; i < sampCnt; i++)
        {
            out[i] = AFX_MAX(out[i], in[i]);
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxSpuCmd_CommenceMixScope(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = { 0 };

    afxUnit sinkChanCnt = cmd->CommenceMixScope.chanCnt;

    AFX_ASSERT(ARRAY_SIZE(mpu->sinkChans) >= sinkChanCnt);
    sinkChanCnt = AFX_MIN(sinkChanCnt, ARRAY_SIZE(mpu->sinkChans));
    
    mpu->sinkChanCnt = sinkChanCnt;
    mpu->sinkBaseChan = cmd->CommenceMixScope.baseChan;
    mpu->sinkBaseSamp = cmd->CommenceMixScope.baseSamp;
    mpu->sinkBaseSeg = cmd->CommenceMixScope.baseSeg;
    mpu->sinkSegCnt = cmd->CommenceMixScope.segCnt;
    mpu->sinkSampCnt = cmd->CommenceMixScope.sampCnt;
    mpu->sinkBuf = cmd->CommenceMixScope.sink;
    mpu->mixFreq = mpu->sinkBuf->freq;

    for (afxUnit i = 0; i < sinkChanCnt; i++)
    {
        mpu->sinkChans[i] = cmd->CommenceMixScope.chans[i];
    }

    afxUnit queuedFrameCnt = mpu->queuedFrameCnt;

    for (afxUnit chIdx = 0; chIdx < mpu->sinkChanCnt; chIdx++)
    {
        amxMixTarget const* msc = &mpu->sinkChans[chIdx];

        switch (msc->loadOp)
        {
        case amxLoadOp_CLEAR:
        {
            //_AmxGenerateSquareWave(mpu->a, chIdx, 1, 440.0, 60);
            amxAudioPeriod op = { 0 };
            op.baseChan = mpu->sinkBaseChan;
            op.baseSamp = mpu->sinkBaseSamp;
            op.baseSeg = mpu->sinkBaseSeg;
            op.sampCnt = queuedFrameCnt;
            op.chanCnt = 1;
            op.segCnt = 1;
            _AmxFillAudio(mpu->a, &op, msc->clearAmpl, mpu->mixFreq, 1);
            break;
        }
        case amxLoadOp_LOAD:
        {
            amxAudioInterference op = { 0 };
            op.dstFreq = mpu->mixFreq;
            op.dst.baseChan = chIdx;
            op.dst.chanCnt = 1;
            op.dst.sampCnt = queuedFrameCnt;
            op.dst.segCnt = 1;

            op.srcFreq = mpu->sinkBuf->freq;
            op.src.baseChan = mpu->sinkBaseChan + msc->chIdx;
            op.src.baseSamp = mpu->sinkBaseSamp;
            op.src.sampCnt = queuedFrameCnt;
            op.src.chanCnt = 1;
            op.src.segCnt = 1;

            _AmxSpu_ResampleWave(mpu, cmd->CommenceMixScope.sink, mpu->a, &op);
            break;
        }
        case amxLoadOp_DONT_CARE:
        {
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_AMX afxError _AmxSpuCmd_ConcludeMixScope(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = { 0 };

    afxUnit queuedFrameCnt = mpu->queuedFrameCnt;

    amxAudio out = mpu->sinkBuf;
    afxUnit sinkSampCnt = out->sampCnt;
    afxUnit sinkBaseSamp = 0;

    // Calculate the number of frames to process
    afxInt blocks = (sinkSampCnt + mpu->samplesPerFrame - 1) / mpu->samplesPerFrame; // Round up

    for (afxInt block = 0; block < blocks; block++)
    {
        int blockStart = block * mpu->samplesPerFrame;
        int blockEnd = (block + 1) * mpu->samplesPerFrame;

        if (blockEnd > sinkSampCnt)
            blockEnd = sinkSampCnt;  // Handle the last chunk that may not be full

        int blockSiz = blockEnd - blockStart;

        for (afxUnit chIdx = 0; chIdx < mpu->sinkChanCnt; chIdx++)
        {
            amxMixTarget const* msc = &mpu->sinkChans[chIdx];

            switch (msc->storeOp)
            {
            case amxStoreOp_STORE:
            {
                amxAudioInterference op = { 0 };
                op.dstFreq = mpu->sinkBuf->freq;
                op.src.chanCnt = 1;
                op.src.baseChan = chIdx;
                op.src.sampCnt = blockSiz;
                op.srcFreq = mpu->mixFreq;
                op.src.segCnt = 1;
                op.dst.baseChan = mpu->sinkBaseChan + chIdx;
                op.dst.baseSamp = sinkBaseSamp;
                op.dst.sampCnt = sinkSampCnt;
                op.dst.chanCnt = 1;
                op.dst.segCnt = 1;
                _AmxSpu_ResampleWave(mpu, mpu->a, out, &op);
                break;
            }
            case amxStoreOp_MERGE:
            {

                break;
            }
            case amxStoreOp_DISCARD:
            {
                break;
            }
            case amxStoreOp_DONT_CARE:
            {
                break;
            }
            default: AfxThrowError(); break;
            }
        }
    }

    mpu->sinkBuf = NIL;
    mpu->sinkBufIdx = AFX_INVALID_INDEX;
    mpu->sinkChanCnt = 0;
    mpu->sinkSampCnt = 0;

    return err;
}

_AMX afxError _AmxSpuCmd_UpdateAudio(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = { 0 };
    
    if (_AmxUpdateAudio(cmd->UpdateAudio.buf, &cmd->UpdateAudio.op, cmd->UpdateAudio.src))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxSpuCmd_ResampleWave(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = { 0 };

    //if (_AmxSpu_ResampleWave(mpu, cmd->ResampleAudio.src, cmd->ResampleAudio.dst, &cmd->ResampleAudio.srci, &cmd->ResampleAudio.dsti))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxSpuCmd_TransposeAudio(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = { 0 };

    if (_AmxTransposeAudio(cmd->TransposeAudio.src, cmd->TransposeAudio.dst, &cmd->TransposeAudio.op))
        AfxThrowError();

    return err;
}

_AMX void _AmxSpuCmd_FetchAudition(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = { 0 };
    //cmd->Audit();
    afxMixSystem msys = mpu->msys;
    afxMixContext mix = mpu->mix;
#if 0
    amxSound snd;
    afxUnit i = 0;
    while (AfxEnumerateObjects(_AmxSndsGetSndClass(msys), i++, 1, (afxObject*)&snd))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

        
        //AfxResampleAudio(snd->audio, mix->latency, mix->chanCnt, mix->freq, sizeof(mpu->bufA), mpu->bufA);

    }
#endif
}

_AMX void _AmxSpuCmd_Reverb(amxMpu* mpu, _amxCmd const* cmd)
{
    //_SpuReverb(mpu, cmd->Reverb.wetMix, cmd->Reverb.roomSiz, cmd->Reverb.width, cmd->Reverb.damp, cmd->Reverb.dryMix);
}

_AMX _amxCmdLut _AMX_SPU_CMD_VMT =
{
    .Reverb = (void*)_AmxSpuCmd_Reverb,
    .UpdateAudio = (void*)_AmxSpuCmd_UpdateAudio,
    .FetchAudition = (void*)_AmxSpuCmd_FetchAudition,
    .TransposeAudio = (void*)_AmxSpuCmd_TransposeAudio,
    .CommenceMixScope = (void*)_AmxSpuCmd_CommenceMixScope,
    .ConcludeMixScope = (void*)_AmxSpuCmd_ConcludeMixScope,
};

_AMXINL void _MixModulusLocalClock(afxMixContext mix)
{
    afxError err = { 0 };
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

_AMXINL afxReal _MixLocalClock(afxMixContext mix, afxReal* clamped)
{
    afxError err = { 0 };
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

#if 0
_AMXINL void AfxQueryMixState(afxMixContext mix, arxCapstanState* state)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    afxReal localClockRaw, localClockClamped;
    localClockRaw = _MixLocalClock(mix, &localClockClamped);
    afxReal speed = mix->motor.speed;
    afxInt iterCnt = state->iterCnt;
    afxInt currIterIdx = mix->motor.currIterIdx;
    afxReal localDur = mix->motor.localDur;

    AFX_ASSERT(state);
    state->active = 1;// (mix->motor.flags & arxCapstanFlag_ACTIVE) == arxCapstanFlag_ACTIVE;
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
    if (mix->motor.flags & arxCapstanFlag_FORCE_CLAMPLED_LOOPS)
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

void _SetUpSampleContext(arxSampleContext* ctx, afxMixContext mix)
{
    arxCapstanState ms;
    AfxQueryMixState(mix, &ms);
    ctx->localClock = ms.localClockClamped;
    ctx->localDur = ms.localDur;
    ctx->frameIdx = 0;//Binding->ca.Binding->ID.Animation->dur / ctx->LocalClock;
    ctx->underflowLoop = ms.underflow;
    ctx->overflowLoop = ms.overflow;
}
#endif

_AMX afxError _AmxMpuRollMixContexts(amxMpu* mpu, afxMixContext mix)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);

    if (mix->state == amxMixState_PENDING)
    {
        AFX_ASSERT(mix->state == amxMixState_PENDING);

        // step mixer
        afxReal64 curTime, lastTime;
        AfxGetThreadTime(&curTime, &lastTime);
        afxReal dt = curTime - mix->motor.localClock;
        mix->motor.localClock = curTime;
        mix->motor.dtLocalClockPending += dt;
#if 0
        arxSampleContext sampCtx;
        _SetUpSampleContext(&sampCtx, mix);
        mpu->sampCtx = sampCtx;
#endif
        mpu->queuedFrameCnt = mix->queuedFrameCnt;

        _amxCmd* cmdHdr;
        AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &mix->commands)
        {
            if (cmdHdr->hdr.id == NIL)
                break;

            if (mix->state != amxMixState_PENDING)
            {
                AfxThrowError();
                break;
            }
            _AMX_SPU_CMD_VMT.f[cmdHdr->hdr.id](mpu, cmdHdr);
        }

        mix->current_sample_time += mpu->queuedFrameCnt;
        mix->queuedFrameCnt -= mpu->queuedFrameCnt;
        mpu->queuedFrameCnt = 0;

#if 0
        if (!err)
        {
            mix->state = amxMixState_EXECUTABLE;
        }

        if (err || mix->m.disposable)
        {
            AFX_ASSERT(mix->m.portId == mpu->portId);
            mix->m.state = avxDrawContextState_INVALID;
            afxMixQueue mque = AfxGetHost(mix);
            AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

            afxUnit poolIdx = mix->m.poolIdx;

            AfxLockFutex(&mque->m.cmdbReqLock);

            if (AfxPushQueue(&mque->m.cmdbRecycQue, &mix))
            {
                AfxDisposeObjects(1, (void**)&mix);
            }

            AfxUnlockFutex(&mque->m.cmdbReqLock);
        }
#endif
    }
    //AfxDecAtom32(&mix->submCnt);
    //AfxDisposeObjects(1, &mix);
    return err;
}

