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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AMX_IMPL___AUDIO_H
#define AMX_IMPL___AUDIO_H

#include "amxImpl_Executor.h"

AMX afxError _AmxFillWave(afxAudio wav, afxWaveInterval const* op, afxReal amplitude, afxReal freq, afxReal period);
AMX afxError _AmxCopyAudio(afxAudio src, afxAudio dst, afxWaveCopy const* op);
AMX afxError _AmxTransposeAudio(afxAudio src, afxAudio dst, afxWaveCopy const* op);
AMX afxError _AmxDumpAudio(afxAudio wav, afxWaveIo const* op, void* dst);
AMX afxError _AmxUpdateAudio(afxAudio wav, afxWaveIo const* op, void const* src);
AMX afxError _AmxUploadAudio(afxAudio wav, afxWaveIo const* op, afxStream iob);
AMX afxError _AmxDownloadAudio(afxAudio wav, afxWaveIo const* op, afxStream iob);

AMX void _AmxSanitizeWaveInterval(afxAudio wav, afxWaveInterval const* raw, afxWaveInterval* san);
AMX void _AmxSanitizeWaveCopy(afxAudio src, afxAudio dst, afxWaveCopy const* raw, afxWaveCopy* san);

AMX afxError _AmxSpu_ResampleI16I16(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op);
AMX afxError _AmxSpu_ResampleF32F32(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op);
AMX afxError _AmxSpu_ResampleF32I16(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op);
AMX afxError _AmxSpu_ResampleI16F32(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op);
AMX afxError _AmxSpu_ResampleWave(amxMpu* mpu, afxAudio src, afxAudio dst, afxWaveResample const* op);

AMX void _AmxGenerateSineWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateSquareWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateTriangleWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);

#endif//AMX_IMPL___AUDIO_H
