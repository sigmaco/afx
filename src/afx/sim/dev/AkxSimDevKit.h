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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AKX_SIM_DEV_KIT_H
#define AKX_SIM_DEV_KIT_H

#include "qwadro/sim/dev/afxSimulation.h"
#include "AkxCadDevKit.h"
#include "AkxSpatialDevKit.h"

#ifdef _AKX_SIMULATION_C
AFX_OBJECT(afxSimulation)
{
    afxMmu          genrlMem;
    afxChain        classes;
    afxClass      bodies;
    afxClass      motors;
    afxClass      lights;
    afxClass      materials;
    afxClass      meshes;
    afxClass      meshDatas;
    afxClass      topologies;
    afxClass      models;
    afxClass      skeletons;
    afxClass      poseMgr;
    afxClass      posbMgr;
    afxClass      curMgr;
    afxClass      motMgr;
    afxClass      aniMgr;
    afxClass      nodes;
    afxClass      renderers;
    afxClass      terCls;

    afxV4d          right;
    afxV4d          up;
    afxV4d          back;
    afxM3d          basis;
    afxV4d          origin;
    afxBox         extent;
    afxReal         unitsPerMeter;
    afxReal         allowedLodErrFadingFactor;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    akxMotive       globalMotorInterlinks;

    afxStringBase   strbMdlSklMotUrns;
    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;

    afxDrawContext  dctx;
};
#endif//_AKX_SIMULATION_C

AKX afxReal _AkxGetAllowedLodErrorFadingFactor(afxSimulation sim);

#endif//AKX_SIM_DEV_KIT_H
