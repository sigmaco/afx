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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef ASX_CONTEXT_IMPL_KIT_H
#define ASX_CONTEXT_IMPL_KIT_H

#include "../dev/afxDevCoreBase.h"
#include "qwadro/sound/dev/afxSoundSystem.h"

#ifdef _ASX_WAVEFORM_C
#ifdef _ASX_WAVEFORM_IMPL
AFX_OBJECT(_asxWaveform)
#else
AFX_OBJECT(afxWaveform)
#endif
{
    afxNat          bps; // bits per sample
    afxNat          freq; // the sample rate, the number of sample frames played per second.
    afxNat          sampleCnt; // its length, meaning the number of sample frames inside the buffer.
    afxNat          chanCnt; // the number of channels (1 for mono, 2 for stereo, etc.).

    void*           udd;
    afxNat          bufCap; // with any alignment
    afxByte*        bytemap;
    afxNat          mappedOffset;
    afxNat          mappedRange;
    afxFlags        mappedFlags;
};
#endif

ASX afxClassConfig const _AsxWavStdImplementation;

#endif//ASX_CONTEXT_IMPL_KIT_H
