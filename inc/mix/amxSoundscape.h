/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// Soundscape

#ifndef AMX_SOUNDSCAPE_H
#define AMX_SOUNDSCAPE_H

#include "qwadro/inc/mix/afxMixDefs.h"

AMX afxError    AmxUpdateAudient(amxSoundscape snds, afxTransform const* placement, afxV3d const velocity);

AMX afxError    AmxRouteAudition(amxSoundscape snds, afxMixContext mix, afxUnit baseSmixIdx);

AMX afxError    AmxSetAuditionRenderer(amxSoundscape snds, afxUnit exuIdx);

AMX afxError    AmxTransformSounds(amxSoundscape snds, afxUnit cnt, afxTransform const t[], afxUnit soundId[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AmxAcquireSoundscape(afxMixSystem msys, amxSoundscape* soundscape);

#endif//AMX_SOUNDSCAPE_H
