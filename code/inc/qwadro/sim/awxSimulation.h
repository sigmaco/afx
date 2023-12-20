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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "qwadro/core/afxClass.h"
#include "qwadro/sim/awxEntity.h"
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/sim/awxSkeleton.h"
#include "qwadro/sim/modeling/awxModel.h"
#include "qwadro/sim/awxMaterial.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/sim/anim/awxMotor.h"
#include "qwadro/sim/rendering/awxRenderer.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. 
// By changing variables in the simulation, predictions may be made about the behaviour of the system. 
// It is a tool to virtually investigate the behaviour of the system under study.

// A space is a purely mathematical representation of a world within space and its interactions.

AFX_DEFINE_STRUCT(awxSimulationConfig)
{
    afxUri const*           driver;
    afxAabb                 extent;
    afxDrawContext          dctx;
    afxDrawInput            din;
    afxV3d                  right;
    afxV3d                  up;
    afxV3d                  back;
    afxV3d                  origin;
    afxReal                 unitsPerMeter;
    afxNat          maxBodCnt;
    afxNat          maxEntCnt;
    afxNat          maxLitCnt;
    afxNat          maxMtlCnt;
    afxNat          maxMshCnt;
    afxNat          maxMdlCnt;
    afxNat          maxSklCnt;
    afxNat          maxNodCnt;
};

#ifdef _AFX_SIMULATION_C
AFX_OBJECT(awxSimulation)
{
    afxMmu              genrlMem;
    afxChain                classes;
    afxClass                assets;
    afxClass                bodies;
    afxClass                motors;
    afxClass                entities;
    afxClass                lights;
    afxClass                materials;
    afxClass                meshes;
    afxClass                meshDatas;
    afxClass                topologies;
    afxClass                models;
    afxClass                skeletons;
    afxClass                nodes;
    afxClass                renderers;

    afxV4d                  right;
    afxV4d                  up;
    afxV4d                  back;
    afxM3d                  basis;
    afxV4d                  origin;
    afxAabb                 extent;
    afxReal                 unitsPerMeter;
    afxReal                 allowedLodErrFadingFactor;

    afxSize                 stepNum, stepsPerSecond;
    afxSize                 stepTime, swapTime;
    afxReal                 stepDeltaTime;

    awxMotorInterlink    globalMotorInterlinks;

    afxDrawContext          dctx;
};
#endif//_AFX_SIMULATION_C

AFX afxMmu      AfxSimulationGetMemory(awxSimulation sim);

AFX afxClass*       AwxGetMotorClass(awxSimulation sim);
AFX afxClass*       AfxGetBodyClass(awxSimulation sim);
AFX afxClass*       AfxGetEntityClass(awxSimulation sim);
AFX afxClass*       AfxGetLightClass(awxSimulation sim);
AFX afxClass*       AfxGetMaterialClass(awxSimulation sim);
AFX afxClass*       AwxGetMeshClass(awxSimulation sim);
AFX afxClass*       AwxGetMeshDataClass(awxSimulation sim);
AFX afxClass*       AwxGetMeshTopologyClass(awxSimulation sim);
AFX afxClass*       AwxGetModelClass(awxSimulation sim);
AFX afxClass*       AfxGetNodeClass(awxSimulation sim);
AFX afxClass*       AfxGetRendererClass(awxSimulation sim);
AFX afxClass*       AfxGetSkeletonClass(awxSimulation sim);

AFX afxNat          AfxEnumerateBodies(awxSimulation sim, afxNat base, afxNat cnt, awxBody bod[]);
AFX afxNat          AfxEnumerateEntities(awxSimulation sim, afxNat base, afxNat cnt, awxEntity ent[]);
AFX afxNat          AfxEnumerateMaterials(awxSimulation sim, afxNat base, afxNat cnt, awxMaterial mtl[]);
AFX afxNat          AfxEnumerateMeshes(awxSimulation sim, afxNat base, afxNat cnt, awxMesh msh[]);
AFX afxNat          AfxEnumerateModels(awxSimulation sim, afxNat base, afxNat cnt, awxModel mdl[]);
AFX afxNat          AfxEnumerateSkeletons(awxSimulation sim, afxNat base, afxNat cnt, awxSkeleton skl[]);


AFX afxError        _AfxSimulationProcess(awxSimulation sim);

AFX afxError        AfxStepSimulation(awxSimulation sim);
AFX afxError        AfxCullSimulation(awxSimulation sim, afxCamera cam, afxArray* pvs);
AFX afxError        AfxRenderSimulation(awxSimulation sim, afxArray const* pvs);

AFX void            AfxRecenterAllMotorClocks(awxSimulation sim, afxReal dCurrentClock);

AFX void            AfxComputeAllowedErrorValues(awxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AFX void            AfxComputeBasisConversion(awxSimulation sim, afxReal unitsPerMeter, afxReal const right[3], afxReal const up[3], afxReal const back[3], afxReal const origin[3], afxReal ltm[3][3], afxReal iltm[3][3], afxReal atv[3]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireSimulations(afxNat cnt, awxSimulationConfig const config[], awxSimulation sim[]);

#endif//AFX_SIMULATION_H
