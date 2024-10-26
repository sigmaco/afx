/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

/// PCM
/// The Pulse-Code Modulation file is a digital representation of analogue signals. 
/// It is an uncompressed and lossless audio format and serves as a standard file format for CDs and DVDs. 
/// PCM files work by converting waveforms into digital bits, they do this by sampling analogue sounds and recording them at certain intervals or pulses. 
/// This means PCM files have a sampling rate and bit depth, where sampling rate refers to how often a sound sample is made and bit depth is the number of bits used to represent each sample.

// Waveform --- a curve showing the shape of a wave at a given time.

#ifndef ASX_BUFFER_H
#define ASX_BUFFER_H

#include "qwadro/inc/sound/afxSoundDefs.h"

// A sample is a single value representing the value of the audio stream at each specific moment in time within a particular channel (left or right, if in the case of stereo).
// A frame, or sample frame, is the set of all values for all channels that will play at a specific moment in time: 
// all the samples of all the channels that play at the same time (two for a stereo sound, six for 5.1, etc.).

// The sample rate is the quantity of those samples (or frames, since all samples of a frame play at the same time) that will play in one second, measured in Hz. 
// The higher the sample rate, the better the sound quality.

AFX_DEFINE_HANDLE(afxWaveform);

AFX_DEFINE_STRUCT(asxWaveInfo)
{
    afxUnit          bps; // bits per sample
    afxUnit          freq; // the sample rate, the number of sample frames played per second. (SPF)
    afxUnit          sampleCnt; // its length, meaning the number of sample frames inside the buffer.
    afxUnit          chanCnt; // the number of channels (1 for mono, 2 for stereo, etc.).
    void const*     src;
    void*           udd;
};

AFX_DEFINE_STRUCT(afxWaveIo)
/// Especificação de operação de transferência arbitrária de afxWaveform.
{
    afxUnit              baseCh; // Y
    afxUnit              chCnt; // H layers?
    afxUnit              baseSamp; // Z
    afxUnit              sampCnt; // D
    afxUnit              atT; // X
    afxUnit              dur; // W

    afxSize             offset;
    afxUnit              rowStride;
    afxUnit              rowCnt;
};

/// Returns a float representing the sample rate, in samples per second, of the PCM data stored in the buffer.
ASX afxReal     AsxGetWaveFrequency(afxWaveform wav);

/// Returns an integer representing the length, in sample-frames, of the PCM data stored in the buffer.
ASX afxUnit      AsxGetWaveLength(afxWaveform wav);

/// Returns a real representing the duration, in seconds, of the PCM data stored in the buffer.
ASX afxReal     AsxGetWaveDuration(afxWaveform wav);

/// Returns an integer representing the number of discrete audio channels described by the PCM data stored in the buffer.
ASX afxUnit      AsxCountWaveChannels(afxWaveform wav);

ASX void*       AsxMapWave(afxWaveform wav);
ASX void        AsxUnmapWave(afxWaveform wav);

/// The channel number of the current buffer to copy the channel data to.
/// An optional offset to copy the data to.
ASX afxError    AsxUpdateWave(afxWaveform wav, afxUnit chanIdx, afxUnit base, afxUnit len, void const* src);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AsxAcquireWaves(afxSoundContext sctx, afxUnit cnt, asxWaveInfo const info[], afxWaveform buffers[]);

#endif//ASX_BUFFER_H
