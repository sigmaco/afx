/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_SIMULATION_H
#define AFX_SIMULATION_H

#include "afx/core/afxClass.h"
#include "afx/sim/afxEntity.h"
#include "afx/sim/afxLight.h"
#include "afx/sim/afxCamera.h"
#include "afx/sim/dag/afxSkeleton.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/modeling/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/afxBodyControl.h"
#include "afx/sim/render/afxRenderer.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. By changing variables in the simulation, predictions may be made about the behaviour of the system. It is a tool to virtually investigate the behaviour of the system under study.

// space is a purely mathematical representation of a world within space and its interactions.

AFX_DEFINE_STRUCT(afxSimulationSpecification)
{
    afxUri const            *driver;
    afxAabb const           *bounding;
    afxDrawContext          dctx;
    afxDrawInput            din;

    afxNat                  maxBodCnt;
    afxNat                  maxEntCnt;
    afxNat                  maxCamCnt;
    afxNat                  maxLitCnt;
    afxNat                  maxMtlCnt;
    afxNat                  maxMshCnt;
    afxNat                  maxMdlCnt;
    afxNat                  maxSklCnt;
    afxNat                  maxNodCnt;
};

AFX_DEFINE_HANDLE(afxSimulation);

#ifndef _AFX_API_ONLY

AFX_OBJECT(afxSimulation)
{
    afxObject       obj;
    afxChain        provisions;

    afxClass        bodObjt;
    afxClass        bodcObjt;
    afxClass        entObjt;
    afxClass        camObjt;
    afxClass        litObjt;

    afxClass        mtlObjt;
    afxClass        mshObjt;
    afxClass        mdlObjt;
    afxClass        sklObjt;
    afxClass        nodObjt;

    afxClass        rndObjt;

    afxMemory    genrlMem;
    afxArray        roots;

    afxAabb         area;
    afxV4d          origin;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    afxBodyControlInterlink  globalBodyControlInterlinks;

    afxDrawContext  dctx;
};

#endif

AFX afxError        AfxSimulationAcquireRenderers(afxSimulation sim, afxNat cnt, void *spec, afxRenderer rnd[]);

AFX afxError        AfxSimulationAcquireSkeletons(afxSimulation sim, afxNat cnt, afxUri const uri[], afxSkeleton skl[]);
AFX afxError        AfxSimulationBuildSkeletons(afxSimulation sim, afxNat cnt, afxSkeletonBlueprint blueprint[], afxSkeleton skl[]);
AFX afxResult       AfxSimulationFindSkeletons(afxSimulation sim, afxNat cnt, afxUri const uri[], afxSkeleton skl[]);
AFX afxError        AfxSimulationUploadSkeletons(afxSimulation sim, afxNat cnt, afxUri const uri[], afxSkeleton skl[]);

AFX afxError        AfxSimulationAcquireMaterials(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMaterial mtl[]);
AFX afxError        AfxSimulationBuildMaterials(afxSimulation sim, afxNat cnt, afxMaterialBlueprint const blueprint[], afxMaterial mtl[]);
AFX afxResult       AfxSimulationFindMaterials(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMaterial mtl[]);
AFX afxError        AfxSimulationUploadMaterials(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMaterial mtl[]);

AFX afxError        AfxSimulationAcquireMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[]);
AFX afxError        AfxSimulationBuildMeshes(afxSimulation sim, afxNat cnt, afxMeshBlueprint blueprint[], afxMesh msh[]);
AFX afxResult       AfxSimulationFindMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[]);
AFX afxError        AfxSimulationUploadMeshes(afxSimulation sim, afxNat cnt, afxUri const uri[], afxMesh msh[]);

AFX afxError        AfxSimulationAcquireModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);
AFX afxError        AfxSimulationBuildModels(afxSimulation sim, afxNat cnt, afxModelBlueprint blueprint[], afxModel mdl[]);
AFX afxResult       AfxSimulationFindModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);
AFX afxError        AfxSimulationUploadModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);

AFX afxError        AfxSimulationAcquireBodies(afxSimulation sim, afxNat cnt, afxModel mdl[], afxBody bod[]);

AFX afxCamera       AfxSimulationAcquireCamera(afxSimulation sim, afxString const *name, afxNode parent, afxBool perspective);
AFX afxEntity       AfxSimulationAcquireEntity(afxSimulation sim, afxString const *name, afxNode parent, afxBody bod);

AFX afxLight        AfxSimulationAcquireDirectionalLight(afxSimulation sim, afxColor color);
AFX afxLight        AfxSimulationAcquirePointLight(afxSimulation sim, afxColor color, afxReal range);
AFX afxLight        AfxSimulationAcquireSpotLight(afxSimulation sim, afxColor color, afxReal range, afxReal radians);
AFX afxLight        AfxSimulationAcquireAreaLight(afxSimulation sim, afxColor color);

AFX afxNode         AfxSimulationAcquireNode(afxSimulation sim, afxNodeSpecification const *spec);

AFX void*           AfxSimulationGetSystem(afxSimulation sim);
AFX void*           AfxSimulationGetDrawSystem(afxSimulation sim);
AFX void*           AfxSimulationGetApplication(afxSimulation sim);

AFX afxMemory       AfxSimulationGetMemory(afxSimulation sim);

AFX afxClass*       AfxSimulationGetBodyClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetBodyControlClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetCameraClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetEntityClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetLightClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetMaterialClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetMeshClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetModelClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetNodeClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetSkeletonClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetRendererClass(afxSimulation sim);

AFX afxResult       AfxSimulationEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, afxBody bod[]);
AFX afxResult       AfxSimulationEnumerateCameras(afxSimulation sim, afxNat base, afxNat cnt, afxCamera cam[]);
AFX afxResult       AfxSimulationEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, afxEntity ent[]);
AFX afxResult       AfxSimulationEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AFX afxResult       AfxSimulationEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AFX afxResult       AfxSimulationEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AFX afxResult       AfxSimulationEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);
AFX afxResult       AfxSimulationForEachNode(afxSimulation sim, void (*f)(afxIterator *iter), void *data);

AFX afxResult       AfxSimulationLoadObjAssets(afxSimulation sim, afxUri const *uri, afxArray(void) *assets);
AFX afxResult       AfxSimulationLoadMD5Assets(afxSimulation sim, afxUri const *uri, afxArray(void) *assets);

AFX afxError        _AfxSimulationProcess(afxSimulation sim);
AFX afxError        AfxSimulationRender(afxSimulation sim, afxCamera cam, afxV4d point, afxDrawInput din);
AFX afxError        AfxSimulationStep(afxSimulation sim);

AFX void            AfxSimulationRecenterAllBodyControlClocks(afxSimulation sim, afxReal dCurrentClock);

#endif//AFX_SIMULATION_H