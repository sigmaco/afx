/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

/// PCM
/// The Pulse-Code Modulation file is a digital representation of analogue signals. 
/// It is an uncompressed and lossless audio format and serves as a standard file format for CDs and DVDs. 
/// PCM files work by converting waveforms into digital bits, they do this by sampling analogue sounds and recording them at certain intervals or pulses. 
/// This means PCM files have a sampling rate and bit depth, where sampling rate refers to how often a sound sample is made and bit depth is the number of bits used to represent each sample.

// Waveform --- a curve showing the shape of a wave at a given time.

// All audio buffers are uncompressed PCM with 32 - bit floating - point samples.
// Internally, all audio buffers are stored deinterleaved for performance reasons.

/*
    (Audio)Buffer
    (Audio)Buffer represents storage for short-term data particles. 
    Temporary data is stored in the buffer until it is processed and transferred to another location within the system.
*/

// A sample is a single value representing the value of the audio stream at each specific moment in time within a particular channel (left or right, if in the case of stereo).
// A frame, or sample frame, is the set of all values for all channels that will play at a specific moment in time: 
// all the samples of all the channels that play at the same time (two for a stereo sound, six for 5.1, etc.).

// The sample rate is the quantity of those samples (or frames, since all samples of a frame play at the same time) that will play in one second, measured in Hz. 
// The higher the sample rate, the better the sound quality.

// Channels should be always separated to ease multithreaded work.

/*
    A sample is the smallest usable quantum of digital audio.
    The term frame isn't formally defined in pure audio terms, but is often used in relation to video that may accompany an audio track.
    In that context a frame is the quantity of audio samples taken during a video frame interval.

    If there are 30 video frames per second and 48000 audio samples per second,
    you could say that there are 48000 / 30 = 1600 samples per frame, or that an "audio frame" contains 1600 samples,
    but there literally is no audio frame, it's just a convention for talking about audio with respect to a video frame rate.
*/

/*
    An audio sample contains amplitude (loudness) information at that particular point in time.
    To produce sound, tens of thousands of samples are played in sequence to produce frequencies.
*/

/*
    Representing audio in digital form involves a number of steps and processes,
    with multiple formats available both for the raw audio and the encoded or compressed audio which is actually used on the web.

    Computers, however, are digital.
    In order to represent a sound wave in a way computers can manipulate and work with (let alone transmit over a network),
    the sound has to be converted into a digital form. This process is called analog to digital conversion (A/D for short).

    The first factor affecting the fidelity of the captured audio is the audio bandwidth; that is,
    the range of audio frequencies the A/D converter is capable of capturing and converting into digital form.
    The audio bandwidth is also affected by the codec, if it chooses to discard any frequency bands while encoding the sound.

    Sound enters the computer through a microphone or other input in the form of a stream of electrons whose voltage varies
    to represent the amplitude of the sound wave. This analog signal is then converted into digital form by a circuit that
    captures the incoming wave's amplitude at regular intervals, converting that data into a number in a form that is understood
    by the audio recording system. Each of these captured moments is a sample.
    By chaining all the samples together, you can approximately represent the original wave, as seen in the diagram below.

    Audio data format and structure
    At the most basic level, audio is represented by a stream of samples, each specifying the amplitude of the audio waveform as
    measured for a given slice of the overall waveform of the audio signal. There are several formats used for the individual samples
    within an audio file. Most audio files use 16-bit signed integers for each sample, but others use 32-bit floating-point values or
    24-bit or 32-bit integers. Some older audio file formats—which you won't find in use on the web—used 8-bit integer samples.
    In addition, samples may use signed or unsigned values, as well. The size of an individual sample is called the sample size.

    The position of each audio source within the audio signal is called a channel. Each channel contains a sample indicating the
    amplitude of the audio being produced by that source at a given moment in time. For instance, in stereo sound, there are two
    audio sources: one speaker on the left, and one on the right. Each of these is represented by one channel, and the number of
    channels contained in the audio signal is called the channel count.

    While recording or generating multi-channel audio files, the channels are assembled into a series of audio frames, each
    consisting of one sample for each of the audio's channels. An individual sample is a numeric value representing the
    amplitude of the sound waveform at a single moment in time, and may be represented in various formats.

    Stereo audio is probably the most commonly used channel arrangement in web audio, and 16-bit samples are used for the majority
    of day-to-day audio in use today. For 16-bit stereo audio, each sample taken from the analog signal is recorded as two 16-bit
    integers, one for the left channel and one for the right. That means each sample requires 32 bits of memory. At the common
    sample rate of 48 kHz (48,000 samples per second), this means each second of audio occupies 192 kB of memory. Therefore, a
    typical three-minute song requires about 34.5 MB of memory. That's a lot of storage, but worse, it's an insane amount of
    network bandwidth to use for a relatively short piece of audio. That's why most digital audio is compressed.

    Audio channels and frames
    There are two types of audio channel. Standard audio channels are used to present the majority of the audible sound.
    The sound for the left and right main channels, as well as all of your surround sound speakers (center, left and right rear,
    left and right sides, ceiling channels, and so forth) are all standard audio channels. Special Low Frequency Enhancement (LFE)
    channels provide the signal for special speakers designed to produce the low frequency sounds and vibration to create a visceral
    sensation when listening to the audio. The LFE channels typically drive subwoofers and similar devices.

    Monophonic audio has one channel, stereo sound has two channels, 5.1 surround sound has 6 channels (five standard and one LFE),
    and so forth. Each audio frame is a data record that contains the samples for all of the channels available in an audio signal.
    The size of an audio frame is calculated by multiplying the sample size in bytes by the number of channels, so a single frame of
    stereo 16-bit audio is 4 bytes long and a single frame of 5.1 floating-point audio is 24 (4 bytes per sample multiplied by 6 channels).
*/

/*
    The Qwadro handles 32-bit float audio data with the 3D layout of float data[frameCnt][chanCnt][sampCnt].
    The data is copied frame-by-frame, and channel-by-channel, sample-by-sample based on the provided transfer parameters.
    This approach can be used for various audio manipulation tasks, such as mixing, processing, or routing audio data in real-time systems.

    The layout is 3D, but it’s essentially describing how to access a linear block of memory.
    This implies that the audio data is stored in a single continuous block, and the 3D indices
    (frame, channel, sample) are used to calculate an offset into that block.

    Memory Layout in a Single Allocation:
    The 3D array you described (float data[frameCnt][chanCnt][sampCnt]) is stored in a single contiguous block,
    and we can use a formula to address any element in the array:

    data[frameIdx][chanIdx][sampIdx]

    Where:
        frameCnt is the number of frames (playable intervals),
        chanCnt is the number of channels (e.g., 2 for stereo),
        sampCnt is the number of samples (time slots for each channel in a frame).

    Given that the data is stored contiguously,
    the layout would be flattened into a 1D array,
    and we need to compute the appropriate index into this array using the given dimensions.

    Formula for Addressing:
        frameCnt: The number of frames (playable intervals).
        chanCnt: The number of channels (e.g., 2 for stereo).
        sampCnt: The number of samples (time slots).

    Since the data is stored in a single contiguous block (a 1D array), we need to address the
    data using the 3D layout data[frameCnt][chanCnt][sampCnt]. The formula to compute the offset
    for any element (frameIdx, chanIdx, sampIdx) in the flat data array is:

        offset = frameIdx * (chanCnt * sampCnt) + chanIdx * sampCnt + sampIdx

    Where:
        frameIdx is the frame index (0 to frameCnt-1),
        chanIdx is the channel index (0 to chanCnt-1),
        sampIdx is the sample index (0 to sampCnt-1).
*/

#ifndef AMX_AUDIO_H
#define AMX_AUDIO_H

#include "qwadro/mix/afxMixDefs.h"

typedef enum amxAudioUsage
{
    amxAudioUsage_SAMPLED = AFX_BITMASK(0),
    amxAudioUsage_MIX = AFX_BITMASK(1),
    amxAudioUsage_SINK = AFX_BITMASK(2)
} amxAudioUsage;

typedef enum amxAudioFlag
{
    amxAudioFlag_RESERVED = AFX_BITMASK(31)
} amxAudioFlags;

AFX_DEFINE_STRUCT(amxAudioInfo)
{
    amxFormat       fmt; // type and bits per sample
    afxUnit         freq; // the sample rate, the number of sample frames played per second. (SPF)
    afxUnit         sampCnt; // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // the number of channels (1 for mono, 2 for stereo, etc.).
    afxUnit         segCnt;
    afxUnit         siz;

    afxString       tag;
    void*           udd;
};

AFX_DEFINE_STRUCT(amxAudioPeriod)
{
    afxUnit         clipIdx;

    // The base (starting) index for samples in the audio data block.
    // It specifies where the sample data begins within the block.
    afxUnit         baseSamp; // X

    // The base (starting) index for channels.
    // It determines where the channel data begins in the audio structure.
    afxUnit         baseChan; // Y

    afxUnit         baseSeg;

    // The count of samples to be handled. 
    // This determines how many samples are involved in the operation, starting from the baseSamp position.
    afxUnit         sampCnt; // W
    
    // The count of channels to be handled. 
    // It specifies how many audio channels are involved, starting from the baseChan position.
    afxUnit         chanCnt; // H

    afxUnit         segCnt;

    afxUnit x, y, z;
    afxUnit w, h, d;
};

AFX_DEFINE_STRUCT(amxAudioCopy)
{
    amxAudioPeriod  src;
    afxUnit         dstBaseSamp; // X
    afxUnit         dstBaseChan; // Y
    afxUnit         dstBaseSeg; // Z
};

AFX_DEFINE_STRUCT(amxAudioInterference)
{
    amxAudioPeriod  src;
    afxUnit         srcFreq;
    amxAudioPeriod  dst;
    afxUnit         dstFreq;
};

AFX_DEFINE_STRUCT(amxAudioIo)
/// Especificação de operação de transferência arbitrária de afxWaveform.
{
    amxAudioPeriod  period;

    // An additional offset (in bytes) that might be used in input/output operations 
    // to locate data more precisely within a buffer or stream.
    afxSize         offset;
    
    // The number of samples per channel in the upload buffer. 
    // This helps in calculating the memory layout when accessing the upload buffer.
    afxUnit         samplesPerChan;
    
    // The number of rows in the upload buffer per frame in the audio object. 
    // This helps to understand how many sample rows exist in the buffer per frame for multi-channel layouts.
    afxUnit         chansPerFrame;

    // freq?
    //amxFormat       fmt;
};

AMX amxBuffer   AmxGetAudioBuffer(amxAudio aud);

/// Returns a real representing the duration, in seconds, of the PCM data stored in the buffer.
AMX afxReal     AmxGetAudioDuration(amxAudio aud);

AMX void        AmxDescribeAudio(amxAudio aud, amxAudioInfo* desc);

AMX afxError    AmxPackAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], amxBuffer buf, avxFence signal);
AMX afxError    AmxUnpackAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], amxBuffer buf, avxFence signal);

AMX afxError    AmxUploadAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], afxStream in, avxFence signal);
AMX afxError    AmxDownloadAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], afxStream out, avxFence signal);

AMX afxError    AmxDumpAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], void* dst, avxFence signal);
AMX afxError    AmxUpdateAudio(amxAudio aud, afxUnit opCnt, amxAudioIo const ops[], void const* src, avxFence signal);

AMX afxError    AmxPrintAudio(amxAudio aud, amxAudioPeriod const* op, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AmxAcquireAudios(afxMixSystem msys, afxUnit cnt, amxAudioInfo const info[], amxAudio audios[]);

AMX afxError    AmxLoadAudios(afxMixSystem msys, afxUnit cnt, afxUri const uris[], amxAudio audios[]);

#endif//AMX_AUDIO_H
