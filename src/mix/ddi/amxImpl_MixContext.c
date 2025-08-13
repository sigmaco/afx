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

#define _AMX_MIX_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#define _AMX_VOICE_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxError _AmxSpuLoadAudio(amxMpu* mpu, amxAudio src, amxAudio dst, afxUnit chIdx, afxUnit baseSamp, afxUnit sampCnt, amxMixFactor fac, afxReal constant)
{
    afxError err = NIL;
    afxReal32* out = &dst->samples32f[chIdx * dst->sampCnt + baseSamp];
    afxReal32 const* in = &src->samples32f[chIdx * src->sampCnt + baseSamp];

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
    afxError err = AFX_ERR_NONE;
    afxReal32* out = &dst->samples32f[chIdx * dst->sampCnt + baseSamp];
    afxReal32 const* in = &src->samples32f[chIdx * src->sampCnt + baseSamp];

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
    afxError err = AFX_ERR_NONE;

    afxUnit sinkChanCnt = cmd->CommenceMixScope.scope.chanCnt;

    AFX_ASSERT(ARRAY_SIZE(mpu->sinkChans) >= sinkChanCnt);
    sinkChanCnt = AFX_MIN(sinkChanCnt, ARRAY_SIZE(mpu->sinkChans));
    
    mpu->sinkChanCnt = sinkChanCnt;
    mpu->sinkBaseChan = cmd->CommenceMixScope.scope.baseChan;
    mpu->sinkBaseSamp = cmd->CommenceMixScope.scope.baseSamp;
    mpu->sinkSampCnt = cmd->CommenceMixScope.scope.sampCnt;
    mpu->sinkBuf = cmd->CommenceMixScope.scope.sink;
    mpu->mixFreq = cmd->CommenceMixScope.scope.freq;

    for (afxUnit i = 0; i < sinkChanCnt; i++)
    {
        mpu->sinkChans[i] = cmd->CommenceMixScope.chans[i];
    }
    return err;
}

_AMX afxError _AmxSpuCmd_ConcludeMixScope(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    amxBufferedTrack room;
    if (AmxLockSinkBuffer(mpu->sinkBuf, 0, 512, &room))
        return err;

    amxAudio out = mpu->sinkBuf->buffers[mpu->sinkBufIdx];
    afxUnit sinkSampCnt = out->sampCnt;
    afxUnit sinkBaseSamp = 0;

    // Calculate the number of frames to process
    afxInt frames = (sinkSampCnt + mpu->samplesPerFrame - 1) / mpu->samplesPerFrame; // Round up

    for (afxInt frame = 0; frame < frames; frame++)
    {
        int frameStart = frame * mpu->samplesPerFrame;
        int frameEnd = (frame + 1) * mpu->samplesPerFrame;

        if (frameEnd > sinkSampCnt)
            frameEnd = sinkSampCnt;  // Handle the last chunk that may not be full

        int frameSiz = frameEnd - frameStart;

        for (afxUnit chIdx = 0; chIdx < mpu->sinkChanCnt; chIdx++)
        {
            amxMixSink const* msc = &mpu->sinkChans[chIdx];

            switch (msc->loadOp)
            {
            case amxLoadOp_CLEAR:
            {
                //_AmxGenerateSquareWave(mpu->a, chIdx, 1, 440.0, 60);
                amxAudioPeriod op = { 0 };
                op.baseChan = mpu->sinkBaseChan + chIdx;
                op.chanCnt = 1;
                op.sampCnt = frameSiz;
                _AmxFillAudio(mpu->a, &op, msc->clearAmpl, mpu->mixFreq, 1);
                break;
            }
            case amxLoadOp_LOAD:
            {
                amxAudioInterference op = { 0 };
                op.dstFreq = mpu->mixFreq;
                op.dst.baseChan = chIdx;
                op.dst.chanCnt = 1;
                op.dst.sampCnt = frameSiz;
                op.srcFreq = mpu->sinkBuf->freq;
                op.src.baseChan = mpu->sinkBaseChan + chIdx;
                op.src.baseSamp = sinkBaseSamp;
                op.src.sampCnt = sinkSampCnt;
                op.src.chanCnt = 1;
                _AmxSpu_ResampleWave(mpu, out, mpu->a, &op);
                break;
            }
            case amxLoadOp_DONT_CARE:
            {
                break;
            }
            default: AfxThrowError(); break;
            }

            switch (msc->storeOp)
            {
            case amxStoreOp_STORE:
            {
                amxAudioInterference op = { 0 };
                op.dstFreq = mpu->sinkBuf->freq;
                op.src.chanCnt = 1;
                op.src.baseChan = chIdx;
                op.src.sampCnt = frameSiz;
                op.srcFreq = mpu->mixFreq;
                op.dst.baseChan = mpu->sinkBaseChan + chIdx;
                op.dst.baseSamp = sinkBaseSamp;
                op.dst.sampCnt = sinkSampCnt;
                op.dst.chanCnt = 1;
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

    AfxPushInterlockedQueue(&mpu->sinkBuf->readyBuffers, &mpu->sinkBufIdx);

    mpu->sinkBuf = NIL;
    mpu->sinkBufIdx = AFX_INVALID_INDEX;
    mpu->sinkChanCnt = 0;
    mpu->sinkSampCnt = 0;

    return err;
}

_AMX afxError _AmxSpuCmd_UpdateAudio(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;
    
    if (_AmxUpdateAudio(cmd->UpdateAudio.buf, &cmd->UpdateAudio.op, cmd->UpdateAudio.src))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxSpuCmd_ResampleWave(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    //if (_AmxSpu_ResampleWave(mpu, cmd->ResampleAudio.src, cmd->ResampleAudio.dst, &cmd->ResampleAudio.srci, &cmd->ResampleAudio.dsti))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxSpuCmd_TransposeAudio(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = AFX_ERR_NONE;

    if (_AmxTransposeAudio(cmd->TransposeAudio.src, cmd->TransposeAudio.dst, &cmd->TransposeAudio.op))
        AfxThrowError();

    return err;
}

_AMX void _AmxSpuCmd_FetchAudition(amxMpu* mpu, _amxCmd const* cmd)
{
    afxError err = NIL;
    //cmd->Audit();
    afxMixSystem msys = mpu->msys;
    afxMixContext mix = mpu->mix;
    
    amxSound snd;
    afxUnit i = 0;
    while (AfxEnumerateObjects(_AmxMsysGetSndClass(msys), i++, 1, (afxObject*)&snd))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SND, 1, &snd);

        
        //AfxResampleAudio(snd->audio, mix->latency, mix->chanCnt, mix->freq, sizeof(mpu->bufA), mpu->bufA);

    }
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

_AMXINL afxReal _MixLocalClock(afxMixContext mix, afxReal* clamped)
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

_AMXINL void AfxQueryMixState(afxMixContext mix, afxCapstanState* state)
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

void _SetUpSampleContext(afxSampleContext* ctx, afxMixContext mix)
{
    afxCapstanState ms;
    AfxQueryMixState(mix, &ms);
    ctx->localClock = ms.localClockClamped;
    ctx->localDur = ms.localDur;
    ctx->frameIdx = 0;//Binding->ca.Binding->ID.Animation->dur / ctx->LocalClock;
    ctx->underflowLoop = ms.underflow;
    ctx->overflowLoop = ms.overflow;
}

_AMX afxError _AmxMpuRollMixContexts(amxMpu* mpu, afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
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

        afxSampleContext sampCtx;
        _SetUpSampleContext(&sampCtx, mix);
        mpu->sampCtx = sampCtx;

        _amxCmd* cmdHdr;
        AFX_ITERATE_CHAIN_B2F(_amxCmd, cmdHdr, hdr.script, &mix->commands)
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

#if 0
        if (!err)
        {
            mix->state = amxMixState_EXECUTABLE;
        }

        if (err || mix->m.disposable)
        {
            AFX_ASSERT(mix->m.portId == mpu->portId);
            mix->m.state = avxDrawContextState_INVALID;
            afxMixQueue mque = AfxGetProvider(mix);
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

_AMX afxError _AmxMixResetCb(afxMixContext mix, afxBool freeMem, afxBool permanent)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(mix->state != amxMixState_PENDING);
    mix->disposable = !permanent;

    AfxDeployChain(&mix->commands, mix);
    AfxExhaustArena(&mix->cmdArena);

    return err;
}

_AMX afxError _AmxMixEndCb(afxMixContext mix)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MIX, 1, &mix);
    return err;
}


