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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_SKY_H
#define AFX_SKY_H

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/draw/afxColor.h"

typedef enum awxSkyType
{
    awxSkyType_BOX,
    awxSkyType_DOME,
} awxSkyType;

AFX_DEFINE_STRUCT(awxSky)
{
    awxSkyType      type;
    afxBuffer       cube;
    afxRaster      cubemap;
    afxReal         cubemapColorIntensity;
    afxV3d          fogColor;
    afxReal         fogUpper;
    afxReal         fogLower;
    afxColor        emissiveColor;
    afxColor        ambientColor;
    afxReal         currRot;
    afxReal         rotSpeed;
    afxV3d          rotPivot;
    afxQuat         rotQuat;
    afxM4d          rotMtx;
    afxPipeline     skyPip;
    afxVertexInput  skyVin;
    afxSampler      smp;
};

AKX afxError    AfxBuildSkybox(awxSky* sky, afxSimulation sim);

AKX void        AfxStepSky(awxSky* sky, afxReal dt);

AKX afxError    AfxDrawSky(afxDrawStream diob, awxSky* sky);

#endif//AFX_SKY_H