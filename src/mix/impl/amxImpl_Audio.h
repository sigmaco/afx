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

AMX afxError _AmxFillAudio(amxAudio aud, amxAudioPeriod const* op, afxReal amplitude, afxReal freq, afxReal period);
AMX afxError _AmxCopyAudio(amxAudio src, amxAudio dst, amxAudioCopy const* op);
AMX afxError _AmxTransposeAudio(amxAudio src, amxAudio dst, amxAudioCopy const* op);
AMX afxError _AmxDumpAudio(amxAudio aud, amxAudioIo const* op, void* dst);
AMX afxError _AmxUpdateAudio(amxAudio aud, amxAudioIo const* op, void const* src);
AMX afxError _AmxUploadAudio(amxAudio aud, amxAudioIo const* op, afxStream iob);
AMX afxError _AmxDownloadAudio(amxAudio aud, amxAudioIo const* op, afxStream iob);

AMX void _AmxSanitizeAudioPeriod(amxAudio aud, amxAudioPeriod const* raw, amxAudioPeriod* san);
AMX void _AmxSanitizeAudioCopy(amxAudio src, amxAudio dst, amxAudioCopy const* raw, amxAudioCopy* san);

AMX afxError _AmxSpu_ResampleI16I16(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxSpu_ResampleF32F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxSpu_ResampleF32I16(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxSpu_ResampleI16F32(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);
AMX afxError _AmxSpu_ResampleWave(amxMpu* mpu, amxAudio src, amxAudio dst, amxAudioInterference const* op);

AMX void _AmxGenerateSineWave(amxAudio aud, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateSquareWave(amxAudio aud, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);
AMX void _AmxGenerateTriangleWave(amxAudio aud, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);

#endif//AMX_IMPL___AUDIO_H
