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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SKY_H
#define AFX_SKY_H

#include "afx/sim/afxNode.h"
#include "afx/core/afxArray.h"
#include "afx/draw/afxColor.h"

typedef enum afxSkyType
{
    afxSkyType_BOX,
    afxSkyType_DOME,
} afxSkyType;

AFX_DEFINE_STRUCT(afxSky)
{
    afxSkyType      type;
    afxBuffer       cube;
    afxTexture      cubemap;
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
    afxSampler      smp;
};

AFX afxError    AfxBuildSkybox(afxSky* sky, afxSimulation sim);

AFX void        AfxStepSky(afxSky* sky, afxReal dt);

AFX afxError    AfxDrawSky(afxDrawScript dscr, afxSky* sky);

#endif//AFX_SKY_H