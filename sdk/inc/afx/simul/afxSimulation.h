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
#include "afxEntity.h"
#include "afxLight.h"
#include "afxCamera.h"
#include "dag/afxSkeleton.h"
#include "modeling/afxModelBlueprint.h"
#include "modeling/afxMaterialBlueprint.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. By changing variables in the simulation, predictions may be made about the behaviour of the system. It is a tool to virtually investigate the behaviour of the system under study.

// space is a purely mathematical representation of a world within space and its interactions.

typedef enum afxSkyType
{
    AFX_SKY_BOX = 1,
    AFX_SKY_DOME,
} afxSkyType;

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
    afxClass        entObjt;
    afxClass        camObjt;
    afxClass        litObjt;

    afxClass        mtlObjt;
    afxClass        mshObjt;
    afxClass        mdlObjt;
    afxClass        sklObjt;
    afxClass        nodObjt;

    afxMemory    genrlMem;
    afxArray        roots;

    afxAabb         area;
    afxV4d          origin;

    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    afxDrawContext  dctx;
};

#endif


AFX afxMaterial     AfxSimulationBuildMaterial(afxSimulation sim, afxMaterialBlueprint *blueprint);
AFX afxMesh         AfxSimulationBuildMesh(afxSimulation sim, afxMeshBlueprint *blueprint, afxVertexBuffer vbuf, afxNat baseVtx, afxIndexBuffer ibuf, afxNat baseIdx);
AFX afxModel        AfxSimulationBuildModel(afxSimulation sim, afxModelBlueprint *blueprint);
AFX afxSkeleton     AfxSimulationBuildSkeleton(afxSimulation sim, afxNode parent, afxSkeletonBlueprint *blueprint);

AFX afxMaterial     AfxSimulationFetchMaterial(afxSimulation sim, afxUri const *uri); // art/path/to/setname.txd.urd?name=
AFX afxMesh         AfxSimulationFetchMesh(afxSimulation sim, afxUri const *uri); // art/path/to/setname.mdl.urd?name=
AFX afxModel        AfxSimulationFetchModel(afxSimulation sim, afxUri const *uri); // art/path/to/setname.mdl.urd

AFX afxMaterial     AfxSimulationFindMaterial(afxSimulation sim, afxString const *name);
AFX afxMesh         AfxSimulationFindMesh(afxSimulation sim, afxString const *name);
AFX afxModel        AfxSimulationFindModel(afxSimulation sim, afxString const *name);

AFX afxBody         AfxSimulationAcquireBody(afxSimulation sim, afxString const *name, afxNode parent, afxModel mdl);
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
AFX afxClass*       AfxSimulationGetCameraClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetEntityClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetLightClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetMaterialClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetMeshClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetModelClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetNodeClass(afxSimulation sim);
AFX afxClass*       AfxSimulationGetSkeletonClass(afxSimulation sim);

AFX afxResult       AfxSimulationForEachBody(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachCamera(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachEntity(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachLight(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachMaterial(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachMesh(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachModel(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachNode(afxSimulation sim, void (*f)(afxIterator *iter), void *data);
AFX afxResult       AfxSimulationForEachSkeleton(afxSimulation sim, void (*f)(afxIterator *iter), void *data);

AFX afxResult       AfxSimulationLoadObjAssets(afxSimulation sim, afxUri const *uri, afxArray(void) *assets);
AFX afxResult       AfxSimulationLoadMD5Assets(afxSimulation sim, afxUri const *uri, afxArray(void) *assets);

AFX afxError        _AfxSimulationProcess(afxSimulation sim);
AFX afxResult       AfxSimulationRender(afxSimulation sim, afxCamera cam, afxV4d point, afxDrawInput din);
AFX afxResult       AfxSimulationStep(afxSimulation sim);

#endif//AFX_SIMULATION_H