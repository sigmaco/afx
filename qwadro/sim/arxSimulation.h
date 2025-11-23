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
// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

#ifndef ASX_SIMULATION_H
#define ASX_SIMULATION_H

#include "qwadro/coll/afxFrustum.h"
#include "qwadro/sim/arxEngine.h"
#include "qwadro/scene/arxBuffer.h"
#include "qwadro/sim/arxSampleContext.h"

AFX_DEFINE_STRUCT(arxSimulationConfig)
{
    afxSimBridgeConfig  prime;
    afxUnit             auxCnt;
    afxSimBridgeConfig* auxs;

    arxEngineFeatures   requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxDrawSystem       dsys;
    arxScenario        scio;

};

ASX afxModule       ArxGetSimulationIcd(arxSimulation sim);

ASX afxDrawSystem   ArxGetSimulationDrawSystem(arxSimulation sim);
ASX arxScenario    ArxGetSimulationDrawInput(arxSimulation sim);

// DRAW BRIDGES AND QUEUES.

ASX afxUnit         ArxGetSimBridges(arxSimulation sim, afxUnit baseExuIdx, afxUnit cnt, arxSimBridge bridges[]);
ASX afxUnit         ArxQuerySimBridges(arxSimulation sim, afxUnit sengId, afxUnit portId, afxUnit first, afxUnit cnt, arxSimBridge bridges[]);

ASX afxError        ArxWaitForSimBridge(arxSimulation sim, afxUnit exuIdx, afxTime timeout);
ASX afxError        ArxWaitForSimulation(arxSimulation sim, afxTime timeout);



ASX afxError        ArxStepSimulation(arxSimulation sim);


ASX afxError        ArxExecuteSampleCommands(arxSimulation sim, asxSubmission* ctrl, afxUnit cnt, arxContext contexts[]);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        ArxConfigureSimulation(afxUnit sengId, arxSimulationConfig* cfg);

ASX afxError        ArxEstablishSimulation(afxUnit sengId, arxSimulationConfig const* cfg, arxSimulation* simulation);

#endif//ASX_SIMULATION_H
