/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

/*
    A scene is a observation of a simulation.
*/

#ifndef ARX_SKY_H
#define ARX_SKY_H

#include "qwadro/render/arxRenderable.h"

typedef enum arxSkyType
{
    arxSkyType_BOX,
    arxSkyType_DOME,
} arxSkyType;

AFX_DEFINE_STRUCT(arxSceneInfo)
{
    arxRenderware    rwe;
    arxSkyType      skyType;
};

ARX afxError    ArxAcquireScenes(arxRenderware rwe, afxUnit cnt, arxSceneInfo infos[], arxScene scenes[]);

#endif//ARX_SKY_H
