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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "qwadro/core/afxClass.h"
#include "qwadro/sim/awxEntity.h"
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/sim/afxSkeleton.h"
#include "qwadro/sim/modeling/afxModel.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/sim/anim/awxAnimus.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#include "qwadro/sound/afxSoundDefs.h"

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
    afxSoundContext         sctx;
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
AFX_OBJECT(afxSimulation)
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

    awxAnimusInterlink    globalAnimusInterlinks;

    afxDrawContext          dctx;
};
#endif//_AFX_SIMULATION_C

AFX afxMmu      AfxGetSimulationMmu(afxSimulation sim);

AFX afxClass*       AwxGetAnimusClass(afxSimulation sim);
AFX afxClass*       AfxGetBodyClass(afxSimulation sim);
AFX afxClass*       AfxGetEntityClass(afxSimulation sim);
AFX afxClass*       AfxGetLightClass(afxSimulation sim);
AFX afxClass*       AfxGetMaterialClass(afxSimulation sim);
AFX afxClass*       AwxGetMeshClass(afxSimulation sim);
AFX afxClass*       AwxGetMeshDataClass(afxSimulation sim);
AFX afxClass*       AwxGetMeshTopologyClass(afxSimulation sim);
AFX afxClass*       AwxGetModelClass(afxSimulation sim);
AFX afxClass*       AfxGetNodeClass(afxSimulation sim);
AFX afxClass*       AfxGetRendererClass(afxSimulation sim);
AFX afxClass*       AfxGetSkeletonClass(afxSimulation sim);

AFX afxNat          AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, awxBody bod[]);
AFX afxNat          AfxEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, awxEntity ent[]);
AFX afxNat          AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AFX afxNat          AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AFX afxNat          AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AFX afxNat          AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);


AFX afxError        _AfxSimulationProcess(afxSimulation sim);

AFX afxError        AfxStepSimulation(afxSimulation sim);
AFX afxError        AfxCullSimulation(afxSimulation sim, afxCamera cam, afxArray* pvs);
AFX afxError        AfxRenderSimulation(afxSimulation sim, afxArray const* pvs);

AFX void            AfxRecenterAllAnimusClocks(afxSimulation sim, afxReal dCurrentClock);

AFX void            AfxComputeAllowedErrorValues(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);

AFX void            AfxComputeBasisConversion(afxSimulation sim, afxReal unitsPerMeter, afxReal const right[3], afxReal const up[3], afxReal const back[3], afxReal const origin[3], afxReal ltm[3][3], afxReal iltm[3][3], afxReal atv[3]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireSimulations(afxNat cnt, awxSimulationConfig const config[], afxSimulation simulations[]);

#endif//AFX_SIMULATION_H
