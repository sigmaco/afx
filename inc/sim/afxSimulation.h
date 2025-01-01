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

#ifndef AMX_SIMULATION_H
#define AMX_SIMULATION_H

#include "qwadro/inc/sim/afxEngine.h"
#include "qwadro/inc/sim/akxLighting.h"

#include "qwadro/inc/sim/body/afxAnimation.h"
#include "qwadro/inc/sim/afxCapstan.h"
#include "qwadro/inc/sim/body/afxMotion.h"
#include "qwadro/inc/sim/akxRenderer.h"

//#include "qwadro/inc/cad/afxSkeleton.h"
#include "qwadro/inc/math/afxCurve.h"

#include "qwadro/inc/sim/body/afxModel.h"
#include "qwadro/inc/sim/body/afxMaterial.h"

#include "qwadro/inc/sim/op/afxSampleContext.h"

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

AMX afxModule       AfxGetSimulationIcd(afxSimulation sim);

// DRAW BRIDGES AND QUEUES.

AMX afxUnit         AfxGetSimBridges(afxSimulation sim, afxUnit baseExuIdx, afxUnit cnt, afxSimBridge bridges[]);
AMX afxUnit         AfxQuerySimBridges(afxSimulation sim, afxUnit mdevId, afxUnit portId, afxUnit first, afxUnit cnt, afxSimBridge bridges[]);



AMX afxError        AfxStepSimulation(afxSimulation sim);

AMX afxError        AfxWaitForSimBridge(afxSimulation sim, afxUnit exuIdx, afxTime timeout);
AMX afxError        AfxWaitForSimulation(afxSimulation sim, afxTime timeout);

AMX afxUnit         AfxRecenterAllCapstanClocks(afxSimulation sim, afxReal dCurrentClock);

AMX void            AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AMX void            AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv);


AMX afxDrawSystem   AfxGetSimulationDrawSystem(afxSimulation sim);
AMX afxDrawInput    AfxGetSimulationDrawInput(afxSimulation sim);

AMX afxUnit         AfxEnumerateBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBody bodies[]);
AMX afxUnit         AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(afxBody, void*), void *udd);

AMX afxError        AfxRollSimCommands(afxSimulation sim, avxSubmission* ctrl, afxUnit cnt, afxCatalyst contexts[]);

AMX afxError        AfxCullBodies(afxSimulation sim, afxFrustum const* f, afxArray* pvs);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AfxConfigureSimulation(afxUnit mdevId, afxSimulationConfig* cfg);

AMX afxError        AfxEstablishSimulation(afxUnit mdevId, afxSimulationConfig const* cfg, afxSimulation* simulation);

#endif//AMX_SIMULATION_H
