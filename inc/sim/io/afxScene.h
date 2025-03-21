/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

/*
    A scene is a observation of a simulation.
*/

#ifndef AFX_SKY_H
#define AFX_SKY_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"

typedef enum avxSkyType
{
    avxSkyType_BOX,
    avxSkyType_DOME,
} avxSkyType;

AFX_DEFINE_STRUCT(afxSceneInfo)
{
    afxDrawInput    din;
    avxSkyType      skyType;
};

ASX afxError    AfxAcquireScenes(afxSimulation sim, afxUnit cnt, afxSceneInfo infos[], afxScene scenes[]);

#endif//AFX_SKY_H
