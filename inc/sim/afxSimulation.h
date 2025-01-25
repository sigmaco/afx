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

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

#ifndef ASX_SIMULATION_H
#define ASX_SIMULATION_H

#include "qwadro/inc/sim/afxEngine.h"
#include "qwadro/inc/sim/akxLighting.h"

#include "qwadro/inc/sim/io/afxAnimation.h"
#include "qwadro/inc/sim/afxCapstan.h"
#include "qwadro/inc/sim/io/afxMotion.h"
#include "qwadro/inc/sim/akxRenderer.h"

//#include "qwadro/inc/cad/afxSkeleton.h"
#include "qwadro/inc/sim/io/afxCurve.h"

#include "qwadro/inc/sim/io/afxModel.h"
#include "qwadro/inc/sim/body/afxMaterial.h"

#include "qwadro/inc/sim/op/afxSampleContext.h"
#include "qwadro/inc/sim/io/afxScene.h"

AFX_DEFINE_STRUCT(afxSimulationConfig)
{
    afxSimBridgeConfig  prime;
    afxUnit             auxCnt;
    afxSimBridgeConfig* auxs;

    afxEngineFeatures   requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxBox              extent;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;

    afxDrawSystem       dsys;
    afxDrawInput        din;

};

ASX afxModule       AfxGetSimulationIcd(afxSimulation sim);

// DRAW BRIDGES AND QUEUES.

ASX afxUnit         AfxGetSimBridges(afxSimulation sim, afxUnit baseExuIdx, afxUnit cnt, afxSimBridge bridges[]);
ASX afxUnit         AfxQuerySimBridges(afxSimulation sim, afxUnit sengId, afxUnit portId, afxUnit first, afxUnit cnt, afxSimBridge bridges[]);



ASX afxError        AfxStepSimulation(afxSimulation sim);

ASX afxError        AfxWaitForSimBridge(afxSimulation sim, afxUnit exuIdx, afxTime timeout);
ASX afxError        AfxWaitForSimulation(afxSimulation sim, afxTime timeout);

ASX afxUnit         AfxRecenterAllCapstanClocks(afxSimulation sim, afxReal dCurrentClock);

ASX void            AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

ASX void            AfxComputeSimilarity(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv);


ASX afxDrawSystem   AfxGetSimulationDrawSystem(afxSimulation sim);
ASX afxDrawInput    AfxGetSimulationDrawInput(afxSimulation sim);

ASX afxUnit         AfxEnumerateBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBody bodies[]);
ASX afxUnit         AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(afxBody, void*), void *udd);

ASX afxError        AfxRollSimCommands(afxSimulation sim, asxSubmission* ctrl, afxUnit cnt, afxContext contexts[]);

ASX afxError        AfxCullBodies(afxSimulation sim, afxFrustum const* f, afxArray* pvs);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxConfigureSimulation(afxUnit sengId, afxSimulationConfig* cfg);

ASX afxError        AfxEstablishSimulation(afxUnit sengId, afxSimulationConfig const* cfg, afxSimulation* simulation);

#endif//ASX_SIMULATION_H
