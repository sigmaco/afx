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

#ifndef AMX_SOUNDSCAPE_H
#define AMX_SOUNDSCAPE_H

#include "qwadro/mix/afxMixDefs.h"

AFX_DEFINE_STRUCT(amxSoundscapeConfig)
{
    afxFlags    flags;
    afxUnit     ambiOrder; // 1
    afxUri      orderHrirCfg;
};

AMX afxError    AmxAcquireSoundscape
(
    afxMixSystem msys, 
    amxSoundscapeConfig const*cfg, 
    amxSoundscape* soundscape
);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AmxUpdateAudient(amxSoundscape snds, afxTransform const* placement, afxV3d const velocity);

AMX afxError    AmxRouteAudition(amxSoundscape snds, afxMixContext mix, afxUnit baseSmixIdx);

AMX afxError    AmxSetAuditionRenderer(amxSoundscape snds, afxUnit exuIdx);

AMX afxError    AmxTransformSounds(amxSoundscape snds, afxUnit cnt, afxTransform const t[], afxUnit soundId[]);

#endif//AMX_SOUNDSCAPE_H
