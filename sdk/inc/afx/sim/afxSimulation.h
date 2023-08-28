/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
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
#include "afx/sim/modeling/afxSkeleton.h"
#include "afx/sim/modeling/afxSkeletonBlueprint.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/modeling/afxModelBlueprint.h"
#include "afx/sim/modeling/afxMeshBlueprint.h"
#include "afx/sim/modeling/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/anim/afxBodyControl.h"
#include "afx/sim/rendering/afxRenderer.h"

// A computer simulation (or "sim") is an attempt to model a real-life or hypothetical situation on a computer so that it can be studied to see how the system works. By changing variables in the simulation, predictions may be made about the behaviour of the system. It is a tool to virtually investigate the behaviour of the system under study.

// space is a purely mathematical representation of a world within space and its interactions.

AFX_DEFINE_STRUCT(afxArtToolInfo)
{
    afxString const*    name; // from art tool name
    afxInt              majorRevision;
    afxInt              minorRevision;
    afxReal             unitsPerMeter;
    afxV3d              origin;
    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxArtExporterInfo)
{
    afxString const*    name;
    afxInt              majorRevision;
    afxInt              minorRevision;
    afxInt              customization;
    afxInt              buildNum;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxArtAssetInfo)
{
    afxArtToolInfo*     toolInfo;
    afxArtExporterInfo* exporterInfo;
    afxUri*             uri; // from file
    afxNat              texCnt;
    afxTexture*         texs;
    afxNat              mtlCnt;
    afxMaterial*        mtls;
    afxNat              sklCnt;
    afxSkeleton*        skls;
    afxNat              vdCnt;
    //granny_vertex_data* vertexDatas;
    afxNat              mshtCnt;
    afxMeshTopology*    mshts;
    afxNat              mshCnt;
    afxMesh*            mshs;
    afxNat              mdlCnt;
    afxModel*           mdls;
    afxNat              trackGroupCnt;
    //granny_track_group* trackGroups;
    afxNat              animCnt;
    //afxAnimation*       anims;
    void*               udd;
};




AFX_DEFINE_STRUCT(afxSimulationConfig)
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

#ifdef _AFX_SIMULATION_C
AFX_OBJECT(afxSimulation)
{
    afxChain        classes;

    afxClass        bodies;
    afxClass        bcontrols;
    afxClass        entities;
    afxClass        cameras;
    afxClass        lights;

    afxClass        materials;
    afxClass        meshes;
    afxClass        models;
    afxClass        skeletons;
    afxClass        nodes;

    afxClass        renderers;

    afxContext    genrlMem;
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

AFX afxError        AfxAcquireRenderers(afxSimulation sim, afxNat cnt, afxRenderer rnd[], void *spec);

AFX afxError        AfxAcquireSkeletons(afxSimulation sim, afxNat cnt, afxSkeleton skl[], afxUri const uri[]);
AFX afxError        AfxBuildSkeletons(afxSimulation sim, afxNat cnt, afxSkeleton skl[], afxSkeletonBlueprint blueprint[]);
AFX afxResult       AfxFindSkeletons(afxSimulation sim, afxNat cnt, afxSkeleton skl[], afxUri const uri[]);
AFX afxError        AfxUploadSkeletons(afxSimulation sim, afxNat cnt, afxSkeleton skl[], afxUri const uri[]);

AFX afxError        AfxAcquireMaterials(afxSimulation sim, afxNat cnt, afxMaterial mtl[], afxUri const uri[]);
AFX afxError        AfxBuildMaterials(afxSimulation sim, afxNat cnt, afxMaterial mtl[], afxMaterialBlueprint const blueprint[]);
AFX afxResult       AfxFindMaterials(afxSimulation sim, afxNat cnt, afxMaterial mtl[], afxUri const uri[]);
AFX afxError        AfxUploadMaterials(afxSimulation sim, afxNat cnt, afxMaterial mtl[], afxUri const uri[]);

AFX afxError        AfxAcquireMeshes(afxSimulation sim, afxNat cnt, afxMesh msh[], afxUri const uri[]);
AFX afxError        AfxBuildMeshes(afxSimulation sim, afxNat cnt, afxMesh msh[], afxMeshBlueprint blueprint[]);
AFX afxResult       AfxFindMeshes(afxSimulation sim, afxNat cnt, afxMesh msh[], afxUri const uri[]);
AFX afxError        AfxUploadMeshes(afxSimulation sim, afxNat cnt, afxMesh msh[], afxUri const uri[]);

AFX afxError        AfxAcquireModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);
AFX afxError        AfxBuildModels(afxSimulation sim, afxNat cnt, afxModel mdl[], afxModelBlueprint blueprint[]);
AFX afxResult       AfxFindModels(afxSimulation sim, afxNat cnt, afxModel mdl[], afxUri const uri[]);
AFX afxError        AfxUploadModels(afxSimulation sim, afxNat cnt, afxModel mdl[], afxUri const uri[]);

AFX afxError        AfxAcquireBodies(afxSimulation sim, afxNat cnt, afxBody bod[], afxModel mdl[]);
AFX afxError        AfxAcquireBodyControls(afxSimulation sim, afxNat cnt, afxBodyControl bodc[], afxReal currClock, afxReal localDur);

AFX afxError        AfxAcquireCameras(afxSimulation sim, afxNat cnt, afxCamera cam[]);
AFX afxError        AfxAcquireEntities(afxSimulation sim, afxNat cnt, afxEntity ent[], afxBody bod[]);

AFX afxLight        AfxSimulationAcquireDirectionalLight(afxSimulation sim, afxColor color);
AFX afxLight        AfxSimulationAcquirePointLight(afxSimulation sim, afxColor color, afxReal range);
AFX afxLight        AfxSimulationAcquireSpotLight(afxSimulation sim, afxColor color, afxReal range, afxReal radians);
AFX afxLight        AfxSimulationAcquireAreaLight(afxSimulation sim, afxColor color);

AFX afxError        AfxAcquireNodes(afxSimulation sim, afxNat cnt, afxNode nod[], afxNodeSpecification const config[]);

AFX afxContext       AfxSimulationGetMemory(afxSimulation sim);

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

AFX afxNat          AfxSimulationEnumerateBodies(afxSimulation sim, afxNat base, afxNat cnt, afxBody bod[]);
AFX afxNat          AfxSimulationEnumerateCameras(afxSimulation sim, afxNat base, afxNat cnt, afxCamera cam[]);
AFX afxNat          AfxSimulationEnumerateEntities(afxSimulation sim, afxNat base, afxNat cnt, afxEntity ent[]);
AFX afxNat          AfxSimulationEnumerateMaterials(afxSimulation sim, afxNat base, afxNat cnt, afxMaterial mtl[]);
AFX afxNat          AfxSimulationEnumerateMeshes(afxSimulation sim, afxNat base, afxNat cnt, afxMesh msh[]);
AFX afxNat          AfxSimulationEnumerateModels(afxSimulation sim, afxNat base, afxNat cnt, afxModel mdl[]);
AFX afxNat          AfxSimulationEnumerateSkeletons(afxSimulation sim, afxNat base, afxNat cnt, afxSkeleton skl[]);

AFX afxError        _AfxSimulationProcess(afxSimulation sim);
AFX afxError        AfxSimulationRender(afxSimulation sim, afxCamera cam, afxV4d point, afxDrawInput din);
AFX afxError        AfxSimulationStep(afxSimulation sim);

AFX void            AfxSimulationRecenterAllBodyControlClocks(afxSimulation sim, afxReal dCurrentClock);

#endif//AFX_SIMULATION_H