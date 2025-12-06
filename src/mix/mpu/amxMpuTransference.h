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

#ifndef AMX_IMPL___TRANSFERENCE_H
#define AMX_IMPL___TRANSFERENCE_H

#include "amxMpuExecutor.h"

typedef enum amxAudioLayout
{
    amxAudioLayout_INTERLEAVED = 0,
    amxAudioLayout_PLANAR = 1 // Deinterleaved
} amxAudioLayout;

AMX afxBool audio_apply_layout(amxAudio a, amxAudioLayout desiredLayout, afxUnit baseSeg, afxUnit segCnt);

AMX afxError _AmxFillAudio(amxAudio aud, amxAudioPeriod const* op, afxReal amplitude, afxReal freq, afxReal period);
AMX afxError _AmxCopyAudio(amxAudio src, amxAudio dst, amxAudioCopy const* op);
AMX afxError _AmxTransposeAudio(amxAudio src, amxAudio dst, amxAudioCopy const* op);
AMX afxError _AmxDumpAudio(amxAudio aud, amxAudioIo const* op, void* dst);
AMX afxError _AmxUpdateAudio(amxAudio aud, amxAudioIo const* op, void const* src);
AMX afxError _AmxUploadAudio(amxAudio aud, amxAudioIo const* op, afxStream iob);
AMX afxError _AmxDownloadAudio(amxAudio aud, amxAudioIo const* op, afxStream iob);

AMX void _AmxSanitizeAudioPeriod(amxAudio aud, amxAudioPeriod const* raw, amxAudioPeriod* san);
AMX void _AmxSanitizeAudioCopy(amxAudio src, amxAudio dst, amxAudioCopy const* raw, amxAudioCopy* san);

AMX afxError _AmxMpu_ResampleI16I16(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxMpu_ResampleF32F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxMpu_ResampleF32I16(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxMpu_ResampleI16F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxMpu_ResampleWave(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxMpu_ResampleBufferedAudio(amxMpu* mpu, amxBufferedAudio const* src, amxBufferedAudio const* dst, amxAudioInterference const* op);

AMX void _AmxGenerateSineWave(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateSquareWave(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateTriangleWave(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateSawtoothWave(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateWhiteNoise(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGeneratePinkNoise(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateBrownianNoise(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateBlueNoise(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateVioletNoise(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateGrayNoise(afxReal32* out, afxUnit sampleCap, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);

AMX afxError _AmxFillBuffer(amxBuffer buf, afxSize offset, afxSize range, afxUnit stride, afxUnit sampleCnt, afxUnit sampleRate, amxFormat fmt, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxError _AmxDumpBuffer(amxBuffer buf, amxBufferIo const* op, void* dst);
AMX afxError _AmxUpdateBuffer(amxBuffer buf, amxBufferIo const* op, void const* src);
AMX afxError _AmxDownloadBuffer(amxBuffer buf, amxBufferIo const* op, afxStream iob);
AMX afxError _AmxUploadBuffer(amxBuffer buf, amxBufferIo const* op, afxStream iob);
AMX afxError _AmxCopyBuffer(amxBuffer buf, amxBufferCopy const* op, amxBuffer dst);

#endif//AMX_IMPL___TRANSFERENCE_H
