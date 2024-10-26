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

#ifndef AFX_SKY_H
#define AFX_SKY_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/afxColor.h"

typedef enum avxSkyType
{
    avxSkyType_BOX,
    avxSkyType_DOME,
} avxSkyType;

AFX_DEFINE_STRUCT(afxSceneInfo)
{
    avxSkyType  skyType;
};

AVX afxError    AfxAcquireScenes(afxDrawInput din, afxUnit cnt, afxSceneInfo infos[], afxScene scenes[]);

#endif//AFX_SKY_H
