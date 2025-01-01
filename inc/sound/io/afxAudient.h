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

#ifndef ASX_AUDIENT_H
#define ASX_AUDIENT_H

#include "qwadro/inc/sound/afxSoundDefs.h"

ASX afxError    AfxAdjustAudition(afxAudient audi, afxTransform const* placement, afxV3d const velocity);

ASX afxError    AfxRouteAudition(afxAudient audi, afxMixer mix, afxUnit baseSmixIdx);

ASX afxError    AfxSetAuditionRenderer(afxAudient audi, afxUnit exuIdx);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AfxAcquireAudient(afxSoundSystem ssys, afxAudient* audition);

#endif//ASX_AUDIENT_H
