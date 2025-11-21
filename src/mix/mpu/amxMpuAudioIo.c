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

// 5417

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

void* get_segment_ptr(amxAudio a, afxUnit segIndex, afxUnit chanIndex)
{
    // This assumes segments are arranged in a [segment][channel][samples] fashion.

    afxUnit segmentSize = a->sampCnt * a->fmtBps; // size of one segment for one channel
    afxUnit offset = (segIndex * a->chanCnt + chanIndex) * segmentSize;
    return (uint8_t*)a->buf->storage[0].hostedAlloc.bytemap + offset;
}

void interleaved_to_deinterleaved(amxAudio in, amxAudio out)
{
    // Interleaved-Deinterleaved conversion
    // Useful when we want to split interleaved stereo(LRLRLR...) into separate segments per channel.

    afxUnit sampleSize = in->fmtBps;
    afxUnit frameSize = in->fmtStride;
    afxUnit totalFrames = in->sampCnt;

    for (afxUnit s = 0; s < totalFrames; ++s)
    {
        for (afxUnit c = 0; c < in->chanCnt; ++c)
        {
            void* src = (uint8_t*)in->buf->storage[0].hostedAlloc.bytemap + s * frameSize + c * sampleSize;
            void* dst = (uint8_t*)out->buf->storage[0].hostedAlloc.bytemap +
                (c * totalFrames + s) * sampleSize;

            AfxCopy(dst, src, sampleSize);
        }
    }
#if 0
    out->chanCnt = in->chanCnt;
    out->sampCnt = in->sampCnt;
    out->segCnt = 1; // one segment with all channels
    out->fmt = in->fmt;
    out->fmtBps = in->fmtBps;
    out->fmtStride = in->fmtBps;
    out->freq = in->freq;
#endif
}

void deinterleaved_to_interleaved(amxAudio in, amxAudio out)
{
    // Deinterleaved-Interleaved conversion
    // To prepare audio for playback APIs expecting interleaved buffers.

    afxUnit sampleSize = in->fmtBps;
    afxUnit frameSize = in->chanCnt * sampleSize;
    afxUnit totalFrames = in->sampCnt;

    for (afxUnit s = 0; s < totalFrames; ++s)
    {
        for (afxUnit c = 0; c < in->chanCnt; ++c)
        {
            void* src = (uint8_t*)in->buf->storage[0].hostedAlloc.bytemap +
                (c * totalFrames + s) * sampleSize;

            void* dst = (uint8_t*)out->buf->storage[0].hostedAlloc.bytemap +
                (s * frameSize + c * sampleSize);

            AfxCopy(dst, src, sampleSize);
        }
    }
#if 0
    out->chanCnt = in->chanCnt;
    out->sampCnt = in->sampCnt;
    out->segCnt = 1;
    out->fmt = in->fmt;
    out->fmtBps = in->fmtBps;
    out->fmtStride = frameSize;
    out->freq = in->freq;
#endif
}

_AMXINL void* get_sample_ptr(amxAudio a, afxUnit seg, afxUnit chan, afxUnit samp)
{
    // Segment-aware Sample Access
    // A function to get a pointer to a specific sample given segment, channel, and sample index.
    // Access a specific sample, regardless of layout:

    afxUnit sampleSize = a->fmtBps;
    afxUnit segmentLength = a->sampCnt; // assuming sampCnt per segment
    afxUnit samplesPerChan = segmentLength;

    // Total offset = segment offset + channel offset + sample offset
    afxUnit segOffset = seg * a->chanCnt * samplesPerChan * sampleSize;
    afxUnit chanOffset = chan * samplesPerChan * sampleSize;
    afxUnit sampOffset = samp * sampleSize;

    return (uint8_t*)a->buf->storage[0].hostedAlloc.bytemap + segOffset + chanOffset + sampOffset;
}

_AMXINL void* get_period_sample_ptr(amxAudio a, amxAudioPeriod* p, afxUnit relSeg, afxUnit relChan, afxUnit relSamp)
{
    // Accessing Data in a Region.
    // We can build a helper to get a sample pointer within an audio object given a period and a relative coordinate.

    afxUnit sampleSize = a->fmtBps;
    afxUnit chanStride = a->sampCnt * sampleSize;  // bytes per channel
    afxUnit segStride = a->chanCnt * chanStride;  // bytes per segment

    afxUnit absSeg = p->baseSeg + relSeg;
    afxUnit absChan = p->baseChan + relChan;
    afxUnit absSamp = p->baseSamp + relSamp;

    afxUnit offset =
        absSeg * segStride +
        absChan * chanStride +
        absSamp * sampleSize;

    return (uint8_t*)a->buf->storage[0].hostedAlloc.bytemap + offset;
}

_AMXINL void process_audio_period(amxAudio a, amxAudioPeriod* p, void(*callback)(void* sample, afxUnit seg, afxUnit chan, afxUnit samp))
{
    // Processing a Region.
    // Here's a basic loop that processes samples in a region.

    for (afxUnit seg = 0; seg < p->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < p->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < p->sampCnt; ++samp)
            {
                void* sample = get_sample_ptr(a, p->baseSeg + seg, p->baseChan + chan, p->baseSamp + samp);
                callback(sample, seg, chan, samp);
            }
        }
    }
}

void copy_audio_region(amxAudio dst, amxAudioCopy const* cpy, amxAudio src)
{
    // Copy Region Between Audio Objects.
    // Useful to extract or paste rectangular audio patches.

    afxUnit sampleSize = src->fmtBps;

    for (afxUnit seg = 0; seg < cpy->src.segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < cpy->src.chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < cpy->src.sampCnt; ++samp)
            {
                void* srcPtr = get_sample_ptr(src, cpy->src.baseSeg + seg, cpy->src.baseChan + chan, cpy->src.baseSamp + samp);
                void* dstPtr = get_sample_ptr(dst, cpy->dstBaseSeg + seg, cpy->dstBaseChan + chan, cpy->dstBaseSamp + samp);
                AfxCopy(dstPtr, srcPtr, sampleSize);
            }
        }
    }
}

void zero_audio_region(amxAudio a, amxAudioPeriod* p)
{
    // Zero-fill a Region
    // Clears a specified region(sets all samples to zero).
    // Assumes PCM data; no special handling for floating point zero vs int zero.

    afxUnit sampleSize = a->fmtBps;

    for (afxUnit seg = 0; seg < p->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < p->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < p->sampCnt; ++samp)
            {
                void* ptr = get_sample_ptr(a, p->baseSeg + seg, p->baseChan + chan, p->baseSamp + samp);
                AfxFill(ptr, 0, sampleSize);
            }
        }
    }
}

void gain_audio_region_i16(amxAudio a, amxAudioPeriod* p, float gain)
{
    // Apply Gain to a Region
    // Multiplies all samples by a scalar (with type awareness: int16_t, float, etc.).
    // This version handles 16-bit signed integer PCM.
    // Add variants for float, int32_t, etc. based on a->fmt.

    for (afxUnit seg = 0; seg < p->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < p->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < p->sampCnt; ++samp)
            {
                int16_t* sample = (int16_t*)get_sample_ptr(a, p->baseSeg + seg, p->baseChan + chan, p->baseSamp + samp);
                int temp = (int)(*sample * gain);
                if (temp > 32767) temp = 32767;
                else if (temp < -32768) temp = -32768;
                *sample = (int16_t)temp;
            }
        }
    }
}

void gain_audio_region_f32(amxAudio a, amxAudioPeriod* p, float gain)
{
    // 32-bit Float PCM.

    for (afxUnit seg = 0; seg < p->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < p->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < p->sampCnt; ++samp)
            {
                float* sample = (float*)get_sample_ptr(a, p->baseSeg + seg, p->baseChan + chan, p->baseSamp + samp);
                *sample *= gain;
            }
        }
    }
}

void crossfade_audio_regions_i16(amxAudio out, amxAudioPeriod* outP, amxAudio a, amxAudioPeriod* aP, amxAudio b, amxAudioPeriod* bP)
{
    // Crossfade Between Two Regions
    // Blends two equally sized regions over a fade curve.
    // Crossfades between two audio buffers over a region using linear interpolation.
    // Assumes both source and destination periods are the same size and format (16-bit PCM).

    afxUnit N = aP->sampCnt;

    for (afxUnit seg = 0; seg < aP->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < aP->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < N; ++samp)
            {
                float t = (float)samp / (N - 1); // fade factor from 0 to 1

                int16_t* aSamp = (int16_t*)get_sample_ptr(a, aP->baseSeg + seg, aP->baseChan + chan, aP->baseSamp + samp);
                int16_t* bSamp = (int16_t*)get_sample_ptr(b, bP->baseSeg + seg, bP->baseChan + chan, bP->baseSamp + samp);
                int16_t* outSamp = (int16_t*)get_sample_ptr(out, outP->baseSeg + seg, outP->baseChan + chan, outP->baseSamp + samp);

                float mixed = (*aSamp) * (1.0f - t) + (*bSamp) * t;

                if (mixed > 32767.0f) mixed = 32767.0f;
                else if (mixed < -32768.0f) mixed = -32768.0f;

                *outSamp = (int16_t)mixed;
            }
        }
    }
}

void crossfade_audio_regions_f32(amxAudio out, amxAudioPeriod* outP, amxAudio a, amxAudioPeriod* aP, amxAudio b, amxAudioPeriod* bP)
{
    afxUnit N = aP->sampCnt;

    for (afxUnit seg = 0; seg < aP->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < aP->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < N; ++samp)
            {
                float t = (float)samp / (N - 1); // fade factor [0,1]

                afxReal* aSamp = (afxReal*)get_sample_ptr(a, aP->baseSeg + seg, aP->baseChan + chan, aP->baseSamp + samp);
                afxReal* bSamp = (afxReal*)get_sample_ptr(b, bP->baseSeg + seg, bP->baseChan + chan, bP->baseSamp + samp);
                afxReal* outSamp = (afxReal*)get_sample_ptr(out, outP->baseSeg + seg, outP->baseChan + chan, outP->baseSamp + samp);

                *outSamp = (*aSamp) * (1.0f - t) + (*bSamp) * t;
            }
        }
    }
}

afxBool intersect_audio_periods(const amxAudioPeriod* a, const amxAudioPeriod* b, amxAudioPeriod* out)
{
    // Region Intersection Utility
    // To compute the overlapping area of two audio_periods:

    afxUnit startSamp = max(a->baseSamp, b->baseSamp);
    afxUnit endSamp = min(a->baseSamp + a->sampCnt, b->baseSamp + b->sampCnt);

    afxUnit startChan = max(a->baseChan, b->baseChan);
    afxUnit endChan = min(a->baseChan + a->chanCnt, b->baseChan + b->chanCnt);

    afxUnit startSeg = max(a->baseSeg, b->baseSeg);
    afxUnit endSeg = min(a->baseSeg + a->segCnt, b->baseSeg + b->segCnt);

    if (startSamp >= endSamp || startChan >= endChan || startSeg >= endSeg)
        return FALSE; // No intersection

    out->baseSamp = startSamp;
    out->sampCnt = endSamp - startSamp;

    out->baseChan = startChan;
    out->chanCnt = endChan - startChan;

    out->baseSeg = startSeg;
    out->segCnt = endSeg - startSeg;

    return TRUE;
}

void fade_in_audio_region_f32(amxAudio a, amxAudioPeriod* p)
{
    // Fade-In (Linear)

    afxUnit N = p->sampCnt;

    for (afxUnit seg = 0; seg < p->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < p->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < N; ++samp)
            {
                float* sample = (float*)get_sample_ptr(a, p->baseSeg + seg, p->baseChan + chan, p->baseSamp + samp);
                float t = (float)samp / (N - 1);
                *sample *= t;
            }
        }
    }
}

void fade_out_audio_region_f32(amxAudio a, amxAudioPeriod* p)
{
    // Fade-Out (Linear)
    afxUnit N = p->sampCnt;

    for (afxUnit seg = 0; seg < p->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < p->chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < N; ++samp)
            {
                float* sample = (float*)get_sample_ptr(a, p->baseSeg + seg, p->baseChan + chan, p->baseSamp + samp);
                float t = 1.0f - (float)samp / (N - 1);
                *sample *= t;
            }
        }
    }
}

void downmix_audio_region_to_mono_f32(amxAudio dst, amxAudioPeriod* dstP, amxAudio src, amxAudioPeriod* srcP)
{
    // Downmix to Mono (float32)
    // Convert a multi-channel region (like stereo) into a single-channel version by averaging or weighted mixing.
    // This works on a region and stores the mono output into a 1-channel destination audio buffer.
    // We can modify this to apply weighting if needed (e.g., L*0.6 + R*0.4 for stereo).

    assert(dstP->chanCnt == 1);
    assert(dstP->sampCnt == srcP->sampCnt);
    assert(dstP->segCnt == srcP->segCnt);

    afxUnit N = srcP->sampCnt;

    for (afxUnit seg = 0; seg < srcP->segCnt; ++seg)
    {
        for (afxUnit samp = 0; samp < N; ++samp)
        {
            float sum = 0.0f;
            for (afxUnit chan = 0; chan < srcP->chanCnt; ++chan)
            {
                float* srcSamp = (float*)get_sample_ptr(src, srcP->baseSeg + seg, srcP->baseChan + chan, srcP->baseSamp + samp);
                sum += *srcSamp;
            }

            float* dstSamp = (float*)get_sample_ptr(dst, dstP->baseSeg + seg, dstP->baseChan + /*chan*/0, dstP->baseSamp + samp);
            *dstSamp = sum / (float)(srcP->chanCnt); // simple average
        }
    }
}

void resample_audio_region_linear_f32(amxAudio dst, amxAudioPeriod* dstP, float dstRate, amxAudio src, amxAudioPeriod* srcP, float srcRate)
{
    // Resample Audio Region (float32, linear interpolation)
    // Resample audio to a new sample rate using simple linear interpolation.
    // Basic resampling from src->freq to a target sample rate, using linear interpolation.

    float rateRatio = srcRate / dstRate;

    for (afxUnit seg = 0; seg < srcP->segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < srcP->chanCnt; ++chan)
        {
            for (afxUnit i = 0; i < dstP->sampCnt; ++i)
            {
                float srcIndex = i * rateRatio;
                afxUnit index0 = (afxUnit)srcIndex;
                afxUnit index1 = index0 + 1;
                float frac = srcIndex - index0;

                if (index1 >= srcP->sampCnt) index1 = srcP->sampCnt - 1;

                float* s0 = (float*)get_sample_ptr(src, srcP->baseSeg + seg, srcP->baseChan + chan, srcP->baseSamp + index0);
                float* s1 = (float*)get_sample_ptr(src, srcP->baseSeg + seg, srcP->baseChan + chan, srcP->baseSamp + index1);
                float* dstSamp = (float*)get_sample_ptr(dst, dstP->baseSeg + seg, dstP->baseChan + chan, dstP->baseSamp + i);

                *dstSamp = *s0 * (1.0f - frac) + *s1 * frac;
            }
        }
    }
}

void upload_buffer_to_audio_f32(amxAudio a, amxAudioIo* io, const float* buffer)
{
    // Upload from Buffer
    // Assume a float* buffer with deinterleaved layout and we want to write it into an audio object using audio_io.
    // This can easily be adapted for interleaved buffers or int16 formats.

    afxUnit sampleSize = sizeof(float);

    for (afxUnit seg = 0; seg < io->period.segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < io->period.chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < io->period.sampCnt; ++samp)
            {
                // Destination in audio object
                float* dst = (float*)get_sample_ptr(a, io->period.baseSeg + seg, io->period.baseChan + chan, io->period.baseSamp + samp);

                // Calculate source index
                afxUnit srcIndex =
                    seg * io->period.chanCnt * io->samplesPerChan +
                    chan * io->samplesPerChan +
                    samp;

                const float* src = (const float*)((const uint8_t*)buffer + io->offset + srcIndex * sampleSize);
                *dst = *src;
            }
        }
    }
}

void dump_audio_to_buffer_f32(amxAudio a, amxAudioIo* io, float* buffer)
{
    // Dump to Buffer (e.g., for saving or upload)

    afxUnit sampleSize = sizeof(float);

    for (afxUnit seg = 0; seg < io->period.segCnt; ++seg)
    {
        for (afxUnit chan = 0; chan < io->period.chanCnt; ++chan)
        {
            for (afxUnit samp = 0; samp < io->period.sampCnt; ++samp)
            {
                float* src = (float*)get_sample_ptr(a, io->period.baseSeg + seg, io->period.baseChan + chan, io->period.baseSamp + samp);

                afxUnit dstIndex =
                    seg * io->period.chanCnt * io->samplesPerChan +
                    chan * io->samplesPerChan +
                    samp;

                float* dst = (float*)((uint8_t*)buffer + io->offset + dstIndex * sampleSize);
                *dst = *src;
            }
        }
    }
}

void pack_audio_region_f32(const amxAudio a, const amxAudioIo* io, void* dstBuffer)
{
    // Writes audio data from audio object into a memory buffer with custom layout.

    float* dst = (float*)((uint8_t*)dstBuffer + io->offset);

    for (afxUnit seg = 0; seg < io->period.segCnt; ++seg)
    {
        for (afxUnit samp = 0; samp < io->period.sampCnt; ++samp)
        {
            for (afxUnit chan = 0; chan < io->period.chanCnt; ++chan)
            {
                float* src = (float*)get_sample_ptr(a, io->period.baseSeg + seg, io->period.baseChan + chan, io->period.baseSamp + samp);

                afxUnit sampleIndex = (
                    seg * io->samplesPerChan * io->chansPerFrame +
                    samp * io->chansPerFrame +
                    chan
                    );

                dst[sampleIndex] = *src;
            }
        }
    }
}

void unpack_audio_region_f32(amxAudio a, const amxAudioIo* io, const void* srcBuffer)
{
    // Reads audio from a buffer into a region of the audio object.

    const float* src = (const float*)((const uint8_t*)srcBuffer + io->offset);

    for (afxUnit seg = 0; seg < io->period.segCnt; ++seg)
    {
        for (afxUnit samp = 0; samp < io->period.sampCnt; ++samp)
        {
            for (afxUnit chan = 0; chan < io->period.chanCnt; ++chan)
            {
                afxUnit sampleIndex = (
                    seg * io->samplesPerChan * io->chansPerFrame +
                    samp * io->chansPerFrame +
                    chan
                    );

                float* dst = (float*)get_sample_ptr(a, io->period.baseSeg + seg, io->period.baseChan + chan, io->period.baseSamp + samp);
                *dst = src[sampleIndex];
            }
        }
    }
}

afxBool write_audio_io_region_to_file_f32(amxAudio a, amxAudioIo* io, afxStream out)
{
    if (AfxSeekStream(out, (long)io->offset, afxSeekOrigin_BEGIN) != 0)
        return FALSE;

    for (afxUnit seg = 0; seg < io->period.segCnt; ++seg)
    {
        for (afxUnit samp = 0; samp < io->period.sampCnt; ++samp)
        {
            for (afxUnit chan = 0; chan < io->period.chanCnt; ++chan)
            {
                float* src = (float*)get_sample_ptr(a, io->period.baseSeg + seg, io->period.baseChan + chan, io->period.baseSamp + samp);

                afxUnit sampleIndex = (
                    seg * io->samplesPerChan * io->chansPerFrame +
                    samp * io->chansPerFrame +
                    chan
                    );

                long filePos = io->offset + sampleIndex * sizeof(float);

                if (AfxSeekStream(out, filePos, afxSeekOrigin_BEGIN) != 0)
                    return FALSE;

                if (AfxWriteStream(out, sizeof(float), 0, src))
                    return FALSE;
            }
        }
    }

    return TRUE;
}

afxBool read_audio_io_region_from_file_f32(amxAudio a, amxAudioIo* io, afxStream in)
{
    if (AfxSeekStream(in, (long)io->offset, afxSeekOrigin_BEGIN) != 0)
        return FALSE;

    for (afxUnit seg = 0; seg < io->period.segCnt; ++seg)
    {
        for (afxUnit samp = 0; samp < io->period.sampCnt; ++samp)
        {
            for (afxUnit chan = 0; chan < io->period.chanCnt; ++chan)
            {
                float* dst = (float*)get_sample_ptr(a, io->period.baseSeg + seg, io->period.baseChan + chan, io->period.baseSamp + samp);

                afxUnit sampleIndex = (
                    seg * io->samplesPerChan * io->chansPerFrame +
                    samp * io->chansPerFrame +
                    chan
                    );

                long filePos = io->offset + sampleIndex * sizeof(float);

                if (AfxSeekStream(in, filePos, afxSeekOrigin_BEGIN) != 0)
                    return FALSE;

                if (AfxReadStream(in , sizeof(float), 0, dst))
                    return FALSE;
            }
        }
    }
    return TRUE;
}

afxBool audio_apply_layout(amxAudio a, amxAudioLayout desiredLayout, afxUnit baseSeg, afxUnit segCnt)
{
    // Data-level layout normalization operation, performed within the audio object itself, 
    // to make the buffer's internal layout match a preferred structure (e.g. deinterleaved/planar layout).
    
    // The function assumes segment-major layout: segments contain frames of [channels × samples].
    
    afxSize const tempBufSize = 4096;
    afxByte tempBuf[4096]; // 16x 256

    if (!a || !tempBuf || segCnt == 0) return FALSE;
    if (baseSeg + segCnt > a->segCnt) return FALSE;

    afxUnit chanCnt = a->chanCnt;
    afxUnit sampCnt = a->sampCnt;
    afxSize sampleSize = a->fmtBps / 8;

    if (chanCnt == 1 || a->fmtStride == chanCnt && desiredLayout == amxAudioLayout_PLANAR)
        return TRUE; // already planar
    if (a->fmtStride == 1 && desiredLayout == amxAudioLayout_INTERLEAVED)
        return TRUE; // already interleaved

    // Determine max sample frames that fit into tempBuf
    afxSize frameBytes = chanCnt * sampleSize;
    afxUnit maxFramesPerPass = (tempBufSize / frameBytes);
    if (maxFramesPerPass == 0) return FALSE; // buffer too small even for 1 frame

    uint8_t* temp = (uint8_t*)tempBuf;

    for (afxUnit seg = baseSeg; seg < baseSeg + segCnt; ++seg)
    {
        afxUnit remainingFrames = sampCnt;
        afxUnit baseFrame = 0;

        while (remainingFrames > 0) {
            afxUnit passFrames = (remainingFrames > maxFramesPerPass) ? maxFramesPerPass : remainingFrames;

            // Process layout
            if (desiredLayout == amxAudioLayout_PLANAR)
            {
                // Interleaved >> Planar
                for (afxUnit chan = 0; chan < chanCnt; ++chan)
                {
                    for (afxUnit f = 0; f < passFrames; ++f)
                    {
                        afxSize srcIndex = ((seg * sampCnt + baseFrame + f) * chanCnt + chan);
                        afxSize dstIndex = chan * passFrames + f;

                        AfxCopy(&temp[dstIndex * sampleSize],
                            (uint8_t*)get_sample_ptr(a, 0, 0, 0) + srcIndex * sampleSize,
                            sampleSize);
                    }
                }

                // Copy rearranged block back to destination
                for (afxUnit chan = 0; chan < chanCnt; ++chan)
                {
                    for (afxUnit f = 0; f < passFrames; ++f)
                    {
                        afxSize dstIndex = ((seg * chanCnt + chan) * sampCnt + baseFrame + f);
                        afxSize srcIndex = chan * passFrames + f;

                        AfxCopy((uint8_t*)get_sample_ptr(a, 0, 0, 0) + dstIndex * sampleSize,
                            &temp[srcIndex * sampleSize],
                            sampleSize);
                    }
                }
            }
            else if (desiredLayout == amxAudioLayout_INTERLEAVED)
            {
                // Planar >> Interleaved
                for (afxUnit chan = 0; chan < chanCnt; ++chan)
                {
                    for (afxUnit f = 0; f < passFrames; ++f)
                    {
                        afxSize srcIndex = ((seg * chanCnt + chan) * sampCnt + baseFrame + f);
                        afxSize dstIndex = f * chanCnt + chan;

                        AfxCopy(&temp[dstIndex * sampleSize],
                            (uint8_t*)get_sample_ptr(a, 0, 0, 0) + srcIndex * sampleSize,
                            sampleSize);
                    }
                }

                // Copy back to interleaved layout
                for (afxUnit f = 0; f < passFrames; ++f)
                {
                    for (afxUnit chan = 0; chan < chanCnt; ++chan)
                    {
                        afxSize srcIndex = f * chanCnt + chan;
                        afxSize dstIndex = ((seg * sampCnt + baseFrame + f) * chanCnt + chan);

                        AfxCopy((uint8_t*)get_sample_ptr(a, 0, 0, 0) + dstIndex * sampleSize,
                            &temp[srcIndex * sampleSize],
                            sampleSize);
                    }
                }
            }

            baseFrame += passFrames;
            remainingFrames -= passFrames;
        }
    }

    // Update layout metadata
    a->fmtStride = (desiredLayout == amxAudioLayout_PLANAR) ? chanCnt : 1;
    return TRUE;
}


_AMX afxError _AmxSpu_ResampleI16I16(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op)
{
    afxError err = { 0 };

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AFX_MIN(outputLen, dst->sampCnt - op->dst.baseSamp);

    for (afxUnit seg = 0; seg < op->dst.segCnt; seg++)
    {
        afxUnit segIdx = seg + op->dst.baseSeg;

        // Loop over the channels for the destination data
        for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
        {
            afxUnit chIdx = ch + op->dst.baseChan;

            afxReal32* dstData = get_sample_ptr(dst, segIdx, chIdx, op->dst.baseSamp);

            // Loop over the destination channels only
            // Check if the current channel is within the source's channel count
            if (chIdx >= op->src.baseChan &&
                chIdx < op->src.baseChan + op->src.chanCnt)
            {
                afxReal32 const* srcData = get_sample_ptr(src, op->src.baseSeg + seg, op->src.baseChan + ch, op->src.baseSamp);

                // Now loop over the samples for each channel
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    // Calculate corresponding index in the source data
                    afxReal srcIndex = i / rateRatio;
                    afxUnit indexFloor = (afxUnit)srcIndex;
                    afxUnit indexCeil = (indexFloor + 1 < op->src.sampCnt) ? indexFloor + 1 : indexFloor;

                    // Linear interpolation weights
                    afxReal weightCeil = srcIndex - indexFloor;
                    afxReal weightFloor = 1.0f - weightCeil;

                    // Calculate source sample indices based on base sample and channel index
                    afxUnit srcIndexFloor = (indexFloor);
                    afxUnit srcIndexCeil = (indexCeil);

                    // Perform linear interpolation and store in destination (cast to int16)
                    afxInt16 resultSample = (afxInt16)(weightFloor * srcData[srcIndexFloor] + weightCeil * srcData[srcIndexCeil]);

                    // Store the result in the destination
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = resultSample;
                }
            }
            else
            {
                // If the destination channel does not have a corresponding source channel, just zero it out
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = 0;  // Zero out missing channels
                }
            }
        }
    }
    return err;
}

_AMX afxError _AmxSpu_ResampleF32F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op)
{
    afxError err = { 0 };

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AFX_MIN(outputLen, dst->sampCnt - op->dst.baseSamp);

    for (afxUnit seg = 0; seg < op->dst.segCnt; seg++)
    {
        afxUnit segIdx = seg + op->dst.baseSeg;

        // Loop over the channels for the destination data
        for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
        {
            afxUnit chIdx = ch + op->dst.baseChan;

            afxReal32* dstData = get_sample_ptr(dst, segIdx, chIdx, op->dst.baseSamp);

            // Loop over the destination channels only
            // Check if the current channel is within the source's channel count
            if (chIdx >= op->src.baseChan &&
                chIdx < op->src.baseChan + op->src.chanCnt)
            {
                afxReal32 const* srcData = get_sample_ptr(src, op->src.baseSeg + seg, op->src.baseChan + ch, op->src.baseSamp);

                // Now loop over the samples for each channel
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    // Calculate corresponding index in the source data
                    afxReal srcIndex = i / rateRatio;
                    afxUnit indexFloor = (afxUnit)srcIndex;
                    afxUnit indexCeil = (indexFloor + 1 < op->src.sampCnt) ? indexFloor + 1 : indexFloor;

                    // Linear interpolation weights
                    afxReal weightCeil = srcIndex - indexFloor;
                    afxReal weightFloor = 1.0f - weightCeil;

                    // Calculate source sample indices based on base sample and channel index
                    afxUnit srcIndexFloor = (indexFloor);
                    afxUnit srcIndexCeil = (indexCeil);

                    // Perform linear interpolation and store in destination
                    afxReal resultSample = weightFloor * srcData[srcIndexFloor] + weightCeil * srcData[srcIndexCeil];

                    // Store the result in the destination
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = resultSample;
                }
            }
            else
            {
                // If the destination channel does not have a corresponding source channel, just zero it out
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = 0;  // Zero out missing channels
                }
            }
        }
    }
    return err;
}

_AMX afxError _AmxSpu_ResampleF32I16(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op)
{
    // Float-to-Short Resampling

    afxError err = { 0 };

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AFX_MIN(outputLen, dst->sampCnt - op->dst.baseSamp);

    for (afxUnit seg = 0; seg < op->dst.segCnt; seg++)
    {
        afxUnit segIdx = seg + op->dst.baseSeg;

        // Loop over the channels for the destination data
        for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
        {
            afxUnit chIdx = ch + op->dst.baseChan;

            afxReal32* dstData = get_sample_ptr(dst, segIdx, chIdx, op->dst.baseSamp);

            // Loop over the destination channels only
            // Check if the current channel is within the source's channel count
            if (chIdx >= op->src.baseChan &&
                chIdx < op->src.baseChan + op->src.chanCnt)
            {
                afxReal32 const* srcData = get_sample_ptr(src, op->src.baseSeg + seg, op->src.baseChan + ch, op->src.baseSamp);

                // Now loop over the samples for each channel
                for (afxInt i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    // Calculate corresponding index in the source data
                    afxReal srcIndex = i / rateRatio;
                    afxUnit indexFloor = (afxUnit)srcIndex;
                    afxUnit indexCeil = (indexFloor + 1 < op->src.sampCnt) ? indexFloor + 1 : indexFloor;

                    // Linear interpolation weights
                    afxReal weightCeil = srcIndex - indexFloor;
                    afxReal weightFloor = 1.0f - weightCeil;

                    // Calculate source sample indices based on base sample and channel index
                    afxUnit srcIndexFloor = (indexFloor);
                    afxUnit srcIndexCeil = (indexCeil);

                    // Perform linear interpolation, then scale from float to short
                    afxReal resultSample = weightFloor * srcData[srcIndexFloor] + weightCeil * srcData[srcIndexCeil];
                    afxInt16 shortResult = (afxInt16)fminf(fmaxf(resultSample * 32768.0f, -32768.0f), 32767.0f);  // Clip to short range

                    // Store the result in the destination
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = shortResult;
                }
            }
            else
            {
                // If the destination channel does not have a corresponding source channel, just zero it out
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = 0;  // Zero out missing channels
                }
            }
        }
    }
    return err;
}

_AMX afxError _AmxSpu_ResampleI16F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op)
{
    // Short-to-Float Resampling

    afxError err = { 0 };

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AFX_MIN(outputLen, dst->sampCnt - op->dst.baseSamp);

    for (afxUnit seg = 0; seg < op->dst.segCnt; seg++)
    {
        afxUnit segIdx = seg + op->dst.baseSeg;

        // Loop over the channels for the destination data
        for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
        {
            afxUnit chIdx = ch + op->dst.baseChan;

            afxReal32* dstData = get_sample_ptr(dst, segIdx, chIdx, op->dst.baseSamp);

            // Loop over the destination channels only
            // Check if the current channel is within the source's channel count
            if (chIdx >= op->src.baseChan &&
                chIdx < op->src.baseChan + op->src.chanCnt)
            {
                afxReal32 const* srcData = get_sample_ptr(src, op->src.baseSeg + seg, op->src.baseChan + ch, op->src.baseSamp);

                // Now loop over the samples for each channel
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    // Calculate corresponding index in the source data
                    afxReal srcIndex = i / rateRatio;
                    afxUnit indexFloor = (afxUnit)srcIndex;
                    afxUnit indexCeil = (indexFloor + 1 < op->src.sampCnt) ? indexFloor + 1 : indexFloor;

                    // Linear interpolation weights
                    afxReal weightCeil = srcIndex - indexFloor;
                    afxReal weightFloor = 1.0f - weightCeil;

                    // Calculate source sample indices based on the base sample and channel index
                    afxUnit srcIndexFloor = (indexFloor);
                    afxUnit srcIndexCeil = (indexCeil);

                    // Perform linear interpolation and scale from short to float
                    afxReal resultSample = weightFloor * (srcData[srcIndexFloor] / 32768.0f) + weightCeil * (srcData[srcIndexCeil] / 32768.0f);

                    // Store the result in the destination data, using the base sample and channel index for destination
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = resultSample;
                }
            }
            else
            {
                // If the destination channel does not have a corresponding source channel, just zero it out
                for (afxUnit i = 0; i < outputLen; i++)
                {
                    // Loop by sample for the current channel
                    afxUnit dstIndex = (i);
                    dstData[dstIndex] = 0.0f;  // Zero out missing channels
                }
            }
        }
    }
    return err;
}

_AMX afxError _AmxSpu_ResampleWave(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op)
{
    afxError err = { 0 };

    if (dst->freq == src->freq && op->dstFreq == op->srcFreq && op->dstFreq == dst->freq && src->fmt == dst->fmt)
    {
        // Just copy it.

        amxAudioCopy cop;
        _AmxSanitizeAudioPeriod(src, &op->src, &cop.src);
        cop.dstBaseChan = op->dst.baseChan;
        cop.dstBaseSamp = op->dst.baseSamp;
        _AmxSanitizeAudioCopy(src, dst, &cop, &cop);

        if (_AmxCopyAudio(src, dst, &cop))
            AfxThrowError();

        return err;
    }

    switch (src->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        switch (dst->fmt)
        {
        case amxFormat_M32f:
        case amxFormat_S32f:
        {
            if (_AmxSpu_ResampleF32F32(mpu, src, dst, op))
                AfxThrowError();
            break;
        }
        case amxFormat_S16i:
        case amxFormat_M16i:
        {
            if (_AmxSpu_ResampleF32I16(mpu, src, dst, op))
                AfxThrowError();
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        switch (dst->fmt)
        {
        case amxFormat_M32f:
        case amxFormat_S32f:
        {
            if (_AmxSpu_ResampleI16F32(mpu, src, dst, op))
                AfxThrowError();
            break;
        }
        case amxFormat_S16i:
        case amxFormat_M16i:
        {
            if (_AmxSpu_ResampleI16I16(mpu, src, dst, op))
                AfxThrowError();
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

typedef enum amxNoiseType
{
    amxNoiseType_WHITE, // aka static
    amxNoiseType_
} amxNoiseType;

typedef enum amxWaveType
{
    amxWaveType_SINE,
    amxWaveType_SQUARE,
    amxWaveType_TRIANGLE,
    amxWaveType_SAWTOOTH
} amxWaveType;

_AMX void _AmxGenerateSineWave(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a sine wave PCM signal
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        The sine wave is generated using the standard sine function. This creates a smooth oscillating waveform.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
        afxReal t = (afxReal)i / sampleRate;  // Time in seconds
        sample = (afxReal)(amplitude * sin(2 * AFX_PI * freq * t));  // Sine wave formula
        out[i] = sample;
    }
}

_AMX void _AmxGenerateSquareWave(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a square wave PCM signal
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        The square wave alternates between two levels: +AMPLITUDE and -AMPLITUDE. 
        The i % (SAMPLE_RATE / FREQUENCY) part controls the period of the wave, and the comparison 
        i < (SAMPLE_RATE / (2 * FREQUENCY)) decides whether the output should be positive or negative.
    */

    //afxInt sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
#if 0
        {
            afxReal t = (afxReal)i / sampleRate;  // Time in seconds
            sample = (t * freq) - floor(t * freq) < 0.5 ? -amplitude : amplitude; // square wave formula
            out[i] = sample;
        }
#endif
        {
            // Square wave equation: 1 or -1 depending on the phase of the sine wave
            out[i] = (i % (afxInt)(sampleRate / freq) < (sampleRate / (2 * freq))) ? amplitude : -amplitude;
        }
    }
}

_AMX void _AmxGenerateTriangleWave(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a triangle wave PCM signal
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        The triangle wave is constructed using an absolute value of a linear ramp. 
        The ramp starts at 0, rises to its peak, then falls back down in a linear fashion. 
        The equation generates values that increase and decrease symmetrically.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];
    
    for (int i = 0; i < num_samples; ++i)
    {
        {
            afxReal t = (afxReal)i / sampleRate;  // Time in seconds
            sample = amplitude * (2 * fabsf(2 * (t * freq - floor(t * freq + 0.5))) - 1); // triangle wave formula
            out[i] = sample;
        }
#if 0
        {
            // Triangle wave: using absolute value of sine wave for linear ramp
            afxReal t = (afxReal)i / sampleRate;
            afxReal period = 1.0 / freq;
            afxReal ramp = fabs((t / period) - 2 * floor((t / period) + 0.5));
            out[i] = (amplitude * (1 - ramp));
        }
#endif
    }
}

_AMX void _AmxGenerateSawtoothWave(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a sawtooth wave PCM signal
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        The sawtooth wave rises linearly from -AMPLITUDE to +AMPLITUDE and then resets. 
        The ramp equation is adjusted to fit between -1 and +1 and then scaled by AMPLITUDE.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
        // Sawtooth wave: linear ramp between -AMPLITUDE and +AMPLITUDE
        afxReal t = (afxReal)i / sampleRate;
        afxReal period = 1.0 / freq;
        afxReal ramp = 2 * (t / period - floor(t / period + 0.5));  // Ramp between -1 and +1
        out[i] = (amplitude * ramp);
    }
}

_AMX void _AmxGenerateWhiteNoise(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a static (white noise) wave PCM signal
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        Generating static noise (also known as white noise) in C involves creating random values over time 
        that mimic the irregularity of noise. In this case, we'll generate random samples within the amplitude 
        range and simulate static noise using a simple random number generator.

        White noise typically has a flat spectral density, meaning all frequencies are present equally. 
        Here's how you can generate a basic static noise or white noise signal with constant amplitude.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate a random sample in the range of -AMPLITUDE to +AMPLITUDE
        out[i] = (afxReal)((rand() % (afxInt)(2 * amplitude + 1)) - amplitude);
    }
}

_AMX void _AmxGeneratePinkNoise(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate pink noise (Voss-McCartney algorithm)
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        Pink noise has equal power per octave, meaning it has more low-frequency content. 
        It's often used in sound design and music production.

        To generate pink noise, we need to use a more advanced algorithm than simply generating 
        random numbers because it requires a specific spectral density distribution.

        Here's a basic approach using a Voss-McCartney algorithm for generating pink noise.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    afxReal b[16] =
    {
         0.02109238, 0.0431695,  0.1065703,   0.2048705, 
         0.2159787,  0.1643036,  0.0960524,   0.0407684, 
        -0.010935,  -0.0508521,  -0.0853765, -0.1174265, 
        -0.1330849, -0.1283779, -0.098332,   -0.045068
    };  // Filter coefficients
    afxReal v[16] = { 0 };
    afxReal x = 0;

    for (int i = 0; i < num_samples; ++i)
    {
        // Randomly generate noise with the filter applied

        x = 0;
        for (int j = 0; j < 16; j++)
        {
            v[j] = v[j] + ((rand() % 2 == 0) ? 1.0 : -1.0) * 2 * b[j];
            x += v[j];
        }

        // Scaling by AMPLITUDE
        out[i] = (afxReal)(x * amplitude);
    }
}

_AMX void _AmxGenerateBrownianNoise(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate brownian noise (integrated white noise)
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        Brownian noise (or red noise) has even more energy at lower frequencies compared to pink noise. 
        This type of noise can be generated by accumulating white noise (integrating white noise).
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    afxReal x = 0; // Brownian noise (integrated white noise)

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate a random step in white noise range
        x += ((rand() % (afxInt)(2 * amplitude + 1)) - amplitude);
        
        // Clamp the values to within the amplitude
        x = AFX_CLAMP(x, -amplitude, amplitude);

        // Assign to waveform
        out[i] = (afxReal)x;
    }
}

_AMX void _AmxGenerateBlueNoise(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate blue noise (Blue Noise Generation (High-Pass Filtered White Noise))
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        Blue noise has a power spectral density that increases with frequency (it's the opposite of pink noise). 
        This can be approximated by generating white noise and applying a high-pass filter to increase high-frequency content. 
        One common approach is to filter white noise with a frequency-dependent gain.
        Here's an approximation for generating blue noise.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    afxReal white_noise, blue_noise;
    afxReal prev_white_noise = 0;

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate white noise
        white_noise = (rand() % (afxInt)(2 * amplitude + 1)) - amplitude;

        // High-pass filter to simulate blue noise (increase high frequencies)
        blue_noise = white_noise - prev_white_noise;

        // Store the current white noise for next iteration
        prev_white_noise = white_noise;

        // Assign to waveform with scaling by AMPLITUDE
        out[i] = (afxReal)blue_noise;
    }
}

_AMX void _AmxGenerateVioletNoise(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate violet noise (Violet Noise Generation (Aggressive High-Pass Filtered White Noise))
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        Violet noise has even more emphasis on high frequencies compared to blue noise, 
        with a spectral density that increases with f^2. It is essentially the high-frequency counterpart of brown noise. 
        This can be achieved by applying an even more aggressive high-pass filter to the white noise.

        To generate violet noise, we can use a higher-order high-pass filter (e.g., a second-order difference filter), 
        or we can simulate it by generating white noise and applying stronger filtering.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];

    afxReal white_noise, violet_noise;
    afxReal prev_white_noise1 = 0, prev_white_noise2 = 0;

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate white noise
        white_noise = (rand() % (afxInt)(2 * amplitude + 1)) - amplitude;

        // Second-order high-pass filter to simulate violet noise (increase higher frequencies even more)
        violet_noise = white_noise - 2 * prev_white_noise1 + prev_white_noise2;

        // Store the current white noise for next iteration
        prev_white_noise2 = prev_white_noise1;
        prev_white_noise1 = white_noise;

        // Assign to waveform with scaling by AMPLITUDE
        out[i] = violet_noise;
    }
}

afxReal a_weighting_filter(afxReal freq)
// A-weighting filter coefficients for frequency domain processing
// These are approximations based on the A-weighting curve used in audio engineering
{
    const afxReal f1 = 20.6;
    const afxReal f2 = 12194.0;
    const afxReal f3 = 107.7;
    const afxReal f4 = 737.9;

    // A-weighting filter formula
    afxReal numerator = pow(2 * AFX_PI * freq, 2) * pow(2 * AFX_PI * freq, 2 + 20.6 * 2);
    afxReal denominator = pow(pow(2 * AFX_PI * freq, 2) + f1, 2) * (pow(2 * AFX_PI * freq, 2) + f2);
    return numerator / denominator;
}

_AMX void _AmxGenerateGrayNoise(afxReal32* out, afxUnit sampleCnt, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate gray noise with true A-weighting filter
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);

    /*
        Gray noise is a perceptually uniform noise, which compensates for the sensitivity of the human ear to different frequencies. 
        This requires applying an A-weighting curve to adjust the amplitude across different frequencies. 
        The A-weighting curve approximates human hearing sensitivity and is often used in audio testing.

        To generate gray noise, we can apply a simple weighting function (typically A(f), the A-weighting filter), but to keep things 
        simpler for this example, we’ll assume a simplistic approach: we will scale the white noise by a static factor that represents 
        the "loudness" across frequencies.

        However, for a truly perceptually uniform noise, you would need to apply a frequency-weighted filter. 
        This is more complex, so for simplicity, here's a basic approach where we simulate a perceptually weighted noise.
    */

    //afxUnit sampleRate = aud->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AFX_MIN(num_samples, sampleCnt);
    afxReal sample;

    //afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[chIdx * aud->sampCnt];
    
    afxReal white_noise, gray_noise;
    afxReal frequency, weight;

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate white noise in range [-AMPLITUDE, AMPLITUDE]
        white_noise = (rand() % (afxInt)(2 * amplitude + 1)) - amplitude;

        // Calculate frequency for the current sample (this is an approximation)
        frequency = (i * sampleRate) / (afxReal)num_samples;  // Linear frequency ramp (0 to sample_rate)

        // Apply A-weighting filter (simplified)
        weight = a_weighting_filter(frequency);

        // Apply the filter by scaling the white noise
        gray_noise = white_noise * weight;

        // Store the result in the waveform
        out[i] = gray_noise;
    }
}

_AMX afxError _AmxFillAudio(amxAudio aud, amxAudioPeriod const* op, afxReal amplitude, afxReal freq, afxReal period)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    afxUnit sampleRate = aud->freq;
    // period is the duration
    afxInt num_samples = (afxInt)(sampleRate * period);  // Total number of samples --- wavelength
    // num_samples is the wavelength (in samples)
    num_samples = AFX_MIN(num_samples, aud->sampCnt);

    afxUnit segCnt = op->segCnt;
    afxUnit chanCnt = op->chanCnt;
    afxUnit sampCnt = op->sampCnt;

    switch (aud->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[op->baseSeg * aud->chanCnt + op->baseChan * aud->sampCnt + op->baseSamp];

        for (afxUnit k = 0; k < segCnt; k++)
        {
            for (afxUnit i = 0; i < chanCnt; i++)
            {
                for (afxUnit j = 0; j < sampCnt; j++)
                {
                    afxReal t = (afxReal)j / sampleRate;  // Time in seconds
                    out[k * aud->segCnt + i * aud->sampCnt + j] = amplitude * (freq * t);

                }
            }
        }
        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16* out = &aud->buf->storage[0].hostedAlloc.f32[op->baseSeg * aud->chanCnt + op->baseChan * aud->sampCnt + op->baseSamp];

        for (afxUnit k = 0; k < segCnt; k++)
        {
            for (afxUnit i = 0; i < chanCnt; i++)
            {
                for (afxUnit j = 0; j < sampCnt; j++)
                {
                    afxReal t = (afxReal)j / sampleRate;  // Time in seconds
                    out[k * aud->segCnt + i * aud->sampCnt + j] = (afxInt16)((amplitude * (freq * t)) * 32767.0f);

                }
            }
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxCopyAudio(amxAudio src, amxAudio dst, amxAudioCopy const* op)
{
    afxError err = { 0 };

    amxAudioCopy op2;
    _AmxSanitizeAudioCopy(src, dst, op, &op2);
    afxUnit chanCnt = op2.src.chanCnt;
    afxUnit sampCnt = op2.src.sampCnt;
    afxUnit segCnt = op2.src.segCnt;

    switch (src->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &dst->buf->storage[0].hostedAlloc.f32[op->dstBaseSeg * dst->chanCnt + op2.dstBaseChan * dst->sampCnt + op2.dstBaseSamp];
        afxReal32 const* in = &src->buf->storage[0].hostedAlloc.f32[op2.src.baseSeg * src->chanCnt + op2.src.baseChan * src->sampCnt + op2.src.baseSamp];

        for (afxUnit k = 0; k < segCnt; k++)
            for (afxUnit i = 0; i < chanCnt; i++)
                for (afxUnit j = 0; j < sampCnt; j++)
                    out[k * dst->segCnt + i * dst->sampCnt + j] = in[k * src->segCnt + i * src->sampCnt + j];

        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16* out = &dst->buf->storage[0].hostedAlloc.i16[op->dstBaseSeg * dst->chanCnt + op2.dstBaseChan * dst->sampCnt + op2.dstBaseSamp];
        afxInt16 const* in = &src->buf->storage[0].hostedAlloc.i16[op2.src.baseSeg * src->chanCnt + op2.src.baseChan * src->sampCnt + op2.src.baseSamp];

        for (afxUnit k = 0; k < segCnt; k++)
            for (afxUnit i = 0; i < chanCnt; i++)
                for (afxUnit j = 0; j < sampCnt; j++)
                    out[k * dst->segCnt + i * dst->sampCnt + j] = in[k * src->segCnt + i * src->sampCnt + j];

        break;
    }
    default: break;
    }
    return err;
}

_AMX afxError _AmxTransposeAudio(amxAudio src, amxAudio dst, amxAudioCopy const* op)
{
    afxError err = { 0 };

    /*
        This code implements the transposition of the audio data layout
        from [chanCnt][sampCnt] to [sampCnt][chanCnt].
        The transposition function calculates the correct offsets and copies data accordingly.
    */

    amxAudioCopy op2;
    _AmxSanitizeAudioCopy(src, dst, op, &op2);
    afxUnit chanCnt = op2.src.chanCnt;
    afxUnit sampCnt = op2.src.sampCnt;
    afxUnit segCnt = op2.src.segCnt;

    // srcAudio is [chanCnt][sampCnt] and dstAudio is [sampCnt][chanCnt]

    switch (src->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &dst->buf->storage[0].hostedAlloc.f32[op->dstBaseSeg * dst->chanCnt + op2.dstBaseChan * dst->sampCnt + op2.dstBaseSamp];
        afxReal32 const* in = &src->buf->storage[0].hostedAlloc.f32[op2.src.baseSeg * src->chanCnt + op2.src.baseChan * src->sampCnt + op2.src.baseSamp];

        for (afxUnit k = 0; k < segCnt; k++)
            for (afxUnit i = 0; i < chanCnt; i++)
                for (afxUnit j = 0; j < sampCnt; j++)
                    out[k * dst->segCnt + i * dst->sampCnt + j] = in[k * src->segCnt + i * src->sampCnt + j];

        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16* out = &dst->buf->storage[0].hostedAlloc.i16[op->dstBaseSeg * dst->chanCnt + op2.dstBaseChan * dst->sampCnt + op2.dstBaseSamp];
        afxInt16 const* in = &src->buf->storage[0].hostedAlloc.i16[op2.src.baseSeg * src->chanCnt + op2.src.baseChan * src->sampCnt + op2.src.baseSamp];

        for (afxUnit k = 0; k < segCnt; k++)
            for (afxUnit i = 0; i < chanCnt; i++)
                for (afxUnit j = 0; j < sampCnt; j++)
                    out[k * dst->segCnt + i * dst->sampCnt + j] = in[k * src->segCnt + i * src->sampCnt + j];

        break;
    }
    default: break;
    }
    return err;
}

_AMX afxError _AmxDumpAudio(amxAudio aud, amxAudioIo const* op, void* dst)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(dst);
    AFX_ASSERT(op);

    amxAudioIo op2;
    _AmxSanitizeAudioPeriod(aud, &op->period, &op2.period);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;

    afxUnit segCnt = op2.period.segCnt;
    afxUnit chanCnt = op2.period.chanCnt;
    afxUnit sampCnt = op2.period.sampCnt;

    switch (aud->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        afxReal32* out = ((afxByte*)(dst)+op2.offset);
        afxReal32 const* in = &aud->buf->storage[0].hostedAlloc.f32[aud->sampCnt * op2.period.baseChan + op2.period.baseSamp];

        for (int k = 0; k < segCnt; k++)
            for (int i = 0; i < chanCnt; i++)
                for (int j = 0; j < sampCnt; j++)
                    out[k * op2.chansPerFrame + i * op2.samplesPerChan + j] = in[k * aud->chanCnt + i * aud->sampCnt + j];

        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16* out = (void*)dst;
        afxInt16 const* in = (void*)aud->buf->storage[0].hostedAlloc.i16;

        for (int k = 0; k < segCnt; k++)
            for (int i = 0; i < chanCnt; i++)
                for (int j = 0; j < sampCnt; j++)
                    out[k * op2.chansPerFrame + i * op2.samplesPerChan + j] = in[k * aud->chanCnt + i * aud->sampCnt + j];

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxUpdateAudio(amxAudio aud, amxAudioIo const* op, void const* src)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(src);
    AFX_ASSERT(op);

    amxAudioIo op2;
    _AmxSanitizeAudioPeriod(aud, &op->period, &op2.period);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;
    afxUnit segCnt = op2.period.segCnt;
    afxUnit chanCnt = op2.period.chanCnt;
    afxUnit sampCnt = op2.period.sampCnt;

    switch (aud->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[aud->sampCnt * op2.period.baseChan + op2.period.baseSamp];
        afxReal32 const* in = ((afxByte*)(src)+op2.offset);

        for (int k = 0; k < segCnt; k++)
            for (int i = 0; i < chanCnt; i++)
                for (int j = 0; j < sampCnt; j++)
                    out[k * aud->chanCnt + i * aud->sampCnt + j] = in[k * op2.chansPerFrame + i * op2.samplesPerChan + j];

        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16* out = &aud->buf->storage[0].hostedAlloc.i16[aud->sampCnt * op2.period.baseChan + op2.period.baseSamp];
        afxInt16 const* in = ((afxByte*)(src)+op2.offset);

        for (int k = 0; k < segCnt; k++)
            for (int i = 0; i < chanCnt; i++)
                for (int j = 0; j < sampCnt; j++)
                    out[k * aud->chanCnt + i * aud->sampCnt + j] = in[k * op2.chansPerFrame + i * op2.samplesPerChan + j];

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxDownloadAudio(amxAudio aud, amxAudioIo const* op, afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(iob);
    AFX_ASSERT(op);

    amxAudioIo op2;
    _AmxSanitizeAudioPeriod(aud, &op->period, &op2.period);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;
    afxUnit chanCnt = op2.period.chanCnt;
    afxUnit sampCnt = op2.period.sampCnt;
    afxUnit segCnt = op2.period.segCnt;

    AfxSeekStream(iob, op2.offset, afxSeekOrigin_BEGIN);

    switch (aud->fmt)
    {
    case amxFormat_M32f:
    case amxFormat_S32f:
    {
        afxReal32 const* in = &aud->buf->storage[0].hostedAlloc.f32[aud->sampCnt * op2.period.baseChan + op2.period.baseSamp];

        for (int k = 0; k < segCnt; k++)
            for (int i = 0; i < chanCnt; i++)
                for (int j = 0; j < sampCnt; j++)
                    AfxWriteStream(iob, sizeof(in[0]), 0, &in[k * op2.chansPerFrame + i * op2.samplesPerChan + j]);

        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16 const* in = (void*)aud->buf->storage[0].hostedAlloc.i16;

        for (int k = 0; k < segCnt; k++)
            for (int i = 0; i < chanCnt; i++)
                for (int j = 0; j < sampCnt; j++)
                    AfxWriteStream(iob, sizeof(in[0]), 0, &in[k * op2.chansPerFrame + i * op2.samplesPerChan + j]);

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxUploadAudio(amxAudio aud, amxAudioIo const* op, afxStream iob)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &aud);
    AFX_ASSERT(iob);
    AFX_ASSERT(op);

    amxAudioIo op2;
    _AmxSanitizeAudioPeriod(aud, &op->period, &op2.period);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;
    afxUnit chanCnt = op2.period.chanCnt;
    afxUnit sampCnt = op2.period.sampCnt;
    afxUnit segCnt = op2.period.segCnt;

    AfxSeekStream(iob, op2.offset, afxSeekOrigin_BEGIN);

    switch (aud->fmt)
    {
    case amxFormat_S32f:
    case amxFormat_M32f:
    {
        afxReal32* out = &aud->buf->storage[0].hostedAlloc.f32[aud->sampCnt * op2.period.baseChan + op2.period.baseSamp];

        for (int k = 0; k < segCnt; k++)
        {
            for (int i = 0; i < chanCnt; i++)
            {
                for (int j = 0; j < sampCnt; j++)
                {
                    AfxSeekStream(iob, op2.offset + ((i * op2.samplesPerChan + j) * sizeof(out[0])), afxSeekOrigin_BEGIN),
                        AfxReadStream(iob, sizeof(out[0]), 0, &out[i * aud->sampCnt + j]);
                }
            }
        }
        break;
    }
    case amxFormat_S16i:
    case amxFormat_M16i:
    {
        afxInt16* out = &aud->buf->storage[0].hostedAlloc.i16[aud->sampCnt * op2.period.baseChan + op2.period.baseSamp];

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                AfxSeekStream(iob, op2.offset + ((i * op2.samplesPerChan + j) * sizeof(out[0])), afxSeekOrigin_BEGIN),
                AfxReadStream(iob, sizeof(out[0]), 0, &out[i * aud->sampCnt + j]);

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}
