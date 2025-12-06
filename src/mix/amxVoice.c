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
#define _AMX_AUDIO_C
#define _AMX_BUFFER_C
#define _AMX_VOICE_C
//#define _AMX_MIX_SYSTEM_C
#include "amxIcd.h"

// This is the sound of Z; the sound that creates a new dimension.

typedef enum amxVoiceState
{
    amxVoiceState_STOP,
    amxVoiceState_PLAY,
    amxVoiceState_PAUSE,
} amxVoiceState;

float interpolate0(const float* input, int len, int pos, double frac)
{
    if (pos >= len - 1) {
        return input[len - 1]; // Edge case: end of buffer
    }

    float a = input[pos];
    float b = input[pos + 1];
    return a + (b - a) * frac;
}

float interpolate(const float* input, int len, int pos, double frac, afxBool looping)
{
    int nextPos = pos + 1;

    if (looping && nextPos >= len) {
        nextPos = 0; // Wrap for loop
    }
    else if (nextPos >= len) {
        nextPos = len - 1; // Clamp to end
    }

    float a = input[pos];
    float b = input[nextPos];

    return a + (b - a) * (float)frac;
}

afxSize minimumBytesPerFrame(amxFormat fmt, int numChans)
{
    switch (fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
        return sizeof(float) * numChans;

    case amxFormat_M32i:
    case amxFormat_S32i:
        return sizeof(int32_t) * numChans;

    case amxFormat_M24i:
    case amxFormat_S24i:
        return 3 * numChans;

    case amxFormat_M16i:
    case amxFormat_S16i:
        return sizeof(int16_t) * numChans;

    case amxFormat_M8i:
    case amxFormat_S8i:
        return sizeof(uint8_t) * numChans;

    default:
        return 0; // Unknown format — return safe 0
    }
}

void loadVoicingInfo(amxVoice vs, const amxVoicingInfo* info)
{
    afxError err = { 0 };
    // Load Playback Info Into Voice.
    // This function populates your voice state from a dequeued amxVoicingInfo.

    vs->srcBuf = info->srcBuf;
    vs->srcOffset = info->srcOffset;
    vs->srcRange = info->srcRange;
    vs->srcStride = info->srcStride;
    vs->sampRate = info->sampleRate;

    //afxUnit totalSamples = vs->srcRange / info->srcBuf->fmtStride;
    afxUnit totalSamples = vs->srcRange / info->srcStride;
    afxUnit playBegin = info->playBegin;
    afxUnit playLen = (info->playLen == 0) ? totalSamples : info->playLen;

    AFX_ASSERT_RANGE(vs->srcRange, playBegin * vs->srcStride, playLen * vs->srcStride);

    vs->iterBegin = info->iterBegin;
    vs->iterEnd = (info->iterLen == 0 && info->iterCnt > 0)
        ? totalSamples
        : info->iterBegin + info->iterLen;

    AFX_ASSERT_RANGE(vs->srcRange, vs->iterBegin * vs->srcStride, vs->iterEnd * vs->srcStride);

    vs->iterCnt = info->iterCnt;
    vs->iterIdx = 0;
    vs->looping2 = (info->iterCnt > 0);
    vs->playing2 = TRUE;

    //vs->srcCursor = playBegin * info->srcBuf->fmtStride; // <--- Initial read cursor
    vs->srcCursor = playBegin * info->srcStride; // <--- Initial read cursor
#if !0
    vs->freqRatio = 48000.0 / (double)vs->sampRate;
    vs->rs.ratio = vs->freqRatio;
#endif

    vs->rs.frac_pos = 0;
    vs->rs.in_len = 0;
    vs->rs.in_pos = 0;
    vs->rs.needs_more_input = TRUE;
}

float convertSampleToFloat(uint8_t* frame, amxFormat fmt)
{
    switch (fmt)
    {
    case amxFormat_M16i:
        return ((int16_t*)frame)[0] / 32768.f;

    case amxFormat_S16i:
        return ((int16_t*)frame)[0] / 32768.f;

    case amxFormat_M32f:
        return ((float*)frame)[0];

    case amxFormat_S32f:
        return ((float*)frame)[0]; // interleaved, so ch offset

    case amxFormat_M8i:
        return (((int)frame[0] - 128) / 128.0f);

    case amxFormat_S8i:
        return (((int)frame[0] - 128) / 128.0f);

        // Add 24-bit and 32-bit int formats as needed:
    case amxFormat_M24i:
    case amxFormat_S24i:
    {
        int32_t sample = (frame[0 * 3 + 0] << 8) |
            (frame[0 * 3 + 1] << 16) |
            (frame[0 * 3 + 2] << 24);
        sample >>= 8; // align sign
        return sample / 8388608.0f;
    }

    default:
        return 0.f;
    }
}
afxSize bytesPerFrame(amxFormat fmt, int numChans)
{
    switch (fmt)
    {
    case amxFormat_M16i: return sizeof(int16_t) * numChans;
    case amxFormat_S16i: return sizeof(int16_t) * numChans;
    case amxFormat_M32f: return sizeof(float) * numChans;
    case amxFormat_S32f: return sizeof(float) * numChans;
    case amxFormat_M8i:  return sizeof(uint8_t) * numChans;
    case amxFormat_S8i:  return sizeof(uint8_t) * numChans;
    case amxFormat_M24i: return 3 * numChans;
    case amxFormat_S24i: return 3 * numChans;
    default: return 0;
    }
}

void fillResampleInput(amxVoice vs)
{
    afxError err = { 0 };
    vs->rs.in_len = 0;
    vs->rs.in_pos = 0;

    amxFormat fmt = vs->srcBuf->fmt;
    int numChans = 2;
    afxSize sampleSize = minimumBytesPerFrame(fmt, 1);
    afxSize frameSize = sampleSize * numChans;

    uint8_t* data = (uint8_t*)vs->srcBuf->storage[0].hostedAlloc.bytemap + vs->srcOffset;
    afxSize loopBeginByte = vs->iterBegin * vs->srcStride;
    afxSize loopEndByte = vs->iterEnd * vs->srcStride;

    afxUnit framesWritten = 0;
    afxUnit frameCap = sizeof(vs->rs.input_buf) / sizeof(vs->rs.input_buf[0]);

    while (framesWritten < frameCap)
    {
        // Loop wrapping: BEFORE checking availability
        if (vs->looping2 && vs->srcCursor >= loopEndByte)
        {
            if (vs->iterCnt == AFX_U32_MAX || vs->iterIdx < vs->iterCnt)
            {
                vs->srcCursor = loopBeginByte;
                vs->iterIdx++;
            }
            else
            {
                // Out of loops
                vs->playing2 = FALSE;
                break;
            }
        }

        afxSize maxDataPos = vs->looping2 ? loopEndByte : vs->srcRange;
        afxSize available = maxDataPos - vs->srcCursor;

        // Make sure loopEndByte is within the actual buffer size.
        if (loopEndByte > vs->srcRange - vs->srcOffset)
        {
            // Loop endpoint is outside of buffer
            AfxThrowError();
            break;
        }

        if (available < vs->srcStride)
        {
            // Not enough data to read a full frame
            break;
        }

        // Ensure that we don't go beyond the actual size of the buffer.
        afxSize effectiveOffset = vs->srcOffset + vs->srcCursor;
        if (effectiveOffset + vs->srcStride > vs->srcRange)
        {
            break; // Prevent buffer overrun
        }

        uint8_t* framePtr = data + vs->srcCursor;

        float sample = convertSampleToFloat(framePtr, fmt);
        vs->rs.input_buf[framesWritten] = sample;

        vs->srcCursor += vs->srcStride;
        framesWritten++;
    }

    vs->rs.in_len = framesWritten;
    vs->rs.needs_more_input = FALSE;
}

afxSize amxProcessVoice(amxVoice vs, float* outBuf, afxSize outFrames, afxUnit stride)
{
    afxSize framesWritten = 0;

    if (!vs->playing2 || vs->paused)
        return 0;
     
    while (framesWritten < outFrames)
    {
        if (vs->rs.needs_more_input)
        {
            if (!vs->srcBuf || ((vs->srcCursor >= vs->srcRange) && !vs->looping2))
            {
                // Load next voice segment (next amxVoicingInfo from the queue)
                amxVoicingInfo info;
                if (AfxPopInterlockedQueue(&vs->bufQue, &info)) {
                    loadVoicingInfo(vs, &info);  // sets srcBuf, srcOffset, srcRange, and resets srcCursor
                }
                else {
                    // Nothing left to play
                    vs->playing2 = FALSE;
                    break;
                }
            }

            fillResampleInput(vs);  // <--- srcCursor is used here
        }

        // Resample from rs.input_buf into output buffer
        while (framesWritten < outFrames && vs->rs.in_pos < vs->rs.in_len)
        {
            outBuf[stride * framesWritten++] = interpolate(
                vs->rs.input_buf,
                vs->rs.in_len,
                vs->rs.in_pos,
                vs->rs.frac_pos, vs->looping2);

            vs->rs.frac_pos += vs->rs.ratio;
            int advance = (int)(vs->rs.frac_pos);
            vs->rs.in_pos += advance;
            vs->rs.frac_pos -= advance;
            vs->samplesPlayed++; // Output samples
        }

        // If all input used, mark that we need more
        if (vs->rs.in_pos >= vs->rs.in_len)
            vs->rs.needs_more_input = TRUE;
    }

    return framesWritten;
}

// Adds a new audio buffer to the voice queue.

_AMX afxError AmxFeedVoice(amxTracker trax, afxUnit id, amxVoicingInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    amxVoicingInfo vi = { 0 };
    vi = *info;

    AFX_ASSERT(info->srcStride);

    if (!AfxPushInterlockedQueue(&vox->bufQue, &vi))
        AfxThrowError();

    return err;
}

// Removes all pending audio buffers from the voice queue.

_AMX afxError AmxPurgeVoice(amxTracker trax, afxUnit id, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    amxVoicingInfo vi = { 0 };
    while (AfxPopInterlockedQueue(&vox->bufQue, &vi));

    return err;
}

// Stops looping the voice when it reaches the end of the current loop region.

_AMX afxError AmxBreakVoiceLoop(amxTracker trax, afxUnit id, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->iterCnt = 0;
    vox->looping2 = FALSE;

    return err;
}

// Starts consumption and processing of audio by the voice. 
// Delivers the result to any connected submix or mastering voices, or to the output device.

_AMX afxError AmxSwitchVoice(amxTracker trax, afxUnit id, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->playing2 = !!flags;
    vox->paused = FALSE;

    return err;
}

// Reconfigures the voice to consume source data at a different sample rate than the rate specified when the voice was created.

_AMX afxError AmxSetVoiceSampleRate(amxTracker trax, afxUnit id, afxUnit sampRate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->sampRate = sampRate;
    vox->freqRatio = (afxReal)48000 / (afxReal)sampRate;
    vox->rs.ratio = (afxReal)48000 / (afxReal)sampRate;

    return err;
}

// Sets the frequency adjustment ratio of the voice.

_AMX afxError AmxSetVoiceFrequencyRatio(amxTracker trax, afxUnit id, afxReal ratio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->freqRatio = ratio;
    vox->rs.ratio = vox->freqRatio;

    return err;
}

// Returns the frequency adjustment ratio of the voice.

_AMX afxError AmxGetVoiceFrequencyRatio(amxTracker trax, afxUnit id, afxReal* ratio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    *ratio = vox->freqRatio;

    return err;
}

// Returns the voice's current state and cursor position data.

_AMX afxError AmxQueryVoiceState(amxTracker trax, afxUnit id, amxBuffer* buf, afxUnit* bufQueued, afxUnit64* samplesPlayed)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    amxVoice vox;
    AfxEnumerateObjects(_AmxTraxGetVoxClass(trax), id, 1, (afxObject*)&vox);
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    *buf = vox->srcBuf;
    *bufQueued = AfxLoadAtom32(&vox->bufQue.readPosn);
    *samplesPlayed = vox->samplesPlayed;

    return err;
}


#if 0
_AMX afxError _AmxToggleVoice(amxVoice vox, amxVoiceState state)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->offset = 0;
    vox->streamPos = 0.0f;

    vox->frameCursPosn = 0;
    vox->framesProced = 0;

    return 0;
}

_AMX void AmxTransformVoice(amxVoice vox, afxTransform const* t)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
    AfxCopyTransform(&vox->t, t);

    // look for audients and relate them?
};

_AMX afxUnit AmxAccumulateVoiceSamples(amxVoice vox, afxUnit sampleCnt, afxUnit chCnt, afxUnit freq, afxUnit bufCap, afxReal output[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
    
    amxAudio aud = vox->audio;

    if (!aud)
        return 0;

    afxUnit dataLeft = aud->sampCnt - vox->offset;
    afxUnit copyLen = dataLeft;

    if (copyLen > sampleCnt)
        copyLen = sampleCnt;

    //AfxResampleAudio(aud, sampleCnt, chCnt, freq, bufCap, output);

    vox->offset += copyLen;

    return copyLen;
}

_AMX afxError AmxRewindVoice(amxVoice vox)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    vox->offset = 0;
    vox->streamPos = 0.0f;

    return 0;
}

_AMX afxBool AmxHasVoiceEnded(amxVoice vox)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    if (!(vox->flags & 0x1) && vox->offset >= vox->audio->sampCnt)
    {
        return 1;
    }

    return 0;
}
#endif

_AMX afxError _AmxVoxDtorCb(amxVoice vox)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    amxTracker trax = AfxGetHost(vox);
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    AfxExhaustInterlockedQueue(&vox->bufQue);

    return err;
}

_AMX afxError _AmxVoxCtorCb(amxVoice vox, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);

    amxTracker trax = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);
    //amxVoiceInfo const* spec = AFX_CAST(amxVoiceInfo const*, args[1]) + invokeNo;
    //AFX_ASSERT(spec);
#if 0
    {
        vox->ctrl.flags = /*arxCapstanFlag_ACTIVE*/0x01;
        //vox->ctrl.timing.currClock = /*startTime*/0;
        vox->ctrl.dtLocalClockPending = 0.0;
        vox->ctrl.localClock = 0.0;
        vox->ctrl.speed = 1.0;
        vox->ctrl.localDur = /*cfg->localDur*/NIL;//spec->playLen;
        vox->ctrl.currIterIdx = 0;
        vox->ctrl.iterCnt = /*iterCnt*/NIL;//spec->iterCnt;
        //vox->ctrl.timing.killClock = 0;
        vox->ctrl.currWeight = 1;
        //vox->ctrl.timing.easeInStartClock = 0;
        //vox->ctrl.timing.easeInEndClock = 0;
        vox->ctrl.easeInValues = (afxUnit)-65536;
        //vox->ctrl.timing.easeOutStartClock = 0;
        //vox->ctrl.timing.easeOutEndClock = 0;
        vox->ctrl.easeOutValues = (afxUnit)0xFFFF;
    }

    vox->audio = NIL;// spec->aud;
    vox->streamPos = 0;
    vox->offset = 0;
    vox->flags = 0;
#endif

    // triple-buffered
    AfxMakeInterlockedQueue(&vox->bufQue, sizeof(amxVoicingInfo), 3);

    vox->rs.needs_more_input = 1;
    vox->rs.ratio = 1;

    return err;
}

_AMX afxClassConfig const _AMX_VOX_CLASS_CONFIG =
{
    .fcc = afxFcc_VOX,
    .name = "Voice",
    .desc = "Mix In Voice",
    .fixedSiz = sizeof(AFX_OBJECT(amxVoice)),
    .ctor = (void*)_AmxVoxCtorCb,
    .dtor = (void*)_AmxVoxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxDeallocateVoices(amxTracker trax, afxUnit firstId, afxUnit cnt, afxUnit voiceIdMap[])
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);

    afxClass* cls = (afxClass*)_AmxTraxGetVoxClass(trax);
    AFX_ASSERT_CLASS(cls, afxFcc_VOX);

    for (afxUnit i = cnt; i--> 0;)
    {
        afxUnit id = voiceIdMap ? voiceIdMap[i] + firstId : firstId + i;

        amxVoice vox;
        if (AfxEnumerateObjects(cls, id, 1, (afxObject*)&vox))
        {
            AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
            AfxDisposeObjects(1, &vox);
        }
    }

    return err;
}

_AMX afxError AmxAllocateVoices(amxTracker trax, amxVoiceCaps caps, afxUnit cnt, afxUnit voiceIds[])
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_TRAX, 1, &trax);
    AFX_ASSERT(voiceIds);

    afxClass* cls = (afxClass*)_AmxTraxGetVoxClass(trax);
    AFX_ASSERT_CLASS(cls, afxFcc_VOX);

    amxVoice voices[32];
    static const afxUnit batchSiz = 32;
    afxUnit remainCnt = cnt % batchSiz;
    afxUnit batchCnt = cnt / batchSiz;
    afxUnit baseArrIdx = 0;
    for (afxUnit i = 0; i < batchCnt; i++)
    {
        if (AfxAcquireObjects(cls, batchSiz, (afxObject)voices, (void const*[]) { trax, NIL }))
        {
            AfxThrowError();
            for (afxUnit j = i; j--> 0;)
            {
                for (afxUnit k = batchSiz; k--> 0;)
                {
                    amxVoice vox;
                    if (AfxEnumerateObjects(cls, voiceIds[(batchSiz * j) + k], 1, (afxObject*)&vox))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
                        AfxDisposeObjects(1, &vox);
                    }
                    else
                    {
                        AfxThrowError();
                    }
                }
            }
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_VOX, batchSiz, voices);
            for (afxUnit j = 0; j < batchCnt; j++)
            {
                afxUnit uId = AfxGetObjectId(voices[j]);
                voiceIds[baseArrIdx + j] = uId;
            }
            baseArrIdx += batchSiz;
        }
    }

    if (AfxAcquireObjects(cls, remainCnt, (afxObject)voices, (void const*[]) { trax, NIL }))
    {
        AfxThrowError();
        for (afxUnit j = batchCnt; j-- > 0;)
        {
            for (afxUnit k = batchSiz; k-- > 0;)
            {
                amxVoice vox;
                if (AfxEnumerateObjects(cls, voiceIds[(batchSiz * j) + k], 1, (afxObject*)&vox))
                {
                    AFX_ASSERT_OBJECTS(afxFcc_VOX, 1, &vox);
                    AfxDisposeObjects(1, &vox);
                }
                else
                {
                    AfxThrowError();
                }
            }
        }
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VOX, remainCnt, voices);
        for (afxUnit j = 0; j < remainCnt; j++)
        {
            afxUnit uId = AfxGetObjectId(voices[j]);
            voiceIds[baseArrIdx + j] = uId;
        }
        baseArrIdx += batchSiz;
    }

    return err;
}
