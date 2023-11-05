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

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "afx/core/afxClass.h"
#include "afx/sim/afxEntity.h"
#include "afx/sim/rendering/afxLight.h"
#include "afx/sim/rendering/afxCamera.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/anim/afxAnimator.h"
#include "afx/sim/rendering/afxRenderer.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. By changing variables in the simulation, predictions may be made about the behaviour of the system. It is a tool to virtually investigate the behaviour of the system under study.

// space is a purely mathematical representation of a world within space and its interactions.

AFX_DEFINE_STRUCT(afxSimulationConfig)
{
    afxUri const*   driver;
    afxAabb const*  bounding;
    afxDrawContext  dctx;
    afxDrawInput    din;

    afxNat          maxBodCnt;
    afxNat          maxEntCnt;
    afxNat          maxCamCnt;
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
    afxContext              genrlMem;
    afxChain                classes;
    afxClass                assets;
    afxClass                bodies;
    afxClass                bcontrols;
    afxClass                entities;
    afxClass                cameras;
    afxClass                lights;
    afxClass                materials;
    afxClass                meshes;
    afxClass                meshDatas;
    afxClass                topologies;
    afxClass                models;
    afxClass                skeletons;
    afxClass                nodes;
    afxClass                renderers;

    afxAabb                 space;
    afxV4d                  origin;
    afxM3d                  basis;
    afxReal                 unitsPerMeter;
    afxReal                 allowedLodErrFadingFactor;

    afxSize                 stepNum, stepsPerSecond;
    afxSize                 stepTime, swapTime;
    afxReal                 stepDeltaTime;

    afxAnimatorInterlink    globalAnimatorInterlinks;

    afxDrawContext          dctx;
};
#endif

AFX afxContext      AfxSimulationGetMemory(afxSimulation sim);

AFX afxClass*       AfxGetAnimatorClass(afxSimulation sim);
AFX afxClass*       AfxGetBodyClass(afxSimulation sim);
AFX afxClass*       AfxGetCameraClass(afxSimulation sim);
AFX afxClass*       AfxGetEntityClass(afxSimulation sim);
AFX afxClass*       AfxGetLightClass(afxSimulation sim);
AFX afxClass*       AfxGetMaterialClass(afxSimulation sim);
AFX afxClass*       AfxGetMeshClass(afxSimulation sim);
AFX afxClass*       AfxGetMeshDataClass(afxSimulation sim);
AFX afxClass*       AfxGetMeshTopologyClass(afxSimulation sim);
AFX afxClass*       AfxGetModelClass(afxSimulation sim);
AFX afxClass*       AfxGetNodeClass(afxSimulation sim);
AFX afxClass*       AfxGetRendererClass(afxSimulation sim);
AFX afxClass*       AfxGetSkeletonClass(afxSimulation sim);

AFX afxNat          AfxEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, afxBody bod[]);
AFX afxNat          AfxEnumerateCameras(afxSimulation sim, afxNat base, afxNat cnt, afxCamera cam[]);
AFX afxNat          AfxEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, afxEntity ent[]);
AFX afxNat          AfxEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AFX afxNat          AfxEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AFX afxNat          AfxEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AFX afxNat          AfxEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);




AFX afxError        _AfxSimulationProcess(afxSimulation sim);

AFX afxError        AfxStepSimulation(afxSimulation sim);
AFX afxError        AfxCullSimulation(afxSimulation sim, afxCamera cam, afxArray* pvs);
AFX afxError        AfxRenderSimulation(afxSimulation sim, afxArray const* pvs);

AFX void            AfxRecenterAllAnimatorClocks(afxSimulation sim, afxReal dCurrentClock);

AFX void            AfxFindAllowedErrorValues(afxSimulation sim, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler);


#endif//AFX_SIMULATION_H