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

typedef enum akxSkyType
{
    akxSkyType_BOX,
    akxSkyType_DOME,
} akxSkyType;

AFX_DEFINE_STRUCT(akxSky)
{
    akxSkyType      type;
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
    afxDrawTechnique skyDtec;
    avxVertexInput  skyVin;
    avxSampler      smp;
};

AMX afxError    AfxBuildSkybox(akxSky* sky, afxSimulation sim, afxDrawInput din);

AMX void        AfxStepSky(akxSky* sky, afxReal dt);

AMX afxError    AfxDrawSky(avxCmdb cmdb, akxSky* sky);

#endif//AFX_SKY_H
