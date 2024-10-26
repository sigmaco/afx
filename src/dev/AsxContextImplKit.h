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

#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/sound/dev/afxSoundSystem.h"

#ifdef _ASX_WAVEFORM_C
#ifdef _ASX_WAVEFORM_IMPL
AFX_OBJECT(_asxWaveform)
#else
AFX_OBJECT(afxWaveform)
#endif
{
    afxUnit          bps; // bits per sample
    afxUnit          freq; // the sample rate, the number of sample frames played per second.
    afxUnit          sampleCnt; // its length, meaning the number of sample frames inside the buffer.
    afxUnit          chanCnt; // the number of channels (1 for mono, 2 for stereo, etc.).

    void*           udd;
    afxUnit          bufCap; // with any alignment
    afxByte*        bytemap;
    afxUnit          mappedOffset;
    afxUnit          mappedRange;
    afxFlags        mappedFlags;
};
#endif

ASX afxClassConfig const _AsxWavStdImplementation;

#endif//ASX_CONTEXT_IMPL_KIT_H
