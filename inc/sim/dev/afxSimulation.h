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

#include "qwadro/inc/sim/rendering/akxLighting.h"

#include "qwadro/inc/sim/afxAnimation.h"
#include "qwadro/inc/sim/afxMotor.h"
#include "qwadro/inc/sim/afxMotion.h"
#include "qwadro/inc/sim/rendering/akxRenderer.h"

#include "qwadro/inc/cad/afxSkeleton.h"
#include "qwadro/inc/math/afxCurve.h"

#include "qwadro/inc/cad/afxModel.h"
#include "qwadro/inc/cad/afxMaterial.h"

AMX afxClass const* AfxGetCurveClass(afxSimulation sim);
AMX afxClass const* AfxGetPoseClass(afxSimulation sim);
AMX afxClass const* AfxGetPoseBufferClass(afxSimulation sim);
AMX afxClass const* AfxGetSkeletonClass(afxSimulation sim);

AMX afxClass const* AfxGetMaterialClass(afxSimulation sim);
AMX afxClass const* AfxGetModelClass(afxSimulation sim);

AMX afxClass*       AkxGetMotorClass(afxSimulation sim);
AMX afxClass*       AfxGetBodyClass(afxSimulation sim);
AMX afxClass*       AfxGetLightClass(afxSimulation sim);
AMX afxClass*       AfxGetNodeClass(afxSimulation sim);
AMX afxClass*       AfxGetRendererClass(afxSimulation sim);
AMX afxClass*       AfxGetTerrainClass(afxSimulation sim);

AMX afxNat          AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, afxBody bodies[]);
AMX afxNat          AfxInvokeBodies(afxSimulation sim, afxNat first, afxNat cnt, afxBool(*f)(afxBody, void*), void *udd);

AMX afxError        AfxStepSimulation(afxSimulation sim);

AMX afxNat          AfxRecenterAllMotorClocks(afxSimulation sim, afxReal dCurrentClock);

AMX void            AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AMX void            AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv);

AMX afxStringBase   AfxGetModelUrnStringBase(afxSimulation sim);

AMX afxNat          AfxCountMathBridges(afxSimulation sim, afxNat baseBridIdx);
AMX afxNat          AfxCountMathQueues(afxSimulation sim, afxNat portIdx, afxNat baseQueIdx);

AMX afxBool         AfxGetMathBridge(afxSimulation sim, afxNat portIdx, afxComboBridge* bridge);
AMX afxBool         AfxGetMathQueue(afxSimulation sim, afxNat portIdx, afxNat queIdx, afxMathQueue* queue);

AMX afxError        AfxWaitForSimulation(afxSimulation sim, afxNat portIdx, afxNat firstQueIdx, afxNat queCnt);

AMX afxDrawContext  AfxGetSimulationDrawContext(afxSimulation sim);
AMX afxDrawInput    AfxGetSimulationDrawInput(afxSimulation sim);

#endif//AMX_SIMULATION_H
