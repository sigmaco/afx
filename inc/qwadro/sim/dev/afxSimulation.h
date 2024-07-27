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

#ifndef AKX_SIMULATION_H
#define AKX_SIMULATION_H

#include "qwadro/base/afxClass.h"
#include "qwadro/sim/rendering/akxLighting.h"
#include "qwadro/space/afxSkeleton.h"
#include "qwadro/cad/afxModel.h"
#include "qwadro/space/akxAnimation.h"
#include "qwadro/cad/afxMaterial.h"
#include "qwadro/draw/dev/afxDrawInput.h"
#include "qwadro/draw/dev/afxDrawContext.h"
#include "qwadro/space/akxMotor.h"
#include "qwadro/space/akxMotion.h"
#include "qwadro/sim/rendering/akxRenderer.h"
#include "qwadro/sound/afxSoundDefs.h"
#include "qwadro/space/afxCurve.h"

AFX_DEFINE_STRUCT(akxSimulationConfig)
{
    afxUri const*   driver;
    afxBox         extent;
    afxDrawContext  dctx;
    afxDrawInput    din;
    afxSoundContext sctx;
    afxV3d          right;
    afxV3d          up;
    afxV3d          back;
    afxV3d          origin;
    afxReal         unitsPerMeter;
};

AKX afxMmu          AfxGetSimulationMmu(afxSimulation sim);

AKX afxClass*     AfxGetCurveClass(afxSimulation sim);
AKX afxClass*     AkxGetMotorClass(afxSimulation sim);
AKX afxClass*     AfxGetBodyClass(afxSimulation sim);
AKX afxClass*     AfxGetLightClass(afxSimulation sim);
AKX afxClass*     AfxGetMaterialClass(afxSimulation sim);
AKX afxClass*     AkxGetMeshClass(afxSimulation sim);
AKX afxClass*     AkxGetMeshDataClass(afxSimulation sim);
AKX afxClass*     AkxGetMeshTopologyClass(afxSimulation sim);
AKX afxClass*     AkxGetModelClass(afxSimulation sim);
AKX afxClass*     AfxGetNodeClass(afxSimulation sim);
AKX afxClass*     AfxGetPoseClass(afxSimulation sim);
AKX afxClass*     AfxGetPoseBufferClass(afxSimulation sim);
AKX afxClass*     AfxGetRendererClass(afxSimulation sim);
AKX afxClass*     AfxGetSkeletonClass(afxSimulation sim);
AKX afxClass*       AfxGetTerrainClass(afxSimulation sim);

AKX afxNat          AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, akxBody bod[]);
AKX afxNat          AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AKX afxNat          AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AKX afxNat          AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AKX afxNat          AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);


AKX afxError        _AfxSimulationProcess(afxSimulation sim);

AKX afxError        AfxStepSimulation(afxSimulation sim);
AKX afxError        AfxCullSimulation(afxSimulation sim, avxCamera cam, afxArray* pvs);
AKX afxError        AfxRenderSimulation(afxSimulation sim, afxArray const* pvs);

AKX afxNat          AfxCountMotors(afxSimulation sim);
AKX afxNat          AfxInvokeMotors(afxSimulation sim, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AKX afxNat          AfxEnumerateMotors(afxSimulation sim, afxNat first, afxNat cnt, afxDrawInput inputs[]);
AKX afxNat          AfxRecenterAllMotorClocks(afxSimulation sim, afxReal dCurrentClock);

AKX void            AfxQuerySimulationErrorTolerance(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AKX void            AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv);

AKX afxBool         AfxRegisterModelUrns(afxSimulation sim, afxNat cnt, afxString const src[], afxString dst[]);
AKX afxBool         AfxRegisterPivotTags(afxSimulation sim, afxNat cnt, afxString const src[], afxString dst[]);
AKX afxBool         AfxRegisterMorphTags(afxSimulation sim, afxNat cnt, afxString const src[], afxString dst[]);

AKX afxStringBase   AfxGetModelUrnStringBase(afxSimulation sim);
AKX afxStringBase   AfxGetPivotTagStringBase(afxSimulation sim);
AKX afxStringBase   AfxGetMorphTagStringBase(afxSimulation sim);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxClass*     AfxGetSimulationClass(void);
AKX afxNat          AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation simulations[]);
AKX afxNat          AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AKX afxError        AfxAcquireSimulations(afxNat cnt, akxSimulationConfig const config[], afxSimulation simulations[]);

#endif//AKX_SIMULATION_H
