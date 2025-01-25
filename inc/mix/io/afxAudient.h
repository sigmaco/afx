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

#ifndef AMX_AUDIENT_H
#define AMX_AUDIENT_H

#include "qwadro/inc/mix/afxMixDefs.h"

AMX afxError    AfxAdjustAudition(afxAudient audi, afxTransform const* placement, afxV3d const velocity);

AMX afxError    AfxRouteAudition(afxAudient audi, afxMixContext mix, afxUnit baseSmixIdx);

AMX afxError    AfxSetAuditionRenderer(afxAudient audi, afxUnit exuIdx);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AfxAcquireAudient(afxMixSystem msys, afxAudient* audition);

#endif//AMX_AUDIENT_H
