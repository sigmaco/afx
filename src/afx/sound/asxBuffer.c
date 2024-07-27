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

#define _ASX_SOUND_C
#define _ASX_BUFFER_C
#include "dev/AsxDevKit.h"

_ASX afxNat AsxCountWaveChannels(asxBuffer wav)
{
    return wav->chanCnt;
}

_ASX afxNat AsxGetWaveLength(asxBuffer wav)
{
    return wav->sampleCnt;
}

_ASX afxReal AsxGetWaveFrequency(asxBuffer wav)
{
    return wav->freq;
}

_ASX afxReal AsxGetWaveDuration(asxBuffer wav)
{
    return wav->sampleCnt / wav->freq;
}
