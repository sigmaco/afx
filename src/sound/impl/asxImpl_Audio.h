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

#ifndef ASX_IMPL___AUDIO_H
#define ASX_IMPL___AUDIO_H

#include "asxImpl_Executor.h"

ASX afxError _AsxFillWave(afxAudio wav, afxWaveInterval const* op, afxReal amplitude, afxReal freq, afxReal period);
ASX afxError _AsxCopyAudio(afxAudio src, afxAudio dst, afxWaveCopy const* op);
ASX afxError _AsxTransposeAudio(afxAudio src, afxAudio dst, afxWaveCopy const* op);
ASX afxError _AsxDumpAudio(afxAudio wav, afxWaveIo const* op, void* dst);
ASX afxError _AsxUpdateAudio(afxAudio wav, afxWaveIo const* op, void const* src);
ASX afxError _AsxUploadAudio(afxAudio wav, afxWaveIo const* op, afxStream iob);
ASX afxError _AsxDownloadAudio(afxAudio wav, afxWaveIo const* op, afxStream iob);

ASX void _AsxSanitizeWaveInterval(afxAudio wav, afxWaveInterval const* raw, afxWaveInterval* san);
ASX void _AsxSanitizeWaveCopy(afxAudio src, afxAudio dst, afxWaveCopy const* raw, afxWaveCopy* san);

ASX afxError _AsxSpu_ResampleI16I16(asxSpu* spu, afxAudio src, afxAudio dst, afxWaveResample const* op);
ASX afxError _AsxSpu_ResampleF32F32(asxSpu* spu, afxAudio src, afxAudio dst, afxWaveResample const* op);
ASX afxError _AsxSpu_ResampleF32I16(asxSpu* spu, afxAudio src, afxAudio dst, afxWaveResample const* op);
ASX afxError _AsxSpu_ResampleI16F32(asxSpu* spu, afxAudio src, afxAudio dst, afxWaveResample const* op);
ASX afxError _AsxSpu_ResampleWave(asxSpu* spu, afxAudio src, afxAudio dst, afxWaveResample const* op);

ASX void _AsxGenerateSineWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);
ASX void _AsxGenerateSquareWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);
ASX void _AsxGenerateTriangleWave(afxAudio wav, afxUnit chIdx, afxReal amplitude, afxReal freq, afxReal dur);

#endif//ASX_IMPL___AUDIO_H
