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
#define _AMX_WAVEFORM_C
#define _AMX_VOICE_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_SINK_C
#include "amxImplementation.h"

_AMX afxError _AmxSpu_ResampleI16I16(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op)
{
    afxError err = NIL;

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AfxMin(outputLen, dst->sampCnt - op->src.baseSamp);

    // Loop over the channels for the destination data
    for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
    {
        afxInt16* dstData = &dst->samples16i[dst->sampCnt * op->dst.baseChan + op->dst.baseSamp];

        // Loop over the destination channels only
        // Check if the current channel is within the source's channel count
        if (ch + op->dst.baseChan >= op->src.baseChan &&
            ch + op->dst.baseChan < op->src.baseChan + op->src.chanCnt)
        {
            afxInt16 const* srcData = &src->samples16i[src->sampCnt * op->src.baseChan + op->src.baseSamp];

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
    return err;
}

_AMX afxError _AmxSpu_ResampleF32F32(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op)
{
    afxError err = NIL;

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AfxMin(outputLen, dst->sampCnt - op->src.baseSamp);

    // Loop over the channels for the destination data
    for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
    {
        afxReal32* dstData = &dst->samples32f[dst->sampCnt * op->dst.baseChan + op->dst.baseSamp];

        // Loop over the destination channels only
        // Check if the current channel is within the source's channel count
        if (ch + op->dst.baseChan >= op->src.baseChan &&
            ch + op->dst.baseChan < op->src.baseChan + op->src.chanCnt)
        {
            afxReal32 const* srcData = &src->samples32f[src->sampCnt * op->src.baseChan + op->src.baseSamp];

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
    return err;
}

_AMX afxError _AmxSpu_ResampleF32I16(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op)
{
    // Float-to-Short Resampling

    afxError err = NIL;

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AfxMin(outputLen, dst->sampCnt - op->dst.baseSamp);

    // Loop over the channels for the destination data
    for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
    {
        afxInt16* dstData = &dst->samples16i[dst->sampCnt * op->dst.baseChan + op->dst.baseSamp];

        // Loop over the destination channels only
        // Check if the current channel is within the source's channel count
        if (ch + op->dst.baseChan >= op->src.baseChan &&
            ch + op->dst.baseChan < op->src.baseChan + op->src.chanCnt)
        {
            afxReal32 const* srcData = &src->samples32f[src->sampCnt * op->src.baseChan + op->src.baseSamp];

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
    return err;
}

_AMX afxError _AmxSpu_ResampleI16F32(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op)
{
    // Short-to-Float Resampling

    afxError err = NIL;

    // Calculate the ratio between the output and input sample rates
    afxReal rateRatio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt outputLen = (afxInt)(src->sampCnt * rateRatio);
    outputLen = AfxMin(outputLen, dst->sampCnt - op->src.baseSamp);

    // Loop over the channels for the destination data
    for (afxUnit ch = 0; ch < op->dst.chanCnt; ch++)
    {
        afxInt16* dstData = &dst->samples16i[dst->sampCnt * op->dst.baseChan + op->dst.baseSamp];

        // Loop over the destination channels only
        // Check if the current channel is within the source's channel count
        if (ch + op->dst.baseChan >= op->src.baseChan &&
            ch + op->dst.baseChan < op->src.baseChan + op->src.chanCnt)
        {
            afxReal32 const* srcData = &src->samples32f[src->sampCnt * op->src.baseChan + op->src.baseSamp];

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
    return err;
}

_AMX afxError _AmxSpu_ResampleWave(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op)
{
    afxError err = AFX_ERR_NONE;

    if (dst->freq == src->freq && op->dstFreq == op->srcFreq && op->dstFreq == dst->freq && src->fmt == dst->fmt)
    {
        // Just copy it.

        afxWaveCopy cop;
        _AmxSanitizeWaveInterval(src, &op->src, &cop.src);
        cop.dstBaseChan = op->dst.baseChan;
        cop.dstBaseSamp = op->dst.baseSamp;
        _AmxSanitizeWaveCopy(src, dst, &cop, &cop);

        if (_AmxCopyAudio(src, dst, &cop))
            AfxThrowError();

        return err;
    }

    afxReal32* out = (void*)dst->bytemap;
    afxReal32 const* in = (void*)src->bytemap;

    // Calculate the ratio between the output and input sample rates
    afxReal rate_ratio = (afxReal)op->dstFreq / op->srcFreq;
    // Calculate the number of samples in the output buffer
    afxInt output_len = (afxInt)(src->sampCnt * rate_ratio);

    // Calculate the number of frames to process
    afxInt frames = (src->sampCnt + mpu->samplesPerFrame - 1) / mpu->samplesPerFrame;  // Round up
    
                                                                                       // Resample in chunks (frames)
    for (afxInt frame = 0; frame < frames; frame++)
    {
        int frameStart = frame * mpu->samplesPerFrame;
        int frameEnd = (frame + 1) * mpu->samplesPerFrame;

        if (frameEnd > src->sampCnt)
            frameEnd = src->sampCnt;  // Handle the last chunk that may not be full

        int frameSiz = frameEnd - frameStart;
    }

    switch (src->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        switch (dst->fmt)
        {
        case amxFormat_A32f:
        case amxFormat_S32f:
        {
            if (_AmxSpu_ResampleF32F32(mpu, src, dst, op))
                AfxThrowError();
            break;
        }
        case amxFormat_S16i:
        case amxFormat_A16i:
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
    case amxFormat_A16i:
    {
        switch (dst->fmt)
        {
        case amxFormat_A32f:
        case amxFormat_S32f:
        {
            if (_AmxSpu_ResampleI16F32(mpu, src, dst, op))
                AfxThrowError();
            break;
        }
        case amxFormat_S16i:
        case amxFormat_A16i:
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

_AMX void _AmxGenerateSineWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a sine wave PCM signal
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        The sine wave is generated using the standard sine function. This creates a smooth oscillating waveform.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
        afxReal t = (afxReal)i / sampleRate;  // Time in seconds
        sample = (afxReal)(amplitude * sin(2 * AFX_PI * freq * t));  // Sine wave formula
        out[i] = sample;
    }
}

_AMX void _AmxGenerateSquareWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a square wave PCM signal
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        The square wave alternates between two levels: +AMPLITUDE and -AMPLITUDE. 
        The i % (SAMPLE_RATE / FREQUENCY) part controls the period of the wave, and the comparison 
        i < (SAMPLE_RATE / (2 * FREQUENCY)) decides whether the output should be positive or negative.
    */

    afxInt sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

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

_AMX void _AmxGenerateTriangleWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a triangle wave PCM signal
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        The triangle wave is constructed using an absolute value of a linear ramp. 
        The ramp starts at 0, rises to its peak, then falls back down in a linear fashion. 
        The equation generates values that increase and decrease symmetrically.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];
    
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

_AMX void _AmxGenerateSawtoothWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a sawtooth wave PCM signal
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        The sawtooth wave rises linearly from -AMPLITUDE to +AMPLITUDE and then resets. 
        The ramp equation is adjusted to fit between -1 and +1 and then scaled by AMPLITUDE.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
        // Sawtooth wave: linear ramp between -AMPLITUDE and +AMPLITUDE
        afxReal t = (afxReal)i / sampleRate;
        afxReal period = 1.0 / freq;
        afxReal ramp = 2 * (t / period - floor(t / period + 0.5));  // Ramp between -1 and +1
        out[i] = (amplitude * ramp);
    }
}

_AMX void _AmxGenerateWhiteNoise(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate a static (white noise) wave PCM signal
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        Generating static noise (also known as white noise) in C involves creating random values over time 
        that mimic the irregularity of noise. In this case, we'll generate random samples within the amplitude 
        range and simulate static noise using a simple random number generator.

        White noise typically has a flat spectral density, meaning all frequencies are present equally. 
        Here's how you can generate a basic static noise or white noise signal with constant amplitude.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate a random sample in the range of -AMPLITUDE to +AMPLITUDE
        out[i] = (afxReal)((rand() % (afxInt)(2 * amplitude + 1)) - amplitude);
    }
}

_AMX void _AmxGeneratePinkNoise(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate pink noise (Voss-McCartney algorithm)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        Pink noise has equal power per octave, meaning it has more low-frequency content. 
        It's often used in sound design and music production.

        To generate pink noise, we need to use a more advanced algorithm than simply generating 
        random numbers because it requires a specific spectral density distribution.

        Here's a basic approach using a Voss-McCartney algorithm for generating pink noise.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

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

_AMX void _AmxGenerateBrownianNoise(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate brownian noise (integrated white noise)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        Brownian noise (or red noise) has even more energy at lower frequencies compared to pink noise. 
        This type of noise can be generated by accumulating white noise (integrating white noise).
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

    afxReal x = 0; // Brownian noise (integrated white noise)

    for (int i = 0; i < num_samples; ++i)
    {
        // Generate a random step in white noise range
        x += ((rand() % (afxInt)(2 * amplitude + 1)) - amplitude);
        
        // Clamp the values to within the amplitude
        x = AfxClamp(x, -amplitude, amplitude);

        // Assign to waveform
        out[i] = (afxReal)x;
    }
}

_AMX void _AmxGenerateBlueNoise(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate blue noise (Blue Noise Generation (High-Pass Filtered White Noise))
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        Blue noise has a power spectral density that increases with frequency (it's the opposite of pink noise). 
        This can be approximated by generating white noise and applying a high-pass filter to increase high-frequency content. 
        One common approach is to filter white noise with a frequency-dependent gain.
        Here's an approximation for generating blue noise.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

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

_AMX void _AmxGenerateVioletNoise(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate violet noise (Violet Noise Generation (Aggressive High-Pass Filtered White Noise))
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

    /*
        Violet noise has even more emphasis on high frequencies compared to blue noise, 
        with a spectral density that increases with f^2. It is essentially the high-frequency counterpart of brown noise. 
        This can be achieved by applying an even more aggressive high-pass filter to the white noise.

        To generate violet noise, we can use a higher-order high-pass filter (e.g., a second-order difference filter), 
        or we can simulate it by generating white noise and applying stronger filtering.
    */

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];

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

_AMX void _AmxGenerateGrayNoise(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur)
// Function to generate gray noise with true A-weighting filter
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);

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

    afxUnit sampleRate = wav->freq;

    int num_samples = (int)(sampleRate * dur);  // Total number of samples --- wavelength
    num_samples = AfxMin(num_samples, wav->sampCnt);
    afxReal sample;

    afxReal32* out = &wav->samples32f[chIdx * wav->sampCnt];
    
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

_AMX afxError _AmxFillWave(afxAudio wav, afxWaveInterval const* op, afxReal amplitude, afxReal freq, afxReal period)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    afxUnit sampleRate = wav->freq;
    // period is the duration
    afxInt num_samples = (afxInt)(sampleRate * period);  // Total number of samples --- wavelength
    // num_samples is the wavelength (in samples)
    num_samples = AfxMin(num_samples, wav->sampCnt);

    switch (wav->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &wav->samples32f[op->baseChan * wav->sampCnt + op->baseSamp];

        for (afxInt i = 0; i < num_samples; ++i)
        {
            afxReal t = (afxReal)i / sampleRate;  // Time in seconds
            out[i] = amplitude * (freq * t);
        }
        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16* out = &wav->samples16i[op->baseChan * wav->sampCnt + op->baseSamp];

        for (afxInt i = 0; i < num_samples; ++i)
        {
            afxReal t = (afxReal)i / sampleRate;  // Time in seconds
            out[i] = (afxInt16)(amplitude * (freq * t));
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxCopyAudio(afxAudio src, afxAudio dst, afxWaveCopy const* op)
{
    afxError err = AFX_ERR_NONE;

    afxWaveCopy op2;
    _AmxSanitizeWaveCopy(src, dst, op, &op2);

    switch (src->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &dst->samples32f[dst->sampCnt * op2.dstBaseChan + op2.dstBaseSamp];
        afxReal32 const* in = &src->samples32f[src->sampCnt * op2.src.baseChan + op2.src.baseSamp];

        afxUnit chanCnt = op2.src.chanCnt;
        afxUnit sampCnt = op2.src.sampCnt;

        for (afxUnit i = 0; i < chanCnt; i++)
            for (afxUnit j = 0; j < sampCnt; j++)
                out[i * dst->sampCnt + j] = in[i * src->sampCnt + j];
        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16* out = &dst->samples32f[dst->sampCnt * op2.dstBaseChan + op2.dstBaseSamp];
        afxInt16 const* in = &src->samples32f[src->sampCnt * op2.src.baseChan + op2.src.baseSamp];

        afxUnit chanCnt = op2.src.chanCnt;
        afxUnit sampCnt = op2.src.sampCnt;

        for (afxUnit i = 0; i < chanCnt; i++)
            for (afxUnit j = 0; j < sampCnt; j++)
                out[i * dst->sampCnt + j] = in[i * src->sampCnt + j];
        break;
    }
    default: break;
    }
    return err;
}

_AMX afxError _AmxTransposeAudio(afxAudio src, afxAudio dst, afxWaveCopy const* op)
{
    afxError err = AFX_ERR_NONE;

    /*
        This code implements the transposition of the audio data layout
        from [chanCnt][sampCnt] to [sampCnt][chanCnt].
        The transposition function calculates the correct offsets and copies data accordingly.
    */

    afxWaveCopy op2;
    _AmxSanitizeWaveCopy(src, dst, op, &op2);

    // srcAudio is [chanCnt][sampCnt] and dstAudio is [sampCnt][chanCnt]

    switch (src->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &dst->samples32f[dst->sampCnt * op2.dstBaseChan + op2.dstBaseSamp];
        afxReal32 const* in = &src->samples32f[src->sampCnt * op2.src.baseChan + op2.src.baseSamp];

        afxUnit chanCnt = op2.src.chanCnt;
        afxUnit sampCnt = op2.src.sampCnt;

        for (afxUnit i = 0; i < chanCnt; i++)
            for (afxUnit j = 0; j < sampCnt; j++)
                out[j * dst->chanCnt + i] = in[i * src->sampCnt + j];
        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16* out = &dst->samples32f[dst->sampCnt * op2.dstBaseChan + op2.dstBaseSamp];
        afxInt16 const* in = &src->samples32f[src->sampCnt * op2.src.baseChan + op2.src.baseSamp];

        afxUnit chanCnt = op2.src.chanCnt;
        afxUnit sampCnt = op2.src.sampCnt;

        for (afxUnit i = 0; i < chanCnt; i++)
            for (afxUnit j = 0; j < sampCnt; j++)
                out[j * dst->chanCnt + i] = in[i * src->sampCnt + j];
        break;
    }
    default: break;
    }
    return err;
}

_AMX afxError _AmxDumpAudio(afxAudio wav, afxWaveIo const* op, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(dst);
    AFX_ASSERT(op);

    afxWaveIo op2;
    _AmxSanitizeWaveInterval(wav, &op->seg, &op2.seg);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;

    switch (wav->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        afxReal32* out = ((afxByte*)(dst)+op2.offset);
        afxReal32 const* in = &wav->samples32f[wav->sampCnt * op2.seg.baseChan + op2.seg.baseSamp];

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                out[i * op2.samplesPerChan + j] = in[i * wav->sampCnt + j];

        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16* out = (void*)dst;
        afxInt16 const* in = (void*)wav->bytemap;

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                out[i * op2.samplesPerChan + j] = in[i * wav->sampCnt + j];

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxUpdateAudio(afxAudio wav, afxWaveIo const* op, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(src);
    AFX_ASSERT(op);

    afxWaveIo op2;
    _AmxSanitizeWaveInterval(wav, &op->seg, &op2.seg);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;

    switch (wav->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        afxReal32* out = &wav->samples32f[wav->sampCnt * op2.seg.baseChan + op2.seg.baseSamp];
        afxReal32 const* in = ((afxByte*)(src)+op2.offset);

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                out[i * wav->sampCnt + j] = in[i * op2.samplesPerChan + j];

        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16* out = &wav->samples32f[wav->sampCnt * op2.seg.baseChan + op2.seg.baseSamp];
        afxInt16 const* in = ((afxByte*)(src)+op2.offset);

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                out[i * wav->sampCnt + j] = in[i * op2.samplesPerChan + j];

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxDownloadAudio(afxAudio wav, afxWaveIo const* op, afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(iob);
    AFX_ASSERT(op);

    afxWaveIo op2;
    _AmxSanitizeWaveInterval(wav, &op->seg, &op2.seg);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;

    AfxSeekStream(iob, op2.offset, afxSeekOrigin_BEGIN);

    switch (wav->fmt)
    {
    case amxFormat_A32f:
    case amxFormat_S32f:
    {
        afxReal32 const* in = &wav->samples32f[wav->sampCnt * op2.seg.baseChan + op2.seg.baseSamp];

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                AfxWriteStream(iob, sizeof(in[0]), 0, &in[i * op2.samplesPerChan + j]);

        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16 const* in = (void*)wav->bytemap;

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                AfxWriteStream(iob, sizeof(in[0]), 0, &in[i * op2.samplesPerChan + j]);

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError _AmxUploadAudio(afxAudio wav, afxWaveIo const* op, afxStream iob)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_WAV, 1, &wav);
    AFX_ASSERT(iob);
    AFX_ASSERT(op);

    afxWaveIo op2;
    _AmxSanitizeWaveInterval(wav, &op->seg, &op2.seg);
    op2.offset = op->offset;
    op2.samplesPerChan = op->samplesPerChan;
    op2.chansPerFrame = op->chansPerFrame;

    AfxSeekStream(iob, op2.offset, afxSeekOrigin_BEGIN);

    switch (wav->fmt)
    {
    case amxFormat_S32f:
    case amxFormat_A32f:
    {
        afxReal32* out = &wav->samples32f[wav->sampCnt * op2.seg.baseChan + op2.seg.baseSamp];

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                AfxSeekStream(iob, op2.offset + ((i * op2.samplesPerChan + j) * sizeof(out[0])), afxSeekOrigin_BEGIN),
                AfxReadStream(iob, sizeof(out[0]), 0, &out[i * wav->sampCnt + j]);

        break;
    }
    case amxFormat_S16i:
    case amxFormat_A16i:
    {
        afxInt16* out = &wav->samples32f[wav->sampCnt * op2.seg.baseChan + op2.seg.baseSamp];

        afxUnit chanCnt = op2.seg.chanCnt;
        afxUnit sampCnt = op2.seg.sampCnt;

        for (int i = 0; i < chanCnt; i++)
            for (int j = 0; j < sampCnt; j++)
                AfxSeekStream(iob, op2.offset + ((i * op2.samplesPerChan + j) * sizeof(out[0])), afxSeekOrigin_BEGIN),
                AfxReadStream(iob, sizeof(out[0]), 0, &out[i * wav->sampCnt + j]);

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}
